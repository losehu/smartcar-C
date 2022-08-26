################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_08/NEFU_SeekLight/libraries/sdk/Core/core_riscv.c 

OBJS += \
./sdk/Core/core_riscv.o 

C_DEPS += \
./sdk/Core/core_riscv.d 


# Each subdirectory must supply rules for building sources it contributes
sdk/Core/core_riscv.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_03_08/NEFU_SeekLight/libraries/sdk/Core/core_riscv.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_08\NEFU_SeekLight\Libraries\doc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_08\NEFU_SeekLight\libraries\sdk\Core" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_08\NEFU_SeekLight\libraries\sdk\Ld" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_08\NEFU_SeekLight\libraries\sdk\Peripheral" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_08\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_08\NEFU_SeekLight\project\user\inc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_08\NEFU_SeekLight\libraries\zf_common" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_08\NEFU_SeekLight\libraries\zf_device" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_08\NEFU_SeekLight\project\code" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_03_08\NEFU_SeekLight\libraries\zf_driver" -std=gnu99 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

