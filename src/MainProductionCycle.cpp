//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <iostream>
#include <string.h>

#include "Platform.h"
#include "MainProductionCycle.h"
#include "Resources.h"
#include "DataStore.h"
#include "DataStoreCheck.h"
#include "InternalModule.h"
#include "InternalModuleMuvr.h"
#include "ConfigurationCreate.h"
//#include "ModbusTcp.h"
#include "ModbusSlaveLinkLayer.h"
#include "ModbusTcpSlaveLinkLayer.h"

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


//-------------------------------------------------------------------------------
CMainProductionCycle::CMainProductionCycle()
{
    std::cout << "CMainProductionCycle constructor"  << std::endl;
    // получим имя класса.
    sprintf(m_acTaskName,
            "%s",
            typeid(*this).name());
    GetCurrentlyRunningTasksListPointer() -> clear();



//    CDataStore* pxDataStoreFileSystem = new CDataStore(new CStorageDeviceFileSystem);
//    CDataStoreCheck* pxDataStoreCheck = new CDataStoreCheck(pxDataStoreFileSystem);
////    CDataStoreCheck xDataStoreCheck(pxDataStoreFileSystem);
//
//    if (!(pxDataStoreCheck -> Check()))
//    {
//        cout << "DataStore check error" << endl;
//        cout << "CreateServiceSection" << endl;
//        pxDataStoreFileSystem -> CreateServiceSection();
//
//        pxDataStoreFileSystem -> WriteBlock(auiTempBlock, sizeof(auiTempBlock), 0);
//        do
//        {
//            pxDataStoreFileSystem -> Fsm();
//        }
//        while (pxDataStoreFileSystem -> GetFsmState() != CDataStore::IDDLE);
//
//
//        pxDataStoreFileSystem -> WriteBlock(auiTempBlock, sizeof(auiTempBlock), 1);
//        do
//        {
//            pxDataStoreFileSystem -> Fsm();
//        }
//        while (pxDataStoreFileSystem -> GetFsmState() != CDataStore::IDDLE);
//
//        pxDataStoreFileSystem -> WriteBlock(auiTempBlock, sizeof(auiTempBlock), 2);
//        do
//        {
//            pxDataStoreFileSystem -> Fsm();
//        }
//        while (pxDataStoreFileSystem -> GetFsmState() != CDataStore::IDDLE);
//
//        pxDataStoreFileSystem -> WriteBlock(auiTempBlock, sizeof(auiTempBlock), 3);
//        do
//        {
//            pxDataStoreFileSystem -> Fsm();
//        }
//        while (pxDataStoreFileSystem -> GetFsmState() != CDataStore::IDDLE);
//
//        pxDataStoreFileSystem -> WriteBlock(auiTempBlock, sizeof(auiTempBlock), 4);
//        do
//        {
//            pxDataStoreFileSystem -> Fsm();
//        }
//        while (pxDataStoreFileSystem -> GetFsmState() != CDataStore::IDDLE);
//
//
//        cout << "DataStore initialized ok" << endl;
//    }
//    else
//    {
//        cout << "DataStore check ok" << endl;
//    }



//    if (!(pxDataStoreFileSystem -> ReadServiceSection()))
//    {
//        cout << "CreateServiceSection" << endl;
//        pxDataStoreFileSystem -> CreateServiceSection();
//    }



//    pxDataStoreFileSystem -> BlockWritePrepare(auiTempBlock, sizeof(auiTempBlock), 0);

////    pxDataStoreFileSystem -> SetFsmEvent(CDataStore::WRITE_IN_PROGRESS_FSM_EVENT);
//    // Запустим процесс записи.
//    pxDataStoreFileSystem -> SetFsmState(CDataStore::START_WRITE_TEMPORARY_SERVICE_SECTION_DATA);

//    pxDataStoreFileSystem -> TemporaryServiceSectionWritePrepare();




//    uint8_t auiTempArray[512];
//
//    if (pxDataStoreFileSystem -> ReadBlock(auiTempArray, 0))
//    {
//        cerr << "ReadBlock ok" << endl;
//    }
//    else
//    {
//        cerr << "ReadBlock error" << endl;
//    }




    m_pxLedBlinker = new CLedBlinker();

    m_uiCoilsNumber = COILS_WORK_ARRAY_LENGTH;
    m_uiDiscreteInputsNumber = DISCRETE_INPUTS_ARRAY_LENGTH;
    m_uiHoldingRegistersNumber = HOLDING_REGISTERS_ARRAY_LENGTH;
    m_uiInputRegistersNumber = INPUT_REGISTERS_ARRAY_LENGTH;

    m_puiCoils = new uint8_t(m_uiCoilsNumber);
    m_puiDiscreteInputs = new uint8_t(m_uiDiscreteInputsNumber);
    m_puiHoldingRegisters = new uint16_t(m_uiHoldingRegistersNumber);
    m_puiInputRegisters = new uint16_t(m_uiInputRegistersNumber);

    m_xResources.SetCoils(m_puiCoils);
    m_xResources.SetCoilsNumber(m_uiCoilsNumber);

    m_xResources.SetDiscreteInputs(m_puiDiscreteInputs);
    m_xResources.SetDiscreteInputsNumber(m_uiDiscreteInputsNumber);

    m_xResources.SetHoldingRegisters(m_puiHoldingRegisters);
    m_xResources.SetHoldingRegistersNumber(m_uiHoldingRegistersNumber);

    m_xResources.SetInputRegisters(m_puiInputRegisters);
    m_xResources.SetInputRegistersNumber(m_uiInputRegistersNumber);

    m_xResources.m_pxDataStore = &m_xDataStore;
    m_xResources.m_pxDeviceControl = &m_xDeviceControl;

    m_pxSpiCommunicationDevice = new CSpi();
    m_pxInternalModule = new CInternalModule();
//    m_pxInternalModule = new CInternalModuleMuvr();
    m_pxInternalModule ->
    SetCommunicationDevice(m_pxSpiCommunicationDevice);
    m_pxInternalModule ->
    SetResources(&m_xResources);

    m_pxInternalModuleMuvr = new CInternalModuleMuvr();
    m_pxInternalModuleMuvr ->
    SetCommunicationDevice(m_pxSpiCommunicationDevice);
    m_pxInternalModuleMuvr ->
    SetResources(&m_xResources);


    m_pxConfigurationCreate = new CConfigurationCreate();
    m_pxConfigurationCreate ->
    SetResources(&m_xResources);
    m_xResources.m_pxConfigurationCreate = m_pxConfigurationCreate;

    m_pxModbusTcpSlaveLinkLayerUpperLevel = new CModbusTcpSlaveLinkLayer();
    m_pxModbusTcpSlaveUpperLevel = new CModbusSlave();
    m_pxModbusTcpSlaveUpperLevel ->
    SetModbusSlaveLinkLayer(m_pxModbusTcpSlaveLinkLayerUpperLevel);

//    m_pxModbusTcpSlaveUpperLevel ->
//    WorkingArraysCreate(COILS_WORK_ARRAY_LENGTH,
//                        DISCRETE_INPUTS_ARRAY_LENGTH,
//                        HOLDING_REGISTERS_ARRAY_LENGTH,
//                        INPUT_REGISTERS_ARRAY_LENGTH);
    m_pxModbusTcpSlaveUpperLevel ->
    SetOwnAddress(17);
    m_pxModbusTcpSlaveUpperLevel ->
    SetResources(&m_xResources);




    m_pxModbusRtuSlaveLinkLayerUpperLevel = new CModbusRtuSlaveLinkLayer();
    m_pxModbusRtuSlaveUpperLevel = new CModbusSlave();
    m_pxModbusRtuSlaveUpperLevel ->
    SetModbusSlaveLinkLayer(m_pxModbusRtuSlaveLinkLayerUpperLevel);

//    m_pxModbusRtuSlaveUpperLevel ->
//    CommunicationDeviceInit("/dev/ttyO1",
//                            9600,
//                            8,
//                            'N',
//                            2);

    m_pxModbusRtuSlaveUpperLevel ->
    WorkingArraysCreate(COILS_WORK_ARRAY_LENGTH,
                        DISCRETE_INPUTS_ARRAY_LENGTH,
                        HOLDING_REGISTERS_ARRAY_LENGTH,
                        INPUT_REGISTERS_ARRAY_LENGTH);
    m_pxModbusRtuSlaveUpperLevel ->
    SetOwnAddress(17);
    m_pxModbusRtuSlaveUpperLevel ->
    SetResources(&m_xResources);

    m_xDataStore.
    SetResources(&m_xResources);
    m_xDeviceControl.
    SetResources(&m_xResources);

//    m_pxRootTask ->
//    AddCommonTask(this);
//    m_pxRootTask ->
//    AddCustomer(this);
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

    delete m_pxModusTcpSlaveTopLevelProduction;
    delete m_pxModbusTcpSlaveLinkLayerUpperLevel;
    delete m_pxModbusTcpSlaveUpperLevel;

    delete m_pxModusRtuSlaveTopLevelProduction;
    delete m_pxModbusRtuSlaveLinkLayerUpperLevel;
    delete m_pxModbusRtuSlaveUpperLevel;
}

