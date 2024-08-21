#include "tft_init.h"


//#define TFT_SCL PA5
//#define TFT_SDA PA7
//#define TFT_RES PB5
//#define TFT_CS  PB7
//#define TFT_BLK PB8

/*
*   �������ݣ���ʼ��SPI0����
*   ������������
*   ����ֵ����
*/
void Init_SPI0_GPIO(void)
{
    //spi�ṹ�����
    spi_parameter_struct spi_init_struct;
    
    //ʹ��GPIOAʱ��
    rcu_periph_clock_enable(RCU_GPIOA);
    
    //ʹ��SPI0ʱ��
    rcu_periph_clock_enable(RCU_SPI0);
    
    //���ø��ù���
    gpio_af_set(GPIOA, GPIO_AF_0,   GPIO_PIN_5 | GPIO_PIN_7);
    
    //ģʽ����
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5 | GPIO_PIN_7);
    
    //�������
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_7);
    
    //SPI0��λ
    spi_i2s_deinit(SPI0);
    
    //SPI������ʼ��
    spi_struct_para_init(&spi_init_struct);
    
    spi_init_struct.trans_mode           = SPI_BIDIRECTIONAL_TRANSMIT;//������ģʽģʽ
    spi_init_struct.device_mode          = SPI_MASTER;              //����ģʽ
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;      //8λ����λ��
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE; //���Ըߣ���2����Ե����
    spi_init_struct.nss                  = SPI_NSS_SOFT;            //NSS�������
    spi_init_struct.prescale             = SPI_PSC_2;               //2��Ƶ
    spi_init_struct.endian               = SPI_ENDIAN_MSB;          //��λ��ǰ
    spi_init(SPI0, &spi_init_struct);
    
    //ʹ��SPI0
    spi_enable(SPI0);
}

/*
*   �������ݣ�SPI DMA����
*   ������������
*   ����ֵ����
*/
void spi_dma_config(void *buf,int size)
{
  dma_parameter_struct  dma_init_struct;
  dma_struct_para_init(&dma_init_struct);
  
  rcu_periph_clock_enable(RCU_DMA);
  
  /* configure I2S0 transmit DMA: DMA_CH2 */
  dma_deinit(DMA_CH2);
  dma_init_struct.periph_addr         = (uint32_t)(SPI0+0x0CU);
  dma_init_struct.memory_addr         = (uint32_t)buf;
  dma_init_struct.direction           = DMA_MEMORY_TO_PERIPHERAL;
  dma_init_struct.periph_width        = DMA_PERIPHERAL_WIDTH_16BIT;
  dma_init_struct.memory_width        = DMA_MEMORY_WIDTH_16BIT;
  dma_init_struct.priority            = DMA_PRIORITY_ULTRA_HIGH;
  dma_init_struct.number              = size;
  dma_init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
  dma_init_struct.memory_inc          = DMA_MEMORY_INCREASE_DISABLE;
  dma_init(DMA_CH2, &dma_init_struct);
  
  dma_circulation_disable(DMA_CH2);
  dma_memory_to_memory_disable(DMA_CH2);
}

/*
*   �������ݣ�SPI DMA������ݷ���
*   ������������
*   ����ֵ����
*/
void spi_dma_fill_transmit(uint16_t color,int size)
{
  gpio_bit_reset(GPIOB,GPIO_PIN_7);   //����Ƭѡ�ź�
  
  uint8_t buff[2] = {0};
  buff[0] = (color >> 8)&0xff;
  buff[1] = (color)&0xff;
  spi_i2s_data_frame_format_config(SPI0,SPI_FRAMESIZE_16BIT);
  spi_dma_config(buff,size);
  
  dma_channel_enable(DMA_CH2);
  
  spi_dma_enable(SPI0, SPI_DMA_TRANSMIT);
  while(RESET == dma_flag_get(DMA_CH2, DMA_FLAG_FTF)) {
  }
  spi_i2s_data_frame_format_config(SPI0,SPI_FRAMESIZE_8BIT);
  
  gpio_bit_set(GPIOB,GPIO_PIN_7);     //����Ƭѡ�ź�
}

