#include <mcs51/8052.h>
#define GPIO_LED P1
#defind LED7SEG_Data P2

unsigned char Time;
unsigned char LED_Data;

void Init()
{
    TMOD = 0x01; 
    TH0 = 0x4C;	 //~50 ms
    TL0 = 0x00; 
    EA = 1;			 
    ET0 = 1;		 
    TR0 = 1;		 
}

void main(void)
{
	LED7SEG_Data = 0xFF;					//turn off the 7-seg LEDs
	Time = 0;
	LED_Data = 0xFE;
	Init();
	while(1)
	{
		if (Time == 10)
		{
			LED_Data = ((LED_Data >> 7) | (LED_Data << 1));
			GPIO_LED = LED_Data;
			Time = 0;
		}
	}				
}

void Timer0() __interrupt (1)
{
	  TH0 = 0x4C;	 //reload for ~50 ms interval
	  TL0 = 0x00;
	  Time++;
}