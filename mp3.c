#include "mp3.h"
void Delay1us ( uint T )//��ʱ����
{
    while ( T-- );
}
void Send ( uchar addr )//���ͺ�����
{
    uchar i;

    EA = 0;/*����ʱ�ص��жϣ���ֹ�ж�Ӱ��ʱ��  */

    sda = 1;/*��ʼ����  */
    Delay1us ( 1000 );
    sda = 0;/*��ʼ������*/
    Delay1us ( 2200 );/*�˴���ʱ����Ҫ����2ms���˲�����ʱΪ310ms  */

    for ( i = 0; i < 8; i++ ) /*�ܹ�8λ����  */
    {
        sda = 1;
        if ( addr & 0x01 ) /*3:1��ʾ����λ1,ÿ��λ�����������ʾ  */
        {
            Delay1us ( 500 );
            sda = 0;
            Delay1us ( 210 );
        }
        else              /*1��3��ʾ����λ0 ,ÿ��λ�����������ʾ  */
        {
            Delay1us ( 210 );
            sda = 0;
            Delay1us ( 500 );
        }
        addr >>= 1;
    }
    sda = 1;
    EA = 1;//�ָ��ж�
}
void playmusic(uchar num)
{	Send(0x0a);
	Send(num);
	Send(0x0b);
}
 