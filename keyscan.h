#ifndef __KEYSCAN_H_
#define __KEYSCAN_H_


#include<reg52.h>
#include<intrins.h>
#include "myutils.h"

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

sbit key1=P1^2;
sbit key2=P1^3;
sbit key3=P1^4;
sbit key4=P1^5;


uchar Key();

#endif