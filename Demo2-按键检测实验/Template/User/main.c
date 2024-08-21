#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include "led.h"
#include "main.h"
#include "key.h"

int main(void)
{
    //����ֵ
    uint8_t keyValue=0;
    //	��ʼ���δ�ʱ��
	systick_config();	
	
    //��ʼ��LED����
	Init_LED_GPIO();
	
    //��ʼ����������
    Init_Key_GPIO();
	while(1)
	{
      //����ɨ��
      keyValue=Scanf_Key_GPIO();
      
      //��������
      Key_Handle(keyValue);
	}
}
