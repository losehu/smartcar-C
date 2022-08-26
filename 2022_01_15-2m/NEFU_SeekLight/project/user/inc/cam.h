/**
 //@FileName	:cam.h
 //@CreatedDate	:2021��12��22��
 //@Author		:LiHao
 //@Description	:
**/

#ifndef CAM_H_
#define CAM_H_

#include "zf_common_headfile.h"
#include <stdbool.h>
#define         ROW                     100//USED_ROW|TRFED_ROW //��Զ����Ϊ0~ROW
#define         COL                     114//USED_COL|TRFED_COL


#define BlackPoint      0
#define WhitePoint      255
#define GrayPoint       127

#define RoadWide0       45//ʵ����͸�Ӻ��������45�����أ�ǡ��һ�����ش���1����

#define LBorderCol      (COL/2 - RoadWide0/2)//35
#define RBorderCol      (COL/2 + RoadWide0/2)//79
#define CenterCol       (COL/2)//57
#define LOutBorderCol   (2 * LBorderCol - CenterCol)//13
#define ROutBorderCol   (2 * RBorderCol - CenterCol)//101

#define ImgDubug        0

#define StartScanRow    1//��ʼɨ����

#define LIMIT_COL(x,_max,_min)  (x > _max ? x = _max : (x < _min ? x = _min : x))

//void ImageUnzip(uint8 *zipsrc,uint8 *unzipsrc) ;
//void  CameraExtract0(void);


#define NoneSlope   100.0f//б�ʲ����ڵ����


typedef struct{
    uint8 x;
    uint8 y;
}PosiType;

typedef struct{
    int16 Center[ROW];//��
    int16 Center_Repair[ROW];//�޸��������
    uint8 Center_Error[ROW];//ͼ���Ƿ�������־

    int16 LeftBorder[ROW];//��
    int16 LeftBorder_Repair[ROW];
    uint8 LeftBorderFindFlag[ROW];
    float LeftSlope[ROW];

    int16 RightBorder[ROW];//��
    int16 RightBorder_Repair[ROW];
    uint8 RightBorderFindFlag[ROW];
    float RightSlope[ROW];

    int8  RoadWide[ROW];//����ͷ��ÿ��
    int8  RoadWideError[ROW];//��������ȱ�Ƚϵõ������
    uint8 RoadNormalFlag[ROW];//�������
}ImageDealType;



//-------------������������--------------//
typedef struct{
    int16    StartSearchColumn_1st;
    int16    StartSearchColumn;
    PosiType firstleftnormal;
    PosiType firstrightnormal;
    PosiType firstleftlose;//��һ�ζ�ʧ���߽������ҵ����ߵĻ����� ���û�ж�ʧ��˵��������Ϊ0
    PosiType firstrightlose;
    uint8    firstroadnormal;
    uint8    firstroaderror;
    uint8    Topline;//��ǰͼ�񶥱�
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


//----------------��״̬-----------------//

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
}LineStatusType;//��Ե״̬ ��ͬ���е���ͳ��

//type
typedef struct{
    uint8 x;
    uint8 y;
    int8 dir;
}BorderType;

//��״̬ ԭʼͼ��
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

//��״̬ ԭʼͼ��
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
//extern const uint8            RoadWide0[ROW] ;//ֱ���������
extern int16            Xsite;
extern int16            Ysite;
extern ImageDealType    Img;
extern BaseTypedef      Base;
extern LineStatusType   LineALL,Line5_35,Line30_60;
extern ColumnTypedef    Column10,Column117,Column64;
extern RowTypedef       Row56;
//---------------ɨ��---------------------//


//fucntion
bool    InRange(int32 x,int32 _min,int32 _max);
uint8   TwoPoint_AddingLine(uint8 x1,uint8 y1,uint8 x2,uint8 y2,int16 *p,uint8 StopYsite);
void    ImageProcessingStatusInit(void);
uint8   GetBorder(void);
uint8   ElementProcessing();



//-------------------------------------ֱ��-------------------------------------------------//
typedef struct{
    uint8 FindFlag;//���ڱ�־
    uint8 Beep;
    uint8 LeftLong;//��ֱ��
    uint8 RightLong;
    uint8 LeftShort;//���ֱ��
    uint8 RightShort;
    ////����
    uint8 LeftCnt;//����
    uint8 RightCnt;
    uint8 LeftCnt40up;
    uint8 RightCnt40up;
}StraightTypedef;


extern StraightTypedef  Straight;


//fun
uint8 StrightJudge(void);


//-------------------------------------���-------------------------------------------------//
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

//-------------------------------------����·��-------------------------------------------------//
typedef enum{
    ForkOut = 0,
    ForkFound,
    ForkInTurn,
    ForkIn,
    ForkOutTurn,
    ForkRelay//����ģʽ
} ForkState;

