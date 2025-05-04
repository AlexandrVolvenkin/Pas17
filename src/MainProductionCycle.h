//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CMAINPRODUCTIONCYCLE_H
#define CMAINPRODUCTIONCYCLE_H

#include <iostream>
#include <string.h>

#include "Platform.h"
#include "Resources.h"
#include "ServiceMarket.h"
#include "DataStore.h"
#include "DeviceControl.h"
#include "InternalModule.h"
//#include "Task.h"
//#include "Production.h"
//#include "ModbusTcp.h"
#include "ModbusSlave.h"
#include "ModbusSlaveLinkLayer.h"
#include "ModbusTcpSlaveLinkLayer.h"
#include "ModbusRtuSlaveLinkLayer.h"

#include "ModbusSmSlave.h"
#include "ModbusSmSlaveLinkLayer.h"

class CTask;
//class CResources;
class CLedBlinker;
class CConfiguration;
class CDataStoreCheck;
//class CDataStore;
//class CDeviceControl;
class CDataContainerInterface;
class CDataContainerDataBase;
class CConfigurationCreate;
class CDataBaseCreate;
class CSystemComponentsCreate;
class CConfigurationCheck;
class CSettingsLoad;

//-------------------------------------------------------------------------------
class CMainProductionCycle : public CTask
{
public:

    enum
    {
        MAIN_CYCLE_SHOW_STATISTICS_PERIOD_TIME = 1000,
    };

    enum
    {
        DATA_STORE_CHECK_START = NEXT_STEP,
        DATA_STORE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DATA_STORE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        DATA_STORE_CHECK_TASK_READY_CHECK,
        DATA_STORE_CHECK_TASK_READY_WAITING,
        DATA_STORE_CHECK_BEGIN,
        DATA_STORE_CHECK_END_WAITING,
        DATA_STORE_CHECK_RECAVERY_END_WAITING,
        DATA_STORE_CHECK_END_OK,
        DATA_STORE_CHECK_END_ERROR,

        CONFIGURATION_CREATE_START,
        CONFIGURATION_CREATE_EXECUTOR_ANSWER_PROCESSING,
//        CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_START,
//        CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_WAITING,
//        CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_START,
//        CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_WAITING,

        SYSTEM_COMPONENTS_CREATE_START,
        SYSTEM_COMPONENTS_CREATE_EXECUTOR_ANSWER_PROCESSING,
        INTERNAL_MODULES_DATA_EXCHANGE_START,
        INTERNAL_MODULES_DATA_EXCHANGE_EXECUTOR_ANSWER_PROCESSING,
        INTERNAL_MODULES_DATA_EXCHANGE_MAIN_CYCLE_START_WAITING,

        CONFIGURATION_CHECK_START,
        CONFIGURATION_CHECK_CONFIGURATION_DATA_BASE_BLOCKS_READ_START,
        CONFIGURATION_CHECK_CONFIGURATION_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        CONFIGURATION_CHECK_CONFIGURATION_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        DATA_BASE_CREATE_START,
        DATA_BASE_CREATE_EXECUTOR_ANSWER_PROCESSING,

        SETTINGS_LOAD_START,
        SETTINGS_LOAD_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        SETTINGS_LOAD_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        MAIN_CYCLE_MODULES_INIT,
        MAIN_CYCLE_MODULES_INIT_END_WAITING,
        MAIN_CYCLE_MODBUS_SLAVE,
        MAIN_CYCLE_START_WAITING,
        MAIN_CYCLE_MODULES_INTERACTION,
        MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING,
        MAIN_CYCLE_END,

        LED_BLINK_ON,
        LED_BLINK_OFF,
    };

    CMainProductionCycle();
    virtual ~CMainProductionCycle();

    void SetInternalModuleMuvrName(const std::string& sName)
    {
        m_sInternalModuleMuvrName = sName;
    };

    void SetConfigurationCreateName(const std::string& sName)
    {
        m_sConfigurationCreateName = sName;
    };

    void SetDataStoreCheckName(const std::string& sName)
    {
        m_sDataStoreCheckName = sName;
    };

    uint8_t Init(void);
//    bool SetTaskData(CDataContainerDataBase* pxDataContainer);
//    bool GetTaskData(CDataContainerDataBase* pxDataContainer);
    void Allocate(void);

//    std::list<CTaskInterface*>* GetCommonTasksListPointer(void)
//    {
//        return &m_lpxCommonTasksList;
//    };
//
//    std::list<CTaskInterface*>* GetCurrentlyRunningTasksListPointer(void)
//    {
//        return &m_lpxCurrentlyRunningTasksList;
//    };

