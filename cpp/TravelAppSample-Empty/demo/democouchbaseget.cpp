#include "democouchbaseget.h"
#include <qdebug.h>

void DemoCouchbaseGet::test()
{
    CBDataSource& ds = CBDataSourceFactory::Instance();

    assertTrue(ds.IsConnected(), "You are not connected!");

    CouchbaseDocument routeDoc = ds.Get("route_52351");

    assertTrue(routeDoc.success(), "Could not retrieve the route document!");

    QString content = routeDoc.asString();

    qDebug() << "Got the following content for document with key route_52351:" ;
    qDebug() << content;

    CouchbaseDocument airlineDoc = ds.Get("airline_10");

    assertTrue(airlineDoc.success(), "Could not retrieve the airline document!");

    QJsonObject obj =  airlineDoc.asJson();
    QJsonValue val = obj.value("name");

    qDebug() << "Got the following name value for document with key airline_10:";
    qDebug() << val.toString();

}
