//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include "Production.h"
//#include "MainProductionCycle.h"

using namespace std;

class CFileDescriptorEventsWaitingProduction;

//-------------------------------------------------------------------------------
CProduction::CProduction()
{
    std::cout << "CProduction constructor"  << std::endl;

    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CProduction::~CProduction()
{

}
\

//---------------------------------------------------------------------------------
uint8_t CProduction::Init(void)
{
//    // создадим указатель на объект "главной задачи"
//    CFileDescriptorEventsWaitingProduction* pxMainProductionCycle;
//    // создадим объект "главной задачи"
//    pxMainProductionCycle = new CFileDescriptorEventsWaitingProduction(this);
////    pxMainThreadProduction ->
//    AddCustomer(pxMainProductionCycle);
}

//-------------------------------------------------------------------------------
void CProduction::AddTask(CTaskInterface* pxTask)
{
    //std::cout << "CProduction::AddTask"  << std::endl;
    GetExecutingTasksListPointer() -> push_back(pxTask);
}

//-------------------------------------------------------------------------------
void CProduction::Place(CTaskInterface* pxTask)
{
    //std::cout << "CProduction::Place"  << std::endl;
    Process(pxTask);
}

//---------------------------------------------------------------------------------
void CProduction::DeleteComletedTasks(void)
{
    //std::cout << "CProduction::DeleteComletedTasks 1" << endl;

//    if (!(GetCustomersListPointer() -> empty()))
//    {
//        std::list<CTaskInterface*>::iterator xListIterator;
//        //	int i = 0;
//        for(xListIterator = GetCustomersListPointer() -> begin();
//                xListIterator != GetCustomersListPointer() -> end();
//                xListIterator++)
//        {
//            if ((*xListIterator) -> GetFsmState() == CTaskInterface::IDDLE)
//            {
//                GetCustomersListPointer() -> erase(xListIterator);
//            }
//        }
//    }
}

//---------------------------------------------------------------------------------
uint8_t CProduction::Fsm(void)
{
    //std::cout << "CProduction::Fsm 1" << endl;

    //std::cout << "CProduction::Fsm 1"  << std::endl;
    switch (GetFsmState())
    {
    case IDDLE:
        //std::cout << "CProduction::Fsm IDDLE"  << std::endl;
        break;

    case START:
        //std::cout << "CProduction::Fsm START"  << std::endl;
        SetFsmState(MAKE_DEAL_START);
        break;

    case MAKE_DEAL_START:
        std::cout << "CProduction::Fsm MAKE_DEAL_START"  << std::endl;
        SetFsmState(MAKE_DEAL);
        break;

    case MAKE_DEAL:
        //std::cout << "CProduction::Fsm MAKE_DEAL"  << std::endl;
//        if (GetCustomersListPointer() -> empty())
//        {
//            //std::cout << "CProduction::Fsm 2" << endl;
//            usleep(500000);
//            SetFsmState(MAKE_DEAL_START);
//        }
//        else
//        {
//            //std::cout << "CProduction::Fsm 3" << endl;
//            std::list<CTaskInterface*>::iterator xListIterator;
//            //	int i = 0;
//            for(xListIterator = GetCustomersListPointer() -> begin();
//                    xListIterator != GetCustomersListPointer() -> end();
//                    xListIterator++)
//            {
//                //std::cout << "CProduction::Fsm 4" << endl;
//                (*xListIterator) -> Fsm();
//            }
//        }
        break;

    case MAKE_DEAL_END:
        //std::cout << "CProduction::Fsm MAKE_DEAL_END"  << std::endl;
        SetFsmState(MAKE_DEAL_START);
        break;

    default:
        break;
    }

    return GetFsmState();


//    while (1)
//    {
//        if (GetCustomersListPointer() -> empty())
//        {
//            //std::cout << "CProduction::Fsm 2" << endl;
//            usleep(500000);
//        }
//        else
//        {
//            //std::cout << "CProduction::Fsm 3" << endl;
//            std::list<CTaskInterface*>::iterator xListIterator;
//            //	int i = 0;
//            for(xListIterator = GetCustomersListPointer() -> begin();
//                    xListIterator != GetCustomersListPointer() -> end();
//                    xListIterator++)
//            {
//                //std::cout << "CProduction::Fsm 4" << endl;
//                (*xListIterator) -> Fsm();
//            }
//        }
//    }
//
//    return GetFsmState();
}

//-------------------------------------------------------------------------------
void CProduction::Process(CTaskInterface* pxTask)
{
    //std::cout << "CProduction::Process 1" << endl;
    while (1)
    {
        cout << "CProduction::Process 2" << endl;
        pxTask -> Fsm();
        cout << "CProduction::Process 3" << endl;
    }
}

//-------------------------------------------------------------------------------








//extern CProductionInterface* pxMainThreadProduction;

//-------------------------------------------------------------------------------
CMainThreadProduction::CMainThreadProduction()
{
    std::cout << "CMainThreadProduction constructor"  << std::endl;
//    // получим имя класса.
//    sprintf(m_acTaskName,
//            "%s",
//            typeid(*this).name());
//    m_pxRootTask = this;
//    m_pxRootTask ->
//    GetCommonTasksListPointer() -> clear();
//    m_pxRootTask ->
//    GetCustomersListPointer() -> clear();
//    m_pxRootTask ->
//    AddCommonTask(this);
////    m_pxRootTask ->
////    AddCustomer(this);
//    SetFsmState(START);
////    // разместим задачу "главная производственная площадка"
////    // на производственной площадке(работает в главном потоке)
////    // цикл исполнения главной задачи
////    Place(this);
}

//-------------------------------------------------------------------------------
CMainThreadProduction::~CMainThreadProduction()
{
//    std::cout << "CMainThreadProduction desstructor"  << std::endl;
}

//---------------------------------------------------------------------------------
uint8_t CMainThreadProduction::Init(void)
{
    std::cout << "CMainThreadProduction::Init 1" << endl;
//    // создадим указатель на объект "главной задачи"
//    CFileDescriptorEventsWaitingProduction* pxMainProductionCycle;
//    // создадим объект "главной задачи"
//    pxMainProductionCycle = new CFileDescriptorEventsWaitingProduction(this);
////    m_pxRootTask ->
////    AddCommonTask(pxMainProductionCycle);
//    m_pxRootTask ->
//    AddCustomer(pxMainProductionCycle);
}

//-------------------------------------------------------------------------------
void CMainThreadProduction::Place(CTaskInterface* pxTask)
{
    //std::cout << "CMainThreadProduction::Place"  << std::endl;
    Process(pxTask);
}

//-------------------------------------------------------------------------------
void CMainThreadProduction::Process(CTaskInterface* pxTask)
{
    //std::cout << "CMainThreadProduction::Process 1" << endl;
    while (1)
    {
//        cout << "CMainThreadProduction::Process 2" << endl;
        pxTask -> Fsm();
//        cout << "CMainThreadProduction::Process 3" << endl;
    }
}

//-------------------------------------------------------------------------------
uint8_t CMainThreadProduction::MakeDeal(void)
{
//    std::cout << "CMainThreadProduction::MakeDeal 1"  << std::endl;
//    if (!(GetCustomersListPointer() -> empty()))
//    {
////        std::cout << "CMainThreadProduction::MakeDeal 2"  << std::endl;
//        if (((*m_xCustomersListIterator) -> Fsm()) == CTaskInterface::IDDLE)
//        {
////            std::cout << "CMainThreadProduction::MakeDeal 3"  << std::endl;
////            GetCustomersListPointer() -> erase(m_xCustomersListIterator);
////            delete (*m_xCustomersListIterator);
////                SetFsmState(MAIN_CYCLE_RTSP_SERVER_MARKET_CREATE);
//        }
//        else
//        {
//
////        std::cout << "CMainThreadProduction::MakeDeal 4"  << std::endl;
//        }
//        m_xCustomersListIterator++;
//
////        std::cout << "CMainThreadProduction::MakeDeal 5"  << std::endl;
//        if (m_xCustomersListIterator == GetCustomersListPointer() -> end())
//        {
////            std::cout << "CMainThreadProduction::MakeDeal 6"  << std::endl;
//            return MAKE_DEAL_END;
//        }
//
//    }
//    else
//    {
////        std::cout << "CMainThreadProduction::MakeDeal 7"  << std::endl;
//        usleep(500000);
//        return MAKE_DEAL_START;
//    }
//
////    std::cout << "CMainThreadProduction::MakeDeal 8"  << std::endl;
//    return GetFsmState();
}

//---------------------------------------------------------------------------------
uint8_t CMainThreadProduction::Fsm(void)
{
//    std::cout << "CMainThreadProduction::Fsm 1" << endl;
//    ProcessMessage();
//
//    //std::cout << "CMainThreadProduction::Fsm 1"  << std::endl;
//    switch (GetFsmState())
//    {
//    case IDDLE:
//        //std::cout << "CMainThreadProduction::Fsm IDDLE"  << std::endl;
//        break;
//
//    case START:
//        //std::cout << "CMainThreadProduction::Fsm START"  << std::endl;
//        Init();
//        SetFsmState(MAKE_DEAL_START);
//        break;
//
//    case MAKE_DEAL_START:
////        std::cout << "CMainThreadProduction::Fsm MAKE_DEAL_START"  << std::endl;
//        m_xCustomersListIterator = GetCustomersListPointer() -> begin();
//        SetFsmState(MAKE_DEAL);
//        break;
//
//    case MAKE_DEAL:
//        //std::cout << "CMainThreadProduction::Fsm MAKE_DEAL"  << std::endl;
//        SetFsmState(MakeDeal());
//        break;
//
//    case MAKE_DEAL_END:
//        //std::cout << "CMainThreadProduction::Fsm MAKE_DEAL_END"  << std::endl;
//        usleep(1000);
//        SetFsmState(MAKE_DEAL_START);
//        break;
//
//    default:
//        break;
//    }
//
//    return GetFsmState();
}

//-------------------------------------------------------------------------------








//-------------------------------------------------------------------------------
CRtspThreadProduction::CRtspThreadProduction()
{

}

//-------------------------------------------------------------------------------
CRtspThreadProduction::~CRtspThreadProduction()
{

    GetThread() -> join();
}

////-------------------------------------------------------------------------------








//-------------------------------------------------------------------------------
CFileDescriptorEventsWaitingProduction::CFileDescriptorEventsWaitingProduction()
{
    std::cout << "CFileDescriptorEventsWaitingProduction constructor"  << std::endl;
    // получим имя класса.
    sprintf(m_acTaskName,
            "%s",
            typeid(*this).name());
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CFileDescriptorEventsWaitingProduction::~CFileDescriptorEventsWaitingProduction()
{

    GetThread() -> join();
}

//-------------------------------------------------------------------------------
void CFileDescriptorEventsWaitingProduction::Place(CTaskInterface* pxTask)
{
    std::cout << "CFileDescriptorEventsWaitingProduction::Place"  << std::endl;
    std::thread m_xThread(CFileDescriptorEventsWaitingProduction::Process, pxTask);
    std::thread::id th_id = m_xThread.get_id();
    //std::cout << "CFileDescriptorEventsWaitingProduction th_id" << " " << th_id << std::endl;
    // не ждем завершения работы функции
    m_xThread.detach();
}

//---------------------------------------------------------------------------------
uint8_t CFileDescriptorEventsWaitingProduction::Fsm(void)
{
//    std::cout << "CFileDescriptorEventsWaitingProduction::Fsm 1" << endl;
    switch (GetFsmState())
    {
    case START:
        std::cout << "CFileDescriptorEventsWaitingProduction::Fsm START"  << std::endl;
        std::cout << "m_acTaskName " << m_acTaskName << std::endl;
        Init();
        SetFsmState(LED_BLINK_ON);
        break;

    case READY:
        //std::cout << "CFileDescriptorEventsWaitingProduction::Fsm READY"  << std::endl;
        break;

    case IDDLE:
        //std::cout << "CFileDescriptorEventsWaitingProduction::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CFileDescriptorEventsWaitingProduction::Fsm STOP"  << std::endl;
        SetFsmState(START);
        break;

    case LED_ON:
        std::cout << "CFileDescriptorEventsWaitingProduction::Fsm LED_ON"  << std::endl;
        m_xTimer.Set(500);
        SetFsmState(LED_ON_PERIOD_END_WAITING);
        break;

    case LED_ON_PERIOD_END_WAITING:
//        //std::cout << "CFileDescriptorEventsWaitingProduction::Fsm LED_ON_PERIOD_END_WAITING"  << std::endl;
        if (m_xTimer.IsOverflow())
        {
            SetFsmState(LED_OFF);
        }
        break;

    case LED_OFF:
        std::cout << "CFileDescriptorEventsWaitingProduction::Fsm LED_OFF"  << std::endl;
        m_xTimer.Set(500);
        SetFsmState(LED_OFF_PERIOD_END_WAITING);
        break;

    case LED_OFF_PERIOD_END_WAITING:
//        //std::cout << "CFileDescriptorEventsWaitingProduction::Fsm LED_OFF_PERIOD_END_WAITING"  << std::endl;
        if (m_xTimer.IsOverflow())
        {
            SetFsmState(LED_ON);
        }
        break;

    case LED_BLINK_ON:
//        //std::cout << "CFileDescriptorEventsWaitingProduction::Fsm LED_BLINK_ON"  << std::endl;
//        m_pxLedBlinker -> Fsm();
//        usleep(1000);
        SetFsmState(LED_ON);
        break;

    case LED_BLINK_OFF:
//        //std::cout << "CFileDescriptorEventsWaitingProduction::Fsm LED_BLINK_ON"  << std::endl;
        SetFsmState(START);
        break;

    default:
        break;
    }

        usleep(1000);
    return GetFsmState();
//
////    while (1)
////    {
//        if (GetCustomersListPointer() -> empty())
//        {
//            //std::cout << "CFileDescriptorEventsWaitingProduction::Fsm 2" << endl;
//            usleep(500000);
//        }
//        else
//        {
//            //std::cout << "CFileDescriptorEventsWaitingProduction::Fsm 3" << endl;
//            std::list<CTaskInterface*>::iterator xListIterator;
//            //	int i = 0;
//            for(xListIterator = GetCustomersListPointer() -> begin();
//                    xListIterator != GetCustomersListPointer() -> end();
//                    xListIterator++)
//            {
//                //std::cout << "CFileDescriptorEventsWaitingProduction::Fsm 4" << endl;
//                (*xListIterator) -> Fsm();
//            }
//        }
//    //    }
//
//    return GetFsmState();
}

//-------------------------------------------------------------------------------
void CFileDescriptorEventsWaitingProduction::Process(CTaskInterface* pxTask)
{
    cout << "CFileDescriptorEventsWaitingProduction::Process 1" << endl;

    while (1)
    {
        pxTask -> Fsm();
    }
}

//-------------------------------------------------------------------------------








//-------------------------------------------------------------------------------
CSecondThreadProduction::CSecondThreadProduction()
//    : accTaskName{"CSecondThreadProduction"}
{
    std::cout << "CSecondThreadProduction constructor 1"  << std::endl;
    // получим имя класса.
    sprintf(m_acTaskName,
            "%s",
            typeid(*this).name());

//    AddCustomer(this);
    SetFsmState(START);
//    std::thread m_xThread(CSecondThreadProduction::Process, this);
//    // не ждем завершения работы функции
//    m_xThread.detach();
}

//-------------------------------------------------------------------------------
CSecondThreadProduction::CSecondThreadProduction(CTaskInterface* pxRootTask)
//    : accTaskName{"CSecondThreadProduction"}
{
    std::cout << "CSecondThreadProduction constructor 2"  << std::endl;
//    // получим имя класса.
//    sprintf(m_acTaskName,
//            "%s",
//            typeid(*this).name());
//    m_pxRootTask = pxRootTask;
//    GetCustomersListPointer() -> clear();
//    m_pxRootTask ->
//    AddCommonTask(this);
//    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CSecondThreadProduction::~CSecondThreadProduction()
{

}

//---------------------------------------------------------------------------------
uint8_t CSecondThreadProduction::Init(void)
{
//    // создадим указатель на объект "главной задачи"
//    CFileDescriptorEventsWaitingProduction* pxMainProductionCycle;
//    // создадим объект "главной задачи"
//    pxMainProductionCycle = new CFileDescriptorEventsWaitingProduction(this);
////    pxMainThreadProduction ->
//    AddCustomer(pxMainProductionCycle);
}

//---------------------------------------------------------------------------------
uint8_t CSecondThreadProduction::Fsm(void)
{
    std::cout << "CSecondThreadProduction::Fsm 1" << endl;
//    ProcessMessage();
//
//    //std::cout << "CSecondThreadProduction::Fsm 1"  << std::endl;
//    switch (GetFsmState())
//    {
//    case IDDLE:
//        //std::cout << "CSecondThreadProduction::Fsm IDDLE"  << std::endl;
//        break;
//
//    case START:
//        //std::cout << "CSecondThreadProduction::Fsm START"  << std::endl;
////        SetFsmState(MAKE_DEAL_START);
//        break;
//
//    case MAKE_DEAL_START:
//        std::cout << "CSecondThreadProduction::Fsm MAKE_DEAL_START"  << std::endl;
//        SetFsmState(MAKE_DEAL);
//        break;
//
//    case MAKE_DEAL:
//        //std::cout << "CSecondThreadProduction::Fsm MAKE_DEAL"  << std::endl;
//        if (GetCustomersListPointer() -> empty())
//        {
//            //std::cout << "CSecondThreadProduction::Fsm 2" << endl;
//            usleep(500000);
//            SetFsmState(MAKE_DEAL_START);
//        }
//        else
//        {
//            //std::cout << "CSecondThreadProduction::Fsm 3" << endl;
//            std::list<CTaskInterface*>::iterator xListIterator;
//            //	int i = 0;
//            for(xListIterator = GetCustomersListPointer() -> begin();
//                    xListIterator != GetCustomersListPointer() -> end();
//                    xListIterator++)
//            {
//                //std::cout << "CSecondThreadProduction::Fsm 4" << endl;
//                (*xListIterator) -> Fsm();
//            }
//        }
//        break;
//
//    case MAKE_DEAL_END:
//        //std::cout << "CSecondThreadProduction::Fsm MAKE_DEAL_END"  << std::endl;
//        SetFsmState(MAKE_DEAL_START);
//        break;
//
//    default:
//        break;
//    }
//
//    return GetFsmState();
}

//-------------------------------------------------------------------------------
void CSecondThreadProduction::Place(CTaskInterface* pxTask)
{
    //std::cout << "CMainThreadProduction::Place"  << std::endl;
    std::thread m_xThread(CSecondThreadProduction::Process, this);
//    // не ждем завершения работы функции
//    m_xThread.detach();
////    Process(pxTask);
}

//-------------------------------------------------------------------------------
void CSecondThreadProduction::Process(CTaskInterface* pxTask)
{
    cout << "CSecondThreadProduction::Process 1" << endl;

//    while (1)
//    {
//        cout << "CSecondThreadProduction::Process 2" << endl;
//        pxTask -> Fsm();
//        cout << "CSecondThreadProduction::Process 3" << endl;
//            usleep(500000);
//    }
}

//-------------------------------------------------------------------------------








//-------------------------------------------------------------------------------
CModbusTcpSlaveTopLevelProduction::CModbusTcpSlaveTopLevelProduction()
{
    std::cout << "CModbusTcpSlaveTopLevelProduction constructor"  << std::endl;
    // получим имя класса.
    sprintf(m_acTaskName,
            "%s",
            typeid(*this).name());
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CModbusTcpSlaveTopLevelProduction::~CModbusTcpSlaveTopLevelProduction()
{

    GetThread() -> join();
}

//-------------------------------------------------------------------------------
void CModbusTcpSlaveTopLevelProduction::Place(CTaskInterface* pxTask)
{
    std::cout << "CModbusTcpSlaveTopLevelProduction::Place"  << std::endl;
//    std::thread m_xThread(CModbusTcpSlaveTopLevelProduction::Process, pxTask);
    std::thread m_xThread(CModbusTcpSlaveTopLevelProduction::Process, m_pxModbusSlaveLinkLayer);
    std::thread::id th_id = m_xThread.get_id();
    //std::cout << "CModbusTcpSlaveTopLevelProduction th_id" << " " << th_id << std::endl;
    // не ждем завершения работы функции
    m_xThread.detach();
}

//---------------------------------------------------------------------------------
uint8_t CModbusTcpSlaveTopLevelProduction::Fsm(void)
{
//    std::cout << "CModbusTcpSlaveTopLevelProduction::Fsm 1" << endl;
    switch (GetFsmState())
    {
    case START:
        std::cout << "CModbusTcpSlaveTopLevelProduction::Fsm START"  << std::endl;
        std::cout << "m_acTaskName " << m_acTaskName << std::endl;
        Init();
        SetFsmState(MODBUS_SLAVE_LINK_LAYER);
        break;

    case READY:
        //std::cout << "CModbusTcpSlaveTopLevelProduction::Fsm READY"  << std::endl;
        break;

    case IDDLE:
        //std::cout << "CModbusTcpSlaveTopLevelProduction::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CModbusTcpSlaveTopLevelProduction::Fsm STOP"  << std::endl;
        SetFsmState(START);
        break;

    case MODBUS_SLAVE_LINK_LAYER:
        //std::cout << "CMainProductionCycle::Fsm IDDLE"  << std::endl;
//        m_pxModbusTcpSlaveLinkLayer -> Fsm();
        usleep(1000);
        break;

    case LED_ON:
        std::cout << "CModbusTcpSlaveTopLevelProduction::Fsm LED_ON"  << std::endl;
        m_xTimer.Set(500);
        SetFsmState(LED_ON_PERIOD_END_WAITING);
        break;

    case LED_ON_PERIOD_END_WAITING:
//        //std::cout << "CModbusTcpSlaveTopLevelProduction::Fsm LED_ON_PERIOD_END_WAITING"  << std::endl;
        if (m_xTimer.IsOverflow())
        {
            SetFsmState(LED_OFF);
        }
        break;

    case LED_OFF:
        std::cout << "CModbusTcpSlaveTopLevelProduction::Fsm LED_OFF"  << std::endl;
        m_xTimer.Set(500);
        SetFsmState(LED_OFF_PERIOD_END_WAITING);
        break;

    case LED_OFF_PERIOD_END_WAITING:
//        //std::cout << "CModbusTcpSlaveTopLevelProduction::Fsm LED_OFF_PERIOD_END_WAITING"  << std::endl;
        if (m_xTimer.IsOverflow())
        {
            SetFsmState(LED_ON);
        }
        break;

    case LED_BLINK_ON:
//        //std::cout << "CModbusTcpSlaveTopLevelProduction::Fsm LED_BLINK_ON"  << std::endl;
//        m_pxLedBlinker -> Fsm();
//        usleep(1000);
        SetFsmState(LED_ON);
        break;

    case LED_BLINK_OFF:
//        //std::cout << "CModbusTcpSlaveTopLevelProduction::Fsm LED_BLINK_ON"  << std::endl;
        SetFsmState(START);
        break;

    default:
        break;
    }

        usleep(1000);
    return GetFsmState();
//
////    while (1)
////    {
//        if (GetCustomersListPointer() -> empty())
//        {
//            //std::cout << "CModbusTcpSlaveTopLevelProduction::Fsm 2" << endl;
//            usleep(500000);
//        }
//        else
//        {
//            //std::cout << "CModbusTcpSlaveTopLevelProduction::Fsm 3" << endl;
//            std::list<CTaskInterface*>::iterator xListIterator;
//            //	int i = 0;
//            for(xListIterator = GetCustomersListPointer() -> begin();
//                    xListIterator != GetCustomersListPointer() -> end();
//                    xListIterator++)
//            {
//                //std::cout << "CModbusTcpSlaveTopLevelProduction::Fsm 4" << endl;
//                (*xListIterator) -> Fsm();
//            }
//        }
//    //    }
//
//    return GetFsmState();
}

////-------------------------------------------------------------------------------
//void CModbusTcpSlaveTopLevelProduction::Process(CTaskInterface* pxTask)
//{
//    cout << "CModbusTcpSlaveTopLevelProduction::Process 1" << endl;
//
//    while (1)
//    {
////        pxTask -> Fsm();
////        m_pxModbusSlaveLinkLayer -> Fsm();
//        usleep(1000);
//    }
//}

//-------------------------------------------------------------------------------
void CModbusTcpSlaveTopLevelProduction::Process(CModbusSlaveLinkLayerInterface* pxModbusSlaveLinkLayer)
{
    cout << "CModbusTcpSlaveTopLevelProduction::Process 1" << endl;

    while (1)
    {
        pxModbusSlaveLinkLayer -> Fsm();
        usleep(1000);
    }
}

//-------------------------------------------------------------------------------








//-------------------------------------------------------------------------------
CModbusRtuSlaveTopLevelProduction::CModbusRtuSlaveTopLevelProduction()
{
    std::cout << "CModbusRtuSlaveTopLevelProduction constructor"  << std::endl;
    // получим имя класса.
    sprintf(m_acTaskName,
            "%s",
            typeid(*this).name());
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CModbusRtuSlaveTopLevelProduction::~CModbusRtuSlaveTopLevelProduction()
{

    GetThread() -> join();
}

//-------------------------------------------------------------------------------
void CModbusRtuSlaveTopLevelProduction::Place(CTaskInterface* pxTask)
{
    std::cout << "CModbusRtuSlaveTopLevelProduction::Place"  << std::endl;
//    std::thread m_xThread(CModbusRtuSlaveTopLevelProduction::Process, pxTask);
    std::thread m_xThread(CModbusRtuSlaveTopLevelProduction::Process, m_pxModbusSlaveLinkLayer);
    std::thread::id th_id = m_xThread.get_id();
    //std::cout << "CModbusRtuSlaveTopLevelProduction th_id" << " " << th_id << std::endl;
    // не ждем завершения работы функции
    m_xThread.detach();
}

//---------------------------------------------------------------------------------
uint8_t CModbusRtuSlaveTopLevelProduction::Fsm(void)
{
//    std::cout << "CModbusRtuSlaveTopLevelProduction::Fsm 1" << endl;
    switch (GetFsmState())
    {
    case START:
        std::cout << "CModbusRtuSlaveTopLevelProduction::Fsm START"  << std::endl;
        std::cout << "m_acTaskName " << m_acTaskName << std::endl;
        Init();
        SetFsmState(MODBUS_SLAVE_LINK_LAYER);
        break;

    case READY:
        //std::cout << "CModbusRtuSlaveTopLevelProduction::Fsm READY"  << std::endl;
        break;

    case IDDLE:
        //std::cout << "CModbusRtuSlaveTopLevelProduction::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CModbusRtuSlaveTopLevelProduction::Fsm STOP"  << std::endl;
        SetFsmState(START);
        break;

    case MODBUS_SLAVE_LINK_LAYER:
        //std::cout << "CMainProductionCycle::Fsm IDDLE"  << std::endl;
//        m_pxModbusTcpSlaveLinkLayer -> Fsm();
        usleep(1000);
        break;

    case LED_ON:
        std::cout << "CModbusRtuSlaveTopLevelProduction::Fsm LED_ON"  << std::endl;
        m_xTimer.Set(500);
        SetFsmState(LED_ON_PERIOD_END_WAITING);
        break;

    case LED_ON_PERIOD_END_WAITING:
//        //std::cout << "CModbusRtuSlaveTopLevelProduction::Fsm LED_ON_PERIOD_END_WAITING"  << std::endl;
        if (m_xTimer.IsOverflow())
        {
            SetFsmState(LED_OFF);
        }
        break;

    case LED_OFF:
        std::cout << "CModbusRtuSlaveTopLevelProduction::Fsm LED_OFF"  << std::endl;
        m_xTimer.Set(500);
        SetFsmState(LED_OFF_PERIOD_END_WAITING);
        break;

    case LED_OFF_PERIOD_END_WAITING:
//        //std::cout << "CModbusRtuSlaveTopLevelProduction::Fsm LED_OFF_PERIOD_END_WAITING"  << std::endl;
        if (m_xTimer.IsOverflow())
        {
            SetFsmState(LED_ON);
        }
        break;

    case LED_BLINK_ON:
//        //std::cout << "CModbusRtuSlaveTopLevelProduction::Fsm LED_BLINK_ON"  << std::endl;
//        m_pxLedBlinker -> Fsm();
//        usleep(1000);
        SetFsmState(LED_ON);
        break;

    case LED_BLINK_OFF:
//        //std::cout << "CModbusRtuSlaveTopLevelProduction::Fsm LED_BLINK_ON"  << std::endl;
        SetFsmState(START);
        break;

    default:
        break;
    }

        usleep(1000);
    return GetFsmState();
//
////    while (1)
////    {
//        if (GetCustomersListPointer() -> empty())
//        {
//            //std::cout << "CModbusRtuSlaveTopLevelProduction::Fsm 2" << endl;
//            usleep(500000);
//        }
//        else
//        {
//            //std::cout << "CModbusRtuSlaveTopLevelProduction::Fsm 3" << endl;
//            std::list<CTaskInterface*>::iterator xListIterator;
//            //	int i = 0;
//            for(xListIterator = GetCustomersListPointer() -> begin();
//                    xListIterator != GetCustomersListPointer() -> end();
//                    xListIterator++)
//            {
//                //std::cout << "CModbusRtuSlaveTopLevelProduction::Fsm 4" << endl;
//                (*xListIterator) -> Fsm();
//            }
//        }
//    //    }
//
//    return GetFsmState();
}

////-------------------------------------------------------------------------------
//void CModbusRtuSlaveTopLevelProduction::Process(CTaskInterface* pxTask)
//{
//    cout << "CModbusRtuSlaveTopLevelProduction::Process 1" << endl;
//
//    while (1)
//    {
////        pxTask -> Fsm();
////        m_pxModbusSlaveLinkLayer -> Fsm();
//        usleep(1000);
//    }
//}

//-------------------------------------------------------------------------------
void CModbusRtuSlaveTopLevelProduction::Process(CModbusSlaveLinkLayerInterface* pxModbusSlaveLinkLayer)
{
    cout << "CModbusRtuSlaveTopLevelProduction::Process 1" << endl;

    while (1)
    {
        pxModbusSlaveLinkLayer -> Fsm();
        usleep(1000);
    }
}

//-------------------------------------------------------------------------------
