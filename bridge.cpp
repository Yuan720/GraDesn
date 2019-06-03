#pragma execution_character_set("utf-8")
#include<bridge.h>
half_box_girder::half_box_girder(vector<float> v,bool type){
    h = v[12]; d1 = v[0]; d2 = v[1]; d3 = v[2]; b1 = v[3]; b2 = v[4]; b4 = v[5]; h4 = v[6]; b5 = v[7]; h5 = v[8]; b6 = v[9]; h6 = v[10]; p = v[11]; isSideBeam = type;
    rec1 = Rectangular(b1, d1, (h - d1 / 2));
    rec2 = Rectangular(d2*sqrt(pow(1 / p, 2) + 1), h4, (h - d1 - h4 / 2));
    rec3 = Rectangular((b2 + (d3 + h6) / p - d2 * sqrt(pow(1 / p, 2) + 1)), d3, d3 / 2);
    trangle1 = Triangle(b4, h4, h - h4 / 3 - d1);
    trangle2 = Triangle(b5, h5, h - h5 / 3 - d1);
    trangle3 = Triangle(d2*sqrt(pow(1 / p, 2) + 1), h - d1 - h4 - d3 - h6, h - d1 - h4 - (h - d1 - h4 - d3 - h6) / 3);
    trangle4 = Triangle(d2*sqrt(pow(1 / p, 2) + 1), h - d1 - h4 - d3 - h6, h - d1 - h4 - (h - d1 - h4 - d3 - h6) * 2 / 3);
    trangle5 = Triangle(d2*sqrt(pow(1 / p, 2) + 1), d3 + h6, 0.66666 * (d3 + h6));
    trangle6 = Triangle(d2*sqrt(pow(1 / p, 2) + 1) - (d3 + h6) / p, (d3 + h6), (d3 + h6) / 3);
    trangle7 = Triangle(b6, h6, (d3 + h6 / 3));


}
half_box_girder::half_box_girder(float vh, float vd1, float vd2, float vd3, float vb1, float vb2, float vb4, float vh4, float vb5, float  vh5, float vb6, float vh6, float vp, bool type)
{
	h = vh; d1 = vd1; d2 = vd2; d3 = vd3; b1 = vb1; b2 = vb2; b4 = vb4; h4 = vh4; b5 = vb5; h5 = vh5; b6 = vb6; h6 = vh6; p = vp; isSideBeam = type;
	rec1 = Rectangular(b1, d1, (h - d1 / 2));
	rec2 = Rectangular(d2*sqrt(pow(1 / p, 2) + 1), h4, (h - d1 - h4 / 2));
	rec3 = Rectangular((b2 + (d3 + h6) / p - d2 * sqrt(pow(1 / p, 2) + 1)), d3, d3 / 2);
	trangle1 = Triangle(b4, h4, h - h4 / 3 - d1);
	trangle2 = Triangle(b5, h5, h - h5 / 3 - d1);
	trangle3 = Triangle(d2*sqrt(pow(1 / p, 2) + 1), h - d1 - h4 - d3 - h6, h - d1 - h4 - (h - d1 - h4 - d3 - h6) / 3);
	trangle4 = Triangle(d2*sqrt(pow(1 / p, 2) + 1), h - d1 - h4 - d3 - h6, h - d1 - h4 - (h - d1 - h4 - d3 - h6) * 2 / 3);
	trangle5 = Triangle(d2*sqrt(pow(1 / p, 2) + 1), d3 + h6, 0.66666 * (d3 + h6));
	trangle6 = Triangle(d2*sqrt(pow(1 / p, 2) + 1) - (d3 + h6) / p, (d3 + h6), (d3 + h6) / 3);
	trangle7 = Triangle(b6, h6, (d3 + h6 / 3));


}
float half_box_girder::getAreaSum() {
	return rec1.Area + rec2.Area + rec3.Area + trangle1.Area + trangle2.Area + trangle3.Area + trangle4.Area + trangle5.Area + trangle6.Area + trangle7.Area;
}
float half_box_girder::getSMOASum(float yc) {
	return rec1.getSMOA(yc) + rec2.getSMOA(yc) + rec3.getSMOA(yc) + trangle1.getSMOA(yc) + trangle2.getSMOA(yc) + trangle3.getSMOA(yc) + trangle4.getSMOA(yc)
		+ trangle5.getSMOA(yc) + trangle6.getSMOA(yc) + trangle7.getSMOA(yc);
}
float half_box_girder::getStaticMomentSum() {
	return rec1.getStaticMoment() + rec2.getStaticMoment() + rec3.getStaticMoment() + trangle1.getStaticMoment() + trangle2.getStaticMoment()
		+ trangle3.getStaticMoment() + trangle4.getStaticMoment() + trangle5.getStaticMoment() + trangle6.getStaticMoment() + trangle7.getStaticMoment();
}
float half_box_girder::getCentroidHeight() {
	return getStaticMomentSum() / getAreaSum();
}
small_box_girder::small_box_girder(half_box_girder vleft, half_box_girder vright) {
	left = vleft;
	right = vright;

}
float small_box_girder::SmoaSlove(float yc) {
	return left.getSMOASum(yc) + right.getSMOASum(yc);
}
float small_box_girder::CentroidHeightSolve() {
	return  StaticMomentSolve() / AreaSolve();
}
float small_box_girder::StaticMomentSolve() {
	return (left.getStaticMomentSum() + right.getStaticMomentSum());
}
float small_box_girder::AreaSolve() {
	return (left.getAreaSum() + right.getAreaSum());
}
float small_box_girder::SolveKs() {
	return SmoaSlove(CentroidHeightSolve()) / (AreaSolve()*(left.h - CentroidHeightSolve()));
}
float small_box_girder::SolveKx() {
	return SmoaSlove(CentroidHeightSolve()) / (AreaSolve()*(CentroidHeightSolve()));
}
float small_box_girder::getRho() {
	return (SolveKs() + SolveKx()) / left.h;
}
bool small_box_girder::isValid() {
	return getRho() > 0.5;
}
Triangle::Triangle(float vbottomLong, float vHeight, float vTSHD) {
	bottomLong = vbottomLong;
	Height = vHeight;
	TSHD = vTSHD;
	Area = bottomLong * Height*0.5;
};
float Triangle::getStaticMoment() {

	return TSHD * bottomLong*Height*0.5;
}
float Triangle::getSMOA(float yc) {
	return bottomLong * pow(Height, 3) / 36 + Area * pow(fabs(TSHD - yc), 2);
};
Rectangular::Rectangular(float vLength, float vHeight, float vRSHD) {
	Length = vLength;
	Height = vHeight;
	RSHD = vRSHD;
	Area = Length * Height;
};
float Rectangular::getStaticMoment() {
	return Length * Height*RSHD;
};
float Rectangular::getSMOA(float yc) {
	return Length * pow(Height, 3) / 12 + Area * pow(fabs(RSHD - yc), 2);
};
field_making_girder_beam::field_making_girder_beam(small_box_girder mysbg, float fmgb) {
	{
		sbg = mysbg;
		fmgb_length = fmgb;
		StaticMoment = mysbg.StaticMomentSolve();
		Area = mysbg.AreaSolve();
		if (!mysbg.left.isSideBeam)
		{
			Area += mysbg.left.d1*fmgb / 2;
			StaticMoment += (mysbg.left.h - mysbg.left.d1 / 2)*mysbg.left.d1*fmgb / 2;

		}
		if (!mysbg.right.isSideBeam)
		{
			Area += mysbg.right.d1*fmgb / 2;
			StaticMoment += (mysbg.right.h - mysbg.right.d1 / 2)*mysbg.right.d1*fmgb / 2;
		}
		CentroidHeight = StaticMoment / Area;


	}
}
float field_making_girder_beam::SmoaSlove() {
	float temp;
	temp = sbg.SmoaSlove(CentroidHeight);
	if (!sbg.left.isSideBeam)
	{
		Rectangular myrec(fmgb_length / 2, sbg.left.d1, sbg.left.h - sbg.left.d1 / 2);
		temp += myrec.getSMOA(CentroidHeight);

	}
	if (!sbg.right.isSideBeam)
	{
		Rectangular myrec(fmgb_length / 2, sbg.right.d1, sbg.right.h - sbg.right.d1 / 2);
		temp += myrec.getSMOA(CentroidHeight);
	}
	return temp;
}
float field_making_girder_beam::SolveKs() {
	return  SmoaSlove() / (Area *(sbg.left.h - CentroidHeight));

}
float field_making_girder_beam::SolveKx() {
	return  SmoaSlove() / (Area * CentroidHeight);
}
float field_making_girder_beam::getRho() {
	return (SolveKs() + SolveKx()) / sbg.left.h;
}
bool field_making_girder_beam::isValid() {
    return getRho() > 0.5;
}

