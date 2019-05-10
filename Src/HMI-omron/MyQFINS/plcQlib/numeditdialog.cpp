#include "numeditdialog.h"
#include "ui_numeditdialog.h"

#include <QObject>

NumEditDialog::NumEditDialog(QString label, double value, double min, double max, int decpoints, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NumEditDialog)
{
    ui->setupUi(this);

    startVal = value;

    ui->editedValue->setMaximum( max);
    ui->editedValue->setMinimum( min);
    ui->editedValue->setDecimals( decpoints);
    ui->editedValue->setValue( startVal);
    ui->label->setText( label);

    ui->valText->setVisible( false);

    if( decpoints <= 0) {
        ui->push_comma->setVisible( false);
    }

    QObject::connect( ui->push_0,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->push_1,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->push_2,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->push_3,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->push_4,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->push_5,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->push_6,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->push_7,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->push_8,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->push_9,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->push_bs,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->push_c,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->push_pm,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->push_comma,SIGNAL(pressed()), this, SLOT(clicked()));

    QObject::connect( ui->plus_1,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->plus_01,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->plus_001,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->minus_1,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->minus_01,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->minus_001,SIGNAL(pressed()), this, SLOT(clicked()));


    QObject::connect( ui->push_abort,SIGNAL(pressed()), this, SLOT(clicked()));
    QObject::connect( ui->push_ok,SIGNAL(pressed()), this, SLOT(clicked()));

    ui->push_ok->setFocus();
    newString  = "+0";
}

NumEditDialog::~NumEditDialog()
{
    delete ui; ui=NULL;
}

void NumEditDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void NumEditDialog::clicked()
{
    QObject *s;
    QString so;
    s = sender();
    so = s->objectName();

    double val = ui->editedValue->value();

    if( s == ui->push_abort) {
        finalVal = startVal;
        this->reject();
        return;
    }

    if( s == ui->push_ok) {
        finalVal = val;
        this->accept();
        return;
    }



    if( s == ui->plus_1) {
        ui->editedValue->setValue( val + 1.0);
        newString  = "+0"; ui->valText->setVisible( false);
        return;
    }

    if( s == ui->plus_01) {
        ui->editedValue->setValue( val + 0.1);
        newString  = "+0"; ui->valText->setVisible( false);
        return;
    }

    if( s == ui->plus_001) {
        ui->editedValue->setValue( val + 0.01);
        newString  = "+0"; ui->valText->setVisible( false);
        return;
    }

    if( s == ui->minus_1) {
        ui->editedValue->setValue( val - 1.0);
        newString  = "+0"; ui->valText->setVisible( false);
        return;
    }

    if( s == ui->minus_01) {
        ui->editedValue->setValue( val - 0.1);
        newString  = "+0"; ui->valText->setVisible( false);
        return;
    }

    if( s == ui->minus_001) {
        ui->editedValue->setValue( val - 0.01);
        newString  = "+0"; ui->valText->setVisible( false);
        return;
    }



    if( so == "push_0") { newString += "0";  }
    if( so == "push_1") { newString += "1";  }
    if( so == "push_2") { newString += "2";  }
    if( so == "push_3") { newString += "3";  }
    if( so == "push_4") { newString += "4";  }
    if( so == "push_5") { newString += "5";  }
    if( so == "push_6") { newString += "6";  }
    if( so == "push_7") { newString += "7";  }
    if( so == "push_8") { newString += "8";  }
    if( so == "push_9") { newString += "9";  }
    if( so == "push_c") { newString  = "+0"; ui->valText->setVisible( false); }
    if( so == "push_bs") {
        if( newString.length()>2) {
            newString.remove( newString.length()-1,1);
        }
    }
    if( so == "push_comma") { newString  += this->locale().decimalPoint();  }
    if( so == "push_pm") {
        if(newString.contains('+')) {
            newString.replace('+','-');
        } else {
            newString.replace('-','+');
        }
    }
    ui->valText->setVisible( true);
    QString p=newString;
    p.remove(1,1);  //remove zero
    ui->valText->setText( p);
    ui->editedValue->setValue( newString.toDouble());
}
