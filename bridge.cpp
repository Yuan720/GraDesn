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


};

float half_box_girder::getAreaSum() {
	return rec1.Area + rec2.Area + rec3.Area + trangle1.Area + trangle2.Area + trangle3.Area + trangle4.Area + trangle5.Area + trangle6.Area + trangle7.Area;
};
float half_box_girder::getSMOASum(float yc) {
	return rec1.getSMOA(yc) + rec2.getSMOA(yc) + rec3.getSMOA(yc) + trangle1.getSMOA(yc) + trangle2.getSMOA(yc) + trangle3.getSMOA(yc) + trangle4.getSMOA(yc)
		+ trangle5.getSMOA(yc) + trangle6.getSMOA(yc) + trangle7.getSMOA(yc);
};
float half_box_girder::getStaticMomentSum() {
	return rec1.getStaticMoment() + rec2.getStaticMoment() + rec3.getStaticMoment() + trangle1.getStaticMoment() + trangle2.getStaticMoment()
		+ trangle3.getStaticMoment() + trangle4.getStaticMoment() + trangle5.getStaticMoment() + trangle6.getStaticMoment() + trangle7.getStaticMoment();
};
float half_box_girder::getCentroidHeight() {
	return getStaticMomentSum() / getAreaSum();
};
small_box_girder::small_box_girder(half_box_girder vleft, half_box_girder vright) {
	left = vleft;
	right = vright;

};
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
};
float small_box_girder::SolveKs() {
	return SmoaSlove(CentroidHeightSolve()) / (AreaSolve()*(left.h - CentroidHeightSolve()));
};
float small_box_girder::SolveKx() {
	return SmoaSlove(CentroidHeightSolve()) / (AreaSolve()*(CentroidHeightSolve()));
};
float small_box_girder::getRho() {
	return (SolveKs() + SolveKx()) / left.h;
};
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

};
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
