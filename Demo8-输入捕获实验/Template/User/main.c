#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include "led.h"
#include "main.h"
#include "key.h"
#include "timer.h"
#include "usart.h"
#include "freq.h"

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
    
    //��ʼ������0
    Init_USART(115200);
    
    //��ʼ��Ƶ�ʲ�����ʱ��
    Init_FreqTimer();
    
	while(1)
	{
        
	}
}
