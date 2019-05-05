#ifndef PLCSPINBOXSENDER_H
#define PLCSPINBOXSENDER_H

#include <QObject>
#include <QSlider>
#include "cdoublespinbox.h"
#include "plcproxy.h"

class WidgetPlcSender : public QObject
{
    Q_OBJECT
public:
    WidgetPlcSender( QObject *parent);

signals:
    void valueChanged_double( double value);
    void valueChanged_int( long int value);
    void valueChanged_str( QString value);
private:
};

class SpinboxPlcSender : public WidgetPlcSender
{
Q_OBJECT
public:
    explicit SpinboxPlcSender( CSpinBox       *spinbox, QString label, QObject *parent = 0, bool clearValue=false);
    explicit SpinboxPlcSender( CDoubleSpinBox *spinbox, QString label, QObject *parent = 0, bool clearValue=false);
    explicit SpinboxPlcSender( QDoubleSpinBox *spinbox, QObject *parent = 0);
    explicit SpinboxPlcSender( QSpinBox *spinbox, QObject *parent = 0);

public slots:
    void showEditor();

private slots:
    void spinboxValChangeF( double); //used for normal spinbox without editor
    void spinboxValChangeI( int); //used for normal spinbox without editor

private:
    CSpinBox        *cspinbox;
    CDoubleSpinBox  *cdoublespinbox;
    QString          label;
    bool             clearValue;
};

class SliderPlcSender : public WidgetPlcSender
{
Q_OBJECT
public:
    explicit SliderPlcSender( QSlider *slider, QObject *parent = 0);

public slots:
    void sliderValueChanged(int);

private:
    QSlider         *slider;
};

#endif // PLCSPINBOXSENDER_H
