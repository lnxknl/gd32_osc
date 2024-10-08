#include "usart.h"
#include "string.h"

#define USART0_RDATA_ADDRESS      ((uint32_t)&USART_RDATA(USART0))

//接收完成标志位
static __IO uint8_t receive_flag = 0;
//接受数据数量
static uint8_t rx_count = 0;
//接收数据缓冲区
static uint8_t rxbuffer[256];

/*
*   函数内容：初始化打印串口
*   函数参数：bound--波特率
*   返回值：  无
*/
void Init_USART(uint32_t bound)
{
    //使能时钟
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);
    
    //复用功能设置
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_10);
    
    //端口模式设置
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_9);
    
    //端口模式设置
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_10);
    
    //复位USART0
    usart_deinit(USART0);
    usart_word_length_set(USART0, USART_WL_8BIT);       //8位数据位宽
    usart_stop_bit_set(USART0, USART_STB_1BIT);         //1位停止位
    usart_parity_config(USART0, USART_PM_NONE);         //无校验位
    usart_baudrate_set(USART0, bound);                  //波特率
    usart_receive_config(USART0, USART_RECEIVE_ENABLE); //接收使能
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);//发送使能
    usart_dma_receive_config(USART0, USART_DENR_ENABLE);//DAM接收使能
    //使能串口0
    usart_enable(USART0);
    
    //等待串口空闲
    while(RESET == usart_flag_get(USART0, USART_FLAG_IDLE));
    //清除空闲标志位
    usart_flag_clear(USART0, USART_FLAG_IDLE);
    //空闲中断使能
    usart_interrupt_enable(USART0,USART_INT_IDLE);
}

/*
*   函数内容：初始化串口0接收DMA
*   函数参数：无
*   返回值：  无
*/
void Init_USART0RecDMA(void)
{
    //DMA参数结构体
    dma_parameter_struct dma_init_struct;
    
    //中断使能
    nvic_irq_enable(USART0_IRQn, 0); 
    
    //DMA时钟使能
    rcu_periph_clock_enable(RCU_DMA);
    
    //复位DMA通道，DMA通道2对应串口0的接收
    dma_deinit(DMA_CH2);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;       //从外设到内存
    dma_init_struct.memory_addr = (uint32_t)rxbuffer;           //内存地址
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;    //内存地址自增
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;       //内存地址宽度-8bit
    dma_init_struct.number = 256;                               //DMA最大传输数量
    dma_init_struct.periph_addr = USART0_RDATA_ADDRESS;         //外设地址
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;   //外设地址不自增
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;   //外设地址宽度
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;         //中断优先级高
    dma_init(DMA_CH2, &dma_init_struct);                        //初始化DMA通道2
    
    //DAM循环模式失能
    dma_circulation_disable(DMA_CH2);
    
    //使能DMA通道2
    dma_channel_enable(DMA_CH2);
}

/*
*   函数内容：USART0发送函数
*   函数参数：char *SendData-发送数据指针
*   返回值：  无
*/
void USART0_SendData(char *SendData)
{
    usart_flag_clear(USART0,USART_FLAG_TC);
    while(*SendData != '\0')
    {
       usart_data_transmit(USART0, (uint8_t)*SendData);
       while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
       usart_flag_clear(USART0,USART_FLAG_TC);
       SendData++;
    }
}


/*
*   函数内容：fputc端口重定向
*   函数参数：无
*   返回值：  无
*/
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t) ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}

/*
*   函数内容：串口0中断服务函数
*   函数参数: 无
*   返回值：  无
*/
void USART0_IRQHandler(void)
{
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE)){
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE);
        
        //接收数据数量
        rx_count = 256 - (dma_transfer_number_get(DMA_CH2));
        //接收完成标志位
        receive_flag = 1;
        
        //失能DMA通道
        dma_channel_disable(DMA_CH2);
        //重新配置DMA计数
        dma_transfer_number_config(DMA_CH2, 256);
        //使能DMA通道
        dma_channel_enable(DMA_CH2);
    }
}

/*
*   函数内容：串口0接收数据处理函数
*   函数参数: 无
*   返回值：  无
*/
void USART0_RecHandle(void)
{
    //如果数据接收完成
    if(receive_flag == 1)
    {
        printf("%s\r\n",rxbuffer);
        memset(rxbuffer,0,256);
        receive_flag=0;
        rx_count=0;
    }
  
}