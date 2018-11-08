#include "dummyitemmodel.h"

DummyItemModel::DummyItemModel(QObject *parent) :
    AbstractObjectModel(QMetaType::type("DummyItem"), parent)
{
    m_has_key=true;
    m_key_name="name";
    m_iswritable=true;
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
        return QVariant(t.toString("HH:mm"));
    }
    return AbstractObjectModel::formatProperty(data, meta);
}