vector<float> field_making_girder_beam::getSnByh(float mh, bool fieldCount)
{ vector<float> leftdata=sbg.left.getSnByh(mh);
  vector<float> rightdata=sbg.right.getSnByh(mh);
  vector<float> res;
 float field_moment=0;
  bool isSideBeam=(sbg.left.isSideBeam|sbg.right.isSideBeam);
  field_moment=fmgb_length*sbg.left.d1*(sbg.left.h-sbg.left.d1/2-mh);
  if(fieldCount){
      if(!isSideBeam){

          field_moment*=2;
      }
      }
  for(int i=0;i<leftdata.size();i++){

      res.push_back(leftdata[i]+rightdata[i]+field_moment);
  }
  return res;


}
float small_box_girder::torsionalMomentInertiaSolve() {
	float tmith = left.h - left.d1 / 2 - left.d3 / 2;
	float *leftMidle = left.getMidleLineS();
	float *rightMidle = right.getMidleLineS();
	float bottomMidleLine = *leftMidle + *rightMidle;
	float leftWebs = *(leftMidle + 1);
	float rightWebs = *(rightMidle + 1);
	float roofMidleLine = *(leftMidle + 2) + *(rightMidle + 2);
	float area = (bottomMidleLine + roofMidleLine)*(left.h - left.d1 / 2 - left.d3 / 2) / 2;
	float btSum = bottomMidleLine / left.d3 + leftWebs / left.d2 + rightWebs / right.d2 + roofMidleLine / left.d1;
	float a[10] = { 0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0 };
	float b[10] = { 0.312,0.291,0.270,0.250,0.229,0.209,0.189,0.171,0.155,0.141 };
	BasicInterpolationTable  cacular(a, b, 10);
	float bt3c1 = cacular.Interpolat(left.d1 / (*(leftMidle + 3)))*pow(left.d1, 3)*(*(leftMidle + 3));
	float bt3c2 = cacular.Interpolat(right.d1 / (*(rightMidle + 3)))*pow(right.d1, 3)*(*(rightMidle + 3));

	return bt3c1 + bt3c2 + 4 * pow(area, 2) / btSum;

}
float * half_box_girder::getMidleLineS() {
	float *a = new float[4];
	a[0] = b2 + d3 / (2 * p) - d2 / 2 * sqrt(1 + pow((1 / p), 2));
	a[1] = (h - (d1 + d3) / 2)*sqrt(1 + pow((1 / p), 2));
	a[2] = a[0] + a[1] * (1 / sqrt(1 + pow(p, 2)));
	float temp = b1 - (b2 + (h - d1 - h4) / p + b4);
	a[3] = (d1*(temp + b4) + 0.5*b4*h4) / d1;
	return a;
}
//返回l0和单向板等效厚度;
float * field_making_girder_beam::getSpanLength() {
	float span1 = 2 * (sbg.left.b2 + (sbg.left.h - sbg.left.d1 - sbg.left.h4) / sbg.left.p) - (sbg.left.d2 / 2)*sqrt(1 + pow(1 / sbg.left.p, 2)) - (sbg.right.d2 / 2)*sqrt(1 + pow(1 / sbg.right.p, 2));
	float span2 = (sbg.left.b1 - (sbg.left.h - sbg.left.d1 - sbg.left.h4) / sbg.left.p - sbg.left.b2) * 2 + (sbg.left.d2 / 2)*sqrt(1 + pow(1 / sbg.left.p, 2)) + (sbg.right.d2 / 2)*sqrt(1 + pow(1 / sbg.right.p, 2)) + fmgb_length;
	float *t = new float[3];
	if (span1 >= span2) {
		float eq_area = span1 * sbg.left.d1 + (sbg.left.d2 / 2)*sqrt(1 + pow(1 / sbg.left.p, 2))*sbg.left.h5 + 0.5*sbg.left.h5*sbg.left.b5 + (sbg.right.d2 / 2)*sqrt(1 + pow(1 / sbg.right.p, 2))*sbg.right.h5 + 0.5*sbg.right.h5*sbg.right.b5;
		t[0] = span1;
		t[1] = eq_area;
		t[2] = eq_area / span1;
	}
	else {
		t[0] = span2;
		float eq_area = ((span2 - fmgb_length) / 2 * sbg.left.d1 + 0.5*sbg.left.h4*sbg.left.b4 + (sbg.left.d2 / 2)*sqrt(1 + pow(1 / sbg.left.p, 2))*sbg.left.h4) * 2 + fmgb_length * sbg.left.d1;
		t[1] = eq_area;
		t[2] = eq_area / span2;
	}



	return t;


}
float * half_box_girder::getCantileverPlate_info() {
	float *result = new float[2];
	if (isSideBeam)
	{
		result[0] = b4;
		result[1] = d1 + h4 / 2;
	}
	else {
		result[0] = 0;
		result[1] = 0;
	}
	return result;

}
vector<float> half_box_girder::getInfo(){
   vector<float>  myres;
   myres.push_back(d1);
   myres.push_back(d2);
   myres.push_back(d3);
   myres.push_back(b1);
   myres.push_back(b2);
   myres.push_back(b4);
   myres.push_back(h4);
   myres.push_back(b5);
   myres.push_back(h5);
   myres.push_back(b6);
   myres.push_back(h6);
   myres.push_back(p);
    myres.push_back(h);
    return myres;

}

