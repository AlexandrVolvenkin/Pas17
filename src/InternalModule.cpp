//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include "Timer.h"
#include "Task.h"
#include "Platform.h"
#include "Resources.h"
#include "DataStore.h"
#include "Crc.h"
#include "CommunicationDevice.h"
#include "SpiCommunicationDevice.h"
#include "DataContainer.h"
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
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CInternalModule::~CInternalModule()
{
    //dtor
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
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));
}

//-------------------------------------------------------------------------------
bool CInternalModule::SetTaskData(CDataContainerDataBase* pxDataContainer)
{
    std::cout << "CInternalModule::SetTaskData 1" << std::endl;
    uint8_t uiFsmState = GetFsmState();

    if (IsTaskReady())
    {
        std::cout << "CInternalModule::SetTaskData 2" << std::endl;
        *m_pxOperatingDataContainer = *pxDataContainer;
        SetFsmState(m_pxOperatingDataContainer -> m_uiFsmCommandState);
        return true;
    }
    else
    {
        std::cout << "CInternalModule::SetTaskData 3" << std::endl;
        return false;
    }
}

//-------------------------------------------------------------------------------
bool CInternalModule::GetTaskData(CDataContainerDataBase* pxDataContainer)
{
    std::cout << "CInternalModule::SetTaskData 1" << std::endl;

    m_pxOperatingDataContainer -> m_uiFsmCommandState = GetFsmState();
    *pxDataContainer = *m_pxOperatingDataContainer;

    return true;
}

//-------------------------------------------------------------------------------
uint8_t CInternalModule::GetType(void)
{
    return MISSING_COMPONENT;
}

//-------------------------------------------------------------------------------
uint8_t CInternalModule::GetWorkingStatus(void)
{
    return MISSING_COMPONENT;
}

//-------------------------------------------------------------------------------
void CInternalModule::SetAddress(uint8_t uiAddress)
{
    m_uiAddress = uiAddress;
}

//-------------------------------------------------------------------------------
uint8_t CInternalModule::GetAddress(void)
{
    return m_uiAddress;
}

//-------------------------------------------------------------------------------
bool CInternalModule::IsAbleToReplace(uint8_t uiType)
{
    return false;
}

//-------------------------------------------------------------------------------
bool CInternalModule::IsReadyToStartWork(void)
{
    return false;
}

//-------------------------------------------------------------------------------
uint8_t CInternalModule::GetModuleType(uint8_t uiAddress)
{
    std::cout << "CInternalModule::GetModuleType 1"  << std::endl;
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
                unsigned char *pucSourceTemp;
                pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
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
    std::cout << "CInternalModule::GetModuleType 3"  << std::endl;
//                std::cout << "CInternalModule::GetModuleType ucSlaveAddress 1 "  << (int)ucSlaveAddress << std::endl;
        // обмен данными прошёл без ошибок?
        if ((iCrcSummOneByteCompare(&auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET],
                                    GET_MODULE_TYPE_COMMAND_LENGTH +
                                    GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH) > 0))
        {
    std::cout << "CInternalModule::GetModuleType 4"  << std::endl;
//            // модуль присутствует. увеличим значение переменной -
//            // фактическое количество модулей в системе.
//            (xPlcConfigSearchTemp.ui8ModulesQuantity)++;
//            // сохраним тип модуля в массиве для упорядочивания следования модулей при поиске на SPI.
//            xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Type =
//                auiSpiRxBuffer[MODULE_TYPE_OFFSET];
//            // сохраним адрес модуля в массиве для упорядочивания следования модулей при поиске на SPI.
//            xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address =
//                ucSlaveAddress;
//
//            AddSystemComponent(auiSpiRxBuffer[MODULE_TYPE_OFFSET],
//                               ucSlaveAddress);
//            std::cout << "CInternalModule::GetModuleType ucSlaveAddress "  << (int)xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address << std::endl;
//            std::cout << "CInternalModule::GetModuleType ui8Type "  << (int)xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Type << std::endl;
        }
        else
        {
    std::cout << "CInternalModule::GetModuleType 5"  << std::endl;
//            // сохраним адрес модуля в массиве для упорядочивания следования модулей при поиске на SPI.
//            xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address =
//                ucSlaveAddress;
//
//            AddSystemComponent(0,
//                               ucSlaveAddress);
        }
    }
    std::cout << "CInternalModule::GetModuleType 6"  << std::endl;
}

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
//        std::cout << "CInternalModule::Fsm INIT 1"  << std::endl;
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

    default:
        break;
    }
}

//-------------------------------------------------------------------------------
