#ifndef __DS1302_H_
#define __DS1302_H_

//---包含头文件---//
#include<reg52.h>
#include<intrins.h>
#include<word.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

// 时间：秒分时日月周年
sbit DSIO=P2^1;
sbit RST=P2^2;
sbit SCLK=P2^0;
// 时间：秒分时日月周年
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
//---定义全局函数---//
void read_time(uchar *timedata);
void Ds1302Init();
void set_time(uchar *timedata);
extern uchar initial_time[7];
void turninto(TIME *timedata,uchar flags);

#endif