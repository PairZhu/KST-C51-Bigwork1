#ifndef __KeyBoard_h__
#define __KeyBoard_h__
#include <reg52.h>

#ifndef __U_8_16__
#define __U_8_16__
typedef unsigned char u8;
typedef unsigned int u16;
#endif

sbit keyout1=P2^3;
sbit keyout2=P2^2;
sbit keyout3=P2^1;
sbit keyout4=P2^0;

sbit keyin1=P2^4;
sbit keyin2=P2^5;
sbit keyin3=P2^6;
sbit keyin4=P2^7;

enum { UP=0x26, DOWN=0x28, LEFT=0x25, RIGHT=0x27, ESC=0x1B, ENTER=0x0D };

u8 readKey(); //自行实现每隔2ms调用一次

#endif