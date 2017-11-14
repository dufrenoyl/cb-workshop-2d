#pragma once

#include "cbqueryresult.h"
#include "cbn1qlresult.h"
#include "couchbasedocument.h"

#include <QString>
#include <QJsonObject>

struct lcb_st;
typedef struct lcb_st *lcb_t;

class CBDataSource
{
public:
    CBDataSource();
    void Destroy();

    void Connect(const QString& connectionString, const QString& username, const QString& password);
    bool IsConnected();

    CouchbaseDocument Get(const QString& key);
    bool Upsert(const QString& key, const QString& document);
    bool Upsert(const QString& key, const QJsonObject& document);
    bool Delete(const QString& key);
    bool IncrCounter(const QString& name, int delta, int initial = 0);
    CouchbaseDocumentMap MultiGet(const QStringList& keys);
    CBQueryResult QueryView(const QString& designDocName, const QString& viewName,
                            int limit=0, int skip=0);
    CBN1qlResult QueryN1ql(const QString& query);

private:
    lcb_t mInstance;
    bool mIsConnected;
};
