#ifndef __FRAMEWORK_H
#define __FRAMEWORK_H

#ifndef __SYSTEM_H
#include "System.h"
#endif
#ifndef __PERIPHERAL_H
#include "Peripheral.h"
#endif

#include "Button.h"

// Button Handles
extern BUTTON_HANDLE Button1;
extern BUTTON_HANDLE Button2;
extern BUTTON_HANDLE Button3;

void FrameworkInit(void);
void FrameworkTick(void);

#endif
