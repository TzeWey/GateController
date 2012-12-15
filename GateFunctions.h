#ifndef __GATEFUNC_H
#define __GATEFUNC_H

#include "Framework.h"
#include "Device.h"

typedef enum
{
    OPEN,
    CLOSE,
}GATE_MOVE_DIR;

typedef enum
{
    FAST,
    SLOW,
}GATE_MOVE_SPEED;

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
    GATE_STATE_DELAY,
}GATE_STATE;

typedef struct
{
    INT32  GateOpenEncoderCount;

    INT16  GateSpeedFast;
    INT16  GateSpeedSlow;

    DWORD  GateGapStopDelay;
    INT32  GateGapEncoderCount;
    INT32  GateSlowSpeedDistance;

    DWORD  GateJamTimeoutWindow;
    INT32  GateJamMinimumEncoderChange;
    INT32  GateJamCloseAutoOffsetDelta;

    DWORD  MotorIdleDelayTimeout;
    UINT16 MotorOverCurrentADCValue;
    INT16  MotorOverCurrentPWM;
    DWORD  MotorSmoothRampRate;


    BYTE   GateStatusErrorFlag;
    BYTE   GateStatusNewState;

    BYTE   GateStatusIsMoving;
    
    GATE_MOVE_DIR   GateStatusDirection;
    GATE_MOVE_SPEED GateStatusSpeed;

    GATE_STATE      CurrentState;
    GATE_STATE      PreviousState;

    DWORD           NextStateDelay;
    GATE_STATE      NextState;
}GATE_STATUS;

extern GATE_STATUS GateStatus;

BYTE GateIsNewState(void);
void GateMove(GATE_MOVE_DIR dir, GATE_MOVE_SPEED speed, INT32 trigger);
void GateStop(void);
void GateStopFast(void);

void GateUpdateState(GATE_STATE newState);
void GateUpdateStateDelay(GATE_STATE newState, DWORD delay);
void GateUpdateStateDelayResume(void);

void GateErrorClear(void);

void GateJamRegEventHandle(void (*func)(void));
void GateJamTick(void);

void GateDirectionCheck(void);
void GateMakeGap(void);
void GateLearningStart(void);
void GateLearningSetOpen(void);

void GateSaveEncoderCount(void);

void GateFunctionsInit(void);

#endif
