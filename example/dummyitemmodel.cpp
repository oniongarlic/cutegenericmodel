#include "dummyitemmodel.h"

DummyItemModel::DummyItemModel(QObject *parent) :
    AbstractObjectModel(QMetaType::type("DummyItem"), parent)
{
    m_has_key=true;
    m_key_name="key";
    m_iswritable=true;
    m_pos.setLatitude(60.45);
    m_pos.setLongitude(22.00);
}

DummyItem *DummyItemModel::getItem(int index) const
{
    return dynamic_cast<DummyItem *>(getObject(index));
}

QVariant DummyItemModel::formatProperty(const QObject *data, const QMetaProperty *meta) const
{
    if (strcmp(meta->name(), "time")==0) {
        QTime t;

        t=meta->read(data).toTime();
        return QVariant(t.toString("HH:mm:ss"));
    } else if (strcmp(meta->name(), "timestamp")==0) {
        QDateTime t;

        t=meta->read(data).toDateTime();
        return QVariant(t.toString("dd.MM.yyyy @ HH:mm"));
    } else if (strcmp(meta->name(), "geo")==0) {
        QGeoCoordinate g=meta->read(data).value<QGeoCoordinate>();
        if (g.isValid()) {
            qreal dist=m_pos.distanceTo(g)/1000;
            return QVariant(dist);
        } else {
            return QVariant("");
        }
    }
    return AbstractObjectModel::formatProperty(data, meta);
}

