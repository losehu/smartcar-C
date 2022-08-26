//
// Created by Administrator on 2022/4/18.
//

#include "RUBO_stack.h"

//��ȡջ��������

//��ʼ��������
void rubo_stack_init(RUBO_Stack* pst,int stack_len)
{
    pst->_a =(STDataType*)malloc(sizeof(STDataType)*stack_len);
    pst->_top = 0;
    pst->_capacity = stack_len;
}
void rubo_stack_destory(RUBO_Stack* pst)
{
//    free(pst->_a);
    pst->_a = NULL;
    pst->_top = pst->_capacity = 0;

}
void rubo_stack_clear(RUBO_Stack* pst)
{
    int tmp_len=pst->_capacity;
    free(pst->_a);
    pst->_a = NULL;
    pst->_top = pst->_capacity = 0;
  pst->_a=(STDataType*)malloc(sizeof(STDataType)*tmp_len);
    pst->_top = 0;
    pst->_capacity = tmp_len;
}
//��ջ
void rubo_stack_push(RUBO_Stack* pst, STDataType x)
{
    //�ռ䲻��������
    if (pst->_top == pst->_capacity)
    {
        pst->_capacity *= 2;//ÿ�����ݿռ�Ĵ�СΪ��һ�ε�����
        STDataType* tmp = (STDataType*)realloc(pst->_a, sizeof(STDataType) * pst->_capacity);
        if (tmp == NULL)//�ж��Ƿ��ڴ�����ɹ�
        {
            stop_flag=1;
            ExSpeed_SUM_tmp=0;

            ips114_clear(RED);
         ips114_show_string(0, 0, "no memory");
        }
        else
        {
            pst->_a = tmp;
        }
    }
    pst->_a[pst->_top] = x;
    pst->_top++;

}
//��ջ
void rubo_stack_pop(RUBO_Stack* pst)
{

    --pst->_top;
}

//��ȡ���ݵĸ���
int rubo_stack_size(RUBO_Stack* pst)
{
    return pst->_top;
}
//����1�ǿգ�����0�Ƿǿ�
int rubo_stack_empty(RUBO_Stack* pst)
{
    return pst->_top == 0 ? 1 : 0;
    //return !pst->_top;
}
//��ȡջ��������
STDataType rubo_stack_top(RUBO_Stack* pst)
{

    return pst->_a[pst->_top - 1];
}
