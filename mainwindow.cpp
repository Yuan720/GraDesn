#pragma execution_character_set("utf-8")
#pragma once
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QWidget>
#include<QPainter>
using namespace std;
Q_DECLARE_METATYPE(std::vector<float>)
Q_DECLARE_METATYPE(QVector<double>)
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mycat=new CacularThread;
    mqt=new QThread(this);
    mycat->moveToThread(mqt);
    mqt->start();



}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_checkBox_3_stateChanged(int arg1)
{if(arg1==2)
    {
        box_3_cheaked=true;
    }else{
       box_3_cheaked=false;
    }
}
void MainWindow::on_listWidget_2_currentRowChanged(int currentRow)
{
    ui-> stackedWidget_2->setCurrentIndex(currentRow);

}

void MainWindow::on_commandLinkButton_clicked()
{
    if( generalTableCheak("tableWidget_6",8)){


      QVariant task=getGeneralTableData ("tableWidget_6",8);
      emit task_4_send(task);;
     }else{
    QMessageBox::information(this,QString("错误"),QString("输入数据不完整或输入数据类型有误!\n请检查后提交!"));
    }
}
void MainWindow::on_commandLinkButton_2_clicked()
{
    if(mycat->mymb==0){
        qDebug()<<"悬壁板求解参数不全!line--102";
        return;
    }
    if(generalTableCheak("tableWidget_7",3)){
      QVariant  data=generalTableInput("tableWidget_7");
        vector<float> temp=data.value<vector<float>>();
        temp.insert(temp.begin(),mycat->mymb->mbd.MixedSoilLayer);
        temp.insert(temp.begin(),mycat->mymb->mbd.AspLayer);
        data.setValue(temp);
        emit task_5_send(data);



   }else{

        QMessageBox::information(this,QString("错误"),QString("输入数据不完整或输入数据类型有误!\n请检查后提交!"));

    }
}

void MainWindow::task_4_finished(QVariant v){

    //todo
}
bool MainWindow::inputCheak(QString tb){
     QTableWidget  *qtw=this->findChild< QTableWidget*>(tb);
     bool isInvalid;
     for(int i=1;i<6;i+=2){
         for(int j=0;j<4;j++){
                QString qs=qtw->item(i,j)==0? " ":qtw->item(i,j)->text();
                qs.toFloat(&isInvalid);
           if(!isInvalid){
               return isInvalid;
           }

         }
     }


  {
        QString qs=qtw->item(7,0)==0? " ":qtw->item(7,0)->text();
        qs.toFloat(&isInvalid);
        if(isInvalid){
            return true; }
        else
        {
        return false;
        }

     }
}
bool MainWindow::lineEditCheak(QString eName){
  QLineEdit  *qtw=this->findChild< QLineEdit*>(eName);

  QString qs=qtw->text();

  bool isInvalid;
  qs.toFloat(&isInvalid);
  return isInvalid;

}
void MainWindow::tableInit(QString tb){
//禁用表格条目的编辑
       QTableWidget  *qtw=this->findChild< QTableWidget*>(tb);

      for(int i=0;i<8;i+=2){
          for(int j=0;j<4;j++){
                 QString qs=qtw->item(i,j)==0? " ":qtw->item(i,j)->text();
              QTableWidgetItem *it=new  QTableWidgetItem();
               it->setText(qs);
               it->setFlags(Qt::ItemIsEditable);
               qtw->setItem(i,j,it);



          }

      }

}
void MainWindow::ResTableInit(QString tb){
     QTableWidget  *qtw=this->findChild< QTableWidget*>(tb);
    for(int i=0;i<=6;i++){
        QString qs=qtw->item(i,0)==0? " ":qtw->item(i,0)->text();
         QTableWidgetItem *it=new  QTableWidgetItem();
         it->setText(qs);
         it->setFlags(Qt::ItemIsEditable);
         qtw->setItem(i,0,it);
    }
}
void MainWindow::generalTableInit(QString tb){
    QTableWidget  *qtw=this->findChild< QTableWidget*>(tb);
    for(int j=0;j<qtw->columnCount();j+=2){
        for(int i=0;i<qtw->rowCount();i++){
            QString qs=qtw->item(i,j)==0? " ":qtw->item(i,j)->text();

             QTableWidgetItem *it=new  QTableWidgetItem();
             it->setText(qs);
             it->setFlags(Qt::ItemIsEditable);
             qtw->setItem(i,j,it);

        }
    }
}
QVariant MainWindow::generalTableInput(QString tb){
vector<float> result;
QVariant data;
QTableWidget  *qtw=this->findChild< QTableWidget*>(tb);
for(int j=1;j<qtw->columnCount();j+=2){
    for(int i=0;i<qtw->rowCount();i++){
       if(!qtw->item(i,j)==0){
        result.push_back(qtw->item(i,j)->text().toFloat());

       }
        }
    data.setValue(result);
}
return data;

}
bool MainWindow::generalTableCheak(QString tb,int itemCount){
    QTableWidget  *qtw=this->findChild< QTableWidget*>(tb);
    bool isInvalid;
    int cheakedCount=0;
    for(int j=1;j<qtw->columnCount();j+=2){
        for(int i=0;i<qtw->rowCount();i++){
            QString qs=qtw->item(i,j)==0? " ":qtw->item(i,j)->text();
            cheakedCount++;
            if(cheakedCount<=itemCount){
               qs.toFloat(&isInvalid);
            }
            if(!isInvalid){

              return isInvalid;
            }


    }

}

        return isInvalid;

}
QVariant MainWindow::getGeneralTableData(QString tb,int itemCount){
    QVariant data;
    vector<float> inputData;
    QTableWidget  *qtw=this->findChild<QTableWidget*>(tb);
    int cheakedCount=0;
    for(int j=1;j<qtw->columnCount();j+=2){
        for(int i=0;i<qtw->rowCount();i++){
            QString qs=qtw->item(i,j)==0? " ":qtw->item(i,j)->text();
            cheakedCount++;
            if(cheakedCount<=itemCount){
              inputData.push_back( qs.toFloat());
            }



    }

}
    data.setValue(inputData);
    return data;

}
void MainWindow::renderCantilever(QVariant v){

    vector<float> input=v.value<vector<float>>();
    QTableWidget *tb=ui->tableWidget_13;
    QTableWidgetItem *it1=new QTableWidgetItem();
    it1->setText(QString("%1").arg(input[0]*1.2f));
    QTableWidgetItem *it2=new QTableWidgetItem();
    it2->setText(QString("%1").arg(input[1]*1.2f));
     tb->setItem(1,0,it1);
     tb->setItem(1,1,it2);


}
void MainWindow::render4(QVariant v)
{   vector<float> input=v.value<vector<float>>();
    QTableWidget  *qtw=ui->tableWidget_191;
    for(int i=0;i<qtw->columnCount();i++){

      QTableWidgetItem *it=new QTableWidgetItem();
      it->setText(QString::number(input[i],'f',3));
qtw->setItem(1,i,it);

    }


}
void MainWindow::resultDisplay(vector<float> v, QString tbname){
      QTableWidget  *qtw=this->findChild< QTableWidget*>(tbname);

      for(int i=0;i<v.size();i++){

        QTableWidgetItem *it=new QTableWidgetItem();
        it->setText(QString("%1").arg(v[i]));
  qtw->setItem(i,1,it);

      }

}
void MainWindow::CarrigeresultRender(QVariant v){
    vector<float> res=v.value<vector<float>>();
    QTableWidget *tb9=ui->tableWidget_9;
     QTableWidget *tb10=ui->tableWidget_10;
     QTableWidget *tb12=ui->tableWidget_12;
    for(int i=0;i<5;i++){
        QTableWidgetItem *it=new QTableWidgetItem();
         QTableWidgetItem *it2=new QTableWidgetItem();
          QTableWidgetItem *it3=new QTableWidgetItem();

       it->setText(QString("%1").arg(res[i]));
       it2->setText(QString("%1").arg(res[i+5]));
       it3->setText(QString("%1").arg(res[i+10]));
        tb9->setItem(1,i,it);
        tb10->setItem(1,i,it2);
        tb12->setItem(1,i,it3);

    }
    for(int i=0;i<3;i++){
        QTableWidget *tb11=ui->tableWidget_11;
         QTableWidgetItem *it4=new QTableWidgetItem();
         it4->setText(QString("%1").arg(res[i+15]));
         tb11->setItem(1,i,it4);
    }

}
void MainWindow::result_1_display(QVariant v){
    vector<float> mvec=v.value<vector<float>>();



    resultDisplay(mvec,"tableWidget_2");

}
void MainWindow::result_2_display(QVariant v){
    vector<float> mvec=v.value<vector<float>>();
    resultDisplay(mvec,"tableWidget_5");

}
void MainWindow::resultRender1(QVariant v,QString tbname){
    vector<float> mvec=v.value<vector<float>>();
    resultDisplay(mvec,tbname);

}
void MainWindow::resultRender2(QVariant v){
    vector<float> datas=v.value<vector<float>>();
    QTableWidget  *qtw=this->findChild< QTableWidget*>("tableWidget_14");
    int cout=0;
    for(int i=1;i<4;i+=2){
        for(int j=0;j<2;j++){
            QTableWidgetItem *it=new QTableWidgetItem();
            it->setText(QString("%1").arg(datas[cout]));
      qtw->setItem(i,j,it);
      cout++;

        }
        QTableWidgetItem *it=new QTableWidgetItem();
        it->setText(QString("%1").arg(datas[4]));
        qtw->setItem(5,0,it);



    }

}

void MainWindow::resultRender4(QVariant v){
    vector<float> res=v.value<vector<float>>();
    QTableWidget *qtw=ui->tableWidget_18;
    int count=0;
    for(int i=1;i<4;i+=2){
        for(int j=0;j<3;j++){
            QTableWidgetItem *it=new QTableWidgetItem();
            it->setText(QString("%1").arg(res[count]));
            qtw->setItem(i,j,it);
            count++;

        }

    }


}
void MainWindow::mcqRender(QVariant v,QString tbName){
    vector<float> input=v.value<vector<float>>();
     QTableWidget  *qtw=this->findChild< QTableWidget*>(tbName);
     qtw->setColumnCount(1+input.size());
     for(int i=1;i<input.size();i++){
         QTableWidgetItem *it=new QTableWidgetItem();
         it->setText(QString("%1").arg(i));
   qtw->setItem(0,i,it);

     }
     QTableWidgetItem *it=new QTableWidgetItem();
     it->setText("Mcq_Max");
        qtw->setItem(0,input.size(),it);
        for(int j=1;j<=input.size();j++){
            QTableWidgetItem *it=new QTableWidgetItem();
            it->setText(QString("%1").arg(input[j-1]));
      qtw->setItem(1,j,it);

        }

}


void MainWindow::Section_combinRender(QVariant V)
{      //填充数据作用效应组合
    int index=0;
    vector<float> res=V.value<vector<float>>();
    QTableWidget *mytb=ui->tableWidget_8;
    for(int i=1;i<mytb->columnCount();i++){
        for(int j=1;j<mytb->rowCount();j++){
            QTableWidgetItem *it=new QTableWidgetItem();
            it->setText(QString("%1").arg(res[index]));
            mytb->setItem(j,i,it);
            index++;
 }
}






}
vector<float> MainWindow::tableDataProcess(QString tableName){
     QTableWidget  *qtw=this->findChild< QTableWidget*>( tableName);
     vector<float> result;
     for(int i=1;i<7;i+=2){
         for(int j=0;j<4;j++){

            result.push_back(qtw->item(i,j)->text().toFloat());
         }

     }
      result.push_back(qtw->item(7,0)->text().toFloat());
     return result;

}
float MainWindow::lineEditDataProcess(QString leID){
    QLineEdit  *qtw=this->findChild<QLineEdit*>( leID);
    return qtw->text().toFloat();}
