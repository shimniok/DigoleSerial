/*********************
 * Digole Serial display module with touch screen demo
 * NOTE: if you using UART mode, you need to disconnect the RX from Arduino
 * When you uploading your sketch
 **********************/
#define _Digole_Serial_I2C_  //To tell compiler compile the special communication only, 
//all available are:_Digole_Serial_UART_, _Digole_Serial_I2C_ and _Digole_Serial_SPI_
#include <DigoleSerial.h>
//--------UART setup
#if defined(_Digole_Serial_UART_)
DigoleSerialDisp mydisp(&Serial, 38400); //UART:Arduino UNO: Pin 1(TX)on arduino to RX on module
#endif
//--------I2C setup
#if defined(_Digole_Serial_I2C_)
#include <Wire.h>
DigoleSerialDisp mydisp(&Wire,'\x27');  //I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
#endif
//--------SPI setup
#if defined(_Digole_Serial_SPI_)
DigoleSerialDisp mydisp(8,9,10,11);  //SPI:Pin 8: data, 9:clock, 10: SS, 11:SI. you can assign 255 to SS, and hard ground SS pin on module
#endif
void flashErase(unsigned long int addr,unsigned long int len);
void flashWriteL(unsigned long int addr,unsigned long int len, const unsigned char *data);
void flashWrite(unsigned long int addr,unsigned long int len, const unsigned char *data);
void flashReadStart(unsigned long int addr,unsigned long int len);
int x,y;
unsigned char d[10]={
  20,19,18,17,16,15,14,13,12,11};
void setup()
{
  mydisp.begin();
  mydisp.clearScreen();
  mydisp.setColor(0B11111100);  //256 color format RRRGGGBB
  //  mydisp.setTrueColor(63,63,0)  //262K color format Red, Green, Blue 0~63    
  mydisp.drawStr(7,10,"Test Flash Memory");
  mydisp.setColor(1);
  Serial.begin(9600);
  flashReadStart(0,10);  //read flash memory 0~10, and display it in Serial Monitor
  for(int i=0;i<10;i++)
  {
    Serial.print(mydisp.read());
    Serial.print('\n');
  }
  flashErase(0,10);        //erase flash memory 0~9
  flashReadStart(0,10);  //check flash memory 0~10, all should be FFh
  for(int i=0;i<10;i++)
  {
    Serial.print(mydisp.read());
    Serial.print('\n');
  }
  flashWrite(0,9,d);  //write array d to flash memory start at 0
  while(mydisp.read()!='O');  //check write memory done
  flashReadStart(0,10);  //read flash memory from 0, the data should same as in array d
  for(int i=0;i<10;i++)
  {
    Serial.print(mydisp.read());
    Serial.print('\n');
  }
  Serial.print('\n');
}
void loop()
{
}
void flashErase(unsigned long int addr, unsigned long int length)
{
  mydisp.write('F');
  mydisp.write('L');
  mydisp.write('M');
  mydisp.write('E');
  mydisp.write('R');
  mydisp.write(addr>>16);
  mydisp.write(addr>>8);
  mydisp.write(addr);
  mydisp.write(length>>16);
  mydisp.write(length>>8);
  mydisp.write(length);
}
void flashWriteL(unsigned long int addr,unsigned long int len, const unsigned char *data)
{
  while(len>1024)
  {
    flashWrite(addr,1024,data);
    data+=1024;
    addr+=1024;
    len=len-1024;
  }
  flashWrite(addr,len,data);
}

void flashWrite(unsigned long int addr,unsigned long int len, const unsigned char *data)
{
  unsigned char c;
  mydisp.write('F');
  mydisp.write('L');
  mydisp.write('M');
  mydisp.write('W');
  mydisp.write('R');
  mydisp.write(addr>>16);
  mydisp.write(addr>>8);
  mydisp.write(addr);
  mydisp.write(len>>16);
  mydisp.write(len>>8);
  mydisp.write(len);
  for(long int i=0;i<len;i++)
  {
    c=pgm_read_byte_near(data+i);
    mydisp.write(c);
  }
  while(mydisp.read()!=17);  //check write memory done
}
void flashReadStart(unsigned long int addr,unsigned long int len)
{
  mydisp.write('F');
  mydisp.write('L');
  mydisp.write('M');
  mydisp.write('R');
  mydisp.write('D');
  mydisp.write(addr>>16);
  mydisp.write(addr>>8);
  mydisp.write(addr);
  mydisp.write(len>>16);
  mydisp.write(len>>8);
  mydisp.write(len);
}


