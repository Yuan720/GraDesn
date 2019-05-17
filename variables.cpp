#include<variables.h>
#include<math.h>
double Pi=3.1415926;
float fpk=1860;
float mu=0.25;
float k=0.0015;
float fcuk=50;//混泥土强度等级默认C50 Mpa;
float ftd;//默认C50 1.83;
float pivot_X=550;
float bridge_total_Span;
float bridge_calspan;
float Ep=1.95*pow(10,5);
float Ec=3.45*pow(10,4);
float delta_l=4;
int m=2;
float phi_1=2.37;
float phi_2=1.768;
float xi_cs=0.000357;
//非预应力钢筋面积
float non_preSteel_area=4826;
int saftyGrade =1;
//非预应力钢筋换算面积
float non_prestreSteel_s=23150;
//非预应力钢筋合力重心
 float non_prestrepip_center=45;
 float fcd=22.4;
 float fpd=1260;
 float fsd=330;
 float Ap=6272;
 float As=4826;
 float as=45;
 float ap=125;
 float cross_beam_foce[5][6];
 int numberOfBeam=4;
 float NominalStArea=140;
 float Fsv=280;//箍筋强度
 float nv=2;//肢数
 float dv=10;//箍筋直径mm
 float Sv1=200;//箍筋间距1;
 float Sv2=200;
 float divid_X=8000;//毫米计算
 //ap=125mm