void MainWindow::pagePrepare(){
    generalTableInit("tableWidget_7");
    generalTableInit("tableWidget_6");
   // ui->spinBox_2->hide();
    ui->label_19->hide();
 //   ui->comboBox_4->hide();
    ui->label_31->hide();
    connect(this,&MainWindow::destroyed,[=](){mqt->terminate();});
    connect(this,&MainWindow::task_1_send,mycat,&CacularThread::task_1_process);
    connect(this,&MainWindow::task_2_send,mycat,&CacularThread::task_2_process);
    connect(mycat,&CacularThread::onTask_1_finished,this,&MainWindow::result_1_display);
    connect(mycat,&CacularThread::onTask_2_finished,this,&MainWindow::result_2_display);
    connect(this,SIGNAL(task_4_send(QVariant)),mycat,SLOT(task_4_process(QVariant)));
    connect(mycat,SIGNAL(onTask_4_finshed(QVariant)),this,SLOT(CarrigeresultRender(QVariant)));
    connect(this,SIGNAL(task_5_send(QVariant)),mycat,SLOT(task_5_process(QVariant)));
    connect(mycat,&CacularThread::onTask_5_finished,this,&MainWindow::renderCantilever);
    connect(this,SIGNAL(task_6_send(QVariant)),mycat,SLOT(task_6_process(QVariant)));
    connect(this,&MainWindow::task_7_send,mycat,&CacularThread::task_7_process);
    connect(mycat,&CacularThread::result_1_render,this,&MainWindow::resultRender1);
    connect(mycat,&CacularThread::onTask_6_finished,this,&MainWindow::resultRender2);
   // connect(this,&MainWindow::task_8_send,mycat,&CacularThread::task_8_process);
   // connect(mycat,&CacularThread::onTask_8_finished,this,&MainWindow::resultRender3);
    connect(this,&MainWindow::task_9_send,mycat,&CacularThread::task_9_process);
    connect(mycat,&CacularThread::onTask_9_finished,this,&MainWindow::resultRender4);
    connect(mycat,&CacularThread::mcqRender,this,&MainWindow::mcqRender);
    connect(this,&MainWindow::thridStageLoad,mycat,&CacularThread::getThridLoad);
   // connect(mycat,&CacularThread::thridLoadFinished,this,&MainWindow::renderThridLoad);
    connect(this,&MainWindow::SectionCompute,mycat,&CacularThread::task_10_process);
    connect(mycat,&CacularThread::Sectionfinished,this,&MainWindow::render4);
    connect(this,&MainWindow::prestrLossRq,mycat,&CacularThread::prestrLossProcess);
    connect(this,&MainWindow::Section_Eff_Combin,mycat,&CacularThread::get_Section_Combination);
    connect(mycat,&CacularThread::Section_combinFinished,this,&MainWindow::Section_combinRender);
    connect(this,&MainWindow::getSigmaValues,mycat,&CacularThread::SigmaValuesSolve);
    connect(this,&MainWindow::RequstForDeadLoad,mycat,&CacularThread::stageDeadloadSolve);
    connect(mycat,&CacularThread::StageLoadFinished,this,&MainWindow::draw__StagedeadLoadFoce);
    connect(this,&MainWindow::RequestForCombinLoad,mycat,&CacularThread::CombinLoadSolve);
    connect(mycat,&CacularThread::CombinationSolveFinished,this,&MainWindow::draw_CombinLoadFoce);
    connect(mycat,&CacularThread::prestrLossFinished,this,&MainWindow::prestrLossRender);
    connect(this,&MainWindow::prestrLossRq,mycat,&CacularThread::prestrLossProcess);
    connect(mycat,&CacularThread::prestrLossFinished,this,&MainWindow::prestrLossRender);
}

void MainWindow::beamInit()
{
    vector<float> temp;
       temp.push_back(mycat->mymb->mbd.CalculaSpan*1000);
       temp.push_back(mycat->mymb->mbd.AspLayer);
       temp.push_back(mycat->mymb->mbd.MixedSoilLayer);
       temp.push_back(mycat->mymb->mbd.CenterCrossBeamV);
       temp.push_back(mycat->mymb->mbd.SideCorssBeamV);
       temp.push_back(mycat->Transationhalf_start);
       temp.push_back(mycat->Transtionhalf_end);
       temp.push_back(mycat->mymb->mbd.Side_Cross_Bean_field_making_Area);
       temp.push_back(mycat->mymb->mbd.Midle_Cross_Bean_field_making_Area);
       temp.push_back(mycat->mymb->mbd.Cross_Bean_t);
       beam  mybeam(*mycat->FulcrMidBeam,*mycat->mid_SpanMidBeam,temp,mycat->mymb->mbd.crossBeamNum);
       beam  sidebeam(*mycat->FulcrSideBeam,*mycat->mid_SpanSideBeam,temp,mycat->mymb->mbd.crossBeamNum);


       mycat->SetBeam(mybeam,sidebeam);

}
void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
   ui-> stackedWidget->setCurrentIndex(currentRow);

}
void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
   emit Section_Eff_Combin(arg1,ui->horizontalSlider_2->value());

}
void MainWindow::on_comboBox_4_currentIndexChanged(int index)
{
    saftyGrade= ui->comboBox_4->currentIndex()+1;

     int beamId=ui->spinBox_2->value();
     emit Section_Eff_Combin(beamId,ui->horizontalSlider_2->value());


}
void MainWindow::Steelplot(myPath path){
     m_customPlot=ui->widget_3;
     m_customPlot->clearGraphs ( );
     m_customPlot->showTracer(true);
       // add title layout element:
       if(!m_customPlot->plotLayout()->hasElement(1,0)){
           m_customPlot->plotLayout()->insertRow(0);
           m_customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(m_customPlot, "钢筋沿桥长变化图", QFont("黑体", 12, QFont::Bold)));
           }

       m_customPlot->legend->setVisible(true);
       QFont legendFont = font();  // start out with MainWindow's font..
       legendFont.setPointSize(9); // and make a bit smaller for legend
       m_customPlot->legend->setFont(legendFont);
       m_customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
       // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
       m_customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignCenter);

       // make left and bottom axes always transfer their ranges to right and top axes:
       connect(m_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->xAxis2, SLOT(setRange(QCPRange)));
       connect(m_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->yAxis2, SLOT(setRange(QCPRange)));

       // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
       m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

       // generate some data:

       int nCount = 100;

       double ts=(path.midSpanPoint.x-path.pathStartPoint.x)/100;

     QVector<double> x(nCount), y0(nCount);//, y1(nCount),y2(nCount); // initialize with entries 0..100
       for (int i = 0; i <nCount; ++i)
       {
           x[i] =path.pathStartPoint.x+i*ts; // x goes from -1 to 1

           y0[i] = path.getYvalue(x[i]);
                   //qSin(i * 10.0f / nCount); //sin
          // y1[i] = qCos(i * 10.0f / nCount); //cos
        //   y2[i]=i*i*1.0f/10000;
       }
       // create graph and assign data to it:
       QPen pen;
       QCPGraph *pGraph = m_customPlot->addGraph();
       m_customPlot->graph(0)->setData(x, y0);
       pGraph->setName("钢筋沿桥长曲线");
       pGraph->setData(x,y0);
       pGraph->setPen(QPen(Qt::blue));

       // give the axes some labels:
       m_customPlot->xAxis->setLabel("截面位置");
       m_customPlot->yAxis->setLabel("钢筋布放高度");
       m_customPlot->rescaleAxes(true);
         m_customPlot->replot();

}
bool MainWindow::tableItemCheak(QTableWidgetItem *t)
{   if(t==0){
    return false;
    }
    bool isInValid;
    t->text().toFloat(&isInValid);
    if(isInValid){
         return true;
       }else{
        return false;
    }
}
void MainWindow::prestrLossSolve()
{
    //预应力损失计算;
    bool beamType;
    int steelId;
    float Sx;//截面位置;
    float myphi_1;
    float myphi_2;
    float myxi_cs;
    int mym;//张拉批次;
    if(lineEditCheak("lineEdit_7")&lineEditCheak("lineEdit_8")&lineEditCheak("lineEdit_9"))
    {
        Sx=ui->horizontalSlider_5->value()+(mycat->bridgeSpan*1e3-mycat->myobs->cal_span*1e3)/2;
        myphi_1=lineEditDataProcess("lineEdit_7");
        myphi_2=lineEditDataProcess("lineEdit_8");
        myxi_cs=lineEditDataProcess("lineEdit_9");
        QSpinBox *sp=ui->spinBox_3;
        mym=sp->value();
        QComboBox *qcb=ui->comboBox;
        steelId=qcb->currentIndex();
        beamType=ui->comboBox_6->currentIndex()==0? false:true;
        phi_1= myphi_1;
        phi_2=myphi_2;
        xi_cs=myxi_cs*1e-4;
        m= mym;
         emit prestrLossRq(Sx,beamType,steelId);




    }else{

        qDebug()<<"输入不完整!";
    }




}
void MainWindow::SectionSolve(int x)
{
    if((mycat->FulcrMidBeam==0)|(mycat->FulcrSideBeam==0)|(mycat->mid_SpanMidBeam==0)|(mycat->mid_SpanSideBeam==0)){
        QMessageBox::information(this,QString("数据缺失"),QString("请先预存储支点和跨中截面主梁尺寸!"));
        return;
        }
       bool toCountField;
       bool isMidBeaan;
       if(box_3_cheaked){
                toCountField=true;

          }else
       {
                toCountField=false;

    }
            if(ui->comboBox_3->currentIndex()==0)
            {
                //边梁
             isMidBeaan=false;
            }else{
                //中梁
            isMidBeaan=true;
            }
            vector<float> temp;
            temp.push_back( mycat->bridgeSpan);
            temp.push_back(x/1e3);
            temp.push_back(mycat->Transationhalf_start);
            temp.push_back(mycat->Transtionhalf_end);
            QVariant data;
            data.setValue(temp);
            emit task_7_send(data,toCountField,isMidBeaan);
}
void MainWindow::draw_liveLoadM(int beamId)
{
    m_customPlot=ui->widget_5;
    QString str=QString::number(beamId)+"号梁活载弯矩包络图";
    QString str2=QString::number(beamId)+"号梁活载弯矩";
  CavasSet(m_customPlot, str);
      // generate some data:
     int nCount = 100;
     double ts=(mycat->myobs->cal_span*1e3)/100;
     QVector<double> x(nCount+1), y0(nCount+1);//, y1(nCount),y2(nCount); // initialize with entries 0..100
      for (int i = 0; i <nCount; ++i)
      {
          x[i] =i*ts;
         y0[i] = mycat->myobs->getLiveLoad_M(beamId,x[i]);
        }
      x[100]=mycat->myobs->cal_span*1e3;
     y0[100] = mycat->myobs->getLiveLoad_M(1,x[100]);
      // create graph and assign data to it:
      QPen pen;
      QCPGraph *pGraph = m_customPlot->addGraph();
      m_customPlot->graph(0)->setData(x, y0);
      pGraph->setName(str2);
      pGraph->setData(x,y0);
      pGraph->setPen(QPen(Qt::blue));
 // give the axes some labels:
      m_customPlot->xAxis->setLabel("截面位置(mm)");
      m_customPlot->yAxis->setLabel("M (KN/m)");
      m_customPlot->rescaleAxes(true);
        m_customPlot->replot();
}
void MainWindow::draw_liveLoadSf(int beamId)
{
    m_customPlot=ui->widget_6;
    QString str=QString::number(beamId)+"号梁活载剪力包络图";
    QString str2=QString::number(beamId)+"号梁活载剪力";
    m_customPlot->clearGraphs ( );
    m_customPlot->showTracer(true);
      if(!m_customPlot->plotLayout()->hasElement(1,0)){
          m_customPlot->plotLayout()->insertRow(0);
          m_customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(m_customPlot, str, QFont("黑体", 12, QFont::Bold)));
          }else{
          m_customPlot->plotLayout()->remove(m_customPlot->plotLayout()->element(0,0));
          m_customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(m_customPlot, str, QFont("黑体", 12, QFont::Bold)));
   }
     m_customPlot->legend->setVisible(true);
      QFont legendFont = font();  // start out with MainWindow's font..
      legendFont.setPointSize(9); // and make a bit smaller for legend
      m_customPlot->legend->setFont(legendFont);
      m_customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
      // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
      m_customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignCenter);
     // make left and bottom axes always transfer their ranges to right and top axes:
      connect(m_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->xAxis2, SLOT(setRange(QCPRange)));
      connect(m_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->yAxis2, SLOT(setRange(QCPRange)));
     // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
      m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
      // generate some data:
     int nCount = 100;
     double ts=(mycat->myobs->cal_span*1e3)/100;
     QVector<double> x(nCount+1), y0(nCount+1),z0(nCount+1);//, y1(nCount),y2(nCount); // initialize with entries 0..100
      for (int i = 0; i <nCount; ++i)
      {
          x[i] =i*ts; // x goes from -1 to 1
          vector<float> sfs=mycat->myobs->getLiveLoad_Sf(beamId,x[i]);
     y0[i] =sfs[2];
     z0[i]=sfs[3];

   }
      x[100]=mycat->myobs->cal_span*1e3;
    y0[100] = mycat->myobs->getLiveLoad_Sf(1,x[100])[2];
     z0[100] = mycat->myobs->getLiveLoad_Sf(1,x[100])[3];
      // create graph and assign data to it:
      QPen pen;
      QCPGraph *pGraph = m_customPlot->addGraph();

      m_customPlot->graph(0)->setData(x, y0);
       QCPGraph *myGraph = m_customPlot->addGraph();
        m_customPlot->graph(1)->setData(x, z0);
      pGraph->setName(str2+"最大值");
      myGraph->setName(str2+"最小值");

      pGraph->setData(x,y0);
      pGraph->setPen(QPen(Qt::red));
      myGraph->setPen(QPen(Qt::green));
 // give the axes some labels:
      m_customPlot->xAxis->setLabel("截面位置(mm)");
      m_customPlot->yAxis->setLabel("SF (KN)");
      m_customPlot->rescaleAxes(true);
      m_customPlot->replot();
}

