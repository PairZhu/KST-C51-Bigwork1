#include "KeyBoard.h"

u8 code KeyCodeMap[] = { //矩阵按键编号到标准键盘键码的映射表
	0x00,					//空字符
	0x31, 0x32, 0x33, 0x26 , //数字键 1、数字键 2、数字键 3、向上键
	0x34, 0x35, 0x36, 0x25 , //数字键 4、数字键 5、数字键 6、向左键
	0x37, 0x38, 0x39, 0x28 , //数字键 7、数字键 8、数字键 9、向下键
	0x30, 0x1B, 0x0D, 0x27 , //数字键 0、ESC 键、 回车键、 向右键
	/*
	//以下为长按情况，将对应键值最高位置为1，即与0x80位或所得值
	0xB1, 0xB2, 0xB3, 0xA6 , //长按：数字键 1、数字键 2、数字键 3、向上键
	0xB4, 0xB5, 0xB6, 0xA5 , //长按：数字键 4、数字键 5、数字键 6、向左键
	0xB7, 0xB8, 0xB9, 0xA8 , //长按：数字键 7、数字键 8、数字键 9、向下键
	0xB0, 0x9B, 0x8D, 0xA7   //长按：数字键 0、ESC 键、 回车键、 向右键
	*/
};

static u8 getKey();
u8 readKey()
{
	static u8 cnt=1,lastkey=0;
	static bit long_press=0; //长按标志，若为1则表示键被长按，否则为短按或未按下
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
		if(cnt==200) //按了400ms即是长按,则此后每150ms视为按下一次按钮
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