#include "uart.h"

/*********************************************************************
 *  函数名称：uart_Init
 *  函数功能：串口初始化
 *  形    参：无
 *  输    出：无
 *  备    注：无
 ********************************************************************/
void uart_Init()
{
  WL_SERIAL.begin(9600);
}

/*********************************************************************
 *  函数名称：Read_Data
 *  函数功能：读取数据
 *  形    参：*Data: 读取到的数据
 *  输    出：无
 *  备    注：无
 ********************************************************************/
void Read_Data(unsigned int *Data)	       
{	
  unsigned char y = 0;
  unsigned char USART_RX_STA[3] = { 0 };       //数据缓存区
  unsigned char Num = 0;              //数组计数
  unsigned int Receive_data = 0;       //数据缓存区
	
  WL_SERIAL.write(0x57);
  ///////////////////////////数字量数值///////////////////////////////	
 
  for(y=0;y <= 7;y++)
  {
    delay(1);
    if(WL_SERIAL.available() > 0)
    {
      USART_RX_STA[Num++] = WL_SERIAL.read();	//依次读取接收到的数据
      if(Num == 2)
      {
        Num = 0;
        Receive_data = USART_RX_STA[1];
        Receive_data <<= 8;
        Receive_data |= USART_RX_STA[0];
        
        *Data = Receive_data;
        break;
      }
    } 
  }

}

