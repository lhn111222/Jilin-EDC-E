#ifndef __MOTOR_H
#define __MOTOR_H

#include <Arduino.h>

void Motor_Init();
void Stop(void);
void Go();
void Right();
void Left();
void Back(int space);
void Motor_Trace(void);
void tuigan(unsigned char x);
void Duct();
void Delay500ms(unsigned char t);
void Start();
void Voice(unsigned char n);
void Start_go();
void Start_right();
void Read_IO();
void IO_j();
void IO_pr();
void Led(unsigned char x);
void ALLwhite();
void Beep(unsigned char flag);
void Beep_Init();
void Fast_go();
void duoji(unsigned char x);
void Duct_go();
#endif