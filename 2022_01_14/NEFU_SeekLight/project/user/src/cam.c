/**
 //@FileName	:cam.c
 //@CreatedDate	:2021��12��22��
 //@Author		:LiHao
 //@Description	:
**/

#include "cam.h"

/***********************************************************************
    (1)����4�����ҵ����ұ߽�,<��¼ɨ�ߵ�����,��Ե����ķ���>
    (2)��¼ÿ�е������޸��Լ�δ�޸��߽�,ɨ���޸�δ�޸�����,
    ��һ�ζ�ʧ ��һ������ �ڶ��ζ�ʧ �ڶ�����������ÿ��б��,����״̬
    (3)<������¼������֮�����״̬>
    (3)<Ԫ���ж� ֱ��-���-�µ�-ʮ��-����-����-����>
    (4)<����Ԫ���޸����߱���>
    (5)��������
************************************************************************/

/***************************��������(ctrl + G��ת)*********************
    (1)�ܵı�־��ʼ��      ------------------------------------     70
    (2)���㲹��             ------------------------------------    115
    (3)��Ե����             ------------------------------------    340
    (4)����Ԫ��             ------------------------------------    780
    (5)ֱ���б�             ------------------------------------    810
    (6)����б�             ------------------------------------    880
    (7)�����б�             ------------------------------------   1000
    (8)������             ------------------------------------   1050
    (9)�����б�             ------------------------------------   1200
    (10)���ⲹ��            ------------------------------------   1250
    (11)�����б�            ------------------------------------   1500
    (12)��������            ------------------------------------   1500
    (13)�µ��б�            ------------------------------------   1870
    (14)ʮ���б�            ------------------------------------   1950
    (15)ʮ�ֲ���            ------------------------------------   2000
*********************************************************************/

//ֱ���������
//const uint8 RoadWide[ROW] =
//{
//    128,128,128,128,128,127,125,123,121,120,
//    119,117,116,114,112,110,108,107,105,103,
//    101,99,97,95,94,92,90,88,86,84,
//    83,80,78,76,74,72,70,68,67,65,
//    63,61,59,57,55,53,52,51,50,47,
//    45,43,42,39,37,36,34,32,30,28,
//};




BaseTypedef     Base;
int16           Pic_Ysite = 0;
int16           Xsite = 0;//��ǰɨ��X����
int16           Ysite = 0;//��ǰɨ��Y����
ImageDealType   Img;
LineStatusType  LineALL,Line5_35,Line30_60;
ColumnTypedef   Column10,Column117,Column64;
RowTypedef      Row56;
//---------------------------------�����״̬��������--------------------------//
void LineStatusClear(LineStatusType *Line)
{
    Line->AllNormal = 0;
    Line->AllLose = 0;
    Line->LeftNormal = 0;

    Line->RightNormal = 0;
    Line->LeftLose = 0;
    Line->RightLose = 0;
    Line->LeftLoseRightNormal = 0;
    Line->LeftNormalRightLose = 0;
}

//----------------------��־��ʼ��(��bsp_init��ʼ��һ��)-------------------//
void ImageProcessingStatusInit(void)
{
    //----------------------------BASE----------------------------------//
    Base.Topline = ROW - 1;
    Base.ToplineTemp = ROW -1;
    Base.ToplineFindFlag = 'T';
    LineStatusClear(&LineALL);
    LineStatusClear(&Line5_35);
    LineStatusClear(&Line30_60);
    Base.CenterErrorCnt = 0;
    Base.element_check = 'F';
    //Ԫ�ر�־��ʼ��
    //straight
    Straight.Beep = 'F';
    Straight.FindFlag = 'F';
    //curve
    Curve.Beep = 'F';
    Curve.FindFlag = 'F';
    //fork
    Fork.FindFlag = 'F';
    Fork.Peak.x = 0;
    Fork.Peak.y = 0;
    Fork.StartLine = 0;
    Fork.In_Direct = 'R';
    Fork.state = ForkOut;
    Fork.InDis = 0;
    Fork.ForkNum = 0;
    Fork.TurnInK = 0;
    Fork.TurnOutK = 0;
    Fork.ForkAmount = 2;
    Fork.OutSpeed = 180;
    Fork.InSpeed = 240;
    //Barn
    Barn.FindFlag = 'F';
    Barn.OutDir = 'R';
    Barn.state = BarnOutOK_Start;
    Barn.BarnFoundELCSum14Min = 10;
    Barn.BarnNum = 0;
    Barn.BranInNum = 1;
    Barn.BarnFindDis = 0;
    Barn.BarnLength = 0.6f;
    Barn.BarnInStraightSpeed = 230;
    Barn.BarnInDistanceCount = 0.0;
    Barn.StartLineFoundY = 20;//��ʼ������
    Barn.BarnTurnCenter = 150;
    Barn.BarnTurnSpeed = 230;
    Barn.BarnInRunDis = 0.30;

    //Circle
    Circle.FindFlag = 'F';
    Circle.state = CircleOut;
    Circle.RoundCenterY = 0;
    Circle.CircleNum = 0;//��ǰ��������
    //
    Circle.CircleAmount = 2;//��������
    Circle.DontELCJudgeDis = 1.0;//����������б𻷵�����
    Circle.CircleELC14 = 140;//ELC14�б𻷵�
    Circle.CircleELC23 = 0;
    Circle.FoundOutSpeed = 240;
    //Ramp
    Ramp.FindFlag = 'F';
    Ramp.MayRampGyroYMax = 100;
    Ramp.MayRampDis = 0;
    Ramp.MayRampFlag = 'F';
    Ramp.MayRampFlagClearDis = 0.3;
    Ramp.UpRampDis = 0;
    Ramp.RampLegth = 1.0;
    Ramp.State = EndRamp;
    Ramp.RecoverDis = 1.0;
    Ramp.RecoverFlag = 'F';


    Ramp.DontJudgeRampDis = 0;
    //Cross
    Cross.FindFlag = 'F';
    Cross.state = CrossEnd;
    Cross.CrossLength = 0.5;
}


//������ֵ��Χ�ж�
bool InRange(int32 x,int32 _min,int32 _max)
{
    if(x <= _max && x >= _min)
        return 1;
    else
        return 0;
}


//����֮�䲹��
uint8 TwoPoint_AddingLine(uint8 x1,uint8 y1,uint8 x2,uint8 y2,int16 *p,uint8 StopYsite)
{
    int16 i = 0;
    float k = 0.000f,b = 0.000f;
    //�Ƿ���ֱ��
    if(x1 == x2)
    {
        if(StopYsite >= max_ab(y1,y2))//���ϲ���
        {
            for(i = min_ab(y1,y2);i < StopYsite;i ++)
            {
                *p = x1;
                p ++;
            }
        }
        else if(StopYsite > min_ab(y1,y2))//��
        {
            for(i = max_ab(y1,y2);i > StopYsite;i --)
            {
                *p = x1;
                p --;
            }
        }
        else if (StopYsite == 0) {
            for (i = min_ab(y1,y2); i <= max_ab(y1,y2); i++) {
                *p = x1;
                p++;
            }
        }
        return 1;
    }
    else//����K����
    {
        //����ֱ�� y = kx + b
        k = (float)(y2 - y1) / (float)(x2 - x1);
        b = (float)(y1 - k*x1);
        if(StopYsite >= max_ab(y1,y2))//���ϲ���
        {
            for(i = min_ab(y1,y2);i < StopYsite;i ++)
            {
                *p = (uint8)((float)(i - b) / k);
                p ++;
            }
        }
        else if(StopYsite > min_ab(y1,y2))//��
        {
            for(i = max_ab(y1,y2);i > StopYsite;i --)
            {
                *p = (uint8)((float)(i - b) / k);
                p --;
            }
        }
        else if(StopYsite == 0)//ֱ�ӽ������㲹�� �����Ǵ�������
        {
            for(i = min_ab(y1,y2);i < max_ab(y1,y2);i ++)
            {
                *p = (uint8)((float)(i - b) / k);
                p ++;
            }
        }
        return 1;
    }
}





//��Ե���� ͳ�Ʋ�ͬ�еı߽�״̬
void LineBorderStateCount(void)
{
    //���Ҿ��б���
    if(Img.LeftBorderFindFlag[Ysite] == 'T' && Img.RightBorderFindFlag[Ysite] == 'T')
    {
        LineALL.AllNormal ++;
        LineALL.LeftNormal ++;
        LineALL.RightNormal ++;
        if(Ysite <= 35)
        {
            Line5_35.AllNormal ++;
            Line5_35.LeftNormal ++;
            Line5_35.RightNormal ++;
        }
        if(Ysite > 30)
        {
            Line30_60.AllNormal ++;
            Line30_60.LeftNormal ++;
            Line30_60.RightNormal ++;
        }
    }
    //������ұ߶�ʧ
    if(Img.LeftBorderFindFlag[Ysite] == 'T' && Img.RightBorderFindFlag[Ysite] == 'F')
    {
        LineALL.LeftNormalRightLose ++;
        LineALL.LeftNormal ++;
        LineALL.RightLose ++;
        if(Ysite <= 35)
        {
            Line5_35.LeftNormalRightLose ++;
            Line5_35.LeftNormal ++;
            Line5_35.RightLose ++;
        }
        if(Ysite > 30)
        {
            Line30_60.LeftNormalRightLose ++;
            Line30_60.LeftNormal ++;
            Line30_60.RightLose ++;
        }
    }
    //��߶�ʧ�ұ���
    if(Img.LeftBorderFindFlag[Ysite] == 'F' && Img.RightBorderFindFlag[Ysite] == 'T')
    {
        LineALL.LeftLoseRightNormal ++;
        LineALL.LeftLose ++;
        LineALL.RightNormal ++;
        if(Ysite <= 35)
        {
            Line5_35.LeftLoseRightNormal ++;
            Line5_35.LeftLose ++;
            Line5_35.RightNormal ++;
        }
        if(Ysite > 30)
        {
            Line30_60.LeftLoseRightNormal ++;
            Line30_60.LeftLose ++;
            Line30_60.RightNormal ++;
        }
    }
    //����ʧ
    if(Img.LeftBorderFindFlag[Ysite] == 'F' && Img.RightBorderFindFlag[Ysite] == 'F')
    {
        LineALL.AllLose ++;
        LineALL.LeftLose ++;
        LineALL.RightLose ++;
        if(Ysite <= 35)
        {
            Line5_35.AllLose ++;
            Line5_35.LeftLose ++;
            Line5_35.RightLose ++;
        }
        if(Ysite > 30)
        {
            Line30_60.AllLose ++;
            Line30_60.LeftLose ++;
            Line30_60.RightLose ++;
        }
    }
}


//--------------------------------------------ͼ���������--------------------------------------------//


//��¼ÿ�еĵ�һ����Ե��Ϊ��ǰ�б�Ե
//var
int16 StartColSum = 0;
int16 StartColIndex = 0;

