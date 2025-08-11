#include <Servo.h>           // 声明调用Servo.h库
#include <SoftwareSerial.h> 
#include "motor.h"
#include "uart.h"

#define LOW 80               //巡线速度
#define MAX 255               //全速状态速度   

unsigned int Temp_Data[2] = {0};       //数据缓存区

void setup(){  
  Serial.begin(9600);
  Motor_Init();
  uart_Init();
}

void loop(){
  Read_Data(Temp_Data); 

  Serial.println(Temp_Data[0]);
  //Serial.println(Temp_Data[1]);
  Motor_Trace();
}
