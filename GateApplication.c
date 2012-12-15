#include "GateApplication.h"

static volatile DWORD GateDelayBeginTimestamp = 0;

static void GateJammedEvent(void)
{
    // Since it appears to be jammed, stop quickly
    GateStopFast();

    switch (GateStatus.CurrentState)
    {
    case GATE_STATE_OPENING: // Was in the midst of opening
        // set gate as stopped, most probably the gate hit something
        GateUpdateState(GATE_STATE_STOP);
        break;

    case GATE_STATE_CLOSING: // Was in the midst of closing
        // Check if we are close to closing and adjust for CLOSE offset
        if (QEIEncoderCountGet() < GateStatus.GateJamCloseAutoOffsetDelta)
        {
            // It appears to be very close to closed (possible) or has passed closed (should not be physically possible)
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

    case GATE_STATE_DELAY: // should not be able to trigger jam event as it is not moving
        // Do nothing
        break;

    default:
        // All othee states
        // GATE_STATE_OPEN, GATE_STATE_CLOSE, GATE_STATE_STOP (should not be able to cause jam events)
        // GATE_STATE_MAKE_GAP, was making a gap, assuming the encoder had fallen off, the gate could have opened a little
        GateUpdateState(GATE_STATE_STOP);
        break;
    }
}

static void GateEncoderCountReachedEvent(void)
{
    switch (GateStatus.CurrentState)
    {
    case GATE_STATE_OPENING: // Was in the midst of opening
        // stop and set new state as open
        GateStopFast();
        GateUpdateState(GATE_STATE_OPEN);
        break;
        
    case GATE_STATE_CLOSING: // Was in the midst of closing
        // stop and set new state as closed
        GateStopFast();
        GateUpdateState(GATE_STATE_CLOSE);
        break;

    case GATE_STATE_MAKE_GAP: // Was making the close gap
        // stop, and determine what to do next based on the previous state
        GateStopFast();
        
        switch (GateStatus.PreviousState)
        {
        case GATE_STATE_CLOSING:           // Was calibrating close
        case GATE_STATE_CALIBRATING_CLOSE: // Was calibrating close
            // set new state as closed and set current position as zero
            QEIEncoderCountReset();
            GateUpdateState(GATE_STATE_CLOSE);
            break;

        case GATE_STATE_LEARNING_CLOSE: // Was also calibrating close, but was also set to learn opening size
            // set new state as learning open and set current position as zero
            QEIEncoderCountReset();
            GateUpdateState(GATE_STATE_LEARNING_OPEN);
            //GateSlow();
            break;

        default:
            // Dont do anything else, but code should not reach here
            break;
        }        
        break;

    default:
        // GATE_STATE_OPEN  ,should not be moving to cause this event
        // GATE_STATE_CLOSE ,should not be moving to cause this event
        // GATE_STATE_STOP  ,should not be moving to cause this event
        // GATE_STATE_CALIBRATING_CLOSE, should be stopped by a jam event
        // GATE_STATE_LEARNING_CLOSE, should be stopped by a jam event
        // GATE_STATE_LEARNING_OPEN, should be stopped by a jam event
        // GATE_STATE_DELAY ,should not be moving to cause this event
        break;
    }

}

void GateLearnToggle(void)
{
    switch (GateStatus.CurrentState)
    {
    case GATE_STATE_LEARNING_OPEN: // It is trying to learn opening
        // set current position as open limit        
        GateLearningSetOpen();
        break;

    case GATE_STATE_LEARNING_CLOSE: // It is trying to learn closing
        // stop, set current position as zero, start learning open
        GateStopFast();
        QEIEncoderCountReset();
        GateUpdateStateDelay(GATE_STATE_LEARNING_OPEN, GateStatus.GateGapStopDelay);
        break;

    default: // was in some other state, start learn more
        GateLearningStart();
        break;
    }
}

void GateToggle(void)
{
    // Clear Error flags
    GateErrorClear();
    
    switch (GateStatus.CurrentState)
    {
    case GATE_STATE_STOP: // It is Stopped
        switch (GateStatus.PreviousState)
        {
        case GATE_STATE_OPENING: // It was last opening
            // so start closing
            GateUpdateState(GATE_STATE_CLOSING);
            break;

        case GATE_STATE_CLOSING: // It was last closing
            // so start opening
            GateUpdateState(GATE_STATE_OPENING);
            break;

        default: // Initial state handler, this should only occur once at startup and occur when GATE_STATE_MAKE_GAP caused a jam event
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
    case GATE_STATE_DELAY:
        // Do nothing
        break;

    }
}

void GateCalibrateClose(void)
{
    GateUpdateState(GATE_STATE_CALIBRATING_CLOSE);
}

void GateTick(void)
{   
    GateJamTick();

    switch (GateStatus.CurrentState)
    {
    case GATE_STATE_OPEN:
        // Should be nothing to do as gate should already be stopped
        break;
        
    case GATE_STATE_CLOSE:
        if (GateIsNewState())
        {
            // Reset encoder count to zero
            QEIEncoderCountReset();
        }
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

    case GATE_STATE_DELAY: // This state should ONLY be accessed by using the GateUpdateStateDelay() function
        if (GateIsNewState())
        {
            // Remember current timestamp
            GateDelayBeginTimestamp = TickGet();
        }
        else
        {
            // Move to next state after predefined delay
            if ((TickGet() - GateDelayBeginTimestamp) > GateStatus.NextStateDelay)
            {
                // Resume transition to new state
                GateUpdateStateDelayResume();
            }
        }
        break;
    }
}

void GateInit(void)
{
    // Initialize Gate Funtions which loads settigns
    GateFunctionsInit();

    GateJamRegEventHandle(&GateJammedEvent);
    
    QEIRegTriggerHandle(&GateEncoderCountReachedEvent);
}
