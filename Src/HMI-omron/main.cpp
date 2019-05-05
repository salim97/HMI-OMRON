#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <mainwindow.h>

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    MainWindow mainwindow ;
    mainwindow.show();
    return app.exec();
    /*-------------*/
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
