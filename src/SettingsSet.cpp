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
#include "ModbusSlave.h"
#include "SettingsLoad.h"
#include "SettingsSet.h"

using namespace std;

//-------------------------------------------------------------------------------
CSettingsSet::CSettingsSet()
{
    std::cout << "CSettingsSet constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[256];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CSettingsSet::~CSettingsSet()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
void CSettingsSet::SetInternalModuleName(std::string sName)
{
    m_sInternalModuleName = sName;
}

//-------------------------------------------------------------------------------
void CSettingsSet::SetDeviceControlName(std::string sName)
{
    m_sDeviceControlName = sName;
}

//-------------------------------------------------------------------------------
uint8_t CSettingsSet::Init(void)
{
    std::cout << "CSettingsSet Init"  << std::endl;
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
    SetCustomerDataContainer(GetExecutorDataContainerPointer());

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
uint8_t CSettingsSet::Fsm(void)
{
//    std::cout << "CSettingsSet::Fsm 1" << endl;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CSettingsSet::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CSettingsSet::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CSettingsSet::Fsm START"  << std::endl;
        Init();
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CSettingsSet::Fsm INIT 1"  << std::endl;
        m_uiInternalModuleId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleName);

        m_uiDeviceControlId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDeviceControlName);

        SetFsmState(READY);

        break;

    case READY:
//        std::cout << "CSettingsSet::Fsm READY"  << std::endl;
        break;

    case DONE_OK:
//        std::cout << "CSettingsSet::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CSettingsSet::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START:
        std::cout << "CSettingsSet::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING:
//        std::cout << "CSettingsSet::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CSettingsSet::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else
        {
            std::cout << "CSettingsSet::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CSettingsSet::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
        std::cout << "CSettingsSet::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        std::cout << "CSettingsSet::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CSettingsSet::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
            std::cout << "CSettingsSet::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CSettingsSet::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
        std::cout << "CSettingsSet::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
        }

        break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        std::cout << "CSettingsSet::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CSettingsSet::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CSettingsSet::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(GetFsmNextStateDoneWaitingDoneError());
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CSettingsSet::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateDoneWaitingError());
            }
        }
    }
    break;

////-------------------------------------------------------------------------------
//    case SETTINGS_LOAD_START:
//        std::cout << "CSettingsSet::Fsm SETTINGS_LOAD_START"  << std::endl;
//        {
//            SetFsmState(SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_START);
//        }
//        break;
//
//    case SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_START:
//        std::cout << "CSettingsSet::Fsm SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_START"  << std::endl;
//        {
//            m_uiDataStoreId =
//                GetResources() ->
//                GetTaskIdByNameFromMap(m_sDataStoreName);
//
//            CDataContainerDataBase* pxDataContainer =
//                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//            pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
//            pxDataContainer -> m_uiFsmCommandState =
//                CDataStore::READ_BLOCK_DATA_START;
//            // сетевой адрес блок 99
//            pxDataContainer -> m_uiDataIndex = NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET;
//            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;
//
//            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
//            SetFsmNextStateDoneOk(SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
//            SetFsmNextStateReadyWaitingError(SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
//            SetFsmNextStateDoneWaitingError(SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
//            SetFsmNextStateDoneWaitingDoneError(SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
//        }
//        break;
//
//    case SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
//        std::cout << "CSettingsSet::Fsm SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
//        {
//            SetFsmState(SETTINGS_LOAD_NETWORK_ADDRESS_SET_START);
//        }
//        break;
//
//    case SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
//        std::cout << "CSettingsSet::Fsm SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
//        {
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
//            SetFsmState(DONE_ERROR);
//        }
//        break;
//
//    case SETTINGS_LOAD_NETWORK_ADDRESS_SET_START:
//        std::cout << "CSettingsSet::Fsm SETTINGS_LOAD_NETWORK_ADDRESS_SET_START"  << std::endl;
//        {
//            uint8_t uiAddress =
//                (m_puiIntermediateBuff[0] +
//                 CONVERT_INTEGER_TO_NATURAL_NUMBER);
//            std::cout << "CSettingsSet::Fsm uiAddress "  << (int)uiAddress << std::endl;
//
//            CModbusSlave* pxModbusRtuSlaveUpperLevel =
//                (CModbusSlave*)(GetResources() ->
//                                GetTaskPointerByNameFromMap("ModbusRtuSlaveUpperLevel"));
//            pxModbusRtuSlaveUpperLevel ->
//            SetOwnAddress(uiAddress);
//
//            CModbusSlave* pxModbusTcpSlaveUpperLevel =
//                (CModbusSlave*)(GetResources() ->
//                                GetTaskPointerByNameFromMap("ModbusTcpSlaveUpperLevel"));
//            pxModbusTcpSlaveUpperLevel ->
//            SetOwnAddress(uiAddress);
//
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//            SetFsmState(DONE_OK);
//        }
//        break;

//-------------------------------------------------------------------------------
    // чтение параметров интерфейса связи верхнего уровня.
    case SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_START:
        //cout << "CDeviceControl::Fsm SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_START" << endl;
    {
        m_uiDataStoreId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDataStoreName);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
        pxDataContainer -> m_uiFsmCommandState =
            CDataStore::READ_BLOCK_DATA_START;
        // параметры настроек блок 101
        pxDataContainer -> m_uiDataIndex = SETTINGS_DATA_BASE_BLOCK_OFFSET;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            memcpy(((uint8_t*)(&(((TPlcSettingsPackOne*)(m_puiIntermediateBuff)) ->
                                 xTRs485HighLevelSettingsPackOne))),
                   (pxCustomerDataContainer -> m_puiDataPointer),
                   (sizeof(struct TPortSettingsPackOne)));

            SetFsmState(SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_START);
        }
        break;

    case SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_START:
        std::cout << "CSettingsSet::Fsm SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_START"  << std::endl;
        {
            m_uiDataStoreId =
                GetResources() ->
                GetTaskIdByNameFromMap(m_sDataStoreName);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
            // сетевой адрес блок 99
            pxDataContainer -> m_uiDataIndex = SETTINGS_DATA_BASE_BLOCK_OFFSET;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CSettingsSet::Fsm SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_INTERFACE_RESTART_START);
        }
        break;

    case SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CSettingsSet::Fsm SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_INTERFACE_RESTART_START:
        std::cout << "CSettingsSet::Fsm SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_INTERFACE_RESTART_START"  << std::endl;
        {
            uint8_t uiTaskId =
                GetResources() ->
                GetTaskIdByNameFromMap("SettingsLoad");

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = uiTaskId;
            pxDataContainer -> m_uiFsmCommandState =
                CSettingsLoad::SETTINGS_LOAD_STOP_RTU_UPPER_LEVEL_INTERFACE;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_INTERFACE_RESTART_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_INTERFACE_RESTART_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CSettingsSet::Fsm SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_INTERFACE_RESTART_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CSettingsSet::Fsm SETTINGS_SET_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
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

