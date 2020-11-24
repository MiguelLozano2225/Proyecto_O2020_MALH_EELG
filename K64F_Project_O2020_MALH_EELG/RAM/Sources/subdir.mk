################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/MFRC522.c" \
"../Sources/NVIC.c" \
"../Sources/SPI.c" \
"../Sources/UART.c" \
"../Sources/clocks_and_pin.c" \
"../Sources/i2c.c" \
"../Sources/main.c" \
"../Sources/pcf8574.c" \

C_SRCS += \
../Sources/MFRC522.c \
../Sources/NVIC.c \
../Sources/SPI.c \
../Sources/UART.c \
../Sources/clocks_and_pin.c \
../Sources/i2c.c \
../Sources/main.c \
../Sources/pcf8574.c \

OBJS += \
./Sources/MFRC522.o \
./Sources/NVIC.o \
./Sources/SPI.o \
./Sources/UART.o \
./Sources/clocks_and_pin.o \
./Sources/i2c.o \
./Sources/main.o \
./Sources/pcf8574.o \

C_DEPS += \
./Sources/MFRC522.d \
./Sources/NVIC.d \
./Sources/SPI.d \
./Sources/UART.d \
./Sources/clocks_and_pin.d \
./Sources/i2c.d \
./Sources/main.d \
./Sources/pcf8574.d \

OBJS_QUOTED += \
"./Sources/MFRC522.o" \
"./Sources/NVIC.o" \
"./Sources/SPI.o" \
"./Sources/UART.o" \
"./Sources/clocks_and_pin.o" \
"./Sources/i2c.o" \
"./Sources/main.o" \
"./Sources/pcf8574.o" \

C_DEPS_QUOTED += \
"./Sources/MFRC522.d" \
"./Sources/NVIC.d" \
"./Sources/SPI.d" \
"./Sources/UART.d" \
"./Sources/clocks_and_pin.d" \
"./Sources/i2c.d" \
"./Sources/main.d" \
"./Sources/pcf8574.d" \

OBJS_OS_FORMAT += \
./Sources/MFRC522.o \
./Sources/NVIC.o \
./Sources/SPI.o \
./Sources/UART.o \
./Sources/clocks_and_pin.o \
./Sources/i2c.o \
./Sources/main.o \
./Sources/pcf8574.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/MFRC522.o: ../Sources/MFRC522.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/MFRC522.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/MFRC522.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/NVIC.o: ../Sources/NVIC.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/NVIC.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/NVIC.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/SPI.o: ../Sources/SPI.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/SPI.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/SPI.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/UART.o: ../Sources/UART.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/UART.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/UART.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/clocks_and_pin.o: ../Sources/clocks_and_pin.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/clocks_and_pin.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/clocks_and_pin.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/i2c.o: ../Sources/i2c.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/i2c.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/i2c.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/pcf8574.o: ../Sources/pcf8574.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/pcf8574.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/pcf8574.o"
	@echo 'Finished building: $<'
	@echo ' '