uint8 GetBorder(void)
{
    //��ʼ��ɨ�߱�־var
    StartColSum = 0;
    StartColIndex = 0;

    //-------------------Line---------------------//
    LineStatusClear(&LineALL);
    LineStatusClear(&Line5_35);
    LineStatusClear(&Line30_60);

    //-------------------Base---------------------//
    Base.StartSearchColumn = 0;
    Base.firstleftnormal.y = 0;
    Base.firstleftnormal.x = 0;
    Base.firstrightnormal.y = 0;
    Base.firstrightnormal.x = 0;
    Base.firstleftlose.y = 0;
    Base.firstleftlose.x = 0;
    Base.firstrightlose.y = 0;
    Base.firstrightlose.x = 0;
    Base.ToplineTemp = 0;
    Base.ToplineFindFlag = 'F';
    Base.firstroadnormal = 0;
    Base.firstroaderror = 0;

    #if ImgDubug
    Base.LeftMax.x = 0;
    Base.LeftMin.x = COL - 1;
    Base.RightMax.x = 0;
    Base.RightMin.x = COL - 1;
    Base.CenterMax.x = 0;
    Base.CenterMin.x = COL - 1;
    Base.CenterErrorCnt = 0;
    Base.LeftMax20up.x = 0;
    Base.LeftMin20up.x = COL - 1;
    Base.RightMax20up.x = 0;
    Base.RightMin20up.x = COL - 1;


    //����0��127����
    Column10.firstblackYFlag = 'F';
    Column10.firstwhiteYFlag = 'F';
    Column10.firstblackY20upFlag = 'F';
    Column10.firstwhiteY20upFlag = 'F';
    Column10.Line30_60_White = 0;
    Column10.Line0_30_Black = 0;
    Column10.Line0_30_White = 0;
    Column10.Line30_60_Black = 0;
    Column117.firstblackYFlag = 'F';
    Column117.firstwhiteYFlag = 'F';
    Column117.firstblackY20upFlag = 'F';
    Column117.firstwhiteY20upFlag = 'F';
    Column117.Line30_60_White = 0;
    Column117.Line0_30_Black = 0;
    Column117.Line0_30_White = 0;
    Column117.Line30_60_Black = 0;
    Column64.firstblackYFlag = 'F';
    Column64.firstwhiteYFlag = 'F';
    Column64.firstblackY20upFlag = 'F';
    Column64.firstwhiteY20upFlag = 'F';
    Column64.Line30_60_White = 0;
    Column64.Line0_30_Black = 0;
    Column64.Line0_30_White = 0;
    Column64.Line30_60_Black = 0;
    Row56.LeftToRightFirstBlack = 0;
    Row56.LeftToRightFirstWhite = 0;
    Row56.RightToLeftFirstBlack = 127;
    Row56.RightToLeftFirstWhite = 127;
    Row56.LeftToRightFirstBlackFindFlag = 'F';
    Row56.RightToLeftFirstBlackFindFlag = 'F';
    Row56.LeftToRightFirstWhiteFindFlag = 'F';
    Row56.RightToLeftFirstWhiteFindFlag = 'F';
    #endif
    //------------------------Ԫ�����־-------------------------//
    //ֱ��
    Straight.LeftCnt = 0;
    Straight.LeftCnt40up = 0;
    Straight.RightCnt = 0;
    Straight.RightCnt40up = 0;
    //���
    Curve.LB_TR_Cnt = 0;
    Curve.RB_TL_Cnt = 0;
    //����
    Fork.PeakFindFlag = 'F';
    Fork.CenterLineBlack = 0;
    Fork.MaybeCorssFlag = 'F';
    //����

    //����

    //-------------------------�ŷ�����------------------------//
    #if ImgDubug
    //����010��,107��״̬
    for(Ysite = 0;Ysite < ROW;Ysite ++)
    {
        if(InRange(Ysite,3,ROW-3))//3~ROW-3��
        {
            //10��
            if(ImageUsed[ROW - 1 - Ysite     ][10] == 0 && \
                ImageUsed[ROW - 1 - Ysite - 1][10] == 0 && \
                ImageUsed[ROW - 1 - Ysite - 2][10] == 0 && \
                Column10.firstblackYFlag == 'F')
            {
                Column10.firstblackY = Ysite;
                Column10.firstblackYFlag = 'T';
            }
            if(ImageUsed[ROW - 1 - Ysite     ][10] == 255 && \
                ImageUsed[ROW - 1 - Ysite - 1][10] == 255 && \
                ImageUsed[ROW - 1 - Ysite - 2][10] == 255 && \
                Column10.firstwhiteYFlag == 'F')
            {
                Column10.firstwhiteY = Ysite;
                Column10.firstwhiteYFlag = 'T';
            }
            //117��
            if(ImageUsed[ROW - 1 - Ysite     ][117] == 0 && \
                ImageUsed[ROW - 1 - Ysite - 1][117] == 0 && \
                ImageUsed[ROW - 1 - Ysite - 2][117] == 0 && \
                Column117.firstblackYFlag == 'F')
            {
                Column117.firstblackY = Ysite;
                Column117.firstblackYFlag = 'T';
            }
            if(ImageUsed[ROW - 1 - Ysite     ][117] == 255 && \
                ImageUsed[ROW - 1 - Ysite - 1][117] == 255 && \
                ImageUsed[ROW - 1 - Ysite - 2][117] == 255 && \
                Column117.firstwhiteYFlag == 'F')
            {
                Column117.firstwhiteY = Ysite;
                Column117.firstwhiteYFlag = 'T';
            }
            //64��
            if(ImageUsed[ROW - 1 - Ysite     ][64] == 0 && \
                ImageUsed[ROW - 1 - Ysite - 1][64] == 0 && \
                ImageUsed[ROW - 1 - Ysite - 2][64] == 0 && \
                Column64.firstblackYFlag == 'F')
            {
                Column64.firstblackY = Ysite;
                Column64.firstblackYFlag = 'T';
            }
            if(ImageUsed[ROW - 1 - Ysite     ][64] == 255 && \
                ImageUsed[ROW - 1 - Ysite - 1][64] == 255 && \
                ImageUsed[ROW - 1 - Ysite - 2][64] == 255 && \
                Column64.firstwhiteYFlag == 'F')
            {
                Column64.firstwhiteY = Ysite;
                Column64.firstwhiteYFlag = 'T';
            }
        }
        if(InRange(Ysite,23,ROW-3))//23~ROW-3��
        {
            //10��
            if(ImageUsed[ROW - 1 - Ysite     ][10] == 0 && \
                ImageUsed[ROW - 1 - Ysite - 1][10] == 0 && \
                Column10.firstblackY20upFlag == 'F')
            {
                Column10.firstblackY20up = Ysite;
                Column10.firstblackY20upFlag = 'T';
            }
            if(ImageUsed[ROW - 1 - Ysite     ][10] == 255 && \
                ImageUsed[ROW - 1 - Ysite - 1][10] == 255 && \
                Column10.firstwhiteY20upFlag == 'F')
            {
                Column10.firstwhiteY20up = Ysite;
                Column10.firstwhiteY20upFlag = 'T';
            }
            //107��
            if(ImageUsed[ROW - 1 - Ysite     ][117] == 0 && \
                ImageUsed[ROW - 1 - Ysite - 1][117] == 0 && \
                Column117.firstblackY20upFlag == 'F')
            {
                Column117.firstblackY20up = Ysite;
                Column117.firstblackY20upFlag = 'T';
            }
            if(ImageUsed[ROW - 1 - Ysite     ][117] == 255 && \
                ImageUsed[ROW - 1 - Ysite - 1][117] == 255 && \
                Column117.firstwhiteY20upFlag == 'F')
            {
                Column117.firstwhiteY20up = Ysite;
                Column117.firstwhiteY20upFlag = 'T';
            }
            //64��
            if(ImageUsed[ROW - 1 - Ysite     ][64] == 0 && \
                ImageUsed[ROW - 1 - Ysite - 1][64] == 0 && \
                Column64.firstblackY20upFlag == 'F')
            {
                Column64.firstblackY20up = Ysite;
                Column64.firstblackY20upFlag = 'T';
            }
            if(ImageUsed[ROW - 1 - Ysite     ][64] == 255 && \
                ImageUsed[ROW - 1 - Ysite - 1][64] == 255 && \
                Column64.firstwhiteY20upFlag == 'F')
            {
                Column64.firstwhiteY20up = Ysite;
                Column64.firstwhiteY20upFlag = 'T';
            }
        }
        if(Ysite < 30)
        {
            //10��
            if(ImageUsed[ROW - 1 - Ysite][10] == 0)
                Column10.Line0_30_Black ++;
            else
                Column10.Line0_30_White ++;
            //107��
            if(ImageUsed[ROW - 1 - Ysite][117] == 0)
                Column117.Line0_30_Black ++;
            else
                Column117.Line0_30_White ++;
            //64��
            if(ImageUsed[ROW - 1 - Ysite][64] == 0)
                Column64.Line0_30_Black ++;
            else
                Column64.Line0_30_White ++;
        }
        if(Ysite >= 30)
        {
            //10��
            if(ImageUsed[ROW - 1 - Ysite][10] == 0)
                Column10.Line30_60_Black ++;
            else
                Column10.Line30_60_White ++;
            //107��
            if(ImageUsed[ROW - 1 - Ysite][117] == 0)
                Column117.Line30_60_Black ++;
            else
                Column117.Line30_60_White ++;
            //64��
            if(ImageUsed[ROW - 1 - Ysite][64] == 0)
                Column64.Line30_60_Black ++;
            else
                Column64.Line30_60_White ++;
        }
    }
    //����56��״̬
    //��������ɨ�ߺڰ���ʼ��
    for(Xsite = 0;Xsite < 127; Xsite ++)
    {
        if(ImageUsed[ROW - 1 - 56][Xsite] == 0 && Row56.LeftToRightFirstBlackFindFlag == 'F')
        {
            Row56.LeftToRightFirstBlack = Xsite;
            Row56.LeftToRightFirstBlackFindFlag = 'T';
        }
        if(ImageUsed[ROW - 1 - 56][Xsite] == 255 && Row56.LeftToRightFirstWhiteFindFlag == 'F')
        {
            Row56.LeftToRightFirstWhite = Xsite;
            Row56.LeftToRightFirstWhiteFindFlag = 'T';
        }
        if(Row56.LeftToRightFirstBlackFindFlag == 'T' && Row56.LeftToRightFirstWhiteFindFlag == 'T')
            break;
    }
    //��������ɨ�ߺڰ���ʼ��
    for(Xsite = 127;Xsite > 0; Xsite --)
    {
        if(ImageUsed[ROW - 1 - 56][Xsite] == 0 && Row56.RightToLeftFirstBlackFindFlag == 'F')
        {
            Row56.RightToLeftFirstBlack = Xsite;
            Row56.RightToLeftFirstBlackFindFlag = 'T';
        }
        if(ImageUsed[ROW - 1 - 56][Xsite] == 255 && Row56.RightToLeftFirstWhiteFindFlag == 'F')
        {
            Row56.RightToLeftFirstWhite = Xsite;
            Row56.RightToLeftFirstWhiteFindFlag = 'T';
        }
        if(Row56.RightToLeftFirstBlackFindFlag == 'T' && Row56.RightToLeftFirstWhiteFindFlag == 'T')
            break;
    }
    #endif
    //��ȡ������ʼ��
    Ysite = StartScanRow;
    for(Xsite = CenterCol - RoadWide0/2  - 5 ; Xsite < CenterCol + RoadWide0/2 + 5;Xsite ++)
    {
        if(ImageUsed[ROW - 1 - Ysite][Xsite] == 255)
        {
            StartColSum += Xsite;
            StartColIndex ++;
        }
    }
    Base.StartSearchColumn_1st = StartColSum / StartColIndex;
    if(Base.StartSearchColumn_1st <= 0) Base.StartSearchColumn_1st = CenterCol;//ʹ���ӹ���ת��
    Base.StartSearchColumn = Base.StartSearchColumn_1st;    //������ʼ������
    //���ռ̳еķ�ʽ���м�����������
    for(Ysite = StartScanRow;Ysite < ROW;Ysite ++)
    {
        //ɨ�����
        for(Xsite = Base.StartSearchColumn;Xsite >= 0;Xsite --)
        {
            if(ImageUsed[ROW - 1 - Ysite][Xsite] == BlackPoint && \
               !(Xsite == 0 || ImageUsed[ROW - 1 - Ysite][Xsite] == GrayPoint))//������������ʼ��
            {
                Img.LeftBorder[Ysite] = Xsite + 1;
                Img.LeftBorderFindFlag[Ysite] = 'T';
                if(Base.firstleftnormal.y == 0)
                {
                    Base.firstleftnormal.y = Ysite;
                    Base.firstleftnormal.x = Xsite + 1;
                }
                break;
            }
            else if(Xsite == 0 || ImageUsed[ROW - 1 - Ysite][Xsite] == GrayPoint)
            {
                Img.LeftBorder[Ysite] = 0;
                Img.LeftBorderFindFlag[Ysite] = 'F';
                if(Base.firstleftlose.y == 0 && Base.firstleftnormal.y != 0)//����������ʧ��
                {
                    Base.firstleftlose.y = Ysite;
                    Base.firstleftlose.x = Xsite;
                }
            }
            if(Ysite >= 3 && Img.LeftBorderFindFlag[Ysite] == 'T' && \
               Img.LeftBorderFindFlag[Ysite-1] == 'T' && \
               Img.LeftBorderFindFlag[Ysite-2] == 'T')
                Img.LeftSlope[Ysite] = ((float)(Img.LeftBorder[Ysite]) - (float)(Img.LeftBorder[Ysite - 3]))/3;//����б��
            else if(Ysite <3 && Img.LeftBorderFindFlag[Ysite] == 'T')
                Img.LeftSlope[Ysite] = 0.00f ;
            else
                Img.LeftSlope[Ysite] = NoneSlope;
        }
        //ɨ�ұ���
        for(Xsite = Base.StartSearchColumn;Xsite < COL;Xsite ++)
        {
            if(ImageUsed[ROW - 1 - Ysite][Xsite] == BlackPoint && \
               !(Xsite == (COL - 1) || ImageUsed[ROW - 1 - Ysite][Xsite] == GrayPoint))//ɨ�������Ҳ��ӽ���Ե
            {
                Img.RightBorder[Ysite] = Xsite - 1;
                Img.RightBorderFindFlag[Ysite] = 'T';
                if(Base.firstrightnormal.y == 0)
                {
                    Base.firstrightnormal.y = Ysite;
                    Base.firstrightnormal.x = Xsite - 1;
                }
                break;
            }
            else if(Xsite == (COL - 1) || ImageUsed[ROW - 1 - Ysite][Xsite] == GrayPoint)
            {
                Img.RightBorder[Ysite] = COL - 1;
                Img.RightBorderFindFlag[Ysite] = 'F';
                if(Base.firstrightlose.y == 0 && Base.firstrightnormal.y != 0)
                {
                    Base.firstrightlose.y = Ysite;
                    Base.firstrightlose.x = Xsite;
                }
            }
            if(Ysite >= 3 && Img.RightBorderFindFlag[Ysite] == 'T' && \
               Img.RightBorderFindFlag[Ysite-1] == 'T' && \
               Img.RightBorderFindFlag[Ysite-2] == 'T')
                Img.RightSlope[Ysite] = ((float)(Img.RightBorder[Ysite-3]) - (float)(Img.RightBorder[Ysite]))/3;//����б��
            else if(Ysite <3 && Img.RightBorderFindFlag[Ysite] == 'T')
                Img.RightSlope[Ysite] = 0.00f ;
            else
                Img.RightSlope[Ysite] = NoneSlope;
        }
        //����ɨ����ʼ��
        Base.StartSearchColumn = (Img.LeftBorder[Ysite] + Img.RightBorder[Ysite]) / 2;
        //����������ȶ����־
        Img.RoadWide[Ysite] = Img.RightBorder[Ysite] - Img.LeftBorder[Ysite];
        Img.RoadWideError[Ysite] = Img.RoadWide[Ysite] - RoadWide0;
        if(InRange(Img.RoadWide[Ysite],RoadWide0 - 5,RoadWide0 + 5))
            Img.RoadNormalFlag[Ysite] = 'T';
        else
            Img.RoadNormalFlag[Ysite] = 'F';
        if(Img.RoadNormalFlag[Ysite] == 'T' && \
            //Img.RoadNormalFlag[Ysite + 1] == 'T' && ��
            //Img.RoadNormalFlag[Ysite + 2] == 'T' && ��
            Base.firstroadnormal == 0 && Ysite >= StartScanRow )
            Base.firstroadnormal = Ysite;
        if(Img.RoadNormalFlag[Ysite] == 'F' && \
//            Img.RoadNormalFlag[Ysite + 1] == 'F' && ��
//            Img.RoadNormalFlag[Ysite + 2] == 'F' && ��
            Base.firstroaderror == 0 && Ysite >= StartScanRow)
            Base.firstroaderror = Ysite;
        //���¶���
        //��Ч�н�ֹ ��Ҫʵ�ز���
        if(Img.RoadWide[Ysite] < RoadWide0/*Img.RoadWide[Ysite] > 10 +RoadWide0*/ && Base.ToplineFindFlag == 'F')
        {
            Base.ToplineTemp = Ysite;
            Base.ToplineFindFlag = 'T';
        }
        if(Ysite == ROW - 1 && Base.ToplineFindFlag == 'F')
        {
            Base.ToplineTemp = ROW - 1;
        }
        #if ImgDubug
        //�������߼��� ȫ��
        if(Img.LeftBorder[Ysite] >= Base.LeftMax.x && Img.LeftBorder[Ysite] > 1)
        {
            Base.LeftMax.x = Img.LeftBorder[Ysite];
            Base.LeftMax.y = Ysite;
        }
        if(Img.LeftBorder[Ysite] <= Base.LeftMin.x && Img.LeftBorder[Ysite] > 1)
        {
            Base.LeftMin.x = Img.LeftBorder[Ysite];
            Base.LeftMin.y = Ysite;
        }
        if(Img.RightBorder[Ysite] >= Base.RightMax.x && Img.RightBorder[Ysite] < 158)
        {
            Base.RightMax.x = Img.RightBorder[Ysite];
            Base.RightMax.y = Ysite;
        }
        if(Img.RightBorder[Ysite] <= Base.RightMin.x && Img.RightBorder[Ysite] < 158)
        {
            Base.RightMin.x = Img.RightBorder[Ysite];
            Base.RightMin.y = Ysite;
        }
        if(Img.Center[Ysite] >= Base.CenterMax.x )
        {
            Base.CenterMax.x = Img.Center[Ysite];
            Base.CenterMax.y = Ysite;
        }
        if(Img.Center[Ysite] <= Base.CenterMin.x)
        {
            Base.CenterMin.x = Img.Center[Ysite];
            Base.CenterMin.y = Ysite;
        }
        //���¼��� 20������
        if(Ysite > 20)
        {
            if(Img.LeftBorder[Ysite] >= Base.LeftMax20up.x && Img.LeftBorder[Ysite] > 1)
            {
                Base.LeftMax20up.x = Img.LeftBorder[Ysite];
                Base.LeftMax20up.y = Ysite;
            }
            if(Img.LeftBorder[Ysite] <= Base.LeftMin20up.x && Img.LeftBorder[Ysite] > 1)
            {
                Base.LeftMin20up.x = Img.LeftBorder[Ysite];
                Base.LeftMin20up.y = Ysite;
            }
            if(Img.RightBorder[Ysite] >= Base.RightMax20up.x && Img.RightBorder[Ysite] < 158 )
            {
                Base.RightMax20up.x = Img.RightBorder[Ysite];
                Base.RightMax20up.y = Ysite;
            }
            if(Img.RightBorder[Ysite] <= Base.RightMin20up.x && Img.RightBorder[Ysite] < 158)
            {
                Base.RightMin20up.x = Img.RightBorder[Ysite];
                Base.RightMin20up.y = Ysite;
            }
        }
        #endif
        //��������
        if(Ysite < StartScanRow)
            Img.Center[Ysite] = CenterCol;//�ײ�����ֵ����ƽ��

        else
        {
            //�˴�Ӧ���ж��߽�����߽��д���
            if(Img.RightBorder[Ysite] !=COL-1&&Img.LeftBorder[Ysite]!=0)
                Img.Center[Ysite] = (Img.LeftBorder[Ysite] + Img.RightBorder[Ysite]) / 2;
            else if( Img.RightBorder[Ysite] ==COL-1&&Img.LeftBorder[Ysite]==0)
                Img.Center[Ysite] = CenterCol;
            else if(Img.RightBorder[Ysite] ==COL-1)
                Img.Center[Ysite] =Img.LeftBorder[Ysite]+RoadWide0/2;
            else
                Img.Center[Ysite] =Img.RightBorder[Ysite]-RoadWide0/2;
        }
        Img.Center_Error[Ysite] = myabs(CenterCol - Img.Center[Ysite]);
        //��������
        Img.LeftBorder_Repair[Ysite] = Img.LeftBorder[Ysite];
        Img.RightBorder_Repair[Ysite] = Img.RightBorder[Ysite];
        Img.Center_Repair[Ysite] = Img.Center[Ysite];
        //��Ե״̬����
        LineBorderStateCount();
    }
    //---------------------------����ɨ�߽�ֹ-----------------------------//
    Base.Topline = Base.ToplineTemp;//���ն��߸���
    ElementProcessing();//Ԫ���жϺʹ���
    //ret
    return 1;
}

