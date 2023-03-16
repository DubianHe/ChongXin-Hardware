#include "yanwubaojing.h"
void delayyanwu()
{	uchar m,n,s;
	for(m=20;m>0;m--)
	for(n=20;n>0;n--)
	for(s=248;s>0;s--);
}
uchar yanwujiance()
{	uchar i=0;
	if(yanwu==0)
	{	delayyanwu();
		if(yanwu==0)
		{	i=1;
		}
	}
	return i;

}