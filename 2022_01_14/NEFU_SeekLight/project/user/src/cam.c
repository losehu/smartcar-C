/**
 //@FileName	:cam.c
 //@CreatedDate	:2021年12月22日
 //@Author		:LiHao
 //@Description	:
**/

#include "cam.h"

/***********************************************************************
    (1)采用4邻域找到左右边界,<记录扫线的趋势,边缘延伸的方向>
    (2)记录每行的左右修复以及未修复边界,扫出修复未修复中线,
    第一次丢失 第一次正常 第二次丢失 第二次正常计算每行斜率,丢线状态
    (3)<单独记录部分行之间的线状态>
    (3)<元素判断 直道-弯道-坡道-十字-三岔-车库-环岛>
    (4)<根据元素修复中线边线>
    (5)计算中线
************************************************************************/

/***************************程序索引(ctrl + G跳转)*********************
    (1)总的标志初始化      ------------------------------------     70
    (2)两点补线             ------------------------------------    115
    (3)边缘搜索             ------------------------------------    340
    (4)特殊元素             ------------------------------------    780
    (5)直道判别             ------------------------------------    810
    (6)弯道判别             ------------------------------------    880
    (7)三岔判别             ------------------------------------   1000
    (8)三岔补线             ------------------------------------   1050
    (9)车库判别             ------------------------------------   1200
    (10)车库补线            ------------------------------------   1250
    (11)环岛判别            ------------------------------------   1500
    (12)环岛补线            ------------------------------------   1500
    (13)坡道判别            ------------------------------------   1870
    (14)十字判别            ------------------------------------   1950
    (15)十字补线            ------------------------------------   2000
*********************************************************************/

//直道赛道宽度
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
int16           Xsite = 0;//当前扫线X坐标
int16           Ysite = 0;//当前扫线Y坐标
ImageDealType   Img;
LineStatusType  LineALL,Line5_35,Line30_60;
ColumnTypedef   Column10,Column117,Column64;
RowTypedef      Row56;
//---------------------------------清除行状态计数函数--------------------------//
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

//----------------------标志初始化(在bsp_init初始化一次)-------------------//
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
    //元素标志初始化
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
    Barn.StartLineFoundY = 20;//起始搜索行
    Barn.BarnTurnCenter = 150;
    Barn.BarnTurnSpeed = 230;
    Barn.BarnInRunDis = 0.30;

    //Circle
    Circle.FindFlag = 'F';
    Circle.state = CircleOut;
    Circle.RoundCenterY = 0;
    Circle.CircleNum = 0;//当前环岛个数
    //
    Circle.CircleAmount = 2;//环岛总数
    Circle.DontELCJudgeDis = 1.0;//出环不电磁判别环岛距离
    Circle.CircleELC14 = 140;//ELC14判别环岛
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


//布尔数值范围判断
bool InRange(int32 x,int32 _min,int32 _max)
{
    if(x <= _max && x >= _min)
        return 1;
    else
        return 0;
}


