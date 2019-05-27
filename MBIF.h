#pragma once
#include<math.h>
#include<bridge.h>
#include<QDebug>
//��������
struct MainBeamData
{	//�ſ�;
    float BrigeWidth;
    //�����������װ���
    float AspLayer;
    //ˮ���������װ���
    float MixedSoilLayer;
    float MBArea;//����������
    float SideBeamArea;//����������
    float CenterCrossBeamV;//�к������
    float SideCorssBeamV;//�˺������
    float CalculaSpan;//����羶
    float gr;//���˼���;
    int crossBeamNum;//�������Ŀ
    int mianBeanNum;//����Ƭ��;
    float Main_Bean_field_making_Area;//�����ֽ������;
    float Side_Cross_Bean_field_making_Area;//�˺�����ֽ������;
    float Midle_Cross_Bean_field_making_Area;//�к����ֽ������
    float Cross_Bean_t;//�����(��)���






};

float getArraySum(float *p, int size);
class MainBeam {
public:
    MainBeamData mbd;
    //������
    //����
    MainBeam() {}
    MainBeam(MainBeamData mymbd) {
        mbd = mymbd;

    }
    //����
    float collectionDegreeSolve() {
        return 2 * mbd.gr / mbd.mianBeanNum;

    }
    float getMainBeanShearFoce(float g, float x)
    {
        return g*(0.5*mbd.CalculaSpan -  x);

    }
    //������
    float getMainBeanBending(float g, float x) {

        return 0.5*g*mbd.CalculaSpan*x - 0.5*g*pow(x, 2);
    }
    //���һ�ں��ؼ���
    float * firstStageLoad() {
        float *re = new float[2];
        //����������
        float	bzl1 = mbd.SideBeamArea * 25;
        //�ߺ�������
        float	bhl1 = (mbd.SideCorssBeamV * 2 + (mbd.crossBeamNum - 2)*mbd.CenterCrossBeamV) * 25 / mbd.CalculaSpan;
        //����������
        float	zzl2 = mbd.MBArea * 25;
        //�к�������
        float	zhl2 = 2 * (mbd.SideCorssBeamV * 2 + (mbd.crossBeamNum - 2)*mbd.CenterCrossBeamV) * 25 / mbd.CalculaSpan;
        re[0] = bzl1 + bhl1;
        re[1] = zzl2 + zhl2;
        //���ش�СΪ2������,�±�1��Ӧ����һ�ں��ؼ���,�±�2��Ӧ����һ�ں��ؼ���;
     //   qDebug()<<"yiqi:"<<re[0]<<"---"<<re[1];
        return re;


    }
    //�����ں���
    float * secondStageLoad() {
        //������װ�����
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

