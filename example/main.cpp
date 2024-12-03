#include <QCoreApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>
#include <QMetaObject>
#include <QMetaType>
#include <QMetaProperty>

#include <QRandomGenerator>

#include "abstractobjectmodel.h"
#include "dummyitemmodel.h"
#include "dummyitem.h"

#define ITEMS_MAX (1225)

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<DummyItem>("org.tal.model", 1,0, "DummyItem");
    qmlRegisterType<DummyItemModel>("org.tal.model", 1,0, "DummyItemModel");

    qRegisterMetaType<DummyItem *>("DummyItem");
    qRegisterMetaType<DummyItemModel *>();

    DummyItemModel dim1;
    DummyItemModel dim2;

    QObjectList data;

    dim1.blockSignals(true);
    // Model owns item
    for (int id=1;id<ITEMS_MAX;id++) {
        DummyItem *dm=new DummyItem(&dim1);

        QDateTime dt=QDateTime::fromSecsSinceEpoch(QRandomGenerator::global()->generate());
        QDate d=dt.date();

        QString key=QString("KEY-%1").arg(id);
        QString name=QString("%1-%2").arg(id % 2 ? "Tuoli" : "Kaappi").arg(id);

        dm->setProperty("id", id);
        dm->setProperty("key", key);
        dm->setProperty("name", name);
        dm->setProperty("timestamp", dt);
        dm->setProperty("datestamp", d);
        
        QGeoCoordinate pos;
        pos.setLatitude(QRandomGenerator::global()->generateDouble()*90.0);
        pos.setLongitude(QRandomGenerator::global()->generateDouble()*180.0);
        pos.setAltitude(500);
        dm->setProperty("geo", QVariant::fromValue(pos));
        
        dim1.append(dm);
    }
    dim1.blockSignals(false);

    dim2.blockSignals(true);
    // External list of objects    
    for (int id=100;id<ITEMS_MAX;id++) {
        DummyItem *dm=new DummyItem(&dim2);
        QTime t=QTime::fromMSecsSinceStartOfDay(rand() % (86400*1000));
        QString key=QString("DKEY:%1").arg(id);

        dm->setProperty("id", id);
        dm->setProperty("key", key);
        QString name=QString("Hylly %1").arg(id);
        dm->setProperty("name", name);
        dm->setProperty("time", t);
        dm->setProperty("datestamp", QDateTime::fromSecsSinceEpoch(QRandomGenerator::global()->generate()));

        QGeoCoordinate pos;
        pos.setLatitude(QRandomGenerator::global()->generateDouble()*90.0);
        pos.setLongitude(QRandomGenerator::global()->generateDouble()*180.0);
        dm->setProperty("geo", QVariant::fromValue(pos));

        data.append(dm);
    }
    dim2.blockSignals(false);

    dim2.setList(data);

    for (int i=0;i<dim2.count();i++) {
        qDebug() << dim2[i];
    }

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("diModel", &dim1);
    engine.rootContext()->setContextProperty("diModelL", &dim2);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
