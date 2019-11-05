################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/main.c" \
"../Sources/task.c" \
"../Sources/usercode.c" \

C_SRCS += \
../Sources/main.c \
../Sources/task.c \
../Sources/usercode.c \

OBJS_OS_FORMAT += \
./Sources/main.o \
./Sources/task.o \
./Sources/usercode.o \

C_DEPS_QUOTED += \
"./Sources/main.d" \
"./Sources/task.d" \
"./Sources/usercode.d" \

OBJS += \
./Sources/main.o \
./Sources/task.o \
./Sources/usercode.o \

OBJS_QUOTED += \
"./Sources/main.o" \
"./Sources/task.o" \
"./Sources/usercode.o" \

C_DEPS += \
./Sources/main.d \
./Sources/task.d \
./Sources/usercode.d \


# Each subdirectory must supply rules for building sources it contributes
Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #37 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/main.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/task.o: ../Sources/task.c
	@echo 'Building file: $<'
	@echo 'Executing target #38 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/task.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/task.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/usercode.o: ../Sources/usercode.c
	@echo 'Building file: $<'
	@echo 'Executing target #39 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/usercode.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/usercode.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


