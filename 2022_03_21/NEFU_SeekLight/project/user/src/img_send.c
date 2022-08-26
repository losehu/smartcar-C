/*
 * img_send.c
 *
 *  Created on: 2022年3月21日
 *      Author: Administrator
 */

#include "img_send.h"
 uint8_t bin_excel[8][3]={{0,0, 0}, {0,0, 1}, {0,1, 0}, {0,1, 1},{1,0,0},{1,0,1},{1,1,0},{1,1,1},};//195
 int img_way[4][2]={{7,0},{1,2},{3,4},{5,6},};
 int img_setp[8][2]={{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};
uint8_t img_info[5000], img_bit_cnt = 0, img_bit_sum = 0;
bool blocked(int head, int dir, int center_x, int center_y) {
    center_x = go[head][dir][1] + center_x;
    center_y = go[head][dir][0] + center_y;
    if (center_x < 0 || center_x >= COL)return 1;
    if (center_y < 0 || center_y >= ROW)return 1;
    if (ImageUsed[ROW - 1 - center_y][center_x] == 0)return 1;
    return 0;
}
void write_img_bits(bool num) {
    img_info[img_bit_sum] = img_info[img_bit_sum] | (num << img_bit_cnt);
    img_bit_cnt++;
    if (img_bit_cnt == 8) {
        img_bit_sum++;
        img_bit_cnt = 0;
    }
}

void zip_img(void) {
    img_bit_cnt = 0, img_bit_sum = 0;
memset(img_info,0,sizeof(img_info));
    uint8_t img_tmp[ROW][COL];
    start_line = 5;
    int search_len = 20;
    int search_start = 0;
    seed_left_x = 0, seed_left_y = 0;
    int search_start_sum = 0, search_start_sum_cnt = 0;
    for (int i = 0; i < COL; i++) {
        if (ImageUsed[ROW - 1][i] != 0) {
            search_start_sum += i;
            search_start_sum_cnt++;
        }
    }
    search_start = search_start_sum / search_start_sum_cnt;
    memset(img_tmp, 0, sizeof(img_tmp));
    while (start_line >= 0){
        stack_top = 0, stack_top1 = 0;
        seed_right_x = 0, seed_left_x = 0;
        for (int i =
                (search_start + search_len) < COL - 3 ?
                (search_start + search_len) : COL - 3; i >= 0; i--) {
            if ((ImageUsed[ROW - 1 - start_line][i]
                 != ImageUsed[ROW - 1 - start_line][i + 1]
                 && ImageUsed[ROW - 1 - start_line][i] == 0)
                || (i == 0 && ImageUsed[ROW - 1 - start_line][i] != 0)
                || (ImageUsed[ROW - 1 - start_line][i] != 0 && i == 0)) {
                if (ImageUsed[ROW - 1 - start_line][i + 1]
                    == ImageUsed[ROW - 1 - start_line][i + 2]
                        ) {
                    if (ImageUsed[ROW - 1 - start_line][i] != 0 && i == 0) {
                        if (pull_stack(i, start_line, img_tmp))
                            return;
                        img_tmp[start_line][i + 1] = 1;
                        seed_left_x = i;
                        seed_left_y = start_line;
                        break;
                    } else if (pull_stack(i + 1, start_line, img_tmp))
                        return;
                    img_tmp[start_line][i + 1] = 1;
                    seed_left_x = i + 1;
                    seed_left_y = start_line;
                    break;
                }
            }
        }
        start_line--;
    }
    if (stack_top == 0 )return;
    int head_left = 0;
    int left_center_x = seed_left_x, left_center_y = seed_left_y;
    max_py = left_center_y;
    min_py = left_center_y;
    bool search_flag = 1;
    //head
   write_img_bits(1);
    write_img_bits(0);
    write_img_bits(1);
    write_img_bits(0);
    write_img_bits(1);
    write_img_bits(0);
    write_img_bits(1);
    write_img_bits(0);
    //start_point
    for(int i=0;i<8;i++)write_img_bits((seed_left_x>>i)&1);
for(int i=0;i<8;i++)write_img_bits((seed_left_y>>i)&1);
int buff_cnt=0;
    img_bit_sum+=2;
    while (search_flag) {
        while (blocked(head_left, 2, left_center_x, left_center_y))head_left++, head_left = (head_left + 4) % 4;
        if (!blocked(head_left, 1, left_center_x, left_center_y)) {
            left_center_x += go[head_left][1][1];
            left_center_y += go[head_left][1][0];
            buff_cnt++;
            write_img_bits(bin_excel[img_way[head_left][0]][2]);
            write_img_bits(bin_excel[img_way[head_left][0]][1]);
            write_img_bits(bin_excel[img_way[head_left][0]][0]);
            head_left--;
            head_left = (head_left + 4) % 4;
        } else if (blocked(head_left, 1, left_center_x, left_center_y)) {
            left_center_x += go[head_left][2][1];
            left_center_y += go[head_left][2][0];
            buff_cnt++;
            write_img_bits(bin_excel[img_way[head_left][1]][2]);
            write_img_bits(bin_excel[img_way[head_left][1]][1]);
            write_img_bits(bin_excel[img_way[head_left][1]][0]);
        }
        img_tmp[left_center_y][left_center_x] = 1;
        if (left_center_x == seed_left_x && seed_left_y == left_center_y)search_flag = 0;
    }
    //end
    write_img_bits(1);
    write_img_bits(0);
    write_img_bits(1);
    write_img_bits(1);
    write_img_bits(1);
    write_img_bits(1);
    write_img_bits(0);
    write_img_bits(1);
//step_len
    img_bit_sum=3,img_bit_cnt=0;
    for(int i=0;i<16;i++)write_img_bits(((buff_cnt)>>i)&1);

    uart_write_buffer(WRIELESS_UART_INDEX, img_info,(int)ceil(1.0*(6*8+buff_cnt*3) / 8) );   // 发送最后的数据
//805E
//E805
//F402
//402F
//00BD
}
