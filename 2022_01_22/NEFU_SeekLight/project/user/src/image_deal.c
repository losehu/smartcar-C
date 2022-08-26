/**
 //@FileName	:image_deal.c
 //@CreatedDate	:2021��12��31��
 //@Author		:LiHao&WuSheng
 //@Description	:ͼ��Ļ�����������|��ֵ��|��͸��
 **/

#include "image_deal.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
//int ipts0[POINTS_MAX_LEN][2];
//int ipts1[POINTS_MAX_LEN][2];
//int ipts0_num, ipts1_num;
//
//float thres = 20;
//float block_size = 7;
//float clip_value = 2;
//float begin_x = 5;
//float begin_y = 90;
//uint8_t CamImage[USED_ROW][USED_COL];//������ȡ�ĻҶ�ͼ����ImageBuffer()��ȡ
uint8_t SimBinImage[USED_ROW][USED_COL]; //��������͸�Ӷ�ֵ��ͼ����GetSimBinImage()��ȡ
uint8_t AdpBinImage[USED_ROW][USED_COL]; //����Ӧ��ֵ��ֵ��ͼ��
//uint8_t PerImage[TRFED_ROW][TRFED_COL];//Transformed_Binary_Image͸�ӱ任���ͼ��

int16_t ThresholdAdd = 0; //�Դ�򷨵õ�����ֵ�����ֶ�����
uint8_t *PerImg_ip[TRFED_ROW][TRFED_COL];

/**
 *@Name			:ImageBuffer
 *@Description 	:ImageBufferͼ�񻺳�&���е�CamImage��
 *@Param			:None
 *@Return		:None
 *@Sample		:ImageBuffer();
 **/
void ImageBuffer(void) {
    //memcpy(&CamImage, &mt9v03x_image_dvp, USED_ROW*USED_COL);
    uint8_t FirstRow = 0; //��ʼ�� ��0��ʼ
    uint8_t FirstCol = (MT9V03X_DVP_W - USED_COL) / 2; //��ʼ��
    if (FirstRow > MT9V03X_DVP_H - USED_ROW) {
        FirstRow = MT9V03X_DVP_H - USED_ROW;
    }
    for (uint8_t i = 0; i < USED_ROW; i++) {
        memcpy(&CamImage[i][0], &mt9v03x_image_dvp[FirstRow][FirstCol],
        USED_COL);
        FirstRow++;
    }
}
/**
 *@Name			:GetBinImage
 *@Description 	:GetBinImage
 *@Param			:img[ROW][COL]  ԭͼ��
 *@Return		:None
 *@Sample		:GetSimBinImage(CamImage[ROW][COL]);
 **/
void GetSimBinImage(void) {
    unsigned short i = 0, j = 0;

    uint8_t Threshold1 = ThresholdAdd + OSTUThreshold(CamImage);

    for (i = 0; i < USED_ROW; i++) {
        for (j = 0; j < USED_COL; j++) {
            if (CamImage[i][j] > Threshold1) //��ֵԽ����ʾ������Խ�࣬��ǳ��ͼ��Ҳ����ʾ����
                    {
                SimBinImage[i][j] = 255;
            } else {
                SimBinImage[i][j] = 0;
            }
        }
    }

}
/**
 *@Name			:GetPerImage
 *@Description 	:GetPerImage �õ���͸�Ӻ��ͼ��������ʾ
 *@Param			:None
 *@Return		:None
 *@Sample		:GetPerImage();
 **/
//void ShowPerImage(void)
//{
//    uint8_t PerImage[TRFED_ROW][TRFED_COL];
//    uint16_t i = 0;
//    uint16_t j = 0;
//    for (i = 0; i < TRFED_ROW; i++) {
//        for (j = 0; j < TRFED_COL; j++) {
//            PerImage[i][j] = *PerImg_ip[i][j];
//        }
//    }
//    ips114_displayimage032(PerImage[0],TRFED_COL,TRFED_ROW);
//    //ips114_show_gray_image_vec(0,0,PerImg_ip[0],TRFED_COL,TRFED_ROW,TRFED_COL,TRFED_ROW,0);
//}
/**
 *@Name			:ImagePerspectiveInit
 *@Description 	:ImagePerspectiveInit �õ�͸�ӱ任��ͼ��ĵ�ַӳ��;ֻ����һ��
 *@Param			:None
 *@Return		:None
 *@Sample		:ImagePerspectiveInit();
 **/
void ImagePerspective_Init(void) {
    for (int i = 0; i < ROW; i++) {

    }
    static uint8_t BlackColor = GrayPoint;
    double change_un_Mat[3][3] = {          //114w*100h
            { -0.3855060034305325, 0.5587822210003545, -56.36044041094684 }, {
                    1.194165587945464e-016, 0.08050404113450624,
                    -25.33253526654363 }, { 3.787330025255824e-018,
                    0.005731099702567735, -0.8034285395537146 }, };
//# 5.bmp 114 100 75 45 48 101 45 150 77 118//        double change_un_Mat[3][3] = {        //128w*80h
//                {0.2977941176470612, -0.3103991596638676, 20.982668067227},
//                {1.656469944593194e-16, 0.04254201680672303, 8.012079831932811},
//                {3.589441210453756e-18, -0.003151260504201701, 0.4065126050420192}
//                };

    for (int i = 0; i < TRFED_COL; i++) {
        for (int j = 0; j < TRFED_ROW; j++) {
            int local_x = (int) ((change_un_Mat[0][0] * i
                    + change_un_Mat[0][1] * j + change_un_Mat[0][2])
                    / (change_un_Mat[2][0] * i + change_un_Mat[2][1] * j
                            + change_un_Mat[2][2]));
            int local_y = (int) ((change_un_Mat[1][0] * i
                    + change_un_Mat[1][1] * j + change_un_Mat[1][2])
                    / (change_un_Mat[2][0] * i + change_un_Mat[2][1] * j
                            + change_un_Mat[2][2]));
            if (local_x
                    >= 0&& local_y >= 0 && local_y < USED_ROW && local_x < USED_COL)
                PerImg_ip[j][i] = &PER_IMG[local_y][local_x];
            else

            {

                PerImg_ip[j][i] = &BlackColor;          //&PER_IMG[0][0];
            }

        }
    }
}
/**
 *@Name			:GetAdpBinImage
 *@Description 	:Get_Adaptive_Binary_Image
 *@Param			:block0 �Ƚϸ�ֵ�����С����Ϊż����block1 ������ֵ�����С����Ϊż���������block0
 *@Return		:None
 *@Sample		:GetAdpBinImage(7);
 **/
void GetAdpBinImage(short block0, short block1) {
//    short block0 = 2;//������
//    short block1 = 4;//������
    uint8_t dx0 = 0;
    uint8_t dy0 = 0;
    uint8_t dx1 = 0;
    uint8_t dy1 = 0;
    uint8_t x = 0;
    uint8_t y = 0;
    uint16_t thres = 0;
    for (x = 0; x < USED_COL - (block1 - block0); x += block0) {
        for (y = 0; y < USED_ROW - (block1 - block0); y += block0) {
            thres = 0;
            for (dx1 = 0; dx1 < block1; dx1++) {          //����block1�����ֵ
                for (dy1 = 0; dy1 < block1; dy1++) {
                    thres += CamImage[y + dy1][x + dx1];
                }
            }
            thres = thres / (block1 * block1);
            for (dx0 = 0; dx0 < block0; dx0++) {          //��block0���ֵ��ֵ��
                for (dy0 = 0; dy0 < block0; dy0++) {
                    AdpBinImage[y + dy0 + (block1 - block0) / 2][x + dx0
                            + (block1 - block0) / 2] =
                    CamImage[y + dy0 + (block1 - block0) / 2][x + dx0
                            + (block1 - block0) / 2] > thres ? 255 : 0;
                }
            }
            if (x == 0) {          //����ڱ�
                for (uint8_t jx0 = 0; jx0 < (block1 - block0) / 2; jx0++) {
                    for (int ix0 = 0; ix0 < block0; ix0++) {
                        AdpBinImage[ix0][jx0] =
                        CamImage[ix0][jx0] > thres ? 255 : 0;
                    }
                }
            }
            if (x) {          //���Һڱ�

            }
            if (y == 0) {          //���Ϻڱ�
                for (uint8_t jy0 = 0; jy0 < block0; jy0++) {
                    for (int iy0 = 0; iy0 < (block1 - block0) / 2; iy0++) {
                        AdpBinImage[iy0][jy0] =
                        CamImage[iy0][jy0] > thres ? 255 : 0;
                    }
                }
            }
            if (y) {          //���ºڱ�

            }
        }
    }
}

/**
 *@Name			:OSTUThreshold
 *@Description 	:OSTUThreshold��򷨵õ���ֵ
 *@Param			:img[USED_ROW][USED_COL]  ԭͼ��
 *@Return		:�õ�����ֵ
 *@Sample		:��GetSimBinImage()����
 **/
