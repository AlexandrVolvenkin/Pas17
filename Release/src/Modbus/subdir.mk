################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Modbus/ModbusMaster.cpp \
../src/Modbus/ModbusMasterLinkLayer.cpp \
../src/Modbus/ModbusRtuMasterLinkLayer.cpp \
../src/Modbus/ModbusRtuSlaveLinkLayer.cpp \
../src/Modbus/ModbusSlave.cpp \
../src/Modbus/ModbusSlaveLinkLayer.cpp \
../src/Modbus/ModbusSmMaster.cpp \
../src/Modbus/ModbusSmMasterLinkLayer.cpp \
../src/Modbus/ModbusSmSlave.cpp \
../src/Modbus/ModbusSmSlaveLinkLayer.cpp \
../src/Modbus/ModbusTcpMasterLinkLayer.cpp \
../src/Modbus/ModbusTcpSlaveLinkLayer.cpp 

CPP_DEPS += \
./src/Modbus/ModbusMaster.d \
./src/Modbus/ModbusMasterLinkLayer.d \
./src/Modbus/ModbusRtuMasterLinkLayer.d \
./src/Modbus/ModbusRtuSlaveLinkLayer.d \
./src/Modbus/ModbusSlave.d \
./src/Modbus/ModbusSlaveLinkLayer.d \
./src/Modbus/ModbusSmMaster.d \
./src/Modbus/ModbusSmMasterLinkLayer.d \
./src/Modbus/ModbusSmSlave.d \
./src/Modbus/ModbusSmSlaveLinkLayer.d \
./src/Modbus/ModbusTcpMasterLinkLayer.d \
./src/Modbus/ModbusTcpSlaveLinkLayer.d 

OBJS += \
./src/Modbus/ModbusMaster.o \
./src/Modbus/ModbusMasterLinkLayer.o \
./src/Modbus/ModbusRtuMasterLinkLayer.o \
./src/Modbus/ModbusRtuSlaveLinkLayer.o \
./src/Modbus/ModbusSlave.o \
./src/Modbus/ModbusSlaveLinkLayer.o \
./src/Modbus/ModbusSmMaster.o \
./src/Modbus/ModbusSmMasterLinkLayer.o \
./src/Modbus/ModbusSmSlave.o \
./src/Modbus/ModbusSmSlaveLinkLayer.o \
./src/Modbus/ModbusTcpMasterLinkLayer.o \
./src/Modbus/ModbusTcpSlaveLinkLayer.o 


# Each subdirectory must supply rules for building sources it contributes
src/Modbus/%.o: ../src/Modbus/%.cpp src/Modbus/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\EveDisplay" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\Modbus" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\CommunicationDevice" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-Modbus

clean-src-2f-Modbus:
	-$(RM) ./src/Modbus/ModbusMaster.d ./src/Modbus/ModbusMaster.o ./src/Modbus/ModbusMasterLinkLayer.d ./src/Modbus/ModbusMasterLinkLayer.o ./src/Modbus/ModbusRtuMasterLinkLayer.d ./src/Modbus/ModbusRtuMasterLinkLayer.o ./src/Modbus/ModbusRtuSlaveLinkLayer.d ./src/Modbus/ModbusRtuSlaveLinkLayer.o ./src/Modbus/ModbusSlave.d ./src/Modbus/ModbusSlave.o ./src/Modbus/ModbusSlaveLinkLayer.d ./src/Modbus/ModbusSlaveLinkLayer.o ./src/Modbus/ModbusSmMaster.d ./src/Modbus/ModbusSmMaster.o ./src/Modbus/ModbusSmMasterLinkLayer.d ./src/Modbus/ModbusSmMasterLinkLayer.o ./src/Modbus/ModbusSmSlave.d ./src/Modbus/ModbusSmSlave.o ./src/Modbus/ModbusSmSlaveLinkLayer.d ./src/Modbus/ModbusSmSlaveLinkLayer.o ./src/Modbus/ModbusTcpMasterLinkLayer.d ./src/Modbus/ModbusTcpMasterLinkLayer.o ./src/Modbus/ModbusTcpSlaveLinkLayer.d ./src/Modbus/ModbusTcpSlaveLinkLayer.o

.PHONY: clean-src-2f-Modbus

