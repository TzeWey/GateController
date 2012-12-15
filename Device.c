#include "Framework.h"
#include "Device.h"
#include "Motor.h"

void DeviceInit(void)
{
    MotorInit();
    EncoderInit();
}

void DeviceTick(void)
{
    MotorTick();
    EncoderTick();
}

