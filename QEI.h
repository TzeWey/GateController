#ifndef __QEI_H
#define __QEI_H

#define QEI_POS    1
#define QEI_NEG    0

BYTE  QEIDirection(void);
void  QEIPosReset(void);
INT16 QEIPosRead(void);

void  QEIInit(void);

void  QEIEncoderCountReset(void);
INT32 QEIEncoderCountGet(void);

void  QEIRegTriggerHandle(void (*func)(void));
void  QEISetTrigger(INT32 count);

void  QEIInterruptHandler(void);

#endif
