#pragma once
#include<math.h>
float min(float a, float b);
float max(float a, float b);
struct CarrigeWayPlate
{
	//沥青混凝土铺装厚度	
	float AspLayer;
	//水泥混凝土铺装厚度
	float MixedSoilLayer;
	//板的净跨径
	float l0;
	//板的厚度
	float t;
	//a2,b2轮胎与桥面接触的矩形面的尺寸,b梁肋宽度;P标准轴载;h肋高
    float a2, b2, b, d, P, h, mu ;

};
//悬壁板
struct CantileverPlate {
	//沥青混凝土铺装厚度	
	float AspLayer;
	//水泥混凝土铺装厚度
	float MixedSoilLayer;
	//t翼缘板厚
	float t;
	//g沥青混泥土铺装,水泥混泥土铺装,翼缘板集度之和.
    float   lb, gb , g ;



};

class CarrigeWayPlateSolver {
public:
	CarrigeWayPlate MyCarrige;
	CarrigeWayPlateSolver(CarrigeWayPlate v) {
		MyCarrige = v;
	}
	float * basicValueSolve(bool isBendingSolve = true) {
		float *result = new float[14];
		float a1 = MyCarrige.a2 + 2 * (MyCarrige.AspLayer + MyCarrige.MixedSoilLayer);
		float b1 = MyCarrige.b2 + 2 * (MyCarrige.AspLayer + MyCarrige.MixedSoilLayer);
		float l;
		float x;
		if (isBendingSolve)
		{
			l = MyCarrige.l0 + min(MyCarrige.t, MyCarrige.b);
			x = b1 / 2 - (1.3 - l / 2);
		}
		else {
			l = MyCarrige.l0;
			x = b1 - (b1 + 1.3 - l);
		}

		float a_e = a1 + MyCarrige.t;
		float a = max(a1 + l / 3, 2 * l / 3);
		float c = (a - a_e) / 2;
		if (a > MyCarrige.d)
		{
			a = a + MyCarrige.d;
		}
		float e = (MyCarrige.d - a_e) / 2;
		float ax = a_e + 2 * x + MyCarrige.d*(a > MyCarrige.d)*(x > e);
		float ae = a_e + 2 * e;
		float g1 = 23 * MyCarrige.AspLayer * 1;
		float g2 = 24 * MyCarrige.MixedSoilLayer * 1;
		float g3 = 25 * MyCarrige.t * 1;
		float gsum = g1 + g2 + g3;
		result[0] = a1; result[1] = b1; result[2] = l; result[3] = a_e; result[4] = a; result[5] = c; result[6] = e; result[7] = x; result[8] = ax; result[9] = ae;
		result[10] = g1; result[11] = g2; result[12] = g3; result[13] = gsum;
		return result;
	}
	float * P_Value_Solve(float *va)
	{
		//p->荷载作用在道板中间对应P
		float p;
		if (*(va + 4) > MyCarrige.d)
		{
			p = (MyCarrige.P / 2 * 2) / (*(va + 4)*(*(va + 1)));
		}
		else {
			p = (MyCarrige.P / 2) / (*(va + 4)*(*(va + 1)));
		}
		//p'->pfe荷载作用在支撑处对应p
		float pfe = (MyCarrige.P / 2) / (*(va + 3)*(*(va + 1)));
		//pe荷载作用在作用重叠交点对应的p
		float pe = (MyCarrige.P / 2) / (*(va + 9)*(*(va + 1)));
		//荷载作用在右边轮里侧对应P
		float px = (MyCarrige.P / 2) / (*(va + 8)*(*(va + 1)));
		float ae2 = va[4] - 2 * (va[5] - va[6]);
		float pe2 = (MyCarrige.P / 2) / (ae2*(*(va + 1)));

		float *pv = new float[5];
		pv[0] = p;	pv[1] = pfe;	pv[2] = pe;	pv[3] = px; pv[4] = pe2;
		return pv;

	}
	//计算活载弯矩
	float * CarrigeBendingSolve() {
		float *myresult = new float[5];
		float *m = basicValueSolve();
		float *pvalues = P_Value_Solve(m);
		if (*(m + 7) <= 0)
		{
			//已验证
			myresult[0] = getFoceArea(pvalues[0], m[1])*getOrdinate(m[2] / 2 - m[1] / 4);

		}
		else

		{
			//重叠且右轮进来
			if (*(m + 4) > MyCarrige.d)
			{

				//判断x与e的位置关系
				if (*(m + 7) > m[6])
				{

					//判断x与c关系
					if (*(m + 7) > m[5])
					{
						myresult[0] = getFoceArea(pvalues[0], m[1])*getOrdinate(m[2] / 2 - m[1] / 4) + getFoceArea(pvalues[2], m[6])*getOrdinate(m[6] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[2], m[6])*getOrdinate(m[6] / 3)
							+ getFoceArea(pvalues[3], m[7] - m[6])*getOrdinate(m[6] + (m[7] - m[6]) / 2) + getTriangleFoceArea(pvalues[4] - pvalues[3], m[7] - m[6])*getOrdinate(m[6] + (m[7] - m[6]) / 3);
					}
					else {

						myresult[0] = getFoceArea(pvalues[0], m[1]) *getOrdinate(m[2] / 2 - m[1] / 4) + getFoceArea(pvalues[2], m[6])*getOrdinate(m[6] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[2], m[6])*getOrdinate(m[6] / 3)
							+ getFoceArea(pvalues[0], m[7] - m[6])*getOrdinate(m[6] + (m[7] - m[6]) / 2) + getTriangleFoceArea(pvalues[4] - pvalues[0], m[5] - m[6])*getOrdinate(m[6] + (m[5] - m[6]) / 3);

					}

				}
				else
				{

					//当x<e时
					myresult[0] = getFoceArea(pvalues[0], m[1])*getOrdinate(m[2] / 2 - m[1] / 4) + getFoceArea(pvalues[3], m[7])*getOrdinate(m[7] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[3], m[7])*getOrdinate(m[7] / 3);


				}


			}
			else

			{
				//不重叠但右轮进来
					//判断x与c关系
				if (*(m + 7) > m[5])
				{
					myresult[0] = getFoceArea(pvalues[0], m[1])*getOrdinate(m[2] / 2 - m[1] / 4) + getFoceArea(pvalues[0], m[7])*getOrdinate(m[7] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[0], m[5])*getOrdinate(m[5] / 3);
				}
				else
				{//已验证
					myresult[0] = getFoceArea(pvalues[0], m[1])*getOrdinate(m[2] / 2 - m[1] / 4) + getFoceArea(pvalues[3], m[7])*getOrdinate(m[7] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[3], m[7])*getOrdinate(m[7] / 3);
				}



			}


		}
		myresult[1] = ((float)1 / 8)*m[13] * pow(m[2], 2);
		myresult[2] = 1.2*myresult[1] + 1.8*(1 + MyCarrige.mu)*myresult[0];
		if (MyCarrige.t / MyCarrige.h < 0.25)
		{
			myresult[3] = 0.5*myresult[2];

		}
		else {
			myresult[3] = 0.7*myresult[2];

		}
		myresult[4] = -0.7*myresult[2];


		return myresult;
	}
	float * CarrigeShearSolve() {
		float *myresult = new float[5];
		float *m = basicValueSolve(false);
		float *pvalues = P_Value_Solve(m);

		if (m[4] > MyCarrige.d)
		{//当发生轮胎重叠时
			//todo

			if (m[7] <= 0)
			{
				if (m[1] > m[5]) {
					myresult[0] = getFoceArea(pvalues[0], m[1])*getShearOrdinate(m[1] / 2)
						+ getTriangleFoceArea(pvalues[4] - pvalues[0], m[5] - m[6])*getShearOrdinate(m[6] + (m[5] - m[6]) / 3) + getFoceArea(pvalues[2] - pvalues[0], m[6])*getShearOrdinate(m[6] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[2], m[6])*getShearOrdinate(m[6] / 3);

				}
				else {
					if (m[1] > m[6]) {
						float temp_h = pvalues[0] + (m[5] - m[1]) / (m[5] - m[6])*(pvalues[4] - pvalues[0]);
						float temp_x = m[1] - m[6];
						myresult[0] = getFoceArea(temp_h, m[1])*getShearOrdinate(m[1] / 2) + getTriangleFoceArea(pvalues[4] - temp_h, temp_x)*getShearOrdinate(m[6] + temp_x / 3) + getFoceArea(pvalues[2] - temp_h, m[6])*getShearOrdinate(m[6] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[2], m[6])*getShearOrdinate(m[6] / 3);


					}
					else {
						float temp_h = pvalues[2] + (pvalues[1] - pvalues[2])*(m[6] - m[1]) / m[6];
						myresult[0] = getFoceArea(temp_h, m[1])*getShearOrdinate(m[1] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[2], m[1])*getShearOrdinate(m[1] / 3);
					}

				}
			}
			else
			{
				//重叠且右轮进入
				if (m[1] > m[5])
				{
					myresult[0] = getFoceArea(pvalues[0], m[1])*getShearOrdinate(m[1] / 2)
						+ getTriangleFoceArea(pvalues[4] - pvalues[0], m[5] - m[6])*getShearOrdinate(m[6] + (m[5] - m[6]) / 3) + getFoceArea(pvalues[2] - pvalues[0], m[6])*getShearOrdinate(m[6] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[2], m[6])*getShearOrdinate(m[6] / 3);

				}
				else
				{
					if (m[1] > m[6]) {
						float temp_h = pvalues[0] + (m[5] - m[1]) / (m[5] - m[6])*(pvalues[4] - pvalues[0]);
						float temp_x = m[1] - m[6];
						myresult[0] = getFoceArea(temp_h, m[1])*getShearOrdinate(m[1] / 2) + getTriangleFoceArea(pvalues[4] - temp_h, temp_x)*getShearOrdinate(m[6] + temp_x / 3) + getFoceArea(pvalues[2] - temp_h, m[6])*getShearOrdinate(m[6] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[2], m[6])*getShearOrdinate(m[6] / 3);


					}
					else
					{
						float temp_h = pvalues[2] + (pvalues[1] - pvalues[2])*(m[6] - m[1]) / m[6];
						myresult[0] = getFoceArea(temp_h, m[1])*getShearOrdinate(m[1] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[2], m[1])*getShearOrdinate(m[1] / 3);

					}

				}



				if (m[7] < m[6])
				{//右轮压在e之前
					if (m[7] > m[1])
					{//右轮完全进入
						float temp_h1 = pvalues[2] + (pvalues[1] - pvalues[2])*(m[6] - m[7]) / m[6];
						float temp_h2 = pvalues[2] + (pvalues[1] - pvalues[2])*(m[6] - m[7] + m[1]) / m[6];

						myresult[0] = myresult[0] + getFoceArea(temp_h1, m[1])*getShearOrdinate(m[2] - m[7] + m[1] / 2) + getTriangleFoceArea(temp_h2 - temp_h1, m[1])*getShearOrdinate(m[2] - m[7] + 2 * m[1] / 3);
					}
					else {
						//右轮没有完全进入
						float temp_h = (m[6] - m[7]) / m[6] * (pvalues[1] - pvalues[2]) + pvalues[2];
						myresult[0] = myresult[0] + getFoceArea(temp_h, m[7])*getShearOrdinate(m[2] - m[7] / 2) + getTriangleFoceArea(pvalues[1] - temp_h, m[7])*getShearOrdinate(m[2] - m[7] / 3);
					}
				}
				else {
					//右轮在e与c之间
					if (m[7] < m[5])
					{

						if (m[7] > m[1])
						{//右轮完全进入,默认c-e是小于b1的
							float temp_h1 = (m[6] - (m[7] - m[1])) / m[6] * (pvalues[1] - pvalues[2]) + pvalues[2];
							float temp_h2 = (m[5] - m[7]) / (m[5] - m[6])*(pvalues[4] - pvalues[0]) + pvalues[0];
							float temp_l = m[6] - (m[7] - m[1]);
							float temp_l2 = m[1] - temp_l;
							myresult[0] = myresult[0] + getFoceArea(pvalues[2], temp_l)*getShearOrdinate(m[2] - m[6] + temp_l / 2) + getTriangleFoceArea(temp_h1 - pvalues[2], temp_l)*getShearOrdinate(m[2] - m[6] + 2 * temp_l / 3) + getFoceArea(temp_h2, temp_l2)*getShearOrdinate(m[2] - m[7] + temp_l2 / 2) + getTriangleFoceArea(pvalues[4] - temp_h2, temp_l2)*getShearOrdinate(m[2] - m[7] + 2 * temp_l2 / 3);

						}
						else
						{
							//右轮没有完全进入
							float temp_h = pvalues[0] + (pvalues[4] - pvalues[0])*(m[5] - m[7]) / (m[5] - m[6]);
							float temp_l = m[7] - m[6];

							myresult[0] = myresult[0] + getFoceArea(pvalues[2], m[6])*getShearOrdinate(m[2] - m[6] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[2], m[6])*getShearOrdinate(m[2] - 2 * m[6] / 3)
								+ getFoceArea(temp_h, temp_l)*getShearOrdinate(m[2] - m[7] + temp_l / 2) + getTriangleFoceArea(pvalues[4] - temp_h, temp_l)*getShearOrdinate(m[2] - m[7] + 2 * temp_l / 3);


						}

					}
					else {
						//右轮在c的左边,过于复杂,只讨论轮是否完全进入,完全进入时默认轮右边缘在e之前;
						if (m[7] > m[1])

						{//右轮完全进入
							float temp_l = m[7] - m[1];
							float temp_h = (pvalues[1] - pvalues[2])*(m[6] - temp_l) / m[6];
							myresult[0] = myresult[0] + getFoceArea(pvalues[2], m[6])*getShearOrdinate(m[2] - m[6] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[2], m[6])*getShearOrdinate(m[2] - 2 * m[6] / 3) + getFoceArea(pvalues[0], m[5] - m[6])*getShearOrdinate(m[2] - m[6] - (m[5] - m[6]) / 2) + getTriangleFoceArea(pvalues[4] - pvalues[0], m[5] - m[6])*getShearOrdinate(m[2] - m[6] - (m[5] - m[6]) / 3) + getFoceArea(pvalues[0], m[7] - m[5])*getShearOrdinate(m[2] - m[5] - (m[7] - m[5]) / 2) - getFoceArea(temp_h, temp_l)*getShearOrdinate(m[2] - temp_l / 2) - getTriangleFoceArea(pvalues[1] - temp_h, temp_l)*getShearOrdinate(m[2] - temp_l / 3);
						}
						else

						{//未完全进入
							myresult[0] = myresult[0] + getFoceArea(pvalues[2], m[6])*getShearOrdinate(m[2] - m[6] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[2], m[6])*getShearOrdinate(m[2] - 2 * m[6] / 3) + getFoceArea(pvalues[0], m[5] - m[6])*getShearOrdinate(m[2] - m[6] - (m[5] - m[6]) / 2) + getTriangleFoceArea(pvalues[4] - pvalues[0], m[5] - m[6])*getShearOrdinate(m[2] - m[6] - (m[5] - m[6]) / 3) + getFoceArea(pvalues[0], m[7] - m[5])*getShearOrdinate(m[2] - m[5] - (m[7] - m[5]) / 2);

						}

					}
				}



			}
		}
		else
		{
			//不发生轮胎重叠时
			if (m[7] <= 0) {
				if (m[1] < m[5])
				{
					float ax2 = 2 * m[1] + m[3];
					float px2 = (MyCarrige.P / 2) / (ax2*m[1]);
					myresult[0] = getFoceArea(px2, m[1])*getShearOrdinate(m[1] / 2) + getTriangleFoceArea(pvalues[1] - px2, m[1])*getShearOrdinate(m[1] / 3);

				}
				else {
					myresult[0] = getFoceArea(pvalues[0], m[1])*getShearOrdinate(m[1] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[0], m[5])*getShearOrdinate(m[5] / 2);

				}
				//右轮不进入时
			}
			else {
				//右轮进入时

				if (m[1] < m[5])
				{//当轮宽b1小于c时
					float ax2 = 2 * m[1] + m[3];
					float px2 = (MyCarrige.P / 2) / (ax2*m[1]);
					if (m[7] <= m[1])
					{//当右轮没有完全压入时
						myresult[0] = getFoceArea(px2, m[1])*getShearOrdinate(m[1] / 2) + getTriangleFoceArea(pvalues[1] - px2, m[1])*getShearOrdinate(m[1] / 3) + getFoceArea(pvalues[3], m[7])*getShearOrdinate(m[2] - m[7] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[3], m[7])*getShearOrdinate(m[2] - m[7] / 2);
					}
					else {
						//右轮完全压入时
						if (m[7] - m[5] >= m[1])
						{
							//当右轮完全压在c左边时
												//todo
						}
						else {
							//当右轮不完全压在c左边时
							//todo
						}
					}


				}
				else {
					//当b1大于C时
					//这里指左轮的图乘值
					myresult[0] = getFoceArea(pvalues[0], m[1])*getShearOrdinate(m[1] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[0], m[5])*getShearOrdinate(m[5] / 2);
					if (m[7] >= m[5])
					{///当x>c
						if (m[7] < m[1])
						{//当x值不大于b1值时候
							myresult[0] = myresult[0] + getFoceArea(pvalues[0], m[7])*getShearOrdinate(m[2] - m[7] / 2) + getTriangleFoceArea(pvalues[1] - pvalues[0], m[5])*getShearOrdinate(m[2] - m[5] / 3);
						}
						else {
							float temp_x = m[5] - (m[7] - m[1]);
							float temp_h = (pvalues[1] - pvalues[0]) *(m[5] - (m[7] - m[1])) / m[5];

							myresult[0] = myresult[0] + getFoceArea(pvalues[0], m[1])*getShearOrdinate(m[2] - (m[7] - m[1] / 2)) + getTriangleFoceArea(temp_h, temp_x)*getShearOrdinate(m[2] - m[5] + ((float)2 / 3)*temp_x);
						}

					}
					else {
						//当x不大于c时
						if (m[7] <= m[1])
						{//当x值不大于b1值时候
							float temp_h = (pvalues[1] - pvalues[0])*(m[5] - m[7]) / m[5];

							myresult[0] = myresult[0] + getFoceArea(pvalues[0] + temp_h, m[7])*getShearOrdinate(m[2] - m[7] / 2) + getTriangleFoceArea(pvalues[1] - temp_h, m[7])*getShearOrdinate(m[2] - m[7] / 3);
						}
						else {
							float temp_h1 = pvalues[0] + (pvalues[1] - pvalues[0])*(m[5] - m[7]) / m[5];
							float temp_h2 = pvalues[0] + (pvalues[1] - pvalues[0])*(m[5] - m[7] + m[1]) / m[5];
							myresult[0] = myresult[0] + getFoceArea(temp_h1, m[1])*getShearOrdinate(m[2] - m[7] + m[1] / 2) + getTriangleFoceArea(temp_h2 - temp_h1, m[7])*getShearOrdinate(m[2] - m[7] + 2 * m[1] / 3);
						}

					}


				}
			}

		}
        myresult[1] = 0.5*m[13] * m[2];
        myresult[2] = 1.2*myresult[1] + 1.8*1.3*myresult[0];
		return myresult;

	}
	//求面积
	float getFoceArea(float len, float q) {
		return len * q;
	}
	float getTriangleFoceArea(float len, float q) {
		return len * q*0.5;
	}
	//求yx
	float getOrdinate(float x) {
		return 0.5*x;

	}
	float getShearOrdinate(float x) {
		float *temp = basicValueSolve(false);
		return (temp[2] - x) / temp[2];
	}
};
class CantileverPlateSolver {
	CantileverPlate clp;
public:
	CantileverPlateSolver(CantileverPlate v) {
		clp = v;

	}
	float BendingSolver() {
        //有错误
		return  -0.5*clp.g*pow(clp.lb, 2) - clp.gb*clp.lb;
	}
	float ShearFoceSolver() {
        //有错误
		return clp.g*clp.lb + clp.gb;
	}



};