void MainWindow::draw_deadLoadFoce(bool beamtype,bool foceType)
{ //foceType true弯矩 false 剪力;
    beam beamTodraw=beamtype? mycat->MDemonBeam:mycat->SDemonBeam;
    float g=beamTodraw.getFirstStageLoad()+beamTodraw.getSecondStageLoad()+beamTodraw.getThirdStageLoad(mycat->mymb->mbd.mianBeanNum);
     QString str;
     QString str2;
    int nCount = 100;
    double ts=(mycat->myobs->cal_span)/100;
    QVector<double> x(nCount+1), y0(nCount+1);//, y1(nCount),y2(nCount); // initialize with entries 0..100

    QString typeofbeam=beamtype? "中":"边";
    if(foceType){
        m_customPlot=ui->widget_7;
        str=typeofbeam+"梁恒载弯矩图";
       str2=typeofbeam+"梁恒载弯矩";
        for (int i = 0; i <nCount; ++i)
         {
        x[i] =i*ts; // x goes from -1 to 1
        y0[i] = beamTodraw.bendingSolve(g,x[i]);

        }
        x[100]=mycat->myobs->cal_span;
       y0[100] = beamTodraw.bendingSolve(g,x[100]);
    }else{
        m_customPlot=ui->widget_8;
        str=typeofbeam+"梁恒载剪力图";
        str2=typeofbeam+"梁恒载剪力";

        //todo
        for (int i = 0; i <nCount; ++i)
         {
            x[i] =i*ts; // x goes from -1 to 1
            y0[i] = beamTodraw.shearFoceSolve(g,x[i]);

            }
            x[100]=mycat->myobs->cal_span;
           y0[100] =beamTodraw.shearFoceSolve(g,x[100]);

    }
    m_customPlot->clearGraphs ( );
    m_customPlot->showTracer(true);
    if(!m_customPlot->plotLayout()->hasElement(1,0)){
          m_customPlot->plotLayout()->insertRow(0);
          m_customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(m_customPlot, str, QFont("黑体", 12, QFont::Bold)));
    }else{
        m_customPlot->plotLayout()->remove(m_customPlot->plotLayout()->element(0,0));
        m_customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(m_customPlot, str, QFont("黑体", 12, QFont::Bold)));
 }
     m_customPlot->legend->setVisible(true);
      QFont legendFont = font();  // start out with MainWindow's font..
      legendFont.setPointSize(9); // and make a bit smaller for legend
      m_customPlot->legend->setFont(legendFont);
      m_customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
      // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
      m_customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignCenter);
     // make left and bottom axes always transfer their ranges to right and top axes:
      connect(m_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->xAxis2, SLOT(setRange(QCPRange)));
      connect(m_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->yAxis2, SLOT(setRange(QCPRange)));
     // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
      m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
      // generate some data:
   // create graph and assign data to it:
      QPen pen;
      QCPGraph *pGraph = m_customPlot->addGraph();
      m_customPlot->graph(0)->setData(x, y0);
      pGraph->setName(str2);
      pGraph->setData(x,y0);
      if(foceType){
          pGraph->setPen(QPen(Qt::blue));
          m_customPlot->yAxis->setLabel("M (KN/m)");
      }else{
           pGraph->setPen(QPen(Qt::red));
           m_customPlot->yAxis->setLabel("SF (KN)");
      }

 // give the axes some labels:
      m_customPlot->xAxis->setLabel("截面位置(mm)");
      m_customPlot->rescaleAxes(true);
      m_customPlot->replot();
}

void MainWindow::deadLoadRequest()
{
    int stage=ui->comboBox_17->currentIndex()+1;
     bool foceType=ui->comboBox_18->currentIndex()==0? true:false;
     int beamId=ui->spinBox_2->value();
     bool beamType= (beamId==1|beamId==mycat->myobs->bean_nums)? false:true;
     emit RequstForDeadLoad(stage,foceType,beamType);


}

void MainWindow::getCombinLoadFoce()
{   int beamId=ui->spinBox_2->value();
    int combinType=ui->comboBox_19->currentIndex()+1;
    bool foceType=ui->comboBox_25->currentIndex()==0? true:false;
    emit RequestForCombinLoad(beamId,combinType,foceType);

}

vector<float> MainWindow::getdeflecation(int beamId)
{
    bool beamType=(beamId==1|beamId==mycat->myobs->bean_nums)? false:true;
    beam beamToSolve=beamType? mycat->MDemonBeam:mycat->SDemonBeam;
    beamToSolve.setSteel(mycat->paths);
    float Ms=0.7*mycat->myobs->getLiveLoad_M(beamId,mycat->myobs->cal_span*1e3/2)/mycat->myobs->vmcffe();
  vector<float> res=beamToSolve.deflecationSolve(Ms);
  return res;


}

void MainWindow::deflectionRender(int beamId)
{   vector<float> data= getdeflecation(beamId);
    QTreeWidget *tw=ui->treeWidget;
    QTreeWidgetItem *topItem1=tw->topLevelItem(0);
    QTreeWidgetItem *topItem2=tw->topLevelItem(1);
    QTreeWidgetItem *topItem3=tw->topLevelItem(2);
    QTreeWidgetItem *items[6]={topItem1->child(0),topItem1->child(1),topItem1->child(2),topItem2->child(0),topItem2->child(1),topItem3->child(0)};

    for(int i=0;i<6;i++){
         QTreeWidgetItem *qit=new QTreeWidgetItem();
          qit->setText(0,QString::number(data[i],'g',6)+" mm");
        if(items[i]->childCount()==0){
      items[i]->addChild(qit);
    }else{

            items[i]->child(0)->setText(0,QString::number(data[i],'g',6)+" mm");

        }

    }



}

void MainWindow::draw_CoopingCombinaM()
{    m_customPlot=ui->widget_13;
    QComboBox *combina=ui->comboBox_14;
    int index=combina->currentIndex();
     QString str="弯矩";
    CavasSet(m_customPlot, str);
    int nCount = 100;
    double ts=(mycat->mycp.l)/100;
    QVector<double> x(nCount+1), y0(nCount+1), y1(nCount+1),y2(nCount+1),y3(nCount+1),y4(nCount+1),y5(nCount+1);;
            //, y1(nCount),y2(nCount); // initialize with entries 0..100
     for (int i = 0; i <nCount; ++i)
     {      x[i]=ts*i;
           vector<float> liveload=mycat->mycp.getExtremumfoces(x[i]);
          vector<float> combinVal;
           switch (index) {
           case 0:
              combinVal=mycat->mycp.getCombinSud(x[i]);

               break;
           case 1:
               combinVal=mycat->mycp.getCombinSfd(x[i]);


               break;
           case 2:
               combinVal=mycat->mycp.getCombinSqd(x[i]);

               break;
           default:
               break;
           }

             vector<float> temp=mycat->mycp.NormalSCheak(x[i]);

           y0[i]=liveload[0];
           y1[i]=liveload[1];
           y2[i]=combinVal[0];
           y3[i]=combinVal[1];
           y4[i]=temp[0];
           y5[i]=temp[1];

}
   x[100]=mycat->mycp.l;
   vector<float> liveload=mycat->mycp.getExtremumfoces(x[100]);

  y0[100]=liveload[0];
  y1[100]=liveload[1];
  vector<float> combinVal;
   switch (index) {
   case 0:
      combinVal=mycat->mycp.getCombinSud(x[100]);

       break;
   case 1:
       combinVal=mycat->mycp.getCombinSfd(x[100]);


       break;
   case 2:
       combinVal=mycat->mycp.getCombinSqd(x[100]);

       break;
   default:
       break;
   }
  y2[100]=combinVal[0];
  y3[100]=combinVal[1];
   vector<float> temp=mycat->mycp.NormalSCheak(x[100]);
   y4[100]=temp[0];
   y5[100]=temp[1];
  QCPGraph *PG1=m_customPlot->addGraph();
  QCPGraph *PG2=m_customPlot->addGraph();
  QCPGraph *PG3=m_customPlot->addGraph();
  QCPGraph *PG4=m_customPlot->addGraph();
   QCPGraph *PG5=m_customPlot->addGraph();
   QCPGraph *PG6=m_customPlot->addGraph();
  PG1->setData(x,y0);
  PG2->setData(x,y1);
  PG3->setData(x,y2);
  PG4->setData(x,y3);
   PG5->setData(x,y4);
   PG6->setData(x,y5);
  PG1->setName("活载最大弯矩");
   PG2->setName("活载最小弯矩");
   PG3->setName("组合最大弯矩");
   PG4->setName("组合最小弯矩");
    PG5->setName("正弯矩抗弯承载力");
    PG6->setName("负弯矩抗弯承载力");
  PG1->setPen(QPen(Qt::red));
   PG2->setPen(QPen(Qt::blue));
    PG3->setPen(QPen(Qt::green));
     PG4->setPen(QPen(Qt::darkMagenta));
      PG5->setPen(QPen(Qt::darkRed));
       PG6->setPen(QPen(Qt::darkCyan));
      m_customPlot->xAxis->setLabel("截面位置(mm)");
       m_customPlot->yAxis->setLabel("KN.m");
      m_customPlot->yAxis->setRangeReversed (true);
       m_customPlot->rescaleAxes(true);
       m_customPlot->yAxis->setRangeUpper( m_customPlot->yAxis->range().upper+500);
     m_customPlot->replot();




}

void MainWindow::draw_CoopingCombinaSf()
{

    m_customPlot=ui->widget_13;
       QComboBox *combina=ui->comboBox_14;
       int index=combina->currentIndex();
        QString str="剪力";
       CavasSet(m_customPlot, str);
       int nCount = 100;
       double ts=(mycat->mycp.l)/100;
       QVector<double> x(nCount+1), y0(nCount+1), y1(nCount+1),y2(nCount+1),y3(nCount+1);
               //, y1(nCount),y2(nCount); // initialize with entries 0..100
        for (int i = 0; i <nCount; ++i)
        {      x[i]=ts*i;
              vector<float> liveload=mycat->mycp.getExtremumfoces(x[i]);
             vector<float> combinVal;
              switch (index) {
              case 0:
                 combinVal=mycat->mycp.getCombinSud(x[i]);

                  break;
              case 1:
                  combinVal=mycat->mycp.getCombinSfd(x[i]);


                  break;
              case 2:
                  combinVal=mycat->mycp.getCombinSqd(x[i]);

                  break;
              default:
                  break;
              }
              y0[i]=liveload[2];
              y1[i]=liveload[3];
              y2[i]=combinVal[2];
              y3[i]=combinVal[3];

   }
      x[100]=mycat->mycp.l;
      vector<float> liveload=mycat->mycp.getExtremumfoces(x[100]);

     y0[100]=liveload[2];
     y1[100]=liveload[3];
     vector<float> combinVal;
      switch (index) {
      case 0:
         combinVal=mycat->mycp.getCombinSud(x[100]);

          break;
      case 1:
          combinVal=mycat->mycp.getCombinSfd(x[100]);


          break;
      case 2:
          combinVal=mycat->mycp.getCombinSqd(x[100]);

          break;
      default:
          break;
      }
     y2[100]=combinVal[2];
     y3[100]=combinVal[3];
     QCPGraph *PG1=m_customPlot->addGraph();
     QCPGraph *PG2=m_customPlot->addGraph();
     QCPGraph *PG3=m_customPlot->addGraph();
     QCPGraph *PG4=m_customPlot->addGraph();
     PG1->setData(x,y0);
     PG2->setData(x,y1);
     PG3->setData(x,y2);
     PG4->setData(x,y3);
     PG1->setName("活载最大剪力");
      PG2->setName("活载最小剪力");
      PG3->setName("组合最大剪力");
      PG4->setName("组合最小剪力");
     PG1->setPen(QPen(Qt::red));
      PG2->setPen(QPen(Qt::blue));
       PG3->setPen(QPen(Qt::green));
        PG4->setPen(QPen(Qt::darkMagenta));
         m_customPlot->xAxis->setLabel("截面位置(m)");
          m_customPlot->yAxis->setLabel("KN");

          m_customPlot->rescaleAxes(true);
          m_customPlot->yAxis->setRange( m_customPlot->yAxis->range().lower*1.1,m_customPlot->yAxis->range().upper*1.1);
        m_customPlot->replot();



}

void MainWindow::draw_CoopingCrackWidth()
{
    m_customPlot=ui->widget_16;
     QString str="裂缝宽度";
       CavasSet(m_customPlot, str);
       int nCount = 100;
       double ts=(mycat->mycp.l)/100;
       QVector<double> x(nCount+1), y0(nCount+1), y1(nCount+1);
               //, y1(nCount),y2(nCount); // initialize with entries 0..100
        for (int i = 0; i <nCount; ++i)
        {      x[i]=ts*i;
         y0[i]=mycat->mycp.P_CrackSolve(x[i]);
          y1[i]=mycat->mycp. N_CrackSolve(x[i]);

   }     x[100]=mycat->mycp.l;
        y0[100]=mycat->mycp.P_CrackSolve(x[100]);
         y1[100]=mycat->mycp. N_CrackSolve(x[100]);
         QCPGraph *PG1=m_customPlot->addGraph();
         QCPGraph *PG2=m_customPlot->addGraph();
         PG1->setData(x,y0);
         PG2->setData(x,y1);
         PG1->setName("正弯矩裂缝");
          PG2->setName("负弯矩裂缝");
          PG1->setPen(QPen(Qt::red));
           PG2->setPen(QPen(Qt::blue));
           m_customPlot->xAxis->setLabel("截面位置(m)");
            m_customPlot->yAxis->setLabel("mm");
            m_customPlot->rescaleAxes(true);
            m_customPlot->yAxis->setRangeLower(-0.2);
             m_customPlot->yAxis->setRangeUpper(0.2);
          m_customPlot->replot();

}

