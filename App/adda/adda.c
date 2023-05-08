#include<STC12C5A.h>
#include"adda.h"

unsigned char xdata ad_memo[MEMO_LEN]; 	// A/D采集信号存储区
unsigned char
	ad_result = 128, // A/D单次采集信号结果 
	memo_p = 0,	   	 // A/D采集存储区指针
void ad_save() // A/D采样值存储
{
    ad_memo[memo_p++] = ad_result;
    if (memo_p >= MEMO_LEN)
    {
        memo_p = 0;
    }
}

