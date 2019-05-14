#pragma once
#include<math.h>
#include<vector>
using namespace std;
#include<iostream>
#include<QDebug>
#include<variables.h>

//基本插值表类
//提供给定表的任意x的插值结果计算;
struct FulcrumSectionMq
{
	float s1, s2, s3, s4, s5, stl = 0.5;//stl行道路缘石宽度默认0.5m;



};
class BasicInterpolationTable {
public:
	int sizex;
	float *ivar;
	float *idvar;
	BasicInterpolationTable() {};
	BasicInterpolationTable(float x[], float y[], int xsz) {
		sizex = xsz;
		ivar = x;
		idvar = y;

	};
	void update(float x[], float y[], int xsz) {
		sizex = xsz;
		ivar = x;
		idvar = y;

	};
	float Interpolat(float var) {
		if (var < *ivar) {
			return var * ((*idvar) / (*ivar));

		}
		else {

			if (var > *(ivar + (sizex - 1)))
			{

				return (var - *(ivar + sizex - 1))*(*(idvar + sizex - 1) / (*(ivar + sizex - 1)) + (*(idvar + sizex - 1)));
			}
			else {

				for (int i = 0; i < sizex; i++)
				{



					if (var == *(ivar + i)) {
						return *(idvar + i);
					}
                    if ((ivar[i + 1] > var)&(var > ivar[i])) {

						return	*(idvar + i) + (var - *(ivar + i))*(*(idvar + i + 1) - *(idvar + i)) / (*(ivar + i + 1) - *(ivar + i));

					}

				}
			}
		}


	};

};
//三角形类
//bottomLong 底边长
//Height 高
//SHD形心距离底边高度
class Triangle {
public:
    Triangle() {}
	Triangle(float bottomLong, float Height, float TSHD);
	float bottomLong, Height, TSHD;
	double Area;
	float getSMOA(float yc);
	float getStaticMoment();
};
//矩形类
//Length 长
//Height 宽
//RSHD形心距离底边高度
class Rectangular {
public:
	Rectangular() {};
	Rectangular(float Length, float Height, float RSHD);
	float Length, Height, RSHD;
	double Area;
	float getSMOA(float yc);
	float getStaticMoment();
};
//类小箱梁
//h 总高度
//1/2小箱梁类
//d1顶板厚度,d2腹板厚度,d3底版厚度
//b1 1/2顶板宽度 b2 1/2底版宽度
//b4,h4上顶板左侧过渡区横纵变化值
//b5,h5上顶板右侧过渡区横纵变化值
//b6,h6下板过渡区横纵变化值
//p腹板斜率
//水平所限,默认h4=h5;
class half_box_girder {
public:
    half_box_girder(vector<float> v,bool type);
	half_box_girder() {};
	half_box_girder(float h, float d1, float d2, float d3, float b1, float b2, float b4, float h4, float b5, float  h5, float b6, float h6, float p, bool type);
	float h;
	float d1, d2, d3, b1, b2;
	float b4, h4;
	float b5, h5;
	float b6, h6;
	float p;
    vector<float> getDivided() ;
	bool isSideBeam;
	//求解惯性矩算之和
	float * getCantileverPlate_info();
	float getSMOASum(float yc);
	//求解面积之和
	float getAreaSum();
	//求解形心高度
	float getStaticMomentSum();
	float getCentroidHeight();
	float * getMidleLineS();
    vector<float> getInfo();
    vector<float> getSnByh(float mh);
	//private:
	Rectangular rec1, rec2, rec3;
	Triangle trangle1, trangle2, trangle3, trangle4, trangle5, trangle6, trangle7;
};
class small_box_girder {
public:
	small_box_girder() {};
	small_box_girder(half_box_girder vleft, half_box_girder vright);
	half_box_girder left;
	half_box_girder right;
	float SmoaSlove(float yc);
	float StaticMomentSolve();
	float CentroidHeightSolve();
	float AreaSolve();
	//ks截面效率指标中ks=(惯性矩之和)/(截面面积*形心到上边缘高度)
	//kx截面效率指标中kx=(惯性矩之和)/(截面面积*形心到下边缘高度)
	float SolveKs();
	float SolveKx();
	//抗扭惯性矩
	float torsionalMomentInertiaSolve();
	//求解截面效率指标ρ
	float getRho();
	//返回布尔值判断截面效率指标是否符合要求.
	bool isValid();
};
class field_making_girder_beam {
public:
	small_box_girder  sbg;
	float fmgb_length;
	float Area;
	float StaticMoment;
	float CentroidHeight;
	//fmgb 现浇段长度 浮点值
    field_making_girder_beam() {}
	field_making_girder_beam(small_box_girder mysbg, float fmgb);
	float * getSpanLength();
	float SmoaSlove();
	float SolveKs();
	float SolveKx();
	float getRho();
	bool isValid();
    vector<float> getSnByh(float mh,bool fieldCount);


};
class OrdinaryBrigeSection
{
public:

