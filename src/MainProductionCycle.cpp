//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <iostream>
#include <string.h>

#include "Task.h"
#include "Platform.h"
#include "Gpio.h"
#include "CommunicationDevice.h"
#include "SpiCommunicationDevice.h"
#include "SerialPortCommunicationDevice.h"
#include "TcpCommunicationDevice.h"
#include "SharedMemoryCommunicationDevice.h"
#include "Resources.h"
#include "TaskManager.h"
#include "ServiceMarket.h"
#include "StorageDevice.h"
#include "DataStore.h"
#include "DataStoreCheck.h"
#include "InternalModule.h"
#include "InternalModuleMuvr.h"
#include "ConfigurationCreate.h"
#include "DataBaseCreate.h"
//#include "ModbusTcp.h"
#include "ModbusSlaveLinkLayer.h"
#include "ModbusTcpSlaveLinkLayer.h"
#include "Link.h"
#include "DataContainer.h"
#include "AnalogueSignals.h"
#include "SystemComponentsCreate.h"
#include "ConfigurationCheck.h"
#include "SettingsLoad.h"
#include "SettingsSet.h"
#include "DiscreteSignals.h"
#include "AnalogueSignalsArchiveCreate.h"
#include "StorageDevice.h"
#include "Events.h"
#include "EventsDB.h"

#include "MainProductionCycle.h"

//class CMainThreadProduction;

using namespace std;

