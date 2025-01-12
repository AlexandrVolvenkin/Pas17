﻿//-------------------------------------------------------------------------------
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

////-------------------------------------------------------------------------------
//uint8_t CInternalModuleMuvr::GetType(void)
//{
//    return MISSING_COMPONENT;
//}
//
////-------------------------------------------------------------------------------
//uint8_t CInternalModuleMuvr::GetWorkingStatus(void)
//{
//    return MISSING_COMPONENT;
//}

////-------------------------------------------------------------------------------
//void CInternalModuleMuvr::SetAddress(uint8_t uiAddress)
//{
//    m_uiAddress = uiAddress;
//}
//
////-------------------------------------------------------------------------------
//uint8_t CInternalModuleMuvr::GetAddress(void)
//{
//    return m_uiAddress;
//}
//
////-------------------------------------------------------------------------------
//bool CInternalModuleMuvr::IsAbleToReplace(uint8_t uiType)
//{
//    return false;
//}
//
////-------------------------------------------------------------------------------
//bool CInternalModuleMuvr::IsReadyToStartWork(void)
//{
//    return false;
//}

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

//-----------------------------------------------------------------------------------------------------
uint8_t CInternalModuleMuvr::DataExchange(void)
{
//    int i;
//    int j;
    float fData;
    unsigned char ucCalibrPlus;
    unsigned char ucCalibrMinus;
    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    ucCalibrPlus = 0;
    ucCalibrMinus = 0;
    uint8_t uiCommonIndex = GetCommonIndex();
    // калибруем вход?(если ucCommonIndex не равен 0, то содержит номер калибруемого входа)
    if ((uiCommonIndex > 0) && (uiCommonIndex < (MUVR_TXS_INPUT_NUMBER + 1)))
    {
        // получим номер калибруемого входа.
        ucCalibrPlus = uiCommonIndex;
        // установим начало шкалы НШК?
        if (GetCommandControl() == MUVR_COMMAND_CONTROL_SET_BOTTOM_OF_SCALE)
        {
            SetCommandControl(0);
            // добавим команду модулю.
            ucCalibrPlus |= MUVR_SET_BOTTOM_OF_SCALE;
        }
        // установим конец шкалы ВШК?
        else if (GetCommandControl() == MUVR_COMMAND_CONTROL_SET_TOP_OF_SCALE)
        {
            SetCommandControl(0);
            // добавим команду модулю.
            ucCalibrPlus |= MUVR_SET_TOP_OF_SCALE;
        }
        // данные калибровки передаются дважды, в нормальном виде в байте(ucCalibrPlus) и
        // в инвертированном(!ucCalibrPlus + 1) в байте(ucCalibrMinus).
        // при контроле, сумма (ucCalibrPlus + ucCalibrMinus) должна быть 0.
        ucCalibrMinus = 0x00 - ucCalibrPlus;
    }

    auiSpiTxBuffer[MUVR_CALIBRATION_DATA_OFFSET] = ucCalibrPlus;
    auiSpiTxBuffer[MUVR_CALIBRATION_DATA_OFFSET + 1] = ucCalibrMinus;
    auiSpiTxBuffer[0] = MUVR_GET_MEASURE_DATA_COMMAND;

    //    xSpiCommunicationDevice.Exchange(pxModuleContext ->
//                                     xModuleContextStatic.
//                                     ucModuleAddress,
//                                     auiSpiTxBuffer,
//                                     auiSpiRxBuffer,
//                                     SPI_PREAMBLE_LENGTH +
//                                     MUVR_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH +
//                                     TWO_BYTE_CRC_LENGTH +
//                                     TAIL_ANSWER_LENGTH,
//                                     SPEED_IN_HZ
//                                    );

    usleep(10000);
    m_pxCommunicationDevice -> Exchange(GetAddress(),
                                        auiSpiTxBuffer,
                                        auiSpiRxBuffer,
                                        SPI_PREAMBLE_LENGTH +
                                        MUVR_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH +
                                        TWO_BYTE_CRC_LENGTH +
                                        TAIL_ANSWER_LENGTH,
                                        LOW_SPEED_IN_HZ);

    // что ответил модуль?
    switch(auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET])
    {
    case MUVR_ANSWER_REPER_POINTS_ADC_DATABASE_ERROR:
//        // ошибка БД реперных точек, но будет продолжение обмена.
//        fbAnalogueInputModuleReperPointsAdcDataBaseError = 1;
        // получим код ошибки;
        SetErrorCode(INTERNAL_MODULE_ERROR_REPER_POINTS);

//        // активное состояние события ещё не зарегистрировано?
//        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                    pxModuleContext ->
//                    xModuleContextStatic.
//                    ucModuleContextIndex,
//                    MVAI_REPER_POINTS_DATA_BASE_ERROR_OFFSET))
//        {
//            // зарегистрируем активное состояние события.
//            CEvents::EventRegistration(
//                pxModuleContext ->
//                xModuleContextStatic.
//                ucModuleContextIndex,
//                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                 CEvents::HANDLED_EVENTS_IS_POPUP |
//                 CEvents::HANDLED_EVENTS_IS_SOUND |
//                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                MVAI_REPER_POINTS_DATA_BASE_ERROR_OFFSET,
//                "Реперные т.");
//        }


    case MUVR_GET_MEASURE_DATA_COMMAND:
        // данные не повреждены?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   MUVR_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH) > 0)
        {
            // модуль исправен.
            SetBadAnswerCounter(BAD_MODULE_CYCLE_COUNT_DEFAULT);
//            // сбросим флаг отказа модуля.
//            *(pxModuleContext ->
//              xModuleContextStatic.
//              pucModuleBadStateBufferPointer) = BAD_MODULE_RESPONDED_OK;

//            // неактивное состояние события ещё не зарегистрировано?
//            if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        ucModuleContextIndex,
//                        MTVI5_BAD_ANSWER_ERROR_OFFSET))
//            {
//                // зарегистрируем неактивное состояние события.
//                // норма обмена данными.
//                CEvents::EventRegistration(
//                    pxModuleContext ->
//                    xModuleContextStatic.
//                    ucModuleContextIndex,
//                    (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                    MTVI5_BAD_ANSWER_ERROR_OFFSET,
//                    "Норм. обмена");
//            }

//            //iBadModuleBuffUpdate();
//            // получим данные состояния каналов аналоговых входов.
//            memcpy(pxModuleContext ->
//                   xModuleContextStatic.
//                   pucStatAiBufferPointer,
//                   &auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET],
//                   MUVR_ANALOG_INPUT_QUANTITY);

//            cout << "MUVR_GET_MEASURE_DATA_COMMAND ucCalibrMinus" << (int)ucCalibrMinus << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET];//pxModbusMapping -> buffer_pointer;//
//            for(int i=0; i<8; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }

            // получим измеренные значения всех аналоговых входов модуля.
            for (uint8_t i = 0; i < MUVR_ANALOG_INPUT_QUANTITY; i++)
            {
                // есть обрыв линии или переполнение при расчётах текущего входа?
                if ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_LINE_BREAK) ||
                        (auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_CALCULATION_OVERFLOW))
                {
//                    // данные входа недостоверны, обнулим их.
//                    memset(&(pxModuleContext ->
//                             xModuleContextStatic.
//                             pucAiValueBufferPointer[i * sizeof(float)]),
//                           0,
//                           sizeof(float));
//                    // установим флаг недостоверности - вход недостоверен.
//                    pxModuleContext ->
//                    xModuleContextStatic.
//                    pucBadAiBufferPointer[i] = 1;
//                    // дискретные данные входа недостоверны, обнулим их.
//                    memset(&(pxModuleContext ->
//                             xModuleContextStatic.
//                             pucDiValueBufferPointer[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
//                           0,
//                           MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
//                    // установим флаги недостоверности уставок LL, L, H, HH - недостоверны.
//                    memset(&(pxModuleContext ->
//                             xModuleContextStatic.
//                             pucBadDiBufferPointer[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
//                           1,
//                           MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                }
                // включен режим калибровки текущего входа?
                else if(
                    (((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_LINE_BREAK) == 0) &&
                     ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_CALCULATION_OVERFLOW) == 0)) &&
                    ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_CHANNEL_CALIBRATION)))
                {
//                    // получим измеренное значение и преобразуем.
//                    fData = fStep5ToFloat(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
//                                                                               (i * MUVR_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY)]);
//                    // поместим его в рабочий массив.
//                    memcpy(&(pxModuleContext ->
//                             xModuleContextStatic.
//                             pucAiValueBufferPointer[i * sizeof(float)]),
//                           (unsigned char*)&fData,
//                           sizeof(float));
//                    // установим флаг недостоверности - вход недостоверен.
//                    pxModuleContext ->
//                    xModuleContextStatic.
//                    pucBadAiBufferPointer[i] = 1;
//                    // дискретные данные входа недостоверны, обнулим их.
//                    memset(&(pxModuleContext ->
//                             xModuleContextStatic.
//                             pucDiValueBufferPointer[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
//                           0,
//                           MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
//                    // установим флаги недостоверности уставок LL, L, H, HH - недостоверны.
//                    memset(&(pxModuleContext ->
//                             xModuleContextStatic.
//                             pucBadDiBufferPointer[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
//                           1,
//                           MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                }
                else
                {
//                    // аналоговый вход выведен из обработки?
//                    if (pxModuleContext ->
//                            xModuleContextStatic.
//                            pucAinOffBufferPointer[i])
//                    {
//                        // данные входов модуля выведены из обработки, обнулим их.
//                        memset(&(pxModuleContext ->
//                                 xModuleContextStatic.
//                                 pucAiValueBufferPointer[i * sizeof(float)]),
//                               0,
//                               sizeof(float));
//                        // установим флаг недостоверности - вход недостоверен.
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        pucBadAiBufferPointer[i] = 1;
//                        // дискретные данные входа недостоверны, обнулим их.
//                        memset(&(pxModuleContext ->
//                                 xModuleContextStatic.
//                                 pucDiValueBufferPointer[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
//                               0,
//                               MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
//                        // установим флаги недостоверности уставок LL, L, H, HH - недостоверны.
//                        memset(&(pxModuleContext ->
//                                 xModuleContextStatic.
//                                 pucBadDiBufferPointer[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
//                               1,
//                               MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
//                    }
//                    else
//                    {
//                        // получим измеренное значение и преобразуем.
//                        fData = fStep5ToFloat(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
//                                                                                   (i * MUVR_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY)]);
//                        // поместим его в рабочий массив.
//                        memcpy(&(pxModuleContext ->
//                                 xModuleContextStatic.
//                                 pucAiValueBufferPointer[i * sizeof(float)]),
//                               (unsigned char*)&fData,
//                               sizeof(float));
//                        // сбросим флаг недостоверности - вход достоверен.
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        pucBadAiBufferPointer[i] = 0;
//                        // сбросим флаги уставок LL, L, H, HH.
//                        memset(&(pxModuleContext ->
//                                 xModuleContextStatic.
//                                 pucDiValueBufferPointer[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
//                               0,
//                               MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
//                        // сбросим флаги недостоверности уставок LL, L, H, HH - достоверны.
//                        memset(&(pxModuleContext ->
//                                 xModuleContextStatic.
//                                 pucBadDiBufferPointer[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
//                               0,
//                               MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
//                        // нарушена уставка LL + L?
//                        if ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
//                                ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L)
//                        {
//                            // установим флаг нарушения уставки LL.
//                            (pxModuleContext ->
//                             xModuleContextStatic.
//                             pucDiValueBufferPointer[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
//                                                                                            ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L_OFFSET]) = 1;
//
//                            // установим флаг нарушения уставки L.
//                            (pxModuleContext ->
//                             xModuleContextStatic.
//                             pucDiValueBufferPointer[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
//                                                                                            ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
//                        }
//                        // нарушена уставка L?
//                        if ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
//                                ANALOGUE_INPUT_SET_POINT_VIOLATION_L)
//                        {
//                            // установим флаг нарушения уставки L.
//                            (pxModuleContext ->
//                             xModuleContextStatic.
//                             pucDiValueBufferPointer[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
//                                                                                            ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
//                        }
//                        // нарушена уставка H?
//                        if ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
//                                ANALOGUE_INPUT_SET_POINT_VIOLATION_H)
//                        {
//                            // установим флаг нарушения уставки H.
//                            (pxModuleContext ->
//                             xModuleContextStatic.
//                             pucDiValueBufferPointer[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
//                                                                                            ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
//                        }
//                        // нарушена уставка HH + H?
//                        if ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
//                                ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H)
//                        {
//                            // установим флаг нарушения уставки HH.
//                            (pxModuleContext ->
//                             xModuleContextStatic.
//                             pucDiValueBufferPointer[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
//                                                                                            ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H_OFFSET]) = 1;
//
//                            // установим флаг нарушения уставки H.
//                            (pxModuleContext ->
//                             xModuleContextStatic.
//                             pucDiValueBufferPointer[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
//                                                                                            ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
//                        }
//                    }
                }
            }
            return 0;
        }
        break;

    case MUVR_ANSWER_DATA_NOT_READY:
        // данные не готовы.
        return 0;
        break;

    case MUVR_ANSWER_DATABASE_ERROR:
