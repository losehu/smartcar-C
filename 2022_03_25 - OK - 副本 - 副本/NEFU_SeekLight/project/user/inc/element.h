/*
 * element.h
 *
 *  Created on: Feb 13, 2022
 *      Author: Administrator
 */

#ifndef ELEMENT_H_
#define ELEMENT_H_
#include "zf_common_headfile.h"
/**************³µ¿â**************************/
typedef struct{


    bool judge;
    bool find_line;
    bool find_door;
    bool out_door;


}door;
extern door door_flag;
void judge_door();
void clear_door_flag();



/*****************»·µº***********************/

typedef struct{
    bool judge_left;
    bool judge_right;
    bool find_left_circle;
    bool enter_left_circle;
    bool on_left_circle;
   bool out_left_circle;
         bool leave_left_circle;
    bool find_right_circle;
     bool enter_right_circle;
     bool on_right_circle;
     bool out_right_circle;
     bool leave_right_circle;
}circle;
extern  circle circle_flag;    //Õ»
void judge_circle();
void clear_circle_flag();
void judge_circle_left() ;
void judge_circle_right() ;
/*****************Èý²æ***********************/
float pow_diff(bool left);

typedef struct{
    bool judge;
    bool find_fork;
    bool on_fork;
   bool out_fork;
         bool leave_fork ;
int cnt;
}fork;
extern  fork fork_flag;    //Õ»
void clear_fork_flag() ;
void judge_fork();
extern float kp1_tmp;

/*****************SUM***********************/
void close_judge(int flag);
void init_flag();
void judge_unit();
bool block_judge(int start_ROW,int end_ROW,int start_COL,int end_COL,int tmp,int mode);


#endif /* ELEMENT_H_ */