uint8_t auiTempBlock[]
{
    0, 1, 2, 3, 4, 5, 6, 7,
    8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

//CSpi xSpiCommunicationDevice;
CArchiveEventsDB xCArchiveEventsDB;


//-------------------------------------------------------------------------------
CMainProductionCycle::CMainProductionCycle()
{
    std::cout << "CMainProductionCycle constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[1024];
    SetResources(&m_xResources);
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CMainProductionCycle::~CMainProductionCycle()
{
    std::cout << "CMainProductionCycle destructor"  << std::endl;
    delete m_pxLedBlinker;
//    delete m_pxFileDescriptorEventsWaitingProduction;

    m_xResources.ModbusWorkingArraysDestroy();

    delete m_puiCoils;
    delete m_puiDiscreteInputs;
    delete m_puiHoldingRegisters;
    delete m_puiInputRegisters;

    //CGpio::Close();
    delete m_pxSpiCommunicationDevice;
    delete m_pxInternalModule;
    delete m_pxInternalModuleMuvr;
    delete m_pxConfigurationCreate;

//    delete m_pxModusTcpSlaveTopLevelProduction;
    delete m_pxModbusTcpSlaveLinkLayerUpperLevel;
    delete m_pxModbusTcpSlaveUpperLevel;

//    delete m_pxModusRtuSlaveTopLevelProduction;
    delete m_pxModbusRtuSlaveLinkLayerUpperLevel;
    delete m_pxModbusRtuSlaveUpperLevel;

    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
uint8_t CMainProductionCycle::Init(void)
{
    std::cout << "CMainProductionCycle Init"  << std::endl;
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));

    SetCustomerDataContainer(GetExecutorDataContainerPointer());
//    SetCurrentCustomerDataContainer(GetExecutorDataContainerPointer());

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

    Allocate();
}

////-------------------------------------------------------------------------------
//bool CMainProductionCycle::SetTaskData(CDataContainerDataBase* pxDataContainer)
//{
//    std::cout << "CMainProductionCycle::SetTaskData 1" << std::endl;
//
//    if (IsTaskReady())
//    {
//        std::cout << "CMainProductionCycle::SetTaskData 2" << std::endl;
//        *m_pxOperatingDataContainer = *pxDataContainer;
//        SetFsmState(m_pxOperatingDataContainer -> m_uiFsmCommandState);
//        return true;
//    }
//    else
//    {
//        std::cout << "CMainProductionCycle::SetTaskData 3" << std::endl;
//        return false;
//    }
//}
//
////-------------------------------------------------------------------------------
//bool CMainProductionCycle::GetTaskData(CDataContainerDataBase* pxDataContainer)
//{
//    std::cout << "CMainProductionCycle::GetTaskData 1" << std::endl;
//
//    m_pxOperatingDataContainer -> m_uiFsmCommandState = GetFsmState();
//    *pxDataContainer = *m_pxOperatingDataContainer;
//
//    return true;
//}

//-------------------------------------------------------------------------------
void CMainProductionCycle::Allocate(void)
{
    std::cout << "CMainProductionCycle::Allocate 1"  << std::endl;

    pxCurrentTime = &(GetResources() -> xCurrentTime);


    // Получим указатель на буфер с серийным номером и идентификатором прибора.
    m_puiSerialAndId =
        (GetResources() -> m_puiSerialAndId);
}

//-------------------------------------------------------------------------------
uint8_t CMainProductionCycle::CreateTasks(void)
{
    std::cout << "CMainProductionCycle CreateTasks"  << std::endl;

//-------------------------------------------------------------------------------
    CMainProductionCycle* pxMainProductionCycle = this;
    pxMainProductionCycle ->
    SetConfigurationCreateName("ConfigurationCreate");
    pxMainProductionCycle ->
    SetInternalModuleMuvrName("InternalModuleMuvr");
    pxMainProductionCycle ->
    SetDataStoreCheckName("DataStoreCheck");

////-------------------------------------------------------------------------------
//    CLedBlinker* pxLedBlinker = 0;
//    pxLedBlinker =
//        static_cast<CLedBlinker*>(m_xResources.AddCommonTaskToMap("LedBlinkerMainProductionCycle",
//                                     std::make_shared<CLedBlinker>()));
//    pxLedBlinker ->
//    SetResources(&m_xResources);
////    pxLedBlinker ->
////    SetDataStoreLinkName("DataStoreFileSystem");
////    pxLedBlinker ->
////    SetDataStoreName("DataStoreFileSystem");
////    pxLedBlinker ->
////    SetConfigurationCreateName("ConfigurationCreate");
////    m_xResources.AddCurrentlyRunningTasksList(pxLedBlinker);


//-------------------------------------------------------------------------------
    CDeviceControl* pxDeviceControl = 0;
    pxDeviceControl =
        static_cast<CDeviceControl*>(m_xResources.AddCommonTaskToMap("DeviceControlRtuUpperLevel",
                                     std::make_shared<CDeviceControl>()));
    pxDeviceControl ->
    SetResources(&m_xResources);
//    pxDeviceControl ->
//    SetDataStoreLinkName("DataStoreFileSystem");
    pxDeviceControl ->
    SetDataStoreName("DataStoreFileSystem");
    pxDeviceControl ->
    SetConfigurationCreateName("ConfigurationCreate");
    pxDeviceControl ->
    SetInternalModuleMuvrName("InternalModuleMuvr0");
    m_xResources.AddCurrentlyRunningTasksList(pxDeviceControl);

//-------------------------------------------------------------------------------
    CStorageDeviceFileSystem* pxStorageDeviceFileSystem = 0;
    pxStorageDeviceFileSystem =
        static_cast<CStorageDeviceFileSystem*>(m_xResources.AddCommonTaskToMap("StorageDeviceFileSystem",
                std::make_shared<CStorageDeviceFileSystem>()));
    pxStorageDeviceFileSystem ->
    SetResources(&m_xResources);
    m_xResources.AddCurrentlyRunningTasksList(pxStorageDeviceFileSystem);

//-------------------------------------------------------------------------------
    CDataStore* pxDataStoreFileSystem = 0;
    pxDataStoreFileSystem =
        static_cast<CDataStore*>(m_xResources.AddCommonTaskToMap("DataStoreFileSystem",
                                 std::make_shared<CDataStore>()));
    pxDataStoreFileSystem ->
    SetResources(&m_xResources);
    pxDataStoreFileSystem ->
    SetStorageDeviceName("StorageDeviceFileSystem");
    m_xResources.AddCurrentlyRunningTasksList(pxDataStoreFileSystem);
    m_pxDataStoreFileSystem = pxDataStoreFileSystem;

//-------------------------------------------------------------------------------
    CDataStoreCheck* pxDataStoreCheck = 0;
    pxDataStoreCheck =
        static_cast<CDataStoreCheck*>(m_xResources.AddCommonTaskToMap("DataStoreCheck",
                                      std::make_shared<CDataStoreCheck>()));
    pxDataStoreCheck ->
    SetResources(&m_xResources);
    pxDataStoreCheck ->
    SetStorageDeviceName("StorageDeviceFileSystem");
    m_xResources.AddCurrentlyRunningTasksList(pxDataStoreCheck);
    m_pxDataStoreCheck = pxDataStoreCheck;


//-------------------------------------------------------------------------------
    m_xResources.ModbusWorkingArraysCreate(COILS_WORK_ARRAY_LENGTH,
                                           DISCRETE_INPUTS_ARRAY_LENGTH,
                                           HOLDING_REGISTERS_ARRAY_LENGTH,
                                           INPUT_REGISTERS_ARRAY_LENGTH);

    CSerialPortCommunicationDevice* pxSerialPortCommunicationDeviceCom1 = 0;
    pxSerialPortCommunicationDeviceCom1 =
        static_cast<CSerialPortCommunicationDevice*>(m_xResources.AddCommonTaskToMap("SerialPortCommunicationDeviceCom1",
                std::make_shared<CSerialPortCommunicationDevice>()));
    pxSerialPortCommunicationDeviceCom1 ->
    SetResources(&m_xResources);

//-------------------------------------------------------------------------------
    CModbusRtuSlaveLinkLayer* pxModbusRtuSlaveLinkLayerUpperLevel = 0;
    pxModbusRtuSlaveLinkLayerUpperLevel =
        static_cast<CModbusRtuSlaveLinkLayer*>(m_xResources.AddCommonTaskToMap("ModbusRtuSlaveLinkLayerUpperLevel",
                std::make_shared<CModbusRtuSlaveLinkLayer>()));
    pxModbusRtuSlaveLinkLayerUpperLevel ->
    SetResources(&m_xResources);
    pxModbusRtuSlaveLinkLayerUpperLevel ->
    SetCommunicationDeviceName("SerialPortCommunicationDeviceCom1");

//-------------------------------------------------------------------------------
    CModbusSlave* pxModbusRtuSlaveUpperLevel = 0;
    pxModbusRtuSlaveUpperLevel =
        static_cast<CModbusSlave*>(m_xResources.AddCommonTaskToMap("ModbusRtuSlaveUpperLevel",
                                   std::make_shared<CModbusSlave>()));
    pxModbusRtuSlaveUpperLevel ->
    SetResources(&m_xResources);
    pxModbusRtuSlaveUpperLevel ->
    SetModbusSlaveLinkLayerName("ModbusRtuSlaveLinkLayerUpperLevel");
    pxModbusRtuSlaveUpperLevel ->
    SetDeviceControlName("DeviceControlRtuUpperLevel");
    pxModbusRtuSlaveUpperLevel ->
    WorkingArraysInit();
    m_xResources.AddCurrentlyRunningTasksList(pxModbusRtuSlaveUpperLevel);

//-------------------------------------------------------------------------------
    CTcpCommunicationDevice* pxTcpCommunicationDeviceUpperLevel = 0;
    pxTcpCommunicationDeviceUpperLevel =
        static_cast<CTcpCommunicationDevice*>(m_xResources.AddCommonTaskToMap("TcpCommunicationDeviceUpperLevel",
                std::make_shared<CTcpCommunicationDevice>()));
    pxTcpCommunicationDeviceUpperLevel ->
    SetResources(&m_xResources);

//-------------------------------------------------------------------------------
    CModbusTcpSlaveLinkLayer* pxModbusTcpSlaveLinkLayerUpperLevel = 0;
    pxModbusTcpSlaveLinkLayerUpperLevel =
        static_cast<CModbusTcpSlaveLinkLayer*>(m_xResources.AddCommonTaskToMap("ModbusTcpSlaveLinkLayerUpperLevel",
                std::make_shared<CModbusTcpSlaveLinkLayer>()));
    pxModbusTcpSlaveLinkLayerUpperLevel ->
    SetResources(&m_xResources);
    pxModbusTcpSlaveLinkLayerUpperLevel ->
    SetCommunicationDeviceName("TcpCommunicationDeviceUpperLevel");

//-------------------------------------------------------------------------------
    CModbusSlave* pxModbusTcpSlaveUpperLevel = 0;
    pxModbusTcpSlaveUpperLevel =
        static_cast<CModbusSlave*>(m_xResources.AddCommonTaskToMap("ModbusTcpSlaveUpperLevel",
                                   std::make_shared<CModbusSlave>()));
    pxModbusTcpSlaveUpperLevel ->
    SetResources(&m_xResources);
    pxModbusTcpSlaveUpperLevel ->
    SetModbusSlaveLinkLayerName("ModbusTcpSlaveLinkLayerUpperLevel");
    pxModbusTcpSlaveUpperLevel ->
    SetDeviceControlName("DeviceControlRtuUpperLevel");
    pxModbusTcpSlaveUpperLevel ->
    WorkingArraysInit();
    m_xResources.AddCurrentlyRunningTasksList(pxModbusTcpSlaveUpperLevel);


//-------------------------------------------------------------------------------
    CSharedMemoryCommunicationDevice* pxSharedMemoryCommunicationDeviceEveDisplay = 0;
    pxSharedMemoryCommunicationDeviceEveDisplay =
        static_cast<CSharedMemoryCommunicationDevice*>(m_xResources.AddCommonTaskToMap("SharedMemoryCommunicationDeviceEveDisplay",
                std::make_shared<CSharedMemoryCommunicationDevice>()));
    pxSharedMemoryCommunicationDeviceEveDisplay ->
    SetResources(&m_xResources);

//-------------------------------------------------------------------------------
    CModbusSmSlaveLinkLayer* pxModbusSmSlaveLinkLayerEveDisplay = 0;
    pxModbusSmSlaveLinkLayerEveDisplay =
        static_cast<CModbusSmSlaveLinkLayer*>(m_xResources.AddCommonTaskToMap("ModbusSmSlaveLinkLayerEveDisplay",
                std::make_shared<CModbusSmSlaveLinkLayer>()));
    pxModbusSmSlaveLinkLayerEveDisplay ->
    SetResources(&m_xResources);
    pxModbusSmSlaveLinkLayerEveDisplay ->
    SetCommunicationDeviceName("SharedMemoryCommunicationDeviceEveDisplay");
    m_xResources.AddCurrentlyRunningTasksList(pxModbusSmSlaveLinkLayerEveDisplay);

//-------------------------------------------------------------------------------
    CModbusSlave* pxModbusSmSlaveEveDisplay = 0;
    pxModbusSmSlaveEveDisplay =
        static_cast<CModbusSlave*>(m_xResources.AddCommonTaskToMap("ModbusSmSlaveEveDisplay",
                                   std::make_shared<CModbusSlave>()));
//    CModbusSmSlave* pxModbusSmSlaveEveDisplay = 0;
//    pxModbusSmSlaveEveDisplay =
//        static_cast<CModbusSmSlave*>(m_xResources.AddCommonTaskToMap("ModbusSmSlaveEveDisplay",
//                                   std::make_shared<CModbusSmSlave>()));
    pxModbusSmSlaveEveDisplay ->
    SetResources(&m_xResources);
    pxModbusSmSlaveEveDisplay ->
    SetModbusSlaveLinkLayerName("ModbusSmSlaveLinkLayerEveDisplay");
    pxModbusSmSlaveEveDisplay ->
    SetDeviceControlName("DeviceControlRtuUpperLevel");
    pxModbusSmSlaveEveDisplay ->
    WorkingArraysInit();
    m_xResources.AddCurrentlyRunningTasksList(pxModbusSmSlaveEveDisplay);



//-------------------------------------------------------------------------------
    CSpiCommunicationDevice* pxSpiCommunicationDeviceSpi0 = 0;
    pxSpiCommunicationDeviceSpi0 =
        static_cast<CSpiCommunicationDevice*>(m_xResources.AddCommonTaskToMap("SpiCommunicationDeviceSpi0",
                std::make_shared<CSpiCommunicationDevice>()));
    pxSpiCommunicationDeviceSpi0 ->
    SetResources(&m_xResources);

//-------------------------------------------------------------------------------
    CInternalModule* pxInternalModuleCommon = 0;
    pxInternalModuleCommon =
        static_cast<CInternalModule*>(m_xResources.AddCommonTaskToMap("InternalModuleCommon",
                                      std::make_shared<CInternalModule>()));
    pxInternalModuleCommon ->
    SetResources(&m_xResources);
    pxInternalModuleCommon ->
    SetCommunicationDeviceName("SpiCommunicationDeviceSpi0");
    m_xResources.AddCurrentlyRunningTasksList(pxInternalModuleCommon);
    m_pxInternalModule = pxInternalModuleCommon;

////-------------------------------------------------------------------------------
//    CInternalModuleMuvr* pxInternalModuleMuvr = 0;
//    pxInternalModuleMuvr =
//        static_cast<CInternalModuleMuvr*>(m_xResources.AddCommonTaskToMap("InternalModuleMuvr",
//                                          std::make_shared<CInternalModuleMuvr>()));
//    pxInternalModuleMuvr ->
//    SetResources(&m_xResources);
//    pxInternalModuleMuvr ->
//    SetCommunicationDeviceName("SpiCommunicationDeviceSpi0");
//    m_xResources.AddCurrentlyRunningTasksList(pxInternalModuleMuvr);
//    m_pxInternalModuleMuvr = pxInternalModuleMuvr;

//-------------------------------------------------------------------------------
    CAnalogueSignals* pxAnalogueSignals = 0;
    pxAnalogueSignals =
        static_cast<CAnalogueSignals*>(m_xResources.AddCommonTaskToMap("AnalogueSignals",
                                       std::make_shared<CAnalogueSignals>()));
    pxAnalogueSignals ->
    SetResources(&m_xResources);
    pxAnalogueSignals ->
    SetDeviceControlName("DeviceControlRtuUpperLevel");
    m_xResources.AddCurrentlyRunningTasksList(pxAnalogueSignals);
    m_pxAnalogueSignals = pxAnalogueSignals;

//-------------------------------------------------------------------------------
    CConfigurationCreate* pxConfigurationCreate = 0;
    pxConfigurationCreate =
        static_cast<CConfigurationCreate*>(m_xResources.AddCommonTaskToMap("ConfigurationCreate",
                                           std::make_shared<CConfigurationCreate>()));
    pxConfigurationCreate ->
    SetResources(&m_xResources);
    pxConfigurationCreate ->
    SetInternalModuleName("InternalModuleCommon");
    m_xResources.AddCurrentlyRunningTasksList(pxConfigurationCreate);
    m_pxConfigurationCreate = pxConfigurationCreate;

//-------------------------------------------------------------------------------
    CSystemComponentsCreate* pxSystemComponentsCreate = 0;
    pxSystemComponentsCreate =
        static_cast<CSystemComponentsCreate*>(m_xResources.AddCommonTaskToMap("SystemComponentsCreate",
                std::make_shared<CSystemComponentsCreate>()));
    pxSystemComponentsCreate ->
    SetResources(&m_xResources);
    pxSystemComponentsCreate ->
    SetInternalModuleName("InternalModuleCommon");
    m_xResources.AddCurrentlyRunningTasksList(pxSystemComponentsCreate);
//    m_pxSystemComponentsCreate = pxSystemComponentsCreate;

//-------------------------------------------------------------------------------
    CDataBaseCreate* pxDataBaseCreate = 0;
    pxDataBaseCreate =
        static_cast<CDataBaseCreate*>(m_xResources.AddCommonTaskToMap("DataBaseCreate",
                                      std::make_shared<CDataBaseCreate>()));
    pxDataBaseCreate ->
    SetResources(&m_xResources);
    pxDataBaseCreate ->
    SetDataStoreName("DataStoreFileSystem");
    pxDataBaseCreate ->
    SetInternalModuleName("InternalModuleCommon");
    pxDataBaseCreate ->
    SetInternalModuleMuvrName("InternalModuleMuvr0");
    pxDataBaseCreate ->
    SetDeviceControlName("DeviceControlRtuUpperLevel");
    m_xResources.AddCurrentlyRunningTasksList(pxDataBaseCreate);
//    m_pxDataBaseCreate = pxDataBaseCreate;

//-------------------------------------------------------------------------------
    CConfigurationCheck* pxConfigurationCheck = 0;
    pxConfigurationCheck =
        static_cast<CConfigurationCheck*>(m_xResources.AddCommonTaskToMap("ConfigurationCheck",
                                          std::make_shared<CConfigurationCheck>()));
    pxConfigurationCheck ->
    SetResources(&m_xResources);
    pxConfigurationCheck ->
    SetInternalModuleName("InternalModuleCommon");
    pxConfigurationCheck ->
    SetDeviceControlName("DeviceControlRtuUpperLevel");
    m_xResources.AddCurrentlyRunningTasksList(pxConfigurationCheck);
//    m_pxConfigurationCheck = pxConfigurationCheck;

//-------------------------------------------------------------------------------
    CSettingsLoad* pxSettingsLoad = 0;
    pxSettingsLoad =
        static_cast<CSettingsLoad*>(m_xResources.AddCommonTaskToMap("SettingsLoad",
                                    std::make_shared<CSettingsLoad>()));
    pxSettingsLoad ->
    SetResources(&m_xResources);
    pxSettingsLoad ->
    SetDataStoreName("DataStoreFileSystem");
//    pxSettingsLoad ->
//    SetInternalModuleName("InternalModuleCommon");
//    pxSettingsLoad ->
//    SetInternalModuleMuvrName("InternalModuleMuvr0");
    pxSettingsLoad ->
    SetDeviceControlName("DeviceControlRtuUpperLevel");
    m_xResources.AddCurrentlyRunningTasksList(pxSettingsLoad);
//    m_pxSettingsLoad = pxSettingsLoad;

//-------------------------------------------------------------------------------
    CSettingsSet* pxSettingsSet = 0;
    pxSettingsSet =
        static_cast<CSettingsSet*>(m_xResources.AddCommonTaskToMap("SettingsSet",
                                    std::make_shared<CSettingsSet>()));
    pxSettingsSet ->
    SetResources(&m_xResources);
    pxSettingsSet ->
    SetDataStoreName("DataStoreFileSystem");
//    pxSettingsSet ->
//    SetInternalModuleName("InternalModuleCommon");
//    pxSettingsSet ->
//    SetInternalModuleMuvrName("InternalModuleMuvr0");
    pxSettingsSet ->
    SetDeviceControlName("DeviceControlRtuUpperLevel");
    m_xResources.AddCurrentlyRunningTasksList(pxSettingsSet);
//    m_pxSettingsSet = pxSettingsSet;

//-------------------------------------------------------------------------------
    CDiscreteSignals* pxDiscreteSignals = 0;
    pxDiscreteSignals =
        static_cast<CDiscreteSignals*>(m_xResources.AddCommonTaskToMap("DiscreteSignals",
                                       std::make_shared<CDiscreteSignals>()));
    pxDiscreteSignals ->
    SetResources(&m_xResources);
    pxDiscreteSignals ->
    SetDataStoreName("DataStoreFileSystem");
    pxDiscreteSignals ->
    SetDeviceControlName("DeviceControlRtuUpperLevel");
    m_xResources.AddCurrentlyRunningTasksList(pxDiscreteSignals);
//    m_pxDiscreteSignals = pxDiscreteSignals;

//-------------------------------------------------------------------------------
//    CLedBlinker* pxLedBlinker = 0;
//    pxLedBlinker =
//        static_cast<CLedBlinker*>(m_xResources.AddCommonTaskToMap("LedBlinkerMainProductionCycle",
//                                     std::make_shared<CLedBlinker>()));
//    pxLedBlinker ->
//    SetResources(&m_xResources);
//    pxLedBlinker ->
//    SetExecutorTaskName("DataStoreFileSystem");
//    pxLedBlinker ->
//    SetNextTaskDoneOkName("DataStoreFileSystem");
//    pxLedBlinker ->
//    SetNextTaskDoneErrorName("ConfigurationCreate");
////    m_xResources.AddCurrentlyRunningTasksList(pxLedBlinker);

//-------------------------------------------------------------------------------
    CAnalogueSignalsArchiveCreate* pxAnalogueSignalsArchiveCreate = 0;
    pxAnalogueSignalsArchiveCreate =
        static_cast<CAnalogueSignalsArchiveCreate*>(m_xResources.AddCommonTaskToMap("AnalogueSignalsArchiveCreate",
                std::make_shared<CAnalogueSignalsArchiveCreate>()));
    pxAnalogueSignalsArchiveCreate ->
    SetResources(&m_xResources);
    m_xResources.AddCurrentlyRunningTasksList(pxAnalogueSignalsArchiveCreate);
//    m_pxAnalogueSignalsArchiveCreate = pxAnalogueSignalsArchiveCreate;

////-------------------------------------------------------------------------------
//    xCArchiveEventsDB.SetResources(&m_xResources);

//-------------------------------------------------------------------------------
    xCInternalModuleErrorEvent.SetResources(&m_xResources);

//-------------------------------------------------------------------------------
    xCAlarmEvent.SetResources(&m_xResources);

//-------------------------------------------------------------------------------
    xCPlcErrorEvent.SetResources(&m_xResources);

//-------------------------------------------------------------------------------
    xCInfoEvent.SetResources(&m_xResources);

//-------------------------------------------------------------------------------
    xCPlcArchiveEvent.SetResources(&m_xResources);

}

//-------------------------------------------------------------------------------
// создаёт события: время включения и выключения.
void CMainProductionCycle::PlcOnOffEvetnsCreate(void)
{
    std::cout << "CMainProductionCycle PlcOnOffEvetnsCreate 1"  << std::endl;
    CEvents::TEventDataCommon *pxEventData;
    struct tm xCurrentTime;

    // получим указатель на место в кольцевом буфере журнала событий.
    pxEventData = xCAlarmEvent.EventDataPush();
// сохраним данные события.
    // установим id события.
    (pxEventData -> ui16ID) =
        1;
    // установим тип события.
    (pxEventData -> uiType) =
        CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE;
    // установим адрес источника события.
    (pxEventData -> ui16Address) = 0;
    // установим код события.
    (pxEventData -> uiState) =
        0;

    // получим последнее сохранённое время в FRAM.
    CStorageDeviceSpiFram::Read((uint8_t*)&xCurrentTime,
                                FRAM_LAST_SAVED_TIME_OFFSET,
                                sizeof(xCurrentTime));
    std::cout << "CMainProductionCycle PlcOnOffEvetnsCreate 2"  << std::endl;

    // установим время события.
    (pxEventData -> xCurrentTime) =
        xCurrentTime;
    // создадим строку текстового реквизита.
    sprintf((char*)(pxEventData -> acTextDescriptor),
            "%s",
            "Окончание работы");
    // создадим пустую строку в поле дополнительных текстовых данных таблицы архива в базе данных.
    sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
            "%s",
            " ");
    xCArchiveEventsDB.DataBaseDataPush(pxEventData);
    std::cout << "CMainProductionCycle PlcOnOffEvetnsCreate 3"  << std::endl;

    // модуль индикации МИНД последовательно запрашивает данные событий из кольцевого буфера.
    // когда МИНД получает данные события с маркером - нет события, он перестаёт посылать
    // запросы. на дисплее появляется информация о том, что событий больше нет.
    // чтобы сообщить модулю индикации МИНД о том, что событий больше нет - вытесним из
    // кольцевого буфера самое давнее событие. пометим его маркером - нет события(ui16ID = 0).
    // установим индекс (ui16EventDataCommonPopIndex) на место следующего в кольцевом буфере самого давнего события.
    xCAlarmEvent.EventDataPopIndexSet(0);
    // получим указатель на место следующего в кольцевом буфере самого давнего события.
    pxEventData = xCAlarmEvent.EventDataPop();
    // пометим событие маркером - нет события.
    pxEventData -> ui16ID = 0;
    std::cout << "CMainProductionCycle PlcOnOffEvetnsCreate 4"  << std::endl;

    // Получаем текущее время
    time_t now = time(nullptr);
    // Получаем текущую дату
    xCurrentTime = *gmtime(&now);

//    time_t xLocalLastTime;
//    uint32_t ui32Seconds;
//
//    // преобразуем в целочисленный формат последнее сохранённое время в FRAM.
//    xLocalLastTime = mktime(&xCurrentTime);
////	// обновим текущее время в переменную xCurrentTime.
////	vCurrentTimeUpdate();CTimer::
//    // получим разницу в секундах между временем выключения
//    // и временем включения.
//    ui32Seconds = (uint32_t)difftime(mktime(&xCurrentTime), xLocalLastTime);

//	// предыдущее выключение было больше чем 1 минуту назад?
//	if (ui32Seconds > 60)
//	{
//		// найдём модули МТВИ5, чтобы отправить им команду - перезагрузка больше чем через одну минуту.
//		// выставить OSF и перейти в режим «РУЧНОЙ».
//		// пройдём по всем имеющимся в конфигурации модулям.
//		for (int i = 0; i < (xPlcConfigService.
//							 xPlcConfigServiceData.
//							 ucInternalModulesQuantity); i++)
//		{
//			// найден модуль МТВИ5?
//			if (xAllModulesContext.axAllModulesContext[i].
//					xModuleContextStatic.ucModuleType == MODULE_TYPE_MTVI5)
//			{
//				// отправим команду модулю МТВИ5 - перезагрузка больше чем через одну минуту.
//				// выставить OSF и перейти в режим «РУЧНОЙ».
//				xAllModulesContext.axAllModulesContext[i].
//				xModuleContextDinamic.
//				ucCommandControl = DEVICE_CONTROL_MTVI5_LOAD_OUTS_OSF;
//			}
//		}
//	}

    // получим указатель на место в кольцевом буфере журнала событий.
    pxEventData = xCAlarmEvent.EventDataPush();

    // сохраним данные события.
    // установим id события.
    (pxEventData -> ui16ID) =
        1;
    // установим тип события.
    (pxEventData -> uiType) =
        CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE;
    // установим адрес источника события.
    (pxEventData -> ui16Address) = 0;
    // установим код события.
    (pxEventData -> uiState) =
        0;

    // установим время события.
    (pxEventData -> xCurrentTime) =
        xCurrentTime;
    // создадим строку текстового реквизита.
    sprintf((char*)(pxEventData -> acTextDescriptor),
            "%s",
            "Начало работы");
    // создадим пустую строку в поле дополнительных текстовых данных таблицы архива в базе данных.
    sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
            "%s",
            " ");
    xCArchiveEventsDB.DataBaseDataPush(pxEventData);
    std::cout << "CMainProductionCycle PlcOnOffEvetnsCreate 5"  << std::endl;

    // модуль индикации МИНД последовательно запрашивает данные событий из кольцевого буфера.
    // когда МИНД получает данные события с маркером - нет события, он перестаёт посылать
    // запросы. на дисплее появляется информация о том, что событий больше нет.
    // чтобы сообщить модулю индикации МИНД о том, что событий больше нет - вытесним из
    // кольцевого буфера самое давнее событие. пометим его маркером - нет события(ui16ID = 0).
    // установим индекс (ui16EventDataCommonPopIndex) на место следующего в кольцевом буфере самого давнего события.
    xCAlarmEvent.EventDataPopIndexSet(0);
    // получим указатель на место следующего в кольцевом буфере самого давнего события.
    pxEventData = xCAlarmEvent.EventDataPop();
    // пометим событие маркером - нет события.
    pxEventData -> ui16ID = 0;
    std::cout << "CMainProductionCycle PlcOnOffEvetnsCreate 6"  << std::endl;
}

