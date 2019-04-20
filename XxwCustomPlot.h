#ifndef XCUSTOMPLOT_H
#define XCUSTOMPLOT_H

#include "XxwTracer.h"
#include "qcustomplot.h"
#include <QObject>
#include <QList>

class XxwCustomPlot:public QCustomPlot
{
    Q_OBJECT

public:
    XxwCustomPlot(QWidget *parent = 0);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);

public:
    ///
    /// \brief �����Ƿ���ʾ���׷����
    /// \param show:�Ƿ���ʾ
    ///
    void showTracer(bool show)
    {
        m_isShowTracer = show;
        if(m_xTracer)
            m_xTracer->setVisible(m_isShowTracer);
        foreach (XxwTracer *tracer, m_dataTracers)
        {
            if(tracer)
                tracer->setVisible(m_isShowTracer);
        }
        if(m_lineTracer)
            m_lineTracer->setVisible(m_isShowTracer);
    }

    ///
    /// \brief �Ƿ���ʾ���׷����
    /// \return
    ///
    bool isShowTracer(){return m_isShowTracer;}

private:
    bool m_isShowTracer;//�Ƿ���ʾ׷�����������ͼ���ƶ�����ʾ��Ӧ��ֵ��
    XxwTracer *m_xTracer;//x��
    XxwTracer *m_yTracer;//y��
    QList<XxwTracer *> m_dataTracers;//
    XxwTraceLine  *m_lineTracer;//ֱ��
};

#endif // XCUSTOMPLOT_H
