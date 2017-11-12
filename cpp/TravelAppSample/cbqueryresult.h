#pragma once
#include <QStringList>

struct CBQueryResultEntry
{
    QString key;
    QString value;
};

struct CBQueryResult
{
    QList<CBQueryResultEntry> items;
    int limit;
    int skip;
    int total;

    static CBQueryResult* fromPointer(const void *cookie)
    {
        return const_cast<CBQueryResult*>(
                    reinterpret_cast<const CBQueryResult*>(cookie));
    }
};
