################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_01_22-edge\ -\ 副本/NEFU_SeekLight/libraries/sdk/Startup/system_ch32v30x.c 

S_UPPER_SRCS += \
C:/Users/Administrator/Desktop/github/smartcar-4/2022_01_22-edge\ -\ 副本/NEFU_SeekLight/libraries/sdk/Startup/startup_ch32v30x.S 

OBJS += \
./sdk/Startup/startup_ch32v30x.o \
./sdk/Startup/system_ch32v30x.o 

S_UPPER_DEPS += \
./sdk/Startup/startup_ch32v30x.d 

C_DEPS += \
./sdk/Startup/system_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
sdk/Startup/startup_ch32v30x.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_01_22-edge\ -\ 副本/NEFU_SeekLight/libraries/sdk/Startup/startup_ch32v30x.S
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -x assembler -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_01_22-edge - 副本\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_01_22-edge - 副本\NEFU_SeekLight\libraries\zf_driver" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
sdk/Startup/system_ch32v30x.o: C:/Users/Administrator/Desktop/github/smartcar-4/2022_01_22-edge\ -\ 副本/NEFU_SeekLight/libraries/sdk/Startup/system_ch32v30x.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall  -g -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_01_22-edge - 副本\NEFU_SeekLight\Libraries\doc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_01_22-edge - 副本\NEFU_SeekLight\libraries\sdk\Core" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_01_22-edge - 副本\NEFU_SeekLight\libraries\sdk\Ld" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_01_22-edge - 副本\NEFU_SeekLight\libraries\sdk\Peripheral" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_01_22-edge - 副本\NEFU_SeekLight\libraries\sdk\Startup" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_01_22-edge - 副本\NEFU_SeekLight\project\user\inc" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_01_22-edge - 副本\NEFU_SeekLight\libraries\zf_common" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_01_22-edge - 副本\NEFU_SeekLight\libraries\zf_device" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_01_22-edge - 副本\NEFU_SeekLight\project\code" -I"C:\Users\Administrator\Desktop\github\smartcar-4\2022_01_22-edge - 副本\NEFU_SeekLight\libraries\zf_driver" -std=gnu99 -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

