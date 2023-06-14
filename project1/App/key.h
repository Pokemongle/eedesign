//==宏定义==
#define SIN 1// 正弦波
#define TRI 2// 三角波
#define REC	3// 方波
#define SAW 4// 锯齿波
//==========
sbit KEY1 = P3 ^ 5;
sbit KEY2 = P3 ^ 4;
//==函数声明==
void key_service(); // 按键扫描
void mode_select(); // 模式选择
//============
//==变量声明==
extern unsigned char
	amp_modi,  // 调幅标志
	amp_index,  // 调幅参数
	flag_amp,  // 显示幅值标志
	flag_fre,  // 显示频率标志 
	fre_index, // 调频参数
	fre_modi,  // 调频标志
	key_col,   // 按键扫描当前列
	key_num,   // 按键序号
	key_sta,
	mode,	   // 模式选择
	wave_sel;  // 信号发生器波形选择

//============