vector<float> half_box_girder::getSnByh(float mh)
{  //结果以米为单位;
   //指定重心高度后的特征点的上半部分面积矩求解;
    //返回----1.截面上肋梗上部面积矩2.重心以上面积矩 3.截面下肋梗上部面积矩
     mh/=1e3;
    vector<float> res;
    float sn1=rec1.Area*(rec1.RSHD-mh)+rec2.Area*(rec2.RSHD-mh)+trangle1.Area*(trangle1.TSHD-mh)+trangle2.Area*(trangle2.TSHD-mh);
   float test1= rec1.Area*(rec1.RSHD-mh);float test2=rec1.Area ;float test3=rec1.RSHD ;float test4=rec1.RSHD-mh;
    res.push_back(1e9*sn1);
    float ed2=d2*sqrt(1+pow(1/p,2));
    float sh1=h-d1-h4-mh;
    float sn2=sn1+(ed2*sh1)*sh1/2;
    res.push_back(1e9*sn2);
    float sh2=h-d1-h4-d3-h6;
    float sn3=sn1+(ed2*sh2)*(d3+h6+sh2/2-mh);
    res.push_back(1e9*sn3);
     return res;




}
vector<float> half_box_girder::getDivided()
{
    float num1 = b1 - b2 - (h - d1 - h4) / 4+ d2 * sqrt(1 + pow((1 / p), 2))/2;
    float num2 = b1 - num1;
    vector<float> res;
    res.push_back(num1);
    res.push_back(num2);
    return res;
}
void OrdinaryBrigeSection::InitFsm(){
  float num1=side_Fulcr_bean->sbg.left.getDivided()[0];
  float num2=side_Fulcr_bean->sbg.left.getDivided()[1]+side_Fulcr_bean->sbg.right.getDivided()[1];
  float num3= side_Fulcr_bean->sbg.right.getDivided()[0]+side_Fulcr_bean->fmgb_length+mid_Fulcr_bean->fmgb_length+mid_Fulcr_bean->sbg.left.getDivided()[0];
  float num4=mid_Fulcr_bean->sbg.left.getDivided()[1]+mid_Fulcr_bean->sbg.right.getDivided()[1];
  float num5=(mid_Fulcr_bean->sbg.left.getDivided()[0]+mid_Fulcr_bean->fmgb_length)*2;
  fsm.s1=num1;fsm.s2=num2;fsm.s3=num3;fsm.s4=num4;fsm.s5=num5;

}


float OrdinaryBrigeSection::get_Mr(float px, float s)
{
    int lf=getLeftBeamId(s);
    float m=0;
    if(lf>=0){
        for(int i=1;i<=lf;i++){
           m+=get_R(i,px-total_span/2)*(s-aix[i-1]);
        }
    }
    if(px<=s){
        m-=1*(s-px);
    }
    return m;


}

float OrdinaryBrigeSection::get_Vr(float px, float s)
{
    int lf=getLeftBeamId(s);
        float V=0;
        if(lf>=0){
            for(int i=1;i<=lf;i++){
               V+=get_R(i,px-total_span/2);
            }
        }
        if(px<=s){
            V-=1;
        }
        return V;

}

int OrdinaryBrigeSection::getLeftBeamId(float s)
{   int l=0;
    for(int i=0;i<aix.size();i++)
    {
        if(aix[i]<s){
         l=i+1;

        }else{
            return l;
        }
    }
    return l;
}
//横隔梁
float OrdinaryBrigeSection::get_R(int i,float e)
{   //i 梁号 从1开始;
    field_making_girder_beam calbeam=(i==bean_nums|i==1)? side_main_bean:mid_main_bean;
    float Isum=2*side_main_bean.SmoaSlove()+(bean_nums-2)*mid_main_bean.SmoaSlove();//惯性矩之和 基本单位为m
    float I=calbeam.SmoaSlove();
    vector<float> AiI2=get_aid_moif();
    float temp=getVectorSum(AiI2, 1, AiI2.size());
    float result=I/Isum+I*aid[i-1]*e/(temp);
    return result;
    //I/Isum+I*aid[i-1]*e/(temp);
}

float OrdinaryBrigeSection::get_R(int i, int j)
{ //梁号以一开始下标一表示求解的梁,下标2表示力的作用梁号;
    return get_R(i ,aid[j-1]);
}



