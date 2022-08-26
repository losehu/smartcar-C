/**
 //@FileName	:Device.c
 //@CreatedDate	:2021年12月19日
 //@Author		:LiHao
 //@Description	:各种外设和设备初始化和测试
**/

#include "Device.h"


/**
*@Name			:Devices_Init
*@Description 	:外设接口初始化
*@Param			:None
*@Return		:None
*@Sample        :Devices_Init();
**/
void Devices_Init(void)
{
    //LED
    gpio_init(LED0_PIN, GPO, GPIO_LOW, GPIO_PIN_CONFIG);

    //Beep
  //  gpio_init(BEEP_PIN, GPO, GPIO_LOW, GPIO_PIN_CONFIG);

    //Key
    gpio_init(KEY_UP_PIN,     GPI, GPIO_HIGH, GPIO_INT_CONFIG);
    gpio_init(KEY_DOWN_PIN,   GPI, GPIO_HIGH, GPIO_INT_CONFIG);
    gpio_init(KEY_LEFT_PIN,   GPI, GPIO_HIGH, GPIO_INT_CONFIG);
    gpio_init(KEY_RIGHT_PIN,  GPI, GPIO_HIGH, GPIO_INT_CONFIG);
    gpio_init(KEY_CENTER_PIN, GPI, GPIO_HIGH, GPIO_INT_CONFIG);
    gpio_init(KEY_NEXT_PIN,   GPI, GPIO_HIGH, GPIO_INT_CONFIG);
    gpio_init(KEY_LAST_PIN,   GPI, GPIO_HIGH, GPIO_INT_CONFIG);

    //Code
//    gpio_init(CODE1_PIN, GPI, GPIO_HIGH, GPIO_INT_CONFIG);
//    gpio_init(CODE2_PIN, GPI, GPIO_HIGH, GPIO_INT_CONFIG);
//    gpio_init(CODE3_PIN, GPI, GPIO_HIGH, GPIO_INT_CONFIG);
//    gpio_init(CODE4_PIN, GPI, GPIO_HIGH, GPIO_INT_CONFIG);

    //Encoder
    encoder_init_dir(ENCODER_L_TIM, ENCODER_L_DIR, ENCODER_L_CH);
    encoder_init_dir(ENCODER_R_TIM, ENCODER_R_DIR, ENCODER_R_CH);

    //Motor
        #if MOTOR_TYPE
        gpio_init(MR1_DIR, GPO, GPIO_LOW, GPIO_PIN_CONFIG);
        pwm_init(MR0_PWM, MOTOR_FREQ, 0);
        gpio_init(ML1_DIR, GPO, GPIO_LOW, GPIO_PIN_CONFIG);
        pwm_init(ML0_PWM, MOTOR_FREQ, 0);
        #else
        pwm_init(MR0_PWM, MOTOR_FREQ, 0);
        pwm_init(MR1_PWM, MOTOR_FREQ, 0);
        pwm_init(ML0_PWM, MOTOR_FREQ, 0);
        pwm_init(ML1_PWM, MOTOR_FREQ, 0);
        #endif





    //Servo
    pwm_init(SERVO_PWM, SERVO_FREQ, SERVO_MID);

    //Elc
    adc_init(ELCL0);
    adc_init(ELCL1);
    adc_init(ELCR0);
    adc_init(ELCR1);

}

//==============================================================//
//=========================外设服务函数==========================//
//==============================================================//

//=============================LED==============================//
/**
*@Name			:LEDTest
*@Description 	:LEDTest
*@Param			:None
*@Return		:None
*@Sample		:LED_Test();
**/
void LEDTest(void)
{
    gpio_high(LED0_PIN);
    gpio_low(LED1_PIN);
    my_delay(200);
    gpio_high(LED1_PIN);
    gpio_low(LED0_PIN);
    my_delay(200);
}

//============================BEEP==============================//
BeepTypedef Beep;
 MY_BEEP STATE_BEEP;
void BEEP_RING(int time)
{
STATE_BEEP.flag=1;
STATE_BEEP.ms=time;
}
/**
*@Name			:BeepTest
*@Description 	:BeepTest
*@Param			:None
*@Return		:None
*@Sample		:Beep_Test();
**/
void BeepTest(void)
{
    gpio_high(BEEP_PIN);
    my_delay(200);
    gpio_low(BEEP_PIN);
    my_delay(200);
}

