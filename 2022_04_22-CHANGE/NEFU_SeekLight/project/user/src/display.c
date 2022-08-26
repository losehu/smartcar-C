/**
 //@FileName	:display.c
 //@CreatedDate	:2021年12月22日
 //@Author		:赵延国
 //@Description	:
 **/

#include "display.h"

/**************  总的显示界面  *******************/

//调整第x个参数
uint8 ParamsAdjustFloat(float *p, uint16 pos_x, uint16 pos_y, float step) {
    //读取
    float now = *p;
    //变色
    ips114_pencolor = PURPLE;
    while(KeyNext != onepress)
    {
        if(KeyLeft == onepress)
        {
            KeyLeft = nopress;
            now += step * 10;
        }
        if(KeyRight == onepress)
        {
            KeyRight = nopress;
            now -= step * 10;
        }
        if(KeyUp == onepress)
        {
            KeyUp = nopress;
            now += step;
        }
        if(KeyDown == onepress)
        {
            KeyDown = nopress;
            now -= step;
        }
        ips114_show_float(pos_x,pos_y,now,3,3);
    }
    KeyNext = nopress;
    ips114_pencolor = RED;
    *p = now;
    return 1;
}

//调整第x个参数
uint8 ParamsAdjustShort(short *p, uint16 pos_x, uint16 pos_y, short step) {
    //读取
    short now = *p;
    ips114_pencolor = PURPLE;
    while(KeyNext != onepress)
    {
        if(KeyLeft == onepress)
        {
            KeyLeft = nopress;
            now += step * 10;
        }
        if(KeyRight == onepress)
        {
            KeyRight = nopress;
            now -= step * 10;
        }
        if(KeyUp == onepress)
        {
            KeyUp = nopress;
            now += step;
        }
        if(KeyDown == onepress)
        {
            KeyDown = nopress;
            now -= step;
        }
        ips114_show_int16(pos_x,pos_y,now);
    }
    KeyNext = nopress;
    ips114_pencolor = RED;
    *p = now;

    return 1;
}

//调整第x个参数
uint8 ParamsAdjustUint32(uint32 *p, uint16 pos_x, uint16 pos_y, short step) {
    //读取
    uint32 now = *p;
    ips114_pencolor = PURPLE;
    while(KeyNext != onepress)
    {
        if(KeyLeft == onepress)
        {
            KeyLeft = nopress;
            now += step * 10;
        }
        if(KeyRight == onepress)
        {
            KeyRight = nopress;
            now -= step * 10;
        }
        if(KeyUp == onepress)
        {
            KeyUp = nopress;
            now += step;
        }
        if(KeyDown == onepress)
        {
            KeyDown = nopress;
            now -= step;
        }
        ips114_show_int16(pos_x,pos_y,now);
    }
    KeyNext = nopress;
    ips114_pencolor = RED;
    *p = now;

    return 1;
}

//调整第x个参数的状态 为是或者否
uint8 StateAdjust(uint8 *p, uint16 pos_x, uint16 pos_y) {
    //读取
    uint8 now = *p;
    ips114_pencolor = PURPLE;
    while(KeyNext != onepress)
    {
        if(KeyLeft == onepress)
        {
            KeyLeft = nopress;
            if(now == 'T')
            now = 'F';
            else
            now = 'T';
        }
        if(KeyRight == onepress)
        {
            KeyRight = nopress;
            if(now == 'T')
            now = 'F';
            else
            now = 'T';
        }
        if(KeyUp == onepress)
        {
            KeyUp = nopress;
            if(now == 'T')
            now = 'F';
            else
            now = 'T';
        }
        if(KeyDown == onepress)
        {
            KeyDown = nopress;
            if(now == 'T')
            now = 'F';
            else
            now = 'T';
        }
        if(now == 'T')
        ips114_show_string(pos_x,pos_y,(const char*)("T"));
        else
        ips114_show_string(pos_x,pos_y,(const char*)("F"));
    }
    KeyNext = nopress;
    ips114_pencolor = RED;
    *p = now;

    return 1;
}

//调整第x个参数的状态 为左或者右
uint8 DirAdjust(uint8 *p, uint16 pos_x, uint16 pos_y) {
    //读取
    uint8 now = *p;
    ips114_pencolor = PURPLE;
    while(KeyNext != onepress)
    {
        if(KeyLeft == onepress)
        {
            KeyLeft = nopress;
            if(now == 'L')
            now = 'R';
            else
            now = 'L';
        }
        if(KeyRight == onepress)
        {
            KeyRight = nopress;
            if(now == 'L')
            now = 'R';
            else
            now = 'L';
        }
        if(KeyUp == onepress)
        {
            KeyUp = nopress;
            if(now == 'T')
            now = 'F';
            else
            now = 'T';
        }
        if(KeyDown == onepress)
        {
            KeyDown = nopress;
            if(now == 'T')
            now = 'F';
            else
            now = 'T';
        }
        if(now == 'L')
        ips114_show_string(pos_x,pos_y,(const char*)("L"));
        else
        ips114_show_string(pos_x,pos_y,(const char*)("R"));
    }
    KeyNext = nopress;
    ips114_pencolor = RED;
    *p = now;

    return 1;
}

