

/*******************************************************************

 //@FileName    :main.c
 //@CreatedDate :2021��12��19��
 //@Author      :LiHao&WuSheng
 //@Description :NEFU_SeekLight|NEFU_XunGuang|����Ѱ��

*******************************************************************/

/*
            v&&                       $%z        i$8.
           ;@@@n                      @@@+       6@@o
          -@@@-        1@@u           @@@        6@@;
  -^^^^^^;@@@*^^^^^^^^^^^^^.          @@@ #@*    6@@;  n@@~
        ^@@@. i%                *@@$$#@@@$$$a&1^!@@@@^^^^^^
       n@@%   @@@u                   #@@@       @@@@@*
     .#@@i    @@@     u&&           1@@@@@#+   8@@@@@@-
    o@@@@@@@@@@@@@@@@@@@@8         ^@@@@@!@@@ 8@@$@@&@@-
         3@#  @@@ 33              +@@1@@@ 3@v8@@ 6@@;8@@~
       -@@@@! @@@  u@@#.         ^@@o @@@   @@z  6@@; 8@@@+
      6@@&.   @@@    a@@@6      u@%   @@@ 3@@    6@@;  v@@@@u
    8@@1      @@@      3@@@z    -.    @@@&8      6@@;    -
 -@@8    .^   @@@       ~@@i          @@@        6@@;
           i@@@@8                     @@@        6@@;
             *+                       o+         .-

                                            v8+
    #@@@@@@@@@@@@@@@@@@@                    1@@$
                    @@@z            ^@@z    1@@n   *@@3
     6%%%%%%%%%%%%%%@@@z              @@@6  1@@n  .@@@z
                    @@@z               @@@% 1@@n  @@#
                    @@@z               .@@. 1@@n @@u   .$z
    -^++++++++++++++@@@8        @@@@@@@@@@@@@@@@@@@@@@@@@@@*
                  @@@!                  ^@@%   @@@
 $&&&&&&&&&&&&&&&&@@@&&&@@@&            o@@v   @@@
       #@n        @@@                   @@@    @@@       *-
        #@@#      @@@                  a@@v    @@@      ~@!
         @@@.     @@@                 3@@a     @@@      ~@6
            &@@@@@@@@               1@@&       @@@      @@@8
               o@@@@             8@@#          .@@@@@@@@@@@+
*/

#include "zf_common_headfile.h"

int main(void)
{
    All_Init();
    while(1)
    {
        HMIFlagCheck();//����S1��ʾHMI
      //  WaveformWrite();
        if (mt9v03x_finish_flag_dvp == 1) {
            //ImageBuffer();
//            ControlPeriodCheck();
//            if (InControlPeriod_Flag == 'T') {//�ݲ�ʹ��
                //int32_t RunTime = systick_getvel_ms();
               AllImageDeal();
          //  ips114_displayimage032(CamImage[0],114,100);

                mt9v03x_finish_flag_dvp = 0;
//                InControlPeriod_Flag = 'F';
                //if (CarInfo.U4444444444444444pTime <= 10) RunCnt++;
//            }
      //      camera_send_image(UART_2, &CamImage, USED_ROW*USED_COL);
        }
    }
}




