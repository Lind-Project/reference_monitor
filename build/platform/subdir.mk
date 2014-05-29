################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../platform/lind_platform.c 

OBJS += \
./platform/lind_platform.o 

C_DEPS += \
./platform/lind_platform.d 


# Each subdirectory must supply rules for building sources it contributes
platform/%.o: ../platform/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 `python-config --includes` --std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


