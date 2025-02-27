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
    m_xDeviceConfigSearch = {0};
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
}

//-----------------------------------------------------------------------------------------------------
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
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
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
            memset(reinterpret_cast<uint8_t*>(&m_xDeviceConfigSearch),
                   0,
                   sizeof(struct TConfigDataPackOne));

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiInternalModuleId;
            pxDataContainer -> m_uiFsmCommandState =
                CInternalModule::SEARCH_MODULES_START;
            pxDataContainer -> m_puiDataPointer = (uint8_t*)&m_xDeviceConfigSearch;

//            if (SetTaskData(GetExecutorDataContainerPointer()))
//            {
//                SetFsmState(CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_START);
//            }
//            else
//            {
//                SetFsmState(CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_START);
//            }

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DONE_OK);
//        SetFsmNextStateDoneWaitingDoneOk(EXECUTOR_ANSWER_PROCESSING);
//        SetFsmNextStateDoneError(DONE_ERROR);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_START:
        std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_WAITING);
        }

        break;

    case CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_WAITING:
//        //std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
            std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(DONE_ERROR);
            }
        }
    }
    break;

    case CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_START:
        std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_WAITING);
        }

        break;

    case CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_WAITING:
//        //std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_WAITING"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(DONE_ERROR);
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case CONFIGURATION_REQUEST_START:
        std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_START"  << std::endl;
        {
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//            SetFsmState(DONE_OK);

//            CDataContainerDataBase* pxDataContainer =
//                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//            pxDataContainer -> m_uiTaskId = m_uiInternalModuleId;
//            pxDataContainer -> m_uiFsmCommandState =
//                CInternalModule::SEARCH_MODULES_START;
//            pxDataContainer -> m_puiDataPointer = (uint8_t*)&m_xDeviceConfigSearch;
//
//            if (SetTaskData(GetExecutorDataContainerPointer()))
//            {
//                SetFsmState(CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_START);
//            }
//            else
//            {
//                SetFsmState(CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_START);
//            }

//            memset(reinterpret_cast<uint8_t*>(&m_xDeviceConfigSearch),
//                   0,
//                   sizeof(struct TConfigDataPackOne));
//
//            CDataContainerDataBase* pxDataContainer =
//                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//            pxDataContainer -> m_uiTaskId = m_uiInternalModuleId;
//            pxDataContainer -> m_uiFsmCommandState =
//                CInternalModule::SEARCH_MODULES_START;
//            pxDataContainer -> m_puiDataPointer = (uint8_t*)&m_xDeviceConfigSearch;
//
////            if (SetTaskData(GetExecutorDataContainerPointer()))
////            {
////                SetFsmState(CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_START);
////            }
////            else
////            {
////                SetFsmState(CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_START);
////            }

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING);
//        SetFsmNextStateDoneWaitingDoneOk(EXECUTOR_ANSWER_PROCESSING);
//        SetFsmNextStateDoneError(DONE_ERROR);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

//    case CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_START:
//        std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_START"  << std::endl;
//        {
//            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
//            SetFsmState(CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_WAITING);
//        }
//
//        break;
//
//    case CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_WAITING:
////        //std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
//    {
//        if (SetTaskData(GetExecutorDataContainerPointer()))
//        {
//            std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
//            SetFsmState(CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_START);
//        }
//        else
//        {
//            std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
//            // Время ожидания выполнения запроса закончилось?
//            if (GetTimerPointer() -> IsOverflow())
//            {
//                std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
//                SetFsmState(DONE_ERROR);
//            }
//        }
//    }
//    break;
//
//    case CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_START:
//        std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_START"  << std::endl;
//        {
//            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
//            SetFsmState(CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING);
//        }
//
//        break;
//
//    case CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING:
////        //std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING"  << std::endl;
//    {
//        CDataContainerDataBase* pxDataContainer =
//            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//
//        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;
//
//        if (uiFsmState == DONE_OK)
//        {
//            std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//            SetFsmState(DONE_OK);
//        }
//        else if (uiFsmState == DONE_ERROR)
//        {
//            std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
//            SetFsmState(DONE_ERROR);
//        }
//        else
//        {
//            // Время ожидания выполнения запроса закончилось?
//            if (GetTimerPointer() -> IsOverflow())
//            {
//                std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
//                SetFsmState(DONE_ERROR);
//            }
//        }
//    }
//    break;

    case CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            CDataContainerDataBase* pxExecutorDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
            uint16_t  uiLength = sizeof(auiTempData);
            memcpy(pxExecutorDataContainer -> m_puiDataPointer,
                   auiTempData,
                   uiLength);

            pxExecutorDataContainer -> m_uiDataLength = uiLength;

            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
                   (pxExecutorDataContainer -> m_puiDataPointer),
                   pxExecutorDataContainer -> m_uiDataLength);
            pxCustomerDataContainer -> m_uiDataLength =
                pxExecutorDataContainer -> m_uiDataLength;

            std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING uiLength "  << (int)uiLength << std::endl;
            std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING pxCustomerDataContainer -> m_uiDataLength "  << (int)pxCustomerDataContainer -> m_uiDataLength << std::endl;
            std::cout << "CConfigurationCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING pxExecutorDataContainer -> m_uiDataLength "  << (int)pxExecutorDataContainer -> m_uiDataLength << std::endl;

            SetFsmState(DONE_OK);
        }
        break;

//-------------------------------------------------------------------------------
    default:
        break;
    }
}

//-------------------------------------------------------------------------------

