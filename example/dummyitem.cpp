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
    qDebug() << "Deleted" << m_key;
}

DummyItem *DummyItem::fromVariantMap(const QVariantMap &map)
{
    DummyItem *dm=new DummyItem();
    
    dm->m_id=map["id"].toInt();
    dm->m_key=map["key"].toString();
    
    dm->m_category=map["category"].toString().toUpper();
    dm->m_name=map["name"].toString();
    dm->m_time=map["time"].toTime();
    dm->m_datestamp=map["date"].toDate();
    dm->m_timestamp=map["datetime"].toDateTime();
    
    QVector2D v2;
    if (map.contains("vec2")) {
        QVariantMap m=map["vec2"].toMap();
        v2.setX(m["x"].toFloat());
        v2.setY(m["y"].toFloat());
        dm->m_vec2=v2;
    }
    
    QVector3D v3;
    if (map.contains("vec3")) {
        QVariantMap m=map["vec3"].toMap();
        v3.setX(m["x"].toFloat());
        v3.setY(m["y"].toFloat());
        v3.setY(m["z"].toFloat());
        dm->m_vec3=v3;
    }
    
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
