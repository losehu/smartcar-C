/*
 * img_send.h
 *
 *  Created on: 2022Äê3ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef IMG_SEND_H_
#define IMG_SEND_H_
#include "zf_common_headfile.h"

bool blocked(int head, int dir, int center_x, int center_y) ;
void write_img_bits(bool num) ;
void zip_img(void) ;
extern uint8_t bin_excel[8][3];//195
extern int img_way[4][2];
extern int img_setp[8][2];
#endif /* IMG_SEND_H_ */
