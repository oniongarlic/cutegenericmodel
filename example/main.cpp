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

    DummyItemModel dim;

    for (int id=1;id<100;id++) {
        DummyItem *dm=new DummyItem(&dim);

        dm->setProperty("id", id);
        dm->setProperty("name", "Tuoli: ");        
        dim.append(dm);
    }

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("diModel", &dim);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
