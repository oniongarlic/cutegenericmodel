#include "dummyitem.h"

#include <QDebug>

DummyItem::DummyItem(QObject *parent) :
    QObject(parent),
    m_category("notSetableFromProperties")
{

}

DummyItem::~DummyItem()
{

}

DummyItem *DummyItem::fromVariantMap(const QVariantMap &map)
{
    DummyItem *dm=new DummyItem();

    dm->m_category=map["category"].toString();
    dm->m_name=map["name"].toString();
    dm->m_key=map["key"].toString();
    dm->m_id=map["id"].toInt();

    return dm;
}

int DummyItem::id() const
{
    return m_id;
}

QString DummyItem::name() const
{
    return m_name;
}

void DummyItem::setId(int id)
{
    if (m_id == id)
        return;

    m_id = id;
    emit idChanged(m_id);
}

void DummyItem::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}
