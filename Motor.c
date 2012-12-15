#include "Framework.h"
#include "Device.h"

#include "GateFunctions.h"

#define MOTOR_PWM_SCALE     10000

#define PWMA                HSPWM5
#define PWMB                HSPWM1

// Maximum and Minimum PWM value out of 10000
#define MOTOR_MAX_PWM       8000
#define MOTOR_MIN_PWM       250

// Maximum Delta to zero before motor is considered to be stopped.
// This is to eliminate close to Zero Duty as Gate Driver cannot drive zero PWM
#define PWM_DIFF            100

static volatile  INT16    SetPWM     = 0;
static volatile  INT16 TargetPWM     = 0;
static volatile  INT16  MotorPWM     = 0;

static volatile DWORD  MotorLastSpeedTick = 0;

static volatile DWORD  MotorLastOCTimestamp = 0;
static volatile BYTE   MotorIsOC = 0;

static volatile DWORD  MotorIdleTimestamp = 0;
static volatile BYTE   MotorIdleTimeout   = 0;

void MotorStop(void)
{
    HSPWMDutySetScale(PWMA, 0, MOTOR_PWM_SCALE);
    HSPWMDutySetScale(PWMB, 0, MOTOR_PWM_SCALE);
    AHI = 0;
    BHI = 0;

    TargetPWM = 0;
    MotorPWM  = 0;
}

void MotorSetPWM(INT16 pwm)
{
    // Limit speed
    if (pwm < -MOTOR_MAX_PWM)
    {
        pwm = -MOTOR_MAX_PWM;
    }
    else if (MOTOR_MAX_PWM > MOTOR_MAX_PWM)
    {
        pwm = MOTOR_MAX_PWM;
    }
    SetPWM    = pwm;
    TargetPWM = SetPWM;
}

void MotorTick(void)
{
    // Motor Smooth Ramping
    if ((TickGet() - MotorLastSpeedTick) > GateStatus.MotorSmoothRampRate)
    {
        if (MotorPWM != TargetPWM)
        {
            // Calculate speed difference for smooth ramping
            INT16 diffPWM = TargetPWM - MotorPWM;
            if (diffPWM > PWM_DIFF)
            {
                diffPWM = PWM_DIFF;
            }
            else if (diffPWM < -PWM_DIFF)
            {
                diffPWM = -PWM_DIFF;
            }

            // Adjust MotorPWM
            MotorPWM = MotorPWM + diffPWM;

            if ((!HSPWMIsEnabled()) && (MotorPWM != 0))
            {
                ADCStart();
                HSPWMEnable(1);
            }

            if ((MotorPWM <= MOTOR_MIN_PWM)&&(MotorPWM >= -MOTOR_MIN_PWM))
            {
                HSPWMDutySetScale(PWMA, 0, MOTOR_PWM_SCALE);
                HSPWMDutySetScale(PWMB, 0, MOTOR_PWM_SCALE);
                AHI = 0;
                BHI = 0;
            }
            else
            {
                if (MotorPWM < 0)
                {
                     // Forward
                    HSPWMDutySetScale(PWMB, -MotorPWM, MOTOR_PWM_SCALE);
                    BHI = 1;
                    HSPWMDutySetScale(PWMA, 0, MOTOR_PWM_SCALE);
                    AHI = 0;
                }
                else
                {
                    // Backward
                    HSPWMDutySetScale(PWMA, MotorPWM, MOTOR_PWM_SCALE);
                    AHI = 1;
                    HSPWMDutySetScale(PWMB, 0, MOTOR_PWM_SCALE);
                    BHI = 0;
                }
            }

        }

        MotorLastSpeedTick = TickGet();
    }

    // Motor Idle PWM Shutdown
    if (HSPWMIsEnabled())
    {
        if (MotorPWM == 0)
        {
            // IDLE PWM SHUTDOWN
            if (MotorIdleTimeout == 0)
            {
                MotorIdleTimeout = 1;
                MotorIdleTimestamp = TickGet();
            }
            else
            {
                if (TickGet() - MotorIdleTimestamp >= GateStatus.MotorIdleDelayTimeout)
                {
                    ADCStop();
                    HSPWMEnable(0);
                    LEDY = 0;
                    LEDG = 0;
                    MotorIdleTimeout = 0;
                }
            }
        }
        else
        {
            // Reset idle timeout
            MotorIdleTimeout = 0;
            
            // ADC OC Check
            if (ADCGetCurrent() > GateStatus.MotorOverCurrentADCValue)
            {
                // Motor has OCed, slow things down
                if (SetPWM < 0)
                {
                    TargetPWM = -(GateStatus.MotorOverCurrentPWM);
                }
                else
                {
                    TargetPWM = GateStatus.MotorOverCurrentPWM;
                }
                
                MotorIsOC = 1;
                MotorLastOCTimestamp = TickGet();
            }
            else
            {
                if (MotorIsOC && (TickGet() - MotorLastOCTimestamp > TICK_SECOND))
                {
                    // OC has not occured for some time, restore speed and clear OC
                    MotorIsOC = 0;
                    TargetPWM = SetPWM;
                }
            }


        }
    }

}

void MotorInit(void)
{
    MotorStop();
}
