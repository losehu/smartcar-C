/**
 //@FileName	:control.c
 //@CreatedDate	:2021��12��22��
 //@Author		:LiHao
 //@Description	:
**/

#include "control.h"
#include <math.h>

CarInfoTypedef   CarInfo;
//��ʱ���ж� 1ms
static int16 Timer_IntCnt = 0;
int16 SpeedOut_L =  0;
int16 SpeedOut_R =  0;
int16 DirectionOut = 0;
int16 ExSpeed = NormalSpeed;
int16 ExSpeed_L = 0;
int16 ExSpeed_R = 0;
int16 L_SingleControlOut = 0,R_SingleControlOut = 0;
int8  InControlPeriod_Flag = 'F';

/**
*@Name          :Timer6_IT
*@Description   :Timer6_IT������ʱ��6�жϷ����������ڵ�����Ƽ������жϷ���
*@Param         :None
*@Return        :NULL
*@Sample        :Timer6_IT();
**/
void Timer6_IT(void)
{
    Timer_IntCnt ++;
    CarInfo.UpTime += 0.001f;
    Beep_IntCnt();
    KeyScan();
    //��������ֵ��ȡ
//  Data_Filter();
//  if(TIMx_Cnt % 5 == 0 )
//  {
//      //�ǶȻ���
//      ICM_I_Int();
//      //���½Ƕ�
//      IMUupdate(ICM_Treated.gyro.x*0.01745, ICM_Treated.gyro.y*0.01745, ICM_Treated.gyro.z*0.01745, ��
//              ICM_Treated.accdata.x , ICM_Treated.accdata.y, ICM_Treated.accdata.z);
//  }
    //-------------------------------�������------------------------------------//

    DirectionControl();

    //-------------------------------�ٶȿ���------------------------------------//

    if(Timer_IntCnt % 10 == 0 ){
        ExSpeed_L = ExSpeed;
        ExSpeed_R = ExSpeed;
        ExSpeedControl();//˳���ܴ�
        DiffSpeedControl();
        SpeedControl();
    }

    //���η��ͱ�־
    if(Timer_IntCnt % 20 == 0 )
    {
        Waveform_Flag = 'T';
    }
    //�������
    if(Timer_IntCnt >= 1000)
    {
        gpio_toggle_level(LED0_PIN);
        Timer_IntCnt = 0;
        CarProtect_IntCnt = 0;
    }
}
//��ʼ��һЩ����
uint8 CarSystem_Init(void)
{
    //���ñ�־
    CarInfo.Protect_Flag = 'F';
    CarInfo.ControlMode = CAMMODE;
    CarInfo.ClosedLoopMode = AllLoop;
    CarInfo.CAR_PROTECT_MODE = 'F';//��������ź���������
    CarInfo.StopTime = 0.000f;
    CarInfo.UpTime = 0.000f;
    CarInfo.ReadFlashParams = 'F';
    CarInfo.Camera_FPS = 150;
    //ret
    return 1;
}


