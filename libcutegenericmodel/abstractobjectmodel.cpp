#include "abstractobjectmodel.h"

#include <QDebug>
#include <QMetaProperty>

using namespace Cute;

AbstractObjectModel::AbstractObjectModel(int meta_id, QObject *parent)
    : QAbstractListModel(parent),
      m_metaid(meta_id),      
      m_has_key(false),
      m_key_name(nullptr),
      m_has_id(false),
      m_meta(nullptr)
{    
    resolveProperties();
}

void AbstractObjectModel::resolveProperties()
{
    Q_ASSERT(m_metaid>0);

    m_meta=QMetaType::metaObjectForType(m_metaid);
    Q_ASSERT(m_meta);

    for (int i=0;i<m_meta->propertyCount();i++) {
        QMetaProperty p=m_meta->property(i);

        qDebug() << p.name() << p.isReadable() << p.isWritable() << p.typeName() << p.type() << p.isEnumType() << p.isStored();

        m_properties.insert(Qt::UserRole+i, p.name());
        if (QString(p.name())=="id" && p.type()==QVariant::Int) {
            m_has_id=true;
        }
    }
}

void AbstractObjectModel::createIndex()
{
    if (!m_has_key || !m_key_name)
        return;

    m_index.clear();

    for (int i=0;i<m_data.size();i++) {
        QObject *o=m_data.at(i);
        QString key=o->property(m_key_name).toString();

        m_index.insert(key, i);
    }
}

int AbstractObjectModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_data.size();
}

QVariant AbstractObjectModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(m_metaid>0);
    Q_ASSERT(m_meta);

    if (!index.isValid())
        return QVariant();

    int pid=role-Qt::UserRole;

    QMetaProperty p=m_meta->property(pid);

    if (!p.isValid()) {
        qWarning() << "Invalid role for property requested" << role << pid;
        return QVariant();
    }

    QObject *o=m_data.at(index.row());

    if (!o)
        return QVariant();

    return QVariant(p.read(o));
}

QObject *AbstractObjectModel::getKey(const QString key) const
{
    if (m_has_key==false) {
        qWarning("Key indexing not enabled for model");
        return nullptr;
    }

    return getObject(m_index.value(key, -1));
}

QObject *AbstractObjectModel::getId(int id)
{
    if (m_has_id==false) {
        qWarning("ID not enabled for model");
        return nullptr;
    }

    for (int i = 0; i < m_data.size(); ++i) {
        QObject *o=m_data.at(i);
        int oid=o->property("id").toInt();
        if (oid==id)
            return o;
    }
    return nullptr;
}

QHash<int, QByteArray> AbstractObjectModel::roleNames() const
{
    return m_properties;
}

QVariantMap AbstractObjectModel::get(int index) const
{
    QVariantMap vm;

    if (index>m_data.size() || index<0)
        return vm;

    const QObject *o=m_data.at(index);
    const QMetaObject *m=QMetaType::metaObjectForType(m_metaid);

    for (int i=0;i<m->propertyCount();i++) {
        QMetaProperty p=m->property(i);
        if (!p.isReadable())
            continue;

        vm.insert(p.name(), p.read(o));
    }

    return vm;
}

QObject *AbstractObjectModel::getObject(int index) const
{
    if (index>m_data.size() || index<0)
        return nullptr;

    return m_data.at(index);
}

bool AbstractObjectModel::append(QObject *item)
{
    if (m_data.contains(item)) {
        qWarning("Duplicates are not supported in model");
        return false;
    }

    int p=m_data.size();
    beginInsertRows(QModelIndex(), p, p);    
    m_data.append(item);
    if (m_has_key && m_key_name) {
        QString key=item->property(m_key_name).toString();
        m_index.insert(key, m_data.size());
    }
    endInsertRows();

    emit countChanged(m_data.size());

    return true;
}

bool AbstractObjectModel::remove(int index)
{
    if (index>m_data.size() || index<0)
        return false;

    beginRemoveRows(QModelIndex(), index, index);
    QObject *o=m_data.at(index);
    if (o->parent()==this)
        o->deleteLater();
    m_data.removeAt(index);    
    endRemoveRows();

    createIndex();

    emit countChanged(m_data.size());

    return true;
}

void AbstractObjectModel::clear()
{
    beginResetModel();
    for (int i=0;i<m_data.size();i++) {
        QObject *o=m_data.at(i);
        if (o->parent()==this)
            o->deleteLater();
    }
    m_data.clear();
    m_index.clear();
    endResetModel();

    emit countChanged(m_data.size());
}

bool AbstractObjectModel::search(const QString needle)
{
    return false;
}
