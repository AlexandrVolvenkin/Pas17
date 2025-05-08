//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include "Configuration.h"
#include "Timer.h"
#include "Task.h"
#include "Platform.h"
#include "Resources.h"
#include "DataStore.h"
#include "Crc.h"
#include "CommunicationDevice.h"
#include "SpiCommunicationDevice.h"
#include "DataContainer.h"
#include "ConfigurationCreate.h"
#include "InternalModuleMuvr.h"
#include "InternalModule.h"

using namespace std;

//-------------------------------------------------------------------------------
CInternalModuleInterface::CInternalModuleInterface()
{
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CInternalModuleInterface::~CInternalModuleInterface()
{
    //dtor
}

//-------------------------------------------------------------------------------








//-------------------------------------------------------------------------------
CInternalModule::CInternalModule()
{
    std::cout << "CInternalModule constructor"  << std::endl;
    m_vpxDevices.clear();
    m_xDevicesIterator = m_vpxDevices.begin();
    m_vuiDevicesId.clear();
    m_xDevicesIdIterator = m_vuiDevicesId.begin();
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CInternalModule::~CInternalModule()
{
    for (auto it = m_vpxDevices.begin(); it != m_vpxDevices.end(); ++it)
    {
        delete *it;
    }
    m_vpxDevices.clear();

    // удалять не нужнор. это не указатели на выделенную память.
//    for (auto it = m_vuiDevicesId.begin(); it != m_vuiDevicesId.end(); ++it)
//    {
//        delete *it;
//    }
    m_vuiDevicesId.clear();
}

//-------------------------------------------------------------------------------
void CInternalModule::SetCommunicationDeviceName(std::string sName)
{
    m_sCommunicationDeviceName = sName;
}

//-------------------------------------------------------------------------------
void CInternalModule::SetCommunicationDevice(CCommunicationDeviceInterface* pxCommunicationDevice)
{
    m_pxCommunicationDevice = pxCommunicationDevice;
}

//-------------------------------------------------------------------------------
uint8_t CInternalModule::Init(void)
{
    std::cout << "CInternalModule Init"  << std::endl;
//    m_pxCommandDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
//                               AddDataContainer(std::make_shared<CDataContainerDataBase>()));
//    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
//                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
    SetCustomerDataContainer(GetExecutorDataContainerPointer());
}

////-------------------------------------------------------------------------------
//uint8_t CInternalModule::GetType(void)
//{
//    return MISSING_COMPONENT;
//}
//
////-------------------------------------------------------------------------------
//uint8_t CInternalModule::GetWorkingStatus(void)
//{
//    return MISSING_COMPONENT;
//}

////-------------------------------------------------------------------------------
//void CInternalModule::SetAddress(uint8_t uiAddress)
//{
//    m_uiAddress = uiAddress;
//}
//
////-------------------------------------------------------------------------------
//uint8_t CInternalModule::GetAddress(void)
//{
//    return m_uiAddress;
//}

////-------------------------------------------------------------------------------
//bool CInternalModule::IsAbleToReplace(uint8_t uiType)
//{
//    return false;
//}
//
////-------------------------------------------------------------------------------
//bool CInternalModule::IsReadyToStartWork(void)
//{
//    return false;
//}

////-------------------------------------------------------------------------------
//void CInternalModule::SetCommonIndex(uint8_t uiCommonIndex)
//{
//    m_uiCommonIndex = uiCommonIndex;
//}
//
////-------------------------------------------------------------------------------
//uint8_t CInternalModule::GetCommonIndex(void)
//{
//    return m_uiCommonIndex;
//}
//
////-------------------------------------------------------------------------------
//void CInternalModule::SetCommandControl(uint8_t uiCommandControl)
//{
//    m_uiCommandControl = uiCommandControl;
//}
//
////-------------------------------------------------------------------------------
//uint8_t CInternalModule::GetCommandControl(void)
//{
//    return m_uiCommandControl;
//}
//
////-------------------------------------------------------------------------------
//void CInternalModule::SetErrorCode(uint8_t uiErrorCode)
//{
//    m_uiErrorCode = uiErrorCode;
//}
//
////-------------------------------------------------------------------------------
//uint8_t CInternalModule::GetErrorCode(void)
//{
//    return m_uiErrorCode;
//}
//
////-------------------------------------------------------------------------------
//void CInternalModule::SetBadAnswerCounter(uint8_t uiBadAnswerCounter)
//{
//    m_uiBadAnswerCounter = uiBadAnswerCounter;
//}
//
////-------------------------------------------------------------------------------
//uint8_t CInternalModule::GetBadAnswerCounter(void)
//{
//    return m_uiBadAnswerCounter;
//}
//
//public uint8_t GetAddress()
//{
//    return m_uiAddress;
//}
//public void SetAddress(uint8_t uiAddress)
//{
//    m_uiAddress = uiAddress;
//}
//
//public uint8_t GetCommonIndex()
//{
//    return m_uiCommonIndex;
//}
//public void SetCommonIndex(uint8_t uiCommonIndex)
//{
//    m_uiCommonIndex = uiCommonIndex;
//}
//
//public uint8_t GetCommandControl()
//{
//    return m_uiCommandControl;
//}
//public void SetCommandControl(uint8_t uiCommandControl)
//{
//    m_uiCommandControl = uiCommandControl;
//}
//
//public uint8_t GetErrorCode()
//{
//    return m_uiErrorCode;
//}
//public void SetErrorCode(uint8_t uiErrorCode)
//{
//    m_uiErrorCode = uiErrorCode;
//}
//
//public uint8_t* GetRxBuffer()
//{
//    return m_puiRxBuffer;
//}
//public void SetRxBuffer(uint8_t* puiRxBuffer)
//{
//    m_puiRxBuffer = puiRxBuffer;
//}
//
//public uint8_t* GetTxBuffer()
//{
//    return m_puiTxBuffer;
//}
//public void SetTxBuffer(uint8_t* puiTxBuffer)
//{
//    m_puiTxBuffer = puiTxBuffer;
//}
//
//public uint8_t* GetDiscreteInputs()
//{
//    return m_puiDiscreteInputs;
//}
//public void SetDiscreteInputs(uint8_t* puiDiscreteInputs)
//{
//    m_puiDiscreteInputs = puiDiscreteInputs;
//}
//
//public uint8_t* GetDiscreteInputsBadState()
//{
//    return m_puiDiscreteInputsBadState;
//}
//public void SetDiscreteInputsBadState(uint8_t* puiDiscreteInputsBadState)
//{
//    m_puiDiscreteInputsBadState = puiDiscreteInputsBadState;
//}
//
//public uint8_t GetBadAnswerCounter()
//{
//    return m_uiBadAnswerCounter;
//}
//public void SetBadAnswerCounter(uint8_t uiBadAnswerCounter)
//{
//    m_uiBadAnswerCounter = uiBadAnswerCounter;
//}

//-------------------------------------------------------------------------------
void CInternalModule::Allocate(void)
{
    std::cout << "CInternalModule::Allocate 1"  << std::endl;

//    m_uiAddress = xMemoryAllocationContext.uiAddress;
//    m_puiRxBuffer = xMemoryAllocationContext.puiRxBuffer;
//    m_puiTxBuffer = xMemoryAllocationContext.puiTxBuffer;
//    m_puiErrorCode = xMemoryAllocationContext.puiErrorCode;
//    // Получим указатель на место в массиве дискретных входов для текущего модуля.
//    m_puiDiscreteInputs =
//        &xMemoryAllocationContext.
//        puiDiscreteInputs[xMemoryAllocationContext.uiUsedDiscreteInputs];
//    // Увеличим общий объём выделенной памяти.
//    xMemoryAllocationContext.
//    uiUsedDiscreteInputs +=
//        MVSN21_DISCRETE_INPUTS_NUMBER;
//
//    // Получим указатель на место в массиве достоверности дискретных входов для текущего модуля.
//    m_puiDiscreteInputsBadState =
//        &xMemoryAllocationContext.
//        puiDiscreteInputsBadState[xMemoryAllocationContext.uiUsedDiscreteInputsBadState];
//    // Увеличим общий объём выделенной памяти.
//    xMemoryAllocationContext.
//    uiUsedDiscreteInputsBadState +=
//        MVSN21_DISCRETE_INPUTS_NUMBER;
//
//    // Получим указатель на место в массиве состояний ошибок для текущего модуля.
//    m_puiErrorAlarmDataArray =
//        &xMemoryAllocationContext.
//        puiErrorAlarmDataArray[DISCRETE_INPUT_MODULE_FAILURE];
//
//    m_uiBadAnswerCounter = 0;
}

//-------------------------------------------------------------------------------
uint8_t CInternalModule::GetModuleType(uint8_t uiAddress)
{
//    std::cout << "CInternalModule::GetModuleType 1"  << std::endl;
    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    usleep(10000);
    auiSpiTxBuffer[0] = GET_MODULE_TYPE_COMMAND;
    m_pxCommunicationDevice -> Exchange(uiAddress,
                                        auiSpiTxBuffer,
                                        auiSpiRxBuffer,
                                        SPI_PREAMBLE_LENGTH +
                                        GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH +
                                        ONE_BYTE_CRC_LENGTH,
                                        LOW_SPEED_IN_HZ);
    std::cout << "CInternalModule::GetModuleType 2"  << std::endl;

    {
        cout << "GET_MODULE_TYPE_COMMAND auiSpiRxBuffer" << endl;
        uint8_t *pucSourceTemp;
        pucSourceTemp = (uint8_t*)auiSpiRxBuffer;
        for(int i=0; i<16; )
        {
            for(int j=0; j<8; j++)
            {
                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
            }
            cout << endl;
            i += 8;
        }
    }

    // модуль отвечает?
    if (auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET] ==
            GET_MODULE_TYPE_COMMAND) // if echo answer is ok.
    {
//        std::cout << "CInternalModule::GetModuleType 3"  << std::endl;
        // обмен данными прошёл без ошибок?
        if ((iCrcSummOneByteCompare(&auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET],
                                    GET_MODULE_TYPE_COMMAND_LENGTH +
                                    GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH) > 0))
        {
//            std::cout << "CInternalModule::GetModuleType 4"  << std::endl;
            // модуль присутствует.
            return auiSpiRxBuffer[MODULE_TYPE_OFFSET];
        }
        else
        {
            std::cout << "CInternalModule::GetModuleType 5"  << std::endl;
            return 0;
        }
    }
    else
    {
        std::cout << "CInternalModule::GetModuleType 6"  << std::endl;
        return 0;
    }
}

