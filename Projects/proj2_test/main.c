#include <STC12C5A.h> // special function register declarations
#include <absacc.h>
#include <intrins.h> //void _nop_(void)
#include <math.h>
#define DA_CH2 XBYTE[0x2000]
#define DA_LEN 8000
void init_timer0();
void init_special_interrupts();
void init_ad();
void fdisp_float(float n);
void model_select();
unsigned char dspbuf[4] = {0xef, 0xef, 0xef, 0xef}, sel = 0, key_sta = 0, intr_count = 0;
unsigned int key_num = 0, model = 0;
unsigned int dspcount = 0;
unsigned int da_p = 0, daplayback_p = 0, clearflag = 0, dag_p = 0;
unsigned int adresult = 0;
int ampl = 0;
amp = 0, ad_p = 0;
int ii = 0, disp_flag = 0, save_flag = 0, save_count = 0, disp_count = 0;
float adamp = 0.0;
unsigned char xdata da_data[DA_LEN];
sbit D_SER = P1^0;
sbit D_SRCLK = P1^1;
sbit D_RCLK = P1^2;
sbit KEY = P3 ^ 5;
sbit Y = P1 ^ 4;

//---------------------------------------------------------------------------------------
void init_timer0(void) // 定时器初始化
{
    TMOD &= 0XF0;
    TMOD |= 0X02;
    TL0 = 0X06;
    TH0 = 0X06;
    TR0 = 1;
    ET0 = 1;
}
//---------------------------------------------------------------------------------------
void init_ad() // ad 寄存器初始化
{
    P1ASF = 0x08;
    ADC_CONTR = 0xE3;
    AUXR1 &= 0xfb;
    CLK_DIV = 0x01;
}
//---------------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------------
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
    a = dspbuf[sel];
    key_num = sel;
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
//---------------------------------------------------------------------------------------
void key_service() // 按键扫描
{
    if (key_sta & 0x01)
        return; // key_sta.0=1
    if (KEY)
    {
        model = key_num + 1;
        key_sta = key_sta | 0x01; // key_sta.0=1
    }
}
//---------------------------------------------------------------------------------------
void fdisp_num(int float_flag, int i, int n) // 数码管段码表
{
    char c;
    switch (n)
    {
    case 0:
        c = 0x11;
        break;
    case 1:
        c = 0x7d;
        break;
    case 2:
        c = 0x23;
        break;
    case 3:
        c = 0x29;
        break;
    case 4:
        c = 0x4d;
        break;
    case 5:
        c = 0x89;
        break;
    case 6:
        c = 0x81;
        break;
    case 7:
        c = 0x3d;
        break;
    case 8:
        c = 0x01;
        break;
    case 9:
        c = 0x09;
        break;
    default:
        c = 0xff;
    }
    if (float_flag == 1)
    {
        c = c & 0xFE;
    }
    dspbuf[i] = c;
}
//---------------------------------------------------------------------------------------
void ad_get() // ad 采样
{
    int i = 0;
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
//--------------------------------------------------------------------------------------- 
void ad_save() //ad 采样值存储
{
    Y = !Y;
    da_data[da_p] = adresult * 10;
    da_p++;
    if (da_p >= DA_LEN)
    {
        da_p = 0;
    }
}
//--------------------------------------------------------------------------------------- 
void da_display(int disp_flag) //da 转换输出
{
    if (disp_flag == 1)
    {
        DA_CH2 = da_data[daplayback_p];
        disp_count = disp_count + 10;
        if (disp_count == 150)
        {
            disp_count = 0;
            daplayback_p++;
        }
        if (daplayback_p >= DA_LEN)
        {
            daplayback_p = 0;
        }
    }
}
//--------------------------------------------------------------------------------------- 
void fdisp_float(float n) //数码管显示
{
    int i, order, n_int;
    unsigned int s;
    order = (int)log10(n);
    if (order > 0)
    {
        i = 3;
        n_int = (int)(n * 10);
        for (i = 3; i >= 0; i--)
        {
            s = n_int % 10;
            if (i == 2)
            {
                fdisp_num(1, i, s);
            }
            else
            {
                fdisp_num(0, i, s);
            }
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
//--------------------------------------------------------------------------------------- 
void amp_measure() //幅值测量
{
    adamp = (adresult - 127) * 5.0 / 256;
    adamp = -36.37 * adamp + 41.55;
    ii++;
    if (ii == 3)
    {
        ii = 0;
        fdisp_float(adamp);
    }
}
//--------------------------------------------------------------------------------------- 
void clear() //清零
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
//--------------------------------------------------------------------------------------- 
void model_select() //模式选择
{
    switch (model)
    {
    case 1: // 显示温度
        amp_measure();
        save_flag = 1;
        break;
    case 2:
        amp_measure(); // 显示温度和回放测量
        disp_flag = 1;
        save_flag = 0;
        break;
    case 3:
        disp_flag = 0;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------------- 
void timer_isr() interrupt 1 //定时器中断处理
{
    ad_get();
    save_count++;
    if ((save_count == 150) && (save_flag == 1))
    {
        save_count = 0;
        ad_save();
    }
    da_display(disp_flag);
}
//--------------------------------------------------------------------------------------- 
void main(void) //主函数
{
    init_timer0();             // 定时器初始化
    init_special_interrupts(); // 中断设置初始化
    init_ad();                 // ad 寄存器初始化
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
            fdisp_num(0, 2, 10);
            fdisp_num(0, 3, model);
            key_sta = key_sta & 0xfe; // key_sta.0=0
        }
        if (clearflag)
        {
            clear();
        }
    }
}