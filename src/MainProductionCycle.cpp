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
#include "CommunicationDevice.h"
#include "SerialPortCommunicationDevice.h"
#include "Resources.h"
#include "TaskManager.h"
#include "ServiceMarket.h"
#include "StorageDevice.h"
#include "DataStore.h"
#include "DataStoreCheck.h"
#include "InternalModule.h"
#include "InternalModuleMuvr.h"
#include "ConfigurationCreate.h"
//#include "ModbusTcp.h"
#include "ModbusSlaveLinkLayer.h"
#include "ModbusTcpSlaveLinkLayer.h"

#include "EVE_HAL.h"
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

CSpi xSpiCommunicationDevice;


//-------------------------------------------------------------------------------
CMainProductionCycle::CMainProductionCycle()
{
    std::cout << "CMainProductionCycle constructor"  << std::endl;
    // получим имя класса.
    sprintf(m_acTaskName,
            "%s",
            typeid(*this).name());
    SetResources(&m_xResources);
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CMainProductionCycle::~CMainProductionCycle()
{
    std::cout << "CMainProductionCycle destructor"  << std::endl;
    delete m_pxLedBlinker;
//    delete m_pxFileDescriptorEventsWaitingProduction;

    delete m_puiCoils;
    delete m_puiDiscreteInputs;
    delete m_puiHoldingRegisters;
    delete m_puiInputRegisters;

    CGpio::Close();
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
}

//-------------------------------------------------------------------------------
uint8_t CMainProductionCycle::CreateTasks(void)
{
    std::cout << "CMainProductionCycle CreateTasks"  << std::endl;

    m_pxSpiCommunicationDevice = new CSpi();
//-------------------------------------------------------------------------------
    CStorageDeviceFileSystem* pxStorageDeviceFileSystem = new CStorageDeviceFileSystem();
    m_xResources.AddCommonTaskToMap("StorageDeviceFileSystem",
                                    pxStorageDeviceFileSystem);
    pxStorageDeviceFileSystem ->
    SetResources(&m_xResources);
    pxStorageDeviceFileSystem ->
    SetTaskCustomerName("DataStoreFileSystem");
    m_xResources.AddCurrentlyRunningTasksList(pxStorageDeviceFileSystem);

    CDataStore* pxDataStoreFileSystem = new CDataStore();
    m_xResources.AddCommonTaskToMap("DataStoreFileSystem",
                                    pxDataStoreFileSystem);
    pxDataStoreFileSystem ->
    SetResources(&m_xResources);
    pxDataStoreFileSystem ->
    SetStorageDeviceName("StorageDeviceFileSystem");
    pxStorageDeviceFileSystem ->
    SetTaskCustomerName("DataStoreCheck");
    m_xResources.AddCurrentlyRunningTasksList(pxDataStoreFileSystem);
    m_pxDataStoreFileSystem = pxDataStoreFileSystem;

    CDataStoreCheck* pxDataStoreCheck = new CDataStoreCheck();
    m_xResources.AddCommonTaskToMap("DataStoreCheck",
                                    pxDataStoreCheck);
    pxDataStoreCheck ->
    SetResources(&m_xResources);
    pxDataStoreCheck ->
    SetDataStoreName("DataStoreFileSystem");
    m_xResources.AddCurrentlyRunningTasksList(pxDataStoreCheck);
    m_pxDataStoreCheck = pxDataStoreCheck;


////-------------------------------------------------------------------------------
//    CSerialPortCommunicationDevice* pxSerialPortCommunicationDeviceCom1 =
//        new CSerialPortCommunicationDevice();
//    m_xResources.AddCommonTaskToMap("SerialPortCommunicationDeviceCom1",
//                                    pxSerialPortCommunicationDeviceCom1);
//    pxSerialPortCommunicationDeviceCom1 ->
//    SetResources(&m_xResources);
//
////-------------------------------------------------------------------------------
//    CModbusRtuSlaveLinkLayer* pxModbusRtuSlaveLinkLayerUpperLevel =
//        new CModbusRtuSlaveLinkLayer();
//    m_xResources.AddCommonTaskToMap("ModbusRtuSlaveLinkLayerUpperLevel",
//                                    pxModbusRtuSlaveLinkLayerUpperLevel);
//    pxModbusRtuSlaveLinkLayerUpperLevel ->
//    SetResources(&m_xResources);
//    pxModbusRtuSlaveLinkLayerUpperLevel ->
//    SetCommunicationDeviceName("SerialPortCommunicationDeviceCom1");
//
////-------------------------------------------------------------------------------
//    CModbusSlave* pxModbusRtuSlaveUpperLevel =
//        new CModbusSlave();
//    m_xResources.AddCommonTaskToMap("ModbusRtuSlaveUpperLevel",
//                                    pxModbusRtuSlaveUpperLevel);
//    pxModbusRtuSlaveUpperLevel ->
//    SetResources(&m_xResources);
//    pxModbusRtuSlaveUpperLevel ->
//    SetModbusSlaveLinkLayerName("ModbusRtuSlaveLinkLayerUpperLevel");
}

//-------------------------------------------------------------------------------
uint8_t CMainProductionCycle::InitTasks(void)
{
    std::cout << "CMainProductionCycle Init"  << std::endl;


    CGpio::Init();
    cout << "CGpio::Init" << endl;
//	CPlatform::LedInitialization();
//	cout << "CPlatform::LedInitialization" << endl;
    m_pxSpiCommunicationDevice -> Init();
    cout << "m_pxSpiCommunicationDevice -> Open" << endl;


////-------------------------------------------------------------------------------
//    CSerialPortCommunicationDevice* pxSerialPortCommunicationDeviceCom1 =
//        (CSerialPortCommunicationDevice*)(GetResources() ->
//                                          GetCommonTaskFromMapPointer("SerialPortCommunicationDeviceCom1"));
//
//    pxSerialPortCommunicationDeviceCom1 -> Init();
//    pxSerialPortCommunicationDeviceCom1 -> SetDeviceName("/dev/ttyO1");
//    pxSerialPortCommunicationDeviceCom1 -> SetBaudRate(9600);
//    pxSerialPortCommunicationDeviceCom1 -> SetDataBits(8);
//    pxSerialPortCommunicationDeviceCom1 -> SetParity('N');
//    pxSerialPortCommunicationDeviceCom1 -> SetStopBit(2);
//
////    m_uiGuardTimeout = ((((1000000UL / uiBaudRate) * 8UL * 4UL) / 1000UL) + 1);
//
////-------------------------------------------------------------------------------
//    CModbusSlave* pxModbusRtuSlaveUpperLevel =
//        (CModbusSlave*)(GetResources() ->
//                        GetCommonTaskFromMapPointer("ModbusRtuSlaveUpperLevel"));
//
//    pxModbusRtuSlaveUpperLevel ->
//    SetOwnAddress(17);



//    // Проход по всем элементам std::map с использованием итераторов
//    std::map<std::string, CTaskInterface*>::iterator it;
//
//    while (1)
//    {
//        bool bTaskIsReady = 0;
//
//        for (it = GetResources() -> m_mpxCommonTaskMap.begin();
//                it != GetResources() -> m_mpxCommonTaskMap.end();
//                it++)
//        {
//            if ((it -> second -> Fsm()) < READY)
//            {
//                bTaskIsReady = 0;
//            }
//        }
//    }
}

//-------------------------------------------------------------------------------
void CMainProductionCycle::CurrentlyRunningTasksExecution(void)
{
//    std::cout << "CMainProductionCycle CurrentlyRunningTasksExecution"  << std::endl;

//        std::list<CTaskInterface*>::iterator xListIterator;
//
//        for(xListIterator =
//                    GetResources() -> m_lpxCurrentlyRunningTasksList.begin();
//                xListIterator !=
//                GetResources() -> m_lpxCurrentlyRunningTasksList.end();
//                xListIterator++)
//        {
//            (*xListIterator) -> Fsm();
//        }

    for(GetResources() -> m_xCurrentlyRunningTasksListIterator =
                GetResources() -> m_lpxCurrentlyRunningTasksList.begin();
            GetResources() -> m_xCurrentlyRunningTasksListIterator !=
            GetResources() -> m_lpxCurrentlyRunningTasksList.end();
            GetResources() -> m_xCurrentlyRunningTasksListIterator++)
    {
        (*(GetResources() -> m_xCurrentlyRunningTasksListIterator)) -> Fsm();
    }
}

//-------------------------------------------------------------------------------
uint8_t CMainProductionCycle::Fsm(void)
{
//        std::cout << "CMainProductionCycle::Fsm 1"  << std::endl;

    switch (GetFsmState())
    {
    case IDDLE:
        //std::cout << "CMainProductionCycle::Fsm IDDLE"  << std::endl;
        usleep(1000);
        break;

    case STOP:
//        //std::cout << "CMainProductionCycle::Fsm STOP"  << std::endl;
//        SetFsmState(START);
        break;

    case START:
        std::cout << "CMainProductionCycle::Fsm START"  << std::endl;
        std::cout << "m_acTaskName " << m_acTaskName << std::endl;
        CreateTasks();

//        m_pxInternalModule ->
//        GetModuleType(0);
//        {
//
////            CInternalModuleMuvr xInternalModuleMuvr;
//////            CInternalModule xInternalModuleMuvr;
////            xInternalModuleMuvr.
////            GetModuleType(0);
//
////            CInternalModuleMuvr xInternalModuleMuvr;
//////            CInternalModule xInternalModuleMuvr;
////            xInternalModuleMuvr.
////            DataBaseRead(0);
//            m_pxInternalModuleMuvr ->
//            DataBaseRead(0);
//
//        }

//        EVE_HAL::InitializeNEW(true);

//        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CMainProductionCycle::Fsm INIT"  << std::endl;
        InitTasks();
        SetFsmState(READY);
        break;

    case READY:
        std::cout << "CMainProductionCycle::Fsm READY"  << std::endl;
        SetFsmState(DATABASE_CHECK_TASK_READY_CHECK);
        break;

    case DATABASE_CHECK_TASK_READY_CHECK:
//        std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_TASK_READY_CHECK"  << std::endl;
        CurrentlyRunningTasksExecution();

        if ((m_pxDataStoreCheck -> GetFsmState()) == READY)
        {
            SetFsmState(DATABASE_CHECK_BEGIN);
            std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_TASK_READY_CHECK 1"  << std::endl;
        }
        else
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(DATABASE_CHECK_TASK_READY_WAITING);
            std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_TASK_READY_CHECK 2"  << std::endl;
        }
        break;

    case DATABASE_CHECK_TASK_READY_WAITING:
//        std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_TASK_READY_WAITING"  << std::endl;
        CurrentlyRunningTasksExecution();

        if ((m_pxDataStoreCheck -> GetFsmState()) == READY)
        {
            SetFsmState(DATABASE_CHECK_BEGIN);
            std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_TASK_READY_WAITING 1"  << std::endl;
        }
        else
        {
            if (GetTimerPointer() -> IsOverflow())
            {
                SetFsmState(STOP);
                std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_TASK_READY_WAITING 2"  << std::endl;
            }
        }
        break;

    case DATABASE_CHECK_BEGIN:
        std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_BEGIN"  << std::endl;
        CurrentlyRunningTasksExecution();
//        m_pxDataStoreCheck -> Check();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(DATABASE_CHECK_END_WAITING);

        m_pxDataStoreFileSystem -> CreateServiceSection();
        m_pxDataStoreFileSystem -> WriteBlock(auiTempBlock, sizeof(auiTempBlock), 0);
//        if (!(m_pxDataStoreCheck -> Check()))
//        {
//            cout << "DataStore check error" << endl;
//            cout << "CreateServiceSection" << endl;
//            m_pxDataStoreFileSystem -> CreateServiceSection();
//
//            m_pxDataStoreFileSystem -> WriteBlock(auiTempBlock, sizeof(auiTempBlock), 0);
//            do
//            {
//                m_pxDataStoreFileSystem -> Fsm();
//            }
//            while (m_pxDataStoreFileSystem -> GetFsmState() != READY);
//
//
//            cout << "DataStore initialized ok" << endl;
//        }
//        else
//        {
//            cout << "DataStore check ok" << endl;
//        }
//        usleep(1000);
        break;

    case DATABASE_CHECK_END_WAITING:
//        std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_END_WAITING"  << std::endl;
        CurrentlyRunningTasksExecution();

//        if ((m_pxDataStoreCheck -> GetFsmState()) == CDataStoreCheck::DATA_STORE_CHECK_ERROR)
//        {
//        std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_END_WAITING 1"  << std::endl;
//            m_pxDataStoreFileSystem -> CreateServiceSection();
//            m_pxDataStoreFileSystem -> WriteBlock(auiTempBlock, sizeof(auiTempBlock), 0);
//            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
//            SetFsmState(DATABASE_CHECK_RECAVERY_END_WAITING);
////            std::cout << "CDataStore::Fsm DATABASE_CHECK_END_ERROR"  << std::endl;
//        }
//        else if (((m_pxDataStoreCheck -> GetFsmState()) == CDataStoreCheck::DATA_STORE_NEW_VERSION_ACCEPTED) ||
//                 ((m_pxDataStoreCheck -> GetFsmState()) == CDataStoreCheck::DATA_STORE_OLD_VERSION_ACCEPTED) ||
//                 ((m_pxDataStoreCheck -> GetFsmState()) == CDataStoreCheck::DATA_STORE_CHECK_OK))
//        {
//        std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_END_WAITING 2"  << std::endl;
//            SetFsmState(DATABASE_CHECK_END_OK);
////            std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_END_OK"  << std::endl;
//        }
//        else
//        {
//            if (GetTimerPointer() -> IsOverflow())
//            {
//                SetFsmState(STOP);
////                std::cout << "CMainProductionCycle::Fsm STOP"  << std::endl;
//        std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_END_WAITING 3"  << std::endl;
//            }
//        }
        break;

    case DATABASE_CHECK_RECAVERY_END_WAITING:
        std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_RECAVERY_END_WAITING"  << std::endl;
        CurrentlyRunningTasksExecution();

        if ((m_pxDataStoreCheck -> GetFsmState()) == CDataStoreCheck::DATA_STORE_CHECK_ERROR)
        {
            SetFsmState(DATABASE_CHECK_END_ERROR);
            std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_RECAVERY_END_WAITING 1"  << std::endl;
        }
        else if (((m_pxDataStoreCheck -> GetFsmState()) == CDataStoreCheck::DATA_STORE_NEW_VERSION_ACCEPTED) ||
                 ((m_pxDataStoreCheck -> GetFsmState()) == CDataStoreCheck::DATA_STORE_OLD_VERSION_ACCEPTED) ||
                 ((m_pxDataStoreCheck -> GetFsmState()) == CDataStoreCheck::DATA_STORE_CHECK_OK))
        {
            SetFsmState(DATABASE_CHECK_END_OK);
            std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_RECAVERY_END_WAITING 2"  << std::endl;
        }
        else
        {
            if (GetTimerPointer() -> IsOverflow())
            {
                SetFsmState(DATABASE_CHECK_END_ERROR);
                std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_RECAVERY_END_WAITING 3"  << std::endl;
            }
        }
        break;

    case DATABASE_CHECK_END_OK:
//        std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_END_OK"  << std::endl;
        CurrentlyRunningTasksExecution();
        break;

    case DATABASE_CHECK_END_ERROR:
//        std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_END_ERROR"  << std::endl;
        CurrentlyRunningTasksExecution();
        break;

    case MAIN_CYCLE_MODBUS_SLAVE:
//        std::cout << "CMainProductionCycle::Fsm MAIN_CYCLE_MODBUS_SLAVE"  << std::endl;
//        m_pxModbusTcpSlaveUpperLevel -> Fsm();
//        m_pxModbusRtuSlaveUpperLevel -> Fsm();
        CurrentlyRunningTasksExecution();

        usleep(1000);
        break;

    case LED_BLINK_ON:
//        //std::cout << "CMainProductionCycle::Fsm LED_BLINK_ON"  << std::endl;
//        m_pxLedBlinker -> Fsm();
        usleep(1000);
        break;

    case LED_BLINK_OFF:
//        //std::cout << "CMainProductionCycle::Fsm LED_BLINK_ON"  << std::endl;
        SetFsmState(START);
        break;

    default:
        break;
    }

    return GetFsmState();
}

//-------------------------------------------------------------------------------




//-------------------------------------------------------------------------------
CLedBlinker::CLedBlinker()
{
    std::cout << "CLedBlinker constructor"  << std::endl;
    SetFsmState(START);
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
    case START:
        //std::cout << "CLedBlinker::Fsm START"  << std::endl;
        SetFsmState(LED_ON);
        break;

    case READY:
        //std::cout << "CLedBlinker::Fsm READY"  << std::endl;
        break;

    case IDDLE:
        //std::cout << "CLedBlinker::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CLedBlinker::Fsm STOP"  << std::endl;
        SetFsmState(START);
        break;

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
