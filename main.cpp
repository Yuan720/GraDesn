#pragma once
#include "mainwindow.h"
#include <QApplication>
#include<vector>
#include<math.h>

//using namespace std;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
     w.setWindowTitle("箱型梁桥设计计算");
    w.show();
    w.pagePrepare();

    return a.exec();
   /* float Pi=3.141592653;
    myPath my(Point(-600,600),Point(0,0),500,35000);

    qDebug()<<my.tangentPoint1.x<<"----"<<my.tangentPoint1.y<<endl;
    qDebug()<<my.tangentPoint2.x<<"----"<<my.tangentPoint2.y<<endl;
    qDebug()<<my.getYvalue(-292.8932)<<QString("----测试");
*/


}
