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
	ad_max = 128,  // �����źŵ�ǰ���ֵ
	ad_min = 128,  // �����źŵ�ǰ��Сֵ
	ad_result = 128, // A/D���βɼ��źŽ��
	amp_p1 = 129,  	 // Ƶ�ʲ���ָ��1
	amp_p2 = 129,  	 // Ƶ�ʲ���ָ��2
	da_count = 0,  	 // �źŷ�������ʱ����
	da_result = 0, 	 // �źŷ��������β����������ź�
	fre_count = 0,	 // ��ֵ��������
	fre_p1 = 1,	   	 // Ƶ�ʲ���ָ��1 
	fre_p2 = 1,	   	 // Ƶ�ʲ���ָ��2
	fre_sum = 0,   	 // Ƶ�ʲ����ۼ�
	rec_count = 0, 	 // �����źŷ�������
	rec_value = 64,  // �����ź�
	review_p = 0,  // D/A�ط���ʾָ��
	saw_value = 64,  // ��ݲ��ź�
	time_count = 0,  // ����ʱ�����
	tri_value = 64,  // ���ǲ��ź�
	tri_flag = 1;    // ���ǲ���ֵ�����ݼ���־

unsigned int 
	angle = 0,	   // ���Ҳ��Ƕȣ��Ƕ�ֵ��[0,360]
	memo_p = 0;	   // A/D�ɼ��洢��ָ��

float
	sinAngle = 0.0f,  // ���Ҳ��Ƕȣ������ƣ�[-1,1]
	fre_es = 0.0f,	  // ������Ƶ��
	amp_es = 0.0f;	  // ��ֵ����ֵ�������ƣ�[-5,5]
unsigned char xdata
    ad_memo[MEMO_LEN]; 	// A/D�ɼ��źŴ洢��
//============
//==����==
unsigned char ad_get()// A/D���������Σ�
{
    ADC_CONTR = ADC_CONTR | ADC_POWER | 3 | ADC_START;
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
    ad_memo[memo_p++] = ad_result;
    if (memo_p >= MEMO_LEN)
    {
        memo_p = 0;
    }
}
void ad_measure()	// Ƶ�ʷ�ֵ���μ��
{
	// Ƶ�ʲ���
	amp_p2 = ad_result;
	time_count++;
	if(amp_p2 > 128 && amp_p1 <= 128){
		fre_p2 = time_count; 
		if(fre_p1 != 0){
		 	fre_sum = fre_sum + fabs(fre_p2 - fre_p1);
			fre_count++;
		}
		fre_p1 = fre_p2;	
	}
	amp_p1 = amp_p2;

	// ��ֵ����
	if(ad_result > ad_max){
	 	ad_max = ad_result;
	}	
	if(ad_result < ad_min){
	 	ad_min = ad_result;
	}
	if(time_count >= AD_LEN){
	 	fre_es = 1500.0f / (fre_sum * 1.0f / fre_count);
		amp_es = 5.0f * ((ad_max-ad_min)/256.0f);
		fre_sum = 0;fre_count = 0;
		time_count = 0;
		fre_p1 = fre_p2 = 0;
		amp_p1 = amp_p2 = 129;
		ad_max = ad_min = 128;
	}	
}
void da_display()
{										
	switch(mode){
	 	case 1:{	 // ģʽ1 CH1ʵʱ��ʾ
			DA_CH1 = ad_result;// ʵʱ��ʾ
		};break;
		case 2:{ 	 // ģʽ2 CH1ʵʱ��ʾ CH2�ط���ʾ
		 	DA_CH1 = ad_result;// ʵʱ��ʾ
			DA_CH2 = ad_memo[review_p++]; // �ط���ʾ
			if (review_p >= MEMO_LEN){ // ����ʾָ�뵽��洢��ĩβ����ͷ��ʼ��ʾ
        		review_p = 0;
   			}
		};break;
		default: ;break;
	}
}
void da_generator()// �źŷ�����
{
 	switch(wave_sel){ // ����wave_sel��ѡ�������Ӧ����
		case SIN:{	// ���Ҳ�
			angle++;
			if(angle > 360){
			 	angle = 0;
			}
		 	sinAngle = 	sin(((float)angle * PI / 180.0f));// ��Ƕȶ�Ӧ�Ļ���ֵ[-1,1]
			da_result = (int)(((sinAngle + 1.0f) / 2.0f) * 255.0f); // ��[-1,1]�ڵ�sinֵӳ�䵽[0,255]
		};break;
		case TRI:{	// ���ǲ�
			if(tri_flag){//����
				tri_value++;
				if(tri_value == 255){
				 	tri_flag = 0;
				}	
			}
			else{//�ݼ�
				tri_value--;
				if(tri_value == 0){
					tri_flag = 1;
				}
			}	 
			da_result = tri_value;	
		};break;	
		case REC:{	// ����
			rec_count++;
			if(rec_count == 200){
				rec_count = 0;	 		
			}
			if(rec_count>100){
				da_result = 255;
			}else{
			 	da_result = 0;
			}		
		};break;
		case SAW:{	// ��ݲ�
			saw_value++;
			if(saw_value > 255){
			 	saw_value = 0;
			}	
			da_result = saw_value;
		};break;
		default:da_result = 128;break;	
	}
}
void da_process(){ 	// �������ʾ��D/A���ֵ
	da_result = (int)(da_result * 1.0f / da_index + 128.0f -128.0f / da_index); 
}
void dspbuf_measure()	// �������õ���Ƶ�ʺͷ�ֵ��ʾ���������
{
	// Ƶ��ֵת��Ϊ�������ʾ
	if(flag_fre){
		unsigned int integer_fre = 0;
		unsigned int fraction_fre = 0;
		integer_fre = (int)fre_es;
		dspbuf[3] = dspchar[integer_fre % 10];
		dspbuf[2] = dspchar[integer_fre / 10 % 10];
		dspbuf[1] = dspchar[integer_fre / 100 % 10];
		dspbuf[0] = dspchar[integer_fre / 1000 % 10];
	}
	// ��ֵת��Ϊ�������ʾ
	else if(flag_amp){
	 	if(amp_es >= 0.0f && amp_es <10.0f){
			unsigned int integer_amp = 0;
			unsigned int fraction_amp = 0;
			integer_amp = (int) amp_es;
			fraction_amp = (int) ((amp_es-integer_amp)*1000);
			dspbuf[0] = dspchar[integer_amp] & 0xfe;// ��С����
			dspbuf[1] = dspchar[fraction_amp / 100 % 10];
			dspbuf[2] = dspchar[fraction_amp / 10 % 10];
			dspbuf[3] = dspchar[fraction_amp % 10];
		}
	} 
}
//========