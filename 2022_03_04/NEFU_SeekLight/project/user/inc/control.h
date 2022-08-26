/**
 //@FileName	:control.h
 //@CreatedDate	:2021年12月22日
 //@Author		:LiHao
 //@Description	:
**/

#ifndef CONTROL_H_
#define CONTROL_H_

#include "zf_common_headfile.h"


//速度给定

#define NormalSpeed                320   //320//280


typedef enum
{
    NOCONTROL = 0,
    ELCMODE = 1,
    CAMMODE = 2
} ControlModeEnum;

typedef enum
{
    NoLoop   = 0,
    SpeedLoop,
    DirectLoop,
    AllLoop,
    LR_SingleControl
}CloseLoopModeEnum;

typedef struct
{
    float UpTime;//s
    float StopTime;
    uint8 Protect_Flag;
    ControlModeEnum ControlMode;
    CloseLoopModeEnum ClosedLoopMode;
    uint8 CAR_PROTECT_MODE;
    uint8 ReadFlashParams;
    uint8 Camera_FPS;
} CarInfoTypedef;


//sattement
//extern uint8 HMI_START_KEY;
extern int8  InControlPeriod_Flag;
extern int32 point_center;
extern int32 point_center0;
extern int32 point_center1;
extern int32 point_center2;
extern int16 CarProtect_IntCnt;
//extern int16 AccAndRealSpeedProtect;
extern CarInfoTypedef   CarInfo;
extern int16 ExSpeed;
extern int16 circle_speed;

extern int16 ExSpeed_L;
extern int16 ExSpeed_R;
extern int16 SpeedOut_L;
extern int16 SpeedOut_R;
extern int16 DirectionOut;
extern int16 ExSpeed_SUM;
extern bool protect_cam;
extern int cam_die ;

//extern uint32_t RunCnt;
//fun
uint8 CarSystem_Init(void);
uint8 CenterCal(void);
void  ControlPeriodCheck(void);
void  AllImageDeal(void);
void  DirectionControl(void);
void  Timer6_IT(void);
void  Timer7_IT(void);
void  CarProtect(void);
uint8 CarInfoShow(void);

void ExSpeedControl(void);
void DiffSpeedControl(void);
void SpeedControl(void);
void Timer7_IT(void);
void judge_unit();


#endif /* CONTROL_H_ */
