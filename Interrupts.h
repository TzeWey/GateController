#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

void __attribute__((interrupt, no_auto_psv)) _OscillatorFail(void)
{
    static UINT32 x = 0;
    while (1)
    {
        x = 100000;
        while (x--) asm("nop");
        LEDG ^= 1;
        LEDR = !LEDG;
    }
}

void __attribute__((interrupt, no_auto_psv)) _AddressError(void)
{
    asm("nop");
    asm("nop");
    asm("nop");
    static UINT32 x = 0;
    while (1)
    {
        x = 100000;
        while (x--) asm("nop");
        LEDG ^= 1;
        LEDR = !LEDG;
        LEDY = LEDG;
    }
}

void __attribute__((interrupt, no_auto_psv)) _StackError(void)
{
    static UINT32 x = 0;
    while (1)
    {
        x = 500000;
        while (x--) asm("nop");
        LEDG ^= 1;
        LEDR = !LEDG;
    }
}

void __attribute__((interrupt, no_auto_psv)) _MathError(void)
{
    static UINT32 x = 0;
    while (1)
    {
        x = 100000;
        while (x--) asm("nop");
        LEDG ^= 1;
        LEDR = !LEDG;
    }
}

void __attribute__((interrupt, no_auto_psv)) _DefaultInterrupt(void)
{
    static UINT32 x = 0;
    while (1)
    {
        x = 500000;
        while (x--) asm("nop");
        LEDG ^= 1;
        LEDR = LEDG;
    }
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    // Reset interrupt flag
    TickUpdate();
    IFS0bits.T1IF = 0;    
}

void __attribute__((interrupt, no_auto_psv)) _QEI1Interrupt(void)
{
    IFS3bits.QEI1IF = 0; // Clear QEI interrupt
    QEIInterruptHandler();
}


#endif
