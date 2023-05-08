#include <STC12C5A.h>   // special function register declarations
#include <absacc.h>
#include <intrins.h> //void _nop_(void) 
#include <math.h>

// #define DA_CH1 XBYTE[0x5fff]
// #define DA_CH2 XBYTE[0x3fff]
#define DA_CH1 XBYTE[0x4000]
#define DA_CH2 XBYTE[0x2000]
#define DAC_1 XBYTE[0x2000]

#define AD_LEN 250
#define DA_LEN 7000
#define GEN_LEN 200

void init_timer0();
void init_special_interrupts();
void init_ad();
void init_da_generator();
void fdisp_float(float n);
void model_select();
void model2();

unsigned char dspbuf[4] = {0xef, 0xef, 0xef, 0xef}, sel = 0, key_sta = 0, intr_count = 0;
unsigned int key_num = 0, model[2] = {0, 0};
unsigned int dspcount = 0, adcount = 0;
unsigned int da_p = 0, daplayback_p = 0, clearflag = 0, dag_p = 0, wave_flag = 0, wave_amp = 0, wave_fre = 0;
unsigned int adresult = 0;
int ampl = 0, amp = 0, fre = 0, amp_up = 128, amp_low = 128, ad_p = 0, fre_up = 0, fre_low = 0;
float adamp = 0.0, adfre = 0.0, fre_count = 0.0;
unsigned char xdata da_data[DA_LEN];

unsigned char xdata sine[GEN_LEN] = {
    128, 128, 129, 129, 130, 130, 131, 131, 132, 132, 133, 133, 134, 134, 134, 135, 135, 136, 136, 136,
    137, 137, 138, 138, 138, 139, 139, 139, 140, 140, 140, 140, 141, 141, 141, 141, 142, 142, 142, 142,
    142, 142, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 142,
    142, 142, 142, 142, 142, 141, 141, 141, 141, 140, 140, 140, 140, 139, 139, 139, 138, 138, 138, 137,
    137, 136, 136, 136, 135, 135, 134, 134, 134, 133, 133, 132, 132, 131, 131, 130, 130, 129, 129, 128,
    128, 128, 127, 127, 126, 126, 125, 125, 124, 124, 123, 123, 122, 122, 122, 121, 121, 120, 120, 120,
    119, 119, 118, 118, 118, 117, 117, 117, 116, 116, 116, 116, 115, 115, 115, 115, 114, 114, 114, 114,
    114, 114, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 114,
    114, 114, 114, 114, 114, 115, 115, 115, 115, 116, 116, 116, 116, 117, 117, 117, 118, 118, 118, 119,
    119, 120, 120, 120, 121, 121, 122, 122, 122, 123, 123, 124, 124, 125, 125, 126, 126, 127, 127, 128};
unsigned char xdata saw[GEN_LEN] = {
    115, 115, 115, 115, 116, 116, 116, 116, 116, 116, 116, 116, 117, 117, 117, 117, 117, 117, 117, 117,
    118, 118, 118, 118, 118, 118, 118, 119, 119, 119, 119, 119, 119, 119, 119, 120, 120, 120, 120, 120,
    120, 120, 120, 121, 121, 121, 121, 121, 121, 121, 122, 122, 122, 122, 122, 122, 122, 122, 123, 123,
    123, 123, 123, 123, 123, 123, 124, 124, 124, 124, 124, 124, 124, 124, 125, 125, 125, 125, 125, 125,
    125, 126, 126, 126, 126, 126, 126, 126, 126, 127, 127, 127, 127, 127, 127, 127, 127, 128, 128, 128,
    128, 128, 128, 128, 129, 129, 129, 129, 129, 129, 129, 129, 130, 130, 130, 130, 130, 130, 130, 130,
    131, 131, 131, 131, 131, 131, 131, 132, 132, 132, 132, 132, 132, 132, 132, 133, 133, 133, 133, 133,
    133, 133, 133, 134, 134, 134, 134, 134, 134, 134, 135, 135, 135, 135, 135, 135, 135, 135, 136, 136,
    136, 136, 136, 136, 136, 136, 137, 137, 137, 137, 137, 137, 137, 137, 138, 138, 138, 138, 138, 138,
    138, 139, 139, 139, 139, 139, 139, 139, 139, 140, 140, 140, 140, 140, 140, 140, 140, 141, 141, 141};
