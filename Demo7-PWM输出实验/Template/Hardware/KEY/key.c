#include "key.h"
#include "led.h"

/*
*   �������ݣ���ʼ������GPIO
*   ������������
*   ����ֵ��  ��
*/
void Init_Key_GPIO(void)
{
    //ʹ��ʱ��
    rcu_periph_clock_enable(RCU_GPIOB);
    
    //�������ģʽ������
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
}

/*
*   �������ݣ���ʼ������GPIO
*   ������������
*   ����ֵ��  uint8_t ����ֵ
*/
uint8_t Scanf_Key_GPIO(void)
{
    if(gpio_input_bit_get(GPIOB,GPIO_PIN_13)==RESET)
    {
        delay_1ms(10);
        if(gpio_input_bit_get(GPIOB,GPIO_PIN_13)==RESET)
        {
            while(gpio_input_bit_get(GPIOB,GPIO_PIN_13)==RESET);
            return KEY1;
        }
    }
    if(gpio_input_bit_get(GPIOB,GPIO_PIN_15)==RESET)
    {
        delay_1ms(10);
        if(gpio_input_bit_get(GPIOB,GPIO_PIN_15)==RESET)
        {
            while(gpio_input_bit_get(GPIOB,GPIO_PIN_15)==RESET);
            return KEY2;
        }
    }
    if(gpio_input_bit_get(GPIOB,GPIO_PIN_14)==RESET)
    {
        delay_1ms(10);
        if(gpio_input_bit_get(GPIOB,GPIO_PIN_14)==RESET)
        {
            while(gpio_input_bit_get(GPIOB,GPIO_PIN_14)==RESET);
            return KEY3;
        }
    }
    return 0;
}

/*
*   �������ݣ�����������
*   ����������uint8_t ����ֵ
*   ����ֵ��  ��
*/
void Key_Handle(uint8_t keyValue)
{
    switch(keyValue)
    {
        case KEY1:
            Open_LED(led2);
            break;
        case KEY2:
            Open_LED(led3);
            break;
        case KEY3:
            CLose_LED(led2);
            CLose_LED(led3);
            break;
        default:
            break;
    }
}