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
#define MAX_LEN 10

sbit BUZZ = P1^6;

u8 Total;
double param_a_mm, param_b_mm, param_H_mm, param_M_g, param_g, Period;
bit MeasureFlag = 0,	//测量完成标志
	BuzzFlag = 0,	//蜂鸣器振荡标志
	ErrorFlag = 0;	//测量失误标志

u8 Image[8]={0xE7,0xE7,0xE7,0xE7,0xE7,0xFF,0xE7,0xE7};

void readParam();
void showResult();
void showTime();
void delay(u8);
void RereadM();
u8 uftoa(u8*, double, u8);
u8 utoa(char*, unsigned long);

void main()
{
	readParam();
	InitDigitalTube();
	InitTimeCounter();
	InitLightGate();
	showTime();
	showResult();
	while(1) //多次测量
	{
		if (readKey()==ENTER)
		{
			DISABLELED=1;
			ErrorFlag=0;
			MeasureFlag=0;
			RereadM();
			InitLcd1602(0);
			LcdShowStr(0,0,"EnterToBegin...");
			do
			{
				delay(2);
			} while (readKey()!=ENTER);
			LcdClear();
			LcdShowStr(0,0,"Measuring...");
			InitDigitalTube();
			InitTimeCounter();
			InitLightGate();
			showTime();
			showResult();
		}
		delay(2);
	}
}

void delay(u8 ms)
{
	u8 i=ms<<1, j=113;
	do
	{
		while (--j);
	} while (--i);
}

void RereadM()
{
	u8 key, len;
	u8 str[20];
	unsigned long num=param_M_g*100;
	len=utoa(str,num);
	str[len]=0;
	InitLcd1602(1);
	LcdShowStr(0,0,"100*M(g):");
	LcdShowStr(0,1,str);
	while(1)
	{
		if (key = readKey())
		{
			if (key >= '0' && key <= '9' && len < MAX_LEN)
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
				if (num>=1)
				{
					param_M_g=num/100.0;
					return;
				}
				else
				{
					LcdShowStr(0,0,"OutOfRange:M>0");
					LcdSetCursor(len,1);
				}
			}
		}
		delay(2);
	}

}

