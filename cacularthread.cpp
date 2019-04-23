#pragma execution_character_set("utf-8")
#include "cacularthread.h"
#include<typeinfo>
using namespace std;
CacularThread::CacularThread(QObject *parent) : QObject(parent)
{

}
//信号处理函数--------------------------------
void CacularThread::task_1_process(QVariant mv, bool taskType)
{
vector<float> v=mv.value<vector<float>>();
half_box_girder myhbg(v[12],v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9],v[10],v[11],false);
small_box_girder mysbg(myhbg,myhbg);
vector<float> result;
    if(taskType){

        field_making_girder_beam fmgb(mysbg,v[13]);
        result.push_back(fmgb.Area);
        result.push_back(fmgb.SmoaSlove());
        result.push_back(fmgb.StaticMoment);
        result.push_back(fmgb.CentroidHeight);
        result.push_back(fmgb.SolveKx());
        result.push_back(fmgb.SolveKs());
        result.push_back(fmgb.getRho());

    }else{
        result.push_back(mysbg.AreaSolve());
        result.push_back(mysbg.SmoaSlove(mysbg.CentroidHeightSolve()));
        result.push_back(mysbg.StaticMomentSolve());
        result.push_back(mysbg.CentroidHeightSolve());
        result.push_back(mysbg.SolveKx());
        result.push_back(mysbg.SolveKs());
        result.push_back(mysbg.getRho());

    }
    QVariant data;
    data.setValue(result);
   emit onTask_1_finished(data);


}
void CacularThread::task_2_process(QVariant mv,QVariant mw, bool taskType){
vector<float> v=mv.value<vector<float>>();
vector<float> w=mw.value<vector<float>>();
    half_box_girder myhbg_l(v[12],v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9],v[10],v[11],true);
    half_box_girder myhbg_r(w[12],w[0],w[1],w[2],w[3],w[4],w[5],w[6],w[7],w[8],w[9],w[10],w[11],false);
    small_box_girder mysbg(myhbg_l,myhbg_r);
    vector<float> result;
    if(taskType){
        field_making_girder_beam fmgb(mysbg,v[13]);
        result.push_back(fmgb.Area);
        result.push_back(fmgb.SmoaSlove());
        result.push_back(fmgb.StaticMoment);
        result.push_back(fmgb.CentroidHeight);
        result.push_back(fmgb.SolveKx());
        result.push_back(fmgb.SolveKs());
        result.push_back(fmgb.getRho());

    }else{
        result.push_back(mysbg.AreaSolve());
        result.push_back(mysbg.SmoaSlove(mysbg.CentroidHeightSolve()));
        result.push_back(mysbg.StaticMomentSolve());
        result.push_back(mysbg.CentroidHeightSolve());
        result.push_back(mysbg.SolveKx());
        result.push_back(mysbg.SolveKs());
        result.push_back(mysbg.getRho());

    }
    QVariant data;
    data.setValue(result);
   emit onTask_2_finished(data);
}
void CacularThread::task_3_process(QVariant v,QVariant w,QVariant z,int type){
    //todo


}
void CacularThread::task_4_process(QVariant v){
    vector<float> input=v.value<vector<float>>();
    vector<float> ve;

    CarrigeWayPlate mycwp{input[0],input[1],input[2],input[3],0.2,0.6,input[4],input[5],input[6],input[7]};
    CarrigeWayPlateSolver mycwpsl(mycwp);
      float *temp1=mycwpsl.basicValueSolve();
    ve.push_back(temp1[4]);
    ve.push_back(temp1[3]);
    ve.push_back(temp1[6]);
    ve.push_back(temp1[5]);
    ve.push_back(temp1[7]);
    float *temp2=mycwpsl.CarrigeBendingSolve();
    ve.push_back(temp2[0]);
    ve.push_back(temp2[1]);
    ve.push_back(temp2[2]);
    ve.push_back(temp2[3]);
    ve.push_back(temp2[4]);
    float *temp3=mycwpsl.basicValueSolve(false);
    ve.push_back(temp3[4]);
    ve.push_back(temp3[3]);
    ve.push_back(temp3[6]);
    ve.push_back(temp3[5]);
    ve.push_back(temp3[7]);
    float *temp4=mycwpsl.CarrigeShearSolve();
    ve.push_back(temp4[0]);
    ve.push_back(temp4[1]);
    ve.push_back(temp4[2]);
   QVariant data;
   data.setValue(ve);
   emit onTask_4_finshed(data);


}
void CacularThread::task_5_process(QVariant v){
    vector<float> input=v.value<vector<float>>();
    vector<float> res;
     CantileverPlate myclp{input[0],input[1],input[3],input[2],input[4]};
    CantileverPlateSolver mycps(myclp);
    res.push_back(mycps.BendingSolver());
    res.push_back(mycps.ShearFoceSolver());
    QVariant data;
    data.setValue(res);
    emit onTask_5_finished(data);




}
void CacularThread::task_6_process(QVariant v){
   vector<float> input=v.value<vector<float>>();
    vector<float> result;
    float *temp;
   MainBeamData mymbd{input[0],input[1],input[2],input[3],input[4],input[5],input[6],input[7],input[8],input[9],input[10],input[11],input[12],input[13],input[14]};
    mymb=new MainBeam(mymbd);
   temp=mymb->firstStageLoad();
   result.push_back(temp[0]);
    result.push_back(temp[1]);
    temp=mymb->secondStageLoad();
    result.push_back(temp[0]);
     result.push_back(temp[1]);

     result.push_back(mymb->ThirdStageLoad());
     QVariant data;
     data.setValue(result);
        emit onTask_6_finished(data);

}
void CacularThread::task_7_process(QVariant v,bool field_count,bool beamType){

    QVariant data;
    data.setValue(task_7_Dataprocess( v,field_count,beamType));
    emit result_1_render(data,"tableWidget_16");
}
void CacularThread::task_8_process(float x,float foce1,float foce2){
    vector<float> res;

    res.push_back(mymb->getMainBeanShearFoce(foce1,x));
    res.push_back(mymb->getMainBeanBending(foce1,x));
    res.push_back(mymb->getMainBeanShearFoce(foce2,x));
    res.push_back(mymb->getMainBeanBending(foce2,x));
    QVariant data;
    data.setValue(res);
    emit onTask_8_finished(data);

}
void CacularThread::task_9_process(QVariant v){
   vector<float> input=v.value<vector<float>>();
   vector<float> sf;
   vector<float> bending;

    //OrdinaryBrigeSection
      myobs=new OrdinaryBrigeSection(*mid_SpanSideBeam,*mid_SpanMidBeam,FulcrSideBeam,FulcrMidBeam,input[0],input[1],input[2]);
    myobs->InitFsm();


     sf=myobs->getLivaLoadSF(input[3]);
     bending=myobs->getLivaLoadBending(input[3]);


    bending.push_back(sf[0]);
    bending.push_back(sf[1]);
    bending.push_back(sf[2]);
    QVariant data;
    data.setValue(bending);

    emit onTask_9_finished(data);
    vector<float> fculmcqs;
    vector<float> midmcqs;
    fculmcqs=myobs->getFulcrMcqsByBeamId(input[3]);
    midmcqs=myobs->liveLoadmcqSolve(input[3]);
    QVariant data2;
    QVariant data3;
    data2.setValue(fculmcqs);
    data3.setValue(midmcqs);
    emit mcqRender(data2,"tableWidget_19");
     emit mcqRender(data3,"tableWidget_20");

}
void CacularThread::getThridLoad(float f,float x){
    vector<float> a;
    QVariant data;
   a.push_back(CacularThread::mymb->getMainBeanShearFoce(f,x));
    a.push_back(CacularThread::mymb->getMainBeanBending(f,x));
    data.setValue(a);
    emit thridLoadFinished(data);

}
void CacularThread::effCombin(int beanId,int saftyGrade){
    vector<float> result=effCombinDataprocessor(beanId,saftyGrade);
    QVariant data;
    data.setValue(result);
    emit eff_combinFinished(data);
}


