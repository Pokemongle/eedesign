//==========ͷ�ļ�=========
#include <STC12C5A.h>
#include <absacc.h>
#include <intrins.h>
#include <math.h>
//=========================



//============��������===============


void amp_measure();				// ��ֵ���
void da_display();              // D/A��ʾ
void da_generator();			// �źŷ�����
void da_process();				// ����[0,255]�ڵĴ���ʾD/A���ֵ
void dsptask();                 // �������ʾ����
void dspbuf_measure();			// ģʽ3���������ʾ������Ϊ��ǰ����ֵ
void fdisp(unsigned char n,unsigned char m);// �����������ʾ				
//nΪ�������ʾ������ mΪ�����λѡ	
unsigned char GetADC();         // A/D���������Σ�
void init_ad();                 // �ڲ�A/D�Ĵ�����ʼ��
void init_timer0();             // ��ʱ��0��ʼ��
void init_special_interrupts(); // �жϳ�ʼ��
//void key_scan();				// ����ɨ��
void key_service();				// ����ɨ��
void mode_select();			    // ģʽѡ���޸Ķ�Ӧ����ܻ�����
//void memo_increment();			// �洢��ȡλ�ø��� 
//--ģʽ1������ʵʱ��ʾģʽ
//---------CH1: ����ʵʱ�����ͬʱ�洢����
//---------CH2: �źŷ�����	���Ҳ�|���ǲ�|����|��ݲ�						
//--ģʽ2�����λط���ʾģʽ															
//---------CH1: ����ʵʱ���
//---------CH2�����ģʽ1�д洢���źŵĲ���																
//--ģʽ3������ģʽ
//--ģʽ4����������Ƶģʽ
//---------CH1: ����ʵʱ���
//---------CH2: �źŷ�����	���Ҳ�|���ǲ�|����|��ݲ�

//===============ȫ�ֱ�������===========================


unsigned char
	amp_modi = 0,  // ������־
	fre_modi = 0,  // ��Ƶ��־
	ad_max = 128,  // �����źŵ�ǰ���ֵ
	ad_min = 128,  // �����źŵ�ǰ��Сֵ
	ad_count = 0,  // A/D��������
    
	dag_p = 0,	   // �źŷ�����ָ��
	da_result = 0, // �źŷ��������β����������ź�
	flag_amp = 0,  // �������ʾ��ֵ
	flag_fre = 0,  // �������ʾƵ�ʱ�־
	key_col = 0,   // ����ɨ�赱ǰ��
	key_num = 0,   // �������
	mode = 0,	   // ģʽѡ��
	review_p = 0,  // D/A�ط���ʾָ��
	rec_count = 0, // �����źŷ�������
	saw_value = 64,// ��ݲ��ź�
	rec_value = 64,// �����ź�
	tri_value = 64,// ���ǲ��ź�
	tri_flag = 1,  // ���ǲ���ֵ�����ݼ���־
	//volt_max = 192,// ��ѹ���ֵ
	//volt_min = 64, // ��ѹ��Сֵ
	wave_sel = 0,  // �źŷ���������ѡ��
	i = 0,		   // ����ɨ��λѡ
	fre_p1 = 1,	   // Ƶ�ʲ���ָ��1 
	fre_p2 = 1,	   // Ƶ�ʲ���ָ��2
	amp_p1 = 129,  // Ƶ�ʲ���ָ��
	amp_p2 = 129,  // Ƶ�ʲ���ָ��
	key_p = 0;	   // ����ɨ�軺����ָ��
unsigned int
	//fre_es = 0,    // ������Ƶ��
	fre_count = 0, // ��ֵ��������
	fre_sum = 0,   // Ƶ�ʲ����ۼ�
	time_count = 0,// ����ʱ�����
	angle = 0,	   // ���Ҳ��Ƕȣ��Ƕ�ֵ��[0,360]
	da_count = 0,  // �źŷ�������ʱ����
	da_index = 2,// ��������
    clocktime = 0, //
	gen_count = 4; // �źŷ�����Ƶ�ʿ���
	
