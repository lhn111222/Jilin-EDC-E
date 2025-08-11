#ifndef __MOTOR_H
#define __MOTOR_H

#include <Arduino.h>

void Motor_Init();
void Stop(void);
void Go(int space);
void Right(int space);
void Left(int space);
void Back(int space);
void Motor_Trace(void);

#endif
