#pragma once
#include <QList>
#include <QJsonObject>

struct CBN1qlResult
{
    QList<QJsonObject> items;

    static CBN1qlResult* fromPointer(const void *cookie)
    {
        return const_cast<CBN1qlResult*>(
                    reinterpret_cast<const CBN1qlResult*>(cookie));
    }
};
