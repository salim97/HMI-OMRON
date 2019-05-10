#include "plcwidgetsender.h"

/** This Object is owned by its sender widget, destroyed with widget.
  */
PlcWidgetSender::PlcWidgetSender(QString addr, QAbstractButton * button, PlcProxy *plcProxy) :
    QObject(button)
{
    this->button    = button;
    this->plcProxy  = plcProxy;
    this->addr      = addr;
    this->inverted  = false;
    QObject::connect( this, SIGNAL(PlcWriteData(QString,u16)),plcProxy,SLOT(writeData(QString,u16)));
}

//----- Settupers

PlcWidgetSender* PlcWidgetSender::ZeroIfClicked()
{
    QObject::connect( button, SIGNAL(clicked()), this, SLOT(SendZero()));
    return this;
}

PlcWidgetSender* PlcWidgetSender::OneIfClicked()
{
    QObject::connect( button, SIGNAL(clicked()), this, SLOT(SendOne()));
    return this;
}

PlcWidgetSender* PlcWidgetSender::ValueIfTogled()
{
    QObject::connect( button, SIGNAL(toggled(bool)), this, SLOT(SendChecked(bool)));
    return this;
}

PlcWidgetSender* PlcWidgetSender::PulseOneIfClicked()
{
    QObject::connect( button, SIGNAL(clicked()), this, SLOT(SendTruePulse()));
    SendZero(); //init
    return this;
}

PlcWidgetSender* PlcWidgetSender::OneWhenPressed()
{
    QObject::connect( button, SIGNAL(pressed()), this, SLOT(SendOne()));
    QObject::connect( button, SIGNAL(released()), this, SLOT(SendZero()));
    SendZero(); //init
    return this;
}

PlcWidgetSender* PlcWidgetSender::IOPressButton(bool inverted)
{
    this->inverted = inverted;
    //data from widget too PLC
    QObject::connect( button, SIGNAL(toggled(bool)), this, SLOT(SendChecked(bool)));

    //from PLC to this object widget
    plcProxy->itemRegister( addr,0);
    QObject::connect( plcProxy, SIGNAL( readed(QString,QList<u16>)),this,SLOT( dataFromPlc(QString,QList<u16>)));

    //and signal bask to widget ( init values ....)
    QObject::connect( this, SIGNAL(sendValue(bool)), button,SLOT(setChecked(bool)));
    return this;
}

PlcWidgetSender* PlcWidgetSender::PressToInvert()
{
    //data from widget too PLC
    QObject::connect( button, SIGNAL(pressed()), this, SLOT(SendInvert()));
    return this;
}

//Plcd to wdget path
void PlcWidgetSender::dataFromPlc(QString addr, QList<u16>data)
{
    if( addr != this->addr) return;
    if( data[0]) {
        emit sendValue( !inverted);
    } else {
        emit sendValue(  inverted);
    }
}

//----- Senders

void PlcWidgetSender::SendOne()
{
    emit PlcWriteData( addr, (u16)0x01);
}

void PlcWidgetSender::SendZero()
{
    emit PlcWriteData( addr, (u16)0x00);
}

void PlcWidgetSender::SendChecked( bool checked)
{
    //bool checked = button->isChecked();

    if( checked ^ inverted) {
        emit PlcWriteData( addr, (u16)0x01);
    } else {
        emit PlcWriteData( addr, (u16)0x00);
    }
}

void PlcWidgetSender::SendInvert( )
{
    //bool checked = button->isChecked();

    if( plcProxy->readProxyData( addr)) {
        emit PlcWriteData( addr, (u16)0x00);
    } else {
        emit PlcWriteData( addr, (u16)0x01);
    }
}

void PlcWidgetSender::SendTruePulse()
{
        emit PlcWriteData( addr, (u16)0x01);
        emit PlcWriteData( addr, (u16)0x00);
}
