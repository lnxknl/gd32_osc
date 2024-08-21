#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include "led.h"
#include "main.h"
#include "tft_init.h"
#include "tft.h"

int main(void)
{
    uint8_t i=0;
    uint8_t data[120]={0};
    //	��ʼ���δ�ʱ��
	systick_config();	
	
    //��ʼ��LED����
	Init_LED_GPIO();
	
    //��ʼ��TFT��Ļ���ż�Ĭ������
    TFT_Init();
    //TFT��Ļ��ʾUI
    TFT_ShowUI();
    
	while(1)
	{
        //ģ��ADC�ɼ�����
        for(i=0;i<30;i++)
        {
           data[i]=0; 
        }
        for(i=30;i<60;i++)
        {
           data[i]=33; 
        }
        for(i=60;i<90;i++)
        {
           data[i]=0; 
        }
        for(i=90;i<120;i++)
        {
           data[i]=33; 
        }
        
        //��ʾ����
        for(i=0;i<120;i++)
        {
            drawCurve(80,data[i]);
        }
	}
}
