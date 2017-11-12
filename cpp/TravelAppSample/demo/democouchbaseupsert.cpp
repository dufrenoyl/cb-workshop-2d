#include "democouchbaseupsert.h"
#include <qdebug.h>

void DemoCouchbaseUpsert::test()
{

    CBDataSource& ds = CBDataSourceFactory::Instance();

    assertTrue(ds.IsConnected(), "You are not connected!");

    bool isUpserted = ds.Upsert("test::hello", "{\"msg\"  : \"hello\"}");

    assertTrue(isUpserted, "Could not upsert the document!");

    CouchbaseDocument doc = ds.Get("test::hello");

    assertTrue(doc.success(), "Could not retrieve the upserted document!");

    qDebug() <<  doc.asString();

    QJsonObject json;
    json.insert("msg", QString("hello again"));

    bool isSecondUpserted = ds.Upsert("test::hello::2", json);

    assertTrue(isSecondUpserted, "Could upsert the second the document!");

    CouchbaseDocument doc2 =  ds.Get("test::hello::2");

    assertTrue(doc2.success(), "Could not retrieve the second document");

    qDebug() << doc2.asString();
}
