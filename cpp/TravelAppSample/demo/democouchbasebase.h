#ifndef DEMOCOUCHBASEBASE_H
#define DEMOCOUCHBASEBASE_H

#include <libcouchbase/couchbase.h>
#include "cbdatasource.h"
#include "cbdatasourcefactory.h"
#include <qdebug.h>

class DemoCouchbaseBase
{
public:
    void virtual test() = 0;

    static void assertTrue(bool cond, char* msg) {

        if (!cond) {

            qDebug() << msg;
            exit(1);
        }
    }

};

#endif // DEMOCOUCHBASEBASE_H
