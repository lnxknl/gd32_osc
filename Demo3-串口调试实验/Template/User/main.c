#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include "led.h"
#include "main.h"
#include "key.h"
#include "usart.h"

int main(void)
{
    //按键值
    uint8_t keyValue=0;
    //	初始化滴答定时器
	systick_config();	
	
    //初始化LED引脚
	Init_LED_GPIO();
	
    //初始化按键引脚
    Init_Key_GPIO();
    
    //初始化串口
    Init_USART(115200);// @NOTE 
    Init_USART0RecDMA();
    USART0_SendData("Hello World\r\n");
  
	while(1)
	{
        //串口接收处理函数
        USART0_RecHandle();
	}
}
