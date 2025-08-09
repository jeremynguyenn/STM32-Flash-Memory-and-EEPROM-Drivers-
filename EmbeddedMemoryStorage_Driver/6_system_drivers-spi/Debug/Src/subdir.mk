################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc.c \
../Src/bsp.c \
../Src/fpu.c \
../Src/main.c \
../Src/spi.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/timebase.c \
../Src/uart.c 

OBJS += \
./Src/adc.o \
./Src/bsp.o \
./Src/fpu.o \
./Src/main.o \
./Src/spi.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/timebase.o \
./Src/uart.o 

C_DEPS += \
./Src/adc.d \
./Src/bsp.d \
./Src/fpu.d \
./Src/main.d \
./Src/spi.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/timebase.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/adc.o: ../Src/adc.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/adc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/bsp.o: ../Src/bsp.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/bsp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/fpu.o: ../Src/fpu.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/fpu.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/main.o: ../Src/main.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/spi.o: ../Src/spi.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/spi.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/timebase.o: ../Src/timebase.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/timebase.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/uart.o: ../Src/uart.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Gaming Laptop RTX/Documents/EmbeddedMemoryStorage/6_system_drivers-spi/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/uart.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