unsigned char xdata rec[GEN_LEN] = {
    115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 
    115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 
    115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 
    115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 141, 141, 141, 
    141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
    141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
    141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
    141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
    141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 115, 115, 115, 
    115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115};
unsigned char xdata tri[GEN_LEN] = {
    115,115,115,115,116,116,116,116,116,116,117,117,117,117,117,117,117,118,118,118,
    118,118,118,118,119,119,119,119,119,119,120,120,120,120,120,120,120,121,121,121,
    121,121,121,121,122,122,122,122,122,122,123,123,123,123,123,123,123,124,124,124,
    124,124,124,124,125,125,125,125,125,125,126,126,126,126,126,126,126,127,127,127,
    127,127,127,127,128,128,128,128,128,128,129,129,129,129,129,129,129,130,130,130,
    130,130,130,130,129,129,129,129,129,129,129,128,128,128,128,128,128,127,127,127,
    127,127,127,127,126,126,126,126,126,126,126,125,125,125,125,125,125,124,124,124,
    124,124,124,124,123,123,123,123,123,123,123,122,122,122,122,122,122,121,121,121,
    121,121,121,121,120,120,120,120,120,120,120,119,119,119,119,119,119,118,118,118,
    118,118,118,118,117,117,117,117,117,117,117,116,116,116,116,116,116,115,115,115};
sbit D_SER = P1 ^ 0;
sbit D_SRCLK = P1 ^ 1;
sbit D_RCLK = P1 ^ 2;
sbit KEY1 = P3 ^ 4;
sbit KEY2 = P3 ^ 5;
sbit Y = P1 ^ 4;
/*sfr P1ASF = 0x9D;
sfr ADC_CONTR = 0xBC;
sfr ADC_RES = 0xBD;
sfr AUXR1 = 0xA2;
sfr CLK_DIV = 0x97;	 */

//
void init_timer0(void) // 定时器初始化
{
    TMOD &= 0XF0;
    TMOD |= 0X02;
    TL0 = 0X06;
    TH0 = 0X06;
    TR0 = 1;
    ET0 = 1;
}
//
void init_ad() // ad 寄存器初始化
{
    P1ASF = 0x08;
    ADC_CONTR = 0xE3;
    AUXR1 &= 0xfb;
    CLK_DIV = 0x01;
}
//
void init_special_interrupts(void) // 中断初始化
{
    EX0 = 0;
    ET0 = 1;
    EX1 = 0;
    ET1 = 0;
    ES = 1;
    EA = 1;
    PT0 = 0;
    PX1 = 1;
    PS = 0;
}
//
void dsptask() // 数码管扫描
{
    unsigned char i;
    unsigned char a, b;

    switch (sel)
    {

    case 0:
        a = 0x01;
        break;
    case 1:
        a = 0x02;
        break;
    case 2:
        a = 0x04;
        break;
    case 3:
        a = 0x08;
        break;
    default:
        a = 0x10;
        break;
    }
    for (b = 0x80, i = 0; i < 8; i++)
    {
        if (a & b)
            D_SER = 1;
        else
            D_SER = 0;
        D_SRCLK = 0;
        D_SRCLK = 1;
        D_SRCLK = 0;
        b = b >> 1;
        b = b & 0x7f;
    }
    if (sel < 4)
    {
        a = dspbuf[sel];
        key_num = sel;
    }
    else
    {
        switch (wave_flag)
        {

        case 1:
            a = 0x07;
            break;
        case 2:
            a = 0x0B;
            break;
        case 3:
            a = 0x0D;
            break;
        case 4:
            a = 0x0E;
            break;
        default:
            a = 0x10;
            break;
        }
    }
    sel++;
    if (sel >= 5)
        sel = 0;
    for (b = 0x80, i = 0; i < 8; i++)
    {
        if (a & b)
            D_SER = 1;
        else
            D_SER = 0;
        D_SRCLK = 0;
        D_SRCLK = 1;
        D_SRCLK = 0;
        b = b >> 1;
        b = b & 0x7fff;
    }
    D_RCLK = 0;
    D_RCLK = 1;
    D_RCLK = 0;
}
//
void key_service() // 按键扫描
{
    if (key_sta & 0x01)
        return; // key_sta.0=1
	if(KEY2)
    {
        model[0] = key_num + 1;
        model[1] = 0;
        key_sta = key_sta | 0x01; // key_sta.0=1
    }
    else if(KEY1)
    {
        model[1] = key_num + 1;
        key_sta = key_sta | 0x01; // key_sta.0=1
    }

}

