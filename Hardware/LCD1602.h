#ifndef __LCD1602_h__
#define __LCD1602_h__

#include <reg52.h>

#ifndef __U_8_16__
#define __U_8_16__
typedef unsigned char u8;
typedef unsigned int u16;
#endif

#define LCD1602_DB P0

sbit LCD1602_RS = P1^0;
sbit LCD1602_RW = P1^1;
sbit LCD1602_E = P1^5;

void InitLcd1602(bit);
void LcdClear();
void LcdSetCursor(u8, u8);
void LcdWriteDat(unsigned char);
void LcdShowStr(u8, u8, unsigned char*);

#endif // !__LCD1602_h__