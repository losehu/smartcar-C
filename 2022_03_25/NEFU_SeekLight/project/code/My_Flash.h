/**
 //@FileName	:My_Flash.h
 //@CreatedDate	:2022��1��1��
 //@Author		:LiHao
 //@Description	:
**/

#ifndef MY_FLASH_H_
#define MY_FLASH_H_

#include "zf_common_headfile.h"

extern uint32 ParamsNow[255];
uint8 FlashParamsWrite(void);
uint8 FlashParamsRead(void);

#endif /* MY_FLASH_H_ */
