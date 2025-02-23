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

            if (SetTaskData(GetExecutorDataContainerPointer()))
            {
                SetFsmState(CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_START);
            }
            else
            {
                SetFsmState(CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_START);
            }
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
                ((CDataContainerDataBase*)GetCustomertDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
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
            ((CDataContainerDataBase*)GetCustomertDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
            ((CDataContainerDataBase*)GetCustomertDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CConfigurationCreate::Fsm CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomertDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(DONE_ERROR);
            }
        }
    }
    break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------

