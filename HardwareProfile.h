#ifndef __HARDWARE_PROFILE_H
#define __HARDWARE_PROFILE_H

#include "Compiler.h"

// Clock frequency values
// These directly influence timed events using the Tick module.  They also are used for UART and SPI baud rate generation.
#define GetSystemClock()        60*MHz                 // Fosc
#define GetPeripheralClock()    GetSystemClock()       // Fosc
#define GetInstructionClock()   (GetSystemClock() / 2) // Fcy

// UI IOs
#define LEDSTAT         LATGbits.LATG9
#define LEDSTAT_TRIS    TRISGbits.TRISG9

#define LEDG            LATEbits.LATE7  // LED1
#define LEDG_TRIS       TRISEbits.TRISE7

#define LEDY            LATGbits.LATG6  // LED2
#define LEDY_TRIS       TRISGbits.TRISG6

#define LEDR            LATGbits.LATG7  // LED3
#define LEDR_TRIS       TRISGbits.TRISG7

#define BTN1            !PORTEbits.RE4
#define BTN2            !PORTEbits.RE5
#define BTN3            !PORTEbits.RE6

// Gate Driver
#define DIS             LATDbits.LATD5
#define DIS_TRIS        TRISDbits.TRISD5

#define AHI             LATDbits.LATD6
#define AHI_TRIS        TRISDbits.TRISD6

#define ALI             LATDbits.LATD7
#define ALI_TRIS        TRISDbits.TRISD7

#define BHI             LATEbits.LATE1
#define BHI_TRIS        TRISEbits.TRISE1

#define BLI             LATEbits.LATE0
#define BLI_TRIS        TRISEbits.TRISE0

// Encoder

#define ENCA            PORTDbits.RD11
#define ENCB            PORTDbits.RD0

#endif
