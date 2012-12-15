#include "Framework.h"
#include "Device.h"

static volatile BYTE EncoderDir   = 1;
static volatile long EncoderCount = 0;
static volatile long EncoderTriggerCount = 10;
static void (*TriggerHandle)(void) = NULL;

void EncoderRegTriggerHandle(void (*func)(void))
{
    TriggerHandle = func;
}

void EncoderSetCountUp(BYTE up)
{
    EncoderDir = up;
}

void EncoderSetCount(long count)
{
    EncoderCount = count;
}

void EncoderSetTrigger(long count)
{
    EncoderTriggerCount = count;
}

long EncoderGetCount(void)
{
    return EncoderCount;
}

long EncoderGetRemaining(void)
{
    return (EncoderTriggerCount - EncoderCount);
}

void EncoderTick(void)
{
    
}

void EncoderInit(void)
{
    EncoderCount = 0;
}

void EncoderInterrupt(void)
{

    // Increment or decrement encoder
    if (EncoderDir == 1)
    {   // Increment
        EncoderCount += 1;
    }
    else
    {   // Decrement
        EncoderCount -= 1;
    }
    
    // Trigger callback if count reached, check handle is valid before trying
    if ((TriggerHandle != NULL) && (EncoderCount == EncoderTriggerCount))
    {
        (*TriggerHandle)();
    }    
}