//数据处理函数-----------------------------------------------
vector<float> CacularThread::effCombinDataprocessor(int beanId, int saftyGrade)
{
    bool isSideBeam=(beanId==1|beanId==mymb->mbd.mianBeanNum)? true:false;

    setSaftyGrade(saftyGrade);
    //一期
  float *p1=mymb->firstStageLoad();
  //二期
  float *p2=mymb->secondStageLoad();
  //三期
  float p3=mymb->ThirdStageLoad();
  //三期求和
  //指三期恒载集度之和
  vector<float> loadSum;
  //指恒载产生的各界面弯矩和剪力;
  vector<float> dieLoad;
  //计冲击系数
  vector<float> impacCountedLiveload;
  //不计冲击系数
   vector<float> Liveload;
  loadSum.push_back(p1[0]+p2[0]+p3);
  loadSum.push_back(p1[1]+p2[1]+p3);
  if(isSideBeam){
      dieLoad.push_back(mymb->getMainBeanBending(loadSum[0],mymb->mbd.CalculaSpan/2));
      dieLoad.push_back(mymb->getMainBeanBending(loadSum[0],mymb->mbd.CalculaSpan/4));
      dieLoad.push_back(mymb->getMainBeanShearFoce(loadSum[0],mymb->mbd.CalculaSpan/2));
      dieLoad.push_back(mymb->getMainBeanShearFoce(loadSum[0],mymb->mbd.CalculaSpan/4));
      dieLoad.push_back(mymb->getMainBeanShearFoce(loadSum[0],0));




  }else{
      dieLoad.push_back(mymb->getMainBeanBending(loadSum[1],mymb->mbd.CalculaSpan/2));
      dieLoad.push_back(mymb->getMainBeanBending(loadSum[1],mymb->mbd.CalculaSpan/4));
      dieLoad.push_back(mymb->getMainBeanShearFoce(loadSum[1],mymb->mbd.CalculaSpan/2));
      dieLoad.push_back(mymb->getMainBeanShearFoce(loadSum[1],mymb->mbd.CalculaSpan/4));
      dieLoad.push_back(mymb->getMainBeanShearFoce(loadSum[1],0));

  }
  //指定梁号活载弯矩;
  vector<float> temp1=myobs->getLivaLoadBending(beanId);

  //指定梁号活载剪力
  vector<float> temp2=myobs->getLivaLoadSF(beanId);

  impacCountedLiveload.push_back(temp1[2]);
  impacCountedLiveload.push_back(temp1[1]);
  impacCountedLiveload.push_back(temp2[2]);
  impacCountedLiveload.push_back(temp2[1]);
  impacCountedLiveload.push_back(temp2[0]);
  //冲击系数
    float vecf=myobs->vmcffe();


    for(int i=0;i<impacCountedLiveload.size();i++){
        Liveload.push_back(impacCountedLiveload[i]/vecf);


    }

    //基本组合结果
    vector<float> basicCombin;
    //频遇组合
    vector<float> frequeCombin;
    //准永久组合
    vector<float> quasiCombin;
    for(int i=0;i<dieLoad.size();i++){
       basicCombin.push_back(ultimateLimitSta(dieLoad[i],impacCountedLiveload[i]));
       frequeCombin.push_back(sfd(dieLoad[i],impacCountedLiveload[i]));
       quasiCombin.push_back(sqd(dieLoad[i],impacCountedLiveload[i]));
    }
    vector<float> result;
    result.insert(result.end(),dieLoad.begin(),dieLoad.end());
    result.insert(result.end(),impacCountedLiveload.begin(),impacCountedLiveload.end());
    result.insert(result.end(),Liveload.begin(),Liveload.end());
    result.insert(result.end(),basicCombin.begin(),basicCombin.end());
    result.insert(result.end(),frequeCombin.begin(),frequeCombin.end());
    result.insert(result.end(),quasiCombin.begin(),quasiCombin.end());
    return result;
}
vector<float> CacularThread::getMaxSfd(int index){
    vector<float> result;
    result.push_back(1);
    result.push_back(0);
    vector<float> cheakFor;
    for(int i=1;i<=myobs->bean_nums;i++){
        cheakFor=effCombinDataprocessor(i,1);

        result[1]=cheakFor[index]>result[1]? cheakFor[index]:result[1];
        result[0]=cheakFor[index]>result[1]? result[index]:i;


    }
    return result;

}
float CacularThread::steelAreaSolve(vector<float> sfdparam,float fpk,float ap){

    bool isSideBeam=false;
    //梁面积
    float beamArea;
    //重心位置
    float centerHeight;
    //惯性矩
    float smoa;
    float ep;
    float W;
    small_box_girder mysmbg;

    if((int)sfdparam[0]==1|(int)sfdparam[0]==myobs->bean_nums){
       isSideBeam=true;
    }
    if(isSideBeam){
      mysmbg=myobs->side_main_bean.sbg;



    }else{

       mysmbg= myobs->mid_main_bean.sbg;
    }
    beamArea=mysmbg.AreaSolve()*pow(10,6);
    centerHeight=mysmbg.CentroidHeightSolve();
    smoa=mysmbg.SmoaSlove(centerHeight)*pow(10,12);
    centerHeight*=1000;
    ep=centerHeight-ap;
    W=smoa/centerHeight;
    float npe=(sfdparam[1]*pow(10,6)/W-0.7*2.65)/(1/beamArea+ep/W);
   float as=npe/((1-0.2)*0.75*fpk);
    return as;





}
float CacularThread::getAverageSteelHeight(double x)
{   float heightSum;
    for(int i=0;i<paths.size();i++){
        heightSum+=paths[i].getYvalue(x);
    }
    return heightSum/paths.size();

}
vector<float> CacularThread::getNewBeamSize(float I, float S, float b, float h)
{
    //todo
    vector<float> a;
    return a;
}
float CacularThread::ApSolve(float fpk, float ap)
{
   return steelAreaSolve(getMaxSfd(20), fpk, ap);

}
vector<float> CacularThread::task_7_Dataprocess(QVariant v,bool field_count,bool beamType)
{
    vector<float> input=v.value<vector<float>>();
    float halfLength=input[3]-input[2];
    float xe=input[1]>input[0]/2? input[0]-input[1]:input[1];
    if(xe<=input[2]){

        xe=0;
    }else{
        if(xe<input[3]){

            xe=xe-input[2];
        }else{

            xe=halfLength;
        }
    }
    float field_length1;
    float field_length2;
    vector<float> half_left;
    vector<float> half_right;
    vector<float> fulc_left;
    vector<float> fulc_right;
    vector<float> mid_left;
    vector<float> mid_right;
    vector<float> result;
    half_box_girder myhbg1;
    half_box_girder myhbg2;
    if(!beamType){
     //边梁
      fulc_left=FulcrSideBeam->sbg.left.getInfo();

      fulc_right=FulcrSideBeam->sbg.right.getInfo();
      mid_left=mid_SpanSideBeam->sbg.left.getInfo();
      mid_right=mid_SpanSideBeam->sbg.right.getInfo();
      field_length1=FulcrSideBeam->fmgb_length;
      field_length2=mid_SpanSideBeam->fmgb_length;


    }else{
        //中梁

        fulc_left=FulcrMidBeam->sbg.left.getInfo();
        fulc_right=FulcrMidBeam->sbg.right.getInfo();
        mid_left=mid_SpanMidBeam->sbg.left.getInfo();
        mid_right=mid_SpanMidBeam->sbg.right.getInfo();
        field_length1=FulcrMidBeam->fmgb_length;
        field_length2=mid_SpanMidBeam->fmgb_length;

    }
    for(int i=0;i<fulc_left.size();i++){

      float temp1;
      float temp2;
      temp1=fulc_left[i]+(mid_left[i]-fulc_left[i])*(xe/halfLength);
      temp2= fulc_right[i]+(mid_right[i]-fulc_right[i])*(xe/halfLength);
      half_left.push_back(temp1);
      half_right.push_back(temp2);


    }

    if(!beamType){
        myhbg1=half_box_girder(half_left,true);
        myhbg2= half_box_girder (half_right,false);

    }else{
        myhbg1=half_box_girder(half_left,false);
        myhbg2= half_box_girder (half_right,false);

    }
    float fl=field_length1+(field_length2-field_length1)*(xe/halfLength);
    small_box_girder mysbg(myhbg1,myhbg2);
    if(field_count){

        field_making_girder_beam fmgb(mysbg,fl);
        result.push_back(fmgb.Area);
        result.push_back(fmgb.SmoaSlove());
        result.push_back(fmgb.StaticMoment);
        result.push_back(fmgb.CentroidHeight);
        result.push_back(fmgb.SolveKx());
        result.push_back(fmgb.SolveKs());
        result.push_back(fmgb.getRho());

    }else{
        result.push_back(mysbg.AreaSolve());
        result.push_back(mysbg.SmoaSlove(mysbg.CentroidHeightSolve()));
        result.push_back(mysbg.StaticMomentSolve());
        result.push_back(mysbg.CentroidHeightSolve());
        result.push_back(mysbg.SolveKx());
        result.push_back(mysbg.SolveKs());
        result.push_back(mysbg.getRho());
    }

    return result;

}
