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


    BYTE   GateStatusErrorFlag;
    BYTE   GateStatusNewState;

    BYTE   GateStatusIsMoving;
    
    GATE_MOVE_SPEED GateStatusSpeed;
}GATE_STATUS;

extern GATE_STATUS GateStatus;

BYTE GateIsNewState(void);
void GateStop(void);
void GateStopFast(void);

void GateFunctionsInit(void);

#endif