vector<float> OrdinaryBrigeSection::getVr_At(float s)
{//s截面位置;

   vector<float> res;
   float valueTosave=0;
 int carloads=getMaximumVehicle();
    for(int i=1;i<=carloads;i++){
       vector<float> e=getEccentricDistances(i);

        float step=0;
     while(e[2*i-1]+step<total_span-stoneWidth-0.5)
       {    float temp=0;
           for(int j=1;j<2*i;j++){
               temp+=get_Vr(e[j]+step, s);
           }

             valueTosave=temp>valueTosave? temp:valueTosave;

           step+=0.1;

       }
       res.push_back(valueTosave*CoefficientloadArrange[i-1]);
       valueTosave=0;
    }
return res;

}

vector<float> OrdinaryBrigeSection::getMr_At(float s,bool symb)
{
    vector<float> res;
     float valueTosave=0;
    if(!symb){
     valueTosave= get_Mr(stoneWidth+0.5, s) + get_Mr(stoneWidth+0.5+1.8, s)+get_Mr(total_span-stoneWidth-0.5, s)+get_Mr(total_span-stoneWidth-0.5-1.8, s);
    res.push_back(valueTosave*CoefficientloadArrange[1]);
    return res;
    }

  int carloads=getMaximumVehicle();
     for(int i=1;i<=carloads;i++){
        vector<float> e=getEccentricDistances(i);

         float step=0;

        while(e[2*i-1]+step<total_span-stoneWidth-0.5)
        {    float temp=0;
            for(int j=1;j<2*i;j++){
                temp+=get_Mr(e[j]+step, s);
            }

              valueTosave=temp>valueTosave? temp:valueTosave;

            step+=0.1;

        }
        res.push_back(valueTosave*CoefficientloadArrange[i-1]);
        valueTosave=0;
     }
     return res;
}


float OrdinaryBrigeSection::cross_bending(float s, bool symb)
{ float temp=0;
  vector<float> M=getMr_At(s,symb);
  if(M.size()==1){
      temp=M[0];
  }else{
      for(int i=0;i<M.size();i++){
          temp=M[i]>temp? M[i]:temp;
      }
  }

  return 1.1*get_Poq(5)[0]*temp;


}

float OrdinaryBrigeSection::cross_sf(float s)
{
  float temp=0;
  vector<float> M=getVr_At(s);
  for(int i=0;i<M.size();i++){
      temp=M[i]>temp? M[i]:temp;
  }


  return 1.1*get_Poq(5)[1]*temp;

}

void OrdinaryBrigeSection::cross_storge()
{
    cross_beam_foce[0][0]=cross_bending(aix[1]+0.01,true);
    cross_beam_foce[0][1]=cross_bending(aix[1]+0.01,false);
    cross_beam_foce[0][2]=cross_bending(total_span/2,true);
    cross_beam_foce[0][3]=cross_bending(total_span/2,false);
    cross_beam_foce[0][4]=cross_sf(aix[0]+0.01);
    cross_beam_foce[0][5]=cross_sf(aix[1]+0.01);

        for(int j=0;j<6;j++){
          cross_beam_foce[1][j]=cross_beam_foce[0][j]*vmcffe();
          cross_beam_foce[2][j]=cross_beam_foce[0][j]*1.4*vmcffe() ;
          cross_beam_foce[3][j]=cross_beam_foce[0][j]*0.7 ;
          cross_beam_foce[4][j]=cross_beam_foce[0][j]*0.4 ;

   }





}

float OrdinaryBrigeSection::get_e_value(int VehicleNum)
{       vector<float> e=getEccentricDistances(VehicleNum);
        float esum=0;
            for(int i=0;i<e.size();i++){
                 esum+=e[i];

            }


return 0.5*total_span-esum/e.size();

}

float OrdinaryBrigeSection::PartialLoadMcq(int beamId, int VehicleNum)
{ //偏压法求下部结构横向分布系数;

    float e=get_e_value(VehicleNum);
   float mcq=getPerTireLoad(beamId,e)*CoefficientloadArrange[VehicleNum - 1]*VehicleNum;
   return mcq;


}


vector<float> OrdinaryBrigeSection::getCenterLoadArrange(int VehicleNum)
{

    if (VehicleNum>getMaximumVehicle()){
    qDebug()<<"布载系数超出最大布载值";

    }

vector<float> res;
res=E_val_bulder(false,VehicleNum);

/*if(VehicleNum==1){
    res.push_back(total_span/2-0.9);
      res.push_back(total_span/2+0.9);
}
if(VehicleNum==2){
    res.push_back(total_span/2-0.65);
      res.push_back(total_span/2+0.65);
       res.push_back(res[0]-1.8);
        res.push_back(res[1]+1.8);

}
if(VehicleNum==3){
    res.push_back(total_span/2-0.9);
      res.push_back(total_span/2+0.9);
       res.push_back(res[0]-1.3);
        res.push_back(res[0]-1.3-1.8);
        res.push_back(res[1]+1.3);
         res.push_back(res[1]+1.3+1.8);
}*/

return res;


}

float OrdinaryBrigeSection::getCenterLoadMcq(int beamId, int VehicleNum)
{        BasicInterpolationTable valSolver;
         //fsm3==fsm5;
    vector<float> val;
    val.push_back(0);
    val.push_back(fsm.s1);
    val.push_back(fsm.s1+fsm.s2);
    val.push_back(fsm.s1+fsm.s2+fsm.s3);
    val.push_back(fsm.s1+fsm.s2+fsm.s3+fsm.s4);
    val.push_back(fsm.s1+fsm.s2+fsm.s3+fsm.s4+fsm.s5);
        val.push_back(fsm.s1+fsm.s2+fsm.s3+2*fsm.s4+fsm.s5);
        val.push_back(fsm.s1+fsm.s2+2*fsm.s3+2*fsm.s4+fsm.s5);
         val.push_back(fsm.s1+2*fsm.s2+2*fsm.s3+2*fsm.s4+fsm.s5);
         val.push_back(total_span);
         if(beamId>bean_nums|beamId<1){
         qDebug()<<"非法计算请求!";
         }
              if( beamId==1){
             float c[4]={val[0],val[2],val[3],val[9]};
             float d[4]={1,1,0,0};
             valSolver.update(c,d,4);


              }else{

                  if(beamId==bean_nums){
                      float c[4]={val[0],total_span- val[3],total_span- val[2],val[9]};
                    float d[4]={0,0,1,1};
                    valSolver.update(c,d,4);

                  }
                  else
                  { float gap=fsm.s3+fsm.s4;
                      float offset=(beamId-2)*gap;
                      float c[6]={val[0],val[2]+offset,val[3]+offset,val[4]+offset,val[5]+offset,val[9]+offset};
                        float d[6]={0,0,1,1,0,0};
                      valSolver.update(c,d,6);

                  }


              }




         float result=0;

        vector<float> evals=getCenterLoadArrange(VehicleNum);
        for(int i=0;i<evals.size();i++){

            float test=valSolver.Interpolat(evals[i]);;

            result+=valSolver.Interpolat(evals[i]);

        }
        return 0.5*result*CoefficientloadArrange[VehicleNum - 1];

}

