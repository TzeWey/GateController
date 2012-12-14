#include "System.h"

#include "Oscillator.h"

// DSPIC33FJ32GS406 Configuration Bit Settings

#include <p33Fxxxx.h>

// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (General Segment may be written)
#pragma config GSS = OFF                // General Segment Code Protection (General Segment Code protect is disabled)

// FOSCSEL
#pragma config FNOSC = PRIPLL           // Oscillator Source Selection (Primary Oscillator (XT, HS, EC) with PLL)
#pragma config IESO = OFF               // Internal External Switch Over Mode (Start up device with user-selected oscillator source)

// FOSC
#pragma config POSCMD = HS              // Primary Oscillator Source (HS Crystal Oscillator Mode)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function (OSC2 is clock output)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR2             // POR Timer Value (2ms)
#pragma config ALTSS1 = OFF             // Enable Alternate SS1 pin bit (SS1 is selected as the I/O pin for SPI1)
#pragma config ALTQIO = OFF             // Enable Alternate QEI1 pin bit (QEA1, QEB1, INDX1 are selected as inputs to QEI1)

// FICD
#pragma config ICS = PGD3               // Comm Channel Select (Communicate on PGC3/EMUC3 and PGD3/EMUD3)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is disabled)


void PinMapInit(void)
{
    TRISB = 0b1111111111111111; //
    TRISD = 0b1111111111111111; //
    TRISE = 0b1111111111111111; //
    TRISF = 0b1111111111111111; //
    TRISG = 0b1111111111111111; //

    // Set Outputs
    LEDSTAT_TRIS = 0;
    LEDG_TRIS    = 0;
    LEDY_TRIS    = 0;
    LEDR_TRIS    = 0;
    DIS_TRIS     = 0;
    AHI_TRIS     = 0;
    ALI_TRIS     = 0;
    BHI_TRIS     = 0;
    BLI_TRIS     = 0;

    // CN Setup, BTN1, BTN2, BTN3
    //CNPU1bits.CN5PUE = 1;
    //CNPU1bits.CN6PUE = 1;
    //CNPU1bits.CN7PUE = 1;

    // ADC Input Setup
    ADPCFG  = 0xFFFF;
    ADPCFGbits.PCFG15 = 0;   // SENSE
}

void SystemInit(void)
{
    // Initialise Primary Oscillator
    OscillatorInit();

    // Initialise Emulated EEPROM
    DataEEInit();
    DataEEFlags.value = 0;
   
    // Setup IO pins
    PinMapInit();

    // Start System Tick Timer on Timer 1
    TickInit();
}

UINT32 MemReadLatch(UINT8 page, UINT16 addrLo)
{
    UINT32_BIT temp;
    TBLPAG = page;
    temp.Word.W0 = __builtin_tblrdl(addrLo);
    temp.Word.W1 = __builtin_tblrdh(addrLo);
    return temp.Val;

//    UINT32 DevID;
//    UINT32 DevRev;
//    DevID = MemReadLatch(0xFF, 0x00);
//    DevRev = MemReadLatch(0xFF, 0x02);

}

UINT16 EEPROMRead(UINT16 address)
{
    unsigned int response;
    response = DataEERead(address);
    Nop();
    return response;
}
UINT8 EEPROMWrite(UINT16 address, UINT16 data)
{
    unsigned char response;
    response = DataEEWrite(data, address);
    Nop();
    return response;
}
