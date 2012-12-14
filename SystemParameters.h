#ifndef __SYSTEMPARAM_H
#define __SYSTEMPARAM_H

/* ----- Gate ----- */
// Speed to move at normally (Fast)
#define GATE_SPEED_FAST     8000
// Speed to move at when nearing end (Slow)
#define GATE_SPEED_SLOW     4000


// Time to stop when making the gap
#define GATE_GAP_STOP_DELAY (TICK_SECOND)
// Number of encoder counts for gap size
#define GATE_GAP_COUNT      25
// Absolute distance to end to be run slowly
#define GATE_SLOW_COUNT     500



// Time between Jam detection samples
#define GATE_JAM_TIMEOUT         (1 * TICK_SECOND)
// Required encoder count delta in time stipulated above before gate is considered jammed
#define GATE_JAM_DELTA_TRESHOLD  5
// Number of encoder counts to end if gate jammed when closing to be considered for auto offset calibration
#define GATE_JAM_CLOSE_TRESHOLD  50


/* ----- Motor ----- */
// Time to be Idle before the PWM generators are disabled to save power
#define MOTOR_IDLE_TIMEOUT  (5 * TICK_SECOND)

// The ADC value for Over Current Condition
#define ADC_OC_LIMIT        400

// PWM to run when Motor has OCed out of 10000
#define MOTOR_OC_PWM        4000


/* ----- ADC ----- */
// Averaging Ring Buffer Size
#define ADC_BUFFER_SIZE     16

#endif
