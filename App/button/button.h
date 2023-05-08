//矩阵键盘行列
sbit ROW1 = P1^7;
sbit ROW2 = P1^6;
sbit ROW3 = P1^5;
sbit ROW4 = P1^4;
sbit COL1 = P1^3;
sbit COL2 = P1^2;
sbit COL3 = P1^1;
sbit COL4 = P1^0;
static unsigned char code KeyCodeMap[4][4]={//矩阵按键编号到标准键盘键码的映射表
	{},
	{},
	{},
	{}
};
static unsigned char KeySta[4][4]={//全部按键当前的状态
 	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1}
}
void KeyDriver();//按键驱动函数
void KeyAction(unsigned char keycode);//按键动作函数，根据键码执行相应的操作
