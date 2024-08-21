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
    //��ʼ���δ�ʱ��
	systick_config();	
	
    //��ʼ��LED����
	Init_LED_GPIO();
	
    //��ʼ����������
    Init_Key_GPIO();
    
    //��ʼ��PWM��������Ƶ��=��72000000/((71+1)*(period+1))��
    //ռ�ձ���������Ƚ�ֵ���жϣ�ͬʱ���뼫����ģʽ�й�
    //�Ƚ�ֵ*��������ռ�ձȣ�500/1000=50%ռ�ձȣ�������1K
    Init_PWM_Output(999,500);
    
	while(1)
	{
        
	}
}