//调整枚举类参数
uint8 EnumAdjust(uint8 *p, char (*prams)[], uint8 prams_max, uint16 pos_x,
        uint16 pos_y) {
    //读取
    uint8 now = *p;
    ips114_pencolor = PURPLE;
    char nowPram[10];
    while(KeyNext != onepress)
    {
        if(KeyUp == onepress)
        {
            KeyUp = nopress;
            now += 1;
        }
        if(KeyDown == onepress)
        {
            KeyDown = nopress;
            now -= 1;
        }
        if(now < 0)
        now = prams_max;
        if(now >= prams_max)
        now = 0;
        for (uint8_t i = 0; i < 10; i++)
        {
            nowPram[i] = (*prams)[now*10+i];
        }

        ips114_show_string(pos_x,pos_y,(const char*)&nowPram);
    }
    KeyNext = nopress;
    ips114_pencolor = RED;
    *p = now;
    return 1;
}

//按键控制选择菜单
void MenuChoose(uint8 *menu, uint8 maxnum) {
    uint8 numnow = *menu;
    //扫描按键
    if (KeyDown == onepress) {
        KeyDown = nopress;
        if (numnow == maxnum)
            numnow = 0;
        else
            numnow++;
        ips114_clear(YELLOW);
        ips114_show_int8(208, 7, numnow);
    }
    if (KeyUp == onepress) {
        KeyUp = nopress;
        if (numnow == 0)
            numnow = maxnum;
        else
            numnow--;
        ips114_clear(YELLOW);
        ips114_show_int8(208, 7, numnow);
    }
    if (KeyRight == onepress) {
        KeyRight = nopress;
        numnow = maxnum;
        ips114_clear(YELLOW);
        ips114_show_int8(208, 7, numnow);
    }
    if (KeyLeft == onepress) {
        KeyLeft = nopress;
        numnow = 0;
        ips114_clear(YELLOW);
        ips114_show_int8(208, 7, numnow);
    }
    *menu = numnow;
    //设置确认光标
    ips114_pencolor = BLUE;
    if (numnow < 8)
        ips114_show_string(0, numnow, (const char*) (">>"));
    else
        ips114_show_string(0, numnow - 8, (const char*) (">>"));
    ips114_pencolor = RED;
}

uint8 HMIDisplay_Flag = 'F';
/**
 * @brief    进入HMI页面检测
 * @param    None
 * @return   None
 * @Sample   HMIFlagCheck();
 **/
void HMIFlagCheck(void) {
    if (HMIDisplay_Flag == 'F') {
        if (KeyNext == onepress) {
            HMIDisplay_Flag = 'T';
            ips114_clear(YELLOW);
            KeyNext = nopress;
        }
    }
    if (HMIDisplay_Flag == 'T') {
        HMIDisplay();
    }
}

const char Item0[] = "Camera";
const char Item1[] = "Connect"; //含有无线发射的控制
const char Item2[] = "ELC";
const char Item3[] = "ICM20602";
const char Item4[] = "Motor";
const char Item5[] = "Servo";
const char Item6[] = "Encoder";
const char Item7[] = "CarInfo";

#define MAIN_MENU_NUM  7
//HMI选择目标目录 第一级菜单
uint8 HMIDisplay(void) {
    if (HMIDisplay_Flag == 'T') {
        static uint8 NowChooseItem = 0;
        MenuChoose(&NowChooseItem, MAIN_MENU_NUM);
        //显示条目
        ips114_show_string(16, 0, Item0);
        ips114_show_string(16, 1, Item1);
        ips114_show_string(16, 2, Item2);
        ips114_show_string(16, 3, Item3);
        ips114_show_string(16, 4, Item4);
        ips114_show_string(16, 5, Item5);
        ips114_show_string(16, 6, Item6);
        ips114_show_string(16, 7, Item7);
        //绘制间隔线
        ips114_DrawXLine(118, PINK, 3);
        //扫描
        if (KeyNext == onepress) {
            KeyNext = nopress;
            ips114_clear(YELLOW);
            switch (NowChooseItem) {
            case (0):
                CamShow();
                break;
            case (1): //MeetCar_Show();
                break;
            case (2):
                ElcShow();
                break;
            case (3): //ICM20602Show();
                break;
            case (4):
                MotorShow();
                break;
            case (5):
                ServoShow();
                break;
            case (6):
                EncoderShow();
                break;
            case (7):
                CarInfoShow();
                break;
            }
            ips114_clear(YELLOW);
        }
        //按下S1保存参数
        if (KeyLast == onepress) {
            KeyLast = nopress;
//            FlashParamsWrite();
            HMIDisplay_Flag = 'F';
            ips114_clear(BLACK);
            return 0;
        }
        //按下中键打开图像
//        if(KeyCenter == onepress)
//        {
//            KeyCenter = nopress;
//            ips114_clear(YELLOW);
//            CamProcessing();
//        }
    }
    return 0;
}

/***************************摄像头参数调整******************************/

