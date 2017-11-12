#pragma once

#include <QString>
#include <QJsonObject>
#include <libcouchbase/couchbase.h>

class CouchbaseDocument
{
public:
    CouchbaseDocument(const lcb_get_resp_t *resp, lcb_error_t rc);
    bool success() const;
    QString asString();
    QJsonObject asJson();
    lcb_cas_t cas();
    lcb_error_t error();

    static CouchbaseDocument errorValue(lcb_error_t rc);

private:
    CouchbaseDocument();

private:
    QString mData;
    lcb_cas_t mCas;
    lcb_error_t mError;
};

typedef QMap<QString, CouchbaseDocument> CouchbaseDocumentMap;


