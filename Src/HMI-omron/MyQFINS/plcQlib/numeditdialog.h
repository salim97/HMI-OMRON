#ifndef NUMEDITDIALOG_H
#define NUMEDITDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QEvent>

namespace Ui {
    class NumEditDialog;
}

class NumEditDialog : public QDialog {
    Q_OBJECT
public:
    NumEditDialog(QString label, double value, double min, double max, int decpoints=0, QWidget *parent = 0);
    ~NumEditDialog();
    double getValue() { return finalVal;};

protected:
    void changeEvent(QEvent *e);

private:
    Ui::NumEditDialog *ui;
    QString newString;
    double startVal;
    double finalVal;

private slots:
    void clicked( void);
};

#endif // NUMEDITDIALOG_H
