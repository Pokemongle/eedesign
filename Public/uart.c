#include<reg52.h>
#include"uart.h"

void ConfigUART(unsigned int baud)
{
 	SCON = 0x50;
	TMOD &= 0x0F;
	TMOD |= 0x20;
	TH1 = 256-(11059200/12/32)/baud;
	TL1 = TH1;
	ET1 = 0;
	ES = 1;
	TR1 = 1;
}
