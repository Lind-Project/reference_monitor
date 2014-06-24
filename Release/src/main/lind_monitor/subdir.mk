################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main/lind_monitor/lind_monitor.c \
../src/main/lind_monitor/lind_util.c 

OBJS += \
./src/main/lind_monitor/lind_monitor.o \
./src/main/lind_monitor/lind_util.o 

C_DEPS += \
./src/main/lind_monitor/lind_monitor.d \
./src/main/lind_monitor/lind_util.d 


# Each subdirectory must supply rules for building sources it contributes
src/main/lind_monitor/%.o: ../src/main/lind_monitor/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include -O3 -Wall -c -fmessage-length=0 `python-config --includes` --std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


