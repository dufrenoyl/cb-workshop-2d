#include "democouchbasedelete.h"
#include <qdebug.h>

void DemoCouchbaseDelete::test()
{
   CBDataSource& ds = CBDataSourceFactory::Instance();

   assertTrue(ds.IsConnected(),"You are not connected!");

   QJsonObject json;
   json.insert("msg", QString("to delete"));
   ds.Upsert("test::delete", json);
   qDebug() << "Created document test::delete";

   bool deleted = ds.Delete("test::delete");

   assertTrue(deleted, "Could not delete the document!");

   qDebug() << "Successfully deleted the document.";
}
