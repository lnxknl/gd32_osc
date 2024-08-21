#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include "led.h"
#include "main.h"
#include "key.h"
#include "usart.h"
#include "ec11.h"

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

	//��ʼ������
	Init_USART(115200);
	//��ʼ������DMA
	Init_USART0RecDMA();

	//��ʼ��EC11����
	Init_EC11_GPIO();
	Init_EC11_EXITGPIO();
	
	while(1)
	{
		
	}
}
