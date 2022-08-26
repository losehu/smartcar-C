/**
 //@FileName	:cam.h
 //@CreatedDate	:2021年12月22日
 //@Author		:LiHao
 //@Description	:
**/

#ifndef CAM_H_
#define CAM_H_

#include "zf_common_headfile.h"
#include <stdbool.h>
#define         ROW                     100//USED_ROW|TRFED_ROW //从远到进为0~ROW
#define         COL                     114//USED_COL|TRFED_COL


#define BlackPoint      0
#define WhitePoint      255
#define GrayPoint       127

#define RoadWide0       45//实测逆透视后赛道宽度45个像素，恰好一个像素代表1厘米

#define LBorderCol      (COL/2 - RoadWide0/2)//35
#define RBorderCol      (COL/2 + RoadWide0/2)//79
#define CenterCol       (COL/2)//57
#define LOutBorderCol   (2 * LBorderCol - CenterCol)//13
#define ROutBorderCol   (2 * RBorderCol - CenterCol)//101

#define ImgDubug        0

#define StartScanRow    1//开始扫线行

#define LIMIT_COL(x,_max,_min)  (x > _max ? x = _max : (x < _min ? x = _min : x))

//void ImageUnzip(uint8 *zipsrc,uint8 *unzipsrc) ;
//void  CameraExtract0(void);


#define NoneSlope   100.0f//斜率不存在的情况


typedef struct{
    uint8 x;
    uint8 y;
}PosiType;

typedef struct{
    int16 Center[ROW];//中
    int16 Center_Repair[ROW];//修复后的中线
    uint8 Center_Error[ROW];//图像是否连续标志

    int16 LeftBorder[ROW];//左
    int16 LeftBorder_Repair[ROW];
    uint8 LeftBorderFindFlag[ROW];
    float LeftSlope[ROW];

    int16 RightBorder[ROW];//右
    int16 RightBorder_Repair[ROW];
    uint8 RightBorderFindFlag[ROW];
    float RightSlope[ROW];

    int8  RoadWide[ROW];//摄像头测得宽度
    int8  RoadWideError[ROW];//和赛道宽度表比较得到的误差
    uint8 RoadNormalFlag[ROW];//赛道宽度
}ImageDealType;



//-------------基本赛道条件--------------//
typedef struct{
    int16    StartSearchColumn_1st;
    int16    StartSearchColumn;
    PosiType firstleftnormal;
    PosiType firstrightnormal;
    PosiType firstleftlose;//第一次丢失边线建立在找到边线的基础上 如果没有丢失则此点横纵坐标为0
    PosiType firstrightlose;
    uint8    firstroadnormal;
    uint8    firstroaderror;
    uint8    Topline;//当前图像顶边
    uint8    ToplineTemp;
    uint8    ToplineFindFlag;
    PosiType LeftMax;
    PosiType LeftMin;
    PosiType RightMax;
    PosiType RightMin;
    PosiType CenterMin;
    PosiType CenterMax;
    uint16   CenterErrorCnt;
    uint8    element_check;
    PosiType LeftMin20up;
    PosiType LeftMax20up;
    PosiType RightMin20up;
    PosiType RightMax20up;

}BaseTypedef;


//----------------行状态-----------------//

typedef struct
{
    uint8 AllNormal;
    uint8 LeftNormal;
    uint8 RightNormal;
    uint8 LeftLose;
    uint8 RightLose;
    uint8 AllLose;
    uint8 LeftNormalRightLose;
    uint8 LeftLoseRightNormal;
}LineStatusType;//边缘状态 不同的行单独统计

//type
typedef struct{
    uint8 x;
    uint8 y;
    int8 dir;
}BorderType;

//列状态 原始图像
typedef struct {
    uint8 firstblackY;
    uint8 firstblackYFlag;
    uint8 firstwhiteY;
    uint8 firstwhiteYFlag;
    uint8 firstblackY20up;
    uint8 firstblackY20upFlag;
    uint8 firstwhiteY20up;
    uint8 firstwhiteY20upFlag;
    uint8 Line0_30_Black;
    uint8 Line0_30_White;
    uint8 Line30_60_Black;
    uint8 Line30_60_White;
}ColumnTypedef;

