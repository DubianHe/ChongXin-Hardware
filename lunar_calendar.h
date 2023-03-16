

#ifndef _T_H_
#define _T_H_


#include "DS1302.h"
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

void turn_lunar_calendar(TIME *t, uchar *lunar);
uchar jieqixuhao(TIME *t);

#endif

