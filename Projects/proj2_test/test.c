//==========头文件=========
#include <STC12C5A.h>
#include <absacc.h>
#include <intrins.h>
#include <math.h>
//=========================

//===============宏定义======================
#define DA_CH1 XBYTE[0x4000] // DA显示通道CH1
#define DA_CH2 XBYTE[0x2000] // DA显示通道CH2
#define MEMO_LEN 	8000 // 回放显示存储长度(BYTE)
#define WIN_SIZE	8
#define AD_LEN 250
#define SIN 1// 正弦波
#define TRI 2// 三角波
#define REC	3// 方波
#define SAW 4// 锯齿波
#define PI 3.1415926535		 // 数学常数Π
//===========================================

//============管脚定义================
//---------数码管连接芯片74CH565
sbit D_SER = P1 ^ 0;
sbit D_SRCLK = P1 ^ 1;
sbit D_RCLK = P1 ^ 2;
//---------键盘行扫描
sbit KEY1 = P3 ^ 5;
sbit KEY2 = P3 ^ 4;
//===================================

//============函数声明===============
void ad_save();                 // A/D采样值存储
void ad_measure();				// A/D测量
void amp_measure();				// 幅值检测
void da_display();              // D/A显示
void da_generator();			// 信号发生器
void da_process();				// 处理[0,255]内的待显示D/A输出值
void dsptask();                 // 数码管显示函数
void dspbuf_measure();			// 模式3，数码管显示缓存区为当前测量值
void fdisp(unsigned char n,unsigned char m);// 单个数码管显示				
//n为数码管显示的数字 m为数码管位选	
unsigned char GetADC();         // A/D采样（单次）
void init_ad();                 // 内部A/D寄存器初始化
void init_timer0();             // 定时器0初始化
void init_special_interrupts(); // 中断初始化
void key_service();				// 按键扫描
void mode_select();			    // 模式选择，修改对应数码管缓存区
//void memo_increment();			// 存储读取位置更改 
//--模式1：波形实时显示模式
//---------CH1: 采样实时输出，同时存储输入
//---------CH2: 信号发生器	正弦波|三角波|方波|锯齿波						
//--模式2：波形回放显示模式															
//---------CH1: 采样实时输出
//---------CH2：输出模式1中存储的信号的波形																
//--模式3：测量模式
//--模式4：调幅、调频模式
//---------CH1: 采样实时输出
//---------CH2: 信号发生器	正弦波|三角波|方波|锯齿波

//===============全局变量声明===========================
unsigned char 
dspbuf[4] = {	 // 数码管显示缓存
    0x11, 0x11, 0x11, 0x11
}, 
dspchar[10] = {	 // 数码管字符表0-9
	0x11,0x7d,0x23,0x29,
	0x4d,0x89,0x81,0x3d,
	0x01,0x09	
},
    sel = 0,            // 数码管位选
    key_sta = 0;        // 按键状态
unsigned char xdata
    ad_memo[MEMO_LEN]; 	// A/D采集信号存储区
unsigned char
	amp_modi = 0,  // 调幅标志
	fre_modi = 0,  // 调频标志
	ad_max = 128,  // 测量信号当前最大值
	ad_min = 128,  // 测量信号当前最小值
	ad_count = 0,  // A/D采样计数
    ad_result = 128, // A/D单次采集信号结果
	dag_p = 0,	   // 信号发生器指针
	da_result = 0, // 信号发生器单次产生的数字信号
	flag_amp = 0,  // 数码管显示幅值
	flag_fre = 0,  // 数码管显示频率标志
	key_col = 0,   // 按键扫描当前列
	key_num = 0,   // 按键序号
	memo_p = 0,	   // A/D采集存储区指针
	mode = 0,	   // 模式选择
	review_p = 0,  // D/A回放显示指针
	rec_count = 0, // 方波信号发生计数
	saw_value = 64,// 锯齿波信号
	rec_value = 64,// 方波信号
	tri_value = 64,// 三角波信号
	tri_flag = 1,  // 三角波幅值递增递减标志
	wave_sel = 0,  // 信号发生器波形选择
	i = 0,		   // 按键扫描位选
	fre_p1 = 1,	   // 频率测量指针1 
	fre_p2 = 1,	   // 频率测量指针2
	amp_p1 = 129,  // 频率测量指针
	amp_p2 = 129,  // 频率测量指针
	key_p = 0;	   // 按键扫描缓冲区指针
unsigned int
	fre_count = 0, // 幅值测量计数
	fre_sum = 0,   // 频率测量累加
	time_count = 0,// 测量时间计数
	angle = 0,	   // 正弦波角度，角度值，[0,360]
	da_count = 0,  // 信号发生器定时计数
	da_index = 2,// 调幅参数
    clocktime = 0, //
	gen_count = 4; // 信号发生器频率控制
	
