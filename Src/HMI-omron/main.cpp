#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <mainwindow.h>
#include "myomron.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

//   MainWindow mainwindow ;
//    mainwindow.show();
    /*-------------*/
    QQmlApplicationEngine engine;


    MyOMRON myOMRON("192.168.5.194");
    engine.rootContext()->setContextProperty("myOMRON", &myOMRON);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
