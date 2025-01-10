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
#include "DataContainer.h"
#include "CommunicationDevice.h"
#include "SpiCommunicationDevice.h"
#include "InternalModule.h"
#include "InternalModuleMuvr.h"

using namespace std;

//-------------------------------------------------------------------------------
CInternalModuleMuvr::CInternalModuleMuvr()
{
    std::cout << "CInternalModuleMuvr constructor"  << std::endl;
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CInternalModuleMuvr::~CInternalModuleMuvr()
{
    //dtor
}

//-------------------------------------------------------------------------------
void CInternalModuleMuvr::SetCommunicationDeviceName(std::string sName)
{
    m_sCommunicationDeviceName = sName;
}

//-------------------------------------------------------------------------------
void CInternalModuleMuvr::SetCommunicationDevice(CCommunicationDeviceInterface* pxCommunicationDevice)
{
    m_pxCommunicationDevice = pxCommunicationDevice;
}

//-------------------------------------------------------------------------------
uint8_t CInternalModuleMuvr::Init(void)
{
    std::cout << "CInternalModuleMuvr Init"  << std::endl;
//    m_pxCommandDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
//                               AddDataContainer(std::make_shared<CDataContainerDataBase>()));
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));
}

//-------------------------------------------------------------------------------
bool CInternalModuleMuvr::SetTaskData(CDataContainerDataBase* pxDataContainer)
{
    std::cout << "CInternalModuleMuvr::SetTaskData 1" << std::endl;
    uint8_t uiFsmState = GetFsmState();

    if (IsTaskReady())
    {
        std::cout << "CInternalModuleMuvr::SetTaskData 2" << std::endl;
        *m_pxOperatingDataContainer = *pxDataContainer;
        SetFsmState(m_pxOperatingDataContainer -> m_uiFsmCommandState);
        return true;
    }
    else
    {
        std::cout << "CInternalModuleMuvr::SetTaskData 3" << std::endl;
        return false;
    }
}

//-------------------------------------------------------------------------------
bool CInternalModuleMuvr::GetTaskData(CDataContainerDataBase* pxDataContainer)
{
    std::cout << "CInternalModuleMuvr::SetTaskData 1" << std::endl;

    m_pxOperatingDataContainer -> m_uiFsmCommandState = GetFsmState();
    *pxDataContainer = *m_pxOperatingDataContainer;

    return true;
}

//-------------------------------------------------------------------------------
uint8_t CInternalModuleMuvr::GetType(void)
{
    return MISSING_COMPONENT;
}

//-------------------------------------------------------------------------------
uint8_t CInternalModuleMuvr::GetWorkingStatus(void)
{
    return MISSING_COMPONENT;
}

//-------------------------------------------------------------------------------
void CInternalModuleMuvr::SetAddress(uint8_t uiAddress)
{
    m_uiAddress = uiAddress;
}

//-------------------------------------------------------------------------------
uint8_t CInternalModuleMuvr::GetAddress(void)
{
    return m_uiAddress;
}

//-------------------------------------------------------------------------------
bool CInternalModuleMuvr::IsAbleToReplace(uint8_t uiType)
{
    return false;
}

//-------------------------------------------------------------------------------
bool CInternalModuleMuvr::IsReadyToStartWork(void)
{
    return false;
}

