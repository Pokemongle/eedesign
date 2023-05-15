//==function==
unsigned char ad_get(); // A/D采样（单次）
void ad_measure();	// 频率幅值单次检测
void ad_save(); 	  	// A/D采样值存储
void da_display();
void da_generator();  	// 信号发生器
void da_process();	  	// 处理待显示的D/A输出值
void dspbuf_measure();	// 将采样得到的频率和幅值显示到数码管上
//============
#define MEMO_LEN 8000
#define DA_CH1 XBYTE[0x4000] // DA显示通道CH1
#define DA_CH2 XBYTE[0x2000] // DA显示通道CH2
#define PI 3.1415926535
#define AD_LEN 250
//==parameter==
extern unsigned char
	ad_max,  // 测量信号当前最大值
	ad_min,  // 测量信号当前最小值
	ad_result, // A/D单次采集信号结果
	amp_p1,  	 // 频率测量指针1
	amp_p2,  	 // 频率测量指针2
	da_count,  	 // 信号发生器定时计数
	da_result, 	 // 信号发生器单次产生的数字信号
	fre_count,	 // 幅值测量计数
	fre_p1,	   	 // 频率测量指针1 
	fre_p2,	   	 // 频率测量指针2
	fre_sum,   	 // 频率测量累加
	rec_count, 	 // 方波信号发生计数
	rec_value,  // 方波信号
	review_p,  // D/A回放显示指针
	saw_value,  // 锯齿波信号
	time_count,  // 测量时间计数
	tri_value,  // 三角波信号
	tri_flag;    // 三角波幅值递增递减标志

extern unsigned int 
	angle,	   // 正弦波角度，角度值，[0,360]
	memo_p;	   // A/D采集存储区指针

extern float
	sinAngle,  // 正弦波角度，弧度制，[-1,1]
	fre_es,	  // 测量的频率
	amp_es;	  // 幅值测量值，弧度制，[-5,5]
extern unsigned char xdata
    ad_memo[]; 	// A/D采集信号存储区
//============
