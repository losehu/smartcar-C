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
#ifndef _zf_device_type_h_
#define _zf_device_type_h_

typedef enum
{
    NO_CAMERE = 0,                                                              // ������ͷ
    CAMERA_BIN,                                                                 // С���
    CAMERA_BIN_UART,                                                            // С��紮�ڰ汾
    CAMERA_GRAYSCALE,                                                           // �����
    CAMERA_COLOR,                                                               // ��ͫ
}camera_type_enum;

typedef enum
{
    NO_WIRELESS = 0,                                                            // ���豸
    WIRELESS_UART,                                                              // ���ߴ���
    BULETOOTH_CH9141,                                                           // ���� CH9141
}wireless_type_enum;

extern camera_type_enum camera_type;
extern wireless_type_enum wireless_type;

void set_camera_type (camera_type_enum type_set);
void set_wireless_type (camera_type_enum type_set);

#endif
