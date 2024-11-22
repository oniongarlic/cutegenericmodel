#ifndef DUMMYITEM_H
#define DUMMYITEM_H

#include <QObject>
#include <QMap>
#include <QDateTime>
#include <QGeoCoordinate>

class DummyItem : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("Dummy", "Test")
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(int itemID READ id NOTIFY idChanged)
    Q_PROPERTY(QString key MEMBER m_key)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)    
    Q_PROPERTY(QString category READ category NOTIFY categoryChanged)
    Q_PROPERTY(QDateTime timestamp MEMBER m_timestamp NOTIFY timestampChanged)
    Q_PROPERTY(QDate datestamp MEMBER m_datestamp NOTIFY datestampChanged)
    Q_PROPERTY(QTime time MEMBER m_time NOTIFY timeChanged)
    Q_PROPERTY(bool enabled MEMBER m_enabled NOTIFY enabledChanged)
    Q_PROPERTY(QGeoCoordinate geo MEMBER m_geo NOTIFY geoChanged)

public:
    explicit DummyItem(QObject *parent = nullptr);
    ~DummyItem();

    static DummyItem *fromVariantMap(const QVariantMap &map);

    QString category() const
    {
        return m_category;
    }

    Q_INVOKABLE void setCategory(QString category);

private:
    int m_id;

    QString m_key;
    QString m_name;
    QString m_category;
    QDateTime m_timestamp;
    QDate m_datestamp;
    QTime m_time;
    bool m_enabled;

    QGeoCoordinate m_geo;

    int id() const;
    QString name() const;    

signals:

    void idChanged(int id);
    void nameChanged(QString name);
    void categoryChanged(QString category);
    void timestampChanged();
    void datestampChanged();
    void timeChanged();
    void enabledChanged();
    void geoChanged();

public slots:
    void setId(int id);
    void setName(QString name);

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event);
};

#endif // DUMMYITEM_H

