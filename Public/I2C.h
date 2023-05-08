sbit SCL = P2^1;
sbit SDA = P2^0;
void I2CStart();
void I2CStop();
void I2CDelay();
bit I2CAddressing(unsigned char addr);
bit I2CWrite(unsigned char dat);
unsigned char I2CReadACK();//发送应答信号
unsigned char I2CReadNCK();//发送非应答信号
unsigned char E2ReadByte(unsigned char addr);
void E2WriteByte(unsigned char addr, unsigned char dat);