float 
	sinAngle = 0.0f,  // 正弦波角度，弧度制，[-1,1] 
	fre_es = 0.0f,	  // 测量的频率
	amp_es = 0.0f;	  // 幅值测量值，弧度制，[-5,5]
//======================================================
sbit Y = P1^4;
//==============函数体===============
void main()
{																													                                                            
    init_timer0();
    init_special_interrupts();
    init_ad();
	Y = 1;
    while (1)
    {
        
		if(key_sta & 0x01){//key_sta.0 = 0x01? key_sta.0 = 1检测到按键按下
			mode_select();
			key_sta = key_sta & 0xfe;//key_sta.0 = 0,按键弹起，等待下一次检测到按下
		}
    }
}
void interrupt_timer0() interrupt 1 // 定时器0中断处理
{
	ad_result = GetADC();// 单次采样 		
	if(mode == 1){		// CH1：实时采样并存储 CH2: 信号发生器
		// 信号发生
		da_count++;
		if(da_count >= gen_count){ // gen_count = 4时，每隔1ms输出一次D/A
			da_count = 0;
			da_generator();
			da_process();	// 显示值预处理
		}
		// 模式1对实时采样值进行存储
		ad_result = ((int)(ad_result / 2.0 + 64));
	 	ad_save();
	}
	else if(mode == 2){	// CH1: 实时采样 CH2: 波形回放显示
	 	ad_result = ((int)(ad_result / 2.0 + 64));	
	}
	else if(mode == 3){ // CH1: 实时采样 CH2: 输出0V	
		//time_count++;
		ad_measure();
		dspbuf_measure();//数码管显示缓存改变
		ad_result = ((int)(ad_result / 2.0 + 64));
	}
	else if(mode == 4){	// CH1: 实时采样 CH2: 信号发生器，调频调幅
		// 信号发生器
		da_count++;
		if(da_count >= gen_count){ // gen_count = 4时，每隔1ms输出一次D/A
			da_count = 0;
			da_generator();
			da_process();	// 显示值预处理
		}	
	}
	dsptask();		// 数码管扫描
	key_service(); 	// 按键扫描
    da_display();	// D/A显示
}
void da_process(){ 	// 处理待显示的D/A输出值
	da_result = (int)(da_result * 1.0f / da_index + 128.0f -128.0f / da_index); 
}
void da_display()
{										
	switch(mode){
	 	case 1:{	 // 模式1 CH1实时显示 CH2信号发生器
			DA_CH1 = ad_result;// 实时显示
			//DA_CH2 = da_result; // 信号发生器
			DA_CH2 = Y;
		};break;
		case 2:{ 	 // 模式2 CH1实时显示 CH2回放显示
		 	DA_CH1 = ad_result;// 实时显示
			DA_CH2 = ad_memo[review_p++]; // 回放显示
			if (review_p >= MEMO_LEN){ // 若显示指针到达存储区末尾，从头开始显示
        		review_p = 0;
   			}
		};break;
		case 3:{	// 模式3 测量模式
			DA_CH1 = ad_result;// 实时测量 
			DA_CH2 = 128; 
		};break;
		case 4:{ 	// 模式4 信号发生器调频、调幅
			DA_CH1 = ad_result; // 实时显示
			DA_CH2 = da_result; // 信号发生器 	
		};break;
		default: ;break;
	}
}
void da_generator()
{
 	switch(wave_sel){ // 根据wave_sel的选择产生相应波形
		case SIN:{	// 正弦波
			angle++;
			if(angle > 360){
			 	angle = 0;
			}
		 	sinAngle = 	sin(((float)angle * PI / 180.0f));// 求角度对应的弧度值[-1,1]
			da_result = (int)(((sinAngle + 1.0f) / 2.0f) * 255.0f); // 将[-1,1]内的sin值映射到[0,255]
		};break;
		case TRI:{	// 三角波
			if(tri_flag){//递增
				tri_value++;
				if(tri_value == 255){
				 	tri_flag = 0;
				}	
			}
			else{//递减
				tri_value--;
				if(tri_value == 0){
					tri_flag = 1;
				}
			}	 
			da_result = tri_value;	
		};break;	
		case REC:{	// 方波
			rec_count++;
			if(rec_count == 200){
				rec_count = 0;	 		
			}
			if(rec_count>100){
				da_result = 255;
			}else{
			 	da_result = 0;
			}		
		};break;
		case SAW:{	// 锯齿波
			saw_value++;
			if(saw_value > 255){
			 	saw_value = 0;
			}	
			da_result = saw_value;
		};break;
		default:da_result = 128;break;	
	}
}

