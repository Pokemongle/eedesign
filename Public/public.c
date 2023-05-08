#include<reg52.h>
#include"public.h"
unsigned char T0RH;
unsigned char T0RL;
void ConfigTimer0(unsigned int ms)//定时器0
{
 	unsigned long tmp;
	tmp = 11059200/12;
	tmp = (tmp * ms)/1000;
	tmp = 65536-tmp;
	T0RH = (unsigned char)(tmp>>8);
	T0RL = (unsigned char)tmp;
	TMOD = TMOD & 0xF0;
	TMOD = TMOD | 0x01;
	TH0 = T0RH;
	TL0 = T0RL;
	ET0 = 1;
	TR0 = 1;
}
void ResetTimer0()//定时器0重载
{
 	TH0 = T0RH;
	TL0 = T0RL;
}
void ConfigTimer1()//定时器1
{
	TMOD = TMOD & 0x0F;
	TMOD = TMOD | 0x10;
	TH1 = 0xFB;
	TL1 = 0xAE;
	TF1 = 0;
	TR1 = 1;
	ET1 = 1;
	EA = 1;
}
void ResetTimer1()//定时器1重载
{
 	TH1 = 0xFB;
	TL1 = 0xAE;
}
void Delay(unsigned int i){
	while(i--); 	
}