/**
* @brief    Ԫ���жϺʹ���
* @param    None
* @return   0
* @Sample   ElementProcessing();
**/
uint8 ElementProcessing(void)
{
    //---------------------------���������жϿ�ʼ-----------------------------//
    if(Base.element_check == 'T')
    {
        StrightJudge();
        CurveJudge();
//      if(Fork.FindFlag == 'F' && Circle.FindFlag == 'F' && Ramp.FindFlag == 'F')
//          Barn_Judge();//�����ж�
        if(CarInfo.UpTime >= 0.300f)
        {
          if(Fork.FindFlag == 'F' && Circle.FindFlag == 'F' && \
              Barn.FindFlag == 'F' && Ramp.FindFlag == 'F')
              CrsosJudge();
//          if(Barn.FindFlag == 'F' && Circle.FindFlag == 'F' && ��
//              Ramp.FindFlag == 'F' && Cross.FindFlag == 'F' && ��
//              CarInfo.UpTime >= 1.000f )
//              Fork_Judge();
//            if(Fork.FindFlag == 'F' && Barn.FindFlag == 'F' && Ramp.FindFlag == 'F')
//              CircleJudge();
//          if(Fork.FindFlag == 'F' && Circle.FindFlag == 'F' && ��
//              Barn.FindFlag == 'F' && Cross.FindFlag == 'F')
//              RampJudge();//ע�⣺�µ���ͼ��ʶ���趨��ImgDebugΪ1��ͼ��ʶ��Ч�����ã�����ʹ�ü�������д�µ�ʶ��
        }
    }
    //---------------------------���������жϽ���-----------------------------//
    //--------------------------------���߿�ʼ--------------------------------//
    if(Cross.FindFlag == 'T')
        CrossAddingLine();
    if(Fork.FindFlag == 'T')
        ForkAddingLine();
    if(Barn.FindFlag == 'T')
        BarnAddingLine();
    if(Circle.FindFlag == 'T')
        CircleAddingLine();

    for(Ysite = 5;Ysite < Base.Topline;Ysite ++)
    {
        // �޸�������ߺͱ����޷�
        LIMIT_COL(Img.Center_Repair[Ysite],127,0);
        LIMIT_COL(Img.LeftBorder_Repair[Ysite],127,0);
        LIMIT_COL(Img.RightBorder_Repair[Ysite],127,0);
    }
    //--------------------------------���߽���--------------------------------//
    return 0 ;
}



