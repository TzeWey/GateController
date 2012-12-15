#include "GateApplication.h"

static volatile DWORD GateJamLastWindowTick   = 0;
static volatile INT32 GateJamLastEncoderValue = 0;


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

static void GateStartLearning(void);
static void GateLearningSetOpen(void);

static void GateUpdateState(GATE_STATE newState)
{
    PreviousState = CurrentState;
    CurrentState  = newState;

    // Indicate new state
    GateStatus.GateStatusNewState = 1;
}
static void GateErrorClear(void)
{
    GateStatus.GateStatusErrorFlag = 0;
}

static void GateJamTick(void)
{
    if (GateStatus.GateStatusIsMoving)
    {
        // Gate is moving, check for jam
        if ((TickGet() - GateJamLastWindowTick) > GateStatus.GateJamTimeoutWindow)
        {
            // Check if count has changed
            INT32 EncoderDiff = QEIEncoderCountGet() - GateJamLastEncoderValue;

            LEDY ^= 1;

            if (((EncoderDiff <= 0) && (EncoderDiff > -GateStatus.GateJamMinimumEncoderChange)) || ((EncoderDiff > 0) && (EncoderDiff < GateStatus.GateJamMinimumEncoderChange)))
            {

                // Gate encoder count has not changed much, assume jammed
                GateStatus.GateStatusErrorFlag = 1;
                GateJammed();
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
        GateJamLastWindowTick = TickGet();
    }
}

// TODO
void GateMakeGap(void)
{
    GateStopFast();
    
    GateMakeGapFlag = 1;
    GateMakeGapDelay = TickGet();
}

// TODO
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
        if (QEIEncoderCountGet() < GateStatus.GateJamCloseAutoOffsetDelta)
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
        GateErrorClear();
        GateMakeGap(); // Make gap, gap code should know what to do next
        break;

    case GATE_STATE_LEARNING_OPEN: // Was in the midst of learning open limit, this jam is expected
        GateErrorClear();
        GateLearningSetOpen();
        break;

    default:
        GateUpdateState(GATE_STATE_STOP);
        break;
    }

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
    GateStatus.GateOpenEncoderCount = QEIEncoderCountGet();
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
    // Clear Error flags
    GateErrorClear();
    
    switch (CurrentState)
    {
    case GATE_STATE_STOP: // It is Stopped
        switch (PreviousState)
        {
        case GATE_STATE_OPENING: // It was last opening
            // so start closing
            GateUpdateState(GATE_STATE_CLOSING);
            break;

        case GATE_STATE_CLOSING: // It was last closing
            // so start opening
            GateUpdateState(GATE_STATE_OPENING);
            break;

        default: // Initial state handler, this should only occur once at startup
            GateUpdateState(GATE_STATE_OPENING);
            break;
        }
        break;

    case GATE_STATE_OPEN: // It is open
        // so start closing
        GateUpdateState(GATE_STATE_CLOSING);
        break;

    case GATE_STATE_CLOSE: // It is closed
        // so start opening
        GateUpdateState(GATE_STATE_OPENING);
        break;

    case GATE_STATE_OPENING: // It is in the midst of opening or closing
    case GATE_STATE_CLOSING:
    case GATE_STATE_CALIBRATING_CLOSE:
        // so stop
        GateUpdateState(GATE_STATE_STOP);
        break;

    case GATE_STATE_LEARNING_CLOSE:
    case GATE_STATE_LEARNING_OPEN:
        // pass press as learn button pressed
        GateLearnToggle();
        break;

    case GATE_STATE_MAKE_GAP:
        break;

    }
}

void GateInit(void)
{
    // Initialize Gate Funtions which loads settigns
    GateFunctionsInit();

    QEIRegTriggerHandle(&GateEncoderCountReachedEvent);
}

void GateTick(void)
{
    
    GateJamTick();

    switch (CurrentState)
    {
    case GATE_STATE_OPEN: // TODO : Currently doing nothing
        break;
        
    case GATE_STATE_CLOSE: // TODO : Currently doing nothing, should reset encoder to zero
        break;

    case GATE_STATE_STOP:
        if (GateIsNewState())
        {
            // Stop gate
            GateStop();
        }
        break;

    case GATE_STATE_OPENING:
        if (GateIsNewState())
        {
            // Open gate, fast, to END
            GateMove(OPEN, FAST, GateStatus.GateOpenEncoderCount);
        }

        // Slow down if nearning END
        if (((GateStatus.GateOpenEncoderCount - QEIEncoderCountGet()) < GateStatus.GateSlowSpeedDistance) && (GateStatus.GateStatusSpeed == FAST))
        {
            // Update speed
            GateMove(OPEN, SLOW, GateStatus.GateOpenEncoderCount);
        }
        break;

    case GATE_STATE_CLOSING:
        if (GateIsNewState())
        {
            // Stop gate
            GateMove(CLOSE, FAST, 0);
        }

        // Slow down if nearning END
        if ((QEIEncoderCountGet() < GateStatus.GateSlowSpeedDistance) && (GateStatus.GateStatusSpeed == FAST))
        {
            // Update speed
            GateMove(CLOSE, SLOW, 0);
        }
        break;

    case GATE_STATE_CALIBRATING_CLOSE:
        if (GateIsNewState())
        {
            // Close gate slowly
            GateMove(CLOSE, SLOW, 0);
        }
        break;

    case GATE_STATE_LEARNING_CLOSE:
        if (GateIsNewState())
        {
            // Close gate slowly
            GateMove(CLOSE, SLOW, 0);
        }
        break;

    case GATE_STATE_LEARNING_OPEN:
        if (GateIsNewState())
        {
            // Open gate slowly
            GateMove(OPEN, SLOW, 0);
        }
        break;

    case GATE_STATE_MAKE_GAP:
        if (GateIsNewState())
        {
            // Open gate slowly
            GateMove(OPEN, SLOW, GateStatus.GateGapEncoderCount);
        }
        break;
    }
}

void GateTick(void)
{

    // If Make gap
    if (GateMakeGapFlag && ((TickGet() - GateMakeGapDelay) > GateStatus.GateGapStopDelay))
    {
        // Clear Flag
        GateMakeGapFlag = 0;
        
        // Set current position as zero
        QEIEncoderCountReset();
        // Set new state
        GateUpdateState(GATE_STATE_MAKE_GAP);
        // Set trigger point
        QEISetTrigger(GateStatus.GateGapEncoderCount);
        // Move
        GateSlow();
    }

}
