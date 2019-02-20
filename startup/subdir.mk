################################################################################
# subdir Makefile
# kyChu@2019-2-20
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
./startup/startup_stm32.s 

OBJS += \
$(BuildPath)/startup/startup_stm32.o 

OBJ_DIRS = $(sort $(dir $(OBJS)))

# Each subdirectory must supply rules for building sources it contributes
$(BuildPath)/startup/%.o: ./startup/%.s | $(OBJ_DIRS)
	@echo ' AS $<'
	$(AS) -mcpu=cortex-m0 -mthumb -mfloat-abi=soft $(INCS) -g -o "$@" "$<"
