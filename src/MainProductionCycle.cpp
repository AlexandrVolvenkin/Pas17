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
//#include "ModbusTcp.h"
#include "ModbusSlaveLinkLayer.h"
#include "ModbusTcpSlaveLinkLayer.h"
#include "Link.h"
#include "DataContainer.h"

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

    m_xResources.ModbusWorkingArraysDestroy();

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

//    m_pxSpiCommunicationDevice = new CSpiCommunicationDevice();

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

////-------------------------------------------------------------------------------
//    CInternalModule* pxInternalModuleCommon = 0;
//    pxInternalModuleCommon =
//        static_cast<CInternalModule*>(m_xResources.AddCommonTaskToMap("InternalModuleCommon",
//                                      std::make_shared<CInternalModule>()));
//    pxInternalModuleCommon ->
//    SetResources(&m_xResources);
//    pxInternalModuleCommon ->
//    SetCommunicationDeviceName("SpiCommunicationDeviceSpi0");
//    m_xResources.AddCurrentlyRunningTasksList(pxInternalModuleCommon);
//    m_pxInternalModule = pxInternalModuleCommon;

//-------------------------------------------------------------------------------
    CInternalModuleMuvr* pxInternalModuleMuvr = 0;
    pxInternalModuleMuvr =
        static_cast<CInternalModuleMuvr*>(m_xResources.AddCommonTaskToMap("InternalModuleMuvr",
                                          std::make_shared<CInternalModuleMuvr>()));
    pxInternalModuleMuvr ->
    SetResources(&m_xResources);
    pxInternalModuleMuvr ->
    SetCommunicationDeviceName("SpiCommunicationDeviceSpi0");
    m_xResources.AddCurrentlyRunningTasksList(pxInternalModuleMuvr);
    m_pxInternalModuleMuvr = pxInternalModuleMuvr;


}

