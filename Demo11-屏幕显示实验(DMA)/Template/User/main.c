#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include <string.h>
#include "led.h"
#include "main.h"
#include "tft_init.h"
#include "tft.h"

int main(void)
{
    //	��ʼ���δ�ʱ��
    systick_config();
    
    //��ʼ��LED����
    Init_LED_GPIO();
    
    //��ʼ��TFT��Ļ���ż�Ĭ������
    TFT_Init();
    
    //TFT��ʾ��ɫ�������
    TFT_Fill(0,0,160,128,WHITE);
    while(1)
    {
        
    }
}
