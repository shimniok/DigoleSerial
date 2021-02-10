/*********************
Digole Serial display module with touch screen demo
NOTE: if you using UART mode, you need to disconnect the RX from Arduino
When you uploading your sketch
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
void readTouchScreen(void);
int x,y;
void setup()
{
    mydisp.begin();
    mydisp.clearScreen();
    mydisp.setColor(0B11111100);  //256 color format RRRGGGBB
//  mydisp.setTrueColor(63,63,0)  //262K color format Red, Green, Blue 0~63    
    mydisp.drawStr(7,10,"Touch Screen Test");
    mydisp.setColor(1);
//Serial.begin(9600);
}
void loop()
{
  readTouchScreen();
//  x=mydisp.read();
//  x<<=8;
//  x+=mydisp.read();
  x=mydisp.readInt();
//  Serial.println(x);
  y=mydisp.read();
  y<<=8;
  y+=mydisp.read();
  mydisp.drawPixel(x,y);
}
void readTouchScreen()
{
  mydisp.write('R');
  mydisp.write('P');
  mydisp.write('N');
  mydisp.write('X');
  mydisp.write('Y');
  mydisp.write('W');
}
