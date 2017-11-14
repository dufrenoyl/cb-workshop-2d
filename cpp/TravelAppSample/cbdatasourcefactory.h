#ifndef CBDATASOURCEFACTORY_H
#define CBDATASOURCEFACTORY_H
#include "cbdatasource.h"

class CBDataSourceFactory
{      
public:
  static void Create(const QString &connectionString, const QString& username, const QString& password);
  static  CBDataSource& Instance();

private:
  static CBDataSource mInstance;

};

#endif // CBDATASOURCEFACTORY_H
