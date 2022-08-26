/**
 //@FileName    :My_Flash.c
 //@CreatedDate :2022年1月1日
 //@Author      :LiHao
 //@Description :
**/

#include "My_Flash.h"

#define UserFlashSection FLASH_SECTION_44
#define UserFlashPage    FLASH_PAGE_1

#define UserFlashAddr0    0x08047c00//0x08018f00//0x08019e00
#define UserFlashAddr1
/******************************************  参数写入以及读取  ******************************************/

//参数设置u32型的参数
uint32 ParamsNow1[64];

/**
*@Name          :FlashParamsWrite
*@Description   :FlashParamsWrite Flash参数写入
*@Param         :None
*@Return        :0 成功; 1 失败
*@Sample        :FlashParamsWrite();
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
    ////////////控制参数////////////
    ParamsNow[3] = NormalSpeed;
    ParamsNow[4] = ServoPIDParam[0][0] * 100;
    ParamsNow[5] = ServoPIDParam[0][1] * 100;
    ParamsNow[6] = ServoPIDParam[0][2] * 100;
    ParamsNow[7] = ServoPIDParam[0][3] * 100;
    ////////////图像阈值///////////
    ParamsNow[8] = ThresholdAdd;
    /////////////车库参数///////////
    ParamsNow[9]  = Barn.OutDir;
    ParamsNow[10] = Barn.BranInNum;

    ParamsNow[11] = BarnInInfo[0][0];
    ParamsNow[12] = BarnInInfo[0][1];

    ParamsNow[13] = CenterCal;
    ParamsNow[14] = CenterCal;
//    ParamsNow[13] = BarnInInfo[0][2];
//    ParamsNow[14] = BarnInInfo[0][3];
//    ParamsNow[15] = BarnInInfo[0][4];
//    ParamsNow[16] = BarnInInfo[0][5];
    ParamsNow[17] = BarnInInfo[1][0];
    ParamsNow[18] = BarnInInfo[1][1];
//    ParamsNow[19] = BarnInInfo[1][2];
//    ParamsNow[20] = BarnInInfo[1][3];
//    ParamsNow[21] = BarnInInfo[1][4];
//    ParamsNow[22] = BarnInInfo[1][5];
    ////////////三岔参数///////////
    ParamsNow[23] = Fork.ForkAmount;
//    ParamsNow[24] = Fork.InSpeed;
//    ParamsNow[25] = Fork.OutSpeed;

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
//    ParamsNow[38] = Circle.CircleAmount;
//    ParamsNow[39] = Circle.CircleELC14;
//    ParamsNow[40]= Circle.CircleELC23;
//    ParamsNow[41] = Circle.FoundOutSpeed;
//
//    ParamsNow[42] = CircleInfo[0][0];
//    ParamsNow[43] = CircleInfo[0][1];
//    ParamsNow[44] = CircleInfo[0][2];
//    ParamsNow[45] = CircleInfo[0][3];
//    ParamsNow[46] = CircleInfo[0][4];
//    ParamsNow[47] = CircleInfo[0][5];
//    ParamsNow[48] = CircleInfo[0][6];
//    ParamsNow[49] = CircleInfo[0][7];
//    ParamsNow[50] = CircleInfo[0][8];
//    ParamsNow[51] = CircleInfo[1][0];
//    ParamsNow[52] = CircleInfo[1][1];
//    ParamsNow[53] = CircleInfo[1][2];
//    ParamsNow[54] = CircleInfo[1][3];
//    ParamsNow[55] = CircleInfo[1][4];
//    ParamsNow[56] = CircleInfo[1][5];
//    ParamsNow[57] = CircleInfo[1][6];
//    ParamsNow[58] = CircleInfo[1][7];
//    ParamsNow[59] = CircleInfo[1][8];

    //写flash
    //128*60/32 = 240 < 256
    FLASH_Unlock_Fast();
    FLASH_ErasePage_Fast(UserFlashAddr0);
    FLASH_ProgramPage_Fast(UserFlashAddr0, &ParamsNow[0]);
    FLASH_Lock_Fast();
//    for(uint16 i = 0; i < 64; i++) {                                                // 根据指定长度读取{
//        ParamsNow1[i] = *(__IO uint32_t*)(UserFlashAddr0+i*4);                                             // 循环读取 Flash 的值
//    }

    ips114_clear(GREEN);
    ips114_bgcolor = GREEN;
    ips114_show_string(112,3,(const int8*)("OK!"));
    ips114_bgcolor = YELLOW;
    my_delay(300);
    ips114_clear(YELLOW);
    return 1;

}
/**
*@Name          :FlashParamsRead
*@Description   :FlashParamsRead
*@Param         :None
*@Return        :1 成功; 0 失败
*@Sample        :FlashParamsRead();
**/
uint8 FlashParamsRead(void)
{
}


//uint32 ImgZip0[(ROW * COL)/32 + 1];
///**
//*@Name            :ImageZipToFlash
//*@Description     :压缩图片到Flash
//*@Param           :ImgSrc:源图片地址
//*@Return      :
//*@Sample      :
//**/
//uint8 ImageZipToFlash(uint8 *ImgSrc_P)
//{
//    //转换
//    int16 i = 0,j = 0, k = 0;
//    uint32 *ImgZipTempPointer = &ImgZip0;//将被写进flash里的数组的某字的地址
//    uint8 ImgSrcTempData[32];//原图片某32字节的数据
//    while (i < ROW * COL) {
//        for (j = 0; j < 32; j++) {
//            if (i < ROW * COL) {
//                ImgSrcTempData[j] = *ImgSrc_P;
//                ImgSrc_P++;
//                i++;
//            }
//            else {
//                ImgSrcTempData[j] = 0;
//                ImgSrc_P++;
//                i++;
//            }
//        }
//        for (k = 0; k < 32; k++) {
//            if(ImgSrcTempData[k] == 255)
//                *ImgZipTempPointer |= (0x80000000 >> k);
//            else if(ImgSrcTempData[k] == 0)
//                *ImgZipTempPointer &= (0x7FFFFFFF >> k);
//        }
//    }
//
//    //写flash
//    //128*60/32 = 240 < 256
//    flash_erase_page(UserFlashSection, UserFlashPage);
//    if(flash_write_page(UserFlashSection, UserFlashPage, &ImgZip0[0][0], 240))
//        return 1;
//    else
//        return 0;
//}
//
//
////摄像头图像解压
//uint8 ImageUzipToSram(uint8 *imgsrc1)
//{
//    uint32 *imgzipFlash1;
//    int16 i,j,k;
//    uint32 temp = 0;
//    uint8* temp0;
//    //flash -> sram
//    flash_page_read(UserFlashSection, UserFlashPage,&ImgZip0[0][0], 240);
//    //解压图像
//    imgzipFlash1 = &ImgZip0[0][0];
//    for(i=0;i<ROW;i++)
//    {
//        for(j=0;j < (COL/8/4 + 1);j ++)
//        {
//            temp = *imgzipFlash1;
//            imgzipFlash1++;
//            for(k = 0; k < 32; k++)
//            {
//                temp0 = imgsrc1;
//                imgsrc1 ++;
//                if( (temp >> k) & 0x00000001 )//图像先读取低位
//                    *temp0 = 255;
//                else
//                    *temp0 = 0;
//            }
//        }
//    }
//    return 1;
//}
