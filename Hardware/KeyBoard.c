#include "KeyBoard.h"

u8 code KeyCodeMap[] = { //���󰴼���ŵ���׼���̼����ӳ���
	0x00,					//���ַ�
	0x31, 0x32, 0x33, 0x26 , //���ּ� 1�����ּ� 2�����ּ� 3�����ϼ�
	0x34, 0x35, 0x36, 0x25 , //���ּ� 4�����ּ� 5�����ּ� 6�������
	0x37, 0x38, 0x39, 0x28 , //���ּ� 7�����ּ� 8�����ּ� 9�����¼�
	0x30, 0x1B, 0x0D, 0x27 , //���ּ� 0��ESC ���� �س����� ���Ҽ�
	/*
	//����Ϊ�������������Ӧ��ֵ���λ��Ϊ1������0x80λ������ֵ
	0xB1, 0xB2, 0xB3, 0xA6 , //���������ּ� 1�����ּ� 2�����ּ� 3�����ϼ�
	0xB4, 0xB5, 0xB6, 0xA5 , //���������ּ� 4�����ּ� 5�����ּ� 6�������
	0xB7, 0xB8, 0xB9, 0xA8 , //���������ּ� 7�����ּ� 8�����ּ� 9�����¼�
	0xB0, 0x9B, 0x8D, 0xA7   //���������ּ� 0��ESC ���� �س����� ���Ҽ�
	*/
};

static u8 getKey();
u8 readKey()
{
	static u8 cnt=1,lastkey=0;
	static bit long_press=0; //������־����Ϊ1���ʾ��������������Ϊ�̰���δ����
	if(getKey()==lastkey && lastkey!=0)
	{
		++cnt;
		if(cnt==8)
		{
			return KeyCodeMap[lastkey];
		}
		if(cnt==75 && long_press)
		{
			cnt=1;
		}
		if(cnt==200) //����400ms���ǳ���,��˺�ÿ150ms��Ϊ����һ�ΰ�ť
		{
			long_press=1;
			cnt=1;
			return KeyCodeMap[lastkey];
		}
	}
	else
	{
		long_press=0;
		lastkey=getKey();
		cnt=1;
	}
	return 0;
}
u8 getKey()
{
	keyout1 = 0;
	keyout2 = 1;
	keyout3 = 1;
	keyout4 = 1;
	if (keyin1 == 0)
		return 1;
	if (keyin2 == 0)
		return 2;
	if (keyin3 == 0)
		return 3;
	if (keyin4 == 0)
		return 4;

	keyout1 = 1;
	keyout2 = 0;
	keyout3 = 1;
	keyout4 = 1;
	if (keyin1 == 0)
		return 5;
	if (keyin2 == 0)
		return 6;
	if (keyin3 == 0)
		return 7;
	if (keyin4 == 0)
		return 8;

	keyout1 = 1;
	keyout2 = 1;
	keyout3 = 0;
	keyout4 = 1;
	if (keyin1 == 0)
		return 9;
	if (keyin2 == 0)
		return 10;
	if (keyin3 == 0)
		return 11;
	if (keyin4 == 0)
		return 12;

	keyout1 = 1;
	keyout2 = 1;
	keyout3 = 1;
	keyout4 = 0;
	if (keyin1 == 0)
		return 13;
	if (keyin2 == 0)
		return 14;
	if (keyin3 == 0)
		return 15;
	if (keyin4 == 0)
		return 16;

	return 0;
}