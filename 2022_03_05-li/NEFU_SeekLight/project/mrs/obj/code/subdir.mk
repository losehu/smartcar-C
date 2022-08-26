################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_05-li/NEFU_SeekLight/project/code/ANO_DT.c \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_05-li/NEFU_SeekLight/project/code/All_Init.c \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_05-li/NEFU_SeekLight/project/code/Device.c \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_05-li/NEFU_SeekLight/project/code/Filter.c \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_05-li/NEFU_SeekLight/project/code/My_Flash.c 

OBJS += \
./code/ANO_DT.o \
./code/All_Init.o \
./code/Device.o \
./code/Filter.o \
./code/My_Flash.o 

C_DEPS += \
./code/ANO_DT.d \
./code/All_Init.d \
./code/Device.d \
./code/Filter.d \
./code/My_Flash.d 


# Each subdirectory must supply rules for building sources it contributes
code/ANO_DT.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_05-li/NEFU_SeekLight/project/code/ANO_DT.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\Libraries\doc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Core" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Ld" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Peripheral" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\project\user\inc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\zf_common" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\zf_device" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\project\code" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\zf_driver" -std=gnu99 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/All_Init.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_05-li/NEFU_SeekLight/project/code/All_Init.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\Libraries\doc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Core" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Ld" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Peripheral" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\project\user\inc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\zf_common" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\zf_device" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\project\code" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\zf_driver" -std=gnu99 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/Device.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_05-li/NEFU_SeekLight/project/code/Device.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\Libraries\doc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Core" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Ld" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Peripheral" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\project\user\inc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\zf_common" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\zf_device" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\project\code" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\zf_driver" -std=gnu99 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/Filter.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_05-li/NEFU_SeekLight/project/code/Filter.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\Libraries\doc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Core" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Ld" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Peripheral" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\project\user\inc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\zf_common" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\zf_device" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\project\code" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\zf_driver" -std=gnu99 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
code/My_Flash.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_05-li/NEFU_SeekLight/project/code/My_Flash.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\Libraries\doc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Core" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Ld" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Peripheral" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\project\user\inc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\zf_common" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\zf_device" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\project\code" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_05-li\NEFU_SeekLight\libraries\zf_driver" -std=gnu99 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

