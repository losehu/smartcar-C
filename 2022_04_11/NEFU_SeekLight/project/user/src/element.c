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
cross cross_flag;
 stc stc_flag;

int ECL_circle=80;
/******************十字**********************/
void judge_cross() {
    if (cross_flag.find_cross == 0) {
        if (connect.room != 8)
            return;
        if (left_lost_cnt < RoadWide0 - 10 && right_lost_cnt < RoadWide0 - 10)
            return;
        int cnt = 0;
        if (block_judge(ROW - 1 - 5, ROW - 1, 0, 5, 20, 0))
            cnt++;
        if (block_judge(ROW - 1 - 5, ROW - 1, COL - 1 - 5, COL - 1, 20, 0))
            cnt++;
        if (block_judge(0, 5, 0, 5, 20, 0))
            cnt++;
        if (block_judge(0, 5, COL - 1 - 5, COL - 1, 20, 0))
            cnt++;
        if (cnt < 3)
            return;
        stop_flag = 1;
        gpio_low(BEEP_PIN);
    }

}
void clear_cross_flag() {
    cross_flag.judge = 1;
    cross_flag.find_cross = 0;
    cross_flag.on_cross = 0;
    cross_flag.out_cross = 0;
}
/****************SUM***********************/
int connect_line(int x1 ,int y1,int x2,int y2,char way)
{
    int i=0;
    float k,b;
    int y_max,y_min;
    y_max = max_ab(y1,y2);
    y_min = min_ab(y1,y2);
    k = 1.0*(y2-y1)/(x2 - x1); //直线
    b = 1.0*(((y2+y1)-k*(x2+x1)))/2;
    for(i= y_min; i<= y_max; i++)
    {
if(way=='L')Img.LeftBorder[i]=(int)(1.0*(i-b)/k);
else Img.RightBorder[i]=(int)(1.0*(i-b)/k);
    }
}
void init_flag() {
    clear_circle_flag();
    clear_fork_flag();
    clear_cross_flag();
    clear_door_flag();
    clear_cross_flag();
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
    cross_flag.judge = 0;

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
    else if (flag == 4)
        cross_flag.judge = 1;
}