//-------------------------------------------------------------------------------
uint8_t CMainProductionCycle::Init(void)
{
    std::cout << "CMainProductionCycle Init"  << std::endl;

////    // создадим указатель на объект
////    // "производственная площадка задачи ожидания событий"
////    CProductionInterface* m_pxFileDescriptorEventsWaitingProduction;
//    // создадим объект "производственная площадка задачи ожидания событий"
//    m_pxFileDescriptorEventsWaitingProduction = new CFileDescriptorEventsWaitingProduction();
////    // добавим задачу "задача ожидания событий"
////    // в список исполняемых на производственной площадке
////    m_pxFileDescriptorEventsWaitingProduction ->
////    AddCustomer((CTaskInterface*)pxFileDescriptorEventsWaiting);
////    m_pxFileDescriptorEventsWaitingProduction ->
////    AddTask((CTaskInterface*)pxFileDescriptorEventsWaiting);
//    // разместим задачу "производственная площадка задачи ожидания событий"
//    // на производственной площадке(запустим поток)
//    m_pxFileDescriptorEventsWaitingProduction ->
//    Place((CTaskInterface*)m_pxFileDescriptorEventsWaitingProduction);

    CGpio::Init();
    cout << "CGpio::Init" << endl;
//	CPlatform::LedInitialization();
//	cout << "CPlatform::LedInitialization" << endl;
    m_pxSpiCommunicationDevice -> Init();
    cout << "m_pxSpiCommunicationDevice -> Open" << endl;

    m_pxModusTcpSlaveTopLevelProduction = new CModbusTcpSlaveTopLevelProduction();
    m_pxModusTcpSlaveTopLevelProduction ->
    SetModbusSlaveLinkLayer(m_pxModbusTcpSlaveLinkLayerUpperLevel);
    m_pxModusTcpSlaveTopLevelProduction ->
    Place((CTaskInterface*)m_pxModbusTcpSlaveLinkLayerUpperLevel);

    m_pxModbusTcpSlaveUpperLevel ->
    SetFsmState(CModbusSlave::COMMUNICATION_START);


    m_pxModusRtuSlaveTopLevelProduction = new CModbusRtuSlaveTopLevelProduction();
    m_pxModusRtuSlaveTopLevelProduction ->
    SetModbusSlaveLinkLayer(m_pxModbusRtuSlaveLinkLayerUpperLevel);
    m_pxModusRtuSlaveTopLevelProduction ->
    Place((CTaskInterface*)m_pxModbusRtuSlaveLinkLayerUpperLevel);

    m_pxModbusRtuSlaveUpperLevel ->
    SetFsmState(CModbusSlave::COMMUNICATION_START);
}

