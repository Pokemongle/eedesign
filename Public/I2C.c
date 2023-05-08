#include<reg52.h>
#include<intrins.h>
#include"I2C.h"

void I2CStart(){
	SDA = 1;
 	SCL = 1;
	I2CDelay();
	SDA = 0;
	I2CDelay();
	SCL = 0;
}
void I2CDelay(){
 	_nop_();
	_nop_();
	_nop_();
	_nop_();
}
bit I2CAddressing(unsigned char addr){
 	bit ack;
	I2CStart();
	ack = I2CWrite(addr<<1);
	I2CStop();
	return ack;
}
void I2CStop(){
	SCL = 0;
	SDA = 0;
	I2CDelay();
	SCL = 1;
	I2CDelay();
	SDA = 1;
	I2CDelay(); 	
}
bit I2CWrite(unsigned char dat){
	bit ack;
	unsigned char mask;
	for(mask = 0x80;mask!=0;mask>>=1){
	 	if((mask&dat)==0){
		 	SDA = 0;
		}
		else{
		 	SDA = 1;
		}
		I2CDelay();
		SCL = 1;
		I2CDelay();
		SCL = 0;
	}
	SDA = 1;
	I2CDelay();
	SCL = 1;
	ack = SDA;
	I2CDelay();
	SCL = 0;
	return ~ack;
}
unsigned char I2CReadACK()
{
	unsigned char mask;
	unsigned char dat;
	SDA = 1;
	for(mask=0x80;mask!=0;mask>>=1){
	 	I2CDelay();
		SCL = 1;
		if(SDA == 0){
		 	dat &= ~mask;
		}
		else{
		 	dat |= mask;
		}
		I2CDelay();
		SCL = 0;
	}
	SDA = 0;
	I2CDelay();
	SCL = 1;
	I2CDelay();
	SCL = 0;
	return dat; 	
}
unsigned char I2CReadNAK()
{
	unsigned char mask;
	unsigned char dat;
	SDA = 1;
	for(mask=0x80;mask!=0;mask>>=1){
	 	I2CDelay();
		SCL = 1;
		if(SDA == 0){
		 	dat &= ~mask;
		}
		else{
		 	dat |= mask;
		}
		I2CDelay();
		SCL = 0;
	}
	SDA = 1;
	I2CDelay();
	SCL = 1;
	I2CDelay();
	SCL = 0;
	return dat;
}
unsigned char E2ReadByte(unsigned char addr){
 	unsigned char dat;
	I2CStart();
	I2CWrite(0x50<<1);
	I2CWrite(addr);
	I2CStart();
	I2CWrite((0x50<<1)|0x01);
	dat = I2CReadNAK();
	I2CStop();
	return dat;
}
void E2WriteByte(unsigned char addr, unsigned char dat){
 	I2CStart();
	I2CWrite(0x50<<1);
	I2CWrite(addr);
	I2CWrite(dat);
	I2CStop();
}