float 
	sinAngle = 0.0f,  // ���Ҳ��Ƕȣ������ƣ�[-1,1] 
	fre_es = 0.0f,	  // ������Ƶ��
	amp_es = 0.0f;	  // ��ֵ����ֵ�������ƣ�[-5,5]

    

//======================================================

//==============������===============
void main()
{																													                                                            
    init_timer0();
    init_special_interrupts();
    init_ad();
    while (1)
    {
        
		if(key_sta & 0x01){//key_sta.0 = 0x01? key_sta.0 = 1��⵽��������
			mode_select();
			key_sta = key_sta & 0xfe;//key_sta.0 = 0,�������𣬵ȴ���һ�μ�⵽����
		}
    }
}
void interrupt_timer0() interrupt 1 // ��ʱ��0�жϴ���
{
	EA = 0;
	ad_result = GetADC();// ���β��� 		

	if(mode == 1){		// CH1��ʵʱ�������洢 CH2: �źŷ�����
		// �źŷ���
		da_count++;
		if(da_count >= gen_count){ // gen_count = 4ʱ��ÿ��1ms���һ��D/A
			da_count = 0;
			da_generator();
			da_process();	// ��ʾֵԤ����
		}
		// ģʽ1��ʵʱ����ֵ���д洢
		ad_result = ((int)(ad_result / 2.0 + 64));
	 	ad_save();
	}
	else if(mode == 2){	// CH1: ʵʱ���� CH2: ���λط���ʾ
	 	ad_result = ((int)(ad_result / 2.0 + 64));	
	}
	else if(mode == 3){ // CH1: ʵʱ���� CH2: ���0V	
		//time_count++;
		ad_measure();
		/*
		if(time_count >= 250){// 0.25ms*400=0.1s����һ�β���ֵ
		 	time_count = 0;
			//fre_es = fre_count / 0.15f;  // �������Ƶ��
			fre_es = 500.0f / (fre_sum * 1.0f / fre_count);
			amp_es = 5.0f*((ad_max - ad_min) / 256.0f); // ���������ֵ
			fre_sum = 0; fre_count = 0;
			fre_p1 = fre_p2 = 1;
			amp_p1 = amp_p2 = 129;
			ad_max = ad_min = 128;
		}
	   */
		dspbuf_measure();//�������ʾ����ı�
		ad_result = ((int)(ad_result / 2.0 + 64));
	}
	else if(mode == 4){	// CH1: ʵʱ���� CH2: �źŷ���������Ƶ����
		// �źŷ�����
		da_count++;
		if(da_count >= gen_count){ // gen_count = 4ʱ��ÿ��1ms���һ��D/A
			da_count = 0;
			da_generator();
			da_process();	// ��ʾֵԤ����
		}	
	}
	dsptask();		// �����ɨ��
	key_service(); 	// ����ɨ��
	//da_process();	// ��ʾֵԤ����
	//ad_result = ((int)(ad_result / 2.0 + 64)); // ���Ԥ����
    da_display();	// D/A��ʾ
	EA = 1;
}
void da_process(){ 	// �������ʾ��D/A���ֵ
	da_result = (int)(da_result * 1.0f / da_index + 128.0f -128.0f / da_index); 
	//da_result = (int)((da_result - 128.0f)*(da_index * 1.0f / 10000.0f) + 128.0f);
	//da_result = (int)(da_result * 1.0f / da_index + 128.0f -128.0f / da_index);
}
void da_display()
{										
	switch(mode){
	 	case 1:{	 // ģʽ1 CH1ʵʱ��ʾ CH2�źŷ�����
			DA_CH1 = ad_result;// ʵʱ��ʾ
			DA_CH2 = da_result; // �źŷ�����
		};break;
		case 2:{ 	 // ģʽ2 CH1ʵʱ��ʾ CH2�ط���ʾ
		 	DA_CH1 = ad_result;// ʵʱ��ʾ
			DA_CH2 = ad_memo[review_p++]; // �ط���ʾ
			if (review_p >= MEMO_LEN){ // ����ʾָ�뵽��洢��ĩβ����ͷ��ʼ��ʾ
        		review_p = 0;
   			}
		};break;
		case 3:{	// ģʽ3 ����ģʽ
			DA_CH1 = ad_result;// ʵʱ���� 
			DA_CH2 = 128; 
		};break;
		case 4:{ 	// ģʽ4 �źŷ�������Ƶ������
			DA_CH1 = ad_result; // ʵʱ��ʾ
			DA_CH2 = da_result; // �źŷ����� 	
		};break;
		default: ;break;
	}
}
void da_generator()
{
 	switch(wave_sel){ // ����wave_sel��ѡ�������Ӧ����
		case SIN:{	// ���Ҳ�
			angle++;
			if(angle > 360){
			 	angle = 0;
			}
		 	sinAngle = 	sin(((float)angle * PI / 180.0f));// ��Ƕȶ�Ӧ�Ļ���ֵ[-1,1]
			da_result = (int)(((sinAngle + 1.0f) / 2.0f) * 255.0f); // ��[-1,1]�ڵ�sinֵӳ�䵽[0,255]
		};break;
		case TRI:{	// ���ǲ�
			if(tri_flag){//����
				tri_value++;
				if(tri_value == 255){
				 	tri_flag = 0;
				}	
			}
			else{//�ݼ�
				tri_value--;
				if(tri_value == 0){
					tri_flag = 1;
				}
			}	 
			da_result = tri_value;	
		};break;	
		case REC:{	// ����
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
		case SAW:{	// ��ݲ�
			saw_value++;
			if(saw_value > 255){
			 	saw_value = 0;
			}	
			da_result = saw_value;
		};break;
		default:da_result = 128;break;	
	}
}

void dspbuf_measure()	// �������õ���Ƶ�ʺͷ�ֵ��ʾ���������
{
	// Ƶ��ֵת��Ϊ�������ʾ
	if(flag_fre){
		unsigned int integer_fre = 0;
		unsigned int fraction_fre = 0;
		integer_fre = (int)fre_es;
		dspbuf[3] = dspchar[integer_fre % 10];
		dspbuf[2] = dspchar[integer_fre / 10 % 10];
		dspbuf[1] = dspchar[integer_fre / 100 % 10];
		dspbuf[0] = dspchar[integer_fre / 1000 % 10];
		/*
		if(fre_es >= 100 && fre_es <1000){ // Ƶ�����Ϊ��λ��
			integer_fre = (int)fre_es;
			fraction_fre = (int)(10*(fre_es-integer_fre));	// ��ʾ1λС��
			dspbuf[3] = dspchar[fraction_fre];
			dspbuf[2] = dspchar[integer_fre % 10] & 0xfe;// ��С����
			dspbuf[1] = dspchar[integer_fre / 10 % 10] ; 
			dspbuf[0] = dspchar[integer_fre / 100 % 10]; 	
		}
		else if(fre_es >= 1000 && fre_es <10000) //Ƶ��Ϊ��λ��
		{
			integer_fre = (int)fre_es; 
			dspbuf[3] = dspchar[integer_fre % 10];
			dspbuf[2] = dspchar[integer_fre / 10 % 10];
			dspbuf[1] = dspchar[integer_fre / 100 % 10]; 	
			dspbuf[0] = dspchar[integer_fre / 1000 % 10];
		}
		else if(fre_es > 0 && fre_es <10){//Ƶ��Ϊһλ��
			integer_fre = (int)fre_es;
			fraction_fre = (int)(1000*(fre_es - integer_fre));
			dspbuf[0] = dspchar[integer_fre] & 0xfe;//��С����
			dspbuf[3] = dspchar[fraction_fre % 10];
			dspbuf[2] = dspchar[fraction_fre / 10 % 10] ; 
			dspbuf[1] = dspchar[fraction_fre / 100 % 10]; 	 	
		}
		*/
	}
	// ��ֵת��Ϊ�������ʾ
	else if(flag_amp){
	 	if(amp_es >= 0.0f && amp_es <10.0f){
			unsigned int integer_amp = 0;
			unsigned int fraction_amp = 0;
			integer_amp = (int) amp_es;
			fraction_amp = (int) ((amp_es-integer_amp)*1000);
			dspbuf[0] = dspchar[integer_amp] & 0xfe;// ��С����
			dspbuf[1] = dspchar[fraction_amp / 100 % 10];
			dspbuf[2] = dspchar[fraction_amp / 10 % 10];
			dspbuf[3] = dspchar[fraction_amp % 10];
		}
	} 
}


void mode_select()	// ģʽѡ��
{
 	switch(key_num){
	 	case 1:{		// ģʽ1��ʵʱ����+�洢���źŷ�����
			mode = 1;
			fdisp(1,0); // ������������ʾģʽ1
			fdisp(0,1);
			fdisp(0,2);
			fdisp(wave_sel,3);
		};break;
		case 2:{		// ģʽ2��ʵʱ���������λط�
			mode = 2;
			fdisp(2,0);	// ������������ʾģʽ2
			fdisp(0,1);
			fdisp(0,2);
			fdisp(0,3);	// ��������Ҷ˲�����ʾ���� 	
		};break;
		case 3:{		// ģʽ3������ʵʱ�����ķ�ֵ��Ƶ�ʣ������ڣ�
			mode = 3;
			fdisp(3,0);
			fdisp(0,1);
			fdisp(0,2);
			fdisp(0,3);
		};break;
		case 4:{		// �źŷ�������Ƶ����
			mode = 4;			
			fdisp(4,0);		  // ������������ʾģʽ4
			fdisp(0,1);
			fdisp(0,2);
			fdisp(wave_sel,3);// ��������Ҷ���ʾ����	
		};break;
		case 5:{
		 	if(mode == 1){			// ģʽ1�£�����5��ʾѡ�����Ҳ�
			 	wave_sel = SIN;
				fdisp(wave_sel,3); 	// ����������Ҷ���ʾ����ѡ��
			}
			else if(mode == 2){;}
			else if(mode == 3){		// ģʽ3�£�����5��ʾ����Ƶ��
				flag_amp = 0;																 
				flag_fre = 1;
			}
			else if(mode == 4){		// ģʽ4�£�����5��ʾƵ��1
				/*if(gen_count >= 50000){
					gen_count = 50000;
				}
				else{
					gen_count++;
				}
				*/
				if(fre_modi){
				 	gen_count = 4;
				}
				if(amp_modi){
				 	da_index = 2;
				}
				//gen_count = 4;
				//da_index = 2;
			}
		};break;
		case 6:{
			if(mode == 1){ 			// ģʽ1�£�����5��ʾѡ�����ǲ�
			 	wave_sel = TRI;
				fdisp(wave_sel,3);
			}
			else if(mode == 3){		// ģʽ3�£�����6��ʾ������ֵ
			 	flag_fre = 0;
				flag_amp = 1;		
			} 
			else if(mode == 4){		// ģʽ4�£�����6��ʾƵ��2
				/*
				if(gen_count <= 4){
					gen_count = 4;
				}else{
					gen_count--;
				}
				*/
				if(fre_modi){
				 	gen_count = 3;
				}
				if(amp_modi){
				 	da_index = 3;
				}
			}	
		};break;
		case 7:{
			if(mode == 1){			// ģʽ1�£�����7��ʾѡ�񷽲�
			 	wave_sel = REC;		
				fdisp(wave_sel,3);	
			}
			else if(mode == 3){
			 	fre_modi = 1;
				amp_modi = 0;
			}
			else if(mode == 4){		// ģʽ4�£�����7��ʾ���ȼ�С,Ƶ��3
			 	/*
				if(da_index <= 1000){
					da_index = 1000;
				}
				else{
				 	da_index--;
				}
				*/
				if(fre_modi){
				 	gen_count = 2;
				}
				if(amp_modi){
				 	da_index = 4;
				}
			}
		};break;
		case 8:{					
			if(mode == 1){			// ģʽ1�£�����8��ʾѡ���ݲ�
			 	wave_sel = SAW;
				fdisp(wave_sel,3);
			}
			else if(mode == 3){
				fre_modi = 0;
				amp_modi = 1;
			}
			else if(mode == 4){		// ģʽ4�£�����8��ʾ��������,Ƶ��4
				/*
				if(da_index >= 5000){
				 	da_index = 5000;
				}
				else{
				 	da_index++;
				}
				*/
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
/*
unsigned char amp = 128;
unsigned char amp_up = 128, amp_low =128;
float adamp = 0.0f;
unsigned int num = 0;
void amp_measure() // ��ֵ����
{
    //int i = 0;
    amp = ad_result;
    memo_p++;
    if (amp > amp_up)
    {
        amp_up = amp;
    }
    if (amp < amp_low)
    {
        amp_low = amp;
    }
    if (memo_p >= MEMO_LEN)
    {
        adamp = (amp_up * 5.0 - amp_low * 5.0) / 256;
        memo_p = 0;
        amp_up = amp_low = 128;
    }

    num =(int)adamp*1000;
	fdisp(num%10,0);
	fdisp(num/10%10,1);
	fdisp(num/100%10,2);
	fdisp(num/1000%10,3);
}
*/

void key_service()
{
	if(key_sta & 0x01) return;//key_sta.0 = 1������Ϊ����״̬
	// key_sta.0 = 0������Ϊ����״̬����⵽KEY1/KEY2���а���������
	if(KEY1){
	 	key_num = key_col + 1; 
		key_sta = key_sta | 0x01;//key_sta.0 = 1
	}	
	else if(KEY2){
	 	key_num = key_col + 5;
		key_sta = key_sta | 0x01;//key_sta.0 = 1	
	}
	
}
/*
void ad_delay()
{
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}
*/
/*
void ad_get()
{
    ADC_RES = 0;       // ADC�洢��������
    ADC_CONTR |= 0x08; // ADCSTART = 1
    ad_delay();        // ��Ҫ��4��nop
    while (ADC_CONTR & 0x10 == 0)
        ;                // ADC_FLAG = 0,�ȴ�ADC_FLAG = 1
    ADC_CONTR &= 0xE7;   // ������λ����ȴ��´β���
    ad_result = ADC_RES; // �������ֵ
}
*/



void dsptask()
{
    unsigned char i;	// for loop ��ʱ����
    unsigned char a, b; // a:�����λѡ��b:����

    // �����λѡ
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
	// ����ʾ�����λѡ����Ϣ���뵽74HC565�ĸ�λ8bit 
	// ��Ӧ����ܵ�COM1Ϊ�ߵ�ƽ������������ܣ�
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
    // ����λѡ����ܶ�Ӧ����ʾ����
    a = dspbuf[sel];
	// ������ѡ��
	key_col = sel;
	// �����ı䣬ѡ����һ�������
    sel++;
    if (sel >= 4)
        sel = 0; 
	// ������ܶ�Ӧ��ʾ�������뵽����74HC565�ĵ�8bit
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
        ;                   // �ȴ�A/D��־λ��1��ADC_FLAG
    ADC_CONTR &= ~ADC_FLAG; // ��־λ����
    return ADC_RES;
}


void init_ad() // ad�Ĵ�����ʼ��
{
    P1ASF = 0x08;     // P1.3��Ϊad����
    ADC_CONTR = 0xE3; // ADC���ƼĴ���         Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                      // λ����            ADC_POWER  SPEED1 SPEED0 ADC_FLAG ADC_START CHS2    CHS1    CHS0
                      // ��ʼֵ=0000,0000      0       0       0       0       0       0       0       0
                      // ����ֵ=0000,0000      1       1       1       0       0       0       1       1
    AUXR1 &= 0xfb;    // ��8λ�洢����ADC_RES
    CLK_DIV = 0x01;   // ϵͳʱ��/2
}
void init_special_interrupts() // �жϳ�ʼ��
{
    EX0 = 0;
    ET0 = 1; // ��ʱ��0�ж�
    EX1 = 0;
    ET1 = 1; // ��ʱ��1�ж�
    ES = 1;
    EA = 1; // ���ж�
    PT0 = 0;
    PX1 = 1;
    PS = 0;
}
void init_timer0() // ��ʱ��0��ʼ��
{                  // 1/4ms
    TMOD &= 0XF0;  // Timer0�������
    TMOD |= 0X02;  // 8λ�Զ�����
    TL0 = 0X06;
    TH0 = 0X06;
    TR0 = 1;
    ET0 = 1;
}



