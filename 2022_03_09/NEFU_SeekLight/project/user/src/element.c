/*
 * element.c
 *
 *  Created on: Feb 13, 2022
 *      Author: Administrator
 */

#ifndef ELEMENT_C_
#define ELEMENT_C_
#include "element.h"
circle circle_flag;
fork fork_flag;    //栈
door door_flag;

/****************SUM***********************/

void init_flag() {
    clear_circle_flag();
    clear_fork_flag();
    clear_cross_flag();
    clear_door_flag();
}
bool block_judge(int start_ROW, int end_ROW, int start_COL, int end_COL,
        int tmp, int mode) {
    int block_cnt = 0;
    if (mode == 0) {
        for (int i = start_ROW; i < end_ROW; i++) {
            for (int j = start_COL; j < end_COL; j++) {
                if (ImageUsed[i][j] == 0)
                    block_cnt++;
            }
        }
    } else {
        for (int i = start_ROW; i < end_ROW; i++) {
            for (int j = start_COL; j < end_COL; j++) {
                if (ImageUsed[i][j] != 0)
                    block_cnt++;
            }
        }
    }
    if (block_cnt <= tmp)
        return 0;
    return 1;
}
void close_judge(int flag) {
    circle_flag.judge_left = 0;
    circle_flag.judge_right = 0;
    fork_flag.judge = 0;
    Cross.judge = 0;
    door_flag.judge = 0;
    if (flag == -1)
        circle_flag.judge_left = 1;
    else if (flag == 0)
        circle_flag.judge_right = 1;
    else if (flag == 1)
        fork_flag.judge = 1;
    else if (flag == 2)
        Cross.judge = 1;
    else if (flag == 3)
        door_flag.judge = 1;
}
void judge_unit() {
    if (circle_flag.judge_left == 1 || circle_flag.judge_right)
        judge_circle();
     if (fork_flag.judge == 1)     judge_fork();
    //if(Cross.judge==1)CrsosJudge();
  //  if (door_flag.judge == 1)    judge_door();
}
/*****************车库***********************/
void clear_door_flag() {
    door_flag.find_line=0;
    door_flag.judge = 1;
    door_flag.find_door = 0;
}
void judge_door() {
    if (door_flag.find_line == 1&&door_flag.find_door==0) {
        int cnt = 0;
        for (int i = 0; i < ROW / 2; i++) {
            if (Img.RightBorder[i] - Img.LeftBorder[i] > RoadWide0 * 1.5&&(Img.RightBorderFindFlag[i]=='T'||Img.LeftBorderFindFlag[i]=='T'))
                cnt++;
        }
        if (cnt < RoadWide0 * 0.6) {
            clear_door_flag();
            return;
        }
        door_flag.find_door=1;
        close_judge(3);
        ips114_clear(BROWN);
    }else if(  door_flag.find_door==1)
    {
        int cnt = 0;
               for (int i = 0; i <RoadWide0*0.8; i++) {
                   if (Img.RightBorder[i] - Img.LeftBorder[i] <= RoadWide0 * 1.5&&(Img.RightBorderFindFlag[i]=='T'||Img.LeftBorderFindFlag[i]=='T'))
                       cnt++;
               }
               if (cnt <=RoadWide0*0.8-5) return;
                  ips114_clear(YELLOW);
               init_flag();

    }
    return;
}
/*****************三叉***********************/

void clear_fork_flag() {
    fork_flag.judge = 1;
    fork_flag.find_fork = 0;
    fork_flag.on_fork = 0;
    fork_flag.out_fork = 0;
    fork_flag.leave_fork = 0;
}

