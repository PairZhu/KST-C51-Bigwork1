#include "TimeCounter.h"

u16 cnt50ms = 0; //���澭�����ٸ�50ms

void InitTimeCounter()
{
    EA=1;
	TMOD |= 0x1; //���ö�ʱ��0Ϊģʽ1
	cnt50ms = 0;
	TL0 = L50MS; //���ö�ʱ��ֵ
	TH0 = H50MS;
	ET0 = 1; //ʹ�ܶ�ʱ��0�ж�
    PT0 = 1; //����Ϊ�����ȼ�����֤��ʱ��׼ȷ��
}

void startTimeCounter() //�򿪶�ʱ��0�����������Ϊ50ms
{
	TR0 = 1; //��ʱ��0��ʼ��ʱ
}

void stopTimeCounter() //�رն�ʱ��0
{
	TR0 = 0;
	ET0 = 0;
}

void Timer1() interrupt 1
{
	TL0 = L50MS; //���ö�ʱ��ֵ
	TH0 = H50MS;
	++cnt50ms;
}