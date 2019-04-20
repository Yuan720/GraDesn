#pragma execution_character_set("utf-8")
#ifndef MYTOOLKIT_H
#define MYTOOLKIT_H
#include<math.h>
const double Pi=3.1415926;
class Point{



public:
    Point(){};
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
    //弧圆心
    Point center;
    Point pathStartPoint;
    Point trunPoint;
    //切点1,和切点2;规定切点一为右边切点
    Point tangentPoint1;
    Point tangentPoint2;
    //跨中点
    Point midSpanPoint;
    MyArc midArc;

    double getYvalue(double x);
};
#endif // MYTOOLKIT_H
