//==�궨��==
#define SIN 1// ���Ҳ�
#define TRI 2// ���ǲ�
#define REC	3// ����
#define SAW 4// ��ݲ�
//==========
sbit KEY1 = P3 ^ 5;
sbit KEY2 = P3 ^ 4;
//==��������==
void key_service(); // ����ɨ��
void mode_select(); // ģʽѡ��
//============
//==��������==
extern unsigned char
	amp_modi,  // ������־
	da_index,  // ��������
	flag_amp,  // ��ʾ��ֵ��־
	flag_fre,  // ��ʾƵ�ʱ�־ 
	fre_modi,  // ��Ƶ��־
	key_col,   // ����ɨ�赱ǰ��
	key_num,   // �������
	key_sta,
	mode,	   // ģʽѡ��
	wave_sel;  // �źŷ���������ѡ��
extern unsigned int
	gen_count; // �źŷ�����Ƶ�ʿ���

//============