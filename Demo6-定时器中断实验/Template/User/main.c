#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include "led.h"
#include "main.h"
#include "key.h"
#include "timer.h"

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
    
    //��ʼ��ͨ�ö�ʱ��15��ʱ����ڣ�((7199+1)*(period+1))/72000000��s,
    Init_GeneralTimer(4999);
	while(1)
	{
        
	}
}
