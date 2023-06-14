
//==函数声明==
void dsptask();
void fdisp(unsigned char num,unsigned char id);//在id号led显示数字num
void dsptem();// 显示温度
//============
//==变量==
extern unsigned char 
	dspbuf[],
	dspchar[],
	sel;
extern float
	temper;
//========