void readParam()
{
	u8 state=0, key, len=2;
	unsigned long num=10;
	InitLcd1602(1);
	LcdShowStr(0,0,"NumberOfPeriods:");
	LcdShowStr(0,1,"10");
	while(1)
	{
		if (key = readKey())
		{
			if (key >= '0' && key <= '9' && len < MAX_LEN && state<6)
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
				switch (state)
				{
				case 0://输入周期数
					if (num<=99 && num>=1)
					{
						LcdClear();
						Total=num;
						LcdShowStr(0,0,"100*a(mm):");
						num=5000;
						len=4;
						LcdShowStr(0,1,"5000");
						++state;
					}
					else
					{
						LcdShowStr(0,0,"OutOfRange:1-99");
						LcdSetCursor(len,1);
					}
					break;
				case 1://输入上圆盘悬线孔构成的三角形边长a
					if (num>=1)
					{
						LcdClear();
						param_a_mm=num/100.0;
						LcdShowStr(0,0,"100*b(mm):");
						num=15000;
						len=5;
						LcdShowStr(0,1,"15000");
						++state;
					}
					else
					{
						LcdShowStr(0,0,"OutOfRange:a>0");
						LcdSetCursor(len,1);
					}
					break;
				case 2://输入下圆盘悬线孔构成的三角形边长b
					if (num>=1)
					{
						LcdClear();
						param_b_mm=num/100.0;
						LcdShowStr(0,0,"100*H(mm):");
						num=50000;
						len=5;
						LcdShowStr(0,1,"50000");
						++state;
					}
					else
					{
						LcdShowStr(0,0,"OutOfRange:b>0");
						LcdSetCursor(len,1);
					}
					break;
				case 3://输入上、下圆盘之间的距离H
					if (num>=1)
					{
						LcdClear();
						param_H_mm=num/100.0;
						LcdShowStr(0,0,"100*M(g):");
						num=50000;
						len=5;
						LcdShowStr(0,1,"50000");
						++state;
					}
					else
					{
						LcdShowStr(0,0,"OutOfRange:H>0");
						LcdSetCursor(len,1);
					}
					break;
				case 4://输入被转动物体总质量M
					if (num>=1)
					{
						LcdClear();
						param_M_g=num/100.0;
						LcdShowStr(0,0,"100*g(mm/s2):");
						num=980000;
						len=6;
						LcdShowStr(0,1,"980000");
						++state;
					}
					else
					{
						LcdShowStr(0,0,"OutOfRange:M>0");
						LcdSetCursor(len,1);
					}
					break;
				case 5://输入重力加速度g
					if (num>=1)
					{
						LcdClear();
						param_g=num/100.0;
						InitLcd1602(0);
						LcdShowStr(0,0,"EnterToBegin...");
						++state;
					}
					else
					{
						LcdShowStr(0,0,"OutOfRange:g>0");
						LcdSetCursor(len,1);
					}
					break;
				case 6://准备开始测量
					LcdClear();
					LcdShowStr(0,0,"Measuring...");
					return;
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
		num_len=uftoa(str,CurrentTime/1000,2);
		str[num_len]='\0';
		DISABLELED = 1;	//禁用,以免操作影响液晶屏
		LcdShowStr(0,1,"Sec:");
		LcdShowStr(4,1,str);
		LcdShowStr(4+num_len,1,"s");
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
	//		500		*	9800	* 50  *	150	*    1000	*  1000	/(12*3.14*3.14*1e12*500)	
	J_gm2=param_M_g*param_g*param_a_mm*param_b_mm*Period*Period/(12*PI*PI*1e12*param_H_mm);//96191
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
u8 uftoa(u8* dst, double num, u8 fs)
{
	u8 i, chars_cnt, digits = 0;
	unsigned long factor = 10, temp; //因数多10倍，以便四舍五入
	for (i = 0; i != fs; ++i, factor *= 10);
	factor *= num;
	if (factor % 10 >= 5) //四舍五入
		factor += 10;
	factor /= 10;
	for (temp = factor; temp > 0; temp /= 10, ++digits);	//计算转为整数后的位数
	if (digits < fs + 1)
		digits = fs + 1;
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

u8 utoa(char *dst, unsigned long num)
{
	u8 chars_cnt, digits = 1;
	unsigned long temp;
	for (temp = num; temp > 9; temp /= 10, ++digits);
	chars_cnt=digits;
	while (digits)
	{
		dst[--digits]=num%10+'0';
		num /= 10;
	}
	return chars_cnt;
}

void InterruptTime2() interrupt 3
{
    static u8 i = 0, buzz_cnt = 50;
	static bit handled_error = 0;
    TH1 = 0xFE;
    TL1 = 0x32;
	if(ErrorFlag)
	{
		if(!handled_error)
		{
			buzz_cnt=255;
			handled_error = 1;
		}
	}
	else
	{
		handled_error = 0;
	}
    if(BuzzFlag)
	{
		BUZZ=~BUZZ;
		--buzz_cnt;
		if(buzz_cnt==0)
		{
			BuzzFlag=0;
			buzz_cnt=25;
		}
	}
	if(DISABLELED) return;
    P0 = 0xFF;
    switch (i)
    {
	case 0:
		ADDR3 = 0; ADDR2=0; ADDR1=0; ADDR0=0;
		break;
	case 1:
		ADDR2=0; ADDR1=0; ADDR0=1;
		break;
	case 2:
		ADDR2=0; ADDR1=1; ADDR0=0;
		break;
	case 3:
		ADDR2=0; ADDR1=1; ADDR0=1;
		break;
	case 4:
		ADDR2=1; ADDR1=0; ADDR0=0;
		break;
	case 5:
		ADDR2=1; ADDR1=0; ADDR0=1;
		break;
	case 6:
		ADDR2=1; ADDR1=1; ADDR0=0;
		break;
	case 7:
		ADDR2=1; ADDR1=1; ADDR0=1;
		break;
    case 8:
        ADDR0 = 0; ADDR1 = 0; ADDR2 = 0; ADDR3 = 1;
        break;
    case 9:
        ADDR0 = 1; ADDR1 = 0; ADDR2 = 0;
        break;
		
    }
	if(i>7)
    	P0 = LedBuff[i-8];
	else if(ErrorFlag)
		P0 = Image[i];
	++i;
    if(i==10)
        i=0;
		
}