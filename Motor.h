#ifndef __MOTOR_H
#define __MOTOR_H

void MotorStop(void);
void MotorSetPWM(INT16 speed);

void MotorTick(void);
void MotorInit(void);

#endif
