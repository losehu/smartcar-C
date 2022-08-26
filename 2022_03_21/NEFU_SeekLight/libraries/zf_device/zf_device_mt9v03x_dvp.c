/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_mt9v03x
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             version:
*                   V1.1 2021.12.23 ����ͷ�ɼ���ɱ�־λ����volatile����
*
*                   ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   TXD                 �鿴 zf_device_mt9v03x.h �� MT9V03X_COF_UART_TX_DVP        �궨��
*                   RXD                 �鿴 zf_device_mt9v03x.h �� MT9V03X_COF_UART_RX_DVP        �궨��
*                   D0                  �鿴 zf_device_mt9v03x.h �� MT9V03X_D0_PIN_DVP             �궨��
*                   D1                  �鿴 zf_device_mt9v03x.h �� MT9V03X_D1_PIN_DVP             �궨��
*                   D2                  �鿴 zf_device_mt9v03x.h �� MT9V03X_D2_PIN_DVP             �궨��
*                   D3                  �鿴 zf_device_mt9v03x.h �� MT9V03X_D3_PIN_DVP             �궨��
*                   D4                  �鿴 zf_device_mt9v03x.h �� MT9V03X_D4_PIN_DVP             �궨��
*                   D5                  �鿴 zf_device_mt9v03x.h �� MT9V03X_D5_PIN_DVP             �궨��
*                   D6                  �鿴 zf_device_mt9v03x.h �� MT9V03X_D6_PIN_DVP             �궨��
*                   D7                  �鿴 zf_device_mt9v03x.h �� MT9V03X_D7_PIN_DVP             �궨��
*                   PCLK                �鿴 zf_device_mt9v03x.h �� MT9V03X_PCLK_PIN_DVP           �궨��
*                   VSYNC               �鿴 zf_device_mt9v03x.h �� MT9V03X_VSY_PIN_DVP            �궨��
*                   HSYNC               �鿴 zf_device_mt9v03x.h �� MT9V03X_HERF_PIN_DVP           �궨��
*                   ------------------------------------
********************************************************************************************************************/

#include "zf_device_mt9v03x_dvp.h"
#include "zf_device_camera.h"
#include "zf_device_type.h"


uint8_t *camera_buffer_addr;                                                    // ����ͷ��������ַָ��

uint8_t volatile mt9v03x_finish_flag_dvp = 0;                                                // һ��ͼ��ɼ���ɱ�־λ
uint8_t mt9v03x_image_dvp[MT9V03X_DVP_H][MT9V03X_DVP_W];

static          uint8_t     receive_dvp[3];
static          uint8_t     receive_num_dvp = 0;
static volatile uint8_t     uart_receive_flag_dvp;

//��Ҫ���õ�����ͷ������
int16_t mt9v03x_set_confing_buffer_dvp[CONFIG_FINISH][2]=
{//63 450 150 0 0 50 1
    {AUTO_EXP,          63},                                                     // �Զ��ع�����       ��Χ1-63 0Ϊ�ر� ����Զ��ع⿪��  EXP_TIME�������õ����ݽ����Ϊ����ع�ʱ�䣬Ҳ�����Զ��ع�ʱ�������
                                                                                // һ������ǲ���Ҫ����������ܣ���Ϊ�������ع���һ�㶼�ȽϾ��ȣ�����������߷ǳ������ȵ�������Գ������ø�ֵ������ͼ���ȶ���
    {EXP_TIME,          450},                                                   // �ع�ʱ��         ����ͷ�յ�����Զ����������ع�ʱ�䣬������ù���������Ϊ�������������ع�ֵ
    {FPS,               300},                                                    // ͼ��֡��         ����ͷ�յ�����Զ���������FPS���������������Ϊ������������FPS
    {SET_COL,           MT9V03X_DVP_W},                                         // ͼ��������        ��Χ1-752     K60�ɼ���������188
    {SET_ROW,           MT9V03X_DVP_H},                                         // ͼ��������        ��Χ1-480
    {LR_OFFSET,         0},                                                     // ͼ������ƫ����  ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ188 376 752ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
    {UD_OFFSET,         0},                                                     // ͼ������ƫ����  ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ120 240 480ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
    {GAIN,              50},                                                    // ͼ������         ��Χ16-64     ����������ع�ʱ��̶�������¸ı�ͼ�������̶�
    {PCLK_MODE,         1},                                                     //�������MT9V034 V1.5�Լ����ϰ汾֧�ָ����
                                                                                //����ʱ��ģʽ���� PCLKģʽ     Ĭ�ϣ�0     ��ѡ����Ϊ��0 1��        0������������źţ�1����������źš�(ͨ��������Ϊ0�����ʹ��CH32V307��DVP�ӿڻ�STM32��DCMI�ӿڲɼ���Ҫ����Ϊ1)


    {INIT,              0}                                                      // ����ͷ��ʼ��ʼ��
};

