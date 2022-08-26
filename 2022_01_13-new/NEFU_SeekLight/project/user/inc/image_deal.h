/**
 //@FileName	:image_deal.h
 //@CreatedDate	:2021��12��31��
 //@Author		:LiHao&WuSheng
 //@Description	:
 **/

#ifndef IMAGE_DEAL_H_
#define IMAGE_DEAL_H_

#include "zf_common_headfile.h"

//def
//ͨ��ImageBuffer���к��ͼ��������
#define         USED_ROW                120//80
#define         USED_COL                188//128
//͸�ӱ任��ͼ��������
#define         TRFED_ROW               100
#define         TRFED_COL               114
//����ͼ���ʹ��ROW��COL��Ϊ���������������޸�
#define         ROW                     100//USED_ROW|TRFED_ROW //��Զ����Ϊ0~ROW
#define         COL                     114//USED_COL|TRFED_COL

#define CamImage    mt9v03x_image_dvp
#define PER_IMG     SimBinImage//CamImage//��������͸�ӱ任��ͼ��
#define ImageUsed   *PerImg_ip//*PerImg_ip//SimBinImage//����ʹ�õ�ͼ��ImageUsedΪ����Ѳ�ߺ�ʶ���ͼ����cam.c��ʹ��

//param
//extern uint8_t CamImage[USED_ROW][USED_COL];//������ȡ�ĻҶ�ͼ����ImageBuffer()��ȡ���ߺ궨��
extern uint8_t SimBinImage[USED_ROW][USED_COL]; //��������͸�Ӷ�ֵ��ͼ����GetSimBinImage()��ȡ
extern uint8_t AdpBinImage[USED_ROW][USED_COL]; //����Ӧ��ֵ��ֵ��ͼ��
//extern uint8_t PerImage[TRFED_ROW][TRFED_COL];//Transformed_Image͸�ӱ任���ͼ��
extern int16_t ThresholdAdd; //�Դ�򷨵õ�����ֵ�����ֶ�����
extern uint8_t *PerImg_ip[TRFED_ROW][TRFED_COL]; //͸�ӱ任ӳ���ַ ָ������//ֱ��ʹ��*PerImg_ip����

//fun
void ImageBuffer(void);
void GetSimBinImage(void);
void GetAdpBinImage(short block0, short block1);
//void ShowPerImage(void);
#define ShowPerImage()  ips114_show_gray_image_vec(0,0,PerImg_ip,TRFED_COL,TRFED_ROW,TRFED_COL,TRFED_ROW,0)
void ImagePerspective_Init(void);
uint8_t OSTUThreshold(uint8_t img[USED_ROW][USED_COL]);

/********������***********/
typedef struct  {
    int16 x0;
    int16 y0;
}size_point;

extern   size_point stack_seed[500];    //ջ
extern int16 stack_top ;
extern int16 left_line[ROW];
extern int16 right_line[ROW];
void pull_stack(int16 x, int16 y, uint8_t img_tmp[][COL]);    //��ջ
 size_point push_stack();    //��ջ
void search_line(void);
void cal_middle_line(void);

#endif /* IMAGE_DEAL_H_ */
