/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_device_type
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             
********************************************************************************************************************/

#include "zf_device_type.h"


camera_type_enum    camera_type     = NO_CAMERE;                                // 摄像头类型变量
wireless_type_enum  wireless_type   = NO_WIRELESS;

void set_camera_type (camera_type_enum type_set)
{
    camera_type = type_set;
}

void set_wireless_type (camera_type_enum type_set)
{
    wireless_type = type_set;
}