//-------------------------------------------------------------------------------
uint8_t CMainProductionCycle::InitTasks(void)
{
    std::cout << "CMainProductionCycle Init"  << std::endl;

//-------------------------------------------------------------------------------
    GetResources() -> Allocate();

//-------------------------------------------------------------------------------
    //CGpio::Init();

//-------------------------------------------------------------------------------
    CSerialPortCommunicationDevice* pxSerialPortCommunicationDeviceCom1 =
        (CSerialPortCommunicationDevice*)(GetResources() ->
                                          GetTaskPointerByNameFromMap("SerialPortCommunicationDeviceCom1"));

    pxSerialPortCommunicationDeviceCom1 -> SetDeviceName("/dev/ttyO1");
//    pxSerialPortCommunicationDeviceCom1 -> SetBaudRate(BIT_RATE_9600);
//    pxSerialPortCommunicationDeviceCom1 -> SetDataBits(8);
//    pxSerialPortCommunicationDeviceCom1 -> SetParity(PARITY_NO);
//    pxSerialPortCommunicationDeviceCom1 -> SetStopBit(2);
//    pxSerialPortCommunicationDeviceCom1 -> Init();

//-------------------------------------------------------------------------------
    CModbusSlave* pxModbusRtuSlaveUpperLevel =
        (CModbusSlave*)(GetResources() ->
                        GetTaskPointerByNameFromMap("ModbusRtuSlaveUpperLevel"));

    (GetResources() -> m_pxGpioRtsControlPin) =
        CGpio::Create(3,
                      21,
                      "RTS_CONTROL_PIN");
    pxModbusRtuSlaveUpperLevel ->
    m_pxGpioRtsControlPin = (GetResources() -> m_pxGpioRtsControlPin.get());

    pxModbusRtuSlaveUpperLevel ->
    SetOwnAddress(1);


//-------------------------------------------------------------------------------
    CTcpCommunicationDevice* pxTcpCommunicationDeviceUpperLevel =
        (CTcpCommunicationDevice*)(GetResources() ->
                                   GetTaskPointerByNameFromMap("TcpCommunicationDeviceUpperLevel"));

    pxTcpCommunicationDeviceUpperLevel -> Init();
    pxTcpCommunicationDeviceUpperLevel -> SetIpAddress("127.0.0.1");
    pxTcpCommunicationDeviceUpperLevel -> SetPort(502);

//-------------------------------------------------------------------------------
    CModbusSlave* pxModbusTcpSlaveUpperLevel =
        (CModbusSlave*)(GetResources() ->
                        GetTaskPointerByNameFromMap("ModbusTcpSlaveUpperLevel"));

//    pxModbusTcpSlaveUpperLevel ->
//    m_pxGpioRtsControlPin =
//        (CGpio::Create(3,
//                       21,
//                       "RTS_CONTROL_PIN").get());
    pxModbusTcpSlaveUpperLevel ->
    m_pxGpioRtsControlPin = (GetResources() -> m_pxGpioRtsControlPin.get());

    pxModbusTcpSlaveUpperLevel ->
    SetOwnAddress(1);


//-------------------------------------------------------------------------------
    CSharedMemoryCommunicationDevice* pxSharedMemoryCommunicationDeviceEveDisplay =
        (CSharedMemoryCommunicationDevice*)(GetResources() ->
                                            GetTaskPointerByNameFromMap("SharedMemoryCommunicationDeviceEveDisplay"));

    pxSharedMemoryCommunicationDeviceEveDisplay -> Init();
//    pxSharedMemoryCommunicationDeviceEveDisplay -> SetDeviceName("/dev/ttyO1");

//-------------------------------------------------------------------------------
//    CModbusSmSlave* pxModbusSmSlaveEveDisplay =
//        (CModbusSmSlave*)(GetResources() ->
//                          GetTaskPointerByNameFromMap("ModbusSmSlaveEveDisplay"));
    CModbusSlave* pxModbusSmSlaveEveDisplay =
        (CModbusSlave*)(GetResources() ->
                        GetTaskPointerByNameFromMap("ModbusSmSlaveEveDisplay"));

    // несуществующий светодиод-заглушка. для всех интерфейсо модас славе один
    // чтобы светодиод разделяемой памяти не мешал ему нужно присвоить пустой
    // светодиод чтобы небыло нулевого указателя.
    (GetResources() -> m_pxGpioSmRtsControlPin) =
        CGpio::Create(0,
                      12,
                      "RTS_CONTROL_PIN");
    pxModbusSmSlaveEveDisplay ->
    m_pxGpioRtsControlPin = (GetResources() -> m_pxGpioSmRtsControlPin.get());

    pxModbusSmSlaveEveDisplay ->
    SetOwnAddress(1);


//-------------------------------------------------------------------------------
    CSpiCommunicationDevice* pxSpiCommunicationDeviceSpi0 =
        (CSpiCommunicationDevice*)(GetResources() ->
                                   GetTaskPointerByNameFromMap("SpiCommunicationDeviceSpi0"));

    pxSpiCommunicationDeviceSpi0 -> Init();

////-------------------------------------------------------------------------------
//    CInternalModule* pxInternalModuleCommon =
//        (CInternalModule*)(GetResources() ->
//                          GetTaskPointerByNameFromMap("InternalModuleCommon"));

////-------------------------------------------------------------------------------
//    CInternalModuleMuvr* pxInternalModuleMuvr =
//        (CInternalModuleMuvr*)(GetResources() ->
//                               GetTaskPointerByNameFromMap("InternalModuleMuvr"));
//    pxInternalModuleMuvr ->
//    SetAddress(0);

////-------------------------------------------------------------------------------
//    xCArchiveEventsDB.Allocate();

//-------------------------------------------------------------------------------
    xCInternalModuleErrorEvent.Allocate();

//-------------------------------------------------------------------------------
    xCAlarmEvent.Allocate();

//-------------------------------------------------------------------------------
    xCPlcErrorEvent.Allocate();

//-------------------------------------------------------------------------------
    xCInfoEvent.Allocate();

//-------------------------------------------------------------------------------
    xCPlcArchiveEvent.Allocate();

}