//------------------------------------------ֱ��----------------------------------------//
StraightTypedef Straight;


//ֱ��ʶ��
uint8 StrightJudge(void)
{
    //ͳһ�����־
    if(Base.Topline >= 40)
    {
        for(Ysite = 0;Ysite < Base.Topline;Ysite ++)
        {
            if(Img.LeftSlope[Ysite] > 0.6f && Img.LeftSlope[Ysite] < 1.7f && \
                Img.LeftBorder[Ysite] >= Img.LeftBorder[Ysite - 1] && \
                Img.LeftBorderFindFlag[Ysite] == 'T' )
            {
                Straight.LeftCnt ++;
                if(Ysite > 40)
                    Straight.LeftCnt40up ++;
            }
            if(Img.RightSlope[Ysite] > 0.6f && Img.RightSlope[Ysite] < 1.7f && \
                Img.RightBorder[Ysite] <= Img.RightBorder[Ysite - 1])
            {
                Straight.RightCnt ++;
                if(Ysite > 40)
                    Straight.RightCnt40up ++;
            }
        }
    }
    else
        return 0;
    //�ж�
    if(Base.Topline >= 55 && Straight.LeftCnt40up > 12 && Straight.LeftCnt > 15)
        Straight.LeftLong = 'T';
    else
        Straight.LeftLong = 'F';
    if(Base.Topline >= 55 && Straight.RightCnt40up > 12 && Straight.RightCnt > 15)
        Straight.RightLong = 'T';
    else
        Straight.RightLong = 'F';
    if(Base.Topline >= 40 && Straight.LeftCnt > 25 && Straight.LeftLong == 'F')
        Straight.LeftShort = 'T';
    else
        Straight.LeftShort = 'F';
    if(Base.Topline >= 40 && Straight.RightCnt > 25 && Straight.RightLong == 'F')
        Straight.RightShort = 'T';
    else
        Straight.RightShort = 'F';
    //��־��λ
    if(Straight.LeftLong == 'T' || Straight.RightLong == 'T' )
    {
        Straight.FindFlag = 'T';
        if(Straight.Beep == 'T')
            BeepTick(1,100);
    }
    if(Straight.LeftShort == 'T' || Straight.RightShort == 'T' )
    {
        //Straight.FindFlag = 'T';//��ֱ����־������λ
        if(Straight.Beep == 'T')
            BeepTick(1,100);
    }
    else
        Straight.FindFlag = 'F';

    return 1;
}


//--------------------------------------------���------------------------------------------//
CurveTypedef    Curve;


//�ж�
uint8 CurveJudge(void)
{
    //ͳһ�����־
    for(Ysite = 0;Ysite < Base.Topline;Ysite ++)
    {
        //������ƫ
        if(Img.LeftSlope[Ysite] > 1.7f &&  \
            Img.LeftBorder[Ysite] >= Img.LeftBorder[Ysite - 1] && \
            Img.LeftBorderFindFlag[Ysite] == 'T' )
        {
            Curve.LB_TR_Cnt ++;
        }
        //������ƫ
        if(Img.RightSlope[Ysite] > 1.7f  && \
            Img.RightBorder[Ysite] <= Img.RightBorder[Ysite - 1] && \
            Img.RightBorderFindFlag[Ysite] == 'T')
        {
            Curve.RB_TL_Cnt ++;
        }
    }
    //�ж�
    if(Curve.LB_TR_Cnt >= 10)
        Curve.TurnRight = 'T';
    else
        Curve.TurnRight = 'F';
    if(Curve.RB_TL_Cnt >= 10)
        Curve.TurnLeft = 'T';
    else
        Curve.TurnLeft = 'F';
    if(Curve.TurnLeft == 'T' || Curve.TurnRight == 'T')
    {
        Curve.FindFlag = 'T';
        if(Curve.Beep == 'T')
            BeepTick(1,100);
    }
    else
        Curve.FindFlag = 'F';
    if(Curve.TurnRight == 'T')//������
    {



    }
    else if (Curve.TurnLeft == 'T')//������
    {



    }
    return 1;
}


//------------------------------------------����----------------------------------------/
ForkTypedef  Fork;


//Ѱ���Ϲյ� ���������᷵��1 �Ҳ����Ϲյ㷵��0
uint8 Fork_Find_Peak(void)
{
    uint8 xPoint0 = 64;
    //Ѱ������Ķ������ұ�Ե
    //���������Ѻ���
    for(Ysite = min_ab(Column64.firstblackY + 10,55);Ysite >= 5;Ysite --)
    {
        for(Xsite = xPoint0;Xsite > 0;Xsite --)
        {
            if(Xsite == 1)
            {
                Fork.ForkLeftBorder[Ysite] = Xsite;
                break;
            }
            if(ImageUsed[59 -Ysite][Xsite] == 255 )
            {
                Fork.ForkLeftBorder[Ysite] = Xsite - 1;
                break;
            }
        }
        for(Xsite = xPoint0;Xsite < 127;Xsite ++)
        {
            if(Xsite == 126)
            {
                Fork.ForkRightBorder[Ysite] = Xsite;
                break;
            }
            if(ImageUsed[59 -Ysite][Xsite] == 255)
            {
                Fork.ForkRightBorder[Ysite] = Xsite + 1;
                break;
            }
        }
        xPoint0 = (Fork.ForkLeftBorder[Ysite] + Fork.ForkRightBorder[Ysite])/2;
        if((Fork.ForkRightBorder[Ysite] - Fork.ForkLeftBorder[Ysite]) <= 2)
        {
            Fork.StartLine = Ysite;
            Fork.Peak.y = Ysite;
            xPoint0 = limit(59,69);
            Fork.Peak.x = xPoint0;
            Fork.PeakFindFlag = 'T';
            break;
        }
    }
    return Fork.StartLine;
}


//�������ߺ�ɫ���ֵĳ���
uint8 Ret_BlackCenterLine_Length(void)
{
    uint8 Cntx0 = 0;
    for(Ysite = 5;Ysite < 60;Ysite ++)
    {
        if(ImageUsed[59 - Ysite][64] == 0)
         Cntx0 ++;
        if(Cntx0 >= 10)
            break;
    }
    Fork.CenterLineBlack = Cntx0;
    return Cntx0;
}


//���ʮ������ ��30���������������߱�Ե ������ڱ�Ե������
uint8 Fork_Avoid_Cross(void)
{
    uint8 VaildCntColumn = 0;
    uint8 ColumnVaildX[128];
    for(Xsite = 34;Xsite < 94;Xsite ++)
    {
        for(Ysite = 30;Ysite < 55;Ysite ++)
        {
            if(ImageUsed[59 - Ysite][Xsite] == 0)
                break;
        }
        if(Ysite >= 55)
        {
            ColumnVaildX[Xsite] = 'T';
            if(ColumnVaildX[Xsite] == 'T')
                VaildCntColumn ++;
        }
        else
            ColumnVaildX[Xsite] = 'F';
//      if(VaildCntColumn >= 15)
//          break;
    }
    if(VaildCntColumn < 8)
    {
        return 1;
    }
    else
    {
        Fork.MaybeCorssFlag = 'T';
        return 0;
    }
}


//��������� ���󷢳�ʱ
uint32 ForkInfo[4][3] =
{
    /*  ����    ת������*10    ת������*10 */
    {   'L'   ,      35      ,      32     },
    {   'R'   ,      35      ,      32     },
};


//�ж�
//����
uint8 ForkJudge(void)
{
    //������
    if(Fork.FindFlag == 'F' && Fork_Find_Peak() != 0 &&\
        LineALL.LeftLose >= 25 && LineALL.RightLose >= 25 && \
        Fork_Avoid_Cross() == 1 && Ret_BlackCenterLine_Length() >= 3 &&
        InRange(ElcSum_14,20,80) && \
        myabs(Column10.Line30_60_Black - Column117.Line30_60_Black) < 12 && \
        Column64.firstblackY < max_ab(Column10.firstblackY,Column117.firstblackY) && \
        myabs(Column117.firstblackY20up - Column10.firstblackY20up) < 7)//���߿�ʼ�ջ�ģʽ�л�Ϊת��ջ�
    {
        CarInfo.ClosedLoopMode = DirectLoop;
        BeepTick(1,100);
        Fork.FindFlag = 'T';
        Fork.state = ForkInTurn;
        ICMIntegrate.Yaw_I_Enable = 'T';
        ICMIntegrate.TurnRight_I = 0.000f;
        ICMIntegrate.TurnLeft_I = 0.000f;
        //��¼ ��ȡ
        Fork.ForkNum ++;
        if(Barn.OutDir == 'L')
        {
            Fork.In_Direct = ForkInfo[Fork.ForkNum - 1][0];
            Fork.TurnInK =  ForkInfo[Fork.ForkNum - 1][1];
            Fork.TurnInK /= 10;
            Fork.TurnOutK = ForkInfo[Fork.ForkNum - 1][2];
            Fork.TurnOutK /= 10;
        }
        else
        {
            Fork.In_Direct = ForkInfo[(Fork.ForkAmount - Fork.ForkNum)][0];
            Fork.TurnInK =  ForkInfo[(Fork.ForkAmount - Fork.ForkNum)][1];
            Fork.TurnInK /= 10;
            Fork.TurnOutK = ForkInfo[(Fork.ForkAmount - Fork.ForkNum)][2];
            Fork.TurnOutK /= 10;
        }
    }
    if(Fork.FindFlag == 'T' && Fork.state == ForkInTurn && \
        (ICMIntegrate.TurnRight_I > 20.0f || ICMIntegrate.TurnLeft_I > 20.0f))
    {
        Fork.state = ForkIn;
        Fork.InDis = Wheel.Distance;
        ICMIntegrate.Yaw_I_Enable = 'F';
        ICMIntegrate.TurnRight_I = 0.000f;
        ICMIntegrate.TurnLeft_I = 0.000f;
        CarInfo.ClosedLoopMode = AllLoop;
        ExSpeed = Fork.InSpeed;
    }
    //������
    if(Base.Topline <= 55 && Fork.FindFlag == 'T' && \
        LineALL.AllLose >= 20 && Fork.state == ForkIn && ElcSum_14 <= 30)//���������˶���
    {
        ExSpeed = Fork.OutSpeed;
        Fork.state = ForkOutTurn;
        BeepTick(1,100);
        ICMIntegrate.Yaw_I_Enable = 'T';
    }
    if(Fork.FindFlag == 'T' && Fork.state == ForkOutTurn && \
        (ICMIntegrate.TurnRight_I > 20.000f || ICMIntegrate.TurnLeft_I > 20.000f))
    {
        Fork.state = ForkOut;
        Fork.FindFlag = 'F';
        ICMIntegrate.Yaw_I_Enable = 'F';
        ExSpeed = NormalSpeed;
    }
    return 1;
}

