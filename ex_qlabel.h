#ifndef EX_QLABEL_H
#define EX_QLABEL_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QVector>

class Ex_QLabel : public QLabel
{
    Q_OBJECT
public:
    explicit Ex_QLabel(QWidget *parent=0);
    int index;
    static int count;
    void mousePressEvent(QMouseEvent *ev);
signals:
    void mousePresseed(int index);


};

#endif // EX_QLABEL_H
