#define DA_CH1 XBYTE[0x4000] // DA��ʾͨ��CH1
#define DA_CH2 XBYTE[0x2000] // DA��ʾͨ��CH2
#define MEMO_LEN 	8000 	 // �ط���ʾ�洢����(BYTE)
#define AD_LEN 250
#define SIN 1				 // ���Ҳ�
#define TRI 2				 // ���ǲ�
#define REC	3				 // ����
#define SAW 4				 // ��ݲ�
#define PI 3.1415926535 	 // ��ѧ������
//==========��������==============
extern unsigned char xdata 
	ad_memo[]; // A/D�ɼ��źŴ洢��
extern unsigned char
	ad_result,	// A/D���βɼ��źŽ�� 
	memo_p, 	// A/D�ɼ��洢��ָ��
	
//==========��������==============
void ad_measure(); // A/D����
void ad_save();    // A/D����ֵ�洢

