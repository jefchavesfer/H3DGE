################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/colorbar.c \
../src/platform.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/colorbar.o \
./src/platform.o 

C_DEPS += \
./src/colorbar.d \
./src/platform.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: PowerPC gcc compiler
	powerpc-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../h3dge_bsp/ppc440_0/include -mcpu=440 -mfpu=dp_full -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


