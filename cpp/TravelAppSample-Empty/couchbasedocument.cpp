#include "couchbasedocument.h"

#include <QJsonDocument>

CouchbaseDocument::CouchbaseDocument(const lcb_get_resp_t *resp, lcb_error_t rc)
{
    mError = rc;
    if (!success())
    {
        return;
    }
    QByteArray ba = QByteArray((const char*)resp->v.v0.bytes, resp->v.v0.nbytes);
    mData = QString(ba);
    mCas = resp->v.v0.cas;
}

bool CouchbaseDocument::success() const
{
    return mError == LCB_SUCCESS;
}

QString CouchbaseDocument::asString()
{
    return mData;
}

QJsonObject CouchbaseDocument::asJson()
{
    return QJsonDocument::fromJson(mData.toUtf8()).object();
}

lcb_cas_t CouchbaseDocument::cas()
{
    return mCas;
}

lcb_error_t CouchbaseDocument::error()
{
    return mError;
}

CouchbaseDocument::CouchbaseDocument()
{

}

CouchbaseDocument CouchbaseDocument::errorValue(lcb_error_t rc)
{
    CouchbaseDocument result;
    result.mData = "";
    result.mError = rc;
    return result;
}