vector<float> OrdinaryBrigeSection::E_val_bulder(bool Type, int VehicleNum)
{
    //type true 偏载;false 中载;
        int esum=VehicleNum*2;
        vector<float> eval;
        vector<float> res;
        float offset;//偏移值

        for(int i=1;i<=esum;i++){
            int tgap=i/2;
            int cgap=(i-1)/2;
            eval.push_back(1.8*tgap+cgap*1.3);

        }

        if(Type){
          //偏载
          offset=stoneWidth+0.5;

        }else{
            //中载
             float symtl=(eval[0]+eval[eval.size()-1])/2;//对称线位置
            offset=total_span/2-symtl;

        }
        for(int i=0;i<eval.size();i++){
            res.push_back(eval[i]+offset);
        }
        return res;

}

vector<float> OrdinaryBrigeSection::get_Poq(int corss_bean_num)
{
    vector<float> res;

    float Poq1=0.5*pk*1+0.5*qk*0.5*cal_span/(corss_bean_num-1)*2;
     float Poq2=0.5*1.2*pk*1+0.5*qk*0.5*cal_span/(corss_bean_num-1)*2;
     res.push_back(Poq1);
      res.push_back(Poq2);

return res;
}


void Coping::focePointInit()
{    LcaVal.clear();
    for(int i=0;i<beamLca.size();i++){

        LcaVal.push_back(beamLca[i]-Zx/2);
         LcaVal.push_back(beamLca[i]+Zx/2);
    }
}

float Coping::getHeightSum()
{
    float G=(2*SideBeamLoad+(beamNum-2)*MidBeamLoad)*Lc;
    return G;

}

vector<float> Coping::getSupReaction()
{
    vector<float> res;
    res.push_back(SideBeamLoad*Lc*0.5);
    res.push_back(MidBeamLoad*Lc*0.5);
    return res;
}

float Coping::getCodeByX(float x)
{
    float c[4]={0,l1-d*0.5-bt,l-(l1-d*0.5-bt),l};
    float d[4]={dt,h0,h0,dt};
     BasicInterpolationTable dSolver= BasicInterpolationTable(c,d,4);
     float hd=dSolver.Interpolat(x);
     return hd*bh*25;


}

float Coping::getSideCodeByX()
{
    return dt*bh*25;//个人认为不计入上部比较精确;
}

float Coping::getGh()
{ float g=getCodeByX(l/2);
  float ge=getCodeByX(0);
  return g*(l2+d+2*bt)+(g+ge)*0.5*(l1-0.5*d-bt)*2;
  //返回盖梁总重;

}

float Coping::CoopingBendingSolve(float x)
{  x=x<l/2? x:l-x;
    if(x<(l1-0.5*d-bt)){
      // 如果所求截面在变化段;
        float ge=getCodeByX(0);
         float g1=getCodeByX(x);
         float bending=ge*pow(x,2)*0.5+(g1-ge)*0.5*x*x/3;
         return -bending;

    }else{
      //如果截面已经超出变化段则需要分成两段考虑;
        float xe=l1-0.5*d-bt;
       float ge=getCodeByX(0);
        float g1=getCodeByX(xe);
        float bending1=ge*xe*(x-xe/2)+(g1-ge)*xe*0.5*(x-2*xe/3);
        float mtd=x-xe;
        float bending2=g1*mtd*mtd/2;
        if(x<l1){

             return -(bending1+bending2);


        }else{
            return getGh()/2*(x-l1)-(bending1+bending2);


        }

    }


}

float Coping::CoopingSfSolve(float x)
{    //只对半盖梁跨长进行计算;
    //以米为计算单位
    float flag=x<=l/2? 1:-1;;
    x=x<l/2? x:l-x;
   if(x<(l1-0.5*d-bt))
   {
       // 如果所求截面在变化段;
       float ge=getCodeByX(0);
        float g1=getCodeByX(x);
        return -(ge+g1)*x*0.5*flag;

   }else
   {
     //如果截面已经超出变化段则需要分成两段考虑;
       float xe=l1-0.5*d-bt;
       float ge=getCodeByX(0);
       float g1=getCodeByX(xe);
       float mdt=x-xe;
           if(x<l1){
               //判断在支座1的左边还是右边----左边;

           return -((ge+g1)*xe*0.5-g1*mdt)*flag;



           }else
           {
               //右边----
               return (getGh()/2-(ge+g1)*xe*0.5-g1*mdt)*flag;

           }




   }


}

vector<float> Coping::getBeamLoadReaction()
{
    vector<float> res;
    vector<float> foces;
    vector<float> p=getSupReaction();//边梁和中梁产生的荷载
    float R1;
    float R2;
    float lca2=l-l3;
    float M_Sum=0;
    for(int i=0;i<beamNum;i++){
        foces.push_back(p[1]);
    }
    foces[0]=p[0];foces[foces.size()-1]=p[0];
    for(int i=0;i<beamNum;i++){

       M_Sum+=foces[i]*(lca2-beamLca[i]);
    }
    R1=2*M_Sum/l2;
    R2=getHeightSum()-R1;
    res.push_back(R1);
    res.push_back(R2);
    return res;

}

