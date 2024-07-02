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
//#include "ModbusTcp.h"
#include "ModbusSlaveLinkLayer.h"
#include "ModbusTcpSlaveLinkLayer.h"

//class CMainThreadProduction;


//-------------------------------------------------------------------------------
CMainProductionCycle::CMainProductionCycle()
{
    std::cout << "CMainProductionCycle constructor"  << std::endl;
    // получим имя класса.
    sprintf(m_acTaskName,
            "%s",
            typeid(*this).name());
    GetCurrentlyRunningTasksListPointer() -> clear();

    m_pxLedBlinker = new CLedBlinker();
    m_pxModbusTcpSlaveLinkLayer = new CModbusTcpSlaveLinkLayer();
    m_pxModbusSlave = new CModbusSlave();
    m_pxModbusSlave ->
    SetModbusSlaveLinkLayer(m_pxModbusTcpSlaveLinkLayer);

//    COILS_WORK_ARRAY_LENGTH = 128,
//    DISCRETE_INPUTS_ARRAY_LENGTH = 128,
//    HOLDING_REGISTERS_ARRAY_LENGTH = 128,
//    INPUT_REGISTERS_ARRAY_LENGTH = 128,
//    m_puiCoils = COILS_WORK_ARRAY_LENGTH;
//    m_puiDiscreteInputs;
//    m_pui16HoldingRegisters;
//    m_pui16InputRegisters;
//    m_uiCoilsNumber = COILS_WORK_ARRAY_LENGTH;
//    m_uiDiscreteInputsNumber = DISCRETE_INPUTS_ARRAY_LENGTH;
//    m_uiHoldingRegistersNumber = HOLDING_REGISTERS_ARRAY_LENGTH;
//    m_uiInputRegistersNumber = INPUT_REGISTERS_ARRAY_LENGTH;

    m_pxModbusSlave ->
    WorkingArraysCreate(COILS_WORK_ARRAY_LENGTH,
                        DISCRETE_INPUTS_ARRAY_LENGTH,
                        HOLDING_REGISTERS_ARRAY_LENGTH,
                        INPUT_REGISTERS_ARRAY_LENGTH);

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
    delete m_pxModusTcpSlaveTopLevelProduction;
    delete m_pxModbusTcpSlaveLinkLayer;
    delete m_pxModbusSlave;
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

    m_pxModusTcpSlaveTopLevelProduction = new CModbusTcpSlaveTopLevelProduction();
    m_pxModusTcpSlaveTopLevelProduction ->
    SetModbusSlaveLinkLayer(m_pxModbusTcpSlaveLinkLayer);
    m_pxModusTcpSlaveTopLevelProduction ->
    Place((CTaskInterface*)m_pxModbusTcpSlaveLinkLayer);

    m_pxModbusSlave ->
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
        m_pxModbusSlave -> Fsm();
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
