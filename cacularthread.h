#pragma execution_character_set("utf-8")
#pragma once
#ifndef CACULARTHREAD_H
#define CACULARTHREAD_H
#include<QObject>
#include<vector>
#include<QVariant>
#include<CarriagewayPlate.h>
#include<bridge.h>
#include<QDebug>
#include<QThread>
#include<MBIF.h>
#include<Casoa.h>
#include<math.h>
#include<mytoolkit.h>
Q_DECLARE_METATYPE(vector<myPath>)
using namespace std;
class CacularThread : public QObject
{
    Q_OBJECT
public:
    explicit CacularThread(QObject *parent = nullptr);
    MainBeam *mymb=NULL;
    OrdinaryBrigeSection *myobs=NULL;
    //支点边梁
    field_making_girder_beam *FulcrSideBeam=NULL;
    //支点中梁
    field_making_girder_beam *FulcrMidBeam=NULL;
        //跨中边梁
    field_making_girder_beam *mid_SpanSideBeam=NULL;
     //跨中中梁
    field_making_girder_beam *mid_SpanMidBeam=NULL;
    vector<myPath> paths;


    //总跨径
    float bridgeSpan;
    //半桥长过渡段起点和终点;
    float Transtionhalf_start;
    float Transtionhalf_end;
        float getPipSmoaSum(float x,float newCenterHeight );
        float getPipStaticMomentSum(float x);
        float getPipAreaSum();
        void getThridLoad(float f,float x);
        void effCombin(int beanId,int saftyGrade);
        vector<float> effCombinDataprocessor(int beanId,int saftyGrade);
        vector<float> task_7_Dataprocess(QVariant v,bool field_count,bool beamType);
        vector<float> getMaxSfd(int index);


        float steelAreaSolve(vector<float> sfdparam, float fpk, float ap);
        float getAverageSteelHeight(double x);
        vector<float> getSectionInfo(bool fieldCount,bool beamType,float x);
        float ApSolve(float fpk, float ap);
signals:
    void onTask_1_finished(QVariant v);
    void onTask_2_finished(QVariant v);
    void onTask_4_finshed(QVariant v);
    void onTask_5_finished(QVariant v);
    void result_1_render(QVariant v,QString tbname);
    void onTask_6_finished(QVariant v);
    void onTask_8_finished(QVariant v);
    void onTask_9_finished(QVariant v);
    void mcqRender(QVariant v,QString tbName);
    void thridLoadFinished(QVariant v);
    void eff_combinFinished(QVariant v);
    void Sectionfinished(QVariant v);

public slots:
    void task_1_process(QVariant v,bool taskType);
    void task_2_process(QVariant v,QVariant w,bool taskType);
    void task_3_process(QVariant v,QVariant w,QVariant z,int type);
    void task_4_process(QVariant v);
    void task_5_process(QVariant v);
    void task_6_process(QVariant v);
    void task_7_process(QVariant v,bool field_count,bool beamType);
    void task_8_process(float x,float foce1,float foce2);
    void task_9_process(QVariant v);
    void task_10_process(bool fieldCount,bool beamType,float x);
};

#endif // CACULARTHREAD_H