int a_cnt = 0;
void judge_fork() {
    if (fork_flag.find_fork == 0) {
        if (max_py1 < ROW - 5 && max_py < ROW - 5)
            return;
        if (!block_judge(ROW - 1 - 5, ROW - 1, 0, 5, 20, 0))
            return;
        if (!block_judge(ROW - 1 - 5, ROW - 1, COL - 1 - 5, COL - 1, 20, 0))
            return;
        if (!block_judge(0 + ROW / 5, 5 + ROW / 5, COL / 2 - 2, COL / 2 + 3, 20,
                0))
            return;
        int useful = 0;
        for (int i = 0; i < 10; i++) {
            if (Img.LeftBorderFindFlag[i] == 'T'
                    && Img.RightBorderFindFlag[i] == 'T')
                useful++;
        }
        if (Img.RightBorder[ROW / 5 * 2] - Img.LeftBorder[ROW / 5 * 2]
                < RoadWide0 * 2.5)
            return;
        int16 k_error = 4;
        int16 error_num0 = abs(Img.LeftBorder[0] - Img.LeftBorder[3]);
        int16 error_num1 = abs(Img.LeftBorder[3] - Img.LeftBorder[6]);
        int16 error_num2 = abs(Img.LeftBorder[6] - Img.LeftBorder[9]);
        if (abs(error_num0 - error_num1) > k_error)
            return;
        if (abs(error_num1 - error_num2) > k_error)
            return;
        if (abs(error_num0 - error_num2) > 2 * k_error)
            return;
        error_num0 = abs(Img.RightBorder[0] - Img.RightBorder[3]);
        error_num1 = abs(Img.RightBorder[3] - Img.RightBorder[6]);
        error_num2 = abs(Img.RightBorder[6] - Img.RightBorder[9]);
        if (abs(error_num0 - error_num1) > k_error)
            return;
        if (abs(error_num1 - error_num2) > k_error)
            return;
        if (abs(error_num0 - error_num2) > 2 * k_error)
            return;

        ips114_clear(BLUE);

        fork_flag.find_fork = 1;
        gpio_high(BEEP_PIN);

        close_judge(1);
    } else if (fork_flag.on_fork == 0) {

        int useful = 0;
        for (int i = 0; i < ROW / 2; i++) {
            if (Img.LeftBorderFindFlag[i] == 'T'
                    && Img.RightBorderFindFlag[i] == 'T')
                useful++;
        }
        if (useful < ROW / 2 - 8)
            return;

        fork_flag.on_fork = 1;
    } else if (fork_flag.out_fork == 0) {


        if (!block_judge(ROW - 1 - 5, ROW - 1, 0, 5, 20, 0))
            return;
        if (!block_judge(ROW - 1 - 5, ROW - 1, COL - 1 - 5, COL - 1, 20, 0))
            return;


        int useful = 0;
        for (int i = 0; i < 10; i++) {
            if (Img.LeftBorderFindFlag[i] == 'T'
                    && Img.RightBorderFindFlag[i] == 'T')
                useful++;
        }
        if (Img.RightBorder[ROW / 5 * 2] - Img.LeftBorder[ROW / 5 * 2]
                < RoadWide0 * 2.5)
            return;
        if (Img.RightBorder[ROW / 5 * 2 + 1] - Img.LeftBorder[ROW / 5 * 2 + 1]
                < RoadWide0 * 2.5)
            return;
        if (Img.RightBorder[ROW / 5 * 2 - 1] - Img.LeftBorder[ROW / 5 * 2 - 1]
                < RoadWide0 * 2.5)
            return;

        fork_flag.out_fork = 1;

    } else if (fork_flag.leave_fork == 0) {

        if (Img.LeftBorderFindFlag[0] == 'F'
                || Img.RightBorderFindFlag[0] == 'F')
            return;
        int useful = 0;
        for (int i = 0; i < ROW / 5; i++) {
            if (Img.LeftBorderFindFlag[i] == 'T'
                    && Img.RightBorderFindFlag[i] == 'T')
                useful++;
        }
        if (useful < ROW / 5 - 5)
            return;
        int16 k_error = 3;
        int error_num0 = abs(
                Img.RightBorder[ROW - 1 - 0]
                        - Img.RightBorder[ROW - 1 - ROW / 20]);
        int error_num1 = abs(
                Img.RightBorder[ROW - 1 - ROW / 10]
                        - Img.RightBorder[ROW - 1 - ROW / 20]);
        int error_num2 = abs(
                Img.RightBorder[ROW - 1 - ROW / 10]
                        - Img.RightBorder[ROW - 1 - ROW / 20 * 3]);
        if (abs(error_num0 - error_num1) > k_error)
            return;
        if (abs(error_num1 - error_num2) > k_error)
            return;
        if (abs(error_num0 - error_num2) > 2 * k_error)
            return;
        error_num0 = abs(
                Img.LeftBorder[ROW - 1 - 0]
                        - Img.LeftBorder[ROW - 1 - ROW / 20]);
        error_num1 = abs(
                Img.LeftBorder[ROW - 1 - ROW / 10]
                        - Img.LeftBorder[ROW - 1 - ROW / 20]);
        error_num2 = abs(
                Img.LeftBorder[ROW - 1 - ROW / 10]
                        - Img.LeftBorder[ROW - 1 - ROW / 20 * 3]);
        if (abs(error_num0 - error_num1) > k_error)
            return;
        if (abs(error_num1 - error_num2) > k_error)
            return;
        if (abs(error_num0 - error_num2) > 2 * k_error)
            return;
        if (!block_judge(ROW - 1 - 5, ROW - 1, 0, 5, 20, 0))
            return;
        if (!block_judge(ROW - 1 - 5, ROW - 1, COL - 1 - 5, COL - 1, 20, 0))
            return;
        gpio_low(BEEP_PIN);
        //   clear_circle_flag();

        //   stop_flag=1;
        gpio_low(BEEP_PIN);
       // stop_flag = 1;
        ips114_show_gray_image_vec(0, 0, PerImg_ip, 114, 100, 114, 100, 0);
        fork_flag.leave_fork = 1;
        init_flag();
fork_flag.cnt++;
    }
}
/*****************环岛***********************/

