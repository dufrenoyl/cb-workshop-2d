#include "cbdatasource.h"

#include "cbcookieget.h"
#include "cbcookieremove.h"
#include "cbqstringconvert.h"

#include <libcouchbase/couchbase.h>
#include <libcouchbase/views.h>
#include <libcouchbase/n1ql.h>
#include <stdio.h>

#include <QDebug>
#include <QStringList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QException>

static void
storage_callback(lcb_t instance, const void *cookie,
                 lcb_storage_t op, lcb_error_t err,
                 const lcb_store_resp_t *resp)
{
    if (err != LCB_SUCCESS)
    {
        qDebug() << QString("Couldn't store item to cluster: %1")
                    .arg(QString::fromUtf8(lcb_strerror(instance, err)));
    }
}

static void
get_callback(lcb_t instance, const void *cookie, lcb_error_t err,
             const lcb_get_resp_t *resp)
{
    CBCookieGet* cbCookieGet = CBCookieGet::fromPointer(cookie);
    QString key = CBQStringConvert(resp->v.v0.key, resp->v.v0.nkey);
    cbCookieGet->items.insert(key, CouchbaseDocument(resp, err));
}

static void
removed_callback(lcb_t instance, const void *cookie, lcb_error_t err, const lcb_remove_resp_t *resp)
{
    CBCookieRemove* cbCookieRemove = CBCookieRemove::fromPointer(cookie);
    if (err != LCB_SUCCESS)
    {
        qDebug() << QString("Failed to remove item: %1").arg(QString::fromUtf8(lcb_strerror(instance, err)));
        cbCookieRemove->success = false;
    }
    else
    {
        cbCookieRemove->success = true;
    }
}

CBDataSource::CBDataSource()
    : mIsConnected(false)
{
}

void CBDataSource::Connect(const QString& connectionString, const QString& username, const QString& password)
{
    if (mIsConnected)
    {
        return;
    }

    CBQStringConvert connStrConv(connectionString);
    CBQStringConvert passwordConv(password);
    CBQStringConvert usernameConv(username);

    struct lcb_create_st cropts;
    memset(&cropts, 0, sizeof cropts);
    cropts.version = 3;
    cropts.v.v3.connstr = connStrConv;
    cropts.v.v3.username = usernameConv;
    if (!password.isEmpty())
    {
        cropts.v.v3.passwd = passwordConv;
    }

    lcb_error_t err;
    err = lcb_create(&mInstance, &cropts);

    if (err != LCB_SUCCESS)
    {
        qDebug() << QString("Couldn't create instance: %1").arg(QString::fromUtf8(lcb_strerror(mInstance, err)));
        exit(1); // fatal
    }
    else
    {
        qDebug() << "Successfully created Couchbase instance.";
    }

    err = lcb_connect(mInstance);
    if (err != LCB_SUCCESS)
    {
        qDebug() << QString("Connect failed: %1").arg(QString::fromUtf8(lcb_strerror(mInstance, err)));
        exit(1); // fatal
    }
    else
    {
        qDebug() << "Connected with Bucket=" << connectionString << "; username=" << username << "; password=" << password;
    }

    lcb_wait(mInstance);
    if ((err = lcb_get_bootstrap_status(mInstance)) != LCB_SUCCESS)
    {
        qDebug() << QString("Couldn't bootstrap: %1").arg(QString::fromUtf8(lcb_strerror(mInstance, err)));
        exit(1);
    }
    else
    {
        qDebug() << "Sucessfully bootstrapped!";
    }

    lcb_set_store_callback(mInstance, storage_callback);
    lcb_set_get_callback(mInstance, get_callback);
    lcb_set_remove_callback(mInstance, removed_callback);

    mIsConnected = true;
}

bool CBDataSource::IsConnected()
{
    return mInstance;
}

void CBDataSource::Destroy()
{
    if (mIsConnected && mInstance != NULL)
    {
        lcb_destroy(mInstance);
        mInstance = NULL;
    }
}

bool CBDataSource::Upsert(const QString& key, const QString& document)
{
    CBQStringConvert keyConv(key);
    CBQStringConvert docConv(document);

    lcb_error_t err;
    lcb_store_cmd_t scmd;
    memset(&scmd, 0, sizeof scmd);
    scmd.version = 0;
    const lcb_store_cmd_t *scmdlist = &scmd;

    scmd.v.v0.key = keyConv;
    scmd.v.v0.nkey = keyConv.length();

    //Optionally use the CAS value
    //scmd.v.v0.cas
    scmd.v.v0.bytes = docConv;
    scmd.v.v0.nbytes = docConv.length();
    scmd.v.v0.operation = LCB_SET;

    err = lcb_store(mInstance, NULL, 1, &scmdlist);
    if (err != LCB_SUCCESS)
    {
        qDebug() << QString("Couldn't schedule storage operation: %1").arg(QString::fromUtf8(lcb_strerror(mInstance, err)));
        return false;
    }
    else
    {
        lcb_wait(mInstance);
        return true;
    }
}

bool CBDataSource::Upsert(const QString& key, const QJsonObject& document)
{
    QJsonDocument doc(document);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    return Upsert(key, strJson);
}

bool CBDataSource::Delete(const QString& key)
{
    CBCookieRemove cookie;
    CBQStringConvert keyConv(key);

    lcb_error_t err;

    lcb_remove_cmd_t cmd;
    memset(&cmd, 0, sizeof cmd);
    const lcb_remove_cmd_t *cmdlist = &cmd;
    cmd.v.v0.key = keyConv;
    cmd.v.v0.nkey = keyConv.length();
    err = lcb_remove(mInstance, &cookie, 1, &cmdlist);

    if (err != LCB_SUCCESS)
    {
        qDebug() << QString("Couldn't schedule remove operation: %1").arg(QString::fromUtf8(lcb_strerror(mInstance, err)));
        return false;
    }
    else
    {
        lcb_wait(mInstance);
        return cookie.success;
    }
}

