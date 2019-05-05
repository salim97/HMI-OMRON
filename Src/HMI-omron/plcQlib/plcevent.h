#ifndef CPLCEVENT_H
#define CPLCEVENT_H

#include <QObject>
#include <QWidget>
#include <QSpinBox>
#include <QSlider>
#include <QProgressBar>
#include <QLabel>

#include <QTextStream>

#include "types.h"
#include "widgetplcinterface.h"


/** Plc Event action structure abstract class
  * if associaded with tWidgetPlcInterface every data is forwarded to this object
  */
class PlcEvent
{

public:
    typedef enum   {
        NOACTION,
        VISIBLE_IF_TRUE,
        VISIBLE_IF_FALSE,
        ENABLED_IF_TRUE,
        ENABLED_IF_FALSE,
        UPDATE_VALUE_QLIST,
        UPDATE_VALUE_UNSIG,
        UPDATE_VALUE_SIG,
        UPDATE_VALUE_FLOAT,
        UPDATE_VALUE_BCD,
        STYLE_CHANGE,
        UPDATE_VALUE_STRING
    } eAction;

    PlcEvent() {Clear();}

    virtual void HandleData( QList<u16> data)=0;
    // Obsolete void HandleData( u16 value);

    unsigned int GetItemLength();

    void Clear( void) {
        this->action          = NOACTION;

        this->item            = 0;

        this->itemLength      = 0;

        this->trueStyle       = QString("");
        this->falseStyle      = QString("");
    }

    PlcEvent& operator= ( const PlcEvent& o)
    {
        this->action          = o.action;

        this->item            = o.item;

        this->itemLength      = o.itemLength;

        this->trueStyle       = o.trueStyle;
        this->falseStyle      = o.falseStyle;

        return *this;
    }

    PlcEvent(const PlcEvent& o)
    {
        this->action          = o.action;

        this->item            = o.item;

        this->itemLength      = o.itemLength;

        this->trueStyle       = o.trueStyle;
        this->falseStyle      = o.falseStyle;
    }
    QString getTargetName( void) {return this->item->objectName();}

protected:

    eAction         action;    //action to-do when data come

    QObject        *item;       //pointer to target object

    //plc related
    unsigned int   itemLength;

    //color related
    QString trueStyle;
    QString falseStyle;

    void setStyle( QWidget *widget, u16 fval);
    void showWidget(QWidget *widget, u16 fval);
    void hideWidget(QWidget *widget, u16 fval);

};


class PlcEvent_QWidget : public PlcEvent
{
public:
    PlcEvent_QWidget( eAction action, QWidget *item, QString trueStyle=QString(""), QString falseStyle=QString(""));
    void HandleData( QList<u16> data);
private:
    QWidget *item;
};


class PlcEvent_QSpinBox : public PlcEvent
{
public:
    PlcEvent_QSpinBox( eAction action, QSpinBox *item, unsigned int itemLength);
    void HandleData( QList<u16> data);
private:
    QSpinBox *item;
};


class PlcEvent_QDoubleSpinBox : public PlcEvent
{
public:
    PlcEvent_QDoubleSpinBox( eAction action, QDoubleSpinBox *item, unsigned int itemLength);
    void HandleData( QList<u16> data);
private:
    QDoubleSpinBox *item;
};


class PlcEvent_QProgressBar : public PlcEvent
{
public:
    PlcEvent_QProgressBar( eAction action, QProgressBar *item, unsigned int itemLength);
    void HandleData( QList<u16> data);
private:
    QProgressBar *item;
};

class PlcEvent_QSlider : public PlcEvent
{
public:
    PlcEvent_QSlider( eAction action, QSlider *item, unsigned int itemLength);
    void HandleData( QList<u16> data);
private:
    QSlider *item;
};

class PlcEvent_QLabel : public PlcEvent
{
public:
    PlcEvent_QLabel( eAction action, QLabel *item, QTextStream *format, unsigned int itemLength);
    void HandleData( QList<u16> data);
private:
    QLabel *item;
    QTextStream *formatStream;
};


class PlcEvent_WPI : public PlcEvent
{
public:
    PlcEvent_WPI( WidgetPlcInterface *item, unsigned int itemLength);
    void HandleData( QList<u16> data);
private:
    WidgetPlcInterface *item;
};



#endif // CPLCEVENT_H
