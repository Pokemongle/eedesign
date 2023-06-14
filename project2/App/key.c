//==头文件==
#include <STC12C5A.h> // 51标准头文件
#include "key.h"
#include "led.h"
#include "adda.h"
//==========
//==变量声明==
unsigned char
	key_col = 0,   // 按键扫描当前列
	key_num = 0,   // 按键序号
	key_sta = 0,   // 按键当前状态
	mode = 0;	   // 模式选择

//============
void key_service()
{
	if(key_sta & 0x01) return;//key_sta.0 = 1，按键为按下状态
	// key_sta.0 = 0，按键为弹起状态，检测到KEY1/KEY2行有按键被按下
	if(KEY1){
	 	key_num = key_col + 1; 
		key_sta = key_sta | 0x01;//key_sta.0 = 1
	}	
	else if(KEY2){
	 	key_num = key_col + 5;
		key_sta = key_sta | 0x01;//key_sta.0 = 1	
	}
}
void mode_select()	// 模式选择
{
 	switch(key_num){
	 	case 1:{		// 模式1：实时采样+存储
			mode = 1;
			fdisp(1,0); // 数码管最左端显示模式1
			fdisp(0,1);
			fdisp(0,2);
			fdisp(0,3);
		};break;
		case 2:{		// 模式2：波形回放10min波形
			mode = 2;
			fdisp(2,0);	// 数码管最左端显示模式2
			fdisp(0,1);
			fdisp(0,2);
			fdisp(0,3);	// 数码管最右端波形显示清零 	
		};break;
		case 3:{		// 模式3：停止显示
			mode = 3;
			fdisp(3,0);
			fdisp(0,1);
			fdisp(0,2);
			fdisp(0,3);
		};break;	
		default:break;
	}	
}
