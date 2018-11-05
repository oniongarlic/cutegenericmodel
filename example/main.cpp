#include <QCoreApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>
#include <QMetaObject>
#include <QMetaType>
#include <QMetaProperty>

#include "abstractobjectmodel.h"
#include "dummyitemmodel.h"
#include "dummyitem.h"

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

    // Model owns item
    for (int id=1;id<100;id++) {
        DummyItem *dm=new DummyItem(&dim1);

        dm->setProperty("id", id);
        dm->setProperty("name", id % 2 ? "Tuoli-" : "Kaappi-" );
        dim1.append(dm);
    }

    // External list of objects
    for (int id=100;id<200;id++) {
        DummyItem *dm=new DummyItem();

        dm->setProperty("id", id);
        dm->setProperty("name", "Hylly");
        data.append(dm);
    }

    dim2.setList(data);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("diModel", &dim1);
    engine.rootContext()->setContextProperty("diModelL", &dim2);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
