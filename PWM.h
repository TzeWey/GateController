#ifndef __PWM_H
#define __PWM_H

// Edit Starts here
#define SCALE_RES 1000

#define OC1_TMR   2 // EN1
#define OC2_TMR   2 // EN2
#define OC3_TMR   0 // NONE
#define OC4_TMR   0 // NONE
#define OC5_TMR   0 // NONE

#define TMR2_PRES 1     // 1:1 Prescale
#define TMR2_FREQ 20000 // Hz

#define TMR3_PRES 0     // 1:8 Prescale
#define TMR3_FREQ 0     // Hz
// Edit Ends here

void PWMInit(void);

UINT PWMDutyGet(UINT8 pwm);
void PWMDutySet(UINT8 pwm, FLOAT duty);
void PWMDutySetScale(UINT8 pwm, UINT16 value, UINT16 scale);

#endif
