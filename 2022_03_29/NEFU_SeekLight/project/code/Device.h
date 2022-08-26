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
////LED
//#define LED0_PIN        E2      //zf核心板
//#define LED1_PIN        E9      //zf核心板
//#define LEDR_PIN        E8
//#define LEDG_PIN        E7
//#define LEDB_PIN        E10
////Beep
//#define BEEP_PIN        /*D15*/C13     //zf
////Key
//#define KEY_S1          A8
//#define KEY_S2          D8
//#define KEY_S3          B12
//#define KEY_S4          B0
//
//#define KEY_UP_PIN      KEY_S3//*/E13
//#define KEY_DOWN_PIN    KEY_S4//*/B14
//#define KEY_LEFT_PIN    /*D10//*/A1
//#define KEY_RIGHT_PIN   /*E12//*/A2
//#define KEY_CENTER_PIN  /*D12//*/A3
//#define KEY_NEXT_PIN    KEY_S1//*/B12
//#define KEY_LAST_PIN    KEY_S2//*/E14
////Code
//#define CODE1_PIN       B2
//#define CODE2_PIN       A7
//#define CODE3_PIN       A3
//#define CODE4_PIN       A2
////Encoder
//#define ENCODER_L_TIM   /*TIM2_ENCOEDER*/TIM1_ENCOEDER        //zf
//#define ENCODER_R_TIM   /*TIM1_ENCOEDER*/TIM9_ENCOEDER        //zf
//#define ENCODER_L_CH    /*TIM2_CH2_ENCOEDER_A1*/TIM1_CH2_ENCOEDER_E11//zf
//#define ENCODER_L_DIR   /*TIM2_CH1_ENCOEDER_A0*/TIM1_CH1_ENCOEDER_E9 //zf
//#define ENCODER_R_CH    /*TIM1_CH2_ENCOEDER_E11*/TIM9_CH2_ENCOEDER_D11//zf
//#define ENCODER_R_DIR   /*TIM1_CH1_ENCOEDER_E9*/TIM9_CH1_ENCOEDER_D9 //zf
////Motor
//#define MOTOR_FREQ      17000
//#define MR0_PWM         /*TIM10_PWM_CH3_D5*/TIM4_PWM_CH2_D13        //zf
//#define MR1_PWM         /*TIM10_PWM_CH4_D7*/TIM4_PWM_CH1_D12        //zf
//#define MR1_DIR         D12                     //zf
//#define ML0_PWM         /*TIM10_PWM_CH2_D3*/TIM4_PWM_CH3_D14        //zf
//#define ML1_PWM         /*TIM10_PWM_CH1_D1*/TIM4_PWM_CH4_D15        //zf
//#define ML1_DIR         D15                     //zf
////Servo
//#define SERVO_FREQ      50
//#define SERVO_PWM       /*TIM3_PWM_CH2_C7*/TIM2_PWM_CH1_A15        //zf
////Elc
//#define ELCL0           /*ADC_IN14_C4*/  ADC_IN9_B1    //zf
//#define ELCL1           /*ADC_IN15_C5*/ADC_IN7_A7      //zf
//#define ELCR0           /*ADC_IN8_B0*/ADC_IN10_C0      //zf
//#define ELCR1           /*ADC_IN9_B1*/ADC_IN11_C1     //zf

//LED
#define LED0_PIN        E2      //zf核心板
#define LED1_PIN        E9      //zf核心板
#define LEDR_PIN        E8
#define LEDG_PIN        E7
#define LEDB_PIN        E10
//Beep
#define BEEP_PIN        D15
//Key
//#define KEY_S1          A8
//#define KEY_S2          D8
//#define KEY_S3          B12
//#define KEY_S4          B0

#define KEY_UP_PIN      D10
#define KEY_DOWN_PIN    D8
#define KEY_LEFT_PIN    B12
#define KEY_RIGHT_PIN   D9
#define KEY_CENTER_PIN  E13
#define KEY_NEXT_PIN    B14
#define KEY_LAST_PIN    E12
//Code
#define CODE1_PIN       B2
#define CODE2_PIN       A7
#define CODE3_PIN       A3
#define CODE4_PIN       A2
//Encoder
#define ENCODER_L_TIM   TIM2_ENCOEDER
#define ENCODER_R_TIM   TIM1_ENCOEDER
#define ENCODER_L_CH    TIM2_CH2_ENCOEDER_A1
#define ENCODER_L_DIR   TIM2_CH1_ENCOEDER_A0
#define ENCODER_R_CH    TIM1_CH2_ENCOEDER_E11
#define ENCODER_R_DIR   TIM1_CH1_ENCOEDER_E9
//Motor
#define MOTOR_FREQ      17000
#define MR0_PWM         TIM10_PWM_CH2_D3
#define MR1_PWM         TIM10_PWM_CH3_D5
#define MR1_DIR         D1                    //zf
#define ML0_PWM         TIM10_PWM_CH4_D7
#define ML1_PWM         TIM10_PWM_CH1_D1
#define ML1_DIR         D5                     //zf
//Servo
#define SERVO_FREQ      50
#define SERVO_PWM       TIM3_PWM_CH2_C7
//Elc
#define ELCL0           ADC_IN14_C4
#define ELCL1           ADC_IN15_C5
#define ELCR0           ADC_IN8_B0
#define ELCR1           ADC_IN9_B1


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
#define SERVO_MID           8650    //50Hz:8000-8850-9700/100000
#define SERVO_ADDMAX        800

extern short ServoAdd;
void BEEP_RING(int time);

void ServoWrite(void);
void ServoShow(void);




#endif /* DEVICE_H_ */
