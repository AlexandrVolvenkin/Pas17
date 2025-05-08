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

using namespace std;

//-------------------------------------------------------------------------------
CConfigurationCreate::CConfigurationCreate()
{
    std::cout << "CConfigurationCreate constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[256];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CConfigurationCreate::~CConfigurationCreate()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
void CConfigurationCreate::SetInternalModuleName(std::string sName)
{
    m_sInternalModuleName = sName;
}

//-------------------------------------------------------------------------------
uint8_t CConfigurationCreate::Init(void)
{
    std::cout << "CConfigurationCreate Init"  << std::endl;
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
    SetCustomerDataContainer(GetExecutorDataContainerPointer());

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
// сериализует, преобразовывает объект конфигурации из внутреннего формата в формат передачи по интерфейсам связи.
void CConfigurationCreate::
ConfigurationToProgrammerFormat(TConfigDataProgrammerPackOne *pxDestination,
                                TConfigDataPackOne *pxSourse)
{
    std::cout << "CConfigurationCreate ConfigurationToProgrammerFormat 1"  << std::endl;
    pxDestination -> uiModulesQuantity =
        pxSourse -> uiModulesQuantity;

    for (int i = 0;
            (i < INTERNAL_MODULE_QUANTITY);
            i++)
    {
        pxDestination -> auiModulesContext[i] =
            pxSourse -> axModulesContext[i].uiType;
    }

    pxDestination -> uiLastDiscreteInputModuleAddresPlusOne =
        pxSourse -> uiLastDiscreteInputModuleAddresPlusOne;
    pxDestination -> uiLastAnalogueInputModuleAddresPlusOne =
        pxSourse -> uiLastAnalogueInputModuleAddresPlusOne;
    pxDestination -> uiDiscreteOutputQuantity =
        pxSourse -> uiDiscreteOutputQuantity;
    pxDestination -> uiDiscreteInputSignalsQuantity =
        pxSourse -> uiDiscreteInputSignalsQuantity;
    pxDestination -> uiAnalogueInputSignalsQuantity =
        pxSourse -> uiAnalogueInputSignalsQuantity;
    pxDestination -> uiDisplayType =
        pxSourse -> uiDisplayType;
//    pxDestination -> uiErrorCode =
//        pxSourse -> uiErrorCode;
}

//-------------------------------------------------------------------------------
uint8_t CConfigurationCreate::Fsm(void)
{
//    std::cout << "CConfigurationCreate::Fsm 1" << endl;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CConfigurationCreate::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CConfigurationCreate::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CConfigurationCreate::Fsm START"  << std::endl;
        Init();
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CConfigurationCreate::Fsm INIT 1"  << std::endl;
        m_uiInternalModuleId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleName);
        SetFsmState(READY);

        break;

    case READY:
//        std::cout << "CConfigurationCreate::Fsm READY"  << std::endl;
        break;

    case DONE_OK:
//        std::cout << "CConfigurationCreate::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CConfigurationCreate::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START:
        std::cout << "CConfigurationCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING:
//        std::cout << "CConfigurationCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CConfigurationCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else
        {
            std::cout << "CConfigurationCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CConfigurationCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
        std::cout << "CConfigurationCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        std::cout << "CConfigurationCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CConfigurationCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
            std::cout << "CConfigurationCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CConfigurationCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
        std::cout << "CConfigurationCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
        }

        break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        std::cout << "CConfigurationCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CConfigurationCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CConfigurationCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(GetFsmNextStateDoneWaitingDoneError());
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CConfigurationCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateDoneWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case CONFIGURATION_CREATE_START:
        std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_START"  << std::endl;
        {
            *(GetResources() -> GetDeviceConfigSearchPointer()) = {0};
            SetFsmState(CONFIGURATION_CREATE_INTERNAL_MODULES_SEARCH_MODULES_START);
        }
        break;

//-------------------------------------------------------------------------------
    case CONFIGURATION_CREATE_INTERNAL_MODULES_SEARCH_MODULES_START:
        std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_INTERNAL_MODULES_SEARCH_MODULES_START"  << std::endl;
        {
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiInternalModuleId;
            pxDataContainer -> m_uiFsmCommandState =
                CInternalModule::SEARCH_MODULES_START;
            pxDataContainer -> m_puiDataPointer =
                (uint8_t*)(GetResources() -> GetDeviceConfigSearchPointer());

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(CONFIGURATION_CREATE_INTERNAL_MODULES_SEARCH_MODULES_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case CONFIGURATION_CREATE_INTERNAL_MODULES_SEARCH_MODULES_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_INTERNAL_MODULES_SEARCH_MODULES_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(CONFIGURATION_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_START);
        }
        break;

//-------------------------------------------------------------------------------
    case CONFIGURATION_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_START:
        std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_START"  << std::endl;
        {
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiInternalModuleId;
            pxDataContainer -> m_uiFsmCommandState =
                CInternalModule::SERVICE_DATA_CREATE_START;
            pxDataContainer -> m_puiDataPointer =
                (uint8_t*)(GetResources() -> GetDeviceConfigSearchPointer());

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(CONFIGURATION_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case CONFIGURATION_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            CDataContainerDataBase* pxExecutorDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            uint16_t  uiLength = sizeof(struct TConfigDataPackOne);
            memcpy(pxExecutorDataContainer -> m_puiDataPointer,
                   (uint8_t*)(GetResources() -> GetDeviceConfigSearchPointer()),
                   uiLength);

            pxExecutorDataContainer -> m_uiDataLength = uiLength;

            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
                   (pxExecutorDataContainer -> m_puiDataPointer),
                   pxExecutorDataContainer -> m_uiDataLength);
            pxCustomerDataContainer -> m_uiDataLength =
                pxExecutorDataContainer -> m_uiDataLength;

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

//-------------------------------------------------------------------------------
    case CONFIGURATION_REQUEST_START:
        std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_START"  << std::endl;
        {
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }

        SetFsmState(CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING);
        break;

    case CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            CDataContainerDataBase* pxExecutorDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            uint16_t  uiLength = sizeof(struct TConfigDataPackOne);

            ConfigurationToProgrammerFormat((TConfigDataProgrammerPackOne*)(pxExecutorDataContainer -> m_puiDataPointer),
                                            (GetResources() -> GetDeviceConfigSearchPointer()));

            pxExecutorDataContainer -> m_uiDataLength = uiLength;

            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
                   (pxExecutorDataContainer -> m_puiDataPointer),
                   pxExecutorDataContainer -> m_uiDataLength);
            pxCustomerDataContainer -> m_uiDataLength =
                pxExecutorDataContainer -> m_uiDataLength;

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

//-------------------------------------------------------------------------------
    default:
        break;
    }
}

//-------------------------------------------------------------------------------

