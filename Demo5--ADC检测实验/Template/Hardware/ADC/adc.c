#include "adc.h"

/**********************************************************
 * �� �� �� �ƣ�ADC_Config
 * �� �� �� �ܣ���ʼ��ADC
 * �� �� �� ������
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע��
**********************************************************/
void ADC_Config(void)// @NOTE 
{
    //ʹ������ʱ��
    rcu_periph_clock_enable(RCU_GPIOA);      
    //ʹ��ADCʱ��
    rcu_periph_clock_enable(RCU_ADC);
    //����ADCʱ��
    rcu_adc_clock_config(RCU_ADCCK_AHB_DIV3);
    //��������Ϊģ�⸡������ģʽ
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_3);
    
    //�����Ҷ���
    adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
    
    //ͨ������1
    adc_channel_length_config( ADC_REGULAR_CHANNEL, 1U);
    
    //����ⲿ����
    adc_external_trigger_source_config( ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE); 
    //�ⲿ����ʹ��
    adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);

    //ʹ��adc
    adc_enable();
    delay_1ms(1U);
    //������У׼
    adc_calibration_enable();
}

/**********************************************************
 * �� �� �� �ƣ�Get_ADC_Value
 * �� �� �� �ܣ���ȡADCֵ
 * �� �� �� ����ADC_CHANNEL_x=Ҫ�ɼ���ͨ��
 * �� �� �� �أ���������ֵ
 * ��       �ߣ�LC
 * ��       ע��Ĭ�ϲ�������Ϊ55.5��ADC����ʱ��
**********************************************************/
uint16_t Get_ADC_Value(uint8_t  ADC_CHANNEL_x)
{
    float vref=0;
    float adc_value = 0;
        
    //���òɼ�ͨ��
    adc_regular_channel_config(0, ADC_CHANNEL_x, ADC_SAMPLETIME_55POINT5);
    //��ʼ���ת��
    adc_software_trigger_enable(ADC_REGULAR_CHANNEL);

    //�ȴ� ADC ������� 
    while ( adc_flag_get(ADC_FLAG_EOC) == RESET ){            
    }
    adc_value = adc_regular_data_read();
    
    //���ز���ֵ
    return adc_value;
}
