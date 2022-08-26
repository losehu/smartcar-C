/**
 //@FileName	:ANO_DT.h
 //@CreatedDate	:2021年12月19日
 //@Author		:LiHao
 //@Description	:
**/

#ifndef ANO_DT_H_
#define ANO_DT_H_

#include "zf_common_headfile.h"

//#define WIRELESS_MODE        2//1为调参 2为通信
//#define ANO_UART_BAUD        115200
//#define ANO_UART_DMA_CHANNEL DMA1_Channel2
//#define ANO_UART_DMA_IRQn    DMA1_Channel2_IRQn

//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送//8位1字节，float4字节，double8字节
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

//目标识别命令
#define MYHWADDR    0x05    //拓空者
#define SWJADDR     0xAF

//波形数据
#define USERDATA_1   0
#define USERDATA_2   1
#define USERDATA_3   2
#define USERDATA_4   3
#define USERDATA_5   4
#define USERDATA_6   5
#define USERDATA_7   6
#define USERDATA_8   7
#define USERDATA_9   8
#define USERDATA_10  9
#define USERDATA_11  10
#define USERDATA_12  11
#define USERDATA_13  12
#define USERDATA_14  13
#define USERDATA_15  14
#define USERDATA_16  15
#define USERDATA_17  16
#define USERDATA_18  17
#define USERDATA_19  18
#define USERDATA_20  19

//波形数据存储
extern short waveform[20];
extern unsigned char Waveform_Flag;

void WaveformWrite(void);
void SendWaveform(void);

#endif /* ANO_DT_H_ */
