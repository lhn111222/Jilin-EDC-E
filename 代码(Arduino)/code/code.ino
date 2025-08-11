#include <Servo.h>           // 声明调用Servo.h库
#include <SoftwareSerial.h> 
#include "motor.h"
#include "uart.h"
#include <TimerOne.h>
#include <Keypad.h>

/*

#include <Wire.h>
#include "Timer.h"                                                //时间操作系统头文件  本程序用作timeChange时间采集并处理一次数据

//--------------------------------------------------------------------------------------------------
#define UseOled 1                               //是否使用OLED
#if UseOled
    #include <Adafruit_SSD1306.h>
    Adafruit_SSD1306 display(4);
    int DispLoop = 1;
    int MinPixel = 30;
    int MaxPixel = 63;
    int DispPixel = 0;
    int DispDegrees = 0;
#endif

Timer timer;                                                //时间类

enum State {
  STATE_UPDATE_DISPLAY,
  STATE_PRINT_TIME
};

State currentState = STATE_UPDATE_DISPLAY;
unsigned long lastMillis1 = 0;  // 用于记录上次 millis() 的值
unsigned long lastMillis2 = 0;  // 用于记录上次 millis() 的值
const unsigned long interval1 = 200; // 200 毫秒间隔
const unsigned long interval2 = 500; // 500 毫秒间隔
//--------------------------------------------------------------------------------------------------

*/

#define LOW 80               //巡线速度
#define MAX 255               //全速状态速度   

//矩阵键盘------------------------------------------------------------------
unsigned char key;

const byte ROWS = 2; //两行
const byte COLS = 3; //三列

// 定义连接到Arduino的行列引脚
byte rowPins[ROWS] = {24, 26};       //行
byte colPins[COLS] = {28, 30, 32};  //列

// 定义键盘布局
char keys[ROWS][COLS] = 
{
  {3, 2, 1},
  {7, 6, 5}
};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//矩阵键盘------------------------------------------------------------------

unsigned int Temp_Data[2] = {0};       //数据缓存区
unsigned int Dog = 0; //看门狗变量
unsigned char Text_flag = 0;
unsigned char Key_Val,Key_Old,Key_Down,Key_Up;
unsigned char SV_flag = 0;  //互斥标志位
unsigned char EV_flag = 0;
unsigned char Stop_flag = 0;
unsigned char Oled_flag = 0;
unsigned char Suo_flag = 0; //结束时while(1)是否执行 0是 1否

Servo dj;  // 创建一个Servo类的对象来控制舵机

//函数定义
void Key()
{
	Key_Val = key;
	Key_Down = Key_Val & (Key_Val ^ Key_Old);
	Key_Up =  ~Key_Val & (Key_Val ^ Key_Old);
	Key_Old = Key_Val;
	
	switch(Key_Down)
	{
		case 1://第一题
        tuigan(2);//增轮距
        delay(4350);
        tuigan(0);
        Text_flag = 1;
        Suo_flag = 1;
			break;
		case 2:
        Fast_go();
        delay(1500);
        Stop();
			break;
		case 3:

			break;
		case 5:

			break;
    case 6://增轮距
			tuigan(2);
      delay(4300);
      tuigan(0);
			break;
		case 7://缩轮距
      tuigan(1);
      delay(4350);
      tuigan(0);
			break;
	}
}

//**************************************************************************************//
void setup()
{
  /*
   Serial.begin(9600);
     millis();
     #if UseOled
         display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
         display.display();
         display.clearDisplay();
    #endif
    */
  //LED
  Led(1);
  //Beep
  Beep_Init();
  Beep(0);
  //舵机初始化
  dj.attach(9);  // 将舵机的信号线连接到数字口9
  dj.write(0);  // 初始位置设为90度
  //Voice(1);
  // 配置Timer1
  Serial.begin(9600);
  // 设置定时器中断回调函数
  //Timer1.initialize(1000); // 设置定时器周期为1000微秒（1毫秒）
  //Timer1.attachInterrupt(timerIsr); // 将timerIsr函数绑定为中断服务例程

  Motor_Init();
  uart_Init();
}

//**************************************************************************************//
void loop()
{
  //数据读取区域-------------------------------

  //timer.update();     //oled

  Read_Data(Temp_Data); //灰度传感器
  Read_IO();//IO读取
  IO_j();//IO判断
  key = keypad.getKey(); //矩阵键盘
  //Motor_Trace();
  //串口打印区域-------------------------------

  //Serial.println(Temp_Data[0]);//灰度数据
/*
  if (key)
  {
    Serial.println(key);
  }
*/
  //信息处理区域-------------------------------
  Key();

  switch(Text_flag)//循环执行
  {
    case 1://不改变Text_flag == 1 则一直执行
      if(SV_flag == 0)
      {
        Serial.println("模式1");
        Voice(1);
        SV_flag = 1;

        Go();
        delay(500);
        /*
        while(1) //起始右转弯
        {
          Start_go();
          Read_Data(Temp_Data);
          //Serial.println(Temp_Data[0]);//灰度数据
          if(Temp_Data[0] <= 124 && Temp_Data[0] >= 56)  //右转出库
          {
            Serial.println("右转直角弯出库");
            Start_right();
            Dog = 0;
            delay(700);
            break;
          }
        }
        */
      }
      //Serial.println(Temp_Data[0]);//灰度数据
      Read_Data(Temp_Data);
      Motor_Trace();
      if(Stop_flag == 1)
      {
        Stop();
        tuigan(1);  //缩轮距
        delay(4350);
        tuigan(0);
        while(1);
      }
      break;
    case 2://不改变Text_flag == 2 则一直执行
      if(SV_flag == 0)
      {
        Serial.println("模式2");
        SV_flag = 1;
        dj.write(65);   //放低音响

        tuigan(2);      //增加轮距     限高 宽走
        delay(4300);
        tuigan(0);

        Voice(1);

        Go();
        delay(500);
        /*
        while(1)//起始右转弯
        {
          Start_go();
          Read_Data(Temp_Data);
          //Serial.println(Temp_Data[0]);//灰度数据
          if(Temp_Data[0] <= 124 && Temp_Data[0] >= 56)  //右转出库
          {
            Serial.println("右转直角弯出库");
            Start_right();
            Dog = 0;
            delay(700);
            break;
          }
        }
        */
      }
      Read_Data(Temp_Data);
      Motor_Trace();
      break;
    case 3://不改变Text_flag == 3 则一直执行    限宽 窄走
      if(SV_flag == 0)
      {
        Serial.println("模式3");
        SV_flag = 1;

        tuigan(1);      //缩轮距 变窄
        delay(4350);
        tuigan(0);

        Voice(1); 

        Go();
        delay(500);
      }
      Read_Data(Temp_Data);
      Motor_Trace();
      break;
    case 4://不改变Text_flag == 4 则一直执行 钻管道
      if(SV_flag == 0)
      {
        tuigan(2);      //增加轮距     限高 宽走
        delay(4300);
        tuigan(0);

        Stop_flag = 1;
        Serial.println("模式4");
        dj.write(30);      //管道
        SV_flag = 1;
        Voice(1);
        Go();
        delay(500);
      }
      Read_Data(Temp_Data);
      Motor_Trace();
      ALLwhite();
      break;
  }
}
//**************************************************************************************//
//全白0  127

void timerIsr() //                         1ms /次
{
  /*看门狗程序---------------------------------*/
  /*if(++Dog > 1000) //大于1s 则 终止行进
  {
    Stop();
    Dog = 0;
    while(1)
    {
      Serial.println("-------汪-------");
    }
  }
  */
  /*------------------------------------------*/
  
}