#include "ec11.h"
#include "led.h"

/*
*   �������ݣ���ʼ��EC11����,DE��B��������ͨ����һ��
*   ������������
*   ����ֵ��  ��
*/
void Init_EC11_GPIO(void)
{
    //ʹ��ʱ��
    rcu_periph_clock_enable(RCU_GPIOB);
    
    //�������ģʽ������
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_3|GPIO_PIN_9);

}

/*
*   �������ݣ���ʼ��EC11����,��ת�ж�����A��Ҫʹ���ж�
*   ������������
*   ����ֵ��  ��
*/
void Init_EC11_EXITGPIO(void)
{
	//ʹ��ʱ��GPIOB��CMP
    rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_CFGCMP);
	
	//��������ģʽ������
	gpio_mode_set(GPIOB,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,GPIO_PIN_4);
	
	//�ж���ʹ��
	nvic_irq_enable(EXTI4_15_IRQn,1U);
	
	//�����ж���
	syscfg_exti_line_config(EXTI_SOURCE_GPIOB,EXTI_SOURCE_PIN4);
	
	//��ʼ���ж��ߣ�����Ϊ�ж�ģʽ�������ش���
	exti_init(EXTI_4,EXTI_INTERRUPT,EXTI_TRIG_RISING);
	
	//�жϱ�־λ���
	exti_interrupt_flag_clear(EXTI_4);    
}

/*
*   �������ݣ��ж���4-15�жϷ�����������Ϊ�˷�ֹ�󴥣�������ת�����жϣ���Ҫ��һ���������ת���β���ɹ�
              ע���жϷ������в�Ҫ������ʱ�䳤�Ĳ���
*   ������������
*   ����ֵ��  ��
*/
void EXTI4_15_IRQHandler(void)
{
   //˳ʱ���жϼ���
   static uint8_t clockwiseNum=0;
   //��ʱ���жϼ���
   static uint8_t anticlockwiseNum=0;
    
   if(RESET != exti_interrupt_flag_get(EXTI_4))  
   {
        if(gpio_input_bit_get(GPIOB,GPIO_PIN_3)==RESET) //˳ʱ��ת����
        {
            delay_1ms(5);
            anticlockwiseNum=0;
            if(gpio_input_bit_get(GPIOB,GPIO_PIN_3)==RESET) //˳ʱ��ת����
            {
                clockwiseNum++;
                if(clockwiseNum>=2)
                {
                    clockwiseNum=0;
                    Open_LED(led2);Open_LED(led3);
                }            
            }
        }
        else                                            //��ʱ��ת����
        {
            delay_1ms(5);
            clockwiseNum=0;
            if(gpio_input_bit_get(GPIOB,GPIO_PIN_3)==SET) //��ʱ��ת����
            {
                anticlockwiseNum++;
                if(anticlockwiseNum>=2)
                {
                    anticlockwiseNum=0;
                    CLose_LED(led2);CLose_LED(led3);
                }
            }
        }
        exti_interrupt_flag_clear(EXTI_4);
    }
}