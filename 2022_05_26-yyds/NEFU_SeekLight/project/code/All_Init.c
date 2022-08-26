/**
 //@FileName    :All_Init.c
 //@CreatedDate :2021年12月19日
 //@Author      :LiHao
 //@Description :
**/

#include "All_Init.h"

/**
*@Name			:All_Init
*@Description 	:All_Init
*@Param			:None
*@Return		:None
*@Sample		:All_Init();
**/
void All_Init(void)
{
    interrupt_disable_all();            // 关闭总中断
    clock_init(SYSTEM_CLOCK_144M);      // 务必保留，设置系统时钟。
   // debug_init();                       // 务必保留，本函数用于初始化MPU 时钟 调试串口
//     Ultrasound_Init(1);

    // 此处编写用户代码(例如：外设初始化代码等)

    ips114_init();
    ips114_clear(YELLOW);
//    ips114_show_string(0,0,(const char*)("icm20602_init :"));
//    while (!ICM20602_Offset());
//      while(icm20602_init());
//    ips114_show_string(200,0,(const char*)("OK"));

    ips114_show_string(0,1,(const char*)("mt9v03x_init :"));
  mt9v03x_init_dvp();
   // camer_init();
    ips114_show_string(200,1,(const char*)("OK"));

    ips114_show_string(0,2,(const char*)("params_init :"));
    ImageProcessingStatusInit();
   // ElcStatus_Init();
    PIDParams_Init();
    CarSystem_Init();
    DevicesParams_Init();
    init_flag();
    ips114_show_string(200,2,(const char*)("OK"));
    ips114_show_string(0,3,(const char*)("devices_init :"));
    Devices_Init();

    wireless_uart_init();
    ips114_show_string(200,3,(const char*)("OK"));


//  ips114_show_string(0,4,(const char*)("icm20602_init :"));
//    icm20602_init();//陀螺仪初始化需要5.5秒左右，请耐心等待...
//  ICM20602_Offset();
//  while(ICM20602_Offset_Finished == 0)
//  {
//      //
//
//  }
//  ips114_show_string(200,4,(const char*)("OK"));

//    ips114_show_string(0,5,(const char*)("read flash params:"));
//    FlashParamsRead();
//    ips114_show_string(200,5,(const char*)("OK"));
//    FlashParamsWrite();
    interrupt_enable_all(0);            // 总中断最后开启
    my_delay(50);
    GetSimBinImage();
      ImagePerspective_Init();
    while(!mt9v03x_finish_flag_dvp);
    if(mt9v03x_finish_flag_dvp == 1){
     //   AllImageDeal();
       // camer_get();
        mt9v03x_finish_flag_dvp = 0;
    }
    Base.element_check = 'T';//元素判断 'T':开 'F':关
    point_center = 0;
    ips114_clear(YELLOW);
    ips114_pencolor = RED;
    ips114_bgcolor = YELLOW;
//    //按键S1发车
//    ips114_show_string(0,0,(const uint8*)("Press Button S1 To Start Car!"));
ips114_clear(BLUE);
    //    gpio_high(BEEP_PIN);
//    my_delay(300);
//    gpio_low(BEEP_PIN);

    //    while(gpio_get());
    ips114_clear(BLACK);
    pit_init_ms(TIM6_PIT, 1);
  //  pit_init_ms(TIM7_PIT, 1);
   // interrupt_set_priority(TIM7_IRQn, 0x0C);//1级抢占优先级，4级从优先级//默认0,3
}void send_image_init()
{
    ips114_clear(BLACK);
//    printf("AT+UART=115200,8,1,0,0\r\n");
//    my_delay(2000);
//ips114_show_string(0, 0, "OK");
//    printf("AT+RST\r\n");
    my_delay(1500);

            printf("ATE0\r\n");
            my_delay(800);
            ips114_show_string(0, 0, "OK");

            printf("AT+CWMODE=1\r\n");
            my_delay(800);
            ips114_show_string(0, 1, "OK");

//            printf("AT+CWJAP=\"RUBO\",\"12345678\"\r\n");
//                       my_delay(2000);
            printf("AT+CIPSTART=\"UDP\",\"192.168.137.1\",8080\r\n");
            my_delay(800);
            ips114_show_string(0, 2, "OK");

            printf("AT+CIPMODE=1\r\n");
            my_delay(800);
            ips114_show_string(0, 3, "OK");

            printf("AT+CIPSEND\r\n");
            my_delay(800);
            ips114_show_string(0, 4, "OK");
ips114_clear(BLACK);
//            gpio_high(BEEP_PIN);
//            my_delay(300);
//            gpio_low(BEEP_PIN);
}