float Coping::CoopingTopBending(float x)
{   x=x<l/2? x:l-x;
    vector<float> p=getSupReaction();
    float MSum=0;
     vector<float> foces;
        for(int i=0;i<beamNum*2;i++){
            foces.push_back(p[1]);
        }
        foces[0]=p[0];foces[1]=p[0];foces[foces.size()-2]=p[0];foces[foces.size()-1]=p[0];


      for(int i=0;i<LcaVal.size();i++){
          if(LcaVal[i]<x){
              MSum-=(x-LcaVal[i])*foces[i];

          }

      }
          if(x>l1){
            MSum+=getBeamLoadReaction()[0]*(x-l1);
          }

return MSum;
}

float Coping::CoopingTopSf(float x)
{
        vector<float> p=getSupReaction();
        float SfSUm=0;
         vector<float> foces;
            for(int i=0;i<beamNum*2;i++){
                foces.push_back(p[1]);
            }
                 foces[0]=p[0];foces[1]=p[0];foces[foces.size()-2]=p[0];foces[foces.size()-1]=p[0];
            for(int i=0;i<LcaVal.size();i++){
                if(LcaVal[i]<x){
                    SfSUm-=foces[i];
                }

            }
                if(x>l1){
                   SfSUm+= getBeamLoadReaction()[0];
                }
                if(x>l1+l2){

                   SfSUm+= getBeamLoadReaction()[1];
                }
                return SfSUm;
}

float Coping::CoopingBendingSum(float x)
{
    return CoopingBendingSolve(x)+CoopingTopBending(x);

}

float Coping::CoopingSfSum(float x)
{
    return CoopingSfSolve(x)+CoopingTopSf(x);
}

vector<float> Coping::getSupReactionSum()
{
    vector<float> res;
     vector<float> a=getSupReaction();;
    vector<float> b=getBeamLoadReaction();
    res.push_back(a[0]+b[0]);
    res.push_back(a[1]+b[1]);
    return res;


}

float Coping::getRQi(bool loadType,bool SolveType, int VehicleNum, int beamId)
{   //loadTye true视为单孔布载,否则视为双控布载;
    //SolveType true表示杠杆法 ,否则表示修正偏压法;
    float vmc=demobs.vmcffe();
    float Rq1=vmc*(1.2*demobs.pk+demobs.qk*Lj*0.5);
     float Rq2=vmc*(1.2*demobs.pk+demobs.qk*(Lj+(Lc-Lj)));
     float RCal;
     if(loadType){
         RCal=Rq1;

     }else{
       RCal=Rq2;

     }
     if(SolveType){
         return 0.5*RCal*demobs.getCenterLoadMcq(beamId,VehicleNum);

     }else{
         return 0.5*RCal*demobs.PartialLoadMcq(beamId,VehicleNum);


     }

}

vector<float> Coping::getReactions(bool loadType, bool SolveType,int VehicleNum)
{   //loadTye true视为单孔布载,否则视为双控布载;
    //SolveType true表示杠杆法 ,否则表示修正偏压法;
    vector<float> res;
    for(int i=1;i<=beamNum;i++){
        res.push_back(getRQi(loadType,SolveType,VehicleNum,i));
         res.push_back(getRQi(loadType,SolveType,VehicleNum,i));
    }
    return res;

}//活载产生的各支座力数组

vector<float> Coping::getLiveReaction(bool loadType, bool SolveType, int VehicleNum)
{ vector<float> sups=getReactions(loadType,SolveType,VehicleNum);
   vector<float> res;
    float R1,R2;
    float M=0;
    float supsum=0;
    for(int i=0;i<sups.size();i++){
        supsum+=sups[i];
       M+=sups[i]*(LcaVal[i]-l1);
    }
    R2=M/l2;
    R1=supsum-R2;
    res.push_back(R1);
    res.push_back(R2);

    return res;
}

float Coping::getLiveLoadBending(bool SolveType, int VehicleNum, float x)
{ bool loadType=false;
  float BendingSum=0;
  vector<float> sup=getLiveReaction(loadType,SolveType,VehicleNum);
  vector<float> foces=getReactions(loadType,SolveType,VehicleNum);
  for(int i=0;i<LcaVal.size();i++){
      if(LcaVal[i]<x){
        BendingSum-=(x-LcaVal[i])*foces[i];

      }
  }
  if(x>l1){

      BendingSum+=(x-l1)*sup[0];


  }
  return BendingSum;

}

float Coping::getLiveLoadSf(bool SolveType, int VehicleNum, float x)
{   bool loadType=false;
    float SfSum=0;
    vector<float> sup=getLiveReaction(loadType,SolveType,VehicleNum);
    vector<float> foces=getReactions(loadType,SolveType,VehicleNum);
    for(int i=0;i<LcaVal.size();i++){
        if(LcaVal[i]<x){

            SfSum-=foces[i];
        }
    }
    if(x>l1){

        SfSum+=sup[0];
    }
    return SfSum;

}

vector<float> Coping::getLiveLoadFoces(bool SolveType, int VehicleNum, float x)
{
    bool loadType=false;
       float SfSum=0;
       float bendingSum=0;
       vector<float> sup;
       vector<float> foces=getReactions(loadType,SolveType,VehicleNum);
       for(int i=0;i<LcaVal.size();i++){
           bendingSum+=InnerFoceSolve(LcaVal[i],foces[i],x)[0];
           SfSum+=InnerFoceSolve(LcaVal[i],foces[i],x)[1];

       }
       if(x>l1){
           sup=getLiveReaction(loadType,SolveType,VehicleNum);
           bendingSum+=InnerFoceSolve(l1,-sup[0],x)[0];
           SfSum+=InnerFoceSolve(l1,-sup[0],x)[1];

       }
       if(x>l1+l2){
           bendingSum+=InnerFoceSolve(l1+l2,-sup[1],x)[0];
           SfSum+=InnerFoceSolve(l1+l2,-sup[1],x)[1];

       }
       vector<float> res;
       res.push_back(bendingSum);
       res.push_back(SfSum);
       return res;

}

vector<float> Coping::InnerFoceSolve(float Xp, float p, float X)
{ vector<float> res;
    if(Xp<X){
        res.push_back((Xp-X)*p);
        res.push_back(-p);
    }else{
        res.push_back(0);
        res.push_back(0);

    }
return res;
}

