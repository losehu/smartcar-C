/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_device_bluetooth_ch9141
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note
********************************************************************************************************************/

#include "zf_device_bluetooth_ch9141_ch2.h"
#include "zf_device_type.h"

static  fifo_struct         bluetooth_ch9141_fifo_ch2;
static  uint8_t             bluetooth_ch9141_buffer_ch2[BLUETOOTH_CH9141_BUFFER_SIZE_CH2];                                // 对应第一个 无线串口 数据存放数组

static           uint8_t    bluetooth_ch9141_data_ch2;
static  volatile uint8_t    bluetooth_ch9141_at_reply_ch2[30];      // 接收at命令的缓存
static  volatile uint8_t    bluetooth_ch9141_at_reply_index_ch2;    // 接收at命令的缓存
static  volatile uint32_t   bluetooth_ch9141_at_cmd_state_ch2;      // AT 指令状态 <0-不在 AT 模式> <1-AT 模式等待响应> <2-AT 模式响应成功>

static           uint8_t    bluetooth_ch9141_mac_address_ch2[17];              //本机mac地址



//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块 串口中断回调函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       该函数在ISR文件 串口8中断程序被调用
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_uart_callback_ch2 (void)
{
    uart_query_byte(BLUETOOTH_CH9141_INDEX_CH2, &bluetooth_ch9141_data_ch2);                // 读取串口数据
    fifo_write_buffer(&bluetooth_ch9141_fifo_ch2, &bluetooth_ch9141_data_ch2, 1);           // 存入 FIFO

    if(bluetooth_ch9141_at_cmd_state_ch2 == 1)                                              // 如果当前是 AT 配置模式下
    {
        fifo_read_buffer(
            &bluetooth_ch9141_fifo_ch2,                                                    // 从对应 FIFO
            (uint8_t *)&bluetooth_ch9141_at_reply_ch2[bluetooth_ch9141_at_reply_index_ch2++],   // 读取到 AT 响应缓冲区 自增响应缓冲区索引
            (uint32_t *)&bluetooth_ch9141_at_cmd_state_ch2, FIFO_READ_AND_CLEAN);           // 默认参数

        if( bluetooth_ch9141_at_reply_index_ch2 >= 4 &&                                     // 获取到一段数据 判断是否是 AT 指令的成功响应
            !strncmp("OK\r\n", (const char *)&bluetooth_ch9141_at_reply_ch2[bluetooth_ch9141_at_reply_index_ch2-4], 4))
        {
            bluetooth_ch9141_at_cmd_state_ch2 = 2;                                          // 切换到 AT 指令响应成功状态
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块发送一个没有参数的命令并等待应答信号 内部调用
//  @param      channel         接入的模块通道
//  @param      *str            需要发送的命令 完整字符串
//  @return     void
//  Sample usage:
//  @note                       内部调用 用户无需关心
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_send_at_command (const uint8_t *str)
{
   uint16_t timeout = 0;

   bluetooth_ch9141_at_reply_index_ch2 = 0;        // 接收数量清零
   uart_write_string(BLUETOOTH_CH9141_INDEX_CH2, str);
   uart_write_string(BLUETOOTH_CH9141_INDEX_CH2, (const uint8_t *)"\r\n");

   // 等待收到应答信号
   while(bluetooth_ch9141_at_cmd_state_ch2 != 2)
   {
       system_delay_ms(1);
       if(timeout++ > BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)                   // 超时
             return 1;
   }
   bluetooth_ch9141_at_cmd_state_ch2 = 1;
   return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块发送一个带有参数的命令并等待应答信号
//  @param      *cmd    需要发送的命令名称
//  @param      *data   需要发送的数据
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       用户无需关心
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_send_at_command_parameter (const uint8_t *cmd, const uint8_t *data)
{
   uint16_t timeout = 0;
   bluetooth_ch9141_at_reply_index_ch2 = 0;        //接收数量清零
   uart_write_string(BLUETOOTH_CH9141_INDEX_CH2, (const uint8_t *)"AT+");
   uart_write_string(BLUETOOTH_CH9141_INDEX_CH2, cmd);
   uart_write_string(BLUETOOTH_CH9141_INDEX_CH2, (const uint8_t *)"=");
   uart_write_string(BLUETOOTH_CH9141_INDEX_CH2, data);
   uart_write_string(BLUETOOTH_CH9141_INDEX_CH2, (const uint8_t *)"\r\n");

   //等待收到应答信号
   while(bluetooth_ch9141_at_cmd_state_ch2 != 2)
   {
       system_delay_ms(1);
       if(timeout++ > BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)                            // 超时
             return 1;
   }
   bluetooth_ch9141_at_cmd_state_ch2 = 1;
   return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块进入AT模式 内部调用
//  @param      channel         接入的模块通道
//  @return     void
//  Sample usage:
//  @note                       内部调用 用户无需关心
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_enter_at_mode (void)
{
   system_delay_ms(600);    //发送进入AT模式的命令前需要保证模块在550ms内没有接收过任何数据
   bluetooth_ch9141_at_cmd_state_ch2 = 1;             //进入AT模式
   bluetooth_ch9141_at_reply_index_ch2 = 0;        //接收数量清零

   return bluetooth_ch9141_send_at_command((const uint8_t *)"AT...");
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块退出AT模式
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
   bluetooth_ch9141_at_cmd_state_ch2 = 0;            //进入透传模式
   return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块复位
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
       if(timeout++ > BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)                            // 超时
             return 1;
   }
   bluetooth_ch9141_at_reply_index_ch2 = 0;
   bluetooth_ch9141_at_cmd_state_ch2 = 1;
   return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块获取本机MAC地址
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       调用此函数需要先调用bluetooth_ch9141_enter_at_mode或者拉低CMD引脚 进入AT模式
//              需要特别注意bluetooth_ch9141_enter_at_mode函数内部有500ms的延时
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_get_mac_address (void)
{
   if(bluetooth_ch9141_send_at_command((const uint8_t *)"AT+MAC?"))
       return 1;


   // mac地址为小段格式 mac_address[0] 保存的是 mac 地址最低位
   memcpy(bluetooth_ch9141_mac_address_ch2, (uint8_t *)bluetooth_ch9141_at_reply_ch2, 17);
   return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块设置发送功率
//  @param      tx_power    设置发送功率，可设置选项查看CH9141_TX_POWEER_enum枚举成员
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
//  @brief      蓝牙转串口模块设置模式
//  @param      mode    模式设置，可设置选项查看CH9141_MODE_enum枚举成员
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
//  @brief      蓝牙转串口模块获取状态
//  @param      mode    模式设置，可设置选项查看CH9141_MODE_enum枚举成员
//  @return     CH9141_STATUS_enum  返回状态信息
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
//  @brief      蓝牙转串口模块获取状态
//  @param      mode    模式设置，可设置选项查看CH9141_MODE_enum枚举成员
//  @return     CH9141_STATUS_enum  返回状态信息
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_wait_connect_success (void)
{
   int16_t timeout = 0;
   bluetooth_ch9141_state_enum ret;

   //等待收到应答信号
   while(BLUETOOTH_CH9141_MASTER_CONNECTED != ret)
   {
       system_delay_ms(1);
       ret = bluetooth_ch9141_get_status(BLUETOOTH_CH9141_MODE_CH2);
       if(timeout++ >= 9)                            // 超时
             return 1;
   }
   return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块设置设备名称
//  @param      *str    蓝牙名称
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       名称长度不能超过18个字符 且只能为英文与数字
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
//  @brief      蓝牙转串口模块密码设置
//  @param      enable      使能密码 0：不使用密码，1：使用密码才能连接本设备
//  @param      *password   密码的字符串 必须为6个字符
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note
//-------------------------------------------------------------------------------------------------------------------
static uint8_t bluetooth_ch9141_set_password (uint8_t enable)
{
   if(0 == enable)
   {
       //关闭密码
       if(bluetooth_ch9141_send_at_command_parameter((const uint8_t *)"PASEN", (const uint8_t *)"OFF"))
           return 1;
   }
   else
   {
       //设置密码并使能
       if(bluetooth_ch9141_send_at_command_parameter((const uint8_t *)"PASEN", (const uint8_t *)"ON"))
           return 1;
       if(bluetooth_ch9141_send_at_command_parameter((const uint8_t *)"PASS", (const uint8_t *)BLUETOOTH_CH9141_SELF_PASSWORD_CH2))
           return 1;
   }
   return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块默认连接参数设置（设置好后，每次开机蓝牙会自动链接这个设备）
//  @param      *mac_and_password      需要连接的设备mac地址与密码
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       bluetooth_ch9141_default_connect("58:B7:33:E4:C2:84,000000");
//              58:B7:33:E4:C2:84为mac地址 ,为分隔符 000000为从机蓝牙密码
//              ===================特别注意==================
//              如果使用手机查看CH9141的mac地址，将CH9141设置为从机，使用手机连接
//              则使用本函数连接的时候请将mac倒置一下
//              例如手机查看到的mac地址为61:62:63:64:65:67，则使用本函数连接的时候应该写
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

   // mac地址为小段格式 mac_address[0] 保存的是 mac 地址最低位
    memcpy(mac_buffer, (uint8_t *)bluetooth_ch9141_at_reply_ch2, 17);
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块指定MAC地址并立即进行连接
//  @param      *mac_and_password      需要连接的设备mac地址与密码
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       bluetooth_ch9141_connect("58:B7:33:E4:C2:84,000000");
//              58:B7:33:E4:C2:84为mac地址 ,为分隔符 000000为从机蓝牙密码
//              ===================特别注意==================
//              如果使用手机查看蓝牙的mac地址，则使用本函数连接的时候请将mac倒置一下
//              例如手机查看到的mac地址为61:62:63:64:65:66，则使用本函数连接的时候应该写
//              bluetooth_ch9141_connect("66:65:64:63:62:61,000000");
//-------------------------------------------------------------------------------------------------------------------
uint8_t bluetooth_ch9141_connect_ch2 (void)
{
   return bluetooth_ch9141_send_at_command_parameter((const uint8_t *)"CONN", (const uint8_t *)BLUETOOTH_CH9141_TAGET_INFO_CH2);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块获取rssi(信号强度)
//  @param      void
//  @return     int8_t    返回信号强度0到-127
//  @since      v1.0
//  Sample usage:
//  @note       调用此函数需要先调用bluetooth_ch9141_enter_at_mode或者拉低CMD引脚 进入AT模式
//              需要特别注意bluetooth_ch9141_enter_at_mode函数内部有500ms的延时
//-------------------------------------------------------------------------------------------------------------------
int16_t bluetooth_ch9141_get_rssi_ch2 (void)
{
   uint8_t i;
   size_t length;
   int16_t  rssi;

   if(bluetooth_ch9141_send_at_command_parameter((const uint8_t *)"RSSI", (const uint8_t *)"ON,0"))
       return 200;
   length = strlen((const char *)bluetooth_ch9141_at_reply_ch2);
   length -= 12;//计算RSSI 有多少位

   rssi = 0;
   for(i=0; i<length; i++)
   {
       rssi = rssi*10 + (bluetooth_ch9141_at_reply_ch2[0] - '0');
   }

   return -rssi;
}





//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块初始化
//  @param      mode    蓝牙模式 MASTER_MODE(主机)或者SLAVE_MODE(从机)
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
   //本函数使用的波特率为115200，为蓝牙转串口模块的默认波特率，如需其他波特率请使用上位机修改模块参数
   //初始化流控引脚

    bluetooth_ch9141_at_reply_index_ch2 = 0;            // at模式时用于指示数据接收的数量
    bluetooth_ch9141_at_cmd_state_ch2 = 0;              // OK应答命令接收成功的标志位
    memset((void *)bluetooth_ch9141_at_reply_ch2,0,30); // 接收at命令的缓存

    fifo_init(&bluetooth_ch9141_fifo_ch2, bluetooth_ch9141_buffer_ch2, BLUETOOTH_CH9141_BUFFER_SIZE_CH2);
    uart_init(BLUETOOTH_CH9141_INDEX_CH2, BLUETOOTH_CH9141_BUAD_RATE_CH2, BLUETOOTH_CH9141_RX_PIN_CH2, BLUETOOTH_CH9141_TX_PIN_CH2);
    gpio_init(BLUETOOTH_CH9141_RTS_PIN_CH2, GPI, 1, GPI_PULL_UP);
    uart_rx_interrupt(BLUETOOTH_CH9141_INDEX_CH2, 1);

    //蓝牙分为主机与从机模式，两个蓝牙想要连接成功就必须有一个为主机，有一个为从机，所以调用初始化的时候需要合理的填写函数参数才能成功的连接
    //蓝牙分为主机与从机模式，两个蓝牙想要连接成功就必须有一个为主机，有一个为从机，所以调用初始化的时候需要合理的填写函数参数才能成功的连接
    //上电顺序：最好从机先上电，然后主机再上电


    //2.蓝牙设置为从机并等待连接
    if(bluetooth_ch9141_enter_at_mode())  //进入AT模式
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
        if(bluetooth_ch9141_exit_at_mode())          //退出AT模式
        {
            zf_log(0, "CH9141 exit_at_mode faile");
            return 1;
        }
        return 0;
    }

    if(bluetooth_ch9141_set_mode())   //设置蓝牙模式
    {
        zf_log(0, "CH9141 set_mode faile");
        return 1;
    }
    if(bluetooth_ch9141_get_mac_address())//获取本机MAC地址
    {
        zf_log(0, "CH9141 get_mac_address faile");
        return 1;
    }
    if(bluetooth_ch9141_reset())                  //设置完成后需要复位，设置才会生效
    {
        zf_log(0, "CH9141 reset faile");
        return 1;
    }
    system_delay_ms(800);
    if(bluetooth_ch9141_enter_at_mode())          //进入AT模式
    {
        zf_log(0, "CH9141 enter_at_mode faile");
        return 1;
    }
    //设置完模式之后需要复位然后再次进入AT模式才能继续设置其他参数，否则模式设置不成功
    if(bluetooth_ch9141_set_tx_power(BLUETOOTH_CH9141_DEFAULT_TX_POWER))//设置蓝牙发送功率
    {
        zf_log(0, "CH914 mode set_tx_power ack.");
        return 1;
    }

    if(BLUETOOTH_CH9141_MASTER == BLUETOOTH_CH9141_MODE)
    {
        //===================特别注意==================
        //如果使用手机查看蓝牙的mac地址，则使用本函数连接的时候请将mac倒置一下
        //例如手机查看到的mac地址为61:62:63:64:65:66，则使用本函数连接的时候应该写
        //bluetooth_ch9141_connect("66:65:64:63:62:61,000000");
        //58:B7:33:E4:C2:84为mac地址 ,为分隔符 000000为从机蓝牙密码
        //从机MAC地址与密码
        //int8_t salve_mac_password[] = "80:90:2C:E4:C2:84,000000";
        if(bluetooth_ch9141_default_connect())  //配置默认连接参数，即使下次不配置也会自动连接从机
        {
            zf_log(0, "CH9141 default_connect faile");
            return 1;
        }
        if(bluetooth_ch9141_connect_ch2())          //立即连接设置的从机地址
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
        if(bluetooth_ch9141_set_password(1))  //000000为蓝牙密码可以自己修改
        {
            zf_log(0, "CH9141 set_password faile");
            return 1;
        }
        if(bluetooth_ch9141_reset())                  //设置完成后需要复位，设置才会生效
        {
            zf_log(0, "CH9141 reset faile");
            return 1;
        }
        if(bluetooth_ch9141_enter_at_mode())          //进入AT模式
        {
            zf_log(0, "CH9141 enter_at_mode faile");
            return 1;
        }
    }
    //等待连接成功
    if(bluetooth_ch9141_wait_connect_success())
    {
        zf_log(0, "CH9141 connect faile.");
        return 1;
    }
    if(bluetooth_ch9141_exit_at_mode())          //退出AT模式
    {
        zf_log(0, "CH9141 exit_at_mode faile.");
        return 1;
    }
    //如果想获取无线信号强度可以按照下面的示例调用
    //bluetooth_ch9141_enter_at_mode();
    //int16_t  rssi = bluetooth_ch9141_get_rssi();
    interrupt_disable_all();
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块 发送函数
//  @param      buff        需要发送的数据地址
//  @param      len         发送长度
//  @return     uint32      剩余未发送的字节数
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
        while(gpio_get_level(BLUETOOTH_CH9141_RTS_PIN_CH2) && time_count++ < BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)                  // 如果RTS为低电平，则继续发送数据
            system_delay_ms(1);
        if(time_count >= BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)
            return len;                                                                         // 模块忙,如果允许当前程序使用while等待 则可以使用后面注释的while等待语句替换本if语句
        uart_write_buffer(BLUETOOTH_CH9141_INDEX_CH2, buff, 30);

        buff += 30;                                                                             // 地址偏移
        len -= 30;                                                                              // 数量
    }

    time_count = 0;
    while(gpio_get_level(BLUETOOTH_CH9141_RTS_PIN_CH2) && time_count++ < BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)                      // 如果RTS为低电平，则继续发送数据
        system_delay_ms(1);
    if(time_count >= BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2)
        return len;                                                                             // 模块忙,如果允许当前程序使用while等待 则可以使用后面注释的while等待语句替换本if语句
    uart_write_buffer(BLUETOOTH_CH9141_INDEX_CH2, buff, len);                                              // 发送最后的数据

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      蓝牙转串口模块 读取函数
//  @param      buff        需要读取的数据地址
//  @param      len         发送长度
//  @return     uint32      读取的数据长度 如果没有这么多数据这里会被修改
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





