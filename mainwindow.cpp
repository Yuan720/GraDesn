#pragma execution_character_set("utf-8")
#pragma once
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QWidget>
#include<QPainter>
using namespace std;
Q_DECLARE_METATYPE(std::vector<float>)
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
    if(currentRow==1){
        onTabChoosed();
    }
}

void MainWindow::on_pushButton_3_clicked()
{   if((mycat->FulcrMidBeam==0)|(mycat->FulcrSideBeam==0)|(mycat->mid_SpanMidBeam==0)|(mycat->mid_SpanSideBeam==0)){
    QMessageBox::information(this,QString("数据缺失"),QString("请先预存储支点和跨中截面主梁尺寸!"));
    return;
    }

    bool toCountField;
    bool isMidBeaan;
    if(lineEditCheak("lineEdit_4")&lineEditCheak("lineEdit_3")&lineEditCheak("lineEdit_5")&lineEditCheak("lineEdit_6")){

        if(box_3_cheaked){
            toCountField=true;

      }else{
            toCountField=false;

}
        if(ui->comboBox_3->currentIndex()==0){
            //边梁
         isMidBeaan=false;
        }else{
            //中梁
        isMidBeaan=true;
        }
        vector<float> temp;
        temp.push_back(lineEditDataProcess("lineEdit_4"));
        temp.push_back(lineEditDataProcess("lineEdit_3"));
        temp.push_back(lineEditDataProcess("lineEdit_5"));
        temp.push_back(lineEditDataProcess("lineEdit_6"));
        QVariant data;
        data.setValue(temp);
        emit task_7_send(data,toCountField,isMidBeaan);
    }
    else
        {
        QMessageBox::information(this,QString("错误"),QString("输入数据不完整或输入数据类型有误!\n请检查后提交!"));
     }
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

   }else{

        QMessageBox::information(this,QString("错误"),QString("输入数据不完整或输入数据类型有误!\n请检查后提交!"));

    }
}