//-------------------------------------------------------------------------------
uint8_t CMainProductionCycle::InitTasks(void)
{
    std::cout << "CMainProductionCycle Init"  << std::endl;

    CGpio::Init();
    cout << "CGpio::Init" << endl;


//-------------------------------------------------------------------------------
    CSerialPortCommunicationDevice* pxSerialPortCommunicationDeviceCom1 =
        (CSerialPortCommunicationDevice*)(GetResources() ->
                                          GetTaskPointerByNameFromMap("SerialPortCommunicationDeviceCom1"));

    pxSerialPortCommunicationDeviceCom1 -> Init();
    pxSerialPortCommunicationDeviceCom1 -> SetDeviceName("/dev/ttyO1");
    pxSerialPortCommunicationDeviceCom1 -> SetBaudRate(9600);
    pxSerialPortCommunicationDeviceCom1 -> SetDataBits(8);
    pxSerialPortCommunicationDeviceCom1 -> SetParity('N');
    pxSerialPortCommunicationDeviceCom1 -> SetStopBit(2);

//-------------------------------------------------------------------------------
    CModbusSlave* pxModbusRtuSlaveUpperLevel =
        (CModbusSlave*)(GetResources() ->
                        GetTaskPointerByNameFromMap("ModbusRtuSlaveUpperLevel"));

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

//-------------------------------------------------------------------------------
    CInternalModuleMuvr* pxInternalModuleMuvr =
        (CInternalModuleMuvr*)(GetResources() ->
                               GetTaskPointerByNameFromMap("InternalModuleMuvr"));
    pxInternalModuleMuvr ->
    SetAddress(0);

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

//        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CMainProductionCycle::Fsm INIT"  << std::endl;
        if (GetResources() -> CheckCommonTaskMap())
        {
            InitTasks();
            SetFsmState(READY);
        }
        else
        {
            SetFsmState(STOP);
        }
        break;

    case READY:
        std::cout << "CMainProductionCycle::Fsm READY"  << std::endl;
        SetFsmState(DATABASE_CHECK_TASK_READY_CHECK);
        break;

    case DATABASE_CHECK_TASK_READY_CHECK:
//        std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_TASK_READY_CHECK"  << std::endl;
        CurrentlyRunningTasksExecution();

        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(DATABASE_CHECK_TASK_READY_WAITING);
        std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_TASK_READY_CHECK 1"  << std::endl;

//        if ((m_pxDataStoreCheck -> GetFsmState()) == READY)
//        {
//            SetFsmState(DATABASE_CHECK_BEGIN);
//            std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_TASK_READY_CHECK 1"  << std::endl;
//        }
//        else
//        {
//            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
//            SetFsmState(DATABASE_CHECK_TASK_READY_WAITING);
//            std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_TASK_READY_CHECK 2"  << std::endl;
//        }
        break;

    case DATABASE_CHECK_TASK_READY_WAITING:
//        std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_TASK_READY_WAITING"  << std::endl;
        CurrentlyRunningTasksExecution();

        if ((m_pxDataStoreCheck -> GetFsmState()) >= READY)
        {
//            SetFsmState(MAIN_CYCLE_MODBUS_SLAVE);
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
        m_pxDataStoreCheck -> Check();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);

//        m_pxDataStoreFileSystem -> CreateServiceSection();
//        m_pxDataStoreFileSystem -> WriteBlock(auiTempBlock, sizeof(auiTempBlock), 0);

        SetFsmState(DATABASE_CHECK_END_WAITING);
//
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

        if ((m_pxDataStoreCheck -> GetFsmOperationStatus()) == CDataStoreCheck::DATA_STORE_CHECK_ERROR)
        {
            std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_END_WAITING 1"  << std::endl;
            m_pxDataStoreFileSystem -> CreateServiceSection();
            m_pxDataStoreFileSystem -> WriteBlock(auiTempBlock, sizeof(auiTempBlock), 0);
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(DATABASE_CHECK_RECAVERY_END_WAITING);
//            std::cout << "CDataStore::Fsm DATABASE_CHECK_END_ERROR"  << std::endl;
        }
        else if (((m_pxDataStoreCheck -> GetFsmOperationStatus()) == CDataStoreCheck::DATA_STORE_NEW_VERSION_ACCEPTED) ||
                 ((m_pxDataStoreCheck -> GetFsmOperationStatus()) == CDataStoreCheck::DATA_STORE_OLD_VERSION_ACCEPTED) ||
                 ((m_pxDataStoreCheck -> GetFsmOperationStatus()) == CDataStoreCheck::DATA_STORE_CHECK_OK))
        {
            std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_END_WAITING 2"  << std::endl;
            SetFsmState(DATABASE_CHECK_END_OK);
//            std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_END_OK"  << std::endl;
        }
        else
        {
            if (GetTimerPointer() -> IsOverflow())
            {
                m_pxDataStoreCheck ->
                SetFsmCommandState(0);
                SetFsmState(STOP);
//                std::cout << "CMainProductionCycle::Fsm STOP"  << std::endl;
                std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_END_WAITING 3"  << std::endl;
            }
        }
        break;

    case DATABASE_CHECK_RECAVERY_END_WAITING:
        std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_RECAVERY_END_WAITING"  << std::endl;
        CurrentlyRunningTasksExecution();

        if ((m_pxDataStoreCheck -> GetFsmOperationStatus()) == CDataStoreCheck::DATA_STORE_CHECK_ERROR)
        {
            SetFsmState(DATABASE_CHECK_END_ERROR);
            std::cout << "CMainProductionCycle::Fsm DATABASE_CHECK_RECAVERY_END_WAITING 1"  << std::endl;
        }
        else if (((m_pxDataStoreCheck -> GetFsmOperationStatus()) == CDataStoreCheck::DATA_STORE_NEW_VERSION_ACCEPTED) ||
                 ((m_pxDataStoreCheck -> GetFsmOperationStatus()) == CDataStoreCheck::DATA_STORE_OLD_VERSION_ACCEPTED) ||
                 ((m_pxDataStoreCheck -> GetFsmOperationStatus()) == CDataStoreCheck::DATA_STORE_CHECK_OK))
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
        SetFsmState(MAIN_CYCLE_MODBUS_SLAVE);
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

        GetTimerPointer() -> Set(100);

        usleep(1000);
        SetFsmState(MAIN_CYCLE_START_WAITING);
        break;

    case MAIN_CYCLE_START_WAITING:
//        std::cout << "CMainProductionCycle::Fsm MAIN_CYCLE_START_WAITING 1"  << std::endl;
        CurrentlyRunningTasksExecution();

        if (GetTimerPointer() -> IsOverflow())
        {
            std::cout << "CMainProductionCycle::Fsm MAIN_CYCLE_START_WAITING 2"  << std::endl;
            GetTimerPointer() -> Set(100);
            SetFsmState(MAIN_CYCLE_MODULES_INTERACTION);
        }

        break;

    case MAIN_CYCLE_MODULES_INTERACTION:
        std::cout << "CMainProductionCycle::Fsm MAIN_CYCLE_MODULES_INTERACTION"  << std::endl;
        {
            CurrentlyRunningTasksExecution();

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
            ////            m_pxInternalModuleMuvr ->
            ////            DataBaseRead(0);
            //            m_pxInternalModuleMuvr ->
            //            GetModuleType(0);
            //
            //        }

//            CDataContainerDataBase xOperatingDataContainer;
//            xOperatingDataContainer.m_uiFsmCommandState =
//                CInternalModuleMuvr::MUVR_GET_MODULE_TYPE;
//            m_pxInternalModuleMuvr ->
//            SetTaskData(&xOperatingDataContainer);

            CDataContainerDataBase xOperatingDataContainer;
            xOperatingDataContainer.m_uiFsmCommandState =
                CInternalModuleMuvr::MUVR_DATA_EXCHANGE;
            m_pxInternalModuleMuvr ->
            SetTaskData(&xOperatingDataContainer);

            SetFsmState(MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING);
        }
        break;

    case MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING:
//        std::cout << "CMainProductionCycle::Fsm MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING"  << std::endl;
        CurrentlyRunningTasksExecution();

        SetFsmState(MAIN_CYCLE_END);
        break;

    case MAIN_CYCLE_END:
//        std::cout << "CMainProductionCycle::Fsm MAIN_CYCLE_END"  << std::endl;
        CurrentlyRunningTasksExecution();

        SetFsmState(MAIN_CYCLE_START_WAITING);
        break;

    default:
        break;
    }

    return GetFsmState();
}

//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
