#include <reg51.h>                              // special function register declarations
#include<absacc.h>

#define  DAC_1    XBYTE[0x2000]
#define  DAC_2    XBYTE[0x4000]

#define  ADC_0    XBYTE[0x6000]
#define  ADC_1    XBYTE[0x6001]
#define  ADC_2    XBYTE[0x6002]
#define  ADC_3    XBYTE[0x6003]
#define  ADC_4    XBYTE[0x6004]
#define  ADC_5    XBYTE[0x6005]
#define  ADC_6    XBYTE[0x6006]
#define  ADC_7    XBYTE[0x6007]

#define  LEN        10

void init_timer0();
void init_special_interrupts();

unsigned char        buff[LEN];
unsigned char        p_read=0,p_write=0,ad_temp;
unsigned char        dspbuf[4]={0xfe,0xfe,0xfe,0xfe},sel=0;
unsigned int         clocktime=0,adcount=0;

sbit D_SER     = P1^0;
sbit D_SRCLK   = P1^1;
sbit D_RCLK    = P1^2;
sbit KEY1      = P3^4;
sbit KEY2      = P3^5;
//---------------------------------------------------------------------------------------
void init_timer0(void)              //  定时器0初始
  {
  TMOD &= 0XF0;
  TMOD |= 0X02;
  TL0 = 0X06;
  TH0 = 0X06;
  TR0 = 1;
  ET0 = 1;
  }
//---------------------------------------------------------------------------------------
void init_special_interrupts(void)   //  中断设置
  {
  EX0 = 0;
  ET0 = 1;
  EX1 = 0;
  ET1 = 0;
  ES  = 1;
  EA  = 1;
  PT0 = 0;
  PX1 = 1;
  PS  = 0;
  }
//---------------------------------------------------------------------------------------
void dsptask()
  {
  unsigned char i;
  unsigned char a,b;

  switch(sel)
   {
   case 0:  a=0x01;  break;
   case 1:  a=0x02;  break;
   case 2:  a=0x04;  break;
   default: a=0x08;
   }
 for(b=0x80,i=0;i<8;i++)
    {
    if(a&b)  D_SER=1;
    else     D_SER=0;
    D_SRCLK=0;
    D_SRCLK=1;
    D_SRCLK=0;
    b=b>>1;  b=b&0x7f;
    }
  a=dspbuf[sel];
  sel++;
  if(sel>=4) sel=0;
  for(b=0x80,i=0;i<8;i++)
    {
    if(a&b)  D_SER=1;
    else     D_SER=0;
    D_SRCLK=0;
    D_SRCLK=1;
    D_SRCLK=0;
    b=b>>1;  b=b&0x7fff;
    }
  D_RCLK=0;
  D_RCLK=1;
  D_RCLK=0;
  }
//---------------------------------------------------------------------------------------
void timer_isr() interrupt 1           //定时器0中断处理
  {
  unsigned char  t;
  EA=0;
  adcount++;
  if(adcount==20)                      // 1/4*20 = 5ms
    {
    buff[p_write]=ADC_0;               // A/D读入
    ADC_0=0;                           // restart A/D
    p_write++;
    if(p_write==LEN)  p_write=0;
    dsptask();
    clocktime++;
    adcount=0;
    }
  EA=1;
  }
//---------------------------------------------------------------------------------------
fdisp(unsigned char n,unsigned char m)
  {
  char  c;
   switch(n)
    {
    case 0:  c=0x11;  break;
    case 1:  c=0x7d;  break;
    case 2:  c=0x23;  break;
    case 3:  c=0x29;  break;
    case 4:  c=0x4d;  break;
    case 5:  c=0x89;  break;
    case 6:  c=0x81;  break;
    case 7:  c=0x3d;  break;
    case 8:  c=0x01;  break;
    default: c=0x09; 
    }
   dspbuf[m]=c;
  }
//---------------------------------------------------------------------------------------
void main(void)                    // 主函数
  {
unsigned char t;
  init_timer0();                  //初始化定时器0
  init_special_interrupts();      //设置中断
  for(;;)
    { 
	if(p_read!=p_write)
   	   {
        DAC_1=buff[p_read];       // D/A输出
	    p_read++;
	    if(p_read==LEN)  p_read=0;
        }
	}
  }
//----------------------------------End---------------------------------------------------