/**
*@Name			:BeepTick
*@Description 	:BeepTick,中断里计时
*@Param			:n：响几次；time：间隔
*@Return		:None
*@Sample		:BeepTick(2, 100);
**/
void BeepTick(uint8_t n, uint16_t time)
{
    if(Beep.State == 'F')
    {
        Beep.State = 'T';
        Beep.Time  = time;
        Beep.Num   = n;
        if(Beep.Num == 0) Beep.Num = 1;
        Beep.Cnt = 1;
        gpio_low(BEEP_PIN);
    }
}

/**
*@Name			:Beep_IntCnt
*@Description 	:Beep_IntCnt蜂鸣器中断
*@Param			:
*@Return		:
*@Sample		:
**/
void Beep_IntCnt(void)
{
    if(STATE_BEEP.flag == 1||STATE_BEEP.ms!=0)
    {
        if(STATE_BEEP.flag == 1)
        {
        STATE_BEEP.flag=0;
        gpio_high(BEEP_PIN);
        }
        STATE_BEEP.ms--;
        if(STATE_BEEP.ms==0)        gpio_low(BEEP_PIN);


    }
}

/**
*@Name			:BeepParams_Init
*@Description 	:BeepParams_Init
*@Param			:
*@Return		:
*@Sample		:
**/
void BeepParams_Init(void)
{
    Beep.State = 'F';
    Beep.Cnt = 0;
    Beep.Num = 0;
    Beep.Time = 0;
    STATE_BEEP.flag=0;
    STATE_BEEP.ms=0;
}
//=============================KEY==============================//

KeyStateEnum KeyUp, KeyDown, KeyLeft, KeyRight, KeyCenter, KeyNext, KeyLast;
/**
*@Name			:KeyScan
*@Description 	:KeyScan//在中断里扫描
*@Param			:None
*@Return		:None
*@Sample		:KeyScan();
**/
void KeyScan(void)
{
    static uint8 KeyPressNum = 0;
    static uint16 KeyPressTime = 0;
    static uint8 SomeKeyPress_Flag = 0;//0 松开 1 有按下  2:消抖后 3:长按
    #define AlwaysPressTime 1600//一直按检测时间
    #define debouncing  5
    if(SomeKeyPress_Flag == 0 && (KEY_UP == 0 || KEY_DOWN == 0 || KEY_CENTER == 0 ||\
        KEY_LEFT == 0 || KEY_RIGHT == 0 || KEY_NEXT == 0 || KEY_LAST == 0))
    {
         SomeKeyPress_Flag = 1;
    }
    if(SomeKeyPress_Flag > 0)
    {
        KeyPressTime ++;
        //计时5ms消抖
        if(SomeKeyPress_Flag == 1 && KeyPressTime >= debouncing)
        {
            SomeKeyPress_Flag = 2;
            if(KEY_UP == 0)
                KeyPressNum = 1;//检测按键按下
            else if(KEY_DOWN == 0)
                KeyPressNum = 2;
            else if(KEY_LEFT == 0)
                KeyPressNum = 3;
            else if(KEY_RIGHT == 0)
                KeyPressNum = 4;
            else if(KEY_CENTER == 0)
                KeyPressNum = 5;
            else if(KEY_NEXT == 0)
                KeyPressNum = 6;
            else if(KEY_LAST == 0)
                KeyPressNum = 7;
        }
        //按一下就松开的状态
        if((KEY_UP == 1 && KEY_DOWN == 1 && KEY_LEFT == 1 && KEY_RIGHT == 1  && KEY_NEXT == 1 && \
                KEY_LAST == 1 && KEY_CENTER == 1) && KeyPressTime < AlwaysPressTime && SomeKeyPress_Flag == 2)
        {
            SomeKeyPress_Flag = 0;//按键松开了
            if(KeyPressNum == 1)
                KeyUp = onepress;
            else if(KeyPressNum == 2)
                KeyDown = onepress;
            else if(KeyPressNum == 3)
                KeyLeft = onepress;
            else if(KeyPressNum == 4)
                KeyRight = onepress;
            else if(KeyPressNum == 5)
                KeyCenter = onepress;
            else if(KeyPressNum == 6)
                KeyNext = onepress;
            else if(KeyPressNum == 7)
                KeyLast = onepress;
            KeyPressTime = 0;
            SomeKeyPress_Flag = 0;
            BeepTick(1,50);
        }
        //长按不松开的状态
        if(KeyPressTime >= AlwaysPressTime && SomeKeyPress_Flag == 2)
        {
            if(KeyPressNum == 1)      KeyUp = holdpress;
            else if(KeyPressNum == 2) KeyDown = holdpress;
            else if(KeyPressNum == 3) KeyLeft = holdpress;
            else if(KeyPressNum == 4) KeyRight = holdpress;
            else if(KeyPressNum == 5) KeyCenter = holdpress;
            else if(KeyPressNum == 6) KeyNext = holdpress;
            else if(KeyPressNum == 7) KeyLast = holdpress;
            BeepTick(2,50);
            if(KEY_UP == 1 && KEY_DOWN == 1 && KEY_LEFT == 1 && KEY_RIGHT == 1 && \
                KEY_CENTER == 1 && KEY_NEXT == 1 && KEY_LAST == 1)
            {
                SomeKeyPress_Flag = 0;
                KeyPressTime = 0;
                KeyUp = nopress;
                KeyDown = nopress;
                KeyLeft = nopress;
                KeyRight = nopress;
                KeyNext = nopress;
                KeyLast = nopress;
                KeyCenter = nopress;
            }
        }
    }
}

