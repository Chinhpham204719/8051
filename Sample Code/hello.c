#include<reg51.h>
void main()
{
    int i;
    while (1)
    {
	P2 = 0x00;
	for (i=0; i<30000; i++);
	P2 = 0xFF;
	for (i=0; i<30000; i++);
    }
}