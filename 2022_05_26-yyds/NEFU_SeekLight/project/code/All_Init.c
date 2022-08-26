/**
 //@FileName    :All_Init.c
 //@CreatedDate :2021��12��19��
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
    interrupt_disable_all();            // �ر����ж�
    clock_init(SYSTEM_CLOCK_144M);      // ��ر���������ϵͳʱ�ӡ�
   // debug_init();                       // ��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
//     Ultrasound_Init(1);

    // �˴���д�û�����(���磺�����ʼ�������)

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
//    icm20602_init();//�����ǳ�ʼ����Ҫ5.5�����ң������ĵȴ�...
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
    interrupt_enable_all(0);            // ���ж������
    my_delay(50);
    GetSimBinImage();
      ImagePerspective_Init();
    while(!mt9v03x_finish_flag_dvp);
    if(mt9v03x_finish_flag_dvp == 1){
     //   AllImageDeal();
       // camer_get();
        mt9v03x_finish_flag_dvp = 0;
    }
    Base.element_check = 'T';//Ԫ���ж� 'T':�� 'F':��
    point_center = 0;
    ips114_clear(YELLOW);
    ips114_pencolor = RED;
    ips114_bgcolor = YELLOW;
//    //����S1����
//    ips114_show_string(0,0,(const uint8*)("Press Button S1 To Start Car!"));
ips114_clear(BLUE);
    //    gpio_high(BEEP_PIN);
//    my_delay(300);
//    gpio_low(BEEP_PIN);

    //    while(gpio_get());
    ips114_clear(BLACK);
    pit_init_ms(TIM6_PIT, 1);
  //  pit_init_ms(TIM7_PIT, 1);
   // interrupt_set_priority(TIM7_IRQn, 0x0C);//1����ռ���ȼ���4�������ȼ�//Ĭ��0,3
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