/**
*@Name			:KeyParams_Init
*@Description 	:KeyParams_Init
*@Param			:None
*@Return		:None
*@Sample		:KeyParams_Init();
**/
void KeyParams_Init(void)
{
    KeyUp = nopress;
    KeyDown = nopress;
    KeyCenter = nopress;
    KeyLeft = nopress;
    KeyRight = nopress;
    KeyNext = nopress;
    KeyLast = nopress;
}
//============================CODE==============================//
/**
*@Name			:CodeScan
*@Description 	:CodeScan拨码开关
*@Param			:None
*@Return		:None
*@Sample		:CodeScan();
**/
void CodeScan(void)
{

}

//============================ENCODER==============================//
WheelTypedef   Wheel, Wheel_L, Wheel_R;
int32_t Encoder_L_Cnt = 0;
int32_t Encoder_R_Cnt = 0;
int32_t EncoderAll_Cnt = 0;
/**
*@Name			:EncoderRead
*@Description 	:EncoderRead
*@Param			:None
*@Return		:None
*@Sample		:EncoderRead();
**/
void EncoderRead(void)
{
    //获取并清空计数值
    Encoder_L_Cnt = - encoder_get_count(ENCODER_L_TIM);
    Encoder_R_Cnt = encoder_get_count(ENCODER_R_TIM);
    EncoderAll_Cnt += (Encoder_L_Cnt + Encoder_R_Cnt)/2;
    encoder_clean_count(ENCODER_L_TIM);
    encoder_clean_count(ENCODER_R_TIM);
    //计算速度
    Wheel_L.SpeedNow = (float)(Encoder_L_Cnt * RealSpeedK);
    Wheel_R.SpeedNow = (float)(Encoder_R_Cnt * RealSpeedK);
    //计算加速度
    Wheel_L.Acc = Wheel_L.SpeedNow - Wheel_L.SpeedLast;
    Wheel_R.Acc = Wheel_R.SpeedNow - Wheel_R.SpeedLast;
    //速度滤波
    if (myabs(Wheel_L.Acc) > 80)
        Wheel_L.SpeedNow = Wheel_L.SpeedNow * 0.5 + Wheel_L.SpeedLast * 0.5;
    else
        Wheel_L.SpeedNow = Wheel_L.SpeedNow * 0.9 + Wheel_L.SpeedLast * 0.1;
    if (myabs(Wheel_R.Acc) > 80)
        Wheel_R.SpeedNow = Wheel_R.SpeedNow * 0.5 + Wheel_R.SpeedLast * 0.5;
    else
        Wheel_R.SpeedNow = Wheel_R.SpeedNow * 0.9 + Wheel_R.SpeedLast * 0.1;
    //速度限幅
//    Wheel_L.SpeedNow = limit(Wheel_L.SpeedNow, 500);
//    Wheel_R.SpeedNow = limit(Wheel_R.SpeedNow, 500);
    //速度更新
    Wheel_L.SpeedLast = Wheel_L.SpeedNow;
    Wheel_R.SpeedLast = Wheel_R.SpeedNow;
    //距离计算
    Wheel.Distance += (float)((Encoder_L_Cnt + Encoder_R_Cnt)/2/115/100);
    //总体速度计算
    Wheel.SpeedNow = (Wheel_L.SpeedNow + Wheel_R.SpeedNow)/2;
    //总体速度更新
    Wheel.SpeedLast = Wheel.SpeedNow;
}
/**
*@Name			:EncoderShow
*@Description 	:EncoderShow
*@Param			:None
*@Return		:1
*@Sample		:EncoderShow();
**/
uint8 EncoderShow(void)
{
    while(KeyLast != onepress)
    {
        EncoderRead();
        ips114_pencolor = RED;
        ips114_show_string(0,0,(const char*)("LEncoder:"));
        ips114_show_string(0,1,(const char*)("LSpeedNow:"));
        ips114_show_string(0,2,(const char*)("REncoder:"));
        ips114_show_string(0,3,(const char*)("RSpeedNow:"));
        ips114_show_string(0,4,(const char*)("SpeedNow:"));
        ips114_show_string(0,5,(const char*)("Distance:"));
        ips114_show_string(0,6,(const char*)("AllCnt:"));
        ips114_pencolor = BLUE;
        ips114_show_int16(120,0,Encoder_L_Cnt);
        ips114_show_int16(120,1,Wheel_L.SpeedNow);
        ips114_show_int16(120,2,Encoder_R_Cnt);
        ips114_show_int16(120,3,Wheel_R.SpeedNow);
        ips114_show_int16(120,4,Wheel.SpeedNow);
        ips114_show_float(120,5,Wheel.Distance,4,2);
        ips114_show_int32(120,6,EncoderAll_Cnt,5);
        ips114_pencolor = RED;
        my_delay(10);
    }
    KeyLast = nopress;
    return 1;
}

