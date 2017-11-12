#include "cbdatasourcefactory.h"

CBDataSource CBDataSourceFactory::mInstance;

void CBDataSourceFactory::Create(const QString& connectionString, const QString& password)
{
    mInstance.Connect(connectionString, password);
}

CBDataSource& CBDataSourceFactory::Instance()
{
    return mInstance;
}
