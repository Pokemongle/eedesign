//==头文件==
#include <STC12C5A.h> // 51标准头文件
#include "led.h"
//==========
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