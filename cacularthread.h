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
#include<beam.h>
#include<variables.h>
Q_DECLARE_METATYPE(vector<myPath>)
using namespace std;
class CacularThread : public QObject
{
    Q_OBJECT
public:
    explicit CacularThread(QObject *parent = nullptr);
    MainBeam *mymb=NULL;
    OrdinaryBrigeSection *myobs=NULL; //֧�����
    field_making_girder_beam *FulcrSideBeam=NULL;  //֧������
    field_making_girder_beam *FulcrMidBeam=NULL; //���б���
    field_making_girder_beam *mid_SpanSideBeam=NULL; //��������
    field_making_girder_beam *mid_SpanMidBeam=NULL;
    vector<myPath> paths;//ԤӦ���ֽ������
    float bridgeSpan; //�ܿ羶
    float Transationhalf_start;  //���ų����ɶ����;
    float Transtionhalf_end;  //���ų����ɶ��յ�;
     beam SDemonBeam;//����
     beam MDemonBeam;//����
    void SetBeam(beam vbeam,beam sbeam);

    float getPipSmoaSum(float x,float newCenterHeight ,bool precounted);
    float getPipStaticMomentSum(float x,bool precounted);
    float getPipAreaSum(bool precounted);
    void getThridLoad(float f,float x);
        vector<float> effCombinDataprocessor(int beanId,int saftyGrade);
        vector<float> task_7_Dataprocess(QVariant v,bool field_count,bool beamType);
        vector<float> getMaxSfd(int index);
        float steelAreaSolve(vector<float> sfdparam, float fpk, float ap);
        float getAverageSteelHeight(double x);
        vector<float> getSectionInfo(bool fieldCount,bool beamType,float x,bool precounted);
        float ApSolve(float fpk, float ap);
        float getPreSteelAreaSum();
        float getSigma_l4(float Ap,bool isMidBeam);
        float getSigma_l5(float Ap,bool isMidBeam);
        float getSigma_l6(float Ap,bool isMidBeam);
        vector<float> getAveSigma(float x);
        void prestrLossProcess(float Sx,bool beamType,int steelId);
         vector<float> get_Sf_CombinationAt(int beamId,float x,bool result_type);//x���洦�ļ�������ЧӦ���
        vector<float> get_M_CombinationAt(int beamId,float x);//x���洦���������ЧӦ���
        void get_Section_Combination(int beamId,float x);


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
    void Section_combinFinished(QVariant v);
    void Sectionfinished(QVariant v);
    void prestrLossFinished(QVariant v);
    void StageLoadFinished(QVariant v,bool foceType);
    void CombinationSolveFinished(QVariant v,QVariant W,bool foceType);

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
    void task_10_process(bool fieldCount,bool beamType,float x,bool precounted);
    void SigmaValuesSolve(int steelId,bool beamType,float x);
    void stageDeadloadSolve(int stage,bool foceType,bool beamType);
    void  CombinLoadSolve(int beamId,int combinType,bool foceType);
    vector<float>  PermanentCheak(int beamID,float x);//����״������
    vector<float> MainStressSolve(int beamId,float x);
};
#endif // CACULARTHREAD_H
