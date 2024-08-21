#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include "led.h"
#include "main.h"
#include "key.h"
#include "usart.h"
#include "ec11.h"
#include "adc.h"

//ADC�������ݻ�����
float adcData[100]={0};

int main(void)
{
    int i=0;
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
    
    //��ʼ��ADC����
    ADC_Config();
    
	while(1)
	{
       for(i=0;i<1;i++)
       {
            adcData[i]=(Get_ADC_Value(ADC_CHANNEL_3)*3.3f)/4096.0f; 
            delay_1us(20);                              //�������ʱ����̫��������ɼ�����ʧ�棬̫��������̫��
       }
       for(i=0;i<1;i++)
       {
           printf("%.1f ",adcData[i]);
       }
       printf("\r\n");
       delay_1ms(500);
	}
}
