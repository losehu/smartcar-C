/**
 //@FileName    :Device.h
 //@CreatedDate :2021年12月19日
 //@Author      :LiHao
 //@Description :各种外设和设备初始化和测试
**/

#ifndef DEVICE_H_
#define DEVICE_H_

#include "zf_common_headfile.h"


//=============================引脚及属性定义==============================//
void Devices_Init(void);
void DevicesParams_Init(void);



//LED
//LED
#define LED0_PIN        B12      //zf核心板
#define LED1_PIN        E9      //zf核心板
#define LEDR_PIN        E0
#define LEDG_PIN        E1
#define LEDB_PIN        E2
//Beep
#define BEEP_PIN        D0
//Key
#define KEY_UP_PIN      B14
#define KEY_DOWN_PIN    D14
#define KEY_LEFT_PIN    A8
#define KEY_RIGHT_PIN   D13
#define KEY_CENTER_PIN  A15
#define KEY_NEXT_PIN    D15
#define KEY_LAST_PIN    C10
//Code
#define CODE1_PIN       E6
#define CODE2_PIN       E5
#define CODE3_PIN       E4
#define CODE4_PIN       E3
//Encoder
#define ENCODER_L_TIM   TIM8_ENCOEDER
#define ENCODER_R_TIM   TIM2_ENCOEDER
#define ENCODER_L_CH    TIM8_CH2_ENCOEDER_C7
#define ENCODER_L_DIR   TIM8_CH1_ENCOEDER_C6
#define ENCODER_R_CH    TIM2_CH2_ENCOEDER_A1
#define ENCODER_R_DIR   TIM2_CH1_ENCOEDER_A0
//Motor
#define MOTOR_FREQ      17000
#define MR0_PWM         TIM10_PWM_CH2_D3
#define MR1_PWM         TIM10_PWM_CH1_D1
#define MR1_DIR         D1                     //zf
#define ML0_PWM          TIM10_PWM_CH4_D7
#define ML1_PWM          TIM10_PWM_CH3_D5
#define ML1_DIR             D5                 //zf
//Servo
#define SERVO_FREQ      50
#define SERVO_PWM       TIM1_PWM_CH2_E11
//Elc
#define ELCL0           ADC_IN14_C4
#define ELCL1           ADC_IN15_C5
#define ELCR0           ADC_IN8_B0
#define ELCR1           ADC_IN9_B1
#define STC_RECV_FLAG A3// E14//E15 A3
#define STC_FIR_FLAG E14// E14//E15 A3
#define STC_SEC_FLAG E15// E14//E15 A3
extern bool stc_recv;
extern bool stc_fir;
extern bool stc_sec;


//=============================变量函数功能定义==============================//

//LED
//void LEDTest(void);

//BEEP
typedef struct
{
    uint16 State;
    uint16 Num;
    uint16 Time;
    uint32 Cnt;
}BeepTypedef;

extern BeepTypedef Beep;

//void BeepTest(void);
void BeepTick(uint8_t n, uint16_t time);
void Beep_IntCnt(void);
//RUBO BEEP
typedef struct
{
    bool flag;
    int ms;
}MY_BEEP;
extern MY_BEEP STATE_BEEP;
//KEY
typedef enum
{
    nopress = 0,
    onepress,//短按一下
    holdpress,//一直按着
} KeyStateEnum;

extern KeyStateEnum KeyUp, KeyDown, KeyLeft, KeyRight, KeyCenter, KeyNext, KeyLast;
#define KEY_UP      gpio_get_level(KEY_UP_PIN)
#define KEY_DOWN    gpio_get_level(KEY_DOWN_PIN)
#define KEY_LEFT    gpio_get_level(KEY_LEFT_PIN)
#define KEY_RIGHT   gpio_get_level(KEY_RIGHT_PIN)
#define KEY_CENTER  gpio_get_level(KEY_CENTER_PIN)
#define KEY_NEXT    gpio_get_level(KEY_NEXT_PIN)
#define KEY_LAST    gpio_get_level(KEY_LAST_PIN)

void KeyScan(void);

//CODE
//void CodeScan(void);

//ENCODER
#define PAI                 3.14159265358979f
#define RealSpeedK          0.8696f     // 100/115(1024,10ms)

typedef struct
{
    float SpeedNow;
    float SpeedLast;
    float Acc;
    float Distance;
}WheelTypedef;

extern WheelTypedef   Wheel, Wheel_L, Wheel_R;

void EncoderRead(void);
uint8 EncoderShow(void);

//MOTOR
#define MOTOR_TYPE      1//1是一路IO控制方向一路PWM控制转速；0是两路PWM控制

extern short pwm_l;
extern short pwm_r;

void MotorWrite(void);
void MotorShow(void);

//SERVO
#define SERVO_MID           8080    //50Hz:8000-8850-9700/100000
#define SERVO_ADDMAX        800

extern short ServoAdd;
void BEEP_RING(int time);

void ServoWrite(void);
void ServoShow(void);




#endif /* DEVICE_H_ */
