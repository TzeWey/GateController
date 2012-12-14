#include "Peripheral.h"

#define TMR2_PR    ((GetInstructionClock()/TMR2_PRES)/TMR2_FREQ)
#define TMR3_PR    ((GetInstructionClock()/TMR3_PRES)/TMR3_FREQ)

#if   (TMR2_PRES==1)
    #define TMR2_TCKPS 0
#elif (TMR2_PRES==8)
    #define TMR2_TCKPS 1
#elif (TMR2_PRES==64)
    #define TMR2_TCKPS 2
#elif (TMR2_PRES==256)
    #define TMR2_TCKPS 3
#elif (TMR2_PRES==0)
#else
    #error "TMR2 Invalid Prescale"
#endif

#if   (TMR3_PRES==1)
    #define TMR3_TCKPS 0
#elif (TMR3_PRES==8)
    #define TMR3_TCKPS 1
#elif (TMR3_PRES==64)
    #define TMR3_TCKPS 2
#elif (TMR3_PRES==256)
    #define TMR3_TCKPS 3
#elif (TMR3_PRES==0)
#else
    #error "TMR3 Invalid Prescale"
#endif

#if   (OC1_TMR==2)
    #define OC1_PR TMR2_PR
#elif (OC1_TMR==3)
    #define OC1_PR TMR3_PR
#elif (OC1_TMR==0)
#else
    #error "OC1 Invalid Timer [Timer 2 or Timer 3 Only]"
#endif

#if   (OC2_TMR==2)
    #define OC2_PR TMR2_PR
#elif (OC2_TMR==3)
    #define OC2_PR TMR3_PR
#elif (OC2_TMR==0)
#else
    #error "OC2 Invalid Timer [Timer 2 or Timer 3 Only]"
#endif

#if   (OC3_TMR==2)
    #define OC3_PR TMR2_PR
#elif (OC3_TMR==3)
    #define OC3_PR TMR3_PR
#elif (OC3_TMR==0)
#else
    #error "OC3 Invalid Timer [Timer 2 or Timer 3 Only]"
#endif

#if   (OC4_TMR==2)
    #define OC4_PR TMR2_PR
#elif (OC4_TMR==3)
    #define OC4_PR TMR3_PR
#elif (OC4_TMR==0)
#else
    #error "OC4 Invalid Timer [Timer 2 or Timer 3 Only]"
#endif

#if   (OC5_TMR==2)
    #define OC5_PR TMR2_PR
#elif (OC5_TMR==3)
    #define OC5_PR TMR3_PR
#elif (OC5_TMR==0)
#else
    #error "OC5 Invalid Timer [Timer 2 or Timer 3 Only]"
#endif

#if (OC1_TMR!=0)
#define OC1_CON (0b0000000000000110 | ((OC1_TMR-2) << 3));
#endif
#if (OC2_TMR!=0)
#define OC2_CON (0b0000000000000110 | ((OC2_TMR-2) << 3));
#endif
#if (OC3_TMR!=0)
#define OC3_CON (0b0000000000000110 | ((OC3_TMR-2) << 3));
#endif
#if (OC4_TMR!=0)
#define OC4_CON (0b0000000000000110 | ((OC4_TMR-2) << 3));
#endif
#if (OC5_TMR!=0)
#define OC5_CON (0b0000000000000110 | ((OC5_TMR-2) << 3));
#endif

void PWMInit(void)
{
    #if (OC1_TMR!=0)
    // Output Compare 1
    OC1CON = 0;                  // Turn off Output Compare Module
    OC1R   = 0x0;
    OC1RS  = 0x0;
    OC1CON = OC1_CON;
    #endif

    #if (OC2_TMR!=0)
    // Output Compare 2
    OC2CON = 0;                  // Turn off Output Compare Module
    OC2R   = 0x0;
    OC2RS  = 0x0;
    OC2CON = OC2_CON;
    #endif

    #if (OC3_TMR!=0)
    // Output Compare 3
    OC3CON = 0;                  // Turn off Output Compare Module
    OC3R   = 0x0;
    OC3RS  = 0x0;
    OC3CON = OC3_CON;
    #endif

    #if (OC4_TMR!=0)
    // Output Compare 4
    OC4CON = 0;                  // Turn off Output Compare Module
    OC4R   = 0x0;
    OC4RS  = 0x0;
    OC4CON = OC4_CON;
    #endif

    #if (OC5_TMR!=0)
    // Output Compare 5
    OC5CON = 0;                  // Turn off Output Compare Module
    OC5R   = 0x0;
    OC5RS  = 0x0;
    OC5CON = OC5_CON;
    #endif

    #if (TMR2_PRES!=0)
    // Timer 2 Setup
    PR2             = TMR2_PR;
    TMR2            = 0;
    T2CONbits.TCKPS = TMR2_TCKPS; // PreScale
    T2CONbits.TON   = 1; // Enable Timer
    #endif

    #if (TMR3_PRES!=0)
    // Timer 3 Setup
    PR3             = TMR3_PR;
    TMR3            = 0;
    T3CONbits.TCKPS = TMR3_TCKPS; // PreScale
    T3CONbits.TON   = 1; // Enable Timer
    #endif
}

UINT PWMDutyGet(UINT8 pwm)
{
    switch (pwm)
    {
        case 1:
            return OC1RS;
        case 2:
            return OC2RS;
        case 3:
            return OC3RS;
        case 4:
            return OC4RS;
        case 5:
            return OC5RS;
    }
    return 0;
}

void PWMDutySet(UINT8 pwm, FLOAT duty)
{
    switch (pwm)
    {
        case 1:
            #if (OC1_TMR!=0)
            OC1RS  = duty*(OC1_PR + 1);
            #endif
            break;
        case 2:
            #if (OC2_TMR!=0)
            OC2RS  = duty*(OC2_PR + 1);
            #endif
            break;
        case 3:
            #if (OC3_TMR!=0)
            OC3RS  = duty*(OC3_PR + 1);
            #endif
            break;
        case 4:
            #if (OC4_TMR!=0)
            OC4RS  = duty*(OC4_PR + 1);
            #endif
            break;
        case 5:
            #if (OC5_TMR!=0)
            OC5RS  = duty*(OC5_PR + 1);
            #endif
            break;
    }
}

void PWMDutySetScale(UINT8 pwm, UINT16 value, UINT16 scale)
{
    switch (pwm)
    {
        case 1:
            #if (OC1_TMR!=0)
            OC1RS  = (value*(OC1_PR + 1))/scale;
            #endif
            break;
        case 2:
            #if (OC2_TMR!=0)
            OC2RS  = (value*(OC2_PR + 1))/scale;
            #endif
            break;
        case 3:
            #if (OC3_TMR!=0)
            OC3RS  = (value*(OC3_PR + 1))/scale;
            #endif
            break;
        case 4:
            #if (OC4_TMR!=0)
            OC4RS  = (value*(OC4_PR + 1))/scale;
            #endif
            break;
        case 5:
            #if (OC5_TMR!=0)
            OC5RS  = (value*(OC5_PR + 1))/scale;
            #endif
            break;
    }
}

