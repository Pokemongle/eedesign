//==ͷ�ļ�==
#include <STC12C5A.h> // 51��׼ͷ�ļ�
#include "key.h"
#include "led.h"
//==========
//==��������==
unsigned char 
	key_num = 0,	// �������
	mode = 0,	    // ģʽѡ��
	wave_sel = 0;   // �źŷ���������ѡ��
//============
void mode_select()	// ģʽѡ��
{
 	switch(key_num){
	 	case 1:{		// ģʽ1��ʵʱ����+�洢���źŷ�����
			mode = 1;
			fdisp(1,0); // ������������ʾģʽ1
			fdisp(0,1);
			fdisp(0,2);
			fdisp(wave_sel,3);
		};break;
		case 2:{		// ģʽ2��ʵʱ���������λط�
			mode = 2;
			fdisp(2,0);	// ������������ʾģʽ2
			fdisp(0,1);
			fdisp(0,2);
			fdisp(0,3);	// ��������Ҷ˲�����ʾ���� 	
		};break;
		case 3:{		// ģʽ3������ʵʱ�����ķ�ֵ��Ƶ�ʣ������ڣ�
			mode = 3;
			fdisp(3,0);
			fdisp(0,1);
			fdisp(0,2);
			fdisp(0,3);
		};break;
		case 4:{		// �źŷ�������Ƶ����
			mode = 4;			
			fdisp(4,0);		  // ������������ʾģʽ4
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
			else if(mode == 4){		// ģʽ4�£�����5��ʾƵ��1
				/*if(gen_count >= 50000){
					gen_count = 50000;
				}
				else{
					gen_count++;
				}
				*/
				if(fre_modi){
				 	gen_count = 4;
				}
				if(amp_modi){
				 	da_index = 2;
				}
				//gen_count = 4;
				//da_index = 2;
			}
		};break;
		case 6:{
			if(mode == 1){ 			// ģʽ1�£�����5��ʾѡ�����ǲ�
			 	wave_sel = TRI;
				fdisp(wave_sel,3);
			}
			else if(mode == 3){		// ģʽ3�£�����6��ʾ������ֵ
			 	flag_fre = 0;
				flag_amp = 1;		
			} 
			else if(mode == 4){		// ģʽ4�£�����6��ʾƵ��2
				/*
				if(gen_count <= 4){
					gen_count = 4;
				}else{
					gen_count--;
				}
				*/
				if(fre_modi){
				 	gen_count = 3;
				}
				if(amp_modi){
				 	da_index = 3;
				}
			}	
		};break;
		case 7:{
			if(mode == 1){			// ģʽ1�£�����7��ʾѡ�񷽲�
			 	wave_sel = REC;		
				fdisp(wave_sel,3);	
			}
			else if(mode == 3){
			 	fre_modi = 1;
				amp_modi = 0;
			}
			else if(mode == 4){		// ģʽ4�£�����7��ʾ���ȼ�С,Ƶ��3
			 	/*
				if(da_index <= 1000){
					da_index = 1000;
				}
				else{
				 	da_index--;
				}
				*/
				if(fre_modi){
				 	gen_count = 2;
				}
				if(amp_modi){
				 	da_index = 4;
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
			else if(mode == 4){		// ģʽ4�£�����8��ʾ��������,Ƶ��4
				/*
				if(da_index >= 5000){
				 	da_index = 5000;
				}
				else{
				 	da_index++;
				}
				*/
				if(fre_modi){
				 	gen_count = 1;
				}
				if(amp_modi){
				 	da_index = 5;
				}
			}
		}; break;
		default:break;
	}
	
}
