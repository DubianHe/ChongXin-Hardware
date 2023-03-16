#ifndef __LCD12864_H
#define __LCD12864_H

//---����ͷ�ļ�---//
#include<reg52.h>
#include<intrins.h>
#include"word.h"
//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//---���ʹ�û�ͼģʽ�������---//

//#define LCD12864_PICTURE

//---����ʹ�õ�IO��---//

#define LCD12864_DATAPORT P0	  //����IO��

sbit LCD12864_RS  =  P2^6;             //����������Ĵ���ѡ������ 
sbit LCD12864_RW  =  P2^5;             //Һ����/д����
sbit LCD12864_EN  =  P2^7;             //Һ��ʹ�ܿ���
sbit LCD12864_PSB =  P2^4;             //��/����ʽ����


uchar code nong[];
//---����ȫ�ֺ���---//
void LCD12864_WriteCmd(uchar cmd);
void LCD12864_WriteData(uchar dat);
void LCD12864_Init();
void LCD12864_SetWindow(uchar x, uchar y);
void LCD12864_Writechar(uchar *s,uchar x,uchar y);
void LCD12864_TIME(uchar t,uchar x,uchar y);
void LCD12864_LUNAR(uchar mon,uchar day);
void LCD12864_Writestring(uchar c[],uchar num,uchar x,uchar y);
#endif
