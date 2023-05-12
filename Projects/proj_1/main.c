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
//==========
//==ȫ�ֱ�������==
unsigned char 
	key_sta = 0; // ����״̬
//================
//==��������==
void init_special_interrupts() // �жϳ�ʼ��
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