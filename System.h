#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "HardwareProfile.h"
#include "DataTypes.h"

#include "Tick.h"

#include "DataEE16.h"

// System Functions
void   SystemInit(void);
UINT32 MemReadLatch(UINT8 page, UINT16 addrLo);

// DataEE16
UINT16 EEPROMRead(UINT16 address);
UINT8  EEPROMWrite(UINT16 address, UINT16 data);

#endif
