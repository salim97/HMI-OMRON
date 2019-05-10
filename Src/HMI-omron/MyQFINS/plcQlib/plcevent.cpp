#include "plcevent.h"
#include <QDebug>

#include <QString>


PlcEvent_QWidget::PlcEvent_QWidget(eAction action, QWidget *item, QString trueStyle, QString falseStyle)
{
    this->Clear();
    this->action    = action;
    this->item      = item;

    this->trueStyle = trueStyle;
    this->falseStyle = falseStyle;
}

PlcEvent_WPI::PlcEvent_WPI(WidgetPlcInterface *item, unsigned int itemLength)
{
    this->Clear();
    this->action       = NOACTION; //default all data is forwarded to tWidgetPlcInterface
    this->item         = item;
    this->itemLength   = itemLength;
}

PlcEvent_QLabel::PlcEvent_QLabel( eAction action, QLabel *item, QTextStream *format, unsigned int itemLength)
{
    this->Clear();
    this->action    = action;
    this->item      = item;

    this->itemLength   = itemLength;
    this->formatStream = format;

}


PlcEvent_QSpinBox::PlcEvent_QSpinBox(eAction action, QSpinBox *item, unsigned int itemLength)
{
    this->Clear();
    this->action        = action;
    this->item          = item;
    this->itemLength    = itemLength;
}

PlcEvent_QSlider::PlcEvent_QSlider(eAction action, QSlider *item, unsigned int itemLength)
{
    this->Clear();
    this->action        = action;
    this->item          = item;
    this->itemLength    = itemLength;
}

PlcEvent_QDoubleSpinBox::PlcEvent_QDoubleSpinBox(eAction action, QDoubleSpinBox *item, unsigned int itemLength)
{
    this->Clear();
    this->action        = action;
    this->item          = item;
    this->itemLength    = itemLength;
}


PlcEvent_QProgressBar::PlcEvent_QProgressBar(eAction action, QProgressBar *item, unsigned int itemLength)
{
    this->Clear();
    this->action        = action;
    this->item          = item;
    this->itemLength    = itemLength;
}

/*
void PlcEvent::HandleData( u16 value)
{
    QList<u16>  fakeData;
    fakeData.append(value);
    HandleData( fakeData);

} Obsolete */

unsigned int PlcEvent::GetItemLength() {
    return itemLength;
}

double omronData2double( QList<u16> data)
{
    double  newValueD=0;
    union fct {
        u16 v_u16[10];
        float  v_float;
        double v_double;
    } fcu;
    switch (data.length()) {
    case 2: {
            fcu.v_u16[0] = data[0];
            fcu.v_u16[1] = data[1];
            newValueD = fcu.v_float;
        };
        break;
    case 4: {
           fcu.v_u16[0] = data[0];
           fcu.v_u16[1] = data[1];
           fcu.v_u16[3] = data[3];
           fcu.v_u16[4] = data[4];
           newValueD = fcu.v_double;
        };
        break;
    default:
        qWarning() << "bad impud data for float conversion [" << data << "]";
    }
    return newValueD;
}

unsigned int omronData2uint( QList<u16> data)
{
    unsigned int uval=0;
    for( int i=0; i<data.size(); i++) {
        uval += data[i] * (1<<(16*i));
    }
    return uval;
}

int omronData2int( QList<u16> data)
{
    int uval=0;
    for( int i=0; i<data.size(); i++) {
        uval += data[i] * (1<<(16*i));
    }
    if( data.last()&0x8000) {
        uval = (int)(-((long)0x10000L -(long)uval));
    }
    //    qDebug() << uval << -(0xFFFF-uval) << (data.last()&0x8000);
    //qDebug() << "sadasd" << uval;
    return uval;
}

unsigned int omronBcdData2uint( QList<u16> data)
{
    unsigned int retval=0;
    bool valid = true;
    for( int i=0;i<data.size();i++) {
        for( int j=0; j<4; j++) {
            unsigned char n;

            n = ((data[i])>>(12-4*j)) & 0x000f;

            retval *= 10;
            retval += n;

            //validate input
            if( n > 9) valid=false; //bad bcd!
        }
    }

    if( !valid) {
        qWarning() << "Bad input data for BCD conversion";
        return 0;
    }

    return retval;
}

void PlcEvent::showWidget(QWidget *widget, u16 fval)
{
    if( fval) {
        widget->show();
    } else {
        widget->hide();
    }
}

void PlcEvent::hideWidget(QWidget *widget, u16 fval)
{
    if( fval) {
        widget->hide();
    } else {
        widget->show();
    }
}

void PlcEvent::setStyle( QWidget *widget, u16 fval)
{
    QString newStyle;
    if( fval) {
        newStyle = trueStyle;
    } else {
        newStyle = falseStyle;
    }
    widget->setStyleSheet( newStyle);

}

void PlcEvent_QWidget::HandleData(QList<u16>data)
{
    u16 fval = data[0]; //first value

    //qDebug() << "HandleData " << data << " (" << fval << ") @" << item->objectName();
    switch (action) {
        case VISIBLE_IF_TRUE:   showWidget( item, fval); break;
        case VISIBLE_IF_FALSE:  hideWidget( item, fval); break;
        case STYLE_CHANGE:      setStyle( item, fval); break;
        case ENABLED_IF_TRUE:   item->setEnabled( fval); break;
        case ENABLED_IF_FALSE:   item->setEnabled( !fval); break;

        default:
            qWarning() << "Unimplemented action for PlcEvent_QWidget " << action;
    }
}

