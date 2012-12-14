#include "GateApplication.h"

// The number of encoder counts to fully open gate
static volatile INT32 GateOpenCount = 20;

// Absolute distance to end to be run slowly
static volatile INT32 GateSlowCount = GATE_SLOW_COUNT;

// Number of encoder counts for gap size
static volatile INT32 GateGapCount  = GATE_GAP_COUNT;

static volatile DWORD GateLastJamTick      = 0;
static volatile INT32 GateLastEncoderValue = 0;

static volatile BYTE  GateFlagError = 0;

static volatile BYTE  GateIsSlow    = 0;

static volatile BYTE  GateMakeGapFlag  = 0;
static volatile DWORD GateMakeGapDelay = 0;

typedef enum
{
    GATE_STATE_OPEN  = 0,
    GATE_STATE_CLOSE = 1,
    GATE_STATE_STOP  = 2,
    GATE_STATE_OPENING,
    GATE_STATE_CLOSING,    
    GATE_STATE_CALIBRATING_CLOSE,
    GATE_STATE_LEARNING_CLOSE,
    GATE_STATE_LEARNING_OPEN,    
    GATE_STATE_MAKE_GAP,
} GATE_STATE;

static volatile GATE_STATE CurrentState  = GATE_STATE_STOP;
static volatile GATE_STATE PreviousState = GATE_STATE_STOP;

static void GateJamCheck(void);
static void GateJamReset(void);
static void GateStartLearning(void);
static void GateLearningSetOpen(void);

BYTE GateIsError(void)
{
    return GateFlagError;
}

static void GateUpdateState(GATE_STATE newState)
{
    PreviousState = CurrentState;
    CurrentState  = newState;
}
static void GateFast(void)
{
    GateIsSlow = 0;
    GateJamReset();
    switch (CurrentState)
    {
    case GATE_STATE_OPENING:
        MotorSetPWM(GATE_SPEED_FAST);
        break;

    case GATE_STATE_CLOSING: // Was in the midst of closing
        // set new state as closed
        MotorSetPWM(-GATE_SPEED_FAST);
        break;
        
    default:
        break;
    }
}
static void GateSlow(void)
{
    GateIsSlow = 1;
    GateJamReset();
    switch (CurrentState)
    {
    case GATE_STATE_OPENING:
        MotorSetPWM(GATE_SPEED_SLOW);
        break;

    case GATE_STATE_CALIBRATING_CLOSE:
    case GATE_STATE_CLOSING: // Was in the midst of closing
        // set new state as closed
        MotorSetPWM(-GATE_SPEED_SLOW);
        break;        
    
    case GATE_STATE_LEARNING_CLOSE:
        MotorSetPWM(-GATE_SPEED_SLOW);
        break;

    case GATE_STATE_LEARNING_OPEN:
        MotorSetPWM(GATE_SPEED_SLOW);
        break;

    case GATE_STATE_MAKE_GAP:
        MotorSetPWM(GATE_SPEED_SLOW);
        break;

    default:
        break;
    }
}
static void GateOpen(void)
{
    GateUpdateState(GATE_STATE_OPENING);
    GateFast();
    QEISetTrigger(GateOpenCount);
}
static void GateClose(void)
{
    GateUpdateState(GATE_STATE_CLOSING);
    GateFast();
    QEISetTrigger(0);
}

static void GateStop(void)
{
    GateIsSlow = 0;
    MotorSetPWM(0);
}
static void GateStopFast(void)
{
    GateIsSlow = 0;
    MotorStop();
}

void GateMakeGap(void)
{
    GateStopFast();
    
    GateMakeGapFlag = 1;
    GateMakeGapDelay = TickGet();
}

static void GateJammed(void)
{
    // Since it appears to be jammed, stop
    GateStopFast();
    
    switch (CurrentState)
    {
    case GATE_STATE_OPENING: // Was in the midst of opening
        // set gate as stopped, most probably the gate hit something
        GateUpdateState(GATE_STATE_STOP);
        break;

    case GATE_STATE_CLOSING: // Was in the midst of closing
        // Check if we are close to closing and adjust for CLOSE offset
        if (QEIEncoderCountGet() < GATE_JAM_CLOSE_TRESHOLD)
        {
            // It appears to be very close to closed (possible) or has passed closed (should not be possible)
            GateUpdateState(GATE_STATE_CALIBRATING_CLOSE);
            GateMakeGap(); // Make gap, gap code should know what to do next
        }
        else
        {
            // set gate as stopped, most probably the gate hit something
            GateUpdateState(GATE_STATE_STOP);
        }
        break;
        
    case GATE_STATE_CALIBRATING_CLOSE: // Was in the midst of calibrating close, this jam is expected
    case GATE_STATE_LEARNING_CLOSE: // Was in the midst of calibrating close, this jam is expected
        GateJamReset();
        GateMakeGap(); // Make gap, gap code should know what to do next
        break;

    case GATE_STATE_LEARNING_OPEN: // Was in the midst of learning open limit, this jam is expected
        GateJamReset();
        GateLearningSetOpen();
        break;

    default:
        GateUpdateState(GATE_STATE_STOP);
        break;
    }

}

