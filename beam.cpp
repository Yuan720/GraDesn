#include "beam.h"

beam::beam()
{
qDebug()<<"hahha";
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
   MG2.push_back(1e6*bendingSolve(getSecondStageLoad(),spanLength/(2*1000)));
   MG2.push_back(1e6*bendingSolve(getSecondStageLoad(),spanLength/(4*1000)));

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
                res+=steelPaths[i].steelArea;
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
               staticMomt+=steelPaths[i].getYvalue(x)*steelPaths[i].steelArea;
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
           smoa+=pow(steelPaths[i].getYvalue(x)-newCenterHeight,2)*steelPaths[i].steelArea;
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
field_making_girder_beam beam::getSectionAt(float x)
{
    //第一个参数 总跨长 第二个  请求截面位置      第三个参数 过渡起点坐标 第四个 过渡终点坐标
 //过渡段起点 终点以米计
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

    if(!beamtype){
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
            result.push_back(centerHeight*1e3);
            result.push_back(fabs(1e3*centerHeight-getAverageSteelHeight(x)));
            return result;
}
vector<float> beam::sectionFeatures(bool fieldCount, bool precounted, float x)
{
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
    float AreaSum;
     float smoaSuam;
     float staticMoment;
     float centerHeight;
     vector<float> respon=getSectionInfo(x,fieldCount);
     vector<float> result;
    AreaSum=respon[0]-getPipAreaSum(precounted)+non_prestreSteel_s;
    staticMoment=respon[2]-getPipStaticMomentSum(x,precounted)+non_prestreSteel_s*non_prestrepip_center;
    centerHeight=staticMoment/AreaSum;
    smoaSuam=respon[1]+respon[0]*pow(respon[3]-centerHeight,2)-getPipSmoaSum(x,centerHeight,precounted)+non_prestreSteel_s*pow(non_prestrepip_center-centerHeight,2);
    result.push_back(AreaSum);
    result.push_back(smoaSuam);
    result.push_back(staticMoment);
    result.push_back(centerHeight);
    float temp=fabs(centerHeight-getAverageSteelHeight(x));
    result.push_back(fabs(centerHeight-getAverageSteelHeight(x)));
    return result;
}