void ForkAddingLine(void)
{
    //������벹�� ˫�߶�ʧ
    if(Fork.state == ForkInTurn)
    {
        if(Fork.In_Direct == 'R')
        {
            Fork_Find_Peak();
            TwoPoint_AddingLine(0,5,Fork.Peak.x,Fork.Peak.y,&Img.LeftBorder_Repair[5],0);
            for(Ysite = 5;Ysite < Fork.Peak.y;Ysite ++)
            {
                Img.Center_Repair[Ysite] = Img.LeftBorder_Repair[Ysite] + RoadWide0/2;
            }
        }
        else if(Fork.In_Direct == 'L')
        {
            Fork_Find_Peak();
            TwoPoint_AddingLine(127,5,Fork.Peak.x,Fork.Peak.y,&Img.RightBorder_Repair[5],0);
            for(Ysite = 5;Ysite < Fork.Peak.y;Ysite ++)
            {
                Img.Center_Repair[Ysite] = Img.RightBorder_Repair[Ysite] - RoadWide0/2;
            }
        }
    }
    //��������� Ҳ����ֱ�Ӳ��õ�ų����� Ȼ����ת����ͷ
    if(Fork.state == ForkOutTurn)
    {
        if(Fork.In_Direct == 'R')
        {
            Fork_Find_Peak();
            TwoPoint_AddingLine(0,5,Fork.Peak.x,Fork.Peak.y,&Img.LeftBorder_Repair[5],0);
            for(Ysite = 5;Ysite < Fork.Peak.y;Ysite ++)
            {
                Img.Center_Repair[Ysite] = Img.LeftBorder_Repair[Ysite] + RoadWide0/2;
            }
        }
        else if(Fork.In_Direct == 'L')
        {
            Fork_Find_Peak();
            TwoPoint_AddingLine(127,5,Fork.Peak.x,Fork.Peak.y,&Img.RightBorder_Repair[5],0);
            for(Ysite = 5;Ysite < Fork.Peak.y;Ysite ++)
            {
                Img.Center_Repair[Ysite] = Img.RightBorder_Repair[Ysite] - RoadWide0/2;
            }
        }
    }
}







//-------------------------------------����-------------------------------------------------//
BarnTypedef     Barn;

//����ֱ�Ӵ����ν� �ò��Ų���
//����ָ���з�Χ�ڰ����ĵ���ڶ�ֵ�������Ƿ���ڶ�ֵ
uint8 Line_Ret_WhiteBlackChange_VaildN(void)
{
    uint8 Y0_ColorChange[60];
    uint8 ValidRow = 0;
    for(Ysite = Barn.StartLineFoundY;Ysite < (Barn.StartLineFoundY + 30);Ysite ++)
    {
        Y0_ColorChange[Ysite] = 0;
        for(Xsite = (64 - RoadWide0/2);Xsite < (Ysite + RoadWide0/2);Xsite ++)
        {
            if((ImageUsed[59 - Ysite][Xsite] == 0 && ImageUsed[59 - Ysite][Xsite - 1] == 255) ||
                (ImageUsed[59 - Ysite][Xsite] == 0 && ImageUsed[59 - Ysite ][Xsite + 1] == 255) )
            Y0_ColorChange[Ysite] ++;
        }
        if(Y0_ColorChange[Ysite] >= 6)
        {
            ValidRow ++;
        }
        if(ValidRow >= 5)
            break;
    }
    if(ValidRow >= 5)
        return 1;
    else
        return 0;
}


uint8 Line_Ret_WhiteBlackChangeY(void)
{
    uint8 Y0_ColorChange[60];
    uint8 ValidRow = 0;
    for(Ysite =0;Ysite < 45;Ysite ++)
    {
        Y0_ColorChange[Ysite] = 0;
        for(Xsite = (64 - RoadWide0/2);Xsite < (Ysite + RoadWide0/2);Xsite ++)
        {
            if((ImageUsed[59 - Ysite][Xsite] == 0 && ImageUsed[59 - Ysite][Xsite - 1] == 255) ||
                (ImageUsed[59 - Ysite][Xsite] == 0 && ImageUsed[59 - Ysite ][Xsite + 1] == 255) )
            Y0_ColorChange[Ysite] ++;
        }
        if(Y0_ColorChange[Ysite] >= 6)
        {
            ValidRow ++;
        }
        if(ValidRow >= 5)
            break;
    }
    if(ValidRow >= 5)
        return 1;
    else
        return 0;
}


uint16 BarnTurnInOk(void)
{
    Ysite = 0;
    uint16 ColorChange_cnt = 0;
    for(Ysite = 0;Ysite < 20;Ysite ++)
    {
        for(Xsite = 20;Xsite < 107;Xsite ++)
        {
            if(ImageUsed[59 - Ysite][Xsite] == BLACK)
                ColorChange_cnt ++;
        }
    }
    return ColorChange_cnt;
}


//�����Ϣ
uint16 BarnInInfo[2][6] =
{
    /* �����ʶ ʶ����ʼ�� ʶ�����ʻ����cm ת�����߸�ֵ   ת���ٶ�   ת�����ܵľ���*/
    {     'L'  ,   20     ,     20         ,    100      ,   230     ,    30   },
    {     'R'  ,   20     ,     15         ,    100      ,   230     ,    30   },
};


//�жϳ���
void BarnJudge(void)
{
    //�жϳ���
    if(Barn.FindFlag == 'F' && Barn.state == BarnOutOK_Start && \
        ElcProtect_Flag == 'T' && CarInfo.UpTime <= 0.3f)
    {
        CarInfo.CAR_PROTECT_MODE = 'F';
        Barn.state = BarnOutStraight;
        Barn.FindFlag = 'T';
        BeepTick(1,100);
        CarInfo.ClosedLoopMode = LR_SingleControl;
        if(Barn.OutDir == 'L')
        {
            ExSpeed_L = 100;
            ExSpeed_R = 250;
        }
        else if(Barn.OutDir == 'R')
        {
            ExSpeed_L = 70;
            ExSpeed_R = 200;
        }
        Barn.BarnStartDistance = Wheel.Distance;
        //��ȡ�����Ϣ
        if(Barn.OutDir == 'L')
        {
            Barn.StartLineFoundY = BarnInInfo[0][1];
            Barn.BeforeTurnRunDis = BarnInInfo[0][2];
            Barn.BeforeTurnRunDis /= 100;
            Barn.BarnTurnCenter = BarnInInfo[0][3];
            Barn.BarnTurnSpeed = BarnInInfo[0][4];
            Barn.BarnInRunDis = BarnInInfo[0][5];
            Barn.BarnInRunDis /= 100;
        }
        else if(Barn.OutDir == 'R')
        {
            Barn.StartLineFoundY = BarnInInfo[1][1];
            Barn.BeforeTurnRunDis = BarnInInfo[1][2];
            Barn.BeforeTurnRunDis /= 100;
            Barn.BarnTurnCenter = BarnInInfo[1][3];
            Barn.BarnTurnSpeed = BarnInInfo[1][4];
            Barn.BarnInRunDis = BarnInInfo[1][5];
            Barn.BarnInRunDis /= 100;
        }
    }
    //�������������ֵ��������ٶ�
    if(Barn.state == BarnOutStraight && \
        ((Wheel.Distance >= (Barn.BarnStartDistance + 0.200f) && Barn.OutDir == 'R') ||
    (Wheel.Distance >= (Barn.BarnStartDistance + 0.100f) && Barn.OutDir == 'L')))
    {
        CarInfo.ClosedLoopMode = LR_SingleControl;
        if(Barn.OutDir == 'L')
        {
            ExSpeed_L = 20;
            ExSpeed_R = 250;
        }
        else if(Barn.OutDir == 'R')
        {
            ExSpeed_L = 150;
            ExSpeed_R = 60;
        }
        Barn.state = BarnOutTurn;
        ICMIntegrate.Yaw_I_Enable = 'T';
        ICMIntegrate.TurnLeft_I = 0.000f;
        ICMIntegrate.TurnRight_I = 0.000f;
    }
    //ת�����
    if(Barn.state == BarnOutTurn && \
        ((ICMIntegrate.TurnLeft_I >= 60.0f && Barn.OutDir == 'L') || \
        (ICMIntegrate.TurnRight_I >= 60.0f && Barn.OutDir == 'R')))
    {
        CarInfo.ClosedLoopMode = AllLoop;
        Barn.state = BarnOutOK;
        ExSpeed = NormalSpeed;
        Barn.FindFlag = 'F';
        CarInfo.CAR_PROTECT_MODE = 'T';
        ICMIntegrate.Yaw_I_Enable = 'F';
    }
    ///////////////////////����ж�////////////////////////////
    if(Barn.FindFlag == 'F' && Barn.state == BarnOutOK && \
        ElcSum_14 >= Barn.BarnFoundELCSum14Min )
    {
        if( Column64.Line30_60_White > 20 && \
            Row56.LeftToRightFirstWhite < 64 &&  Row56.RightToLeftFirstBlack > 64 && \
            Line_Ret_WhiteBlackChange_VaildN() == 1)
        {
            //������ʶ�� �ȽϺ�ʱ ��˷��ں��
            Barn.BarnNum ++;
            BeepTick(1,50);
            if(Barn.BarnNum == Barn.BranInNum)//��Ҫ���
            {
                Barn.state = BarnTurnStraight;
                CarInfo.CAR_PROTECT_MODE = 'F';
                Barn.FindFlag = 'T';
            }
            else//����Ҫ���
            {
                //BEEP_ON;
                Barn.FindFlag = 'T';
                Barn.state = BarnPass;
                CarInfo.ControlMode = ELCMODE;
                Barn.FindFlag = 'T';
                Barn.BarnFindDis = Wheel.Distance;
            }
        }
    }
    //����1���ͨ��
    if(Barn.FindFlag == 'T' && Barn.state == BarnPass && \
        Wheel.Distance >= (Barn.BarnLength + Barn.BarnFindDis))
    {
        Barn.FindFlag = 'F';
        CarInfo.ControlMode = CAMMODE;
        Barn.state = BarnOutOK;
        //BEEP_OFF;
        //���������־
        Circle.CircleNum = 0;
        Fork.ForkNum = 0;
    }
    //����2ͣ��
    //����ת��
    if(Barn.FindFlag == 'T' && Barn.state == BarnTurnStraight && BarnTurnInOk() <= 5 )
    {
        //BEEP_ON;
        Barn.state = BarnInTurnStraight;
        Barn.BarnInTurnDis = Wheel.Distance;
    }
    //����Сֱ��ת��ʼ
    if(Barn.FindFlag == 'T' && Barn.state == BarnInTurnStraight && \
        Wheel.Distance >= (Barn.BarnInTurnDis + Barn.BeforeTurnRunDis))
    {
        //BEEP_ON;
        ExSpeed = Barn.BarnTurnSpeed;
        Barn.state = BarnInTurn;
        ICMIntegrate.Yaw_I_Enable = 'T';
        ICMIntegrate.TurnLeft_I = 0.00f;
        ICMIntegrate.TurnRight_I = 0.00f;
    }
    //ת����ɽ��복��Сֱ��
    if(Barn.FindFlag == 'T' && Barn.state == BarnInTurn && \
        ((ICMIntegrate.TurnLeft_I >= 60.0f && Barn.OutDir == 'L') || \
        (ICMIntegrate.TurnRight_I >= 60.0f && Barn.OutDir == 'R')))
    {
        BeepTick(1,100);
        Barn.FindFlag = 'T';
        Barn.state = BarnInStraight;
        Barn.BarnInDistanceCount = Wheel.Distance;
        CarInfo.ClosedLoopMode = SpeedLoop;
        ExSpeed = Barn.BarnInStraightSpeed;
    }
    //������
    if(Barn.FindFlag == 'T' && Barn.state == BarnInStraight && \
        Wheel.Distance >= (Barn.BarnInDistanceCount + Barn.BarnInRunDis) )
    {
        Barn.state = BarnIn;
        Barn.FindFlag = 'T';
        CarInfo.CAR_PROTECT_MODE = 'T';
        CarInfo.Protect_Flag = 'T';
        //BEEP_OFF;
    }
}


