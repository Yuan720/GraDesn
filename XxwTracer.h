#ifndef MYTRACER_H
#define MYTRACER_H

#include <QObject>
#include "qcustomplot.h"

///
/// \brief The XxwTracer class:��ͼ������ʾ�������λ�õ�x,yֵ��׷����ʾ��
///
class XxwTracer : public QObject
{
	Q_OBJECT

public:
	enum TracerType
	{
        XAxisTracer,//������x������ʾxֵ
        YAxisTracer,//������y������ʾyֵ
        DataTracer//��ͼ����ʾx,yֵ
	};
    explicit XxwTracer(QCustomPlot *_plot, TracerType _type, QObject *parent = Q_NULLPTR);
    ~XxwTracer();
	void setPen(const QPen &pen);
	void setBrush(const QBrush &brush);
	void setText(const QString &text);
	void setLabelPen(const QPen &pen);
	void updatePosition(double xValue, double yValue);
    void setVisible(bool m_visible);

protected:
    bool m_visible;//�Ƿ�ɼ�
    TracerType m_type;//����
    QCustomPlot *m_plot;//ͼ��
    QCPItemTracer *m_tracer;//���ٵĵ�
    QCPItemText *m_label;//��ʾ����ֵ
    QCPItemLine *m_arrow;//��ͷ
};


///
/// \brief The XxwCrossLine class:������ʾ����ƶ������е����λ�õ�ֱ��
///
class XxwTraceLine : public QObject
{
public:
    enum LineType
    {
        VerticalLine,//��ֱ��
        HorizonLine, //ˮƽ��
        Both//ͬʱ��ʾˮƽ�ʹ�ֱ��
    };
    explicit XxwTraceLine(QCustomPlot *_plot, LineType _type = VerticalLine, QObject *parent = Q_NULLPTR);
    ~XxwTraceLine();
    void initLine();
    void updatePosition(double xValue, double yValue);

    void setVisible(bool vis)
    {
        if(m_lineV)
            m_lineV->setVisible(vis);
        if(m_lineH)
            m_lineH->setVisible(vis);
    }

protected:
    bool m_visible;//�Ƿ�ɼ�
    LineType m_type;//����
    QCustomPlot *m_plot;//ͼ��
    QCPItemStraightLine *m_lineV; //��ֱ��
    QCPItemStraightLine *m_lineH; //ˮƽ��
};

#endif // MYTRACER_H