void clear_circle_flag() {
    circle_flag.judge_right = 1;
    circle_flag.judge_left = 1;
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

void judge_circle() {
    if (circle_flag.judge_right == 1)
        judge_circle_right();
    if (circle_flag.judge_left == 1)
        judge_circle_left();
}
bool temp_flag = 0;
int cntt = 0;
void judge_circle_right() {
    if (circle_flag.find_right_circle == 0) {
        temp_flag = 0;

        if ((abs(right_lost_cnt - left_lost_cnt) < ROW / 3 * 2)
                && left_lost_cnt >= ROW / 5)
            return;
        if (right_lost_cnt < ROW / 3)
            return;
        int search_col = COL - 1 - 10;
        if (!(ImageUsed[ROW - 1][search_col] == ImageUsed[ROW - 2][search_col]
                && ImageUsed[ROW - 1][search_col]
                        == ImageUsed[ROW - 1 - 5][search_col]
                && ImageUsed[ROW - 1][search_col] != 0
                && Img.LeftBorderFindFlag[0] == Img.LeftBorderFindFlag[1]
                && Img.LeftBorderFindFlag[5] == Img.LeftBorderFindFlag[5]
                && Img.LeftBorderFindFlag[0] == 'T'))
            return;
        if (Img.LeftBorderFindFlag[ROW / 4] == 'F'
                || Img.LeftBorderFindFlag[ROW / 2] == 'F'
                || Img.LeftBorderFindFlag[ROW / 4 * 3] == 'F')
            return;
        int block_cnt = 0;
        for (int i = ROW - 1; i >= ROW - 5; i--) {
            for (int j = COL - 1 - 5; j < COL - 1; j++) {
                if (ImageUsed[i][j - 20] != 0)
                    block_cnt++;
            }
        }
        if (block_cnt <= 20)
            return;
        block_cnt = 0;
        for (int i = ROW - 1 - ROW / 2 + 2; i >= ROW - 1 - ROW / 2 - 2; i--) {
            for (int j = COL - 1 - 5; j < COL - 1; j++) {
                if (ImageUsed[i][j - 10] == 0)
                    block_cnt++;
            }
        }
        if (block_cnt <= 20)
            return;

        block_cnt = 0; //ZUO上角不能是白色！！
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (ImageUsed[i][j] == 0)
                    block_cnt++;
            }
        }
        if (block_cnt <= 20)
            return;

        int16 k_error = 4;
        int16 error_num0 = abs(
                Img.LeftBorder[ROW - 1 - 0]
                        - Img.LeftBorder[ROW - 1 - ROW / 4]);
        int16 error_num1 = abs(
                Img.LeftBorder[ROW - 1 - ROW / 2]
                        - Img.LeftBorder[ROW - 1 - ROW / 4]);
        int16 error_num2 = abs(
                Img.LeftBorder[ROW - 1 - ROW / 2]
                        - Img.LeftBorder[ROW - 1 - ROW / 4 * 3]);
        if (abs(error_num0 - error_num1) > k_error)
            return;
        if (abs(error_num1 - error_num2) > k_error)
            return;
        if (abs(error_num0 - error_num2) > 2 * k_error)
            return;
        error_num0 = abs(
                Img.LeftBorder[ROW - 1 - 0]
                        - Img.LeftBorder[ROW - 1 - ROW / 20]);
        error_num1 = abs(
                Img.LeftBorder[ROW - 1 - ROW / 10]
                        - Img.LeftBorder[ROW - 1 - ROW / 20]);
        error_num2 = abs(
                Img.LeftBorder[ROW - 1 - ROW / 10]
                        - Img.LeftBorder[ROW - 1 - ROW / 20 * 3]);
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
        int sum_temp = 0;
        for (int i = 0; i < ROW; i++) {
            sum_temp += Img.LeftBorder[i];
        }
        if (sum_temp > ROW * COL * 0.5)
            return;

        gpio_high(BEEP_PIN);
        close_judge(0);

        circle_flag.find_right_circle = 1;
        ExSpeed_SUM = circle_speed;
        CenterCalMinRow = CenterCalMinRow_tmp;
        CenterCalMaxRow = CenterCalMaxRow_tmp;

    } else if (circle_flag.enter_right_circle == 0) {
        if ((abs(right_lost_cnt - left_lost_cnt) < ROW / 3 * 2)
                && left_lost_cnt >= ROW / 5)
            return;
        if (!(Img.LeftBorderFindFlag[0] == Img.LeftBorderFindFlag[1]
                && Img.LeftBorderFindFlag[5] == Img.LeftBorderFindFlag[5]
                && Img.LeftBorderFindFlag[0] == 'T'))
            return;
        if (Img.LeftBorderFindFlag[ROW / 4] == 'F'
                || Img.LeftBorderFindFlag[ROW / 2] == 'F'
                || Img.LeftBorderFindFlag[ROW / 4 * 3] == 'F')
            return;
        int block_cnt = 0;
        for (int i = ROW - 1; i >= ROW - 10; i--) {
            for (int j = COL - 1 - 10; j < COL - 1; j++) {
                if (ImageUsed[i][j] == 0)
                    block_cnt++;
            }
        }
        if (block_cnt <= 80)
            return;
        block_cnt = 0;
        for (int i = ROW / 2 - 5; i <= ROW / 2 + 4; i++) {
            for (int j = COL - 1 - 10; j < COL - 1; j++) {
                if (ImageUsed[i][j] != 0)
                    block_cnt++;
            }
        }
        if (block_cnt <= 80)
            return;
        int16 k_error = 4;
        int16 error_num0 = abs(
                Img.LeftBorder[ROW - 1 - 0]
                        - Img.LeftBorder[ROW - 1 - ROW / 4]);
        int16 error_num1 = abs(
                Img.LeftBorder[ROW - 1 - ROW / 2]
                        - Img.LeftBorder[ROW - 1 - ROW / 4]);
        int16 error_num2 = abs(
                Img.LeftBorder[ROW - 1 - ROW / 2]
                        - Img.LeftBorder[ROW - 1 - ROW / 4 * 3]);
        if (abs(error_num0 - error_num1) > k_error)
            return;
        if (abs(error_num1 - error_num2) > k_error)
            return;
        if (abs(error_num0 - error_num2) > 2 * k_error)
            return;
        error_num0 = abs(
                Img.LeftBorder[ROW - 1 - 0]
                        - Img.LeftBorder[ROW - 1 - ROW / 20]);
        error_num1 = abs(
                Img.LeftBorder[ROW - 1 - ROW / 10]
                        - Img.LeftBorder[ROW - 1 - ROW / 20]);
        error_num2 = abs(
                Img.LeftBorder[ROW - 1 - ROW / 10]
                        - Img.LeftBorder[ROW - 1 - ROW / 20 * 3]);
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
        circle_flag.enter_right_circle = 1;

    } else if (circle_flag.on_right_circle == 0) {
        if (max_py >= ROW / 5 * 3 || max_py <= 5)
            return;
        if (Img.LeftBorder[0] - Img.LeftBorder[ROW / 20] >= 0)
            return;
        if (Img.LeftBorder[ROW / 20] - Img.LeftBorder[ROW / 10] >= 0)
            return;
        if (Img.LeftBorderFindFlag[max_py - 2] == 'F')
            return;
        if (Img.LeftBorderFindFlag[max_py - 3] == 'F')
            return;
        if (Img.LeftBorderFindFlag[max_py - 5] == 'F')
            return;
        circle_flag.on_right_circle = 1;

    } else if (circle_flag.out_right_circle == 0) {
        if (!(max_py1 >= ROW * 0.8 && max_py >= 0.8))
            return;
        circle_flag.out_right_circle = 1;

    } else if (circle_flag.leave_right_circle == 0) {
        if (temp_flag == 0) {
            if (max_py <= ROW * 0.9 || max_py1 <= ROW * 0.9)
                return;
            if (left_lost_cnt >= ROW * 0.1)
                return;

            temp_flag = 1;

        } else {
            if (max_py >= ROW * 0.8 || max_py1 >= ROW * 0.8)
                return;
            int sum_temp = 0;
            for (int i = 0; i < ROW; i++)
                sum_temp += Img.RightBorder[i];
            if (sum_temp < (COL - 1 - 10) * ROW)
                return;
            int useful = 0;
            for (int i = 0; i < 25; i++) {
                if (Img.RightBorderFindFlag[i] == 'F'
                        && Img.LeftBorderFindFlag[i] == 'T')
                    useful++;
            }
            if (useful < 20)
                return;
            int block_cnt = 0;
            for (int i = 0; i < 5; i++) {
                for (int j = COL - 1 - 5; j < COL - 1; j++) {
                    if (ImageUsed[ROW - 1 - i][j] != 0)
                        block_cnt++;
                }
            }
            if (block_cnt <= 20)
                return;
            block_cnt = 0;

            for (int i = 0; i < 5; i++) {
                for (int j = COL - 1; j >= COL - 1 - 5; j--) {
                    if (ImageUsed[ROW - 1 - i][COL - 1 - j] == 0)
                        block_cnt++;
                }
            }
            if (block_cnt <= 20)
                return;

            circle_flag.leave_right_circle = 1;

//gpio_low(BEEP_PIN);
//        stop_flag=1;
//        ips114_show_gray_image_vec(0, 0, PerImg_ip, 114, 100, 114, 100, 0);

//     stop_flag=1;
            //     ips114_show_gray_image_vec(0, 0, PerImg_ip, 114, 100, 114, 100, 0);
            ExSpeed_SUM = circle_speed - 70;
//            stop_flag=1;
//                             ips114_show_gray_image_vec(0,0,PerImg_ip,TRFED_COL,TRFED_ROW,TRFED_COL,TRFED_ROW,0);
        }
    } else {
        if (Img.RightBorderFindFlag[0] == 'F'
                || Img.LeftBorderFindFlag[0] == 'F')
            return;
        int find_cnt_right = 0, find_cnt_left = 0, diff_cnt = 0;
        for (int i = 0; i < ROW / 3; i++) {
            if (Img.RightBorderFindFlag[i] == 'T')
                find_cnt_right++;
            if (Img.LeftBorderFindFlag[i] == 'T')
                find_cnt_left++;
            if (Img.RightBorder[i] - Img.LeftBorder[i] <= RoadWide0 + 5)
                diff_cnt++;
        }
        if (find_cnt_right < ROW / 3 - 2)
            return;
        if (find_cnt_left < ROW / 3 - 2)
            return;
        if (diff_cnt < ROW / 3 - 2)
            return;
//        int16 k_error = 3;
//        int error_num0 = abs(
//                Img.LeftBorder[ROW - 1 - 0]
//                        - Img.LeftBorder[ROW - 1 - ROW / 20]);
//        int error_num1 = abs(
//                Img.LeftBorder[ROW - 1 - ROW / 10]
//                        - Img.LeftBorder[ROW - 1 - ROW / 20]);
//        int error_num2 = abs(
//                Img.LeftBorder[ROW - 1 - ROW / 10]
//                        - Img.LeftBorder[ROW - 1 - ROW / 20 * 3]);
//        if (abs(error_num0 - error_num1) > k_error)
//            return;
//        if (abs(error_num1 - error_num2) > k_error)
//            return;
//        if (abs(error_num0 - error_num2) > 2 * k_error)
//            return;
//        error_num0 = abs(
//                Img.RightBorder[ROW - 1 - 0]
//                        - Img.RightBorder[ROW - 1 - ROW / 20]);
//        error_num1 = abs(
//                Img.RightBorder[ROW - 1 - ROW / 10]
//                        - Img.RightBorder[ROW - 1 - ROW / 20]);
//        error_num2 = abs(
//                Img.RightBorder[ROW - 1 - ROW / 10]
//                        - Img.RightBorder[ROW - 1 - ROW / 20 * 3]);
//        if (abs(error_num0 - error_num1) > k_error)
//            return;
//        if (abs(error_num1 - error_num2) > k_error)
//            return;
//        if (abs(error_num0 - error_num2) > 2 * k_error)
//            return;
        gpio_low(BEEP_PIN);
        init_flag();
        //   clear_circle_flag();
        CenterCalMinRow = CenterCalMinRow_tmp;
        CenterCalMaxRow = CenterCalMaxRow_tmp;
        ExSpeed_SUM = ExSpeed_SUM_tmp;
        //  stop_flag=1;
        //       fake_do++;

    }
}

