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


private:
    Ui::MainWindow *ui;

    //PLC
    PlcProxy                *plc1Proxy;
    PlcEventHandler         plc1EventHandler;    //proxy to widgets
    PlcWidgetEventHandler   widget1EventHandler; //widgets to proxy

    QTimer *plc1RestartTimer;

};

#endif // MAINWINDOW_H
