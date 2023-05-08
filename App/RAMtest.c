#include<STC12C5A.h>
#include<intrins.h>
#include<absacc.h>
#include"myled.h"
#include"public.h"

unsigned int cnt_clk = 0;
// external RAM: 0000-1FFFF
// DAC: 2000-2FFF
#define ORIGIN 0x0000
#define LEN 16
unsigned char i = 0;
unsigned char dat = 0;

void main(){
	ld_ad3=0;
	ld_ad2=0;
	ld_ad1=0;
	ConfigTimer0(1);
	dsptask();
	EA = 1;
	while(1){
		if(cnt_clk == 1000){
			cnt_clk = 0;
			i++;
			dat++;
			if(i == LEN){
			 	i = 0;
			}
			if(dat == 0x10){
				dat = 0x00;
			}
			XBYTE[ORIGIN+(unsigned int)i] = dat;
			dspbuf[0]=myLedChar[XBYTE[ORIGIN+(unsigned int)i]];
		}
	}
	 	
}
void InterruptTimer0() interrupt 1
{
 	ResetTimer0();
	cnt_clk++;
	dsptask();
}