//        // ошибка БД обработки, конец связи.
//        fbAnalogueInputModuleDataBaseError = 1;
        // получим код ошибки;
        SetErrorCode(INTERNAL_MODULE_ERROR_DATA_BASE);

//        // активное состояние события ещё не зарегистрировано?
//        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                    pxModuleContext ->
//                    xModuleContextStatic.
//                    ucModuleContextIndex,
//                    MTVI5_DATA_BASE_ERROR_OFFSET))
//        {
//            // зарегистрируем активное состояние события.
//            CEvents::EventRegistration(
//                pxModuleContext ->
//                xModuleContextStatic.
//                ucModuleContextIndex,
//                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                 CEvents::HANDLED_EVENTS_IS_POPUP |
//                 CEvents::HANDLED_EVENTS_IS_SOUND |
//                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                MTVI5_DATA_BASE_ERROR_OFFSET,
//                "Ошиб. б. МВА");
//        }

        return 0;
        break;

    default:
        break;
    };


    // модуль признан неисправным?
    if (GetBadAnswerCounter() == 0)
    {
//        // модуль признан неисправным.
//        *(pxModuleContext ->
//          xModuleContextStatic.
//          pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;

//        // активное состояние события ещё не зарегистрировано?
//        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                    pxModuleContext ->
//                    xModuleContextStatic.
//                    ucModuleContextIndex,
//                    MTVI5_BAD_ANSWER_ERROR_OFFSET))
//        {
//            // зарегистрируем активное состояние события.
//            // ошибка обмена данными.
//            CEvents::EventRegistration(
//                pxModuleContext ->
//                xModuleContextStatic.
//                ucModuleContextIndex,
//                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                 CEvents::HANDLED_EVENTS_IS_POPUP |
//                 CEvents::HANDLED_EVENTS_IS_SOUND |
//                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                MTVI5_BAD_ANSWER_ERROR_OFFSET,
//                "Ошиб. обмена");
//        }

//        // данные входов модуля недостоверны, обнулим их.
//        memset(pxModuleContext ->
//               xModuleContextStatic.
//               pucAiValueBufferPointer,
//               0,
//               MUVR_ANALOG_INPUT_QUANTITY * sizeof(float));
//        // установим флаги недостоверности - входы недостоверны.
//        memset(pxModuleContext ->
//               xModuleContextStatic.
//               pucBadAiBufferPointer,
//               1,
//               MUVR_ANALOG_INPUT_QUANTITY);
//        // дискретные данные входов модуля недостоверны, обнулим их.
//        memset(pxModuleContext ->
//               xModuleContextStatic.
//               pucDiValueBufferPointer,
//               0,
//               MUVR_ANALOG_INPUT_QUANTITY * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
//        // установим флаги недостоверности всех дискретных сигналов модуля.
//        memset(pxModuleContext ->
//               xModuleContextStatic.
//               pucBadDiBufferPointer,
//               1,
//               MUVR_ANALOG_INPUT_QUANTITY * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
//        //iBadModuleBuffUpdate();
    }
    else
    {
        // уменьшим значение счётчика - "количество сеансов связи с модулем без ответа".
        SetBadAnswerCounter(GetBadAnswerCounter() - 1);
    }
    return 0;
}

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
        DataExchange();
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------
