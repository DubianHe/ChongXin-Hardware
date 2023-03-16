#include "main.h"
TIME time,tmp_time;
ALARM alarm={10,10};//闹钟时间
uchar mod=0;
uchar lunar[2];
uchar clock=1;
uchar music=1;
Sensor sensor;
uchar ampm=1;
uchar breakflag=1;
uchar UART_buff;
uchar tianqi=1;
uchar huozai=0;
uchar weatherdata[]="Cloudy  ";
uchar count=0;
uchar mynum=0;

/*
模式0是万年历的主界面 其中包括静态框架和变量功能
*/
void sendData(uchar *datas);
void mod0kuangjia()
{	
	LCD12864_Writechar("温",2,0);
	LCD12864_Writechar("湿",2,4);
	LCD12864_Writechar("度",2,2);
	LCD12864_Writechar("%",2,6);
	LCD12864_Writestring(weather,tianqi*2,2,7);	
}
void mod0time()
{	uchar keyval;
	keyval=Key();
	huozai=yanwujiance();
	if(huozai==1)
	{	LCD12864_WriteCmd(0x01);
		mod=4;
		
	}
	if(ampm==1){
	if (time.hour>12)
	LCD12864_Writechar("PM",1,4);
	else
	LCD12864_Writechar("AM",1,4);}
	else
	LCD12864_Writechar("  ",1,4);
	if(clock==1)
		{LCD12864_Writechar("闹钟",0,6);
		if(time.min==alarm.min&&time.hour==alarm.hour)
		{mod=4;
		LCD12864_WriteCmd(0x01);}}
	else
		LCD12864_Writechar("    ",0,6);
	
	read_time((uchar *)&time);
	turninto(&time,ampm);
	LCD12864_Writechar(time.date,0,0);
	LCD12864_Writechar(time.times,1,0);
	turn_lunar_calendar(&time, lunar);
	LCD12864_LUNAR(lunar[0],lunar[1]);
	LCD12864_Writestring(nongliyear,(time.year%10)*2,3,0);
	LCD12864_Writestring(nongliyear,((2000+time.year)%12)*2+20,3,1);
	LCD12864_Writestring(jieqis,jieqixuhao(&time)*4,3,6);
	LCD12864_Writestring(jieqis,jieqixuhao(&time)*4+2,3,7);
	LCD12864_TIME(sensor.shidu,2,1);
	LCD12864_TIME(sensor.wendu,2,5);


	if(mod!=0)
	{
	LCD12864_WriteCmd(0x01);
	}
	if(keyval==1)
	{LCD12864_WriteCmd(0x01);	
	mod=5;

	}
	}
void mod0()
{	mod0kuangjia();
	mod0time();
	}

