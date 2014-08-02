################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lind_test/testcases/test_chown.c \
../src/lind_test/testcases/test_curl.c \
../src/lind_test/testcases/test_date.c \
../src/lind_test/testcases/test_getgid.c \
../src/lind_test/testcases/test_getpid.c \
../src/lind_test/testcases/test_getuid.c \
../src/lind_test/testcases/test_mkdir.c \
../src/lind_test/testcases/test_read.c \
../src/lind_test/testcases/test_rmdir.c \
../src/lind_test/testcases/test_sleep.c 

OBJS += \
./src/lind_test/testcases/test_chown.o \
./src/lind_test/testcases/test_curl.o \
./src/lind_test/testcases/test_date.o \
./src/lind_test/testcases/test_getgid.o \
./src/lind_test/testcases/test_getpid.o \
./src/lind_test/testcases/test_getuid.o \
./src/lind_test/testcases/test_mkdir.o \
./src/lind_test/testcases/test_read.o \
./src/lind_test/testcases/test_rmdir.o \
./src/lind_test/testcases/test_sleep.o 

C_DEPS += \
./src/lind_test/testcases/test_chown.d \
./src/lind_test/testcases/test_curl.d \
./src/lind_test/testcases/test_date.d \
./src/lind_test/testcases/test_getgid.d \
./src/lind_test/testcases/test_getpid.d \
./src/lind_test/testcases/test_getuid.d \
./src/lind_test/testcases/test_mkdir.d \
./src/lind_test/testcases/test_read.d \
./src/lind_test/testcases/test_rmdir.d \
./src/lind_test/testcases/test_sleep.d 


# Each subdirectory must supply rules for building sources it contributes
src/lind_test/testcases/%.o: ../src/lind_test/testcases/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include -O3 -Wall -c -fmessage-length=0 `python-config --includes` --std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


