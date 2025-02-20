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
class CConfigurationCreate;
class CDataStoreCheck;
//class CDataStore;
//class CDeviceControl;
class CDataContainerInterface;
class CDataContainerDataBase;

//-------------------------------------------------------------------------------
class CMainProductionCycle : public CTask//, public CDfa
{
public:

    enum
    {
        MAIN_CYCLE_SHOW_STATISTICS_PERIOD_TIME = 1000,
    };

    enum
    {
        DATABASE_CHECK_TASK_READY_CHECK = NEXT_STEP,
        DATABASE_CHECK_TASK_READY_WAITING,
        DATABASE_CHECK_BEGIN,
        DATABASE_CHECK_END_WAITING,
        DATABASE_CHECK_RECAVERY_END_WAITING,
        DATABASE_CHECK_END_OK,
        DATABASE_CHECK_END_ERROR,
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

    uint8_t Init(void);
    bool SetTaskData(CDataContainerDataBase* pxDataContainer);
    bool GetTaskData(CDataContainerDataBase* pxDataContainer);
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

    CSpiCommunicationDevice* m_pxSpiCommunicationDevice;
    CInternalModuleInterface* m_pxInternalModule;
    CInternalModuleInterface* m_pxInternalModuleMuvr;

    CConfigurationCreate* m_pxConfigurationCreate;

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
};
//-------------------------------------------------------------------------------





//-------------------------------------------------------------------------------
class CLedBlinker : public CTask//, public CDfa
{
public:

    enum
    {
        BLINK_PERIOD_TIME = 1000,
    };

    enum
    {
        LED_ON = NEXT_STEP,
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
