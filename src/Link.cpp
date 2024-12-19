
//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <iostream>
#include <string.h>
#include <typeinfo>
#include <memory>

#include "Task.h"
#include "Resources.h"
#include "StorageDevice.h"
#include "Crc.h"
#include "HammingCodes.h"
#include "DataContainer.h"
#include "MessageBox.h"
#include "DataStore.h"
#include "Link.h"

//-------------------------------------------------------------------------------
CLink::CLink()
{
    std::cout << "CLink constructor"  << std::endl;
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CLink::~CLink()
{
    //dtor
}

//-------------------------------------------------------------------------------
size_t CLink::GetObjectLength(void)
{
    std::cout << "CLink GetObjectLength"  << std::endl;
    return sizeof(*this);
}

//-------------------------------------------------------------------------------
void CLink::SetTaskPerformerName(std::string sName)
{
    m_sTaskPerformerName = sName;
}

//-------------------------------------------------------------------------------
void CLink::SetTaskPerformer(CTaskInterface* pxTaskPerformer)
{
    m_pxTaskPerformer = pxTaskPerformer;
}

//-------------------------------------------------------------------------------
CTaskInterface* CLink::GetTaskPerformerPointer(void)
{
    return m_pxTaskPerformer;
};

//-------------------------------------------------------------------------------
void CLink::SetDataContainer(CDataContainerInterface* pxDataContainer)
{
    m_pxDataContainer = pxDataContainer;
}

//-------------------------------------------------------------------------------
CDataContainerInterface* CLink::GetDataContainerPointer(void)
{
    return m_pxDataContainer;
}

//-------------------------------------------------------------------------------
uint8_t CLink::Fsm(void)
{
//    std::cout << "CLink::Fsm 1" << endl;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CLink::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CLink::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CLink::Fsm START"  << std::endl;
        std::cout << "CLink::Fsm m_sTaskPerformerName" << " " << (m_sTaskPerformerName) << std::endl;
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
//                    SetFsmState(READY);
        break;

    case INIT:
        std::cout << "CLink::Fsm INIT 1"  << std::endl;
//        {
//            CTaskInterface* pxTask =
//                GetResources() ->
//                GetTaskPointerByNameFromMap(m_sTaskPerformerName);
//
//            if (pxTask != 0)
//            {
//                std::cout << "CLink::Fsm INIT 2"  << std::endl;
//                if (pxTask -> GetFsmState() >= READY)
//                {
//                    SetTaskPerformer(pxTask);
//                    SetFsmState(READY);
//                    std::cout << "CLink::Fsm INIT 3"  << std::endl;
//                }
//            }
//            else
//            {
//                std::cout << "CLink::Fsm INIT 4"  << std::endl;
//                if (GetTimerPointer() -> IsOverflow())
//                {
//                    std::cout << "CLink::Fsm INIT 5"  << std::endl;
//                    SetFsmState(STOP);
//                }
//            }
//        }

                    SetFsmState(READY);
        break;

    case READY:
//        std::cout << "CLink::Fsm READY"  << std::endl;
//        {
//            if (m_pxCommandDataContainer != 0)
//            {
//                std::cout << "CLink::Fsm READY 2"  << std::endl;
//                m_pxOperatingDataContainer = m_pxCommandDataContainer;
//                SetFsmState(GetFsmCommandState());
//                SetFsmCommandState(0);
//                m_pxCommandDataContainer = 0;
//            }
//        }


//        {
//            if (GetFsmCommandState() != 0)
//            {
//                SetFsmState(GetFsmCommandState());
//                SetFsmCommandState(0);
//            }
//        }
//        SetFsmState(COMMUNICATION_START);
        break;

    case DONE_OK:
        std::cout << "CLink::Fsm DONE_OK"  << std::endl;
        SetFsmOperationStatus(DONE_OK);
        SetFsmState(READY);
        break;

    case DONE_ERROR:
        std::cout << "CLink::Fsm DONE_ERROR"  << std::endl;
        SetFsmOperationStatus(DONE_ERROR);
        SetFsmState(READY);
        break;

    default:
        break;
    }
}






//// CLink.cpp
//#include "Link.h"
//#include <iostream>
//
//CLink::CLink(const std::string& performerName, CTaskInterface* performer, CDataContainerInterface* dataContainer)
//    : m_sTaskPerformerName(performerName),
//      m_pxTaskPerformer(performer),
//      m_pxDataContainer(dataContainer)
//{
//    // Constructor implementation
//}
//
//CLink::~CLink()
//{
//    // Destructor implementation
//    delete m_pxTaskPerformer;
//    delete m_pxDataContainer;
//}
//
//void CLink::setTaskPerformerName(const std::string& performerName)
//{
//    m_sTaskPerformerName = performerName;
//}
//
//std::string CLink::getTaskPerformerName() const
//{
//    return m_sTaskPerformerName;
//}
//
//void CLink::setTaskPerformer(CTaskInterface* performer)
//{
//    m_pxTaskPerformer = performer;
//}
//
//CTaskInterface* CLink::getTaskPerformer() const
//{
//    return m_pxTaskPerformer;
//}
//
//void CLink::setDataContainer(CDataContainerInterface* dataContainer)
//{
//    m_pxDataContainer = dataContainer;
//}
//
//CDataContainerInterface* CLink::getDataContainer() const
//{
//    return m_pxDataContainer;
//}
