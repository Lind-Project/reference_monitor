################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main/platform/lind_platform.c 

OBJS += \
./src/main/platform/lind_platform.o 

C_DEPS += \
./src/main/platform/lind_platform.d 


# Each subdirectory must supply rules for building sources it contributes
src/main/platform/%.o: ../src/main/platform/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


