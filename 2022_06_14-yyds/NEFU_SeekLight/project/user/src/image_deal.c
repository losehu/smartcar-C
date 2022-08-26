/**
 //@FileName    :image_deal.c
 //@CreatedDate :2021年12月31日
 //@Author      :LiHao&WuSheng
 //@Description :图像的基本处理，剪切|二值化|逆透视
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
//uint8_t CamImage[USED_ROW][USED_COL];//总钻风获取的灰度图像，由ImageBuffer()获取
uint8_t SimBinImage[USED_ROW][USED_COL]; //简易无逆透视二值化图像，由GetSimBinImage()获取
uint8_t AdpBinImage[USED_ROW][USED_COL]; //自适应阈值二值化图像，

//uint8_t PerImage[TRFED_ROW][TRFED_COL];//Transformed_Binary_Image透视变换后的图像

int16_t ThresholdAdd = 5; //对大津法得到的阈值进行手动调节
uint8_t *PerImg_ip[TRFED_ROW][TRFED_COL];
int16 seed_left_x, seed_left_y, seed_right_x, seed_right_y;

/**
 *@Name         :ImageBuffer
 *@Description  :ImageBuffer图像缓冲&剪切到CamImage中
 *@Param            :None
 *@Return       :None
 *@Sample       :ImageBuffer();
 **/
int fake_do = 0;
void ImageBuffer(void) {
    //memcpy(&CamImage, &mt9v03x_image_dvp, USED_ROW*USED_COL);
    uint8_t FirstRow = 0; //起始行 从0开始
    uint8_t FirstCol = (MT9V03X_DVP_W - USED_COL) / 2; //起始列
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
 *@Name         :GetBinImage
 *@Description  :GetBinImage
 *@Param            :img[ROW][COL]  原图像
 *@Return       :None
 *@Sample       :GetSimBinImage(CamImage[ROW][COL]);
 **/



void GetSimBinImage(void) {
    unsigned short i = 0, j = 0;
//memcpy(SimBinImage,CamImage,188*120);
//if(adj_flag)    ips114_show_gray_image_vec(0,0,PerImg_ip,TRFED_COL,TRFED_ROW,TRFED_COL,TRFED_ROW,0);
//
 //  process_image();






    uint8_t Threshold1 = ThresholdAdd + OSTUThreshold(CamImage);
  //  sendimg_binary(mt9v03x_image_dvp[0], 90, 60,Threshold1);
  //   sendimg_binary_vec();


    for (i = 0; i < USED_ROW; i++) {
        for (j = 0; j < USED_COL; j++) {
            if (CamImage[i][j] > Threshold1) //数值越大，显示的内容越多，较浅的图像也能显示出来
                    {
                SimBinImage[i][j] = 255;
            } else {
                SimBinImage[i][j] = 0;
            }
        }
    }
//    uint8_t aa[100][114];
//    for(int i=0;i<100;i++)
//    {
//        for(int j=0;j<114;j++)
//        {
//        aa[i][j]=ImageUsed[i][j];
//        }
//    }
//    sendimg_binary(aa[0], 114, 100,50);
}
/**
 *@Name         :GetPerImage
 *@Description  :GetPerImage 得到逆透视后的图像，用于显示
 *@Param            :None
 *@Return       :None
 *@Sample       :GetPerImage();
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
 *@Name         :ImagePerspectiveInit
 *@Description  :ImagePerspectiveInit 得到透视变换的图像的地址映射;只运行一次
 *@Param            :None
 *@Return       :None
 *@Sample       :ImagePerspectiveInit();
 **/
void ImagePerspective_Init(void) {

    static uint8_t BlackColor = GrayPoint;
    double change_un_Mat[3][3] = {          //114w*100h
            { -0.01609759704190238, 0.01932561893613478, -2.040617594981866 }, {
                    0.0004352209945470896, -0.000367865364438621,
                    -0.7035606436969671 }, { 1.115951268069474e-005,
                    0.0001970185393508392, -0.03104642853440032 }, };
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
                    >= 0&& local_y >= 0 && local_y < USED_ROW && local_x < USED_COL){
                PerImg_ip[j][i] = &PER_IMG[local_y][local_x];
            }
            else {
                PerImg_ip[j][i] = &BlackColor;          //&PER_IMG[0][0];
            }

        }
    }
     RUBO_IMG_INIT();
}
/**
 *@Name         :GetAdpBinImage
 *@Description  :Get_Adaptive_Binary_Image
 *@Param            :block0 比较赋值区块大小必须为偶数；block1 计算阈值区块大小必须为偶数必须大于block0
 *@Return       :None
 *@Sample       :GetAdpBinImage(7);
 **/
void GetAdpBinImage(short block0, short block1) {
//    short block0 = 2;//内区块
//    short block1 = 4;//外区块
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
            for (dx1 = 0; dx1 < block1; dx1++) {          //计算block1里的阈值
                for (dy1 = 0; dy1 < block1; dy1++) {
                    thres += CamImage[y + dy1][x + dx1];
                }
            }
            thres = thres / (block1 * block1);
            for (dx0 = 0; dx0 < block0; dx0++) {          //对block0里的值二值化
                for (dy0 = 0; dy0 < block0; dy0++) {
                    AdpBinImage[y + dy0 + (block1 - block0) / 2][x + dx0
                            + (block1 - block0) / 2] =
                    CamImage[y + dy0 + (block1 - block0) / 2][x + dx0
                            + (block1 - block0) / 2] > thres ? 255 : 0;
                }
            }
            if (x == 0) {          //修左黑边
                for (uint8_t jx0 = 0; jx0 < (block1 - block0) / 2; jx0++) {
                    for (int ix0 = 0; ix0 < block0; ix0++) {
                        AdpBinImage[ix0][jx0] =
                        CamImage[ix0][jx0] > thres ? 255 : 0;
                    }
                }
            }
            if (x) {          //修右黑边

            }
            if (y == 0) {          //修上黑边
                for (uint8_t jy0 = 0; jy0 < block0; jy0++) {
                    for (int iy0 = 0; iy0 < (block1 - block0) / 2; iy0++) {
                        AdpBinImage[iy0][jy0] =
                        CamImage[iy0][jy0] > thres ? 255 : 0;
                    }
                }
            }
            if (y) {          //修下黑边

            }
        }
    }
}

/**
 *@Name         :OSTUThreshold
 *@Description  :OSTUThreshold大津法得到阈值
 *@Param            :img[USED_ROW][USED_COL]  原图像
 *@Return       :得到的阈值
 *@Sample       :由GetSimBinImage()调用
 **/
uint8_t OSTUThreshold(uint8_t img[USED_ROW][USED_COL]) {
    signed short j, i;
    unsigned long Amount = 0;
    unsigned long PixelBack = 0;
    unsigned long PixelshortegralBack = 0;
    unsigned long Pixelshortegral = 0;
    signed long PixelshortegralFore = 0;
    signed long PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
    signed short MinValue, MaxValue;
    signed short Threshold0 = 0;
    unsigned char HistoGram[256];              //

    for (i = 0; i < 256; i++)
        HistoGram[i] = 0; //初始化灰度直方图

    for (i = 0; i < USED_ROW; i++) {
        for (j = 0; j < USED_COL; j++) {
            HistoGram[img[i][j]]++; //统计灰度级中每个像素在整幅图像中的个数
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++)
        ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0;
            MaxValue--)
        ; //获取最大灰度的值

    if (MaxValue == MinValue)
        return MaxValue;         // 图像中只有一个颜色
    if (MinValue + 1 == MaxValue)
        return MinValue;        // 图像中只有二个颜色

    for (i = MinValue; i <= MaxValue; i++)
        Amount += HistoGram[i];        //  像素总数

    Pixelshortegral = 0;
    for (i = MinValue; i <= MaxValue; i++) {
        Pixelshortegral += HistoGram[i] * i;        //灰度值总数
    }
    SigmaB = -1;
    for (i = MinValue; i < MaxValue; i++) {
        PixelBack = PixelBack + HistoGram[i];     //前景像素点数
        PixelFore = Amount - PixelBack;           //背景像素点数
        OmegaBack = (float) PixelBack / Amount;   //前景像素百分比
        OmegaFore = (float) PixelFore / Amount;   //背景像素百分比
        PixelshortegralBack += HistoGram[i] * i;  //前景灰度值
        PixelshortegralFore = Pixelshortegral - PixelshortegralBack;  //背景灰度值
        MicroBack = (float) PixelshortegralBack / PixelBack;   //前景灰度百分比
        MicroFore = (float) PixelshortegralFore / PixelFore;   //背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore)
                * (MicroBack - MicroFore);   //计算类间方差
        if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
                {
            SigmaB = Sigma;
            Threshold0 = i;
        }
    }
    return Threshold0;                        //返回最佳阈值;
}

