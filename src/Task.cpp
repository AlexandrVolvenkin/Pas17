//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <iostream>
#include <list>
#include <string.h>

#include "Timer.h"
#include "Dfa.h"
#include "Resources.h"
#include "DataContainer.h"
#include "MessageBox.h"
#include "Task.h"

////-------------------------------------------------------------------------------
//CTaskInterface::CTaskInterface()
//{
//    std::cout << "CTaskInterface constructor 1"  << std::endl;
////    SetFsmState(IDDLE);
//}
//
////-------------------------------------------------------------------------------
//CTaskInterface::~CTaskInterface()
//{
//    std::cout << "CTaskInterface destructor"  << std::endl;
//}
//    char* CTaskInterface::GetTaskNamePointer(void) {};
//    std::shared_ptr<CTaskInterface::TArgumentData> CTaskInterface::GetArgumentDataPointer(void) {};
//    void CTaskInterface::SetResources(CResources* pxResources) {};
//    CResources* CTaskInterface::GetResources(void) {};
//    void CTaskInterface::SetTaskCustomerName(std::string sName) {};
//-------------------------------------------------------------------------------
CTask::CTask()
{
    std::cout << "CTask constructor 1"  << std::endl;
//    SetFsmState(IDDLE);
}
//-------------------------------------------------------------------------------
CTask::CTask(CResources* pxResources)
{
    std::cout << "CTask constructor 2"  << std::endl;
//    SetFsmState(IDDLE);
}

//-------------------------------------------------------------------------------
CTask::~CTask()
{
    std::cout << "CTask destructor"  << std::endl;
}

//-------------------------------------------------------------------------------
uint8_t CTask::Fsm(void)
{

}

//-------------------------------------------------------------------------------
uint8_t CTask::GetFsmState(void)
{
    return m_uiFsmState;
}

//-------------------------------------------------------------------------------
void CTask::SetFsmState(uint8_t uiData)
{
    m_uiFsmState = uiData;
}

//-------------------------------------------------------------------------------
CTimer* CTask::GetTimerPointer(void)
{
    return &m_xTimer;
}

//-------------------------------------------------------------------------------
char* CTask::GetTaskNamePointer(void)
{
    return m_acTaskName;
};

////-------------------------------------------------------------------------------
//std::shared_ptr<CTaskInterface::TArgumentData> CTask::GetArgumentDataPointer(void)
//{
//    return m_pxArgumentData;
//};
//
////-------------------------------------------------------------------------------
//std::shared_ptr<CDataContainerInterface> CTask::GetDataContainerPointer(void)
//{
//    return m_pxDataContainer;
//};

//-------------------------------------------------------------------------------
void CTask::SetResources(CResources* pxResources)
{
    m_pxResources = pxResources;
}

//-------------------------------------------------------------------------------
CResources* CTask::GetResources(void)
{
    return m_pxResources;
}

//-------------------------------------------------------------------------------
void CTask::SetTaskCustomerName(std::string sName)
{
    m_sTaskCustomerName = sName;
}

//-------------------------------------------------------------------------------
void CTask::SetMessageBoxDataContainer(CDataContainerInterface* pxDataContainer)
{
    m_pxMessageBoxDataContainer = pxDataContainer;
}

//-------------------------------------------------------------------------------
CDataContainerInterface* CTask::GetMessageBoxDataContainerPointer(void)
{
    return m_pxMessageBoxDataContainer;
};

//-------------------------------------------------------------------------------
void CTask::SetOperatingDataContainer(CDataContainerInterface* pxDataContainer)
{
    m_pxOperatingDataContainer = pxDataContainer;
}

//-------------------------------------------------------------------------------
CDataContainerInterface* CTask::GetOperatingDataContainerPointer(void)
{
    return m_pxOperatingDataContainer;
};

//-------------------------------------------------------------------------------
void CTask::SetCommandDataContainer(CDataContainerInterface* pxDataContainer)
{
    m_pxCommandDataContainer = pxDataContainer;
}

//-------------------------------------------------------------------------------
CDataContainerInterface* CTask::GetCommandDataContainerPointer(void)
{
    return m_pxCommandDataContainer;
};

//-------------------------------------------------------------------------------
void CTask::SetMessageBoxPoiner(std::shared_ptr<CMessageBoxInterface> pxMessageBox)
{
    m_pxMessageBox = pxMessageBox;
}

//-------------------------------------------------------------------------------
std::shared_ptr<CMessageBoxInterface> CTask::GetMessageBoxPointer(void)
{
    return m_pxMessageBox;
};

////-------------------------------------------------------------------------------
//uint8_t CTask::Init(void)
//{
//    std::cout << "CTask Init"  << std::endl;
//}

////-------------------------------------------------------------------------------
//void CTask::AddCurrentlyRunningTask(CTaskInterface* pxTask)
//{
//    //std::cout << "CTask::AddCurrentlyRunningTask"  << std::endl;
//
//}

////-------------------------------------------------------------------------------
//void CTask::AddCommonTask(CTaskInterface* pxTask)
//{
//    std::cout << "CTask::AddCommonTask 1"  << std::endl;
//
//}

////-------------------------------------------------------------------------------
//uint8_t CTask::Fsm(void)
//{
////        std::cout << "CTask::Fsm 1"  << std::endl;
//
//    switch (GetFsmState())
//    {
//    case IDDLE:
//        //std::cout << "CTask::Fsm IDDLE"  << std::endl;
//        break;
//
//    case START:
////        //std::cout << "CTask::Fsm START"  << std::endl;
//        SetFsmState(START);
//        break;
//
//    default:
//        break;
//    }
//
//    return GetFsmState();
//}

//-------------------------------------------------------------------------------

