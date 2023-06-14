#include <reg51.h>                              // special function register declarations
#include<absacc.h>

void init_timer0();
void init_special_interrupts();

//unsigned char  xdata buff[LEN];

unsigned char       dspbuf[4]={0x7d,0x23,0x29,0x4d},sel=0;
unsigned int        adcount=0;

sbit D_SER     = P1^0;
sbit D_SRCLK   = P1^1;
sbit D_RCLK    = P1^2;
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
    b=b>>1;  //b=b&0x7f;
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
//-----------------------------------
void timer_isr() interrupt 1           //定时器0中断处理
  {
  EA=0;
  adcount++;
  if(adcount==20)                      // 1/4*20 = 5ms
    {
    adcount=0;
    dsptask();
    }
  EA=1;
  }
//---------------------------------------------------------------------------------------
void main(void)                    // 主函数
  {
  init_timer0();                  //初始化定时器0
  init_special_interrupts();      //设置中断
  for(;;);
  }
//-----------------------------------End------------------------------------------------

