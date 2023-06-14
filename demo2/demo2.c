#include <reg51.h>                              // special function register declarations
#include<absacc.h>

void init_timer0();
void init_special_interrupts();

unsigned char        dspbuf[4]={0x41,0x3b,0x6b,0x4d},sel=0;
unsigned int         clocktime=0,adcount=0;

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
    b=b>>1;  //b=b&0x7fff;
    }
  D_RCLK=0;
  D_RCLK=1;
  D_RCLK=0;
  }
//--------------------------------------
void timer_isr() interrupt 1           //定时器0中断处理
  {
  EA=0;

  adcount++;
  if(adcount==20)                      // 1/4*20 = 5ms
    {
    adcount=0;
    dsptask();
    clocktime++;
    }
  EA=1;
  }
//---------------------------------------------------------------------------------------
fdisp(int n)
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
  dspbuf[0]=dspbuf[1];
  dspbuf[1]=dspbuf[2];
  dspbuf[2]=dspbuf[3];
  dspbuf[3]=c;
  }
//-------------------------------
void main(void)                    // 主函数
  {
  unsigned char a=0;
  init_timer0();                  //初始化定时器0
  init_special_interrupts();      //设置中断
  for(;;)
    { 
    if(clocktime>=100)                //5ms x 200 = 1s
       {clocktime=0;
        fdisp(a); 
        a++;
        if(a==10)  a=0;
		}
	}
  }
//-----------------------------------End------------------------------------------------

