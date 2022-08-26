/**
 //@FileName	:ANO_DT.c
 //@CreatedDate	:2021年12月19日
 //@Author		:LiHao
 //@Description	:
**/

#include "ANO_DT.h"

//需要设置
#define   WaveformNum   10        //有几路波形 会发送前x路波形

short  Waveform[20];         //波形缓存
//unsigned char data_to_receive[100]; //接收的缓存
unsigned char DataToSend[100]; //发送数据缓存
unsigned char Waveform_Flag = 'T';//中断里将此标志位定时置'T'

/**
*@Name			:WaveformWrite
*@Description 	:WaveformWrite//数据类型int16//主循环里直接调用
*@Param			:None
*@Return		:None
*@Sample		:WaveformWrite();
**/
void WaveformWrite(void)
{
    Waveform[USERDATA_1]   = (int16)(Wheel_L.SpeedNow);
    Waveform[USERDATA_2]   = (int16)(Wheel_R.SpeedNow);
    Waveform[USERDATA_3]   = (int16)(Wheel.SpeedNow);
    Waveform[USERDATA_4]   = pwm_l;//(int16)(ICM_Treated.accdata.x*100);
    Waveform[USERDATA_5]   = pwm_r;//(int16)(ICM_Treated.accdata.y*100);
    Waveform[USERDATA_6]   = InControlPeriod_Flag;//Circle.LeftUpInflexion.x;
    Waveform[USERDATA_7]   = Circle.LeftUpInflexion.y;
    Waveform[USERDATA_8]   = point_center;
    Waveform[USERDATA_9]   = ServoAdd;
    Waveform[USERDATA_10]  = Base.Topline;
    //userdate11:CircleState
    if(Circle.state == CircleOut)               Waveform[USERDATA_11]  = 0;
    else if(Circle.state == CircleFound)        Waveform[USERDATA_11]  = 10;
    else if(Circle.state == CircleInStraight)   Waveform[USERDATA_11]  = 20;
    else if(Circle.state == CircleCenter)       Waveform[USERDATA_11]  = 30;
    else if(Circle.state == CircleCouldInTurn)  Waveform[USERDATA_11]  = 40;
    else if(Circle.state == CircleInTurn)       Waveform[USERDATA_11]  = 50;
    else if(Circle.state == CircleIn)           Waveform[USERDATA_11]  = 60;
    else if(Circle.state == CircleOutTurn)      Waveform[USERDATA_11]  = 70;
    else if(Circle.state == CircleOutTurnOK)    Waveform[USERDATA_11]  = 80;
    else if(Circle.state == CircleOutCenter)    Waveform[USERDATA_11]  = 90;
    else if(Circle.state == CircleOutStraight)  Waveform[USERDATA_11]  = 100;
    Waveform[USERDATA_12]  = 0;
    Waveform[USERDATA_13]  = 0;
    Waveform[USERDATA_14]  = 0;
    Waveform[USERDATA_15]  = 0;
    Waveform[USERDATA_16]  = 0;
    Waveform[USERDATA_17]  = 0;
    Waveform[USERDATA_18]  = 0;
    Waveform[USERDATA_19]  = 0;
    Waveform[USERDATA_20]  = 0;
    //send
    SendWaveform();
}


/**
 * @editior: zyg
 * @brief  : 调用此函数,则向上位机发送20路波形,每一路波形均为short型变量
 * @param  : none
 * @return : NULL
 * @example: Send_Waveform();
 */
void SendWaveform(void)
{
    unsigned char _cnt = 0;
    if(Waveform_Flag == 'T'||1)
    {
        DataToSend[_cnt++]=0xAA;    //头字节
        DataToSend[_cnt++]=MYHWADDR;//器件地址
        DataToSend[_cnt++]=0xF1;    //数据波形发送指令,只发送F1这一帧
        DataToSend[_cnt++]=WaveformNum * 2;   //有效数据字节数
        //发送数据波形数组
        for(unsigned char i = 0;i <WaveformNum;i ++)
        {
            DataToSend[_cnt++]=BYTE0(Waveform[i]);
            DataToSend[_cnt++]=BYTE1(Waveform[i]);
            // DataToSend[_cnt++]=BYTE2(Waveform[i]);
            // DataToSend[_cnt++]=BYTE3(Waveform[i]);
        }
        unsigned char Sum_check = 0;//和校验
        unsigned char Additional_check = 0;//附加校验
        for(unsigned char i=0;i<DataToSend[3] + 4;i++)
        {
            Sum_check += DataToSend[i];
            Additional_check += Sum_check;
        }
        DataToSend[_cnt++]=Sum_check;
        DataToSend[_cnt++]=Additional_check;
        //串口发送数据 ///////////////////写在这里
        wireless_uart_send_buff(&DataToSend[0], _cnt);
        Waveform_Flag = 'F';
    }
}
unsigned char img_head[50] ;
int error_cnt=0;
unsigned char img_bin[1500];

void img_send()
{
memset(img_bin,0,sizeof(img_bin));
img_bin[0]='9',img_bin[1]='9',img_bin[2]='9';
unsigned int cnt_sum = 3, cnt_tmp = 0;
for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 114; j++) {
        if ((*PerImg_ip[i][j]) == 0)
       //  if(cnt_tmp%2==0)
             img_bin[cnt_sum] = img_bin[cnt_sum] << 1;

         else img_bin[cnt_sum] = img_bin[cnt_sum] << 1 | 1;
        cnt_tmp++;
        if (cnt_tmp == 8)
            cnt_sum++, cnt_tmp = 0;
    }
}
img_bin[1428]='9',img_bin[1429]='9',img_bin[1430]='9';
//printf("image:1,1425,114,100,2\n");
uart_write_buffer(WRIELESS_UART_INDEX, img_bin, 100 * 114 / 8+3+3);   // 发送最后的数据
//printf("%s\n",img_bin);
}