//-------------------------------------------------------------------------------
void CInternalModule::SearchModules(void)
{
//    std::cout << "CInternalModule::SearchModules 1"  << std::endl;

    // опросим интерфейс SPI. какие модули присутствуют?
    for (uint8_t i = 0; i < SPI_MAX_BUS_ADDRESS; i++)
    {
        // если модуль не ответил MODULE_REQUEST_QUANTITY раз,
        // то его нет или он неисправен.
        for (uint8_t j = 0; j < MODULE_REQUEST_QUANTITY; j++)
        {
            uint8_t uiType = GetModuleType(i);
//            std::cout << "CInternalModule::SearchModules 2"  << std::endl;
//            std::cout << "CInternalModule::SearchModules uiType "  << (int)uiType << std::endl;
            if (uiType != 0)
            {
//                std::cout << "CInternalModule::SearchModules 3"  << std::endl;
                CConfigurationCreate::TConfigDataPackOne* pxDeviceConfigSearch =
                    (CConfigurationCreate::TConfigDataPackOne*)
                    (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);
                // сохраним тип модуля в массиве для упорядочивания следования модулей при поиске на SPI.
                pxDeviceConfigSearch ->
                axModulesContext[pxDeviceConfigSearch -> uiModulesQuantity].uiType = uiType;
                // сохраним адрес модуля в массиве для упорядочивания следования модулей при поиске на SPI.
                pxDeviceConfigSearch ->
                axModulesContext[pxDeviceConfigSearch -> uiModulesQuantity].uiAddress = i;

                std::cout << "CInternalModule::SearchModules ucSlaveAddress "  << (int)pxDeviceConfigSearch ->
                          axModulesContext[pxDeviceConfigSearch -> uiModulesQuantity].uiAddress << std::endl;
                std::cout << "CInternalModule::SearchModules uiType "  << (int)pxDeviceConfigSearch ->
                          axModulesContext[pxDeviceConfigSearch -> uiModulesQuantity].uiType << std::endl;
                // модуль присутствует. увеличим значение переменной -
                // фактическое количество модулей в системе.
                (pxDeviceConfigSearch -> uiModulesQuantity)++;
                (pxDeviceConfigSearch -> uiInternalModulesQuantity)++;

                if (pxDeviceConfigSearch -> uiModulesQuantity >=
                        INTERNAL_MODULE_QUANTITY)
                {
                    return;
                }

                // перейдём к опросу следующего адреса.
                break;
            }
        }
    }
}