//行状态 原始图像
typedef struct
{
    uint16 LeftToRightFirstBlack;
    uint8  LeftToRightFirstBlackFindFlag;
    uint16 RightToLeftFirstBlack;
    uint8  RightToLeftFirstBlackFindFlag;
    uint16 LeftToRightFirstWhite;
    uint8  LeftToRightFirstWhiteFindFlag;
    uint16 RightToLeftFirstWhite;
    uint8  RightToLeftFirstWhiteFindFlag;
}RowTypedef;

//statement
//extern const uint8            RoadWide0[ROW] ;//直道赛道宽度
extern int16            Xsite;
extern int16            Ysite;
extern ImageDealType    Img;
extern BaseTypedef      Base;
extern LineStatusType   LineALL,Line5_35,Line30_60;
extern ColumnTypedef    Column10,Column117,Column64;
extern RowTypedef       Row56;
//---------------扫线---------------------//


//fucntion
bool    InRange(int32 x,int32 _min,int32 _max);
uint8   TwoPoint_AddingLine(uint8 x1,uint8 y1,uint8 x2,uint8 y2,int16 *p,uint8 StopYsite);
void    ImageProcessingStatusInit(void);
uint8   GetBorder(void);
uint8   ElementProcessing();



//-------------------------------------直道-------------------------------------------------//
typedef struct{
    uint8 FindFlag;//存在标志
    uint8 Beep;
    uint8 LeftLong;//左长直道
    uint8 RightLong;
    uint8 LeftShort;//左短直道
    uint8 RightShort;
    ////计数
    uint8 LeftCnt;//计数
    uint8 RightCnt;
    uint8 LeftCnt40up;
    uint8 RightCnt40up;
}StraightTypedef;


extern StraightTypedef  Straight;


//fun
uint8 StrightJudge(void);


//-------------------------------------弯道-------------------------------------------------//
typedef struct{
    uint8 FindFlag;
    uint8 Beep;
    uint8 TurnLeft;//??
    uint8 TurnRight;
    ////Cnt
    uint8 LB_TR_Cnt;
    uint8 RB_TL_Cnt;
}CurveTypedef;

extern CurveTypedef     Curve;


//fun
uint8 CurveJudge(void);

//-------------------------------------三岔路口-------------------------------------------------//
typedef enum{
    ForkOut = 0,
    ForkFound,
    ForkInTurn,
    ForkIn,
    ForkOutTurn,
    ForkRelay//接力模式
} ForkState;

typedef struct{
    //识别
    uint8 FindFlag;
    uint8 InState;
    uint8 StartLine;
    uint8 CenterLineBlack;
    ForkState state;
    PosiType Peak;
    uint8 PeakFindFlag;
    uint8 ForkLeftBorder[ROW];//黑色部分的左右边缘
    uint8 ForkRightBorder[ROW];
    uint8 MaybeCorssFlag;
    //记录
    uint16 ForkAmount;
    uint16 ForkNum;
    uint8 In_Direct;
    float InDis;
    float TurnInK;
    float TurnOutK;
    uint16 InSpeed;
    uint16 OutSpeed;
}ForkTypedef;

extern ForkTypedef  Fork;

extern uint32 ForkInfo[4][3];
uint8 ForkJudge(void);
void ForkAddingLine(void);


//-------------------------------------车库-------------------------------------------------//


typedef enum
{
    BarnOutOK_Start = 0,
    BarnStart,
    BarnOutStraight,
    BarnOutTurn,
    BarnOutOK,
    BarnTurnStraight,
    BarnInTurnStraight,//就是允许转向后行驶的一小段距离
    BarnInTurn,
    BarnInStraight,
    BarnIn,
    BarnPass,
} BarnStateEnum;

typedef struct
{
    uint8 FindFlag;
    BarnStateEnum state;
    float BarnStartDistance;//出库跑的直道一小段距离
    uint16 BarnNum;//当前经过车库数
    uint16 BarnFoundELCSum14Min;//车库最低识别电磁阈值
    float BarnLength;//电磁跑过需要用
    float BarnFindDis;//发现车库当前里程
    float BarnInTurnDis;//转向允许开启时当前里程
    float BarnInDistanceCount;//转向完毕当前距离
    uint16 BarnInStraightSpeed;//转过去的行驶速度

    //车库菜单可调
    uint8 OutDir;//出库方向
    uint16 BranInNum;//第几次经过车库时入库
    uint16 StartLineFoundY;//起跑线开始识别行
    float  BeforeTurnRunDis;//识别后跑一段距离再拐
    uint16 BarnTurnCenter;//识别后的中线赋值
    uint16 BarnTurnSpeed;//识别后的转向速度
    float  BarnInRunDis;//转过去后跑多长距离停车


}BarnTypedef;

