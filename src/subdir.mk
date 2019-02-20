################################################################################
# subdir Makefile
# kyChu@2019-2-20
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
./src/CAN.c \
./src/CRC16.c \
./src/Delay.c \
./src/LED.c \
./src/Maths.c \
./src/TOF.c \
./src/TimerCounter.c \
./src/UltraSonic.c \
./src/main.c \
./src/stm32f0xx_it.c \
./src/syscalls.c \
./src/system_stm32f0xx.c \
./src/uart1.c \
./src/uart2.c 

OBJS += \
$(BuildPath)/src/CAN.o \
$(BuildPath)/src/CRC16.o \
$(BuildPath)/src/Delay.o \
$(BuildPath)/src/LED.o \
$(BuildPath)/src/Maths.o \
$(BuildPath)/src/TOF.o \
$(BuildPath)/src/TimerCounter.o \
$(BuildPath)/src/UltraSonic.o \
$(BuildPath)/src/main.o \
$(BuildPath)/src/stm32f0xx_it.o \
$(BuildPath)/src/syscalls.o \
$(BuildPath)/src/system_stm32f0xx.o \
$(BuildPath)/src/uart1.o \
$(BuildPath)/src/uart2.o 

C_DEPS += \
$(BuildPath)/src/CAN.d \
$(BuildPath)/src/CRC16.d \
$(BuildPath)/src/Delay.d \
$(BuildPath)/src/LED.d \
$(BuildPath)/src/Maths.d \
$(BuildPath)/src/TOF.d \
$(BuildPath)/src/TimerCounter.d \
$(BuildPath)/src/UltraSonic.d \
$(BuildPath)/src/main.d \
$(BuildPath)/src/stm32f0xx_it.d \
$(BuildPath)/src/syscalls.d \
$(BuildPath)/src/system_stm32f0xx.d \
$(BuildPath)/src/uart1.d \
$(BuildPath)/src/uart2.d 

OBJ_DIRS = $(sort $(dir $(OBJS)))

# Each subdirectory must supply rules for building sources it contributes
$(BuildPath)/src/%.o: ./src/%.c | $(OBJ_DIRS)
	@echo ' CC $<'
	$(CC) -mcpu=cortex-m0 -mthumb -mfloat-abi=soft $(DEFS) $(INCS) -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