//-------------------------------------------------------------------------------
void CMainProductionCycle::CurrentlyRunningTasksExecution(void)
{
//    std::cout << "CMainProductionCycle CurrentlyRunningTasksExecution"  << std::endl;


//    xTimeMeasure.Begin();

    for(GetResources() -> m_xCurrentlyRunningTasksListIterator =
                GetResources() -> m_lpxCurrentlyRunningTasksList.begin();
            GetResources() -> m_xCurrentlyRunningTasksListIterator !=
            GetResources() -> m_lpxCurrentlyRunningTasksList.end();
            GetResources() -> m_xCurrentlyRunningTasksListIterator++)
    {
        (*(GetResources() -> m_xCurrentlyRunningTasksListIterator)) -> Fsm();
    }

    usleep(500);
//
//    xTimeMeasure.End();
}

//-------------------------------------------------------------------------------
uint8_t CMainProductionCycle::Fsm(void)
{
//        std::cout << "CMainProductionCycle::Fsm 1"  << std::endl;

//    for (uint8_t i; i < 20; i++)
//    {
//    CTimeMeasure xTimeMeasure;

//    usleep(1000);
//    CurrentlyRunningTasksExecution();

    switch (GetFsmState())
    {
    case IDDLE:
        //std::cout << "CMainProductionCycle::Fsm IDDLE"  << std::endl;
//        usleep(1000);
        break;

    case STOP:
//        //std::cout << "CMainProductionCycle::Fsm STOP"  << std::endl;
//        SetFsmState(START);
        break;

    case START:
        std::cout << "CMainProductionCycle::Fsm START"  << std::endl;
        std::cout << "m_acTaskName " << m_acTaskName << std::endl;
        Init();
        CreateTasks();

//        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME_LONG);
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CMainProductionCycle::Fsm INIT"  << std::endl;
        if (GetResources() -> CheckCommonTaskMap())
        {
            InitTasks();

            m_uiDeviceControlId =
                GetResources() ->
                GetTaskIdByNameFromMap("DeviceControlRtuUpperLevel");

            m_uiConfigurationCreateId =
                GetResources() ->
                GetTaskIdByNameFromMap("ConfigurationCreate");

            m_uiDataStoreCheckId =
                GetResources() ->
                GetTaskIdByNameFromMap("DataStoreCheck");

            m_uiSystemComponentsCreateId =
                GetResources() ->
                GetTaskIdByNameFromMap("SystemComponentsCreate");

            m_uiInternalModuleId =
                GetResources() ->
                GetTaskIdByNameFromMap("InternalModuleCommon");

            m_uiInternalModuleMuvrId =
                GetResources() ->
                GetTaskIdByNameFromMap("InternalModuleMuvr0");

            m_uiDataBaseCreateId =
                GetResources() ->
                GetTaskIdByNameFromMap("DataBaseCreate");

            m_uiConfigurationCheckId =
                GetResources() ->
                GetTaskIdByNameFromMap("ConfigurationCheck");

            m_uiSettingsLoadId =
                GetResources() ->
                GetTaskIdByNameFromMap("SettingsLoad");

            m_uiSettingsSetId =
                GetResources() ->
                GetTaskIdByNameFromMap("SettingsSet");

            m_uiDiscreteSignalsId =
                GetResources() ->
                GetTaskIdByNameFromMap("DiscreteSignals");

            m_uiAnalogueSignalsArchiveCreateId =
                GetResources() ->
                GetTaskIdByNameFromMap("AnalogueSignalsArchiveCreate");

            SetFsmState(READY);
        }
        else
        {
            SetFsmState(STOP);
        }
        break;

    case READY:
        std::cout << "CMainProductionCycle::Fsm READY"  << std::endl;
        SetFsmState(CONFIGURATION_CREATE_START);

        break;

    case DONE_OK:
//        std::cout << "CMainProductionCycle::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CMainProductionCycle::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START:
//        std::cout << "CMainProductionCycle::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME_LONG);
        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING);
    }
    break;

    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING:
