#include "Peripheral.h"

void InputCaptureInit(void)
{
    // Setup IC1
    IC1CONbits.ICM = 0b001; // Capture mode, every Rising Edge

    // Setup Interrupt
    IPC0bits.IC1IP = 5;	// Interrupt priority 5 (High)
    IFS0bits.IC1IF = 0; // Clear interrupt flag
    IEC0bits.IC1IE = 1; // Enable Interrupt
    
}
