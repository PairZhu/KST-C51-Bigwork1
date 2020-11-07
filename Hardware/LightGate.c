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
    static double last_time, last_period, this_period, this_time;
    ++cnt;
	BuzzFlag = 1;
	showNumber(Total-(cnt-1>>1));
    if (cnt == 1)
    {
        startTimeCounter();
        last_time = 0;
        return;
    }
    if (cnt == Total * 2 + 1)
    {
		stopTimeCounter();
		Period = CurrentTime/Total;
		EX1 = 0;//�ر��ⲿ�ж�
        IT1 = 0;
        cnt=0;
		MeasureFlag = 1;//��־�������;
    }
    if(cnt&0x01 && !ErrorFlag)
    {
        this_time=CurrentTime;
        this_period=this_time-last_time;
        if(cnt!=3 && (this_period>last_period? this_period-last_period:last_period-this_period)/last_period>0.2)
            ErrorFlag=1;
        last_period=this_period;
        last_time=this_time;
    }
}