//�����������
sbit ROW1 = P1^7;
sbit ROW2 = P1^6;
sbit ROW3 = P1^5;
sbit ROW4 = P1^4;
sbit COL1 = P1^3;
sbit COL2 = P1^2;
sbit COL3 = P1^1;
sbit COL4 = P1^0;
static unsigned char code KeyCodeMap[4][4]={//���󰴼���ŵ���׼���̼����ӳ���
	{},
	{},
	{},
	{}
};
static unsigned char KeySta[4][4]={//ȫ��������ǰ��״̬
 	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1}
}
void KeyDriver();//������������
void KeyAction(unsigned char keycode);//�����������������ݼ���ִ����Ӧ�Ĳ���
