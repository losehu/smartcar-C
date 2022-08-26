/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_type
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             
********************************************************************************************************************/

#include "zf_device_type.h"


camera_type_enum    camera_type     = NO_CAMERE;                                // ����ͷ���ͱ���
wireless_type_enum  wireless_type   = NO_WIRELESS;

void set_camera_type (camera_type_enum type_set)
{
    camera_type = type_set;
}

void set_wireless_type (camera_type_enum type_set)
{
    wireless_type = type_set;
}

