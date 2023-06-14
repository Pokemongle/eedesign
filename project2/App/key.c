//==ͷ�ļ�==
#include <STC12C5A.h> // 51��׼ͷ�ļ�
#include "key.h"
#include "led.h"
#include "adda.h"
//==========
//==��������==
unsigned char
	key_col = 0,   // ����ɨ�赱ǰ��
	key_num = 0,   // �������
	key_sta = 0,   // ������ǰ״̬
	mode = 0;	   // ģʽѡ��

//============
void key_service()
{
	if(key_sta & 0x01) return;//key_sta.0 = 1������Ϊ����״̬
	// key_sta.0 = 0������Ϊ����״̬����⵽KEY1/KEY2���а���������
	if(KEY1){
	 	key_num = key_col + 1; 
		key_sta = key_sta | 0x01;//key_sta.0 = 1
	}	
	else if(KEY2){
	 	key_num = key_col + 5;
		key_sta = key_sta | 0x01;//key_sta.0 = 1	
	}
}
void mode_select()	// ģʽѡ��
{
 	switch(key_num){
	 	case 1:{		// ģʽ1��ʵʱ����+�洢
			mode = 1;
			fdisp(1,0); // ������������ʾģʽ1
			fdisp(0,1);
			fdisp(0,2);
			fdisp(0,3);
		};break;
		case 2:{		// ģʽ2�����λط�10min����
			mode = 2;
			fdisp(2,0);	// ������������ʾģʽ2
			fdisp(0,1);
			fdisp(0,2);
			fdisp(0,3);	// ��������Ҷ˲�����ʾ���� 	
		};break;
		case 3:{		// ģʽ3��ֹͣ��ʾ
			mode = 3;
			fdisp(3,0);
			fdisp(0,1);
			fdisp(0,2);
			fdisp(0,3);
		};break;	
		default:break;
	}	
}
