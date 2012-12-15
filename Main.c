/*
	Made by Hion, kacau-ed by Brian
*/

#include "System.h"
#include "Peripheral.h"
#include "Framework.h"
#include "Device.h"

#include "GateApplication.h"
#include "GateFunctions.h"

#include "Interrupts.h"

DWORD StatusLastTick = 0;
DWORD ErrorLastTick = 0;

void Button1PressEvent(void)
{
    GateToggle();
    //MotorSetPWM(1000);
}

void Button2PressEvent(void)
{
    //MotorSetPWM(8000);
    GateCalibrateClose();
}

void Button3PressEvent(void)
{
    GateLearnToggle();
}

void Button3HoldEvent(void)
{
    MotorStop();
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
    ButtonRegButtonPressHandle(&Button2, &Button2PressEvent);
    ButtonRegButtonPressHandle(&Button3, &Button3PressEvent);
    ButtonRegButtonHoldHandle (&Button3, &Button3HoldEvent);

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

        if (GateStatus.GateStatusErrorFlag)
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
        ClrWdt();
    }

    return 0;
}