//两点之间补线
uint8 TwoPoint_AddingLine(uint8 x1,uint8 y1,uint8 x2,uint8 y2,int16 *p,uint8 StopYsite)
{
    int16 i = 0;
    float k = 0.000f,b = 0.000f;
    //是否补竖直线
    if(x1 == x2)
    {
        if(StopYsite >= max_ab(y1,y2))//向上补线
        {
            for(i = min_ab(y1,y2);i < StopYsite;i ++)
            {
                *p = x1;
                p ++;
            }
        }
        else if(StopYsite > min_ab(y1,y2))//下
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
    else//计算K补线
    {
        //计算直线 y = kx + b
        k = (float)(y2 - y1) / (float)(x2 - x1);
        b = (float)(y1 - k*x1);
        if(StopYsite >= max_ab(y1,y2))//向上补线
        {
            for(i = min_ab(y1,y2);i < StopYsite;i ++)
            {
                *p = (uint8)((float)(i - b) / k);
                p ++;
            }
        }
        else if(StopYsite > min_ab(y1,y2))//下
        {
            for(i = max_ab(y1,y2);i > StopYsite;i --)
            {
                *p = (uint8)((float)(i - b) / k);
                p --;
            }
        }
        else if(StopYsite == 0)//直接进行两点补线 方向是从下往上
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





//边缘计数 统计不同行的边界状态
void LineBorderStateCount(void)
{
    //左右均有边线
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
    //左边有右边丢失
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
    //左边丢失右边有
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
    //均丢失
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


//--------------------------------------------图像基本处理--------------------------------------------//


//记录每行的第一个边缘点为当前行边缘
//var
int16 StartColSum = 0;
int16 StartColIndex = 0;

uint8 GetBorder(void)
{
    //初始化扫线标志var
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


    //左右0和127两列
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
    //------------------------元素清标志-------------------------//
    //直道
    Straight.LeftCnt = 0;
    Straight.LeftCnt40up = 0;
    Straight.RightCnt = 0;
    Straight.RightCnt40up = 0;
    //弯道
    Curve.LB_TR_Cnt = 0;
    Curve.RB_TL_Cnt = 0;
    //三岔
    Fork.PeakFindFlag = 'F';
    Fork.CenterLineBlack = 0;
    Fork.MaybeCorssFlag = 'F';
    //车库

    //环岛

    //-------------------------古法搜线------------------------//
    #if ImgDubug
    //更新010列,107列状态
    for(Ysite = 0;Ysite < ROW;Ysite ++)
    {
        if(InRange(Ysite,3,ROW-3))//3~ROW-3行
        {
            //10列
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
            //117列
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
            //64列
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
        if(InRange(Ysite,23,ROW-3))//23~ROW-3行
        {
            //10列
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
            //107列
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
            //64列
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
            //10列
            if(ImageUsed[ROW - 1 - Ysite][10] == 0)
                Column10.Line0_30_Black ++;
            else
                Column10.Line0_30_White ++;
            //107列
            if(ImageUsed[ROW - 1 - Ysite][117] == 0)
                Column117.Line0_30_Black ++;
            else
                Column117.Line0_30_White ++;
            //64列
            if(ImageUsed[ROW - 1 - Ysite][64] == 0)
                Column64.Line0_30_Black ++;
            else
                Column64.Line0_30_White ++;
        }
        if(Ysite >= 30)
        {
            //10列
            if(ImageUsed[ROW - 1 - Ysite][10] == 0)
                Column10.Line30_60_Black ++;
            else
                Column10.Line30_60_White ++;
            //107列
            if(ImageUsed[ROW - 1 - Ysite][117] == 0)
                Column117.Line30_60_Black ++;
            else
                Column117.Line30_60_White ++;
            //64列
            if(ImageUsed[ROW - 1 - Ysite][64] == 0)
                Column64.Line30_60_Black ++;
            else
                Column64.Line30_60_White ++;
        }
    }
    //更新56行状态
    //从左向右扫线黑白起始点
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
    //从右向左扫线黑白起始点
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
    //获取首行起始列
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
    if(Base.StartSearchColumn_1st <= 0) Base.StartSearchColumn_1st = CenterCol;//使车子过急转弯
    Base.StartSearchColumn = Base.StartSearchColumn_1st;    //更新起始搜索点
    //按照继承的方式从中间向两边搜线
    for(Ysite = StartScanRow;Ysite < ROW;Ysite ++)
    {
        //扫左边线
        for(Xsite = Base.StartSearchColumn;Xsite >= 0;Xsite --)
        {
            if(ImageUsed[ROW - 1 - Ysite][Xsite] == BlackPoint && \
               !(Xsite == 0 || ImageUsed[ROW - 1 - Ysite][Xsite] == GrayPoint))//设置赛道左起始点
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
                if(Base.firstleftlose.y == 0 && Base.firstleftnormal.y != 0)//设置赛道左丢失点
                {
                    Base.firstleftlose.y = Ysite;
                    Base.firstleftlose.x = Xsite;
                }
            }
            if(Ysite >= 3 && Img.LeftBorderFindFlag[Ysite] == 'T' && \
               Img.LeftBorderFindFlag[Ysite-1] == 'T' && \
               Img.LeftBorderFindFlag[Ysite-2] == 'T')
                Img.LeftSlope[Ysite] = ((float)(Img.LeftBorder[Ysite]) - (float)(Img.LeftBorder[Ysite - 3]))/3;//计算斜率
            else if(Ysite <3 && Img.LeftBorderFindFlag[Ysite] == 'T')
                Img.LeftSlope[Ysite] = 0.00f ;
            else
                Img.LeftSlope[Ysite] = NoneSlope;
        }
        //扫右边线
        for(Xsite = Base.StartSearchColumn;Xsite < COL;Xsite ++)
        {
            if(ImageUsed[ROW - 1 - Ysite][Xsite] == BlackPoint && \
               !(Xsite == (COL - 1) || ImageUsed[ROW - 1 - Ysite][Xsite] == GrayPoint))//扫到黑线且不接近边缘
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
                Img.RightSlope[Ysite] = ((float)(Img.RightBorder[Ysite-3]) - (float)(Img.RightBorder[Ysite]))/3;//计算斜率
            else if(Ysite <3 && Img.RightBorderFindFlag[Ysite] == 'T')
                Img.RightSlope[Ysite] = 0.00f ;
            else
                Img.RightSlope[Ysite] = NoneSlope;
        }
        //更新扫线起始点
        Base.StartSearchColumn = (Img.LeftBorder[Ysite] + Img.RightBorder[Ysite]) / 2;
        //更新赛道宽度多个标志
        Img.RoadWide[Ysite] = Img.RightBorder[Ysite] - Img.LeftBorder[Ysite];
        Img.RoadWideError[Ysite] = Img.RoadWide[Ysite] - RoadWide0;
        if(InRange(Img.RoadWide[Ysite],RoadWide0 - 5,RoadWide0 + 5))
            Img.RoadNormalFlag[Ysite] = 'T';
        else
            Img.RoadNormalFlag[Ysite] = 'F';
        if(Img.RoadNormalFlag[Ysite] == 'T' && \
            //Img.RoadNormalFlag[Ysite + 1] == 'T' && 。
            //Img.RoadNormalFlag[Ysite + 2] == 'T' && 。
            Base.firstroadnormal == 0 && Ysite >= StartScanRow )
            Base.firstroadnormal = Ysite;
        if(Img.RoadNormalFlag[Ysite] == 'F' && \
//            Img.RoadNormalFlag[Ysite + 1] == 'F' && 。
//            Img.RoadNormalFlag[Ysite + 2] == 'F' && 。
            Base.firstroaderror == 0 && Ysite >= StartScanRow)
            Base.firstroaderror = Ysite;
        //更新顶边
        //有效行截止 需要实地测试
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
        //更新三边极点 全局
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
        //更新极点 20行向上
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
        //更新中线
        if(Ysite < StartScanRow)
            Img.Center[Ysite] = CenterCol;//底部赋中值保持平滑

        else
        {
            //此处应对有丢边界的中线进行处理
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
        //复制坐标
        Img.LeftBorder_Repair[Ysite] = Img.LeftBorder[Ysite];
        Img.RightBorder_Repair[Ysite] = Img.RightBorder[Ysite];
        Img.Center_Repair[Ysite] = Img.Center[Ysite];
        //边缘状态计数
        LineBorderStateCount();
    }
    //---------------------------整体扫线截止-----------------------------//
    Base.Topline = Base.ToplineTemp;//最终顶线更新
    ElementProcessing();//元素判断和处理
    //ret
    return 1;
}

/**
* @brief    元素判断和处理
* @param    None
* @return   0
* @Sample   ElementProcessing();
**/
uint8 ElementProcessing(void)
{
    //---------------------------赛道类型判断开始-----------------------------//
    if(Base.element_check == 'T')
    {
        StrightJudge();
        CurveJudge();
//      if(Fork.FindFlag == 'F' && Circle.FindFlag == 'F' && Ramp.FindFlag == 'F')
//          Barn_Judge();//车库判断
        if(CarInfo.UpTime >= 0.300f)
        {
          if(Fork.FindFlag == 'F' && Circle.FindFlag == 'F' && \
              Barn.FindFlag == 'F' && Ramp.FindFlag == 'F')
              CrsosJudge();
//          if(Barn.FindFlag == 'F' && Circle.FindFlag == 'F' && 。
//              Ramp.FindFlag == 'F' && Cross.FindFlag == 'F' && 。
//              CarInfo.UpTime >= 1.000f )
//              Fork_Judge();
//            if(Fork.FindFlag == 'F' && Barn.FindFlag == 'F' && Ramp.FindFlag == 'F')
//              CircleJudge();
//          if(Fork.FindFlag == 'F' && Circle.FindFlag == 'F' && 。
//              Barn.FindFlag == 'F' && Cross.FindFlag == 'F')
//              RampJudge();//注意：坡道的图像识别需定义ImgDebug为1，图像识别效果不好，建议使用激光测距重写坡道识别
        }
    }
    //---------------------------赛道类型判断结束-----------------------------//
    //--------------------------------补线开始--------------------------------//
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
        // 修复后的中线和边线限幅
        LIMIT_COL(Img.Center_Repair[Ysite],127,0);
        LIMIT_COL(Img.LeftBorder_Repair[Ysite],127,0);
        LIMIT_COL(Img.RightBorder_Repair[Ysite],127,0);
    }
    //--------------------------------补线结束--------------------------------//
    return 0 ;
}



//------------------------------------------直道----------------------------------------//
StraightTypedef Straight;


//直道识别
uint8 StrightJudge(void)
{
    //统一计算标志
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
    //判断
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
    //标志置位
    if(Straight.LeftLong == 'T' || Straight.RightLong == 'T' )
    {
        Straight.FindFlag = 'T';
        if(Straight.Beep == 'T')
            BeepTick(1,100);
    }
    if(Straight.LeftShort == 'T' || Straight.RightShort == 'T' )
    {
        //Straight.FindFlag = 'T';//短直道标志不被置位
        if(Straight.Beep == 'T')
            BeepTick(1,100);
    }
    else
        Straight.FindFlag = 'F';

    return 1;
}


//--------------------------------------------弯道------------------------------------------//
CurveTypedef    Curve;


//判断
uint8 CurveJudge(void)
{
    //统一计算标志
    for(Ysite = 0;Ysite < Base.Topline;Ysite ++)
    {
        //左线右偏
        if(Img.LeftSlope[Ysite] > 1.7f &&  \
            Img.LeftBorder[Ysite] >= Img.LeftBorder[Ysite - 1] && \
            Img.LeftBorderFindFlag[Ysite] == 'T' )
        {
            Curve.LB_TR_Cnt ++;
        }
        //右线左偏
        if(Img.RightSlope[Ysite] > 1.7f  && \
            Img.RightBorder[Ysite] <= Img.RightBorder[Ysite - 1] && \
            Img.RightBorderFindFlag[Ysite] == 'T')
        {
            Curve.RB_TL_Cnt ++;
        }
    }
    //判断
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
    if(Curve.TurnRight == 'T')//若右弯
    {



    }
    else if (Curve.TurnLeft == 'T')//若左弯
    {



    }
    return 1;
}


//------------------------------------------三岔----------------------------------------/
ForkTypedef  Fork;


//寻找上拐点 满足条件会返回1 找不到上拐点返回0
uint8 Fork_Find_Peak(void)
{
    uint8 xPoint0 = 64;
    //寻找三岔的顶点左右边缘
    //从左向右搜黑线
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


//返回中线黑色部分的长度
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


//规避十字误判 从30行向上搜索三岔顶边边缘 计算存在边缘的列数
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


//三岔参数表 向左发车时
uint32 ForkInfo[4][3] =
{
    /*  方向    转入增益*10    转出增益*10 */
    {   'L'   ,      35      ,      32     },
    {   'R'   ,      35      ,      32     },
};


//判断
//三岔
uint8 ForkJudge(void)
{
    //入三岔
    if(Fork.FindFlag == 'F' && Fork_Find_Peak() != 0 &&\
        LineALL.LeftLose >= 25 && LineALL.RightLose >= 25 && \
        Fork_Avoid_Cross() == 1 && Ret_BlackCenterLine_Length() >= 3 &&
        InRange(ElcSum_14,20,80) && \
        myabs(Column10.Line30_60_Black - Column117.Line30_60_Black) < 12 && \
        Column64.firstblackY < max_ab(Column10.firstblackY,Column117.firstblackY) && \
        myabs(Column117.firstblackY20up - Column10.firstblackY20up) < 7)//补线开始闭环模式切换为转向闭环
    {
        CarInfo.ClosedLoopMode = DirectLoop;
        BeepTick(1,100);
        Fork.FindFlag = 'T';
        Fork.state = ForkInTurn;
        ICMIntegrate.Yaw_I_Enable = 'T';
        ICMIntegrate.TurnRight_I = 0.000f;
        ICMIntegrate.TurnLeft_I = 0.000f;
        //记录 读取
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
    //出三岔
    if(Base.Topline <= 55 && Fork.FindFlag == 'T' && \
        LineALL.AllLose >= 20 && Fork.state == ForkIn && ElcSum_14 <= 30)//赛道到不了顶边
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
    //三岔进入补线 双边丢失
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
    //三岔出补线 也可以直接采用电磁出三岔 然后电磁转摄像头
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







//-------------------------------------车库-------------------------------------------------//
BarnTypedef     Barn;

//起跑直接打两次角 用不着补线
//返回指定行范围黑白相间的点大于定值的行数是否大于定值
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


//入库信息
uint16 BarnInInfo[2][6] =
{
    /* 方向标识 识别起始行 识别后行驶距离cm 转向中线赋值   转向速度   转向完跑的距离*/
    {     'L'  ,   20     ,     20         ,    100      ,   230     ,    30   },
    {     'R'  ,   20     ,     15         ,    100      ,   230     ,    30   },
};


//判断车库
void BarnJudge(void)
{
    //判断出库
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
        //读取入库信息
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
    //出库设置左右轮单独控制速度
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
    //转向完成
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
    ///////////////////////入库判断////////////////////////////
    if(Barn.FindFlag == 'F' && Barn.state == BarnOutOK && \
        ElcSum_14 >= Barn.BarnFoundELCSum14Min )
    {
        if( Column64.Line30_60_White > 20 && \
            Row56.LeftToRightFirstWhite < 64 &&  Row56.RightToLeftFirstBlack > 64 && \
            Line_Ret_WhiteBlackChange_VaildN() == 1)
        {
            //起跑线识别 比较耗时 因此放在后边
            Barn.BarnNum ++;
            BeepTick(1,50);
            if(Barn.BarnNum == Barn.BranInNum)//需要入库
            {
                Barn.state = BarnTurnStraight;
                CarInfo.CAR_PROTECT_MODE = 'F';
                Barn.FindFlag = 'T';
            }
            else//不需要入库
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
    //方案1电磁通过
    if(Barn.FindFlag == 'T' && Barn.state == BarnPass && \
        Wheel.Distance >= (Barn.BarnLength + Barn.BarnFindDis))
    {
        Barn.FindFlag = 'F';
        CarInfo.ControlMode = CAMMODE;
        Barn.state = BarnOutOK;
        //BEEP_OFF;
        //清除环岛标志
        Circle.CircleNum = 0;
        Fork.ForkNum = 0;
    }
    //方案2停车
    //允许转向
    if(Barn.FindFlag == 'T' && Barn.state == BarnTurnStraight && BarnTurnInOk() <= 5 )
    {
        //BEEP_ON;
        Barn.state = BarnInTurnStraight;
        Barn.BarnInTurnDis = Wheel.Distance;
    }
    //结束小直道转向开始
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
    //转向完成进入车库小直道
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
    //入库完成
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


//出库补线
void BarnAddingLine(void)
{
    //出车库补线 右上端点
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
    //入库补线
    if(Barn.FindFlag == 'T' && Barn.state == BarnInTurn && Barn.OutDir == 'L')
    {
        TwoPoint_AddingLine(127,5,0,Column10.firstblackY20up,&Img.RightBorder_Repair[5],0);
        for(Ysite = 6;Ysite < Column10.firstblackY20up;Ysite ++)
        {
            Img.Center_Repair[Ysite] = Img.RightBorder_Repair[Ysite] - RoadWide0/2;
        }
    }
    //入库补线
    if(Barn.FindFlag == 'T' && Barn.state == BarnInTurn && Barn.OutDir == 'R')
    {
        TwoPoint_AddingLine(0,5,127,Column117.firstblackY20up,&Img.LeftBorder_Repair[5],0);
        for(Ysite = 6;Ysite < Base.firstrightnormal.y;Ysite ++)
        {
            Img.Center_Repair[Ysite] = Img.LeftBorder_Repair[Ysite] + RoadWide0/2;
        }
    }
}


//-------------------------------------环岛-------------------------------------------------//
CircleTypedef   Circle;


//环岛找补线的上拐点
uint8 Circle_Find_LeftUp_Inflexion(void)
{
    Circle.LeftUpInflexion.y = 0;
    Circle.LeftUpInflexion.x = 0;
    //右边界自下而上向左遍历得到的第一个点
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


//左环岛进入直道补线找的拐点
uint8  LeftCircleFindStraightInflexion(void)
{
    //在发生边缘左移前找到的最靠右的边缘点
    PosiType PointAfterBorderTurnLeft;
    uint8 LessthanMaxBorderCnt = 0;

    PointAfterBorderTurnLeft.x = 0;
    PointAfterBorderTurnLeft.y = 0;
    //记录最大值以及边缘左移的行列数
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


//环岛找补线的上拐点
uint8 Circle_Find_RightUp_Inflexion(void)
{
    Circle.RightUpInflexion.y = 0;
    Circle.RightUpInflexion.x = 0;
    //左边界自下而上向左遍历得到的第一个点
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


//右环岛进入直道补线找的拐点
uint8  RightCircleFindStraightInflexion(void)
{
    //在边缘右移前找到的最靠右的边缘点
    PosiType PointAfterBorderTurnRight;
    uint8 MorethanMinBorderCnt = 0;

    PointAfterBorderTurnRight.x = 127;
    PointAfterBorderTurnRight.y = 0;
    //记录最大值以及边缘右移的行列数
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


//向左边发车时的环岛大小以及位置
uint32 CircleInfo[][9] =
{
    /*  方向  半径    中阈值  走一段拐  转向上点  转向k    转向速  环内速度  出环k  */
    {   'R' ,  50 ,    155  ,   0      ,   42   ,   35   ,   250   ,  240  ,  45   },
    {   'L' , 100 ,    105  ,   0      ,   41   ,   7    ,   250   ,  250  ,  15   },
//  {   'L' ,  65 ,    70   ,   0      ,   50   ,   25   ,   230   ,  240  ,  30   },
};


//环岛判断
void CircleJudge(void)
{
    //电磁信号判断环岛
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
        //读参数
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
        //设置入环速度
        ExSpeed = Circle.FoundOutSpeed;
    }
    //环岛中央 标志置位 电磁
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
    //------------------------------------------------   左环判断  ---------------------------------------------------------//
    //入环左转开始
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
    //入环左转完毕
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
    //出环岛左转
    if(Circle.FindFlag == 'T' && Circle.state == CircleIn && \
        Circle.dir == 'L' && Line5_35.LeftLose >= 20 &&  \
        LineALL.RightLose >= 10 && ICMIntegrate.TurnLeft_I > 250.0f)
    {
        Circle.state = CircleOutTurn;
        ICMIntegrate.TurnLeft_I = 0;
    }
    //完成出环左转
    if(Circle.FindFlag == 'T' && Circle.state == CircleOutTurn && \
        Circle.dir == 'L' && ICMIntegrate.TurnLeft_I > 25.000f)
    {
        ICMIntegrate.Yaw_I_Enable = 'F';
        Circle.state = CircleOutTurnOK;
        ExSpeed = Circle.FoundOutSpeed;
    }
    //出环再次经过环岛中心
    if(Circle.FindFlag == 'T' && Circle.state == CircleOutTurnOK && \
        Circle.dir == 'L' && ElcSum_14 >= Circle.CenterELC)
    {
        Circle.state = CircleOutStraight;
        //BEEP_OFF;
    }
    //出环完成环岛标志置位
    if(Circle.FindFlag == 'T' && Circle.state == CircleOutStraight && Circle.dir == 'L' \
        && ElcSum_14 <= Circle.CircleELC14 && Line5_35.LeftNormal >= 15 && Line5_35.RightNormal >= 15)
    {
        ExSpeed = NormalSpeed;
        BeepTick(1,100);
        Circle.state = CircleOut;
        Circle.FindFlag = 'F';
        Circle.CircleEndDis = (Wheel.Distance + Circle.DontELCJudgeDis);
    }
    //------------------------------------------------    右环判断   ---------------------------------------------------------//

    //入环右转开始
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
    //入环右转完毕
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
    //出环岛右转
    if(Circle.FindFlag == 'T' && Circle.state == CircleIn && \
        Circle.dir == 'R' && Line5_35.RightLose >= 20 &&  \
        LineALL.LeftLose >= 10 && ICMIntegrate.TurnRight_I > 250.0f
    )
    {
        Circle.state = CircleOutTurn;
        ICMIntegrate.TurnRight_I = 0;
    }
    //完成出环右转
    if(Circle.FindFlag == 'T' && Circle.state == CircleOutTurn && \
        Circle.dir == 'R' && ICMIntegrate.TurnRight_I > 30.000f && \
        Line30_60.RightNormal >= 7)
    {
        ICMIntegrate.Yaw_I_Enable = 'F';
        Circle.state = CircleOutTurnOK;
        ExSpeed = Circle.FoundOutSpeed;
    }
    //出环再次经过环岛中心
    if(Circle.FindFlag == 'T' && Circle.state == CircleOutTurnOK && \
        Circle.dir == 'R' && ElcSum_14 >= Circle.CenterELC )
    {
        Circle.state = CircleOutStraight;
        //BEEP_OFF;
    }
    //出环完成环岛标志置位
    if(Circle.FindFlag == 'T' && Circle.state == CircleOutStraight && Circle.dir == 'R' && \
        ElcSum_14 <= Circle.CircleELC14 && Line5_35.LeftNormal >= 15 && Line5_35.RightNormal >= 15)
    {
        ExSpeed = NormalSpeed;
        Circle.state = CircleOut;
        Circle.FindFlag = 'F';
        Circle.CircleEndDis = (Wheel.Distance + Circle.DontELCJudgeDis);
    }
}


//环岛补线
void CircleAddingLine(void)
{
    //入环直道补线
    if(Circle.FindFlag == 'T' && (Circle.state == CircleInStraight || \
        Circle.state == CircleCouldInTurn || Circle.state == CircleCenter))
    {
        if(Circle.dir == 'L')//右线补左线
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
    //入环转向补线
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
    //出环转向补线
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
    //出环直道补线
    if(Circle.FindFlag == 'T' && Circle.state == CircleOutStraight)
    {
        if(Circle.dir == 'L')
        {
            for(Ysite = 5;Ysite < Base.Topline;Ysite ++)
            {
                Img.LeftBorder_Repair[Ysite] = Img.RightBorder_Repair[Ysite] - RoadWide0;//令向右偏移
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


//-------------------------------------坡道-------------------------------------------------//


//使用摄像头判断坡道后使用陀螺仪进行确认坡道
//否则摄像头标志清零
RampTypedef Ramp;


//计算满足斜率的左右行数
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


//判断电磁和陀螺仪联合判断
void RampJudge(void)
{
    //图像判断
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
    //图像预先标志后陀螺仪识别
    if(Ramp.MayRampFlag == 'T' && ICM_Treated.gyro.y > Ramp.MayRampGyroYMax)
    {
        //BEEP_ON;
        Ramp.FindFlag = 'T';
        CarInfo.ControlMode = ELCMODE;
        Ramp.UpRampDis = Wheel.Distance;
        Ramp.MayRampFlag = 'F';
    }//超过陀螺仪判断距离 清零标志
    else if(Ramp.MayRampFlag == 'T' && Wheel.Distance >= (Ramp.MayRampDis + Ramp.MayRampFlagClearDis))
    {
        Ramp.MayRampFlag = 'F';
        Ramp.State = EndRamp;
        ExSpeed =NormalSpeed;
    }
    //坡道结束 手动设置距离  一般1m
    if(Ramp.FindFlag == 'T' && Wheel.Distance > (Ramp.UpRampDis + 1.0) \
        && Ramp.RecoverFlag == 'F')
    {
        Ramp.State = DownRamp;
        Ramp.EndRampDis = Wheel.Distance;
        Ramp.RecoverFlag = 'T';
        ExSpeed = 210;
        Ramp.FindFlag = 'F';
    }
    //下坡道电磁稳定一段距离 陀螺仪下坡后采值不正确
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


//-------------------------------------------------十字-------------------------------------------------//

CrossTypedef Cross;

//搜拐点
uint8 CrossCheckInflexionPoint(void)
{
    Cross.LD_UpAbnormalCnt = 0;
    Cross.RD_UpAbnormalCnt = 0;
    Cross.LU_UpNormalCnt = 0;
    Cross.RU_UpNormalCnt = 0;
    //左边向上搜线第一个不正常点//左下拐点
    if (Base.firstroadnormal < 20) {
        for(Ysite = Base.firstroadnormal;Ysite < min_ab(Base.firstroadnormal + 20,ROW - 5);Ysite ++){//搜最近的20行
            if(myabs(Img.LeftSlope[Ysite]) >= 0.67 && Img.LeftSlope[Ysite] != NoneSlope){
                Cross.LD_Inflexion.y = max_ab(Base.firstroadnormal, Ysite - 1 );
                Cross.LD_Inflexion.x = Img.LeftBorder[Cross.LD_Inflexion.y];
                break;
            }
        }
    }
    else {//已看不到下拐点
        Cross.LD_Inflexion.y = 0;
        Cross.LD_Inflexion.x = LBorderCol;
    }
    //右边向上搜线第一个不正常点//右下拐点
    if (Base.firstroadnormal < 20) {
        for(Ysite = Base.firstroadnormal;Ysite < min_ab(Base.firstroadnormal + 20,ROW - 5);Ysite ++){
            if(myabs(Img.RightSlope[Ysite]) >= 0.67 && Img.RightSlope[Ysite] != NoneSlope){
                Cross.RD_Inflexion.y = max_ab(Base.firstroadnormal, Ysite - 1);
                Cross.RD_Inflexion.x = Img.RightBorder[Cross.RD_Inflexion.y];
                break;
            }
        }
    }
    else {//已看不到下拐点
        Cross.RD_Inflexion.y = 0;
        Cross.RD_Inflexion.x = RBorderCol;
    }
    //左下拐点向上搜线确认线数
    for(Ysite = Cross.LD_Inflexion.y ;Ysite < (Cross.LD_Inflexion.y  + RoadWide0);Ysite ++){
        if(Img.LeftBorderFindFlag[Ysite] == 'F'/*Img.LeftSlope[Ysite] >= 0.3f && Img.LeftSlope[Ysite] < 3.0f*/)
            Cross.LD_UpAbnormalCnt ++;
        if (Cross.LD_UpAbnormalCnt >= RoadWide0/3)
            break;
    }
    //右下拐点向上搜线确认线数
    for(Ysite = Cross.RD_Inflexion.y ;Ysite < (Cross.RD_Inflexion.y  + RoadWide0);Ysite ++){
        if(Img.RightBorderFindFlag[Ysite] == 'F'/*Img.RightSlope[Ysite] >= 0.3f && Img.RightSlope[Ysite] < 3.0f*/)
            Cross.RD_UpAbnormalCnt ++;
        if (Cross.RD_UpAbnormalCnt >= RoadWide0/3)
            break;
    }
    //向上搜左上拐点
    for (Ysite = Cross.LD_Inflexion.y + RoadWide0/3; Ysite < Cross.LD_Inflexion.y + (RoadWide0 * 4)/3; Ysite++) {
        if (myabs(Img.RoadWideError[Ysite]) <= 10) {
            Cross.LU_Inflexion.y = Ysite;
            Cross.LU_Inflexion.x = Img.LeftBorder[Ysite];
            break;
        }
    }
    //向上搜右上拐点
    for (Ysite = Cross.RD_Inflexion.y + RoadWide0/3; Ysite < Cross.RD_Inflexion.y + (RoadWide0 * 4)/3; Ysite++) {
        if (myabs(Img.RoadWideError[Ysite]) <= 10) {
            Cross.RU_Inflexion.y = Ysite;
            Cross.RU_Inflexion.x = Img.RightBorder[Ysite];
            break;
        }
    }
    //左上拐点向上搜线确认线数
    for (Ysite = Cross.LU_Inflexion.y; Ysite < Cross.LU_Inflexion.y + 15; Ysite++) {
        if(Img.LeftBorderFindFlag[Ysite] == 'T' && myabs(Img.RoadWideError[Ysite]) <= 10)
            Cross.LU_UpNormalCnt ++;
        if(Cross.LU_UpNormalCnt >= 10)
            break;
    }
    //右上拐点向上搜线确认线数
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


//十字判别
void CrsosJudge(void)
{
    if( Cross.FindFlag == 'F' && \
        //Base.Topline >= 45 && 。
        //Line5_35.AllLose >= 15 && Line30_60.AllNormal >= 10 && 。
        //myabs(Base.firstleftnormal.y - Base.firstrightnormal.y) < (RoadWide0 + 10) && 。
        //Column64.firstblackY20up > (max_ab(Base.firstleftnormal.y,Base.firstrightnormal.y) + 15) && 。
        //Base.firstleftnormal.y >= 25 && Base.firstrightnormal.y >= 25 && 。
            CrossCheckInflexionPoint() == 1)// && 。
        //Wheel.Distance >= (Circle.DontELCJudgeDis ))
    {
        //IMAGETOFLASH;
        Cross.FindFlag = 'T';
        BeepTick(1,300);
        Cross.state = CrossIn;
        //CarInfo.ControlMode = ELCMODE;
        Cross.CrossStartDis = Wheel.Distance;
    }
    if( //(Cross.FindFlag == 'T' && Line5_35.AllNormal >= 15) || 。
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