/*
模式1是闹钟界面
*/
void mod1kuangjia()
{	uchar key_val;
	uchar states=1;
	LCD12864_Writechar("闹钟",0,3);
	LCD12864_TIME(alarm.hour,1,2);
	LCD12864_TIME(alarm.min,1,5);
	LCD12864_Writechar(" :: ",1,3);
	LCD12864_Writechar("音乐",2,3);
	while(1)
	{	key_val=Key();
		LCD12864_TIME(alarm.hour,1,2);
		LCD12864_TIME(alarm.min,1,5);
		if(clock==0)
			LCD12864_Writechar("关",0,7);
		else if(clock==1)
			LCD12864_Writechar("开",0,7);
		if(music==1)
		LCD12864_Writechar("爱你",3,3);
		else if(music==2)
		LCD12864_Writechar("她说",3,3);
		switch(states)
		{	case 1:{LCD12864_Writestring(shijian,2,1,7);LCD12864_Writechar("  ",2,5);break;}
			case 2:LCD12864_Writestring(shijian,4,1,7);break;
			case 3:LCD12864_Writechar("->",0,5);break;
			case 4:{LCD12864_Writechar("<-",2,5);LCD12864_Writechar("  ",0,5);break;}
		}
		if(key_val==2)
		{	if (states>=4)
				states=1;
			else states++;
		}
		else if(key_val==3||key_val==4)
		{	if(states==1)
			{	if(key_val==3)
				alarm.min++;
				else
				alarm.min--;
				if(alarm.min<0)
					alarm.min=59;
				else if(alarm.min>59)
					alarm.min=0;
				LCD12864_TIME(alarm.min,1,5);
			}
			else if(states==2)
			{	if(key_val==3)
				alarm.hour++;
				else
				alarm.hour--;
				if(alarm.hour<0)
					time.hour=23;
				else if(alarm.hour>23)
					alarm.hour=0;
				
				LCD12864_TIME(alarm.hour,1,2);
			}
			else if(states==3)
			{	if(clock==0)
					clock=1;
				else
				clock=0;
				if(clock==0)
					LCD12864_Writechar("关",0,7);
				else if(clock==1)
					LCD12864_Writechar("开",0,7);}
			else if(states==4)
			{	if (music==1)
					music=2;
				else
					music=1;
				}
			
		}
		else if(key_val==1)
		{	mod=5;
			LCD12864_WriteCmd(0x01);
			break;
		}
		if(mod!=1)
		{	LCD12864_WriteCmd(0x01);
			break;	
		}
	}
}
void mod1()
{	
	mod1kuangjia();
}
/*
模式2是时间设置界面
*/
void mod2()
{	uchar states=1;
	uchar keyval;
	LCD12864_Writechar(time.date,1,1);
	LCD12864_Writechar(time.times,2,2);
	LCD12864_Writechar("时间设置",0,2);
	while(1)
	{	keyval=Key();
		if(ampm==1){
			if (time.hour>12)
			LCD12864_Writechar("PM",2,6);
		else
		LCD12864_Writechar("AM",2,6);}
		else
	LCD12864_Writechar("  ",2,6);
		LCD12864_Writechar("->",0,6);
		LCD12864_Writestring(shijian,(states-1)*2,0,7);
		if(keyval==2)
		{	if (states>=7)
				states=1;
			else states++;
		}
		else if(keyval==3 || keyval==4)
		{	if(states==1)
			{	if(keyval==3)
				time.sec++;
				else if (keyval==4)
				time.sec--;
				if(time.sec<0)
					time.sec=59;
				else if(time.sec>59)
					time.sec=0;
				turninto(&time,ampm);
				LCD12864_Writechar(time.times,2,2);
			}
			if(states==2)
			{	if(keyval==3)
				time.min++;
				else if (keyval==4)
				time.min--;
				if(time.min<0)
					time.min=59;
				else if(time.min>59)
					time.min=0;
				turninto(&time,ampm);
				LCD12864_Writechar(time.times,2,2);
			}
			if(states==3)
			{	if(keyval==3)
				time.hour++;
				else if (keyval==4)
				time.hour--;
				if(time.hour<0)
					time.hour=23;
				else if(time.hour>23)
					time.hour=0;
				turninto(&time,ampm);
				LCD12864_Writechar(time.times,2,2);
			}
			if(states==4)
			{	if(keyval==3)
				time.day++;
				else if (keyval==4)
				time.day--;
				if(time.day<1)
					time.day=31;
				else if(time.day>31)
					time.day=1;
				turninto(&time,ampm);
				LCD12864_Writechar(time.date,1,1);
			}
			if(states==5)
			{	if(keyval==3)
				time.mon++;
				else if (keyval==4)
				time.mon--;
				if(time.mon<1)
					time.mon=12;
				else if(time.mon>12)
					time.mon=1;
				turninto(&time,ampm);
				LCD12864_Writechar(time.date,1,1);
			}
			if(states==6)
			{	if(keyval==3)
				time.year++;
				else if (keyval==4)
				time.year--;
				turninto(&time,ampm);
				LCD12864_Writechar(time.date,1,1);
			}
			if(states==7)
			{	if(ampm==0)
					ampm=1;
				else
					ampm=0;
			}
		}
		else if(keyval==1)
		{	mod=5;
			set_time((uchar *)&time);
			LCD12864_WriteCmd(0x01);
			break;
		}
		if(mod!=2)
		{
			LCD12864_WriteCmd(0x01);
			break;	
		}
	}	
}
void mod4()
{	uchar keyval;
	Send(0x0a);
	Send(0x08);
	Send(0x00);
	Send(0x0c);
	Send(0x0a);
	Send(music);
	Send(0x0b);
	//led=0;
	if(huozai==1){
	sendData("F");
	huozai=0;}
	LCD12864_WriteCmd(0x01);
	sendData("FF");
	LCD12864_Writechar("火灾报警中",1,0);
	LCD12864_Writechar("按下任意键可取消",2,0);
	while(1)
	{	
		if(keyval!=0)
		{	mod=0;
			huozai=0;
			Send(0x13);
			LCD12864_WriteCmd(0x01);
			break;
			}
		if(mod==0)
		{	clock=0;
			huozai=0;
			LCD12864_WriteCmd(0x01);
			Send(0x13);
			break;
		}
	}
}
void mod6()
{	uchar keyval;
	Send(0x0a);
	Send(0x08);
	Send(0x00);
	Send(0x0c);
	Send(0x0a);
	Send(2);
	Send(0x0b);
	//led=0;
	LCD12864_WriteCmd(0x01);
	LCD12864_Writechar("时间到了",0,2);
	LCD12864_Writechar("闹钟闹钟",1,2);
	LCD12864_Writechar("按任意键取消",2,1);
	while(1)
	{	keyval=0;
		if(keyval!=0)
		{	mod=0;
			clock=0;
			Send(0x13);
			LCD12864_WriteCmd(0x01);
			break;
			}
		if(mod==0)
		{	clock=0;
			huozai=0;
			LCD12864_WriteCmd(0x01);
			Send(0x13);
			break;
		}
	}
}
void mod3()
{	uchar keyval;

	while(1)
	{	keyval=Key();
		LCD12864_Writechar("今日天气",0,2);
		LCD12864_Writechar(weatherdata,1,1);
		LCD12864_Writechar("天气在线获取",2,1);
		if(keyval==1)
		{	mod=5;
			LCD12864_WriteCmd(0x01);
	 		break;	
		}
	 if(mod!=3)
	 {	LCD12864_WriteCmd(0x01);
	 	break;
	 }
	}
}
void modset()
{	uchar state=0;
	uchar keyval=0;
	LCD12864_Writechar("1.主界面",0,0);
	LCD12864_Writechar("2.闹钟设置",1,0);
	LCD12864_Writechar("3.时间设置",2,0);
	LCD12864_Writechar("4.天气获取",3,0);
	while(1)
	{	keyval=Key();
		LCD12864_Writechar("<-",state,6);
		if(keyval==1)
		{	LCD12864_Writechar("  ",state,6);
			state++;
			if(state>=4)
				state=0;
		}
		if(keyval==2)
		{	LCD12864_WriteCmd(0x01);
			mod=state;
			break;
		}	
	}
}
/*蓝牙配置*/
void UsartInit()
{
	SCON=0X50;			//设置为工作方式1
	TMOD=0X20;			//设置计数器工作方式2
	PCON=0X00;			//波特率加倍
	TH1=0XFD;				//计数器初始值设置，注意波特率是4800的
	TL1=0XFD;
	ES=1;						//打开接收中断
	EA=1;						//打开总中断
	TR1=1;					//打开计数器
}
/*蓝牙数据发送*/
void sendData(uchar *datas)
{	uchar i=0;
	while(datas[i]!='\0')
    {	SBUF=datas[i];		  //接收到的数据放入发送缓存器发送
    	while(!TI);       //等待发送数据完成
    	TI=0;
		i++;}			  //清除发送完成标志位
}