void MainWindow::on_commandLinkButton_4_clicked()
{
    if(generalTableCheak("tableWidget_8",15))
    {
   QVariant data=getGeneralTableData("tableWidget_8",15);
   emit task_6_send(data);
    }
    else
    {

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
            return true;

        }
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
void MainWindow::resultRender3(QVariant v){
    vector<float> res=v.value<vector<float>>();
    QTableWidget *qtw=ui->tableWidget_15;
    for(int i=0;i<4;i++){
        QTableWidgetItem *it=new QTableWidgetItem();
        it->setText(QString("%1").arg(res[i]));
        qtw->setItem(1,i+1,it);

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
void MainWindow::eff_combinRender(QVariant v){
    vector<float> input=v.value<vector<float>>();

   QTableWidget *qtw=ui->tableWidget_22;
   int count=0;
   for(int i=1;i<qtw->rowCount();i++){
       for(int j=1;j<qtw->columnCount();j++){
           QTableWidgetItem *it=new QTableWidgetItem();
           it->setText(QString("%1").arg(input[count]));
     qtw->setItem(i,j,it);
     count++;

       }


   }

}
void MainWindow::renderThridLoad(QVariant v ){
    vector<float> input=v.value<vector<float>>();
    QTableWidget *qw=ui->tableWidget_21;
    for(int i=0;i<input.size();i++){

        QTableWidgetItem *it=new QTableWidgetItem();
        it->setText(QString("%1").arg(input[i]));
  qw->setItem(1,i,it);
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
    generalTableInit("tableWidget_8");
    ui->label_28->hide();
    ui->spinBox->hide();
    ui->spinBox_2->hide();
    ui->label_19->hide();
    ui->comboBox_4->hide();
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
    connect(this,&MainWindow::task_8_send,mycat,&CacularThread::task_8_process);
    connect(mycat,&CacularThread::onTask_8_finished,this,&MainWindow::resultRender3);
    connect(this,&MainWindow::task_9_send,mycat,&CacularThread::task_9_process);
    connect(mycat,&CacularThread::onTask_9_finished,this,&MainWindow::resultRender4);
    connect(mycat,&CacularThread::mcqRender,this,&MainWindow::mcqRender);
    connect(this,&MainWindow::thridStageLoad,mycat,&CacularThread::getThridLoad);
    connect(mycat,&CacularThread::thridLoadFinished,this,&MainWindow::renderThridLoad);
    connect(this,&MainWindow::eff_combin,mycat,&CacularThread::effCombin);
    connect(mycat,&CacularThread::eff_combinFinished,this,&MainWindow::eff_combinRender);
    connect(this,&MainWindow::SectionCompute,mycat,&CacularThread::task_10_process);
    connect(mycat,&CacularThread::Sectionfinished,this,&MainWindow::render4);

}
void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
   ui-> stackedWidget->setCurrentIndex(currentRow);

}






void MainWindow::on_commandLinkButton_5_clicked()
{   float num1;
    float num2;
    if(ui->tableWidget_14->item(1,0)->text()==""){
        QMessageBox::information(this,QString("错误"),QString("请先计算恒载!"));

    }else{
        bool isInvalid;
       QString qs=ui->tableWidget_15->item(1,0)->text();
      float a=qs.toFloat(&isInvalid);
      if(isInvalid){
          if(ui->comboBox_2->currentIndex()==0){
              //一期
              num1=ui->tableWidget_14->item(1,0)->text().toFloat();


              num2=ui->tableWidget_14->item(1,1)->text().toFloat();



          }else{
            //二期
              num1=ui->tableWidget_14->item(3,0)->text().toFloat();
              num2=ui->tableWidget_14->item(3,1)->text().toFloat();
          }
        float  mnum=ui->tableWidget_14->item(5,0)->text().toFloat();

          emit task_8_send(a,num1,num2);
           thridStageLoad(mnum,a);


      }else{
           QMessageBox::information(this,QString("错误"),QString("x应该为数值且不为空!"));

      }

    }
}
void MainWindow::on_pushButton_4_clicked()
{
    //主梁活载内力计算按钮
    if((mycat->FulcrMidBeam==0)|(mycat->FulcrSideBeam==0)|(mycat->mid_SpanMidBeam==0)|(mycat->mid_SpanSideBeam==0)){
        QMessageBox::information(this,QString("数据缺失"),QString("请先预存储支点和跨中截面主梁尺寸!"));
        return;
    }else{
        if( generalTableCheak("tableWidget_17",3)){


          QVariant task=getGeneralTableData ("tableWidget_17",3);
          vector<float> test=task.value<vector<float>>();
          test.push_back((float)ui->spinBox->value());


          QVariant data;
          data.setValue(test);
            emit task_9_send(data);





        }else{

            QMessageBox::information(this,QString("错误"),QString("输入数据不完整或输入数据类型有误!\n请检查后提交!"));

        }

    }
}

void MainWindow::on_tableWidget_17_itemChanged(QTableWidgetItem *item)
{
    if(item->row()==0&item->column()==1){
        bool isInvalid;
        int a=ui->tableWidget_17->item(0,1)->text().toInt(&isInvalid);
        if(isInvalid){
            ui->spinBox->setMinimum(1);
           ui->spinBox->setMaximum(a);
           ui->label_28->show();
          ui->spinBox->show();
        }
        else{
           ui->label_28->hide();
          ui->spinBox->hide();
        }

    }
}
void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(index==3&ui->listWidget_2->currentRow()==1){
   onTabChoosed();
    }


}
void MainWindow::onTabChoosed(){

    if(mycat->myobs==0|mycat->mymb==0){

        QMessageBox::information(this,QString("请先完善数据!"),QString("要计算作用效应组合,需要先在恒载和活载\n内力计算界面预输入必要数据!"));


    }else{
        float a=mycat->myobs->bean_nums;
        ui->spinBox_2->setMinimum(1);
        ui->spinBox_2->setMaximum(a);
        ui->label_19->show();
        ui->spinBox_2->show();
        ui->comboBox_4->show();
        ui->label_31->show();
        int saftyLevel= ui->comboBox_4->currentIndex()+1;

        emit eff_combin(ui->spinBox_2->value(),saftyLevel);


    }


}
void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    int saftyLevel= ui->comboBox_4->currentIndex()+1;

    emit eff_combin(ui->spinBox_2->value(),saftyLevel);

}

void MainWindow::on_comboBox_4_currentIndexChanged(int index)
{
    int saftyLevel= ui->comboBox_4->currentIndex()+1;

    emit eff_combin(ui->spinBox_2->value(),saftyLevel);
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
void MainWindow::on_commandLinkButton_6_clicked()
{
    int itemCount=6;
    QTableWidget  *qtw=ui->tableWidget_23;
    bool isInvalid;
    int cheakedCount=0;
    vector<float> callDatas;

        for(int i=1;i<qtw->rowCount();i+=2){
            for(int j=0;j<qtw->columnCount();j++){
                QString qs=qtw->item(i,j)==0? " ":qtw->item(i,j)->text();

                if(cheakedCount<itemCount){
                 callDatas.push_back(qs.toFloat(&isInvalid));
                }
                if(!isInvalid){

                    QMessageBox::information(this,QString("错误"),QString("输入数据不完整或输入数据类型有误!\n请检查后提交!"));
                    return;

                }
                  cheakedCount++;

            }


Point start(callDatas[0],callDatas[1]);
Point turn(callDatas[2],callDatas[3]);
myPath path(start,turn,callDatas[4],callDatas[5]);

Steelplot(path);





    }

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


          }else{
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
    }else{
        if(generalTableCheak("tableWidget_190",16)&table_192_cheak){
            mycat->Transtionhalf_start=ui->tableWidget_192->item(1,0)->text().toFloat();
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
    for(int i=1;i<=4;i++){
        myPath temp(Point(res[(i-1)*7],res[(i-1)*7+1]),Point(res[(i-1)*7+2],res[(i-1)*7+3]),res[(i-1)*7+4],1000*mycat->bridgeSpan/2,res[(i-1)*7+5],res[(i-1)*7+6]);
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



}
void MainWindow::on_pushButton_2_clicked()
{   qDebug()<<"调试..... "<<endl;





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
{ QTableWidget *tb=ui->tableWidget_2;
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
{       bool precounted;
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
{
   vector<float> temp;
   temp.push_back(mycat->mymb->mbd.CalculaSpan*1000);
   temp.push_back(mycat->mymb->mbd.AspLayer);
   temp.push_back(mycat->mymb->mbd.MixedSoilLayer);
   temp.push_back(mycat->mymb->mbd.CenterCrossBeamV);
   temp.push_back(mycat->mymb->mbd.SideCorssBeamV);
   temp.push_back(mycat->Transtionhalf_start);
   temp.push_back(mycat->Transtionhalf_end);
   temp.push_back(mycat->mymb->mbd.Side_Cross_Bean_field_making_Area);
   temp.push_back(mycat->mymb->mbd.Midle_Cross_Bean_field_making_Area);
   temp.push_back(mycat->mymb->mbd.Cross_Bean_t);
   beam  mybeam(*mycat->FulcrMidBeam,*mycat->mid_SpanMidBeam,temp,5);
   mybeam.setSteel(mycat->paths);
   qDebug()<<mybeam.getSigma_l6(6200)<<endl<<"beam分割!!!!";

   qDebug()<<mycat->getSigma_l6(6200,true);


}
