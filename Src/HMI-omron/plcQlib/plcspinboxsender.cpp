#include "plcspinboxsender.h"
#include "numeditdialog.h"

WidgetPlcSender::WidgetPlcSender( QObject *parent)
    :QObject( parent)
{

}

SpinboxPlcSender::SpinboxPlcSender(CSpinBox *spinbox, QString label, QObject *parent, bool clearValue)
    :    WidgetPlcSender(parent)
{
    qDebug() << QString("SpinboxPlcSender::SpinboxPlcSender $%1,%2")
            .arg(spinbox->objectName()).arg(label);
    this->label = label;
    this->clearValue = clearValue;
    cspinbox = spinbox;
    cdoublespinbox = NULL;
    QObject::connect( spinbox, SIGNAL(wasclicked()), this, SLOT(showEditor()));
}

SpinboxPlcSender::SpinboxPlcSender(CDoubleSpinBox *spinbox, QString label, QObject *parent, bool clearValue)
    :    WidgetPlcSender(parent)
{
    qDebug() << QString("SpinboxPlcSender::SpinboxPlcSender $%1,%2")
            .arg(spinbox->objectName()).arg(label);
    this->label = label;
    this->clearValue = clearValue;
    cspinbox = NULL;
    cdoublespinbox = spinbox;
    QObject::connect( spinbox, SIGNAL(wasclicked()), this, SLOT(showEditor()));
}

SpinboxPlcSender::SpinboxPlcSender( QDoubleSpinBox *spinbox, QObject *parent)
            :    WidgetPlcSender(parent)
{
     cspinbox = NULL;
     cdoublespinbox = NULL;
     QObject::connect( spinbox, SIGNAL(valueChanged(double)), this, SLOT(spinboxValChangeF(double)));
}

SpinboxPlcSender::SpinboxPlcSender( QSpinBox *spinbox, QObject *parent)
            :    WidgetPlcSender(parent)
{
     cspinbox = NULL;
     cdoublespinbox = NULL;
     QObject::connect( spinbox, SIGNAL(valueChanged(int)), this, SLOT(spinboxValChangeI(int)));
}

void SpinboxPlcSender::spinboxValChangeI(int val)
{
   emit valueChanged_int( val);
}

void SpinboxPlcSender::spinboxValChangeF(double val)
{
   emit valueChanged_double( val);
}

//==============================================

SliderPlcSender::SliderPlcSender(QSlider *slider, QObject *parent)
    :    WidgetPlcSender(parent)
{
    this->slider = slider;
    QObject::connect( slider, SIGNAL( valueChanged(int)), this, SLOT( sliderValueChanged(int)));
}

void SliderPlcSender::sliderValueChanged(int newValue)
{
     emit valueChanged_int( newValue);
}

//==============================================

void SpinboxPlcSender::showEditor()
{
    qDebug() << QString("SpinboxPlcSender::showEditor() from %1").arg(sender()->objectName());
    //qDebug() << QString( ) this->connect;
    if( cspinbox) {
        NumEditDialog *ed;
        ed = new NumEditDialog(
                label,
                clearValue?0:cspinbox->value(),
                cspinbox->minimum(),
                cspinbox->maximum(),
                0,
                NULL);
        ed->exec();
        cspinbox->setValue( ed->getValue());
        qDebug() << "emmit result from SpinboxPlcSender::showEditor - int";
        emit valueChanged_double( ed->getValue());   //emit both signals, decision is made by listener
        emit valueChanged_int(    ed->getValue());
        delete ed; ed = NULL;
        return;
    }
    if( cdoublespinbox) {
        NumEditDialog *ed;
        ed = new NumEditDialog(
                label,
                clearValue?0.0:cdoublespinbox->value(),
                cdoublespinbox->minimum(),
                cdoublespinbox->maximum(),
                cdoublespinbox->decimals(),
                NULL);
        if( ed->exec() == QDialog::Accepted) {
            qDebug() << "emmit result from SpinboxPlcSender::showEditor - double";
            emit valueChanged_double( ed->getValue());   //emit both signals, decision is made by listener
            emit valueChanged_int(    ed->getValue());
        } else {
            qDebug() << "cancel edit " << cdoublespinbox->objectName();
        }
        delete ed; ed = NULL;
        return;
    }
}