void STC_read()
{
    stc_recv=gpio_get_level(STC_RECV_FLAG);
    stc_fir=gpio_get_level(STC_FIR_FLAG);
    stc_sec=gpio_get_level(STC_SEC_FLAG);

    if(stc_recv==0&&stc_fir==0&&stc_sec==0&&stop_flag==0)
    {
        ExSpeed_SUM = 0;
        ExSpeed = 0;
               ips114_clear(GREEN);
           stop_flag = 1;
           gpio_low(BEEP_PIN);
           ips114_show_string(1, 1, "ELC die!");
    }else if(stc_recv==0&&stc_fir==0&&stc_sec==1)
    {
        stc_flag.out_circle=0;

stc_flag.circle=1;
stc_flag.leave_circle=0;

    }
    else if(stc_recv==0&&stc_fir==1&&stc_sec==1)
    {
        stc_flag.circle=0;

        stc_flag.out_circle=1;
        stc_flag.leave_circle=0;

    }
    else if(stc_recv==0&&stc_fir==1&&stc_sec==0)
    {
        stc_flag.circle=0;

        stc_flag.out_circle=0;
        stc_flag.leave_circle=1;
    }
    else {
        stc_flag.out_circle=0;

        stc_flag.circle=0;
        stc_flag.leave_circle=0;

    }
    //   ips114_show_int16(0, 0,(int16) gpio_get_level(STC_RECV_FLAG));
    //   ips114_show_int16(0, 1,(int16) gpio_get_level(STC_FIR_FLAG));
    //   ips114_show_int16(0, 2,(int16) gpio_get_level(STC_SEC_FLAG));
}
void judge_unit() {
     STC_read();

  if (circle_flag.judge_left == 1 || circle_flag.judge_right)      judge_circle();
    if (fork_flag.judge == 1)     judge_fork();
    //if(Cross.judge==1)CrsosJudge();
    //  if (door_flag.judge == 1)    judge_door();
//     if(cross_flag.judge==1)judge_cross();
}
/*****************车库***********************/
void clear_door_flag() {
    door_flag.find_line = 0;
    door_flag.judge = 1;
    door_flag.find_door = 0;
}
void judge_door() {
    if (door_flag.find_line == 1 && door_flag.find_door == 0) {
        int cnt = 0;
        for (int i = 0; i < ROW / 2; i++) {
            if (Img.RightBorder[i] - Img.LeftBorder[i] > RoadWide0 * 1.5
                    && (Img.RightBorderFindFlag[i] == 'T'
                            || Img.LeftBorderFindFlag[i] == 'T'))
                cnt++;
        }
        if (cnt < RoadWide0 * 0.6) {
            clear_door_flag();
            return;
        }
        door_flag.find_door = 1;
        close_judge(3);
        ips114_clear(BROWN);
    } else if (door_flag.find_door == 1) {
        int cnt = 0;
        for (int i = 0; i < RoadWide0 * 0.8; i++) {
            if (Img.RightBorder[i] - Img.LeftBorder[i] <= RoadWide0 * 1.5
                    && (Img.RightBorderFindFlag[i] == 'T'
                            || Img.LeftBorderFindFlag[i] == 'T'))
                cnt++;
        }
        if (cnt <= RoadWide0 * 0.8 - 5)
            return;
        ips114_clear(YELLOW);
        init_flag();

    }
    return;
}
/*****************三叉***********************/
float pow_diff(bool left) {
    float error[ROW], err_cnt = 0;
    float ave = 0, sum_sum = 0;

    if (!left) {
        for (int i = 3; i < ROW; i++) {
            if (Img.LeftBorder[i] != COL - 1
                    && Img.LeftBorder[i] >= lose_error) {
                error[i - 3] = (float) abs(
                        Img.LeftBorder[i] - Img.LeftBorder[i - 3]);
                ave += error[i - 3];
                err_cnt++;
            } else {
                goto a;
            }
        }

    } else {

        for (int i = 3; i < ROW; i++) {
            if (Img.RightBorder[i] != 0
                    && Img.RightBorder[i] <= COL - 1 - lose_error) {
                error[i - 3] = (float) abs(
                        Img.RightBorder[i] - Img.RightBorder[i - 3]);
                ave += error[i - 3];
                err_cnt++;
            } else {

                goto a;
            }
        }

    }

    a:

    if (err_cnt == 0)
        return 0;
    ave = 1.0 * ave / err_cnt;
    for (int i = 0; i < err_cnt; i++) {
        sum_sum += 1.0 * (error[i] - ave) * (error[i] - ave);
    }
    sum_sum /= err_cnt;
    return sum_sum;
}
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
        if (connect.room != 6 || connect.room_cnt <= 2)
            return;
        int useful = 0;
        for (int i = 0; i < 10; i++) {
            if (Img.LeftBorderFindFlag[i] == 'T'
                    && Img.RightBorderFindFlag[i] == 'T')
                useful++;
        }
        if (Img.RightBorder[ROW / 2] - Img.LeftBorder[ROW / 2] < RoadWide0 * 2)
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
        if(Img.RightBorder[0]-Img.LeftBorder[0]>=RoadWide0*1.2) return;
        if(Img.RightBorder[2]-Img.LeftBorder[2]>=RoadWide0*1.2) return;
        if(Img.RightBorder[4]-Img.LeftBorder[4]>=RoadWide0*1.2) return;
        if(Img.RightBorder[6]-Img.LeftBorder[6]>=RoadWide0*1.2) return;
        if(Img.RightBorder[0]-Img.LeftBorder[0]<=RoadWide0*0.8) return;
             if(Img.RightBorder[2]-Img.LeftBorder[2]<=RoadWide0*0.8) return;
             if(Img.RightBorder[4]-Img.LeftBorder[4]<=RoadWide0*0.8) return;
            if(Img.RightBorder[6]-Img.LeftBorder[6]<=RoadWide0*0.8) return;
        if (pow_diff(0) <= 2 || pow_diff(1) <= 2||pow_diff(0)>=10||pow_diff(1)>=10)
            return;
        ips114_clear(BLUE);

        fork_flag.find_fork = 1;
        gpio_high(BEEP_PIN);
        CenterCalMinRow = 0;
        CenterCalMaxRow = 20;
        ExSpeed_SUM = fork_speed;
        close_judge(1);
        kp1_tmp = ServoPIDParam[0][0];
        ServoPIDParam[0][0] = 2.0;

    } else if (fork_flag.on_fork == 0) {
        if (connect.room != 4)
            return;
        int useful = 0;
        for (int i = 0; i < ROW * 0.6; i++) {
            if (Img.LeftBorderFindFlag[i] == 'T'
                    && Img.RightBorderFindFlag[i] == 'T')
                useful++;
        }
        if (useful < ROW * 0.6 - 5)
            return;
        fork_flag.on_fork = 1;
        ExSpeed_SUM = ExSpeed_SUM_tmp;
        CenterCalMinRow = 0;
        CenterCalMaxRow = 60;
        ips114_clear(PURPLE);

    } else if (fork_flag.out_fork == 0) {
        if (connect.room != 6)
            return;
//        if (!block_judge(ROW - 1 - 5, ROW - 1, 0, 5, 20, 0))
//            return;
//        if (!block_judge(ROW - 1 - 5, ROW - 1, COL - 1 - 5, COL - 1, 20, 0))
//            return;
//
//
//        int useful = 0;
//        for (int i = 0; i < 10; i++) {
//            if (Img.LeftBorderFindFlag[i] == 'T'
//                    && Img.RightBorderFindFlag[i] == 'T')
//                useful++;
//        }
        if (Img.RightBorder[ROW / 5 * 2] - Img.LeftBorder[ROW / 5 * 2]
                < RoadWide0 * 1.6)
            return;
        if (Img.RightBorder[ROW / 5 * 2 + 1] - Img.LeftBorder[ROW / 5 * 2 + 1]
                < RoadWide0 * 1.6)
            return;
        if (Img.RightBorder[ROW / 5 * 2 - 1] - Img.LeftBorder[ROW / 5 * 2 - 1]
                < RoadWide0 * 1.6)
            return;
        ExSpeed_SUM = fork_speed;
        fork_flag.out_fork = 1;
        CenterCalMinRow = 0;
        CenterCalMaxRow = 20;
        ips114_clear(GREEN);

    } else if (fork_flag.leave_fork == 0) {
        if (connect.room == 6)
            return;

        if (Img.LeftBorderFindFlag[0] == 'F'
                || Img.RightBorderFindFlag[0] == 'F')
            return;
        int find_cnt_left = 0, find_cnt_right = 0, diff_cnt = 0;
        for (int i = 0; i < ROW / 10; i++) {
            if (Img.LeftBorderFindFlag[i] == 'T')
                find_cnt_left++;
            if (Img.RightBorderFindFlag[i] == 'T')
                find_cnt_right++;
            if (Img.RightBorder[i] - Img.LeftBorder[i] <= RoadWide0 + 10)
                diff_cnt++;
        }
        if (find_cnt_left < ROW /10  - 2)
            return;
        if (find_cnt_right < ROW / 10 - 2)
            return;
//               if (diff_cnt < ROW / 4 - 2)
//                   return;

        gpio_low(BEEP_PIN);
        ips114_clear(BLACK);
        fork_flag.leave_fork = 1;
        init_flag();
        ExSpeed_SUM = ExSpeed_SUM_tmp;
        CenterCalMinRow = CenterCalMinRow_tmp;
        CenterCalMaxRow = CenterCalMaxRow_tmp;
        //     start_line=search_start_line;
        ServoPIDParam[0][0] = kp1_tmp;
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

        if (connect.room < 4)
            return;
        if(out_cnt>=10)return;
                if( stc_flag.circle==0)return;
        if(max_py<ROW*0.9||max_py1<ROW*0.9)return;

        if(right_lost_cnt-RoadWide0/2<=left_lost_cnt)return;


ips114_clear(WHITE);

        gpio_high(BEEP_PIN);
        close_judge(0);

        circle_flag.find_right_circle = 1;
        ExSpeed_SUM = circle_speed-5;
        CenterCalMinRow = CenterCalMinRow_tmp;
        CenterCalMaxRow = 30;
        kp1_tmp = ServoPIDParam[0][0];
        ServoPIDParam[0][0] =3.6;

    } else if (circle_flag.enter_right_circle == 0) {
        if(!(Base.first_right_find_to_lose_y<=ROW/5&&Base.last_first_right_find_to_lose_y<=ROW/5))return;
        circle_flag.enter_right_circle = 1;

    } else if (circle_flag.on_right_circle == 0) {
        if(connect.room>4)return;

                ServoPIDParam[0][0] =2.4;
        circle_flag.on_right_circle = 1;
    //    stop_flag=1;

    } else if (circle_flag.out_right_circle == 0) {
        if(connect.room<4||connect.room_cnt<=3)return;
        circle_flag.out_right_circle = 1;
        ExSpeed_SUM = circle_speed-50;
              // stop_flag=1;

    } else if (circle_flag.leave_right_circle == 0) {
    //    if( !(stc_flag.out_circle==1||stc_flag.circle==1||        stc_flag.leave_circle==1))return;

        if( !(stc_flag.out_circle==1||stc_flag.circle==1))return;
        circle_flag.leave_right_circle =1;
        ServoPIDParam[0][0] =3.0;

        out_cnt=180;
        ExSpeed_SUM = ExSpeed_SUM_tmp-20;

//        stop_flag=1;
    } else {

        if( out_cnt>50)return;
               init_flag();
               CenterCalMinRow = CenterCalMinRow_tmp;
               CenterCalMaxRow = CenterCalMaxRow_tmp;
               ExSpeed_SUM = ExSpeed_SUM_tmp;
               ips114_clear(BLACK);
               ServoPIDParam[0][0] = kp1_tmp;
               out_cnt=500;

    }
}
float kp1_tmp;

