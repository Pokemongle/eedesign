//==头文件==
#include <STC12C5A.h> // 51标准头文件
#include "key.h"
#include "led.h"
//==========
//==变量声明==
unsigned char 
	key_num = 0,	// 按键序号
	mode = 0,	    // 模式选择
	wave_sel = 0;   // 信号发生器波形选择
//============
void mode_select()	// 模式选择
{
 	switch(key_num){
	 	case 1:{		// 模式1：实时采样+存储，信号发生器
			mode = 1;
			fdisp(1,0); // 数码管最左端显示模式1
			fdisp(0,1);
			fdisp(0,2);
			fdisp(wave_sel,3);
		};break;
		case 2:{		// 模式2：实时采样，波形回放
			mode = 2;
			fdisp(2,0);	// 数码管最左端显示模式2
			fdisp(0,1);
			fdisp(0,2);
			fdisp(0,3);	// 数码管最右端波形显示清零 	
		};break;
		case 3:{		// 模式3：测量实时采样的幅值和频率（或周期）
			mode = 3;
			fdisp(3,0);
			fdisp(0,1);
			fdisp(0,2);
			fdisp(0,3);
		};break;
		case 4:{		// 信号发生器调频调幅
			mode = 4;			
			fdisp(4,0);		  // 数码管最左端显示模式4
			fdisp(0,1);
			fdisp(0,2);
			fdisp(wave_sel,3);// 数码管最右端显示波形	
		};break;
		case 5:{
		 	if(mode == 1){			// 模式1下，按键5表示选择正弦波
			 	wave_sel = SIN;
				fdisp(wave_sel,3); 	// 在数码管最右端显示波形选择
			}
			else if(mode == 2){;}
			else if(mode == 3){		// 模式3下，按键5显示测量频率
				flag_amp = 0;																 
				flag_fre = 1;
			}
			else if(mode == 4){		// 模式4下，按键5表示频率1
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
			if(mode == 1){ 			// 模式1下，按键5表示选择三角波
			 	wave_sel = TRI;
				fdisp(wave_sel,3);
			}
			else if(mode == 3){		// 模式3下，按键6表示测量幅值
			 	flag_fre = 0;
				flag_amp = 1;		
			} 
			else if(mode == 4){		// 模式4下，按键6表示频率2
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
			if(mode == 1){			// 模式1下，按键7表示选择方波
			 	wave_sel = REC;		
				fdisp(wave_sel,3);	
			}
			else if(mode == 3){
			 	fre_modi = 1;
				amp_modi = 0;
			}
			else if(mode == 4){		// 模式4下，按键7表示幅度减小,频率3
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
			if(mode == 1){			// 模式1下，按键8表示选择锯齿波
			 	wave_sel = SAW;
				fdisp(wave_sel,3);
			}
			else if(mode == 3){
				fre_modi = 0;
				amp_modi = 1;
			}
			else if(mode == 4){		// 模式4下，按键8表示幅度增大,频率4
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
