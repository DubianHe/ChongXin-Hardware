#ifndef _DTH11_H_
#define _DTH11_H_

#include<reg52.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif
sbit Data = P2^3;
typedef struct sensor
{	uchar wendu;
	uchar shidu;
}Sensor;
void getdata(uchar *sensordata);
#endif 