//基于图像坐标 非屏幕坐标
void ImageWritePoint(uint16 x, uint16 y, uint16 color) {
    x >= COL ? x = (COL - 1) : x;
    y >= ROW ? y = (ROW - 1) : y;
    //写点
    ips114_draw_point(x, ROW - 1 - y, color);
}

//绘制垂直于X轴的直线 size是按照中间扩充出去的距离
void ImageDrawXLine(int16 x, uint16 color, uint16 size) {
    int16 _y = 0;
    for (_y = 0; _y < ROW; _y++) {
        if (size == 1)
            ImageWritePoint(x, _y, color);
        else if (size == 3) {
            ImageWritePoint(x - 1, _y, color);
            ImageWritePoint(x, _y, color);
            ImageWritePoint(x + 1, _y, color);
        } else if (size == 5) {
            ImageWritePoint(x - 2, _y, color);
            ImageWritePoint(x - 1, _y, color);
            ImageWritePoint(x, _y, color);
            ImageWritePoint(x + 1, _y, color);
            ImageWritePoint(x + 2, _y, color);
        }
    }
}

//绘制一条垂直于Y轴的直线
void ImageDrawYLine(int16 y, uint16 color, uint16 size) {
    int16 _x = 0;
    for (_x = 0; _x < COL; _x++) {
        if (size == 1)
            ImageWritePoint(_x, y, color);
        else if (size == 3) {
            ImageWritePoint(_x, y - 1, color);
            ImageWritePoint(_x, y, color);
            ImageWritePoint(_x, y + 1, color);
        } else if (size == 5) {
            ImageWritePoint(_x, y - 2, color);
            ImageWritePoint(_x, y - 1, color);
            ImageWritePoint(_x, y, color);
            ImageWritePoint(_x, y + 1, color);
            ImageWritePoint(_x, y + 2, color);
        }
    }
}

//在特定位置显示一个X
void ImageShowX(uint16 x, uint16 y, uint16 color, uint16 size) {
    unsigned short i;
    if (size % 2 == 0)
        size++;
    //中点
    ImageWritePoint(x, y, color);
    //边缘
    for (i = 1; i <= ((size - 1) / 2); i++) {
        ImageWritePoint(x - i, y - i, color);
        ImageWritePoint(x + i, y - i, color);
        ImageWritePoint(x - i, y + i, color);
        ImageWritePoint(x + i, y + i, color);
    }
}

//给图像添加网格
//图像128 * 60   C:64   C:30
void ImgShowGird(void) {
    //横线
    ImageDrawYLine(20, PINK, 1);
    ImageDrawYLine(40, PINK, 1);
    ImageDrawYLine(60, PINK, 1);
    ImageDrawYLine(80, PINK, 1);

    //竖线
    // ImageDrawXLine(14,PINK,1);
    ImageDrawXLine(COL / 2 - RoadWide0 / 2, BLUE, 1);
    ImageDrawXLine(57, GREEN, 3);
    ImageDrawXLine(COL / 2 + RoadWide0 / 2, BLUE, 1);
    //  ImageDrawXLine(100,PINK,1);
    //绘制坐标
    //X
    ips114_pencolor = RED;

    ips114_show_uint8(10, 7, COL / 2 - RoadWide0 / 2);
    ips114_pencolor = BLUE;
    ips114_show_string(49, 7, (const char*) ("57"));
    ips114_pencolor = RED;
    ips114_show_uint8(88, 7, COL / 2 + RoadWide0 / 2);

    ips114_pencolor = BLUE;
    //X
    ips114_show_string(120, 5, (const char*) ("15"));
    ips114_show_string(120, 4, (const char*) ("30"));
    ips114_show_string(120, 3, (const char*) ("45"));
    ips114_show_string(120, 2, (const char*) ("60"));
    ips114_show_string(120, 1, (const char*) ("75"));
    ips114_show_string(120, 0, (const char*) ("90"));
    ips114_pencolor = RED;
}

//绘制四邻域边缘
void ShowLeftAndRightBorder0(void) {
//    //绘制起始搜索列坐标
//    for(Ysite = 0;Ysite <= 10;Ysite ++)
//    {
//        ImageWritePoint(Base.StartSearchColumn_1st,Ysite,CYAN);
//        ImageWritePoint(Base.StartSearchColumn_1st + 1 ,Ysite,CYAN);
//        ImageWritePoint(Base.StartSearchColumn_1st + 2,Ysite,CYAN);
//        ImageWritePoint(Base.StartSearchColumn_1st - 1 ,Ysite,CYAN);
//        ImageWritePoint(Base.StartSearchColumn_1st - 2,Ysite,CYAN);
//    }
//    //绘制
//    //绘制三边
//    for(Ysite = 5;Ysite < ROW - 1;Ysite ++)
//    {
//        // 修复后的中线和边线限幅
////      LIMIT_COL(Img.Center[Ysite],127,0);
//        LIMIT_COL(Img.LeftBorder[Ysite],127,0);
//        LIMIT_COL(Img.RightBorder[Ysite],127,0);
//        ImageWritePoint(Img.LeftBorder[Ysite] + 1,Ysite,BLUE);
//        ImageWritePoint(Img.LeftBorder[Ysite] + 2 ,Ysite,BLUE);
//        ImageWritePoint(Img.LeftBorder[Ysite] + 3,Ysite,BLUE);
//        ImageWritePoint(Img.LeftBorder[Ysite] + 4,Ysite,BLUE);
//        ImageWritePoint(Img.RightBorder[Ysite] - 1,Ysite,BLUE);
//        ImageWritePoint(Img.RightBorder[Ysite] - 2,Ysite,BLUE);
//        ImageWritePoint(Img.RightBorder[Ysite] - 3,Ysite,BLUE);
//        ImageWritePoint(Img.RightBorder[Ysite] - 4,Ysite,BLUE);
//        ImageWritePoint(Img.Center[Ysite],Ysite,RED);
//        ImageWritePoint(Img.Center[Ysite] - 1,Ysite,RED);
//        ImageWritePoint(Img.Center[Ysite] + 1,Ysite,RED);
//    }
}

