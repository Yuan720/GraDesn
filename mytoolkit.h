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
//自定义圆弧
class Square{
public:
    Square(){}
    Point leftTop;
    //边长
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
    myPath(Point pathStart, Point pathTurn, double radius,double half_span,double area);
    //弧圆心
    double d;
    Point center;
    Point pathStartPoint;
    Point trunPoint;
    //切点1,和切点2;规定切点一为右边切点
    Point tangentPoint1;
    Point tangentPoint2;
    //跨中点
    Point midSpanPoint;
    MyArc midArc;
    double getPivotDistance();
    double getYvalue(double x);
    double getAngleByX(double x);
    double getSigma_l1(float x);
    double getSigma_l2(float x);

};
#endif // MYTOOLKIT_H
