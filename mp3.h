#ifndef _MP3_H_
#define _MP3_H_

#include<reg52.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif
sbit sda = P1^0;
void Send( uchar addr );
void playmusic(uchar num);
#endif 