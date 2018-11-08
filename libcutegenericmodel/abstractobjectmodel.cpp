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

void AbstractObjectModel::createKeyIndex()
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

    const QMetaProperty p=m_meta->property(pid);

    if (!p.isValid()) {
        qWarning() << "Invalid role for property requested" << role << pid;
        return QVariant();
    }

    const QObject *o=m_data.at(index.row());

    if (!o)
        return QVariant();

    return formatProperty(o, &p);
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

bool AbstractObjectModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool r=false;

    qDebug() << index << value << role;

    if (!m_iswritable)
        return r;

    if (!index.isValid())
        return r;

    QObject *o=m_data.at(index.row());
    const QMetaObject *m=QMetaType::metaObjectForType(m_metaid);

    QMetaProperty p=m->property(role-Qt::UserRole);
    if (p.isWritable()) {
        r=p.write(o, value);
        emit dataChanged(index, index);
    }

    return r;
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

bool AbstractObjectModel::prepend(QObject *item)
{
    if (m_data.contains(item)) {
        qWarning("Duplicates are not supported in model");
        return false;
    }

    beginInsertRows(QModelIndex(), 0, 0);
    m_data.prepend(item);
    if (m_has_key && m_key_name) {
        QString key=item->property(m_key_name).toString();
        m_index.insert(key, m_data.size());
    }
    endInsertRows();

    emit countChanged(m_data.size());

    return true;
}

bool AbstractObjectModel::contains(QObject *item)
{
    return m_data.contains(item);
}

bool AbstractObjectModel::containsKey(const QString key) const
{
    return m_index.contains(key);
}

bool AbstractObjectModel::remove(int index)
{
    if (index>m_data.size() || index<0)
        return false;

    beginRemoveRows(QModelIndex(), index, index);
    QObject *o=m_data.at(index);
    if (o->parent()==this)
        o->deleteLater();
    else
        emit itemRemoved(o);
    m_data.removeAt(index);    
    endRemoveRows();

    createKeyIndex();

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
        else
            emit itemRemoved(o);
    }
    m_data.clear();
    m_index.clear();
    endResetModel();

    emit countChanged(m_data.size());
}

bool AbstractObjectModel::compareProperty(QObject *v1, QObject *v2)
 {
    return v1->property(m_sort_property.toLocal8Bit().constData()) < v2->property(m_sort_property.toLocal8Bit().constData());
}

QVariant AbstractObjectModel::formatProperty(const QObject *data, const QMetaProperty *meta) const
{
    return meta->read(data);
}

void AbstractObjectModel::sortByProperty(const QString property, SortDirection by)
{
    m_sort_property=property;
    beginResetModel();
    if (by==SortAsc)
        std::sort(m_data.begin(), m_data.end(), [this](QObject *v1, QObject *v2){ return compareProperty(v1,v2); });
    else
        std::sort(m_data.begin(), m_data.end(), [this](QObject *v1, QObject *v2){ return compareProperty(v2,v1); });
    endResetModel();
}

bool AbstractObjectModel::search(const QString needle)
{
    return false;
}

bool AbstractObjectModel::refresh(int index)
{
    QModelIndex i=createIndex(index, 0);
    emit dataChanged(i, i);
}

void AbstractObjectModel::setList(QObjectList data)
{
    if (!m_data.empty())
        clear();
    beginResetModel();
    m_data=data;
    createKeyIndex();
    endResetModel();
    emit countChanged(m_data.size());

}
