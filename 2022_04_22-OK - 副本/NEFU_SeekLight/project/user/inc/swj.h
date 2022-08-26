#ifndef SWJ_H_
#define SWJ_H_
#include "zf_common_headfile.h"

//��Ҫ�޸ĵĵط�
#define  sw_write_byte(dat)  uart_write_byte(WRIELESS_UART_INDEX,dat)//���ڷ����ֽ�
#define  sw_write_buffer(dat,len)  wireless_uart_send_buff(dat,len)//���ڷ�������
#define ROW MT9V03X_DVP_W//ͼ���
#define COL MT9V03X_DVP_H//ͼ���
//����
void sendimg_binary_vec();

void put_int(uint8 name,int dat);
void sendimg( uint8 *image, uint16 width, uint16 height);
void sendimg_zoom(uint8* image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);
void sendimg_binary(uint8* image, uint16 width, uint16 height,uint8 otu);
void sendimg_binary_zoom(uint8* image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height,uint8 otu);
void sendline_clear( uint8 color,uint16 width, uint16 height);
void sendline_xy( uint8_t *line_zx,uint8_t *line_yx, uint32_t len);
void sendline( uint8 color,uint8_t *buff, uint32_t len);
void sendline2( uint8 color,uint8_t *linex,uint8_t *liney, uint32_t len);
//wifi
void sendimg_WIFI( uint8 (*src)[COL], uint16 width, uint16 height);
#endif /* SWJ_H_ */
