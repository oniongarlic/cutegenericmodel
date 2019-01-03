#ifndef ABSTRACTOBJECTMODEL_H
#define ABSTRACTOBJECTMODEL_H

#include <QAbstractListModel>

namespace Cute {

class AbstractObjectModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(bool isWritable MEMBER m_iswritable NOTIFY isWritableChanged)

public:
    explicit AbstractObjectModel(int meta_id, QObject *parent = nullptr);

    // QAbstractListModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    enum SortDirection {
        SortAsc,
        SortDesc
    };
    Q_ENUM(SortDirection)

    Q_INVOKABLE QVariantMap get(int index) const;
    Q_INVOKABLE QVariantMap findKey(const QString key) const;
    Q_INVOKABLE int indexKey(const QString key) const;

    Q_INVOKABLE QObject *getObject(int index) const;
    Q_INVOKABLE QObject *getKey(const QString key) const;
    Q_INVOKABLE QObject *getId(int id);

    Q_INVOKABLE bool append(QObject *item);
    Q_INVOKABLE bool prepend(QObject *item);
    Q_INVOKABLE bool contains(QObject *item);
    Q_INVOKABLE bool containsKey(const QString key) const;
    Q_INVOKABLE bool remove(int index);
    Q_INVOKABLE void clear();

    Q_INVOKABLE int count() const;

    Q_INVOKABLE void sortByProperty(const QString property, SortDirection by=SortAsc);

    Q_INVOKABLE bool search(const QString property, const QString needle);

    Q_INVOKABLE bool refresh(int index);

    void setList(QObjectList data);

signals:
    void countChanged(int);
    void isWritableChanged();
    void itemRemoved(QObject *item);

protected:
    void resolveProperties();
    void createKeyIndex();
    bool compareProperty(QObject *v1, QObject *v2);
    void sortRefresh();
    bool searchRefresh();
    void clearFilter();
    int mapIndex(int index) const;

    virtual QVariant formatProperty(const QObject *data, const QMetaProperty *meta) const;

    int m_metaid;
    bool m_has_key;
    int m_key_property_id;
    const char *m_key_name;
    bool m_has_id;

    bool m_iswritable;

    const QMetaObject *m_meta;

    QString m_sort_property;
    SortDirection m_sort_dir;

private:
    QObjectList m_data;
    QMap<int, int>m_id_index;
    QMap<QString, int>m_index;
    QHash<int, QByteArray> m_properties;

    // Search
    QString m_needle;
    QString m_haystack;
    QList<int> m_filter_index;
};

}

#endif // ABSTRACTOBJECTMODEL_H
