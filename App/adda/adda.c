#include<STC12C5A.h>
#include"adda.h"

unsigned char xdata ad_memo[MEMO_LEN]; 	// A/D�ɼ��źŴ洢��
unsigned char
	ad_result = 128, // A/D���βɼ��źŽ�� 
	memo_p = 0,	   	 // A/D�ɼ��洢��ָ��
void ad_save() // A/D����ֵ�洢
{
    ad_memo[memo_p++] = ad_result;
    if (memo_p >= MEMO_LEN)
    {
        memo_p = 0;
    }
}

