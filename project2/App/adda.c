//==ͷ�ļ�==
#include <absacc.h>
#include <intrins.h>
#include <math.h>
#include <STC12C5A.h> // 51��׼ͷ�ļ�
#include "adda.h"
#include "key.h"
#include "led.h"

//==========
//==ȫ�ֱ���==
unsigned char
	ad_get_count = 0,
	ad_save_count = 0,
	ad_display_count = 0,
	ad_result = 128, // A/D���βɼ��źŽ��
	ad_result_display = 128,
	da_result = 0; 	 // �źŷ��������β����������ź�
unsigned int 
	review_p = 0,  // D/A�ط���ʾָ��
	memo_p = 0;	   // A/D�ɼ��洢��ָ��
unsigned char xdata
	ad_memo[MEMO_LEN]; 	// A/D�ɼ��źŴ洢��
//============
//==����==
unsigned char ad_get()// A/D���������Σ�
{
    ADC_CONTR = ADC_CONTR | ADC_POWER | 4 | ADC_START;
    _nop_(); // wait
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG))
        ;                   // �ȴ�A/D��־λ��1��ADC_FLAG
    ADC_CONTR &= ~ADC_FLAG; // ��־λ����
    return ADC_RES;
}
void ad_save() // A/D����ֵ�洢
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
		 	case 1:{	 // ģʽ1 CH1ʵʱ��ʾ
				DA_CH1 = ad_result_display;// ʵʱ��ʾ
				DA_CH2 = 0;
			};break;
			case 2:{ 	 // ģʽ2 CH1ʵʱ��ʾ CH2�ط���ʾ
				DA_CH1 = ad_result_display;// ʵʱ��ʾ
				DA_CH2 = ad_memo[review_p]; // �ط���ʾ
				ad_display_count++;
				if(ad_display_count >= 20){
						ad_display_count = 0;
						review_p++;
				}
				if (review_p >= MEMO_LEN){ // ����ʾָ�뵽��洢��ĩβ����ͷ��ʼ��ʾ
	     			review_p = 0;
				} 			
			};break;
			default: ;break;
		}
	
}
//========