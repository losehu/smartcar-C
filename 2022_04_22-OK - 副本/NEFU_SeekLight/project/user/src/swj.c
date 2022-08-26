/*
 * ���ܳ���λ��  qqȺ:297809802 qq1849057843
 */
#include<swj.h>

//--------------------------------------------------����ģ��-----------------------------------------------//
/*
 * ����ģ�� �Ƽ�ʹ����ɵ�����ģ�飨usb to nrf��*����ʱ����ͬʱ����ttl����������С��������ģ��
 * �����ʿ����������460800 *��ӦС���ϵĳ���,С��������ģ�飬�����ϵ�����ģ�飬��λ��������һ��
 * ����ģ���ٶȱ�wifi�� ���ŵ����ȶ� ���ڻҶ�ͼ���ٶȲ�̫���ã����ж�ֵ��ͼ���Ż� �ʺϱ�ͼ����ʾ����
 *
 */
//--------------ʾ����-------------------//
//name���ݱ�ʶ(ͨ��) dat:����
//��:int a=0;put_int(0,a);
void put_int(uint8 name, int dat)
{
    uint8 fu = 32;
    if (dat < 0) { dat *= -1; fu = 45; }
    uint i = 1; uint8 j = 1;
    while (dat >= i * 10) { i *= 10; j++; }
    uint8 datc[6] = { 122, name, j, fu, 33 };
    sw_write_buffer(datc, 5);//����ת����ģ�� ���ڷ�������
    for (; j > 0; j--)
    {
        uint8 pdat = dat / i;
        pdat %= 10;
        sw_write_byte(pdat);//�����ߴ��ڷ���һ���ֽ�
        i /= 10;
    }
    sw_write_byte(10);//�����ߴ��ڷ���һ���ֽ�
}


//--------------�Ҷ�ͼ��-------------------//
//*imageͼ���ַ widthͼ��� heightͼ���
//����sendimg(mt9v03x_image_dvp[0], MT9V03X_DVP_W, MT9V03X_DVP_H);
void sendimg(uint8* image, uint16 width, uint16 height)
{
    uint8 dat[6] = { 0x21, 0x7A, width, height, 0x21, 0x7A };
    sw_write_buffer(dat, 6);
    sw_write_buffer(image, width * height);
}

//����ѹ��ͼ�� ���� 120x180��ͼ��̫�� �����ٶ���  ����Է��� 60x90��ͼ��������ٶ�
//*imageͼ���ַ widthͼ��� heightͼ���dis_widthѹ�����ͼ��� dis_heightѹ�����ͼ���
//����sendimg(mt9v03x_image_dvp[0], MT9V03X_DVP_W, MT9V03X_DVP_H,MT9V03X_DVP_W/2,MT9V03X_DVP_H/2);
void sendimg_zoom(uint8* image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
{
    uint8 dat[6] = { 0x21, 0x7A, dis_width, dis_height, 0x21, 0x7A };
    sw_write_buffer(dat, 6);
    uint8 i,j;
    for(j=0;j<dis_height;j++)
   {
       for(i=0;i<dis_width;i++)
       {
           sw_write_byte(*(image+(j*height/dis_height)*width+i*width/dis_width));//��ȡ���ص�

       }
   }
}
//--------------��ֵ��ͼ��-------------------//
//uint8 (*src)[COL]ͼ���ַ widthͼ��� heightͼ��� otu��ֵ����ֵ
//����sendimg_binary(mt9v03x_image_dvp[0], MT9V03X_DVP_W, MT9V03X_DVP_H,100);
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
              if(*(image+(j*height/dis_height)*width+i*width/dis_width)>otu)//��ȡ���ص�
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
//--------------����-------------------//
/*
 * ���Խ�Ѱ�ߵõ������ұ��ߣ�������߷��͵���λ���鿴
 * ���磺
 * ���� ͼ��img[H][W];�õ����ұ��ߴ����zx[H] yx[H] �����������Ϊw[H]
 * sendline_clear(swj_BLACK,H,W);//���� ������ɫ
 * sendline(swj_WHITE,zx,H);//���������
 * sendline(swj_WHITE,yx,H);//�����ұ���
 * sendline(swj_WHITE,wx,H);//��������
 * ���磺
 * sendline_clear(swj_BLACK,H,W);//���� ������ɫ
 * sendline_xy(zx,yx,H)//�������ұ���
 * sendline(swj_WHITE,wx,H);//��������
 *
 * ������������Ч��һ�µģ��������������λ���϶�Ӧʹ��������ԭ����
 * ע�⣺
 * ��ÿ������һ֡��ͼ����� ��Ҫ����sendline_clear��������
 * ���������sendline_xy������ʹ����λ��������ԭ����ʱ�����ٵ���sendline һ������sendline_xy���� ��ֹ������
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
//�������   color������ı�����ɫ  uint16 width uint16 height ͼ��Ĵ�С
void sendline_clear( uint8 color,uint16 width, uint16 height)
{
    sw_write_byte(0x21); sw_write_byte(0x7A);
    sw_write_byte(width);sw_write_byte(height);
    sw_write_byte(color);sw_write_byte(0x21);
}
//ͼ�������߽�  uint8_t *zx:��߽�   uint8_t *yx:�ұ߽�, uint32_t len���͵ı��߳���
//�ú������·ź����ֱ������������кβ�ͬ? �ú����ɶ�Ӧ��λ����ԭ�����Ĺ���*  ע���Ⱥ�˳��
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
/*Ĭ��ÿ��һ����*/
//���Ʊ���   color������ɫ  uint8_t *buff ���͵ı��������ַ  len���͵ı��߳���
void sendline( uint8 color,uint8_t *buff, uint32_t len)
{
    sw_write_byte(0x21);
    sw_write_byte(color);
    sw_write_byte(len);
    sw_write_byte(255);
    sw_write_byte(255);
    sw_write_buffer(buff,len);
}
/*˵��:
 * ������������ a(x1,y1)b(x2,y2)c(x3,y3)
 * �� uint8 x[3]={x1,x2,x3};uint8 y[3]={y1,y2,y3};
 *  sendline2(swj_WHITE,x,y,3);
 *  sendline����ֻ�ܰ�˳��ÿ��һ�㷢�ͱ߽��
 *  sendline2��������ڰ������ ÿ�в��̶������ı߽�
  *           Ҳ�����ڷ�������� ����յ� �������
 *
 * */
//������Ʊ���  color������ɫ linex��Ӧ���x���꼯�� liney��Ӧ���y���꼯��  len���͵ı��߳���
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
 * ����ģ��������߲����ʣ� �Ա��ٷ���������������� ���Ը��Źٷ��ֲ���Ľ̳���
 * ����������߲����ʣ� ���ڳ�ʼ��������
 * �ٶ�̫����ô�죿�ٽ�������������ߢ��Ƽ�ʹ�ö�ֵ��ͼ��
 * */
//--------------------------------------------------����ģ��-----------------------------------------------//








//--------------------------------------------------wifiģ��-----------------------------------------------//
/*
 * wifiģ�� ����ʹ��esp8266 10Ԫ���� �ɱ��ͣ��ٶȿ�
 * ��Ƭ��-����-wifi-tcp(udp)-��λ�� ��Ҳ�ǽ�����ͨ�����ڷ��͵�wifiģ����
 * ��Ի���Ҫ��� �����ܸ��ų��ֶ���
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