    //跨中边梁
	field_making_girder_beam side_main_bean;
    //跨中中梁
	field_making_girder_beam mid_main_bean;
    //支点中梁
    field_making_girder_beam *side_Fulcr_bean;
    //支点边梁
    field_making_girder_beam *mid_Fulcr_bean;
	//主梁片数
	int bean_nums;
	float FulcrumInterval[5];
	//总桥宽
	float total_span;
	//桥长方向计算跨径
	float cal_span;
	//路缘石宽度
	float stoneWidth;
    //横隔梁
    float f_c_lca;
	float qk = 10.5;
	float pk = cal_span <= 5 ? 270 : cal_span > 50 ? 360 : 2 * (130 + cal_span);
	vector<float> area_es;
	vector<float> ais;
	vector<float> aix;
	vector<float> aid;
	//惯性矩
	vector<float> moi;
	vector<float> moi_field_included;
	//不同布载列数对应系数
	float CoefficientloadArrange[8] = { 1.2,1,0.78,0.67,0.60,0.55,0.52,0.50 };
	//抗扭惯性矩
	vector<float> tmi;
	FulcrumSectionMq fsm;
    void InitFsm();
	float getVectorSum(vector<float> va, int sub_began, int sub_end) {
		float sum = 0;
		for (int i = sub_began - 1; i <= sub_end - 1; i++) {
			sum += va[i];
		}
		return sum;


    }
	void SetFsm(FulcrumSectionMq myfsm) {
		fsm = myfsm;
	}
    OrdinaryBrigeSection(){}
    OrdinaryBrigeSection(field_making_girder_beam smb, field_making_girder_beam mmb,field_making_girder_beam *fsmb, field_making_girder_beam *fmmb, int bnu, float calspan, float stw)
	{
		side_main_bean = smb;
		mid_main_bean = mmb;
        side_Fulcr_bean=fsmb;
        mid_Fulcr_bean=fmmb;
		bean_nums = bnu;
		total_span = 2 * (smb.sbg.right.b1 + smb.sbg.left.b1 + smb.fmgb_length) + (bnu - 2)*(mmb.sbg.left.b1 + mmb.sbg.right.b1 + 2 * mmb.fmgb_length);

        cal_span = calspan;
        f_c_lca = cal_span / (5 - 2);
		stoneWidth = stw;
		half_box_girder temp = side_main_bean.sbg.left.isSideBeam ? side_main_bean.sbg.left : side_main_bean.sbg.right;
		half_box_girder temp_mid = side_main_bean.sbg.left.isSideBeam ? side_main_bean.sbg.right : side_main_bean.sbg.left;
		FulcrumInterval[0] = temp.b1 - temp.b2 - (temp.h - temp.d1 - temp.h4) / temp.p + (temp.d2*sqrt(1 + (float)1 / pow(temp.p, 2))) / 2;
		float right_side = temp_mid.b1 - temp_mid.b2 - (temp_mid.h - temp_mid.d1 - temp_mid.h4) / temp_mid.p + (temp_mid.d2*sqrt(1 + (float)1 / pow(temp_mid.p, 2))) / 2;
		FulcrumInterval[1] = temp.b1 + temp_mid.b1 - right_side - FulcrumInterval[0];
		half_box_girder temp2 = mid_main_bean.sbg.left;
		float left_side = temp2.b1 - temp2.b2 - (temp2.h - temp2.d1 - temp2.h4) / temp2.p + (temp2.d2*sqrt(1 + (float)1 / pow(temp2.p, 2))) / 2;
		FulcrumInterval[2] = right_side + left_side + smb.fmgb_length + mmb.fmgb_length;
		FulcrumInterval[3] = 2 * temp2.b1 - 2 * left_side;
		FulcrumInterval[4] = 2 * left_side + 2 * mid_main_bean.fmgb_length;

		//惯性矩
		float	Imt1 = side_main_bean.sbg.SmoaSlove(side_main_bean.sbg.CentroidHeightSolve());
		float	Imt2 = mid_main_bean.sbg.SmoaSlove(mid_main_bean.sbg.CentroidHeightSolve());
		//含现浇段惯性矩
		float	Imt_field_in = side_main_bean.SmoaSlove();
		float	Imt_field_in2 = mid_main_bean.SmoaSlove();
		//抗扭惯性矩
		float Timt1 = side_main_bean.sbg.torsionalMomentInertiaSolve();
		float Timt2 = mid_main_bean.sbg.torsionalMomentInertiaSolve();
		//
		float L1 = temp.b1;
		float L2 = temp_mid.b1 + side_main_bean.fmgb_length + mid_main_bean.fmgb_length + temp2.b1;
		float L3 = 2 * (temp2.b1 + mid_main_bean.fmgb_length);
		for (int i = 0; i < bean_nums; i++) {
			ais.push_back(L3);
			moi.push_back(Imt2);
			tmi.push_back(Timt2);
			moi_field_included.push_back(Imt_field_in2);
			area_es.push_back(mid_main_bean.Area);
		};
		ais[0] = L1;
		ais[bean_nums - 1] = L2;
		moi[0] = Imt1; moi[bean_nums - 1] = Imt1;
		tmi[0] = Timt1; tmi[bean_nums - 1] = Timt1;
		moi_field_included[0] = Imt_field_in; moi_field_included[bean_nums - 1] = Imt_field_in;
		area_es[0] = side_main_bean.Area; area_es[bean_nums - 1] = side_main_bean.Area;
		if (bean_nums >= 3) { ais[2] = L2; ais[bean_nums - 2] = L2; };
        for (int i = 0; i < bean_nums; i++)
		{
			aix.push_back(0);
			moi.push_back(0);
			tmi.push_back(0);

		}
        for (int i = 1; i <= bean_nums; i++)
		{
			aix[i - 1] = getVectorSum(ais, 1, i);
		}
        for (int i = 0; i < aix.size(); i++)
		{
			aid.push_back(aix[i] - total_span / 2);
		}
	}
	vector<float> get_aid_moif() {
		vector<float> temp;
        for (int i = 0; i < aid.size(); i++)
		{
			temp.push_back(pow(aid[i], 2)*moi_field_included[i]);
		}
		return temp;

	}
	//跨中截面横向分布系数
	float getBeta() {
		float param1 = getVectorSum(tmi, 1, tmi.size());
		float 	param2 = getVectorSum(get_aid_moif(), 1, get_aid_moif().size());
		return (float)1 / (1 + 0.42*(pow(cal_span, 2) / 12)*(param1 / param2));

	}
	float getPerTireLoad(int bean_Id, float e) {
		float param1 = moi_field_included[bean_Id - 1] / getVectorSum(moi_field_included, 1, moi_field_included.size());
		float param2 = getVectorSum(get_aid_moif(), 1, get_aid_moif().size());
		float param3 = (aid[bean_Id - 1] * e*moi_field_included[bean_Id - 1] / param2);
		return param1 + getBeta()*param3 / param2;

	}
	float vmcffe() {
		//求解冲击系数
		float Ic = getVectorSum(moi_field_included, 1, moi_field_included.size());
		float Mc = getVectorSum(area_es, 1, area_es.size()) * 25 * 1000 / 9.81;
		float f = 3.1415926 / (2 * pow(cal_span, 2))*sqrt((3.45*pow(10, 10)*Ic) / Mc);
		float mu;
		if (f < 1.5)
		{
            mu = 0.05f;
		}
		else {
			mu = f <= 14 ? 0.1767*log(f) - 0.0157 : 0.45;

		}
		return mu + 1;

	}
	int getMaximumVehicle() {
		//返回能够布载的最大车辆列数
		return (int)((total_span - 2 * stoneWidth - 2 * 0.5 + 1.3) / 3.1);

	}
	vector<float> getEccentricDistances(int VehicleNum) {
		vector<float> ese;
		int tireNum = VehicleNum * 2;
		for (int i = 1; i <= tireNum; i++)
		{
			float gapNum = ceil((float)(i - 2) / (float)2);
			float carWidthNum = floor(i / 2);
			ese.push_back(stoneWidth + 0.5 + gapNum * 1.3 + carWidthNum * 1.8);

		}

		return ese;
	}
	float loadArrangement(int beamId, int VehicleNum) {
		//布载
		//主梁编号
		//传入布载的车辆列数
		if ((2 * beamId) >= bean_nums + 1)
		{
			return loadArrangement(bean_nums + 1 - beamId, VehicleNum);

		}
		else
		{
			float result = 0;
			vector<float> myese = getEccentricDistances(VehicleNum);
            for (int i = 0; i < VehicleNum * 2; i++)
			{
				result += getPerTireLoad(beamId, myese[i]);

			}
			return result * 0.5*CoefficientloadArrange[VehicleNum - 1];
		}


	}
	float getMcqByBeamId(int beamId) {
		float maxVehicleNum = getMaximumVehicle();
		float result = 0;
        for (int i = 1; i <= maxVehicleNum; i++)
		{
			if (result < loadArrangement(beamId, i))
			{
				result = loadArrangement(beamId, i);
			}

		}
		return result;



	}
	//支点截面横向分布系数
	float getTDC_for_FulcrByBeamId(int beamId) {
		if (beamId > (bean_nums + 1) / 2)
		{
			beamId = 1 + bean_nums - beamId;
		}

		float result;
		if (beamId == 1 | beamId == bean_nums)
		{
			float a[3] = { 0,fsm.s1 + fsm.s2,fsm.s1 + fsm.s2 + fsm.s3 };
			float b[3] = { 1,1,0 };
			//一号梁单列
			BasicInterpolationTable  mqcal(a, b, 3);
			result = 1.2*(mqcal.Interpolat(fsm.stl + 0.5) + mqcal.Interpolat(fsm.stl + 0.5 + 1.8)) / 2;
		}
		else
		{
			if (beamId == 2 | beamId == bean_nums - 1)
			{	//二号梁单列
				float c[6] = { 0,fsm.s1 + fsm.s2,fsm.s1 + fsm.s2 + fsm.s3 ,fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4,fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4 + fsm.s5,fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4 + fsm.s5 + 1.8 };
				float d[6] = { 0,0,1,1,0,0 };
				BasicInterpolationTable mqcal(c, d, 6);
				if (fsm.s3 < fsm.s5)
				{
					float left = fsm.s1 + fsm.s2 + fsm.s3;
                    result = 1.2f*(mqcal.Interpolat(left) + mqcal.Interpolat(left + 1.8)) / 2;
				}
				else {
					float right = fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4;
                    result = 1.2f*(mqcal.Interpolat(right) + mqcal.Interpolat(right - 1.8)) / 2;
				}
				float tempVal;
				//二号梁双列
				float begain = fsm.s1 + fsm.s2;
				bool isEnded = false;
				tempVal = 0;
				while (!isEnded)
				{
                    float val1 = begain - 1.8f;
                    float val2 = begain + 1.3f;
                    float val3 = val2 + 1.8f;
					float temp = mqcal.Interpolat(val1) + mqcal.Interpolat(begain) + mqcal.Interpolat(val2) + mqcal.Interpolat(val3);
					tempVal = tempVal > temp ? tempVal : temp;
                    begain += 0.1f;
					if (begain >= fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4)
					{
						isEnded = true;
					}
				}
                tempVal = tempVal * 0.5f;
				result = result > tempVal ? result : tempVal;

			}
			else
			{	//三号梁单列
				float c[6] = { 0,fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4,fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4 + fsm.s5 ,fsm.s1 + fsm.s2 + fsm.s3 + 2 * fsm.s4 + fsm.s5,fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4 * 2 + fsm.s5 * 2 };

				float d[6] = { 0,0,1,1,0,0 };
				BasicInterpolationTable mqcal(c, d, 6);
				float left = fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4 + fsm.s5;
                result = 1.2f*(mqcal.Interpolat(left) + mqcal.Interpolat(left + 1.8)) / 2;

				float tempVal;
				//三号梁双列
				float begain = fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4;
				bool isEnded = false;
				tempVal = 0;
				while (!isEnded)
				{
                    float val1 = begain - 1.8f;
                    float val2 = begain + 1.3f;
                    float val3 = val2 + 1.8f;
					float temp = mqcal.Interpolat(val1) + mqcal.Interpolat(begain) + mqcal.Interpolat(val2) + mqcal.Interpolat(val3);
					tempVal = tempVal > temp ? tempVal : temp;
                    begain += 0.1f;
					if (begain >= fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4 * 2 + fsm.s5)
					{
						isEnded = true;
					}
				}
                tempVal = tempVal * 0.5f;
				result = result > tempVal ? result : tempVal;



			}

		}

		return result;



	}
	vector<float> getLivaLoadBending(int beamId) {
		//返回向量组,对应该梁号的 支点 1/4截面 跨中截面 最大弯矩
		vector<float> result;
        //float a[4] = { 0,f_c_lca,cal_span - f_c_lca ,cal_span };
        //float b[4] = { getTDC_for_FulcrByBeamId(beamId),getMcqByBeamId(beamId),getMcqByBeamId(beamId),  getTDC_for_FulcrByBeamId(beamId) };
        //BasicInterpolationTable  test(a, b, 4);
		float temp1, temp2, temp3;
		temp1 = 0;
        float test1=(float)3 / 16 * cal_span;
         float test2=(float)3/32 * pow(cal_span, 2);
         float test3=getMcqByBeamId(beamId);
          float test4=vmcffe();;

        temp2 = (pk * 3 / 16 * cal_span + qk * 3 / 32 * pow(cal_span, 2))*getMcqByBeamId(beamId)*vmcffe();
        temp3 = (pk*cal_span / 4 + qk * pow(cal_span, 2) / 8)*vmcffe()*getMcqByBeamId(beamId);
		result.push_back(temp1);
		result.push_back(temp2);
		result.push_back(temp3);
        qDebug()<<temp2<<"1/4截面弯矩_____"<<endl<<"----1/4截面弯矩-----"<<getLiveLoad_M(beamId,(cal_span/4)*1e3)<<"----halo!";
		return	result;
	}
	vector<float> getLivaLoadSF(int beamId) {
		//返回向量组,对应该梁号的 支点 1/4截面 跨中截面 最大剪力
		vector<float> result;
		float a[4] = { 0,f_c_lca,cal_span - f_c_lca ,cal_span };
		float b[4] = { getTDC_for_FulcrByBeamId(beamId),getMcqByBeamId(beamId),getMcqByBeamId(beamId),  getTDC_for_FulcrByBeamId(beamId) };
		BasicInterpolationTable  test(a, b, 4);
		float temp1, temp2, temp3;
		temp1 = (1.2*pk * 1 * getTDC_for_FulcrByBeamId(beamId) + getMcqByBeamId(beamId)*qk*cal_span / 2 + qk * f_c_lca / 2 * (getTDC_for_FulcrByBeamId(beamId) - getMcqByBeamId(beamId))*((cal_span - f_c_lca / 3) / cal_span));
		temp1 *= vmcffe();

		temp2 = (1.2*pk*0.75 + qk * cal_span * 9 / 32)*vmcffe()*test.Interpolat(cal_span / 4);
        temp3 = (1.2*pk*0.5 + qk * cal_span / 8)*test.Interpolat(cal_span / 2)*vmcffe();
		result.push_back(temp1);
		result.push_back(temp2);
		result.push_back(temp3);

		return	result;

	}

