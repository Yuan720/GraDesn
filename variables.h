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
//摩擦系数和摩擦影响系数
extern float mu;
extern float k;
//支点中心线坐标;
extern float pivot_X;

//非预应力钢筋换算面积
extern float non_prestreSteel_s;
//非预应力钢筋合力重心
extern float non_prestrepip_center;


#endif // VARIABLES_H
