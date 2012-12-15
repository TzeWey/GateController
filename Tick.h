#ifndef __TICK_H
#define __TICK_H

#define TICKS_PER_SECOND		(GetInstructionClock()/256ull)	// Internal core clock drives timer with 1:256 prescaler

// Represents one milisecond in Ticks
#define TICK_MILISECOND			((unsigned long)TICKS_PER_SECOND/1000)
// Represents one second in Ticks
#define TICK_SECOND			((unsigned long)TICKS_PER_SECOND)
// Represents one minute in Ticks
#define TICK_MINUTE			((unsigned long)TICKS_PER_SECOND*60)
// Represents one hour in Ticks
#define TICK_HOUR                       ((unsigned long)TICKS_PER_SECOND*3600)

void TickInit(void);
unsigned long TickGet(void);
unsigned long TickGetDiv256(void);
unsigned long TickGetDiv64K(void);

void TickUpdate(void);

#endif
