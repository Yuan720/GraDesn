#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include<QTableWidget>
#include<cacularthread.h>
#include<QDebug>
#include<QMessageBox>
#include<QThread>
#include<QTableWidgetItem>
#include<vector>
#include<QVariant>
#include<XxwCustomPlot.h>
#include<qcustomplot.h>
#include<mytoolkit.h>
#include<variables.h>
#include<beam.h>
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool inputCheak(QString tb);
    void tableInit(QString tb);
    void ResTableInit(QString tb);
    void generalTableInit(QString tb);
    QVariant generalTableInput(QString tb);
    bool generalTableCheak(QString tb,int itemCount);
    QVariant getGeneralTableData(QString tb,int itemCount);
    void resultDisplay(vector<float> v,QString tbname);
    void result_1_display(QVariant v);
    void result_2_display(QVariant v);
    void resultRender1(QVariant v,QString tbname);
    void resultRender2(QVariant v);
    void resultRender3(QVariant v);
    void resultRender4(QVariant v);
    void eff_combinRender(QVariant v);
    void Section_combinRender(QVariant V);
    void renderThridLoad(QVariant v );
    void getIputDataByTable(QString tableName);
    bool lineEditCheak(QString eName);
    vector<float> tableDataProcess(QString tableName);
    float lineEditDataProcess(QString leID);
    void renderCantilever(QVariant v);
    void render4(QVariant v);
    void pagePrepare();
    void beamInit();
    void Steelplot(myPath path);
    bool tableItemCheak(QTableWidgetItem *t);
    void Init_table_189();
    void prestrLossSolve();
    void SectionSolve(int x);
    void draw_liveLoadM(int beamId);
    void draw_liveLoadSf(int beamId);
    void draw_deadLoadFoce(bool beamtype,bool foceType);
    void deadLoadRequest();
    void getCombinLoadFoce();
signals:
    void task_1_send(QVariant v,bool taskType);
    void task_2_send(QVariant v,QVariant w,bool taskType);
    void task_3_send(QVariant v,QVariant w,QVariant z,int type);
    void task_4_send(QVariant v);
    void task_5_send(QVariant v);
    void task_6_send(QVariant v);
    void task_7_send(QVariant v,bool field_count,bool beamType);
    void task_8_send(float x,float foce1,float foce2);
    void task_9_send(QVariant v);
    void thridStageLoad(float f,float x);
    void nonPreStreSolve(float a,float fpd,float Ap);
    void SectionCompute(bool fieldCount,bool beamType,float x,bool precounted);
    void prestrLossRq(float Sx,bool beamType,int steelId);
    void Section_Eff_Combin(int beamId,float x);//信号获取指定截面效应组合请求
    void getSigmaValues(int steelId,bool beamType,float x);//请求截面x处的指定钢束的预应力损失值
    void RequstForDeadLoad(int stage,bool foceType,bool beamType);
    void RequestForCombinLoad(int beamId,int CombinType,bool foceType);
private slots:

    void task_4_finished(QVariant v);
    void CarrigeresultRender(QVariant v);
    void mcqRender(QVariant v,QString tbName);
    void on_listWidget_currentRowChanged(int currentRow);
    void on_commandLinkButton_clicked();
    void on_commandLinkButton_4_clicked();
    void on_listWidget_2_currentRowChanged(int currentRow);
    void on_commandLinkButton_2_clicked();
    void on_checkBox_3_stateChanged(int arg1);
    void on_commandLinkButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_spinBox_2_valueChanged(int arg1);
    void on_comboBox_4_currentIndexChanged(int index);
    void on_commandLinkButton_6_clicked();
    void on_commandLinkButton_49_clicked();
    void on_commandLinkButton_50_clicked();
    void on_commandLinkButton_51_clicked();
    void on_pushButton_2_clicked();
    void on_commandLinkButton_3_clicked();
    void on_commandLinkButton_52_clicked();
    void on_tableWidget_189_cellClicked(int row, int column);
    void on_lineEdit_editingFinished();
     void on_comboBox_34_currentIndexChanged(int index);
    void on_comboBox_5_currentIndexChanged(int index);
    void on_pushButton_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_horizontalSlider_valueChanged(int value);
    void on_checkBox_3_clicked();
    void on_comboBox_3_currentIndexChanged(int index);
    void on_spinBox_5_valueChanged(int arg1);
    void on_comboBox_7_currentIndexChanged(int index);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_comboBox_8_currentIndexChanged(int index);
    void on_horizontalSlider_5_valueChanged(int value);
    void draw__StagedeadLoadFoce(QVariant data,bool foceType);//绘制三期恒载内力图像
    void on_pushButton_9_clicked();
    void on_comboBox_18_currentIndexChanged(const QString &arg1);
    void on_comboBox_17_currentIndexChanged(int index);
    void draw_CombinLoadFoce(QVariant data,QVariant W,bool foceType);
    void on_comboBox_25_currentIndexChanged(int index);
    void on_comboBox_19_currentIndexChanged(int index);

    void prestrLossRender(QVariant data);

    void on_horizontalSlider_5_actionTriggered(int action);
    void  draw_prestrLossP();
    void  draw_prestrLossL();
    void draw_Mu(int beamID);
    void draw_oblique_sf(int beamID);
    void drawSigmaCt(int beamId);
    void drawPermanentStress(int beamId);
    void draw_MainStress(int beamId);
    void drawfocescheaking(int beamId);//对所用应力验算进行绘图;

    void on_spinBox_16_valueChanged(int arg1);
    void mainBeamdeadLoad();


private:
    bool box_1_cheaked=false;
    bool box_2_cheaked=false;
    bool box_3_cheaked=false;
    Ui::MainWindow *ui;
    CacularThread *mycat;
    QThread *mqt;
    XxwCustomPlot  *m_customPlot;
    QCustomPlot  *myplot;

};



#endif // MAINWINDOW_H
