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

//-------------------------------------------------------------------------------
CTask::CTask()
{
    std::cout << "CTask constructor 1"  << std::endl;
}
//-------------------------------------------------------------------------------
CTask::CTask(CResources* pxResources)
{
    std::cout << "CTask constructor 2"  << std::endl;
}

//-------------------------------------------------------------------------------
CTask::~CTask()
{
    std::cout << "CTask destructor"  << std::endl;
}

//-------------------------------------------------------------------------------
char* CTask::GetTaskNamePointer(void)
{
    return m_acTaskName;
}

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

////-------------------------------------------------------------------------------
//void CTask::SetOperatingDataContainer(CDataContainerInterface* pxDataContainer)
//{
//    m_pxOperatingDataContainer = pxDataContainer;
//}
//
////-------------------------------------------------------------------------------
//CDataContainerInterface* CTask::GetOperatingDataContainerPointer(void)
//{
//    return m_pxOperatingDataContainer;
//}
//
////-------------------------------------------------------------------------------
//void CTask::SetCommandDataContainer(CDataContainerInterface* pxDataContainer)
//{
//    m_pxCommandDataContainer = pxDataContainer;
//}
//
////-------------------------------------------------------------------------------
//CDataContainerInterface* CTask::GetCommandDataContainerPointer(void)
//{
//    return m_pxCommandDataContainer;
//}

////-------------------------------------------------------------------------------
//void CTask::SetTaskData(CDataContainerInterface* pxDataContainer)
//{
//
//}
//
////-------------------------------------------------------------------------------
//CDataContainerInterface* CTask::GetTaskData(void)
//{
//    return 0;
//}

//-------------------------------------------------------------------------------
size_t CTask::GetObjectLength(void)
{
    std::cout << "CTask GetObjectLength"  << std::endl;
    return sizeof(*this);
}

//-------------------------------------------------------------------------------
bool CTask::IsTaskReady(void)
{
    uint8_t uiFsmState = GetFsmState();

    if ((uiFsmState == READY) ||
            (uiFsmState == DONE_OK) ||
            (uiFsmState == DONE_ERROR))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//-------------------------------------------------------------------------------