//
void fdisp_num(int float_flag, int i, int n) // 数码管段码表
{
    char c;
    switch (n)
    {
	    case 0:c = 0x11;break;
	    case 1:c = 0x7d;break;
	    case 2:c = 0x23;break;
	    case 3:c = 0x29;break;
	    case 4:c = 0x4d;break;
	    case 5:c = 0x89;break;
	    case 6:c = 0x81;break;
	    case 7:c = 0x3d;break;
	    case 8:c = 0x01;break;
	    case 9:c = 0x09;break;
	    default:c = 0xff;
    }
    if (float_flag == 1)
    {
        c = c & 0xFE;
    }
    dspbuf[i] = c;
}

//
void ad_get() // ad 采样
{
    int i = 0;
    Y = !Y; // P1.4 翻转检测采样频率
    ADC_RES = 0;
    ADC_CONTR |= 0x08;
    for (i = 0; i < 4; i++)
    {
        _nop_();
    }
    while (ADC_CONTR & 0x10 == 0);
    ADC_CONTR &= 0xE7;
    adresult = ADC_RES; // 存储采样值
}
//
void ad_save() // ad 采样值存储
{
    da_data[da_p] = adresult;
    da_p++;
    if (da_p >= DA_LEN)
    {
        da_p = 0;
    }
}
//
void da_display(int da_type) // da 转换输出
{
    DA_CH1 = adresult; // 实时显示
    if (da_type)       // 波形回放
    {
        DA_CH2 = da_data[daplayback_p];
        daplayback_p++;
        if (daplayback_p >= DA_LEN)
        {
            daplayback_p = 0;
        }
    }
}
//1
void da_generator(int wave, int amp, int fre) // da 波形发生
{
    int da_result;
    wave_flag = wave;
    wave_amp = amp;
    wave_fre = fre;
    switch (wave_flag)

    {
    case 1: // 正弦波
        da_result = sine[dag_p];
        break;
    case 2: // 三角波
        da_result = tri[dag_p];
        break;
    case 3: // 方波
        da_result = rec[dag_p];
        break;
    case 4: // 锯齿波
        da_result = saw[dag_p];
        break;
    default:
        break;
    }                  
	if (fre!=0){dag_p=dag_p+fre+2;}else{dag_p=dag_p+2;}		// 调频
	if (amp!=0){da_result=(int)((da_result-128)*amp/2+128);}// 调幅
    DAC_1 = da_result; // 输出波形
    if (dag_p >= GEN_LEN){dag_p = 0;}
}
//
void fdisp_float(float n) // 数码管显示
{
    int i, order, n_int;
    unsigned int s;
    order = (int)log10(n);
    if (order > 0)
    {
        i = 3;
        n_int = (int)(n);
        for (i = 3; i >= 0; i--)
        {
            s = n_int % 10;
            fdisp_num(0, i, s);
            n_int = n_int / 10;
        }
    }
    else
    {
        if (n < 0)
        {
            dspbuf[0] = 0xef;
            n = -1 * n;
        }
        else
        {
            dspbuf[0] = 0xff;
        }
        for (i = 1; i < 4; i++)
        {
            s = (int)n;
            if (i == 1)
            {
                fdisp_num(1, i, s);
            }

            else
            {
                fdisp_num(0, i, s);
            }
            n = (n - s) * 10;
        }
    }
}
//
void amp_measure() // 幅值测量
{
    int i = 0;
    amp = adresult;
    ad_p++;
    if (amp > amp_up)
    {
        amp_up = amp;
    }
    if (amp < amp_low)
    {
        amp_low = amp;
    }
    if (ad_p >= AD_LEN)
    {
        adamp = (amp_up * 5.0 - amp_low * 5.0) / 256;
        ad_p = 0;
        amp_up = amp_low = 128;
    }

    fdisp_float(adamp);
}

