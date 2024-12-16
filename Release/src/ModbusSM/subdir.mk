################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ModbusSM/ModbusSmSlave.cpp \
../src/ModbusSM/ModbusSmSlaveLinkLayer.cpp 

CPP_DEPS += \
./src/ModbusSM/ModbusSmSlave.d \
./src/ModbusSM/ModbusSmSlaveLinkLayer.d 

OBJS += \
./src/ModbusSM/ModbusSmSlave.o \
./src/ModbusSM/ModbusSmSlaveLinkLayer.o 


# Each subdirectory must supply rules for building sources it contributes
src/ModbusSM/%.o: ../src/ModbusSM/%.cpp src/ModbusSM/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\EveDisplay" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\Modbus" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\CommunicationDevice" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-ModbusSM

clean-src-2f-ModbusSM:
	-$(RM) ./src/ModbusSM/ModbusSmSlave.d ./src/ModbusSM/ModbusSmSlave.o ./src/ModbusSM/ModbusSmSlaveLinkLayer.d ./src/ModbusSM/ModbusSmSlaveLinkLayer.o

.PHONY: clean-src-2f-ModbusSM