//显示未修复的边线
void ShowNoRepaiorLine(void) {
    //显示修复后的中线和边线
    for (Ysite = 5; Ysite < Base.Topline; Ysite++) {
        // 修复后的中线和边线限幅
        LIMIT_COL(Img.Center[Ysite], 127, 0);
        LIMIT_COL(Img.LeftBorder[Ysite], 127, 0);
        LIMIT_COL(Img.RightBorder[Ysite], 127, 0);
        ImageWritePoint(Img.LeftBorder[Ysite] + 1, Ysite, BLUE);
        ImageWritePoint(Img.LeftBorder[Ysite] + 2, Ysite, BLUE);
        ImageWritePoint(Img.LeftBorder[Ysite] + 3, Ysite, BLUE);
        ImageWritePoint(Img.LeftBorder[Ysite] + 4, Ysite, BLUE);
        ImageWritePoint(Img.RightBorder[Ysite] - 1, Ysite, BLUE);
        ImageWritePoint(Img.RightBorder[Ysite] - 2, Ysite, BLUE);
        ImageWritePoint(Img.RightBorder[Ysite] - 3, Ysite, BLUE);
        ImageWritePoint(Img.RightBorder[Ysite] - 4, Ysite, BLUE);
        ImageWritePoint(Img.Center[Ysite], Ysite, RED);
        ImageWritePoint(Img.Center[Ysite] - 1, Ysite, RED);
        ImageWritePoint(Img.Center[Ysite] + 1, Ysite, RED);
    }
}

//显示修复的中线 边线
void ShowRepairLine(void) {
    //显示修复后的中线和边线
    for (Ysite = 0; Ysite < ROW; Ysite++) {
        if (Ysite == stack_top || Ysite == stack_top1)
            continue;
if(Img.LeftBorderFindFlag[Ysite]=='T'||Img.RightBorderFindFlag[Ysite]=='T'){
        ImageWritePoint(Img.LeftBorder[Ysite] - 1, Ysite, PURPLE);
        ImageWritePoint(Img.LeftBorder[Ysite], Ysite, PURPLE);
        ImageWritePoint(Img.LeftBorder[Ysite] + 1, Ysite, PURPLE);
        ImageWritePoint(Img.RightBorder[Ysite] - 1, Ysite, BLUE);
        ImageWritePoint(Img.RightBorder[Ysite], Ysite, BLUE);
        ImageWritePoint(Img.RightBorder[Ysite] + 1, Ysite, BLUE);
        ImageWritePoint(Img.Center[Ysite], Ysite, RED);
        ImageWritePoint(Img.Center[Ysite] - 1, Ysite, RED);
        ImageWritePoint(Img.Center[Ysite] + 1, Ysite, RED);
    }}
}
//三岔路口元素显示
void ShowForkFlag(void) {
    ImageShowX(Fork.Peak.x, Fork.Peak.y, RED, 12);
}

//显示理想边线
void ShowExBorder(void) {
    //显示修复后的中线和边线
    for (Ysite = 5; Ysite < ROW; Ysite++) {
        ImageWritePoint((64 - RoadWide0 / 2), Ysite, RED);
        ImageWritePoint((64 - RoadWide0 / 2) + 1, Ysite, RED);
        ImageWritePoint((64 - RoadWide0 / 2) + 2, Ysite, RED);
        ImageWritePoint((64 + RoadWide0 / 2), Ysite, RED);
        ImageWritePoint((64 + RoadWide0 / 2) - 1, Ysite, RED);
        ImageWritePoint((64 + RoadWide0 / 2) - 2, Ysite, RED);
    }
}

//调整阈值
bool adj_flag=0;
uint8 AdjustThreshold(void) {
    while(KeyLast != onepress)
    {adj_flag=1;
        AllImageDeal();

        for(int i=0;i<ipts0_num;i++) {

            ImageWritePoint(ipts0[i][0] - 1, ipts0[i][1], RED);
               ImageWritePoint(ipts0[i][0], ipts0[i][1], RED);
               ImageWritePoint(ipts0[i][0] + 1, ipts0[i][1], RED);
        }
        for(int i=0;i<ipts1_num;i++) {
            ImageWritePoint(ipts1[i][0] - 1, ipts1[i][1], BLUE);
                    ImageWritePoint(ipts1[i][0], ipts1[i][1], BLUE);
                    ImageWritePoint(ipts1[i][0] + 1, ipts1[i][1], BLUE);        }
my_delay(10);

    }
    ips114_clear(YELLOW);
    adj_flag=0;
    KeyLast = nopress;
          return 0;
}