//������ͷ�ڲ���ȡ������������
int16_t mt9v03x_get_confing_buffer_dvp[CONFIG_FINISH-1][2]=
{
    {AUTO_EXP,          0},                                                     // �Զ��ع�����
    {EXP_TIME,          0},                                                     // �ع�ʱ��
    {FPS,               0},                                                     // ͼ��֡��
    {SET_COL,           0},                                                     // ͼ��������
    {SET_ROW,           0},                                                     // ͼ��������
    {LR_OFFSET,         0},                                                     // ͼ������ƫ����
    {UD_OFFSET,         0},                                                     // ͼ������ƫ����
    {GAIN,              0},                                                     // ͼ������
    {PCLK_MODE,         0},                                                     //����ʱ��ģʽ(�������MT9V034 V1.5�Լ����ϰ汾֧�ָ�����)
};

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��������ͷ�ڲ�������Ϣ �ڲ�����
// @param       uartn           ѡ��ʹ�õĴ���
// @param       buff            ����������Ϣ�ĵ�ַ
// @return      uint8_t         1-ʧ�� 0-�ɹ�
// Sample usage:                ���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
static uint8_t mt9v03x_set_config_dvp (int16_t buff[CONFIG_FINISH-1][2])
{
    uint16_t temp, i;
    uint8_t  send_buffer[4];
    volatile int16_t timeout = MT9V03X_INIT_TIMEOUT;

    uart_receive_flag_dvp = 0;

    // ���ò���  ������ο���������ֲ�
    // ��ʼ��������ͷ�����³�ʼ��
    for(i=0; i<CONFIG_FINISH; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = buff[i][0];
        temp = buff[i][1];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8_t)temp;
        uart_write_buffer(MT9V03X_COF_UART_DVP,send_buffer,4);

        system_delay_ms(2);
    }

    while(!uart_receive_flag_dvp && timeout-- > 0)                                   // �ȴ����ܻش�����
    {
        system_delay_ms(1);
    }
    timeout = MT9V03X_INIT_TIMEOUT;

    uart_receive_flag_dvp = 0;
    while(((0xff != receive_dvp[1]) || (0xff != receive_dvp[2])) && timeout-- > 0)   // �ж������Ƿ��ȡ����Ӧ����
    {
        system_delay_ms(1);
    }

    // ���ϲ��ֶ�����ͷ���õ�����ȫ�����ᱣ��������ͷ��51��Ƭ����eeprom��
    // ����set_exposure_time�����������õ��ع����ݲ��洢��eeprom��
    if(timeout <= 0)                                                         // ��ʱ
        return 1;
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ȡ����ͷ�ڲ�������Ϣ �ڲ�����
// @param       uartn           ѡ��ʹ�õĴ���
// @param       buff            ����������Ϣ�ĵ�ַ
// @return      uint8_t         1-ʧ�� 0-�ɹ�
// Sample usage:                ���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
static uint8_t mt9v03x_get_config_dvp (int16_t buff[CONFIG_FINISH-1][2])
{
    uint16_t temp, i;
    uint8_t  send_buffer[4];
    volatile int16_t timeout = MT9V03X_INIT_TIMEOUT;

    for(i=0; i<(CONFIG_FINISH-1); i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = GET_STATUS;
        temp = buff[i][0];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8_t)temp;
        uart_write_buffer(MT9V03X_COF_UART_DVP,send_buffer,4);

        timeout = MT9V03X_INIT_TIMEOUT;
        while(!uart_receive_flag_dvp && timeout-- > 0)                              // �ȴ����ܻش�����
        {
            system_delay_ms(1);
        }
        if(timeout <= 0 && i==0)                                                     // ��ʱ
            return 1;

        uart_receive_flag_dvp = 0;
        buff[i][1] = receive_dvp[1]<<8 | receive_dvp[2];
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       MT9V03X����ͷ�����жϻص�����
// @param       void
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_uart_callback_dvp (void)
{
//    receive_dvp[receive_num_dvp] = ((USART_TypeDef*)uart_index[MT9V03X_COF_UART_DVP])->DATAR & 0xFF;
//    receive_num_dvp++;

    if(uart_query_byte(MT9V03X_COF_UART_DVP, &receive_dvp[receive_num_dvp]))
        receive_num_dvp++;

    if(1 == receive_num_dvp && 0XA5!=receive_dvp[0])
        receive_num_dvp = 0;
    if(3 == receive_num_dvp)
    {
        receive_num_dvp = 0;
        uart_receive_flag_dvp = 1;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ȡ����ͷ�̼��汾
// @param       void            ѡ��ʹ�õĴ���
// @return      uint16_t        0-��ȡ���� N-�汾��
// Sample usage:                ���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
uint16_t mt9v03x_get_version_dvp (void)
{
    uint16_t temp;
    uint8_t  send_buffer[4];
    volatile int16_t timeout = MT9V03X_INIT_TIMEOUT;

    send_buffer[0] = 0xA5;
    send_buffer[1] = GET_STATUS;
    temp = GET_VERSION;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8_t)temp;
    uart_write_buffer(MT9V03X_COF_UART_DVP,send_buffer,4);

    while(!uart_receive_flag_dvp && timeout-- > 0)                                  // �ȴ����ܻش�����
    {
        system_delay_ms(1);
    }
    uart_receive_flag_dvp = 0;

    if(timeout <= 0)                                                            // ��ʱ
        return 0;
    return ((uint16_t)(receive_dvp[1]<<8) | receive_dvp[2]);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������������ͷ�ع�ʱ��
// @param       uartn           ѡ��ʹ�õĴ���
// @param       light           �����ع�ʱ��Խ��ͼ��Խ��������ͷ�յ������ݷֱ��ʼ�FPS��������ع�ʱ��������õ����ݹ�����ô����ͷ��������������ֵ
// @return      uint8_t         1-ʧ�� 0-�ɹ�
// Sample usage:                ���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
uint8_t mt9v03x_set_exposure_time_dvp (uint16_t light)
{
    uint16_t temp;
    uint8_t  send_buffer[4];
    volatile int16_t timeout = MT9V03X_INIT_TIMEOUT;

    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_EXP_TIME;
    temp = light;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8_t)temp;
    uart_write_buffer(MT9V03X_COF_UART_DVP,send_buffer,4);

    while(!uart_receive_flag_dvp && timeout-- > 0)                                  // �ȴ����ܻش�����
    {
        system_delay_ms(1);
    }
    uart_receive_flag_dvp = 0;

    temp = receive_dvp[1]<<8 | receive_dvp[2];

    if(timeout <= 0 || temp != light)
        return 0;
    return 1;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ͷ�ڲ��Ĵ�������д����
// @param       uartn           ѡ��ʹ�õĴ���
// @param       addr            ����ͷ�ڲ��Ĵ�����ַ
// @param       data            ��Ҫд�������
// @return      uint8_t         1-ʧ�� 0-�ɹ�
// Sample usage:                ���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
uint8_t mt9v03x_set_reg_dvp (uint8_t addr, uint16_t data)
{
    uint16_t temp;
    uint8_t  send_buffer[4];
    volatile int16_t timeout = MT9V03X_INIT_TIMEOUT;

    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_ADDR;
    temp = addr;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8_t)temp;
    uart_write_buffer(MT9V03X_COF_UART_DVP,send_buffer,4);

    system_delay_ms(10);
    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_DATA;
    temp = data;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8_t)temp;
    uart_write_buffer(MT9V03X_COF_UART_DVP,send_buffer,4);

    while(!uart_receive_flag_dvp && timeout-- > 0)                                  // �ȴ����ܻش�����
    {
        system_delay_ms(1);
        timeout--;
    }
    uart_receive_flag_dvp = 0;

    temp = receive_dvp[1]<<8 | receive_dvp[2];
    if(timeout <= 0 || temp != data)
        return 0;
    return 1;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V03X����ͷ���ж�
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:               ��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_handler_dvp(void)
{
  //  DVP->DMA_BUF0 = (uint32_t)camera_buffer_addr;       // �ָ�DMA��ַ



    mt9v03x_finish_flag_dvp = 1;                        // ����ͷ�ɼ���ɱ�־λ��1
  //  DVP->CR0 |= RB_DVP_ENABLE;                          // ���´�DVP���ɼ���һ��ͼ��
}



//-------------------------------------------------------------------------------------------------------------------
// @brief       MT9V03X����ͷ��ʼ��
// @param       void
// @return      uint8_t         1-ʧ�� 0-�ɹ�
// Sample usage:                ʹ��FLEXIO�ӿڲɼ�����ͷ
//-------------------------------------------------------------------------------------------------------------------
uint8_t mt9v03x_init_dvp (void)
{
    set_camera_type(CAMERA_GRAYSCALE);             //������������ͷ����Ϊ�����

    uart_init (MT9V03X_COF_UART_DVP, MT9V03X_COF_BAUR_DVP, MT9V03X_COF_UART_RX_DVP, MT9V03X_COF_UART_TX_DVP);    //��ʼ������ ��������ͷ
    uart_rx_interrupt(MT9V03X_COF_UART_DVP, ENABLE);

    interrupt_enable_all(0);

    //�ȴ�����ͷ�ϵ��ʼ���ɹ� ��ʽ�����֣���ʱ����ͨ����ȡ���õķ�ʽ ��ѡһ
    //system_delay_ms(1000);                                                    // ��ʱ��ʽ
    uart_receive_flag_dvp = 0;
    if(mt9v03x_get_config_dvp(mt9v03x_get_confing_buffer_dvp))                  // ��ȡ���õķ�ʽ
    {
        zf_assert(0);
        return 1;
    }

    uart_receive_flag_dvp = 0;
    if(mt9v03x_set_config_dvp(mt9v03x_set_confing_buffer_dvp))
    {
        zf_assert(0);
        return 1;
    }

    uart_receive_flag_dvp = 0;
    //��ȡ���ñ��ڲ鿴�����Ƿ���ȷ
    if(mt9v03x_get_config_dvp(mt9v03x_get_confing_buffer_dvp))
    {
        zf_assert(0);
        return 1;
    }

    interrupt_disable_all();

    // DVP���ų�ʼ��
    dvp_gpio_init(
            MT9V03X_D0_PIN_DVP, MT9V03X_D1_PIN_DVP, MT9V03X_D2_PIN_DVP, MT9V03X_D3_PIN_DVP,
            MT9V03X_D4_PIN_DVP, MT9V03X_D5_PIN_DVP, MT9V03X_D6_PIN_DVP, MT9V03X_D7_PIN_DVP,
            MT9V03X_PCLK_PIN_DVP, MT9V03X_VSY_PIN_DVP, MT9V03X_HERF_PIN_DVP);
    // ���õ�ַ
    camera_buffer_addr = mt9v03x_image_dvp[0];

    // DVP�ӿڳ�ʼ��
    dvp_camera_init((uint32_t *)camera_buffer_addr, NULL, MT9V03X_DVP_W*MT9V03X_DVP_H, 1);
    return 0;
}