//���ⲹ��
void BarnAddingLine(void)
{
    //�����ⲹ�� ���϶˵�
    if(Barn.FindFlag == 'T' && Barn.state == BarnOutTurn && Barn.OutDir == 'R')
    {
        TwoPoint_AddingLine(0,5,Base.firstrightnormal.x,Base.firstrightnormal.y,&Img.LeftBorder_Repair[5],0);
        for(Ysite = 6;Ysite < Base.firstrightnormal.y;Ysite ++)
        {
            Img.Center_Repair[Ysite] = Img.LeftBorder_Repair[Ysite] + RoadWide0/2;
        }
    }
    if(Barn.FindFlag == 'T' && Barn.state == BarnOutTurn && Barn.OutDir == 'L')
    {
        TwoPoint_AddingLine(127,5,Base.firstleftnormal.x,Base.firstleftnormal.y,&Img.RightBorder_Repair[5],0);
        for(Ysite = 6;Ysite < Base.firstleftnormal.y;Ysite ++)
        {
            Img.Center_Repair[Ysite] = Img.RightBorder_Repair[Ysite] - RoadWide0/2;
        }
    }
    //��ⲹ��
    if(Barn.FindFlag == 'T' && Barn.state == BarnInTurn && Barn.OutDir == 'L')
    {
        TwoPoint_AddingLine(127,5,0,Column10.firstblackY20up,&Img.RightBorder_Repair[5],0);
        for(Ysite = 6;Ysite < Column10.firstblackY20up;Ysite ++)
        {
            Img.Center_Repair[Ysite] = Img.RightBorder_Repair[Ysite] - RoadWide0/2;
        }
    }
    //��ⲹ��
    if(Barn.FindFlag == 'T' && Barn.state == BarnInTurn && Barn.OutDir == 'R')
    {
        TwoPoint_AddingLine(0,5,127,Column117.firstblackY20up,&Img.LeftBorder_Repair[5],0);
        for(Ysite = 6;Ysite < Base.firstrightnormal.y;Ysite ++)
        {
            Img.Center_Repair[Ysite] = Img.LeftBorder_Repair[Ysite] + RoadWide0/2;
        }
    }
}


//-------------------------------------����-------------------------------------------------//
CircleTypedef   Circle;


//�����Ҳ��ߵ��Ϲյ�
uint8 Circle_Find_LeftUp_Inflexion(void)
{
    Circle.LeftUpInflexion.y = 0;
    Circle.LeftUpInflexion.x = 0;
    //�ұ߽����¶�����������õ��ĵ�һ����
    for(Ysite = max_ab(Base.firstleftlose.y + 2,7);Ysite < 59;Ysite ++)
    {
        for(Xsite = 125;Xsite > 0;Xsite --)
        {
            if(ImageUsed[59 - Ysite][Xsite] == 255 && \
                ImageUsed[59 - Ysite][Xsite - 1] == 255 && \
                ImageUsed[59 - Ysite][Xsite - 2] == 255)
                break;
        }
        for(;Xsite > 0;Xsite --)
        {
            if(ImageUsed[59 - Ysite][Xsite] == 0)
            {
                Circle.LeftUpInflexion.y = Ysite;
                Circle.LeftUpInflexion.x = Xsite;
                break;
            }
        }
        if(Circle.LeftUpInflexion.y != 0)
            break;
    }
    if(Circle.LeftUpInflexion.y == 0)
    {
        return 0;
    }
    else
        return 1;
}


//�󻷵�����ֱ�������ҵĹյ�
uint8  LeftCircleFindStraightInflexion(void)
{
    //�ڷ�����Ե����ǰ�ҵ�����ҵı�Ե��
    PosiType PointAfterBorderTurnLeft;
    uint8 LessthanMaxBorderCnt = 0;

    PointAfterBorderTurnLeft.x = 0;
    PointAfterBorderTurnLeft.y = 0;
    //��¼���ֵ�Լ���Ե���Ƶ�������
    for(Ysite = Base.firstleftnormal.y;Ysite < 55;Ysite ++)
    {
        if(Img.LeftBorder[Ysite] <= PointAfterBorderTurnLeft.x)
        {
            LessthanMaxBorderCnt ++;
        }
        if(LessthanMaxBorderCnt >= 3)
            break;
        if(Img.LeftBorder[Ysite] >= PointAfterBorderTurnLeft.x)
        {
            PointAfterBorderTurnLeft.x = Img.LeftBorder[Ysite];
            PointAfterBorderTurnLeft.y = Ysite;
            LessthanMaxBorderCnt = 0;
        }
    }
    Circle.LeftUpInflexion.x = PointAfterBorderTurnLeft.x;
    Circle.LeftUpInflexion.y = PointAfterBorderTurnLeft.y;
    return 0;
}


//�����Ҳ��ߵ��Ϲյ�
uint8 Circle_Find_RightUp_Inflexion(void)
{
    Circle.RightUpInflexion.y = 0;
    Circle.RightUpInflexion.x = 0;
    //��߽����¶�����������õ��ĵ�һ����
    for(Ysite = max_ab(Base.firstrightlose.y + 2,7);Ysite < 59;Ysite ++)
    {
        for(Xsite = 2;Xsite < 127;Xsite ++)
        {
            if(ImageUsed[59 - Ysite][Xsite] == 255 && \
                ImageUsed[59 - Ysite][Xsite + 1] == 255 && \
                ImageUsed[59 - Ysite][Xsite + 2] == 255)
                break;
        }
        for(;Xsite < 127;Xsite ++)
        {
            if(ImageUsed[59 - Ysite][Xsite] == 0)
            {
                Circle.RightUpInflexion.y = Ysite;
                Circle.RightUpInflexion.x = Xsite;
                break;
            }
        }
        if(Circle.RightUpInflexion.y != 0)
            break;
    }
    if(Circle.RightUpInflexion.y == 0)
        return 0;
    else
        return 1;
}


//�һ�������ֱ�������ҵĹյ�
uint8  RightCircleFindStraightInflexion(void)
{
    //�ڱ�Ե����ǰ�ҵ�����ҵı�Ե��
    PosiType PointAfterBorderTurnRight;
    uint8 MorethanMinBorderCnt = 0;

    PointAfterBorderTurnRight.x = 127;
    PointAfterBorderTurnRight.y = 0;
    //��¼���ֵ�Լ���Ե���Ƶ�������
    for(Ysite = Base.firstrightnormal.y;Ysite < 55;Ysite ++)
    {
        if(Img.RightBorder[Ysite] >= PointAfterBorderTurnRight.x)
        {
            MorethanMinBorderCnt ++;
        }
        if(MorethanMinBorderCnt >= 3)
            break;
        if(Img.RightBorder[Ysite] <= PointAfterBorderTurnRight.x)
        {
            PointAfterBorderTurnRight.x = Img.RightBorder[Ysite];
            PointAfterBorderTurnRight.y = Ysite;
            MorethanMinBorderCnt = 0;
        }
    }
    Circle.RightUpInflexion.x = PointAfterBorderTurnRight.x;
    Circle.RightUpInflexion.y = PointAfterBorderTurnRight.y;
    return 0;
}


//����߷���ʱ�Ļ�����С�Լ�λ��
uint32 CircleInfo[][9] =
{
    /*  ����  �뾶    ����ֵ  ��һ�ι�  ת���ϵ�  ת��k    ת����  �����ٶ�  ����k  */
    {   'R' ,  50 ,    155  ,   0      ,   42   ,   35   ,   250   ,  240  ,  45   },
    {   'L' , 100 ,    105  ,   0      ,   41   ,   7    ,   250   ,  250  ,  15   },
//  {   'L' ,  65 ,    70   ,   0      ,   50   ,   25   ,   230   ,  240  ,  30   },
};


