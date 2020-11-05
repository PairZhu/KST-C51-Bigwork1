#ifndef __DigitalTube_h__
#define __DigitalTube_h__

#include <reg52.h>

#ifndef __U_8_16__
#define __U_8_16__
typedef unsigned char u8;
typedef unsigned int u16;
#endif

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit DISABLELED = P1^4;
extern unsigned char LedBuff[];
extern unsigned char code LedChar[];

void InitDigitalTube();
void showNumber(u8);


#endif // !__DigitalTube_h__