//        std::cout << "CMainProductionCycle::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
//            std::cout << "CMainProductionCycle::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else
        {
//            std::cout << "CMainProductionCycle::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CMainProductionCycle::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
//        std::cout << "CMainProductionCycle::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME_LONG);
        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
    }
    break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        std::cout << "CMainProductionCycle::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
//            std::cout << "CMainProductionCycle::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
//            std::cout << "CMainProductionCycle::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CMainProductionCycle::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
//        std::cout << "CMainProductionCycle::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME_LONG);
        SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
    }

    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        std::cout << "CMainProductionCycle::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
//            std::cout << "CMainProductionCycle::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CMainProductionCycle::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(GetFsmNextStateDoneWaitingDoneError());
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CMainProductionCycle::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateDoneWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case CONFIGURATION_CREATE_START:
        std::cout << "CMainProductionCycle::Fsm CONFIGURATION_CREATE_START"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

//            // при старте нужно прочитать из хранилища в поле класса сервиный блок.
//            m_pxDataStoreFileSystem -> ReadServiceSection();

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiConfigurationCreateId;
            pxDataContainer -> m_uiFsmCommandState =
                CConfigurationCreate::CONFIGURATION_CREATE_START;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(CONFIGURATION_CREATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(CONFIGURATION_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(CONFIGURATION_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(CONFIGURATION_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case CONFIGURATION_CREATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CMainProductionCycle::Fsm CONFIGURATION_CREATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            // текущая конфигурация и сохранённая в базе данных совпадают.
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DATA_STORE_CHECK_START);
        }
        break;

    case CONFIGURATION_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CMainProductionCycle::Fsm CONFIGURATION_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            // текущая конфигурация и сохранённая в базе данных не совпадают.
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(INCORRECT_CONFIGURATION_ERROR_HANDLER_START);
//            SetFsmState(DATA_STORE_CHECK_START);

        }
        break;

//-------------------------------------------------------------------------------
    case DATA_STORE_CHECK_START:
        std::cout << "CMainProductionCycle::Fsm DATA_STORE_CHECK_START"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataStoreCheckId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStoreCheck::DATA_STORE_CHECK_START;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_STORE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DATA_STORE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(DATA_STORE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(DATA_STORE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case DATA_STORE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CMainProductionCycle::Fsm DATA_STORE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            // текущая база данных не повреждена.
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(SERIAL_AND_ID_LOAD_START);
        }
        break;

    case DATA_STORE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CMainProductionCycle::Fsm DATA_STORE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            // текущая база данных повреждена.
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DATA_BASE_CREATE_START);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_CREATE_START:
        std::cout << "CMainProductionCycle::Fsm DATA_BASE_CREATE_START"  << std::endl;
        {
            CurrentlyRunningTasksExecution();
            // для создания новой базы данных нужно создать новый сервисный блок в хранилище,
            // чтобы стереть прошлую информацию о сохранённых блоках.
            m_pxDataStoreFileSystem -> CreateServiceSection();

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataBaseCreateId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataBaseCreate::DATA_BASE_CREATE_START;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_CREATE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DATA_BASE_CREATE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CMainProductionCycle::Fsm DATA_BASE_CREATE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(SERIAL_AND_ID_LOAD_START);
        }
        break;

////-------------------------------------------------------------------------------
//    case SERIAL_AND_ID_ARCHIVE_FILES_RENAME_START:
//        std::cout << "CMainProductionCycle::Fsm SERIAL_AND_ID_ARCHIVE_FILES_RENAME_START"  << std::endl;
//        {
//            CurrentlyRunningTasksExecution();
//
//            // при старте нужно прочитать из хранилища в поле класса сервиный блок.
//            m_pxDataStoreFileSystem -> ReadServiceSection();
//
//            CDataContainerDataBase* pxDataContainer =
//                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//            pxDataContainer -> m_uiTaskId = m_uiAnalogueSignalsArchiveCreateId;
//            pxDataContainer -> m_uiFsmCommandState =
//                CAnalogueSignalsArchiveCreate::ANALOGUE_SIGNALS_SERIAL_AND_ID_ARCHIVE_FILES_RENAME_START;
//
//            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
//            SetFsmNextStateDoneOk(SERIAL_AND_ID_ARCHIVE_FILES_RENAME_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
//            SetFsmNextStateReadyWaitingError(SERIAL_AND_ID_ARCHIVE_FILES_RENAME_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
//            SetFsmNextStateDoneWaitingError(SERIAL_AND_ID_ARCHIVE_FILES_RENAME_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
//            SetFsmNextStateDoneWaitingDoneError(SERIAL_AND_ID_ARCHIVE_FILES_RENAME_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
//        }
//        break;
//
//    case SERIAL_AND_ID_ARCHIVE_FILES_RENAME_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
//        std::cout << "CMainProductionCycle::Fsm SERIAL_AND_ID_ARCHIVE_FILES_RENAME_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
//        {
//            CurrentlyRunningTasksExecution();
//
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//            SetFsmState(SETTINGS_LOAD_START);
//        }
//        break;
//
//    case SERIAL_AND_ID_ARCHIVE_FILES_RENAME_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
//        std::cout << "CMainProductionCycle::Fsm SERIAL_AND_ID_ARCHIVE_FILES_RENAME_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
//        {
//            CurrentlyRunningTasksExecution();
//
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
//            SetFsmState(INCORRECT_CONFIGURATION_ERROR_HANDLER_START);
//        }
//        break;

//-------------------------------------------------------------------------------
    case SERIAL_AND_ID_LOAD_START:
        std::cout << "CMainProductionCycle::Fsm SERIAL_AND_ID_LOAD_START"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            // при старте нужно прочитать из хранилища в поле класса сервиный блок.
            m_pxDataStoreFileSystem -> ReadServiceSection();

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiAnalogueSignalsArchiveCreateId;
            pxDataContainer -> m_uiFsmCommandState =
                CAnalogueSignalsArchiveCreate::ANALOGUE_SIGNALS_SERIAL_AND_ID_LOAD_START;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(SERIAL_AND_ID_LOAD_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(SERIAL_AND_ID_LOAD_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(SERIAL_AND_ID_LOAD_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(SERIAL_AND_ID_LOAD_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case SERIAL_AND_ID_LOAD_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CMainProductionCycle::Fsm SERIAL_AND_ID_LOAD_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(SETTINGS_LOAD_START);
        }
        break;

    case SERIAL_AND_ID_LOAD_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CMainProductionCycle::Fsm SERIAL_AND_ID_LOAD_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(INCORRECT_CONFIGURATION_ERROR_HANDLER_START);
        }
        break;

//-------------------------------------------------------------------------------
    case SETTINGS_LOAD_START:
        std::cout << "CMainProductionCycle::Fsm SETTINGS_LOAD_START"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiSettingsLoadId;
            pxDataContainer -> m_uiFsmCommandState =
                CSettingsLoad::SETTINGS_LOAD_START;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(SETTINGS_LOAD_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(SETTINGS_LOAD_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(SETTINGS_LOAD_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(SETTINGS_LOAD_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case SETTINGS_LOAD_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CMainProductionCycle::Fsm SETTINGS_LOAD_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(CONFIGURATION_CHECK_START);
        }
        break;

    case SETTINGS_LOAD_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CMainProductionCycle::Fsm SETTINGS_LOAD_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(INCORRECT_CONFIGURATION_ERROR_HANDLER_START);
        }
        break;

//-------------------------------------------------------------------------------
    case CONFIGURATION_CHECK_START:
        std::cout << "CMainProductionCycle::Fsm CONFIGURATION_CHECK_START"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

//            // при старте нужно прочитать из хранилища в поле класса сервиный блок.
//            m_pxDataStoreFileSystem -> ReadServiceSection();

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiConfigurationCheckId;
            pxDataContainer -> m_uiFsmCommandState =
                CConfigurationCheck::CONFIGURATION_CHECK_START;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(CONFIGURATION_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(CONFIGURATION_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(CONFIGURATION_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(CONFIGURATION_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case CONFIGURATION_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CMainProductionCycle::Fsm CONFIGURATION_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            // текущая конфигурация и сохранённая в базе данных совпадают.
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(SYSTEM_COMPONENTS_CREATE_START);
        }
        break;

    case CONFIGURATION_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CMainProductionCycle::Fsm CONFIGURATION_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            // текущая конфигурация и сохранённая в базе данных не совпадают.
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_START);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_CREATE_CONFIGURATION_DATA_BASE_BLOCKS_WRITE_START:
        std::cout << "CMainProductionCycle::Fsm DATA_BASE_CREATE_CONFIGURATION_DATA_BASE_BLOCKS_WRITE_START"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataBaseCreateId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataBaseCreate::DATA_BASE_CREATE_CONFIGURATION_DATA_BASE_BLOCKS_WRITE_START;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_CREATE_CONFIGURATION_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DATA_BASE_CREATE_CONFIGURATION_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CMainProductionCycle::Fsm DATA_BASE_CREATE_CONFIGURATION_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            // получим указатель на объект управения пином порта ввода-вывода(синий светодиод)
            CGpio* pxGpioPrdEnablePin = (GetResources() -> m_pxGpioPrdEnablePin.get());
            // погасим синий светодиод сигнализирующий об ошибке конфигурации
            pxGpioPrdEnablePin -> ClearPin();

            // зарегистрируем событие.
            CEvents::EventRegistration(
                0,
                (CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                0,
                "Новая конф. и бд.");

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(SERIAL_AND_ID_LOAD_START);
        }
        break;

//-------------------------------------------------------------------------------
    case SYSTEM_COMPONENTS_CREATE_START:
        std::cout << "CMainProductionCycle::Fsm SYSTEM_COMPONENTS_CREATE_START"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiSystemComponentsCreateId;
            pxDataContainer -> m_uiFsmCommandState =
                CSystemComponentsCreate::SYSTEM_COMPONENTS_CREATE_DISCRETE_SIGNALS_SERVICE_DATA_CREATE_START;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(SYSTEM_COMPONENTS_CREATE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case SYSTEM_COMPONENTS_CREATE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CMainProductionCycle::Fsm SYSTEM_COMPONENTS_CREATE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(MAIN_CYCLE_START);
        }
        break;

//-------------------------------------------------------------------------------
    case MAIN_CYCLE_START:
        std::cout << "CMainProductionCycle::Fsm MAIN_CYCLE_START"  << std::endl;
        CurrentlyRunningTasksExecution();

        // откроем базу данных.
        if (xCArchiveEventsDB.Connect((GetResources() -> m_puiSerialAndId)))
        {
            // error.
        }

        // создадим событие времени включения и выключения
        PlcOnOffEvetnsCreate();

        SetFsmState(INTERNAL_MODULES_DATA_EXCHANGE_START);
        break;

//-------------------------------------------------------------------------------
    case INTERNAL_MODULES_DATA_EXCHANGE_START:
//        std::cout << "CMainProductionCycle::Fsm INTERNAL_MODULES_DATA_EXCHANGE_START"  << std::endl;
    {

//        xTimeMeasure.Begin();

        CurrentlyRunningTasksExecution();

        m_xMainCycle100McTimer.Set(100);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiInternalModuleId;
        pxDataContainer -> m_uiFsmCommandState =
            CInternalModule::MODULES_DATA_EXCHANGE_START;
        pxDataContainer -> m_puiDataPointer =
            (uint8_t*)(GetResources() -> GetDeviceConfigSearchPointer());

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(INTERNAL_MODULES_DATA_EXCHANGE_EXECUTOR_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(INTERNAL_MODULES_DATA_EXCHANGE_EXECUTOR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(INTERNAL_MODULES_DATA_EXCHANGE_EXECUTOR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(INTERNAL_MODULES_DATA_EXCHANGE_EXECUTOR_ANSWER_PROCESSING);
//        SetFsmNextStateReadyWaitingError(DONE_ERROR);
//        SetFsmNextStateDoneWaitingError(DONE_ERROR);
//        SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);

//        SetFsmState(INTERNAL_MODULES_DATA_EXCHANGE_MAIN_CYCLE_START_WAITING);
    }
    break;

    case INTERNAL_MODULES_DATA_EXCHANGE_EXECUTOR_ANSWER_PROCESSING:
//        std::cout << "CMainProductionCycle::Fsm INTERNAL_MODULES_DATA_EXCHANGE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(INTERNAL_MODULES_DATA_EXCHANGE_MAIN_CYCLE_START_WAITING);
    }
    break;

    case INTERNAL_MODULES_DATA_EXCHANGE_MAIN_CYCLE_START_WAITING:
//        std::cout << "CMainProductionCycle::Fsm INTERNAL_MODULES_DATA_EXCHANGE_MAIN_CYCLE_START_WAITING 1"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        if (m_xMainCycle100McTimer.IsOverflow())
        {
//            std::cout << "CMainProductionCycle::Fsm INTERNAL_MODULES_DATA_EXCHANGE_MAIN_CYCLE_START_WAITING 2"  << std::endl;
            m_xMainCycle100McTimer.Set(100);
            // время периода записи аналоговых сигналов в архив(1 секунда) не прошло?
            if (m_uiCreateArchiveEntryCounter < 5)
            {
                m_uiCreateArchiveEntryCounter++;
                SetFsmState(MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING);
            }
            else
            {
                m_uiCreateArchiveEntryCounter = 1;
                SetFsmState(ANALOGUE_SIGNALS_ARCHIVE_CREATE_START);
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case ANALOGUE_SIGNALS_ARCHIVE_CREATE_START:
//        std::cout << "CMainProductionCycle::Fsm ANALOGUE_SIGNALS_ARCHIVE_CREATE_START"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiAnalogueSignalsArchiveCreateId;
        pxDataContainer -> m_uiFsmCommandState =
            CAnalogueSignalsArchiveCreate::ANALOGUE_SIGNALS_ARCHIVE_CREATE_START;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
//        std::cout << "CMainProductionCycle::Fsm ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING);
    }
    break;

    case ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
//        std::cout << "CMainProductionCycle::Fsm ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING);
    }
    break;

//-------------------------------------------------------------------------------
    case MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING:
//        std::cout << "CMainProductionCycle::Fsm MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING"  << std::endl;
        CurrentlyRunningTasksExecution();

        SetFsmState(MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_START);
        break;

//-------------------------------------------------------------------------------
    case MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_START:
//        std::cout << "CMainProductionCycle::Fsm MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_START"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiDiscreteSignalsId;
        pxDataContainer -> m_uiFsmCommandState =
            CDiscreteSignals::DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_START;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
//        std::cout << "CMainProductionCycle::Fsm MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(MAIN_CYCLE_END);
//            SetFsmState(DONE_OK);
    }
    break;

    case MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CMainProductionCycle::Fsm MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(MAIN_CYCLE_END);
//            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case MAIN_CYCLE_END:
//        std::cout << "CMainProductionCycle::Fsm MAIN_CYCLE_END"  << std::endl;
        CurrentlyRunningTasksExecution();

        // обработаем события.
        CEvents::EventsHandler();
        (GetResources() -> m_uiModbusReceipt) = 0;
        (GetResources() -> m_uiModbusReset) = 0;
        SetFsmState(INTERNAL_MODULES_DATA_EXCHANGE_START);

//        xTimeMeasure.End();
        break;

//-------------------------------------------------------------------------------
    case INCORRECT_CONFIGURATION_ERROR_HANDLER_START:
//        std::cout << "CMainProductionCycle::Fsm INCORRECT_CONFIGURATION_ERROR_HANDLER_START"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        // получим указатель на объект управения пином порта ввода-вывода(синий светодиод)
        CGpio* pxGpioPrdEnablePin = (GetResources() -> m_pxGpioPrdEnablePin.get());
        // зажжём синий светодиод сигнализирующий об ошибке конфигурации
        pxGpioPrdEnablePin -> SetPin();

        // откроем базу данных.
        if (xCArchiveEventsDB.Connect((GetResources() -> m_puiSerialAndId)))
        {
            // error.
        }

        // создадим событие времени включения и выключения
        PlcOnOffEvetnsCreate();

        // зарегистрируем активное состояние события.
        CEvents::EventRegistration(
            0,
            (CEvents::HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE |
             CEvents::HANDLED_EVENTS_IS_POPUP |
             CEvents::HANDLED_EVENTS_IS_SOUND |
             CEvents::HANDLED_EVENTS_IS_OCCURED_ON_START |
             CEvents::HANDLED_EVENTS_IS_ARCHIVE),
            0,
            "Некор. конфигурация");

        m_xMainCycle100McTimer.Set(100);
        SetFsmState(INCORRECT_CONFIGURATION_ERROR_HANDLER_TIME_UPDATE_END_CYCLE_WAITING);

    }
    break;

    case INCORRECT_CONFIGURATION_ERROR_HANDLER_TIME_UPDATE_END_CYCLE_WAITING:
//        std::cout << "CMainProductionCycle::Fsm INCORRECT_CONFIGURATION_ERROR_HANDLER_TIME_UPDATE_END_CYCLE_WAITING"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        if (m_xMainCycle100McTimer.IsOverflow())
        {
            m_xMainCycle100McTimer.Set(100);

            // получим указатель на объект управения пином порта ввода-вывода(синий светодиод)
            CGpio* pxGpioPrdEnablePin = (GetResources() -> m_pxGpioPrdEnablePin.get());
            // зажжём синий светодиод сигнализирующий об ошибке конфигурации
            pxGpioPrdEnablePin -> SetPin();

            // обработаем события.
            CEvents::EventsHandler();
            SetFsmState(INCORRECT_CONFIGURATION_ERROR_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        }
    }
    break;

    case INCORRECT_CONFIGURATION_ERROR_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
//        std::cout << "CMainProductionCycle::Fsm INCORRECT_CONFIGURATION_ERROR_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        m_xMainCycle100McTimer.Set(100);

//        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(INCORRECT_CONFIGURATION_ERROR_HANDLER_TIME_UPDATE_END_CYCLE_WAITING);
    }
    break;

    case INCORRECT_CONFIGURATION_ERROR_HANDLER_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
//        std::cout << "CMainProductionCycle::Fsm INCORRECT_CONFIGURATION_ERROR_HANDLER_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

//        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_START:
//        std::cout << "CMainProductionCycle::Fsm CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_START"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        // получим указатель на объект управения пином порта ввода-вывода(синий светодиод)
        CGpio* pxGpioPrdEnablePin = (GetResources() -> m_pxGpioPrdEnablePin.get());
        // зажжём синий светодиод сигнализирующий об ошибке конфигурации
        pxGpioPrdEnablePin -> SetPin();

        // откроем базу данных.
        if (xCArchiveEventsDB.Connect((GetResources() -> m_puiSerialAndId)))
        {
            // error.
        }

        // создадим событие времени включения и выключения
        PlcOnOffEvetnsCreate();

        // зарегистрируем активное состояние события.
        CEvents::EventRegistration(
            0,
            (CEvents::HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE |
             CEvents::HANDLED_EVENTS_IS_POPUP |
             CEvents::HANDLED_EVENTS_IS_SOUND |
             CEvents::HANDLED_EVENTS_IS_OCCURED_ON_START |
             CEvents::HANDLED_EVENTS_IS_ARCHIVE),
            0,
            "Ошибка конфигурации");

        SetFsmState(CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_COMMAND_SEND);
    }
    break;

    case CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_COMMAND_SEND:
//        std::cout << "CMainProductionCycle::Fsm CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_COMMAND_SEND"  << std::endl;
    {
        CurrentlyRunningTasksExecution();
        // проверим записан ли блок 97 базы данных с идентификатором прибора.
        // при первом запуске конфигурация ещё не сохранена в блоке базы данных
        // и во время проверки конфигурации при сравнении созданной и сохранённой
        // происходит ошибка и прибор переходит в
        // аварийное состояние ожидая подтверждения конфигурации.
        // конфигурация считается подтверждённой после записи 97 блока базы данных
        //  с идентификатором прибора.

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
        pxDataContainer -> m_uiFsmCommandState =
            CDeviceControl::CONFIGURATION_CONFIRMATION_CHECK_START;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_COMMAND_SEND_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_COMMAND_SEND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_COMMAND_SEND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_COMMAND_SEND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_COMMAND_SEND_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CMainProductionCycle::Fsm CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_COMMAND_SEND_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DATA_BASE_CREATE_CONFIGURATION_DATA_BASE_BLOCKS_WRITE_START);
        }
        break;

    case CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_COMMAND_SEND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
//        std::cout << "CMainProductionCycle::Fsm CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_COMMAND_SEND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_TIME_UPDATE_START);
    }
    break;

    case CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_TIME_UPDATE_START:
//        std::cout << "CMainProductionCycle::Fsm CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_TIME_UPDATE_START"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
        pxDataContainer -> m_uiFsmCommandState =
            CDeviceControl::TIME_UPDATE_START;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_TIME_UPDATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_TIME_UPDATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_TIME_UPDATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_TIME_UPDATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);

        m_xMainCycle100McTimer.Set(500);
    }
    break;

    case CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_TIME_UPDATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
//        std::cout << "CMainProductionCycle::Fsm CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_TIME_UPDATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_TIME_UPDATE_END_CYCLE_WAITING);
    }
    break;

    case CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_TIME_UPDATE_END_CYCLE_WAITING:
//        std::cout << "CMainProductionCycle::Fsm CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_TIME_UPDATE_END_CYCLE_WAITING"  << std::endl;
    {
        CurrentlyRunningTasksExecution();

        if (m_xMainCycle100McTimer.IsOverflow())
        {
            // получим указатель на объект управения пином порта ввода-вывода(синий светодиод)
            CGpio* pxGpioPrdEnablePin = (GetResources() -> m_pxGpioPrdEnablePin.get());
            // зажжём синий светодиод сигнализирующий об ошибке конфигурации
            pxGpioPrdEnablePin -> SetPin();

            // обработаем события.
            CEvents::EventsHandler();
            SetFsmState(CONFIGURATION_CONFIRMATION_WAITING_ERROR_HANDLER_COMMAND_SEND);
        }
    }
    break;

//-------------------------------------------------------------------------------
    default:
        break;
    }
    //    }

    return GetFsmState();
}

//-------------------------------------------------------------------------------








//-------------------------------------------------------------------------------
CLedBlinker::CLedBlinker()
{
    std::cout << "CLedBlinker constructor"  << std::endl;
    SetFsmState(INIT);
}

//-------------------------------------------------------------------------------
CLedBlinker::~CLedBlinker()
{
    std::cout << "CLedBlinker destructor"  << std::endl;
}