///*******************上交******************************/
//Camer camer;
//int camer_init(void)
//{
//    mt9v03x_init_dvp(); //初始化摄像头 使用CSI接口
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
//    static float standardMid = nWidth/2;    //标准中线列数
//    static float camServoError; //中线加权偏差
//    static float errorAverge;    //中线偏差
//    static uint8 i;
//    static double sum,weight_sum;  //权重中线和，权重和
//    static float sum_ave,add_ave; //不带权重中线和，加的中线点数
//    static float camServoErrorLast,camServoErrorAdd;
//
//
//
//
//    uint8 image[TRFED_ROW][TRFED_COL];
//
//    //偏差权重值,底部15行舍去
//        static uint16 weight[nHeight]={  0,0,0,0,0,0,0,0,0,0, //40行到49行
//                            0,0,0,0,0,1,1,1,1,1, //40行到49行
//                            2,2,1,2,1,2,1,2,1,2,  //60行到59行
//                            4,3,3,4,2,4,2,5,2,5,   //20行到29行
//                            4,4,4,4,4,4,5,5,5,5,   //20行到29行
//                            4,10,10,2,10,2,10,2,10,10,   //20行到29行
//                            14,14,14,34,24,24,15,35,55,65,   //20行到29行
//                            31,52,43,54,55,56,37,58,79,70,   //20行到29行
//                            40,70,80,70,85,66,60,60,80,80,   //20行到29行
//                            70,100,130,104,100,100,80,80,90,90,  //40行到49行
//                            155,135,105,105,155,100,100,100,100,105,  //40行到49行
//                            0,0,0,0,0,0,0,0,0,0 //70行到79行
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
//        //进行透视变换
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
//        //单纯显示使用
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
//            mt9v03x_finish_flag = 0;//在图像使用完毕后  务必清除标志位，否则不会开始采集下一幅图像
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
////一次函数连线
//void lian_xian_zhi(int column1,int row1,int column2,int row2,Point_info_t* point)
////a,b一队，de一队    a>b   两点之间连线    二次函数
//{
//
//    int i=0,ab=0,cd=0;
//    for(i=row1; i>=row2; i--)
//    {
//        cd=(int)(  column2-column1)*(row1-i)/(row1-row2); //直线
//        //cd=(column2-column1)*(row1-i)*(row1-i)/(row1-row2)/(row1-row2); //二次函数
//        ab=(int)cd+column1;
//        ab=range_protect(ab,159,0);
//        point[i].x = ab;
//
//    }
//}
// /********赛道宽度(1.3m前瞻)**********/
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
// /********中线补线斜率**********/
//uint8 k_tiaojie=7;//(进环补线)
//uint8 k_tiaojie_huandao=6;//(预出环补线)
//uint8 k_tiaojie_huandao1=6;//(出环补线)
//uint8 k_tiaojie_huandao2=7;//(出环补线)
//uint8 k_forkroad = 7;
// /********扫线变量(结构体)**********/
//Point_info_t left_point[nHeight];           //左线点情况
//Point_info_t right_point[nHeight];          //右线点情况
//Point_info_t mid_point[nHeight];            //中线点情况
//static int16 last_mid_point;
//
///********有效行**********/
//int8  length=nHeight;
//uint8 length_old_end;
//int   length_old;
//uint8 length_flag;
//
//
///********赛道环境**********/
//float k_right[nHeight]={0};  //  四轮定值80，目前估计是因为他们nHeight就是80，忘了改成宏定义了
//float k_left[nHeight]={0};   //  四轮定值80，目前估计是因为他们nHeight就是80，忘了改成宏定义了
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
//int first_left_point=nHeight;//左边找到的第一个点        //nHeight80
//int first_right_point=nHeight;//右边找到的第一个点
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
///********环岛**********/
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
///********十字**********/
//int HOW;
//uint8 crossroad=0;
//uint8 crossroad_flag = 0 ;
//uint8 crossroad_ready=0;
//int16 crossroad_count  =0;
//int buzzer_flag =0 ;
//
///********三岔路口(部分)**********/
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
///**边界信息提取----图像主函数**/
//void edge_line_extract(uint8 *src)
//{
//
//    int16 i,j;
//    uint8 length_flag=0;
//    int first_lose_flag = 0;
//
//
//    /**图像基础信息**/
//    all_lose_count=0;                   //清零计数变量
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
//        //三岔路口使用参数
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
//     /*****加入*****/
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
//     /**图像底下十五行舍去，寻找第十六行中点**/
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
//    last_mid_first=first_line_sum/first_line_num;  //求中线样本均值 累计白点坐标及个数
//    first_line_num=0;
//    first_line_sum=0;
//    last_mid_point=last_mid_first;
//
//    if(last_mid_point==BLACKFORCAM)               //如果第一行全为黑就将中值设为80
//    {
//        last_mid_point=nWidth/2;//可以处理车过急弯图像丢失
//    }
//
//
//    //三岔路口使用参数
//    for(int i = nHeight-10;i>0 ; i-- )
//    {
//        if(src+i*nWidth+nWidth/2 == 0)
//        {
//            white_first = i;//中间第一个黑点所在的行数
//            break;
//        }
//     }
//
//    for(int i = nHeight;i<nWidth;i++)
//    {
//        if(src+white_first*nWidth+i==BLACKFORCAM)
//        white_num_you++;//扫线结束
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
//    }//扫线结束
//
//
//    mid_point[14].x = last_mid_point;
//
//    int K ;
//    /**边界限制条件**/  //nHeight = 120  Data:2021.8.30
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
//    /*****整体扫线for循环开始*****/
//    for(K=(nHeight-15);K>0;K--)                          //nHeight=80高度
//    {
//
//       pimg=src+(K)*nWidth;                           //nWidth=160宽度
//       i=nHeight-K;
//
//       /**扫左线**/
//       for(j= last_mid_point;j>=0;j--)
//       {
//           if(BLACKFORCAM==*(pimg+j) && j!=limit[i])                  //pimg+j里面的是黑色就记录下来     //这一行数组溢出!!!!!！！！！！！
//           {                                             //记录坐标，x表示高度
//               left_point[i].x=j+1;
//               left_point[i].chuli=j+1;
//               left_point[i].state=NORMAL;
//               left_case=1;
//
//               //粗略计算斜率
//               if(i>10&&left_point[i-1].state==NORMAL&&left_point[i-2].state==NORMAL)
//               {k_left[i]=((float)(left_point[i].x-left_point[i-1].x)+(float)(left_point[i-1].x-left_point[i-2].x)+(float)(left_point[i-2].x-left_point[i-3].x))/3;}
//               break;
//
//           }
//           else if(j==limit[i])                       //丢线
//           {
//               left_point[i].x=0;
//               left_point[i].chuli=0;
//               left_point[i].state=LOSE;
//               left_case=3;
//
//           }
//       }
//       /**扫右线**/
//       for(j=last_mid_point;j<nWidth;j++)         //记录right_point    first_right_point   first_right_lose
//       {
//           if(BLACKFORCAM==*(pimg+j) && j!=(nWidth-limit[i]))                                     //&&*(pimg+j-1)+*(pimg+j-2)+*(pimg+j-3)==0)
//           {
//               right_point[i].x=j-1;
//               right_point[i].chuli=j-1;
//               right_case=1;
//               right_point[i].state=NORMAL;
//
//               //粗略计算斜率
//               if(i>10&&right_point[i-1].state==NORMAL&&right_point[i-2].state==NORMAL)
//               {k_right[i]=((float)(right_point[i-1].x-right_point[i].x)+(float)(right_point[i-2].x-right_point[i-1].x)+(float)(right_point[i-3].x-right_point[i-2].x))/3;}
//
//               break;
//           }
//
//           else if(j==nWidth-1-limit[i])                  //表示first_right_lose还没有改变是第一次丢失
//           {
//               right_point[i].x=nWidth-1;
//               right_point[i].chuli=nWidth-1;
//               right_case=0;
//               right_point[i].state=LOSE;
//           }
//         }
//
//
//       //这里用来判断是否为正常直道
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
//        //3.确定该行情况           记录赛道信息
//             if(length_flag==0)
//             {
//                switch(left_case-right_case)      //这里是对每一行都处理一次
//                 {
//                 case 0:             //左右边线都扫到
//                     all_normal_count++;
//                     right_normal_count++;
//                     left_normal_count++;
//
//                     if(i>35)
//                     {
//                         all_normal_35up++;  //有使用
//                         right_normal_35up++;
//                         left_normal_35up++;
//                     }
//                     if(i>45)
//                     {
//                         all_normal_45up++;  //有使用
//                         right_normal_45up++;
//                         left_normal_45up++;
//                     }
//                     break;
//                 case 1:            //扫到左边线，丢失右边线
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
//                 case 2:           //丢失左边线，扫到右边线
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
//                 case 3:          //左右边线均丢失
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
//                     switch(left_case-right_case)      //这里是对每一行都处理一次
//                     {
//                     case 0:             //左右边线都扫到
//                         if(i<=45&&i>15)
//                         {
//                             all_normal_15_30++;
//                             left_normal_15_30++;
//                             right_normal_15_30++;
//                         }
//                         break;
//                     case 1:            //扫到左边线，丢失右边线
//                         if(i<=45&&i>15)
//                         {
//                             left_normal_15_30++;
//                             right_lose_15_30++;
//                         }
//                         break;
//                     case 2:           //丢失左边线，扫到右边线
//                         if(i<=45&&i>15)
//                         {
//                             left_lose_15_30++;
//                             right_normal_15_30++;
//                         }
//                         break;
//                     case 3:          //左右边线均丢失
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
//     //用赛道宽度判断丢线数
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
//    //整体扫线结束
//
//
//
//
//
//    /********正常补线********/
//
//          if(i<17)
//          {
//            mid_point[i].x=(left_point[i].x+right_point[i].x)/2;
//          }//底部赋中值保证平滑
//          else if(left_point[i].state==LOSE && right_point[i].state==LOSE)
//          {
//            mid_point[i].x=(left_point[i].x+right_point[i].x)/2;
//          }
//          else if(left_point[i].state==LOSE)//左侧丢失
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
//          else if(right_point[i].state==LOSE)//右侧丢失
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
//              if(circle_ready_r==1)      // 去掉了else
//              {mid_point[i].x=left_point[i].x+distance3[i]/2;}
//              else if(circle_ready_l==1)
//              {mid_point[i].x=right_point[i].x-distance3[i]/2;}
//              else
//              {
//                mid_point[i].x=(left_point[i].x+right_point[i].x)/2;
//              }
//          }
//
//       //边线限幅
//     if(mid_point[i].x<0)     {mid_point[i].x=0;}
//     if(mid_point[i].x>=nWidth)   {mid_point[i].x=nWidth;}
//     if(right_point[i].x>nWidth) {right_point[i].x=nWidth;}
//     if(left_point[i].x<0)    {left_point[i].x=0;}
//
//
//
//////////////******求出有效行******///////////////////////
//
//
//    if(right_point[i].x<(15+left_point[i].x)&&length_flag==0)
//        if(right_point[i].x<(15+left_point[i].x)&&length_flag==0)  //去掉了上面这一行的else
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
////三岔路口处理
//void forkroad_deal(void)
//{
//
//    //黄氏三岔路口检测
//    //寻找跳变点补线法
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
//        HOW = 4;//斜出十字开始补线标志，比赛朝一个方向 ，只考虑了一种
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
//// 自适应阈值二值化
//void adaptive_threshold(image_t *img0, image_t *img1, int block_size, int down_value, uint8_t low_value, uint8_t high_value)
//{
//    int half = block_size / 2;
//    // 先遍历y后遍历x比较cache-friendly
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
///************************八领域*******************************/
size_point stack_seed[stack_size];    //栈
size_point stack_seed1[stack_size];    //栈
size_point stack_seed2[stack_size1];    //栈
size_point stack_seed3[stack_size1];    //栈
int16 stack_top = 0, stack_top1 = 0, stack_top2 = 0, stack_top3 = 0;
const int16 search_start_line = 3;
int16 start_line = search_start_line;

int16 left_lost_cnt = 0, right_lost_cnt = 0;
bool stop_flag = 0;
int16 max_py = 0, max_py1 = 0;
int16 min_py = 0, min_py1 = 0;
const int CenterCalMinRow_tmp = 0;
const int CenterCalMaxRow_tmp = 95;
int CenterCalMinRow = CenterCalMinRow_tmp;
int CenterCalMaxRow = CenterCalMaxRow_tmp; //15
int CenterCalMinRow_circle = 0;
int CenterCalMaxRow_circle = 20; //15

int lose_error = 10;
#define line_debug 0
bool search_line_flag = 0;
size_point connects[16] = {    //八领域扫点
        { 0, -1 },    //上
                { -1, -1 },    //左上123 567
                { -1, 0 },    //左
                { -1, 1 },    //左下
                { 0, 1 },    //下
                { 1, 1 },    //右下
                { 1, 0 },    //右
                { 1, -1 },    //右上
        };
