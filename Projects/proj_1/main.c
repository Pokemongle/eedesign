/*
 * @ packaged_edition
 * - package edition 主程序
 * - 各模块封装于 Projects, App, Obj, Public 文件夹中便于维护
 * - ../../demos文件夹为 master git 库，用于版本控制
 *
 */

//==头文件==
#include <STC12C5A.h> // 51标准头文件
#include <absacc.h>	  // 时钟分频？
#include <intrins.h>  // 用于调用系统延时函数 _nop_()
#include <math.h>	  // 用于调用数学函数 sin, abs等
//==========
//==全局变量声明==
unsigned char 
	key_sta = 0; // 按键状态
//================
//==函数声明==
void init_special_interrupts() // 中断初始化
void init_timer0(); // 定时器0初始化

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