################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/EveDisplay/EVE_CMD.cpp \
../src/EveDisplay/EVE_HAL.cpp \
../src/EveDisplay/Hardware.cpp \
../src/EveDisplay/USPI.cpp 

CPP_DEPS += \
./src/EveDisplay/EVE_CMD.d \
./src/EveDisplay/EVE_HAL.d \
./src/EveDisplay/Hardware.d \
./src/EveDisplay/USPI.d 

OBJS += \
./src/EveDisplay/EVE_CMD.o \
./src/EveDisplay/EVE_HAL.o \
./src/EveDisplay/Hardware.o \
./src/EveDisplay/USPI.o 


# Each subdirectory must supply rules for building sources it contributes
src/EveDisplay/%.o: ../src/EveDisplay/%.cpp src/EveDisplay/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\EveDisplay" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\CommunicationDevice" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\Modbus" -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-EveDisplay

clean-src-2f-EveDisplay:
	-$(RM) ./src/EveDisplay/EVE_CMD.d ./src/EveDisplay/EVE_CMD.o ./src/EveDisplay/EVE_HAL.d ./src/EveDisplay/EVE_HAL.o ./src/EveDisplay/Hardware.d ./src/EveDisplay/Hardware.o ./src/EveDisplay/USPI.d ./src/EveDisplay/USPI.o

.PHONY: clean-src-2f-EveDisplay

