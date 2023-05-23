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
	ad_max = 128,  // 测量信号当前最大值
	ad_min = 128,  // 测量信号当前最小值
	ad_result = 128, // A/D单次采集信号结果
	amp_p1 = 129,  	 // 频率测量指针1
	amp_p2 = 129,  	 // 频率测量指针2
	da_count = 0,  	 // 信号发生器定时计数
	da_result = 0, 	 // 信号发生器单次产生的数字信号
	fre_count = 0,	 // 幅值测量计数
	fre_p1 = 1,	   	 // 频率测量指针1 
	fre_p2 = 1,	   	 // 频率测量指针2
	fre_sum = 0,   	 // 频率测量累加
	rec_count = 0, 	 // 方波信号发生计数
	rec_value = 64,  // 方波信号
	review_p = 0,  // D/A回放显示指针
	saw_value = 64,  // 锯齿波信号
	time_count = 0,  // 测量时间计数
	tri_value = 64,  // 三角波信号
	tri_flag = 1;    // 三角波幅值递增递减标志

unsigned int 
	angle = 0,	   // 正弦波角度，角度值，[0,360]
	memo_p = 0;	   // A/D采集存储区指针

float
	sinAngle = 0.0f,  // 正弦波角度，弧度制，[-1,1]
	fre_es = 0.0f,	  // 测量的频率
	amp_es = 0.0f;	  // 幅值测量值，弧度制，[-5,5]
unsigned char xdata
    ad_memo[MEMO_LEN]; 	// A/D采集信号存储区
//============
//==函数==
unsigned char ad_get()// A/D采样（单次）
{
    ADC_CONTR = ADC_CONTR | ADC_POWER | 3 | ADC_START;
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
    ad_memo[memo_p++] = ad_result;
    if (memo_p >= MEMO_LEN)
    {
        memo_p = 0;
    }
}
void ad_measure()	// 频率幅值单次检测
{
	// 频率测量
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

	// 幅值测量
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
	 	case 1:{	 // 模式1 CH1实时显示
			DA_CH1 = ad_result;// 实时显示
		};break;
		case 2:{ 	 // 模式2 CH1实时显示 CH2回放显示
		 	DA_CH1 = ad_result;// 实时显示
			DA_CH2 = ad_memo[review_p++]; // 回放显示
			if (review_p >= MEMO_LEN){ // 若显示指针到达存储区末尾，从头开始显示
        		review_p = 0;
   			}
		};break;
		default: ;break;
	}
}
void da_generator()// 信号发生器
{
 	switch(wave_sel){ // 根据wave_sel的选择产生相应波形
		case SIN:{	// 正弦波
			angle++;
			if(angle > 360){
			 	angle = 0;
			}
		 	sinAngle = 	sin(((float)angle * PI / 180.0f));// 求角度对应的弧度值[-1,1]
			da_result = (int)(((sinAngle + 1.0f) / 2.0f) * 255.0f); // 将[-1,1]内的sin值映射到[0,255]
		};break;
		case TRI:{	// 三角波
			if(tri_flag){//递增
				tri_value++;
				if(tri_value == 255){
				 	tri_flag = 0;
				}	
			}
			else{//递减
				tri_value--;
				if(tri_value == 0){
					tri_flag = 1;
				}
			}	 
			da_result = tri_value;	
		};break;	
		case REC:{	// 方波
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
		case SAW:{	// 锯齿波
			saw_value++;
			if(saw_value > 255){
			 	saw_value = 0;
			}	
			da_result = saw_value;
		};break;
		default:da_result = 128;break;	
	}
}
void da_process(){ 	// 处理待显示的D/A输出值
	da_result = (int)(da_result * 1.0f / da_index + 128.0f -128.0f / da_index); 
}
void dspbuf_measure()	// 将采样得到的频率和幅值显示到数码管上
{
	// 频率值转换为数码管显示
	if(flag_fre){
		unsigned int integer_fre = 0;
		unsigned int fraction_fre = 0;
		integer_fre = (int)fre_es;
		dspbuf[3] = dspchar[integer_fre % 10];
		dspbuf[2] = dspchar[integer_fre / 10 % 10];
		dspbuf[1] = dspchar[integer_fre / 100 % 10];
		dspbuf[0] = dspchar[integer_fre / 1000 % 10];
	}
	// 幅值转换为数码管显示
	else if(flag_amp){
	 	if(amp_es >= 0.0f && amp_es <10.0f){
			unsigned int integer_amp = 0;
			unsigned int fraction_amp = 0;
			integer_amp = (int) amp_es;
			fraction_amp = (int) ((amp_es-integer_amp)*1000);
			dspbuf[0] = dspchar[integer_amp] & 0xfe;// 加小数点
			dspbuf[1] = dspchar[fraction_amp / 100 % 10];
			dspbuf[2] = dspchar[fraction_amp / 10 % 10];
			dspbuf[3] = dspchar[fraction_amp % 10];
		}
	} 
}
//========