//���߼���
int32 point_center;
int32 point_center0;
int32 point_center1;
int32 point_center2;
uint8 CenterCal(void)
{
    //���㵱ǰ�е�ƫ����
    float excursion[60];
    float sumcenter = 0;
    //����������߼���
//  if(Fork.FindFlag == 'T' && (Fork.state == ForkInTurn || Fork.state == ForkOutTurn))
//  {
//      if(Fork.Peak.y <= 15)
//      {
//          if(Fork.In_Direct == 'R')
//              point_center = 60;
//          else
//              point_center = -60;
//          return 1;
//      }
//      for(Ysite = 5;Ysite < 15;Ysite ++)
//      {
//          excursion[Ysite] = (float)(Img.Center_Repair[Ysite] - 64) / (float)(RoadWide0[Ysite] / 2) * 100;
//      }
//      excursion[5] = 0.000f;
//      excursion[6] = 0.000f;
//      excursion[7] = 0.000f;
//      for(Ysite = 5;Ysite < 15 ;Ysite ++)
//      {
//          sumcenter += excursion[Ysite];
//      }
//      if(Fork.state == ForkInTurn)
//          point_center = (int32)(sumcenter / 10 * 3.0f);
//      else if(Fork.state == ForkOutTurn)
//          point_center = (int32)(sumcenter / 10 * 2.2f);
//      if(Fork.In_Direct == 'R')
//          point_center = range_protect(point_center,-100,300);
//      else
//          point_center = range_protect(point_center,-300,100);
//  }
//  //��������߼���
//  else if(Barn.FindFlag == 'T' && (Barn.state == BarnOutTurn || Barn.state == BarnInTurn))
//  {
//      for(Ysite = 10;Ysite < 25;Ysite ++)
//      {
//          excursion[Ysite] = (float)(Img.Center_Repair[Ysite] - 64) / (float)(RoadWide0[Ysite] / 2) * 100;
//      }
//      excursion[10] = 0.000f;
//      excursion[11] = 0.000f;
//      excursion[12] = 0.000f;
//      for(Ysite = 10;Ysite < 25 ;Ysite ++)
//      {
//          sumcenter += excursion[Ysite];
//      }
//      if(Barn.state == BarnOutTurn)
//          point_center = (int32)(sumcenter / 15 * 1.0f);
//      else if(Barn.state == BarnInTurn)
//      {
//          if(Barn.OutDir == 'R')
//              point_center = 200;
//          else if(Barn.OutDir == 'L')
//              point_center = -200;
//      }
//
//  }
//  //�뻷��ת��
//  /*else*/ if(Circle.FindFlag == 'T' && Circle.state == CircleInTurn )
//  {
//      uint8 StartCalLine = 0,EndCalLine = 0;
//      if(Circle.RightUpInflexion.y >= 16 && Circle.dir == 'R')
//      {
//          EndCalLine = Circle.RightUpInflexion.y;
//          StartCalLine = EndCalLine - 10;
//      }
//      else if(Circle.LeftUpInflexion.y >= 16 && Circle.dir == 'L')
//      {
//          EndCalLine = Circle.LeftUpInflexion.y;
//          StartCalLine = EndCalLine - 10;
//      }
//      else if(Circle.state == CircleInTurn)
//      {
//          if(Circle.dir == 'R')
//              point_center = 100;
//          else
//              point_center = -100;
//          return 1;
//      }
//      //����
//      for(Ysite = StartCalLine;Ysite < EndCalLine;Ysite ++)
//      {
//          excursion[Ysite] = (float)(Img.Center_Repair[Ysite] - 64) / (float)(RoadWide0[Ysite] / 2) * 100;
//      }
//          excursion[StartCalLine] = 0.000f;
//      excursion[StartCalLine + 1] = 0.000f;
//      excursion[StartCalLine + 2] = 0.000f;
//      for(Ysite = StartCalLine;Ysite < EndCalLine;Ysite ++)
//      {
//          sumcenter += excursion[Ysite];
//      }
//      point_center = (int32)(sumcenter / 10 * Circle.InTurnK);
//      if(Circle.dir == 'R')
//      {
//          point_center = range_protect(point_center,0,300);
//      }
//      if(Circle.dir == 'L')
//      {
//          point_center = range_protect(point_center,-300,0);
//      }
//  }
//  //������
//  else if(Circle.FindFlag == 'T' && Circle.state == CircleOutTurn)
//  {
//      for(Ysite = 10;Ysite < 20;Ysite ++)
//      {
//          excursion[Ysite] = (float)(Img.Center_Repair[Ysite] - 64) / (float)(RoadWide0[Ysite] / 2) * 100;
//      }
//      excursion[10] = 0.000f;
//      excursion[11] = 0.000f;
//      excursion[12] = 0.000f;
//      for(Ysite = 10;Ysite < 20 ;Ysite ++)
//      {
//          sumcenter += excursion[Ysite];
//      }
//      point_center = (int32)(sumcenter / 10 * Circle.OutTurnK);
//      if(Circle.dir == 'R')
//      {
//          point_center = range_protect(point_center,-100,300);
//      }
//      if(Circle.dir == 'L')
//      {
//          point_center = range_protect(point_center,-300,100);
//      }
//  }
//  //����Ѳ��
//  else if(Circle.FindFlag == 'T' && Circle.state == CircleIn)
//  {
//      for(Ysite = 5;Ysite < 15;Ysite ++)
//      {
//          excursion[Ysite] = (float)(Img.Center_Repair[Ysite] - 64) / (float)(RoadWide0[Ysite] / 2) * 100;
//      }
//      excursion[5] = 0.000f;
//      excursion[6] = 0.000f;
//      for(Ysite = 5;Ysite < 15 ;Ysite ++)
//      {
//          sumcenter += excursion[Ysite];
//      }
//          point_center = (int32)(sumcenter / 10 * 2.0f);
//  }
//  //�뻷ֱ��
//  else if(Circle.FindFlag == 'T' && ��
//          (Circle.state == CircleInStraight || Circle.state == CircleOutCenter || Circle.state == CircleOutStraight))/* ǰհ 15 - 25 */
//  {
//      for(Ysite = 15;Ysite < 25;Ysite ++)
//      {
//          excursion[Ysite] = (float)(Img.Center_Repair[Ysite] - 64) / (float)(RoadWide0[Ysite] / 2) * 100;
//      }
//      excursion[15] = 0.000f;
//      excursion[16] = 0.000f;
//      excursion[17] = 0.000f;
//      for(Ysite = 15;Ysite < 25 ;Ysite ++)
//      {
//          sumcenter += excursion[Ysite];
//      }
//      point_center0 = (int32)(sumcenter / 10 * 2.0f);
//      point_center1 = point_center0;
//      point_center2 = point_center1;
//      point_center = point_center0 * 0.7f + point_center1 * 0.2f + point_center2 * 0.1f;
//      point_center *= 0.5f;
//      //      for(Ysite = 10;Ysite < 20;Ysite ++)
//      {
//          excursion[Ysite] = (float)(Img.Center_Repair[Ysite] - 64) / (float)(RoadWide0[Ysite] / 2) * 100;
//      }
//      excursion[10] = 0.000f;
//      excursion[11] = 0.000f;
//      excursion[12] = 0.000f;
//      for(Ysite = 10;Ysite < 20 ;Ysite ++)
//      {
//          sumcenter += excursion[Ysite];
//      }
//      point_center0 = (int32)(sumcenter / 10 * 2.0f);
//      point_center1 = point_center0;
//      point_center2 = point_center1;
//      point_center = point_center0 * 0.7f + point_center1 * 0.2f + point_center2 * 0.1f;
//  }
    //��ͨ��������
//  else/* ǰհ 15 - 25 *///15-30//30-55
//  {
        #define CenterCalMinRow 12
        #define CenterCalMaxRow 45
        for(Ysite = CenterCalMinRow;Ysite < CenterCalMaxRow;Ysite ++)
        {
            excursion[Ysite] = (float)(Img.Center_Repair[Ysite] - (COL/2)) / (float)(RoadWide0 / 2) * 100;
        }
        for(Ysite = CenterCalMinRow;Ysite < CenterCalMaxRow ;Ysite ++)
        {
            sumcenter += excursion[Ysite];
        }
        point_center0 = (int32)(sumcenter / (CenterCalMaxRow-CenterCalMinRow) * 2.0f);
        point_center1 = point_center0;
        point_center2 = point_center1;
        point_center = point_center0 * 0.7f + point_center1 * 0.2f + point_center2 * 0.1f;
//  }
    return 1;
}



