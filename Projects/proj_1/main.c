/*
 * @ packaged_edition
 * - package edition ������
 * - ��ģ���װ�� Projects, App, Obj, Public �ļ����б���ά��
 * - ../../demos�ļ���Ϊ master git �⣬���ڰ汾����
 *
 */

//==ͷ�ļ�==
#include <STC12C5A.h> // 51��׼ͷ�ļ�
#include <absacc.h>	  // ʱ�ӷ�Ƶ��
#include <intrins.h>  // ���ڵ���ϵͳ��ʱ���� _nop_()
#include <math.h>	  // ���ڵ�����ѧ���� sin, abs��
#include "adda.h"
#include "key.h"
#include "led.h"
//==========
//==��������==
void init_ad();//A/D��ʼ��
void init_special_interrupts(); // �жϳ�ʼ��
void init_timer0(); // ��ʱ��0��ʼ��

//============

//==������==
void main()
{																													                                                            
    init_timer0();
    init_special_interrupts();
    init_ad();
    while (1)
    {
        
		if(key_sta & 0x01){//key_sta.0 = 0x01? key_sta.0 = 1��⵽��������
			mode_select();
			key_sta = key_sta & 0xfe;//key_sta.0 = 0,�������𣬵ȴ���һ�μ�⵽����
		}
    }
}
void interrupt_timer0() interrupt 1 // ��ʱ��0�жϴ���
{
	EA = 0;
	ad_result = ad_get();// ���β��� 		

	if(mode == 1){		// CH1��ʵʱ�������洢 CH2: �źŷ�����
		// �źŷ���
		da_count++;
		if(da_count >= gen_count){ // gen_count = 4ʱ��ÿ��1ms���һ��D/A
			da_count = 0;
			da_generator();
			da_process();	// ��ʾֵԤ����
		}
		// ģʽ1��ʵʱ����ֵ���д洢
		ad_result = ((int)(ad_result / 2.0 + 64));
	 	ad_save();
	}
	else if(mode == 2){	// CH1: ʵʱ���� CH2: ���λط���ʾ
	 	ad_result = ((int)(ad_result / 2.0 + 64));	
	}
	else if(mode == 3){ // CH1: ʵʱ���� CH2: ���0V	
		//time_count++;
		ad_measure();
		dspbuf_measure();//�������ʾ����ı�
		ad_result = ((int)(ad_result / 2.0 + 64));
	}
	else if(mode == 4){	// CH1: ʵʱ���� CH2: �źŷ���������Ƶ����
		// �źŷ�����
		da_count++;
		if(da_count >= gen_count){ // gen_count = 4ʱ��ÿ��1ms���һ��D/A
			da_count = 0;
			da_generator();
			da_process();	// ��ʾֵԤ����
		}	
	}
	dsptask();		// �����ɨ��
	key_service(); 	// ����ɨ��
    da_display();	// D/A��ʾ
	EA = 1;
}
void init_ad() // ad�Ĵ�����ʼ��
{
    P1ASF = 0x08;     // P1.3��Ϊad����
    ADC_CONTR = 0xE3; // ADC���ƼĴ���         Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                      // λ����            ADC_POWER  SPEED1 SPEED0 ADC_FLAG ADC_START CHS2    CHS1    CHS0
                      // ��ʼֵ=0000,0000      0       0       0       0       0       0       0       0
                      // ����ֵ=0000,0000      1       1       1       0       0       0       1       1
    AUXR1 &= 0xfb;    // ��8λ�洢����ADC_RES
    CLK_DIV = 0x01;   // ϵͳʱ��/2
}
void init_timer0() // ��ʱ��0��ʼ��
{                  // 1/4ms
    TMOD &= 0XF0;  // Timer0�������
    TMOD |= 0X02;  // 8λ�Զ�����ģʽ
    TL0 = 0X06;
    TH0 = 0X06;
    TR0 = 1;
    ET0 = 1;
}
void init_special_interrupts() // �жϳ�ʼ��
{
    EX0 = 0;
    ET0 = 1; // ��ʱ��0�ж�
    EX1 = 0;
    ET1 = 1; // ��ʱ��1�ж�
    ES = 1;	 // �ⲿ�ж�
    EA = 1;  // ���ж�
    PT0 = 0;
    PX1 = 1;
    PS = 0;
}
//==========