uint8_t OSTUThreshold(uint8_t img[USED_ROW][USED_COL]) {
    signed short j, i;
    unsigned long Amount = 0;
    unsigned long PixelBack = 0;
    unsigned long PixelshortegralBack = 0;
    unsigned long Pixelshortegral = 0;
    signed long PixelshortegralFore = 0;
    signed long PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��;
    signed short MinValue, MaxValue;
    signed short Threshold0 = 0;
    unsigned char HistoGram[256];              //

    for (i = 0; i < 256; i++)
        HistoGram[i] = 0; //��ʼ���Ҷ�ֱ��ͼ

    for (i = 0; i < USED_ROW; i++) {
        for (j = 0; j < USED_COL; j++) {
            HistoGram[img[i][j]]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++)
        ;        //��ȡ��С�Ҷȵ�ֵ
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0;
            MaxValue--)
        ; //��ȡ���Ҷȵ�ֵ

    if (MaxValue == MinValue)
        return MaxValue;         // ͼ����ֻ��һ����ɫ
    if (MinValue + 1 == MaxValue)
        return MinValue;        // ͼ����ֻ�ж�����ɫ

    for (i = MinValue; i <= MaxValue; i++)
        Amount += HistoGram[i];        //  ��������

    Pixelshortegral = 0;
    for (i = MinValue; i <= MaxValue; i++) {
        Pixelshortegral += HistoGram[i] * i;        //�Ҷ�ֵ����
    }
    SigmaB = -1;
    for (i = MinValue; i < MaxValue; i++) {
        PixelBack = PixelBack + HistoGram[i];     //ǰ�����ص���
        PixelFore = Amount - PixelBack;           //�������ص���
        OmegaBack = (float) PixelBack / Amount;   //ǰ�����ذٷֱ�
        OmegaFore = (float) PixelFore / Amount;   //�������ذٷֱ�
        PixelshortegralBack += HistoGram[i] * i;  //ǰ���Ҷ�ֵ
        PixelshortegralFore = Pixelshortegral - PixelshortegralBack;  //�����Ҷ�ֵ
        MicroBack = (float) PixelshortegralBack / PixelBack;   //ǰ���ҶȰٷֱ�
        MicroFore = (float) PixelshortegralFore / PixelFore;   //�����ҶȰٷֱ�
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore)
                * (MicroBack - MicroFore);   //������䷽��
        if (Sigma > SigmaB)                    //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
                {
            SigmaB = Sigma;
            Threshold0 = i;
        }
    }
    return Threshold0;                        //���������ֵ;
}

