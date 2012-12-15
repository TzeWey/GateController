#ifndef __GATEAPP_H
#define __GATEAPP_H

#include "GateFunctions.h"

BYTE GateIsError(void);

void GateMakeGap(void);

void GateLearnToggle(void);
void GateToggle(void);

void GateInit(void);
void GateTick(void);

#endif