//-------------------------------------------------------------------------------
void CInternalModule::ServiceDataCreate(void)
{
//    std::cout << "CInternalModule::ServiceDataCreate 1"  << std::endl;

    // получим указатель на объект конфигурации.
    CConfigurationCreate::TConfigDataPackOne* pxDeviceConfigSearch =
        (CConfigurationCreate::TConfigDataPackOne*)
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);

    // вычисление служебных переменных для внутренних модулей.
    for (uint8_t i = 0;
            i < INTERNAL_MODULE_QUANTITY;
            i++)
    {
        switch(pxDeviceConfigSearch ->
                axModulesContext[i].uiType)
        {
        case MODULE_TYPE_MUVR:
            pxDeviceConfigSearch ->
            uiLastAnalogueInputModuleAddresPlusOne =
                (pxDeviceConfigSearch ->
                 axModulesContext[i].uiAddress + 1);
            (pxDeviceConfigSearch ->
             uiServiceDiscreteInputModuleQuantity)++;
            (pxDeviceConfigSearch ->
             uiServiceAnalogueInputModuleQuantity)++;
            pxDeviceConfigSearch ->
            uiLastAnalogueInputModuleIndex = i;
            pxDeviceConfigSearch ->
            uiDiscreteOutputQuantity += MUVR_DISCRETE_OUTPUT_QUANTITY;
            pxDeviceConfigSearch ->
            uiDiscreteInputSignalsQuantity += MUVR_DISCRETE_SIGNALS_QUANTITY;
            pxDeviceConfigSearch ->
            uiAnalogueInputSignalsQuantity += MUVR_ANALOG_INPUT_QUANTITY;
            pxDeviceConfigSearch ->
            uiServiceDiscreteInputQuantity += MUVR_DISCRETE_SIGNALS_QUANTITY;
            pxDeviceConfigSearch ->
            uiServiceAnalogueInputQuantity += MUVR_ANALOG_INPUT_QUANTITY;
            break;

        default:
            break;
        };
    }
}

