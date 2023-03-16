#include "keyscan.h"

uchar Key()
{	uchar keyval;
	if (key1==0)
	{	
		if(key1==0)
			keyval=1;
		while(!key1);	
	}
	else if (key2==0)
	{	
		if(key2==0)
			keyval=2;
		while(!key2);	
	}
	else if (key3==0)
	{	
		if(key3==0)
			keyval=3;
		while(!key3);	
	}
	else if (key4==0)
	{	
		if(key4==0)
			keyval=4;
		while(!key4);	
	}
	else
	keyval=0;
	return keyval;
}