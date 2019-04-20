#include "mywidget.h"

mywidget::mywidget(QWidget *parent) : QWidget(parent)
{

}

void mywidget::paintEvent(QPaintEvent *event)
{

    QPainter mypainter(this);
mypainter.setPen(Qt::red);
  //  mypainter.drawLine(0,0,100,100);
    mypainter.drawRect(0,0,this->width()-1,this->height()-1);
   QRectF rectf(0,0,this->width()-1,this->height()-1);
   mypainter.scale(0.5,0.5);
   mypainter.drawArc(rectf,16*-30,-150*16);


/*QRectF rectangle(10.0, 20.0, 80.0, 60.0);
int startAngle = 30 * 16;
int spanAngle = 120 * 16;

QPainter painter(this);
painter.drawArc(rectangle, startAngle, spanAngle);
*/

}
