#include "beam.h"
beam::beam()
{
qDebug()<<"创建beam实例!"<<endl;
}
beam::beam(field_making_girder_beam pivot, field_making_girder_beam middle, vector<float> param1, int cBN)
{
    pivotSetion=pivot;
    middleSetion=middle;
     spanLength=param1[0];
     AspLayer=param1[1];
     MixedSoilLayer=param1[2];
    CenterCrossBeamV=param1[3];
    SideCorssBeamV=param1[4];
     half_Trans_start=param1[5];
    half_Trans_end=param1[6];
    Side_Cross_Bean_field_making_Area=param1[7];
    Midle_Cross_Bean_field_making_Area=param1[8];
    Cross_Bean_t=param1[9];
    crossBeamNum=cBN;
    sectionArea=middle.sbg.AreaSolve();//计算主梁自重横截面积取的跨中的,表示疑问?;
    beamtype=pivot.sbg.left.isSideBeam|pivot.sbg.right.isSideBeam;

}
void beam::setSteel(vector<myPath> paths)
{
    steelPaths.assign(paths.begin(),paths.end());

}
float beam::getAverageSteelHeight(double x)
{
    float AreaMoment=0;//面积矩
    float Area=0;//面积
    for(int i=0;i<steelPaths.size();i++){

            AreaMoment+=steelPaths[i].getYvalue(x)*steelPaths[i].steelArea;
            Area+=steelPaths[i].steelArea;
        }
        return AreaMoment/Area;

}
float beam::getAveSigma_l1(float x)
{
    //todo
    float sigma_l1=0;
    for(int i=0;i<steelPaths.size();i++){
        sigma_l1+=steelPaths[i].getSigma_l1(x);

    }
    return sigma_l1/steelPaths.size();

}
float beam::getAveSigma_l2(float x)
{
    float sigma_l2=0;
    for(int i=0;i<steelPaths.size();i++){
        sigma_l2+=steelPaths[i].getSigma_l2(x);

    }
    return sigma_l2/steelPaths.size();
}
float beam::getFirstStageLoad()
{   if(beamtype){
        return  25*sectionArea+(2*SideCorssBeamV+(crossBeamNum-2)*CenterCrossBeamV)*25/(spanLength/1000);

    }else{

        return  25*sectionArea+2*(2*SideCorssBeamV+(crossBeamNum-2)*CenterCrossBeamV)*25/(spanLength/1000);

    }
}
float beam::getSecondStageLoad()
{
    float gh;
    float gf1;
    float gf2;
    if(beamtype){
        gh=(MixedSoilLayer*24)*(middleSetion.sbg.left.b1+middleSetion.sbg.right.b1+middleSetion.fmgb_length);
        gf1=(middleSetion.fmgb_length*middleSetion.sbg.left.d1)*25;
        gf2=(2*Side_Cross_Bean_field_making_Area+(crossBeamNum-2)*Midle_Cross_Bean_field_making_Area)*Cross_Bean_t*25/(spanLength/1000);

        }else{
        gh=(MixedSoilLayer*24)*(middleSetion.sbg.left.b1+middleSetion.sbg.right.b1+2*middleSetion.fmgb_length);
        gf1=(middleSetion.fmgb_length*middleSetion.sbg.left.d1)*25*2;
        gf2=2*(2*Side_Cross_Bean_field_making_Area+(crossBeamNum-2)*Midle_Cross_Bean_field_making_Area)*Cross_Bean_t*25/(spanLength/1000);

        }
    return gh+gf1+gf2;


}
float beam::getThirdStageLoad(int n)
{   float gh;
    if(beamtype){
             gh=(AspLayer*23)*(middleSetion.sbg.left.b1+middleSetion.sbg.right.b1+middleSetion.fmgb_length);

             }else{
             gh=(AspLayer*23)*(middleSetion.sbg.left.b1+middleSetion.sbg.right.b1+2*middleSetion.fmgb_length);

             }
    return (float)(2*5)/n +gh;
}
float beam::bendingSolve(float g,float x)
{ //输入单位m
   //返回单位kn/m
    return   0.5*g*(spanLength/1000)*x - 0.5*g*pow(x, 2);
}
float beam::shearFoceSolve(float g,float x)
{   //输入单位m
    //返回单位kn
    return g*(0.5*(spanLength/1000)-x);
}
float beam::getSigma_l4(float Ap)
{
  float  x=0.25*spanLength;
  float sigma_al1=getAveSigma_l1(x);
  float  sigma_al2=getAveSigma_l2(x);
   float Np=(fpk*0.75-sigma_al1-sigma_al2)*Ap;
   vector<float> mydata=sectionFeatures(false,false,x);
  float A=mydata[0];float I=mydata[1];float ep=mydata[4];
  float apha_ep=1.95*pow(10,5)/(3.35*pow(10,4));
  float sigma_pc=Np/A+(Np*pow(ep,2))/I;
  return (float)(m-1)/(2*m)*apha_ep*sigma_pc;

}
float beam::getSigma_l5(float Ap)
{
       float sigma_l1=getAveSigma_l1(spanLength/4);
       float sigma_l2=getAveSigma_l2(spanLength/4);
       float  sigma_l4=getSigma_l4(Ap);
       float  sigma_pe=0.75*fpk-sigma_l1-sigma_l2-sigma_l4;
       float param1=0.9f; //张拉系数
       float   param2=0.3f; //松弛系数
       return param1*param2*(0.52*sigma_pe/fpk-0.26)*sigma_pe;
}
float beam::getSigma_l6(float Ap)
{
    vector<float> MG1;
    vector<float> MG2;
    vector<float> Wnp;
    vector<float> W0p;
    vector<float> NpI_1;
    vector<float> An;
    vector<float> In;
    vector<float> ep;
    vector<float> sectionInfo_1=sectionFeatures(false, false, spanLength/2);
    vector<float> sectionInfo_2=sectionFeatures(true, true, spanLength/2);
    vector<float> sectionInfo_3=sectionFeatures(false, false, spanLength/4);
    vector<float> sectionInfo_4=sectionFeatures(true,true, spanLength/4);
    vector<float> sigma_pc;
    vector<float> e_ps;
    float apha_ep=5.65f;

   MG1.push_back(1e6*bendingSolve(getFirstStageLoad(),spanLength/(2*1000)));
   MG1.push_back(1e6*bendingSolve(getFirstStageLoad(),spanLength/(4*1000)));
   MG2.push_back(1e6*bendingSolve(getSecondStageLoad()+getThirdStageLoad(4),spanLength/(2*1000)));
   MG2.push_back(1e6*bendingSolve(getSecondStageLoad()+getThirdStageLoad(4),spanLength/(4*1000)));

    vector<float> avrege1;
   avrege1.push_back(getAveSigma_l1(spanLength/2));
   avrege1.push_back(getAveSigma_l2(spanLength/2));
    vector<float> avrege2;
    avrege2.push_back(getAveSigma_l1(spanLength/4));
     avrege2.push_back(getAveSigma_l2(spanLength/4));
    NpI_1.push_back((0.75*fpk-avrege1[0]-avrege1[1]-getSigma_l4(Ap))*Ap);
    NpI_1.push_back((0.75*fpk-avrege2[0]-avrege2[1]-getSigma_l4(Ap))*Ap);
    Wnp.push_back(sectionInfo_1[1]/sectionInfo_1[4]);
    Wnp.push_back(sectionInfo_3[1]/sectionInfo_3[4]);
    W0p.push_back(sectionInfo_2[1]/sectionInfo_2[4]);
    W0p.push_back(sectionInfo_4[1]/sectionInfo_4[4]);
    An.push_back(sectionInfo_1[0]);
    An.push_back(sectionInfo_3[0]);
    In.push_back(sectionInfo_1[1]);
    In.push_back(sectionInfo_3[1]);
    ep.push_back(sectionInfo_1[4]);
    ep.push_back(sectionInfo_3[4]);
  float temp1=NpI_1[0]/An[0]+NpI_1[0]*pow(sectionInfo_1[4],2)/In[0]-MG1[0]/Wnp[0]-phi_2/phi_1*MG2[0]/W0p[0];
  float temp2=NpI_1[1]/An[1]+NpI_1[1]*pow(sectionInfo_3[4],2)/In[1]-MG1[1]/Wnp[1]-phi_2/phi_1*MG2[1]/W0p[1];
  sigma_pc.push_back(temp1);
  sigma_pc.push_back(temp2);
  sigma_pc.push_back(0.5*(sigma_pc[0]+sigma_pc[1]));
  float rho=(Ap+non_preSteel_area)/sectionInfo_2[0];
  e_ps.push_back((Ap*sectionInfo_1[4]+non_preSteel_area*fabs(non_prestrepip_center-sectionInfo_1[3]))/(Ap+non_preSteel_area));
  e_ps.push_back((Ap*sectionInfo_3[4]+non_preSteel_area*fabs(non_prestrepip_center-sectionInfo_3[3]))/(Ap+non_preSteel_area));
  float e_psav=(e_ps[0]+e_ps[1])/2;
  float A_avr=(sectionInfo_2[0]+sectionInfo_4[0])/2;
  float I_avr=(sectionInfo_2[1]+sectionInfo_4[0])/2;
  float rho_Ps=1+pow(e_psav,2)/(I_avr/A_avr);
  float result=0.9*(1.95*pow(10,4)*xi_cs+apha_ep*sigma_pc[2]*phi_1)/(1+15*rho*rho_Ps);
  return result;
}
float beam::getPipAreaSum(bool precounted)
{
    float res=0;
        for(int i=0;i<steelPaths.size();i++){
            res+= M_PI*pow(steelPaths[i].d,2)*0.25;
            if(precounted){
                res-=steelPaths[i].steelArea;
            }
        }
        return res;
}
float beam::getPipStaticMomentSum(float x, bool precounted)
{
    //返回截面x处的预留管的等效

        float staticMomt=0;
        for(int i=0;i<steelPaths.size();i++){
            staticMomt+= M_PI*(pow(steelPaths[i].d,2)*0.25)*steelPaths[i].getYvalue(x);
            if(precounted){
               staticMomt-=steelPaths[i].getYvalue(x)*steelPaths[i].steelArea;
            }
}
  return  staticMomt;
}
float beam::getPipSmoaSum(float x, float newCenterHeight, bool precounted)
{
        float smoa=0;
        //返回截面x处预留管的等效惯性矩和
        for(int i=0;i<steelPaths.size();i++){
         smoa+= M_PI*pow(steelPaths[i].d,4)/64+pow(steelPaths[i].getYvalue(x)-newCenterHeight,2)* M_PI*pow(steelPaths[i].d/2,2);
         if(precounted){
             //预应力筋等效惯性矩;
           smoa-=pow(steelPaths[i].getYvalue(x)-newCenterHeight,2)*steelPaths[i].steelArea;
        }
    }
        return smoa;
}
float beam::getSigma_L1_ById(int id,float x)
{
    return steelPaths[id+1].getSigma_l1( x);
}
float beam::getSigma_L2_ById(int id, float x)
{
    return steelPaths[id+1].getSigma_l2( x);
}
float beam::getSigma_L_I(float x,float Ap)
{ //平均值
 return getAveSigma_l1(x)+getAveSigma_l2(x)+getSigma_l4(Ap);
}
float beam::getSigma_L_II(float Ap)
{   //平均值
    return getSigma_l5(Ap)+getSigma_l6(Ap);
}
float beam::getSigma_P_I(float x, float Ap)
{
    return 0.75*fpk-getSigma_L_I(x,Ap);
}
float beam::getSigma_P_II(float x,float Ap)
{
    return 0.75*fpk-getSigma_L_II(Ap)-getSigma_L_I(x,Ap);
}
vector<float> beam:: PermanentCheak(float x,float Mq)
{   //Mq指三期荷载与计入冲击系数的汽车荷载之和;
    //截面混凝土正应力
    //x截面位置;
    //输入的x以mm为单位;
    //默认四片梁
    vector<float> result;
     float myd=(bridge_total_Span*1e3-spanLength)/2;
    float MG1=1e6*bendingSolve(getFirstStageLoad(),x/1000);
    float MG21=1e6*bendingSolve(getSecondStageLoad(),x/1000);
    float MG22=1e6*bendingSolve(getThirdStageLoad(4),x/1000);
    float Sigma_PII=getSigma_P_II(x+myd,Ap);
    float Sigma_l6=getSigma_l6(Ap);
    float As=non_preSteel_area;
    float as=non_prestrepip_center;
    float ap;
    float An;
    ap=getAverageSteelHeight(x+myd);
    float NpII=getSigma_P_II(x+myd,Ap)*Ap-getSigma_l6(Ap)*As;
    vector<float> sectionInfo_1=sectionFeatures(false, false, x);//以支点为零点
    float ynb=sectionInfo_1[3];
    float epn=(Sigma_PII*Ap*(ynb-ap)-Sigma_l6*As*(ynb-as))/(Sigma_PII*Ap-Sigma_l6*As);
   vector<float> sectionInfo_2=sectionFeatures(false, true, x);
   vector<float> sectionInfo_3=sectionFeatures(true, true, x);
   An=sectionInfo_1[0];
   float Wnu=sectionInfo_1[6];
   float W0u=sectionInfo_2[6];
   float We0u=sectionInfo_3[6];
   float sigma_cu;
   sigma_cu=NpII/An-NpII*epn/Wnu+MG1/Wnu+MG21/We0u+(MG22+Mq)/W0u;
   float sigma_Kt;
   float apha_ep;
   float sigma_Val;
   float Wop,Weop;
   apha_ep=Ep/Ec;
  Weop=sectionInfo_2[1]/sectionInfo_2[4];
  Wop=sectionInfo_3[1]/sectionInfo_2[4];
  sigma_Kt=MG21/Weop+(MG22+Mq)/Wop;
  sigma_Val=getSigma_P_II(x+myd,Ap)+apha_ep*sigma_Kt;


   result.push_back(sigma_cu);
   result.push_back(sigma_Val);
   return result;

}
float beam::getDepthOfCompression(float x)
{//受压区高度;
    //x以mm为单位;
    field_making_girder_beam SolveBeam=getSectionAt(x);
    bool beamtype=(SolveBeam.sbg.left.isSideBeam|SolveBeam.sbg.right.isSideBeam);
    float field=beamtype? SolveBeam.fmgb_length:2*SolveBeam.fmgb_length;
    //(middleSetion.sbg.left.isSideBeam|middleSetion.sbg.right.isSideBeam)? middleSetion.fmgb_length:2*middleSetion.fmgb_length;
    float bf=SolveBeam.sbg.left.b1+SolveBeam.sbg.right.b1+field;
     bf*=1e3;

     return (fpd*Ap+fsd*As)/(fcd*bf);//单位毫米
}
float beam::getSteel_a()
{   //a

    float a=(fpd*Ap*ap+fsd*As*as)/(fpd*Ap+fsd*As);
    return a;
}
float beam::getSteel_a(float ap)
{
    float a=(fpd*Ap*ap+fsd*As*as)/(fpd*Ap+fsd*As);
    return a;
}
float beam::getSinThetaByX(float x)
{   x=x>bridge_total_Span*1e3/2? bridge_total_Span*1e3-x:x;
    float sintheta=0;
    for(int i=0;i<steelPaths.size();i++){
      float test=steelPaths[i].getAngleByX(x);
        sintheta+=sin(steelPaths[i].getAngleByX(x));

    }
    return sintheta/steelPaths.size();
}
float beam::get_Mu(float lca)
{    float myd=(bridge_total_Span*1e3-spanLength)/2;
    field_making_girder_beam SolveBeam=getSectionAt(lca);
     bool beamtype=(SolveBeam.sbg.left.isSideBeam|SolveBeam.sbg.right.isSideBeam);
     float field=beamtype? SolveBeam.fmgb_length:2*SolveBeam.fmgb_length;
     float bf=SolveBeam.sbg.left.b1+SolveBeam.sbg.right.b1+field;
      bf*=1e3;
     float x=getDepthOfCompression(lca);
    float h0=1e3*middleSetion.sbg.left.h-getSteel_a(getAverageSteelHeight(lca+myd));
    return fcd*bf*x*(h0-x/2)/1e6;//返回单位kn.m;
}
float beam::get_Apb(float x)
{   //x是以主梁全场端点为零点的;
    x=x>1e3*bridge_total_Span/2? 1e3*bridge_total_Span-x:x;
    float Apb=0;
    for(int i=0;i<steelPaths.size();i++){
        if(steelPaths[i].getAngleByX(x)>0){
            Apb+=steelPaths[i].steelArea;
        }

    }
      return Apb/4.6521739;
}
float beam::get_epn(float x)
{
    return 0;
}
vector<float> beam::deflecationSolve(float Ms)
{  Ms*=1e6;
 float apha=(float)5/48;
 float Ec=3.45*1e4;
 float L=spanLength;
 vector<float> sectionInfo_3=sectionFeatures(true, true, L/4);
 vector<float> sectionInfo_1=sectionFeatures(false, false, L/4);
 float I0=sectionInfo_3[1];
 float In=sectionInfo_1[1];
float ynb=sectionInfo_3[3];
 float Wqs;
 Wqs=apha*pow(L,2)/(0.95*Ec)*Ms/I0;
 float Wql;
 Wql=Wqs*1.43;
 float WG1,MG1,MG2;
 MG1=1e6*bendingSolve(getFirstStageLoad(),spanLength/(2*1e3));
 MG2=1e6*bendingSolve(getSecondStageLoad()+getThirdStageLoad(4),spanLength/(2*1e3));

 WG1=1.43*apha*pow(L,2)/(0.95*Ec)*(MG1+MG2)/I0;
float NpII=getSigma_P_II(L/4,Ap)*get_Apb(L/4)*sqrt(1-pow(getSinThetaByX(L/4),2))+getSigma_P_II(L/4,Ap)*(Ap-get_Apb(L/4))-getSigma_l6(Ap)*As;
float ep0=((getSigma_P_II(L/4,Ap)*get_Apb(L/4)*sqrt(1-pow(getSinThetaByX(L/4),2))+getSigma_P_II(L/4,Ap)*(Ap-get_Apb(L/4)))*(ynb-getAverageSteelHeight(L/4))-getSigma_l6(Ap)*As*(ynb-as))/NpII;
float Mpe=NpII*ep0;
float delta_pe=-(Mpe*pow(L,2)/(8*0.95*Ec*In));
float delta_pel=2*delta_pe;
float Wl=Wql+WG1-fabs(delta_pel);
vector<float> res;
res.push_back(Wqs);
res.push_back(Wql);
res.push_back(Wqs);
res.push_back(WG1);
res.push_back(delta_pe);
res.push_back(delta_pel);
res.push_back(Wl);
return res;

}
vector<float>  beam::obliqueSectionCheaking(float x)
{
  //-------------------------
    vector<float> res;
    float myd=(1e3*bridge_total_Span-spanLength)/2;
    float apha1=1, apha2=1.25, apha3=1.1;
    float ap=getAverageSteelHeight(x+myd);
    float a=getSteel_a(ap);
    float h0=1e3*middleSetion.sbg.left.h-a;
    float b;
    field_making_girder_beam fb=getSectionAt(x);
    b=(fb.sbg.left.d2+fb.sbg.right.d2)*1e3;
    float Asv=nv*M_PI*pow(dv,2)/4;
    float sv=x<divid_X? Sv1:Sv2;
    float rhosv=Asv/(sv*b);
    float p=100*(Ap+As)/(b*h0);
    float vcs=apha1*apha2*apha3*0.45*1e-3*b*h0*sqrt((2+0.6*p)*sqrt(fcuk)*rhosv*Fsv);
    float sinThetaP=getSinThetaByX(x+myd);
    float apb=get_Apb(x+myd);
    float vpd=0.75*1e-3*fpd*apb*sinThetaP;
    res.push_back(0.5*1e-3*apha2*ftd*b*h0);
    res.push_back(0.51*1e-3*sqrt(fcuk)*b*h0);
    res.push_back(vcs+vpd);
    return res;
}
vector<float> beam::crackChecking(float MQs,float Mql)
{  //按照跨中截面计算
    //Wn    第一阶段
    //We0   阶段二
    //W0    阶段三
    MQs*=1e6;
    Mql*=1e6;
    float NpII;
    float rho_pc;
    float rho_st;
    float rho_lt;
    float  Wnb,An,epn, W,Wn,We0,W0,Ms,MG1,MG21,MG22,ynb;
    vector<float> sectiondata1=sectionFeatures(false,false,spanLength/2);
    vector<float> sectiondata2=sectionFeatures(false,true,spanLength/2);
    vector<float> sectiondata3=sectionFeatures(true,true,spanLength/2);
    //todo;
    ynb=sectiondata1[3];
    Wn=sectiondata1[5];We0=sectiondata2[5];W0=sectiondata3[5];
    NpII=getSigma_P_II(spanLength/2,Ap)*Ap-getSigma_l6(Ap)*As;
     epn=(getSigma_P_II(spanLength/2,Ap)*Ap*(ynb-ap)-getSigma_l6(Ap)*As*(ynb-as))/(getSigma_P_II(spanLength/2,Ap)*Ap-getSigma_l6(Ap)*As);
    An=sectiondata1[0];Wnb=sectiondata1[5];
    MG1=1e6*bendingSolve(getFirstStageLoad(),spanLength/2/1e3);
    MG21=1e6*bendingSolve(getSecondStageLoad(),spanLength/2/1e3);
    MG22=1e6*bendingSolve(getThirdStageLoad(4),spanLength/2/1e3);
    rho_pc=NpII/An+(NpII*epn)/Wnb;
    rho_st=MG1/Wn+MG21/We0+MG22/W0+MQs/W0;
    rho_lt=MG1/Wn+MG21/We0+MG22/W0+Mql/W0;
    vector<float> res;
    res.push_back(rho_pc);
    res.push_back(rho_st);
    res.push_back(rho_st-rho_pc);
    res.push_back(rho_lt);
    res.push_back(rho_lt-rho_pc);
    return res;

}
vector<float> beam::obliqueCrackChecking(float x)
{//斜截面抗裂
    vector<float> res;
    return res;
}
vector<float> beam::MainStress(float x, float Mq, float Vq)
{   //x以毫米为单位
    Mq*=1e6;
    Vq*=1e3;
    vector<float> MSn1;
    vector<float> MSn2;
    vector<float> MSn3;
    vector<float> sectiondata1=sectionFeatures(false,false,x);//第一阶段
    vector<float> sectiondata2=sectionFeatures(false,true,x);//第二阶段
    vector<float> sectiondata3=sectionFeatures(true,true,x);//第三阶段
    vector<float> yna,ye0a; //它们分别存储着三个计算点与一二阶段重心距离;
    float ynb;//第一阶段重心高度(距离底边)
    float myd=(bridge_total_Span*1e3-spanLength)/2;
    ynb=sectiondata1[3];
    {
        vector<float> stage1,stage2,stage3;
        stage1=getSpecifiedAreaMoment(x+myd, false,false);
        stage2=getSpecifiedAreaMoment(x+myd, false,true);
        stage3=getSpecifiedAreaMoment(x+myd, true,true);
        MSn1.push_back( stage1[0]);
        MSn1.push_back(stage2[0]);
        MSn1.push_back( stage3[0]);//a-a截面
        MSn2.push_back( stage1[1]);
        MSn2.push_back(stage2[1]);
        MSn2.push_back( stage3[1]);//b-b截面
        MSn3.push_back( stage1[2]);
        MSn3.push_back(stage2[2]);
        MSn3.push_back( stage3[2]);//c-c截面
        yna.push_back(fabs(sectiondata1[7]-sectiondata1[3]));
        ye0a.push_back(fabs(sectiondata2[7]-sectiondata2[3]));
        yna.push_back(fabs(sectiondata1[3]-sectiondata1[3]));
        ye0a.push_back(fabs(sectiondata2[3]-sectiondata2[3]));
        yna.push_back(fabs(sectiondata1[8]-sectiondata1[3]));
        ye0a.push_back(fabs(sectiondata2[8]-sectiondata2[3]));

  }

float VG1,VG21,VG22,b,NpII,epn,MG1,MG21,MG22,An,In,Ie0,I0,Sn,Se0,sinThetaP,S0;
vector<float> tau;
 vector<float> rho_cx;
vector<float>  sigma_tp,sigma_cp;
 VG1=1e3*shearFoceSolve(getFirstStageLoad(),x/1e3);
 VG21=1e3*shearFoceSolve(getSecondStageLoad(),x/1e3);
 VG22=1e3*shearFoceSolve(getThirdStageLoad(4),x/1e3);
 MG1=1e6*bendingSolve(getFirstStageLoad(),x/1e3);
 MG21=1e6*bendingSolve(getSecondStageLoad(),x/1e3);
 MG22=1e6*bendingSolve(getThirdStageLoad(4),x/1e3);
 sinThetaP=getSinThetaByX(x+myd);
 An=sectiondata1[0];
 In=sectiondata1[1];
 Ie0=sectiondata2[1];
 I0=sectiondata3[1];
 {
     Sn=MSn1[0];
     Se0=MSn1[1];
     S0=MSn1[2];
    float tau1=VG1*Sn/(sectiondata1[7]*In)+VG21*Se0/(sectiondata1[7]*Ie0)+(VG22+Vq)*S0/(sectiondata1[7]*I0)-getSigma_P_II(x+myd,Ap)*get_Apb(x+myd)*sinThetaP*Sn/(sectiondata1[7]*In);
    Sn=MSn2[0];
    Se0=MSn2[1];
    S0=MSn2[2];
    float tau2=VG1*Sn/(sectiondata1[7]*In)+VG21*Se0/(sectiondata1[7]*Ie0)+(VG22+Vq)*S0/(sectiondata1[7]*I0)-getSigma_P_II(x+myd,Ap)*get_Apb(x+myd)*sinThetaP*Sn/(sectiondata1[7]*In);
    Sn=MSn3[0];
    Se0=MSn3[1];
    S0=MSn3[2];
    float tau3=VG1*Sn/(sectiondata1[7]*In)+VG21*Se0/(sectiondata1[7]*Ie0)+(VG22+Vq)*S0/(sectiondata1[7]*I0)-getSigma_P_II(x+myd,Ap)*get_Apb(x+myd)*sinThetaP*Sn/(sectiondata1[7]*In);
    tau.push_back(tau1);
    tau.push_back(tau2);
    tau.push_back(tau3);

 }
 {

     NpII=getSigma_P_II(x+myd,Ap)*get_Apb(myd+x)*sqrt(1-pow(sinThetaP,2))+getSigma_P_II(x+myd,Ap)*(Ap-get_Apb(x+myd))-getSigma_l6(Ap)*As;
     epn=((getSigma_P_II(x+myd,Ap)*get_Apb(x+myd)*sqrt(1-pow(sinThetaP,2))+getSigma_P_II(x+myd,Ap)*(Ap-get_Apb(x+myd)))*(ynb-getAverageSteelHeight(x+myd))-getSigma_l6(Ap)*As*(ynb-as))/NpII;
     rho_cx.push_back(NpII/An-(NpII*epn*yna[0])/In+MG1*yna[0]/In+MG21*ye0a[0]/Ie0+(MG22+Mq)*yna[0]/I0);
     rho_cx.push_back(NpII/An-(NpII*epn*yna[1])/In+MG1*yna[1]/In+MG21*ye0a[1]/Ie0+(MG22+Mq)*yna[1]/I0);
     rho_cx.push_back(NpII/An-(NpII*epn*yna[2])/In+MG1*yna[2]/In+MG21*ye0a[2]/Ie0+(MG22+Mq)*yna[2]/I0);
for(int i=0;i<3;i++){
    sigma_tp.push_back(rho_cx[i]/2-sqrt(pow(rho_cx[i]/2,2)+pow(tau[i],2)));
    sigma_cp.push_back(rho_cx[i]/2+sqrt(pow(rho_cx[i]/2,2)+pow(tau[i],2)));

}
 sigma_cp.insert(sigma_cp.end(),sigma_tp.begin(),sigma_tp.end());




 }

 return sigma_cp;

}
vector<float> beam::getSpecifiedAreaMoment(float x, bool fieldCount, bool preCounted)
{   vector<float> res;
    field_making_girder_beam cacularBeam=getSectionAt( x);
    float mh=sectionFeatures(fieldCount, preCounted, x)[3];
    res=cacularBeam.getSnByh(mh,fieldCount);
    return res;
}
field_making_girder_beam beam::getSectionAt(float x)
{       //单位mm
    //第一个参数 总跨长 第二个  请求截面位置      第三个参数 过渡起点坐标 第四个 过渡终点坐标
 //过渡段起点 终点以米计
    bool isSideBeam=(middleSetion.sbg.left.isSideBeam|middleSetion.sbg.right.isSideBeam);
    float halfLength=half_Trans_end*1000-half_Trans_start*1000;
    float xe=x>spanLength/2? spanLength-x:x;
    if(xe<=1000*half_Trans_start){
     xe=0;
        }else
    {
        if(xe<1000*half_Trans_end)
        {
         xe=xe-1000*half_Trans_start;
        }else
        {
        xe=halfLength;
        }
        }
    float field_length1;
    float field_length2;
    vector<float> half_left;//指定截面左边半箱梁参数
    vector<float> half_right;//指定截面右边半箱梁参数
    vector<float> fulc_left;
    vector<float> fulc_right;
    vector<float> mid_left;
    vector<float> mid_right;
     half_box_girder myhbg1;
    half_box_girder myhbg2;
    {
     //
      fulc_left=pivotSetion.sbg.left.getInfo();
      fulc_right=pivotSetion.sbg.right.getInfo();
      mid_left=middleSetion.sbg.left.getInfo();
      mid_right=middleSetion.sbg.right.getInfo();
      field_length1=pivotSetion.fmgb_length;
      field_length2=middleSetion.fmgb_length;


    }
    for(int i=0;i<fulc_left.size();i++){

      float temp1;
      float temp2;
      temp1=fulc_left[i]+(mid_left[i]-fulc_left[i])*(xe/halfLength);
      temp2=fulc_right[i]+(mid_right[i]-fulc_right[i])*(xe/halfLength);
      half_left.push_back(temp1);
      half_right.push_back(temp2);


    }

    if(isSideBeam){
        myhbg1=half_box_girder(half_left,true);
        myhbg2= half_box_girder (half_right,false);

    }else{
        myhbg1=half_box_girder(half_left,false);
        myhbg2= half_box_girder (half_right,false);

    }
    float fl=field_length1+(field_length2-field_length1)*(xe/halfLength);
    small_box_girder mysbg(myhbg1,myhbg2);
   field_making_girder_beam fmgb(mysbg,fl);
   return fmgb;

}
vector<float> beam::getSectionInfo(float x, bool fieldCount)
{       //返回以mm为基本单位的数值;
        float AreaSum;
        float smoaSuam;
        float staticMoment;
        float centerHeight;
        vector<float> result;
      field_making_girder_beam processBeam=getSectionAt(x);
          if(fieldCount)
          {
           AreaSum=processBeam.Area;
           smoaSuam=processBeam.SmoaSlove();
           staticMoment=processBeam.StaticMoment;
           centerHeight=processBeam.CentroidHeight;

          }else
          {
              AreaSum=processBeam.sbg.AreaSolve();
              centerHeight=processBeam.sbg.CentroidHeightSolve();
              smoaSuam=processBeam.sbg.SmoaSlove( centerHeight);
              staticMoment=processBeam.sbg.StaticMomentSolve();

          }
            result.push_back(AreaSum*1e6);
            result.push_back(smoaSuam*1e12);
            result.push_back(staticMoment*1e9);
            result.push_back(centerHeight*1e3);//yb
           // result.push_back(fabs(1e3*centerHeight-getAverageSteelHeight(x)));//wp
            result.push_back(1e3*processBeam.sbg.left.h);//h
            result.push_back((processBeam.sbg.left.h-processBeam.sbg.left.d1-processBeam.sbg.left.h4)*1e3);
            result.push_back((processBeam.sbg.left.d3+processBeam.sbg.left.h6)*1e3);
            result.push_back((processBeam.sbg.left.d2*2)*1e3);
            return result;
}
vector<float> beam::sectionFeatures(bool fieldCount, bool precounted, float x)
{   //x是以支点为零点的坐标
    //布尔值1表示是否计入现浇段
    //布尔值2true表示中梁,false表示边梁
    //布尔值三表示是否计入预应力筋;
    //面积
    //惯性矩
    //静弯
    //重心
    //kx
    //ks
    // 效率指标
    //返回的是计入非预应力钢筋和预留管道的几何特性;
    float myd=(bridge_total_Span*1e3-spanLength)/2;
    float AreaSum;
     float smoaSuam;
     float staticMoment;
     float centerHeight;
     vector<float> respon=getSectionInfo(x,fieldCount);
     vector<float> result;
    AreaSum=respon[0]-getPipAreaSum(precounted)+non_prestreSteel_s;
    staticMoment=respon[2]-getPipStaticMomentSum(x+myd,precounted)+non_prestreSteel_s*non_prestrepip_center;
    centerHeight=staticMoment/AreaSum;
    smoaSuam=respon[1]+respon[0]*pow(respon[3]-centerHeight,2)-getPipSmoaSum(x+myd,centerHeight,precounted)+non_prestreSteel_s*pow(non_prestrepip_center-centerHeight,2);
    result.push_back(AreaSum);
    result.push_back(smoaSuam);
    result.push_back(staticMoment);
    result.push_back(centerHeight);//yb
    result.push_back(fabs(centerHeight-getAverageSteelHeight(x+myd)));//ep
    result.push_back(smoaSuam/centerHeight);//wb
    result.push_back(smoaSuam/(respon[4]-centerHeight));//wu
    result.push_back(respon[5]);
    result.push_back(respon[6]);
    result.push_back(respon[7]);

    return result;
}
vector<float> beam::getSigma_ct(float x)
{
    float myd=(bridge_total_Span*1e3-spanLength)/2;
    float N_pI=getSigma_P_I(x+myd,Ap)*Ap;
    float MG1=1e6*bendingSolve(getFirstStageLoad(),x/1e3);
    vector<float> sectionInfo=sectionFeatures(false, false, x);
    float An=sectionInfo[0];float epn=sectionInfo[4];float Wnu=sectionInfo[6];float Wnb=sectionInfo[5];
    vector<float> res;
    res.push_back((N_pI/An)-(N_pI*epn/Wnu)+MG1/Wnu);
    res.push_back((N_pI/An)+(N_pI*epn/Wnb)-MG1/Wnb);
    return res;//fck c45 29.6
}
float beam::steelAreaSolve(float ms,float myfpk,float myap){

//计算预应力筋面积;
    float beamArea;
    //重心位置
    float centerHeight;
    //惯性矩
    float smoa;
    float ep;
    float W;
  vector<float> datas=getSectionInfo(spanLength/2, true);


    beamArea=datas[0];
    centerHeight=datas[3];
    smoa=datas[1];
    ep=centerHeight-myap;
    W=smoa/centerHeight;
    float npe=(ms*pow(10,6)/W-0.7*2.65)/(1/beamArea+ep/W);
   float as=npe/((1-0.2)*0.75*myfpk);
    return as;

}
float beam::unPreSteelAreaSlove(float Md,float a, float myfpd, float myAp, float myfsd)
{   //非预应力钢筋面积求解;
    //弯矩的基本组合
    float fsd=myfsd;
    float Fpd=myfpd;
    float ap=myAp;
    float bf;
    float  m=Md;
    float h0;
    float param1;
    float param2;
    float param3;
    float root;
    float field=(middleSetion.sbg.left.isSideBeam|middleSetion.sbg.right.isSideBeam)? middleSetion.fmgb_length:2*middleSetion.fmgb_length;
    bf=middleSetion.sbg.left.b1+middleSetion.sbg.right.b1+field;
    bf=1e3*bf;
    h0=1000*middleSetion.sbg.left.h-a;
     param1=-0.5*22.4*(bf);
     param2=param1*(-2)*h0;
     param3=-m*pow(10,6);
    if(pow(param2,2)>=4*param1*param3){
      float temp1=(-param2+sqrt(pow(param2,2)-4*param1*param3))/(2*param1);
      float temp2=(-param2-sqrt(pow(param2,2)-4*param1*param3))/(2*param1);

      if(0<temp1&temp1<middleSetion.sbg.left.d1*1000){
          root=temp1;
      }
      if(0<temp2&temp2<middleSetion.sbg.left.d1*1000){
        root=temp2;
      }
    }
float as=0;
 as=(22.4*bf*root-Fpd*ap)/fsd;

 return as;
}
float beam::getStageDieLoad(int stage, float x, bool foceType)
{
     //foceType true表示弯矩 false表示剪力;
        x/=1e3;//输入毫米转为米
        float g;
        switch (stage) {
        case 0:
          g=getFirstStageLoad()+getSecondStageLoad()+getThirdStageLoad(numberOfBeam);
            break;
        case 1:
          g=getFirstStageLoad();
            break;
        case 2:
            g=getSecondStageLoad();

            break;
        case 3:
            g=getThirdStageLoad(numberOfBeam);

            break;
        default:
            g=getFirstStageLoad()+getSecondStageLoad()+getThirdStageLoad(numberOfBeam);
            break;
 }
        if(foceType){
            //弯矩
            return bendingSolve(g,x);
        }else{
           //剪力
           return shearFoceSolve(g,x);
        }


}