//�����ж�
void CircleJudge(void)
{
    //����ź��жϻ���
    if(ElcJudge() && Circle.FindFlag == 'F' && Wheel.Distance >= Circle.CircleEndDis)
    {
        uint8 InfoGroupx = 0;
        Circle.FindFlag = 'T';
        Circle.CircleNum ++;
        Circle.state = CircleInStraight;
        if(Barn.OutDir == 'L')
            InfoGroupx = Circle.CircleNum - 1;
        else if(Barn.OutDir == 'R')
            InfoGroupx = (Circle.CircleAmount - Circle.CircleNum);
        //������
        if(Barn.OutDir == 'L')
            Circle.dir = CircleInfo[InfoGroupx][0];
        else if(Barn.OutDir == 'R')
        {
            if( CircleInfo[InfoGroupx][0] == 'R')
                Circle.dir = 'L';
            else
                Circle.dir = 'R';
        }
        Circle.CircleR    = CircleInfo[InfoGroupx][1];
        Circle.CenterELC  = CircleInfo[InfoGroupx][2];
        Circle.TurnDis    = CircleInfo[InfoGroupx][3];
        Circle.TurnDis   /= 100;
        Circle.InTurnUP_Y = CircleInfo[InfoGroupx][4];
        Circle.InTurnK    = CircleInfo[InfoGroupx][5];
        Circle.InTurnK   /= 10;
        Circle.TurnSpeed  = CircleInfo[InfoGroupx][6];
        Circle.InSpeed    = CircleInfo[InfoGroupx][7];
        Circle.OutTurnK   = CircleInfo[InfoGroupx][8];
        Circle.OutTurnK  /= 10;
        //�����뻷�ٶ�
        ExSpeed = Circle.FoundOutSpeed;
    }
    //�������� ��־��λ ���
    if(Circle.FindFlag == 'T' && Circle.state == CircleInStraight && \
        (myabs(ElcCenter_23) <= 40 || myabs(ElcCenter_14) <= 15) && \
        ElcSum_23 > Circle.CenterELC && Wheel.Distance >= (Circle.DontELCJudgeDis))
    {
        Circle.state = CircleCenter;
        Circle.CenterDis = Wheel.Distance;
    }
    if(Circle.state == CircleCenter )//&& Wheel.Distance > (Circle.CenterDis + Circle.TurnDis))
    {
        Circle.state = CircleCouldInTurn;
    }
    //------------------------------------------------   ���ж�  ---------------------------------------------------------//
    //�뻷��ת��ʼ
    if(Circle.FindFlag == 'T' && Circle.state == CircleCouldInTurn && \
        Circle.dir == 'L' && Line30_60.LeftNormal > 5 && \
        Line5_35.LeftLose >= 15 && LineALL.LeftLoseRightNormal >= 5 && \
        InRange(Base.firstleftnormal.y,25,Circle.InTurnUP_Y))
    {
        Circle.state = CircleInTurn;
        if(Circle.TurnSpeed == 0)
            CarInfo.ClosedLoopMode = DirectLoop;
        else
            ExSpeed = Circle.TurnSpeed;
        ICMIntegrate.Yaw_I_Enable = 'T';
        ICMIntegrate.TurnLeft_I = 0.00f;
        //BEEP_ON;
    }
    //�뻷��ת���
    if((Circle.FindFlag == 'T' && Circle.state == CircleInTurn && \
        Circle.dir == 'L' && ICMIntegrate.TurnLeft_I > 25.0f && \
        Base.firstleftnormal.y <= 30) || \
        (Circle.FindFlag == 'T' && Circle.state == CircleInTurn && \
        Circle.dir == 'L' && ICMIntegrate.TurnLeft_I > 30.0f ) )
    {
        Circle.state = CircleIn;
        CarInfo.ClosedLoopMode = AllLoop;
        ExSpeed = Circle.InSpeed;
    }
    //��������ת
    if(Circle.FindFlag == 'T' && Circle.state == CircleIn && \
        Circle.dir == 'L' && Line5_35.LeftLose >= 20 &&  \
        LineALL.RightLose >= 10 && ICMIntegrate.TurnLeft_I > 250.0f)
    {
        Circle.state = CircleOutTurn;
        ICMIntegrate.TurnLeft_I = 0;
    }
    //��ɳ�����ת
    if(Circle.FindFlag == 'T' && Circle.state == CircleOutTurn && \
        Circle.dir == 'L' && ICMIntegrate.TurnLeft_I > 25.000f)
    {
        ICMIntegrate.Yaw_I_Enable = 'F';
        Circle.state = CircleOutTurnOK;
        ExSpeed = Circle.FoundOutSpeed;
    }
    //�����ٴξ�����������
    if(Circle.FindFlag == 'T' && Circle.state == CircleOutTurnOK && \
        Circle.dir == 'L' && ElcSum_14 >= Circle.CenterELC)
    {
        Circle.state = CircleOutStraight;
        //BEEP_OFF;
    }
    //������ɻ�����־��λ
    if(Circle.FindFlag == 'T' && Circle.state == CircleOutStraight && Circle.dir == 'L' \
        && ElcSum_14 <= Circle.CircleELC14 && Line5_35.LeftNormal >= 15 && Line5_35.RightNormal >= 15)
    {
        ExSpeed = NormalSpeed;
        BeepTick(1,100);
        Circle.state = CircleOut;
        Circle.FindFlag = 'F';
        Circle.CircleEndDis = (Wheel.Distance + Circle.DontELCJudgeDis);
    }
    //------------------------------------------------    �һ��ж�   ---------------------------------------------------------//

    //�뻷��ת��ʼ
    if(Circle.FindFlag == 'T' && Circle.state == CircleCouldInTurn && \
        Circle.dir == 'R' && Line30_60.RightNormal > 5 && \
        Line5_35.RightLose >= 15 && LineALL.LeftNormalRightLose >= 5 && \
        InRange(Base.firstrightnormal.y,25,Circle.InTurnUP_Y))
    {
        //BEEP_ON;
        if(Circle.TurnSpeed == 0)
            CarInfo.ClosedLoopMode = DirectLoop;
        else
            ExSpeed = Circle.TurnSpeed;
        Circle.state = CircleInTurn;
        ICMIntegrate.Yaw_I_Enable = 'T';
        ICMIntegrate.TurnRight_I = 0.00f;
    }
    //�뻷��ת���
    if((Circle.FindFlag == 'T' && Circle.state == CircleInTurn && \
        Circle.dir == 'R' && ICMIntegrate.TurnRight_I > 35.0f && \
        Base.firstrightnormal.y <= 30) || \
        (Circle.FindFlag == 'T' && Circle.state == CircleInTurn && \
        Circle.dir == 'R' && ICMIntegrate.TurnRight_I > 45.0f ))
    {
        Circle.state = CircleIn;
        CarInfo.ClosedLoopMode = AllLoop;
        ExSpeed = Circle.InSpeed;
    }
    //��������ת
    if(Circle.FindFlag == 'T' && Circle.state == CircleIn && \
        Circle.dir == 'R' && Line5_35.RightLose >= 20 &&  \
        LineALL.LeftLose >= 10 && ICMIntegrate.TurnRight_I > 250.0f
    )
    {
        Circle.state = CircleOutTurn;
        ICMIntegrate.TurnRight_I = 0;
    }
    //��ɳ�����ת
    if(Circle.FindFlag == 'T' && Circle.state == CircleOutTurn && \
        Circle.dir == 'R' && ICMIntegrate.TurnRight_I > 30.000f && \
        Line30_60.RightNormal >= 7)
    {
        ICMIntegrate.Yaw_I_Enable = 'F';
        Circle.state = CircleOutTurnOK;
        ExSpeed = Circle.FoundOutSpeed;
    }
    //�����ٴξ�����������
    if(Circle.FindFlag == 'T' && Circle.state == CircleOutTurnOK && \
        Circle.dir == 'R' && ElcSum_14 >= Circle.CenterELC )
    {
        Circle.state = CircleOutStraight;
        //BEEP_OFF;
    }
    //������ɻ�����־��λ
    if(Circle.FindFlag == 'T' && Circle.state == CircleOutStraight && Circle.dir == 'R' && \
        ElcSum_14 <= Circle.CircleELC14 && Line5_35.LeftNormal >= 15 && Line5_35.RightNormal >= 15)
    {
        ExSpeed = NormalSpeed;
        Circle.state = CircleOut;
        Circle.FindFlag = 'F';
        Circle.CircleEndDis = (Wheel.Distance + Circle.DontELCJudgeDis);
    }
}


//��������
void CircleAddingLine(void)
{
    //�뻷ֱ������
    if(Circle.FindFlag == 'T' && (Circle.state == CircleInStraight || \
        Circle.state == CircleCouldInTurn || Circle.state == CircleCenter))
    {
        if(Circle.dir == 'L')//���߲�����
        {
            LeftCircleFindStraightInflexion();
            TwoPoint_AddingLine(0,5,Circle.LeftUpInflexion.x,Circle.LeftUpInflexion.y,&Img.LeftBorder_Repair[5],55);
            for(Ysite = 6;Ysite < 55;Ysite ++)
            {
                Img.Center_Repair[Ysite] =(Img.LeftBorder_Repair[Ysite] + Img.RightBorder_Repair[Ysite])/2;
            }
        }
        if(Circle.dir == 'R')
        {
            RightCircleFindStraightInflexion();
            TwoPoint_AddingLine(127,5,Circle.RightUpInflexion.x,Circle.RightUpInflexion.y,&Img.RightBorder_Repair[5],55);
            for(Ysite = 6;Ysite < Base.Topline;Ysite ++)
            {
                Img.Center_Repair[Ysite] = (Img.LeftBorder_Repair[Ysite] + Img.RightBorder_Repair[Ysite])/2;
            }
        }
    }
    //�뻷ת����
    if(Circle.FindFlag == 'T' && Circle.state == CircleInTurn)
    {
        if(Circle.dir == 'L')
        {
            Circle_Find_LeftUp_Inflexion();
            if(Circle.LeftUpInflexion.y >= 5)
            {
                TwoPoint_AddingLine(127,5,Circle.LeftUpInflexion.x,Circle.LeftUpInflexion.y,&Img.RightBorder_Repair[5],0);
                for(Ysite = 6;Ysite < Circle.LeftUpInflexion.y;Ysite ++)
                {
                    Img.Center_Repair[Ysite] =(Img.LeftBorder_Repair[Ysite] + Img.RightBorder_Repair[Ysite])/2;
                }
            }
        }
        if(Circle.dir == 'R')
        {
            Circle_Find_RightUp_Inflexion();
            if(Circle.RightUpInflexion.y >= 5)
            {
                TwoPoint_AddingLine(0,5,Circle.RightUpInflexion.x,Circle.RightUpInflexion.y,&Img.LeftBorder_Repair[5],0);
                for(Ysite = 6;Ysite < Circle.RightUpInflexion.y;Ysite ++)
                {
                    Img.Center_Repair[Ysite] =(Img.LeftBorder_Repair[Ysite] + Img.RightBorder_Repair[Ysite])/2;
                }
            }
        }
    }
    //����ת����
    if(Circle.FindFlag == 'T' && Circle.state == CircleOutTurn)
    {
        if(Circle.dir == 'L')
        {
            TwoPoint_AddingLine(127,5,10,Column10.firstblackY20up,&Img.RightBorder_Repair[5],0);
            for(Ysite = 6;Ysite < Base.firstleftnormal.y;Ysite ++)
            {
                Img.Center_Repair[Ysite] =(Img.LeftBorder_Repair[Ysite] + Img.RightBorder_Repair[Ysite])/2;
            }
        }
        if(Circle.dir == 'R')
        {
            TwoPoint_AddingLine(0,5,107,Column117.firstblackY20up,&Img.LeftBorder_Repair[5],0);
            for(Ysite = 6;Ysite < Base.firstrightnormal.y;Ysite ++)
            {
                Img.Center_Repair[Ysite] =(Img.LeftBorder_Repair[Ysite] + Img.RightBorder_Repair[Ysite])/2;
            }
        }
    }
    //����ֱ������
    if(Circle.FindFlag == 'T' && Circle.state == CircleOutStraight)
    {
        if(Circle.dir == 'L')
        {
            for(Ysite = 5;Ysite < Base.Topline;Ysite ++)
            {
                Img.LeftBorder_Repair[Ysite] = Img.RightBorder_Repair[Ysite] - RoadWide0;//������ƫ��
                Img.Center_Repair[Ysite] = (Img.LeftBorder_Repair[Ysite] + Img.RightBorder_Repair[Ysite])/2;
            }
        }
        if(Circle.dir == 'R')
        {
            for(Ysite = Base.firstrightlose.y;Ysite < Base.Topline;Ysite ++)
            {
                Img.RightBorder_Repair[Ysite] = Img.LeftBorder_Repair[Ysite] + RoadWide0;
                Img.Center_Repair[Ysite] = (Img.LeftBorder_Repair[Ysite] + Img.RightBorder_Repair[Ysite])/2;
            }
        }
    }
}


//-------------------------------------�µ�-------------------------------------------------//


//ʹ������ͷ�ж��µ���ʹ�������ǽ���ȷ���µ�
//��������ͷ��־����
RampTypedef Ramp;


