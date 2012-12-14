#include "Framework.h"

// TODO
// Add handle NULL check

void ButtonInitHandle(BUTTON_HANDLE *handle, BYTE (*IOReadHandle)(void))
{
    handle->TriggerType   = BUTTON_TRIGGER_TYPE_PRESS;
    handle->DebounceDelay = (TICK_SECOND/20);
    handle->ReleaseDelay  = (TICK_SECOND/50);
    handle->HoldDelay     = (TICK_SECOND/2);
    handle->PressHandle   = NULL;
    handle->HoldHandle    = NULL;
    handle->ReadHandle    = IOReadHandle;

    // State Handle
    handle->State         = BUTTON_STATE_RELEASED;
    handle->MetaState     = BUTTON_META_NONE;
    handle->LastPressTimestamp   = 0;
    handle->LastReleaseTimestamp = 0;
}
void ButtonSetTriggerType(BUTTON_HANDLE *handle, BUTTON_TRIGGER_TYPE type)
{
    handle->TriggerType     = type;
}
void ButtonSetDebounceDelay(BUTTON_HANDLE *handle, DWORD delay)
{
    handle->DebounceDelay   = delay;
}
void ButtonSetHoldDelay(BUTTON_HANDLE *handle, DWORD delay)
{
    handle->HoldDelay       = delay;
}
void ButtonRegButtonPressHandle(BUTTON_HANDLE *handle, void (*func)(void))
{
    handle->PressHandle     = func;
}
void ButtonRegButtonHoldHandle(BUTTON_HANDLE *handle, void (*func)(void))
{
    handle->HoldHandle      = func;
}

void ButtonTick(BUTTON_HANDLE *handle)
{
    if ((*handle->ReadHandle)())
    {
        // Button is Pressed

        if (handle->State == BUTTON_STATE_RELEASED)
        {
            if (handle->MetaState == BUTTON_META_PRESSED)
            {
                // Button is in meta pressed state, check if button has been held down long enough
                if ((TickGet() - handle->LastPressTimestamp) > handle->DebounceDelay)
                {
                    // Button has been held down long enough, change state to Pressed
                    handle->State     = BUTTON_STATE_PRESSED;
                    handle->MetaState = BUTTON_META_NONE;
                    
                    if (handle->TriggerType == BUTTON_TRIGGER_TYPE_PRESS)
                    {
                        // Button Trigger type is press, invoke callback
                        (*handle->PressHandle)();
                    }
                }
            }
            else
            {
                // Button is just pressed, begin debounce
                // Change button state to a meta pressed state and record timestamp
                handle->MetaState          = BUTTON_META_PRESSED;
                handle->LastPressTimestamp = TickGet();
            }
        }
        else if ((handle->State == BUTTON_STATE_PRESSED)&&((handle->TriggerType == BUTTON_TRIGGER_TYPE_HOLD)||(handle->TriggerType == BUTTON_TRIGGER_TYPE_RELEASE_AND_HOLD)||(handle->TriggerType == BUTTON_TRIGGER_TYPE_RELEASE_AND_HOLD_RELEASE)))
        {
            // Button is configured to recognised Hold events
            // Check if the button has been held down long enough to trigger a Hold event
            if ((TickGet() - handle->LastPressTimestamp) > handle->HoldDelay)
            {
                // Button has been held down long enough for a hold event, change state to Hold
                handle->State = BUTTON_STATE_HOLD;

                // Invoke hold callback if configured
                if ((handle->TriggerType == BUTTON_TRIGGER_TYPE_HOLD)||(handle->TriggerType == BUTTON_TRIGGER_TYPE_RELEASE_AND_HOLD))
                {
                    (*handle->HoldHandle)();
                }
            }
        }
    }
    else
    {
        // Button is not pressed

        // Current button state is not released, check if state needs to be changed
        if (handle->State != BUTTON_STATE_RELEASED)
        {
            // Check button meta state
            if (handle->MetaState == BUTTON_META_RELEASED)
            {
                // Button is in a meta released state being debounced therefore
                // Check if the release delay has been exceeded
                if ((TickGet() - handle->LastReleaseTimestamp) > handle->ReleaseDelay)
                {
                    // Release delay has been exceeded, it is determined that the button was released
                    // Check condition to determine what to do next
                    if      (handle->State == BUTTON_STATE_PRESSED)
                    {
                        // Button was in a Pressed state, trigger callback according to trigger type
                        if ((handle->TriggerType == BUTTON_TRIGGER_TYPE_RELEASE)||(handle->TriggerType == BUTTON_TRIGGER_TYPE_RELEASE_AND_HOLD)||(handle->TriggerType == BUTTON_TRIGGER_TYPE_RELEASE_AND_HOLD_RELEASE))
                        {
                            (*handle->PressHandle)();
                        }
                    }
                    else if (handle->State == BUTTON_STATE_HOLD)
                    {
                        // Button was in a Hold state, trigger callback according to trigger type
                        if ((handle->TriggerType == BUTTON_TRIGGER_TYPE_HOLD_RELEASE)||(handle->TriggerType == BUTTON_TRIGGER_TYPE_RELEASE_AND_HOLD_RELEASE))
                        {
                            (*handle->HoldHandle)();
                        }
                    }

                    // Set Button state to Released and Meta State to None
                    handle->State     = BUTTON_STATE_RELEASED;
                    handle->MetaState = BUTTON_META_NONE;
                }
            }
            else
            {
                // Button is in a pressed or hold state, seems like the button is released
                // Start release delay check
                handle->MetaState            = BUTTON_META_RELEASED;
                handle->LastReleaseTimestamp = TickGet();
            }
            
        }
        else
        {
            // Button state is released, reset MetaState to None if in meta pressed state
            // This ensures that the delay counter for press debouce does not continue 
            // accumulating despite having intermittent releases
            if (handle->MetaState == BUTTON_META_PRESSED)
            {
                handle->MetaState = BUTTON_META_NONE;
            }
        }
        
    }
}


