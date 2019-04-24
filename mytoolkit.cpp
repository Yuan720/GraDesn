#include<mytoolkit.h>
#include<QDebug>

Point::Point(double vx,double vy){
    x=vx;y=vy;
}
double Point::distanceFrom(Point p){

    return sqrt(pow(x-p.x,2)+pow(y-p.y,2));
}

Square::Square(Point vtopleft,double vel){

    leftTop=vtopleft;
    el=vel;
}

MyArc::MyArc(double vradius,Point vstartPoint,Point vendPoint,double vangleLength){
    radius=vradius;startPoint=vstartPoint;endPoint=vendPoint;angleLength=vangleLength;
    outerSquare=Square(Point(startPoint.x-radius,startPoint.y+2*radius),2*radius);
    startAngle=-Pi/2;




}

myPath::myPath(Point pathStart,Point pathTurn,double radius,double half_span){
double tangent1=fabs(pathStart.y-pathTurn.y)/fabs(pathStart.x-pathTurn.x);
//Բ�Ľ�
double circleCor=atan( tangent1);
double l1=radius*tan(circleCor/2);
pathStartPoint=pathStart;
trunPoint=pathTurn;
tangentPoint1=Point(pathTurn.x+l1,pathTurn.y);
center=Point(tangentPoint1.x,tangentPoint1.y+radius);
tangentPoint2=Point(center.x-radius*sin(circleCor),center.y-radius*cos(circleCor));
midSpanPoint=Point(half_span,tangentPoint1.y);
midArc=MyArc(radius,tangentPoint1,tangentPoint2,circleCor);

}

myPath::myPath(Point vpathStart, Point vpathTurn, double vradius, double vhalf_span, double vd)
{
    double tangent1=fabs(vpathStart.y-vpathTurn.y)/fabs(vpathStart.x-vpathTurn.x);
    //Բ�Ľ�
    double circleCor=atan( tangent1);
    double l1=vradius*tan(circleCor/2);
    pathStartPoint=vpathStart;
    trunPoint=vpathTurn;
    tangentPoint1=Point(vpathTurn.x+l1,vpathTurn.y);
    center=Point(tangentPoint1.x,tangentPoint1.y+vradius);
    tangentPoint2=Point(center.x-vradius*sin(circleCor),center.y-vradius*cos(circleCor));
    midSpanPoint=Point(vhalf_span,tangentPoint1.y);
    midArc=MyArc(vradius,tangentPoint1,tangentPoint2,circleCor);
    d=vd;
}
double myPath::getYvalue(double x){
    x=x<midSpanPoint.x? x:2*midSpanPoint.x-x;

    if (x<pathStartPoint.x)
        return 0;
    double tangent1=fabs(pathStartPoint.y-trunPoint.y)/fabs(pathStartPoint.x-trunPoint.x);

    if(x<tangentPoint2.x)
        return pathStartPoint.y-(x-pathStartPoint.x)*tangent1;
    if(x<tangentPoint1.x)
        return center.y-sqrt(pow(midArc.radius,2)-pow(tangentPoint1.x-x,2));
    else
        return tangentPoint1.y;
}
