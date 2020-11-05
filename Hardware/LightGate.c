#include "LightGate.h"

void InitLightGate()
{
    EA = 1;
	IT1 = 1; //�����ⲿ�ж�1Ϊ�½��ش���
    EX1 = 1; //�����ⲿ�ж�1
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
		EX1 = 0;//�ر��ⲿ�ж�
		MeasureFlag = 1;//��־�������;
    }
}