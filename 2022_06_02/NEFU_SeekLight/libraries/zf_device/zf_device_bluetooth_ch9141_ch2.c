/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_bluetooth_ch9141
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note
********************************************************************************************************************/

#include "zf_device_bluetooth_ch9141_ch2.h"
#include "zf_device_type.h"

static  fifo_struct         bluetooth_ch9141_fifo_ch2;
static  uint8_t             bluetooth_ch9141_buffer_ch2[BLUETOOTH_CH9141_BUFFER_SIZE_CH2];                                // ��Ӧ��һ�� ���ߴ��� ���ݴ������

static           uint8_t    bluetooth_ch9141_data_ch2;
static  volatile uint8_t    bluetooth_ch9141_at_reply_ch2[30];      // ����at����Ļ���
static  volatile uint8_t    bluetooth_ch9141_at_reply_index_ch2;    // ����at����Ļ���
static  volatile uint32_t   bluetooth_ch9141_at_cmd_state_ch2;      // AT ָ��״̬ <0-���� AT ģʽ> <1-AT ģʽ�ȴ���Ӧ> <2-AT ģʽ��Ӧ�ɹ�>

static           uint8_t    bluetooth_ch9141_mac_address_ch2[17];              //����mac��ַ



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�� �����жϻص�����
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       �ú�����ISR�ļ� ����8�жϳ��򱻵���
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_uart_callback_ch2 (void)
{
    uart_query_byte(BLUETOOTH_CH9141_INDEX_CH2, &bluetooth_ch9141_data_ch2);                // ��ȡ��������
    fifo_write_buffer(&bluetooth_ch9141_fifo_ch2, &bluetooth_ch9141_data_ch2, 1);           // ���� FIFO

    if(bluetooth_ch9141_at_cmd_state_ch2 == 1)                                              // �����ǰ�� AT ����ģʽ��
    {
        fifo_read_buffer(
            &bluetooth_ch9141_fifo_ch2,                                                    // �Ӷ�Ӧ FIFO
            (uint8_t *)&bluetooth_ch9141_at_reply_ch2[bluetooth_ch9141_at_reply_index_ch2++],   // ��ȡ�� AT ��Ӧ������ ������Ӧ����������
            (uint32_t *)&bluetooth_ch9141_at_cmd_state_ch2, FIFO_READ_AND_CLEAN);           // Ĭ�ϲ���

        if( bluetooth_ch9141_at_reply_index_ch2 >= 4 &&                                     // ��ȡ��һ������ �ж��Ƿ��� AT ָ��ĳɹ���Ӧ
            !strncmp("OK\r\n", (const char *)&bluetooth_ch9141_at_reply_ch2[bluetooth_ch9141_at_reply_index_ch2-4], 4))
        {
            bluetooth_ch9141_at_cmd_state_ch2 = 2;                                          // �л��� AT ָ����Ӧ�ɹ�״̬
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�鷢��һ��û�в���������ȴ�Ӧ���ź� �ڲ�����
//  @param      channel         �����ģ��ͨ��
//  @param      *str            ��Ҫ���͵����� �����ַ���
//  @return     void
//  Sample usage:
//  @note                       �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_send_at_command (const uint8_t *str)
{
   uint16_t timeout = 0;

   bluetooth_ch9141_at_reply_index_ch2 = 0;        // ������������
   uart_write_string(BLUETOOTH_CH9141_INDEX_CH2, str);
   uart_write_string(BLUETOOTH_CH9141_INDEX_CH2, (const uint8_t *)"\r\n");

   // �ȴ��յ�Ӧ���ź�
   while(bluetooth_ch9141_at_cmd_state_ch2 != 2)
   {
       system_delay_ms(1);
       if(timeout++ > BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)                   // ��ʱ
             return 1;
   }
   bluetooth_ch9141_at_cmd_state_ch2 = 1;
   return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�鷢��һ�����в���������ȴ�Ӧ���ź�
//  @param      *cmd    ��Ҫ���͵���������
//  @param      *data   ��Ҫ���͵�����
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       �û��������
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_send_at_command_parameter (const uint8_t *cmd, const uint8_t *data)
{
   uint16_t timeout = 0;
   bluetooth_ch9141_at_reply_index_ch2 = 0;        //������������
   uart_write_string(BLUETOOTH_CH9141_INDEX_CH2, (const uint8_t *)"AT+");
   uart_write_string(BLUETOOTH_CH9141_INDEX_CH2, cmd);
   uart_write_string(BLUETOOTH_CH9141_INDEX_CH2, (const uint8_t *)"=");
   uart_write_string(BLUETOOTH_CH9141_INDEX_CH2, data);
   uart_write_string(BLUETOOTH_CH9141_INDEX_CH2, (const uint8_t *)"\r\n");

   //�ȴ��յ�Ӧ���ź�
   while(bluetooth_ch9141_at_cmd_state_ch2 != 2)
   {
       system_delay_ms(1);
       if(timeout++ > BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)                            // ��ʱ
             return 1;
   }
   bluetooth_ch9141_at_cmd_state_ch2 = 1;
   return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�����ATģʽ �ڲ�����
//  @param      channel         �����ģ��ͨ��
//  @return     void
//  Sample usage:
//  @note                       �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_enter_at_mode (void)
{
   system_delay_ms(600);    //���ͽ���ATģʽ������ǰ��Ҫ��֤ģ����550ms��û�н��չ��κ�����
   bluetooth_ch9141_at_cmd_state_ch2 = 1;             //����ATģʽ
   bluetooth_ch9141_at_reply_index_ch2 = 0;        //������������

   return bluetooth_ch9141_send_at_command((const uint8_t *)"AT...");
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ���˳�ATģʽ
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_exit_at_mode (void)
{
   if(bluetooth_ch9141_send_at_command((const uint8_t *)"AT+EXIT"))
       return 1;
   bluetooth_ch9141_at_cmd_state_ch2 = 0;            //����͸��ģʽ
   return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�鸴λ
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_reset (void)
{
   uint16_t timeout = 0;
   if(bluetooth_ch9141_send_at_command((const uint8_t *)"AT+RESET"))
       return 1;
   while(bluetooth_ch9141_at_reply_index_ch2 < 7)
   {
       system_delay_ms(1);
       if(timeout++ > BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)                            // ��ʱ
             return 1;
   }
   bluetooth_ch9141_at_reply_index_ch2 = 0;
   bluetooth_ch9141_at_cmd_state_ch2 = 1;
   return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ���ȡ����MAC��ַ
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       ���ô˺�����Ҫ�ȵ���bluetooth_ch9141_enter_at_mode��������CMD���� ����ATģʽ
//              ��Ҫ�ر�ע��bluetooth_ch9141_enter_at_mode�����ڲ���500ms����ʱ
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_get_mac_address (void)
{
   if(bluetooth_ch9141_send_at_command((const uint8_t *)"AT+MAC?"))
       return 1;


   // mac��ַΪС�θ�ʽ mac_address[0] ������� mac ��ַ���λ
   memcpy(bluetooth_ch9141_mac_address_ch2, (uint8_t *)bluetooth_ch9141_at_reply_ch2, 17);
   return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�����÷��͹���
//  @param      tx_power    ���÷��͹��ʣ�������ѡ��鿴CH9141_TX_POWEER_enumö�ٳ�Ա
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_set_tx_power (bluetooth_ch9141_tx_power_enum tx_power)
{
   uint8_t tx_power_data;

   tx_power_data = tx_power + 0x30;
   return bluetooth_ch9141_send_at_command_parameter((const uint8_t *)"TPL", (const uint8_t *)&tx_power_data);

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ������ģʽ
//  @param      mode    ģʽ���ã�������ѡ��鿴CH9141_MODE_enumö�ٳ�Ա
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_set_mode (void)
{
   uint8_t mode_data;

   mode_data = (uint8_t)BLUETOOTH_CH9141_MODE_CH2 + 0x30;
   return bluetooth_ch9141_send_at_command_parameter((const uint8_t *)"BLEMODE", (const uint8_t *)&mode_data);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ���ȡ״̬
//  @param      mode    ģʽ���ã�������ѡ��鿴CH9141_MODE_enumö�ٳ�Ա
//  @return     CH9141_STATUS_enum  ����״̬��Ϣ
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
static bluetooth_ch9141_state_enum bluetooth_ch9141_get_status (bluetooth_ch9141_mode_enum mode)
{
   bluetooth_ch9141_state_enum ch9141_status;
   uint8_t mode_data;

   mode_data = (uint8_t)mode + 0x30;
   bluetooth_ch9141_send_at_command_parameter((const uint8_t *)"BLEMODE", (const uint8_t *)&mode_data);

   bluetooth_ch9141_send_at_command((const uint8_t *)"AT+BLESTA?");

   ch9141_status = (bluetooth_ch9141_at_reply_ch2[0] - '0') * 10 + (bluetooth_ch9141_at_reply_ch2[1] - '0');
   if(BLUETOOTH_CH9141_SLAVE == mode)
   {
       ch9141_status += BLUETOOTH_CH9141_SLAVE_NO_INIT;
   }

   return ch9141_status;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ���ȡ״̬
//  @param      mode    ģʽ���ã�������ѡ��鿴CH9141_MODE_enumö�ٳ�Ա
//  @return     CH9141_STATUS_enum  ����״̬��Ϣ
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_wait_connect_success (void)
{
   int16_t timeout = 0;
   bluetooth_ch9141_state_enum ret;

   //�ȴ��յ�Ӧ���ź�
   while(BLUETOOTH_CH9141_MASTER_CONNECTED != ret)
   {
       system_delay_ms(1);
       ret = bluetooth_ch9141_get_status(BLUETOOTH_CH9141_MODE_CH2);
       if(timeout++ >= 9)                            // ��ʱ
             return 1;
   }
   return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�������豸����
//  @param      *str    ��������
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       ���Ƴ��Ȳ��ܳ���18���ַ� ��ֻ��ΪӢ��������
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_set_name (void)
{
   if(bluetooth_ch9141_send_at_command_parameter((const uint8_t *)"NAME", (const uint8_t *)BLUETOOTH_CH9141_SELF_NAME_CH2))
       return 1;
   if(bluetooth_ch9141_send_at_command_parameter((const uint8_t *)"PNAME", (const uint8_t *)BLUETOOTH_CH9141_SELF_NAME_CH2))
       return 1;
   return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ����������
//  @param      enable      ʹ������ 0����ʹ�����룬1��ʹ������������ӱ��豸
//  @param      *password   ������ַ��� ����Ϊ6���ַ�
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_set_password (uint8_t enable)
{
   if(0 == enable)
   {
       //�ر�����
       if(bluetooth_ch9141_send_at_command_parameter((const uint8_t *)"PASEN", (const uint8_t *)"OFF"))
           return 1;
   }
   else
   {
       //�������벢ʹ��
       if(bluetooth_ch9141_send_at_command_parameter((const uint8_t *)"PASEN", (const uint8_t *)"ON"))
           return 1;
       if(bluetooth_ch9141_send_at_command_parameter((const uint8_t *)"PASS", (const uint8_t *)BLUETOOTH_CH9141_SELF_PASSWORD_CH2))
           return 1;
   }
   return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ��Ĭ�����Ӳ������ã����úú�ÿ�ο����������Զ���������豸��
//  @param      *mac_and_password      ��Ҫ���ӵ��豸mac��ַ������
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       bluetooth_ch9141_default_connect("58:B7:33:E4:C2:84,000000");
//              58:B7:33:E4:C2:84Ϊmac��ַ ,Ϊ�ָ��� 000000Ϊ�ӻ���������
//              ===================�ر�ע��==================
//              ���ʹ���ֻ��鿴CH9141��mac��ַ����CH9141����Ϊ�ӻ���ʹ���ֻ�����
//              ��ʹ�ñ��������ӵ�ʱ���뽫mac����һ��
//              �����ֻ��鿴����mac��ַΪ61:62:63:64:65:67����ʹ�ñ��������ӵ�ʱ��Ӧ��д
//              bluetooth_ch9141_default_connect("67:65:64:63:62:61,000000");
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_default_connect (void)
{
   return bluetooth_ch9141_send_at_command_parameter((const uint8_t *)"CONADD", (const uint8_t *)BLUETOOTH_CH9141_TAGET_INFO_CH2);
}

static uint8_t bluetooth_ch9141_get_connect_mac (uint8_t *mac_buffer)
{
    if(bluetooth_ch9141_send_at_command((const uint8_t *)"AT+CCADD?"))
       return 1;

   // mac��ַΪС�θ�ʽ mac_address[0] ������� mac ��ַ���λ
    memcpy(mac_buffer, (uint8_t *)bluetooth_ch9141_at_reply_ch2, 17);
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ��ָ��MAC��ַ��������������
//  @param      *mac_and_password      ��Ҫ���ӵ��豸mac��ַ������
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       bluetooth_ch9141_connect("58:B7:33:E4:C2:84,000000");
//              58:B7:33:E4:C2:84Ϊmac��ַ ,Ϊ�ָ��� 000000Ϊ�ӻ���������
//              ===================�ر�ע��==================
//              ���ʹ���ֻ��鿴������mac��ַ����ʹ�ñ��������ӵ�ʱ���뽫mac����һ��
//              �����ֻ��鿴����mac��ַΪ61:62:63:64:65:66����ʹ�ñ��������ӵ�ʱ��Ӧ��д
//              bluetooth_ch9141_connect("66:65:64:63:62:61,000000");
//-------------------------------------------------------------------------------------------------------------------
uint8_t bluetooth_ch9141_connect_ch2 (void)
{
   return bluetooth_ch9141_send_at_command_parameter((const uint8_t *)"CONN", (const uint8_t *)BLUETOOTH_CH9141_TAGET_INFO_CH2);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ���ȡrssi(�ź�ǿ��)
//  @param      void
//  @return     int8_t    �����ź�ǿ��0��-127
//  @since      v1.0
//  Sample usage:
//  @note       ���ô˺�����Ҫ�ȵ���bluetooth_ch9141_enter_at_mode��������CMD���� ����ATģʽ
//              ��Ҫ�ر�ע��bluetooth_ch9141_enter_at_mode�����ڲ���500ms����ʱ
//-------------------------------------------------------------------------------------------------------------------
int16_t bluetooth_ch9141_get_rssi_ch2 (void)
{
   uint8_t i;
   size_t length;
   int16_t  rssi;

   if(bluetooth_ch9141_send_at_command_parameter((const uint8_t *)"RSSI", (const uint8_t *)"ON,0"))
       return 200;
   length = strlen((const char *)bluetooth_ch9141_at_reply_ch2);
   length -= 12;//����RSSI �ж���λ

   rssi = 0;
   for(i=0; i<length; i++)
   {
       rssi = rssi*10 + (bluetooth_ch9141_at_reply_ch2[0] - '0');
   }

   return -rssi;
}





//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ���ʼ��
//  @param      mode    ����ģʽ MASTER_MODE(����)����SLAVE_MODE(�ӻ�)
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
uint8_t bluetooth_ch9141_init_ch2 (void)
{
    uint8_t mac_buffer_temp[17];
    set_wireless_type(BULETOOTH_CH9141);
    interrupt_enable_all(0);
   //������ʹ�õĲ�����Ϊ115200��Ϊ����ת����ģ���Ĭ�ϲ����ʣ�����������������ʹ����λ���޸�ģ�����
   //��ʼ����������

    bluetooth_ch9141_at_reply_index_ch2 = 0;            // atģʽʱ����ָʾ���ݽ��յ�����
    bluetooth_ch9141_at_cmd_state_ch2 = 0;              // OKӦ��������ճɹ��ı�־λ
    memset((void *)bluetooth_ch9141_at_reply_ch2,0,30); // ����at����Ļ���

    fifo_init(&bluetooth_ch9141_fifo_ch2, bluetooth_ch9141_buffer_ch2, BLUETOOTH_CH9141_BUFFER_SIZE_CH2);
    uart_init(BLUETOOTH_CH9141_INDEX_CH2, BLUETOOTH_CH9141_BUAD_RATE_CH2, BLUETOOTH_CH9141_RX_PIN_CH2, BLUETOOTH_CH9141_TX_PIN_CH2);
    gpio_init(BLUETOOTH_CH9141_RTS_PIN_CH2, GPI, 1, GPI_PULL_UP);
    uart_rx_interrupt(BLUETOOTH_CH9141_INDEX_CH2, 1);

    //������Ϊ������ӻ�ģʽ������������Ҫ���ӳɹ��ͱ�����һ��Ϊ��������һ��Ϊ�ӻ������Ե��ó�ʼ����ʱ����Ҫ�������д�����������ܳɹ�������
    //������Ϊ������ӻ�ģʽ������������Ҫ���ӳɹ��ͱ�����һ��Ϊ��������һ��Ϊ�ӻ������Ե��ó�ʼ����ʱ����Ҫ�������д�����������ܳɹ�������
    //�ϵ�˳����ôӻ����ϵ磬Ȼ���������ϵ�


    //2.��������Ϊ�ӻ����ȴ�����
    if(bluetooth_ch9141_enter_at_mode())  //����ATģʽ
    {
        zf_log(0, "CH9141 enter_at_mode faile");
        return 1;
    }

    if(bluetooth_ch9141_get_connect_mac(mac_buffer_temp))
    {
        zf_log(0, "CH9141 get_connect_mac faile");
        return 1;
    }

    if(strncmp((const char *)mac_buffer_temp, (const char *)BLUETOOTH_CH9141_TAGET_INFO, 17) == 0)
    {
        if(bluetooth_ch9141_exit_at_mode())          //�˳�ATģʽ
        {
            zf_log(0, "CH9141 exit_at_mode faile");
            return 1;
        }
        return 0;
    }

    if(bluetooth_ch9141_set_mode())   //��������ģʽ
    {
        zf_log(0, "CH9141 set_mode faile");
        return 1;
    }
    if(bluetooth_ch9141_get_mac_address())//��ȡ����MAC��ַ
    {
        zf_log(0, "CH9141 get_mac_address faile");
        return 1;
    }
    if(bluetooth_ch9141_reset())                  //������ɺ���Ҫ��λ�����òŻ���Ч
    {
        zf_log(0, "CH9141 reset faile");
        return 1;
    }
    system_delay_ms(800);
    if(bluetooth_ch9141_enter_at_mode())          //����ATģʽ
    {
        zf_log(0, "CH9141 enter_at_mode faile");
        return 1;
    }
    //������ģʽ֮����Ҫ��λȻ���ٴν���ATģʽ���ܼ���������������������ģʽ���ò��ɹ�
    if(bluetooth_ch9141_set_tx_power(BLUETOOTH_CH9141_DEFAULT_TX_POWER))//�����������͹���
    {
        zf_log(0, "CH914 mode set_tx_power ack.");
        return 1;
    }

    if(BLUETOOTH_CH9141_MASTER == BLUETOOTH_CH9141_MODE)
    {
        //===================�ر�ע��==================
        //���ʹ���ֻ��鿴������mac��ַ����ʹ�ñ��������ӵ�ʱ���뽫mac����һ��
        //�����ֻ��鿴����mac��ַΪ61:62:63:64:65:66����ʹ�ñ��������ӵ�ʱ��Ӧ��д
        //bluetooth_ch9141_connect("66:65:64:63:62:61,000000");
        //58:B7:33:E4:C2:84Ϊmac��ַ ,Ϊ�ָ��� 000000Ϊ�ӻ���������
        //�ӻ�MAC��ַ������
        //int8_t salve_mac_password[] = "80:90:2C:E4:C2:84,000000";
        if(bluetooth_ch9141_default_connect())  //����Ĭ�����Ӳ�������ʹ�´β�����Ҳ���Զ����Ӵӻ�
        {
            zf_log(0, "CH9141 default_connect faile");
            return 1;
        }
        if(bluetooth_ch9141_connect_ch2())          //�����������õĴӻ���ַ
        {
            zf_log(0, "CH9141 connect faile");
            return 1;
        }
    }
    else if(BLUETOOTH_CH9141_SLAVE == BLUETOOTH_CH9141_MODE)
    {
        if(bluetooth_ch9141_set_name())
        {
            zf_log(0, "CH9141 set_name faile");
            return 1;
        }
        if(bluetooth_ch9141_set_password(1))  //000000Ϊ������������Լ��޸�
        {
            zf_log(0, "CH9141 set_password faile");
            return 1;
        }
        if(bluetooth_ch9141_reset())                  //������ɺ���Ҫ��λ�����òŻ���Ч
        {
            zf_log(0, "CH9141 reset faile");
            return 1;
        }
        if(bluetooth_ch9141_enter_at_mode())          //����ATģʽ
        {
            zf_log(0, "CH9141 enter_at_mode faile");
            return 1;
        }
    }
    //�ȴ����ӳɹ�
    if(bluetooth_ch9141_wait_connect_success())
    {
        zf_log(0, "CH9141 connect faile.");
        return 1;
    }
    if(bluetooth_ch9141_exit_at_mode())          //�˳�ATģʽ
    {
        zf_log(0, "CH9141 exit_at_mode faile.");
        return 1;
    }
    //������ȡ�����ź�ǿ�ȿ��԰��������ʾ������
    //bluetooth_ch9141_enter_at_mode();
    //int16_t  rssi = bluetooth_ch9141_get_rssi();
    interrupt_disable_all();
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�� ���ͺ���
//  @param      buff        ��Ҫ���͵����ݵ�ַ
//  @param      len         ���ͳ���
//  @return     uint32      ʣ��δ���͵��ֽ���
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
uint32_t bluetooth_ch9141_send_buff_ch2 (uint8_t *buff, uint32_t len)
{
    uint16_t time_count = 0;
    while(len > 30)
    {
        time_count = 0;
        while(gpio_get_level(BLUETOOTH_CH9141_RTS_PIN_CH2) && time_count++ < BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)                  // ���RTSΪ�͵�ƽ���������������
            system_delay_ms(1);
        if(time_count >= BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)
            return len;                                                                         // ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
        uart_write_buffer(BLUETOOTH_CH9141_INDEX_CH2, buff, 30);

        buff += 30;                                                                             // ��ַƫ��
        len -= 30;                                                                              // ����
    }

    time_count = 0;
    while(gpio_get_level(BLUETOOTH_CH9141_RTS_PIN_CH2) && time_count++ < BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)                      // ���RTSΪ�͵�ƽ���������������
        system_delay_ms(1);
    if(time_count >= BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)
        return len;                                                                             // ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
    uart_write_buffer(BLUETOOTH_CH9141_INDEX_CH2, buff, len);                                              // ������������

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�� ��ȡ����
//  @param      buff        ��Ҫ��ȡ�����ݵ�ַ
//  @param      len         ���ͳ���
//  @return     uint32      ��ȡ�����ݳ��� ���û����ô����������ᱻ�޸�
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
uint32_t bluetooth_ch9141_read_buff_ch2(uint8_t *buff, uint32_t len)
{
    uint32_t data_len = len;
    fifo_read_buffer(&bluetooth_ch9141_fifo_ch2, buff, &data_len, FIFO_READ_AND_CLEAN);
    return data_len;
}





