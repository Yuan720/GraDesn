#pragma once
#include "mainwindow.h"
#include <QApplication>
#include<vector>
#include<math.h>
int main(int argc, char *argv[])
{
int y;
y=5;
   QApplication a(argc, argv);
    MainWindow w;
     w.setWindowTitle("����������Ƽ���");
    w.show();
    w.pagePrepare();
     return a.exec();

}
