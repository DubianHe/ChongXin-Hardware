#ifndef __DS1302_H_
#define __DS1302_H_

//---����ͷ�ļ�---//
#include<reg52.h>
#include<intrins.h>
#include<word.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

// ʱ�䣺���ʱ��������
sbit DSIO=P2^1;
sbit RST=P2^2;
sbit SCLK=P2^0;
// ʱ�䣺���ʱ��������
typedef struct time
{	char sec;
	char min;
	char hour;
	char day;
	char mon;
	char week;
	char year;
	uchar date[11];
	uchar times[9];
}TIME;
typedef struct alarm
{	char min;
	char hour;
}ALARM;
//---����ȫ�ֺ���---//
void read_time(uchar *timedata);
void Ds1302Init();
void set_time(uchar *timedata);
extern uchar initial_time[7];
void turninto(TIME *timedata,uchar flags);

#endif