int16 stack_max = 0;
bool pull_stack(int16 x, int16 y, uint8_t img_tmp[][COL]) //入栈
{
    if (stack_top == stack_size) {
        search_line_flag = 1;
        return 1;
    }
    img_tmp[y][x] = 1;
    stack_seed[stack_top].x0 = x;
    stack_seed[stack_top].y0 = y;
    stack_top++;
    return 0;
}
size_point push_stack() //出栈
{
    stack_seed[stack_top].y0 = 0;
    stack_seed[stack_top].x0 = 0;
    return stack_seed[--stack_top];
}
bool pull_stack1(int16 x, int16 y, uint8_t p_Pixels[][COL]) //入栈
{
    if (stack_top1 == stack_size) {
        search_line_flag = 1;
        return 1;
    }
    p_Pixels[y][x] = 1;
    stack_seed1[stack_top1].x0 = x;
    stack_seed1[stack_top1].y0 = y;
    stack_top1++;
    return 0;
}
size_point push_stack1() //出栈
{
    stack_seed1[stack_top1].y0 = 0;
    stack_seed1[stack_top1].x0 = 0;
    return stack_seed1[--stack_top1];
}
bool pull_stack2(int16 x, int16 y) //入栈
{
    if (stack_top2 == stack_size1) {
        search_line_flag = 1;
        return 1;
    }
    stack_seed2[stack_top2].x0 = x;
    stack_seed2[stack_top2].y0 = y;
    stack_top2++;
    return 0;
}
size_point push_stack2() //出栈
{
    stack_seed2[stack_top2].y0 = 0;
    stack_seed2[stack_top2].x0 = 0;
    return stack_seed2[--stack_top2];
}
bool pull_stack3(int16 x, int16 y) //入栈
{
    if (stack_top3 == stack_size1) {
        search_line_flag = 1;
        return 1;
    }
    stack_seed3[stack_top3].x0 = x;
    stack_seed3[stack_top3].y0 = y;
    stack_top3++;
    return 0;
}
size_point push_stack3() //出栈
{
    stack_seed3[stack_top3].y0 = 0;
    stack_seed3[stack_top3].x0 = 0;
    return stack_seed3[--stack_top3];
}
int start = 0;
float turn_num[2][ROW / 2];
int16 up_left_flag,up_right_flag, up_left_num,up_right_num;
int search_start = 0;
void search_line(void) {
    for (int i = 0; i < ROW; i++) {

            Img.LeftBorder[i] = 0, Img.RightBorder[i] = COL - 1;

    }

int16 px, py;
search_line_flag = 0;
uint8_t img_tmp[ROW][COL]={0};
size_point center_seed;
stack_top = 0, stack_top1 = 0;
start_line = search_start_line;
int search_len = 20;
int turn_flag = 0;
seed_left_x = 0, seed_left_y = 0;
seed_right_x = 0, seed_right_y = 0;
init_connect();
bool down_flag = 0;
int search_start_sum = 0, search_start_sum_cnt = 0;
int find = 0;

for (int i = 0; i < COL; i++) {
    if (ImageUsed[ROW - 1-find][i] != 0) {
        search_start_sum += i;
        search_start_sum_cnt++;
    }
}

search_start = search_start_sum / search_start_sum_cnt;
/*************************找种子**************************/
   while (start_line >= 0 //行仲未负
          && (stack_top == 0 || stack_top1 == 0 //左右点仲未揾到
              || (seed_right_x - seed_left_x <= 10)) //或者两点太贴近，继续揾种子
          && (start_line < ROW - 1)) {//行唔得超过ROW
       stack_top = 0, stack_top1 = 0;
       seed_right_x = 0, seed_left_x = 0;
       for (int i =
               (search_start + search_len) < COL - 3 ?
               (search_start + search_len) : COL - 3; i >= 0; i--) { //揾左点
           if ((ImageUsed[ROW - 1 - start_line][i]
                != ImageUsed[ROW - 1 - start_line][i + 1]
                && ImageUsed[ROW - 1 - start_line][i] == 0) //条件1:黑(i)+白(i+1)
               || (i == 0 && ImageUsed[ROW - 1 - start_line][i] != 0)//条件2:左边界，且系白
                   ) {
               if (ImageUsed[ROW - 1 - start_line][i] != 0 && i == 0) //条件2得行
               {
                   if (pull_stack(i, start_line, img_tmp))
                       return;
                   seed_left_x = i;
                   seed_left_y = start_line;
                   break;
               }
               if (ImageUsed[ROW - 1 - start_line][i + 1]
                   == ImageUsed[ROW - 1 - start_line][i + 2] //条件3:一黑+二白(i+1,i+2)系左边界。
                       ) {
                   if (pull_stack(i + 1, start_line, img_tmp))
                       return;
                   seed_left_x = i + 1;
                   seed_left_y = start_line;
                   break;
               }
           }
       }
       for (int i =
               search_start - search_len >= 2 ? search_start - search_len : 2;
            i <= COL - 1; i++) {    //揾右点
           if ((ImageUsed[ROW - 1 - start_line][i]
                != ImageUsed[ROW - 1 - start_line][i - 1]
                && ImageUsed[ROW - 1 - start_line][i] == 0)   //条件1:白(i-1)+黑(i)
               || (i == COL - 1 && ImageUsed[ROW - 1 - start_line][i] != 0) //条件2:右边界，且系白
                   ) {
               if (ImageUsed[ROW - 1 - start_line][i] != 0
                   && i == COL - 1) {       //条件2得行
                   if (pull_stack1(i, start_line, img_tmp))
                       return;
                   seed_right_x = i;
                   seed_right_y = start_line;
                   break;
               } else if (ImageUsed[ROW - 1 - start_line][i - 1]
                          == ImageUsed[ROW - 1 - start_line][i - 2] //条件3:二白(i-1,i-2)+一黑系右边界。
                       ) {
                   if (pull_stack1(i - 1, start_line, img_tmp))
                       return;
                   seed_right_x = i - 1;
                   seed_right_y = start_line;
                   break;
               }
           }
       }
       if (seed_right_x - seed_left_x <= 10 && turn_flag == 0) { //，佢哋唔应靠太近，可能系车房 ，揾到异常啲，开始向上头揾点
           turn_flag = 1;
           door_flag.find_line = 1;
       }
       if (turn_flag == 0) start_line--; //仲未揾到异常啲，向下揾种子
       else if (turn_flag == 1) {  //已经揾到异常啲，向上揾种子
           if (seed_left_x != 0 && seed_right_x != 0) { //都揾到，但唔得，从佢哋上一行向上揾
               start_line = start_line + 1, turn_flag++;
           } else { //下面都揾唔到，死咗~
               start_line = -120;
               break;
           }
       } else start_line++; //继续向上揾
   }

   bool cam_protect=0;
//for(int i=COL/2-RoadWide0;i<COL/2+RoadWide0;i++)
//if(ImageUsed[ROW-1][i]!=0)cam_protect=0;
//   if (stack_top == 0 || stack_top1 == 0 || start_line < 0 //摄像头保护
//   || search_line_flag == 1 || start_line > ROW - 1||cam_protect) {
//ExSpeed = 0;
//if (stop_flag == 0) {
//ips114_clear(PINK);
//}
//stop_flag=1;
//   }
max_py = 0, max_py1 = 0;
min_py = 99, min_py1 = 99;
up_left_flag = 0, up_right_flag = 0;
up_left_num = 0, up_right_num = 0;


if (line_debug) {
    ips114_clear(YELLOW);
}



while ((stack_top + stack_top1 != 0 && ((up_left_num != up_right_num) || !(max_py == max_py1 && max_py == 99))) ||
        (down_flag && stack_top + stack_top1 != 0 && !(min_py == min_py1 && min_py == 0))) {
    while (stack_top != 0 && (((max_py <= max_py1 && !up_left_flag )|| stack_top1 == 0) ||
                    (up_left_flag && up_left_num <= up_right_num) ||
                    (down_flag && stack_top != 0 && (stack_top1 == 0 || min_py >= min_py1)))) {
        center_seed = push_stack();
        for (int i = 0; i < 8; i++) {
            px = center_seed.x0 + connects[i].x0;
            py = center_seed.y0 + connects[i].y0;
            if (!(px < 0 || py < 0 || px >= COL || py >= ROW)) {
                if (img_tmp[py][px] == 0) {
                    if (ImageUsed[ROW - 1 - py][px] != ImageUsed[ROW - 1 - center_seed.y0][center_seed.x0] ||
                            (((px == 0 || (py == 0 && i != 6)) && ImageUsed[ROW - 1 - py][px] != 0 )||
                                    ( py == ROW - 1 && ImageUsed[ROW - 1 - py][px] != 0))) {
                        if (pull_stack(px, py, img_tmp)) return;
                        if (line_debug) {
                            ips114_draw_point(px, ROW - 1 - py, RED);
                            my_delay(10);
                        }
                        if (py == ROW - 1 && ImageUsed[ROW - 1 - py][px] != 0) up_left_flag = 1;
                        if (up_left_flag) up_left_num++;
//                        while (px > 0 && ImageUsed[ROW - 1 - py][px - 1] != 0) px--;
//                        while (px < COL - 1 && ImageUsed[ROW - 1 - py][px] == 0) px++;
                 if (Img.LeftBorder[py] < px) Img.LeftBorder[py] = px;
                        if (py > max_py) max_py = py;
                        if (py < min_py) min_py = py;
                    }
                }
            }
        }
    }
    while (stack_top1 != 0 && (((max_py >= max_py1 && !up_right_flag )|| stack_top == 0) ||
                    (up_right_flag && up_right_num <= up_left_num))) {
        center_seed = push_stack1();
        for (int i = 0; i < 8; i++) {
            px = center_seed.x0 + connects[i].x0;
            py = center_seed.y0 + connects[i].y0;
            if (!(px < 0 || py < 0 || px >= COL || py >= ROW)) {
                if (img_tmp[py][px] == 0) {
                    if (ImageUsed[ROW - 1 - py][px] != ImageUsed[ROW - 1 - center_seed.y0][center_seed.x0] ||
                            (((px == COL - 1 || (py == 0 && i != 2)) && ImageUsed[ROW - 1 - py][px] != 0 )||
                                    ( py == ROW - 1 && ImageUsed[ROW - 1 - py][px] != 0))) {
                        if (pull_stack1(px, py, img_tmp)) return;
                        if (line_debug) {
                            ips114_draw_point(px, ROW - 1 - py, BLUE);
                            my_delay(10);
                        }
                        if (py == ROW - 1 && ImageUsed[ROW - 1 - py][px] != 0) up_right_flag = 1;
                        if (up_right_flag)up_right_num++;
//                        while (px < COL - 1 && ImageUsed[ROW - 1 - py][px + 1] != 0) px++;
//                        while (px > 0 && ImageUsed[ROW - 1 - py][px] == 0) px--;
                 if (Img.RightBorder[py] > px) Img.RightBorder[py] = px;
                        if (py > max_py1) max_py1 = py;
                        if (py < min_py1) min_py1 = py;
                    }
                }
            }
        }
    }
    if (!(stack_top + stack_top1 != 0 &&
                    ((up_left_num != up_right_num) || !(max_py == max_py1 && max_py == 99))))
    down_flag = 1;
}
left_lost_cnt = 0, right_lost_cnt = 0;
for (int i = 0; i < ROW; i++) {
    if (Img.LeftBorder[i] <= 0 + lose_error
            || Img.LeftBorder[i] >= COL - 1 - lose_error)
    Img.LeftBorderFindFlag[i] = 'F', left_lost_cnt++;
    else Img.LeftBorderFindFlag[i] = 'T';
    if (Img.RightBorder[i] <= 0 + lose_error
            || Img.RightBorder[i] >= COL - 1 - lose_error)
    Img.RightBorderFindFlag[i] = 'F', right_lost_cnt++;
    else Img.RightBorderFindFlag[i] = 'T';
}

int k_num = 3;
float k_excel[COL];
for (int i = 0; i < COL; i++) k_excel[i] = 1.0 * i / 10;
for (int i = 0; i < ROW; i++) {
    if (i < 0 + k_num || i >= ROW - k_num) Img.RoadWide[i] = RoadWide0;
    else {
        if (Img.RightBorderFindFlag[i] == 'T' && Img.LeftBorderFindFlag[i] == 'T') Img.RoadWide[i] = RoadWide0;
        else if (Img.RightBorderFindFlag[i] == 'T' && Img.RightBorderFindFlag[i + k_num] == 'T' &&
                Img.RightBorderFindFlag[i - k_num] == 'T' && Img.LeftBorderFindFlag[i] == 'F')
        Img.RoadWide[i] =
        RoadWide0 * (1 + k_excel[abs(Img.RightBorder[i + k_num] - Img.RightBorder[i - k_num])]);
        else if (Img.RightBorderFindFlag[i] == 'F' && Img.LeftBorderFindFlag[i] == 'T' &&
                Img.LeftBorderFindFlag[i - k_num] == 'T' && Img.LeftBorderFindFlag[i + k_num] == 'T')
        Img.RoadWide[i] = RoadWide0 * (1 + k_excel[abs(Img.LeftBorder[i + k_num] - Img.LeftBorder[i - k_num])]);
        else Img.RoadWide[i] = RoadWide0;
    }
}
edge_cnt( img_tmp);
GetBorder( img_tmp);
solve_line(img_tmp);
}