//uint32_t RunCnt = 0;
/**
*@Name			:AllImageDeal
*@Description 	:AllImageDeal
*@Param			:None
*@Return		:NULL
*@Sample		:AllImageDeal();
**/
void AllImageDeal(void)
{
    GetSimBinImage();
    GetBorder();
    CenterCal();
}
/**
*@Name			:ControlPeriodCheck
*@Description 	:ControlPeriodCheck�������ڼ��:��һ�������������ɷ������
*@Param			:None
*@Return		:NULL
*@Sample		:ControlPeriodCheck();
**/
void ControlPeriodCheck(void)
{
    //144MHz��50Hz��PWM��Ƶϵ��Ϊ43,��Ƶ��Ϊ3,348,837Hz,��3349��/ms
    //16λ����������65,536(20ms����)ʱ��������ж�
    uint16_t MinCnt = 7000;//CNT�Ĵ���С�ڴ�ֵʱ�ñ�־λ ��Ҫ����&����
    if (TIM_GetCounter(((TIM_TypeDef *) tim_index[SERVO_PWM>>8])) <= MinCnt) {//timer_get();
        InControlPeriod_Flag = 'T';
    }
}

/**
*@Name			:DirectionControl
*@Description 	:DirectionControl
*@Param			:None
*@Return		:None
*@Sample		:DirectionControl();
**/
void DirectionControl(void)
{
    if ((CarInfo.ClosedLoopMode==DirectLoop || \
            CarInfo.ClosedLoopMode==AllLoop || \
            CarInfo.ClosedLoopMode==SpeedLoop) && \
            CarInfo.Protect_Flag=='F') {
        if (CarInfo.ControlMode==CAMMODE) {//����ͷ����
            DirectionOut = ServoPosition_PID(&ServoPIDParam[0][0],&ServoControlCam,0,point_center);
    //      DirectionOut = Position_PID(&TurnPIDParam[0][0],&TurnLoopCam,0,point_center );
    //      DirectionOut = Fuzzy_PID(&AngleLoop_Fuzzy, point_center,0);
        }
        else if (CarInfo.ControlMode==ELCMODE) {//��ſ���
    //      DirectionOut = Position_PID(&TurnPIDParam[0][0],&TurnLoopCam,0,ELC_Center_14);
        }
        else {
            DirectionOut = 0;
        }
    }
    else {
        DirectionOut = 0;
    }
    ServoAdd = DirectionOut;
    ServoWrite();
}