vector<float> Coping::getInnerFoceAt(float X){
   // Flag 标记 true计算上部结构恒载产生内力
  //false 计算上部结构活载产生内力
    vector<float> res;
    float bending=0;
    float Sf=0;
    vector<float> foces1=getBearLoad1();//梁自重对各支座产生的压力(从左到右)
   for(int i=0;i<foces1.size();i++){
       vector<float> temp=InnerFoceSolve(LcaVal[i],foces1[i],X);
        bending+=temp[0];
        Sf+=temp[1];
   }
   if(X>l1){
       vector<float> temp=InnerFoceSolve(l1,-getBeamLoadReaction()[0],X);
        bending+=temp[0];
        Sf+=temp[1];

   }
   if(X>l1+l2){
       vector<float> temp=InnerFoceSolve(l1+l2,-getBeamLoadReaction()[1],X);
        bending+=temp[0];
        Sf+=temp[1];

   }
   res.push_back(bending);
   res.push_back(Sf);
    return res;

}

vector<float> Coping::getBearLoad1()
{   vector<float> foces1;
    vector<float> p=getSupReaction();
        for(int i=0;i<beamNum*2;i++){
            foces1.push_back(p[1]);
        }
        foces1[0]=p[0];foces1[1]=p[0];foces1[foces1.size()-2]=p[0];foces1[foces1.size()-1]=p[0];
        return foces1;
}

vector<float> Coping::getExtremumfoces(float x)
{
    float M_max,M_min,Sf_Max,Sf_min;
    vector<float> M,Sf;
    for(int i=1;i<=3;i++){
    vector<float> temp=getLiveLoadFoces(false,i, x);
    M.push_back(temp[0]);
    Sf.push_back(temp[1]);
    temp=getLiveLoadFoces(true,i, x);
    M.push_back(temp[0]);
    Sf.push_back(temp[1]);


    }
    M_max=M[0],M_min=M[0],Sf_Max=Sf[0],Sf_min=Sf[0];
    for(int i=0;i<M.size();i++){
        M_max=M_max>M[i]? M_max:M[i];
        M_min=M_min<M[i]? M_min:M[i];
        Sf_Max= Sf_Max>Sf[i]? Sf_Max:Sf[i];
         Sf_min= Sf_min<Sf[i]? Sf_min:Sf[i];

    }
    vector<float> res;
    res.push_back(M_max);
     res.push_back(M_min);
     res.push_back( Sf_Max);
    res.push_back(Sf_min);
    return res;
}

vector<float> Coping::getCombinSud(float x)
{
    vector<float> liveLoad=getExtremumfoces( x);
    vector<float> res;
    float bending=CoopingBendingSum( x);
    float Sf=CoopingTopSf(x);
    res.push_back(ultimateLimitSta( bending,liveLoad[0]*demobs.vmcffe()));
    res.push_back(ultimateLimitSta( bending,liveLoad[1]*demobs.vmcffe()));
    res.push_back(ultimateLimitSta( Sf,liveLoad[2]*demobs.vmcffe()));
    res.push_back(ultimateLimitSta(Sf,liveLoad[3]*demobs.vmcffe()));
    return res;

}

vector<float> Coping::getCombinSfd(float x)
{
    vector<float> liveLoad=getExtremumfoces( x);
    vector<float> res;
    float bending=CoopingBendingSum( x);
    float Sf=CoopingTopSf(x);

        res.push_back(sfd( bending,liveLoad[0]));
        res.push_back(sfd( bending,liveLoad[1]));
        res.push_back(sfd( Sf,liveLoad[2]));
        res.push_back(sfd(Sf,liveLoad[3]));


    return res;
}

vector<float> Coping::getCombinSqd(float x)
{
    vector<float> liveLoad=getExtremumfoces( x);
    vector<float> res;
    float bending=CoopingBendingSum( x);
    float Sf=CoopingTopSf(x);
    res.push_back(sqd( bending,liveLoad[0]));
    res.push_back(sqd( bending,liveLoad[1]));
    res.push_back(sqd( Sf,liveLoad[2]));
    res.push_back(sqd(Sf,liveLoad[3]));
    return res;
}

bool Coping::getMembersType()
{
 /* float ln=l2-0.8*d;
  float ltemp=1.15*ln<l2? 1.15*ln:l2;*/
  return 2<get_s_d_ratio()<=5;
}

vector<float> Coping::getMinNM()
{ float startflag=0;
    float gap=l/200;
    float Min_M=0;
    vector<float> res;
    while(startflag<l/2){
        float temp=getCombinSud(startflag)[1];
        Min_M=Min_M<temp? Min_M:temp;
        startflag+=gap;

    }
    res.push_back(Min_M);
    res.push_back(startflag-gap);
    return res;
}

vector<float>  Coping::getAs(float as)
{ //as以毫米为单位
    float M_Max=getCombinSud(l/2)[0];//跨中极限组合内力值
    float M_Min=fabs(getMinNM()[0]);
    float param1,param2,param3,param4;
     float x1=0,x2=0;
    if(getMembersType()){
        float h1=h0*1e3-as;
     param1=0.5*myfcd*bh*1e3;
     param2=-myfcd*bh*h1*1e3;
     param3=M_Max*1e6;
     param4=M_Min*1e6;
     vector<float> root1;
     vector<float> root2;

     float temp=pow(param2,2)-4*param1*param3;
     float temp1=pow(param2,2)-4*param1*param4;
     if(temp>=0){
         root1.push_back((-param2+sqrt(temp))/(2*param1));
         root1.push_back((-param2-sqrt(temp))/(2*param1));
         x1=0<root1[0]<1e3*h0?  root1[0]:x1;
         x1=0<root1[1]<1e3*h0? root1[1]:x1;


     }else{qDebug()<<"1无解的方程!";}
     if(temp1>0){
         root2.push_back((-param2+sqrt(temp1))/(2*param1));
         root2.push_back((-param2-sqrt(temp1))/(2*param1));
         x2=0<root2[0]<1e3*h0?  root2[0]:x2;
         x2=0<root2[1]<1e3*h0? root2[1]:x2;

     }else{
         qDebug()<<"2无解的方程!";}

    }else{



    }
  vector<float> res;
  res.push_back(myfcd*bh*1e3*x1/myfsd);
  res.push_back(myfcd*bh*1e3*x2/myfsd);

  return res;
}