float line_wight[ROW] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       //1~10
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,        //11~20
    10, 10, 10, 10, 10,
    //21~30
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10,   //41~50
    10, 10, 10, 10, 10,         //31~40
    1,1,1, 1, 1,  1, 1, 1,    1, 1, 1, 1, 1,

    1, 1, 1, 1, 1, 1, 1,      //51~60

    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,        //61~70
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,        //71~80
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,        //81~90
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,        //91~100
    };




bool img_flag=0;

void cal_middle_line(void) {
//计算当前行的偏移量
//    float excursion[ROW];
    float sumcenter = 0;

    float cnt = 0;
    for (Ysite = CenterCalMinRow; Ysite < CenterCalMaxRow; Ysite++) {
        if (Ysite == stack_top || Ysite == stack_top1)
            continue;
        if (Img.LeftBorderFindFlag[Ysite] == 'T'
                || Img.RightBorderFindFlag[Ysite] == 'T') {

            sumcenter += (float) (Img.Center[Ysite] - (COL / 2))
                                    / (float) (RoadWide0 / 2) * 100*line_wight[Ysite];
                            cnt+=line_wight[Ysite];

        }

    }
    point_center0 = (int32) (sumcenter / (cnt) * 2.0f);
    point_center = point_center0 * 0.6f + point_center1 * 0.3f
            + point_center2 * 0.1f;
    point_center2 = point_center1;
    point_center1 = point_center0;
img_flag=1;


}
bool error_find = 0;
int view_tmp = 0;
void solve_line(uint8_t img_tmp[][COL]) {
float kk = 1.2;
// int CenterCalMaxRow = CenterCalMaxRow_tmp; //15
error_find = 0;
if (circle_flag.find_left_circle == 0 && circle_flag.find_right_circle == 0
        && fork_flag.find_fork == 0 && Cross.FindFlag == 'F'
        && door_flag.find_door == 0) {



    uint8 SlopeCnt = 0,SlopeCnt1=0;
                      for(Ysite = 0; Ysite <  ROW; Ysite++)
                                {
                                    if(myabs(Img.RightSlope[Ysite]) <= 0.4f&&Img.RoadWide[Ysite]<RoadWide0*1.1)
                                        SlopeCnt++;
                                    if(myabs(Img.LeftSlope[Ysite]) <= 0.4f&&Img.RoadWide[Ysite]<RoadWide0*1.1)
                                    SlopeCnt1++;
                                }
                      if(SlopeCnt > ROW-RoadWide0/3||SlopeCnt1>ROW-RoadWide0/3)         ExSpeed_SUM = 390;
                      else   ExSpeed_SUM=ExSpeed_SUM_tmp ;

if(SlopeCnt > ROW-RoadWide0/3)
{
    for(int i=0;i<ROW;i++)
    {
        Img.Center[i]=Img.RightBorder[i]-RoadWide0/2;
    }
    return;
}else if(SlopeCnt1>ROW-RoadWide0/3)
{
    for(int i=0;i<ROW;i++)
       {
           Img.Center[i]=Img.LeftBorder[i]+RoadWide0/2;
       }
    return;

}
    for (int Ysite = StartScanRow; Ysite < ROW; Ysite++) {
        if (Img.RightBorder[Ysite] < Img.LeftBorder[Ysite]) {
            Img.LeftBorderFindFlag[Ysite] = 'F';
            Img.RightBorderFindFlag[Ysite] = 'F';
            if (error_find == 0) {
                CenterCalMaxRow = Ysite;
                CenterCalMaxRow = Ysite;
                error_find = 1;
            }
        }
        if (((Img.RightBorderFindFlag[Ysite] == 'T'
                && Img.LeftBorderFindFlag[Ysite] == 'F')
                || (Img.RightBorderFindFlag[Ysite] == 'F'
                        && Img.LeftBorderFindFlag[Ysite] == 'T'))
                && (Img.RightBorder[Ysite] - Img.LeftBorder[Ysite]
                        >= RoadWide0 * 2)
                && (Img.LeftBorder[Ysite] < lose_error
                        && Img.RightBorder[Ysite] > COL - 1 - lose_error)) {
            if (error_find == 0) {
                CenterCalMaxRow = Ysite;
                CenterCalMaxRow = Ysite;
                error_find = 1;
            }
            if (Img.LeftBorderFindFlag[Ysite] == 'T')
                left_lost_cnt++;
            if (Img.RightBorderFindFlag[Ysite] == 'T')
                right_lost_cnt++;
            Img.LeftBorderFindFlag[Ysite] = 'F';
            Img.RightBorderFindFlag[Ysite] = 'F';

        }
  /**************防止两线夹着黑*******************/
            if(Img.LeftBorderFindFlag[Ysite]=='T'&&Img.RightBorderFindFlag[Ysite]=='T'){
            int white_black=0;
            for(int j=Img.LeftBorder[Ysite];j<=Img.RightBorder[Ysite];j++) if(ImageUsed[ROW-1-Ysite][j]==0) white_black++;
            if(white_black>=(Img.RightBorder[Ysite]-Img.LeftBorder[Ysite])*0.6)
            {
                              left_lost_cnt++;
                              right_lost_cnt++;
                          Img.LeftBorderFindFlag[Ysite] = 'F';
                          Img.RightBorderFindFlag[Ysite] = 'F';
            }
            }
            /*****************************************/
        if (Ysite < StartScanRow + 1) {
            if (Img.RightBorderFindFlag[Ysite] == 'T'
                    && Img.LeftBorderFindFlag[Ysite] == 'T') //无丢线
                Img.Center[Ysite] = (Img.LeftBorder[Ysite]
                        + Img.RightBorder[Ysite]) / 2;
            else if (Img.RightBorderFindFlag[Ysite] == 'F'
                    && Img.LeftBorderFindFlag[Ysite] == 'F') //两边都丢
                Img.Center[Ysite] = CenterCol;
            else if (Img.RightBorderFindFlag[Ysite] == 'F'
                    && Img.LeftBorderFindFlag[Ysite] == 'T') //丢了右边线
                Img.Center[Ysite] = Img.LeftBorder[Ysite]
                        + Img.RoadWide[Ysite] / 2
                        + (search_start - COL / 2)
                                * (0
                                        + kk
                                                * (1.0
                                                        - 1.0
                                                                * max(max_py,
                                                                        max_py1)
                                                                / ROW));
            else if (Img.RightBorderFindFlag[Ysite] == 'T'
                    && Img.LeftBorderFindFlag[Ysite] == 'F')  //丢了左边
                Img.Center[Ysite] = Img.RightBorder[Ysite]
                        - Img.RoadWide[Ysite] / 2
                        + (search_start - COL / 2)
                                * (0
                                        + kk
                                                * (1.0
                                                        - 1.0
                                                                * max(max_py,
                                                                        max_py1)
                                                                / ROW));
        } else {

            if (Img.RightBorderFindFlag[Ysite] == 'T'
                    && Img.LeftBorderFindFlag[Ysite] == 'T') //无丢线
                Img.Center[Ysite] = (Img.LeftBorder[Ysite]
                        + Img.RightBorder[Ysite]) / 2 * 0.9
                        + 0.1 * Img.Center[Ysite - 1];
            else if (Img.RightBorderFindFlag[Ysite] == 'F'
                    && Img.LeftBorderFindFlag[Ysite] == 'F') //两边都丢
                Img.Center[Ysite] = CenterCol;
            else if (Img.RightBorderFindFlag[Ysite] == 'F'
                    && Img.LeftBorderFindFlag[Ysite] == 'T') //丢了右边线
                Img.Center[Ysite] = (Img.LeftBorder[Ysite]
                        + Img.RoadWide[Ysite] / 2) * 0.9
                        + 0.1 * Img.Center[Ysite - 1]
                        + (search_start - COL / 2)
                                * (0
                                        + kk
                                                * (1.0
                                                        - 1.0
                                                                * max(max_py,
                                                                        max_py1)
                                                                / ROW));
            else if (Img.RightBorderFindFlag[Ysite] == 'T'
                    && Img.LeftBorderFindFlag[Ysite] == 'F')  //丢了左边
                Img.Center[Ysite] = (Img.RightBorder[Ysite]
                        - Img.RoadWide[Ysite] / 2) * 0.9
                        + 0.1 * Img.Center[Ysite - 1]
                        + (search_start - COL / 2)
                                * (0
                                        + kk
                                                * (1.0
                                                        - 1.0
                                                                * max(max_py,
                                                                        max_py1)
                                                                / ROW));

        }

    }
    return;
}
/*****************环岛************************/
if (circle_flag.find_left_circle == 1) {
    if(       circle_flag.enter_left_circle ==0){
        int search_end=ROW;
        for(int i=0;i<ROW-3;i++)
        {
            if(ImageUsed[ROW-1-i][lose_error]!=0&&ImageUsed[ROW-1-i-1][lose_error]!=0&&
                                          ImageUsed[ROW-1-i-2][lose_error]==0&&ImageUsed[ROW-1-i-1-3][lose_error]==0 &&img_tmp[i+1][lose_error]    )
            {
                search_end=i+1;
                break;
            }
        }

        connect_line(Img.LeftBorder[0]+RoadWide0/2,0 , 0, search_end, 'R');
        for(int i=0;i<ROW;i++)
        {
            if(i<search_end)
            {
                Img.LeftBorderFindFlag[i]='T';
                Img.RightBorderFindFlag[i]='T';
            Img.Center[i]=Img.RightBorder[i];
            }
            else {
                Img.LeftBorderFindFlag[i]='F';
                     Img.RightBorderFindFlag[i]='F';
            }
        }
    }
    else {
            if(circle_flag.on_left_circle == 0)    //环内巡线
            {
                for (int Ysite = 0; Ysite < ROW; Ysite++) {




                        if (Img.RightBorderFindFlag[Ysite] == 'T'
                                && Img.LeftBorderFindFlag[Ysite] == 'T') //无丢线
                            Img.Center[Ysite] = (Img.LeftBorder[Ysite]
                                    + Img.RightBorder[Ysite]) / 2 * 0.9
                                    + 0.1 * Img.Center[Ysite - 1];
                        else if (Img.RightBorderFindFlag[Ysite] == 'F'
                                && Img.LeftBorderFindFlag[Ysite] == 'F') //两边都丢
                            Img.Center[Ysite] = CenterCol;
                        else if (Img.RightBorderFindFlag[Ysite] == 'F'
                                && Img.LeftBorderFindFlag[Ysite] == 'T') //丢了右边线
                            Img.Center[Ysite] = (Img.LeftBorder[Ysite]
                                    + Img.RoadWide[Ysite] / 2) * 0.9
                                    + 0.1 * Img.Center[Ysite - 1];

                        else if (Img.RightBorderFindFlag[Ysite] == 'T'
                                && Img.LeftBorderFindFlag[Ysite] == 'F')  //丢了左边
                            Img.Center[Ysite] = (Img.RightBorder[Ysite]
                                    - Img.RoadWide[Ysite] / 2) * 0.9
                                    + 0.1 * Img.Center[Ysite - 1];


                }
            }
            else if(circle_flag.out_left_circle==0) //出环补线
            {
                int search_end=ROW;
                      for(int i=0;i<ROW-3;i++)
                      {
                          if(ImageUsed[ROW-1-i][lose_error]!=0&&ImageUsed[ROW-1-i-1][lose_error]!=0&&
                                                  ImageUsed[ROW-1-i-2][lose_error]==0&&ImageUsed[ROW-1-i-1-3][lose_error]==0 &&img_tmp[i+1][lose_error]    )
                          {
                              search_end=i+1;
                              break;
                          }
                      }

                      connect_line(Img.LeftBorder[0]+RoadWide0/2,0 , 0, search_end, 'R');
                      for(int i=0;i<ROW;i++)
                      {
                          if(i<search_end)
                          {
                              Img.LeftBorderFindFlag[i]='T';
                              Img.RightBorderFindFlag[i]='T';
                          Img.Center[i]=Img.RightBorder[i];
                          }
                          else {
                              Img.LeftBorderFindFlag[i]='F';
                                   Img.RightBorderFindFlag[i]='F';
                          }
                      }
            }
    }
    return;
} else if (circle_flag.find_right_circle == 1) {
    if(circle_flag.enter_right_circle==0){
int search_end=ROW;
for(int i=0;i<ROW-3;i++)
{
    if(ImageUsed[ROW-1-i][COL-1-lose_error]!=0&&ImageUsed[ROW-1-i-1][COL-1-lose_error]!=0&&
                                  ImageUsed[ROW-1-i-2][COL-1-lose_error]==0&&ImageUsed[ROW-1-i-1-3][COL-1-lose_error]==0 &&img_tmp[i+1][COL-1-lose_error]    )
    {
        search_end=i+1;
        break;
    }
}
connect_line(Img.RightBorder[0]-RoadWide0/2,0 , COL-1, search_end, 'L');
for(int i=0;i<ROW;i++)
{
    if(i<search_end)
    {
        Img.LeftBorderFindFlag[i]='T';
        Img.RightBorderFindFlag[i]='T';
    Img.Center[i]=Img.LeftBorder[i];
    }
    else {
        Img.LeftBorderFindFlag[i]='F';
             Img.RightBorderFindFlag[i]='F';
    }
}

    }else {
       if(circle_flag.on_right_circle==0)//环内巡线
       {
           for (int Ysite = 0; Ysite < ROW; Ysite++) {




                   if (Img.RightBorderFindFlag[Ysite] == 'T'
                           && Img.LeftBorderFindFlag[Ysite] == 'T') //无丢线
                       Img.Center[Ysite] = (Img.LeftBorder[Ysite]
                               + Img.RightBorder[Ysite]) / 2 * 0.9
                               + 0.1 * Img.Center[Ysite - 1];
                   else if (Img.RightBorderFindFlag[Ysite] == 'F'
                           && Img.LeftBorderFindFlag[Ysite] == 'F') //两边都丢
                       Img.Center[Ysite] = CenterCol;
                   else if (Img.RightBorderFindFlag[Ysite] == 'F'
                           && Img.LeftBorderFindFlag[Ysite] == 'T') //丢了右边线
                       Img.Center[Ysite] = (Img.LeftBorder[Ysite]
                               + Img.RoadWide[Ysite] / 2) * 0.9
                               + 0.1 * Img.Center[Ysite - 1];

                   else if (Img.RightBorderFindFlag[Ysite] == 'T'
                           && Img.LeftBorderFindFlag[Ysite] == 'F')  //丢了左边
                       Img.Center[Ysite] = (Img.RightBorder[Ysite]
                               - Img.RoadWide[Ysite] / 2) * 0.9
                               + 0.1 * Img.Center[Ysite - 1];


           }

       }else {
        if(circle_flag.out_right_circle==0)//出环补线
        {
            int search_end=ROW;
            for(int i=0;i<ROW-3;i++)
            {
                if(ImageUsed[ROW-1-i][COL-1-lose_error]!=0&&ImageUsed[ROW-1-i-1][COL-1-lose_error]!=0&&
                                        ImageUsed[ROW-1-i-2][COL-1-lose_error]==0&&ImageUsed[ROW-1-i-1-3][COL-1-lose_error]==0 &&img_tmp[i+1][COL-1-lose_error]    )
                {
                    search_end=i+1;
                    break;
                }
            }
            connect_line(Img.RightBorder[0]-RoadWide0/2,0 , COL-1, search_end, 'L');
            for(int i=0;i<ROW;i++)
            {
                if(i<search_end)
                {
                    Img.LeftBorderFindFlag[i]='T';
                    Img.RightBorderFindFlag[i]='T';
                Img.Center[i]=Img.LeftBorder[i];
                }
                else {
                    Img.LeftBorderFindFlag[i]='F';
                         Img.RightBorderFindFlag[i]='F';
                }
            }
        }
    }

    }
    return;
}
/***************************三叉*************************/

if (fork_flag.find_fork == 1) {
    if (fork_flag.on_fork == 0) {
        if (fork_flag.leave_fork == 0) {
            if (fork_flag.cnt % 2 == 0) {
                for (int Ysite = StartScanRow + 1; Ysite < ROW; Ysite++)
                    Img.Center[Ysite] = Img.LeftBorder[Ysite]
                            + RoadWide0 * 0.3;
            } else {
                for (int Ysite = StartScanRow + 1; Ysite < ROW; Ysite++)
                    Img.Center[Ysite] = Img.RightBorder[Ysite]
                            - RoadWide0 * 0.3;
            }
        }
    } else {
        if (fork_flag.out_fork == 0) {
            for (int Ysite = StartScanRow + 1; Ysite < ROW; Ysite++) {
                //此处应对有丢边界的中线进行处理
                if (Img.RightBorderFindFlag[Ysite] == 'T'
                        && Img.LeftBorderFindFlag[Ysite] == 'T') //无丢线
                                {
                    if (fork_flag.cnt % 2 == 0) {
                        Img.Center[Ysite] = (Img.LeftBorder[Ysite] * 0.5
                                + Img.RightBorder[Ysite] * 0.5);
                    } else {
                        Img.Center[Ysite] = (Img.LeftBorder[Ysite] * 0.5
                                + Img.RightBorder[Ysite] * 0.5);
                    }

                } else if (Img.RightBorderFindFlag[Ysite] == 'F'
                        && Img.LeftBorderFindFlag[Ysite] == 'F') //两边都丢
                    Img.Center[Ysite] = CenterCol;
                else if (Img.RightBorderFindFlag[Ysite] == 'F'
                        && Img.LeftBorderFindFlag[Ysite] == 'T') //丢了右边线
                                {
                    Img.Center[Ysite] = Img.LeftBorder[Ysite]
                            + Img.RoadWide[Ysite] * 0.5;
                } else if (Img.RightBorderFindFlag[Ysite] == 'T'
                        && Img.LeftBorderFindFlag[Ysite] == 'F')  //丢了左边
                                {
                    Img.Center[Ysite] = Img.RightBorder[Ysite]
                            - Img.RoadWide[Ysite] * 0.5;
                }
            }
        } else {
            if (fork_flag.leave_fork == 0) {
                if (fork_flag.cnt % 2 == 0) {
                    for (int Ysite = StartScanRow + 1; Ysite < ROW; Ysite++)
                        Img.Center[Ysite] = Img.LeftBorder[Ysite]
                                + RoadWide0 * 0.3;
                } else {
                    for (int Ysite = StartScanRow + 1; Ysite < ROW; Ysite++)
                        Img.Center[Ysite] = Img.RightBorder[Ysite]
                                - RoadWide0 * 0.3;
                }
            }
        }
    }
}

if (door_flag.find_door == 1) {
    for (int Ysite = StartScanRow + 1; Ysite < ROW; Ysite++) {
        Img.Center[Ysite] = Img.LeftBorder[Ysite] + RoadWide0 / 2;
    }
    return;
}

}


