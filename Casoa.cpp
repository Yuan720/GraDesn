#pragma execution_character_set("utf-8")
#include<Casoa.h>
#include<QDebug>
int saftyGrade =1;
float ultimateLimitSta(float dl, float cl){
	float param1 = 1.2 - 0.1*saftyGrade;

	param1 = param1 < 0.9 ? 0.9 : param1;
	return param1 * (1.2*dl + 1.4 * 1 * cl);
}
float sfd(float dl, float cl) {
	return dl + 0.7*cl;
}
float sqd(float dl, float cl) {
	return dl + 0.4*cl;
}
void setSaftyGrade(int sg) {
	if (sg>0&sg<4)
	{
        saftyGrade =sg;

	}

}
