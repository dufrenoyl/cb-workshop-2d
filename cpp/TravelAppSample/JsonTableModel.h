#pragma once

#include <QAbstractTableModel>
#include <QJsonObject>

class JsonTablemodel : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit JsonTablemodel(QObject *parent = 0);

	void setData(QList<QJsonObject>);
    void addMapping(QString key, QString header);

    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QJsonObject jsonData(const QModelIndex &index);
    QList<QJsonObject> jsonData();

private:
	typedef QAbstractTableModel BaseClass_t;
	QList<QJsonObject> mData;
	QList<QString> mMapping;
    QList<QString> mHeader;
};