void dspbuf_measure()	// 将采样得到的频率和幅值显示到数码管上
{
	// 频率值转换为数码管显示
	if(flag_fre){
		unsigned int integer_fre = 0;
		unsigned int fraction_fre = 0;
		integer_fre = (int)fre_es;
		dspbuf[3] = dspchar[integer_fre % 10];
		dspbuf[2] = dspchar[integer_fre / 10 % 10];
		dspbuf[1] = dspchar[integer_fre / 100 % 10];
		dspbuf[0] = dspchar[integer_fre / 1000 % 10];
	}
	// 幅值转换为数码管显示
	else if(flag_amp){
	 	if(amp_es >= 0.0f && amp_es <10.0f){
			unsigned int integer_amp = 0;
			unsigned int fraction_amp = 0;
			integer_amp = (int) amp_es;
			fraction_amp = (int) ((amp_es-integer_amp)*1000);
			dspbuf[0] = dspchar[integer_amp] & 0xfe;// 加小数点
			dspbuf[1] = dspchar[fraction_amp / 100 % 10];
			dspbuf[2] = dspchar[fraction_amp / 10 % 10];
			dspbuf[3] = dspchar[fraction_amp % 10];
		}
	} 
}
void ad_measure()	// 频率幅值单次检测
{
	// 频率测量
	amp_p2 = ad_result;
	time_count++;
	if(amp_p2 > 128 && amp_p1 <= 128){
		fre_p2 = time_count; 
		if(fre_p1 != 0){
		 	fre_sum = fre_sum + fabs(fre_p2 - fre_p1);
			fre_count++;
		}
		fre_p1 = fre_p2;	
	}
	amp_p1 = amp_p2;

	// 幅值测量
	if(ad_result > ad_max){
	 	ad_max = ad_result;
	}	
	if(ad_result < ad_min){
	 	ad_min = ad_result;
	}
	if(time_count >= AD_LEN){
	 	fre_es = 1500.0f / (fre_sum * 1.0f / fre_count);
		amp_es = 5.0f * ((ad_max-ad_min)/256.0f);
		fre_sum = 0;fre_count = 0;
		time_count = 0;
		fre_p1 = fre_p2 = 0;
		amp_p1 = amp_p2 = 129;
		ad_max = ad_min = 128;
	}
}

void mode_select()	// 模式选择
{
 	switch(key_num){
	 	case 1:{		// 模式1：实时采样+存储，信号发生器
			mode = 1;
			fdisp(1,0); // 数码管最左端显示模式1
			fdisp(0,1);
			fdisp(0,2);
			fdisp(wave_sel,3);
		};break;
		case 2:{		// 模式2：实时采样，波形回放
			mode = 2;
			fdisp(2,0);	// 数码管最左端显示模式2
			fdisp(0,1);
			fdisp(0,2);
			fdisp(0,3);	// 数码管最右端波形显示清零 	
		};break;
		case 3:{		// 模式3：测量实时采样的幅值和频率（或周期）
			mode = 3;
			fdisp(3,0);
			fdisp(0,1);
			fdisp(0,2);
			fdisp(0,3);
		};break;
		case 4:{		// 信号发生器调频调幅
			mode = 4;			
			fdisp(4,0);		  // 数码管最左端显示模式4
			fdisp(0,1);
			fdisp(0,2);
			fdisp(wave_sel,3);// 数码管最右端显示波形	
		};break;
		case 5:{
		 	if(mode == 1){			// 模式1下，按键5表示选择正弦波
			 	wave_sel = SIN;
				fdisp(wave_sel,3); 	// 在数码管最右端显示波形选择
			}
			else if(mode == 2){;}
			else if(mode == 3){		// 模式3下，按键5显示测量频率
				flag_amp = 0;																 
				flag_fre = 1;
			}
			else if(mode == 4){		// 模式4下，按键5表示频率1
				if(fre_modi){
				 	gen_count = 4;
				}
				if(amp_modi){
				 	da_index = 2;
				}
			}
		};break;
		case 6:{
			if(mode == 1){ 			// 模式1下，按键5表示选择三角波
			 	wave_sel = TRI;
				fdisp(wave_sel,3);
			}
			else if(mode == 3){		// 模式3下，按键6表示测量幅值
			 	flag_fre = 0;
				flag_amp = 1;		
			} 
			else if(mode == 4){		// 模式4下，按键6表示频率2
				if(fre_modi){
				 	gen_count = 3;
				}
				if(amp_modi){
				 	da_index = 3;
				}
			}	
		};break;
		case 7:{
			if(mode == 1){			// 模式1下，按键7表示选择方波
			 	wave_sel = REC;		
				fdisp(wave_sel,3);	
			}
			else if(mode == 3){
			 	fre_modi = 1;
				amp_modi = 0;
			}
			else if(mode == 4){		// 模式4下，按键7表示幅度减小,频率3
				if(fre_modi){
				 	gen_count = 2;
				}
				if(amp_modi){
				 	da_index = 4;
				}
			}
		};break;
		case 8:{					
			if(mode == 1){			// 模式1下，按键8表示选择锯齿波
			 	wave_sel = SAW;
				fdisp(wave_sel,3);
			}
			else if(mode == 3){
				fre_modi = 0;
				amp_modi = 1;
			}
			else if(mode == 4){		// 模式4下，按键8表示幅度增大,频率4
				if(fre_modi){
				 	gen_count = 1;
				}
				if(amp_modi){
				 	da_index = 5;
				}
			}
		}; break;
		default:break;
	}
	
}
void key_service()
{
	if(key_sta & 0x01) return;//key_sta.0 = 1，按键为按下状态
	// key_sta.0 = 0，按键为弹起状态，检测到KEY1/KEY2行有按键被按下
	if(KEY1){
	 	key_num = key_col + 1; 
		key_sta = key_sta | 0x01;//key_sta.0 = 1
	}	
	else if(KEY2){
	 	key_num = key_col + 5;
		key_sta = key_sta | 0x01;//key_sta.0 = 1	
	}
	
}