//LCD显示 图像总共处理后显示 包括扫线等等
    uint8 CamProcessing(void) {
        while(KeyLast != onepress)
        {
            AllImageDeal();

//
            //   GetSimBinImage();
            ShowPerImage();
            //  GetBorder();
            //  CenterCal();
            // search_line() ;
            //     cal_middle_line();
            ImgShowGird();
            ShowRepairLine();

            ImageShowX(Cross.LD_Inflexion.x, Cross.LD_Inflexion.y, RED, 10);
            ImageShowX(Cross.LU_Inflexion.x, Cross.LU_Inflexion.y, BLUE, 10);
            ImageShowX(Base.firstleftnormal.x, Base.firstleftnormal.y, GREEN, 10);
            ImageShowX(Cross.RD_Inflexion.x, Cross.RD_Inflexion.y, RED, 10);
            ImageShowX(Cross.RU_Inflexion.x, Cross.RU_Inflexion.y, BLUE, 10);
            ImageShowX(Base.firstrightnormal.x, Base.firstrightnormal.y, GREEN, 10);
            ips114_show_string(150,0,(const char*)("Center:"));
            ips114_show_int16(150,1,point_center);
            ips114_show_string(150,2,(const char*)("TopLine:"));
            ips114_show_int8(150,3,Base.Topline);
            ips114_show_string(150,4,(const char*)("ServoAdd:"));
            ips114_show_int16(150,5,ServoAdd);
            ips114_show_int8(150, 6, Cross.LD_UpAbnormalCnt);
            ips114_show_int8(150, 7, Cross.LU_UpNormalCnt);
            ips114_show_int8(190, 6, Cross.RD_UpAbnormalCnt);
            ips114_show_int8(190, 7, Cross.RU_UpNormalCnt);
            my_delay(30);
        }
        ips114_clear(YELLOW);
        KeyLast = nopress;
        return 0;
    }

//摄像头对角度
    uint8 Cam_sFocus(void) {
        while(KeyLast != onepress)
        {
            //CameraExtract0();
            GetSimBinImage();
            ips114_displayimage032_zoom(&ImageUsed[0][0],COL,ROW,COL,ROW);
            //显示边缘
            ShowExBorder();
            my_delay(30);

        }
        KeyLast = nopress;
        return 0;
    }

//从flash中读取图像处理后显示 包括扫线等等
    uint8 PhotoProcessing(void) {
        while(KeyLast != onepress)
        {
//        ElcRead();
//        ElcNormalized();
//        ElcJudge();
//        ElcCenterCalculate();
            //FLASHTOIMAGE;
            //  GetBorder();
            // CenterCal();
            //Waveform_Write();

            ips114_show_gray_image(0, 0, mt9v03x_image_dvp[0], 188, 120, 188, 120, 0);
            // ImgShowGird();
            //ShowLeftAndRightBorder0();
            //ShowNoRepaiorLine();
            //ShowForkFlag();
            //  ShowRepairLine();
            //ips114_show_string(180,0,(const char*)("Center:"));
            //ips114_show_int16(180,1,point_center);
            my_delay(30);
        }
        KeyLast = nopress;
        return 0;
    }

    const char CamItem0[] = "All";
    const char CamItem1[] = "Threshold";        //含有无线发射的控制
    const char CamItem2[] = "Photo";
    const char CamItem3[] = "Ramp Set";
    const char CamItem4[] = "Barn Set";
    const char CamItem5[] = "Fork Set";
    const char CamItem6[] = "Circle Set";
    const char CamItem7[] = "Normal Set";

#define CAM_MENU_NUM  7

//HMI选择目标目录 第一级菜单
    uint8 CamShow(void) {
        static uint8 CamChooseItem = 0;
        while(KeyLast != onepress)
        {
            MenuChoose(&CamChooseItem,CAM_MENU_NUM);
            //显示条目
            ips114_show_string(16,0,CamItem0);
            ips114_show_string(16,1,CamItem1);
            ips114_show_string(16,2,CamItem2);
            ips114_show_string(16,3,CamItem3);
            ips114_show_string(16,4,CamItem4);
            ips114_show_string(16,5,CamItem5);
            ips114_show_string(16,6,CamItem6);
            ips114_show_string(16,7,CamItem7);
            //绘制间隔线
            ips114_DrawXLine(110,PINK,3);
            //
            if(KeyNext == onepress)
            {
                KeyNext = nopress;
                ips114_clear(YELLOW);
                switch(CamChooseItem)
                {
                    case(0):CamProcessing();
                    break;
                    case(1):AdjustThreshold();
                    break;
                    case(2):PhotoProcessing();
                    break;
                    case(3):;        //Ramp_Show();
                    break;
                    case(4):BarnShow();
                    break;
                    case(5):ForkShow();
                    break;
                    case(6):;CircleShow();
                    break;
                    case(7)://Normal_Show();
                    break;
                }
                ips114_clear(YELLOW);
            }
        }
        KeyLast = nopress;
        //
        return 1;
    }

