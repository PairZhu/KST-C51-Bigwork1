#ifndef __TimeCounter_h__
#define __TimeCounter_h__

#include <reg52.h>

#ifndef __U_8_16__
#define __U_8_16__
typedef unsigned char u8;
typedef unsigned int u16;
#endif

#define H50MS 0x4b
#define L50MS 0xff

void InitTimeCounter();
void startTimeCounter();
void stopTimeCounter();

extern unsigned long cnt50ms;

#endif // !__TimeCounter_h__
