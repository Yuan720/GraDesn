#ifndef BEAM_H
#define BEAM_H
#include<bridge.h>
#include<mytoolkit.h>
#include<QDebug>
#include<Casoa.h>


class beam
{//��׼��λ:m;
public:
    beam();
    beam(field_making_girder_beam pivot,field_making_girder_beam middle,vector<float> param1,int  cBN );
    field_making_girder_beam pivotSetion;//֧�����;
    field_making_girder_beam middleSetion;//���н���;
    float spanLength; //����糤;
    float AspLayer; //�����������װ���
    float MixedSoilLayer; //ˮ���������װ���
    float CenterCrossBeamV;//�к������
    float SideCorssBeamV;//�˺������
    int crossBeamNum;//�������Ŀ
    float half_Trans_start;//���������;
    float half_Trans_end;//�������յ�;
    float sectionArea;//������
    float Side_Cross_Bean_field_making_Area;//�˺�����ֽ������;
    float Midle_Cross_Bean_field_making_Area;//�к����ֽ������
    float Cross_Bean_t;//�����(��)���
    int m=2;//��������
    bool beamtype;
    vector<myPath> steelPaths;//�ֽ�����;
    float getFirstStageLoad();//һ�ں���;
    float getSecondStageLoad();//���ں���;
    float getThirdStageLoad(int n);//���ں���;
    float bendingSolve(float g,float x);
    float shearFoceSolve(float g,float x);
    void  setSteel(vector<myPath> paths);
    float getAverageSteelHeight(double x);
    float getAveSigma_l1(float x);
    float getAveSigma_l2(float x);
    float getSigma_l4(float Ap);
    float getSigma_l5(float Ap);
    float getSigma_l6(float Ap);
    float getPipAreaSum(bool precounted);
    float getPipStaticMomentSum(float x,bool precounted);
    float getPipSmoaSum(float x,float newCenterHeight ,bool precounted);
    float getSigma_L1_ById(int id,float x);
    float getSigma_L2_ById(int id,float x);
    float getSigma_L_I(float x,float Ap);
    float getSigma_L_II(float Ap);
    float getSigma_P_I(float x,float Ap);
    float getSigma_P_II(float x,float Ap);
    float getSigma_cu(float x,float Ap,float Mq);
    float getDepthOfCompression();
    float getSteel_a();//���ֽ�ĺ������õ㵽����ױ߾���
    float getSteel_a(float ap);
    float getSinThetaByX(float x);
    float get_Mu();
    float get_Apb(float x);
    float get_ep0(float x);
    float get_epn(float x);
    vector<float> deflecationSolve(float Ms);//�Ӷȼ���;
    vector<float> localPressure();//�ֲ���ѹ
    vector<float>  obliqueSectionCheaking(float x,float vd);//б���濹������
    vector <float> crackChecking(float MQs,float Mql);//�����濹������;
    vector <float> obliqueCrackChecking(float x); //б���濹��
    vector<float> MainStress(float x,float Mq,float VQs,int stage);//��Ӧ������
    vector<float> getSpecifiedAreaMoment(float x, bool fieldCount, bool preCounted);
    //  float getSigma_steel();
    vector<float> getSectionInfo(float x,bool fieldCount);//ǰ�ڼ��μ���
    vector<float> sectionFeatures(bool fieldCount,bool precounted,float x);//���Ǹֽ��ļ�������;
    vector<float> getSigma_ct(float Ap);
    vector<float> normalSectionCheaking();//���������������;
    vector<float> obliqueSectionCheaking();//б�������������;
    field_making_girder_beam getSectionAt(float x);
   float steelAreaSolve(float ms,float myfpk,float myap);//ԤӦ���ֽ����
   float unPreSteelAreaSlove(float Md,float a,float  myfpd,float myAp,float myfsd);



};

#endif // BEAM_H