/**
*@Name			:ExSpeedControl
*@Description 	:ExSpeedControl�������ڼӼ��ٿ���
*@Param			:None
*@Return		:NULL
*@Sample		:ExSpeedControl();
**/
void ExSpeedControl(void)
{
//    if (1) {
//        ExSpeed = ExSpeed;
//    }
}

/**
*@Name			:DiffSpeedControl
*@Description 	:DiffSpeedControl���ٿ���
*@Param			:None
*@Return		:NULL
*@Sample		:DiffSpeedControl();
**/
void DiffSpeedControl(void)
{
    static uint16_t Ackerman_kl = 200;//���Ҳ���ϵ��
    static uint16_t Ackerman_kr = 200;

    if (CarInfo.ClosedLoopMode == AllLoop && \
            CarInfo.Protect_Flag == 'F') {
        if(ServoAdd <= 0)//��ת
        {
            ExSpeed_L=ExSpeed;
            ExSpeed_R=(int)(ExSpeed*(1+((float)Ackerman_kr/1000)*tan((float)ServoAdd*3.14/4600)/0.396));
        }
        else if(ServoAdd > 0)//��ת
        {
            ExSpeed_L=(int)(ExSpeed*(1-((float)Ackerman_kl/1000)*tan((float)ServoAdd*3.14/4600)/0.396));
            ExSpeed_R=ExSpeed;
        }
        else
        {
            ExSpeed_L=ExSpeed;
            ExSpeed_R=ExSpeed;//���ּ������ּ���ȷ���������ٶȲ���
        }
    }
}
/**
*@Name			:SpeedControl
*@Description 	:SpeedControl�ٶ�PID����
*@Param			:None
*@Return		:NULL
*@Sample		:SpeedControl();
**/
void SpeedControl(void)
{

    EncoderRead();

    if (CarInfo.Protect_Flag == 'F') {
        if (CarInfo.ClosedLoopMode == AllLoop || \
            CarInfo.ClosedLoopMode == SpeedLoop) {
            SpeedOut_L = SimpleIncremental_PID(&SpeedPIDParam[0][0],&SpeedPIDLeft,(float)(ExSpeed_L),Wheel_L.SpeedNow);
            SpeedOut_R = SimpleIncremental_PID(&SpeedPIDParam[0][0],&SpeedPIDRight,(float)(ExSpeed_R),Wheel_R.SpeedNow);
        }
        else if (CarInfo.ClosedLoopMode == LR_SingleControl) {
            SpeedOut_L = SimpleIncremental_PID(&SpeedPIDParam[0][0],&SpeedPIDLeft,(float)(ExSpeed_L),Wheel_L.SpeedNow);
            SpeedOut_R = SimpleIncremental_PID(&SpeedPIDParam[0][0],&SpeedPIDRight,(float)(ExSpeed_R),Wheel_R.SpeedNow);
        }
        else {
            SpeedOut_L = 0;
            SpeedOut_R = 0;
        }
    }
    else {//�ٶȿ��㱣��

        SpeedOut_L = SimpleIncremental_PID(&LeftMotorSpeedPIDParam[0][0],&SpeedPIDLeft,0,Wheel_L.SpeedNow);
        SpeedOut_R = SimpleIncremental_PID(&RightMotorSpeedPIDParam[0][0],&SpeedPIDRight,0,Wheel_R.SpeedNow);
    }
    pwm_r   = SpeedOut_L;
   pwm_l = SpeedOut_R;
  //     pwm_l = 100;
   //    pwm_r = 100;

    MotorWrite();
}
/**
*@Name			:Timer7_IT
*@Description 	:Timer7_IT������ʱ��7�жϷ�����������ͼ�����������
*@Param			:None
*@Return		:NULL
*@Sample		:Timer7_IT();
**/
void Timer7_IT(void)
{
//    ControlPeriodCheck();
//    if (InControlPeriod_Flag == 'T') {//�ݲ�ʹ��
//        //int32_t RunTime = systick_getvel_ms();
////        AllImageDeal();
//        DirectionControl();
//        InControlPeriod_Flag = 'F';
//    }
}

