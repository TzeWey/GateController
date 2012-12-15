#include "GateFunctions.h"

#define GATE_MAX_COUNT      25000

GATE_STATUS GateStatus;

BYTE GateIsNewState(void)
{
    BYTE response;
    // Buffer response
    response = GateStatus.GateStatusNewState;
    // Clear flag
    GateStatus.GateStatusNewState = 0;
    return response;
}

void GateMove(GATE_MOVE_DIR dir, GATE_MOVE_SPEED speed, INT32 trigger)
{
    QEISetTrigger(trigger);
    
    GateStatus.GateStatusIsMoving = 1;
    
    switch (dir)
    {
    case OPEN:
        if (speed == FAST)
        {
            MotorSetPWM(GateStatus.GateSpeedFast);
            GateStatus.GateStatusSpeed = FAST;
        }
        else
        {
            MotorSetPWM(GateStatus.GateSpeedSlow);
            GateStatus.GateStatusSpeed = SLOW;
        }
        break;

    case CLOSE:
        if (speed == FAST)
        {
            MotorSetPWM(-GateStatus.GateSpeedFast);
            GateStatus.GateStatusSpeed = FAST;
        }
        else
        {
            MotorSetPWM(-GateStatus.GateSpeedSlow);
            GateStatus.GateStatusSpeed = SLOW;            
        }
        break;

    }
}
void GateStop(void)
{
    MotorSetPWM(0);
    GateStatus.GateStatusIsMoving = 0;
}
void GateStopFast(void)
{
    MotorStop();
    GateStatus.GateStatusIsMoving = 0;
}


static void GateInitSettings(void)
{
    GateStatus.GateOpenEncoderCount           = 4000;

    // Speed to move at normally (Fast)
    GateStatus.GateSpeedFast                  = 8000;

    // Speed to move at when nearing ENDs (Slow)
    GateStatus.GateSpeedSlow                  = 5000;

    // Time to stop before making the gap
    GateStatus.GateGapStopDelay               = (TICK_SECOND);

    // Number of encoder counts for gap size
    GateStatus.GateGapEncoderCount            = 25;

    // Absolute distance to end to be run slowly
    GateStatus.GateSlowSpeedDistance          = 500;

    // Time between Jam detection samples
    GateStatus.GateJamTimeoutWindow           = (1 * TICK_SECOND);

    // Required encoder count delta in time window above before gate is considered jammed
    GateStatus.GateJamMinimumEncoderChange    = 5;

    // Number of encoder counts to END when gate jammed if closing to be considered for auto offset
    GateStatus.GateJamCloseAutoOffsetDelta    = 50;

    // Time to be Idle before the PWM generators are disabled to save power
    GateStatus.MotorIdleDelayTimeout          = (5 * TICK_SECOND);

    // The ADC value for Over Current Condition
    GateStatus.MotorOverCurrentADCValue       = 400;

    // PWM to run when Motor has OCed out of 10000
    GateStatus.MotorOverCurrentPWM            = 4000;


    // Init Status Flags
    GateStatus.GateStatusErrorFlag            = 0;
    GateStatus.GateStatusNewState             = 0;
}

void GateFunctionsInit(void)
{
    // Init Settings
    GateInitSettings();

}