float Coping::P_CrackSolve(float x)
{   float P_C2,P_C3,sigma_ss;
     float Wfk;
    if(getMembersType()){
        float ln=l2-0.8*d;
        float ltemp=1.15*ln<l2? 1.15*ln:l2;
        float Ms=fabs(getCombinSfd(x)[0]);
        P_C2=1+0.5*fabs(getCombinSqd(x)[0])/Ms;
        P_C3=(0.4*ltemp/h0+1)/3;
        sigma_ss=1e6*Ms/(0.87*myAs*(h0*1e3-myas));
        float rho=getRho(myAs,myas);
        Wfk=C1*P_C2*P_C3*sigma_ss/Es*((30+steel_d1)/(0.28+10*rho));


    }

    return Wfk;
}

float Coping::N_CrackSolve(float x)
{
float P_C2,P_C3,sigma_ss;
         float Wfk;
        if(getMembersType()){
            float ln=l2-0.8*d;
            float ltemp=1.15*ln<l2? 1.15*ln:l2;
            float Ms=fabs(getCombinSfd(x)[1]);
            P_C2=1+0.5*fabs(getCombinSqd(x)[1])/Ms;
            P_C3=(0.4*ltemp/h0+1)/3;
            sigma_ss=1e6*Ms/(0.87*myAs2*(h0*1e3-myas1));
            float rho=getRho(myAs2,myas1);
            Wfk=C1*P_C2*P_C3*sigma_ss/Es*((30+steel_d2)/(0.28+10*rho));


        }

        return Wfk;
}

float Coping::getRho(float mAs,float mas)
{
    return mAs/(bh*1e3*(h0*1e3-mas));

}

vector<float> Coping::NormalSCheak(float x)
{// vector<float> mx=get_X(myas,x);
    vector<float> mx;
    mx.push_back(myfsd*myAs/myfcd/(bh*1e3));
      mx.push_back(myfsd*myAs2/myfcd/(bh*1e3));
        float mh0=1e3*h0-myas;
        float Mu1;
        float Mu2;
    if(getMembersType()){
        float ln=l2-0.8*d;
        float ltemp=1.15*ln<l2? 1.15*ln:l2;
        float param1= ltemp/h0;

        float mh1=1e3*h0-myas1;
        float z1=(0.75+0.05*param1)*(mh0-0.5*mx[0]);
        float z2=(0.75+0.05*param1)*(mh1-0.5*mx[1]);
         Mu1=myfsd*myAs*z1;
         Mu2=-myfsd*myAs2*z2;

    }else{
        Mu1=myfsd*myAs*(mh0-mx[0]/2);
         Mu2=-myfsd*myAs2*(mh0-mx[1]/2);

    }


    vector<float> res;

    res.push_back(Mu1/1e6);

    res.push_back(Mu2/1e6);
    return  res;



}

vector<float> Coping::get_X(float as, float x)
{    float x1=0,x2=0;
    //单位m
    /*float M_Max=fabs(getCombinSud(x)[0]);//跨中极限组合内力值
    float M_Min=fabs(getCombinSud(x)[1]);
     float param1,param2,param3,param4;

    if(getMembersType()){
        float h1=h0*1e3-as;
     param1=0.5*myfcd*bh*1e3;
     param2=-myfcd*bh*h1*1e3;
     param3=M_Max*1e6;
     param4=M_Min*1e6;
     vector<float> root1;
     vector<float> root2;

     float temp=pow(param2,2)-4*param1*param3;
     float temp1=pow(param2,2)-4*param1*param4;
     if(temp>=0){
         root1.push_back((-param2+sqrt(temp))/(2*param1));
         root1.push_back((-param2-sqrt(temp))/(2*param1));
         x1=0<root1[0]<1e3*h0?  root1[0]:x1;
         x1=0<root1[1]<1e3*h0? root1[1]:x1;


     }else{qDebug()<<"1无解的方程!";}
     if(temp1>0){
         root2.push_back((-param2+sqrt(temp1))/(2*param1));
         root2.push_back((-param2-sqrt(temp1))/(2*param1));
         x2=0<root2[0]<1e3*h0?  root2[0]:x2;
         x2=0<root2[1]<1e3*h0? root2[1]:x2;

     }else{
         qDebug()<<"2无解的方程!";}

    }else{



    }*/
    vector<float> res;

    res.push_back(x1);
    res.push_back(x2);
    return res;
}

vector<float> Coping::obliqueCheake(float x)
{

vector<float> res;
return res;

}

vector<float> Coping::ObShearStrenthCheak(float x)
{// 斜截面抗剪计算;
    float myasv=nv*3.1415926*pow(dv,2)/4;


    float apha1=1.0;
    float rho_sv=myasv/(1e3*bh*get_SvAt(x));
    float test=get_SvAt(x);
    float h=(h0*1e3-myas);

    float p=myAs/(1e3*bh*h)*100;
    float l_h=get_s_d_ratio();

    vector<float> res;
     if(getMembersType()){
         //如果是短梁
         res.push_back(0.33*1e-4*(l_h+10.3)*sqrt(myfcuk)*bh*1e3*h);
         res.push_back(apha1*0.5*1e-4*(14-l_h)*bh*1e3*h*sqrt((2+0.6*p)*sqrt(myfcuk)*rho_sv*fsv));




     }else{
         //普通构件
          res.push_back(0.51*1e-3*sqrt(myfcuk)*bh*h0);
          res.push_back((apha1*0.45*1e-3)*bh*h0*sqrt((2+0.6*p)*sqrt(myfcuk)*rho_sv*fsv));
     }
return res;
}

float Coping::get_s_d_ratio()
{
    float ln=l2-0.8*d;
    float ltemp=1.15*ln<l2? 1.15*ln:l2;
   return ltemp/h0;

}

float Coping::get_SvAt(float x)
{   x=x<l? x:l-x;
    if(0<=x<divi_x1){
        return Sv1;
    }
    if(divi_x1<x<=divi_x2){
        return Sv2;
    }
    if(divi_x2<x<=divi_x3){
        return Sv3;
    }
return 0;
}