void ad_save() // A/D采样值存储
{
    ad_memo[memo_p++] = ad_result;
    if (memo_p >= MEMO_LEN)
    {
        memo_p = 0;
    }
}

void dsptask()
{
    unsigned char i;	// for loop 临时变量
    unsigned char a, b; // a:数码管位选，b:掩码

    // 数码管位选
    switch (sel)
    {
    case 0:	   // L1-T204
        a = 0x01;
        break; 
    case 1:	   // L2-T203
        a = 0x02;  
        break; 
    case 2:	   // L3-T202
        a = 0x04;
        break; 		
    default:   // L4-T201
        a = 0x08; 
    }
	// 将表示数码管位选的信息输入到74HC565的高位8bit 
	// 对应数码管的COM1为高电平（共阳极数码管）
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
        // b=b&0x7f;
    }
    // 载入位选数码管对应的显示内容
    a = dspbuf[sel];
	// 按键列选择
	key_col = sel;
	// 索引改变，选择下一个数码管
    sel++;
    if (sel >= 4)
        sel = 0; 
	// 将数码管对应显示内容输入到级联74HC565的低8bit
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
        // b=b&0x7fff;
    }
    D_RCLK = 0;
    D_RCLK = 1;
    D_RCLK = 0;
}
void fdisp(unsigned char n,unsigned char m)
{
 	char c;
	switch(n){
	 	case 0: c = 0x11; break;
		case 1: c = 0x7d; break;
		case 2: c = 0x23; break;
		case 3: c = 0x29; break;
		case 4: c = 0x4d; break;
		case 5: c = 0x89; break;
		case 6: c = 0x81; break;
		case 7: c = 0x3d; break;
		case 8: c = 0x01; break;
		case 9: c = 0x09; break;
		default:c = 0xff; break;
	}
	dspbuf[m] = c;
}
unsigned char GetADC()
{
    ADC_CONTR = ADC_CONTR | ADC_POWER | 3 | ADC_START;
    _nop_(); // wait
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG))
        ;                   // 等待A/D标志位变1，ADC_FLAG
    ADC_CONTR &= ~ADC_FLAG; // 标志位清零
    return ADC_RES;
}


void init_ad() // ad寄存器初始化
{
    P1ASF = 0x08;     // P1.3作为ad输入
    ADC_CONTR = 0xE3; // ADC控制寄存器         Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                      // 位描述            ADC_POWER  SPEED1 SPEED0 ADC_FLAG ADC_START CHS2    CHS1    CHS0
                      // 初始值=0000,0000      0       0       0       0       0       0       0       0
                      // 设置值=0000,0000      1       1       1       0       0       0       1       1
    AUXR1 &= 0xfb;    // 高8位存储在在ADC_RES
    CLK_DIV = 0x01;   // 系统时钟/2
}
void init_special_interrupts() // 中断初始化
{
    EX0 = 0;
    ET0 = 1; // 定时器0中断
    EX1 = 0;
    ET1 = 1; // 定时器1中断
    ES = 1;
    EA = 1; // 总中断
    PT0 = 0;
    PX1 = 1;
    PS = 0;
}
void init_timer0() // 定时器0初始化
{                  // 1/4ms
    TMOD &= 0XF0;  // Timer0设置清除
    TMOD |= 0X02;  // 8位自动重载
    TL0 = 0X06;
    TH0 = 0X06;
    TR0 = 1;
    ET0 = 1;
}



