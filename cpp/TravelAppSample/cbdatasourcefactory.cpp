#include "cbdatasourcefactory.h"

CBDataSource CBDataSourceFactory::mInstance;

void CBDataSourceFactory::Create(const QString& connectionString, const QString& username, const QString& password)
{
    mInstance.Connect(connectionString, username, password);
}

CBDataSource& CBDataSourceFactory::Instance()
{
    return mInstance;
}