///*******************�Ͻ�******************************/
//Camer camer;
//int camer_init(void)
//{
//    mt9v03x_init_dvp(); //��ʼ������ͷ ʹ��CSI�ӿ�
//    ImagePerspective_Init();
//  //  ImageBuffer();
//    camer.length = 0;
//    camer.camer_error = 0;
//    camer.camer_error_last = 0;
//    camer.image.data = mt9v03x_image_dvp[0];
//    camer.image.width = MT9V03X_W;
//    camer.image.height = MT9V03X_H;
//
//    return 0;
//}
//
//
//float nu=0,nu2=0;
//void camer_get(void)
//{
//    static uint16 Threshold;
//    static image_t imageRaw = DEF_IMAGE(NULL, TRFED_COL, TRFED_ROW);//80,128
//    static uint8_t img_thres_data[TRFED_ROW][TRFED_COL];
//    static image_t imageAdaptiveThreshold = DEF_IMAGE((uint8_t *) img_thres_data, TRFED_COL, TRFED_ROW);//80,128
//
//    static float standardMid = nWidth/2;    //��׼��������
//    static float camServoError; //���߼�Ȩƫ��
//    static float errorAverge;    //����ƫ��
//    static uint8 i;
//    static double sum,weight_sum;  //Ȩ�����ߺͣ�Ȩ�غ�
//    static float sum_ave,add_ave; //����Ȩ�����ߺͣ��ӵ����ߵ���
//    static float camServoErrorLast,camServoErrorAdd;
//
//
//
//
//    uint8 image[TRFED_ROW][TRFED_COL];
//
//    //ƫ��Ȩ��ֵ,�ײ�15����ȥ
//        static uint16 weight[nHeight]={  0,0,0,0,0,0,0,0,0,0, //40�е�49��
//                            0,0,0,0,0,1,1,1,1,1, //40�е�49��
//                            2,2,1,2,1,2,1,2,1,2,  //60�е�59��
//                            4,3,3,4,2,4,2,5,2,5,   //20�е�29��
//                            4,4,4,4,4,4,5,5,5,5,   //20�е�29��
//                            4,10,10,2,10,2,10,2,10,10,   //20�е�29��
//                            14,14,14,34,24,24,15,35,55,65,   //20�е�29��
//                            31,52,43,54,55,56,37,58,79,70,   //20�е�29��
//                            40,70,80,70,85,66,60,60,80,80,   //20�е�29��
//                            70,100,130,104,100,100,80,80,90,90,  //40�е�49��
//                            155,135,105,105,155,100,100,100,100,105,  //40�е�49��
//                            0,0,0,0,0,0,0,0,0,0 //70�е�79��
//                        };
//        static uint16 weight_circle[nHeight]={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//                            0, 0, 0, 0, 0,10,10,10,10,10,
//                        10,10,10,10,10,10,10,10,10,10,
//                        10,10,10,10,10,10,10,10,10,10,
//                        10,10,10,10,10,10,10,10,10,10,
//                        3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
//                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//                    };
//
//        if(mt9v03x_finish_flag)
//        {
//
//        //����͸�ӱ任
//       // GetSimBinImage();
//        for(int i = 0; i< TRFED_ROW; i++)
//        {
//            for(int j =0; j< TRFED_COL; j++)
//            {
//                image[i][j] = *PerImg_ip[i][j];
//            }
//        }
//        imageRaw.data = image[0];
//        imageRaw.height = 100;
//        imageRaw.width = 114;
//        int x1 = imageRaw.width / 2 - begin_x, y1 = begin_y;
//
//        ipts0_num = sizeof(ipts0) / sizeof(ipts0[0]);
//        for (; x1 > 0; x1--) {nu=AT_IMAGE(&imageRaw, x1 - 1, y1) ;if (AT_IMAGE(&imageRaw, x1 - 1, y1) < thres) break;}
//        nu2=AT_IMAGE(&imageRaw, x1, y1);
//        if (AT_IMAGE(&imageRaw, x1, y1) >= thres)
//            findline_lefthand_adaptive(&imageRaw, block_size, clip_value, x1, y1, ipts0, &ipts0_num);
//        else ipts0_num = 0;
//        int x2 = imageRaw.width / 2 + begin_x, y2 = begin_y;
//        ipts1_num = sizeof(ipts1) / sizeof(ipts1[0]);
//        for (; x2 < imageRaw.width - 1; x2++) if (AT_IMAGE(&imageRaw, x2 + 1, y2) < thres) break;
//        if (AT_IMAGE(&imageRaw, x2, y2) >= thres)
//            findline_righthand_adaptive(&imageRaw, block_size, clip_value, x2, y2, ipts1, &ipts1_num);
//        else ipts1_num = 0;
//
//        //������ʾʹ��
//       adaptive_threshold(&imageRaw, &imageAdaptiveThreshold, block_size, clip_value, 0, 255);
//
//   //     ips114_displayimage032(  image[0],114,100);
//
//       // image
//        for(i=12;i<22;i++)
//        {
//
//        //           else if((forkroad_flag_right ==1
//        //                   ||forkroad_flag_left ==1 ||forkroad_flag_right_out==1
//        //                   ||forkroad_flag_left_out==1) )
//        //           {
//        //               sum+= mid_point[i].x*weight[i] ;
//        //               weight_sum+=weight[i];
//        //           }
//        //           else
//
//                sum+= mid_point[i].x;
//
//
//
//        }
//        point_center=sum/10;
//            camServoErrorLast = camServoError;
//            camServoError=standardMid-sum/weight_sum;
//            camServoErrorAdd = camServoError - camServoErrorLast;
//            errorAverge   =standardMid-sum_ave/add_ave;
//            sum = 0;
//            weight_sum = 0;
//            sum_ave = 0;
//            add_ave = 0;
//
//
//            camer.length           = length;
//            camer.camer_error      = camServoError;
//            camer.camer_error_last = camServoErrorLast;
//            camer.image            = imageAdaptiveThreshold;
//            camer.x1                = x1;
//            camer.y1                = y1;
//            camer.x2                = x2;
//            camer.y2                = y2;
//            mt9v03x_finish_flag = 0;//��ͼ��ʹ����Ϻ�  ��������־λ�����򲻻Ὺʼ�ɼ���һ��ͼ��
//            ips114_displayimage032(  imageAdaptiveThreshold.data,114,100);
//
//        }
//}
//
//
//const int dir_front[4][2] = {{0,  -1},
//                             {1,  0},
//                             {0,  1},
//                             {-1, 0}};
//const int dir_frontleft[4][2] = {{-1, -1},
//                                 {1,  -1},
//                                 {1,  1},
//                                 {-1, 1}};
//const int dir_frontright[4][2] = {{1,  -1},
//                                  {1,  1},
//                                  {-1, 1},
//                                  {-1, -1}};
//
//void findline_lefthand_adaptive(image_t *img, int block_size, int clip_value, int x, int y, int pts[][2], int *num)
//{
//    int half = block_size / 2;
//    int step = 0, dir = 0, turn = 0;
//    while (step < *num && half < x && x < img->width - half - 1 && half < y && y < img->height - half - 1 && turn < 4)
//    {
//        int local_thres = 0;
//        for (int dy = -half; dy <= half; dy++)
//        {
//            for (int dx = -half; dx <= half; dx++)
//            {
//                local_thres += AT(img, x + dx, y + dy);
//            }
//        }
//        local_thres /= block_size * block_size;
//        local_thres -= clip_value;
//
//        int current_value = AT(img, x, y);
//        int front_value = AT(img, x + dir_front[dir][0], y + dir_front[dir][1]);
//        int frontleft_value = AT(img, x + dir_frontleft[dir][0], y + dir_frontleft[dir][1]);
//        if (front_value < local_thres)
//        {
//            dir = (dir + 1) % 4;
//            turn++;
//        }
//        else if (frontleft_value < local_thres)
//        {
//            x += dir_front[dir][0];
//            y += dir_front[dir][1];
//            pts[step][0] = x;
//            pts[step][1] = y;
//            step++;
//            turn = 0;
//        }
//            else
//        {
//            x += dir_frontleft[dir][0];
//            y += dir_frontleft[dir][1];
//            dir = (dir + 3) % 4;
//            pts[step][0] = x;
//            pts[step][1] = y;
//            step++;
//            turn = 0;
//        }
//    }
//    *num = step;
//}
//
//void findline_righthand_adaptive(image_t *img, int block_size, int clip_value, int x, int y, int pts[][2], int *num)
//{
//    int half = block_size / 2;
//    int step = 0, dir = 0, turn = 0;
//    while (step < *num && 0 < x && x < img->width - 1 && 0 < y && y < img->height - 1 && turn < 4) {
//        int local_thres = 0;
//        for (int dy = -half; dy <= half; dy++) {
//            for (int dx = -half; dx <= half; dx++) {
//                local_thres += AT(img, x + dx, y + dy);
//            }
//        }
//        local_thres /= block_size * block_size;
//        local_thres -= clip_value;
//
//        int current_value = AT(img, x, y);
//        int front_value = AT(img, x + dir_front[dir][0], y + dir_front[dir][1]);
//        int frontright_value = AT(img, x + dir_frontright[dir][0], y + dir_frontright[dir][1]);
//        if (front_value < local_thres) {
//            dir = (dir + 3) % 4;
//            turn++;
//        } else if (frontright_value < local_thres) {
//            x += dir_front[dir][0];
//            y += dir_front[dir][1];
//            pts[step][0] = x;
//            pts[step][1] = y;
//            step++;
//            turn = 0;
//        } else {
//            x += dir_frontright[dir][0];
//            y += dir_frontright[dir][1];
//            dir = (dir + 1) % 4;
//            pts[step][0] = x;
//            pts[step][1] = y;
//            step++;
//            turn = 0;
//        }
//    }
//    *num = step;
//}
//
////һ�κ�������
//void lian_xian_zhi(int column1,int row1,int column2,int row2,Point_info_t* point)
////a,bһ�ӣ�deһ��    a>b   ����֮������    ���κ���
//{
//
//    int i=0,ab=0,cd=0;
//    for(i=row1; i>=row2; i--)
//    {
//        cd=(int)(  column2-column1)*(row1-i)/(row1-row2); //ֱ��
//        //cd=(column2-column1)*(row1-i)*(row1-i)/(row1-row2)/(row1-row2); //���κ���
//        ab=(int)cd+column1;
//        ab=range_protect(ab,159,0);
//        point[i].x = ab;
//
//    }
//}
// /********�������(1.3mǰհ)**********/
// uint8 distance3[nHeight]=
// {
//   180,180,180,180,180,159,159,158,156,156,
//   155,151,150,148,150,  159,157,156,155,154,
//   152,151,150,148,147,146,144,144,142,141,
//   140,138,138,136,135,134,132,132,130,128,
//   128,126,125,124,123,121,120,119,117,116,
//   114,114,112,111,110,109,108,107,106,104,
//   103,101,100,98, 98, 96, 96, 93, 92, 90,
//   88, 88, 86, 85, 83, 82, 81, 79, 77, 77,
//   75, 74, 73, 72, 70, 70, 68, 67, 65, 65,
//   63, 63, 61, 60, 59, 58, 56, 56, 54, 53,
//   51, 51, 49, 48, 47, 45, 43, 43, 42, 41,
//   39, 38, 37, 35, 34, 33, 33, 32, 32, 31
// };
//
// /********���߲���б��**********/
//uint8 k_tiaojie=7;//(��������)
//uint8 k_tiaojie_huandao=6;//(Ԥ��������)
//uint8 k_tiaojie_huandao1=6;//(��������)
//uint8 k_tiaojie_huandao2=7;//(��������)
//uint8 k_forkroad = 7;
// /********ɨ�߱���(�ṹ��)**********/
//Point_info_t left_point[nHeight];           //���ߵ����
//Point_info_t right_point[nHeight];          //���ߵ����
//Point_info_t mid_point[nHeight];            //���ߵ����
//static int16 last_mid_point;
//
///********��Ч��**********/
//int8  length=nHeight;
//uint8 length_old_end;
//int   length_old;
//uint8 length_flag;
//
//
///********��������**********/
//float k_right[nHeight]={0};  //  ���ֶ�ֵ80��Ŀǰ��������Ϊ����nHeight����80�����˸ĳɺ궨����
//float k_left[nHeight]={0};   //  ���ֶ�ֵ80��Ŀǰ��������Ϊ����nHeight����80�����˸ĳɺ궨����
//int first_lose_flag = 0;
//int first_lose = 0;
//int16 all_lose_count;
//int16 left_lose_count;
//int16 right_lose_count;
//int16 all_lose_count_old;
//int16 all_normal_count;
//int16 right_normal_count;
//int16 left_normal_count;
//int16 left_normal_right_lose_count;
//int16 left_lose_right_nomal_count;
//int first_left_point=nHeight;//����ҵ��ĵ�һ����        //nHeight80
//int first_right_point=nHeight;//�ұ��ҵ��ĵ�һ����
//int left_lose_15;
//int right_lose_15;
//int left_nomal_15;
//int right_nomal_15;
//int all_lose_15;
//int all_nomal_15;
//int left_lose_35up;
//int right_lose_35up;
//int all_lose_35up;
//int all_normal_35up;
//int left_normal_35up;
//int right_normal_35up;
//int left_lose_45up;
//int right_lose_45up;
//int all_lose_45up;
//int all_normal_45up;
//int left_normal_45up;
//int right_normal_45up;
//uint8 all_lose_15_30;
//uint8 right_lose_15_30;
//uint8 left_lose_15_30;
//uint8 all_normal_15_30;
//uint8 right_normal_15_30;
//uint8 left_normal_15_30;
//uint8 leftline;
//uint8 rightline;
//int8  rightcircle;
//int8  leftcircle;
//uint8 leftline_45up;
//uint8 rightline_45up;
//int16 lose_count;
//
///********����**********/
//uint8  circle_point;
//uint8  circle_flag = 0;
//uint16 circle_crossroad = 0;
//uint8 circle_ready=0;
//uint8 circle_out_flag=0;
//uint8 circle_ready_l=0;
//uint8 circle_ready_r=0;
//int left_circlezero;
//int right_circlezero;
//int angel_count = 0;
//int huandao_angel = 0;
//uint8 hd_zuo=0;
//uint8 hd_you=0;
//uint8 circle_out_flag2=0;
//uint8 circle_chack=0;
//uint16 circle_out_count=0;
//uint16 circle_count = 0;
//
///********ʮ��**********/
//int HOW;
//uint8 crossroad=0;
//uint8 crossroad_flag = 0 ;
//uint8 crossroad_ready=0;
//int16 crossroad_count  =0;
//int buzzer_flag =0 ;
//
///********����·��(����)**********/
//int white_first;
//int white_num_you;
//int white_num_zuo;
//int white_num_130;
//int white_num_135;
//int white_num_125;
//int white_num_25;
//int white_num_30;
//int white_num_35;
//
//int white_xiezuo;
//int white_xieyou;
//
//
//static int16 last_mid_first;
//static int16 first_line_num,first_line_sum;
//
///**�߽���Ϣ��ȡ----ͼ��������**/
//void edge_line_extract(uint8 *src)
//{
//
//    int16 i,j;
//    uint8 length_flag=0;
//    int first_lose_flag = 0;
//
//
//    /**ͼ�������Ϣ**/
//    all_lose_count=0;                   //�����������
//    left_lose_count=0;
//    right_lose_count=0;
//    all_normal_count=0;
//    right_normal_count=0;
//    left_normal_count=0;
//    left_normal_right_lose_count=0;
//    left_lose_right_nomal_count=0;
//    right_lose_15=0;
//    left_nomal_15=0;
//    right_nomal_15=0;
//    all_lose_15=0;
//    all_normal_45up=0;
//    left_normal_45up=0;
//    right_normal_45up=0;
//    all_lose_45up=0;
//    left_lose_45up=0;
//    right_lose_45up=0;
//    all_lose_15_30=0;
//    right_lose_15_30=0;
//    left_lose_15_30=0;
//    all_normal_15_30=0;
//    right_normal_15_30=0;
//    left_normal_15_30=0;
//    left_lose_35up=0;
//    right_lose_35up=0;
//    all_lose_35up=0;
//    all_normal_35up=0;
//    left_normal_35up=0;
//    right_normal_35up=0;
//    length_old_end=0;
//    lose_count = 0;
//
//        //����·��ʹ�ò���
//    white_num_you= 0;
//    white_num_zuo= 0;
//    white_first   = 0;
//    white_num_130 = 0;
//    white_num_135 = 0;
//    white_num_125 = 0;
//    white_num_25  = 0;
//    white_num_30  = 0;
//    white_num_35  = 0;
//
//    white_xiezuo = 0;
//    white_xieyou = 0;
//
//     /*****����*****/
//   uint8  *pimg;
//   uint8  *pimg1;
//   uint8   tmpsrc;
//
//   uint8 left_case,right_case;
//
//   leftline=0;
//   rightline=0;
//   rightcircle=0;
//   leftcircle=0;
//   leftline_45up=0;
//   rightline_45up=0;
////   left_circlezero = 0;
////   right_circlezero = 0;
//
////   white_num_80 = 0 ;
////   white_first = 0;
//
//
//
//     /**ͼ�����ʮ������ȥ��Ѱ�ҵ�ʮ�����е�**/
//   pimg1=src+((nHeight-15)*nWidth);
//   for(j=0;j<nWidth;j++)
//   {
//       tmpsrc=*(pimg1+j);
//       if(tmpsrc==WHITEFORCAM)
//       {
//           first_line_num++;
//           first_line_sum+=j;
//       }
//   }
//
//    last_mid_first=first_line_sum/first_line_num;  //������������ֵ �ۼư׵����꼰����
//    first_line_num=0;
//    first_line_sum=0;
//    last_mid_point=last_mid_first;
//
//    if(last_mid_point==BLACKFORCAM)               //�����һ��ȫΪ�ھͽ���ֵ��Ϊ80
//    {
//        last_mid_point=nWidth/2;//���Դ���������ͼ��ʧ
//    }
//
//
//    //����·��ʹ�ò���
//    for(int i = nHeight-10;i>0 ; i-- )
//    {
//        if(src+i*nWidth+nWidth/2 == 0)
//        {
//            white_first = i;//�м��һ���ڵ����ڵ�����
//            break;
//        }
//     }
//
//    for(int i = nHeight;i<nWidth;i++)
//    {
//        if(src+white_first*nWidth+i==BLACKFORCAM)
//        white_num_you++;//ɨ�߽���
//    }
//
//    for(int i = nHeight;i>0;i--)
//    {
//        if( *(src+white_first*nWidth+i) ==BLACKFORCAM)
//            white_num_zuo++;
//        if( *(src+i*nWidth+124)         ==WHITEFORCAM)
//            white_num_130++;
//        if( *(src+i*nWidth+64)          ==WHITEFORCAM)
//            white_num_30++;
//    }//ɨ�߽���
//
//
//    mid_point[14].x = last_mid_point;
//
//    int K ;
//    /**�߽���������**/  //nHeight = 120  Data:2021.8.30
//    int limit[nHeight] = {0,0,0,0,0,0,0,0,0,0,
//                     0,0,0,0,0,0,0,0,0,0,
//                     0,0,0,0,0,0,0,0,0,0,
//                     0,0,0,0,0,20,20,20,20,20,
//                     20,20,20,20,20,25,25,25,25,25,
//                     25,25,25,25,25,30,30,30,30,30,
//                     30,30,30,30,30,30,30,30,30,30,
//                     30,30,30,30,30,30,30,30,30,30,
//                    };
//
//    /*****����ɨ��forѭ����ʼ*****/
//    for(K=(nHeight-15);K>0;K--)                          //nHeight=80�߶�
//    {
//
//       pimg=src+(K)*nWidth;                           //nWidth=160���
//       i=nHeight-K;
//
//       /**ɨ����**/
//       for(j= last_mid_point;j>=0;j--)
//       {
//           if(BLACKFORCAM==*(pimg+j) && j!=limit[i])                  //pimg+j������Ǻ�ɫ�ͼ�¼����     //��һ���������!!!!!������������
//           {                                             //��¼���꣬x��ʾ�߶�
//               left_point[i].x=j+1;
//               left_point[i].chuli=j+1;
//               left_point[i].state=NORMAL;
//               left_case=1;
//
//               //���Լ���б��
//               if(i>10&&left_point[i-1].state==NORMAL&&left_point[i-2].state==NORMAL)
//               {k_left[i]=((float)(left_point[i].x-left_point[i-1].x)+(float)(left_point[i-1].x-left_point[i-2].x)+(float)(left_point[i-2].x-left_point[i-3].x))/3;}
//               break;
//
//           }
//           else if(j==limit[i])                       //����
//           {
//               left_point[i].x=0;
//               left_point[i].chuli=0;
//               left_point[i].state=LOSE;
//               left_case=3;
//
//           }
//       }
//       /**ɨ����**/
//       for(j=last_mid_point;j<nWidth;j++)         //��¼right_point    first_right_point   first_right_lose
//       {
//           if(BLACKFORCAM==*(pimg+j) && j!=(nWidth-limit[i]))                                     //&&*(pimg+j-1)+*(pimg+j-2)+*(pimg+j-3)==0)
//           {
//               right_point[i].x=j-1;
//               right_point[i].chuli=j-1;
//               right_case=1;
//               right_point[i].state=NORMAL;
//
//               //���Լ���б��
//               if(i>10&&right_point[i-1].state==NORMAL&&right_point[i-2].state==NORMAL)
//               {k_right[i]=((float)(right_point[i-1].x-right_point[i].x)+(float)(right_point[i-2].x-right_point[i-1].x)+(float)(right_point[i-3].x-right_point[i-2].x))/3;}
//
//               break;
//           }
//
//           else if(j==nWidth-1-limit[i])                  //��ʾfirst_right_lose��û�иı��ǵ�һ�ζ�ʧ
//           {
//               right_point[i].x=nWidth-1;
//               right_point[i].chuli=nWidth-1;
//               right_case=0;
//               right_point[i].state=LOSE;
//           }
//         }
//
//
//       //���������ж��Ƿ�Ϊ����ֱ��
//       if(i<length&&k_left[i]>0.3&&k_left[i]<1.4)
//       {
//         leftline++;
//         if(i>45)  {leftline_45up++;}
//       }
//
//       if(i<length&&k_right[i]>0.3&&k_right[i]<1.4)
//       {
//         rightline++;
//         if(i>45)  {rightline_45up++;}
//
//       }
//
//        //3.ȷ���������           ��¼������Ϣ
//             if(length_flag==0)
//             {
//                switch(left_case-right_case)      //�����Ƕ�ÿһ�ж�����һ��
//                 {
//                 case 0:             //���ұ��߶�ɨ��
//                     all_normal_count++;
//                     right_normal_count++;
//                     left_normal_count++;
//
//                     if(i>35)
//                     {
//                         all_normal_35up++;  //��ʹ��
//                         right_normal_35up++;
//                         left_normal_35up++;
//                     }
//                     if(i>45)
//                     {
//                         all_normal_45up++;  //��ʹ��
//                         right_normal_45up++;
//                         left_normal_45up++;
//                     }
//                     break;
//                 case 1:            //ɨ������ߣ���ʧ�ұ���
//                     right_lose_count++;
//                     left_normal_count++;
//                     left_normal_right_lose_count++;
//                     if(i>35)
//                     {
//                         right_lose_35up++;
//                         left_normal_35up++;
//                     }
//                     if(i>45)
//                     {
//                         right_lose_45up++;
//                         left_normal_45up++;
//                     }
//                     break;
//                 case 2:           //��ʧ����ߣ�ɨ���ұ���
//                     left_lose_count++;
//                     right_normal_count++;
//                     left_lose_right_nomal_count++;
//                     if(i>35)
//                     {
//                         left_lose_35up++;
//                         right_normal_35up++;
//                     }
//                     if(i>45)
//                     {
//                         left_lose_45up++;
//                         right_normal_45up++;
//                     }
//                     break;
//                 case 3:          //���ұ��߾���ʧ
//                     left_lose_count++;
//                     right_lose_count++;
//                     all_lose_count++;
//
//                     if(i>35)
//                     {
//                         left_lose_35up++;
//                         right_lose_35up++;
//                         all_lose_35up++;
//                     }
//                     if(i>45)
//                     {
//                         left_lose_45up++;
//                         right_lose_45up++;
//                         all_lose_45up++;
//                     }
//                     break;
//                 }
//
//                 if(i<55&&i>5)
//                 {
//                     switch(left_case-right_case)      //�����Ƕ�ÿһ�ж�����һ��
//                     {
//                     case 0:             //���ұ��߶�ɨ��
//                         if(i<=45&&i>15)
//                         {
//                             all_normal_15_30++;
//                             left_normal_15_30++;
//                             right_normal_15_30++;
//                         }
//                         break;
//                     case 1:            //ɨ������ߣ���ʧ�ұ���
//                         if(i<=45&&i>15)
//                         {
//                             left_normal_15_30++;
//                             right_lose_15_30++;
//                         }
//                         break;
//                     case 2:           //��ʧ����ߣ�ɨ���ұ���
//                         if(i<=45&&i>15)
//                         {
//                             left_lose_15_30++;
//                             right_normal_15_30++;
//                         }
//                         break;
//                     case 3:          //���ұ��߾���ʧ
//                         if(i<=45&&i>15)
//                         {
//                             all_lose_15_30++;
//                             left_lose_15_30++;
//                             right_lose_15_30++;
//                         }
//                         break;
//                     }
//                 }
//
//            }
//
//     //����������ж϶�����
//     if((right_point[i].x-left_point[i].x)>(distance3[i]+30))
//     {
//         lose_count++;
//         if(first_lose_flag == 0)
//         {
//             first_lose = i;
//             first_lose_flag = 1;
//         }
//     }
//
//
//    //����ɨ�߽���
//
//
//
//
//
//    /********��������********/
//
//          if(i<17)
//          {
//            mid_point[i].x=(left_point[i].x+right_point[i].x)/2;
//          }//�ײ�����ֵ��֤ƽ��
//          else if(left_point[i].state==LOSE && right_point[i].state==LOSE)
//          {
//            mid_point[i].x=(left_point[i].x+right_point[i].x)/2;
//          }
//          else if(left_point[i].state==LOSE)//��ඪʧ
//          {
//              if(circle_ready_l==1/*||forkroad_flag_right_in==1*/)
//              {mid_point[i].x=right_point[i].x-distance3[i]/2;}
//              else if(right_point[i-1].x>=right_point[i].x)
//
//              {
//                mid_point[i].x=mid_point[i-1].x+right_point[i].x-right_point[i-1].x;
//              }
//              else
//              {
//                mid_point[i].x=mid_point[i-1].x;
//              }
//          }
//          else if(right_point[i].state==LOSE)//�Ҳඪʧ
//          {
//              if(circle_ready_r==1/*||forkroad_flag_left_in==1*/)
//              {mid_point[i].x=left_point[i].x+distance3[i]/2;}
//              else if(left_point[i].x>=left_point[i-1].x)
//
//              {
//                mid_point[i].x=mid_point[i-1].x+left_point[i].x-left_point[i-1].x;
//              }
//              else
//              {
//                mid_point[i].x=mid_point[i-1].x;
//              }
//          }
//          else
//          {
//////              if(forkroad_flag_left_in==1)
//////              {mid_point[i].x=(left_point[i].x+right_point[i].x)/2;}
//////              else if(forkroad_flag_right_in==1)
//////              {mid_point[i].x=(left_point[i].x+right_point[i].x)/2;}
//              if(circle_ready_r==1)      // ȥ����else
//              {mid_point[i].x=left_point[i].x+distance3[i]/2;}
//              else if(circle_ready_l==1)
//              {mid_point[i].x=right_point[i].x-distance3[i]/2;}
//              else
//              {
//                mid_point[i].x=(left_point[i].x+right_point[i].x)/2;
//              }
//          }
//
//       //�����޷�
//     if(mid_point[i].x<0)     {mid_point[i].x=0;}
//     if(mid_point[i].x>=nWidth)   {mid_point[i].x=nWidth;}
//     if(right_point[i].x>nWidth) {right_point[i].x=nWidth;}
//     if(left_point[i].x<0)    {left_point[i].x=0;}
//
//
//
//////////////******�����Ч��******///////////////////////
//
//
//    if(right_point[i].x<(15+left_point[i].x)&&length_flag==0)
//        if(right_point[i].x<(15+left_point[i].x)&&length_flag==0)  //ȥ����������һ�е�else
//    {
//        length=i;
//        length_flag=1;
//    }
//    if(i==nHeight-1&&length_flag==0)
//    {
//        length=nHeight;
//    }
//
//
//}
//
//
//}
//
//
//
////����·�ڴ���
//void forkroad_deal(void)
//{
//
//    //��������·�ڼ��
//    //Ѱ������㲹�߷�
//
//    if(crossroad_ready == 1&&right_lose_15_30>=20&&left_lose_15_30>=20)
//    {
////        BeepDiDi(3,300);
//        if(right_point[75].x-left_point[75].x>=10)
//        {
//        lian_xian_zhi(right_point[75].x,75,160,0,right_point);
//        lian_xian_zhi(left_point[75].x,75,0,0,left_point);
//        HOW = 1;
//        }
//        else if(right_point[70].x-left_point[70].x>=10)
//        {
//        lian_xian_zhi(right_point[70].x,70,160,0,right_point);
//        lian_xian_zhi(left_point[70].x,70,0,0,left_point);
//        HOW = 1;
//        }
//        else if(right_point[65].x-left_point[65].x>=10)
//        {
//            lian_xian_zhi(right_point[65].x,65,160,0,right_point);
//            lian_xian_zhi(left_point[65].x,65,0,0,left_point);
//            HOW = 1;
//        }
//
//        if(HOW==1)
//        {
//        for(int i = length;i>=15;i--)
//        {
//            mid_point[i].x = (right_point[i].x+left_point[i].x)/2 ;
//        }
//        }
//
//    }
//
//    if(crossroad_ready==0)
//    {
//        HOW = 0;
//    }
//
//    else if(crossroad_ready==1&&right_lose_15_30>=24&&(left_lose_45up>=10)&&left_lose_15_30<=25
//        &&crossroad==0)
//    {
////        BeepDiDi(3,300);
//        HOW = 4;//б��ʮ�ֿ�ʼ���߱�־��������һ������ ��ֻ������һ��
//    }
//
//
//   if(HOW==3&&white_first>=8)
//    {
//
//        lian_xian_zhi(0,80,160,0,right_point);
//      for(int i = length;i>=15;i--)
//      {
//      mid_point[i].x = (right_point[i].x+left_point[i].x)/2 ;
//      }
//    }
//
//   else  if(HOW==4&&white_first>=6)
//   {
//
//       lian_xian_zhi(160,80,0,0,left_point);
//       for(int i = length;i>=15;i--)
//       {
//       mid_point[i].x = (right_point[i].x+left_point[i].x)/2 ;
//       }
//   }
//
//
//}
//// ����Ӧ��ֵ��ֵ��
//void adaptive_threshold(image_t *img0, image_t *img1, int block_size, int down_value, uint8_t low_value, uint8_t high_value)
//{
//    int half = block_size / 2;
//    // �ȱ���y�����x�Ƚ�cache-friendly
//    for (int y = 0; y < img0->height; y++) {
//        for (int x = 0; x < img0->width; x++) {
//            int thres_value = 0;
//            for (int dy = -half; dy <= half; dy++) {
//                for (int dx = -half; dx <= half; dx++) {
//                    thres_value += AT_CLIP(img0, x + dx, y + dy);
//                }
//            }
//            thres_value /= block_size * block_size;
//            thres_value -= down_value;
//                    AT(img1, x, y) = AT(img0, x, y) < thres_value ? low_value : high_value;
//        }
//    }
//}
//
//int clip(int x, int low, int up) {
//    return x > up ? up : x < low ? low : x;
//}
//
//float fclip(float x, float low, float up) {
//    return x > up ? up : x < low ? low : x;
//}
//
///************************������*******************************/
size_point stack_seed[stack_size];    //ջ
size_point stack_seed1[stack_size];    //ջ
int16 start_line = 10;
int16 left_lost_cnt = 0, right_lost_cnt = 0;
int16 stack_top = 0;
bool stop_flag = 0;
int16 stack_top1 = 0;
int16 max_py = 0, max_py1 = 0;
int16 min_py = 0, min_py1 = 0;
circle circle_flag;    //ջ
int CenterCalMinRow = 5;
int CenterCalMaxRow = 30;
int CenterCalMinRow_tmp = 0;
int CenterCalMaxRow_tmp = 0;
int lose_error = 5;
#define line_debug 0
size_point connects[16] = {    //������ɨ��
        { 0, -1 },    //��
                { -1, -1 },    //����
                { -1, 0 },    //��
                { -1, 1 },    //����
                { 0, 1 },    //��
                { 1, 1 },    //����
                { 1, 0 },    //��
                { 1, -1 },    //����

        };

