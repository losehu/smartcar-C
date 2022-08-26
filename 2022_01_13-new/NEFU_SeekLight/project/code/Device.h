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
#define LED0_PIN        E2      //zf核心板
#define LED1_PIN        E9      //zf核心板
#define LEDR_PIN        A0
#define LEDG_PIN        A1
#define LEDB_PIN        A2
//Beep
#define BEEP_PIN        C13     //zf
//Key
#define KEY_S1          A8
#define KEY_S2          D8
#define KEY_S3          B12
#define KEY_S4          B0

#define KEY_UP_PIN      KEY_S3////C10
#define KEY_DOWN_PIN    KEY_S4//D3
#define KEY_LEFT_PIN    C7//D0
#define KEY_RIGHT_PIN   C6//D1
#define KEY_CENTER_PIN  D1//D4
#define KEY_NEXT_PIN    KEY_S1//D7
#define KEY_LAST_PIN    KEY_S2//A15
//Code
#define CODE1_PIN       E3
#define CODE2_PIN       E1
#define CODE3_PIN       E0
#define CODE4_PIN       B7
//Encoder
#define ENCODER_L_TIM   TIM1_ENCOEDER        //zf
#define ENCODER_R_TIM   TIM9_ENCOEDER        //zf
#define ENCODER_L_CH    TIM1_CH2_ENCOEDER_E11//zf
#define ENCODER_L_DIR   TIM1_CH1_ENCOEDER_E9 //zf
#define ENCODER_R_CH    TIM9_CH2_ENCOEDER_D11//zf
#define ENCODER_R_DIR   TIM9_CH1_ENCOEDER_D9 //zf
//Motor
#define MOTOR_FREQ      17000
#define MR0_PWM         TIM4_PWM_CH2_D13//TIM4_PWM_CH1_D12        //zf
#define MR1_PWM         TIM4_PWM_CH1_D12//TIM4_PWM_CH2_D13        //zf
#define MR1_DIR         D12                     //zf
#define ML0_PWM         TIM4_PWM_CH3_D14//TIM4_PWM_CH4_D15        //zf
#define ML1_PWM         TIM4_PWM_CH4_D15//TIM4_PWM_CH3_D14        //zf
#define ML1_DIR         D15                     //zf
//Servo
#define SERVO_FREQ      50
#define SERVO_PWM       TIM2_PWM_CH1_A15        //zf
//Elc
#define ELCL0           ADC_IN12_C2     //zf
#define ELCL1           ADC_IN13_C3     //zf
#define ELCR0           ADC_IN14_C4     //zf
#define ELCR1           ADC_IN15_C5     //zf

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
#define SERVO_MID           8450    //50Hz:8000-8850-9700/100000
#define SERVO_ADDMAX        800

extern short ServoAdd;

void ServoWrite(void);
void ServoShow(void);




#endif /* DEVICE_H_ */
