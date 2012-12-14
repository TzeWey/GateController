#include "Framework.h"

// Button Handles
BYTE BTN1Read(void){return BTN1;}
BYTE BTN2Read(void){return BTN2;}
BYTE BTN3Read(void){return BTN3;}

BUTTON_HANDLE Button1;
BUTTON_HANDLE Button2;
BUTTON_HANDLE Button3;

void FrameworkInit(void)
{
    // Button1 Setup
    ButtonInitHandle(&Button1, &BTN1Read);
    ButtonSetTriggerType(&Button1, BUTTON_TRIGGER_TYPE_RELEASE_AND_HOLD);
   
    // Button2 Setup
    ButtonInitHandle(&Button2, &BTN2Read);
    ButtonSetTriggerType(&Button2, BUTTON_TRIGGER_TYPE_PRESS);

    // Button3 Setup
    ButtonInitHandle(&Button3, &BTN3Read);
    ButtonSetTriggerType(&Button3, BUTTON_TRIGGER_TYPE_PRESS);
}

void FrameworkTick(void)
{
    ButtonTick(&Button1);
    ButtonTick(&Button2);
    ButtonTick(&Button3);
}