/*
*   �������ݣ�SPI0��������
*   ������������
*   ����ֵ����
*/
static void SPI0_Write(uint8_t data)
{
    //�ȴ����ͻ���������
    while(spi_i2s_flag_get(SPI0,SPI_FLAG_TBE)==RESET);
    
    //��������
    spi_i2s_data_transmit(SPI0, data);
    
}

/*
*   �������ݣ���ʼ��TFT��������
*   ������������
*   ����ֵ����
*/
static void TFT_GPIO_Init(void)
{
    //ʹ��ʱ��
    rcu_periph_clock_enable(RCU_GPIOB);
    
    //�������ģʽ����������
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8);
    
    //����������ͣ����������50Mhz
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8);

    gpio_bit_set(GPIOB,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8);
}

/*
*   �������ݣ�TFT���͵����ֽ�����
*   ������������
*   ����ֵ����
*/
void TFT_WR_DATA8(uint8_t data)
{
    gpio_bit_reset(GPIOB,GPIO_PIN_7);   //����Ƭѡ�ź�
    
    SPI0_Write(data);
    
    gpio_bit_set(GPIOB,GPIO_PIN_7);     //����Ƭѡ�ź�
}

/*
*   �������ݣ�TFT����2���ֽ�����
*   ������������
*   ����ֵ����
*/
void TFT_WR_DATA(uint16_t data)
{
    gpio_bit_reset(GPIOB,GPIO_PIN_7);   //����Ƭѡ�ź�
    
    SPI0_Write(data>>8);
    
    SPI0_Write(data);
    
    gpio_bit_set(GPIOB,GPIO_PIN_7);     //����Ƭѡ�ź�
}

/*
*   �������ݣ�TFT������������
*   ������������
*   ����ֵ����
*/
void TFT_WR_REG(uint8_t reg)
{
    gpio_bit_reset(GPIOB,GPIO_PIN_6);   //���������ź�
    gpio_bit_reset(GPIOB,GPIO_PIN_7);   //����Ƭѡ�ź�
    
    SPI0_Write(reg);
    
    gpio_bit_set(GPIOB,GPIO_PIN_6);     //���������ź�
    gpio_bit_set(GPIOB,GPIO_PIN_7);     //����Ƭѡ�ź�
}

void TFT_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		TFT_WR_REG(0x2a);//�е�ַ����
		TFT_WR_DATA(x1);
		TFT_WR_DATA(x2);
		TFT_WR_REG(0x2b);//�е�ַ����
		TFT_WR_DATA(y1);
		TFT_WR_DATA(y2);
		TFT_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==1)
	{
		TFT_WR_REG(0x2a);//�е�ַ����
		TFT_WR_DATA(x1);
		TFT_WR_DATA(x2);
		TFT_WR_REG(0x2b);//�е�ַ����
		TFT_WR_DATA(y1);
		TFT_WR_DATA(y2);
		TFT_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==2)
	{
		TFT_WR_REG(0x2a);//�е�ַ����
		TFT_WR_DATA(x1);
		TFT_WR_DATA(x2);
		TFT_WR_REG(0x2b);//�е�ַ����
		TFT_WR_DATA(y1);
		TFT_WR_DATA(y2);
		TFT_WR_REG(0x2c);//������д
	}
	else
	{
		TFT_WR_REG(0x2a);//�е�ַ����
		TFT_WR_DATA(x1);
		TFT_WR_DATA(x2);
		TFT_WR_REG(0x2b);//�е�ַ����
		TFT_WR_DATA(y1);
		TFT_WR_DATA(y2);
		TFT_WR_REG(0x2c);//������д
	}    
}

