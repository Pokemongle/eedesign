
/**
	* @brief   Memory oscilloscope system design 
	* @edition packaged edition, peripheral functions are embedded in demos/App, convenient for maintenance
	*		-edtition controlled by github: https://github.com/Pokemongle/eedesign
	*			-branches:
	*					master: all functions listed in main.c
	*					release_01(now): packaged edition
	* @tips	
	*		-Xtal:12Hz
	*		-Remember to set Include Path, especially for the standard head file: STC12C5A.h in demos/Public
	*		 do not use the standard head file reg52.h, this will cause errors because some pins are not defined
	*		-Have fun :)
	* @author	from 2020 Biomedical Engineering Institute, Zhejiang University 
	*		-Name: 				Ziyue Xiong,	Min Zhao
	*		-Student ID:	3200105229,		3200100446		
	*/

//==头文件==
#include <STC12C5A.h> // STC12C5A标准头文件
#include <absacc.h>	  // 时钟分频，外部RAM
#include <intrins.h>  // 用于调用系统延时函数 _nop_()
#include <math.h>	  	// 用于调用数学函数 sin, abs等
#include "adda.h"
#include "key.h"
#include "led.h"
//==========
//==函数声明==
void init_ad();//A/D初始化
void init_special_interrupts(); // 中断初始化
void init_timer0(); // 定时器0初始化
//============
//==引脚设置==
sbit Y = P1^5;
//============

//==函数体==
void main()
{																													                                                            
    init_timer0();
    init_special_interrupts();
    init_ad();
    while (1)
    {
			if(key_sta & 0x01){//key_sta.0 = 0x01? key_sta.0 = 1检测到按键按下
				mode_select();
				key_sta = key_sta & 0xfe;//key_sta.0 = 0,按键弹起，等待下一次检测到按下
			}
    }
}
void interrupt_timer0() interrupt 1 // 定时器0中断处理
{
	EA = 0;
	ad_result = ad_get();// 单次采样 		
	Y = ~Y;
	ad_result = ((int)(ad_result / 2.0 + 64));
	if(mode == 1){		// CH1：实时采样并存储 CH2: 信号发生器
		// 信号发生
		da_generator();
		da_result = (int)(da_result * 1.0f / 2 + 128.0f -128.0f / 2); // 原幅值输出
		// 模式1对实时采样值进行存储
	 	ad_save();
	}
	else if(mode == 2){	// CH1: 实时采样 CH2: 波形回放显示	
	}
	else if(mode == 3){ // CH1: 实时采样 CH2: 输出0V	
		ad_measure();
		dspbuf_measure();//数码管显示缓存改变
	}
	else if(mode == 4){	// CH1: 实时采样 CH2: 信号发生器，调频调幅
		// 信号发生器
		da_count++;
		if(da_count >= fre_index){ // fre_index = 4时，每隔1ms输出一次D/A
			da_count = 0;
			da_generator();
			da_process();	// 显示值预处理
		}	
	}
	dsptask();		// 数码管扫描
	key_service(); 	// 按键扫描
  da_display();	// D/A显示
	EA = 1;
}
void init_ad() // ad寄存器初始化
{
    P1ASF = 0x08;     // P1.3作为ad输入
    ADC_CONTR = 0xE3; // ADC控制寄存器         Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                      // 位描述            ADC_POWER  SPEED1 SPEED0 ADC_FLAG ADC_START CHS2    CHS1    CHS0
                      // 初始值=0000,0000      0       0       0       0       0       0       0       0
                      // 设置值=0000,0000      1       1       1       0       0       0       1       1
    AUXR1 &= 0xfb;    // 高8位存储在在ADC_RES
    CLK_DIV = 0x01;   // 系统时钟/2
}
void init_timer0() // 定时器0初始化
{                  // 1/4ms
    TMOD &= 0XF0;  // Timer0设置清除
    TMOD |= 0X02;  // 8位自动重载模式
    TL0 = 0X06;
    TH0 = 0X06;
    TR0 = 1;
    ET0 = 1;
}
void init_special_interrupts() // 中断初始化
{
    EX0 = 0;
    ET0 = 1; // 定时器0中断
    EX1 = 0;
    ET1 = 1; // 定时器1中断
    ES = 1;	 // 外部中断
    EA = 1;  // 总中断
    PT0 = 0;
    PX1 = 1;
    PS = 0;
}
//==========