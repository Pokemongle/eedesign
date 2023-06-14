//==头文件==
#include <absacc.h>
#include <intrins.h>
#include <math.h>
#include <STC12C5A.h> // 51标准头文件
#include "adda.h"
#include "key.h"
#include "led.h"

//==========
//==全局变量==
unsigned char
	ad_get_count = 0,
	ad_save_count = 0,
	ad_display_count = 0,
	ad_result = 128, // A/D单次采集信号结果
	ad_result_display = 128,
	da_result = 0; 	 // 信号发生器单次产生的数字信号
unsigned int 
	review_p = 0,  // D/A回放显示指针
	memo_p = 0;	   // A/D采集存储区指针
unsigned char xdata
	ad_memo[MEMO_LEN]; 	// A/D采集信号存储区
//============
//==函数==
unsigned char ad_get()// A/D采样（单次）
{
    ADC_CONTR = ADC_CONTR | ADC_POWER | 4 | ADC_START;
    _nop_(); // wait
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG))
        ;                   // 等待A/D标志位变1，ADC_FLAG
    ADC_CONTR &= ~ADC_FLAG; // 标志位清零
    return ADC_RES;
}
void ad_save() // A/D采样值存储
{
    ad_memo[memo_p++] = ad_result_display;
/*
	if(memo_p<=2){
	 	XBYTE[memo_p++]=0;
	}
	else if(memo_p<=100){
	 	XBYTE[memo_p++]=32;
	}
	else if(memo_p<=200){
	 	ad_memo[memo_p++]=64;
	}
	else if(memo_p<=300){
	 	ad_memo[memo_p++]=96;
	}
	else if(memo_p<=400){
	 	ad_memo[memo_p++]=128;
	}
	else if(memo_p<=500){
	 	ad_memo[memo_p++]=160;
	}
	else if(memo_p<=600){
	 	ad_memo[memo_p++]=192;
	}
	else if(memo_p<=700){
	 	ad_memo[memo_p++]=255;
	}	  */
	//ad_memo[memo_p++] = 0;
    if (memo_p >= MEMO_LEN)
    {
        memo_p = 0;
    }
}
void da_display()
{							
		switch(mode){
		 	case 1:{	 // 模式1 CH1实时显示
				DA_CH1 = ad_result_display;// 实时显示
				DA_CH2 = 0;
			};break;
			case 2:{ 	 // 模式2 CH1实时显示 CH2回放显示
				DA_CH1 = ad_result_display;// 实时显示
				DA_CH2 = ad_memo[review_p]; // 回放显示
				ad_display_count++;
				if(ad_display_count >= 20){
						ad_display_count = 0;
						review_p++;
				}
				if (review_p >= MEMO_LEN){ // 若显示指针到达存储区末尾，从头开始显示
	     			review_p = 0;
				} 			
			};break;
			default: ;break;
		}
	
}
//========