////-------------------------------------------------------------------------------
//uint8_t CInternalModuleMuvr::GetModuleType(uint8_t uiAddress)
//{
//    std::cout << "CInternalModuleMuvr::GetModuleType 1"  << std::endl;
//    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];
//
//    usleep(10000);
//    auiSpiTxBuffer[0] = GET_MODULE_TYPE_COMMAND;
//    m_pxCommunicationDevice -> Exchange(uiAddress,
//                                        auiSpiTxBuffer,
//                                        auiSpiRxBuffer,
//                                        SPI_PREAMBLE_LENGTH +
//                                        GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH +
//                                        ONE_BYTE_CRC_LENGTH,
//                                        LOW_SPEED_IN_HZ);
//    std::cout << "CInternalModuleMuvr::GetModuleType 2"  << std::endl;
//
//    {
//        cout << "GET_MODULE_TYPE_COMMAND auiSpiRxBuffer" << endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
//        for(int i=0; i<16; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//    }
//
//    // модуль отвечает?
//    if (auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET] ==
//            GET_MODULE_TYPE_COMMAND) // if echo answer is ok.
//    {
//        std::cout << "CInternalModuleMuvr::GetModuleType 3"  << std::endl;
////                std::cout << "CInternalModuleMuvr::GetModuleType ucSlaveAddress 1 "  << (int)ucSlaveAddress << std::endl;
//        // обмен данными прошёл без ошибок?
//        if ((iCrcSummOneByteCompare(&auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET],
//                                    GET_MODULE_TYPE_COMMAND_LENGTH +
//                                    GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH) > 0))
//        {
//            std::cout << "CInternalModuleMuvr::GetModuleType 4"  << std::endl;
////            // модуль присутствует. увеличим значение переменной -
////            // фактическое количество модулей в системе.
////            (xPlcConfigSearchTemp.ui8ModulesQuantity)++;
////            // сохраним тип модуля в массиве для упорядочивания следования модулей при поиске на SPI.
////            xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Type =
////                auiSpiRxBuffer[MODULE_TYPE_OFFSET];
////            // сохраним адрес модуля в массиве для упорядочивания следования модулей при поиске на SPI.
////            xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address =
////                ucSlaveAddress;
////
////            AddSystemComponent(auiSpiRxBuffer[MODULE_TYPE_OFFSET],
////                               ucSlaveAddress);
////            std::cout << "CInternalModuleMuvr::GetModuleType ucSlaveAddress "  << (int)xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address << std::endl;
////            std::cout << "CInternalModuleMuvr::GetModuleType ui8Type "  << (int)xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Type << std::endl;
//        }
//        else
//        {
//            std::cout << "CInternalModuleMuvr::GetModuleType 5"  << std::endl;
////            // сохраним адрес модуля в массиве для упорядочивания следования модулей при поиске на SPI.
////            xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address =
////                ucSlaveAddress;
////
////            AddSystemComponent(0,
////                               ucSlaveAddress);
//        }
//    }
//    std::cout << "CInternalModuleMuvr::GetModuleType 6"  << std::endl;
//}

//-------------------------------------------------------------------------------
uint8_t CInternalModuleMuvr::DataBaseRead(uint8_t uiAddress)
{
    std::cout << "CInternalModuleMuvr::DataBaseRead 1"  << std::endl;
    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char aucTempArray[256];

    memset(&aucTempArray[0],
           0,
           sizeof(aucTempArray));
//
    usleep(10000);
    auiSpiTxBuffer[0] = MUVR_GET_DATA_BASE_COMMAND;
    m_pxCommunicationDevice -> Exchange(uiAddress,
                                        auiSpiTxBuffer,
                                        auiSpiRxBuffer,
                                        SPI_PREAMBLE_LENGTH +
                                        ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
                                        TWO_BYTE_CRC_LENGTH,
                                        LOW_SPEED_IN_HZ);

    {
        cout << "MUVR_GET_DATA_BASE_COMMAND auiSpiRxBuffer" << endl;
        unsigned char *pucSourceTemp;
        pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
        for(int i=0; i<64; )
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
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MUVR_GET_DATA_BASE_COMMAND) // if echo answer is ok.
    {
        // данные не повреждены?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH) > 0)
        {
// в модулях аналогового ввода и в памяти прибора, базы данных хранятся в разных форматах.
//  в модулях аналогового ввода сначала идут данные программирования входов(6 входов, по 20 байт) 120 байт, потом
// текстовые реквизиты входов(6 входов, по 8 байт) 48 байт. 6 входов(120 + 48 = 168) байт.
//  в памяти прибора данные программирования и текстовые реквизиты идут отдельными блоками для каждого входа
// (один вход: 20 байт - данные программирования и 8 байт - текстовые реквизиты). один блок - 28 байт. 6 входов(6 * 28 = 168) байт.
// здесь производится преобразование формата базы данных, от модуля к прибору.
            // получим указатель на данные программирования первого входа, принятые из модуля.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET];
            pucDestination = &aucTempArray[0];
            for (i = 0; i < MUVR_ANALOG_INPUT_QUANTITY; i++)
            {
                // скопируем во временный буфер часть блока - данные программирования одного входа(20 байт).
                memcpy(pucDestination, pucSource, 20);
                // перейдём к данным программирования следующего входа в модуле(+20 байт).
                pucSource += 20;
                // перейдём к следующему блоку данных одного входа в приборе(+28 байт).
                pucDestination += 28;
            }
            // получим указатель на данные текстового реквизита первого входа, принятые из модуля.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + (20 * 6)];
            pucDestination = &aucTempArray[20];
            for (i = 0; i < MUVR_ANALOG_INPUT_QUANTITY; i++)
            {
                // скопируем во временный буфер часть блока - текстовые реквизиты одного входа(8 байт).
                memcpy(pucDestination, pucSource, 8);
                // перейдём к данным текстового реквизита следующего входа в модуле(+8 байт).
                pucSource += 8;
                // перейдём к следующему блоку данных одного входа в приборе(+28 байт).
                pucDestination += 28;
            }

            pucSource = &aucTempArray[0];
//            // получим указатель на блок в базе данных прибора.
//            pucDestination = (unsigned char*)(pxModuleContext ->
//                                              xModuleContextStatic.
//                                              pucDataBasePointer);
//            // база данных не совпадает?
//            if (memcmp((const void*)pucDestination,
//                       (const void*)pucSource,
//                       ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH) != 0)
//            {
//
//                // скопируем базу данных из модуля в прибор.
//                memcpy(pucDestination,
//                       pucSource,
//                       ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH);
//                cout << "CInternalModuleMuvr::DataBaseRead DataBaseCheck NO" << endl;
//                return 0;
//            }
//            else
//            {
//                // база данных совпадает.
//                cout << "CInternalModuleMuvr::DataBaseRead DataBaseCheck OK" << endl;
//                return 1;
//            }

            cout << "CInternalModuleMuvr::DataBaseRead DataBaseCheck CRC OK" << endl;
        }
        else
        {
            // ошибка обмена данными.
            cout << "CInternalModuleMuvr::DataBaseRead DataBaseCheck CRC error" << endl;
            //cout << "MUVR_GET_DATA_BASE_CRC error" << endl;
        }
    }
    else
    {
        // модуль не отвечает.
        cout << "iCInternalModuleMuvr::DataBaseRead DataBaseCheck COMMAND error" << endl;
    }
    return 0;
}