int16 stack_max = 0;
bool pull_stack(int16 x, int16 y, uint8_t img_tmp[][COL]) //��ջ
{
    if (stack_top == stack_size)
        return 1;

    img_tmp[y][x] = 1;
    stack_seed[stack_top].x0 = x;
    stack_seed[stack_top].y0 = y;
    stack_top++;
    return 0;

}
size_point push_stack() //��ջ
{
    stack_seed[stack_top].y0 = 0;
    stack_seed[stack_top].x0 = 0;
    return stack_seed[--stack_top];
}
bool pull_stack1(int16 x, int16 y, uint8_t p_Pixels[][COL]) //��ջ
{
    if (stack_top1 == stack_size)
        return 1;

    p_Pixels[y][x] = 1;
    stack_seed1[stack_top1].x0 = x;
    stack_seed1[stack_top1].y0 = y;
    stack_top1++;
    return 0;

    //   if (stack_top > max_top)max_top = stack_top, cout << max_top << endl;
}
size_point push_stack1() //��ջ
{

    stack_seed1[stack_top1].y0 = 0;
    stack_seed1[stack_top1].x0 = 0;
    return stack_seed1[--stack_top1];
}
void search_line(void) {
    for (int i = 0; i < ROW; i++)
        Img.LeftBorder[i] = 0, Img.RightBorder[i] = COL - 1;
//    int error_right_tmp = 0, error_left_tmp = 0;
//    bool error_right_flag = 0, error_left_flag = 0;
    int16 px, py;
    uint8_t img_tmp[ROW][COL];
    size_point center_seed;
    stack_top = 0;
    stack_top1 = 0;
    start_line = 1;
    memset(img_tmp, 0, sizeof(img_tmp));
    while (start_line >= 0 && (stack_top == 0 || stack_top1 == 0)) {
        for (int i = COL / 2 + 10; i >= 0; i--) {
            if ((ImageUsed[ROW - 1 - start_line][i]
                    != ImageUsed[ROW - 1 - start_line][i + 1]
                    && ImageUsed[ROW - 1 - start_line][i] == 0)
                    || (i == 0 && ImageUsed[ROW - 1 - start_line][i] != 0
                            && ImageUsed[ROW - 1 - start_line][i + RoadWide0 / 2]
                                    != 0)) {
                if (ImageUsed[ROW - 1 - start_line][i + 1]
                        == ImageUsed[ROW - 1 - start_line][i + 2]) {
                    if (pull_stack(i + 1, start_line, img_tmp))
                        return;
                    break;
                }
            }
        }
        for (int i = COL / 2 - 10; i <= COL - 1; i++) {
            if ((ImageUsed[ROW - 1 - start_line][i]
                    != ImageUsed[ROW - 1 - start_line][i - 1]
                    && ImageUsed[ROW - 1 - start_line][i] == 0)
                    || (i == COL - 1 && ImageUsed[ROW - 1 - start_line][i] != 0
                            && ImageUsed[ROW - 1 - start_line][i - RoadWide0 / 2]
                                    != 0)) {
                if (ImageUsed[ROW - 1 - start_line][i - 1]
                        == ImageUsed[ROW - 1 - start_line][i - 2]) {
                    if (pull_stack1(i - 1, start_line, img_tmp))
                        return;
                    break;
                }
            }
        }
        start_line--;
    }

    if (stack_top == 0 || stack_top1 == 0 || start_line < 0) {
        ExSpeed = 0;
        while (1) {
            ips114_clear(RED);
            ips114_show_string(1, 1, "speed die!");
        }
        return;
    }
    max_py = 0, max_py1 = 0;
    min_py = 99, min_py1 = 99;
    if (line_debug) {
        ips114_clear(YELLOW);
        ShowPerImage();
    }
    while (stack_top + stack_top1 != 0 && !(max_py == max_py1 && max_py == 99)) {
        while (stack_top != 0 && (max_py <= max_py1 || stack_top1 == 0)) {
            center_seed = push_stack();
            for (int i = 0; i < 8; i++) {
                px = center_seed.x0 + connects[i].x0;
                py = center_seed.y0 + connects[i].y0;
                if (!(px < 0 || py < 0 || px >= COL || py >= ROW)) {
                    if (img_tmp[py][px] == 0) {
                        if (ImageUsed[ROW - 1 - py][px]
                                != ImageUsed[ROW - 1 - center_seed.y0][center_seed.x0]
                                || ((px == 0 || (py == 0 && i != 6))
                                        && ImageUsed[ROW - 1 - py][px] != 0)) {
                            if (pull_stack(px, py, img_tmp))
                                return;

                            //   if(ImageUsed[ROW - 1 -py][px]==0)while(px<COL-1&&ImageUsed[ROW - 1 -py][px]==0)px++;
                            if (ImageUsed[ROW - 1 - py][px] != 0)
                                while (px > 0
                                        && ImageUsed[ROW - 1 - py][px - 1] != 0)
                                    px--;

//ips114_show_int16(120, 1, stack_top);
//               ips114_show_int16(120, 2, stack_top1);
//               ips114_show_int16(120, 3, max_py);
//                                  ips114_show_int16(120, 4, max_py1);
                            if (line_debug) {
                                ips114_draw_point(px, ROW - 1 - py, RED);

                                my_delay(10);
                            }

                            if (Img.LeftBorder[py] < px)
                                Img.LeftBorder[py] = px;
                            if (py > max_py)
                                max_py = py;

                        }
                    }
                }
            }
        }

        while (stack_top1 != 0 && (max_py >= max_py1 || stack_top == 0)) {
            center_seed = push_stack1();
            for (int i = 0; i < 8; i++) {
                px = center_seed.x0 + connects[i].x0;
                py = center_seed.y0 + connects[i].y0;
                if (!(px < 0 || py < 0 || px >= COL || py >= ROW)) {
                    if (img_tmp[py][px] == 0) {
                        if (ImageUsed[ROW - 1 - py][px]
                                != ImageUsed[ROW - 1 - center_seed.y0][center_seed.x0]
                                || ((px == COL - 1 || (py == 0 && i != 2))
                                        && ImageUsed[ROW - 1 - py][px] != 0)) {
                            if (pull_stack1(px, py, img_tmp))
                                return;
                            // if(ImageUsed[ROW - 1 -py][px]==0)while(px>0&&ImageUsed[ROW - 1 -py][px]!=0)px--;
                            if (ImageUsed[ROW - 1 - py][px] != 0)
                                while (px < COL - 1
                                        && ImageUsed[ROW - 1 - py][px + 1] != 0)
                                    px++;
//                           ips114_show_int16(120, 1, stack_top);
//                           ips114_show_int16(120, 2, stack_top1);
//                           ips114_show_int16(120, 3, max_py);
//                                              ips114_show_int16(120, 4, max_py1);
                            if (line_debug) {
                                ips114_draw_point(px, ROW - 1 - py, BLUE);

                                my_delay(10);
                            }

                            if (Img.RightBorder[py] > px)
                                Img.RightBorder[py] = px;
                            if (py > max_py1)
                                max_py1 = py;

                            // break;
                        }
                    }
                }
            }

        }
    }

    while (stack_top + stack_top1 != 0 && !(min_py == min_py1 && min_py == 0)) {
        while (stack_top != 0 && (stack_top1 == 0 || min_py >= min_py1)) {
            center_seed = push_stack();
            for (int i = 0; i < 8; i++) {
                px = center_seed.x0 + connects[i].x0;
                py = center_seed.y0 + connects[i].y0;
                if (!(px < 0 || py < 0 || px >= COL || py >= ROW)) {
                    if (img_tmp[py][px] == 0) {
                        if (ImageUsed[ROW - 1 - py][px]
                                != ImageUsed[ROW - 1 - center_seed.y0][center_seed.x0]
                                || ((px == 0 || (py == 0 && i != 6))
                                        && ImageUsed[ROW - 1 - py][px] != 0)) {
                            if (pull_stack(px, py, img_tmp))
                                return;

                            //   if(ImageUsed[ROW - 1 -py][px]==0)while(px<COL-1&&ImageUsed[ROW - 1 -py][px]==0)px++;
                            if (ImageUsed[ROW - 1 - py][px] != 0)
                                while (px > 0
                                        && ImageUsed[ROW - 1 - py][px - 1] != 0)
                                    px--;

//ips114_show_int16(120, 1, stack_top);
//               ips114_show_int16(120, 2, stack_top1);
//               ips114_show_int16(120, 3, max_py);
//                                  ips114_show_int16(120, 4, max_py1);
                            if (line_debug) {
                                ips114_draw_point(px, ROW - 1 - py, RED);

                                my_delay(10);
                            }

                            if (Img.LeftBorder[py] < px)
                                Img.LeftBorder[py] = px;

                            if (py < min_py)
                                min_py = py;

                        }
                    }
                }
            }
        }
    }

    left_lost_cnt = 0, right_lost_cnt = 0;

    for (int i = 0; i < ROW; i++) {
        if (Img.LeftBorder[i] < 0 + lose_error
                || Img.LeftBorder[i] > COL - 1 - lose_error)
            Img.LeftBorderFindFlag[i] = 'F', left_lost_cnt++;
        else
            Img.LeftBorderFindFlag[i] = 'T';
        if (Img.RightBorder[i] < 0 + lose_error
                || Img.RightBorder[i] > COL - 1 - lose_error)
            Img.RightBorderFindFlag[i] = 'F', right_lost_cnt++;
        else
            Img.RightBorderFindFlag[i] = 'T';
    }
    solve_line();
    cal_middle_line();

}

