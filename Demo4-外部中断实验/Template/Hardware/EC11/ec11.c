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
	
	//�ж�ʹ��
	nvic_irq_enable(EXTI4_15_IRQn,1U);
	
	//�����ж���
	syscfg_exti_line_config(EXTI_SOURCE_GPIOB,EXTI_SOURCE_PIN4);// @NOTE 
	
	//��ʼ���ж��ߣ�����Ϊ�ж�ģʽ�������غ��½��ش���
	exti_init(EXTI_4,EXTI_INTERRUPT,EXTI_TRIG_BOTH);
	
  //�ж���ʹ��
  exti_interrupt_enable(EXTI_4);
    
	//�жϱ�־λ���
	exti_interrupt_flag_clear(EXTI_4);    
}

uint8_t A_cnt=0;
uint8_t B_value=0;
/*
*   �������ݣ��ж���4-15�жϷ�����
*   ������������
*   ����ֵ��  ��
*/
void EXTI4_15_IRQHandler(void)// @NOTE 
{
   if(RESET != exti_interrupt_flag_get(EXTI_4))  
   {
		 if((gpio_input_bit_get(GPIOB,GPIO_PIN_4) == RESET) && (A_cnt == 0))	//A���½��ش���һ��
		 {
			 A_cnt++;			//����ֵ��һ����ʾ�Ѿ������˵�һ���ж�
			 B_value = 0;	//��ȡB���ƽ����Ϊ�ߵ�ƽ��B_level��1����֮����0
			 if(gpio_input_bit_get(GPIOB,GPIO_PIN_3) == SET)
			 {
				 B_value = 1;
			 }
		 }
		 else if((gpio_input_bit_get(GPIOB,GPIO_PIN_4) == SET) && (A_cnt == 1))	//A�������ش���һ��
		 {
			 A_cnt = 0;
			 if((B_value == 1) && (gpio_input_bit_get(GPIOB,GPIO_PIN_3) == RESET))
			 {
				 Open_LED(1);CLose_LED(2);
				 //��ת
			 }
			 if((B_value == 0) && (gpio_input_bit_get(GPIOB,GPIO_PIN_3) == SET))
			 {
				 Open_LED(2);CLose_LED(1);
				 //��ת
			 }
		 }
		 exti_interrupt_flag_clear(EXTI_4);
   }
}
