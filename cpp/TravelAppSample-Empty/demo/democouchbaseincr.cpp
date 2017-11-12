#include "democouchbaseincr.h"


void DemoCouchbaseIncr::test()
{

    CBDataSource& ds = CBDataSourceFactory::Instance();

    assertTrue(ds.IsConnected(), "You are not connected!");

    bool isIncr = ds.IncrCounter("test::counter",1,0);

    assertTrue(isIncr, "Could not increment the counter!");

    CouchbaseDocument countDoc = ds.Get("test::counter");

    assertTrue(countDoc.success(), "Could retrieve counter value!");

    QString countInit = countDoc.asString();

    qDebug() << "Initialized Counter: " + countInit;


    for (int var = 0; var < 10; ++var) {

        ds.IncrCounter("test::counter", 1);

    }

    QString final = ds.Get("test::counter").asString();
    qDebug() << "Final Counter Value is: " + final;

}
