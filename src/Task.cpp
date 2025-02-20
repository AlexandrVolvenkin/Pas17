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

//-------------------------------------------------------------------------------
void CTask::SetCustomertDataContainer(CDataContainerInterface* pxDataContainer)
{
    m_pxCustomertDataContainer = pxDataContainer;
}

//-------------------------------------------------------------------------------
CDataContainerInterface* CTask::GetCustomertDataContainerPointer(void)
{
    return m_pxCustomertDataContainer;
}

//-------------------------------------------------------------------------------
void CTask::SetExecutorDataContainer(CDataContainerInterface* pxDataContainer)
{
    m_pxExecutorDataContainer = pxDataContainer;
}

//-------------------------------------------------------------------------------
CDataContainerInterface* CTask::GetExecutorDataContainerPointer(void)
{
    return m_pxExecutorDataContainer;
}

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
bool CTask::SetTaskData(uint8_t uiTaskId, CDataContainerInterface* pxDataContainer)
{
    std::cout << "CTask::SetTaskData 1" << std::endl;

    CTaskInterface* pxTask =
        GetResources() ->
        GetTaskPointerById(uiTaskId);

    if (pxTask -> IsTaskReady())
    {
        std::cout << "CTask::SetTaskData 2" << std::endl;
        pxTask -> SetCustomertDataContainer(pxDataContainer);
        pxTask -> SetFsmState(pxDataContainer ->
                              GetFsmCommandState());
        return true;
    }
    else
    {
        std::cout << "CTask::SetTaskData 3" << std::endl;
        return false;
    }
}

//-------------------------------------------------------------------------------