int start_COL=0,start_ROW=0,end_COL=COL-1,end_ROW=ROW-1;
/**************连通域*****************/
Connected_Component connect;
void init_connect() {
    memset(connect.edge, 0, sizeof(connect.edge));
    connect.room = 0;
    connect.edge_len = 3;
}
bool add_edge(int num, uint8_t img_tmp[ROW][COL]) {
    for (int j = 0; j < 2 * connect.edge_len ; j++) {
        int num_tmp = num + j;
        int i = (num_tmp + (end_COL-start_COL+1+end_ROW-start_ROW+1) * 2 - 4) % ((end_COL-start_COL+1+end_ROW-start_ROW+1) * 2 - 4);
        if (i < end_COL-start_COL+1) {

            if (img_tmp[start_ROW][start_COL+i] == 1)
                return 1;
        } else if (i < end_COL-start_COL+1+ end_ROW-start_ROW +1-1) {

            if (img_tmp[start_ROW+(i - (end_COL-start_COL))][end_COL] == 1)
                return 1;
        } else if (i <  end_COL-start_COL+1 +  end_COL-start_COL+1 + end_ROW-start_ROW +1 - 2) {

            if (img_tmp[end_ROW][(end_COL - i + (end_COL-start_COL+1  + end_ROW-start_ROW +1 - 2))-1] == 1)
                return 1;
        } else {

            if (img_tmp[start_ROW+(end_COL-start_COL+1 + end_COL-start_COL+1 +  end_ROW-start_ROW +1  +  end_ROW-start_ROW +1  - 4 - i)][start_COL+0] == 1)
                return 1;

        }
    }
    return 0;
}

