#pragma once
#include "mainwindow.h"
#include <QApplication>
#include<vector>
#include<math.h>
#include<QDebug>
#include<beam.h>

int main(int argc, char *argv[])
{

  QApplication a(argc, argv);
    MainWindow w;
     w.setWindowTitle("箱型梁桥设计计算");
    w.show();
    w.pagePrepare();
    return a.exec();






}
