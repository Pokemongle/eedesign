//==ͷ�ļ�==
#include <STC12C5A.h> // 51��׼ͷ�ļ�
#include "led.h"
#include "key.h"
#include "adda.h"
#include "math.h"
//==========
sbit D_SER = P1 ^ 0;
sbit D_SRCLK = P1 ^ 1;
sbit D_RCLK = P1 ^ 2;
//==����==
unsigned char 
dspbuf[4] = {	 // �������ʾ����
    0x11, 0x11, 0x11, 0x11
}, 
dspchar[10] = {	 // ������ַ���0-9
	0x11,0x7d,0x23,0x29,
	0x4d,0x89,0x81,0x3d,
	0x01,0x09	
},
	sel = 0;
float temper = 0.0;
//========
void dsptem(){
	unsigned char baiwei;
	unsigned char shiwei;
	unsigned char gewei;
	unsigned char xiaoshu;
	float u = 0.0;
	u = ((ad_result_display-128.0)/128.0) * 5.0;
	temper=11.62*(u*u)-64.07*u+99.61;
	baiwei = ((int)(temper)) / 100 % 10;
	shiwei = ((int)(temper)) / 10 % 10;
	gewei = ((int)(temper)) % 10;
	xiaoshu = (int)((temper-((int)(temper))) * 10);
	dspbuf[0] = dspchar[baiwei];
	dspbuf[1] = dspchar[shiwei];
	dspbuf[2] = dspchar[gewei] & 0xFE;//��С����
	dspbuf[3] = dspchar[xiaoshu];	 	
}
void dsptask()
{
    unsigned char i;	// for loop ��ʱ����
    unsigned char a, b; // a:�����λѡ��b:����

    // �����λѡ
    switch (sel)
    {
    case 0:	   // L1-T204
        a = 0x01;
        break; 
    case 1:	   // L2-T203
        a = 0x02;  
        break; 
    case 2:	   // L3-T202
        a = 0x04;
        break; 		
    default:   // L4-T201
        a = 0x08; 
    }
	// ����ʾ�����λѡ����Ϣ���뵽74HC565�ĸ�λ8bit 
	// ��Ӧ����ܵ�COM1Ϊ�ߵ�ƽ������������ܣ�
    for (b = 0x80, i = 0; i < 8; i++)
    {
        if (a & b)
            D_SER = 1;
        else
            D_SER = 0;
        D_SRCLK = 0;
        D_SRCLK = 1;
        D_SRCLK = 0;
        b = b >> 1;
        // b=b&0x7f;
    }
    // ����λѡ����ܶ�Ӧ����ʾ����
    a = dspbuf[sel];
	// ������ѡ��
	key_col = sel;
	// �����ı䣬ѡ����һ�������
    sel++;
    if (sel >= 4)
        sel = 0; 
	// ������ܶ�Ӧ��ʾ�������뵽����74HC565�ĵ�8bit
    for (b = 0x80, i = 0; i < 8; i++)
    {
        if (a & b)
            D_SER = 1;
        else
            D_SER = 0;
        D_SRCLK = 0;
        D_SRCLK = 1;
        D_SRCLK = 0;
        b = b >> 1;
        // b=b&0x7fff;
    }
    D_RCLK = 0;
    D_RCLK = 1;
    D_RCLK = 0;
}
void fdisp(unsigned char num,unsigned char id)
{
 	char c;
	switch(num){
	 	case 0: c = 0x11; break;
		case 1: c = 0x7d; break;
		case 2: c = 0x23; break;
		case 3: c = 0x29; break;
		case 4: c = 0x4d; break;
		case 5: c = 0x89; break;
		case 6: c = 0x81; break;
		case 7: c = 0x3d; break;
		case 8: c = 0x01; break;
		case 9: c = 0x09; break;
		default:c = 0xff; break;
	}
	dspbuf[id] = c;
}