//-------------------------------------------------------------------------------
uint8_t CMainProductionCycle::Fsm(void)
{
//        std::cout << "CMainProductionCycle::Fsm 1"  << std::endl;

    switch (GetFsmState())
    {
    case START:
        std::cout << "CMainProductionCycle::Fsm START"  << std::endl;
        std::cout << "m_acTaskName " << m_acTaskName << std::endl;
        Init();

        m_pxInternalModule ->
        GetModuleType(0);
        {

//            CInternalModuleMuvr xInternalModuleMuvr;
////            CInternalModule xInternalModuleMuvr;
//            xInternalModuleMuvr.
//            GetModuleType(0);

//            CInternalModuleMuvr xInternalModuleMuvr;
////            CInternalModule xInternalModuleMuvr;
//            xInternalModuleMuvr.
//            DataBaseRead(0);
            m_pxInternalModuleMuvr ->
            DataBaseRead(0);

        }

        SetFsmState(MAIN_CYCLE_MODBUS_SLAVE);
        break;

    case READY:
        //std::cout << "CMainProductionCycle::Fsm READY"  << std::endl;
        break;

    case IDDLE:
        //std::cout << "CMainProductionCycle::Fsm IDDLE"  << std::endl;
        usleep(1000);
        break;

    case STOP:
//        //std::cout << "CMainProductionCycle::Fsm STOP"  << std::endl;
        SetFsmState(START);
        break;

    case MAIN_CYCLE_MODBUS_SLAVE:
        //std::cout << "CMainProductionCycle::Fsm IDDLE"  << std::endl;
//        m_pxModbusTcpSlaveUpperLevel -> Fsm();
        m_pxModbusRtuSlaveUpperLevel -> Fsm();
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
        m_xTimer.Set(500);
        SetFsmState(LED_ON_PERIOD_END_WAITING);
        break;

    case LED_ON_PERIOD_END_WAITING:
//        //std::cout << "CLedBlinker::Fsm LED_ON_PERIOD_END_WAITING"  << std::endl;
        if (m_xTimer.IsOverflow())
        {
            SetFsmState(LED_OFF);
        }
        break;

    case LED_OFF:
        std::cout << "CLedBlinker::Fsm LED_OFF"  << std::endl;
        m_xTimer.Set(500);
        SetFsmState(LED_OFF_PERIOD_END_WAITING);
        break;

    case LED_OFF_PERIOD_END_WAITING:
//        //std::cout << "CLedBlinker::Fsm LED_OFF_PERIOD_END_WAITING"  << std::endl;
        if (m_xTimer.IsOverflow())
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
