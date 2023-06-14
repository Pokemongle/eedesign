//==function==
unsigned char ad_get(); // A/D采样（单次）
void ad_save(); 	  	// A/D采样值存储
void da_display();		// D/A显示
//============
#define MEMO_LEN 8000		 // 回放显示存储长度(BYTE)
#define SAVE_GAP 300		 // 存储间隔
#define DA_CH1 XBYTE[0x4000] // DA显示通道CH1
#define DA_CH2 XBYTE[0x2000] // DA显示通道CH2
#define PI 3.1415926535
#define AD_LEN 250
//==parameter==
extern unsigned char
	ad_get_count,
	ad_save_count,
	ad_display_count,
	ad_result,   // A/D单次采集信号结果
	ad_result_display,
	da_result; 	 // 信号发生器单次产生的数字信号	
extern unsigned int 
    review_p,    // D/A回放显示指针
	memo_p;	   // A/D采集存储区指针
//extern unsigned char xdata
    //ad_memo[]; 	// A/D采集信号存储区
//============