void MainWindow::draw_CoopNormalS()
{
    m_customPlot=ui->widget_19;
     QString str="正截面抗弯";
       CavasSet(m_customPlot, str);
       int nCount = 100;
       double ts=(mycat->mycp.l)/100;
       QVector<double> x(nCount+1), y0(nCount+1), y1(nCount+1);
               //, y1(nCount),y2(nCount); // initialize with entries 0..100
        for (int i = 0; i <nCount; ++i)
        {      x[i]=ts*i;
            vector<float> temp=mycat->mycp.NormalSCheak(x[i]);
         y0[i]=temp[0];
          y1[i]=temp[1];

   }     x[100]=mycat->mycp.l;
        vector<float> temp=mycat->mycp.NormalSCheak(x[100]);
        y0[100]=temp[0];
        y1[100]=temp[1];
         QCPGraph *PG1=m_customPlot->addGraph();
         QCPGraph *PG2=m_customPlot->addGraph();
         PG1->setData(x,y0);
         PG2->setData(x,y1);
         PG1->setName("正弯矩");
          PG2->setName("负弯矩");
          PG1->setPen(QPen(Qt::red));
           PG2->setPen(QPen(Qt::blue));
           m_customPlot->xAxis->setLabel("截面位置(m)");
            m_customPlot->yAxis->setLabel("KN.M");
            m_customPlot->rescaleAxes(true);
             m_customPlot->replot();


}


void MainWindow::on_commandLinkButton_49_clicked()
{
    if(inputCheak("tableWidget_162")&lineEditCheak("lineEdit_43")&inputCheak("tableWidget_187")&inputCheak("tableWidget_188")&lineEditCheak("lineEdit_51"))
    {
        vector<float> taskdata1=tableDataProcess("tableWidget_162");
        taskdata1.push_back(lineEditDataProcess("lineEdit_43"));
        vector<float> taskdata2=tableDataProcess("tableWidget_187");
        taskdata2.push_back(lineEditDataProcess("lineEdit_51"));
        vector<float> taskdata3=tableDataProcess("tableWidget_188");
                taskdata3.push_back(lineEditDataProcess("lineEdit_51"));
                //中梁
                half_box_girder zl(taskdata1[12],taskdata1[0],taskdata1[1],taskdata1[2],taskdata1[3],taskdata1[4],taskdata1[5],taskdata1[6],taskdata1[7],taskdata1[8],taskdata1[9],taskdata1[10],taskdata1[11],false);
                half_box_girder bl1(taskdata2[12],taskdata2[0],taskdata2[1],taskdata2[2],taskdata2[3],taskdata2[4],taskdata2[5],taskdata2[6],taskdata2[7],taskdata2[8],taskdata2[9],taskdata2[10],taskdata2[11],true);
                half_box_girder bl2(taskdata3[12],taskdata3[0],taskdata3[1],taskdata3[2],taskdata3[3],taskdata3[4],taskdata3[5],taskdata3[6],taskdata3[7],taskdata3[8],taskdata3[9],taskdata3[10],taskdata3[11],false);
                small_box_girder sbzl(zl,zl);
                small_box_girder sbbl(bl1,bl2);



                if(ui->comboBox_33->currentIndex()==0){
                    if(mycat->FulcrMidBeam==0&mycat->FulcrSideBeam==0){

                        mycat->FulcrMidBeam=new field_making_girder_beam(sbzl,taskdata1[13]);
                        mycat->FulcrSideBeam=new field_making_girder_beam(sbbl,taskdata2[13]);
                        QMessageBox::information(this,QString("存入成功"),QString("已存储!"));

                    }else{

                        QMessageBox message(QMessageBox::NoIcon,"数据已存在!",ui->comboBox_33->currentText()+"数据似乎已经\n存在,是否覆盖?",QMessageBox::No|QMessageBox::Yes);

                        if(message.exec()==QMessageBox::Yes){

                            mycat->FulcrMidBeam=new field_making_girder_beam(sbzl,taskdata1[13]);
                            mycat->FulcrSideBeam=new field_making_girder_beam(sbbl,taskdata2[13]);
                            QMessageBox::information(this,QString("存入成功"),QString("已存储!"));
                            }
                        else
                        {
                         return;
                       }
                    }

                }else{
                    if(mycat->mid_SpanSideBeam==0&mycat->mid_SpanMidBeam==0){
                            mycat->mid_SpanMidBeam=new field_making_girder_beam(sbzl,taskdata1[13]);
                            mycat->mid_SpanSideBeam=new field_making_girder_beam(sbbl,taskdata2[13]);
                                   QMessageBox::information(this,QString("存入成功"),QString("已存储!"));

                        }else{

                            QMessageBox message(QMessageBox::NoIcon,"数据已存在!",ui->comboBox_33->currentText()+"数据似乎已经\n存在,是否覆盖?",QMessageBox::No|QMessageBox::Yes);
                            if(message.exec()==QMessageBox::Yes){
                                mycat->mid_SpanMidBeam=new field_making_girder_beam(sbzl,taskdata1[13]);
                                mycat->mid_SpanSideBeam=new field_making_girder_beam(sbbl,taskdata2[13]);
                                QMessageBox::information(this,QString("存入成功"),QString("已存储!"));
                            }else{

                                return;
                           }
                        }
                     }
                  }
    else{
QString qs1("必须输入");
QString qs2(ui->comboBox_33->currentText());
QString qs3("含现浇段主梁(边梁和中梁)\n的全部数据");
QMessageBox::information(this,QString("错误"),qs1+qs2+qs3);

    }
}
void MainWindow::on_commandLinkButton_50_clicked()
{   bool table_192_cheak=tableItemCheak(ui->tableWidget_192->item(1,0))&tableItemCheak(ui->tableWidget_192->item(1,1));
    if(mycat->mid_SpanSideBeam==0|mycat->mid_SpanMidBeam==0|mycat->FulcrMidBeam==0|mycat->FulcrSideBeam==0){
     QMessageBox::information(this,QString("提示"),"请先存入主梁数据!");
    }else
    {
        if(generalTableCheak("tableWidget_190",16)&table_192_cheak){
            mycat->Transationhalf_start=ui->tableWidget_192->item(1,0)->text().toFloat();
            mycat->Transtionhalf_end=ui->tableWidget_192->item(1,1)->text().toFloat();
            QVariant data=getGeneralTableData("tableWidget_190",16);
            vector<float> input=data.value<vector<float>>();
            if(mycat->myobs==0){
              mycat->myobs=new OrdinaryBrigeSection(*(mycat->mid_SpanSideBeam),*(mycat->mid_SpanMidBeam),mycat->FulcrSideBeam,mycat->FulcrMidBeam,(int)input[10],input[0],input[7]);

              mycat->myobs->InitFsm();
              bridge_calspan=input[0];
              MainBeamData mymbd{mycat->myobs->total_span,input[6],input[14],input[1],input[2],input[3],input[4],input[0],input[8],(int)input[9],(int)input[10],input[11],input[12],input[5],input[13]};
              mycat->mymb=new MainBeam(mymbd);
              mycat->bridgeSpan=input[15];
              bridge_total_Span=input[15];
              ui->spinBox_4->setMaximum(input[15]*1e3);
              ui->spinBox_5->setMaximum(mycat->mymb->mbd.mianBeanNum);
              ui->horizontalSlider->setMaximum(input[15]*1e3);
              ui->spinBox_6->setMaximum(mycat->myobs->cal_span*1e3);
              ui->horizontalSlider_2->setMaximum(mycat->myobs->cal_span*1e3);
              ui->horizontalSlider_5->setMaximum(mycat->myobs->cal_span*1e3);
              ui->spinBox_13->setMaximum(mycat->myobs->cal_span*1e3);
              ui->spinBox_2->setMaximum(mycat->myobs->bean_nums);
                ui->spinBox_16->setMaximum(mycat->myobs->bean_nums);
              numberOfBeam=mycat->mymb->mbd.mianBeanNum;
              beamInit();
              field_making_girder_beam *smb=mycat->myobs->side_Fulcr_bean;
               field_making_girder_beam *mmb=mycat->myobs->mid_Fulcr_bean;
               mycat->mycp.demobs= OrdinaryBrigeSection(*smb, *mmb,smb, mmb, mycat->myobs->bean_nums,mycat->myobs->cal_span,mycat->myobs->stoneWidth);

              QMessageBox::information(this,QString("存入成功"),QString("已存储!"));



            }else{
                QMessageBox message(QMessageBox::NoIcon,"数据已存在!","数据似乎已经存在,是否覆盖?",QMessageBox::No|QMessageBox::Yes);
                if(message.exec()==QMessageBox::Yes){
                    mycat->myobs=new OrdinaryBrigeSection(*(mycat->mid_SpanSideBeam),*(mycat->mid_SpanMidBeam),mycat->FulcrSideBeam,mycat->FulcrMidBeam,(int)input[10],input[0],input[7]);
                    bridge_calspan=input[0];
                    mycat->myobs->InitFsm();
                    MainBeamData mymbd{mycat->myobs->total_span,input[6],input[14],input[1],input[2],input[3],input[4],input[0],input[8],(int)input[9],(int)input[10],input[11],input[12],input[5],input[13]};
                    mycat->mymb=new MainBeam(mymbd);
                    mycat->bridgeSpan=input[15];
                    bridge_total_Span=input[15];
                    ui->spinBox_4->setMaximum(input[15]*1e3);
                    ui->horizontalSlider->setMaximum(input[15]*1e3);
                    ui->spinBox_2->setMaximum(mycat->myobs->bean_nums);
                    ui->spinBox_16->setMaximum(mycat->myobs->bean_nums);
                    ui->spinBox_6->setMaximum(mycat->myobs->cal_span*1e3);
                    ui->horizontalSlider_2->setMaximum(mycat->myobs->cal_span*1e3);
                    ui->horizontalSlider_5->setMaximum(mycat->myobs->cal_span*1e3);
                     ui->spinBox_13->setMaximum(mycat->myobs->cal_span*1e3);
                    numberOfBeam=mycat->mymb->mbd.mianBeanNum;
                    beamInit();
                    field_making_girder_beam *smb=mycat->myobs->side_Fulcr_bean;
                     field_making_girder_beam *mmb=mycat->myobs->mid_Fulcr_bean;
                     mycat->mycp.demobs= OrdinaryBrigeSection(*smb, *mmb,smb, mmb, mycat->myobs->bean_nums,mycat->myobs->cal_span,mycat->myobs->stoneWidth);
      QMessageBox::information(this,QString("存入成功"),QString("已存储!"));


                }else{

                    return;
                }

            }
        }else{

            QMessageBox::information(this,QString("错误"),"输入有误或数据不完整!");

        }

    }
}
void MainWindow::on_commandLinkButton_51_clicked()
{   if(mycat->myobs==0){
        QMessageBox::information(this,QString("数据不完整!"),"请先到界面一存入数据!");
        return;

    }
    vector<myPath> datas;
    QTableWidget *tab=ui->tableWidget_189;
    vector<float> res;
    for(int i=1;i<tab->rowCount();i++){
        for(int j=1;j<tab->columnCount();j++)
        {
            if(!tableItemCheak(tab->item(i,j))){
                QMessageBox::information(this,QString("输入错误"),"数据有误或输入不完整");
                return;
            }else{

                res.push_back(tab->item(i,j)->text().toFloat());
            }

        }


    }
     SetPreSteelInfo();
    for(int i=1;i<=4;i++){
        myPath temp(Point(res[(i-1)*7],res[(i-1)*7+1]),Point(res[(i-1)*7+2],res[(i-1)*7+3]),res[(i-1)*7+4],1000*mycat->bridgeSpan/2,res[(i-1)*7+5],res[(i-1)*7+6]*NominalStArea*(Ep/Ec-1));
        datas.push_back(temp);
    }

    if(mycat->paths.size()>0)
    {
            QMessageBox message(QMessageBox::NoIcon,"数据已存在!","数据似乎已经存在,是否覆盖?",QMessageBox::No|QMessageBox::Yes);
            if(message.exec()==QMessageBox::Yes){
                datas.insert(datas.end(),datas.begin(),datas.end());
                mycat->paths=datas;

                QMessageBox::information(this,QString("保存成功"),"已存入!");

            }else{

                return;
            }
    }else
    {         datas.insert(datas.end(),datas.begin(),datas.end());
              mycat->paths=datas;

        QMessageBox::information(this,QString("保存成功"),"已存入!");
    }
    SetAp(mycat->paths);




}

