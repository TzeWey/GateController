#include "System.h"
#include "Peripheral.h"
#include "Framework.h"
#include "Device.h"

#include "GateApplication.h"

#include "Interrupts.h"

DWORD StatusLastTick = 0;
DWORD ErrorLastTick = 0;

void Button1PressEvent(void)
{
    MotorStop();
}

void Button1HoldEvent(void)
{
    //GateLearnToggle();
}

void Button2PressEvent(void)
{
    GateToggle();
}

void Button3PressEvent(void)
{
    MotorSetPWM(3000);
}


int main(void)
{
    SystemInit();
    PeripheralInit();
    FrameworkInit();
    DeviceInit();

    GateInit();

    // Register Button Event handlers
    ButtonRegButtonPressHandle(&Button1, &Button1PressEvent);
    ButtonRegButtonHoldHandle (&Button1, &Button1HoldEvent);
    ButtonRegButtonPressHandle(&Button2, &Button2PressEvent);
    ButtonRegButtonPressHandle(&Button3, &Button3PressEvent);

    while (1)
    {
        FrameworkTick();
        DeviceTick();

        GateTick();

        if ((TickGet() - StatusLastTick) > TICK_SECOND)
        {
            LEDSTAT ^= 1;
            StatusLastTick = TickGet();
        }

        if (GateIsError())
        {
            if ((TickGet() - ErrorLastTick) > (TICK_SECOND / 4))
            {
                LEDR ^= 1;
                ErrorLastTick = TickGet();
            }
        }
        else
        {
            LEDR = 0;
        }
        
        // Clear Watchdog Timer
        //ClrWdt();
    }

    return 0;
}
