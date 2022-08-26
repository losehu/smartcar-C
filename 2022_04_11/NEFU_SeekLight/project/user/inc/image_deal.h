/**
 //@FileName	:image_deal.h
 //@CreatedDate	:2021��12��31��
 //@Author		:LiHao&WuSheng
 //@Description	:
 **/

#ifndef IMAGE_DEAL_H_
#define IMAGE_DEAL_H_
#include "stdbool.h"
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
 extern int16 seed_left_x,seed_left_y,seed_right_x,seed_right_y;
#define stack_size 400
extern   size_point stack_seed[stack_size];    //ջ
extern size_point stack_seed1[stack_size];//ջ
extern bool stop_flag;

extern int16 stack_top ;
extern int16 stack_top1 ;
extern int16 max_py,max_py1;
extern int16 min_py,min_py1;
extern int search_start;

bool pull_stack(int16 x, int16 y, uint8_t img_tmp[][COL]);    //��ջ
 size_point push_stack();    //��ջ
 bool pull_stack1(int16 x, int16 y, uint8_t p_Pixels[][COL]); //��ջ
 size_point push_stack1(); //��ջ
void search_line(void);
extern int left_right_cnt,left_left_cnt,right_right_cnt,right_left_cnt;
void solve_line();
void cal_middle_line(void);
extern const int16  search_start_line  ;
extern int16  start_line ;

extern int lose_error;
extern int16 left_lost_cnt , right_lost_cnt ;
extern int CenterCalMinRow ;
extern int CenterCalMaxRow ;
extern const int CenterCalMinRow_tmp ;
extern const int CenterCalMaxRow_tmp ;
extern int CenterCalMinRow_circle ;
extern int CenterCalMaxRow_circle ; //15
void search_line_fork(void);
bool judge_black(int y, int x) ;
void blur_points(int16 pts_in[][2], int num, float pts_out[][2], int kernel) ;
int clip(int x, int low, int up) ;
//��ͨ��
typedef struct  {
     uint8_t edge[(ROW + COL) * 2 - 4];
     int room;
     int last_room;
     int room_cnt;
     int edge_len;
int error_cnt,error_max;

}Connected_Component;
extern int search_start ;

extern Connected_Component connect;
bool add_edge(int num, uint8_t img_tmp[ROW][COL]);
void init_connect();
void edge_cnt(uint8_t img_tmp[ROW][COL]);
void room_cnt();
////�Ͻ�
typedef struct image {
    uint8_t *data;
    uint32_t width;
    uint32_t height;
    uint32_t step;
} image_t;
void findline_righthand_adaptive(image_t *img, int block_size, int clip_value, int x, int y, int pts[][2], int *num) ;
void findline_lefthand_adaptive(image_t *img, int block_size, int clip_value, int x, int y, int pts[][2], int *num) ;
    void RUBO_IMG_INIT() ;
   extern image_t deal_img;
extern    int ipts0[ROW][2], ipts0_num , ipts1[ROW][2], ipts1_num ;
extern bool img_flag;

#define AT AT_IMAGE
int  AT_IMAGE(image_t img,int x,int y);




#endif /* IMAGE_DEAL_H_ */
