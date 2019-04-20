#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include<QDebug>
#include<QPainter>

class mywidget : public QWidget
{
    Q_OBJECT
public:
    explicit mywidget(QWidget *parent = nullptr);

signals:

public slots:
 private:
   void paintEvent(QPaintEvent *event);
};

#endif // MYWIDGET_H
