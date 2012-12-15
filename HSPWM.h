#ifndef __HSPWM_H
#define __HSPWM_H

#define HSPWM1    1
#define HSPWM2    2
#define HSPWM3    3
#define HSPWM4    4
#define HSPWM5    5
#define HSPWM6    6

#define PWM_DIV      2   // 1:4 Clock Divide
#define PWM_MAX  11992   // (100MHz * 8 * (1/20KHz)) / PWM_DIV) - 8

void HSPWMInit(void);
BYTE HSPWMIsEnabled(void);
void HSPWMEnable(BYTE enable);

UINT16 HSPWMDutyGet(UINT8 pwm);
void   HSPWMDuty(UINT8 pwm, FLOAT duty);
void   HSPWMDutyInt(UINT8 pwm, UINT16 duty);

void   HSPWMDutySetScale(UINT8 pwm, UINT16 value, UINT16 scale);

#endif