//图像处理参数

//车库设置
    const char BarnItem0[] = "OutDir";
    const char BarnItem1[] = "InNum";
    const char BarnItem2[] = "StartLineY";
    const char BarnItem3[] = "NoTurnRunDis";
    const char BarnItem4[] = "TurnCenter";
    const char BarnItem5[] = "TurnSpeed";
    const char BarnItem6[] = "InRunDis";

#define BARN_MENU_NUM  5

//HMI选择目标目录 第一级菜单
    uint8 BarnShow(void) {
        static uint8 BarnChooseItem = 0;
        uint8 NowDir = 0;
        while(KeyLast != onepress)
        {
            MenuChoose(&BarnChooseItem,BARN_MENU_NUM);
            //显示条目
            ips114_show_string(16,0,BarnItem0);
            ips114_show_string(16,1,BarnItem1);
            ips114_show_string(16,2,BarnItem2);
            ips114_show_string(16,3,BarnItem3);
            ips114_show_string(16,4,BarnItem4);
            ips114_show_string(16,5,BarnItem5);
            ips114_show_string(16,6,BarnItem6);
            //显示参数
            ips114_pencolor = BLUE;
            if(Barn.OutDir == 'L')
            {
                ips114_show_string(120,0,(const char*)("L"));
                NowDir = 0;
                ips114_show_int16(120,2,BarnInInfo[0][1]);
                ips114_show_int16(120,3,BarnInInfo[0][2]);
                ips114_show_int16(120,4,BarnInInfo[0][3]);
                ips114_show_int16(120,5,BarnInInfo[0][4]);
                ips114_show_int16(120,6,BarnInInfo[0][5]);
            }
            else
            {
                ips114_show_string(120,0,(const char*)("R"));
                NowDir = 1;
                ips114_show_int16(120,2,BarnInInfo[1][1]);
                ips114_show_int16(120,3,BarnInInfo[1][2]);
                ips114_show_int16(120,4,BarnInInfo[1][3]);
                ips114_show_int16(120,5,BarnInInfo[1][4]);
                ips114_show_int16(120,6,BarnInInfo[1][5]);
            }
            ips114_show_int16(120,1,Barn.BranInNum);

            //调整参数
            if(KeyNext == onepress)
            {
                KeyNext = nopress;
                switch(BarnChooseItem)
                {
                    case(0):DirAdjust(&Barn.OutDir,120,0);
                    break;
                    case(1):ParamsAdjustShort((short *)&Barn.BranInNum,120,1,1);
                    break;
                    case(2):ParamsAdjustShort((short *)&BarnInInfo[NowDir][1],120,2,1);
                    break;
                    case(3):ParamsAdjustShort((short *)&BarnInInfo[NowDir][2],120,3,1);
                    break;
                    case(4):ParamsAdjustShort((short *)&BarnInInfo[NowDir][3],120,4,1);
                    break;
                    case(6):ParamsAdjustShort((short *)&BarnInInfo[NowDir][4],120,5,1);
                    break;
                    case(5):ParamsAdjustShort((short *)&BarnInInfo[NowDir][5],120,6,1);
                    break;

                }
                ips114_clear(YELLOW);
            }
        }
        KeyLast = nopress;
        //
        return 1;
    }

//三岔设置

//设置三岔参数
    const char ForkxItem0[] = "Dir";
    const char ForkxItem1[] = "InTurn K";
    const char ForkxItem2[] = "OutTurn K";

#define FORKx_MENU_NUM  2

//三岔二级菜单
    uint8 ForkParamsAdjust(uint32 num) {
        static uint8 ForkxChooseItem = 0;
        uint8 x0dir;
        x0dir = (uint8) ForkInfo[num][0];
        ips114_clear(YELLOW);
        while(KeyLast != onepress)
        {
            MenuChoose(&ForkxChooseItem,FORKx_MENU_NUM);
            //显示条目
            ips114_pencolor = RED;
            ips114_show_string(16,0,ForkxItem0);
            ips114_show_string(16,1,ForkxItem1);
            ips114_show_string(16,2,ForkxItem2);
            //显示参数
            ips114_pencolor = BLUE;
            if(x0dir == 'L')
            ips114_show_string(120,0,(const char*)("L"));
            else
            ips114_show_string(120,0,(const char*)("R"));
            ips114_show_int16(120,1,ForkInfo[num][1]);
            ips114_show_int16(120,2,ForkInfo[num][2]);
            //调整参数
            if(KeyNext == onepress)
            {
                KeyNext = nopress;
                switch(ForkxChooseItem)
                {
                    case(0):DirAdjust(&x0dir,120,0);
                    break;
                    case(1):ParamsAdjustUint32(&ForkInfo[num][1],120,1,1);
                    break;
                    case(2):ParamsAdjustUint32(&ForkInfo[num][2],120,2,1);
                    break;
                }
                ips114_clear(YELLOW);
                ForkInfo[num][0] = x0dir;
            }
        }
        ips114_clear(YELLOW);
        KeyLast = nopress;
        //
        return 1;
    }

