//==function==
unsigned char ad_get(); // A/D���������Σ�
void ad_save(); 	  	// A/D����ֵ�洢
void da_display();		// D/A��ʾ
//============
#define MEMO_LEN 8000		 // �ط���ʾ�洢����(BYTE)
#define SAVE_GAP 300		 // �洢���
#define DA_CH1 XBYTE[0x4000] // DA��ʾͨ��CH1
#define DA_CH2 XBYTE[0x2000] // DA��ʾͨ��CH2
#define PI 3.1415926535
#define AD_LEN 250
//==parameter==
extern unsigned char
	ad_get_count,
	ad_save_count,
	ad_display_count,
	ad_result,   // A/D���βɼ��źŽ��
	ad_result_display,
	da_result; 	 // �źŷ��������β����������ź�	
extern unsigned int 
    review_p,    // D/A�ط���ʾָ��
	memo_p;	   // A/D�ɼ��洢��ָ��
//extern unsigned char xdata
    //ad_memo[]; 	// A/D�ɼ��źŴ洢��
//============