typedef struct{
    //ʶ��
    uint8 FindFlag;
    uint8 InState;
    uint8 StartLine;
    uint8 CenterLineBlack;
    ForkState state;
    PosiType Peak;
    uint8 PeakFindFlag;
    uint8 ForkLeftBorder[ROW];//��ɫ���ֵ����ұ�Ե
    uint8 ForkRightBorder[ROW];
    uint8 MaybeCorssFlag;
    //��¼
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


//-------------------------------------����-------------------------------------------------//


typedef enum
{
    BarnOutOK_Start = 0,
    BarnStart,
    BarnOutStraight,
    BarnOutTurn,
    BarnOutOK,
    BarnTurnStraight,
    BarnInTurnStraight,//��������ת�����ʻ��һС�ξ���
    BarnInTurn,
    BarnInStraight,
    BarnIn,
    BarnPass,
} BarnStateEnum;

typedef struct
{
    uint8 FindFlag;
    BarnStateEnum state;
    float BarnStartDistance;//�����ܵ�ֱ��һС�ξ���
    uint16 BarnNum;//��ǰ����������
    uint16 BarnFoundELCSum14Min;//�������ʶ������ֵ
    float BarnLength;//����ܹ���Ҫ��
    float BarnFindDis;//���ֳ��⵱ǰ���
    float BarnInTurnDis;//ת��������ʱ��ǰ���
    float BarnInDistanceCount;//ת����ϵ�ǰ����
    uint16 BarnInStraightSpeed;//ת��ȥ����ʻ�ٶ�

    //����˵��ɵ�
    uint8 OutDir;//���ⷽ��
    uint16 BranInNum;//�ڼ��ξ�������ʱ���
    uint16 StartLineFoundY;//�����߿�ʼʶ����
    float  BeforeTurnRunDis;//ʶ�����һ�ξ����ٹ�
    uint16 BarnTurnCenter;//ʶ�������߸�ֵ
    uint16 BarnTurnSpeed;//ʶ����ת���ٶ�
    float  BarnInRunDis;//ת��ȥ���ܶ೤����ͣ��


}BarnTypedef;

extern uint16 BarnInInfo[2][6];
extern BarnTypedef      Barn;


//fun
void    BarnOutJudge(void);
void    BarnJudge(void);
void    BarnAddingLine(void);

//-------------------------------------����-------------------------------------------------//

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
    //ʶ��
    uint8 FindFlag;
    CircleStateEnum state;
    uint8 RoundCenterY;
    PosiType LeftUpInflexion;
    PosiType RightUpInflexion;
    PosiType CircleCenter;
    //��¼
    uint16  CircleAmount;//������ 1Ȧ ��Ź�����ʱ����
    uint16 CircleELC14;//ʶ����ֵ14
    uint16 CircleELC23;//ʶ����ֵ23
    uint16  CircleNum;//��ǰ����������
    float  CircleEndDis;//��������
    float  DontELCJudgeDis;//ʧ�ܵ���жϾ��� �ӽ�����ʼ�� ��ֹ������ʱ�ظ�����ж�
    uint8  dir;//����
    uint16 CircleR;//�뾶
    uint16 FoundOutSpeed;//ʶ���ٶ� �����ٶ�
    uint16 CenterELC;//��������ֵ
    float  CenterDis;//�е�ʶ��
    float  TurnDis;//һ�ξ����
    uint16 TurnSpeed;//0��ʧ���ٶȻ� ��0������ٶ�
    uint16 InTurnUP_Y;//�Ϲյ�Y
    float InTurnK;//ת��k
    uint16 InSpeed;//�����ٶ�
    float OutTurnK;//ת��k

}CircleTypedef;


extern uint32 CircleInfo[][9];
extern CircleTypedef    Circle;


//fun
void    CircleJudge(void);
void    CircleAddingLine(void);



//-------------------------------------�µ�-------------------------------------------------//
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
    uint8 MayRampFlag;//����ͷ�б��־
    float MayRampDis;//����ͷ�б�ʱ���
    float MayRampGyroYMax;//����ͷ�б��������Ǵ���ֵ
    float MayRampFlagClearDis;//����ͷ���г������־����
    float UpRampDis;//�µ�ȷ�Ͼ���
    float RampLegth;//�µ�����
    float EndRampDis;//�µ����¾���
    uint8 RecoverFlag;//�ָ���־ ʹ�õ�Żָ�����״̬
    float RecoverDis;//�ָ��ľ���

    float DontJudgeRampDis;//���º�һ�ξ����ڲ��������µ�
    uint8 LeftSlopeSatisfy;
    uint8 RightSlopeSatisfy;
    float RampFindDis;
    float RampEndDis;
}RampTypedef;

//statement
extern RampTypedef Ramp;

//fun
void RampJudge(void);


//-------------------------------------ʮ��-------------------------------------------------//


typedef enum
{
    CrossEnd = 0,
    CrossIn,
}CrossStateEnum;


typedef struct
{
    uint8 FindFlag;
    CrossStateEnum state;
    PosiType LD_Inflexion;//���¹յ�
    PosiType RD_Inflexion;
    PosiType LU_Inflexion;//���Ϲյ�
    PosiType RU_Inflexion;
    uint8    LD_UpAbnormalCnt;//���¹յ����ϲ�������(��ʧ�߽�)����
    uint8    RD_UpAbnormalCnt;
    uint8    LU_UpNormalCnt;//���Ϲյ�����������(�����������)����
    uint8    RU_UpNormalCnt;
    float    CrossStartDis;//ʶ��ʮ��ʱ��Wheel.Distance
    float    CrossLength;//ʮ�ֳ���
}CrossTypedef;


extern CrossTypedef Cross;


void CrsosJudge(void);
void CrossAddingLine(void);



#endif /* CAM_H_ */
