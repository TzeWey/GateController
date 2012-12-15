#include "Peripheral.h"

void HSPWMInit(void)
{
    PWMCON5bits.ITB     = 0;          // PTPER provides the PWM time period value @ 120MHz
    PWMCON1bits.ITB     = 0;
   
    PTCON2bits.PCLKDIV  = PWM_DIV;

    // Load Period
    PTPER = PWM_MAX;

    PWMCON5bits.MDCS = 0;
    PWMCON1bits.MDCS = 0;

    // Setup IO Output

    // ALI
    IOCON5bits.PENH = 1; // PWM module controls PWMxH pin
    IOCON5bits.POLH = 1; // PWMxH pin is active-low

    // BLI
    IOCON1bits.PENL = 1; // PWM module controls PWMxL pin
    IOCON1bits.POLL = 0; // ! PWMxL pin is active-low

    PDC1 = 2000;
    PDC5 = 2000;
    
    // Enabled on Initialisation
    PTCONbits.PTEN  = 0;
}

BYTE HSPWMIsEnabled(void)
{
    return PTCONbits.PTEN;
}

void HSPWMEnable(BYTE enable)
{
    PTCONbits.PTEN  = enable;
}

//UINT16 HSPWMDutyGet(UINT8 pwm)
//{
////    switch (pwm)
////    {
////    case HSPWM1:
////        return PDC1;
////        break;
////    case HSPWM2:
////        return PDC2;
////        break;
////
////    default:
////        return 0;
////    }
//}

void HSPWMDuty(UINT8 pwm, FLOAT duty)
{
    unsigned int set;
    set = duty * (PWM_MAX + 1);

    switch (pwm)
    {
    case HSPWM1:
        PDC1 = set;
        break;
    case HSPWM2:
        PDC2 = set;
        break;
    case HSPWM3:
        PDC3 = set;
        break;
    case HSPWM4:
        PDC4 = set;
        break;
    case HSPWM5:
        PDC5 = set;
        break;
    case HSPWM6:
        PDC6 = set;
        break;
    }
}

//void HSPWMDutyInt(UINT8 pwm, UINT16 duty)
//{
////    if (duty > PWM_MAX)
////        duty = PWM_MAX;
////
////    switch (pwm)
////    {
////    case HSPWM1:
////        PDC1 = duty;
////        break;
////    case HSPWM2:
////        PDC2 = duty;
////        break;
////    }
//}

void HSPWMDutySetScale(UINT8 pwm, UINT16 value, UINT16 scale)
{
    HSPWMDuty(pwm, (FLOAT)value/(FLOAT)scale);
}