//-------------------------------------------------------------------------------
void CInternalModule::CreateDevices(void)
{
    std::cout << "CInternalModule::CreateDevices 1"  << std::endl;

    // получим указатель на объект конфигурации.
    CConfigurationCreate::TConfigDataPackOne* pxDeviceConfigSearch =
        (CConfigurationCreate::TConfigDataPackOne*)
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);

    for (uint8_t i = 0;
            i < (pxDeviceConfigSearch ->
                 uiModulesQuantity);
            i++)
    {
        uint8_t uiType = pxDeviceConfigSearch ->
                         axModulesContext[i].uiType;
        switch (uiType)
        {
        case MODULE_TYPE_MUVR:
        {
            std::string sDeviceName = "InternalModuleMuvr" + std::to_string(i);
            std::cout << "CInternalModule::CreateDevices sDeviceName " << sDeviceName << std::endl;
            CInternalModuleMuvr* pxInternalModuleMuvr = 0;
            //            pxInternalModuleMuvr =
//                static_cast<CInternalModuleMuvr*>(GetResources() ->
//                                                  AddCommonTaskToMap("InternalModuleMuvr" + std::to_string(i),
//                                                          std::make_shared<CInternalModuleMuvr>()));
            pxInternalModuleMuvr =
                static_cast<CInternalModuleMuvr*>(GetResources() ->
                                                  AddCommonTaskToMap(sDeviceName,
                                                          std::make_shared<CInternalModuleMuvr>()));
            pxInternalModuleMuvr ->
            SetResources(GetResources());
            pxInternalModuleMuvr ->
            SetCommunicationDeviceName("SpiCommunicationDeviceSpi0");
//            pxInternalModuleMuvr ->
//            Allocate();
            pxInternalModuleMuvr ->
            SetAddress(pxDeviceConfigSearch ->
                       axModulesContext[i].uiAddress);
            GetResources() -> AddCurrentlyRunningTasksList(pxInternalModuleMuvr);
            m_vpxDevices.push_back(pxInternalModuleMuvr);
            m_vuiDevicesId.push_back(GetResources() ->
                                     GetTaskIdByNameFromMap(sDeviceName));
        }
        break;

        default:
            break;
        }
    }
}

