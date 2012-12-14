#ifndef __OSCILLATOR_H
#define __OSCILLATOR_H

void OscillatorInit(void)
{
    /* Primary Oscillator Setup*/
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    CLKDIVbits.PLLPRE  = 0;  // N1 = 2
    PLLFBD             = 18; // M  = 18+2 = 20
    CLKDIVbits.PLLPOST = 0;  // N2 = 2

    // Initiate Clock Switch to Primary Oscillator with PLL (NOSC = 0b011)
    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(0x01);

    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0b011);

    /* Auxiliary Oscillator Setup*/
    ACLKCONbits.SELACLK  = 0; // Primary PLL (Fvco) provides the source clock for the auxiliary clock divider
    ACLKCONbits.APSTSCLR = 7; // Divide Auxiliary click by 1

    // Wait for Primary PLL to lock
    while(OSCCONbits.LOCK!=1) {};
}

#endif
