#ifndef CDOUBLESPINBOX_H
#define CDOUBLESPINBOX_H

#include <QSpinBox>
#include <QDoubleSpinBox>

#include <QTime>

class CDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:
    CDoubleSpinBox( QWidget * parent = 0);

signals:
    void wasclicked();

protected:
    virtual void focusInEvent(QFocusEvent *event);
    virtual bool event ( QEvent * event );

private:
    //QTime lastClickedTime;

};


class CSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    CSpinBox( QWidget * parent = 0);

signals:
    void wasclicked();

protected:
    virtual void focusInEvent(QFocusEvent *event);
};

#endif // CDOUBLESPINBOX_H
