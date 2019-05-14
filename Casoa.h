#pragma once
#include<variables.h>
//作用效应组合计算
//承载能力极限状态 d1恒载值,cl汽车活载值;

float ultimateLimitSta(float dl, float cl);
//作用频遇组合
//dl恒载 cl汽车荷载
float sfd(float dl,float cl);
//准永久组合
float sqd(float dl,float cl);
void setSaftyGrade(int sg);
