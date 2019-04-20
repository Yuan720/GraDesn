#include<iostream> 
#include"bridge.h"
#include<Eigen/Dense>
#include<string>
#include<CarriagewayPlate.h>
#include<fstream>
#include<MBIF.h>
#include<vector>

using namespace std;
using namespace Eigen;
void demon_fun1(half_box_girder zl1, half_box_girder zl2, float  field_making_width) {

	//跨中边梁
	small_box_girder sbg(zl1, zl2);
	field_making_girder_beam mytest(sbg, field_making_width);
	//跨中中梁
	small_box_girder sbgzl(zl1, zl1);
	field_making_girder_beam zltest(sbgzl, field_making_width);
	cout << "跨中边梁" << endl;
	cout << "预制主梁(不含现浇段)" << endl;
	cout << "主梁面积:" << sbg.AreaSolve() << endl;
	cout << "主梁总静矩:" << sbg.StaticMomentSolve() << endl;
	cout << "形心高度:" << sbg.CentroidHeightSolve() << endl;
	cout << "主梁惯性矩:" << sbg.SmoaSlove(sbg.CentroidHeightSolve()) << endl;
	cout << "主梁截面效率指标:" << sbg.getRho() << endl;
	cout << "抗扭惯性矩:" << sbg.torsionalMomentInertiaSolve() << endl;
	if (sbg.isValid())
	{
		cout << endl << endl << endl << "截面效率指标合格" << endl;
	}
	else {
		cout << endl << endl << endl << "截面效率指标不合格" << endl;
	}

	cout << endl << endl << endl << "含现浇段" << endl;
	cout << "主梁面积:" << mytest.Area << endl;
	cout << "主梁总静矩" << mytest.StaticMoment << endl;
	cout << "形心高度:" << mytest.CentroidHeight << endl;
	cout << "主梁惯性矩:" << mytest.getRho() << endl;
	cout << "主梁截面效率指标:" << mytest.getRho() << endl;
	if (mytest.isValid())
	{
		cout << endl << endl << endl << "截面效率指标合格" << endl;
	}
	else {
		cout << endl << endl << endl << "截面效率指标不合格";
	}
	if (!mytest.sbg.left.isSideBeam & !mytest.sbg.right.isSideBeam)
	{
		cout << "行车道板:" << endl;
		float *p = mytest.getSpanLength();
		cout << "行车道板净跨径:" << p[0] << endl;
		cout << "行车道板等效厚度" << p[2] << endl;

	}

	//跨中中梁
	cout << endl << endl << "跨中中梁" << endl;
	cout << "预制主梁(不含现浇段)" << endl;
	cout << "主梁面积:" << sbgzl.AreaSolve() << endl;
	cout << "主梁总静矩:" << sbgzl.StaticMomentSolve() << endl;
	cout << "形心高度:" << sbgzl.CentroidHeightSolve() << endl;
	cout << "主梁惯性矩:" << sbgzl.SmoaSlove(sbgzl.CentroidHeightSolve()) << endl;
	cout << "主梁截面效率指标:" << sbgzl.getRho() << endl;
	cout << "抗扭惯性矩:" << sbgzl.torsionalMomentInertiaSolve() << endl;
	if (sbgzl.isValid())
	{
		cout << endl << endl << endl << "截面效率指标合格" << endl;
	}
	else {
		cout << endl << endl << endl << "截面效率指标不合格" << endl;
	}

	cout << endl << endl << endl << "含现浇段" << endl;
	cout << "主梁面积:" << zltest.Area << endl;
	cout << "主梁总静矩" << zltest.StaticMoment << endl;
	cout << "形心高度:" << zltest.CentroidHeight << endl;
	cout << "主梁惯性矩:" << zltest.getRho() << endl;
	cout << "主梁截面效率指标:" << zltest.getRho() << endl;
	if (zltest.isValid())
	{
		cout << endl << endl << endl << "截面效率指标合格" << endl;
	}
	else {
		cout << endl << endl << endl << "截面效率指标不合格";
	}

}
void demon_fun2(CarrigeWayPlate cwp, CantileverPlate clp) {
	CarrigeWayPlateSolver cwps(cwp);
	float *re = cwps.CarrigeBendingSolve();
	cout << "行车道板" << endl;
	cout << "Mg0:" << re[1] << endl;
	cout << "M0q:" << re[0] << endl;
	cout << "M中:" << re[3] << endl;
	cout << "M支:" << re[4] << endl;
	cout << endl << endl << endl;
	cout << "悬壁板" << endl;
	CantileverPlateSolver clps(clp);
	cout << "悬壁板弯矩:" << clps.BendingSolver() << endl;
	cout << "悬壁板剪力:" << clps.ShearFoceSolver() << endl;



}
void demon_fun3(MainBeamData mymbd, FulcrumSectionMq myfsmq) {
	MainBeam mymainbean(mymbd, myfsmq);
	cout << "一期荷载集度" << endl;
	cout << "边梁" << mymainbean.firstStageLoad()[0] << endl;
	cout << "中梁" << mymainbean.firstStageLoad()[1] << endl;
	cout << "二期荷载集度" << endl;
	float *temp = mymainbean.secondStageLoad();
	cout << "边梁" << temp[0] << endl;
	cout << "中梁" << temp[1] << endl;
	cout << "铺装层" << temp[2] << endl;
	/*float *myre = mymainbean.getTDC_for_FulcrumSection();
	cout << "支点截面横向分布系数" << endl;
	cout << "1#梁单列" << myre[0] << endl;
	cout << "2#梁单列" << myre[1] << endl;
	cout << "2#梁双列" << myre[2] << endl;*/
	cout << mymainbean.getTDC_for_FulcrByBeamId(1) << endl;
	cout << mymainbean.getTDC_for_FulcrByBeamId(2) << endl;
	cout << mymainbean.getTDC_for_FulcrByBeamId(3) << endl;
	cout << mymainbean.getTDC_for_FulcrByBeamId(4) << endl;

}
int main(void) {


	half_box_girder kzzl(1.6, 0.2, 0.2, 0.2, 1.2, 0.5, 0.1975, 0.05, 0.165, 0.05, 0.05, 0.05, 4, false);
	half_box_girder kzbl(1.6, 0.2, 0.2, 0.2, 1.65, 0.5, 0.8125, 0.05, 0.165, 0.05, 0.05, 0.05, 4, true);
	small_box_girder smbl(kzbl, kzzl);
	small_box_girder smzl(kzzl, kzzl);
	field_making_girder_beam bl(smbl, 0.25);
	field_making_girder_beam zl(smzl, 0.25);
	OrdinaryBrigeSection test(bl, zl, 4, 29, 0.5);


	//cout << test.vmcffe() << endl;
//	vector<float> demon = test.getFsm();
	//vector<float> dem = test.aid;
	//vector<float> dem2 = test.moi_field_included;
//	cout << demon[0] << endl
		//<< demon[1] <<endl << demon[2] << endl << demon[3] <<endl << demon[4] << endl << demon[5] << endl;
	//cout << test.getVectorSum(demon, 1, demon.size()) << endl;
	//cout << test.vmcffe() << endl;
//	cout << dem[0] << endl << dem[1] << endl << dem[2] << endl << dem[3] << endl<<endl;

//	cout << dem2[0] << endl << dem2[1] << endl << dem2[2] << endl << dem2[3] << endl;
	demon_fun1(kzzl, kzbl, 0.25);
	CarrigeWayPlate mycwp{ 0.08,0.1,1.468447,0.2124634,0.2,0.6,0.2,1.4,140,1.6 };
	CantileverPlate myclp{ 0.08,0.1,kzbl.getCantileverPlate_info()[1],kzbl.getCantileverPlate_info()[0] };
	demon_fun2(mycwp, myclp);
	MainBeamData mymbd{ 12,0.08,0.1,1.20477,1.22015,0.11814,0.1079176,34.5,5,5,4,0.05,0.299235,0.0625,0.2 };
	FulcrumSectionMq myflsm{ 0.9917377,1.3407671,1.5592329,1.3407671,1.5592329 };
	//test.SetFsm(myflsm);
	//vector<float> a = test.getLivaLoadSF(1);
	//vector<float> a = test.getLivaLoadBending(1);
	//cout <<a[0] <<endl << a[1] << endl << a[2] << endl;

	demon_fun3(mymbd, myflsm);
	/*ofstream mofs;
	mofs.open("test.dat");
	double a = 3.14159265358;
	mofs << "test ahaha" << endl<<a<<endl;
	mofs.close();*/
	/*float a[3] = { 0,245,420 };
	float b[3] = { 1,1,0 };
	BasicInterpolationTable  test(a, b, 3);


	cout << test.Interpolat(420) << endl;
	/*CarrigeWayPlate te{ 0.08,0.10,1.280,0.204,0.2,0.6,0.2,1.4,140,1.6 };*/

	/*float a[4] = { 1,2,3,4 };
	float b[4] = {0.5,0.2,0.6,0.1};
	BasicInterpolationTable  test(a,b,4);
	cout<<test.Interpolat(2.2)<<endl;
	b[2] = 0.5;
	cout << test.Interpolat(2.2) << endl;*/

	system("pause");



}


