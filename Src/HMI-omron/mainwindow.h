#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "plcqlib.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void startPlc1Comm();
    void initPlc1WidgetsStructure();
private slots:
    void plc1ProxyError(PlcProxy::EplcProxyError error);
    void plc1Restart();
private:
    Ui::MainWindow *ui;

    //PLC
    PlcProxy                *plc1Proxy;
    PlcEventHandler         plc1EventHandler;    //proxy to widgets
    PlcWidgetEventHandler   widget1EventHandler; //widgets to proxy

    QTimer *plc1RestartTimer;

};

#endif // MAINWINDOW_H
