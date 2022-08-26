//
// Created by Administrator on 2022/4/18.
//

#ifndef MAIN_CPP_RUBO_STACK_H
#define MAIN_CPP_RUBO_STACK_H
#include "zf_common_headfile.h"

typedef int STDataType;
typedef struct Stack
{
    STDataType* _a;
    int _top;//栈顶下标
    int _capacity;//栈的容量
}RUBO_Stack;

//初始化和销毁
void rubo_stack_init(RUBO_Stack* pst,int stack_len);
void rubo_stack_destory(RUBO_Stack* pst);

//入栈
void rubo_stack_push(RUBO_Stack* pst,STDataType x);

//出栈
void rubo_stack_pop(RUBO_Stack* pst);

//获取数据的个数
int rubo_stack_size(RUBO_Stack* pst);

//返回1是空，返回0是非空
int rubo_stack_empty(RUBO_Stack* pst);
void rubo_stack_clear(RUBO_Stack* pst);
STDataType rubo_stack_top(RUBO_Stack* pst);

#endif //MAIN_CPP_RUBO_STACK_H
