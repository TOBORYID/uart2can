################################################################################
# subdir Makefile
# kyChu@2019-2-20
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
./StdPeriph_Driver/src/stm32f0xx_adc.c \
./StdPeriph_Driver/src/stm32f0xx_can.c \
./StdPeriph_Driver/src/stm32f0xx_cec.c \
./StdPeriph_Driver/src/stm32f0xx_comp.c \
./StdPeriph_Driver/src/stm32f0xx_crc.c \
./StdPeriph_Driver/src/stm32f0xx_crs.c \
./StdPeriph_Driver/src/stm32f0xx_dac.c \
./StdPeriph_Driver/src/stm32f0xx_dbgmcu.c \
./StdPeriph_Driver/src/stm32f0xx_dma.c \
./StdPeriph_Driver/src/stm32f0xx_exti.c \
./StdPeriph_Driver/src/stm32f0xx_flash.c \
./StdPeriph_Driver/src/stm32f0xx_gpio.c \
./StdPeriph_Driver/src/stm32f0xx_i2c.c \
./StdPeriph_Driver/src/stm32f0xx_iwdg.c \
./StdPeriph_Driver/src/stm32f0xx_misc.c \
./StdPeriph_Driver/src/stm32f0xx_pwr.c \
./StdPeriph_Driver/src/stm32f0xx_rcc.c \
./StdPeriph_Driver/src/stm32f0xx_rtc.c \
./StdPeriph_Driver/src/stm32f0xx_spi.c \
./StdPeriph_Driver/src/stm32f0xx_syscfg.c \
./StdPeriph_Driver/src/stm32f0xx_tim.c \
./StdPeriph_Driver/src/stm32f0xx_usart.c \
./StdPeriph_Driver/src/stm32f0xx_wwdg.c 

OBJS += \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_adc.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_can.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_cec.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_comp.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_crc.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_crs.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_dac.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_dbgmcu.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_dma.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_exti.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_flash.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_gpio.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_i2c.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_iwdg.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_misc.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_pwr.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_rcc.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_rtc.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_spi.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_syscfg.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_tim.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_usart.o \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_wwdg.o 

C_DEPS += \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_adc.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_can.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_cec.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_comp.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_crc.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_crs.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_dac.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_dbgmcu.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_dma.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_exti.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_flash.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_gpio.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_i2c.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_iwdg.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_misc.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_pwr.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_rcc.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_rtc.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_spi.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_syscfg.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_tim.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_usart.d \
$(BuildPath)/StdPeriph_Driver/src/stm32f0xx_wwdg.d 

OBJ_DIRS = $(sort $(dir $(OBJS)))

# Each subdirectory must supply rules for building sources it contributes
$(BuildPath)/StdPeriph_Driver/src/%.o: ./StdPeriph_Driver/src/%.c | $(OBJ_DIRS)
	@echo ' CC $<'
	$(CC) -mcpu=cortex-m0 -mthumb -mfloat-abi=soft $(DEFS) $(INCS) -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
