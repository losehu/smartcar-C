/**
 //@FileName	:display.h
 //@CreatedDate	:2021年12月22日
 //@Author		:LiHao
 //@Description	:
**/

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "zf_common_headfile.h"


//statement
extern uint8      HMIDisplay_Flag;



//fun
void ImageWritePoint(uint16 x,uint16 y,uint16 color);//图像坐标
void ImageDrawXLine(int16 x,uint16 color,uint16 size);
void ImageDrawYLine(int16 y,uint16 color,uint16 size);
void ImageShowX(uint16 x,uint16 y,uint16 color,uint16 size);
void ImgShowGird(void);
void ShowLeftAndRightBorder0(void);
void ShowNoRepaiorLine(void);
void ShowRepairLine(void);
void MenuChoose(uint8 *menu,uint8 maxnum);

void HMIFlagCheck(void);
uint8 HMIDisplay(void);
void  ShowForkFlag(void);
uint8 ParamsAdjustFloat(float *p,uint16 pos_x,uint16 pos_y,float step);
uint8 ParamsAdjustShort(short *p,uint16 pos_x,uint16 pos_y,short step);
uint8 StateAdjust(uint8 *p,uint16 pos_x,uint16 pos_y);
uint8 EnumAdjust(uint8 *p,char (*prams)[],uint8 prams_max,uint16 pos_x,uint16 pos_y);
uint8 CamShow(void);
uint8 CamProcessing(void);
void ShowPerImage();

uint8 BarnShow(void);
uint8 ForkShow(void);
uint8 CircleShow(void);
#endif /* DISPLAY_H_ */
