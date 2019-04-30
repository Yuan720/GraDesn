#pragma once
#pragma execution_character_set("utf-8")
#ifndef MYTOOLKIT_H
#define MYTOOLKIT_H
#include<math.h>
#include<qmath.h>
#include<variables.h>
class Point{



public:
    Point(){}
   double distanceFrom(Point p);
   Point(double vx, double vy);
   double x;
   double y;

};
//�Զ���Բ��
class Square{
public:
    Square(){}
    Point leftTop;
    //�߳�
    double el;
    Square(Point vtopleft, double vel);
};
class MyArc{




public:
   MyArc(){};
  MyArc(double vradius,Point vstartPoint,Point vendPoint,double vangleLength);
  Point center;
  Point startPoint;
  Point endPoint;
  double radius;
  Square outerSquare;
  double startAngle;
  double angleLength;

};

class myPath{


public:
   myPath(){};
    myPath(Point pathStart, Point pathTurn, double radius,double half_span);
    myPath(Point pathStart, Point pathTurn, double radius,double half_span,double d);
    myPath(Point pathStart, Point pathTurn, double radius,double half_span,double d,float stlarea);
    //��Բ��
    float steelArea;
    double d;
    Point center;
    Point pathStartPoint;
    Point trunPoint;
    //�е�1,���е�2;�涨�е�һΪ�ұ��е�
    Point tangentPoint1;
    Point tangentPoint2;
    //���е�
    Point midSpanPoint;
    MyArc midArc;
    double getPivotDistance();
    double getYvalue(double x);
    double getAngleByX(double x);
    double getSigma_l1(float x);
    double getSigma_l2(float x);


};
#endif // MYTOOLKIT_H
