################################################################################
# subdir Makefile
# kyChu@2019-2-20
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
./driver/device/CRC16.c \
./driver/device/LED.c \
./driver/device/TOF.c \
./driver/device/UltraSonic.c

OBJS += \
$(BuildPath)/driver/device/CRC16.o \
$(BuildPath)/driver/device/LED.o \
$(BuildPath)/driver/device/TOF.o \
$(BuildPath)/driver/device/UltraSonic.o

C_DEPS += \
$(BuildPath)/driver/device/CRC16.d \
$(BuildPath)/driver/device/LED.d \
$(BuildPath)/driver/device/TOF.d \
$(BuildPath)/driver/device/UltraSonic.d

OBJ_DIRS = $(sort $(dir $(OBJS)))

# Each subdirectory must supply rules for building sources it contributes
$(BuildPath)/driver/device/%.o: ./driver/device/%.c | $(OBJ_DIRS)
	@echo ' CC $<'
	$(CC) -mcpu=cortex-m0 -mthumb -mfloat-abi=soft $(DEFS) $(INCS) $(CFGS) -Os $(DBGS) -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
