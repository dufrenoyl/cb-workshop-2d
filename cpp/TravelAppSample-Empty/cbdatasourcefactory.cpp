#include "cbdatasourcefactory.h"

CBDataSource CBDataSourceFactory::mInstance;

void CBDataSourceFactory::Create(const QString& connectionString, const QString& password)
{
    //TODO: Excercise 7b - Connect
}

CBDataSource& CBDataSourceFactory::Instance()
{
   return mInstance;
}
