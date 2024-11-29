#ifndef DUMMYITEMMODEL_H
#define DUMMYITEMMODEL_H

#include <QObject>
#include <QMetaProperty>
#include <QJsonValue>
#include <QGeoCoordinate>

#include "abstractobjectmodel.h"
#include "dummyitem.h"

class DummyItemModel : public Cute::AbstractObjectModel
{
    Q_OBJECT
public:
    explicit DummyItemModel(QObject *parent = nullptr);

    Q_INVOKABLE DummyItem *getItem(int index) const;

    // AbstractObjectModel interface
protected:
    QVariant formatProperty(const QObject *data, const QMetaProperty *meta) const;
    QObject *fromVariantMap(const QVariantMap &map);
    bool formatToJson(const QString &key, const QVariant &value, QJsonValue &jvalue) const;
    
    QGeoCoordinate m_pos;
};

#endif // DUMMYITEMMODEL_H
