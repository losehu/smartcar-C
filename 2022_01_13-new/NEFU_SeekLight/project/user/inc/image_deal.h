/**
 //@FileName	:image_deal.h
 //@CreatedDate	:2021年12月31日
 //@Author		:LiHao&WuSheng
 //@Description	:
 **/

#ifndef IMAGE_DEAL_H_
#define IMAGE_DEAL_H_

#include "zf_common_headfile.h"

//def
//通过ImageBuffer剪切后的图像行列数
#define         USED_ROW                120//80
#define         USED_COL                188//128
//透视变换后图像行列数
#define         TRFED_ROW               100
#define         TRFED_COL               114
//最终图像均使用ROW和COL作为行列数，在下面修改
#define         ROW                     100//USED_ROW|TRFED_ROW //从远到进为0~ROW
#define         COL                     114//USED_COL|TRFED_COL

#define CamImage    mt9v03x_image_dvp
#define PER_IMG     SimBinImage//CamImage//定义用于透视变换的图像
#define ImageUsed   *PerImg_ip//*PerImg_ip//SimBinImage//定义使用的图像，ImageUsed为用于巡线和识别的图像，在cam.c中使用

//param
//extern uint8_t CamImage[USED_ROW][USED_COL];//总钻风获取的灰度图像，由ImageBuffer()获取或者宏定义
extern uint8_t SimBinImage[USED_ROW][USED_COL]; //简易无逆透视二值化图像，由GetSimBinImage()获取
extern uint8_t AdpBinImage[USED_ROW][USED_COL]; //自适应阈值二值化图像，
//extern uint8_t PerImage[TRFED_ROW][TRFED_COL];//Transformed_Image透视变换后的图像
extern int16_t ThresholdAdd; //对大津法得到的阈值进行手动调节
extern uint8_t *PerImg_ip[TRFED_ROW][TRFED_COL]; //透视变换映射地址 指针数组//直接使用*PerImg_ip即可

//fun
void ImageBuffer(void);
void GetSimBinImage(void);
void GetAdpBinImage(short block0, short block1);
//void ShowPerImage(void);
#define ShowPerImage()  ips114_show_gray_image_vec(0,0,PerImg_ip,TRFED_COL,TRFED_ROW,TRFED_COL,TRFED_ROW,0)
void ImagePerspective_Init(void);
uint8_t OSTUThreshold(uint8_t img[USED_ROW][USED_COL]);

/********八领域***********/
typedef struct  {
    int16 x0;
    int16 y0;
}size_point;

extern   size_point stack_seed[500];    //栈
extern int16 stack_top ;
extern int16 left_line[ROW];
extern int16 right_line[ROW];
void pull_stack(int16 x, int16 y, uint8_t img_tmp[][COL]);    //入栈
 size_point push_stack();    //出栈
void search_line(void);
void cal_middle_line(void);

#endif /* IMAGE_DEAL_H_ */
