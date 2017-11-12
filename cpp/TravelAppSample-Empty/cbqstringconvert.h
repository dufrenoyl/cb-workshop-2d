#pragma once

#include <libcouchbase/couchbase.h>

#include <QJsonDocument>
#include <QJsonObject>

class CBQStringConvert
{
public:
    CBQStringConvert(const void * data, lcb_SIZE size)
    {
        mData = QByteArray((const char*)data, size);
    }

    CBQStringConvert(const QString& val)
    {
        mData = val.toUtf8();
    }

    size_t length()
    {
        return strlen(mData.data());
    }

    operator const char *() const
    {
        return mData.data();
    }

    operator QString() const
    {
        return QString(mData);
    }

    operator QJsonObject() const
    {
        return QJsonDocument::fromJson(mData).object();
    }

private:
    QByteArray mData;
};