//������������������
int16 CarProtect_IntCnt = 0;
void CarProtect(void)
{
    if((CarInfo.UpTime >= 2.000f) && ((CarInfo.ClosedLoopMode == AllLoop) || \
        (CarInfo.ClosedLoopMode == SpeedLoop)))
    {
        //---------------��ű���--------------//
        if(ElcProtect_Flag == 'T' && CarInfo.CAR_PROTECT_MODE == 'T')
        {
            CarProtect_IntCnt ++;
        }
        if(CarProtect_IntCnt >= 5)
        {
            CarInfo.Protect_Flag = 'T';
        }
    }
    if(CarInfo.StopTime != 0.000f && CarInfo.UpTime >= CarInfo.StopTime)
    {
        CarInfo.Protect_Flag = 'T';
        Base.element_check = 'F';
    }
}

//��ʾ�����������
const char CarInfoItem0[] = "Protect_flag En:";
const char CarInfoItem1[] = "Control Loop:";
const char CarInfoItem2[] = "Params Read:";

const char ClosedLoopModePrams[4][10]={"NOLoop    ","SpeedLoop ","DirectLoop","AllLoop   "};

#define CarInfo_MENU_NUM 2
#define ControlMood_MENU_NUM 4

uint8 CarInfoShow(void)
{
    while(KeyLast != onepress)
    {
//      static int8 NowChooseControlMood = 1;
        static uint8 NowCarInfoItem = 0;
        MenuChoose(&NowCarInfoItem,CarInfo_MENU_NUM);
        //��ʾ��Ŀ
        ips114_pencolor = RED;
        ips114_show_string(16,0,CarInfoItem0);
        ips114_show_string(16,1,CarInfoItem1);
        ips114_show_string(16,2,CarInfoItem2);
        //��ʾ����
        ips114_pencolor = BLUE;
        if(CarInfo.CAR_PROTECT_MODE == 'T')
            ips114_show_string(128,0,(const char*)("T"));
        else
            ips114_show_string(128,0,(const char*)("F"));

        if(CarInfo.ClosedLoopMode == AllLoop)
            ips114_show_string(128,1,(const char*)("AllLoop"));
        else if(CarInfo.ClosedLoopMode == SpeedLoop)
            ips114_show_string(128,1,(const char*)("SpeedLoop"));
        else if(CarInfo.ClosedLoopMode == DirectLoop)
            ips114_show_string(128,1,(const char*)("DirectLoop"));
        else if(CarInfo.ClosedLoopMode == NoLoop)
            ips114_show_string(128,1,(const char*)("NoLoop"));

        if(CarInfo.ReadFlashParams == 'T')
            ips114_show_string(128,2,(const char*)("T"));
        else
            ips114_show_string(128,2,(const char*)("F"));
        //����ȷ��
        if(KeyNext == onepress)
        {
            KeyNext = nopress;
            switch(NowCarInfoItem)
            {
                case(0):
                {
                    StateAdjust(&CarInfo.CAR_PROTECT_MODE,128,0);
                    break;
                }
                case(1):
                {
                    EnumAdjust((uint8*)(&CarInfo.ClosedLoopMode),(char(*)[])(&ClosedLoopModePrams),4,128,1);
                    break;
                }
                case(2):
                {
                    StateAdjust(&CarInfo.ReadFlashParams,128,2);
                    break;
                }
            }
        }
        my_delay(10);
    }
    KeyLast = nopress;
    //
    return 1;
}


