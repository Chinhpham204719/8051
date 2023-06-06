#include <mcs51/8052.h>

#define k3 P3_2 
#define led P1_0

typedef unsigned int u16;
typedef unsigned char u8;

void delay(u16 i)
{
	while(i--);
}
void Init()
{
	IT0=1;
	EX0=1;
	EA=1; 
}

void main()
{
	Init(); 
	while(1);
}

void ISR0() __interrupt (0) 
{
	EA = 0;
	delay(1000); 
	if(k3==0)
	{
		led = !led;
	}
	EA = 1;
}