#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include "led.h"
#include "main.h"


int main(void)
{
    //	��ʼ���δ�ʱ��
	systick_config();	
	
    //��ʼ��LED����
	Init_LED_GPIO();
	
	while(1)
	{
        //��LED2��LED3
        Open_LED(led2);Open_LED(led3);
        
        //��ʱ500ms
        delay_1ms(500);
        
        //�ر�LED2��LED3
        CLose_LED(led2);CLose_LED(led3);
        
        //��ʱ500ms
        delay_1ms(500);
	}
}