void edge_cnt(uint8_t img_tmp[ROW][COL]) {
    int x = start_COL, y = ROW-1-start_ROW ;
    int edge_cnt = 0;
    int way = 0; //0 r 1 up 2 l 3 down
    for (int i = 0; i < (end_COL-start_COL+1+end_ROW-start_ROW+1) * 2 -4; i++) {
        connect.edge[edge_cnt] = ImageUsed[y][x];
        edge_cnt++;
//        my_delay(1);

        if (way == 0)
            x++;
        else if (way == 1)
            y--;
        else if (way == 2)
            x--;
        else if (way == 3)
            y++;
        if (x > end_COL)
            way++, x =end_COL,y= ROW-1-start_ROW-1;
        if (y < ROW-end_ROW-1)
            way++, x=end_COL-1,y =  ROW-1-end_ROW;
        if (x < start_COL)
            way++, x = start_COL,y=ROW-1-end_ROW+1;
        if (y > ROW-1-start_ROW)
            way++;
    }
    for (int i = 0; i < (end_COL-start_COL+1+end_ROW-start_ROW+1)* 2 - 4; i++) {
        int sum_start = 0, sum_end = 0;
        for (int j = i; j < connect.edge_len * 2 + i; j++) {
            if (j < (end_COL-start_COL+1+end_ROW-start_ROW+1) * 2 - 4) {
                if (j < i + connect.edge_len)
                    sum_start += connect.edge[j];
                else
                    sum_end += connect.edge[j];
            } else {
                if (j < i + connect.edge_len)
                    sum_start += connect.edge[j - ((end_COL-start_COL+1+end_ROW-start_ROW+1) * 2 - 4)];
                else
                    sum_end += connect.edge[j - ((end_COL-start_COL+1+end_ROW-start_ROW+1)* 2 - 4)];
            }
        }
        if (abs(sum_end - sum_start) == 255 * connect.edge_len
            && add_edge(i, img_tmp))
            connect.room++;
    }
    room_cnt();
}
void room_cnt() {
    if (connect.room == connect.last_room)
        connect.room_cnt++;
    else {
        connect.error_cnt++;
        if (connect.error_cnt >= connect.error_max) {
            connect.room_cnt = 1;
            connect.error_cnt = 0;
            connect.last_room = connect.room;
        }
    }
}
int clip(int x, int low, int up) {
    return x > up ? up : x < low ? low : x;
}
int16 up_left_flag, up_right_flag, up_left_num, up_right_num;
int go[4][8][2] = {
        //     左,左前,前,右前,右,右后,后,左后                //方位/朝向
        {{-1, 0},  {-1, -1}, {0,  -1}, {1,  -1}, {1,  0},  {1,  1},  {0,  1},  {-1, 1}},//左
        {{0,  -1}, {1,  -1}, {1,  0},  {1,  1},  {0,  1},  {-1, 1},  {-1, 0},  {-1, -1}},//上
        {{1,  0},  {1,  1},  {0,  1},  {-1, 1},  {-1, 0},  {-1, -1}, {0,  -1}, {1,  -1}},//右
        {{0,  1},  {-1, 1},  {-1, 0},  {-1, -1}, {0,  -1}, {1,  -1}, {1,  0},  {1,  1}},//下
};
bool blocked(int head, int dir, int center_x, int center_y) {
    center_x = go[head][dir][1] + center_x;
    center_y = go[head][dir][0] + center_y;
    if (center_x < 0 || center_x >= COL)return 1;
    if (center_y < 0 || center_y >= ROW)return 1;
    if (ImageUsed[ROW - 1 - center_y][center_x] == 0)return 1;
    return 0;
}
bool judge_black(int y, int x) {
    if (ImageUsed[ROW - 1 - y][x] == 0)
        return 0;
//
//int sum=0;
//int cnt=0;
//for(int i=y-1;i<=y+1;i++)
//{
//    if(i<ROW&&i>=0)
//
//        for(int j=x-1;j<=x+1;j++)
//    {
//            if(j>=0&&j<COL)
//            {
//                sum+=ImageUsed[ROW-1-i][j];
//                cnt++;
//            }
//    }
//}
//    if( sum/cnt<100)return 0;



    return 1;
}
void blur_points(int16 pts_in[][2], int num, float pts_out[][2], int kernel) {
    int half = kernel / 2; //3
    for (int i = 0; i < num; i++) {
        pts_out[i][0] = pts_out[i][1] = 0;
        for (int j = -half; j <= half; j++) { //-3~~3 (-2~4)-(0~3)
            pts_out[i][0] += 1.0 * pts_in[clip(i + j, 0, num - 1)][0] * (half + 1 - abs(j));
            pts_out[i][1] += 1.0 * pts_in[clip(i + j, 0, num - 1)][1] * (half + 1 - abs(j));
        }
        pts_out[i][0] /= (2 * half + 2) * (half + 1) / 2;
        pts_out[i][1] /= (2 * half + 2) * (half + 1) / 2;
    }
}
bool check_line(uint8_t img_tmp[ROW][COL], int x, int y) {
    if (x == COL - 1 && y == 0) {
        int a = 0;
    }
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (y + i < ROW && y + i >= 0 && x + j < COL && x + j >= 0)
                if (img_tmp[y + i][x + j])
                    return 1;
    return 0;
}
//clip
void search_line_fork(void) {
    Img.right_line_cnt = 0;
    Img.left_line_cnt = 0;
    Img.left_cnt_itcm = 0;
    Img.right_cnt_itcm = 0;
    Img.left_edge_cnt = 0;
    Img.right_edge_cnt = 0;
    Img.left_cnt_deal = 0;
    Img.right_cnt_deal = 0;


    /**********************************************************/
    for (int i = 0; i < ROW; i++)
        Img.LeftBorder[i] = 0, Img.RightBorder[i] = COL - 1;
    int16 px, py;
    search_line_flag = 0;
    uint8_t img_tmp[ROW][COL] = {0};
    size_point center_seed;
    stack_top = 0, stack_top1 = 0;
    start_line = search_start_line;
    int search_len = 20;
    int turn_flag = 0;
    seed_left_x = 0, seed_left_y = 0;
    seed_right_x = 0, seed_right_y = 0;
    init_connect();
    bool down_flag = 0;
    int search_start_sum = 0, search_start_sum_cnt = 0;
    int find = 0;
    for (int i = 0; i < COL; i++) {
        if (ImageUsed[ROW - 1 - find][i] != 0) {
            search_start_sum += i;
            search_start_sum_cnt++;
        }
    }
    search_start = search_start_sum / search_start_sum_cnt;
    /*************************找种子**************************/
    while (start_line >= 0 //行仲未负
           && (stack_top == 0 || stack_top1 == 0 //左右点仲未揾到
               || (seed_right_x - seed_left_x <= 10)) //或者两点太贴近，继续揾种子
           && (start_line < ROW - 1)) {//行唔得超过ROW
        stack_top = 0, stack_top1 = 0;
        seed_right_x = 0, seed_left_x = 0;
        for (int i =
                (search_start + search_len) < COL - 3 ?
                (search_start + search_len) : COL - 3; i >= 0; i--) { //揾左点
            if ((ImageUsed[ROW - 1 - start_line][i]
                 != ImageUsed[ROW - 1 - start_line][i + 1]
                 && ImageUsed[ROW - 1 - start_line][i] == 0) //条件1:黑(i)+白(i+1)
                || (i == 0 && ImageUsed[ROW - 1 - start_line][i] != 0)//条件2:左边界，且系白
                    ) {
                if (ImageUsed[ROW - 1 - start_line][i] != 0 && i == 0) //条件2得行
                {
                    if (pull_stack(i, start_line, img_tmp))
                        return;
                    seed_left_x = i;
                    seed_left_y = start_line;
                    break;
                }
                if (ImageUsed[ROW - 1 - start_line][i + 1]
                    == ImageUsed[ROW - 1 - start_line][i + 2] //条件3:一黑+二白(i+1,i+2)系左边界。
                        ) {
                    if (pull_stack(i + 1, start_line, img_tmp))
                        return;
                    seed_left_x = i + 1;
                    seed_left_y = start_line;
                    break;
                }
            }
        }
        for (int i =
                search_start - search_len >= 2 ? search_start - search_len : 2;
             i <= COL - 1; i++) {    //揾右点
            if ((ImageUsed[ROW - 1 - start_line][i]
                 != ImageUsed[ROW - 1 - start_line][i - 1]
                 && ImageUsed[ROW - 1 - start_line][i] == 0)   //条件1:白(i-1)+黑(i)
                || (i == COL - 1 && ImageUsed[ROW - 1 - start_line][i] != 0) //条件2:右边界，且系白
                    ) {
                if (ImageUsed[ROW - 1 - start_line][i] != 0
                    && i == COL - 1) {       //条件2得行
                    if (pull_stack1(i, start_line, img_tmp))
                        return;
                    seed_right_x = i;
                    seed_right_y = start_line;
                    break;
                } else if (ImageUsed[ROW - 1 - start_line][i - 1]
                           == ImageUsed[ROW - 1 - start_line][i - 2] //条件3:二白(i-1,i-2)+一黑系右边界。
                        ) {
                    if (pull_stack1(i - 1, start_line, img_tmp))
                        return;
                    seed_right_x = i - 1;
                    seed_right_y = start_line;
                    break;
                }
            }
        }
        if (seed_right_x - seed_left_x <= 10 && turn_flag == 0) { //，佢哋唔应靠太近，可能系车房 ，揾到异常啲，开始向上头揾点
            turn_flag = 1;
            door_flag.find_line = 1;
        }
        if (turn_flag == 0) start_line--; //仲未揾到异常啲，向下揾种子
        else if (turn_flag == 1) {  //已经揾到异常啲，向上揾种子
            if (seed_left_x != 0 && seed_right_x != 0) { //都揾到，但唔得，从佢哋上一行向上揾
                start_line = start_line + 1, turn_flag++;
            } else { //下面都揾唔到，死咗~
                start_line = -120;
                break;
            }
        } else start_line++; //继续向上揾
    }

//     if (stack_top == 0 || stack_top1 == 0 || start_line < 0 //摄像头保护
//         || search_line_flag == 1 || start_line > ROW - 1) {
//         ExSpeed = 0;
//         if (stop_flag == 0) {
//             ips114_clear(GREEN);
//         }
//         stop_flag = 1;
//         gpio_low(BEEP_PIN);
//         ips114_show_string(1, 1, "speed die!");
//         return;
//     }
    up_left_flag = 0, up_right_flag = 0;
    up_left_num = 0, up_right_num = 0;
    int head_left = 0, head_right = 2;
    int left_center_x = seed_left_x, left_center_y = seed_left_y;
    int right_center_x = seed_right_x, right_center_y = seed_right_y;
    Img.LeftBorder[left_center_y] = left_center_x;
    Img.RightBorder[right_center_y] = right_center_x;
    max_py = left_center_y, max_py1 = right_center_y;
    min_py = left_center_y, min_py1 = right_center_y;
    pull_stack2(left_center_x, left_center_y);
    pull_stack3(right_center_x, right_center_y);
    int min_left_x, min_left_y;
    if (line_debug) {
        ips114_draw_point(left_center_x, ROW - 1 - left_center_y, 3);
        ips114_draw_point(right_center_x, ROW - 1 - right_center_y, 1);
        my_delay(10);
    }
    while ((left_center_x != right_center_x || right_center_y != left_center_y)) {
        //找左线
        if (((max_py <= max_py1 && !up_left_flag) || (up_left_flag && up_left_num <= up_right_num)) && min_py1 != 0 &&
            down_flag
            ||
            ((max_py <= max_py1 && !up_left_flag) || (up_left_flag && up_left_num <= up_right_num)) && down_flag == 0
            || down_flag == 1 && min_py1 != 0) {
            while (blocked(head_left, 2, left_center_x, left_center_y)) head_left = (head_left + 5) % 4;
            if (!blocked(head_left, 1, left_center_x, left_center_y)) {
                left_center_x += go[head_left][1][1];
                left_center_y += go[head_left][1][0];
                if (left_center_x == seed_left_x && left_center_y == seed_left_y) return;
                if (left_center_x == right_center_x && right_center_y == left_center_y && !down_flag) {
                    down_flag = 1;
                    head_left = 2, head_right = 0;
                    left_center_x = seed_right_x, left_center_y = seed_right_y;
                    right_center_x = seed_left_x, right_center_y = seed_left_y;
                    continue;
                }
                head_left = (head_left + 3) % 4;
                if (up_left_flag == 1 && !down_flag) up_left_num++;
            } else if (blocked(head_left, 1, left_center_x, left_center_y)) {
                left_center_x += go[head_left][2][1];
                left_center_y += go[head_left][2][0];
                if (left_center_x == seed_left_x && left_center_y == seed_left_y) return;
                if (left_center_x == right_center_x && right_center_y == left_center_y && !down_flag) {
                    down_flag = 1;
                    head_left = 2, head_right = 0;
                    left_center_x = seed_right_x, left_center_y = seed_right_y;
                    right_center_x = seed_left_x, right_center_y = seed_left_y;
                    continue;
                }
                if (up_left_flag == 1 && !down_flag) up_left_num++;
            }
            if (max_py < left_center_y) {
                max_py = left_center_y;
                if (max_py == ROW - 1) up_left_flag = 1;
            }
            if (min_py1 > left_center_y && down_flag) {
                min_py1 = left_center_y;
                if (min_py == min_py1 && min_py == 0) {
                    img_tmp[left_center_y][left_center_x] = 1;
                    break;
                }
            }
            img_tmp[left_center_y][left_center_x] = 1;
            if (!down_flag) {
                Img.Left_line[Img.left_line_cnt][0] = left_center_x;
                Img.Left_line[Img.left_line_cnt][1] = left_center_y;
                Img.left_line_cnt++;
                if (!up_left_flag) Img.left_edge_cnt = Img.left_line_cnt;
                if (line_debug) {
                    ips114_draw_point(left_center_x, ROW - 1 - left_center_y, 3);
                    my_delay(10);
                }
            } else {
                pull_stack3(left_center_x, left_center_y);
                if (line_debug) {
                    ips114_draw_point(left_center_x, ROW - 1 - left_center_y, 1);
                    my_delay(10);
                }
            }
        }



        //找右线
        if (((max_py1 <= max_py && !up_right_flag) || (up_right_flag && up_left_num >= up_right_num)) && min_py != 0 ||
            ((max_py1 <= max_py && !up_right_flag) || (up_right_flag && up_left_num >= up_right_num)) && !down_flag
            || down_flag == 1 && min_py != 0) {
            while (blocked(head_right, 2, right_center_x, right_center_y))
                head_right = (head_right + 3) % 4;
            if (!blocked(head_right, 3, right_center_x, right_center_y)) {
                right_center_x += go[head_right][3][1];
                right_center_y += go[head_right][3][0];
                if (right_center_x == seed_right_x && right_center_y == seed_right_y) return;
                if (left_center_x == right_center_x && right_center_y == left_center_y && !down_flag) {
                    down_flag = 1;
                    head_left = 2, head_right = 0;
                    left_center_x = seed_right_x, left_center_y = seed_right_y;
                    right_center_x = seed_left_x, right_center_y = seed_left_y;
                    continue;
                }
                head_right = (head_right + 5) % 4;
                if (up_right_flag == 1 && !down_flag) up_right_num++;
            } else if (blocked(head_right, 3, right_center_x, right_center_y)) {
                right_center_x += go[head_right][2][1];
                right_center_y += go[head_right][2][0];
                if (right_center_x == seed_right_x && right_center_y == seed_right_y) return;
                if (left_center_x == right_center_x && right_center_y == left_center_y && !down_flag) {
                    down_flag = 1;
                    head_left = 2, head_right = 0;
                    left_center_x = seed_right_x, left_center_y = seed_right_y;
                    right_center_x = seed_left_x, right_center_y = seed_left_y;
                    continue;
                }
                if (up_right_flag == 1 && !down_flag) up_right_num++;
            }
            if (max_py1 < right_center_y) {
                max_py1 = right_center_y;
                if (max_py1 == ROW - 1) up_right_flag = 1;
            }
            if (min_py > right_center_y && down_flag) {
                min_py = right_center_y;
                if (min_py == min_py1 && min_py1 == 0) {
                    if (Img.RightBorder[left_center_y] > left_center_x)
                        Img.RightBorder[left_center_y] = left_center_x;
                    if (Img.LeftBorder[right_center_y] < right_center_x)
                        Img.LeftBorder[right_center_y] = right_center_x;
                    min_left_x = right_center_x, min_left_y = right_center_y;
                    img_tmp[right_center_y][right_center_x] = 1;
                    break;
                }
            }
            img_tmp[right_center_y][right_center_x] = 1;
            if (!down_flag) {
                Img.Right_line[Img.right_line_cnt][0] = right_center_x;
                Img.Right_line[Img.right_line_cnt][1] = right_center_y;
                Img.right_line_cnt++;
                if (!up_right_flag) Img.right_edge_cnt = Img.right_line_cnt;
                if (line_debug) {
                    ips114_draw_point(right_center_x,
                                      ROW - 1 - right_center_y, 1);
                    my_delay(10);
                }
            } else {
                pull_stack2(right_center_x, right_center_y);
                if (line_debug) {
                    ips114_draw_point(right_center_x, ROW - 1 - right_center_y, 3);
                    my_delay(10);
                }
            }
        }
    }
    /*************************************修补左右线**************************************/
    pull_stack2(min_left_x, min_left_y);
    int tmp_len = stack_top2;
    Img.left_cnt_deal = tmp_len + Img.left_line_cnt;
    Img.left_edge_cnt += tmp_len;
    if (max_py == ROW - 1) Img.left_edge_cnt++;
    for (int i = 0; i < Img.left_cnt_deal; i++) {
        if (i < tmp_len) {
            size_point center_seed = push_stack2();
            Img.left_deal_line[i][0] = center_seed.x0;
            Img.left_deal_line[i][1] = center_seed.y0;
        } else {
            Img.left_deal_line[i][0] = Img.Left_line[i - tmp_len][0];
            Img.left_deal_line[i][1] = Img.Left_line[i - tmp_len][1];
        }
    }
    tmp_len = stack_top3;
    Img.right_cnt_deal = tmp_len + Img.right_line_cnt;
    Img.right_edge_cnt += tmp_len;
    if (max_py1 == ROW - 1) Img.right_edge_cnt++;
    for (int j = 0; j < Img.right_cnt_deal; j++) {
        if (j < tmp_len) {
            size_point center_seed = push_stack3();
            Img.right_deal_line[j][0] = center_seed.x0;
            Img.right_deal_line[j][1] = center_seed.y0;
        } else {
            Img.right_deal_line[j][0] = Img.Right_line[j - tmp_len][0];
            Img.right_deal_line[j][1] = Img.Right_line[j - tmp_len][1];
        }
    }
    /*************************************对左右线滤波**************************************/
    blur_points(Img.left_deal_line, Img.left_cnt_deal, Img.Left_line_itcm, 7);
    blur_points(Img.right_deal_line, Img.right_cnt_deal, Img.Right_line_itcm, 7);
    Img.left_cnt_itcm = Img.left_line_cnt;
    Img.right_cnt_itcm = Img.right_line_cnt;

    /*********************************计算每行左右边线*************************************/
    bool find_line_left[ROW] = {0}, find_line_right[ROW] = {0};
    for (int i = 0; i <= Img.left_edge_cnt; i++) {
        int x = (int) round(Img.Left_line_itcm[i][0]);
        int y = (int) round(Img.Left_line_itcm[i][1]);
        if (y < 0 || y >= ROW)continue;
        if (x < 0 || x >= COL)continue;
        if (!check_line(img_tmp, x, y)) continue;
        if (Img.LeftBorder[y] >= x) continue;
        Img.LeftBorder[y] = x;
        find_line_left[y] = 1;
    }
    for (int i = 0; i < Img.right_edge_cnt; i++) {
        int x = (int) round(Img.Right_line_itcm[i][0]);
        int y = (int) round(Img.Right_line_itcm[i][1]);
        if (y < 0 || y >= ROW)continue;
        if (x < 0 || x >= COL)continue;
        if (!check_line(img_tmp, x, y)) continue;
        if (Img.RightBorder[y] <= x) continue;
        Img.RightBorder[y] = x;
        find_line_right[y] = 1;
    }
    for (int i = 0; i <= max_ab(max_py, max_py1); i++)//fix bottom left
    {
        if (find_line_left[i])break;
        for (int j = 0; j < Img.left_cnt_deal; j++) {
            int x = Img.left_deal_line[j][0];
            int y = Img.left_deal_line[j][1];
            if (y == i) {
                while (Img.left_deal_line[j][1] == i) {
                    Img.LeftBorder[Img.left_deal_line[j][1]] = Img.left_deal_line[j][0];
                    j++;
                }
                break;
            }
        }
    }
    for (int i = 0; i <= max_ab(max_py, max_py1); i++)//fix bottom right
    {
        if (find_line_right[i])break;
        for (int j = 0; j < Img.right_cnt_deal; j++) {
            int x = Img.right_deal_line[j][0];
            int y = Img.right_deal_line[j][1];
            if (y == i) {
                while (Img.right_deal_line[j][1] == i) {
                    Img.RightBorder[Img.right_deal_line[j][1]] = Img.right_deal_line[j][0];
                    j++;
                }
                break;
            }
        }
    }
    for (int i = max_ab(max_py, max_py1); i >= 0; i--)//fix font left
    {
        if (find_line_left[i])break;
        for (int j = Img.left_cnt_deal; j >= 0; j--) {
            int x = Img.left_deal_line[j][0];
            int y = Img.left_deal_line[j][1];
            if (y == i) {
                while (Img.left_deal_line[j][1] == i) {
                    Img.LeftBorder[Img.left_deal_line[j][1]] = Img.left_deal_line[j][0];
                    j--;
                }
                break;
            }
        }
    }
    for (int i = max_ab(max_py, max_py1); i >= 0; i--)//fix font right
    {
        if (find_line_right[i])break;
        for (int j = Img.right_cnt_deal; j >= 0; j--) {
            int x = Img.right_deal_line[j][0];
            int y = Img.right_deal_line[j][1];
            if (y == i) {
                while (Img.right_deal_line[j][1] == i) {
                    Img.RightBorder[Img.right_deal_line[j][1]] = Img.right_deal_line[j][0];
                    j--;
                }
                break;
            }
        }
    }
    /**********************记录左右丢线*************************/
    left_lost_cnt = 0, right_lost_cnt = 0;
    for (int i = 0; i < ROW; i++) {
        if (Img.LeftBorder[i] <= 0 + lose_error
            || Img.LeftBorder[i] >= COL - 1 - lose_error)
            Img.LeftBorderFindFlag[i] = 'F', left_lost_cnt++;
        else
            Img.LeftBorderFindFlag[i] = 'T';
        if (Img.RightBorder[i] <= 0 + lose_error
            || Img.RightBorder[i] >= COL - 1 - lose_error)
            Img.RightBorderFindFlag[i] = 'F', right_lost_cnt++;
        else
            Img.RightBorderFindFlag[i] = 'T';
    }

/**********************对路宽进行补线****************************/
    int k_num = 3;
    float k_excel[COL];
    for (int i = 0; i < COL; i++) k_excel[i] = 1.0 * i / 10;
    for (int i = 0; i < ROW; i++) {
        if (i < 0 + k_num || i >= ROW - k_num) Img.RoadWide[i] = RoadWide0;
        else {
            if (Img.RightBorderFindFlag[i] == 'T' && Img.LeftBorderFindFlag[i] == 'T') Img.RoadWide[i] = RoadWide0;
            else if (Img.RightBorderFindFlag[i] == 'T' && Img.RightBorderFindFlag[i + k_num] == 'T' &&
                     Img.RightBorderFindFlag[i - k_num] == 'T' && Img.LeftBorderFindFlag[i] == 'F')
                Img.RoadWide[i] =
                        RoadWide0 * (1 + k_excel[abs(Img.RightBorder[i + k_num] - Img.RightBorder[i - k_num])]);
            else if (Img.RightBorderFindFlag[i] == 'F' && Img.LeftBorderFindFlag[i] == 'T' &&
                     Img.LeftBorderFindFlag[i - k_num] == 'T' && Img.LeftBorderFindFlag[i + k_num] == 'T')
                Img.RoadWide[i] = RoadWide0 * (1 + k_excel[abs(Img.LeftBorder[i + k_num] - Img.LeftBorder[i - k_num])]);
            else Img.RoadWide[i] = RoadWide0;
        }
    }
    edge_cnt(img_tmp);
    GetBorder( img_tmp);

      solve_line(img_tmp);
}

