sbit SCL = P2^1;
sbit SDA = P2^0;
void I2CStart();
void I2CStop();
void I2CDelay();
bit I2CAddressing(unsigned char addr);
bit I2CWrite(unsigned char dat);
unsigned char I2CReadACK();//����Ӧ���ź�
unsigned char I2CReadNCK();//���ͷ�Ӧ���ź�
unsigned char E2ReadByte(unsigned char addr);
void E2WriteByte(unsigned char addr, unsigned char dat);