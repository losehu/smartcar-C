/**
 //@FileName	:My_Flash.c
 //@CreatedDate	:2022年1月1日
 //@Author		:LiHao
 //@Description	:
**/

#include "My_Flash.h"

/******************************************  参数写入以及读取  ******************************************/

//参数设置u32型的参数
uint32 ParamsNow[255];

/**
*@Name			:FlashParamsWrite
*@Description 	:FlashParamsWrite Flash参数写入
*@Param			:None
*@Return		:0 成功; 1 失败
*@Sample		:FlashParamsWrite();
**/
uint8 FlashParamsWrite(void)
{
    //显示界面
    ips114_clear(YELLOW);
    ips114_pencolor = RED;
    ips114_show_string(40,3,(const int8*)("Start Write Params !"));
    my_delay(300);
    //读取各个参数准备写flash
    //////////系统控制////////////
    ParamsNow[0] = CarInfo.ReadFlashParams;
    ParamsNow[1] = CarInfo.CAR_PROTECT_MODE;
    ParamsNow[2] = CarInfo.ClosedLoopMode;
    ////////////交接控制////////////
//    ParamsNow[3] = MeetCar.MeetEnable;
//    ParamsNow[4] = MeetCar.ForkNum;
//    ParamsNow[5] = (uint32)(MeetCar.ReduceSpeedDis * 1000);
//    ParamsNow[6] = MeetCar.ReduceSpeed;
//    ParamsNow[7] = (uint32)(MeetCar.MeetForwardDis * 1000);
    ////////////图像阈值///////////
    ParamsNow[8] = ThresholdAdd;
    /////////////车库参数///////////
    ParamsNow[9]  = Barn.OutDir;
    ParamsNow[10] = Barn.BranInNum;

    ParamsNow[11] = BarnInInfo[0][0];
    ParamsNow[12] = BarnInInfo[0][1];
    ParamsNow[13] = BarnInInfo[0][2];
    ParamsNow[14] = BarnInInfo[0][3];
    ParamsNow[15] = BarnInInfo[0][4];
    ParamsNow[16] = BarnInInfo[0][5];
    ParamsNow[17] = BarnInInfo[1][0];
    ParamsNow[18] = BarnInInfo[1][1];
    ParamsNow[19] = BarnInInfo[1][2];
    ParamsNow[20] = BarnInInfo[1][3];
    ParamsNow[21] = BarnInInfo[1][4];
    ParamsNow[22] = BarnInInfo[1][5];
    ////////////三岔参数///////////
    ParamsNow[23] = Fork.ForkAmount;
    ParamsNow[24] = Fork.InSpeed;
    ParamsNow[25] = Fork.OutSpeed;

    ParamsNow[26] = ForkInfo[0][0];
    ParamsNow[27] = ForkInfo[0][1];
    ParamsNow[28] = ForkInfo[0][2];
    ParamsNow[29] = ForkInfo[1][0];
    ParamsNow[30] = ForkInfo[1][1];
    ParamsNow[31] = ForkInfo[1][2];
    ParamsNow[32] = ForkInfo[2][0];
    ParamsNow[33] = ForkInfo[2][1];
    ParamsNow[34] = ForkInfo[2][2];
    ParamsNow[35] = ForkInfo[3][0];
    ParamsNow[36] = ForkInfo[3][1];
    ParamsNow[37] = ForkInfo[3][2];
    ////////////环岛参数///////////
    ParamsNow[38] = Circle.CircleAmount;
    ParamsNow[39] = Circle.CircleELC14;
    ParamsNow[40]= Circle.CircleELC23;
    ParamsNow[41] = Circle.FoundOutSpeed;

    ParamsNow[42] = CircleInfo[0][0];
    ParamsNow[43] = CircleInfo[0][1];
    ParamsNow[44] = CircleInfo[0][2];
    ParamsNow[45] = CircleInfo[0][3];
    ParamsNow[46] = CircleInfo[0][4];
    ParamsNow[47] = CircleInfo[0][5];
    ParamsNow[48] = CircleInfo[0][6];
    ParamsNow[49] = CircleInfo[0][7];
    ParamsNow[50] = CircleInfo[0][8];
    ParamsNow[51] = CircleInfo[1][0];
    ParamsNow[52] = CircleInfo[1][1];
    ParamsNow[53] = CircleInfo[1][2];
    ParamsNow[54] = CircleInfo[1][3];
    ParamsNow[55] = CircleInfo[1][4];
    ParamsNow[56] = CircleInfo[1][5];
    ParamsNow[57] = CircleInfo[1][6];
    ParamsNow[58] = CircleInfo[1][7];
    ParamsNow[59] = CircleInfo[1][8];




    //写flash
    //128*60/32 = 240 < 256
    flash_erase_page(FLASH_SECTION_15, FLASH_PAGE_3);
    if(!flash_page_program(FLASH_SECTION_15, FLASH_PAGE_3, &ParamsNow[0], 255))
    {
        ips114_clear(GREEN);
        ips114_bgcolor = GREEN;
        ips114_show_string(112,3,(const int8*)("OK!"));
        ips114_bgcolor = YELLOW;
        my_delay(300);
        ips114_clear(YELLOW);
        return 1;
    }
    else
    {
        ips114_clear(BLACK);
        ips114_bgcolor = BLACK;
        ips114_show_string(112,3,(const int8*)("Fail!"));
        ips114_bgcolor = YELLOW;
        my_delay(300);
        ips114_clear(YELLOW);
        return 0;
    }
}
/**
*@Name			:FlashParamsRead
*@Description 	:FlashParamsRead
*@Param			:None
*@Return		:1 成功; 0 失败
*@Sample		:FlashParamsRead();
**/
uint8 FlashParamsRead(void)
{
    //读flash
    flash_page_read(FLASH_SECTION_15, FLASH_PAGE_2,&ParamsNow[0], 255);
    //写各个参数
    CarInfo.ReadFlashParams = ParamsNow[0];
    if(CarInfo.ReadFlashParams != 'T')
        return 0;//人为限制不允许读取
    /////////系统控制/////////
    CarInfo.CAR_PROTECT_MODE = ParamsNow[1];
    CarInfo.ClosedLoopMode = ParamsNow[2];
    ////////////交接控制////////////
//    MeetCar.MeetEnable = ParamsNow[3];
//    MeetCar.ForkNum = ParamsNow[4];
//    MeetCar.ReduceSpeedDis = ParamsNow[5];
//        MeetCar.ReduceSpeedDis /= 1000;
//    MeetCar.ReduceSpeed = ParamsNow[6];
//    MeetCar.MeetForwardDis = ParamsNow[7];
//        MeetCar.MeetForwardDis /= 1000;
    //摄像头阈值
    ThresholdAdd = ParamsNow[8];
    ////////////车库参数///////////
    Barn.OutDir =  ParamsNow[9];
    Barn.BranInNum =  ParamsNow[10];
    BarnInInfo[0][0] = ParamsNow[11];
    BarnInInfo[0][1] = ParamsNow[12];
    BarnInInfo[0][2] = ParamsNow[13];
    BarnInInfo[0][3] = ParamsNow[14];
    BarnInInfo[0][4] = ParamsNow[15];
    BarnInInfo[0][5] = ParamsNow[16];
    BarnInInfo[1][0] = ParamsNow[17];
    BarnInInfo[1][1] = ParamsNow[18];
    BarnInInfo[1][2] = ParamsNow[19];
    BarnInInfo[1][3] = ParamsNow[20];
    BarnInInfo[1][4] = ParamsNow[21];
    BarnInInfo[1][5] = ParamsNow[22];
    ///////////三岔参数///////////
    Fork.ForkAmount = ParamsNow[23];
    Fork.InSpeed = ParamsNow[24];
    Fork.OutSpeed = ParamsNow[25];
    ForkInfo[0][0] = ParamsNow[26];
    ForkInfo[0][1] = ParamsNow[27];
    ForkInfo[0][2] = ParamsNow[28];
    ForkInfo[1][0] = ParamsNow[29];
    ForkInfo[1][1] = ParamsNow[30];
    ForkInfo[1][2] = ParamsNow[31];
    ForkInfo[2][0] = ParamsNow[32];
    ForkInfo[2][1] = ParamsNow[33];
    ForkInfo[2][2] = ParamsNow[34];
    ForkInfo[3][0] = ParamsNow[35];
    ForkInfo[3][1] = ParamsNow[36];
    ForkInfo[3][2] = ParamsNow[37];
    ////////////环岛参数///////////
    Circle.CircleAmount = ParamsNow[38];
    Circle.CircleELC14 = ParamsNow[39];
    Circle.CircleELC23 = ParamsNow[40];
    Circle.FoundOutSpeed = ParamsNow[41];

    CircleInfo[0][0] = ParamsNow[42];
    CircleInfo[0][1] = ParamsNow[43];
    CircleInfo[0][2] = ParamsNow[44];
    CircleInfo[0][3] = ParamsNow[45];
    CircleInfo[0][4] = ParamsNow[46];
    CircleInfo[0][5] = ParamsNow[47];
    CircleInfo[0][6] = ParamsNow[48];
    CircleInfo[0][7] = ParamsNow[49];
    CircleInfo[0][8] = ParamsNow[50];
    CircleInfo[1][0] = ParamsNow[51];
    CircleInfo[1][1] = ParamsNow[52];
    CircleInfo[1][2] = ParamsNow[53];
    CircleInfo[1][3] = ParamsNow[54];
    CircleInfo[1][4] = ParamsNow[55];
    CircleInfo[1][5] = ParamsNow[56];
    CircleInfo[1][6] = ParamsNow[57];
    CircleInfo[1][7] = ParamsNow[58];
    CircleInfo[1][8] = ParamsNow[59];
    return 1;
}