//**********************上交*********************/

const int dir_front[4][2] = {{0,  1}, //上
                             {1,  0},  //右
                             {0,  -1},  //下
                             {-1, 0}};  //左
const int dir_frontright[4][2] = {{1,  1},
                                  {1,  -1},
                                  {-1, -1},
                                  {-1, 1}};
const int dir_frontleft[4][2] = {{-1, 1},//左上
                                 {1,  1},//右上
                                 {1,  -1},//右下
                                 {-1, -1}};//左下
image_t deal_img;
void RUBO_IMG_INIT() {
    deal_img.data = *SimBinImage;
    deal_img.width = COL;
    deal_img.height = ROW;
    deal_img.step = ROW;
}

void findline_lefthand_adaptive(image_t *img, int block_size, int clip_value, int x, int y, int pts[][2], int *num) {
//    zf_assert(img && img->data);
//    zf_assert(num && *num >= 0);
//    zf_assert(block_size > 1 && block_size % 2 == 1);
    int half = block_size / 2;
    int step = 0, dir = 0, turn = 0;
    while (step < *
            num && half < x && x
                               < img->width - half - 1 && half < y && y < img->height - half - 1 && turn < 4) {
        int local_thres = 0;
        for (
                int dy = -half;
                dy <=
                half;
                dy++) {
            for (
                    int dx = -half;
                    dx <=
                    half;
                    dx++) {
                local_thres +=
                                AT(deal_img, x + dx, y + dy);
            }
        }
        local_thres /=
                block_size * block_size;
        local_thres -=
                clip_value;
        int current_value = AT(deal_img, x, y);
        int front_value = AT(deal_img, x + dir_front[dir][0], y + dir_front[dir][1]);
        int frontleft_value = AT(deal_img, x + dir_frontleft[dir][0], y + dir_frontleft[dir][1]);
        if (front_value < local_thres) {
            dir = (dir + 1) % 4;
            turn++;
        } else if (frontleft_value < local_thres) {//前不被当，左上被挡直走
            x += dir_front[dir][0];
            y += dir_front[dir][1];
            pts[step][0] =
                    x;
            pts[step][1] =
                    y;
            step++;
//            ips114_draw_point(x, ROW-1-y, RED);
//            my_delay(10);
            turn = 0;
        } else {//左上未被挡，前不被档   直走转左
            x += dir_frontleft[dir][0];
            y += dir_frontleft[dir][1];
            dir = (dir + 3) % 4;
            pts[step][0] =
                    x;
            pts[step][1] =
                    y;
            step++;
//            ips114_draw_point(x, ROW-1-y, RED);
//            my_delay(10);
            turn = 0;
        }
    }
    *num = step;
}
void findline_righthand_adaptive(image_t *img, int block_size, int clip_value, int x, int y, int pts[][2], int *num) {
//    zf_assert(img && img->data);
//    zf_assert(num && *num >= 0);
//    zf_assert(block_size > 1 && block_size % 2 == 1);
    int half = block_size / 2;
    int step = 0, dir = 0, turn = 0;
    while (step < *num && 0 < x && x < img->width - 1 && 0 < y && y < img->height - 1 && turn < 4) {
        int local_thres = 0;
        for (int dy = -half; dy <= half; dy++) {
            for (int dx = -half; dx <= half; dx++) {
                local_thres += AT(deal_img, x + dx, y + dy);
            }
        }
        local_thres /= block_size * block_size;
        local_thres -= clip_value;
        int current_value = AT(deal_img, x, y);
        int front_value = AT(deal_img, x + dir_front[dir][0], y + dir_front[dir][1]);
        int frontright_value = AT(deal_img, x + dir_frontright[dir][0], y + dir_frontright[dir][1]);
        if (front_value < local_thres) {
            dir = (dir + 3) % 4;
            turn++;
        } else if (frontright_value < local_thres) {
            x += dir_front[dir][0];
            y += dir_front[dir][1];
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
//            ips114_draw_point(x, ROW-1-y, BLUE);
//  my_delay(10);

        } else {
            x += dir_frontright[dir][0];
            y += dir_frontright[dir][1];
            dir = (dir + 1) % 4;
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
//            ips114_draw_point(x, ROW-1-y, BLUE);
//  my_delay(10);
        }
    }
    *num = step;
}
int ipts0[ROW][2], ipts0_num = ROW, ipts1[ROW][2], ipts1_num = ROW;
int  AT_IMAGE(image_t img,int x,int y)
{
    if(y<0||y>ROW-1||x<0||x>COL-1)  {      ips114_clear(WHITE);

    ips114_show_int16(0, 0, x);
    ips114_show_int16(0, 1, y);

    }
    else return ImageUsed[(ROW-1-(y))][x];
}

