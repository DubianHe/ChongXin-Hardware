#include "ds1302.h"
#include <main.h>
//---DS1302写入和读取时分秒的地址命令---//
//---秒分时日月周年 最低位读写位;-------//
uchar code READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d};

uchar code WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};

uchar initial_time[7]={0x00, 0x09, 0x10,0x20,0x05,0x04,0x22};

/*******************************************************************************
* 函 数 名         : Ds1302Write
* 函数功能		   : 向DS1302命令（地址+数据）
* 输    入         : addr,dat
* 输    出         : 无
*******************************************************************************/

void Ds1302Write(uchar addr, uchar dat) {
	uchar n;
	RST = 0;
	_nop_();

	SCLK = 0;//先将SCLK置低电平。
	_nop_();
	RST = 1; //然后将RST(CE)置高电平。
	_nop_();

	for (n = 0; n < 8; n++) { //开始传送八位地址命令
		DSIO = addr & 0x01;//数据从低位开始传送
		addr >>= 1;
		SCLK = 1;//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;
		_nop_();
	}
	for (n = 0; n < 8; n++) { //写入8位数据
		DSIO = dat & 0x01;
		dat >>= 1;
		SCLK = 1;//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;
		_nop_();
	}

	RST = 0;//传送数据结束
	_nop_();
}

/*******************************************************************************
* 函 数 名         : Ds1302Read
* 函数功能		   : 读取一个地址的数据
* 输    入         : addr
* 输    出         : dat
*******************************************************************************/
uchar Ds1302Read(uchar addr) {
	uchar n, dat, dat1;
	RST = 0;
	_nop_();

	SCLK = 0;//先将SCLK置低电平。
	_nop_();
	RST = 1;//然后将RST(CE)置高电平。
	_nop_();

	for (n = 0; n < 8; n++) { //开始传送八位地址命令
		DSIO = addr & 0x01;//数据从低位开始传送
		addr >>= 1;
		SCLK = 1;//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;//DS1302下降沿时，放置数据
		_nop_();
	}
	_nop_();
	for (n = 0; n < 8; n++) { //读取8位数据
		dat1 = DSIO;//从最低位开始接收
		dat = (dat >> 1) | (dat1 << 7);
		SCLK = 1;
		_nop_();
		SCLK = 0;//DS1302下降沿时，放置数据
		_nop_();
	}

	RST = 0;
	_nop_();	//以下为DS1302复位的稳定时间,必须的。
	SCLK = 1;
	_nop_();
	DSIO = 0;
	_nop_();
	DSIO = 1;
	_nop_();
	return dat;
}
void Ds1302Init()
{
	uchar n;
	Ds1302Write(0x8E,0X00);		 //禁止写保护，就是关闭写保护功能
	for (n=0; n<7; n++)//写入7个字节的时钟信号：分秒时日月周年
	{
		Ds1302Write(WRITE_RTC_ADDR[n],initial_time[n]);	
	}
	Ds1302Write(0x8E,0x80);		 //打开写保护功能
}



/*******************************************************************************
* 函 数 名         : Ds1302ReadTime
* 函数功能		   : 读取时钟信息
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void read_time(uchar *timedata) {
	uchar i, tmp;
	for (i = 0; i < 7; i++) {
		timedata[i] = Ds1302Read(READ_RTC_ADDR[i]);

	}
	for (i = 0; i < 7; i++) { //循环7次
		tmp=timedata[i] ;
		timedata[i] = (tmp / 16 % 10) * 10;
		timedata[i] += (tmp % 16);

	}
}
void turninto(TIME *timedata,uchar flags){
	timedata->date[0]=2+'0';
	timedata->date[1]='0';
	timedata->date[2]=timedata->year/10+'0';
	timedata->date[3]=timedata->year%10+'0';
	timedata->date[4]='-';
	timedata->date[5]=timedata->mon/10+'0';
	timedata->date[6]=timedata->mon%10+'0';
	timedata->date[7]='-';
	timedata->date[8]=timedata->day/10+'0';
	timedata->date[9]=timedata->day%10+'0';
	timedata->date[10]='\0';
	timedata->times[0]=timedata->hour/10+'0';
	timedata->times[1]=timedata->hour%10+'0';
	timedata->times[2]=':';
	timedata->times[3]=timedata->min/10+'0';
	timedata->times[4]=timedata->min%10+'0';
	timedata->times[5]=':';
	timedata->times[6]=timedata->sec/10+'0';
	timedata->times[7]=timedata->sec%10+'0';
	timedata->times[8]='\0';
	if(flags==1)
	{	if(timedata->hour>12)
		{	timedata->times[0]=((timedata->hour-12)/10)+'0';
			timedata->times[1]=(timedata->hour-12)%10+'0';}	
	}
}
void set_time(uchar *timedata){
	uchar i, tmp,tmps[7];
	for (i = 0; i < 7; i++) { //循环7次
		tmp=timedata[i] ;
		tmps[i] = (tmp / 10 % 16) * 16;
		tmps[i] += (tmp % 10);

	}
	Ds1302Write(0x8E,0X00);
		for (i=0; i<7; i++)//写入7个字节的时钟信号：分秒时日月周年
	{
		Ds1302Write(WRITE_RTC_ADDR[i],tmps[i]);	
	}
	Ds1302Write(0x8E,0X80);
}