//-------------------------------------------------------------------------------
void CInternalModule::ModulesDataExchange(void)
{
    if (m_xDevicesIterator == m_vpxDevices.end())
    {
        m_xDevicesIterator = m_vpxDevices.begin();
    }

    (*m_xDevicesIterator) -> Fsm();
    m_xDevicesIterator++;


//    for (auto it = m_vpxDevices.begin(); it != m_vpxDevices.end(); ++it)
//    {
//        (*it) -> Fsm();
//    }
}

////-------------------------------------------------------------------------------
//void CInternalModule::DestroyDevices(uint8_t uiLength)
//{
//    for (uint16_t i = 0; i < uiLength; i++)
//    {
////        delete m_vpxDevices[i];
////        delete m_apxDrivers[i];
//    }
//}

//-------------------------------------------------------------------------------
uint8_t CInternalModule::Fsm(void)
{
//    std::cout << "CInternalModule::Fsm 1" << endl;
    uint8_t uiReadyTaskCounter = 0;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CInternalModule::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CInternalModule::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CInternalModule::Fsm START"  << std::endl;
        Init();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CInternalModule::Fsm INIT 1"  << std::endl;
        {
            CTaskInterface* pxTask =
                GetResources() ->
                GetTaskPointerByNameFromMap(m_sCommunicationDeviceName);

            if (pxTask != 0)
            {
//                std::cout << "CInternalModule::Fsm INIT 2"  << std::endl;
                if (pxTask -> GetFsmState() >= READY)
                {
//                    std::cout << "CInternalModule::Fsm INIT 3"  << std::endl;
                    SetCommunicationDevice((CSpiCommunicationDevice*)pxTask);
                    uiReadyTaskCounter += 1;
//                    SetFsmState(READY);
                }
            }
            else
            {
//                std::cout << "CInternalModule::Fsm INIT 4"  << std::endl;
                if (GetTimerPointer() -> IsOverflow())
                {
                    std::cout << "CInternalModule::Fsm INIT 5"  << std::endl;
                    SetFsmState(STOP);
                }
            }
        }

        if (uiReadyTaskCounter > 0)
        {
            SetFsmState(READY);
        }

        break;

    case READY:
//        std::cout << "CInternalModule::Fsm READY"  << std::endl;
//    {
//
//        if (GetCommandDataLink() != 0)
//        {
//            std::cout << "CInternalModule::Fsm READY 2"  << std::endl;
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
//        std::cout << "CInternalModule::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CInternalModule::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START:
//        std::cout << "CInternalModule::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START"  << std::endl;
    {
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING);
    }
    break;

    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING:
