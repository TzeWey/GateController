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
            MotorSetPWM(GateStatus.GateSpeedFast); //GateStatus.GateSpeedFast);
            GateStatus.GateStatusSpeed = FAST;
        }
        else
        {
            MotorSetPWM(GateStatus.GateSpeedSlow); //GateStatus.GateSpeedSlow);
            GateStatus.GateStatusSpeed = SLOW;
        }
        break;

    case CLOSE:
        if (speed == FAST)
        {
            MotorSetPWM(-(GateStatus.GateSpeedFast)); //GateStatus.GateSpeedFast);
            GateStatus.GateStatusSpeed = FAST;
        }
        else
        {
            MotorSetPWM(-(GateStatus.GateSpeedSlow)); //GateStatus.GateSpeedSlow);
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

void GateUpdateState(GATE_STATE newState)
{
    GateStatus.PreviousState  = GateStatus.CurrentState;
    GateStatus.CurrentState   = newState;

    // Indicate new state
    GateStatus.GateStatusNewState = 1;
}
void GateUpdateStateDelay(GATE_STATE newState, DWORD delay)
{
    GateStatus.PreviousState  = GateStatus.CurrentState;
    GateStatus.CurrentState   = GATE_STATE_DELAY;

    GateStatus.NextStateDelay = delay;
    GateStatus.NextState      = newState;
    
    // Indicate new state
    GateStatus.GateStatusNewState = 1;
}
void GateUpdateStateDelayResume(void)
{
    // Update current state to the next state without updating previous state
    // this prevents GATE_STATE_DELAY being the previous state
    GateStatus.CurrentState   = GateStatus.NextState;

    // Indicate new state
    GateStatus.GateStatusNewState = 1;
}

void GateErrorClear(void)
{
    GateStatus.GateStatusErrorFlag = 0;
}

static volatile DWORD GateJamLastWindowTick   = 0;
static volatile INT32 GateJamLastEncoderValue = 0;
static void (*GateJamEventHandle)(void) = NULL;
void GateJamRegEventHandle(void (*func)(void))
{
    GateJamEventHandle = func;
}
void GateJamTick(void)
{
    if (GateStatus.GateStatusIsMoving)
    {
        // Gate is moving, check for jam
        if ((TickGet() - GateJamLastWindowTick) > GateStatus.GateJamTimeoutWindow)
        {
            // Check if count has changed
            INT32 EncoderDiff = QEIEncoderCountGet() - GateJamLastEncoderValue;

            LEDY ^= 1;

            if (((EncoderDiff <= 0) && (EncoderDiff > -(GateStatus.GateJamMinimumEncoderChange))) || ((EncoderDiff > 0) && (EncoderDiff < GateStatus.GateJamMinimumEncoderChange)))
            {
                // Gate encoder count has not changed much, assume jammed
                GateStatus.GateStatusErrorFlag = 1;
                (*GateJamEventHandle)();
            }

            // Save current encoder value
            GateJamLastEncoderValue = QEIEncoderCountGet();
            GateJamLastWindowTick = TickGet();
        }
    }
    else
    {
        // Gate is not moving, reset jam detect window
        GateJamLastEncoderValue  = QEIEncoderCountGet();
        GateJamLastWindowTick    = TickGet();
    }
}

void GateMakeGap(void)
{
    // Set current position as zero for correct gap size
    QEIEncoderCountReset();
    GateUpdateStateDelay(GATE_STATE_MAKE_GAP, GateStatus.GateGapStopDelay);
}

void GateLearningStart(void)
{
    GateUpdateState(GATE_STATE_LEARNING_CLOSE);
}
void GateLearningSetOpen(void)
{
    // Stop gate
    GateStopFast();
    // set current position as open limit
    GateStatus.GateOpenEncoderCount = QEIEncoderCountGet();
    // update state to Open
    GateUpdateState(GATE_STATE_OPEN);
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
    GateStatus.GateJamTimeoutWindow           = (TICK_SECOND / 2);

    // Required encoder count delta in time window above before gate is considered jammed
    GateStatus.GateJamMinimumEncoderChange    = 10;

    // Number of encoder counts to END when gate jammed if closing to be considered for auto offset
    GateStatus.GateJamCloseAutoOffsetDelta    = 50;

    // Time to be Idle before the PWM generators are disabled to save power
    GateStatus.MotorIdleDelayTimeout          = (5 * TICK_SECOND);

    // The ADC value for Over Current Condition
    GateStatus.MotorOverCurrentADCValue       = 400;

    // PWM to run when Motor has OCed out of 10000
    GateStatus.MotorOverCurrentPWM            = 4000;

    // Time between ramp
    GateStatus.MotorSmoothRampRate            = (TICK_SECOND / 100);


    // Init Status Flags
    GateStatus.GateStatusErrorFlag            = 0;
    GateStatus.GateStatusNewState             = 0;

    GateStatus.CurrentState                   = GATE_STATE_STOP;
    GateStatus.PreviousState                  = GATE_STATE_STOP;
}

void GateFunctionsInit(void)
{
    // Init Settings
    GateInitSettings();

}





