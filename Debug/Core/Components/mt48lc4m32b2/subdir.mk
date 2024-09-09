################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Components/mt48lc4m32b2/mt48lc4m32b2.c 

OBJS += \
./Core/Components/mt48lc4m32b2/mt48lc4m32b2.o 

C_DEPS += \
./Core/Components/mt48lc4m32b2/mt48lc4m32b2.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Components/mt48lc4m32b2/%.o Core/Components/mt48lc4m32b2/%.su Core/Components/mt48lc4m32b2/%.cyclo: ../Core/Components/mt48lc4m32b2/%.c Core/Components/mt48lc4m32b2/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Components-2f-mt48lc4m32b2

clean-Core-2f-Components-2f-mt48lc4m32b2:
	-$(RM) ./Core/Components/mt48lc4m32b2/mt48lc4m32b2.cyclo ./Core/Components/mt48lc4m32b2/mt48lc4m32b2.d ./Core/Components/mt48lc4m32b2/mt48lc4m32b2.o ./Core/Components/mt48lc4m32b2/mt48lc4m32b2.su

.PHONY: clean-Core-2f-Components-2f-mt48lc4m32b2

