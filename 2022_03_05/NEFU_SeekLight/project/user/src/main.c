

/*******************************************************************

 //@FileName    :main.c
 //@CreatedDate :2021年12月19日
 //@Author      :LiHao&WuSheng
 //@Description :NEFU_SeekLight|NEFU_XunGuang|东林寻光

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
    stop_flag=0;
    All_Init();
//printf("123\r\n");
  //  send_image_init();
    while(1)
    {

      //  WaveformWrite();
          ips114_show_int32(0, 3, cam_die, 5);

        protect_cam=1;
      //  CamImage[a+10+1][180]=1;
        HMIFlagCheck();//按下S1显示HMI
        if (protect_cam == 1) {
            if (mt9v03x_finish_flag_dvp == 0) {
                cam_die++;
                if (cam_die == 100) {
                    ips114_clear(RED);
                    stop_flag = 1;
                }
            }
        }

        if (mt9v03x_finish_flag_dvp == 1) {
             cam_die = 0;
             AllImageDeal();
           camera_send_image(DEBUG_UART_INDEX, &CamImage[0][0], 188*120);
          //     ips114_show_gray_image_vec(0,0,PerImg_ip,TRFED_COL,TRFED_ROW,TRFED_COL,TRFED_ROW,0);
                mt9v03x_finish_flag_dvp = 0;

        }
    }
}




