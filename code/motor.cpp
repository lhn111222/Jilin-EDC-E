#include "motor.h"
#include "uart.h"
#include <Servo.h>
#include <Keypad.h>

//电机数字引脚
#define IN1 27   //定义IN1为7口 
#define IN2 29   //定义IN2为6口

#define IN3 33   //定义IN3为4口 
#define IN4 31   //定义IN4为3口

#define EN1 3    //定义EN1
#define EN2 2    //定义EN1

//推杆数字引脚
#define tuigan1 47    
#define tuigan2 49

#define EN3 3   //定义EN1
#define EN4 2   //定义EN1

/*
#define RIGHTTRACK 36 //循迹右偏移量
#define LEFTTRACK 2 //循迹左偏移量
*/

#define RIGHTTRACK 48 //循迹右偏移量
#define LEFTTRACK 7 //循迹左偏移量

#define ALLRIGHTTRACK 64 //极限右偏移量
#define ALLLEFTTRACK 1 //极限左偏移量

#define RSPEEDTRACK 190 //转弯-右轮PWM 循迹的速度量
#define RALLSPEEDTRACK 90  //右轮循迹的速度量

#define LSPEEDTRACK 190 //转弯-左轮PWM 循迹的速度量
#define LALLSPEEDTRACK 80  //左轮循迹的速度量

#define White_Temp  2 //全白参数
#define Black_Temp  0 //全黑参数

#define V1 34 //启动检测
#define V2 36 //结束检测

#define V3 38 //2
#define V4 40 //3
#define V5 42 //4

#define V6 35 //蜂鸣

#define LED 44 //Led
#define Bp 46  //Beep

extern unsigned int Temp_Data[2];
extern unsigned int Dog;
extern Servo dj;
extern unsigned char Text_flag;
extern unsigned char Stop_flag;
unsigned int White;       //全白标识变量
unsigned int Black;       //全黑标识变量
extern unsigned char Suo_flag;

bool V3_flag = LOW;
bool V4_flag = LOW;
bool V5_flag = LOW;

void Led(unsigned char x) //0亮 1灭
{
  if(x == 1)
    digitalWrite(LED,LOW);
  else
    digitalWrite(LED,HIGH);
}

void Read_IO()//语音模块IO检测
{
  V3_flag = digitalRead(V3);
  V4_flag = digitalRead(V4);
  V5_flag = digitalRead(V5);
}

void IO_pr()
{
  unsigned char flag = 1;
  if(flag == 1)
  {
    Serial.println(V3_flag);
    Serial.println(V4_flag);
    Serial.println(V5_flag);
    flag = 0;
  }
    
}
void IO_j()//IO读取
{
  if(V3_flag == HIGH)
  {
    Text_flag = 2;
    Serial.println("IO模式2");
  }
  else if(V4_flag == HIGH)
  {
    Text_flag = 3;
    Serial.println("IO模式3");
  }  
  else if(V5_flag == HIGH)
  {
    Text_flag = 4;
    Serial.println("IO模式4");
  }
}

 /******************************************************************************/

//推杆函数 1:缩减  2:伸长 3:停止工作（暂停）
void tuigan(unsigned char x)    
{
  if(x == 1)
  {
    digitalWrite(tuigan1,LOW);
    digitalWrite(tuigan2,HIGH);
  }
  else if(x == 2)
  {
    digitalWrite(tuigan1,HIGH); 
    digitalWrite(tuigan2,LOW);
  }
  else if(x == 0)
  {
    digitalWrite(tuigan1,LOW); 
    digitalWrite(tuigan2,LOW);
  }
}

void Delay500ms(unsigned char t)
{
  unsigned i = 0;
  while(i < t)
  {
    i++;
    delay(500);
    Dog = 0;
  }
}

//电机初始化
void Motor_Init()
{
  pinMode(IN1,OUTPUT);    //轮子初始化
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
 
  pinMode(EN1,OUTPUT);    //轮子转速
  pinMode(EN2,OUTPUT);
  
}

/*停止行进---------------------------------*/
void Stop(void) 
{
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,LOW);

  digitalWrite(IN3,LOW); 
  digitalWrite(IN4,LOW);

}

/*直线行驶---------------------------------*/
void Go() 
{
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,HIGH);
  analogWrite(EN1,LALLSPEEDTRACK); 

  digitalWrite(IN3,HIGH); 
  digitalWrite(IN4,LOW);
  analogWrite(EN2,RALLSPEEDTRACK);


}

/*右转---------------------------------*/
void Right()
{
   
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,HIGH);
  analogWrite(EN1,LSPEEDTRACK); 

  digitalWrite(IN3,LOW); 
  digitalWrite(IN4,HIGH);
  analogWrite(EN2,160); //右轮

}

/*左转---------------------------------*/
void Left()
{
  
  
  digitalWrite(IN1,HIGH); //左轮
  digitalWrite(IN2,LOW);
  analogWrite(EN1,160); 

  digitalWrite(IN3,HIGH); 
  digitalWrite(IN4,LOW);
  analogWrite(EN2,RSPEEDTRACK);

}


/*倒退---------------------------------*/
void Back()
{
  digitalWrite(IN1,HIGH); 
  digitalWrite(IN2,LOW);
  analogWrite(EN1,LSPEEDTRACK); 

  digitalWrite(IN3,LOW); 
  digitalWrite(IN4,HIGH);
  analogWrite(EN2,RSPEEDTRACK); 
}

