#ifndef __BUTTON_H
#define __BUTTON_H

typedef enum
{
    BUTTON_STATE_RELEASED                        = 0,
    BUTTON_STATE_PRESSED                         = 1,
    BUTTON_STATE_HOLD                            = 2,
}BUTTON_STATE;

typedef enum
{
    BUTTON_META_NONE,
    BUTTON_META_PRESSED,
    BUTTON_META_RELEASED,
}BUTTON_META;

typedef enum
{
    BUTTON_TRIGGER_TYPE_PRESS                    = 0, // Trigger button event when  pressed
    BUTTON_TRIGGER_TYPE_RELEASE                  = 1, // Trigger button event when released
    BUTTON_TRIGGER_TYPE_HOLD                     = 2, // Trigger button event after holding
    BUTTON_TRIGGER_TYPE_HOLD_RELEASE             = 3, // Trigger button event after holding release
    BUTTON_TRIGGER_TYPE_RELEASE_AND_HOLD         = 4, // Trigger button event when released and also after holding release
    BUTTON_TRIGGER_TYPE_RELEASE_AND_HOLD_RELEASE = 5, // Trigger button event when released and also after holding
}BUTTON_TRIGGER_TYPE;

typedef struct
{
    // Configurations
    BUTTON_TRIGGER_TYPE TriggerType;
    DWORD DebounceDelay;
    DWORD ReleaseDelay;
    DWORD HoldDelay;
    void (*PressHandle)(void);
    void (*HoldHandle)(void);
    BYTE (*ReadHandle)(void);

    // State Handle
    BUTTON_STATE State;
    BUTTON_META  MetaState;
    DWORD        LastPressTimestamp;
    DWORD        LastReleaseTimestamp;
}BUTTON_HANDLE;

void ButtonInitHandle(BUTTON_HANDLE *handle, BYTE (*IOReadHandle)(void));

void ButtonSetTriggerType(BUTTON_HANDLE *handle, BUTTON_TRIGGER_TYPE type);
void ButtonSetDebounceDelay(BUTTON_HANDLE *handle, DWORD delay);
void ButtonSetHoldDelay(BUTTON_HANDLE *handle, DWORD delay);

void ButtonRegButtonPressHandle(BUTTON_HANDLE *handle, void (*func)(void));
void ButtonRegButtonHoldHandle(BUTTON_HANDLE *handle, void (*func)(void));

//void ButtonInit(void);
void ButtonTick(BUTTON_HANDLE *handle);

#endif



