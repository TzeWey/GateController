#ifndef __ENCODER_H
#define __ENCODER_H

void EncoderRegTriggerHandle(void (*func)(void));

void EncoderSetCountUp(BYTE up);
void EncoderSetCount(long count);
void EncoderSetTrigger(long count);

long EncoderGetCount(void);
long EncoderGetRemaining(void);


void EncoderTick(void);
void EncoderInit(void);

void EncoderInterrupt(void);

#endif
