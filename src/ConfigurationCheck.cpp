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
#include "DeviceControl.h"
#include "DataContainer.h"
#include "InternalModule.h"
#include "ConfigurationCreate.h"
#include "DataBaseCreate.h"
#include "ConfigurationCheck.h"

using namespace std;

//-------------------------------------------------------------------------------
CConfigurationCheck::CConfigurationCheck()
{
    std::cout << "CConfigurationCheck constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[256];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CConfigurationCheck::~CConfigurationCheck()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
void CConfigurationCheck::SetInternalModuleName(std::string sName)
{
    m_sInternalModuleName = sName;
}

//-------------------------------------------------------------------------------
void CConfigurationCheck::SetDeviceControlName(std::string sName)
{
    m_sDeviceControlName = sName;
}

//-------------------------------------------------------------------------------
uint8_t CConfigurationCheck::Init(void)
{
    std::cout << "CConfigurationCheck Init"  << std::endl;
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
    SetCustomerDataContainer(GetExecutorDataContainerPointer());

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
uint8_t CConfigurationCheck::Fsm(void)
{
//    std::cout << "CConfigurationCheck::Fsm 1" << endl;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CConfigurationCheck::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CConfigurationCheck::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CConfigurationCheck::Fsm START"  << std::endl;
        Init();
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CConfigurationCheck::Fsm INIT 1"  << std::endl;
        m_uiInternalModuleId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleName);

        m_uiDeviceControlId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDeviceControlName);

        SetFsmState(READY);

        break;

    case READY:
//        std::cout << "CConfigurationCheck::Fsm READY"  << std::endl;
        break;

    case DONE_OK:
//        std::cout << "CConfigurationCheck::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CConfigurationCheck::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START:
        std::cout << "CConfigurationCheck::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING:
//        std::cout << "CConfigurationCheck::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CConfigurationCheck::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else
        {
            std::cout << "CConfigurationCheck::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CConfigurationCheck::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
        std::cout << "CConfigurationCheck::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        std::cout << "CConfigurationCheck::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CConfigurationCheck::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
            std::cout << "CConfigurationCheck::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CConfigurationCheck::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
        std::cout << "CConfigurationCheck::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
        }

        break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        std::cout << "CConfigurationCheck::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CConfigurationCheck::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CConfigurationCheck::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(GetFsmNextStateDoneWaitingDoneError());
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CConfigurationCheck::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateDoneWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case CONFIGURATION_CHECK_START:
        std::cout << "CConfigurationCheck::Fsm CONFIGURATION_CHECK_START"  << std::endl;
        {
            SetFsmState(CONFIGURATION_CHECK_CONFIGURATION_DATA_BASE_BLOCKS_READ_START);
        }
        break;

    case CONFIGURATION_CHECK_CONFIGURATION_DATA_BASE_BLOCKS_READ_START:
        std::cout << "CConfigurationCheck::Fsm CONFIGURATION_CHECK_CONFIGURATION_DATA_BASE_BLOCKS_READ_START"  << std::endl;
        {
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
            pxDataContainer -> m_uiFsmCommandState =
                CDeviceControl::DATA_BASE_BLOCK_READ;
            // конфигурация блок 100
            pxDataContainer -> m_uiDataIndex = CONFIGURATION_DATA_BASE_BLOCK_OFFSET;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(CONFIGURATION_CHECK_CONFIGURATION_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(CONFIGURATION_CHECK_CONFIGURATION_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(CONFIGURATION_CHECK_CONFIGURATION_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(CONFIGURATION_CHECK_CONFIGURATION_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case CONFIGURATION_CHECK_CONFIGURATION_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CConfigurationCheck::Fsm CONFIGURATION_CHECK_CONFIGURATION_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(CONFIGURATION_CHECK_CONFIGURATION_COMPARE_START);
        }
        break;

    case CONFIGURATION_CHECK_CONFIGURATION_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CConfigurationCheck::Fsm CONFIGURATION_CHECK_CONFIGURATION_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

    case CONFIGURATION_CHECK_CONFIGURATION_COMPARE_START:
        std::cout << "CConfigurationCheck::Fsm CONFIGURATION_CHECK_CONFIGURATION_COMPARE_START"  << std::endl;
        {
            uint8_t auiTempArray[256];
            // преобразуем текущую конфигурацию в общий формат хранения совместимый с программатором.
            CConfigurationCreate::ConfigurationToProgrammerFormat((CConfigurationCreate::TConfigDataProgrammerPackOne*)(auiTempArray),
                    (GetResources() -> GetDeviceConfigSearchPointer()));

            {
                std::cout << "CConfigurationCheck::Fsm auiTempArray"  << std::endl;
                uint8_t *pucSourceTemp;
                pucSourceTemp = (uint8_t*)auiTempArray;
                for(int i=0; i<32; )
                {
                    for(int j=0; j<8; j++)
                    {
                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
                    }
                    cout << endl;
                    i += 8;
                }
            }


            {
                std::cout << "CConfigurationCheck::Fsm m_puiIntermediateBuff " << (sizeof(struct CConfigurationCreate::TConfigDataProgrammerPackOne))  << std::endl;
                uint8_t *pucSourceTemp;
                pucSourceTemp = (uint8_t*)m_puiIntermediateBuff;
                for(int i=0; i<32; )
                {
                    for(int j=0; j<8; j++)
                    {
                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
                    }
                    cout << endl;
                    i += 8;
                }
            }

            // текущая конфигурация и сохранённая в базе данных совпадают?
            if (memcmp(auiTempArray,
                       m_puiIntermediateBuff,
                       (sizeof(struct CConfigurationCreate::TConfigDataProgrammerPackOne))) == 0)
            {
                std::cout << "CConfigurationCheck::Fsm CONFIGURATION_CHECK_CONFIGURATION_COMPARE_START 2"  << std::endl;
                // текущая конфигурация и сохранённая в базе данных совпадают.
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
                SetFsmState(DONE_OK);
            }
            else
            {
                std::cout << "CConfigurationCheck::Fsm CONFIGURATION_CHECK_CONFIGURATION_COMPARE_START 3"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(DONE_ERROR);
            }
        }
        break;

//-------------------------------------------------------------------------------
    default:
        break;
    }
}

//-------------------------------------------------------------------------------

