################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CommunicationDevice/CommunicationDevice.cpp \
../src/CommunicationDevice/SerialPortCommunicationDevice.cpp \
../src/CommunicationDevice/TcpCommunicationDevice.cpp 

CPP_DEPS += \
./src/CommunicationDevice/CommunicationDevice.d \
./src/CommunicationDevice/SerialPortCommunicationDevice.d \
./src/CommunicationDevice/TcpCommunicationDevice.d 

OBJS += \
./src/CommunicationDevice/CommunicationDevice.o \
./src/CommunicationDevice/SerialPortCommunicationDevice.o \
./src/CommunicationDevice/TcpCommunicationDevice.o 


# Each subdirectory must supply rules for building sources it contributes
src/CommunicationDevice/%.o: ../src/CommunicationDevice/%.cpp src/CommunicationDevice/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\ModbusSM" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\EveDisplay" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\CommunicationDevice" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\Modbus" -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-CommunicationDevice

clean-src-2f-CommunicationDevice:
	-$(RM) ./src/CommunicationDevice/CommunicationDevice.d ./src/CommunicationDevice/CommunicationDevice.o ./src/CommunicationDevice/SerialPortCommunicationDevice.d ./src/CommunicationDevice/SerialPortCommunicationDevice.o ./src/CommunicationDevice/TcpCommunicationDevice.d ./src/CommunicationDevice/TcpCommunicationDevice.o

.PHONY: clean-src-2f-CommunicationDevice