/**
*@Name			:EncoderParams_Init
*@Description 	:EncoderParams_Init
*@Param			:
*@Return		:
*@Sample		:EncoderParams_Init();
**/
void EncoderParams_Init(void)
{
    Wheel.SpeedNow = 0;
    Wheel.SpeedLast = 0;
    Wheel.Distance = 0;
    Wheel.Acc = 0;

    Wheel_L.SpeedNow = 0;
    Wheel_L.SpeedLast = 0;
    Wheel_L.Distance = 0;
    Wheel_L.Acc = 0;

    Wheel_R.SpeedNow = 0;
    Wheel_R.SpeedLast = 0;
    Wheel_R.Distance = 0;
    Wheel_R.Acc = 0;
}
//=============================MOTOR==============================//
short pwm_l = 0;
short pwm_r = 0;
/**
*@Name			:MotorWrite
*@Description 	:MotorWrite
*@Param			:None
*@Return		:None
*@Sample		:MotorWrite();
**/
int last_speed=0;
void MotorWrite(void)
{
    if(stop_flag==1&&(Wheel_L.SpeedNow<=0||Wheel_R.SpeedNow<=0||last_speed<Wheel_L.SpeedNow+Wheel_R.SpeedNow))pwm_l=0,pwm_r=0;
    last_speed=Wheel_L.SpeedNow+Wheel_R.SpeedNow;

    pwm_l = limit(pwm_l, 1000);
    pwm_r = limit(pwm_r, 1000);

    #if MOTOR_TYPE
    pwm_set_duty(ML0_PWM, myabs(pwm_l));
    pwm_set_duty(MR0_PWM, myabs(pwm_r));

    if (pwm_l > 0)
        gpio_set_level(ML1_DIR, 1);
    else
        gpio_set_level(ML1_DIR, 0);
    if (pwm_r > 0)
        gpio_set_level(MR1_DIR, 0);
    else
        gpio_set_level(MR1_DIR, 1);

    #elif  !MOTOR_TYPE
    if (pwm_l < 0) {
        pwm_set_duty(ML1_PWM, -pwm_l);
        pwm_set_duty(ML0_PWM, 0);
    }
    else {
        pwm_set_duty(ML1_PWM, 0);
        pwm_set_duty(ML0_PWM, pwm_l);
    }
    if (pwm_r < 0) {
        pwm_set_duty(MR1_PWM,-pwm_r);
        pwm_set_duty(MR0_PWM, 0);
    }
    else {
        pwm_set_duty(MR1_PWM, 0);
        pwm_set_duty(MR0_PWM, pwm_r);
    }
    #endif
}


