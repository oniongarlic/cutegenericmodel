#ifndef ABSTRACTOBJECTMODEL_H
#define ABSTRACTOBJECTMODEL_H

#include <QAbstractListModel>

namespace Cute {

class AbstractObjectModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    explicit AbstractObjectModel(int meta_id, QObject *parent = nullptr);

    // QAbstractListModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    enum SortDirection {
        SortAsc,
        SortDesc
    };
    Q_ENUM(SortDirection)

    Q_INVOKABLE QVariantMap get(int index) const;
    Q_INVOKABLE QObject *getObject(int index) const;
    Q_INVOKABLE QObject *getKey(const QString key) const;
    Q_INVOKABLE QObject *getId(int id);

    Q_INVOKABLE bool append(QObject *item);
    Q_INVOKABLE bool prepend(QObject *item);
    Q_INVOKABLE bool contains(QObject *item);
    Q_INVOKABLE bool containsKey(const QString key) const;
    Q_INVOKABLE bool remove(int index);
    Q_INVOKABLE void clear();

    Q_INVOKABLE void sortByProperty(const QString property, SortDirection by=SortAsc);

    Q_INVOKABLE bool search(const QString needle);

    void setList(QObjectList data);




signals:
    void countChanged(int);

    void itemRemoved(QObject *item);

protected:
    int m_metaid;
    bool m_has_key;
    int m_key_property_id;
    const char *m_key_name;
    bool m_has_id;

    const QMetaObject *m_meta;

    QString m_sort_property;

    void resolveProperties();
    void createIndex();

    bool compareProperty(QObject *v1, QObject *v2);
private:
    QObjectList m_data;
    QMap<QString, int>m_index;
    QHash<int, QByteArray> m_properties;
};

}

#endif // ABSTRACTOBJECTMODEL_H