void MainWindow::on_commandLinkButton_3_clicked()
{
    //计算Ap
    if(mycat->myobs==0){
         QMessageBox::information(this,QString("错误"),"需要先存入主梁数据1");
         return;
        }
    QTableWidgetItem *it1=ui->tableWidget->item(1,0);
    QTableWidgetItem *it2=ui->tableWidget->item(1,1);
    QTableWidgetItem *it3=new QTableWidgetItem();
    if(tableItemCheak(it1)&tableItemCheak(it2)){
        float temp=mycat->ApSolve(it2->text().toFloat(),it1->text().toFloat());
    it3->setText(QString("%1").arg(temp));
        ui->tableWidget_3->setItem(1,0,it3);

    }



}
void MainWindow::on_commandLinkButton_52_clicked()
{
    QTableWidget *tb=ui->tableWidget_2;
    float fsd;
    float a;
    float Fpd;
    float ap;
    float bf;
  a=tb->item(1,0)->text().toFloat();
  Fpd=tb->item(1,1)->text().toFloat();
  ap=tb->item(1,2)->text().toFloat();
   fsd=tb->item(1,3)->text().toFloat();
  vector<float>  m=mycat->getMaxSfd(15);
  float h0;

  float param1;
  float param2;
  float param3;
  float root;

  if(m[0]==1|m[0]==mycat->myobs->bean_nums){
      bf=1000*(mycat->mid_SpanSideBeam->sbg.left.b1+mycat->mid_SpanSideBeam->sbg.right.b1);
     h0=1000*mycat->mid_SpanSideBeam->sbg.left.h-a;
     param1=-0.5*22.4*(bf);
     param2=param1*(-2)*h0;
     param3=-m[1]*pow(10,6);



  }else{
      bf=1000*(mycat->mid_SpanMidBeam->sbg.left.b1+mycat->mid_SpanMidBeam->sbg.right.b1);
      h0=1000*mycat->mid_SpanMidBeam->sbg.left.h-a;
      param1=-0.5*22.4*(bf);
      param2=param1*(-2)*h0;
      param3=-m[1]*pow(10,6);
  }
  if(pow(param2,2)>=4*param1*param3){
      float temp1=(-param2+sqrt(pow(param2,2)-4*param1*param3))/(2*param1);
      float temp2=(-param2-sqrt(pow(param2,2)-4*param1*param3))/(2*param1);

      if(0<temp1&temp1<mycat->mid_SpanMidBeam->sbg.left.d1*1000){
          root=temp1;
      }
      if(0<temp2&temp2<mycat->mid_SpanMidBeam->sbg.left.d1*1000){
          root=temp2;
      }

  }

  float as;
  as=(22.4*bf*root-Fpd*ap)/fsd;
  QTableWidget *t=ui->tableWidget_4;
  QTableWidgetItem *it=new QTableWidgetItem();
  it->setText(QString::number(as,'f',2));
  t->setItem(1,0,it);



}
void MainWindow::on_tableWidget_189_cellClicked(int row, int column)
{
    if(mycat->paths.size()==0){
    return;
    }
        if(column==0&row>0){
            QTableWidget *qtw=ui->tableWidget_189;
                QTableWidgetItem *it=new  QTableWidgetItem();
                QString qs=qtw->item(row,column)==0? " ":qtw->item(row,column)->text();
                it->setText(qs);
                it->setFlags(Qt::ItemIsEditable);
                qtw->setItem(row,column,it);
                Steelplot(mycat->paths[row-1]);



        }


}
void MainWindow::on_lineEdit_editingFinished()
{       if(mycat->paths.size()==0){
    return;
    }

        bool precounted;
        precounted=ui->comboBox_5->currentIndex()>0? true:false;
         bool fieldCount=ui->comboBox_5->currentIndex()>1? true:false;
        QString input=ui->lineEdit->text();
         bool Type=ui->comboBox_34->currentIndex()==0? false:true;
         bool isInvalid;
        float x=input.toFloat(&isInvalid);
        if(isInvalid& x<mycat->bridgeSpan*1000){
        emit SectionCompute(fieldCount,Type,x,precounted);
        }else{
     qDebug()<<"输入非法!!";
        }
}
void MainWindow::on_comboBox_34_currentIndexChanged(int index)
{      bool precounted;
       precounted=ui->comboBox_5->currentIndex()>0? true:false;
        bool fieldCount=ui->comboBox_5->currentIndex()>1? true:false;
       QString input=ui->lineEdit->text();
        bool Type=ui->comboBox_34->currentIndex()==0? false:true;
        bool isInvalid;
        float x=input.toFloat(&isInvalid);
        if(isInvalid& x<mycat->bridgeSpan*1000){
         emit SectionCompute(fieldCount,Type,x,precounted);
        }else{
    qDebug()<<"输入非法!!";
        }
}
void MainWindow::on_comboBox_5_currentIndexChanged(int index)
{
    bool precounted;
          precounted=ui->comboBox_5->currentIndex()>0? true:false;
           bool fieldCount=ui->comboBox_5->currentIndex()>1? true:false;
          QString input=ui->lineEdit->text();
           bool Type=ui->comboBox_34->currentIndex()==0? false:true;
         bool isInvalid;
         float x=input.toFloat(&isInvalid);
         if(isInvalid& x<mycat->bridgeSpan*1000){
        emit SectionCompute(fieldCount,Type,x,precounted);
    }else{
       qDebug()<<"输入非法!!";

    }
}
void MainWindow::on_pushButton_clicked()
{   mycat->mycp.demobs=*mycat->myobs;
       Coping cp;
       cp.demobs=*mycat->myobs;
        draw_CoopingCrackWidth();






}
void MainWindow::on_tabWidget_currentChanged(int index)
{
    ui->spinBox->setMinimum(1);
    if(!mycat->myobs==0){
    ui->spinBox->setMaximum(mycat->myobs->bean_nums);
    ui->label_28->show();
    ui->spinBox->show();

    }
    if(index==4&!mycat->myobs==0&mycat->paths.size()>0){

         drawfocescheaking(ui->spinBox_16->value());
    }
    if(index==5&!mycat->myobs==0&mycat->paths.size()>0){

         draw_prestrLossP();
       draw_prestrLossL();
    }
    if(index==3&!mycat->myobs==0&mycat->paths.size()>0){
    mainBeamdeadLoad();
    bool beamType;
    if(ui->comboBox_7->currentIndex()==0){
      beamType=false;
    }else{
      beamType=true;
    }
    draw_deadLoadFoce(beamType,true);
    draw_deadLoadFoce(beamType,false);

  }
     if(index==6){

      draw_Copingdeadload_M();
      draw_CopingDeadLoadSF();
     }



}
void MainWindow::on_horizontalSlider_valueChanged(int value)
{   if(mycat->myobs==0){
        QMessageBox::information(this,QString("数据缺失"),QString("请先预存储支点和跨中截面主梁尺寸!"));
        return;
    }
    SectionSolve(value);
}
void MainWindow::on_checkBox_3_clicked()
{
    SectionSolve(ui->horizontalSlider->value());
}

void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    SectionSolve(ui->horizontalSlider->value());
}
void MainWindow::on_spinBox_5_valueChanged(int arg1)
{   if(mycat->myobs==0){
        QMessageBox::information(this,QString("数据缺失"),QString("请先预存储支点和跨中截面主梁尺寸!"));
        return;
    }
   draw_liveLoadM(arg1);
   draw_liveLoadSf(arg1);
}

void MainWindow::on_comboBox_7_currentIndexChanged(int index)
{   if(mycat->myobs==0){
    return;
    }
    bool beamType;
    if(index==0){
      beamType=false;
    }else{
      beamType=true;
    }
    draw_deadLoadFoce(beamType,true);
    draw_deadLoadFoce(beamType,false);

}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{   if(mycat->myobs==0){
        return;
    //数据不完整,暂不处理
    }
    float x=value;
   int beamId=ui->spinBox_2->value();
   emit Section_Eff_Combin(beamId,x);
}

void MainWindow::on_comboBox_8_currentIndexChanged(int index)
{   float lj=(int)(mycat->myobs->cal_span*1e3);
    switch (index) {
    case 0:
        ui->horizontalSlider_2->setValue(0);
        break;
    case 1:
        ui->horizontalSlider_2->setValue(lj/4);

        break;
    case 2:
        ui->horizontalSlider_2->setValue(lj/2);
        break;
    default:
        break;
    }
}

void MainWindow::on_horizontalSlider_5_valueChanged(int value)
{
    /*if(mycat->myobs==0|mycat->paths.size()==0){
          //说明数据不完整
      return;
      }

      float a=lineEditDataProcess("lineEdit_7");
      float b=lineEditDataProcess("lineEdit_8");
      float c=lineEditDataProcess("lineEdit_9");
      int steelId=ui->comboBox->currentIndex()+1;
      if(value<mycat->paths[steelId-1].pathStartPoint.x){
          return;
      }
      bool beamType=ui->comboBox_6->currentIndex()==0? false:true;
      m=ui->spinBox_3->value();
      phi_1=a;phi_2=b,xi_cs=c*1e-4;//初始化相关参数;
      emit getSigmaValues( steelId,beamType, 8450);*/

    //截面x处的预应力损失计算;
    prestrLossSolve();
}

void MainWindow::draw__StagedeadLoadFoce(QVariant data, bool foceType)
{    //绘制三期恒载内力图;
    m_customPlot=ui->widget_17;
    QVector<double> y0=data.value<QVector<double>>();
    //foceType true弯矩 false 剪力;
        QString str;
         QString str2;
        int nCount = 100;
         double ts=(mycat->myobs->cal_span)/100;
        QVector<double> x(nCount+1);//, y1(nCount),y2(nCount); // initialize with entries 0..100
        for (int i = 0; i <nCount; ++i)
           {
          x[i] =i*ts; // x goes from -1 to 1


          }
          x[100]=mycat->myobs->cal_span;
        if(foceType){
        str="恒载弯矩图";
        str2="恒载弯矩";
            }else{
            str="恒载剪力图";
            str2="恒载剪力";
            }
         CavasSet(m_customPlot, str);
         // generate some data:
         // create graph and assign data to it:
        QCPGraph *pGraph = m_customPlot->addGraph();
          m_customPlot->graph(0)->setData(x, y0);
          pGraph->setName(str2);
          pGraph->setData(x,y0);
          if(foceType){
              pGraph->setPen(QPen(Qt::blue));
              m_customPlot->yAxis->setLabel("M (KN/m)");
          }else{
               pGraph->setPen(QPen(Qt::red));
               m_customPlot->yAxis->setLabel("SF (KN)");
          }
          m_customPlot->xAxis->setLabel("截面位置(mm)");
          m_customPlot->rescaleAxes(true);
          m_customPlot->replot();
}

void MainWindow::on_pushButton_9_clicked()
{

}

void MainWindow::on_comboBox_18_currentIndexChanged(const QString &arg1)
{
    deadLoadRequest();
}

void MainWindow::on_comboBox_17_currentIndexChanged(int index)
{
    deadLoadRequest();
}

void MainWindow::draw_CombinLoadFoce(QVariant data,QVariant W,bool foceType)
{
    //绘制效应组合内力图;
      m_customPlot=ui->widget_18;
      QVector<double> y0=data.value<QVector<double>>();
      //foceType true弯矩 false 剪力;
          QString str;
           QString str2;
          int nCount = 100;
           double ts=(mycat->myobs->cal_span)/100;
          QVector<double> x(nCount+1);//, y1(nCount),y2(nCount); // initialize with entries 0..100
          for (int i = 0; i <nCount; ++i)
             {
            x[i] =i*ts; // x goes from -1 to 1


            }
            x[100]=mycat->myobs->cal_span;

          str=ui->comboBox_18->currentText();
          str2=ui->comboBox_25->currentText();

           CavasSet(m_customPlot, str);
           // generate some data:
           // create graph and assign data to it:
          QCPGraph *pGraph = m_customPlot->addGraph();
            m_customPlot->graph(0)->setData(x, y0);
            pGraph->setName("剪力最大值组合");
            pGraph->setData(x,y0);
            if(ui->comboBox_25->currentIndex()==0){
                pGraph->setPen(QPen(Qt::blue));
                m_customPlot->yAxis->setLabel("M (KN/m)");
            }else{
                 pGraph->setPen(QPen(Qt::red));
                 m_customPlot->yAxis->setLabel("SF (KN)");
            }
            if(!foceType){
                QCPGraph *myGraph = m_customPlot->addGraph();
                 QVector<double> z0=W.value<QVector<double>>();
                 m_customPlot->graph(1)->setData(x, z0);
                myGraph->setData(x,z0);
                  myGraph->setName("剪力最小值组合");
                   myGraph->setPen(QPen(Qt::green));
            }
            m_customPlot->xAxis->setLabel("截面位置(mm)");
            m_customPlot->rescaleAxes(true);
            m_customPlot->replot();

}

