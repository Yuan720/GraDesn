#pragma once
#ifndef VARIABLES_H
#define VARIABLES_H
extern float bridge_total_Span;
//ԤӦ���ֽ��ߵ���ģ��
extern float Ep;
//����������ģ��
extern float Ec;
//ָ������ê�߱���ֵ
extern float delta_l;
extern float bridge_calspan;
extern double Pi;
extern float fpk;
extern float fcuk;//������ǿ�ȵȼ�;
extern float ftd;//���������Ŀ���;
//Ħ��ϵ����Ħ��Ӱ��ϵ��
extern float mu;
extern float k;
//֧������������;
extern float pivot_X;
//��ԤӦ���ֽ����
extern float non_preSteel_area;

//��ԤӦ���ֽ�����
extern float non_prestreSteel_s;
//��ԤӦ���ֽ��������
extern float non_prestrepip_center;
//m��������
extern int m;
//���ϵ��  ����Ӧ���ռ�ֵ
extern float phi_1;
extern float phi_2;
extern float xi_cs;
extern float fcd;//��ԤӦ���ֽ����Ŀ�ѹǿ��
extern float fpd;//ԤӦ���ֽ��ǿ�����ֵ
extern float fsd;//��ԤӦ���ֽ��ǿ�����ֵ
extern float Ap;//ԤӦ�������;
extern float As;//��ԤӦ���ֽ����
extern float as;
extern float ap;
extern float cross_beam_foce[5][6];
extern int saftyGrade;
extern int numberOfBeam;
extern float NominalStArea;//ԤӦ��������;
extern float Fsv;//����ǿ��
extern float nv;//֫��
extern float dv;//����ֱ��
extern float Sv1;//������1;
extern float Sv2;
extern float divid_X;
extern float sigma_con;//��������Ӧ��;
extern float feck;
extern float fck;
#endif // VARIABLES_H
