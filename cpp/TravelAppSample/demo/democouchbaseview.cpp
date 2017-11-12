#include "democouchbaseview.h"

void DemoCouchbaseView::test()
{
    CBDataSource& ds = CBDataSourceFactory::Instance();

    CBQueryResult  result = ds.QueryView("airports", "by_name", 10);

    assertTrue(!result.items.isEmpty(), "The result is empty!");

    for (QList<CBQueryResultEntry>::Iterator it = result.items.begin();
         it != result.items.end(); ++it)
    {
        //(*it).key
        qDebug() << "key = " + it->key;
        qDebug() <<  "value = " + it->value;
    }

}
