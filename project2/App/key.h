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
	key_col,   // 按键扫描当前列
	key_num,   // 按键序号
	key_sta,
	mode;	   // 模式选择
//============