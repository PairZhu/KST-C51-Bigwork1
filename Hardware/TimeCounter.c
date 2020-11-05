#include "TimeCounter.h"

u16 cnt50ms = 0; //储存经过多少个50ms

void InitTimeCounter()
{
    EA=1;
	TMOD |= 0x1; //设置定时器0为模式1
	cnt50ms = 0;
	TL0 = L50MS; //设置定时初值
	TH0 = H50MS;
	ET0 = 1; //使能定时器0中断
    PT0 = 1; //设置为高优先级，保证计时的准确性
}

void startTimeCounter() //打开定时器0设置溢出周期为50ms
{
	TR0 = 1; //定时器0开始计时
}

void stopTimeCounter() //关闭定时器0
{
	TR0 = 0;
	ET0 = 0;
}

void Timer1() interrupt 1
{
	TL0 = L50MS; //设置定时初值
	TH0 = H50MS;
	++cnt50ms;
}