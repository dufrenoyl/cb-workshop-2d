#pragma once

#include <QString>
#include <QMap>

#include "couchbasedocument.h"

struct CBCookieGet
{
    QMap<QString, CouchbaseDocument> items;

    static CBCookieGet* fromPointer(const void *cookie)
    {
        return const_cast<CBCookieGet*>(
                    reinterpret_cast<const CBCookieGet*>(cookie));
    }
};