    uint8_t CreateTasks(void);
    uint8_t InitTasks(void);
    void CurrentlyRunningTasksExecution(void);
    uint8_t Fsm(void);

private:
//    std::list<CTaskInterface*> m_lpxCommonTasksList;
//    std::list<CTaskInterface*>::iterator m_xCommonTasksListIterator;
//    std::list<CTaskInterface*> m_lpxCurrentlyRunningTasksList;
//    std::list<CTaskInterface*>::iterator m_xCurrentlyRunningTasksListIterator;
    uint8_t* m_puiIntermediateBuff;

    CResources m_xResources;
//    CDeviceControl m_xDeviceControl;
    CDataStore* m_pxDataStoreFileSystem;
    CDataStoreCheck* m_pxDataStoreCheck;
    CAnalogueSignals* m_pxAnalogueSignals;
    CConfigurationCreate* m_pxConfigurationCreate;

    std::string m_sDeviceControlName;
    uint8_t m_uiDeviceControlId;
    CDeviceControl* m_pxDeviceControl;

    std::string m_sInternalModuleName;
    uint8_t m_uiInternalModuleId;

    std::string m_sInternalModuleMuvrName;
    uint8_t m_uiInternalModuleMuvrId;

    std::string m_sConfigurationCreateName;
    uint8_t m_uiConfigurationCreateId;

    std::string m_sSystemComponentsCreateName;
    uint8_t m_uiSystemComponentsCreateId;

    std::string m_sDataStoreCheckName;
    uint8_t m_uiDataStoreCheckId;

    std::string m_sDataBaseCreateName;
    uint8_t m_uiDataBaseCreateId;

    std::string m_sConfigurationCheckName;
    uint8_t m_uiConfigurationCheckId;

    std::string m_sSettingsLoadName;
    uint8_t m_uiSettingsLoadId;

    CSpiCommunicationDevice* m_pxSpiCommunicationDevice;
    CInternalModuleInterface* m_pxInternalModule;
    CInternalModuleInterface* m_pxInternalModuleMuvr;

    CServiceMarket* m_pxServiceMarket;

    CLedBlinker* m_pxLedBlinker;
    // создадим указатель на объект
    // "производственная площадка задачи ожидания событий"
//    CProductionInterface* m_pxFileDescriptorEventsWaitingProduction;
//    CModbusTcpSlaveTopLevelProduction* m_pxModusTcpSlaveTopLevelProduction;
    CModbusTcpSlaveLinkLayer* m_pxModbusTcpSlaveLinkLayerUpperLevel;
    CModbusSlave* m_pxModbusTcpSlaveUpperLevel;

//    CModbusRtuSlaveTopLevelProduction* m_pxModusRtuSlaveTopLevelProduction;
    CModbusRtuSlaveLinkLayer* m_pxModbusRtuSlaveLinkLayerUpperLevel;
    CModbusSlave* m_pxModbusRtuSlaveUpperLevel;

    CDataContainerDataBase* m_pxOperatingDataContainer;

    uint8_t *m_puiCoils;
    uint8_t *m_puiDiscreteInputs;
    uint16_t *m_puiHoldingRegisters;
    uint16_t *m_puiInputRegisters;
    uint16_t m_uiCoilsNumber;
    uint16_t m_uiDiscreteInputsNumber;
    uint16_t m_uiHoldingRegistersNumber;
    uint16_t m_uiInputRegistersNumber;

    CTimer m_xMainCycle100McTimer;
};
//-------------------------------------------------------------------------------





////-------------------------------------------------------------------------------
//class CLedBlinker : public CTask//, public CDfa
//{
//public:
//
//    enum
//    {
//        BLINK_PERIOD_TIME = 1000,
//    };
//
//    enum
//    {
//        LED_ON = NEXT_STEP,
//        LED_ON_PERIOD_END_WAITING,
//        LED_OFF,
//        LED_OFF_PERIOD_END_WAITING,
//    };
//
//    CLedBlinker();
//    virtual ~CLedBlinker();
//
//    uint8_t Init(void);
//    uint8_t Fsm(void);
//
//private:
//
//};





//-------------------------------------------------------------------------------
class CLedBlinker : public CNewTask
{
public:

    enum
    {
        BLINK_PERIOD_TIME = 1000,
    };

    enum
    {
        NO_SUBTASK = 0,
        STOP = 1,
        START,
        INIT,
        DONE_OK,
        DONE_ERROR,

        LED_ON,
        LED_ON_PERIOD_END_WAITING,
        LED_OFF,
        LED_OFF_PERIOD_END_WAITING,
    };

    CLedBlinker();
    virtual ~CLedBlinker();

    uint8_t Init(void);
    uint8_t Fsm(void);

private:

};


//-------------------------------------------------------------------------------
#endif // CMAINPRODUCTIONCYCLE_H