//三岔设置
    const char ForkItem0[] = "Amount";
    const char ForkItem1[] = "In Speed";
    const char ForkItem2[] = "Out Speed";
    const char ForkItem3[] = "Fork 1";
    const char ForkItem4[] = "Fork 2";
    const char ForkItem5[] = "Fork 3";
    const char ForkItem6[] = "Fork 4";

#define FORK_MENU_NUM  6

//三岔菜单
    uint8 ForkShow(void) {
        static uint8 ForkChooseItem = 0;
        while(KeyLast != onepress)
        {
            MenuChoose(&ForkChooseItem,FORK_MENU_NUM);
            //显示条目
            ips114_pencolor = RED;
            ips114_show_string(16,0,ForkItem0);
            ips114_show_string(16,1,ForkItem1);
            ips114_show_string(16,2,ForkItem2);
            ips114_show_string(16,3,ForkItem3);
            ips114_show_string(16,4,ForkItem4);
            ips114_show_string(16,5,ForkItem5);
            ips114_show_string(16,6,ForkItem6);
            //显示参数
            ips114_pencolor = BLUE;
            ips114_show_int16(120,0,Fork.ForkAmount);
            ips114_show_int16(120,1,Fork.InSpeed);
            ips114_show_int16(120,2,Fork.OutSpeed);
            //调整参数
            if(KeyNext == onepress)
            {
                KeyNext = nopress;
                switch(ForkChooseItem)
                {
                    case(0):ParamsAdjustShort((short *)&Fork.ForkAmount,120,0,1);
                    break;
                    case(1):ParamsAdjustShort((short *)&Fork.InSpeed,120,0,1);
                    break;
                    case(2):ParamsAdjustShort((short *)&Fork.OutSpeed,120,0,1);
                    break;
                    case(3):ForkParamsAdjust(0);
                    break;
                    case(4):ForkParamsAdjust(1);
                    break;
                    case(5):ForkParamsAdjust(2);
                    break;
                }
                ips114_clear(YELLOW);
            }
        }
        KeyLast = nopress;
        //
        return 1;
    }

//环岛部分
//设置环岛参数
    const char CirclexItem0[] = "Dir";
    const char CirclexItem1[] = "Circle R";
    const char CirclexItem2[] = "Center ELC";
    const char CirclexItem3[] = "Turn Dis";
    const char CirclexItem4[] = "InTurnUP_Y";
    const char CirclexItem5[] = "InTurn K";
    const char CirclexItem6[] = "TurnSpeed";
    const char CirclexItem7[] = "InSpeed";
    const char CirclexItem8[] = "OutTurnK";

#define Circlex_MENU_NUM  8

//环岛二级菜单
    uint8 CircleParamsAdjust(uint32 num) {
        static uint8 CirclexChooseItem = 0;
        uint8 x1dir;
        x1dir = (uint8) CircleInfo[num][0];
        ips114_clear(YELLOW);
        while(KeyLast != onepress)
        {
            MenuChoose(&CirclexChooseItem,Circlex_MENU_NUM);
            //显示条目
            ips114_pencolor = RED;
            if(CirclexChooseItem <= 7)
            {
                ips114_show_string(16,0,CirclexItem0);
                ips114_show_string(16,1,CirclexItem1);
                ips114_show_string(16,2,CirclexItem2);
                ips114_show_string(16,3,CirclexItem3);
                ips114_show_string(16,4,CirclexItem4);
                ips114_show_string(16,5,CirclexItem5);
                ips114_show_string(16,6,CirclexItem6);
                ips114_show_string(16,7,CirclexItem7);
            }
            else
            {
                ips114_show_string(16,0,CirclexItem8);
            }

            //显示参数
            ips114_pencolor = BLUE;
            if(CirclexChooseItem <= 7)
            {
                if(x1dir == 'L')
                ips114_show_string(120,0,(const char*)("L"));
                else
                ips114_show_string(120,0,(const char*)("R"));
                ips114_show_int16(120,1,CircleInfo[num][1]);
                ips114_show_int16(120,2,CircleInfo[num][2]);
                ips114_show_int16(120,3,CircleInfo[num][3]);
                ips114_show_int16(120,4,CircleInfo[num][4]);
                ips114_show_int16(120,5,CircleInfo[num][5]);
                ips114_show_int16(120,6,CircleInfo[num][6]);
                ips114_show_int16(120,7,CircleInfo[num][7]);
            }
            else
            {
                ips114_show_int16(120,0,CircleInfo[num][8]);
            }
            //调整参数
            if(KeyNext == onepress)
            {
                KeyNext = nopress;
                switch(CirclexChooseItem)
                {
                    case(0):DirAdjust(&x1dir,120,0);
                    break;
                    case(1):ParamsAdjustUint32(&CircleInfo[num][1],120,1,1);
                    break;
                    case(2):ParamsAdjustUint32(&CircleInfo[num][2],120,2,1);
                    break;
                    case(3):ParamsAdjustUint32(&CircleInfo[num][3],120,3,1);
                    break;
                    case(4):ParamsAdjustUint32(&CircleInfo[num][4],120,4,1);
                    break;
                    case(5):ParamsAdjustUint32(&CircleInfo[num][5],120,5,1);
                    break;
                    case(6):ParamsAdjustUint32(&CircleInfo[num][6],120,6,1);
                    break;
                    case(7):ParamsAdjustUint32(&CircleInfo[num][7],120,7,1);
                    break;
                    case(8):ParamsAdjustUint32(&CircleInfo[num][8],120,0,1);
                    break;
                }
                ips114_clear(YELLOW);
                CircleInfo[num][0] = x1dir;
            }
        }
        ips114_clear(YELLOW);
        KeyLast = nopress;
        //
        return 1;
    }

