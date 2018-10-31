#ifndef ABSTRACTOBJECTMODEL_H
#define ABSTRACTOBJECTMODEL_H

#include <QAbstractListModel>

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

    Q_INVOKABLE QVariantMap get(int index) const;
    Q_INVOKABLE QObject *getObject(int index) const;
    Q_INVOKABLE QObject *getKey(const QString key) const;

    Q_INVOKABLE bool append(QObject *item);
    Q_INVOKABLE bool remove(int index);
    Q_INVOKABLE void clear();

    Q_INVOKABLE bool search(const QString needle);

signals:
    void countChanged(int);

protected:
    int m_metaid;
    bool m_has_key;
    int m_key_property_id;
    const char *m_key_name;
    const QMetaObject *m_meta;

    void resolveProperties();
    void createIndex();

private:
    QObjectList m_data;
    QMap<QString, int>m_index;
    QHash<int, QByteArray> m_properties;
};

#endif // ABSTRACTOBJECTMODEL_H
