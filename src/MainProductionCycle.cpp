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
    delete m_pxFileDescriptorEventsWaitingProduction;
}

//-------------------------------------------------------------------------------------------
uint8_t CMainProductionCycle::Init(void)
{
    std::cout << "CMainProductionCycle Init"  << std::endl;

//    // создадим указатель на объект
//    // "производственная площадка задачи ожидания событий"
//    CProductionInterface* m_pxFileDescriptorEventsWaitingProduction;
    // создадим объект "производственная площадка задачи ожидания событий"
    m_pxFileDescriptorEventsWaitingProduction = new CFileDescriptorEventsWaitingProduction();
//    // добавим задачу "задача ожидания событий"
//    // в список исполняемых на производственной площадке
//    m_pxFileDescriptorEventsWaitingProduction ->
//    AddCustomer((CTaskInterface*)pxFileDescriptorEventsWaiting);
//    m_pxFileDescriptorEventsWaitingProduction ->
//    AddTask((CTaskInterface*)pxFileDescriptorEventsWaiting);
    // разместим задачу "производственная площадка задачи ожидания событий"
    // на производственной площадке(запустим поток)
    m_pxFileDescriptorEventsWaitingProduction ->
    Place((CTaskInterface*)m_pxFileDescriptorEventsWaitingProduction);

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
        SetFsmState(LED_BLINK_ON);
        break;

    case READY:
        //std::cout << "CMainProductionCycle::Fsm READY"  << std::endl;
        break;

    case IDDLE:
        //std::cout << "CMainProductionCycle::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CMainProductionCycle::Fsm STOP"  << std::endl;
        SetFsmState(START);
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

//-------------------------------------------------------------------------------------------
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