//调参部分

//环岛主菜单
    const char CircleItem0[] = "CircleAmount";
    const char CircleItem1[] = "Judge ELC14";
    const char CircleItem2[] = "Judge ELC23";
    const char CircleItem3[] = "FindOutSpeed";
    const char CircleItem4[] = "Circle 1";
    const char CircleItem5[] = "Circle 2";
    const char CircleItem6[] = "Circle 3";
    const char CircleItem7[] = "Circle 4";

#define CIRCLE_MENU_NUM  7

//环岛菜单
    uint8 CircleShow(void) {
        static uint8 CircleChooseItem = 0;
        while(KeyLast != onepress)
        {
            MenuChoose(&CircleChooseItem,CIRCLE_MENU_NUM);
            //显示条目
            ips114_pencolor = RED;
            ips114_show_string(16,0,CircleItem0);
            ips114_show_string(16,1,CircleItem1);
            ips114_show_string(16,2,CircleItem2);
            ips114_show_string(16,3,CircleItem3);
            ips114_show_string(16,4,CircleItem4);
            ips114_show_string(16,5,CircleItem5);
            ips114_show_string(16,6,CircleItem6);
            ips114_show_string(16,7,CircleItem7);
            //显示参数
            ips114_pencolor = BLUE;
            ips114_show_int16(120,0,Circle.CircleAmount);
            ips114_show_int16(120,1,Circle.CircleELC14);
            ips114_show_int16(120,2,Circle.CircleELC23);
            ips114_show_int16(120,3,Circle.FoundOutSpeed);
            //调整参数
            if(KeyNext == onepress)
            {
                KeyNext = nopress;
                switch(CircleChooseItem)
                {
                    case(0):ParamsAdjustShort((short *)&Circle.CircleAmount,120,0,1);
                    break;
                    case(1):ParamsAdjustShort((short *)&Circle.CircleELC14,120,1,1);
                    break;
                    case(2):ParamsAdjustShort((short *)&Circle.CircleELC23,120,2,1);
                    break;
                    case(3):ParamsAdjustShort((short *)&Circle.FoundOutSpeed,120,2,1);
                    break;
                    case(4):CircleParamsAdjust(0);
                    break;
                    case(5):CircleParamsAdjust(1);
                    break;
                    case(6):;
                    break;
                    case(7):;
                    break;
                }
                ips114_clear(YELLOW);
            }
        }
        KeyLast = nopress;
        //
        return 1;

    }

////坡道设置
//const char RampItem0[] = "GyroYMax";
//const char RampItem1[] = "UpSpeed";
//const char RampItem2[] = "DownSpeed";
//const char RampItem3[] = "Fork 1";
//const char RampItem4[] = "Fork 2";
//const char RampItem5[] = "Fork 3";
//const char RampItem6[] = "Fork 4";

//#define RAMP_MENU_NUM  6

////三岔菜单
//uint8 RampShow(void)
//{
//  static int8 RampChooseItem = 0;
//  while(KeyLast != onepress)
//  {
//      MenuChoose(&RampChooseItem,Ramp_MENU_NUM);
//      //显示条目
//      ips114_pencolor = RED;
//      ips114_show_string(16,0,RampItem0);
//      ips114_show_string(16,1,RampItem1);
//      ips114_show_string(16,2,RampItem2);
//      ips114_show_string(16,3,RampItem3);
//      ips114_show_string(16,4,RampItem4);
//      ips114_show_string(16,5,RampItem5);
//      ips114_show_string(16,6,RampItem6);
//      //显示参数
//      ips114_pencolor = BLUE;
//      ips114_show_int16(120,0,Ramp.RampAmount);
//      ips114_show_int16(120,1,Ramp.InSpeed);
//      ips114_show_int16(120,2,Ramp.OutSpeed);
//      //调整参数
//      if(KeyNext == onepress)
//      {
//          KeyNext = nopress;
//          switch(RampChooseItem)
//          {
//              case(0):ParamsAdjustShort(&Ramp.RampAmount,120,0,1);1`` 4
//                  break;
//              case(1):ParamsAdjustShort(&Ramp.InSpeed,120,0,1);
//                  break;
//              case(2):ParamsAdjustShort(&Ramp.OutSpeed,120,0,1);
//                  break;
//              case(3):Ramp_ParamsAdjust(0);
//                  break;
//              case(4):Ramp_ParamsAdjust(1);
//                  break;
//              case(5):Ramp_ParamsAdjust(2);
//                  break;
//          }
//          ips114_clear(YELLOW);
//      }
//  }
//  KeyLast = nopress;
//  //
//  return 1;
//}