void ser_int (void) interrupt 4   
{	 
    if(RI == 1) {  //如果收到.  
      RI = 0;      //清除标志.  
      UART_buff = SBUF;  //接收.
	  if (UART_buff>=0&&UART_buff<=2)
	 { mod=UART_buff;
	 }
	 else if(UART_buff==3)
	 {	if(ampm==1)
	 		ampm=0;
		else
			ampm=1;
	 }
	 else if(UART_buff>=4&&UART_buff<=15){
	 if(UART_buff==4) 
	 	time.hour++;
	else if(UART_buff==5)
		time.hour--;
	else if(UART_buff==6)
		time.min++;
	else if(UART_buff==7)
		time.min--;
	else if(UART_buff==8)
		time.sec++;
	else if(UART_buff==9)
		time.sec--;
	else if(UART_buff==10)
		time.year++;
	else if(UART_buff==11)
		time.year--;
	else if(UART_buff==12)
		time.mon++;
	else if(UART_buff==13)
		time.mon--;
	else if(UART_buff==14)
		time.day++;
	else if(UART_buff==15)
		time.day--;
	set_time((uchar *)&time);
		}
	else if(UART_buff>=16&&UART_buff<=20)
	{tianqi=UART_buff%16;}
	else if(UART_buff>=21&&UART_buff<=27)
	{	if(UART_buff==21)
			alarm.hour++;
		else if(UART_buff==22)
			alarm.hour--;
		else if(UART_buff==23)
			alarm.min++;
		else if(UART_buff==24)
			alarm.min--;
		else if(UART_buff==25)
		{	if(clock==1)
				clock=0;
			else
				clock=1;	
			}
		else if(UART_buff==26)
			music=1;
		else if(UART_buff==27)
			music=2;

	}
	else if(UART_buff==28)
	{	Send(0x0a);
		Send(0x02);
		Send(0x00);
		Send(0x0c);
		Send(0x0a);
		Send(1);
		Send(0x0b);	
	}
	else if(UART_buff==29)
	{	count=0;

	}
	else
	{	if (UART_buff!='\0')
		{	weatherdata[count+1]=UART_buff;
			count++;}	
	}	
    }  
  
}

   
main()
{	UsartInit();
	Ds1302Init();
	LCD12864_Init();
	getdata((uchar *)&sensor);
	read_time((uchar *)&time);
	
	while(1)
{		/*if(mod==1)
			{mod1();
			 
			}
		else if(mod==0)
			{mod0();
			}
		else if(mod==2)
			{mod2();}
		else if(mod==3)
			{mod3();
			}
		else if(mod==4)
		mod4();	
		else if(mod==5)
		modset();
		else
		mod6();*/
	LCD12864_Writechar("开",0,7);
}}