//-------------------------------------------------------------------------------
uint8_t CInternalModuleMuvr::Fsm(void)
{
//    std::cout << "CInternalModuleMuvr::Fsm 1" << endl;
    uint8_t uiReadyTaskCounter = 0;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CInternalModuleMuvr::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CInternalModuleMuvr::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CInternalModuleMuvr::Fsm START"  << std::endl;
        Init();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
//        std::cout << "CInternalModuleMuvr::Fsm INIT 1"  << std::endl;
    {
        CTaskInterface* pxTask =
            GetResources() ->
            GetTaskPointerByNameFromMap(m_sCommunicationDeviceName);

        if (pxTask != 0)
        {
//                std::cout << "CInternalModuleMuvr::Fsm INIT 2"  << std::endl;
            if (pxTask -> GetFsmState() >= READY)
            {
//                    std::cout << "CInternalModuleMuvr::Fsm INIT 3"  << std::endl;
                SetCommunicationDevice((CSpiCommunicationDevice*)pxTask);
                uiReadyTaskCounter += 1;
//                    SetFsmState(READY);
            }
        }
        else
        {
//                std::cout << "CInternalModuleMuvr::Fsm INIT 4"  << std::endl;
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CInternalModuleMuvr::Fsm INIT 5"  << std::endl;
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
//        std::cout << "CInternalModuleMuvr::Fsm READY"  << std::endl;
//    {
//
//        if (GetCommandDataLink() != 0)
//        {
//            std::cout << "CInternalModuleMuvr::Fsm READY 2"  << std::endl;
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
//        std::cout << "CInternalModuleMuvr::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CInternalModuleMuvr::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

    case MUVR_GET_MODULE_TYPE:
//        std::cout << "CInternalModuleMuvr::Fsm MUVR_GET_MODULE_TYPE"  << std::endl;
        GetModuleType(GetAddress());
        SetFsmState(READY);
        break;

    case MUVR_DATA_EXCHANGE:
//        std::cout << "CInternalModuleMuvr::Fsm MUVR_DATA_EXCHANGE"  << std::endl;

        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------
