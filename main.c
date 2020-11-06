#include <reg52.h>
#include "KeyBoard.h"
#include "LightGate.h"
#include "TimeCounter.h"
#include "LCD1602.h"
#include "DigitalTube.h"

#ifndef __U_8_16__
#define __U_8_16__
typedef unsigned char u8;
typedef unsigned int u16;
#endif

#define PI 3.141592653589793

sbit BUZZ = P1^6;

u8 Total;
double param_a_mm, param_b_mm, param_H_mm, param_M_g, param_g, Period;
bit MeasureFlag = 0,	//测量完成标志
	BuzzFlag = 0;	//蜂鸣器振荡标志

void readParam();
void showResult();
void showTime();
void delay(u8);
u8 uftoa(char* , double, u8);

void main()
{
	readParam();
	InitDigitalTube();
	InitTimeCounter();
	InitLightGate();
	showTime();
	showResult();
	while(1);
}

void delay(u8 ms)
{
	u8 i=ms<<1, j=113;
	do
	{
		while (--j);
	} while (--i);
}

void readParam()
{
	u8 state=0, key, len=2, max_len=10;
	unsigned char* tip_str;
	bit right_range=0;
	unsigned long num=10;
	InitLcd1602(1);
	LcdShowStr(0,0,"NumberOfPeriods:");
	LcdShowStr(0,1,"10");
	while(1)
	{
		if (key = readKey())
		{
			if (key >= '0' && key <= '9' && len < max_len && state<6)
			{
				LcdWriteDat(key);
				++len;
				num=num*10 + (key - '0');
				continue;
			}
			switch (key)
			{
			case ESC:
				if (len > 0)
				{
					--len;
					num/=10;
					LcdSetCursor(len, 1);
					LcdWriteDat(' ');
					LcdSetCursor(len, 1);
				}
				break;
			case ENTER:
				right_range=0;
				switch (state)
				{
				case 0://输入周期数
					if (num<=99 && num>=1)
					{
						right_range=1;
						Total=num;
						tip_str="100*a(mm):";
					}
					else
					{
						tip_str="OutOfRange:1-99";
					}
					break;
				case 1://输入上圆盘悬线孔构成的三角形边长a
					if (num>=1)
					{
						right_range=1;
						param_a_mm=num/100.0;
						tip_str="100*b(mm):";
					}
					else
					{
						tip_str="OutOfRange:a>0";
					}
					break;
				case 2://输入下圆盘悬线孔构成的三角形边长b
					if (num>=1)
					{
						right_range=1;
						param_b_mm=num/100.0;
						tip_str="100*H(mm):";
					}
					else
					{
						tip_str="OutOfRange:b>0";
					}
					break;
				case 3://输入上、下圆盘之间的距离H
					if (num>=1)
					{
						right_range=1;
						param_H_mm=num/100.0;
						tip_str="100*M(g):";
					}
					else
					{
						tip_str="OutOfRange:H>0";
					}
					break;
				case 4://输入被转动物体总质量M
					if (num>=1)
					{
						right_range=1;
						param_M_g=num/100.0;
						tip_str="100*g(m/s2):";
					}
					else
					{
						tip_str="OutOfRange:M>0";
					}
					break;
				case 5://输入重力加速度g
					if (num>=1)
					{
						right_range=1;
						param_g=num/100.0;
						InitLcd1602(0);
						tip_str="EnterToBegin...";
					}
					else
					{
						tip_str="OutOfRange:g>0";
					}
					break;
				case 6://准备开始测量
					LcdClear();
					LcdShowStr(0,0,"Measuring...");
					return;
				}
				if(right_range)
				{
					++state;
					LcdClear();
					LcdShowStr(0,0,tip_str);
					LcdSetCursor(0,1);
					if(state == 5)
					{
						num = 980;
						len = 3;
						LcdShowStr(0,1,"980");
					}
					else
					{
						num = 0;
						len = 0;
					}
				}
				else
				{
					LcdShowStr(0,0,tip_str);
					LcdSetCursor(len, 1);
				}
				break;
			}
		}
		delay(2);
	}
}

void showTime()
{
	u8 num_len,str[20];
	while(!MeasureFlag)
	{
		num_len=uftoa(str,(50 * cnt50ms + (double)((TH0 << 8 | TL0) - (H50MS << 8 | L50MS)) * 0.001085)/1000,2);
		str[num_len]='\0';
		DISABLELED = 1;	//禁用,以免操作影响液晶屏
		LcdShowStr(0,1,"t:");
		LcdShowStr(2,1,str);
		LcdShowStr(2+num_len,1,"s");
		P0=0xFF;
		DISABLELED = 0;
		delay(20);
	}
}

void showResult()
{
	u8 num_len,str[20];
	double J_gm2;
	num_len=uftoa(str,Period,3);
	str[num_len]='\0';
	DISABLELED = 1;
	LcdClear();
	LcdShowStr(0,0,"T=");
	LcdShowStr(2,0,str);
	LcdShowStr(2+num_len,0,"ms");
	P0=0xFF;
	DISABLELED = 0;
	J_gm2=param_M_g*param_g*param_a_mm*param_b_mm*Period*Period/(12*PI*PI*10e9*param_H_mm);
	num_len=uftoa(str,J_gm2,4);
	str[num_len]='\0';
	DISABLELED = 1;
	LcdShowStr(0,1,"J=");
	LcdShowStr(2,1,str);
	LcdShowStr(2+num_len,1,"g*m2");
	P0=0xFF;
	DISABLELED = 0;
}
/*正浮点数转字符串，返回字符串长度，不在末尾加'\0'*/
u8 uftoa(char* dst, double num, u8 fs)
{
	u8 i, chars_cnt, digits = 3;	//至少3位
	unsigned long factor = 10, temp; //因数多10倍，以便四舍五入
	for (i = 0; i != fs; ++i, factor *= 10);
	factor *= num;
	if (factor % 10 >= 5) //四舍五入
		factor += 10;
	factor /= 10;
	for (temp = factor; temp > 999; temp /= 10, ++digits);	//计算转为整数后的位数
	chars_cnt = digits + 1;
	for (i = 0; i != chars_cnt - 1; ++i)
	{
		if (i == fs)
			dst[digits--] = '.';
		dst[digits--] = factor % 10 + '0';
		factor /= 10;
	}
	return chars_cnt;
}
/*
u8 utoa(char *dst, u16 num)
{
	u8 i, chars_cnt, digits = 1;
	u16 temp;
	for (temp = num; temp > 9; temp /= 10, ++digits);
	chars_cnt=digits;
	while (digits)
	{
		dst[--digits]=num%10+'0';
		num /= 10;
	}
	return chars_cnt;
}
*/
void InterruptTime2() interrupt 3
{
    static u8 i = 0, buzz_cnt = 0;
    TH1 = 0xFC;
    TL1 = 0x67;
    if(BuzzFlag)
	{
		BUZZ=~BUZZ;
		++buzz_cnt;
		if(buzz_cnt==100)
		{
			BuzzFlag=0;
			buzz_cnt=0;
		}
	}
	if(DISABLELED) return;
    P0 = 0xFF;
    ADDR2 = 0;
    ADDR1 = 0;
    switch (i)
    {
    case 0:
        ADDR0 = 0;
        break;
    case 1:
        ADDR0 = 1;
        break;
    }
    P0 = LedBuff[i++];
    if(i==2)
        i=0;
}