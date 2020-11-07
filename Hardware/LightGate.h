#ifndef __LightGate_h__
#define __LightGate_h__

#include <reg52.h>
#include "TimeCounter.h"
#include "LCD1602.h"
#include "DigitalTube.h"

#ifndef __U_8_16__
#define __U_8_16__
typedef unsigned char u8;
typedef unsigned int u16;
#endif

#define CurrentTime (50 * cnt50ms + (double)((TH0 << 8 | TL0) - (H50MS << 8 | L50MS)) * 0.001085)

extern u8 Total;
extern double Period;
extern bit MeasureFlag;
extern bit BuzzFlag;

void InitLightGate();

#endif // !__LightGate_h__