void MainWindow::on_comboBox_25_currentIndexChanged(int index)
{
    getCombinLoadFoce();
}

void MainWindow::on_comboBox_19_currentIndexChanged(int index)
{
    getCombinLoadFoce();
}

void MainWindow::prestrLossRender(QVariant data)
{   QTableWidget *mytb=ui->tableWidget_24;
   int index=0;
    vector<float> val=data.value<vector<float>>();
    for(int i=1;i<mytb->rowCount();i++){
    for(int j=1;j<mytb->columnCount();j++){
            QTableWidgetItem *it=new QTableWidgetItem();
            it->setText(QString("%1").arg(val[index]));
            mytb->setItem(i,j,it);
            index++;

        }

    }





}

void MainWindow::SetAp(vector<myPath> steelpath)
{ float apval=0;
    for(int i=0;i<steelpath.size();i++){
        apval+=steelpath[i].steelArea;


    }
    Ap=apval/(Ep/Ec-1);


}

void MainWindow::on_horizontalSlider_5_actionTriggered(int action)
{

}

void MainWindow::draw_prestrLossP()
{     float myd1=(mycat->bridgeSpan-mycat->myobs->cal_span)*1e3/2;
      bool beamType=ui->comboBox_6->currentIndex()==0? false:true;
      beam beamToSolve=beamType? mycat->MDemonBeam:mycat->SDemonBeam;
        beamToSolve.setSteel(mycat->paths);
    //预应力损失sigma_P1,sigma_P2;绘制,使用主线程绘制,如果出了卡顿问题再改;
    m_customPlot=ui->widget_4;
    QString str="钢束有效预应力";
    QString str2="使用阶段";
     CavasSet(m_customPlot, str);
      // generate some data:
     int nCount = 100;
     double ts=(mycat->myobs->cal_span*1e3)/100;
     QVector<double> x(nCount+1), y0(nCount+1),z0(nCount+1);//, y1(nCount),y2(nCount); // initialize with entries 0..100
      for (int i = 0; i <nCount; ++i)
      {
          x[i] =i*ts; // x goes from -1 to 1
          y0[i] = beamToSolve.getSigma_P_I(x[i]+myd1,Ap);
          z0[i] = beamToSolve.getSigma_P_II(x[i]+myd1,Ap);


   }
      x[100]=mycat->myobs->cal_span*1e3;
    y0[100] = beamToSolve.getSigma_P_I(x[100]+myd1,Ap);
     z0[100] = beamToSolve.getSigma_P_II(x[100]+myd1,Ap);
      // create graph and assign data to it:

      QCPGraph *pGraph = m_customPlot->addGraph();
     m_customPlot->graph(0)->setData(x, y0);
       QCPGraph *myGraph = m_customPlot->addGraph();
        m_customPlot->graph(1)->setData(x, z0);
      pGraph->setName("预加力阶段");
      myGraph->setName("使用阶段");
     pGraph->setData(x,y0);
      pGraph->setPen(QPen(Qt::red));
      myGraph->setPen(QPen(Qt::green));
 // give the axes some labels:
      m_customPlot->xAxis->setLabel("截面位置(mm)");
      m_customPlot->yAxis->setLabel("MPa");
      m_customPlot->rescaleAxes(true);
        m_customPlot->replot();
}

void MainWindow::draw_prestrLossL()
{
    float myd1=(mycat->bridgeSpan-mycat->myobs->cal_span)*1e3/2;
        bool beamType=ui->comboBox_6->currentIndex()==0? false:true;
        beam beamToSolve=beamType? mycat->MDemonBeam:mycat->SDemonBeam;
          beamToSolve.setSteel(mycat->paths);
      //预应力损失sigma_P1,sigma_P2;绘制,使用主线程绘制,如果出了卡顿问题再改;
      m_customPlot=ui->widget_21;
      QString str="预应力损失";
       CavasSet(m_customPlot, str);
        // generate some data:
       int nCount = 100;
       double ts=(mycat->myobs->cal_span*1e3)/100;
       QVector<double> x(nCount+1), y0(nCount+1),z0(nCount+1);//, y1(nCount),y2(nCount); // initialize with entries 0..100
        for (int i = 0; i <nCount; ++i)
        {
            x[i] =i*ts; // x goes from -1 to 1
            y0[i] = beamToSolve.getSigma_L_I(x[i]+myd1,Ap);
          //  z0[i] = beamToSolve.getSigma_L_II(Ap);


     }
        x[100]=mycat->myobs->cal_span*1e3;
      y0[100] = beamToSolve.getSigma_L_I(x[100]+myd1,Ap);
     //  z0[100] = beamToSolve.getSigma_P_II(x[100]+myd1,Ap);
        // create graph and assign data to it:

        QCPGraph *pGraph = m_customPlot->addGraph();
       m_customPlot->graph(0)->setData(x, y0);
      //   QCPGraph *myGraph = m_customPlot->addGraph();
         // m_customPlot->graph(1)->setData(x, z0);
        pGraph->setName("预加应力阶段");
       // myGraph->setName("使用阶段");
       pGraph->setData(x,y0);
        pGraph->setPen(QPen(Qt::red));
      //  myGraph->setPen(QPen(Qt::green));
   // give the axes some labels:
        m_customPlot->xAxis->setLabel("截面位置(mm)");
        m_customPlot->yAxis->setLabel("MPa");

        m_customPlot->rescaleAxes(true);
        m_customPlot->yAxis->setRangeLower( m_customPlot->yAxis->range().lower-20);
        m_customPlot->yAxis->setRangeUpper(m_customPlot->yAxis->range().upper+20);
        m_customPlot->replot();



}

void MainWindow::draw_Mu(int beamID)
{

        bool beamType=(beamID==1|beamID==mycat->myobs->bean_nums)? false:true;
        beam beamToSolve=beamType? mycat->MDemonBeam:mycat->SDemonBeam;
        beamToSolve.setSteel(mycat->paths);
      m_customPlot=ui->widget_22;
      QString str="弯矩承载力";
   CavasSet(m_customPlot, str);
        // generate some data:
       int nCount = 100;
       double ts=(mycat->myobs->cal_span*1e3)/100;
       QVector<double> x(nCount+1), y0(nCount+1),z0(nCount+1);//, y1(nCount),y2(nCount); // initialize with entries 0..100
        for (int i = 0; i <nCount; ++i)
        {
            x[i] =i*ts; // x goes from -1 to 1
            y0[i] = beamToSolve.get_Mu(x[i]);
            z0[i] =mycat->get_M_CombinationAt(beamID,x[i])[6];


     }
        x[100]=mycat->myobs->cal_span*1e3;
      y0[100] = beamToSolve.get_Mu(x[100]);
      z0[100] =mycat->get_M_CombinationAt(beamID,x[100])[6];

     //  z0[100] = beamToSolve.getSigma_P_II(x[100]+myd1,Ap);
        // create graph and assign data to it:

        QCPGraph *pGraph = m_customPlot->addGraph();
       m_customPlot->graph(0)->setData(x, y0);
       QCPGraph *myGraph = m_customPlot->addGraph();
         m_customPlot->graph(1)->setData(x, z0);
        pGraph->setName("抗弯承载力");
       myGraph->setName("弯矩设计值");
        pGraph->setData(x,y0);
        pGraph->setPen(QPen(Qt::red));
       myGraph->setPen(QPen(Qt::green));
   // give the axes some labels:
        m_customPlot->xAxis->setLabel("截面位置(mm)");
        m_customPlot->yAxis->setLabel("KN.m");
        m_customPlot->rescaleAxes(true);
        m_customPlot->replot();
}

void MainWindow::draw_oblique_sf(int beamID)
{    bool beamType=(beamID==1|beamID==mycat->myobs->bean_nums)? false:true;
     beam beamToSolve=beamType? mycat->MDemonBeam:mycat->SDemonBeam;
     beamToSolve.setSteel(mycat->paths);
     m_customPlot=ui->widget_23;
      QString str="抗剪承载力";
        CavasSet(m_customPlot, str);


         int nCount = 100;
         double ts=(mycat->myobs->cal_span*1e3/2)/100;
         QVector<double> x(nCount+1), y0(nCount+1),z0(nCount+1),u0(nCount+1),V0(nCount+1);//, y1(nCount),y2(nCount); // initialize with entries 0..100
          for (int i = 0; i <nCount; ++i)
          {
              x[i] =i*ts; // x goes from -1 to 1
              vector<float> temp=beamToSolve.obliqueSectionCheaking(x[i]);
              y0[i] = temp[0];
                z0[i] =temp[1];
                u0[i]=temp[2];
                V0[i]=mycat->get_Sf_CombinationAt(beamID,x[i],true)[6];//todo


       }
          x[100]=mycat->myobs->cal_span*1e3/2;
           vector<float> temp=beamToSolve.obliqueSectionCheaking(x[100]);
           y0[100] = temp[0];
             z0[100] =temp[1];
             u0[100]=temp[2];
             V0[100]=mycat->get_Sf_CombinationAt(beamID,x[100],true)[6];//todo
       //数据生成;

              QCPGraph *pGraph = m_customPlot->addGraph();
             m_customPlot->graph(0)->setData(x, y0);
             QCPGraph *myGraph = m_customPlot->addGraph();
               m_customPlot->graph(1)->setData(x, z0);
              pGraph->setName("抗剪下限值");
             myGraph->setName("抗剪上限值");
              pGraph->setData(x,y0);
              pGraph->setPen(QPen(Qt::red));
             myGraph->setPen(QPen(Qt::green));

              QCPGraph *Graph = m_customPlot->addGraph();
             m_customPlot->graph(2)->setData(x, u0);
             QCPGraph *Graphu = m_customPlot->addGraph();
               m_customPlot->graph(3)->setData(x, V0);
              Graph->setName("Vs+Vpd");
            Graphu->setName("设计剪力值");
              Graph->setData(x,u0);
              Graphu->setPen(QPen(Qt::blue));
             Graph->setPen(QPen(Qt::gray));
         // give the axes some labels:
              m_customPlot->xAxis->setLabel("截面位置(mm)");
              m_customPlot->yAxis->setLabel("KN");
              m_customPlot->rescaleAxes(true);
               m_customPlot->yAxis->setRangeUpper( m_customPlot->yAxis->range().upper+100);

              m_customPlot->replot();




}

void MainWindow::drawSigmaCt(int beamId)
{

    bool beamType=(beamId==1|beamId==mycat->myobs->bean_nums)? false:true;
        beam beamToSolve=beamType? mycat->MDemonBeam:mycat->SDemonBeam;
        beamToSolve.setSteel(mycat->paths);
        m_customPlot=ui->widget_24;
         QString str="短暂状况正应力验算";
          CavasSet(m_customPlot, str);

            int nCount = 100;
            double ts=(mycat->myobs->cal_span*1e3)/100;
            QVector<double> x(nCount+1), y0(nCount+1),z0(nCount+1),u0(2);//, y1(nCount),y2(nCount); // initialize with entries 0..100
             for (int i = 0; i <nCount; ++i)
             {
                 x[i] =i*ts; // x goes from -1 to 1
                 vector<float> temp=beamToSolve.getSigma_ct(x[i]);
                 y0[i] = temp[0];
                   z0[i] =temp[1];



          }
             x[100]=mycat->myobs->cal_span*1e3;
              vector<float> temp=beamToSolve.getSigma_ct(x[100]);
              y0[100] = temp[0];
                z0[100] =temp[1];

          //数据生成;

                 QCPGraph *pGraph = m_customPlot->addGraph();
                m_customPlot->graph(0)->setData(x, y0);
                QCPGraph *myGraph = m_customPlot->addGraph();
                  m_customPlot->graph(1)->setData(x, z0);
                 pGraph->setName("上缘");
                myGraph->setName("下缘");
                 pGraph->setData(x,y0);
                 pGraph->setPen(QPen(Qt::red));
                myGraph->setPen(QPen(Qt::green));

                 QCPGraph *Graph = m_customPlot->addGraph();
                    QVector<double> k(2);
                 k[0]=0;
                 k[1]=1e3*mycat->myobs->cal_span;
                m_customPlot->graph(2)->setData(k, u0);
                 Graph->setName("0.7fck");
                 u0[0]=0.7*feck;
                  u0[1]=0.7*feck;
                 Graph->setData(k,u0);
                Graph->setPen(QPen(Qt::blue));
            // give the axes some labels:
                 m_customPlot->xAxis->setLabel("截面位置(mm)");
                 m_customPlot->yAxis->setLabel("MPa");
                 m_customPlot->rescaleAxes(true);
                 m_customPlot->yAxis->setRangeUpper( m_customPlot->yAxis->range().upper+5);
                 m_customPlot->yAxis->setRangeLower(m_customPlot->yAxis->range().lower-3);
                 m_customPlot->replot();

}