void process_image() {
    int block_size = 7, clip_value = 2;
    int thres = 150;
    int begin_x = 1;
    int begin_y = 7;
    int x1 = deal_img.width / 2 - begin_x, y1 = begin_y;
    int x2 = deal_img.width / 2 + begin_x, y2 = begin_y;
    ipts0_num = sizeof(ipts0) / sizeof(ipts0[0]);
    ipts1_num = sizeof(ipts1) / sizeof(ipts1[0]);

    for (; x1 > 0; x1--) if (AT_IMAGE(deal_img, x1 - 1, y1) < thres) break; //x1下一点黑
    if (AT_IMAGE(deal_img, x1, y1) >= thres&&x1!=0)    //左边线，左黑右白
        findline_lefthand_adaptive(&deal_img, block_size, clip_value, x1, y1, ipts0, &ipts0_num);
    else ipts0_num = 0;
    for (; x2 < deal_img.width - 1; x2++) if (AT_IMAGE(deal_img, x2 + 1, y2) < thres) break;
    if (AT_IMAGE(deal_img, x2, y2) >= thres&&x2!=COL-1)
        findline_righthand_adaptive(&deal_img, block_size, clip_value, x2, y2, ipts1, &ipts1_num);
    else ipts1_num = 0;
//    rpts1_num = ipts1_num;
//
//    // 边线滤波
//    blur_points(rpts0, rpts0_num, rpts0b, (int) round(line_blur_kernel));
//    rpts0b_num = rpts0_num;
//    blur_points(rpts1, rpts1_num, rpts1b, (int) round(line_blur_kernel));
//    rpts1b_num = rpts1_num;
//
//    // 边线等距采样
//    rpts0s_num = sizeof(rpts0s) / sizeof(rpts0s[0]);
//    resample_points(rpts0b, rpts0b_num, rpts0s, &rpts0s_num, sample_dist * pixel_per_meter);
//    rpts1s_num = sizeof(rpts1s) / sizeof(rpts1s[0]);
//    resample_points(rpts1b, rpts1b_num, rpts1s, &rpts1s_num, sample_dist * pixel_per_meter);
//
//    // 边线局部角度变化率
//    local_angle_points(rpts0s, rpts0s_num, rpts0a, (int) round(angle_dist / sample_dist));
//    rpts0a_num = rpts0s_num;
//    local_angle_points(rpts1s, rpts1s_num, rpts1a, (int) round(angle_dist / sample_dist));
//    rpts1a_num = rpts1s_num;
//
//    // 角度变化率非极大抑制
//    nms_angle(rpts0a, rpts0a_num, rpts0an, (int) round(angle_dist / sample_dist) * 2 + 1);
//    rpts0an_num = rpts0a_num;
//    nms_angle(rpts1a, rpts1a_num, rpts1an, (int) round(angle_dist / sample_dist) * 2 + 1);
//    rpts1an_num = rpts1a_num;
//
//    // 左右中线跟踪
//    track_leftline(rpts0s, rpts0s_num, rptsc0, (int) round(angle_dist / sample_dist), pixel_per_meter * ROAD_WIDTH / 2);
//    rptsc0_num = rpts0s_num;
//    track_rightline(rpts1s, rpts1s_num, rptsc1, (int) round(angle_dist / sample_dist), pixel_per_meter * ROAD_WIDTH / 2);
//    rptsc1_num = rpts1s_num;
}
