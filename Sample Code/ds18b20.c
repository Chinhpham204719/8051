#include <mcs51/8052.h>
#define Beep	P3_6	//Buzzer
#define DQ		P3_7	//Data pin of the DS18B20	 
unsigned char 	temp; 	//Temp variable
//Delay func (not so accurate)
void delay_ms(unsigned int t)
{
	unsigned int i,j;
	for(i = 0; i < t; i++)
		for(j = 0; j < 125; j++);
}
//Delay func for the DS18B20 (not so accurate)
void delay_us_DS18B20(unsigned char t)
{
    while(t--);
}
//Init for the DS18B20 -> See Init Timing in the datasheet
void Init_DS18B20()
{
    DQ = 1;   
    delay_us_DS18B20(10);
    DQ = 0;					//Reset pulse ...   
    delay_us_DS18B20(150); 	//for a minimum period. See the datasheet.  
    DQ = 1;					//Rising edge 
    delay_us_DS18B20(20);   //Wait for the DS18B20 to response
}
//Read a byte from the Scratchpad
unsigned char ReadByteFromScratchpad()
{
    unsigned char i = 0;
    unsigned char byte = 0;
    for (i = 8; i > 0; i--){
          DQ = 0;
          byte >>= 1;
          DQ = 1;			//The master bus releases the 1-Wire bus
          if(DQ)			//Read 1? Otherwise, Read 0
          	byte |= 0x80;
          delay_us_DS18B20(20);
    }
    return(byte);
}
//Write a byte to the Scratchpad
void WriteByteToScratchpad(unsigned char byte)
{
    unsigned char i = 0;
    for (i = 8; i > 0; i--){
        DQ = 0;
        DQ = byte&0x01;
		if(DQ)						//Write 1
        	delay_us_DS18B20(3);
		else						//Write 0
			delay_us_DS18B20(20);	
        DQ = 1;	  					//The master bus releases the 1-Wire bus
        byte >>= 1;
    }
}
//Read the DS18B20
void ReadTemperature()
{	
    unsigned char Byte0 = 0;		//Byte0 of the Scratchpad
    unsigned char Byte1 = 0;		//Byte1 of the Scratchpad

    Init_DS18B20();				   	//DS18B20 initialization
    WriteByteToScratchpad(0xCC);	//The master issues Skip ROM [CCh] command
    WriteByteToScratchpad(0x44);	//Convert T [44h] command. To initiate a temp measurement and A-to-D conversion. 
    delay_us_DS18B20(10); 

    Init_DS18B20();					//DS18B20 initialization 
    WriteByteToScratchpad(0xCC);	//The master issues Skip ROM [CCh] command
    WriteByteToScratchpad(0xBE);	//Read temp value. Read Scratpad [BEh] command.
    delay_us_DS18B20(10);
    Byte0=ReadByteFromScratchpad();	//Read Byte0 of the Scratchpad (low byte of the temp value)
    Byte1=ReadByteFromScratchpad();	//Read Byte1 of the Scratchpad (high byte of the temp value)
    
	temp = ((Byte1*256+Byte0)>>4);	//Calculate the temp value from Byte0 & Byte1. Then save it to the temp variable.
}
void main()
{
	Beep = 0;	//Turn off the buzzer
	while(1){
		  ReadTemperature(); 	//Read the DS18B20 ... 
		  P1 = ~temp;			//and the display the temp value (in binary form) on 8 LEDs of P1 (P1_7...P1_0) ...
		  delay_ms(500);		//for every 0.5s.
	}
}






