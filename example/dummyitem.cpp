#include "dummyitem.h"

#include <QDebug>

DummyItem::DummyItem(QObject *parent) :
    QObject(parent),
    m_category("NSFW")
{
    startTimer(rand() % 1000 + 1000);

    m_time=QTime::currentTime();
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

void DummyItem::setCategory(QString category) {
    // Do some custom extra stuff for example
    m_category=category.trimmed().toUpper();

    emit categoryChanged(m_category);
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

void DummyItem::timerEvent(QTimerEvent *event)
{
    m_time=m_time.addSecs(1);
    // qDebug() << m_time;
    emit timeChanged();
}
