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
//圆心角
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

myPath::myPath(Point vpathStart, Point vpathTurn, double vradius, double vhalf_span, double vd):myPath(vpathStart,vpathTurn,vradius,vhalf_span)
{
    d=vd;
}
myPath::myPath(Point pathStart, Point pathTurn, double radius,double half_span,double d,float stlarea):myPath(pathStart,pathTurn,radius,half_span,d){
    steelArea=stlarea;
}
double myPath::getPivotDistance()
{
    return pivot_X-pathStartPoint.x;
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
      {
        return tangentPoint1.y;}
}

double myPath::getAngleByX(double x)
{  //要返回的角度
   double angle;
    //总角度
    double total_angle;
    total_angle=atan((tangentPoint1.x-tangentPoint2.x)/(center.y-tangentPoint2.y));


    if(x<=tangentPoint2.x){
       angle=total_angle;
    }
    if(tangentPoint2.x<x&x<tangentPoint1.x){
      Point temp(x,getYvalue(x));
    angle=atan((tangentPoint1.x-temp.x)/(center.y-temp.y));

    }
    if(x>tangentPoint1.x){

               angle=0;
    }

    return angle;
}

double myPath::getAngleFromFurByX(double x)
{
    //要返回的角度
      double angle;
       //总角度
       double total_angle;
       total_angle=atan((tangentPoint1.x-tangentPoint2.x)/(center.y-tangentPoint2.y));


       if(x<=tangentPoint2.x){
          angle=0;
       }
       if(tangentPoint2.x<x&x<tangentPoint1.x){
         Point temp(x,getYvalue(x));
       angle=total_angle-atan((tangentPoint1.x-temp.x)/(center.y-temp.y));

       }
       if(x>tangentPoint1.x){

                  angle=total_angle;
       }

       return angle;

}
double myPath::getSigma_l1(float x)
{    //截面摩擦应力损失
    //x在式子里面被转为米为单位

    return 0.75*fpk*(1-pow(M_E,-(mu*getAngleFromFurByX(x)+k*( x-pathStartPoint.x)/1000)));
}
double myPath::getSigma_l2(float x)
{
   float sigma_0=0.75*fpk;
   float sigma_l=sigma_0-getSigma_l1(x);
   float delta_sigma_d=(sigma_0-sigma_l)/(x-pathStartPoint.x);
   //摩擦影响长度
   float lf=sqrt(delta_l*Ep/delta_sigma_d);
   if((x-pathStartPoint.x)>lf){
       return 0;
   }else{

    float delta_sigma=2*delta_sigma_d*lf;
    return delta_sigma*(lf-(x-pathStartPoint.x))/lf;


   }


}
vector<float> myPath::localPressure(float Al, float Ab,float Dcor)
{
    float gamma0=1.0;
    float Eta_s=1.0;
    float beta=sqrt(Ab/Al);
    float Fld=1.2*fpk*0.75*steelArea/4.6521739f;
    float Aln=Al-M_PI*pow(d,2)/4;
    float  value1=gamma0*Fld;
    float  value2=1.3*Eta_s*beta*fcd*Aln;
    float Acor=M_PI*pow(Dcor,2)/4;
   //局部抗压承载力
     float k=2.0;
     float beta_cor=sqrt(Acor/Al);
     float s=40;
     float Assl=78.54;//间接钢筋hrb335 截面面积78.54;
     float  rho_v=4*Assl/(Dcor*s);
     float value3=0.9*(Eta_s*beta*fcd*0.9+k*rho_v*beta_cor*fsd)*Aln;
 vector<float> res;
 res.push_back(value1);
 res.push_back(value2);
 res.push_back(value3);
 return res;










}


