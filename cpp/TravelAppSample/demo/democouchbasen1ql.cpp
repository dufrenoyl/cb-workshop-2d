
#include "democouchbasen1ql.h"

void DemoCouchbaseN1ql::test()
{
    CBDataSource& ds = CBDataSourceFactory::Instance();

    assertTrue(ds.IsConnected(), "You are not connected!");

    QString queryPrep = "SELECT airportname FROM `travel-sample` WHERE faa = 'LHR'";

    CBN1qlResult result = ds.QueryN1ql(queryPrep);

    assertTrue(!result.items.isEmpty(), "The result is empty!");

    for (QList<QJsonObject>::Iterator it = result.items.begin();it != result.items.end(); ++it)
    {
        qDebug() << "result = " << (*it)["airportname"].toString();
    }
}