static void GateJamCheck(void)
{
    // JAM DETECT
    if (HSPWMIsEnabled() && (CurrentState > GATE_STATE_STOP))
    {
        if ((TickGet() - GateLastJamTick) > GATE_JAM_TIMEOUT)
        {
            // Check if count has changed
            INT32 EncoderDiff = QEIEncoderCountGet() - GateLastEncoderValue;

            LEDY ^= 1;

            if (((EncoderDiff <= 0) && (EncoderDiff > -GATE_JAM_DELTA_TRESHOLD)) || ((EncoderDiff > 0) && (EncoderDiff < GATE_JAM_DELTA_TRESHOLD)))
            {
                
                // Gate encoder count has not changed much, assume jammed
                GateFlagError = 1;
                GateJammed();
            }
            
            // Save current encoder value
            GateLastEncoderValue = QEIEncoderCountGet();
            GateLastJamTick = TickGet();
        }

    }
}
static void GateJamReset(void)
{
    //LEDR ^= 1;
    GateLastJamTick = TickGet();
    GateLastEncoderValue = QEIEncoderCountGet();// + GATE_JAM_DELTA_TRESHOLD;
    GateFlagError = 0;
}

static void GateEncoderCountReachedEvent(void)
{
    // Count reached stop gate rapidly
    GateStopFast();

    switch (CurrentState)
    {
    case GATE_STATE_OPENING: // Was in the midst of opening
        // set new state as open
        GateUpdateState(GATE_STATE_OPEN);
        break;
        
    case GATE_STATE_CLOSING: // Was in the midst of closing
        // set new state as closed
        GateUpdateState(GATE_STATE_CLOSE);
        break;
        
    case GATE_STATE_MAKE_GAP:
        switch (PreviousState)
        {
        case GATE_STATE_CALIBRATING_CLOSE: // Was calibrating close
            // set new state as closed and set current position as zero
            QEIEncoderCountReset();
            GateUpdateState(GATE_STATE_CLOSE);           
            break;

        case GATE_STATE_LEARNING_CLOSE: // Was also calibrating close, but set to learn open size
            // set new state as learning open and set current position as zero
            QEIEncoderCountReset();
            GateUpdateState(GATE_STATE_LEARNING_OPEN);
            GateSlow();
            break;

        default:
            break;
        }

    default:
        break;
    }

}

static void GateStartLearning(void)
{
    GateUpdateState(GATE_STATE_LEARNING_CLOSE);
    GateSlow();
}

static void GateLearningSetOpen(void)
{
    // Stop gate
    GateStopFast();
    // set current position as open limit
    GateOpenCount = QEIEncoderCountGet();
    // update state to Open
    GateUpdateState(GATE_STATE_OPEN);
}

void GateLearnToggle(void)
{
    switch (CurrentState)
    {
    case GATE_STATE_LEARNING_OPEN: // It is trying to learn opening
        // set current position as open limit        
        GateLearningSetOpen();
        break;

    case GATE_STATE_LEARNING_CLOSE: // It is trying to learn closing
        // start learning open, assume gate has reached end
        GateStopFast();
        GateMakeGap();
        break;

    default: // was in some other state, start learn more
        GateStartLearning();
        break;
    }
    GateStartLearning();
}

void GateToggle(void)
{
    switch (CurrentState)
    {
    case GATE_STATE_STOP: // It is Stopped
        switch (PreviousState)
        {
        case GATE_STATE_OPENING: // It was last opening
            // so start closing
            GateUpdateState(GATE_STATE_CLOSING);
            GateClose();
            break;

        case GATE_STATE_CLOSING: // It was last closing
            // so start opening
            GateUpdateState(GATE_STATE_OPENING);
            GateOpen();
            break;

        default: // Initial state handler, this should only occur once at startup
            GateUpdateState(GATE_STATE_OPENING);
            GateOpen();
            break;
        }
        break;

    case GATE_STATE_OPEN: // It is open
        // so start closing
        GateUpdateState(GATE_STATE_CLOSING);
        GateClose();
        break;

    case GATE_STATE_CLOSE: // It is closed
        // so start opening
        GateUpdateState(GATE_STATE_OPENING);
        GateOpen();
        break;

    case GATE_STATE_OPENING: // It is in the midst of opening or closing
    case GATE_STATE_CLOSING:
        // so stop
        GateUpdateState(GATE_STATE_STOP);
        GateStop();
        break;

    case GATE_STATE_LEARNING_CLOSE:
    case GATE_STATE_LEARNING_OPEN:
        // pass press as learn button pressed
        GateLearnToggle();
        break;
        
    default:
        break;


    }
}

void GateInit(void)
{
    QEIRegTriggerHandle(&GateEncoderCountReachedEvent);
}

void GateTick(void)
{
    // Check encoder value, change speed to slow when reaching end
    switch (CurrentState)
    {
    case GATE_STATE_CLOSING:
        // Check distance left, if less than 500 ticks slow down
        if ((QEIEncoderCountGet() < GateSlowCount) && !GateIsSlow)
        {
            GateSlow();
        }
        break;
        
    case GATE_STATE_OPENING:
        // Check distance left, if less than 500 ticks slow down
        if (((GateOpenCount - QEIEncoderCountGet()) < GateSlowCount)  && !GateIsSlow)
        {
            GateSlow();
        }
        break;

    default:
        break;
    }

    // If Make gap
    if (GateMakeGapFlag && ((TickGet() - GateMakeGapDelay) > GATE_GAP_STOP_DELAY))
    {
        // Clear Flag
        GateMakeGapFlag = 0;
        
        // Set current position as zero
        QEIEncoderCountReset();
        // Set new state
        GateUpdateState(GATE_STATE_MAKE_GAP);
        // Set trigger point
        QEISetTrigger(GATE_GAP_COUNT);
        // Move
        GateSlow();
    }

    // Jam Detection
    GateJamCheck();

}
