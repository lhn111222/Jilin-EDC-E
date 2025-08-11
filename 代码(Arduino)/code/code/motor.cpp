#include "motor.h"
#include "uart.h"

#define IN1 53  //定义IN1为7口       第一个轮子
#define IN2 51   //定义IN2为6口
#define IN3 49  //定义IN3为4口       第二个轮子
#define IN4 47  //定义IN4为3口
#define IN5 45  //定义IN1为7口       000第一个轮子
#define IN6 43  //定义IN2为6口
#define IN7 41  //定义IN3为4口       第二个轮子
#define IN8 39  //定义IN4为3口

#define EN1 5   //定义EN1
#define EN2 4   //定义EN1
#define EN3 3   //定义EN1
#define EN4 2   //定义EN1

#define RIGHTTRACK 3711   //循迹右偏移量
#define LEFTTRACK 4039   //循迹左偏移量

#define SPEEDTRACK 150   //循迹的速度量

extern unsigned int Temp_Data[2];

/*********************************************************************
 *  函数名称：motor_Init
 *  函数功能：电机初始化
 *  形    参：无
 *  输    出：无
 *  备    注：无
 ********************************************************************/
void Motor_Init()
{
  pinMode(IN1,OUTPUT);    //轮子初始化
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(IN5,OUTPUT);
  pinMode(IN6,OUTPUT);
  pinMode(IN7,OUTPUT);
  pinMode(IN8,OUTPUT);

  pinMode(EN1,OUTPUT);    //轮子转速
  pinMode(EN2,OUTPUT);
  pinMode(EN3,OUTPUT);
  pinMode(EN4,OUTPUT);
}

/*********************************************************************
 *  函数名称：Stop
 *  函数功能：电机停止
 *  形    参：无
 *  输    出：无
 *  备    注：无
 ********************************************************************/
void Stop(void)    //停止
{
  digitalWrite(IN1,HIGH); 
  digitalWrite(IN2,HIGH);

  digitalWrite(IN3,HIGH); 
  digitalWrite(IN4,HIGH);

  digitalWrite(IN5,HIGH); 
  digitalWrite(IN6,HIGH);

  digitalWrite(IN7,HIGH); 
  digitalWrite(IN8,HIGH);
}

/*********************************************************************
 *  函数名称：Go
 *  函数功能：电机前进
 *  形    参：无
 *  输    出：无
 *  备    注：无
 ********************************************************************/
void Go(int space)    //直线行驶
{
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,HIGH);
  analogWrite(EN1,space); 

  digitalWrite(IN3,HIGH); 
  digitalWrite(IN4,LOW);
  analogWrite(EN2,space); 

  digitalWrite(IN5,LOW); 
  digitalWrite(IN6,HIGH);
  analogWrite(EN3,space); 

  digitalWrite(IN7,HIGH); 
  digitalWrite(IN8,LOW);
  analogWrite(EN4,space); 

}

/*********************************************************************
 *  函数名称：Right
 *  函数功能：电机向右
 *  形    参：无
 *  输    出：无
 *  备    注：无
 ********************************************************************/
void Right(int space)
{
  digitalWrite(IN1,HIGH); 
  digitalWrite(IN2,LOW);
  analogWrite(EN1,space); 

  digitalWrite(IN3,HIGH); 
  digitalWrite(IN4,LOW);
  analogWrite(EN2,space); 

  digitalWrite(IN5,LOW); 
  digitalWrite(IN6,HIGH);
  analogWrite(EN3,space); 

  digitalWrite(IN7,LOW); 
  digitalWrite(IN8,HIGH);
  analogWrite(EN4,space);
}

/*********************************************************************
 *  函数名称：Left
 *  函数功能：电机向左
 *  形    参：无
 *  输    出：无
 *  备    注：无
 ********************************************************************/
void Left(int space)
{
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,HIGH);
  analogWrite(EN1,space); 

  digitalWrite(IN3,LOW); 
  digitalWrite(IN4,HIGH);
  analogWrite(EN2,space); 

  digitalWrite(IN5,HIGH); 
  digitalWrite(IN6,LOW);
  analogWrite(EN3,space); 

  digitalWrite(IN7,HIGH); 
  digitalWrite(IN8,LOW);
  analogWrite(EN4,space);
}

/*********************************************************************
 *  函数名称：Back
 *  函数功能：电机后退
 *  形    参：无
 *  输    出：无
 *  备    注：无
 ********************************************************************/
void Back(int space)
{
  digitalWrite(IN1,HIGH); 
  digitalWrite(IN2,LOW);
  analogWrite(EN1,space); 

  digitalWrite(IN3,LOW); 
  digitalWrite(IN4,HIGH);
  analogWrite(EN2,space); 

  digitalWrite(IN5,HIGH); 
  digitalWrite(IN6,LOW);
  analogWrite(EN3,space); 

  digitalWrite(IN7,LOW); 
  digitalWrite(IN8,HIGH);
  analogWrite(EN4,space); 

}

void Motor_Trace(void)
{
  if(Temp_Data[0]!=0){
      if(Temp_Data[0] <= RIGHTTRACK){
        Right(SPEEDTRACK);
      }
      else if(Temp_Data[0] >= LEFTTRACK){
        Left(SPEEDTRACK);
      }
      else if(Temp_Data[0] > RIGHTTRACK && Temp_Data[0] < LEFTTRACK){
        Go(SPEEDTRACK);
      }
      else{
        Stop();
      }

  }

}
