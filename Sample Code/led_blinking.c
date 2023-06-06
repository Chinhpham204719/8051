#include <mcs51/8052.h>
#define led P1_0

typedef unsigned int u16;
typedef unsigned char u8;

void delay(u16 i)
{
	while(i--);	
}

void main()
{
	while(1)
	{
		led=0;
		delay(50000); //~450ms
		led=1;
		delay(50000); //~450ms	
	}		
}