void TFT_Init(void)
{
	//��ʼ��TFT��Ļ����
    TFT_GPIO_Init();
	//��ʼ��SPI0����
    Init_SPI0_GPIO();
    
    gpio_bit_reset(GPIOB,GPIO_PIN_5);   //��λ
    delay_1ms(100);
    gpio_bit_set(GPIOB,GPIO_PIN_5);     //��λ���
    delay_1ms(100);
    
    gpio_bit_set(GPIOB,GPIO_PIN_8);     //�򿪱���
    delay_1ms(100);
    
	//************* Start Initial Sequence **********//
	TFT_WR_REG(0x11); //Sleep out 
	delay_1ms(120);              //Delay 120ms 
	//------------------------------------ST7735S Frame Rate-----------------------------------------// 
	TFT_WR_REG(0xB1); 
	TFT_WR_DATA8(0x05); 
	TFT_WR_DATA8(0x3C); 
	TFT_WR_DATA8(0x3C); 
	TFT_WR_REG(0xB2); 
	TFT_WR_DATA8(0x05);
	TFT_WR_DATA8(0x3C); 
	TFT_WR_DATA8(0x3C); 
	TFT_WR_REG(0xB3); 
	TFT_WR_DATA8(0x05); 
	TFT_WR_DATA8(0x3C); 
	TFT_WR_DATA8(0x3C); 
	TFT_WR_DATA8(0x05); 
	TFT_WR_DATA8(0x3C); 
	TFT_WR_DATA8(0x3C); 
	//------------------------------------End ST7735S Frame Rate---------------------------------// 
	TFT_WR_REG(0xB4); //Dot inversion 
	TFT_WR_DATA8(0x03); 
	//------------------------------------ST7735S Power Sequence---------------------------------// 
	TFT_WR_REG(0xC0); 
	TFT_WR_DATA8(0x28); 
	TFT_WR_DATA8(0x08); 
	TFT_WR_DATA8(0x04); 
	TFT_WR_REG(0xC1); 
	TFT_WR_DATA8(0XC0); 
	TFT_WR_REG(0xC2); 
	TFT_WR_DATA8(0x0D); 
	TFT_WR_DATA8(0x00); 
	TFT_WR_REG(0xC3); 
	TFT_WR_DATA8(0x8D); 
	TFT_WR_DATA8(0x2A); 
	TFT_WR_REG(0xC4); 
	TFT_WR_DATA8(0x8D); 
	TFT_WR_DATA8(0xEE); 
	//---------------------------------End ST7735S Power Sequence-------------------------------------// 
	TFT_WR_REG(0xC5); //VCOM 
	TFT_WR_DATA8(0x1A); 
	TFT_WR_REG(0x36); //MX, MY, RGB mode 
	if(USE_HORIZONTAL==0){
        TFT_WR_DATA8(0x00);
    }
	else if(USE_HORIZONTAL==1){
        TFT_WR_DATA8(0xC0);
    }
	else if(USE_HORIZONTAL==2){
        TFT_WR_DATA8(0x70);
    }
	else {
        TFT_WR_DATA8(0xA0); 
    }
	//------------------------------------ST7735S Gamma Sequence---------------------------------// 
	TFT_WR_REG(0xE0); 
	TFT_WR_DATA8(0x04); 
	TFT_WR_DATA8(0x22); 
	TFT_WR_DATA8(0x07); 
	TFT_WR_DATA8(0x0A); 
	TFT_WR_DATA8(0x2E); 
	TFT_WR_DATA8(0x30); 
	TFT_WR_DATA8(0x25); 
	TFT_WR_DATA8(0x2A); 
	TFT_WR_DATA8(0x28); 
	TFT_WR_DATA8(0x26); 
	TFT_WR_DATA8(0x2E); 
	TFT_WR_DATA8(0x3A); 
	TFT_WR_DATA8(0x00); 
	TFT_WR_DATA8(0x01); 
	TFT_WR_DATA8(0x03); 
	TFT_WR_DATA8(0x13); 
	TFT_WR_REG(0xE1); 
	TFT_WR_DATA8(0x04); 
	TFT_WR_DATA8(0x16); 
	TFT_WR_DATA8(0x06); 
	TFT_WR_DATA8(0x0D); 
	TFT_WR_DATA8(0x2D); 
	TFT_WR_DATA8(0x26); 
	TFT_WR_DATA8(0x23); 
	TFT_WR_DATA8(0x27); 
	TFT_WR_DATA8(0x27); 
	TFT_WR_DATA8(0x25); 
	TFT_WR_DATA8(0x2D); 
	TFT_WR_DATA8(0x3B); 
	TFT_WR_DATA8(0x00); 
	TFT_WR_DATA8(0x01); 
	TFT_WR_DATA8(0x04); 
	TFT_WR_DATA8(0x13); 
	//------------------------------------End ST7735S Gamma Sequence-----------------------------// 
	TFT_WR_REG(0x3A); //65k mode 
	TFT_WR_DATA8(0x05); 
	TFT_WR_REG(0x29); //Display on   
}

