#pragma once
#ifndef VARIABLES_H
#define VARIABLES_H
extern float bridge_total_Span;
//预应力钢绞线弹性模量
extern float Ep;
//混凝土弹性模量
extern float Ec;
//指张拉端锚具变形值
extern float delta_l;
extern float bridge_calspan;
extern double Pi;
extern float fpk;
extern float fcuk;//混泥土强度等级;
extern float ftd;//混泥土轴心抗拉;
//摩擦系数和摩擦影响系数
extern float mu;
extern float k;
//支点中心线坐标;
extern float pivot_X;
//非预应力钢筋面积
extern float non_preSteel_area;

//非预应力钢筋换算面积
extern float non_prestreSteel_s;
//非预应力钢筋合力重心
extern float non_prestrepip_center;
//m张拉批数
extern int m;
//徐变系数  收缩应变终极值
extern float phi_1;
extern float phi_2;
extern float xi_cs;
extern float fcd;//非预应力钢筋轴心抗压强度
extern float fpd;//预应力钢筋抗拉强度设计值
extern float fsd;//非预应力钢筋抗拉强度设计值
extern float Ap;//预应力筋面积;
extern float As;//非预应力钢筋面积
extern float as;
extern float ap;
extern float cross_beam_foce[5][6];
extern int saftyGrade;
extern int numberOfBeam;
extern float NominalStArea;//预应力筋公称面积;
extern float Fsv;//箍筋强度
extern float nv;//肢数
extern float dv;//箍筋直径
extern float Sv1;//箍筋间距1;
extern float Sv2;
extern float divid_X;
extern float sigma_con;//控制张拉应力;
extern float feck;
extern float fck;
#endif // VARIABLES_H
