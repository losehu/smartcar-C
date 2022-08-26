/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_driver_flash
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/

#ifndef _zf_flash_h
#define _zf_flash_h

#include "ch32v30x_misc.h"
#include "ch32v30x_flash.h"

#define FLASH_BASE_ADDR             (0x08000000)                // FALSH�׵�ַ
#define FLASH_PAGE_SIZE             (0x00000400)                // 1K byte
#define FLASH_SECTION_SIZE          (FLASH_PAGE_SIZE*4)         // 4K byte
//һ��16��������һ������4K��
/*
2.5.2 Ƭ�ϴ洢�����Ծ�ģʽ
���� 32K �� 64K �ֽ� SRAM �������ڴ�����ݣ���������ݶ�ʧ��
���� 128K �� 256K �ֽڳ�������洢����Code FLASH���������û���Ӧ�ó���ͳ������ݴ洢�� ���� 256K FLASH+64K SRAM ��Ʒ֧��ѡ������Ϊ��192K FLASH+128K SRAM������224K FLASH+96K SRAM����
��256K FLASH+64K SRAM������288K FLASH+32K SRAM����������е�һ�֡�
���� 28K �ֽ�ϵͳ�洢����System FLASH��������ϵͳ��������洢�����ҹ̻��Ծټ��س��򣩡�
128 �ֽ����ڳ��������ִ洢��128 �ֽ������û�ѡ���ִ洢�� ������ʱ��ͨ���Ծ����ţ�BOOT0 �� BOOT1������ѡ�������Ծ�ģʽ�е�һ�֣�
l �ӳ�������洢���Ծ� l ��ϵͳ�洢���Ծ� l ���ڲ� SRAM �Ծ� �Ծټ��س�������ϵͳ�洢��������ͨ�� USART1 �� USB �ӿڶԳ�������洢�����������±�̡�

 */


//�ж��ٸ������أ����� CH32V307������
// ö�� Flash ����  ��ö�ٶ��岻�����û��޸�
typedef enum
{
    FLASH_SECTION_00,
    FLASH_SECTION_01,
    FLASH_SECTION_02,
    FLASH_SECTION_03,
    FLASH_SECTION_04,
    FLASH_SECTION_05,
    FLASH_SECTION_06,
    FLASH_SECTION_07,
    FLASH_SECTION_08,
    FLASH_SECTION_09,
    FLASH_SECTION_10,
    FLASH_SECTION_11,
    FLASH_SECTION_12,
    FLASH_SECTION_13,
    FLASH_SECTION_14,
    FLASH_SECTION_15,
}FLASH_SECTION_enum;

// ö�� Flash ҳ���� ��ö�ٶ��岻�����û��޸�
typedef enum
{
    FLASH_PAGE_0,
    FLASH_PAGE_1,
    FLASH_PAGE_2,
    FLASH_PAGE_3,
}FLASH_PAGE_enum;

uint8_t flash_check(FLASH_SECTION_enum sector_num, FLASH_PAGE_enum page_num);
uint8_t flash_erase_page(FLASH_SECTION_enum sector_num, FLASH_PAGE_enum page_num);
void flash_page_read (FLASH_SECTION_enum sector_num, FLASH_PAGE_enum page_num, uint32_t *buf, uint16_t len);
uint8_t flash_page_program(FLASH_SECTION_enum sector_num, FLASH_PAGE_enum page_num, const uint32_t *buf, uint16_t len);

#endif
