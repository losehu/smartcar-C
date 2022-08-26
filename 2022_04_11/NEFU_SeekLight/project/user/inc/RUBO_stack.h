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
    int _top;//ջ���±�
    int _capacity;//ջ������
}RUBO_Stack;

//��ʼ��������
void rubo_stack_init(RUBO_Stack* pst,int stack_len);
void rubo_stack_destory(RUBO_Stack* pst);

//��ջ
void rubo_stack_push(RUBO_Stack* pst,STDataType x);

//��ջ
void rubo_stack_pop(RUBO_Stack* pst);

//��ȡ���ݵĸ���
int rubo_stack_size(RUBO_Stack* pst);

//����1�ǿգ�����0�Ƿǿ�
int rubo_stack_empty(RUBO_Stack* pst);
void rubo_stack_clear(RUBO_Stack* pst);
STDataType rubo_stack_top(RUBO_Stack* pst);

#endif //MAIN_CPP_RUBO_STACK_H
