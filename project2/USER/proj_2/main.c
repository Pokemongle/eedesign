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
unsigned int save_count = 0;
sbit Y = P1^5;
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
				switch(mode){
					case 1:{ // ģʽ1��CH1ʵʱ��ʾ
						dsptem();
						da_display();
					}break;
					case 2:{ // ģʽ2��CH1ʵʱ��ʾ��CH2�ط���ʾ
						dsptem();
						da_display();
					}break;
					case 3:{	   //�ر���ʾ
						fdisp(3,0); 
						fdisp(0,1);
						fdisp(0,2);
						fdisp(0,3);
					}
					default: break;
				}
    }
	

}
void interrupt_timer0() interrupt 1 // ��ʱ��0�жϴ���
{
	EA = 0;

	ad_result = ad_get();	// ����
	Y = ~Y;
	ad_result_display = ((int)(ad_result / 2.0 + 128));// ����ֵ����
	 
	ad_save_count = ad_save_count++;
	if(ad_save_count >= 150){
			ad_save_count = 0;
			if(mode == 1){
				ad_save();
			}	
	}
	dsptask();
	key_service();
	EA = 1;
}
void init_ad() // ad�Ĵ�����ʼ��
{
    P1ASF = 0x10;     // P1.3��Ϊad����
    ADC_CONTR = 0xE4; // ADC���ƼĴ���         Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
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