#include"lcd12864.h"

uchar code nong[]="正一二三四五六七八九十冬腊初廿";
/*******************************************************************************
* 函 数 名         : LCD12864_Delay1ms
* 函数功能		   : 延时1MS
* 输    入         : c
* 输    出         : 无
*******************************************************************************/

void LCD12864_Delay1ms(uint c)
{
    uchar a,b;
	for(; c>0; c--)
	{
	    for(b=199; b>0; b--)
		{
	        for(a=1; a>0; a--);
		}
	}
}

/*******************************************************************************
* 函 数 名         : LCD12864_Busy
* 函数功能		   : 检测LCD是否忙
* 输    入         : 无
* 输    出         : 1或0（1表示不忙，0表示忙）
*******************************************************************************/

uchar LCD12864_Busy(void)
{
	uchar i = 0;

	LCD12864_RS = 0;   	//选择命令
	LCD12864_RW = 1;	//选择读取

	LCD12864_EN = 1;
	LCD12864_Delay1ms(1);

	while((LCD12864_DATAPORT & 0x80) == 0x80)	//检测读取到的值
	{
		i++;
		if(i > 100)
		{
			LCD12864_EN = 0;
			return 0;	   //超过等待时间返回0表示失败
		}
	}

	LCD12864_EN = 0;

	return 1;
}

/*******************************************************************************
* 函 数 名         : LCD12864_WriteCmd
* 函数功能		   : 写命令
* 输    入         : cmd
* 输    出         : 无
*******************************************************************************/

void LCD12864_WriteCmd(uchar cmd)
{
	uchar i;
	i = 0;
	while( LCD12864_Busy() == 0)
	{
		LCD12864_Delay1ms(1);
		i++;
		if( i>100)
		{
			return;	   //超过等待退出
		}	
	}
	
	LCD12864_RS = 0;     //选择命令
	LCD12864_RW = 0;     //选择写入
	LCD12864_EN = 0;     //初始化使能端

	LCD12864_DATAPORT = cmd;   //放置数据

	LCD12864_EN = 1;		   //写时序
	LCD12864_Delay1ms(5);
	LCD12864_EN = 0;    					
}

/*******************************************************************************
* 函 数 名         : LCD12864_WriteData
* 函数功能		   : 写数据
* 输    入         : dat
* 输    出         : 无
*******************************************************************************/

void LCD12864_WriteData(uchar dat)
{
	uchar i;
	i = 0;
	while( LCD12864_Busy() == 0)
	{
		LCD12864_Delay1ms(1);
		i++;
		if( i>100)
		{
			return;	   //超过等待退出
		}	
	}

	LCD12864_RS = 1;     //选择数据
	LCD12864_RW = 0;     //选择写入
	LCD12864_EN = 0;     //初始化使能端

	LCD12864_DATAPORT = dat;   //放置数据

	LCD12864_EN = 1;		   //写时序
	LCD12864_Delay1ms(5);
	LCD12864_EN = 0;    								
}

/*******************************************************************************
* 函 数 名         : LCD12864_ReadData
* 函数功能		   : 读取数据
* 输    入         : 无
* 输    出         : 读取到的8位数据
*******************************************************************************/
#ifdef LCD12864_PICTURE

uchar LCD12864_ReadData(void)
{
	uchar i, readValue;

	i = 0;
	while( LCD12864_Busy() == 0)
	{
		LCD12864_Delay1ms(1);
		i++;
		if( i>100)
		{
			return 0;	   //超过等待退出
		}	
	}

	LCD12864_RS = 1;       //选择命令
	LCD12864_RW = 1;
	LCD12864_EN = 0;
	LCD12864_Delay1ms(1);  //等待

	LCD12864_EN = 1;
	LCD12864_Delay1ms(1);
	readValue = LCD12864_DATAPORT;
	LCD12864_EN = 0;

	return readValue;
}

#endif

/*******************************************************************************
* 函 数 名         : LCD12864_Init
* 函数功能		   : 初始化LCD12864
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void LCD12864_Init()
{
	LCD12864_PSB = 1;	  //选择并行输入


	LCD12864_WriteCmd(0x30);  //选择基本指令操作
	LCD12864_WriteCmd(0x0c);  //显示开，关光标
	LCD12864_WriteCmd(0x01);  //清除LCD12864的显示内容
}

/*******************************************************************************
* 函 数 名         : LCD12864_ClearScreen
* 函数功能		   : 在画图模式下，LCD12864的01H命令不能清屏，所以要自己写一个清
*                  * 屏函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
#ifdef LCD12864_PICTURE

void LCD12864_ClearScreen(void)
{
	uchar i,j;

	LCD12864_WriteCmd(0x34);	  //开启拓展指令集

	for(i=0;i<32;i++)			  //因为LCD有纵坐标32格所以写三十二次
	{
		LCD12864_WriteCmd(0x80+i);		  //先写入纵坐标Y的值
		LCD12864_WriteCmd(0x80);		  //再写入横坐标X的值
		for(j=0;j<32;j++)		  //横坐标有16位，每位写入两个字节的的数据，也
		{						  //就写入32次以为当写入两个字节之后横坐标会自
			LCD12864_WriteData(0xFF);	  //动加1，所以就不用再次写入地址了。
		}
	}

	LCD12864_WriteCmd(0x36);	 //0x36扩展指令里面打开绘图显示
	LCD12864_WriteCmd(0x30);	 //恢复基本指令集
}

#endif
/*******************************************************************************
* 函 数 名         : LCD12864_SetWindow
* 函数功能		   : 设置在基本指令模式下设置显示坐标。注意：x是设置行，y是设置列
* 输    入         : x, y
* 输    出         : 无
*******************************************************************************/

void LCD12864_SetWindow(uchar x, uchar y)
{
	uchar pos;

	if(x == 0)	   // 第一行的地址是80H
	{
		x = 0x80;
	}
		
	else if(x == 1)  //第二行的地址是90H
	{
		x = 0x90;	
	}
	
	else if(x == 2)  //第三行的地址是88H
	{
		x = 0x88;
	}
	
	else if(x == 3)
	{
		x = 0x98;
	}
	
	pos = x + y;
	LCD12864_WriteCmd(pos);
}
void LCD12864_Writechar(uchar s[],uchar x,uchar y) //在固定位置写字符串
{	uchar i=0;
	LCD12864_SetWindow(x,y);
	while(s[i]!='\0')
	{	LCD12864_WriteData(s[i]);
		i++;
	}

}
void LCD12864_Writestring(uchar c[],uchar num,uchar x,uchar y)
{	 LCD12864_SetWindow(x,y);
	 LCD12864_WriteData(c[num]);
	 LCD12864_WriteData(c[num+1]);
}
void LCD12864_TIME(uchar t,uchar x,uchar y)
{	  
	LCD12864_SetWindow(x,y);
	LCD12864_WriteData(t/10+'0');
   	LCD12864_WriteData(t%10+'0');
}
void LCD12864_LUNAR(uchar mon,uchar day)
{	if(mon==1)
	LCD12864_Writestring(nong,0,3,2);
	else
	LCD12864_Writestring(nong,2*mon,3,2);
	LCD12864_Writechar("月",3,3);
	if(day<=10)
	LCD12864_Writestring(nong,26,3,4);
	else if((day>10)&&(day<=19))
	LCD12864_Writestring(nong,20,3,4);
	else
	LCD12864_Writestring(nong,28,3,4);
	if(day%10!=0)
	LCD12864_Writestring(nong,2*(day%10),3,5);
	else
	LCD12864_Writestring(nong,20,3,5);
}