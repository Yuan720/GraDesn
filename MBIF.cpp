#include<MBIF.h>
float getArraySum(float *p, int size) {
	float resu = 0;
	for (size_t i = 0; i < size; i++)
	{
		resu += p[i];


	}
	return resu;
}

