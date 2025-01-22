################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SerialMT/SerialMtMaster.cpp \
../src/SerialMT/SerialMtMasterLinkLayer.cpp \
../src/SerialMT/SerialMtSlave.cpp \
../src/SerialMT/SerialMtSlaveLinkLayer.cpp \
../src/SerialMT/SerialMtSpiMasterLinkLayer.cpp \
../src/SerialMT/SerialMtSpiSlaveLinkLayer.cpp 

CPP_DEPS += \
./src/SerialMT/SerialMtMaster.d \
./src/SerialMT/SerialMtMasterLinkLayer.d \
./src/SerialMT/SerialMtSlave.d \
./src/SerialMT/SerialMtSlaveLinkLayer.d \
./src/SerialMT/SerialMtSpiMasterLinkLayer.d \
./src/SerialMT/SerialMtSpiSlaveLinkLayer.d 

OBJS += \
./src/SerialMT/SerialMtMaster.o \
./src/SerialMT/SerialMtMasterLinkLayer.o \
./src/SerialMT/SerialMtSlave.o \
./src/SerialMT/SerialMtSlaveLinkLayer.o \
./src/SerialMT/SerialMtSpiMasterLinkLayer.o \
./src/SerialMT/SerialMtSpiSlaveLinkLayer.o 


# Each subdirectory must supply rules for building sources it contributes
src/SerialMT/%.o: ../src/SerialMT/%.cpp src/SerialMT/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\Modbus" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\CommunicationDevice" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-SerialMT

clean-src-2f-SerialMT:
	-$(RM) ./src/SerialMT/SerialMtMaster.d ./src/SerialMT/SerialMtMaster.o ./src/SerialMT/SerialMtMasterLinkLayer.d ./src/SerialMT/SerialMtMasterLinkLayer.o ./src/SerialMT/SerialMtSlave.d ./src/SerialMT/SerialMtSlave.o ./src/SerialMT/SerialMtSlaveLinkLayer.d ./src/SerialMT/SerialMtSlaveLinkLayer.o ./src/SerialMT/SerialMtSpiMasterLinkLayer.d ./src/SerialMT/SerialMtSpiMasterLinkLayer.o ./src/SerialMT/SerialMtSpiSlaveLinkLayer.d ./src/SerialMT/SerialMtSpiSlaveLinkLayer.o

.PHONY: clean-src-2f-SerialMT

