#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include "led.h"
#include "main.h"
#include "key.h"
#include "usart.h"

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
    
    //��ʼ������
    Init_USART(115200);// @NOTE 
    Init_USART0RecDMA();
    USART0_SendData("Hello World\r\n");
  
	while(1)
	{
        //���ڽ��մ�����
        USART0_RecHandle();
	}
}