#define MOTOR_MENU_NUM  1
/**
*@Name			:MotorShow
*@Description 	:MotorShow
*@Param			:None
*@Return		:None
*@Sample		:MotorShow();
**/
void MotorShow(void)
{
    CarInfo.ClosedLoopMode = NoLoop;
//    CarInfo.Protect_flag = 'F';
//    CarInfo.CAR_PROTECT_MODE = 'F';
    int16 pwm_l0 = 0,pwm_r0 = 0;
    while(KeyLast != onepress)
    {
        static uint8 NowPWMItem = 0;
        MenuChoose(&NowPWMItem,MOTOR_MENU_NUM);
        //显示条目
        ips114_show_string(16,0,"PWM_L");
        ips114_show_string(16,1,"PWM_R");
        if(KeyNext == onepress)
        {
            KeyNext = nopress;
            switch(NowPWMItem)
            {
                case(0):
                {
                    ParamsAdjustShort((short*)(&pwm_l0),120,0,10);
                    break;
                }
                case(1):
                {
                    ParamsAdjustShort((short*)(&pwm_r0),120,1,10);
                    break;
                }
            }
            KeyNext = nopress;
        }
        ips114_pencolor = BLUE;
        ips114_show_int16(120,0,pwm_l0);
        ips114_show_int16(120,1,pwm_r0);
        ips114_pencolor = RED;
        pwm_l = pwm_l0;
        pwm_r = pwm_r0;
        MotorWrite();
        my_delay(10);
    }
//    CarInfo.ClosedLoopMode = AllLoop;
//  CarInfo.Protect_flag = 'F';
//    CarInfo.CAR_PROTECT_MODE = 'T';
    KeyLast = nopress;
}
//=============================SERVO==============================//
short ServoAdd = 0;
/**
*@Name			:ServoWrite
*@Description 	:ServoWrite
*@Param			:None
*@Return		:None
*@Sample		:ServoWrite();
**/
void ServoWrite(void)
{
    ServoAdd = limit(ServoAdd,SERVO_ADDMAX);
    pwm_set_duty(SERVO_PWM, SERVO_MID + ServoAdd);
}
/**
*@Name			:ServoShow
*@Description 	:ServoShow
*@Param			:None
*@Return		:None
*@Sample		:ServoShow();
**/
void ServoShow(void)
{
//    CarInfo.Protect_flag = 'T';
    int16 ServoAdd0 = 0;
    while(KeyLast != onepress)
    {
        //static int8 NowServoItem = 0;

        ips114_show_string(0,0,(const char*)("SevroAdd:"));
        if(KeyNext == onepress)
        {
            KeyNext = nopress;
            ParamsAdjustShort((short*)(&ServoAdd0),120,0,10);
        }
        KeyNext = nopress;
        ips114_pencolor = BLUE;
        ips114_show_int16(120,0,ServoAdd0);
        ServoAdd = ServoAdd0;
        ServoWrite();
        my_delay(10);
    }
//    CarInfo.Protect_flag = 'F';
}

/**
*@Name          :DevicesParams_Init
*@Description   :DevicesParams_Init 外设参数初始化
*@Param         :
*@Return        :
*@Sample        :
**/
void DevicesParams_Init(void)
{
    BeepParams_Init();
    KeyParams_Init();
    EncoderParams_Init();
}



