#include "LightGate.h"

void InitLightGate()
{
    EA = 1;
	IT1 = 1; //设置外部中断1为下降沿触发
    EX1 = 1; //启用外部中断1
	showNumber(Total);
}

void Extern1() interrupt 2
{
    static u8 cnt = 0;
    ++cnt;
	BuzzFlag = 1;
	showNumber(Total-(cnt-1>>1));
    if (cnt == 1)
    {
        startTimeCounter();
        return;
    }
    if (cnt == Total * 2 + 1)
    {
		stopTimeCounter();
		Period = (50 * cnt50ms + (double)((TH0 << 8 | TL0) - (H50MS << 8 | L50MS)) * 0.001085)/Total;
		EX1 = 0;//关闭外部中断
		MeasureFlag = 1;//标志测量完成;
    }
}