#include "cdoublespinbox.h"

#include <QMouseEvent>
#include <QEvent>
#include <QDebug>

CDoubleSpinBox::CDoubleSpinBox( QWidget * parent)
    :QDoubleSpinBox( parent)
{
    //this->clicked();
    setFocusPolicy(Qt::ClickFocus);
    //lastClickedTime.start();
}

//No propagation to dDouble spin box
void CDoubleSpinBox::focusInEvent(QFocusEvent *event)
{
    //qDebug() << "====== " << event->type();
    clearFocus();
    //if( lastClickedTime.elapsed() > 3000) {
    //    qDebug() << "UNMASKED ====== " << event->type() << lastClickedTime.elapsed();
    //    lastClickedTime.start();
        emit wasclicked();
    //} else {
    //    qDebug() << "MASKED ====== " << event->type();
    //}
}

bool CDoubleSpinBox::event( QEvent * event )
{
  bool ep = QDoubleSpinBox::event( event);
  if( ep) {
  }
  return ep;
}

CSpinBox::CSpinBox( QWidget * parent)
    :QSpinBox( parent)
{
    //this->clicked();
    setFocusPolicy(Qt::ClickFocus);
}

//No propagation to dDouble spin box
void CSpinBox::focusInEvent(QFocusEvent *event)
{
    //qDebug() << "====== " << event->type();
    clearFocus();
    emit wasclicked();
}