    float getLiveLoad_M(int beamId,float x){
        //x以毫米为单位输入;
        x/=1e3;//转为米
        float Mc=getMcqByBeamId(beamId);
        float lj=cal_span;
        float vmc=vmcffe();
        float R_left=1-x/lj;
        float R_right=x/lj;
        float M_max=R_left*x;
        float S=M_max*lj*0.5;
        float bend;
        bend=vmc*Mc*(pk*M_max+qk*S);
        return bend;

        //任意位置的活载弯矩
    }
    vector<float> getLiveLoad_Sf(int beamId,float x){
    //任意位置的活载剪力
        vector<float> result;
        x/=1e3;//转为米
        float lj=cal_span;
        float Mc;
        float vmc=vmcffe();
        float flag=x>cal_span/2? -1:1;
        x=x>cal_span/2? cal_span-x:x;
        float R_left=1-x/lj;
        float R_right=x/lj;
        float V_max=R_left;
        float S=V_max*(lj-x)*0.5;
        float Smin=(1-R_left)*x*0.5;
        float Sf;
        float SfMin;

        Mc=getMcqByBeamId(beamId);

        if(x>=lj/4){
            //如果不要考虑横向分布系数变化;

          Sf=vmc*Mc*(1.2*pk*V_max+qk*S);





        }else{
            //考虑横向分布系数变化

            float m[4] = { 0,cal_span/4,cal_span -cal_span/4 ,cal_span };
            float n[4] = { getTDC_for_FulcrByBeamId(beamId),getMcqByBeamId(beamId),getMcqByBeamId(beamId),  getTDC_for_FulcrByBeamId(beamId) };
            BasicInterpolationTable  test(m, n, 4);
            float S3=0.75*0.75*lj*0.5;
               float b=V_max;
               float a=0.75;
               float h=lj/4-x;
               float S4=(a+b)*h/2;
                float CentroidX=h/3*(a+2*b)/(a+b)+x;
                float y=test.Interpolat(CentroidX);
                Sf=vmc*(1.2*pk*V_max*test.Interpolat(x)+qk*S3*Mc+qk*S4*y);



        }
         SfMin=vmc*Mc*(1.2*pk*(1-R_left)+qk*Smin);
        float sectionMax;
        float sectionMmin;
        sectionMax=Sf*flag>-SfMin*flag? Sf*flag:-SfMin*flag;
         sectionMmin=Sf*flag<-SfMin*flag? Sf*flag:-SfMin*flag;
        result.push_back(Sf*flag);
        result.push_back(-SfMin*flag);
        result.push_back(sectionMax);
         result.push_back(sectionMmin);


        return result;





    }
    vector<float> get_Sf_EffCombina(int beamId,float x);
    vector<float> liveLoadmcqSolve(int beamId) {
            float maxVehicleNum = getMaximumVehicle();
            float max=0;
            vector<float> res;
            for (int i = 1; i <= maxVehicleNum; i++)
            {
                       max=max>loadArrangement(beamId, i)? max:loadArrangement(beamId, i);

                    res.push_back(loadArrangement(beamId, i));
            }
            res.push_back(max);
            return res;

        }
    vector<float> getFulcrMcqsByBeamId(int beamId) {
        //求解支点截面的横向分布系数
            vector <float> res;
            if (beamId > (bean_nums + 1) / 2)
            {
                beamId = 1 + bean_nums - beamId;
            }

            float result;
            if ((beamId == 1)|(beamId == bean_nums))
            {
                float a[3] = { 0,fsm.s1 + fsm.s2,fsm.s1 + fsm.s2 + fsm.s3 };
                float b[3] = { 1,1,0 };
                //一号梁单列
                BasicInterpolationTable  mqcal(a, b, 3);
                result = 1.2*(mqcal.Interpolat(fsm.stl + 0.5) + mqcal.Interpolat(fsm.stl + 0.5 + 1.8)) / 2;
                res.push_back(result);
            }
            else
            {
                if ((beamId ==2)|(beamId ==(bean_nums-1)))
                {	//二号梁单列
                    float c[6] = { 0,fsm.s1 + fsm.s2,fsm.s1 + fsm.s2 + fsm.s3 ,fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4,fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4 + fsm.s5,fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4 + fsm.s5 + 1.8 };
                    float d[6] = { 0,0,1,1,0,0 };
                    BasicInterpolationTable mqcal(c, d, 6);
                    if (fsm.s3 < fsm.s5)
                    {
                        float left = fsm.s1 + fsm.s2 + fsm.s3;
                        result = 1.2*(mqcal.Interpolat(left) + mqcal.Interpolat(left + 1.8)) / 2;
                        res.push_back(result);
                    }
                    else {
                        float right = fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4;
                        result = 1.2*(mqcal.Interpolat(right) + mqcal.Interpolat(right - 1.8)) / 2;
                        res.push_back(result);
                    }
                    float tempVal;
                    //二号梁双列
                    float begain = fsm.s1 + fsm.s2;
                    bool isEnded = false;
                    tempVal = 0;
                    while (!isEnded)
                    {
                        float val1 = begain - 1.8;
                        float val2 = begain + 1.3;
                        float val3 = val2 + 1.8;
                        float temp = mqcal.Interpolat(val1) + mqcal.Interpolat(begain) + mqcal.Interpolat(val2) + mqcal.Interpolat(val3);
                        tempVal = tempVal > temp ? tempVal : temp;
                        begain += 0.1f;
                        if (begain >= fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4)
                        {
                            isEnded = true;
                        }
                    }
                    tempVal = tempVal * 0.5;
                    res.push_back(tempVal);


                }
                else
                {	//三号梁单列
                    float c[6] = { 0,fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4,fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4 + fsm.s5 ,fsm.s1 + fsm.s2 + fsm.s3 + 2 * fsm.s4 + fsm.s5,fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4 * 2 + fsm.s5 * 2 };

                    float d[6] = { 0,0,1,1,0,0 };
                    BasicInterpolationTable mqcal(c, d, 6);
                    float left = fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4 + fsm.s5;
                    result = 1.2*(mqcal.Interpolat(left) + mqcal.Interpolat(left + 1.8)) / 2;
                    res.push_back(result);
                    float tempVal;
                    //三号梁双列
                    float begain = fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4;
                    bool isEnded = false;
                    tempVal = 0;
                    while (!isEnded)
                    {
                        float val1 = begain - 1.8;
                        float val2 = begain + 1.3;
                        float val3 = val2 + 1.8;
                        float temp = mqcal.Interpolat(val1) + mqcal.Interpolat(begain) + mqcal.Interpolat(val2) + mqcal.Interpolat(val3);

                        tempVal = tempVal > temp ? tempVal : temp;
                        begain += 0.1f;
                        if (begain >= fsm.s1 + fsm.s2 + fsm.s3 + fsm.s4 * 2 + fsm.s5)
                        {
                            isEnded = true;
                        }
                    }
                    tempVal = tempVal * 0.5;
                    res.push_back(tempVal);




                }

            }

            float max=0;
            for(int i=0;i<res.size();i++){
                max=max>res[i]? max:res[i];

            }
            res.push_back(max);
            return res;



        }
    float get_Mr(float px,float s);
    float get_Vr(float px,float s);//单位力作用在px时对s截面产生的弯矩/剪力
    int getLeftBeamId(float s);//s截面位置;
    vector<float> get_Poq(int corss_bean_num);//返回横隔梁的计算剪力和弯矩时的荷载;
    float get_R(int i,float e);
     float get_R(int i,int j);//重载
     vector<float> getVr_At(float s);
     vector<float> getMr_At(float s,bool symb);
     float cross_bending(float s,bool symb);
     float cross_sf(float s);
     void cross_storge();
};


