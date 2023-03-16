#include "mp3.h"
void Delay1us ( uint T )//延时函数
{
    while ( T-- );
}
void Send ( uchar addr )//发送函数。
{
    uchar i;

    EA = 0;/*发送时关掉中断，防止中断影响时序  */

    sda = 1;/*开始拉高  */
    Delay1us ( 1000 );
    sda = 0;/*开始引导码*/
    Delay1us ( 2200 );/*此处延时最少要大于2ms，此参数延时为310ms  */

    for ( i = 0; i < 8; i++ ) /*总共8位数据  */
    {
        sda = 1;
        if ( addr & 0x01 ) /*3:1表示数据位1,每个位用两个脉冲表示  */
        {
            Delay1us ( 500 );
            sda = 0;
            Delay1us ( 210 );
        }
        else              /*1：3表示数据位0 ,每个位用两个脉冲表示  */
        {
            Delay1us ( 210 );
            sda = 0;
            Delay1us ( 500 );
        }
        addr >>= 1;
    }
    sda = 1;
    EA = 1;//恢复中断
}
void playmusic(uchar num)
{	Send(0x0a);
	Send(num);
	Send(0x0b);
}
 