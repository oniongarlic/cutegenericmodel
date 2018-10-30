#ifndef DUMMYITEM_H
#define DUMMYITEM_H

#include <QObject>
#include <QMap>

class DummyItem : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("Dummy", "Test")
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(int itemID READ id NOTIFY idChanged)
    Q_PROPERTY(QString key MEMBER m_key)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString category READ category NOTIFY categoryChanged)

public:
    explicit DummyItem(QObject *parent = nullptr);
    ~DummyItem();

    static DummyItem *fromVariantMap(const QVariantMap &map);

    QString category() const
    {
        return m_category;
    }

private:
    int m_id;

    QString m_key;
    QString m_name;

    int id() const;

    QString name() const;

    QString m_category;

signals:

    void idChanged(int id);
    void nameChanged(QString name);
    void categoryChanged(QString category);

public slots:
    void setId(int id);
    void setName(QString name);
};

#endif // DUMMYITEM_H