//        std::cout << "CInternalModule::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
//            std::cout << "CInternalModule::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else
        {
//            std::cout << "CInternalModule::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CInternalModule::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
//        std::cout << "CInternalModule::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
    {
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
    }
    break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        std::cout << "CInternalModule::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
//            std::cout << "CInternalModule::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
//            std::cout << "CInternalModule::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CInternalModule::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
//        std::cout << "CInternalModule::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
    {
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
    }

    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        std::cout << "CInternalModule::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
//            std::cout << "CInternalModule::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CInternalModule::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(GetFsmNextStateDoneWaitingDoneError());
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CInternalModule::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateDoneWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SEARCH_MODULES_START:
        std::cout << "CInternalModule::Fsm SEARCH_MODULES_START 1"  << std::endl;
        {
            SearchModules();
            std::cout << "CInternalModule::Fsm SEARCH_MODULES_START 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case SERVICE_DATA_CREATE_START:
        std::cout << "CInternalModule::Fsm SERVICE_DATA_CREATE_START 1"  << std::endl;
        {
            ServiceDataCreate();
            std::cout << "CInternalModule::Fsm SERVICE_DATA_CREATE_START 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case MODULES_HANDLERS_CREATE_START:
        std::cout << "CInternalModule::Fsm MODULES_HANDLERS_CREATE_START 1"  << std::endl;
        {
            CreateDevices();
            std::cout << "CInternalModule::Fsm MODULES_HANDLERS_CREATE_START 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case MODULES_DATA_EXCHANGE_START:
//        std::cout << "CInternalModule::Fsm MODULES_DATA_EXCHANGE_START 1"  << std::endl;
    {
//            m_xDevicesIterator = m_vpxDevices.begin();
        m_xDevicesIdIterator = m_vuiDevicesId.begin();
        SetFsmState(MODULES_DATA_EXCHANGE_NEXT_MODULE_PROCESSING);
    }
    break;

    case MODULES_DATA_EXCHANGE_NEXT_MODULE_PROCESSING:
//        std::cout << "CInternalModule::Fsm MODULES_DATA_EXCHANGE_NEXT_MODULE_PROCESSING 1"  << std::endl;
    {
//            if (m_xDevicesIdIterator == m_vuiDevicesId.end())
//            {
//                std::cout << "CInternalModule::Fsm MODULES_DATA_EXCHANGE_NEXT_MODULE_PROCESSING 2"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//                SetFsmState(DONE_OK);
//            }
//            else
//            {
//            std::cout << "CInternalModule::Fsm MODULES_DATA_EXCHANGE_NEXT_MODULE_PROCESSING 3"  << std::endl;
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = (*m_xDevicesIdIterator);
        m_xDevicesIdIterator++;
        pxDataContainer -> m_uiFsmCommandState =
            CInternalModuleMuvr::MUVR_DATA_EXCHANGE;
        pxDataContainer -> m_puiDataPointer =
            (uint8_t*)(GetResources() -> GetDeviceConfigSearchPointer());

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(MODULES_DATA_EXCHANGE_EXECUTOR_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(DONE_ERROR);
        SetFsmNextStateDoneWaitingError(DONE_ERROR);
        SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
//            }
    }
    break;

    case MODULES_DATA_EXCHANGE_EXECUTOR_ANSWER_PROCESSING:
//        std::cout << "CMainProductionCycle::Fsm MODULES_DATA_EXCHANGE_EXECUTOR_ANSWER_PROCESSING 1"  << std::endl;
    {
        if (m_xDevicesIdIterator == m_vuiDevicesId.end())
        {
//                std::cout << "CInternalModule::Fsm MODULES_DATA_EXCHANGE_EXECUTOR_ANSWER_PROCESSING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        else
        {
//                std::cout << "CInternalModule::Fsm MODULES_DATA_EXCHANGE_EXECUTOR_ANSWER_PROCESSING 3"  << std::endl;
            SetFsmState(MODULES_DATA_EXCHANGE_NEXT_MODULE_PROCESSING);
        }
    }
    break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------
