//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <iostream>
#include <string.h>
#include <typeinfo>

#include "Task.h"
#include "Resources.h"
#include "Platform.h"
#include "DataStore.h"
#include "DataContainer.h"
#include "InternalModule.h"
#include "ConfigurationCreate.h"
#include "DiscreteSignals.h"
#include "SystemComponentsCreate.h"

using namespace std;

//-------------------------------------------------------------------------------
CSystemComponentsCreate::CSystemComponentsCreate()
{
    std::cout << "CSystemComponentsCreate constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[256];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CSystemComponentsCreate::~CSystemComponentsCreate()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
void CSystemComponentsCreate::SetInternalModuleName(std::string sName)
{
    m_sInternalModuleName = sName;
}

//-------------------------------------------------------------------------------
uint8_t CSystemComponentsCreate::Init(void)
{
    std::cout << "CSystemComponentsCreate Init"  << std::endl;
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
    SetCustomerDataContainer(GetExecutorDataContainerPointer());

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
uint8_t CSystemComponentsCreate::Fsm(void)
{
//    std::cout << "CSystemComponentsCreate::Fsm 1" << endl;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CSystemComponentsCreate::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CSystemComponentsCreate::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CSystemComponentsCreate::Fsm START"  << std::endl;
        Init();
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CSystemComponentsCreate::Fsm INIT 1"  << std::endl;
        m_uiInternalModuleId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleName);
        SetFsmState(READY);

        break;

    case READY:
//        std::cout << "CSystemComponentsCreate::Fsm READY"  << std::endl;
        break;

    case DONE_OK:
//        std::cout << "CSystemComponentsCreate::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CSystemComponentsCreate::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START:
        std::cout << "CSystemComponentsCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING:
//        std::cout << "CSystemComponentsCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CSystemComponentsCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else
        {
            std::cout << "CSystemComponentsCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 3"  << std::endl;
            // ¬рем€ ожидани€ выполнени€ запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CSystemComponentsCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
        std::cout << "CSystemComponentsCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        std::cout << "CSystemComponentsCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CSystemComponentsCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
            std::cout << "CSystemComponentsCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // ¬рем€ ожидани€ выполнени€ запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CSystemComponentsCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
        std::cout << "CSystemComponentsCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
        }

        break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        std::cout << "CSystemComponentsCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CSystemComponentsCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CSystemComponentsCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(GetFsmNextStateDoneWaitingDoneError());
        }
        else
        {
            // ¬рем€ ожидани€ выполнени€ запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CSystemComponentsCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateDoneWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SYSTEM_COMPONENTS_CREATE_INTERNAL_MODULES_HANDLERS_CREATE_START:
        std::cout << "CSystemComponentsCreate::Fsm SYSTEM_COMPONENTS_CREATE_INTERNAL_MODULES_HANDLERS_CREATE_START"  << std::endl;
        {
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiInternalModuleId;
            pxDataContainer -> m_uiFsmCommandState =
                CInternalModule::MODULES_HANDLERS_CREATE_START;
            pxDataContainer -> m_puiDataPointer =
                (uint8_t*)(GetResources() -> GetDeviceConfigSearchPointer());

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(SYSTEM_COMPONENTS_CREATE_INTERNAL_MODULES_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(SYSTEM_COMPONENTS_CREATE_INTERNAL_MODULES_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(SYSTEM_COMPONENTS_CREATE_INTERNAL_MODULES_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(SYSTEM_COMPONENTS_CREATE_INTERNAL_MODULES_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case SYSTEM_COMPONENTS_CREATE_INTERNAL_MODULES_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm SYSTEM_COMPONENTS_CREATE_INTERNAL_MODULES_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(SYSTEM_COMPONENTS_CREATE_DISCRETE_SIGNALS_ALARM_HANDLERS_CREATE_START);
        }
        break;

    case SYSTEM_COMPONENTS_CREATE_INTERNAL_MODULES_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDiscreteSignals::Fsm SYSTEM_COMPONENTS_CREATE_INTERNAL_MODULES_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case SYSTEM_COMPONENTS_CREATE_DISCRETE_SIGNALS_ALARM_HANDLERS_CREATE_START:
        std::cout << "CSystemComponentsCreate::Fsm SYSTEM_COMPONENTS_CREATE_DISCRETE_SIGNALS_ALARM_HANDLERS_CREATE_START"  << std::endl;
        {
            uint8_t uiDiscreteSignalsId =
                GetResources() ->
                GetTaskIdByNameFromMap("DiscreteSignals");

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = uiDiscreteSignalsId;
            pxDataContainer -> m_uiFsmCommandState =
                CDiscreteSignals::DISCRETE_SIGNALS_CREATE_ALARM_HANDLERS_START;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(SYSTEM_COMPONENTS_CREATE_DISCRETE_SIGNALS_ALARM_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(SYSTEM_COMPONENTS_CREATE_DISCRETE_SIGNALS_ALARM_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(SYSTEM_COMPONENTS_CREATE_DISCRETE_SIGNALS_ALARM_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(SYSTEM_COMPONENTS_CREATE_DISCRETE_SIGNALS_ALARM_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case SYSTEM_COMPONENTS_CREATE_DISCRETE_SIGNALS_ALARM_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm SYSTEM_COMPONENTS_CREATE_DISCRETE_SIGNALS_ALARM_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case SYSTEM_COMPONENTS_CREATE_DISCRETE_SIGNALS_ALARM_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDiscreteSignals::Fsm SYSTEM_COMPONENTS_CREATE_DISCRETE_SIGNALS_ALARM_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    default:
        break;
    }
}

//-------------------------------------------------------------------------------
