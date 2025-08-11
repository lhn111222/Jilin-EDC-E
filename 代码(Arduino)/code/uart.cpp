#include "uart.h"
//串口初始化
 void uart_Init()
 {
   WL_SERIAL.begin(9600);
 }

// 读取数据 Data: 读取到的数据
void Read_Data(unsigned int *Data)	       
{	
  unsigned char y = 0;
  unsigned char USART_RX_STA[3] = { 0 };       //数据缓存
  unsigned char Num = 0;              //数组计数
  unsigned int Receive_data = 0;       //数据缓存
	
  WL_SERIAL.write(0x57);
 ///////////////////////////数字量数值///////////////////////////////	
 
  for(y=0;y <= 5;y++)
  {
    delay(1);
    if(WL_SERIAL.available() > 0)
    {
      USART_RX_STA[Num++] = WL_SERIAL.read();	//依次读取接收到的数据
      if(Num == 1)
      {
        Num = 0;
        *Data = USART_RX_STA[0];
        break;
      }
    } 
  }
  
///////////////////////////数字量数值///////////////////////////////	

///////////////////////////偏移量数值///////////////////////////////

//  for(y=0;y <= 10;y++)
//  {
//    delay(1);
//    if(WL_SERIAL.available() > 0)
//    {
//      USART_RX_STA[Num++] = WL_SERIAL.read();	//依次读取接收到的数据
//      if(Num == 3)
//      {
//        Num = 0;
//        
//        Receive_data = USART_RX_STA[1];
//        Receive_data <<= 8;
//        Receive_data |= USART_RX_STA[2];
//        
//        *Data = USART_RX_STA[0];
//        *(Data+1) = Receive_data;
//        
//        break;
//      }
//    } 
//  }
  
///////////////////////////偏移量数值///////////////////////////////	
}

