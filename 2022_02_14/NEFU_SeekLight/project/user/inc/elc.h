/**
 //@FileName	:elc.h
 //@CreatedDate	:2021��12��22��
 //@Author		:LiHao
 //@Description	:
**/

#ifndef ELC_H_
#define ELC_H_

#include "zf_common_headfile.h"

#define L1 2
#define L2 0
#define L3 3
#define L4 1

#define ELC_NUM     4       //�������
#define ELC_GROUP   3       //�ɼ�����
#define ELC_TIME    3       //ÿ��Ĳɼ�����
#define ELC_NEED    1       //������������ֵ


extern uint16   ELC[ELC_GROUP][ELC_TIME][ELC_NUM];
extern int32    LNow[ELC_NUM],LMax[ELC_NUM],LMin[ELC_NUM];
extern float    LNor[ELC_NUM];//��һ������ֵ
extern uint16   LNORMAX[ELC_NUM];
extern uint8    MaxNorIndex;
extern uint8    MinNorIndex;
extern uint16   ElcSum_14;
extern uint16   ElcSum_23;
extern uint16   ElcSum_1234;
extern int16    ElcCenter_14;
extern int16    ElcCenter_23;
extern uint8    ElcProtect_Flag;
extern uint16   KFP_ElcSum_14;
extern uint16   KFP_ElcSum_23;
extern float    ElcCenter_14_100;
//function
void    ElcParams_Init(void);
void    ElcRead(void);
void    ElcNormalized(void);
void    ElcCenterCalculate(void);
uint8   ElcJudge(void);
uint8   ElcShow(void);
void    ElcStatus_Init(void);



//---------------------------------------------------����---------------------------------------//


typedef struct
{
    uint8 FindFlag;//���ֱ�־
    uint8 SUM_MAX;//û������ǰ�����ֵ
    uint8 NowValLessThanMaxCnt;//����������־
    uint8 CircleInTurnFlag;
}ElcCircleTypedef;


//statement
extern ElcCircleTypedef ElcCircle;


//fun
uint8   ElcCircleJudge(void);


#endif /* ELC_H_ */
