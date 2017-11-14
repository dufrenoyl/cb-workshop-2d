#include "democouchbaseconnect.h"
#include <qdebug.h>


void DemoCouchbaseConnect::test()
{
   CBDataSourceFactory::Create("couchbase://localhost/travel-sample", "application", QString());
   CBDataSource& ds = CBDataSourceFactory::Instance();

   bool isConnected = ds.IsConnected();

   assertTrue(isConnected, "You are not connected");

}