void PlcEvent_QSpinBox::HandleData(QList<u16>data)
{
    u16 fval = data[0]; //first value

    switch (action) {
        case VISIBLE_IF_TRUE:   showWidget( item, fval); break;
        case VISIBLE_IF_FALSE:  hideWidget( item, fval); break;
        case STYLE_CHANGE:      setStyle( item, fval); break;

        case UPDATE_VALUE_UNSIG:
            if( item->isReadOnly() || !item->hasFocus() ) {
                item->setValue( omronData2uint(data));
            }
            break;

        case UPDATE_VALUE_SIG:
            if( item->isReadOnly() || !item->hasFocus() ) {
                item->setValue( omronData2int(data));
            }
            break;

        case UPDATE_VALUE_FLOAT:
            if( item->isReadOnly() || !item->hasFocus() ) {
                item->setValue( omronData2double( data));
            }
            break;

        case UPDATE_VALUE_BCD:
            if( item->isReadOnly() || !item->hasFocus() ) {
                item->setValue( omronBcdData2uint( data));
            }
            break;

        default:
            qWarning() << "Unimplemented action for PlcEvent_QSpinBox " << action;
    }
}

void PlcEvent_QDoubleSpinBox::HandleData(QList<u16>data)
{
    //qDebug() << "HandleData [" << data << "] " << data.size() << " " << item->objectName() << " " << action;
    u16 fval = data[0]; //first value

    switch (action) {
        case VISIBLE_IF_TRUE:   showWidget( item, fval); break;
        case VISIBLE_IF_FALSE:  hideWidget( item, fval); break;
        case STYLE_CHANGE:      setStyle( item, fval); break;

        case UPDATE_VALUE_UNSIG:
            if( item->isReadOnly() || !item->hasFocus() ) {
                item->setValue( omronData2uint(data));
            }
            break;

        case UPDATE_VALUE_FLOAT:
            if( item->isReadOnly() || !item->hasFocus() ) {
                item->setValue( omronData2double( data));
            }
            break;

        case UPDATE_VALUE_BCD:
            if( item->isReadOnly() || !item->hasFocus() ) {
                item->setValue( omronBcdData2uint( data));
            }
            break;

        default:
            qWarning() << "Unimplemented action for PlcEvent_QSpinBox " << action;
    }
}

void PlcEvent_QProgressBar::HandleData(QList<u16>data)
{
    u16 fval = data[0]; //first value

    switch (action) {
        case VISIBLE_IF_TRUE:   showWidget( item, fval); break;
        case VISIBLE_IF_FALSE:  hideWidget( item, fval); break;
        case STYLE_CHANGE:      setStyle( item, fval); break;

        case UPDATE_VALUE_UNSIG:
            item->setValue( omronData2uint(data));
            break;

        case UPDATE_VALUE_FLOAT:
            item->setValue( omronData2double(data));
            break;

        case UPDATE_VALUE_BCD:
            item->setValue( omronBcdData2uint( data));
            break;

        default:
            qWarning() << "Unimplemented action for PlcEvent_QProgressBar " << action;
    }
}

void PlcEvent_QSlider::HandleData(QList<u16>data)
{
    u16 fval = data[0]; //first value

    switch (action) {
        case VISIBLE_IF_TRUE:   showWidget( item, fval); break;
        case VISIBLE_IF_FALSE:  hideWidget( item, fval); break;
        case STYLE_CHANGE:      setStyle( item, fval); break;

        case UPDATE_VALUE_UNSIG:
            item->setValue( omronData2uint(data));
            break;

        case UPDATE_VALUE_FLOAT:
            item->setValue( omronData2double(data));
            break;

        case UPDATE_VALUE_BCD:
            item->setValue( omronBcdData2uint( data));
            break;

        default:
            qWarning() << "Unimplemented action for PlcEvent_QProgressBar " << action;
    }
}

void PlcEvent_QLabel::HandleData(QList<u16>data)
{
    u16 fval = data[0]; //first value

    int vali,valb;
    double valf;
    QString s;
    int i;
    char c;

    switch (action) {
        case VISIBLE_IF_TRUE:   showWidget( item, fval); break;
        case VISIBLE_IF_FALSE:  hideWidget( item, fval); break;
        case STYLE_CHANGE:      setStyle( item, fval); break;

        case UPDATE_VALUE_UNSIG:
            vali = omronData2uint(data);
            *formatStream << vali;
            item->setText( formatStream->readAll());
            break;

        case UPDATE_VALUE_FLOAT:
            valf =  omronData2double(data);
            *formatStream << valf;
            item->setText( formatStream->readAll());
            break;

        case UPDATE_VALUE_BCD:
            valb = omronBcdData2uint(data);
            *formatStream << valb;
            item->setText( formatStream->readAll());
            break;
        case UPDATE_VALUE_STRING:
            for(i=0; (i<itemLength) && (i<data.size()); i++) {
                c =  data[i]>>8;
                if( c == 0) break;
                s.append( c);

                c =  data[i] & 0xff;
                if( c == 0) break;
                s.append( c);
            }
            item->setText( s);
            break;
        default:
            qWarning() << "Unimplemented action for PlcEvent_QLabel " << action;
    }
}

void PlcEvent_WPI::HandleData(QList<u16>data)
{
    //forward all data to PlcWI
    //no aditional actions
    item->updatePlcValue( data);
}