void Start_go()
{
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,HIGH);
  analogWrite(EN1,60); 

  digitalWrite(IN3,HIGH); 
  digitalWrite(IN4,LOW);
  analogWrite(EN2,70);
}

void Duct_go()
{
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,HIGH);
  analogWrite(EN1,110); 

  digitalWrite(IN3,HIGH); 
  digitalWrite(IN4,LOW);
  analogWrite(EN2,120);
}

void Fast_go()
{
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,HIGH);
  analogWrite(EN1,220); 

  digitalWrite(IN3,HIGH); 
  digitalWrite(IN4,LOW);
  analogWrite(EN2,230);
}

void Start_right()
{
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,HIGH);
  analogWrite(EN1,150); 

  digitalWrite(IN3,LOW); 
  digitalWrite(IN4,HIGH);
  analogWrite(EN2,150); //右轮
}

/*灰度传感器信息处理函数---------------------------------*/
void Motor_Trace(void)
{
  unsigned char flag = 1;//互斥标志位

  if(Temp_Data[0]!=0)
  {
    if((Temp_Data[0] <= RIGHTTRACK) && (Temp_Data[0] >= LEFTTRACK) || Temp_Data[0] == 127)  //直行
    {
      Serial.println("直行");
      Go();
      Dog = 0;
    }
    else if(Temp_Data[0] >= RIGHTTRACK && Temp_Data[0] < 127)  //右转
    {
      if(Temp_Data[0] >= ALLRIGHTTRACK && Temp_Data[0] < 127)//回正
      {
        Serial.println("向右回正");
        Right();
        Dog = 0;
        delay(300);
      }
      else
      {
        Serial.println("右转");
        Right();
        Dog = 0;
        delay(110);
      }      
    }
    else if(Temp_Data[0] > 0 && Temp_Data[0] <= LEFTTRACK)  //左转
    {
       if(Temp_Data[0] < ALLLEFTTRACK)//回正
      {
        Serial.println("向左回正");
        Left();
        Dog = 0;
        delay(300);
      }
      else
      {
        Serial.println("左转");
        Left();
        Dog = 0;
        delay(110);
      }  
    }
    /*else if(Stop_flag == 0 && Temp_Data[0] == 127 || Temp_Data[0] == 63 ||Temp_Data[0] == 60)
    {
      Stop_flag = 1;
      Serial.println("全黑-出库");
      Start_right();
      Dog = 0;
      delay(700);
      Go();
      delay(100);
      Stop();
      // "播报语音" 车辆停止//
      Voice(2);
      Serial.println("结束了-黑");
    }
    */
  }
  else
  {
    if(Stop_flag == 0)
    {
      Stop_flag = 1;
      Serial.println("全白-出库");
      Stop();
      Voice(2);
      Serial.println("结束了- 白");
      if(Suo_flag == 0)
        {
          while(1);
        }
    }
  }
}
  /******************************************************************************/

void Start() //出法
{
  // "播报语音" 车辆开始行进//
    Right();
    Serial.println("开始了");
    Delay500ms(2);
}

void ALLwhite()
{
  if(Temp_Data[0] == 0)
  {
    Stop_flag = 0;
    Led(0);  //开灯
    Stop();  //停下准备缩轮距

    tuigan(1);      //缩轮距 变窄
    delay(4350);
    tuigan(0);

    Fast_go();
    delay(300);
    Duct_go();
    delay(800);
    Led(1);  //关灯
    Beep(1); //提示
    delay(200);
    Beep(0);
  }
}

void Voice(unsigned char n)
{
  switch(n)
  {
    case 1:
     digitalWrite(V1,HIGH);
     delay(1000);
     digitalWrite(V1,LOW);
     break;
    case 2:
     digitalWrite(V2,HIGH);
     delay(1000);
     digitalWrite(V2,LOW);
     break;
    case 3:
      digitalWrite(V6,HIGH);
      delay(1000);
      digitalWrite(V6,LOW);
     break;
  }
}

void duoji(unsigned char x)    //舵机 0:下放   1:抬起
{
  if(x == 0)
  {
    for (int pos = 0; pos <= 110; pos += 1) 
    { // 从0度到180度
      dj.write(pos);             // 命令舵机去到当前角度
      delay(15);                      // 等待15毫秒到达位置
    }
    delay(5000);
  }
  else if(x == 1)
  {
    for (int pos = 110; pos >= 0; pos -= 1) 
    { // 从180度回到0度
    dj.write(pos);             // 命令舵机去到当前角度
    delay(15);                      // 等待15毫秒到达位置
    }
    delay(5000);
  }
}

void Beep_Init()
{
  pinMode(Bp, OUTPUT);
}

void Beep(unsigned char flag)
{
  if(flag == 1)
    digitalWrite(Bp,LOW);
  else
    digitalWrite(Bp,HIGH);
}
/*开始||结束---------------------------------
void Start_End()
{
  if(Temp_Data[0] == 127)
  {
    if(++Black > 25)//结束
    {
      Right();
      delay(500);Dog = 0;delay(500);Dog = 0;delay(500);Dog = 0;delay(500);Dog = 0;
      Stop();
      While(1)
      {
        Serial.println("结束了");
      }
    }
    else if//开始
    {
      Right();
      Serial.println("开始了");
      delay(500);Dog = 0;delay(500);Dog = 0;delay(500);Dog = 0;delay(500);Dog = 0;
    }
  }
}
*/