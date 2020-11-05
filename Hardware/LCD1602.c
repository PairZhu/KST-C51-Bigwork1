#include "LCD1602.h"

/* �ȴ�Һ��׼���� */
void LcdWaitReady()
{
    u8 sta;
    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do
    {
        LCD1602_E = 1;
        sta = LCD1602_DB; //��ȡ״̬��
        LCD1602_E = 0;
    } while (sta & 0x80); //bit7 ���� 1 ��ʾҺ����æ���ظ����ֱ������� 0 Ϊֹ
}
/* �� LCD1602 Һ��д��һ�ֽ����cmd-��д������ֵ */
void LcdWriteCmd(u8 cmd)
{
    LcdWaitReady();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_E = 1;
    LCD1602_E = 0;
}
/* �� LCD1602 Һ��д��һ�ֽ����ݣ�dat-��д������ֵ */
void LcdWriteDat(unsigned char dat)
{
    LcdWaitReady();
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_DB = dat;
    LCD1602_E = 1;
    LCD1602_E = 0;
}
/* ������ʾ RAM ��ʼ��ַ���༴���λ�ã�(x,y)-��Ӧ��Ļ�ϵ��ַ����� */
void LcdSetCursor(u8 x, u8 y)
{
    unsigned char addr;
    if (y == 0)          //���������Ļ���������ʾ RAM �ĵ�ַ
        addr = 0x00 + x; //��һ���ַ���ַ�� 0x00 ��ʼ
    else
        addr = 0x40 + x;      //�ڶ����ַ���ַ�� 0x40 ��ʼ
    LcdWriteCmd(addr | 0x80); //���� RAM ��ַ
}
/* ��Һ������ʾ�ַ�����(x,y)-��Ӧ��Ļ�ϵ���ʼ���꣬
str-�ַ���ָ�룬len-����ʾ���ַ����� */
void LcdShowStr(u8 x, u8 y, unsigned char *str)
{
    LcdSetCursor(x, y); //������ʼ��ַ
    while (*str)       //����д�� len ���ַ�����
    {
        LcdWriteDat(*str++); //��ȡ str ָ������ݣ�Ȼ�� str �Լ� 1
    }
}
/*����*/
void LcdClear()
{
    LcdWriteCmd(0x01);
}
/* ��ʼ�� 1602 Һ�� */
void InitLcd1602(bit input)
{
    LcdWriteCmd(0x38); //16*2 ��ʾ��5*7 ����8 λ���ݽӿ�
    if(input)
        LcdWriteCmd(0x0F);
    else
        LcdWriteCmd(0x0C); //��ʾ���������ر�
    LcdWriteCmd(0x06); //���ֲ�������ַ�Զ�+1
    LcdClear();
}