//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <memory>

#include "Timer.h"
#include "Task.h"
#include "Platform.h"
#include "Resources.h"
#include "DataStore.h"
#include "DataContainer.h"
#include "Link.h"
#include "AnalogueSignals.h"
#include "ConfigurationCreate.h"
#include "DeviceControl.h"

using namespace std;

//-------------------------------------------------------------------------------
CDeviceControl::CDeviceControl()
{
    std::cout << "CDeviceControl constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[256];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CDeviceControl::~CDeviceControl()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
uint8_t CDeviceControl::Init(void)
{
    std::cout << "CDeviceControl Init"  << std::endl;
//    m_pxCommandDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
//                               AddDataContainer(std::make_shared<CDataContainerDataBase>()));
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
}

////-------------------------------------------------------------------------------
//bool CDeviceControl::SetTaskData(CDataContainerDataBase* pxDataContainer)
//{
//    std::cout << "CDeviceControl::SetTaskData 1" << std::endl;
//
//    if (IsTaskReady())
//    {
//        std::cout << "CDeviceControl::SetTaskData 2" << std::endl;
//        *m_pxOperatingDataContainer = *pxDataContainer;
//        SetFsmState(m_pxOperatingDataContainer -> m_uiFsmCommandState);
//        return true;
//    }
//    else
//    {
//        std::cout << "CDeviceControl::SetTaskData 3" << std::endl;
//        return false;
//    }
//}
//
////-------------------------------------------------------------------------------
//bool CDeviceControl::GetTaskData(CDataContainerDataBase* pxDataContainer)
//{
//    std::cout << "CDeviceControl::GetTaskData 1" << std::endl;
//
//    m_pxOperatingDataContainer -> m_uiFsmCommandState = GetFsmState();
//    *pxDataContainer = *m_pxOperatingDataContainer;
//
//    return true;
//}

////-------------------------------------------------------------------------------
//CDataContainerDataBase* CDeviceControl::GetTaskData(void)
//{
//    std::cout << "CDeviceControl::GetTaskData 1" << std::endl;
//
//    m_pxOperatingDataContainer -> m_uiFsmCommandState = GetFsmState();
//    m_pxOperatingDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;
//    m_pxOperatingDataContainer -> m_uiDataLength =
//        m_pxResources ->
//        m_pxDataStore ->
//        GetBlockLength(m_pxOperatingDataContainer -> m_uiDataIndex);
//
//    return m_pxOperatingDataContainer;
//}

////-------------------------------------------------------------------------------
//size_t CDeviceControl::GetObjectLength(void)
//{
//    std::cout << "CDeviceControl GetObjectLength"  << std::endl;
//    return sizeof(*this);
//}

//-------------------------------------------------------------------------------
uint16_t CDeviceControl::ConfigurationRead(uint8_t *puiDestination)
{
    std::cout << "CDeviceControl::ConfigurationRead 1" << std::endl;
    uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
    memcpy(puiDestination, auiTempData, sizeof(auiTempData));
    return sizeof(auiTempData);
}

//-------------------------------------------------------------------------------
uint16_t CDeviceControl::DataBaseBlockRead(uint8_t *puiDestination, uint8_t uiBlockIndex)
{
    std::cout << "CDeviceControl::DataBaseRead 1" << std::endl;
    uint8_t uiFsmState = GetFsmState();

    if ((uiFsmState == READY) ||
            (uiFsmState == DONE_OK) ||
            (uiFsmState == DONE_ERROR))
    {
        std::cout << "CDeviceControl::DataBaseRead 2" << std::endl;
        SetFsmState(DATA_BASE_BLOCK_READ);
        return 1;
    }
    else
    {
        std::cout << "CDeviceControl::DataBaseRead 3" << std::endl;
        return 0;

    }
//    uint16_t uiLength;
//
//    uiLength = m_pxResources ->
//               m_pxDataStore ->
//               GetBlockLength(uiBlockIndex);
//
//    std::cout << "CDeviceControl::DataBaseRead uiLength " << (int)uiLength << std::endl;
//    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CDeviceControl::DataBaseBlockReadAnswer(void)
{
    std::cout << "CDeviceControl::DataBaseBlockReadAnswer 1" << std::endl;

    uint16_t uiLength;

    return uiLength;
}

//-------------------------------------------------------------------------------
// обрабатывает входящие сообщения от Modbus интерфейсов по 69 функции - запись блока базы данных.
uint16_t CDeviceControl::DataBaseBlockWrite(void)
{
    std::cout << "CDeviceControl::DataBaseWrite 1" << std::endl;

    uint8_t uiBlockIndex = m_pxOperatingDataContainer -> m_uiDataIndex;
    uint8_t* puiDataPointer = m_pxOperatingDataContainer -> m_puiDataPointer;
    std::cout << "CDeviceControl::DataBaseBlockWrite  uiBlockIndex "  << (int)uiBlockIndex << std::endl;

    if (m_pxDataStore ->
            WriteBlock(puiDataPointer,
                       (m_pxDataStore ->
                        GetBlockLength(uiBlockIndex)),
                       uiBlockIndex))
    {
        std::cout << "CDeviceControl::DataBaseBlockWrite  2"  << std::endl;
    }
    else
    {
        std::cout << "CDeviceControl::DataBaseBlockWrite  3"  << std::endl;
    }

    // получим указатель на блок базы данных.

    // номер принятого для записи блока базы данных.
    switch(uiBlockIndex)
    {
    case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET:
        cout << "ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET" << endl;
        {
            CAnalogueSignals* pxAnalogueSignals =
                (CAnalogueSignals*)(GetResources() ->
                                    GetTaskPointerByNameFromMap("AnalogueSignals"));

//            m_pxOperatingDataContainer -> m_uiFsmCommandState =
//                CAnalogueSignals::DATA_BASE_BLOCK_MODULE_START_WRITE;
//            m_pxOperatingDataContainer -> m_uiDataIndex = 1;
//            m_pxOperatingDataContainer -> m_puiDataPointer =
//                m_puiIntermediateBuff;
//            pxAnalogueSignals ->
//            SetTaskData(m_pxOperatingDataContainer);

//            CDataContainerDataBase xOperatingDataContainer;
//            xOperatingDataContainer.m_uiFsmCommandState =
//                CAnalogueSignals::DATA_BASE_BLOCK_MODULE_START_WRITE;
//            xOperatingDataContainer.m_uiDataIndex = 1;
//            xOperatingDataContainer.m_puiDataPointer = m_puiIntermediateBuff;
//            pxAnalogueSignals ->
//            SetTaskData(&xOperatingDataContainer);
        }
        break;

    case CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET:
        cout << "CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET" << endl;
        break;

    case CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_OFFSET:
        cout << "CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_OFFSET" << endl;
        break;

    case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET:
        cout << "DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET" << endl;
        break;

    case DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET:
        cout << "DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET" << endl;
        break;

    case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET:
        cout << "TEXT_TITLES_DATA_BASE_BLOCK_OFFSET" << endl;
        break;

    case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET:
        cout << "FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET" << endl;
        break;

    case NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET:
        cout << "NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET" << endl;
        break;

    default:
        cout << "default" << endl;
        break;
    };

    return 0;
}

//-------------------------------------------------------------------------------
uint8_t CDeviceControl::GetFsmOperationStatus(void)
{
//    return ((static_cast<CDataContainerDataBase*>(GetCommandDataContainerPointer())) ->
//            GetFsmOperationStatus());
}

//-------------------------------------------------------------------------------
uint8_t CDeviceControl::Fsm(void)
{
//    std::cout << "CDeviceControl::Fsm 1" << endl;
    uint8_t uiReadyTaskCounter = 0;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CDeviceControl::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CDeviceControl::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CDeviceControl::Fsm START"  << std::endl;
        Init();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
//        std::cout << "CDeviceControl::Fsm INIT 1"  << std::endl;
    {
        CTaskInterface* pxTask =
            GetResources() ->
            GetTaskPointerByNameFromMap(m_sDataStoreName);

        if (pxTask != 0)
        {
//                std::cout << "CDeviceControl::Fsm INIT 2"  << std::endl;
            if (pxTask -> GetFsmState() >= READY)
            {
//                    std::cout << "CDeviceControl::Fsm INIT 3"  << std::endl;
                SetDataStore((CDataStore*)pxTask);
                uiReadyTaskCounter += 1;
//                    SetFsmState(READY);
            }
        }
        else
        {
//                std::cout << "CDeviceControl::Fsm INIT 4"  << std::endl;
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDeviceControl::Fsm INIT 5"  << std::endl;
                SetFsmState(STOP);
            }
        }
    }

    if (uiReadyTaskCounter > 0)
    {
        SetFsmState(READY);
    }


    m_uiConfigurationCreateId =
        GetResources() ->
        GetTaskIdByNameFromMap(m_sConfigurationCreateName);
    break;

    case READY:
//        std::cout << "CDeviceControl::Fsm READY"  << std::endl;
//    {
//
//        if (GetCommandDataLink() != 0)
//        {
//            std::cout << "CDeviceControl::Fsm READY 2"  << std::endl;
//            SetOperatingDataLink(GetCommandDataLink());
//            SetFsmState(GetCommandDataLink() ->
//                        GetDataContainerPointer() ->
//                        GetFsmCommandState());
//            GetCommandDataLink() ->
//            GetDataContainerPointer() ->
//            SetFsmCommandState(0);
//            SetCommandDataLink(0);
//        }
//    }
        break;

    case DONE_OK:
//        std::cout << "CDeviceControl::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CDeviceControl::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START:
        std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING:
//        std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else
        {
            std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
        std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
            std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
        std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
        }

        break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(GetFsmNextStateDoneWaitingDoneError());
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateDoneWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case CONFIGURATION_REQUEST_START:
        std::cout << "CDeviceControl::Fsm CONFIGURATION_REQUEST_START"  << std::endl;
        {
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiConfigurationCreateId;
            pxDataContainer -> m_uiFsmCommandState =
                CConfigurationCreate::CONFIGURATION_REQUEST_START;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            CDataContainerDataBase* pxExecutorDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
                   (pxExecutorDataContainer -> m_puiDataPointer),
                   pxExecutorDataContainer -> m_uiDataLength);
            pxCustomerDataContainer -> m_uiDataLength =
                pxExecutorDataContainer -> m_uiDataLength;

            std::cout << "CDeviceControl::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING pxCustomerDataContainer -> m_uiDataLength "  << (int)pxCustomerDataContainer -> m_uiDataLength << std::endl;
            std::cout << "CDeviceControl::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING pxExecutorDataContainer -> m_uiDataLength "  << (int)pxExecutorDataContainer -> m_uiDataLength << std::endl;

            SetFsmState(DONE_OK);
        }
        break;

    case DATA_BASE_BLOCK_READ:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ 1"  << std::endl;
        {
            uint16_t uiLength =
                m_pxDataStore ->
                ReadBlock(((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer,
                          ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex);

            if (uiLength)
            {
                std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ 2"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataLength = uiLength;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
                SetFsmState(DONE_OK);
            }
            else
            {
                std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ 3"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataLength = uiLength;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
                SetFsmState(DONE_OK);
            }
        }
        break;

    case DATA_BASE_BLOCK_START_WRITE:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_START_WRITE"  << std::endl;
        {
            if (m_pxDataStore ->
                    WriteBlock(((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer,
                               (m_pxDataStore ->
                                GetBlockLength((((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex))),
                               ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex))
            {
                std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_START_WRITE 2"  << std::endl;
                // Установим время ожидания окончания записи.
                GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
                SetFsmState(DATA_BASE_BLOCK_WRITE_END_WAITING);
            }
            else
            {
                std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_START_WRITE 3"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(DONE_ERROR);
            }
        }
        break;

    case DATA_BASE_BLOCK_WRITE_END_WAITING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_END_WAITING"  << std::endl;
        // Ожидаем окончания записи автоматом устройства хранения.
        // Устройство хранения закончило запись успешно?
        if (m_pxDataStore ->
                IsDoneOk())
        {
            std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_END_WAITING 1"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        // Устройство хранения закончило запись не успешно?
        else if (m_pxDataStore ->
                 IsDoneError())
        {
            std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_END_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        else
        {
            // Время ожидания окончания записи закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_END_WAITING 3"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(DONE_ERROR);
            }
        }
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------
