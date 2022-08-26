/*
 * 智能车上位机  qq群:297809802 qq1849057843
 */
#include<swj.h>

//--------------------------------------------------无线模块-----------------------------------------------//
/*
 * 无线模块 推荐使用逐飞的无线模块（usb to nrf）*购买时建议同时购买ttl，用于配置小车的无线模块
 * 波特率可以最高拉到460800 *对应小车上的程序,小车的无线模块，电脑上的无线模块，上位机波特率一致
 * 无线模块速度比wifi慢 但优点是稳定 对于灰度图传速度不太够用，但有二值化图传优化 适合边图传边示波器
 *
 */
//--------------示波器-------------------//
//name数据标识(通道) dat:数据
//例:int a=0;put_int(0,a);
void put_int(uint8 name, int dat)
{
    uint8 fu = 32;
    if (dat < 0) { dat *= -1; fu = 45; }
    uint i = 1; uint8 j = 1;
    while (dat >= i * 10) { i *= 10; j++; }
    uint8 datc[6] = { 122, name, j, fu, 33 };
    sw_write_buffer(datc, 5);//无线转串口模块 串口发送数据
    for (; j > 0; j--)
    {
        uint8 pdat = dat / i;
        pdat %= 10;
        sw_write_byte(pdat);//用无线串口发送一个字节
        i /= 10;
    }
    sw_write_byte(10);//用无线串口发送一个字节
}


//--------------灰度图传-------------------//
//*image图像地址 width图像宽 height图像高
//例：sendimg(mt9v03x_image_dvp[0], MT9V03X_DVP_W, MT9V03X_DVP_H);
void sendimg(uint8* image, uint16 width, uint16 height)
{
    uint8 dat[6] = { 0x21, 0x7A, width, height, 0x21, 0x7A };
    sw_write_buffer(dat, 6);
    sw_write_buffer(image, width * height);
}

