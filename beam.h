#ifndef BEAM_H
#define BEAM_H
#include<bridge.h>
#include<mytoolkit.h>
#include<QDebug>
#include<Casoa.h>


class beam
{//基准单位:m;
public:
    beam();
    beam(field_making_girder_beam pivot,field_making_girder_beam middle,vector<float> param1,int  cBN );
    field_making_girder_beam pivotSetion;//支点截面;
    field_making_girder_beam middleSetion;//跨中截面;
    float spanLength; //计算跨长;
    float AspLayer; //沥青混凝土铺装厚度
    float MixedSoilLayer; //水泥混凝土铺装厚度
    float CenterCrossBeamV;//中横梁体积
    float SideCorssBeamV;//端横梁体积
    int crossBeamNum;//横隔梁数目
    float half_Trans_start;//半跨过渡起点;
    float half_Trans_end;//半跨过渡终点;
    float sectionArea;//横截面积
    float Side_Cross_Bean_field_making_Area;//端横隔梁现浇段面积;
    float Midle_Cross_Bean_field_making_Area;//中横梁现浇段面积
    float Cross_Bean_t;//横隔梁(宽)厚度
    int m=2;//张拉批数
    bool beamtype;
    vector<myPath> steelPaths;//钢筋走向;
    float getFirstStageLoad();//一期恒载;
    float getSecondStageLoad();//二期恒载;
    float getThirdStageLoad(int n);//三期恒载;
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
    float getSteel_a();//求解钢筋的合力作用点到截面底边距离
    float getSteel_a(float ap);
    float getSinThetaByX(float x);
    float get_Mu();
    float get_Apb(float x);
    float get_ep0(float x);
    float get_epn(float x);
    vector<float> deflecationSolve(float Ms);//挠度计算;
    vector<float> localPressure();//局部承压
    vector<float>  obliqueSectionCheaking(float x,float vd);//斜截面抗剪检验
    vector <float> crackChecking(float MQs,float Mql);//正截面抗裂验算;
    vector <float> obliqueCrackChecking(float x); //斜截面抗裂
    vector<float> MainStress(float x,float Mq,float VQs,int stage);//主应力计算
    vector<float> getSpecifiedAreaMoment(float x, bool fieldCount, bool preCounted);
    //  float getSigma_steel();
    vector<float> getSectionInfo(float x,bool fieldCount);//前期几何计算
    vector<float> sectionFeatures(bool fieldCount,bool precounted,float x);//考虑钢筋后的几何特征;
    vector<float> getSigma_ct(float Ap);
    vector<float> normalSectionCheaking();//正截面承载力检验;
    vector<float> obliqueSectionCheaking();//斜截面承载力检验;
    field_making_girder_beam getSectionAt(float x);
   float steelAreaSolve(float ms,float myfpk,float myap);//预应力钢筋计算
   float unPreSteelAreaSlove(float Md,float a,float  myfpd,float myAp,float myfsd);



};

#endif // BEAM_H