void judge_circle_left() {
    if (circle_flag.find_left_circle == 0) {
        temp_flag = 0;
        if (connect.room < 4)
            return;
if(out_cnt>=10)return;
        if( stc_flag.circle==0)return;
if(max_py<ROW*0.9||max_py1<ROW*0.9)return;
if(left_lost_cnt-RoadWide0/2<=right_lost_cnt)return;
ips114_clear(BROWN);
        gpio_high(BEEP_PIN);
        close_judge(-1);
       circle_flag.find_left_circle = 1;
        ExSpeed_SUM = circle_speed;
        CenterCalMinRow = CenterCalMinRow_tmp;
        CenterCalMaxRow = CenterCalMaxRow_tmp;
        cntt++;
        kp1_tmp = ServoPIDParam[0][0];
        ExSpeed_SUM =circle_speed;
        ServoPIDParam[0][0] =3.5;
    } else if (circle_flag.enter_left_circle == 0) {
if(!(Base.first_left_find_to_lose_y<=ROW/5&&Base.last_first_left_find_to_lose_y<=ROW/5))return;
   circle_flag.enter_left_circle=1;
    } else if (circle_flag.on_left_circle == 0) {
if(connect.room>4)return;

         circle_flag.on_left_circle =1;
        ExSpeed_SUM = ExSpeed_SUM_tmp+20;
        ServoPIDParam[0][0] =2.4;

    } else if (circle_flag.out_left_circle == 0) {
        if (max(max_py, max_py1 )<=  ROW * 0.8)return;
        if(connect.room<=4||connect.room_cnt<=1)return;
        circle_flag.out_left_circle =1;
        //stop_flag=1;
    } else if (circle_flag.leave_left_circle == 0) {
       if( !(stc_flag.out_circle==1||stc_flag.circle==1||        stc_flag.leave_circle==1))return;
       circle_flag.leave_left_circle =1;
       out_cnt=800;
       ExSpeed_SUM = ExSpeed_SUM_tmp-20;

    }else {
        if( out_cnt>50)return;
        init_flag();
        CenterCalMinRow = CenterCalMinRow_tmp;
        CenterCalMaxRow = CenterCalMaxRow_tmp;
        ExSpeed_SUM = ExSpeed_SUM_tmp;
        ips114_clear(BLACK);
        ServoPIDParam[0][0] = kp1_tmp;
    }
}
bool find_door = 0;
int16 black_mutation_point;
void black_judge_hang() {
    find_door = 0;
    black_mutation_point = 0;

    int16 black = 0;
    int16 white = 0;
    for (uint8 lie = 55; lie < COL; lie++) {

        if (ImageUsed[ROW - 1 - 55][lie]
                == 255/*Pixle[hang][lie]==white,此处填入的例子，我找不到像素点数组T.T*/) {
            //                    pixel_point = *(src+(nHeight-80)*nWidth+lie);
            white = 1;
        } else {
            white = 0;
        }
        if (white != black) {
            black = white;
            black_mutation_point++;
        }

        if (black_mutation_point > 10) {

        }
    }

}

#endif /* ELEMENT_C_ */
