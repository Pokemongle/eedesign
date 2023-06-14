//==ͷ�ļ�==
#include <STC12C5A.h> // 51��׼ͷ�ļ�
#include "key.h"
#include "led.h"
#include "adda.h"
//==========
//==��������==
unsigned char
	amp_modi = 0,  // ������־
	amp_index = 2,  // ��������
	flag_amp = 0,  // ��ʾ��ֵ��־
	flag_fre = 0,  // ��ʾƵ�ʱ�־ 
	fre_modi = 0,  // ��Ƶ��־
	fre_index = 4, // ��������
	key_col = 0,   // ����ɨ�赱ǰ��
	key_num = 0,   // �������
	key_sta = 0,
	mode = 0,	   // ģʽѡ��
	wave_sel = 0;  // �źŷ���������ѡ��

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
	 	case 1:{				// ģʽ1��ʵʱ����+�洢���źŷ�����
			mode = 1;
			fdisp(1,0); 	// ������������ʾģʽ1
			fdisp(0,1);
			fdisp(0,2);
			fdisp(wave_sel,3);
		};break;
		case 2:{				// ģʽ2��ʵʱ���������λط�
			mode = 2;
			fdisp(2,0);		// ������������ʾģʽ2
			fdisp(0,1);
			fdisp(0,2);
			fdisp(0,3);		// ��������Ҷ˲�����ʾ���� 	
		};break;
		case 3:{				// ģʽ3������ʵʱ�����ķ�ֵ��Ƶ�ʣ������ڣ�
			mode = 3;
			fdisp(3,0);
			fdisp(0,1);
			fdisp(0,2);
			fdisp(0,3);
		};break;
		case 4:{				// �źŷ�������Ƶ����
			mode = 4;			
			fdisp(4,0);		// ������������ʾģʽ4
			fdisp(0,1);
			fdisp(0,2);
			fdisp(wave_sel,3);// ��������Ҷ���ʾ����	
		};break;
		case 5:{
		 	if(mode == 1){			// ģʽ1�£�����5��ʾѡ�����Ҳ�
			 	wave_sel = SIN;
				fdisp(wave_sel,3); 	// ����������Ҷ���ʾ����ѡ��
			}
			else if(mode == 2){;}
			else if(mode == 3){		// ģʽ3�£�����5��ʾ����Ƶ��
				flag_amp = 0;																 
				flag_fre = 1;
			}
			else if(mode == 4){		// ģʽ4�£�����5��ʾƵ��1������ֵ1��
				if(fre_modi){
				 	fre_index = 4;
				}
				if(amp_modi){
				 	amp_index = 2;
				}
			}
		};break;
		case 6:{
			if(mode == 1){ 			// ģʽ1�£�����6��ʾѡ�����ǲ�
			 	wave_sel = TRI;
				fdisp(wave_sel,3);
			}
			else if(mode == 3){		// ģʽ3�£�����6��ʾ������ֵ
			 	flag_fre = 0;
				flag_amp = 1;		
			} 
			else if(mode == 4){		// ģʽ4�£�����6��ʾƵ��2������ֵ2��
				if(fre_modi){
				 	fre_index = 3;
				}
				if(amp_modi){
				 	amp_index = 3;
				}
			}	
		};break;
		case 7:{
			if(mode == 1){			// ģʽ1�£�����7��ʾѡ�񷽲�
			 	wave_sel = REC;		
				fdisp(wave_sel,3);	
			}
			else if(mode == 3){	// ģʽ3�£�����7��ʾ���е�Ƶ
			 	fre_modi = 1;
				amp_modi = 0;
			}
			else if(mode == 4){		// ģʽ4�£�����7��ʾƵ��3������ֵ3��
				if(fre_modi){
				 	fre_index = 2;
				}
				if(amp_modi){
				 	amp_index = 4;
				}
			}
		};break;
		case 8:{					
			if(mode == 1){			// ģʽ1�£�����8��ʾѡ���ݲ�
			 	wave_sel = SAW;
				fdisp(wave_sel,3);
			}
			else if(mode == 3){
				fre_modi = 0;
				amp_modi = 1;
			}
			else if(mode == 4){		// ģʽ4�£�����8��ʾƵ��4������ֵ4��
				if(fre_modi){
				 	fre_index = 1;
				}
				if(amp_modi){
				 	amp_index = 5;
				}
			}
		}; break;
		default:break;
	}	
}
