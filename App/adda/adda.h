#define DA_CH1 XBYTE[0x4000] // DA显示通道CH1
#define DA_CH2 XBYTE[0x2000] // DA显示通道CH2
#define MEMO_LEN 	8000 	 // 回放显示存储长度(BYTE)
#define AD_LEN 250
#define SIN 1				 // 正弦波
#define TRI 2				 // 三角波
#define REC	3				 // 方波
#define SAW 4				 // 锯齿波
#define PI 3.1415926535 	 // 数学常数Π
//==========变量声明==============
extern unsigned char xdata 
	ad_memo[]; // A/D采集信号存储区
extern unsigned char
	ad_result,	// A/D单次采集信号结果 
	memo_p, 	// A/D采集存储区指针
	
//==========函数声明==============
void ad_measure(); // A/D测量
void ad_save();    // A/D采样值存储