//发送压缩图像 例如 120x180的图像太大 传输速度慢  则可以发送 60x90的图像来提高速度
//*image图像地址 width图像宽 height图像高dis_width压缩后的图像宽 dis_height压缩后的图像高
//例：sendimg(mt9v03x_image_dvp[0], MT9V03X_DVP_W, MT9V03X_DVP_H,MT9V03X_DVP_W/2,MT9V03X_DVP_H/2);
void sendimg_zoom(uint8* image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
{
    uint8 dat[6] = { 0x21, 0x7A, dis_width, dis_height, 0x21, 0x7A };
    sw_write_buffer(dat, 6);
    uint8 i,j;
    for(j=0;j<dis_height;j++)
   {
       for(i=0;i<dis_width;i++)
       {
           sw_write_byte(*(image+(j*height/dis_height)*width+i*width/dis_width));//读取像素点

       }
   }
}
//--------------二值化图传-------------------//
//uint8 (*src)[COL]图像地址 width图像宽 height图像高 otu二值化阈值
//例：sendimg_binary(mt9v03x_image_dvp[0], MT9V03X_DVP_W, MT9V03X_DVP_H,100);
void sendimg_binary( uint8* image, uint16 width, uint16 height,uint8 otu)
{

    uint8 dat[6]={0x7A,0x21,width,height,0x7A,0x21};
    sw_write_buffer(dat,6);
    int databool=255;char lon=1;int data=255;
    uint8 line=0,col=0;

    for(line=0;line<width;line++)
        {
           for(col=0;col<height;col++)
           {
              if(*(image+line*height+col)>otu)data=255;
              else data=0;
              if(data==databool)
              {lon++;}else{sw_write_byte(lon);lon=1;}
              if(lon==100){sw_write_byte(lon-1);sw_write_byte(0);lon=1;}
             databool=data;
           }
        }
}

void sendimg_binary_zoom( uint8* image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height,uint8 otu)
{

    uint8 dat[6]={0x7A,0x21,dis_width,dis_height,0x7A,0x21};
    sw_write_buffer(dat,6);
    int databool=255;char lon=1;int data=255;
    uint8 i,j;
       for(j=0;j<dis_height;j++)
      {
          for(i=0;i<dis_width;i++)
          {
              if(*(image+(j*height/dis_height)*width+i*width/dis_width)>otu)//读取像素点
             data=255;
                          else data=0;
                          if(data==databool)
                          {lon++;}
                          else{sw_write_byte(lon);lon=1;}
                          if(lon==100){sw_write_byte(lon-1);sw_write_byte(0);lon=1;}
                         databool=data;
          }
      }

}
//--------------传线-------------------//
/*
 * 可以将寻线得到的左右边线，拟的中线发送到上位机查看
 * 例如：
 * 处理 图像img[H][W];得到左右边线存放在zx[H] yx[H] 拟出来的中线为w[H]
 * sendline_clear(swj_BLACK,H,W);//清屏 背景黑色
 * sendline(swj_WHITE,zx,H);//发送左边线
 * sendline(swj_WHITE,yx,H);//发送右边线
 * sendline(swj_WHITE,wx,H);//发送中线
 * 例如：
 * sendline_clear(swj_BLACK,H,W);//清屏 背景黑色
 * sendline_xy(zx,yx,H)//发送左右边线
 * sendline(swj_WHITE,wx,H);//发送中线
 *
 * 如上两个方法效果一致的，但下面可以在上位机上对应使用赛道还原功能
 * 注意：
 * ①每发送完一帧的图像边线 就要调用sendline_clear进行清屏
 * ②如果调用sendline_xy函数并使用上位机赛道还原功能时，若再调用sendline 一定放在sendline_xy后面 防止被覆盖
 * */
#define swj_BLACK 0
#define swj_WHITE 1
#define swj_RED 2
#define swj_GREEN 3
#define swj_BLUE 4
#define swj_PURPLE 5
#define swj_YELLOW 6
#define swj_CYAN 7
#define swj_ORANGE 8
//清空线条   color清屏后的背景颜色  uint16 width uint16 height 图像的大小
void sendline_clear( uint8 color,uint16 width, uint16 height)
{
    sw_write_byte(0x21); sw_write_byte(0x7A);
    sw_write_byte(width);sw_write_byte(height);
    sw_write_byte(color);sw_write_byte(0x21);
}
//图传赛道边界  uint8_t *zx:左边界   uint8_t *yx:右边界, uint32_t len发送的边线长度
//该函数与下放函数分别发送两个边线有何不同? 该函数可对应上位机还原赛道的功能*  注意先后顺序
void sendline_xy( uint8_t *line_zx,uint8_t *line_yx, uint32_t len)
{
    sw_write_byte(0x21);
    sw_write_byte(9);
    sw_write_byte(len);
    sw_write_byte(255);
    sw_write_byte(255);
    sw_write_buffer(line_zx,len);
    sw_write_buffer(line_yx,len);
}
/*默认每行一个点*/
//绘制边线   color边线颜色  uint8_t *buff 发送的边线数组地址  len发送的边线长度
void sendline( uint8 color,uint8_t *buff, uint32_t len)
{
    sw_write_byte(0x21);
    sw_write_byte(color);
    sw_write_byte(len);
    sw_write_byte(255);
    sw_write_byte(255);
    sw_write_buffer(buff,len);
}
/*说明:
 * 例如有三个点 a(x1,y1)b(x2,y2)c(x3,y3)
 * 则 uint8 x[3]={x1,x2,x3};uint8 y[3]={y1,y2,y3};
 *  sendline2(swj_WHITE,x,y,3);
 *  sendline函数只能按顺序每行一点发送边界点
 *  sendline2函数针对于八邻域等 每行不固定点数的边界
  *           也适用于发送特殊点 例如拐点 灵活运用
 *
 * */
//无序绘制边线  color边线颜色 linex对应点的x坐标集合 liney对应点的y坐标集合  len发送的边线长度
void sendline2( uint8 color,uint8_t *linex,uint8_t *liney, uint32_t len)
{
    sw_write_byte(0x21);
    sw_write_byte(color);
    sw_write_byte(len);
    sw_write_byte(254);
    sw_write_byte(255);
    sw_write_buffer(linex,len);
    sw_write_buffer(liney,len);

}
/*
 * 无线模块如何拉高波特率？ 淘宝官方店里可以下载资料 可以跟着官方手册里的教程来
 * 程序如何拉高波特率？ 串口初始化函数里
 * 速度太慢整么办？①将波特率拉到最高②推荐使用二值化图传
 * */
//--------------------------------------------------无线模块-----------------------------------------------//








//--------------------------------------------------wifi模块-----------------------------------------------//
/*
 * wifi模块 可以使用esp8266 10元左右 成本低，速度快
 * 单片机-串口-wifi-tcp(udp)-上位机 其也是将数据通过串口发送到wifi模块上
 * 其对环境要求高 容易受干扰出现丢包
 *
 */


void sendimg_WIFI( uint8 (*src)[COL], uint16 width, uint16 height)
{

    sw_write_byte(0x21); sw_write_byte(0x7A);
    sw_write_byte(width);sw_write_byte(height);
    sw_write_byte(0x21);sw_write_byte(0x7A);

    uint8 line=0,col=0;
    for(line=0;line<width;line++)
        {
        sw_write_byte(21);
        sw_write_byte(line);
        sw_write_byte(133);
           for(col=0;col<height;col++)
           {
               sw_write_byte(src[line][col]);

           }

        }
}
