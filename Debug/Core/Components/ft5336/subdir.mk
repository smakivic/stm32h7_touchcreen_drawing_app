################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Components/ft5336/ft5336.c \
../Core/Components/ft5336/ft5336_reg.c 

OBJS += \
./Core/Components/ft5336/ft5336.o \
./Core/Components/ft5336/ft5336_reg.o 

C_DEPS += \
./Core/Components/ft5336/ft5336.d \
./Core/Components/ft5336/ft5336_reg.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Components/ft5336/%.o Core/Components/ft5336/%.su Core/Components/ft5336/%.cyclo: ../Core/Components/ft5336/%.c Core/Components/ft5336/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Components-2f-ft5336

clean-Core-2f-Components-2f-ft5336:
	-$(RM) ./Core/Components/ft5336/ft5336.cyclo ./Core/Components/ft5336/ft5336.d ./Core/Components/ft5336/ft5336.o ./Core/Components/ft5336/ft5336.su ./Core/Components/ft5336/ft5336_reg.cyclo ./Core/Components/ft5336/ft5336_reg.d ./Core/Components/ft5336/ft5336_reg.o ./Core/Components/ft5336/ft5336_reg.su

.PHONY: clean-Core-2f-Components-2f-ft5336

