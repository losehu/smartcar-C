################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/cam.c \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/control.c \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/display.c \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/elc.c \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/element.c \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/image_deal.c \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/isr.c \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/main.c \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/pid.c 

OBJS += \
./user_c/cam.o \
./user_c/control.o \
./user_c/display.o \
./user_c/elc.o \
./user_c/element.o \
./user_c/image_deal.o \
./user_c/isr.o \
./user_c/main.o \
./user_c/pid.o 

C_DEPS += \
./user_c/cam.d \
./user_c/control.d \
./user_c/display.d \
./user_c/elc.d \
./user_c/element.d \
./user_c/image_deal.d \
./user_c/isr.d \
./user_c/main.d \
./user_c/pid.d 


# Each subdirectory must supply rules for building sources it contributes
user_c/cam.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/cam.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\Libraries\doc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Core" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Ld" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Peripheral" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\user\inc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_common" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_device" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\code" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_driver" -std=gnu99 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
user_c/control.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/control.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\Libraries\doc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Core" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Ld" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Peripheral" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\user\inc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_common" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_device" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\code" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_driver" -std=gnu99 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
user_c/display.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/display.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\Libraries\doc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Core" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Ld" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Peripheral" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\user\inc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_common" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_device" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\code" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_driver" -std=gnu99 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
user_c/elc.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/elc.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\Libraries\doc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Core" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Ld" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Peripheral" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\user\inc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_common" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_device" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\code" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_driver" -std=gnu99 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
user_c/element.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/element.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\Libraries\doc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Core" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Ld" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Peripheral" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\user\inc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_common" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_device" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\code" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_driver" -std=gnu99 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
user_c/image_deal.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/image_deal.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\Libraries\doc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Core" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Ld" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Peripheral" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\user\inc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_common" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_device" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\code" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_driver" -std=gnu99 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
user_c/isr.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/isr.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\Libraries\doc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Core" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Ld" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Peripheral" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\user\inc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_common" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_device" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\code" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_driver" -std=gnu99 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
user_c/main.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/main.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\Libraries\doc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Core" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Ld" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Peripheral" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\user\inc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_common" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_device" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\code" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_driver" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"user_c/main.d" -c -o "$@" "$<"
	@	@
user_c/pid.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_17-上交/NEFU_SeekLight/project/user/src/pid.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\Libraries\doc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Core" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Ld" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Peripheral" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\user\inc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_common" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_device" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\project\code" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_17-上交\NEFU_SeekLight\libraries\zf_driver" -std=gnu99 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