extern uint16 BarnInInfo[2][6];
extern BarnTypedef      Barn;


//fun
void    BarnOutJudge(void);
void    BarnJudge(void);
void    BarnAddingLine(void);

//-------------------------------------环岛-------------------------------------------------//

typedef enum
{
    CircleFound = 0,
    CircleInStraight,
    CircleCenter,
    CircleCouldInTurn,
    CircleInTurn,
    CircleIn,
    CircleOutTurn,
    CircleOutTurnOK,
    CircleOutCenter,
    CircleOutStraight,
    CircleOut,
} CircleStateEnum;


typedef struct
{
    //识别
    uint8 FindFlag;
    CircleStateEnum state;
    uint8 RoundCenterY;
    PosiType LeftUpInflexion;
    PosiType RightUpInflexion;
    PosiType CircleCenter;
    //记录
    uint16  CircleAmount;//总数量 1圈 电磁过车库时清零
    uint16 CircleELC14;//识别阈值14
    uint16 CircleELC23;//识别阈值23
    uint16  CircleNum;//当前经过环岛数
    float  CircleEndDis;//结束距离
    float  DontELCJudgeDis;//失能电磁判断距离 从结束开始算 防止出环岛时重复电磁判断
    uint8  dir;//方向
    uint16 CircleR;//半径
    uint16 FoundOutSpeed;//识别速度 出环速度
    uint16 CenterELC;//环岛中阈值
    float  CenterDis;//中点识别
    float  TurnDis;//一段距离拐
    uint16 TurnSpeed;//0会失能速度环 非0则控制速度
    uint16 InTurnUP_Y;//上拐点Y
    float InTurnK;//转入k
    uint16 InSpeed;//环内速度
    float OutTurnK;//转出k

}CircleTypedef;


extern uint32 CircleInfo[][9];
extern CircleTypedef    Circle;


//fun
void    CircleJudge(void);
void    CircleAddingLine(void);



//-------------------------------------坡道-------------------------------------------------//
typedef enum
{
    UpRamp,
    DownRamp,
    EndRamp
}RampState;

typedef struct
{
    uint8 FindFlag;
    RampState State;
    uint8 MayRampFlag;//摄像头判别标志
    float MayRampDis;//摄像头判别时里程
    float MayRampGyroYMax;//摄像头判别后的陀螺仪触发值
    float MayRampFlagClearDis;//摄像头误判超距离标志清零
    float UpRampDis;//坡道确认距离
    float RampLegth;//坡道长度
    float EndRampDis;//坡道下坡距离
    uint8 RecoverFlag;//恢复标志 使用电磁恢复下坡状态
    float RecoverDis;//恢复的距离

    float DontJudgeRampDis;//下坡后一段距离内不允许判坡道
    uint8 LeftSlopeSatisfy;
    uint8 RightSlopeSatisfy;
    float RampFindDis;
    float RampEndDis;
}RampTypedef;

//statement
extern RampTypedef Ramp;

//fun
void RampJudge(void);


//-------------------------------------十字-------------------------------------------------//


typedef enum
{
    CrossEnd = 0,
    CrossIn,
}CrossStateEnum;


typedef struct
{
    uint8 FindFlag;
    CrossStateEnum state;
    PosiType LD_Inflexion;//左下拐点
    PosiType RD_Inflexion;
    PosiType LU_Inflexion;//左上拐点
    PosiType RU_Inflexion;
    uint8    LD_UpAbnormalCnt;//左下拐点向上不正常行(丢失边界)计数
    uint8    RD_UpAbnormalCnt;
    uint8    LU_UpNormalCnt;//左上拐点向上正常行(赛道宽度正常)计数
    uint8    RU_UpNormalCnt;
    float    CrossStartDis;//识别到十字时的Wheel.Distance
    float    CrossLength;//十字长度
}CrossTypedef;


extern CrossTypedef Cross;


void CrsosJudge(void);
void CrossAddingLine(void);



#endif /* CAM_H_ */
