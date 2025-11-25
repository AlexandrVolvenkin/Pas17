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
#include "CommunicationDevice.h"
#include "SerialPortCommunicationDevice.h"
#include "ModbusSlave.h"
#include "SettingsLoad.h"

using namespace std;

//-------------------------------------------------------------------------------
CSettingsLoad::CSettingsLoad()
{
    std::cout << "CSettingsLoad constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[256];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CSettingsLoad::~CSettingsLoad()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
void CSettingsLoad::SetInternalModuleName(std::string sName)
{
    m_sInternalModuleName = sName;
}

//-------------------------------------------------------------------------------
void CSettingsLoad::SetDeviceControlName(std::string sName)
{
    m_sDeviceControlName = sName;
}

//-------------------------------------------------------------------------------
uint8_t CSettingsLoad::Init(void)
{
    std::cout << "CSettingsLoad Init"  << std::endl;
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
    SetCustomerDataContainer(GetExecutorDataContainerPointer());

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
uint8_t CSettingsLoad::Fsm(void)
{
//    std::cout << "CSettingsLoad::Fsm 1" << endl;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CSettingsLoad::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CSettingsLoad::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CSettingsLoad::Fsm START"  << std::endl;
        Init();
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CSettingsLoad::Fsm INIT 1"  << std::endl;
        m_uiInternalModuleId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleName);

        m_uiDeviceControlId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDeviceControlName);

        SetFsmState(READY);

        break;

    case READY:
//        std::cout << "CSettingsLoad::Fsm READY"  << std::endl;
        break;

    case DONE_OK:
//        std::cout << "CSettingsLoad::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CSettingsLoad::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START:
        std::cout << "CSettingsLoad::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING:
//        std::cout << "CSettingsLoad::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CSettingsLoad::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else
        {
            std::cout << "CSettingsLoad::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CSettingsLoad::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
        std::cout << "CSettingsLoad::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        std::cout << "CSettingsLoad::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CSettingsLoad::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
            std::cout << "CSettingsLoad::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CSettingsLoad::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
        std::cout << "CSettingsLoad::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
        }

        break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        std::cout << "CSettingsLoad::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CSettingsLoad::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CSettingsLoad::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(GetFsmNextStateDoneWaitingDoneError());
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CSettingsLoad::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateDoneWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SETTINGS_LOAD_START:
        std::cout << "CSettingsLoad::Fsm SETTINGS_LOAD_START"  << std::endl;
        {
            SetFsmState(SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_START);
        }
        break;

//-------------------------------------------------------------------------------
    case SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_START:
        std::cout << "CSettingsLoad::Fsm SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_START"  << std::endl;
        {
            m_uiDataStoreId =
                GetResources() ->
                GetTaskIdByNameFromMap(m_sDataStoreName);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::READ_BLOCK_DATA_START;
            // сетевой адрес блок 99
            pxDataContainer -> m_uiDataIndex = NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CSettingsLoad::Fsm SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(SETTINGS_LOAD_NETWORK_ADDRESS_SET_START);
        }
        break;

    case SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CSettingsLoad::Fsm SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

    case SETTINGS_LOAD_NETWORK_ADDRESS_SET_START:
        std::cout << "CSettingsLoad::Fsm SETTINGS_LOAD_NETWORK_ADDRESS_SET_START"  << std::endl;
        {
            uint8_t uiAddress =
                (m_puiIntermediateBuff[0] +
                 CONVERT_INTEGER_TO_NATURAL_NUMBER);
            std::cout << "CSettingsLoad::Fsm uiAddress "  << (int)uiAddress << std::endl;

            CModbusSlave* pxModbusRtuSlaveUpperLevel =
                (CModbusSlave*)(GetResources() ->
                                GetTaskPointerByNameFromMap("ModbusRtuSlaveUpperLevel"));
            pxModbusRtuSlaveUpperLevel ->
            SetOwnAddress(uiAddress);

            CModbusSlave* pxModbusTcpSlaveUpperLevel =
                (CModbusSlave*)(GetResources() ->
                                GetTaskPointerByNameFromMap("ModbusTcpSlaveUpperLevel"));
            pxModbusTcpSlaveUpperLevel ->
            SetOwnAddress(uiAddress);

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

//-------------------------------------------------------------------------------
    case SETTINGS_LOAD_STOP_RTU_UPPER_LEVEL_INTERFACE:
        std::cout << "CSettingsLoad::Fsm SETTINGS_LOAD_STOP_RTU_UPPER_LEVEL_INTERFACE"  << std::endl;
        {
            uint8_t uiTaskId =
                GetResources() ->
                GetTaskIdByNameFromMap("ModbusRtuSlaveUpperLevel");

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = uiTaskId;
            pxDataContainer -> m_uiFsmCommandState =
                CModbusSlave::IDDLE;

            SetFsmState(SETTINGS_LOAD_SETTINGS_DATA_BASE_BLOCKS_READ_START);
        }
        break;

    case SETTINGS_LOAD_SETTINGS_DATA_BASE_BLOCKS_READ_START:
        std::cout << "CSettingsLoad::Fsm SETTINGS_LOAD_SETTINGS_DATA_BASE_BLOCKS_READ_START"  << std::endl;
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
            SetFsmNextStateDoneOk(SETTINGS_LOAD_SETTINGS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(SETTINGS_LOAD_SETTINGS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(SETTINGS_LOAD_SETTINGS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(SETTINGS_LOAD_SETTINGS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case SETTINGS_LOAD_SETTINGS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CSettingsLoad::Fsm SETTINGS_LOAD_SETTINGS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(SETTINGS_LOAD_SETTINGS_SET_START);
        }
        break;

    case SETTINGS_LOAD_SETTINGS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CSettingsLoad::Fsm SETTINGS_LOAD_SETTINGS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

    case SETTINGS_LOAD_SETTINGS_SET_START:
        std::cout << "CSettingsLoad::Fsm SETTINGS_LOAD_SETTINGS_SET_START"  << std::endl;
        {
            TPortSettingsPackOne* pxPortSettingsPackOne =
                &(((TPlcSettingsPackOne*)(m_puiIntermediateBuff)) ->
                  xTRs485HighLevelSettingsPackOne);

            CSerialPortCommunicationDevice* pxSerialPortCommunicationDeviceCom1 =
                (CSerialPortCommunicationDevice*)(GetResources() ->
                                                  GetTaskPointerByNameFromMap("SerialPortCommunicationDeviceCom1"));

            pxSerialPortCommunicationDeviceCom1 ->
            SetBaudRate(pxPortSettingsPackOne -> ui8BaudRate);
            pxSerialPortCommunicationDeviceCom1 ->
            SetDataBits(pxPortSettingsPackOne -> ui8DataBits);
            pxSerialPortCommunicationDeviceCom1 ->
            SetParity(pxPortSettingsPackOne -> ui8Parity);
            pxSerialPortCommunicationDeviceCom1 ->
            SetStopBit(pxPortSettingsPackOne -> ui8StopBits);


//            CModbusSlave* pxModbusTcpSlaveUpperLevel =
//                (CModbusSlave*)(GetResources() ->
//                                GetTaskPointerByNameFromMap("ModbusTcpSlaveUpperLevel"));
//            pxModbusTcpSlaveUpperLevel ->
//            SetOwnAddress(uiAddress);

//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//            SetFsmState(DONE_OK);
        }
        break;

    case SETTINGS_LOAD_START_RTU_UPPER_LEVEL_INTERFACE:
        std::cout << "CSettingsLoad::Fsm SETTINGS_LOAD_START_RTU_UPPER_LEVEL_INTERFACE"  << std::endl;
        {
            uint8_t uiTaskId =
                GetResources() ->
                GetTaskIdByNameFromMap("ModbusRtuSlaveUpperLevel");

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = uiTaskId;
            pxDataContainer -> m_uiFsmCommandState =
                CModbusSlave::COMMUNICATION_START;

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

