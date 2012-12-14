#include "Peripheral.h"

typedef struct
{
    UINT16 Buffer[ADC_BUFFER_SIZE];
    BYTE   BufferIndex;
    BYTE   BufferCount;
}ADC_BUFFER_HANDLE;

void ADCBufferInit(ADC_BUFFER_HANDLE *handle)
{
    handle->BufferIndex = 0;
    handle->BufferCount = 0;
}
void ADCBufferAdd(ADC_BUFFER_HANDLE *handle, UINT16 value)
{
    handle->Buffer[handle->BufferIndex] = value;
    handle->BufferIndex++;
    if (handle->BufferIndex >= ADC_BUFFER_SIZE)
    {
        handle->BufferIndex = 0;
    }

    if (handle->BufferCount < ADC_BUFFER_SIZE)
    {
        handle->BufferCount++;
    }
}
UINT16 ADCBufferGet(ADC_BUFFER_HANDLE *handle)
{
    BYTE   i;
    UINT16 accumulator = 0;

    if (handle->BufferCount == 0)
    {
        return 0;
    }

    for (i=0;i<handle->BufferCount;i++)
    {
        accumulator += handle->Buffer[i];
    }
    return (accumulator/handle->BufferCount);
}


ADC_BUFFER_HANDLE ADC_SENSE;

UINT16 ADCGetCurrent(void)
{
    return ADCBufferGet(&ADC_SENSE);
}

void ADCInit(void)
{
    ADCONbits.SLOWCLK   = 0;    // Primary PLL is chosen as the input clock, High Speed Clock
    ADCONbits.ADCS      = 7;    // Clock Divider, 120M / 5 = 24M
    ADCONbits.FORM      = 0;    // Unsigned Integer Output Format
    ADCONbits.ORDER     = 0;    // Normal Order of Conversion
    ADCONbits.SEQSAMP   = 0;    // Simultaneous Sampling
    ADCONbits.ASYNCSAMP = 0;    // Synchronous Sampling

    ADCONbits.ADON      = 0;    // ADC Disabled on initialisation for power saving


    
    // Individual software trigger selected
    ADCPC3bits.TRGSRC7 = 1;    // Pair 7, SENSE

    // Pair 7 interrupt setup
    IPC29bits.ADCP7IP  = 0x02; // Set ADC Pair 7 Interrupt Priority (Level 2)
    IFS7bits.ADCP7IF   = 0;    // Clear ADC Pair 7 Interrupt Flag
    IEC7bits.ADCP7IE   = 0;    // Disable ADC Pair 7 Interrupt
    
}

void ADCOn(void)
{
    ADCONbits.ADON  = 1;    // Enable ADC Module
}
void ADCOff(void)
{
    ADCONbits.ADON  = 0;
}

void ADCStart(void)
{
    // Individual software trigger selected
    IFS7bits.ADCP7IF   = 0;    // Clear ADC Pair 7 Interrupt Flag
    IEC7bits.ADCP7IE   = 1;    // Enable ADC Pair 0 Interrupt

    // On ADC
    ADCOn();
    
    // Start ADC Loop
    ADCPC3bits.SWTRG7 = 1;
}
void ADCStop(void)
{
    ADCOff();
    IEC7bits.ADCP7IE   = 0;    // Disable ADC Pair 7 Interrupt    
}

// Pair 7, SENSE
void __attribute__((interrupt, no_auto_psv)) _ADCP7Interrupt(void)
{
    /* Interrupt Service Routine code goes here */
    IFS7bits.ADCP7IF = 0; // Clear ADC Pair 7 Interrupt Flag

    // DO READ HERE
    ADCBufferAdd(&ADC_SENSE, ADCBUF15);
        
    // Trigger Pair 0 Read again
    ADCPC3bits.SWTRG7 = 1;
}