//��������б�ʵ���������
uint8 RampConfigSlope(void)
{
    Ramp.LeftSlopeSatisfy = 0;
    Ramp.RightSlopeSatisfy = 0;
    for(Ysite = 5;Ysite < 60;Ysite ++)
    {
        if(Img.LeftSlope[Ysite] < 0.8 && Img.LeftSlope[Ysite] > 0.3)
            Ramp.LeftSlopeSatisfy ++;
        if(Img.RightSlope[Ysite] < 0.8 && Img.RightSlope[Ysite] > 0.3)
            Ramp.RightSlopeSatisfy ++;
    }
    return 1;
}


//�жϵ�ź������������ж�
void RampJudge(void)
{
    //ͼ���ж�
    RampConfigSlope();
    if(((max_ab(Ramp.LeftSlopeSatisfy,Ramp.RightSlopeSatisfy) > 25 && \
        min_ab(Ramp.LeftSlopeSatisfy,Ramp.RightSlopeSatisfy) > 15 && \
        Base.LeftMax.y >= 55 && Base.RightMin.y >= 55) ||
        (Ramp.LeftSlopeSatisfy > 30 && LineALL.RightLose >= 25  && Base.LeftMax.y >= 55) || \
        (Ramp.RightSlopeSatisfy > 30 && LineALL.LeftLose >= 25  && Base.RightMin.y >= 55))&& \
        Ramp.FindFlag == 'F' && Ramp.RecoverFlag == 'F')
    {
        BeepTick(1,100);
        Ramp.MayRampFlag = 'T';
        Ramp.State = UpRamp;
        Ramp.MayRampDis = Wheel.Distance;
        ExSpeed = 230;
    }
    //ͼ��Ԥ�ȱ�־��������ʶ��
    if(Ramp.MayRampFlag == 'T' && ICM_Treated.gyro.y > Ramp.MayRampGyroYMax)
    {
        //BEEP_ON;
        Ramp.FindFlag = 'T';
        CarInfo.ControlMode = ELCMODE;
        Ramp.UpRampDis = Wheel.Distance;
        Ramp.MayRampFlag = 'F';
    }//�����������жϾ��� �����־
    else if(Ramp.MayRampFlag == 'T' && Wheel.Distance >= (Ramp.MayRampDis + Ramp.MayRampFlagClearDis))
    {
        Ramp.MayRampFlag = 'F';
        Ramp.State = EndRamp;
        ExSpeed =NormalSpeed;
    }
    //�µ����� �ֶ����þ���  һ��1m
    if(Ramp.FindFlag == 'T' && Wheel.Distance > (Ramp.UpRampDis + 1.0) \
        && Ramp.RecoverFlag == 'F')
    {
        Ramp.State = DownRamp;
        Ramp.EndRampDis = Wheel.Distance;
        Ramp.RecoverFlag = 'T';
        ExSpeed = 210;
        Ramp.FindFlag = 'F';
    }
    //���µ�����ȶ�һ�ξ��� ���������º��ֵ����ȷ
    if(Ramp.RecoverFlag == 'T' && Wheel.Distance > (Ramp.EndRampDis + Ramp.RecoverDis))
    {
        //BEEP_OFF;
        Ramp.State = EndRamp;
        Ramp.RecoverFlag = 'F';
        ExSpeed = NormalSpeed;
        CarInfo.ControlMode = CAMMODE;
        Ramp.DontJudgeRampDis = Wheel.Distance + 2 * Ramp.RampLegth;
    }
}


//-------------------------------------------------ʮ��-------------------------------------------------//

CrossTypedef Cross;

//�ѹյ�
uint8 CrossCheckInflexionPoint(void)
{
    Cross.LD_UpAbnormalCnt = 0;
    Cross.RD_UpAbnormalCnt = 0;
    Cross.LU_UpNormalCnt = 0;
    Cross.RU_UpNormalCnt = 0;
    //����������ߵ�һ����������//���¹յ�
    if (Base.firstroadnormal < 20) {
        for(Ysite = Base.firstroadnormal;Ysite < min_ab(Base.firstroadnormal + 20,ROW - 5);Ysite ++){//�������20��
            if(myabs(Img.LeftSlope[Ysite]) >= 0.67 && Img.LeftSlope[Ysite] != NoneSlope){
                Cross.LD_Inflexion.y = max_ab(Base.firstroadnormal, Ysite - 1 );
                Cross.LD_Inflexion.x = Img.LeftBorder[Cross.LD_Inflexion.y];
                break;
            }
        }
    }
    else {//�ѿ������¹յ�
        Cross.LD_Inflexion.y = 0;
        Cross.LD_Inflexion.x = LBorderCol;
    }
    //�ұ��������ߵ�һ����������//���¹յ�
    if (Base.firstroadnormal < 20) {
        for(Ysite = Base.firstroadnormal;Ysite < min_ab(Base.firstroadnormal + 20,ROW - 5);Ysite ++){
            if(myabs(Img.RightSlope[Ysite]) >= 0.67 && Img.RightSlope[Ysite] != NoneSlope){
                Cross.RD_Inflexion.y = max_ab(Base.firstroadnormal, Ysite - 1);
                Cross.RD_Inflexion.x = Img.RightBorder[Cross.RD_Inflexion.y];
                break;
            }
        }
    }
    else {//�ѿ������¹յ�
        Cross.RD_Inflexion.y = 0;
        Cross.RD_Inflexion.x = RBorderCol;
    }
    //���¹յ���������ȷ������
    for(Ysite = Cross.LD_Inflexion.y ;Ysite < (Cross.LD_Inflexion.y  + RoadWide0);Ysite ++){
        if(Img.LeftBorderFindFlag[Ysite] == 'F'/*Img.LeftSlope[Ysite] >= 0.3f && Img.LeftSlope[Ysite] < 3.0f*/)
            Cross.LD_UpAbnormalCnt ++;
        if (Cross.LD_UpAbnormalCnt >= RoadWide0/3)
            break;
    }
    //���¹յ���������ȷ������
    for(Ysite = Cross.RD_Inflexion.y ;Ysite < (Cross.RD_Inflexion.y  + RoadWide0);Ysite ++){
        if(Img.RightBorderFindFlag[Ysite] == 'F'/*Img.RightSlope[Ysite] >= 0.3f && Img.RightSlope[Ysite] < 3.0f*/)
            Cross.RD_UpAbnormalCnt ++;
        if (Cross.RD_UpAbnormalCnt >= RoadWide0/3)
            break;
    }
    //���������Ϲյ�
    for (Ysite = Cross.LD_Inflexion.y + RoadWide0/3; Ysite < Cross.LD_Inflexion.y + (RoadWide0 * 4)/3; Ysite++) {
        if (myabs(Img.RoadWideError[Ysite]) <= 10) {
            Cross.LU_Inflexion.y = Ysite;
            Cross.LU_Inflexion.x = Img.LeftBorder[Ysite];
            break;
        }
    }
    //���������Ϲյ�
    for (Ysite = Cross.RD_Inflexion.y + RoadWide0/3; Ysite < Cross.RD_Inflexion.y + (RoadWide0 * 4)/3; Ysite++) {
        if (myabs(Img.RoadWideError[Ysite]) <= 10) {
            Cross.RU_Inflexion.y = Ysite;
            Cross.RU_Inflexion.x = Img.RightBorder[Ysite];
            break;
        }
    }
    //���Ϲյ���������ȷ������
    for (Ysite = Cross.LU_Inflexion.y; Ysite < Cross.LU_Inflexion.y + 15; Ysite++) {
        if(Img.LeftBorderFindFlag[Ysite] == 'T' && myabs(Img.RoadWideError[Ysite]) <= 10)
            Cross.LU_UpNormalCnt ++;
        if(Cross.LU_UpNormalCnt >= 10)
            break;
    }
    //���Ϲյ���������ȷ������
    for (Ysite = Cross.RU_Inflexion.y; Ysite < Cross.RU_Inflexion.y + 15; Ysite++) {
        if(Img.RightBorderFindFlag[Ysite] == 'T' && myabs(Img.RoadWideError[Ysite]) <= 10)
            Cross.RU_UpNormalCnt ++;
        if(Cross.RU_UpNormalCnt >= 10)
            break;
    }
    //all
       if(InRange(myabs(Cross.LD_Inflexion.x - Cross.RD_Inflexion.x), RoadWide0 - 10, RoadWide0 + 10) && \
          InRange(myabs(Cross.LU_Inflexion.x - Cross.RU_Inflexion.x), RoadWide0 - 10, RoadWide0 + 10) && \
          InRange(Cross.LD_Inflexion.x, LOutBorderCol, CenterCol) && InRange(Cross.RD_Inflexion.x, CenterCol, ROutBorderCol) && \
          InRange(Cross.LU_Inflexion.x, LOutBorderCol, CenterCol) && InRange(Cross.RU_Inflexion.x, CenterCol, ROutBorderCol) && \
          Cross.LD_UpAbnormalCnt >= RoadWide0/3 && Cross.RD_UpAbnormalCnt >= RoadWide0/3 && \
          Cross.LU_UpNormalCnt >= 10 && Cross.RU_UpNormalCnt >= 10)
        return 1;
    else
        return 0;
}


//ʮ���б�
void CrsosJudge(void)
{
    if( Cross.FindFlag == 'F' && \
        //Base.Topline >= 45 && ��
        //Line5_35.AllLose >= 15 && Line30_60.AllNormal >= 10 && ��
        //myabs(Base.firstleftnormal.y - Base.firstrightnormal.y) < (RoadWide0 + 10) && ��
        //Column64.firstblackY20up > (max_ab(Base.firstleftnormal.y,Base.firstrightnormal.y) + 15) && ��
        //Base.firstleftnormal.y >= 25 && Base.firstrightnormal.y >= 25 && ��
            CrossCheckInflexionPoint() == 1)// && ��
        //Wheel.Distance >= (Circle.DontELCJudgeDis ))
    {
        //IMAGETOFLASH;
        Cross.FindFlag = 'T';
        BeepTick(1,300);
        Cross.state = CrossIn;
        //CarInfo.ControlMode = ELCMODE;
        Cross.CrossStartDis = Wheel.Distance;
    }
    if( //(Cross.FindFlag == 'T' && Line5_35.AllNormal >= 15) || ��
        (Cross.FindFlag == 'T' && Wheel.Distance > (Cross.CrossLength + Cross.CrossStartDis)))
    {
        Cross.FindFlag = 'F';
        Cross.state = CrossEnd;
        //CarInfo.ControlMode = CAMMODE;

    }
}

void CrossAddingLine(void)
{
    if(CrossCheckInflexionPoint() == 1)
    {
        TwoPoint_AddingLine(Cross.LD_Inflexion.x,Cross.LD_Inflexion.y,\
                Cross.LU_Inflexion.x,Cross.LU_Inflexion.y,\
                &Img.LeftBorder_Repair[Cross.LD_Inflexion.y],\
                0);
        TwoPoint_AddingLine(Cross.RD_Inflexion.x,Cross.RD_Inflexion.y,\
                Cross.RU_Inflexion.x,Cross.RU_Inflexion.y,\
                &Img.RightBorder_Repair[Cross.RD_Inflexion.y],\
                0);
        for(Ysite = min_ab(Cross.LD_Inflexion.y, Cross.RD_Inflexion.y);\
        Ysite < max_ab(Cross.LU_Inflexion.y ,Cross.RU_Inflexion.y );Ysite ++)
        {
            Img.Center_Repair[Ysite] =(Img.LeftBorder_Repair[Ysite] + Img.RightBorder_Repair[Ysite])/2;
        }
    }
}









