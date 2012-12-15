#include "Peripheral.h"

static volatile INT32 EncoderCount;
static volatile INT32 EncoderTriggerCount = 10;

static void (*TriggerHandle)(void) = NULL;

BYTE QEIDirection(void)
{
    return QEI1CONbits.UPDN;
}
void QEIPosReset(void)
{
    POS1CNT = 0;
}
INT16 QEIPosRead(void)
{
    return POS1CNT;
}

void QEIInit(void)
{
    QEI1CONbits.QEIM     = 0b111;                   // Quadrature Encoder Interface enabled (x4 mode) with position counter reset by match
    QEI1CONbits.SWPAB    = 0;                       // Phase A and Phase B inputs are not swapped

    DFLT1CONbits.QEOUT   = 1;                       // Digital filter outputs enabled
    DFLT1CONbits.QECK    = 0b010;                   // Digital Filter Clock Divide Select bits 1:4

    MAX1CNT              = 1;       // Count and interrupt at this value
    IPC14bits.QEI1IP     = 4;                       // Interrupt priority 4 (low)
    IEC3bits.QEI1IE      = 1;                       // Enable QEI1 interrupt
    IFS3bits.QEI1IF      = 0;                       // Reset  QEI1 interrupt

    EncoderCount         = 0;                       // Encoder count start from zero
}

void QEIEncoderCountReset(void)
{
    EncoderCount = 0;
}

INT32 QEIEncoderCountGet(void)
{
    return EncoderCount;
}

void QEIRegTriggerHandle(void (*func)(void))
{
    TriggerHandle = func;
}

void  QEISetTrigger(INT32 count)
{
    EncoderTriggerCount = count;
}

void QEIInterruptHandler(void)
{
    LEDG ^= 1;

    if (QEI1CONbits.UPDN)
    {
        EncoderCount += 1;
    }
    else
    {
        EncoderCount -= 1;
    }

    // Trigger callback if count reached, check handle is valid before trying
    if ((TriggerHandle != NULL) && (EncoderCount == EncoderTriggerCount))
    {
        (*TriggerHandle)();
    }

}