bool CBDataSource::IncrCounter(const QString& name, int delta, int initial)
{
    CBQStringConvert nameConv(name);

    lcb_arithmetic_cmd_t cmd;
    memset(&cmd, 0, sizeof cmd);
    const lcb_arithmetic_cmd_t *cmdlist = &cmd;
    cmd.v.v0.key = nameConv;
    cmd.v.v0.nkey = nameConv.length();
    cmd.v.v0.delta = delta; // Increment by
    cmd.v.v0.initial = initial; // Set to this if it does not exist
    cmd.v.v0.create = 1; // Create item if it does not exist
    lcb_error_t err = lcb_arithmetic(mInstance, NULL, 1, &cmdlist);
    return (err == LCB_SUCCESS);
}

CouchbaseDocument CBDataSource::Get(const QString& key)
{
    CBCookieGet cookie;
    CBQStringConvert keyConv(key);

    lcb_error_t err;
    lcb_get_cmd_t gcmd;
    memset(&gcmd, 0, sizeof gcmd);
    const lcb_get_cmd_t *gcmdlist = &gcmd;
    gcmd.v.v0.key = keyConv;
    gcmd.v.v0.nkey = keyConv.length();
    err = lcb_get(mInstance, &cookie, 1, &gcmdlist);
    if (err != LCB_SUCCESS)
    {
        qDebug() << QString("Couldn't schedule get operation: %1")
                    .arg(QString::fromUtf8(lcb_strerror(mInstance, err)));
        return CouchbaseDocument::errorValue(err);
    }
    lcb_wait(mInstance);
    return cookie.items.first();
}

CouchbaseDocumentMap CBDataSource::MultiGet(const QStringList& keys)
{
    CBCookieGet cookie;
    for (QStringList::const_iterator it = keys.begin(); it != keys.end(); ++it)
    {
        CBQStringConvert keyConv(*it);

        lcb_get_cmd_t cmd;
        memset(&cmd, 0, sizeof cmd);
        const lcb_get_cmd_t *cmdlist = &cmd;
        cmd.v.v0.key = keyConv;
        cmd.v.v0.nkey = keyConv.length();
        lcb_get(mInstance, &cookie, 1, &cmdlist);
    }
    lcb_wait(mInstance);
    return cookie.items;
}

static void viewCallback(lcb_t instance, int ign, const lcb_RESPVIEWQUERY *rv)
{
    CBQueryResult* cookie = CBQueryResult::fromPointer(rv->cookie);

    if (rv->rflags & LCB_RESP_F_FINAL)
    {
        CBQStringConvert meta(rv->value, rv->nvalue);
        QString metaQs = meta;

        qDebug() << "*** META FROM VIEWS ***";
        qDebug() << metaQs;

        QJsonObject json = meta;
        cookie->total = json["total_rows"].toInt();
        return;
    }

    CBQueryResultEntry entry;
    QString docId = CBQStringConvert(rv->docid, rv->ndocid);
    QString value = CBQStringConvert(rv->value, rv->nvalue);

    entry.key = docId;
    entry.value = value;
    cookie->items.append(entry);
}

CBQueryResult CBDataSource::QueryView(const QString& designDocName, const QString& viewName,
                                      int limit, int skip)
{
    CBQueryResult cookie;

    lcb_CMDVIEWQUERY vq;
    memset(&vq, 0, sizeof vq);
    CBQStringConvert opts(QString("limit=%1&skip=%2&stale=false")
                                .arg(QString::number(limit), QString::number(skip)));

    vq.optstr = opts;
    vq.noptstr = opts.length();

    cookie.limit = limit;
    cookie.skip = skip;

    CBQStringConvert designDoc(designDocName);
    CBQStringConvert view(viewName);

    lcb_view_query_initcmd(&vq, designDoc, view, NULL, viewCallback);
    lcb_error_t rc = lcb_view_query(mInstance, &cookie, &vq);

    if (rc != LCB_SUCCESS)
    {
        qDebug() << QString("Could not execute view query: %1")
                    .arg(QString::fromUtf8(lcb_strerror(mInstance, rc)));
        return CBQueryResult();
    }
    else
    {
        lcb_wait(mInstance);
        return cookie;
    }
}

static void n1qlCallback(lcb_t instance, int cbtype, const lcb_RESPN1QL *resp)
{
    CBN1qlResult* cbN1qlResult = CBN1qlResult::fromPointer(resp->cookie);

    if (!(resp->rflags & LCB_RESP_F_FINAL))
    {
        CBQStringConvert row(resp->row, resp->nrow);
        cbN1qlResult->items.append(row);
    }
}

CBN1qlResult CBDataSource::QueryN1ql(const QString& query)
{
    CBN1qlResult cookie;
    CBQStringConvert queryConv(query);

    lcb_CMDN1QL cmd;
    memset(&cmd, 0, sizeof cmd);
    lcb_N1QLPARAMS *nparams = lcb_n1p_new();
    lcb_n1p_setstmtz(nparams, queryConv);

    lcb_n1p_mkcmd(nparams, &cmd);

    cmd.callback = n1qlCallback;
    lcb_error_t rc = lcb_n1ql_query(mInstance, &cookie, &cmd);
    if (rc != LCB_SUCCESS)
    {
        qDebug() << QString("N1ql Query could not be executed: %1")
                    .arg(QString::fromUtf8(lcb_strerror(mInstance, rc)));
        return CBN1qlResult();
    }
    lcb_n1p_free(nparams);
    lcb_wait(mInstance);
    return cookie;
}

