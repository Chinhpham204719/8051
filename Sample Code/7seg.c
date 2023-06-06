#include <mcs51/8052.h>

#define Q4 P2_0
#define Q3 P2_1
#define Q2 P2_2
#define Q1 P2_3
#define Beep P3_6

typedef unsigned int u16;	  
typedef unsigned char u8;
u8 i;
u8 __code LED_Mask[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};


void delay(u16 i)
{
	while(i--);	
}


void Display()
{
	for(i=0;i<4;i++)
	{
		switch(i)	 
		{
		   case 0:
				Q4=0;
				Q3=1;
				Q2=1;
				Q1=1;
				break;
			case 1:
				Q4=1;
				Q3=0;
				Q2=1;
				Q1=1;
				break;
			case 2:
				Q4=1;
				Q3=1;
				Q2=0;
				Q1=1;
				break;
			case 3:	
				Q4=1;
				Q3=1;
				Q2=1;
				Q1=0;
				break;	
		}
		P1=LED_Mask[i];
		delay(100); 	
		P1=0xFF;
	}
}


void main()
{	
	int i = 0;
	for (i = 0; i< 500;i++)
	{
		Beep = !Beep;
		delay(80);
	}
	Beep = 0;
	delay(3000);
	for (i = 0; i< 500;i++)
	{
		Beep = !Beep;
		delay(80);
	}
	Beep = 0;
	delay(3000);
	for (i = 0; i< 500;i++)
	{
		Beep = !Beep;
		delay(80);
	}
	Beep = 0;
	while(1)
	{	
		Display();  
	}		
}