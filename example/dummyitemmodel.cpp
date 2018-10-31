#include "dummyitemmodel.h"

DummyItemModel::DummyItemModel(QObject *parent) :
    AbstractObjectModel(QMetaType::type("DummyItem"), parent)
{
    m_has_key=true;
    m_key_name="name";
}

DummyItem *DummyItemModel::getItem(int index) const
{
    return dynamic_cast<DummyItem *>(getObject(index));
}

