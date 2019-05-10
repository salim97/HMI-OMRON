#include "mainwindow.h"
#include "ui_mainwindow.h"


#define PLC_IP_1 "192.168.5.194"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


