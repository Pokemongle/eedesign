#include <reg51.h>                              // special function register declarations
#include<absacc.h>

void init_timer0();
void init_special_interrupts();

unsigned char        dspbuf[4]={0xef,0xef,0xef,0xef},sel=0,key_sta=0,key_num;
unsigned int         clocktime=0,adcount=0;

sbit D_SER     = P1^0;
sbit D_SRCLK   = P1^1;
sbit D_RCLK    = P1^2;
sbit KEY1      = P3^5;
sbit KEY2      = P3^4;
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
  key_num=sel;
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
//----------------------------
void key_service()
  {
  if(key_sta&0x01) return;             // key_sta.0=1
   if(KEY1)
  	{
    key_num=key_num+1;                    
	key_sta=key_sta|0x01;              // 置key_sta.0=1
	}
   else if(KEY2)
    	  {
	      key_num=key_num+5;   
          key_sta=key_sta|0x01;        // 置key_sta.0=1
	      }
		  
  }
//----------------------------
void timer_isr() interrupt 1           //定时器0中断处理
  {
  EA=0;
  adcount++;
  if(adcount==20)                      // 1/4*20 = 5ms
    {
	    adcount=0;
	    dsptask();
	    key_service();
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
  dspbuf[0]=0xff;
  dspbuf[1]=0xff;
  dspbuf[2]=0xff;
  dspbuf[3]=c;
  }
//---------------------------
void main(void)                   // 主函数
  {
  init_timer0();                  //初始化定时器0
  init_special_interrupts();      //设置中断
  for(;;)
    { 
    if(key_sta&0x01)              // key_sta.0==1?
         {             
         fdisp(key_num); 
         key_sta=key_sta&0xfe;           // 置key_sta.0=0
         }
	}
  }
//-----------------------------------End------------------------------------------------