void judge_circle_left() {
    if (circle_flag.find_left_circle == 0) {
        temp_flag = 0;

        if ((abs(left_lost_cnt - right_lost_cnt) < ROW / 3 * 2)
                && right_lost_cnt >= ROW / 5)
            return;
        if (left_lost_cnt < ROW / 3)
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
        int block_cnt = 0;
        for (int i = ROW - 1; i >= ROW - 5; i--) {
            for (int j = 0; j < 5; j++) {
                if (ImageUsed[i][j + 20] != 0)
                    block_cnt++;
            }
        }
        if (block_cnt <= 20)
            return;
        block_cnt = 0;
        for (int i = ROW - 1 - ROW / 2 + 2; i >= ROW - 1 - ROW / 2 - 2; i--) {
            for (int j = 0; j < 5; j++) {
                if (ImageUsed[i][j + 10] == 0)
                    block_cnt++;
            }
        }
        if (block_cnt <= 20)
            return;

        block_cnt = 0; //右上角不能是白色！！
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (ImageUsed[i][COL - 1 - j] == 0)
                    block_cnt++;
            }
        }
        if (block_cnt <= 20)
            return;

        int16 k_error = 4;
        int16 error_num0 = abs(
                Img.RightBorder[ROW - 1 - 0]
                        - Img.RightBorder[ROW - 1 - ROW / 4]);
        int16 error_num1 = abs(
                Img.RightBorder[ROW - 1 - ROW / 2]
                        - Img.RightBorder[ROW - 1 - ROW / 4]);
        int16 error_num2 = abs(
                Img.RightBorder[ROW - 1 - ROW / 2]
                        - Img.RightBorder[ROW - 1 - ROW / 4 * 3]);
        if (abs(error_num0 - error_num1) > k_error)
            return;
        if (abs(error_num1 - error_num2) > k_error)
            return;
        if (abs(error_num0 - error_num2) > 2 * k_error)
            return;
        error_num0 = abs(
                Img.RightBorder[ROW - 1 - 0]
                        - Img.RightBorder[ROW - 1 - ROW / 20]);
        error_num1 = abs(
                Img.RightBorder[ROW - 1 - ROW / 10]
                        - Img.RightBorder[ROW - 1 - ROW / 20]);
        error_num2 = abs(
                Img.RightBorder[ROW - 1 - ROW / 10]
                        - Img.RightBorder[ROW - 1 - ROW / 20 * 3]);
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
        int sum_temp = 0;
        for (int i = 0; i < ROW; i++) {
            sum_temp += Img.RightBorder[i];
        }
        if (sum_temp < ROW * COL * 0.5)
            return;

        gpio_high(BEEP_PIN);
        close_judge(-1);

        circle_flag.find_left_circle = 1;
        ExSpeed_SUM = circle_speed;
        CenterCalMinRow = CenterCalMinRow_tmp;
        CenterCalMaxRow = CenterCalMaxRow_tmp;
        cntt++;

    } else if (circle_flag.enter_left_circle == 0) {
        if ((abs(left_lost_cnt - right_lost_cnt) < ROW / 3 * 2)
                && right_lost_cnt >= ROW / 5)
            return;
        if (!(Img.RightBorderFindFlag[0] == Img.RightBorderFindFlag[1]
                && Img.RightBorderFindFlag[5] == Img.RightBorderFindFlag[5]
                && Img.RightBorderFindFlag[0] == 'T'))
            return;
        if (Img.RightBorderFindFlag[ROW / 4] == 'F'
                || Img.RightBorderFindFlag[ROW / 2] == 'F'
                || Img.RightBorderFindFlag[ROW / 4 * 3] == 'F')
            return;
        int block_cnt = 0;
        for (int i = ROW - 1; i >= ROW - 10; i--) {
            for (int j = 0; j < 10; j++) {
                if (ImageUsed[i][j] == 0)
                    block_cnt++;
            }
        }
        if (block_cnt <= 80)
            return;
        block_cnt = 0;
        for (int i = ROW / 2 - 5; i <= ROW / 2 + 4; i++) {
            for (int j = 0; j < 10; j++) {
                if (ImageUsed[i][j] != 0)
                    block_cnt++;
            }
        }
        if (block_cnt <= 80)
            return;
        int16 k_error = 4;
        int16 error_num0 = abs(
                Img.RightBorder[ROW - 1 - 0]
                        - Img.RightBorder[ROW - 1 - ROW / 4]);
        int16 error_num1 = abs(
                Img.RightBorder[ROW - 1 - ROW / 2]
                        - Img.RightBorder[ROW - 1 - ROW / 4]);
        int16 error_num2 = abs(
                Img.RightBorder[ROW - 1 - ROW / 2]
                        - Img.RightBorder[ROW - 1 - ROW / 4 * 3]);
        if (abs(error_num0 - error_num1) > k_error)
            return;
        if (abs(error_num1 - error_num2) > k_error)
            return;
        if (abs(error_num0 - error_num2) > 2 * k_error)
            return;
        error_num0 = abs(
                Img.RightBorder[ROW - 1 - 0]
                        - Img.RightBorder[ROW - 1 - ROW / 20]);
        error_num1 = abs(
                Img.RightBorder[ROW - 1 - ROW / 10]
                        - Img.RightBorder[ROW - 1 - ROW / 20]);
        error_num2 = abs(
                Img.RightBorder[ROW - 1 - ROW / 10]
                        - Img.RightBorder[ROW - 1 - ROW / 20 * 3]);
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
        int sum_temp = 0;
        for (int i = 1; i < ROW / 2; i++) {
            if (Img.RightBorder[i] - Img.LeftBorder[i]
                    > Img.RightBorder[i - 1] - Img.LeftBorder[i - 1])
                sum_temp++;
        }
        if (sum_temp < 18)
            return;
        circle_flag.enter_left_circle = 1;

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
        if (!(max_py >= ROW * 0.8 && max_py1 >= 0.8))
            return;
        circle_flag.out_left_circle = 1;

    } else if (circle_flag.leave_left_circle == 0) {
        if (temp_flag == 0) {
            if (max_py1 <= ROW * 0.9 || max_py <= ROW * 0.9)
                return;
            if (right_lost_cnt >= ROW * 0.1)
                return;

            temp_flag = 1;
            //  ExSpeed_SUM = circle_speed - 100;
        } else {
            if (max_py1 >= ROW * 0.9 && max_py >= ROW * 0.9)
                return;
//            if (max_py1 >=CenterCalMaxRow_circle*2 || max_py >=CenterCalMaxRow_circle*2)    return;
            int sum_temp = 0;
            for (int i = 0; i < 10; i++)
                sum_temp += Img.LeftBorder[i];
            if (sum_temp > 10 * 20)
                return;
            int useful = 0;
            for (int i = 0; i <ROW/3; i++) {
                if (Img.LeftBorderFindFlag[i] == 'F'
                        && Img.RightBorderFindFlag[i] == 'T')
                    useful++;
            }
            if (useful < ROW/3-5)
                return;
            int block_cnt = 0;
//            for (int i = ROW/5; i < ROW/5+5; i++) {
//                for (int j = 0; j < 5; j++) {
//                    if (ImageUsed[ROW - 1 - i][j] != 0)
//                        block_cnt++;
//                }
//            }
//            if (block_cnt <= 20)
//                return;
            block_cnt = 0;

            for (int i = ROW/5+0; i <ROW/5+5; i++) {
                for (int j = COL - 1; j >= COL - 1 - 5; j--) {
                    if (ImageUsed[ROW - 1 - i][j] == 0)
                        block_cnt++;
                }
            }
            if (block_cnt <= 20)
                return;
            block_cnt=0;
            for(int i=0;i<max_py1-10;i++)
            {
                if(Img.RightBorder[i]>Img.RightBorder[i+5]) block_cnt++;
            }

            if(block_cnt<max_py1-20)return;
//            block_cnt = 0;
//
//for(int i=1;i<COL-1;i++)
//{
//    if((ImageUsed[ROW-1][i]!=ImageUsed[ROW-1][i+1])&&
//        (    ImageUsed[ROW-1][i]==ImageUsed[ROW-1][i-1])&&
//         (   ImageUsed[ROW-1][i+1]==ImageUsed[ROW-1][i+2]))block_cnt++;
//}
//if(block_cnt>=2)return;


            circle_flag.leave_left_circle = 1;


//            gpio_low(BEEP_PIN);
//                                     stop_flag=1;
//                                     ips114_show_gray_image_vec(0, 0, PerImg_ip, 114, 100, 114, 100, 0);
        }
    } else {
        if (Img.LeftBorderFindFlag[0] == 'F'
                || Img.RightBorderFindFlag[0] == 'F')
            return;
        int find_cnt_left = 0, find_cnt_right = 0, diff_cnt = 0;
        for (int i = 0; i < ROW / 3; i++) {
            if (Img.LeftBorderFindFlag[i] == 'T')
                find_cnt_left++;
            if (Img.RightBorderFindFlag[i] == 'T')
                find_cnt_right++;
            if (Img.RightBorder[i] - Img.LeftBorder[i] <= RoadWide0 + 5)
                diff_cnt++;
        }
        if (find_cnt_left < ROW / 3 - 2)
            return;
        if (find_cnt_right < ROW / 3 - 2)
            return;
        if (diff_cnt < ROW / 3 - 2)
            return;
//        int16 k_error = 3;
//        int error_num0 = abs(
//                Img.RightBorder[ROW - 1 - 0]
//                        - Img.RightBorder[ROW - 1 - ROW / 20]);
//        int error_num1 = abs(
//                Img.RightBorder[ROW - 1 - ROW / 10]
//                        - Img.RightBorder[ROW - 1 - ROW / 20]);
//        int error_num2 = abs(
//                Img.RightBorder[ROW - 1 - ROW / 10]
//                        - Img.RightBorder[ROW - 1 - ROW / 20 * 3]);
//        if (abs(error_num0 - error_num1) > k_error)
//            return;
//        if (abs(error_num1 - error_num2) > k_error)
//            return;
//        if (abs(error_num0 - error_num2) > 2 * k_error)
//            return;
//        error_num0 = abs(
//                Img.LeftBorder[ROW - 1 - 0]
//                        - Img.LeftBorder[ROW - 1 - ROW / 20]);
//        error_num1 = abs(
//                Img.LeftBorder[ROW - 1 - ROW / 10]
//                        - Img.LeftBorder[ROW - 1 - ROW / 20]);
//        error_num2 = abs(
//                Img.LeftBorder[ROW - 1 - ROW / 10]
//                        - Img.LeftBorder[ROW - 1 - ROW / 20 * 3]);
//        if (abs(error_num0 - error_num1) > k_error)
//            return;
//        if (abs(error_num1 - error_num2) > k_error)
//            return;
//        if (abs(error_num0 - error_num2) > 2 * k_error)
//            return;
        gpio_low(BEEP_PIN);
        init_flag();
        //   clear_circle_flag();
        CenterCalMinRow = CenterCalMinRow_tmp;
        CenterCalMaxRow = CenterCalMaxRow_tmp;
        ExSpeed_SUM = ExSpeed_SUM_tmp;
    //     stop_flag=1;
        //       fake_do++;
    }
}
#endif /* ELEMENT_C_ */