void MainWindow::drawPermanentStress(int beamId)
{
    m_customPlot=ui->widget_25;
   XxwCustomPlot *n_customPlot=ui->widget_9;
         QString str="持久状况正应力验算";
         m_customPlot->clearGraphs ( );
         m_customPlot->showTracer(true);
         n_customPlot->showTracer(true);
           if(!m_customPlot->plotLayout()->hasElement(1,0)){
               m_customPlot->plotLayout()->insertRow(0);
               m_customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(m_customPlot, str, QFont("黑体", 12, QFont::Bold)));
               }else{
               m_customPlot->plotLayout()->remove(m_customPlot->plotLayout()->element(0,0));
               m_customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(m_customPlot, str, QFont("黑体", 12, QFont::Bold)));
        }
          m_customPlot->legend->setVisible(true);
           n_customPlot->legend->setVisible(true);
           QFont legendFont = font();  // start out with MainWindow's font..
           legendFont.setPointSize(9); // and make a bit smaller for legend
           m_customPlot->legend->setFont(legendFont);
           m_customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
           // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
           m_customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignCenter);
          // make left and bottom axes always transfer their ranges to right and top axes:
           connect(m_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->xAxis2, SLOT(setRange(QCPRange)));
           connect(m_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->yAxis2, SLOT(setRange(QCPRange)));
           connect(n_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), n_customPlot->xAxis2, SLOT(setRange(QCPRange)));
           connect(n_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), n_customPlot->yAxis2, SLOT(setRange(QCPRange)));

           // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
           m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
           n_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);


            int nCount = 100;
            double ts=(mycat->myobs->cal_span*1e3)/100;
            QVector<double> x(nCount+1), y0(nCount+1),z0(nCount+1),u0(2),V0(2);//, y1(nCount),y2(nCount); // initialize with entries 0..100
             for (int i = 0; i <nCount; ++i)
             {
                 x[i] =i*ts; // x goes from -1 to 1
                 vector<float> temp=mycat->PermanentCheak(beamId,x[i]);
                 y0[i] = temp[0];
                   z0[i] =temp[1];



          }
             x[100]=mycat->myobs->cal_span*1e3;
              vector<float> temp=mycat->PermanentCheak(beamId,x[100]);;
              y0[100] = temp[0];
                z0[100] =temp[1];

          //数据生成;

                 QCPGraph *pGraph = m_customPlot->addGraph();
                m_customPlot->graph(0)->setData(x, y0);
                QCPGraph *myGraph = n_customPlot->addGraph();
                  n_customPlot->graph(0)->setData(x, z0);
                 pGraph->setName("截面混凝土正应力σ_cu");
                myGraph->setName("预应力钢束拉应力");
                 pGraph->setData(x,y0);
                 pGraph->setPen(QPen(Qt::red));
                myGraph->setPen(QPen(Qt::green));

                 QCPGraph *Graph = n_customPlot->addGraph();
                    QVector<double> k(2);
                 k[0]=0;
                 k[1]=1e3*mycat->myobs->cal_span;
                n_customPlot->graph(1)->setData(k, u0);
                 Graph->setName("0.65fpk");
                 u0[0]=0.65*fpk;
                  u0[1]=0.65*fpk;
                 Graph->setData(k,u0);
                Graph->setPen(QPen(Qt::blue));
                V0[0]=0.5*fck;
                V0[1]=0.5*fck;
                QCPGraph *blGraph = m_customPlot->addGraph();
               m_customPlot->graph(1)->setData(k, V0);
                blGraph->setName("0.5fck");
                 blGraph->setData(k,V0);
                 blGraph->setPen(QPen(Qt::black));
            // give the axes some labels:
                 m_customPlot->xAxis->setLabel("截面位置(mm)");
                 m_customPlot->yAxis->setLabel("MPa");

               m_customPlot->rescaleAxes(true);
               m_customPlot->yAxis->setRangeUpper( m_customPlot->yAxis->range().upper+3);
               m_customPlot->yAxis->setRangeLower(m_customPlot->yAxis->range().lower-3);
                 m_customPlot->replot();
                 n_customPlot->xAxis->setLabel("截面位置(mm)");
                 n_customPlot->yAxis->setLabel("MPa");
                 n_customPlot->rescaleAxes(true);
                 n_customPlot->yAxis->setRange(z0[0]-10,0.65*fpk+10);
                 n_customPlot->replot();




}

void MainWindow::draw_MainStress(int beamId)
{

         m_customPlot=ui->widget_26;
         QString str="主应力验算";
          CavasSet(m_customPlot, str);
            //MainStressSolve
           int nCount = 100;
           double ts=(mycat->myobs->cal_span*1e3/2)/100;
           QVector<double> x(nCount+1), y0(nCount+1), y1(nCount+1), y2(nCount+1), y3(nCount+1), y4(nCount+1),
                    y5(nCount+1),u0(2),V0(2);//, y1(nCount),y2(nCount); // initialize with entries 0..100
            for (int i = 0; i <nCount; ++i)
            {
                x[i] =i*ts; // x goes from -1 to 1
                vector<float> temp=mycat->MainStressSolve(beamId,x[i]);
                y0[i] = temp[0];
                y1[i] = temp[1];
                y2[i] = temp[2];
                y3[i] = temp[3];
                y4[i] = temp[4];
                y5[i] = temp[5];




         }
            x[100]=mycat->myobs->cal_span*1e3/2;
            vector<float> temp=mycat->MainStressSolve(beamId,x[100]);
            y0[100] = temp[0];
            y1[100] = temp[1];
            y2[100] = temp[2];
            y3[100] = temp[3];
            y4[100] = temp[4];
            y5[100] = temp[5];
            u0[0]=0;
            u0[1]=x[100];
            V0[0]=0.6*fck;
             V0[1]=0.6*fck;
             QCPGraph *pGraph1 = m_customPlot->addGraph();
             QCPGraph *pGraph2 = m_customPlot->addGraph();
             QCPGraph *pGraph3 = m_customPlot->addGraph();
             QCPGraph *pGraph4 = m_customPlot->addGraph();
             QCPGraph *pGraph5 = m_customPlot->addGraph();
             QCPGraph *pGraph6 = m_customPlot->addGraph();
             QCPGraph *pGraph7 = m_customPlot->addGraph();
              pGraph1->setData(x,y0);
              pGraph2->setData(x,y1);
              pGraph3->setData(x,y2);
              pGraph4->setData(x,y3);
              pGraph5->setData(x,y4);
              pGraph6->setData(x,y5);
              pGraph7->setData(u0,V0);
               pGraph1->setName("a-a截面sigma_cp");
               pGraph2->setName("b-b截面sigma_cp");
               pGraph3->setName("c-c截面sigma_cp");
               pGraph4->setName("a-a截面sigma_tp");
               pGraph5->setName("b-b截面sigma_tp");
               pGraph6->setName("c-c截面sigma_tp");
               pGraph7->setName("限值");
                pGraph1->setPen(QPen(Qt::yellow));
                pGraph2->setPen(QPen(Qt::blue));
                pGraph3->setPen(QPen(Qt::green));
                pGraph4->setPen(QPen(Qt::darkBlue));
                pGraph5->setPen(QPen(Qt::darkMagenta));
                pGraph6->setPen(QPen(Qt::darkYellow));
                pGraph7->setPen(QPen(Qt::red));

                m_customPlot->xAxis->setLabel("截面位置(mm)");
                m_customPlot->yAxis->setLabel("MPa");


                m_customPlot->rescaleAxes(true);
                m_customPlot->yAxis->setRangeUpper( m_customPlot->yAxis->range().upper+10);


                m_customPlot->replot();


}

void MainWindow::drawfocescheaking(int beamId)
{
   draw_Mu(beamId);
    draw_oblique_sf(beamId);
    drawSigmaCt(beamId);
   drawPermanentStress( beamId);
   draw_MainStress(beamId);
    drawcrackChecking(beamId);
    draw_obliqueCrack(beamId);


}

void MainWindow::drawcrackChecking(int beamId)
{   m_customPlot=ui->widget_10;

    QString str="正截面抗裂验算";
    CavasSet(m_customPlot, str);

       //MainStressSolve
      int nCount = 100;
      double ts=(mycat->myobs->cal_span*1e3/2)/100;
      QVector<double> x(nCount+1), y0(nCount+1), y1(nCount+1),u0(2),V0(2);
      // y1(nCount),y2(nCount); // initialize with entries 0..100
       for (int i = 0; i <nCount; ++i)
       {
           x[i] =i*ts; // x goes from -1 to 1
           vector<float> temp=mycat->drawcrackChecking(beamId,x[i]);
           y0[i] = temp[2];
           y1[i] = temp[4];




    }
       x[100]=mycat->myobs->cal_span*1e3/2;
       vector<float> temp=mycat->drawcrackChecking(beamId,x[100]);
       y0[100] = temp[2];
       y1[100] = temp[4];

       u0[0]=0;
       u0[1]=x[100];
       V0[0]=0.7*ftk;
        V0[1]=0.7*ftk;
        QCPGraph *pGraph1 = m_customPlot->addGraph();
        QCPGraph *pGraph2 = m_customPlot->addGraph();
        QCPGraph *pGraph3 = m_customPlot->addGraph();

         pGraph1->setData(x,y0);
         pGraph2->setData(x,y1);


         pGraph3->setData(u0,V0);
          pGraph1->setName("sigma_st-sigma_pc");
          pGraph2->setName("sigma_lt-sigma_Pc");
          pGraph3->setName("0.7*ftk");

           pGraph1->setPen(QPen(Qt::yellow));
           pGraph2->setPen(QPen(Qt::blue));
           pGraph3->setPen(QPen(Qt::red));


           m_customPlot->xAxis->setLabel("截面位置(mm)");
           m_customPlot->yAxis->setLabel("MPa");


           m_customPlot->rescaleAxes(true);
           m_customPlot->yAxis->setRangeUpper( m_customPlot->yAxis->range().upper+10);
             m_customPlot->replot();


}

void MainWindow::draw_obliqueCrack(int beamId)
{
    m_customPlot=ui->widget_11;

    XxwCustomPlot  *n_customPlot=ui->widget_15;
   QString str="斜截面抗裂验算";
     CavasSet(m_customPlot, str);
     CavasSet(n_customPlot,"sigma_tp");
      int nCount = 100;
      double ts=(mycat->myobs->cal_span*1e3/2)/100;
      QVector<double> x(nCount+1), y0(nCount+1), y1(nCount+1), y2(nCount+1), y3(nCount+1), y4(nCount+1),
               y5(nCount+1),y6(nCount+1), y7(nCount+1),
              y8(nCount+1),u0(2),V0(2);//, y1(nCount),y2(nCount); // initialize with entries 0..100
       for (int i = 0; i <nCount; ++i)
       {
           x[i] =i*ts; // x goes from -1 to 1
           vector<float> temp=mycat->obliqueCrack(beamId,x[i]);
           y0[i] = temp[0];
           y1[i] = temp[1];
           y2[i] = temp[2];
           y3[i] = temp[3];
           y4[i] = temp[4];
           y5[i] = temp[5];

}
       x[100]=mycat->myobs->cal_span*1e3/2;
       vector<float> temp=mycat->obliqueCrack(beamId,x[100]);
       y0[100] = temp[0];
       y1[100] = temp[1];
       y2[100] = temp[2];
       y3[100] = temp[3];
       y4[100] = temp[4];
       y5[100] = temp[5];
         u0[0]=0;
       u0[1]=x[100];
       V0[0]=0.7*ftk;
        V0[1]=0.7*ftk;
        QCPGraph *pGraph1 = m_customPlot->addGraph();
        QCPGraph *pGraph2 = m_customPlot->addGraph();
        QCPGraph *pGraph3 = m_customPlot->addGraph();
        QCPGraph *pGraph4 = n_customPlot->addGraph();
        QCPGraph *pGraph5 = n_customPlot->addGraph();
        QCPGraph *pGraph6 = n_customPlot->addGraph();
        QCPGraph *pGraph7 = m_customPlot->addGraph();

         pGraph1->setData(x,y0);
         pGraph2->setData(x,y1);
         pGraph3->setData(x,y2);
         pGraph4->setData(x,y3);
         pGraph5->setData(x,y4);
         pGraph6->setData(x,y5);
         pGraph7->setData(u0,V0);


          pGraph1->setName("a-a截面sigma_cp");
          pGraph2->setName("b-b截面sigma_cp");
          pGraph3->setName("c-c截面sigma_cp");
          pGraph4->setName("a-a截面sigma_tp");
          pGraph5->setName("b-b截面sigma_tp");
          pGraph6->setName("c-c截面sigma_tp");
          pGraph7->setName("0.7ftk");

           pGraph1->setPen(QPen(Qt::yellow));
           pGraph2->setPen(QPen(Qt::blue));
           pGraph3->setPen(QPen(Qt::green));
           pGraph4->setPen(QPen(Qt::darkBlue));
           pGraph5->setPen(QPen(Qt::darkMagenta));
           pGraph6->setPen(QPen(Qt::darkYellow));
           pGraph7->setPen(QPen(Qt::red));



           m_customPlot->xAxis->setLabel("截面位置(mm)");
           m_customPlot->yAxis->setLabel("MPa");

           n_customPlot->xAxis->setLabel("截面位置(mm)");
           n_customPlot->yAxis->setLabel("MPa");
           n_customPlot->rescaleAxes(true);
         m_customPlot->rescaleAxes(true);
           m_customPlot->yAxis->setRangeUpper( m_customPlot->yAxis->range().upper+10);

             n_customPlot->yAxis->setRange(n_customPlot->yAxis->range().lower*1.1,n_customPlot->yAxis->range().upper*1.1);
            n_customPlot->replot();
             m_customPlot->replot();
        }

