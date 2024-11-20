################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ConfigurationCreate.cpp \
../src/Crc.cpp \
../src/DataContainer.cpp \
../src/DataStore.cpp \
../src/DataStoreCheck.cpp \
../src/DeviceControl.cpp \
../src/Dfa.cpp \
../src/HammingCodes.cpp \
../src/InternalModule.cpp \
../src/InternalModuleMuvr.cpp \
../src/Link.cpp \
../src/MainProductionCycle.cpp \
../src/MessageBox.cpp \
../src/Platform.cpp \
../src/Resources.cpp \
../src/ServiceMarket.cpp \
../src/StorageDevice.cpp \
../src/Task.cpp \
../src/TaskManager.cpp \
../src/Timer.cpp \
../src/main.cpp 

CPP_DEPS += \
./src/ConfigurationCreate.d \
./src/Crc.d \
./src/DataContainer.d \
./src/DataStore.d \
./src/DataStoreCheck.d \
./src/DeviceControl.d \
./src/Dfa.d \
./src/HammingCodes.d \
./src/InternalModule.d \
./src/InternalModuleMuvr.d \
./src/Link.d \
./src/MainProductionCycle.d \
./src/MessageBox.d \
./src/Platform.d \
./src/Resources.d \
./src/ServiceMarket.d \
./src/StorageDevice.d \
./src/Task.d \
./src/TaskManager.d \
./src/Timer.d \
./src/main.d 

OBJS += \
./src/ConfigurationCreate.o \
./src/Crc.o \
./src/DataContainer.o \
./src/DataStore.o \
./src/DataStoreCheck.o \
./src/DeviceControl.o \
./src/Dfa.o \
./src/HammingCodes.o \
./src/InternalModule.o \
./src/InternalModuleMuvr.o \
./src/Link.o \
./src/MainProductionCycle.o \
./src/MessageBox.o \
./src/Platform.o \
./src/Resources.o \
./src/ServiceMarket.o \
./src/StorageDevice.o \
./src/Task.o \
./src/TaskManager.o \
./src/Timer.o \
./src/main.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\EveDisplay" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\Modbus" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\Pas17\Pas17\src\CommunicationDevice" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/ConfigurationCreate.d ./src/ConfigurationCreate.o ./src/Crc.d ./src/Crc.o ./src/DataContainer.d ./src/DataContainer.o ./src/DataStore.d ./src/DataStore.o ./src/DataStoreCheck.d ./src/DataStoreCheck.o ./src/DeviceControl.d ./src/DeviceControl.o ./src/Dfa.d ./src/Dfa.o ./src/HammingCodes.d ./src/HammingCodes.o ./src/InternalModule.d ./src/InternalModule.o ./src/InternalModuleMuvr.d ./src/InternalModuleMuvr.o ./src/Link.d ./src/Link.o ./src/MainProductionCycle.d ./src/MainProductionCycle.o ./src/MessageBox.d ./src/MessageBox.o ./src/Platform.d ./src/Platform.o ./src/Resources.d ./src/Resources.o ./src/ServiceMarket.d ./src/ServiceMarket.o ./src/StorageDevice.d ./src/StorageDevice.o ./src/Task.d ./src/Task.o ./src/TaskManager.d ./src/TaskManager.o ./src/Timer.d ./src/Timer.o ./src/main.d ./src/main.o

.PHONY: clean-src

