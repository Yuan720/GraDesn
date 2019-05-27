#pragma once
#include<math.h>
#include<bridge.h>
#include<QDebug>
//主梁内力
struct MainBeamData
{	//桥宽;
    float BrigeWidth;
    //沥青混凝土铺装厚度
    float AspLayer;
    //水泥混凝土铺装厚度
    float MixedSoilLayer;
    float MBArea;//中梁横截面积
    float SideBeamArea;//边梁横截面积
    float CenterCrossBeamV;//中横梁体积
    float SideCorssBeamV;//端横梁体积
    float CalculaSpan;//计算跨径
    float gr;//栏杆集度;
    int crossBeamNum;//横隔梁数目
    int mianBeanNum;//主梁片数;
    float Main_Bean_field_making_Area;//主梁现浇段面积;
    float Side_Cross_Bean_field_making_Area;//端横隔梁现浇段面积;
    float Midle_Cross_Bean_field_making_Area;//中横梁现浇段面积
    float Cross_Bean_t;//横隔梁(宽)厚度






};

float getArraySum(float *p, int size);
class MainBeam {
public:
    MainBeamData mbd;
    //求解剪力
    //栏杆
    MainBeam() {}
    MainBeam(MainBeamData mymbd) {
        mbd = mymbd;

    }
    //栏杆
    float collectionDegreeSolve() {
        return 2 * mbd.gr / mbd.mianBeanNum;

    }
    float getMainBeanShearFoce(float g, float x)
    {
        return g*(0.5*mbd.CalculaSpan -  x);

    }
    //求解弯矩
    float getMainBeanBending(float g, float x) {

        return 0.5*g*mbd.CalculaSpan*x - 0.5*g*pow(x, 2);
    }
    //求解一期荷载集度
    float * firstStageLoad() {
        float *re = new float[2];
        //边主梁集度
        float	bzl1 = mbd.SideBeamArea * 25;
        //边横梁折算
        float	bhl1 = (mbd.SideCorssBeamV * 2 + (mbd.crossBeamNum - 2)*mbd.CenterCrossBeamV) * 25 / mbd.CalculaSpan;
        //中主梁集度
        float	zzl2 = mbd.MBArea * 25;
        //中横梁折算
        float	zhl2 = 2 * (mbd.SideCorssBeamV * 2 + (mbd.crossBeamNum - 2)*mbd.CenterCrossBeamV) * 25 / mbd.CalculaSpan;
        re[0] = bzl1 + bhl1;
        re[1] = zzl2 + zhl2;
        //返回大小为2的数组,下标1对应边梁一期荷载集度,下标2对应中梁一期荷载集度;
     //   qDebug()<<"yiqi:"<<re[0]<<"---"<<re[1];
        return re;


    }
    //求解二期荷载
    float * secondStageLoad() {
        //桥面铺装层计算
        float gh1= (mbd.MixedSoilLayer * 24)*mbd.BrigeWidth / mbd.mianBeanNum;
        float bzl1 = mbd.Main_Bean_field_making_Area * 25;
        float bhl1 = (mbd.Side_Cross_Bean_field_making_Area * 2 + mbd.Midle_Cross_Bean_field_making_Area*(mbd.crossBeamNum - 2))*mbd.Cross_Bean_t * 25 / mbd.CalculaSpan;


        float zzl2 = mbd.Main_Bean_field_making_Area * 2 * 25;;
        float zhl2 = bhl1 * 2;
        float *re = new float[3];
        re[0] = bzl1 + bhl1+gh1;
        re[1] = zzl2 + zhl2+gh1;
         //   re[2] = gh1;
        return re;
    }

    float  ThirdStageLoad(){

         float gh2 = (mbd.AspLayer * 23)*mbd.BrigeWidth / mbd.mianBeanNum;
         float langan=collectionDegreeSolve();
          float re = gh2+langan;

          return re;

    }
};

