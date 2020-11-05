#include "DigitalTube.h"

unsigned char code LedChar[10]={
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90
	};
unsigned char LedBuff[2]={
	0xFF, 0xFF
	};

void InitDigitalTube()
{
    EA = 1;
	P0=0xFF;
	DISABLELED = 0;
	ADDR3 = 1;
	TMOD |= 0x10;
	TH1 = 0xFC;
	TL1 = 0x67;
	ET1 = 1;
	TR1 = 1;	
}

void showNumber(u8 num)
{
    LedBuff[0] = LedChar[num%10];
	LedBuff[1] = LedChar[num/10%10];
}