//
void fre_measure() // 频率测量
{
    int i = 0;
    amp = adresult;
    ad_p++;
    if (amp > 128 && ampl <= 128)
    {
        fre_up = ad_p;
        if (fre_low != 0)
        {
            fre = fre + fabs(fre_low - fre_up);
            fre_count++;
        }
        fre_low = fre_up;
    }

    if (ad_p >= AD_LEN)
    {
        adfre = 500.0 / (fre * 1.0 / fre_count);
        // adfre+=adfre*0.05;
        fre = 0;
        ad_p = 0;
        fre_up = fre_low = 0;
        fre_count = 0;
        amp = ampl = 129;
    }
    ampl = amp;
    fdisp_float(adfre);
}
//
void clear() // 清零
{
    int ram_p;
    EA = 0;
    for (ram_p = 0; ram_p < da_p; ram_p++)
    {
        XBYTE[ram_p] = 0;
    }
    fdisp_float(0000);
    da_p = 0;
    clearflag = 0;
    EA = 1;
}

//
void model2() // 回放&测量模式
{
    switch (model[1])
    {
	    case 2:amp_measure();break;
	    case 3:fre_measure();break;
	    case 4:clearflag = 1;break;
	    default:break;
    }
}
//
void model_select() // 模式选择
{

    switch (model[0])
    {

    case 1: // model1 波形发生
        adcount++;
        if (adcount == 1)
        {
            da_display(0);
            adcount = 0;
        }
        wave_flag = model[1];
        wave_amp = 0;
        wave_fre = 0;

        break;
    case 2:// model2 回放测量
        model2();
        break;
    case 3:// 调频
        adcount++;
        if (adcount == 1)
        {
            da_display(0);
            adcount = 0;
        }
        wave_amp = model[1];
        break;
    case 4: // 调幅
        adcount++;
        if (adcount == 1)
        {
            da_display(0);
            adcount = 0;
        }
        wave_fre = model[1];
        break;
    default:break;
    }
}
//
void timer_isr() interrupt 1 // 定时器中断处理
{

    ad_get();
    if (model[0] != 2)
    {
        ad_save();
        da_generator(wave_flag, wave_amp, wave_fre);
        da_display(0);
    }
    else
    {
        da_display(1);
    }
}
void main(void)	//主函数
{

    init_timer0();             // 定时器初始化
    init_special_interrupts(); // 中断设置初始化
	init_ad();		//ad 寄存器初始化
    for (;;)
    {
        dspcount++;
        if (dspcount == 3) // 数码管与按键
        {
            dspcount = 0;
            dsptask();
            key_service();
        }

        model_select();
        if (key_sta & 0x01) // key_sta.0==1 数码管显示
        {
            fdisp_num(0, 0, 10);
            fdisp_num(0, 1, 10);
            fdisp_num(0, 2, model[0]);
            fdisp_num(0, 3, model[1]);
            key_sta = key_sta & 0xfe; // key_sta.0=0
        }
        if (clearflag)
        {
            clear();
        }
    }
}
