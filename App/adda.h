//==function==
unsigned char ad_get(); // A/D���������Σ�
void ad_measure();	// Ƶ�ʷ�ֵ���μ��
void ad_save(); 	  	// A/D����ֵ�洢
void da_display();
void da_generator();  	// �źŷ�����
void da_process();	  	// �������ʾ��D/A���ֵ
void dspbuf_measure();	// �������õ���Ƶ�ʺͷ�ֵ��ʾ���������
//============
#define MEMO_LEN 8000
#define DA_CH1 XBYTE[0x4000] // DA��ʾͨ��CH1
#define DA_CH2 XBYTE[0x2000] // DA��ʾͨ��CH2
#define PI 3.1415926535
#define AD_LEN 250
//==parameter==
extern unsigned char
	ad_max,  // �����źŵ�ǰ���ֵ
	ad_min,  // �����źŵ�ǰ��Сֵ
	ad_result, // A/D���βɼ��źŽ��
	amp_p1,  	 // Ƶ�ʲ���ָ��1
	amp_p2,  	 // Ƶ�ʲ���ָ��2
	da_count,  	 // �źŷ�������ʱ����
	da_result, 	 // �źŷ��������β����������ź�
	fre_count,	 // ��ֵ��������
	fre_p1,	   	 // Ƶ�ʲ���ָ��1 
	fre_p2,	   	 // Ƶ�ʲ���ָ��2
	fre_sum,   	 // Ƶ�ʲ����ۼ�
	rec_count, 	 // �����źŷ�������
	rec_value,  // �����ź�
	review_p,  // D/A�ط���ʾָ��
	saw_value,  // ��ݲ��ź�
	time_count,  // ����ʱ�����
	tri_value,  // ���ǲ��ź�
	tri_flag;    // ���ǲ���ֵ�����ݼ���־

extern unsigned int 
	angle,	   // ���Ҳ��Ƕȣ��Ƕ�ֵ��[0,360]
	memo_p;	   // A/D�ɼ��洢��ָ��

extern float
	sinAngle,  // ���Ҳ��Ƕȣ������ƣ�[-1,1]
	fre_es,	  // ������Ƶ��
	amp_es;	  // ��ֵ����ֵ�������ƣ�[-5,5]
extern unsigned char xdata
    ad_memo[]; 	// A/D�ɼ��źŴ洢��
//============
