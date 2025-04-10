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
void CTask::SetCustomerDataContainer(CDataContainerInterface* pxDataContainer)
{
    m_pxCustomerDataContainer = pxDataContainer;
}

//-------------------------------------------------------------------------------
CDataContainerInterface* CTask::GetCustomerDataContainerPointer(void)
{
    return m_pxCustomerDataContainer;
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

//-------------------------------------------------------------------------------
void CTask::SetCurrentCustomerDataContainer(CDataContainerInterface* pxDataContainer)
{
    m_pxCurrentCustomerDataContainer = pxDataContainer;
}

//-------------------------------------------------------------------------------
CDataContainerInterface* CTask::GetCurrentCustomerDataContainerPointer(void)
{
    return m_pxCurrentCustomerDataContainer;
}

//-------------------------------------------------------------------------------
void CTask::SetCurrentExecutorDataContainer(CDataContainerInterface* pxDataContainer)
{
    m_pxCurrentExecutorDataContainer = pxDataContainer;
}

//-------------------------------------------------------------------------------
CDataContainerInterface* CTask::GetCurrentExecutorDataContainerPointer(void)
{
    return m_pxCurrentExecutorDataContainer;
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
bool CTask::IsDoneOk(void)
{
    if (GetFsmOperationStatus() == DONE_OK)
    {
        SetFsmOperationStatus(0);
        return true;
    }
    else
    {
        return false;
    }
}

//-------------------------------------------------------------------------------
bool CTask::IsDoneError(void)
{
    if (GetFsmOperationStatus() == DONE_ERROR)
    {
        SetFsmOperationStatus(0);
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
        pxTask -> SetCustomerDataContainer(pxDataContainer);
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
bool CTask::SetTaskData(CDataContainerInterface* pxDataContainer)
{
//    std::cout << "CTask::SetTaskData 1" << std::endl;

//    std::cout << "CTask::SetTaskData TaskId "  << (int)pxDataContainer ->
//              GetTaskId() << std::endl;
    CTaskInterface* pxTask =
        GetResources() ->
        GetTaskPointerById(pxDataContainer ->
                           GetTaskId());

//    std::cout << "CTask::SetTaskData m_sTaskName "  << pxTask ->
//              GetTaskName() << std::endl;

    if (pxTask)
    {
        if (pxTask -> IsTaskReady())
        {
//            std::cout << "CTask::SetTaskData 2" << std::endl;
            pxTask -> SetCustomerDataContainer(pxDataContainer);
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
    else
    {
        std::cout << "CTask::SetTaskData 4" << std::endl;
        return false;
    }
}















//-------------------------------------------------------------------------------
CNewTask::CNewTask()
{
    std::cout << "CNewTask constructor 1"  << std::endl;
    SetFsmState(INIT);
}

//-------------------------------------------------------------------------------
CNewTask::~CNewTask()
{
    std::cout << "CNewTask destructor"  << std::endl;
}

//-------------------------------------------------------------------------------
void CNewTask::SetResources(CResources* pxResources)
{
    m_pxResources = pxResources;
}

//-------------------------------------------------------------------------------
CResources* CNewTask::GetResources(void)
{
    return m_pxResources;
}

//-------------------------------------------------------------------------------
void CNewTask::SetCustomerDataContainer(CDataContainerInterface* pxDataContainer)
{
    m_pxCustomerDataContainer = pxDataContainer;
}

//-------------------------------------------------------------------------------
CDataContainerInterface* CNewTask::GetCustomerDataContainerPointer(void)
{
    return m_pxCustomerDataContainer;
}

//-------------------------------------------------------------------------------
void CNewTask::SetExecutorDataContainer(CDataContainerInterface* pxDataContainer)
{
    m_pxExecutorDataContainer = pxDataContainer;
}

//-------------------------------------------------------------------------------
CDataContainerInterface* CNewTask::GetExecutorDataContainerPointer(void)
{
    return m_pxExecutorDataContainer;
}

//-------------------------------------------------------------------------------
uint8_t CNewTask::Init(void)
{
    std::cout << "CNewTask Init"  << std::endl;
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
}

//-------------------------------------------------------------------------------
bool CNewTask::SetTaskData(CDataContainerInterface* pxDataContainer)
{
////    std::cout << "CNewTask::SetTaskData 1" << std::endl;
//
////    std::cout << "CNewTask::SetTaskData TaskId "  << (int)pxDataContainer ->
////              GetTaskId() << std::endl;
//    CNewTask* pxTask =
//        GetResources() ->
//        GetTaskPointerById(pxDataContainer ->
//                           GetTaskId());
//
////    std::cout << "CNewTask::SetTaskData m_sTaskName "  << pxTask ->
////              GetTaskName() << std::endl;
//
//    if (pxTask)
//    {
//        if (pxTask -> IsTaskReady())
//        {
////            std::cout << "CNewTask::SetTaskData 2" << std::endl;
//            pxTask -> SetCustomerDataContainer(pxDataContainer);
//            pxTask -> SetFsmState(pxDataContainer ->
//                                  GetFsmCommandState());
//            return true;
//        }
//        else
//        {
//            std::cout << "CNewTask::SetTaskData 3" << std::endl;
//            return false;
//        }
//    }
//    else
//    {
//        std::cout << "CNewTask::SetTaskData 4" << std::endl;
//        return false;
//    }
}

//-------------------------------------------------------------------------------
uint8_t CNewTask::Fsm(void)
{
//    //std::cout << "CNewTask::Fsm 1" << endl;

    switch (GetFsmState())
    {
    case INIT:
//        //std::cout << "CNewTask::Fsm INIT 1"  << std::endl;
        Init();
        SetFsmState(STOP);
        break;

    case STOP:
//        //std::cout << "CNewTask::Fsm STOP"  << std::endl;
        break;

    case START:
        //std::cout << "CNewTask::Fsm START"  << std::endl;
        SetFsmState(INIT);
        break;

    case DONE_OK:
//        //std::cout << "CNewTask::Fsm DONE_OK"  << std::endl;
        break;

    case DONE_ERROR:
//        //std::cout << "CNewTask::Fsm DONE_ERROR"  << std::endl;
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
        std::cout << "CNewTask::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        std::cout << "CNewTask::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CNewTask::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
            std::cout << "CNewTask::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CNewTask::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
        std::cout << "CNewTask::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
        }

        break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        std::cout << "CNewTask::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CNewTask::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CNewTask::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CNewTask::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
            }
        }
    }
    break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------