void MainWindow::draw_Copingdeadload_M()
{ //盖梁上部结构弯矩剪力图;
    Coping mycp;
    m_customPlot=ui->widget_12;
   QString str="恒载弯矩";
    CavasSet(m_customPlot, str);
    int nCount = 100;
    double ts=(mycp.l)/100;
    QVector<double> x(nCount+1), y0(nCount+1), y1(nCount+1);
            //, y1(nCount),y2(nCount); // initialize with entries 0..100
     for (int i = 0; i <nCount; ++i)
     {
         x[i] =i*ts; // x goes from -1 to 1
         vector<float> temp=mycp.getInnerFoceAt(x[i]);
         y0[i] = temp[0];//弯矩
        y1[i] = mycp.CoopingBendingSolve( x[i]);//弯矩

}
     x[100]=mycp.l;
    vector<float> temp=mycp.getInnerFoceAt(x[100]);
     y0[100] = temp[0];
     y1[100] = mycp.CoopingBendingSolve( x[100]);//弯矩
     QCPGraph *pGraph1 = m_customPlot->addGraph();
     QCPGraph *pGraph2 = m_customPlot->addGraph();
     pGraph1->setData(x,y0);
     pGraph2->setData(x,y1);
     pGraph1->setPen(QPen(Qt::yellow));
     pGraph2->setPen(QPen(Qt::blue));
     pGraph1->setName("上部结构弯矩");
     pGraph2->setName("自重弯矩");
      m_customPlot->xAxis->setLabel("截面位置(mm)");
       m_customPlot->yAxis->setLabel("KN.m");
       m_customPlot->rescaleAxes(true);
       m_customPlot->yAxis->setRangeUpper( m_customPlot->yAxis->range().upper+500);


       m_customPlot->replot();



}

void MainWindow::draw_CopingDeadLoadSF()
{
    Coping mycp;
    m_customPlot=ui->widget_14;
   QString str="恒载剪力";
    CavasSet(m_customPlot, str);
    int nCount = 100;
    double ts=(mycp.l)/100;
    QVector<double> x(nCount+1), y0(nCount+1), y1(nCount+1);
    for (int i = 0; i <nCount; ++i)
    {
        x[i] =i*ts; // x goes from -1 to 1

        vector<float> temp=mycp.getInnerFoceAt(x[i]);
        y0[i] = temp[1];//恒载剪力
        y1[i] = mycp.CoopingSfSolve(x[i]);//自重剪力

}
    x[100]=mycp.l;

    vector<float> temp=mycp.getInnerFoceAt(x[100]);
    y0[100] = temp[1];
    y1[100] = mycp.CoopingSfSolve(x[100]);//剪力
    QCPGraph *pGraph1 = m_customPlot->addGraph();
    QCPGraph *pGraph2 = m_customPlot->addGraph();
    pGraph1->setData(x,y0);
    pGraph2->setData(x,y1);
    pGraph1->setPen(QPen(Qt::red));
    pGraph2->setPen(QPen(Qt::blue));
    pGraph1->setName("恒载剪力");
    pGraph2->setName("自重剪力");
     m_customPlot->xAxis->setLabel("截面位置(mm)");
      m_customPlot->yAxis->setLabel("kN");
    m_customPlot->rescaleAxes(true);
     m_customPlot->yAxis->setRange(m_customPlot->yAxis->range().lower*1.1, m_customPlot->yAxis->range().upper*1.1);
     m_customPlot->replot();

}

void MainWindow::draw_CopingCombinas()
{    mycat->mycp.demobs=*mycat->myobs;
    float index=ui->comboBox_15->currentIndex();
    if(index==0){
        draw_CoopingCombinaM();


    }else{
       draw_CoopingCombinaSf();
    }

}
void MainWindow::on_spinBox_16_valueChanged(int arg1)
{
    drawfocescheaking(arg1) ;
}

void MainWindow::mainBeamdeadLoad()
{//主梁内力页数据渲染
    vector<float> mydata;
        if(mycat->mymb==0){
            QMessageBox::information(this,QString("请先录入数据!"),QString("缺少数据,请先到数据录入界面存入相关数据!"));

        }else{

            mydata.push_back(mycat->mymb->mbd.BrigeWidth);
            mydata.push_back(mycat->mymb->mbd.AspLayer);
            mydata.push_back(mycat->mymb->mbd.MixedSoilLayer);
            mydata.push_back(mycat->mymb->mbd.MBArea);
            mydata.push_back(mycat->mymb->mbd.SideBeamArea);
            mydata.push_back(mycat->mymb->mbd.CenterCrossBeamV);
            mydata.push_back(mycat->mymb->mbd.SideCorssBeamV);
            mydata.push_back(mycat->mymb->mbd.CalculaSpan);
            mydata.push_back(mycat->mymb->mbd.gr);
            mydata.push_back(mycat->mymb->mbd.crossBeamNum);
            mydata.push_back(mycat->mymb->mbd.mianBeanNum);
            mydata.push_back(mycat->mymb->mbd.Main_Bean_field_making_Area);
            mydata.push_back(mycat->mymb->mbd.Side_Cross_Bean_field_making_Area);
            mydata.push_back(mycat->mymb->mbd.Midle_Cross_Bean_field_making_Area);
            mydata.push_back(mycat->mymb->mbd.Cross_Bean_t);
             QVariant data;
             data.setValue(mydata);
             emit task_6_send(data);

        }
}

void MainWindow::on_stackedWidget_currentChanged(int arg1)
{

}

void MainWindow::on_stackedWidget_2_currentChanged(int arg1)
{
    if(arg1==1){
        if(mycat->myobs==0){
                return;
            }
           draw_liveLoadM(arg1);
           draw_liveLoadSf(arg1);




            if((mycat->FulcrMidBeam==0)|(mycat->FulcrSideBeam==0)|(mycat->mid_SpanMidBeam==0)|(mycat->mid_SpanSideBeam==0)|(mycat->myobs==0)){
                return;
            }else{

                   vector<float> test;
                  test.push_back(mycat->myobs->bean_nums);
                  test.push_back(mycat->myobs->cal_span);
                  test.push_back(mycat->myobs->stoneWidth);
                  test.push_back((float)ui->spinBox->value());
                 QVariant data;
                  data.setValue(test);
                    emit task_9_send(data);

        }
    }

}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    if((mycat->FulcrMidBeam==0)|(mycat->FulcrSideBeam==0)|(mycat->mid_SpanMidBeam==0)|(mycat->mid_SpanSideBeam==0)|(mycat->myobs==0)){
        return;
    }else{

           vector<float> test;
          test.push_back(mycat->myobs->bean_nums);
          test.push_back(mycat->myobs->cal_span);
          test.push_back(mycat->myobs->stoneWidth);
          test.push_back((float)ui->spinBox->value());
         QVariant data;
          data.setValue(test);
            emit task_9_send(data);

}

}

void MainWindow::on_pushButton_2_clicked()
{

/*QTreeWidget *tw=ui->treeWidget;
QTreeWidgetItem *topItem1=tw->topLevelItem(0);
QTreeWidgetItem *topItem2=tw->topLevelItem(1);
QTreeWidgetItem *topItem3=tw->topLevelItem(2);
QTreeWidgetItem *items[6]={topItem1->child(0),topItem1->child(1),topItem1->child(2),topItem2->child(0),topItem2->child(1),topItem3->child(0)};
QTreeWidgetItem *qit=new QTreeWidgetItem();
qit->setText(0,"测试一个");
items[1]->addChild(qit);*/
//qDebug()<<mycat->myobs->PartialLoadMcq(1, 2)<<endl<<mycat->myobs->PartialLoadMcq(2, 2)<<endl<<mycat->myobs->PartialLoadMcq(3, 2)<<endl<<mycat->myobs->PartialLoadMcq(4, 2);



}
void MainWindow::SetPreSteelInfo()
{   QTableWidget *steeInfo=ui->tableWidget_46;
    for(int i=0;i<steeInfo->columnCount();i++){
        if(!tableItemCheak(steeInfo->item(1,i))){

            return;
        }
    }

    NominalStArea=steeInfo->item(1,0)->text().toFloat();
    Ep=steeInfo->item(1,1)->text().toFloat()*1e5;
    fpk=steeInfo->item(1,2)->text().toFloat();
     fpd=steeInfo->item(1,3)->text().toFloat();
     sigma_con=steeInfo->item(1,4)->text().toFloat();


}
void MainWindow::CavasSet(XxwCustomPlot *m_customPlot,QString str)
{
    m_customPlot->clearGraphs ( );
    m_customPlot->showTracer(true);
      if(!m_customPlot->plotLayout()->hasElement(1,0)){
          m_customPlot->plotLayout()->insertRow(0);
          m_customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(m_customPlot, str, QFont("黑体", 12, QFont::Bold)));
          }else{
          m_customPlot->plotLayout()->remove(m_customPlot->plotLayout()->element(0,0));
          m_customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(m_customPlot, str, QFont("黑体", 12, QFont::Bold)));
   }
     m_customPlot->legend->setVisible(true);
      QFont legendFont = font();  // start out with MainWindow's font..
      legendFont.setPointSize(9); // and make a bit smaller for legend
      m_customPlot->legend->setFont(legendFont);
      m_customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
      // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
      m_customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignCenter);
     // make left and bottom axes always transfer their ranges to right and top axes:
      connect(m_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->xAxis2, SLOT(setRange(QCPRange)));
      connect(m_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->yAxis2, SLOT(setRange(QCPRange)));
     // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
      m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

}
void MainWindow::on_spinBox_17_valueChanged(int arg1)
{
     deflectionRender(arg1);
}
void MainWindow::on_commandLinkButton_58_clicked()
{   QTableWidget *unpresteel=ui->tableWidget_48;
    for(int i=0;i<unpresteel->columnCount();i++){
        if(!tableItemCheak(unpresteel->item(1,i))){
            return;
        }

    }

    nv=unpresteel->item(1,0)->text().toInt();//肢数
    dv=unpresteel->item(1,1)->text().toFloat();//箍筋直径mm
    Fsv=unpresteel->item(1,2)->text().toFloat();;//箍筋强度

    if(lineEditCheak("lineEdit_5")&lineEditCheak("lineEdit_13")&lineEditCheak("lineEdit_14")){
         divid_X=lineEditDataProcess("lineEdit_5");//毫米计算
       Sv1=lineEditDataProcess("lineEdit_13");//箍筋间距1;
      Sv2=lineEditDataProcess("lineEdit_14");

    }

}

void MainWindow::on_horizontalSlider_7_valueChanged(int value)
{
    mycat->mycp.demobs=*mycat->myobs;
    int index=ui->comboBox_9->currentIndex()+1;
    float x=value/1e3;
   vector<float> liveload=mycat->mycp.getExtremumfoces(x);
   vector<float> deadLoad;
   vector<float> combinVal;
   deadLoad.push_back(mycat->mycp.CoopingBendingSolve(x));
   deadLoad.push_back(mycat->mycp.CoopingSfSum(x));
   switch (index) {
   case 1:
       combinVal=mycat->mycp.getCombinSud(x);
       break;
   case 2:
       combinVal=mycat->mycp.getCombinSfd(x);

       break;
   case 3:
       combinVal=mycat->mycp.getCombinSqd(x);
     break;
     default:
       break;
   }
   QTableWidget *tb1=ui->tableWidget_21;
   QTableWidget *tb2=ui->tableWidget_22;
   QTableWidget *tb3=ui->tableWidget_17;
   for(int i=0;i<tb1->columnCount();i++){
       QTableWidgetItem *it=new QTableWidgetItem();
       it->setText(QString::number(deadLoad[i],'g',6));
       tb1->setItem(1,i,it);

   }
   for(int i=0;i<tb2->columnCount();i++){
       QTableWidgetItem *it=new QTableWidgetItem();
       it->setText(QString::number(liveload[i],'g',6));
      tb2->setItem(1,i,it);

   }
   for(int i=0;i<tb3->columnCount();i++){
       QTableWidgetItem *it=new QTableWidgetItem();
       it->setText(QString::number(combinVal[i],'g',6));
      tb3->setItem(1,i,it);

   }





}

void MainWindow::on_horizontalSlider_8_valueChanged(int value)
{

}

void MainWindow::on_comboBox_14_currentIndexChanged(const QString &arg1)
{
    draw_CopingCombinas();
}

void MainWindow::on_comboBox_15_currentIndexChanged(int index)
{
    draw_CopingCombinas();
}

void MainWindow::on_pushButton_3_clicked()
{
     mycat->MDemonBeam.setSteel(mycat->paths);
    mycat->MDemonBeam.MainStress(16900, 1000,300);
}