//-------------------------------------------------------------------------------
uint8_t CLedBlinker::Init(void)
{
    std::cout << "CLedBlinker Init"  << std::endl;
}

//-------------------------------------------------------------------------------
uint8_t CLedBlinker::Fsm(void)
{
//        std::cout << "CLedBlinker::Fsm 1"  << std::endl;

    switch (GetFsmState())
    {
    case INIT:
//        //std::cout << "CLedBlinker::Fsm INIT 1"  << std::endl;
        Init();
        SetFsmState(STOP);
        break;

    case STOP:
//        //std::cout << "CLedBlinker::Fsm STOP"  << std::endl;
        break;

    case START:
        //std::cout << "CLedBlinker::Fsm START"  << std::endl;
        SetFsmState(LED_ON);
        break;

    case DONE_OK:
//        //std::cout << "CLedBlinker::Fsm DONE_OK"  << std::endl;
        break;

    case DONE_ERROR:
//        //std::cout << "CLedBlinker::Fsm DONE_ERROR"  << std::endl;
        break;

//-------------------------------------------------------------------------------
    case LED_ON:
        std::cout << "CLedBlinker::Fsm LED_ON"  << std::endl;
        GetTimerPointer() -> Set(500);
        SetFsmState(LED_ON_PERIOD_END_WAITING);
        break;

    case LED_ON_PERIOD_END_WAITING:
//        //std::cout << "CLedBlinker::Fsm LED_ON_PERIOD_END_WAITING"  << std::endl;
        if (GetTimerPointer() -> IsOverflow())
        {
            SetFsmState(LED_OFF);
        }
        break;

    case LED_OFF:
        std::cout << "CLedBlinker::Fsm LED_OFF"  << std::endl;
        GetTimerPointer() -> Set(500);
        SetFsmState(LED_OFF_PERIOD_END_WAITING);
        break;

    case LED_OFF_PERIOD_END_WAITING:
//        //std::cout << "CLedBlinker::Fsm LED_OFF_PERIOD_END_WAITING"  << std::endl;
        if (GetTimerPointer() -> IsOverflow())
        {
            SetFsmState(LED_ON);
        }
        break;

    default:
        break;
    }

    return GetFsmState();
}

//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
