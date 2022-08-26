/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_driver_uart
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/


#include "zf_driver_gpio.h"
#include "zf_driver_uart.h"






// 该数组禁止修改，内部使用用户无需关心
const uint8_t uart_irq[] = {USART1_IRQn, USART2_IRQn, USART3_IRQn, UART4_IRQn, UART5_IRQn, UART6_IRQn, UART7_IRQn, UART8_IRQn};
const uint32_t uart_index[] = {USART1_BASE, USART2_BASE, USART3_BASE, UART4_BASE, UART5_BASE, UART6_BASE, UART7_BASE, UART8_BASE};
//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口引脚初始化
//  @param      tx_pin      串口发送引脚号
//  @param      rx_pin      串口接收引脚号
//  @return     void
//  Sample usage:           内部使用，用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void uart_gpio_init(uart_pin_enum tx_pin, uart_pin_enum rx_pin)
{
    if     (tx_pin == UART1_TX_A9 )     gpio_init(A9,  GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART1_TX_B6 )     gpio_init(B6,  GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART2_TX_A2 )     gpio_init(A2,  GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART2_TX_D5 )     gpio_init(D5,  GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART3_TX_B10)     gpio_init(B10, GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART3_TX_C10)     gpio_init(C10, GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART3_TX_D8 )     gpio_init(D8,  GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART4_TX_C10)     gpio_init(C10, GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART4_TX_B0)      gpio_init(B0, GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART5_TX_C12)     gpio_init(C12, GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART5_TX_E8)      gpio_init(E8, GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART6_TX_C0 )     gpio_init(C0,  GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART6_TX_E10 )    gpio_init(C0,  GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART7_TX_C2 )     gpio_init(C2,  GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART7_TX_A6 )     gpio_init(A6,  GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART7_TX_E12 )    gpio_init(E12,  GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART8_TX_C4 )     gpio_init(C4,  GPO, 0, GPO_AF_PUSH_PULL);
    else if(tx_pin == UART8_TX_E14 )    gpio_init(E14,  GPO, 0, GPO_AF_PUSH_PULL);

    if     (rx_pin == UART1_RX_A10)     gpio_init(A10, GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART1_RX_B7 )     gpio_init(B7,  GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART2_RX_A3 )     gpio_init(A3,  GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART2_RX_D6 )     gpio_init(D6,  GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART3_RX_B11)     gpio_init(B11, GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART3_RX_C11)     gpio_init(C11, GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART3_RX_D9 )     gpio_init(D9,  GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART4_RX_C11)     gpio_init(C11, GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART4_RX_B1)      gpio_init(B1, GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART5_RX_D2 )     gpio_init(D2,  GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART5_RX_E9 )     gpio_init(E9,  GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART6_RX_C1 )     gpio_init(C1,  GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART6_RX_E11 )    gpio_init(E11,  GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART7_RX_C3 )     gpio_init(C3,  GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART7_RX_A7 )     gpio_init(A7,  GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART7_RX_E13 )    gpio_init(E13,  GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART8_RX_C5 )     gpio_init(C5,  GPI, 0, GPI_PULL_UP);
    else if(rx_pin == UART8_RX_E15 )    gpio_init(E15,  GPI, 0, GPI_PULL_UP);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送一个字节
//  @param      uartn       串口通道
//  @param      dat         串口数据
//  @return     void
//  Sample usage:           uart_write_byte(UART_1, 0x43);        //串口1发送0x43。
//-------------------------------------------------------------------------------------------------------------------
void uart_write_byte(uart_index_enum uartn, uint8_t dat)
{
    while((((USART_TypeDef*)uart_index[uartn])->STATR & USART_FLAG_TXE)==0);
    ((USART_TypeDef*)uart_index[uartn])->DATAR = dat;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送数组
//  @param      uartn       串口通道
//  @param      buff        要发送的数组地址
//  @param      len         数据长度
//  @return     void
//  Sample usage:           uart_write_buffer(UART_1, buff, 10);     //串口1发送10个buff数组。
//-------------------------------------------------------------------------------------------------------------------
void uart_write_buffer(uart_index_enum uartn, uint8_t *buff, uint32_t len)
{
    while(len--)
        uart_write_byte(uartn, *buff++);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送字符串
//  @param      uartn       串口通道
//  @param      str         字符串首地址
//  @return     void
//  Sample usage:           uart_putstr(UART_1, (uint8_t *)"12345")   //串口1发送12345这个字符串
//-------------------------------------------------------------------------------------------------------------------
void uart_write_string(uart_index_enum uartn, const uint8_t *str)
{
    while(*str)
    {
        uart_write_byte(uartn, *str++);
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      中断初始化
//  @param      irqn                        中断号，可以查看CH32V30x.h文件中的IRQn_Type枚举体定义
//  @param      preemption_priority         抢占优先级值越小，优先级越高 (可选参数0-7，最高0，最低7)
//  @param      sub_priority                次优先级值越小，优先级越高     (可选参数0-7，最高0，最低7)
//  @param      status                      使能或者失能
//  @return     void
//  Sample usage:                           nvic_init(EXTI0_IRQn,0, 0, ENABLE); //外部中断0使能,抢占优先级最高，次优先级最高。
//-------------------------------------------------------------------------------------------------------------------
void nvic_init(IRQn_Type irqn, uint8_t preemption_priority, uint8_t sub_priority, uint8_t status)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = irqn;                                  //中断号设置
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=preemption_priority;   //抢占优先级值越小，优先级越高
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_priority;               //次优先级值越小，优先级越高
    NVIC_InitStructure.NVIC_IRQChannelCmd = status;                             //使能
    NVIC_Init(&NVIC_InitStructure);

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      打开串口接收中断
//  @param      uartn       串口通道
//  @param      status      使能或者失能
//  @return     void
//  Sample usage:           uart_rx_irq(UART_1, ENABLE);        //打开串口1接收中断
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_interrupt(uart_index_enum uartn, uint8_t status)
{
    USART_ITConfig(((USART_TypeDef*)uart_index[uartn]), USART_IT_RXNE, status);

    NVIC->IPRIOR[(uint32_t)((IRQn_Type)uart_irq[uartn])] = 0;

    if(status)  interrupt_enable((IRQn_Type)uart_irq[uartn]);
    else        interrupt_disable((IRQn_Type)uart_irq[uartn]);

    //nvic_init((IRQn_Type)uart_irq[uartn], 1, 0, status);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      打开串口发送中断
//  @param      uartn       串口通道
//  @param      status      使能或者失能
//  @return     void
//  Sample usage:           uart_tx_irq(UART_1, DISABLE);       //关闭串口1发送 中断
//-------------------------------------------------------------------------------------------------------------------
void uart_tx_interrupt(uart_index_enum uartn, uint8_t status)
{
    USART_ITConfig(((USART_TypeDef*)uart_index[uartn]), USART_IT_TXE, status);

    NVIC->IPRIOR[(uint32_t)((IRQn_Type)uart_irq[uartn])] = 0;

    if(status)  interrupt_enable((IRQn_Type)uart_irq[uartn]);
    else        interrupt_disable((IRQn_Type)uart_irq[uartn]);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取串口接收的数据（whlie等待）
//  @param      uartn           串口模块号(UART_1,UART_2,UART_3,UART_4)
//  @param      *dat            接收数据的地址
//  @return     void
//  Sample usage:               uint8_t dat; uart_read_byte(USART_1,&dat);       // 接收串口1数据  存在在dat变量里
//-------------------------------------------------------------------------------------------------------------------
void uart_read_byte(uart_index_enum uartn, uint8_t *dat)
{
    while((((USART_TypeDef*)uart_index[uartn])->STATR & USART_FLAG_RXNE) == 0);
    *dat = (uint16_t)(((USART_TypeDef*)uart_index[uartn])->DATAR & (uint16_t)0xFF);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取串口接收的数据（查询接收）
//  @param      uartn           串口模块号(UART_1,UART_2,UART_3,UART_4)
//  @param      *dat            接收数据的地址
//  @return     uint8_t           1：接收成功   0：未接收到数据
//  Sample usage:               uint8_t dat; uart_query_byte(USART_1,&dat);       // 接收串口1数据  存在在dat变量里
//-------------------------------------------------------------------------------------------------------------------
uint8_t uart_query_byte(uart_index_enum uartn, uint8_t *dat)
{
    if((((USART_TypeDef*)uart_index[uartn])->STATR & USART_FLAG_RXNE) != 0)
    {
        *dat = (((USART_TypeDef*)uart_index[uartn])->DATAR & 0xFF);
        return 1;
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口初始化
//  @param      uartn       串口通道
//  @param      baud        波特率
//  @param      tx_pin      串口发送引脚号
//  @param      rx_pin      串口接收引脚号
//  @return     void
//  Sample usage:           uart_init(UART_1, 115200, UART1_TX_A9, UART1_RX_A10); //串口1初始化引脚号,TX为A9,RX为A10
//-------------------------------------------------------------------------------------------------------------------
void uart_init(uart_index_enum uart_num, uint32_t baud, uart_pin_enum tx_pin, uart_pin_enum rx_pin)
{
    USART_InitTypeDef USART_InitStructure;

    // AFIO总线开启
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);


    // RX引脚和TX引脚必须组传入
//    zf_assert(((rx_pin - 1) == tx_pin) || (tx_pin == UART5_TX_C12 && UART5_RX_D2 == rx_pin));

    // 引脚初始化
    uart_gpio_init(tx_pin, rx_pin);

    // 端口重映射
    if(UART1_TX_B6 == tx_pin && UART1_RX_B7 == rx_pin)
        GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
    else if(UART2_TX_D5 == tx_pin && UART2_RX_D6 == rx_pin)
        GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
    else if(UART3_TX_C10 == tx_pin && UART3_RX_C11 == rx_pin)
        GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
    else if(UART3_TX_D8 == tx_pin && UART3_RX_D9 == rx_pin)
        GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
    else if(UART4_TX_B0 == tx_pin && UART4_RX_B1 == rx_pin)
        GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
    else if(UART5_TX_E8 == tx_pin && UART5_RX_E9 == rx_pin)
        GPIO_PinRemapConfig(GPIO_Remap_TIM5CH4_LSI, ENABLE);
    else if(UART6_TX_E10 == tx_pin && UART6_RX_E11 == rx_pin)
        GPIO_PinRemapConfig(GPIO_Remap_TIM67_DAC_DMA, ENABLE);

    else if(UART7_TX_A6 == tx_pin && UART7_RX_A7 == rx_pin)
        AFIO->PCFR2 |= 0x01<<22;                       //[23:22]
    else if(UART7_TX_E12 == tx_pin && UART7_RX_E13 == rx_pin)
        AFIO->PCFR2 |= 0x01<<23;                       //[23:22]

    else if(UART8_TX_E14 == tx_pin && UART8_RX_E15 == rx_pin)
        AFIO->PCFR2 |= 1<<25;    //[25:24]

    //  串口总线使能
    if(UART_1 == uart_num) RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    else if(UART_2 == uart_num) RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    else if(UART_3 == uart_num) RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    else if(UART_4 == uart_num) RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    else if(UART_5 == uart_num) RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    else if(UART_6 == uart_num) RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART6, ENABLE);
    else if(UART_7 == uart_num) RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);
    else if(UART_8 == uart_num) RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE);


    //  串口参数配置
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    //  使能串口
    USART_Init((USART_TypeDef*)uart_index[uart_num], &USART_InitStructure);
    USART_Cmd((USART_TypeDef*)uart_index[uart_num], ENABLE);

}