void cal_middle_line(void) {
    //���㵱ǰ�е�ƫ����
//    float excursion[ROW];
    float sumcenter = 0;

    float cnt = 0;
    for (Ysite = CenterCalMinRow; Ysite < CenterCalMaxRow; Ysite++) {
        if (Ysite == stack_top || Ysite == stack_top1)
            continue;
        if (Img.LeftBorderFindFlag[Ysite] == 'T'
                || Img.RightBorderFindFlag[Ysite] == 'T') {
            sumcenter += (float) (Img.Center[Ysite] - (COL / 2))
                    / (float) (RoadWide0 / 2) * 100;
            cnt++;
        }
    }
    point_center0 = (int32) (sumcenter / (cnt) * 2.0f);

    point_center1 = point_center0;
    point_center2 = point_center1;
    point_center = point_center0 * 0.7f + point_center1 * 0.2f
            + point_center2 * 0.1f;
}

void judge_circle() {
    judge_circle_right();
    judge_circle_left();
}
int flaa = 0;
void judge_circle_right() {
    int search_col = COL - 1 - 10;

    if (circle_flag.find_right_circle == 0) {

        if ((abs(left_lost_cnt - right_lost_cnt) < RoadWide0 / 3 * 2)
                && left_lost_cnt >= ROW / 5)
            return;
        if (!(ImageUsed[ROW - 1][search_col] == ImageUsed[ROW - 2][search_col]
                && ImageUsed[ROW - 1][search_col]
                        == ImageUsed[ROW - 1 - 5][search_col]
                && ImageUsed[ROW - 1][search_col] != 0
                && Img.LeftBorderFindFlag[0] == Img.LeftBorderFindFlag[1]
                && Img.LeftBorderFindFlag[5] == Img.LeftBorderFindFlag[5]
                && Img.LeftBorderFindFlag[0] == 'T'

        ))
            return;

        if (!(ImageUsed[ROW / 2][COL - 1] == ImageUsed[ROW / 2][COL - 2]
                && ImageUsed[ROW / 2][COL - 1]
                        == ImageUsed[ROW / 2][COL - 1 - 5]
                && ImageUsed[ROW / 2][COL - 1] == 0

        )

        )
            return;

//if(!(
//Img.LeftBorder[0]>COL/2||
//Img.LeftBorder[1]>COL/2||
//Img.LeftBorder[5]>COL/2
//
//
//))return;
        int16 k_error = 3;

        int16 error_num0 = abs(Img.LeftBorder[0] - Img.LeftBorder[ROW / 4]);
        int16 error_num1 = abs(
                Img.LeftBorder[ ROW / 2] - Img.LeftBorder[ROW / 4]);
        int16 error_num2 = abs(
                Img.LeftBorder[ROW / 2] - Img.LeftBorder[ROW / 4 * 3]);

        if (abs(error_num0 - error_num1) > k_error)
            return;
        if (abs(error_num1 - error_num2) > k_error)
            return;
        if (abs(error_num0 - error_num2) > 2 * k_error)
            return;

        if (Img.RightBorder[0] - Img.LeftBorder[0] < RoadWide0 / 4 * 3)
            return;
        if (Img.RightBorder[1] - Img.LeftBorder[1] < RoadWide0 / 4 * 3)
            return;
        if (Img.RightBorder[5] - Img.LeftBorder[5] < RoadWide0 / 4 * 3)
            return;

        circle_flag.find_right_circle = 1;
        ExSpeed = circle_speed;

//        gpio_high(BEEP_PIN);
//        my_delay(50);
//        gpio_low(BEEP_PIN);
    } else if (circle_flag.enter_right_circle == 0) {

        if (!(ImageUsed[ROW - 1][
                Img.LeftBorder[0] + RoadWide0 / 4 * 5 >= COL - 1 ?
                        COL - 1 : Img.LeftBorder[0] + RoadWide0 / 4 * 5]
                ==
                ImageUsed[ROW - 2][
                        Img.LeftBorder[1] + RoadWide0 / 4 * 5 >= COL - 1 ?
                                COL - 1 : Img.LeftBorder[1] + RoadWide0 / 4 * 5]
                && ImageUsed[ROW - 1][
                        Img.LeftBorder[0] + RoadWide0 / 4 * 5 >= COL - 1 ?
                                COL - 1 : Img.LeftBorder[0] + RoadWide0 / 4 * 5]
                        == ImageUsed[ROW - 1 - 5][
                                Img.LeftBorder[5] + RoadWide0 / 4 * 5
                                        >= COL - 1 ?
                                        COL - 1 :
                                        Img.LeftBorder[5] + RoadWide0 / 4 * 5]
                && ImageUsed[ROW - 1][
                        Img.LeftBorder[0] + RoadWide0 / 4 * 5 >= COL - 1 ?
                                COL - 1 : Img.LeftBorder[0] + RoadWide0 / 4 * 5]
                        == 0
                && ImageUsed[ROW - 1][COL - 1]
                        == ImageUsed[ROW - 1][COL - 1 - 1]
                && ImageUsed[ROW - 1][COL - 1]
                        == ImageUsed[ROW - 1][COL - 1 - RoadWide0 / 2]
                && ImageUsed[ROW - 2][COL - 1]
                        == ImageUsed[ROW - 2][COL - 1 - 1]
                && ImageUsed[ROW - 2][COL - 1]
                        == ImageUsed[ROW - 2][COL - 1 - RoadWide0 / 2]
                && ImageUsed[ROW - 1][COL - 1] == ImageUsed[ROW - 2][COL - 1]
                && ImageUsed[ROW - 1][COL - 1] == 0
                && ImageUsed[ROW - 1 - RoadWide0 / 2][COL - 1]
                        == ImageUsed[ROW - 1 - RoadWide0 / 2][COL - 1 - 1]
                && ImageUsed[ROW - 1 - RoadWide0 / 2][COL - 1]
                        == ImageUsed[ROW - 1 - RoadWide0 / 2][COL - 1
                                - RoadWide0 / 2]
                && ImageUsed[ROW - 1 - RoadWide0 / 2][COL - 1] == 0

                && Img.LeftBorderFindFlag[0] == Img.LeftBorderFindFlag[1]
                && Img.LeftBorderFindFlag[5] == Img.LeftBorderFindFlag[5]
                && Img.LeftBorderFindFlag[0] == 'T'

        ))
            return;
        if (Img.RightBorder[0] - Img.LeftBorder[0] > RoadWide0 / 4 * 5)
            return;
        if (Img.RightBorder[1] - Img.LeftBorder[1] > RoadWide0 / 4 * 5)
            return;
        if (Img.RightBorder[5] - Img.LeftBorder[5] > RoadWide0 / 4 * 5)
            return;
        CenterCalMinRow = 0;
        CenterCalMaxRow = 20;

        circle_flag.enter_right_circle = 1;

//        gpio_high(BEEP_PIN);
//        my_delay(50);
//        gpio_low(BEEP_PIN);
//        my_delay(50);
//
//        gpio_high(BEEP_PIN);
//          my_delay(50);
//          gpio_low(BEEP_PIN);

    } else if (circle_flag.on_right_circle == 0) {

        if (Img.LeftBorder[0] - Img.LeftBorder[ROW / 20] >= 0)
            return;
        if (Img.LeftBorder[ROW / 20] - Img.LeftBorder[ROW / 10] >= 0)
            return;

        if (max_py >= ROW / 5 * 3 || max_py <= 5)
            return;

        if (Img.LeftBorderFindFlag[max_py - 2] == 'F')
            return;
        if (Img.LeftBorderFindFlag[max_py - 3] == 'F')
            return;
        if (Img.LeftBorderFindFlag[max_py - 5] == 'F')
            return;

        circle_flag.on_right_circle = 1;

    } else if (circle_flag.out_right_circle == 0) {
        if (max_py <= ROW / 5 * 3 || max_py1 <= ROW / 5 * 3)
            return;

        circle_flag.out_right_circle = 1;
    } else if (circle_flag.leave_right_circle == 0) {
        if (left_lost_cnt >= ROW / 5)
            return;
        int16 k_error = 3;

        int16 error_num0 = abs(Img.LeftBorder[0] - Img.LeftBorder[ROW / 4]);
        int16 error_num1 = abs(
                Img.LeftBorder[ ROW / 2] - Img.LeftBorder[ROW / 4]);
        int16 error_num2 = abs(
                Img.LeftBorder[ROW / 2] - Img.LeftBorder[ROW / 4 * 3]);

        if (abs(error_num0 - error_num1) > k_error)
            return;
        if (abs(error_num1 - error_num2) > k_error)
            return;
        if (abs(error_num0 - error_num2) > 2 * k_error)
            return;
        if (max_py <= ROW / 3 * 2)
            return;

        //     ExSpeed = 0;
        circle_flag.leave_right_circle = 1;
        //   clear_circle_flag();

    } else {

        int16 k_error = 3;
        int16 error_num0 = abs(Img.RightBorder[0] - Img.RightBorder[ROW / 4]);
        int16 error_num1 = abs(
                Img.RightBorder[ ROW / 2] - Img.RightBorder[ROW / 4]);
        int16 error_num2 = abs(
                Img.RightBorder[ROW / 2] - Img.RightBorder[ROW / 4 * 3]);
        if (abs(error_num0 - error_num1) > k_error)
            return;
        if (abs(error_num1 - error_num2) > k_error)
            return;
        if (abs(error_num0 - error_num2) > 2 * k_error)
            return;
        error_num0 = abs(Img.LeftBorder[0] - Img.LeftBorder[ROW / 4]);
        error_num1 = abs(Img.LeftBorder[ ROW / 2] - Img.LeftBorder[ROW / 4]);
        error_num2 = abs(Img.LeftBorder[ROW / 2] - Img.LeftBorder[ROW / 4 * 3]);
        if (abs(error_num0 - error_num1) > k_error)
            return;
        if (abs(error_num1 - error_num2) > k_error)
            return;
        if (abs(error_num0 - error_num2) > 2 * k_error)
            return;
        clear_circle_flag();
        CenterCalMinRow = 5;
        ExSpeed = NormalSpeed;

        CenterCalMaxRow = 30;
//                       gpio_high(BEEP_PIN);
//                               my_delay(50);
//                               gpio_low(BEEP_PIN);
        //     ExSpeed=0;

    }

}
void judge_circle_left() {
    if (circle_flag.find_left_circle == 0) {
        if ((abs(left_lost_cnt - right_lost_cnt) < RoadWide0 / 3 * 2)
                && right_lost_cnt >= ROW / 5)
            return;
        int search_col = 0 + 10;
        if (!(ImageUsed[ROW - 1][search_col] == ImageUsed[ROW - 2][search_col]
                && ImageUsed[ROW - 1][search_col]
                        == ImageUsed[ROW - 1 - 5][search_col]
                && ImageUsed[ROW - 1][search_col] != 0
                && Img.RightBorderFindFlag[0] == Img.RightBorderFindFlag[1]
                && Img.RightBorderFindFlag[5] == Img.RightBorderFindFlag[5]
                && Img.RightBorderFindFlag[0] == 'T'))
            return;
        if (Img.RightBorderFindFlag[ROW / 4] == 'F'
                || Img.RightBorderFindFlag[ROW / 2] == 'F'
                || Img.RightBorderFindFlag[ROW / 4 * 3] == 'F')
            return;
        if (!(ImageUsed[ROW / 2][0] == ImageUsed[ROW / 2][1]
                && ImageUsed[ROW / 2][0] == ImageUsed[ROW / 2][5]
                && ImageUsed[ROW / 2][0] == 0

        )

        )
            return;
//        if(!(
//        Img.RightBorder[0]<COL/2||
//        Img.RightBorder[1]<COL/2||
//        Img.RightBorder[5]<COL/2
//
//
//        ))return;
        int16 k_error = 3;

        int16 error_num0 = abs(Img.RightBorder[0] - Img.RightBorder[ ROW / 4]);
        int16 error_num1 = abs(
                Img.RightBorder[ ROW / 2] - Img.RightBorder[ ROW / 4]);

        int16 error_num2 = abs(
                Img.RightBorder[ ROW / 2] - Img.RightBorder[ ROW / 4 * 3]);
        if (abs(error_num0 - error_num1) > k_error)
            return;
        if (abs(error_num1 - error_num2) > k_error)
            return;
        if (abs(error_num0 - error_num2) > 2 * k_error)
            return;
        if (Img.RightBorder[0] - Img.LeftBorder[0] < RoadWide0 / 4 * 3)
            return;
        if (Img.RightBorder[1] - Img.LeftBorder[1] < RoadWide0 / 4 * 3)
            return;
        if (Img.RightBorder[5] - Img.LeftBorder[5] < RoadWide0 / 4 * 3)
            return;
        circle_flag.find_left_circle = 1;
        ExSpeed = circle_speed;
        gpio_high(BEEP_PIN);
        my_delay(50);
        gpio_low(BEEP_PIN);
    } else if (circle_flag.enter_left_circle == 0) {

        if (!(ImageUsed[ROW - 1][
                Img.RightBorder[0] + RoadWide0 / 4 * 5 >= COL - 1 ?
                        COL - 1 : Img.RightBorder[0] + RoadWide0 / 4 * 5]
                ==
                ImageUsed[ROW - 2][
                        Img.RightBorder[1] + RoadWide0 / 4 * 5 >= COL - 1 ?
                                COL - 1 : Img.RightBorder[1] + RoadWide0 / 4 * 5]
                && ImageUsed[ROW - 1][
                        Img.RightBorder[0] + RoadWide0 / 4 * 5 >= COL - 1 ?
                                COL - 1 : Img.RightBorder[0] + RoadWide0 / 4 * 5]
                        == ImageUsed[ROW - 1 - 5][
                                Img.RightBorder[5] + RoadWide0 / 4 * 5
                                        >= COL - 1 ?
                                        COL - 1 :
                                        Img.RightBorder[5] + RoadWide0 / 4 * 5]

                && ImageUsed[ROW - 1][
                        Img.RightBorder[0] + RoadWide0 / 4 * 5 >= COL - 1 ?
                                COL - 1 : Img.RightBorder[0] + RoadWide0 / 4 * 5]
                        == 0 && ImageUsed[ROW - 1][0] == ImageUsed[ROW - 1][1]
                && ImageUsed[ROW - 1][0] == ImageUsed[ROW - 1][RoadWide0 / 2]
                && ImageUsed[ROW - 2][0] == ImageUsed[ROW - 2][1]
                && ImageUsed[ROW - 2][0] == ImageUsed[ROW - 2][RoadWide0 / 2]
                && ImageUsed[ROW - 1][0] == ImageUsed[ROW - 2][0]
                && ImageUsed[ROW - 1][0] == 0
                && ImageUsed[ROW - 1 - RoadWide0 / 2][0]
                        == ImageUsed[ROW - 1 - RoadWide0 / 2][1]
                && ImageUsed[ROW - 1 - RoadWide0 / 2][0]
                        == ImageUsed[ROW - 1 - RoadWide0 / 2][RoadWide0 / 2]
                && ImageUsed[ROW - 1 - RoadWide0 / 2][0] == 0

                && Img.RightBorderFindFlag[0] == Img.RightBorderFindFlag[1]
                && Img.RightBorderFindFlag[0] == Img.RightBorderFindFlag[5]
                && Img.RightBorderFindFlag[0] == 'T'

        ))
            return;

        if (Img.RightBorder[0] - Img.LeftBorder[0] > RoadWide0 / 4 * 5)
            return;
        if (Img.RightBorder[1] - Img.LeftBorder[1] > RoadWide0 / 4 * 5)
            return;
        if (Img.RightBorder[5] - Img.LeftBorder[5] > RoadWide0 / 4 * 5)
            return;

        CenterCalMinRow = 0;
        CenterCalMaxRow = 20;

        circle_flag.enter_left_circle = 1;

//        gpio_high(BEEP_PIN);
//        my_delay(50);
//        gpio_low(BEEP_PIN);
//        my_delay(50);
//
//        gpio_high(BEEP_PIN);
//          my_delay(50);
//          gpio_low(BEEP_PIN);

    } else if (circle_flag.on_left_circle == 0) {
        if (max_py1 >= ROW / 5 * 3 || max_py1 <= 5)
            return;

        if (Img.RightBorder[0] - Img.RightBorder[ROW / 20] <= 0)
            return;

        if (Img.RightBorder[ROW / 20] - Img.RightBorder[ROW / 10] <= 0)
            return;

        if (Img.RightBorderFindFlag[max_py1 - 2] == 'F')
            return;

        if (Img.RightBorderFindFlag[max_py1 - 3] == 'F')
            return;

        if (Img.RightBorderFindFlag[max_py1 - 5] == 'F')
            return;

        circle_flag.on_left_circle = 1;

    } else if (circle_flag.out_left_circle == 0) {
        if (max_py <= ROW / 5 * 3 || max_py1 <= ROW / 5 * 3)
            return;
        circle_flag.out_left_circle = 1;
    } else if (circle_flag.leave_left_circle == 0) {
        if (left_lost_cnt >= ROW / 5)
            return;
        int16 k_error = 3;
        int16 error_num0 = abs(Img.RightBorder[0] - Img.RightBorder[ROW / 4]);
        int16 error_num1 = abs(
                Img.RightBorder[ ROW / 2] - Img.RightBorder[ROW / 4]);
        int16 error_num2 = abs(
                Img.RightBorder[ROW / 2] - Img.RightBorder[ROW / 4 * 3]);
        if (abs(error_num0 - error_num1) > k_error)
            return;
        if (abs(error_num1 - error_num2) > k_error)
            return;
        if (abs(error_num0 - error_num2) > 2 * k_error)
            return;
        if (max_py <= ROW / 3 * 2)
            return;

        circle_flag.leave_left_circle = 1;

    } else {
        int16 k_error = 3;
        int16 error_num0 = abs(Img.RightBorder[0] - Img.RightBorder[ROW / 4]);
        int16 error_num1 = abs(
                Img.RightBorder[ ROW / 2] - Img.RightBorder[ROW / 4]);
        int16 error_num2 = abs(
                Img.RightBorder[ROW / 2] - Img.RightBorder[ROW / 4 * 3]);
        if (abs(error_num0 - error_num1) > k_error)
            return;
        if (abs(error_num1 - error_num2) > k_error)
            return;
        if (abs(error_num0 - error_num2) > 2 * k_error)
            return;
        error_num0 = abs(Img.LeftBorder[0] - Img.LeftBorder[ROW / 4]);
        error_num1 = abs(Img.LeftBorder[ ROW / 2] - Img.LeftBorder[ROW / 4]);
        error_num2 = abs(Img.LeftBorder[ROW / 2] - Img.LeftBorder[ROW / 4 * 3]);
        if (abs(error_num0 - error_num1) > k_error)
            return;
        if (abs(error_num1 - error_num2) > k_error)
            return;
        if (abs(error_num0 - error_num2) > 2 * k_error)
            return;
        clear_circle_flag();
        //   ExSpeed=0;
        CenterCalMinRow = 5;
        CenterCalMaxRow = 30;
        ExSpeed = NormalSpeed;

//                       gpio_high(BEEP_PIN);
//                               my_delay(50);
//                               gpio_low(BEEP_PIN);
    }
}
void clear_circle_flag() {
    circle_flag.find_left_circle = 0;
    circle_flag.enter_left_circle = 0;
    circle_flag.on_left_circle = 0;
    circle_flag.out_left_circle = 0;
    circle_flag.leave_left_circle = 0;
    circle_flag.find_right_circle = 0;
    circle_flag.enter_right_circle = 0;
    circle_flag.on_right_circle = 0;
    circle_flag.out_right_circle = 0;
    circle_flag.leave_right_circle = 0;
}
void init_flag() {
    clear_circle_flag();

}
void solve_line() {

    for (int Ysite = 0; Ysite < StartScanRow + 1; Ysite++) {
        Img.Center[Ysite] = COL / 2;
        Img.RightBorderFindFlag[Ysite] = 'T';
        Img.LeftBorderFindFlag[Ysite] = 'T';
    }
    if (circle_flag.find_left_circle == 0
            && circle_flag.find_right_circle == 0) {
        for (int Ysite = StartScanRow + 1; Ysite < ROW; Ysite++) {
            //�˴�Ӧ���ж��߽�����߽��д���
            if (Img.RightBorderFindFlag[Ysite] == 'T'
                    && Img.LeftBorderFindFlag[Ysite] == 'T') //�޶���
                            {
                Img.Center[Ysite] = (Img.LeftBorder[Ysite]
                        + Img.RightBorder[Ysite]) / 2;
            } else if (Img.RightBorderFindFlag[Ysite] == 'F'
                    && Img.LeftBorderFindFlag[Ysite] == 'F') //���߶���
                Img.Center[Ysite] = CenterCol;
            else if (Img.RightBorderFindFlag[Ysite] == 'F'
                    && Img.LeftBorderFindFlag[Ysite] == 'T') //�����ұ���
                            {
                Img.Center[Ysite] = Img.LeftBorder[Ysite] + RoadWide0 / 2;
            } else if (Img.RightBorderFindFlag[Ysite] == 'T'
                    && Img.LeftBorderFindFlag[Ysite] == 'F')  //�������
                            {
                Img.Center[Ysite] = Img.RightBorder[Ysite] - RoadWide0 / 2;
            }
        }
    } else if (circle_flag.find_left_circle == 1) {
        if (circle_flag.enter_left_circle == 0) {
            for (int Ysite = StartScanRow + 1; Ysite < ROW; Ysite++) {
                //�˴�Ӧ���ж��߽�����߽��д���
                if (Img.RightBorderFindFlag[Ysite] == 'T'
                        && Img.LeftBorderFindFlag[Ysite] == 'T') //�޶���

                    Img.Center[Ysite] = Img.RightBorder[Ysite] - RoadWide0 / 2;

                else if (Img.RightBorderFindFlag[Ysite] == 'T'
                        && Img.LeftBorderFindFlag[Ysite] == 'F') //���������

                    Img.Center[Ysite] = Img.RightBorder[Ysite] - RoadWide0 / 2;
                else
                    Img.Center[Ysite] = CenterCol;

            }
        } else {
            if (circle_flag.on_left_circle == 0) {

                for (int Ysite = StartScanRow + 1; Ysite < ROW; Ysite++) {
                    Img.Center[Ysite] = Img.LeftBorder[Ysite] + RoadWide0 / 2;
                }
            } else {
                if (circle_flag.out_left_circle == 0) {
                    for (int Ysite = StartScanRow + 1; Ysite < ROW; Ysite++) {
                        Img.Center[Ysite] = Img.RightBorder[Ysite]
                                - RoadWide0 / 5 * 3;
                    }
                } else {
                    if (circle_flag.leave_left_circle == 0) {
                        for (int Ysite = StartScanRow + 1; Ysite < ROW;
                                Ysite++) {
                            Img.Center[Ysite] = Img.LeftBorder[Ysite]
                                    + RoadWide0 / 2;
                        }
                    } else {
                        for (int Ysite = StartScanRow + 1; Ysite < ROW;
                                Ysite++) {
                            Img.Center[Ysite] = Img.RightBorder[Ysite]
                                    - RoadWide0 / 2;
                        }
                    }
                }
            }
        }
    } else if (circle_flag.find_right_circle == 1) {
        if (circle_flag.enter_right_circle == 0) {
            for (int Ysite = StartScanRow + 1; Ysite < ROW; Ysite++) {
                //�˴�Ӧ���ж��߽�����߽��д���
                if (Img.RightBorderFindFlag[Ysite] == 'T'
                        && Img.LeftBorderFindFlag[Ysite] == 'T') //�޶���
                    Img.Center[Ysite] = Img.LeftBorder[Ysite] + RoadWide0 / 2;
                else if (Img.RightBorderFindFlag[Ysite] == 'F'
                        && Img.LeftBorderFindFlag[Ysite] == 'T') //�����ұ���

                    Img.Center[Ysite] = Img.LeftBorder[Ysite] + RoadWide0 / 2;
                else
                    Img.Center[Ysite] = CenterCol;

            }
        } else {
            if (circle_flag.on_right_circle == 0) {
                for (int Ysite = StartScanRow + 1; Ysite < ROW; Ysite++) {
                    Img.Center[Ysite] = Img.RightBorder[Ysite] - RoadWide0 / 2;
                }
            } else {
                if (circle_flag.out_right_circle == 0) {
                    for (int Ysite = StartScanRow + 1; Ysite < ROW; Ysite++) {
                        Img.Center[Ysite] = Img.LeftBorder[Ysite]
                                + RoadWide0 / 5 * 3;
                    }
                } else {
                    if (circle_flag.leave_right_circle == 0) {
                        for (int Ysite = StartScanRow + 1; Ysite < ROW;
                                Ysite++) {
                            Img.Center[Ysite] = Img.RightBorder[Ysite]
                                    - RoadWide0 / 3;
                        }
                    } else {
                        for (int Ysite = StartScanRow + 1; Ysite < ROW;
                                Ysite++) {
                            Img.Center[Ysite] = Img.LeftBorder[Ysite]
                                    + RoadWide0 / 5 * 2;
                        }
                    }
                }
            }
        }
    }

}
