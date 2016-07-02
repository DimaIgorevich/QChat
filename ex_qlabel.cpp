#include "ex_qlabel.h"
#include <QMouseEvent>
#include <QPoint>

Ex_QLabel::Ex_QLabel( QWidget *parent): QLabel(parent)
{
    count++;
    index = count;
    setVisible(true);

}

void Ex_QLabel::mousePressEvent(QMouseEvent *ev)
{
    QPoint pos = ev->pos();
    this->setStyleSheet("QLabel { font-size:12px; background-color : #8B5F65; color : white; }");
    emit mousePresseed(index);
}




