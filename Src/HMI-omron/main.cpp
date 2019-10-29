#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "myomron.h"
#include <QDebug>
#include <QtCore>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    //! [style]
    QQuickStyle::setStyle(QStringLiteral("qrc:/qml/Style"));
    //! [style]

    //! [icons]
    QIcon::setThemeName(QStringLiteral("wearable"));

    //! [icons]
    /*-------------*/
    QQmlApplicationEngine engine;

//    QSettings settings("myapp.ini", QSettings::IniFormat );
//    qDebug() << "QSettings settings(myapp.ini, QSettings::IniFormat );" << settings.fileName();

    MyOMRON myOMRON("10.0.0.1");

    engine.rootContext()->setContextProperty("myOMRON", &myOMRON);




    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    int returnval = app.exec();


    return returnval;
}
