#include "System.h"
#include "Peripheral.h"

void PeripheralInit(void)
{
    HSPWMInit();
    QEIInit();
    ADCInit();
}
