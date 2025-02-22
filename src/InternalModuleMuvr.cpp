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
#include "STEP5_floating_point.h"
#include "AnalogueSignals.h"
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

    Allocate();
}

//-------------------------------------------------------------------------------
bool CInternalModuleMuvr::SetTaskData(CDataContainerDataBase* pxDataContainer)
{
//    std::cout << "CInternalModuleMuvr::SetTaskData 1" << std::endl;

    if (IsTaskReady())
    {
//        std::cout << "CInternalModuleMuvr::SetTaskData 2" << std::endl;
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
//    std::cout << "CInternalModuleMuvr::GetTaskData 1" << std::endl;

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

//-----------------------------------------------------------------------------------------------------
void CInternalModuleMuvr::Allocate(void)
{
    std::cout << "CInternalModuleMuvr::Allocate 1"  << std::endl;

//    m_uiAddress = xMemoryAllocationContext.uiAddress;
//    m_puiRxBuffer = xMemoryAllocationContext.puiRxBuffer;
//    m_puiTxBuffer = xMemoryAllocationContext.puiTxBuffer;
//    m_puiErrorCode = xMemoryAllocationContext.puiErrorCode;

    // Получим указатель на место в массиве дискретных входов для текущего модуля.
    m_puiDiscreteInputsState =
        &(GetResources() ->
          m_puiDiscreteInputsState[GetResources() ->
                                                  m_uiUsedDiscreteInputsState]);
    // Увеличим общий объём выделенной памяти.
    GetResources() ->
    m_uiUsedDiscreteInputsState +=
        MUVR_DISCRETE_INPUT_QUANTITY;


    // Получим указатель на место в массиве достоверности дискретных входов для текущего модуля.
    m_puiDiscreteInputsBadState =
        &(GetResources() ->
          m_puiDiscreteInputsBadState[GetResources() ->
                                                     m_uiUsedDiscreteInputsBadState]);
    // Увеличим общий объём выделенной памяти.
    GetResources() ->
    m_uiUsedDiscreteInputsBadState +=
        MUVR_DISCRETE_INPUT_QUANTITY;


    // Получим указатель на место в массиве аналоговых входов для текущего модуля.
    m_pfAnalogueInputsValue =
        &(GetResources() ->
          m_pfAnalogueInputsValue[GetResources() ->
                                                 m_uiUsedAnalogueInputsValue]);
    // Увеличим общий объём выделенной памяти.
    GetResources() ->
    m_uiUsedAnalogueInputsValue +=
        MUVR_ANALOG_INPUT_QUANTITY;


    // Получим указатель на место в массиве достоверности аналоговых входов для текущего модуля.
    m_puiAnalogueInputsBadState =
        &(GetResources() ->
          m_puiAnalogueInputsBadState[GetResources() ->
                                                     m_uiUsedAnalogueInputsBadState]);
    // Увеличим общий объём выделенной памяти.
    GetResources() ->
    m_uiUsedAnalogueInputsBadState +=
        MUVR_ANALOG_INPUT_QUANTITY;


    // Получим указатель на место в массиве отключения аналоговых входов для текущего модуля.
    m_puiAnalogueInputsOff =
        &(GetResources() ->
          m_puiAnalogueInputsOff[GetResources() ->
                                                m_uiUsedAnalogueInputsOff]);
    // Увеличим общий объём выделенной памяти.
    GetResources() ->
    m_uiUsedAnalogueInputsOff +=
        MUVR_ANALOG_INPUT_QUANTITY;


    // Получим указатель на место в массиве распакованной во внутренний формат базы данных
    // аналоговых сигналов.
    m_pxAnalogueInputDescriptionWork =
        &(GetResources() ->
          m_pxAnalogueInputDescriptionWork[GetResources() ->
                                                          m_uiUsedAnalogueInputDescriptionWork]);
    // Увеличим общий объём выделенной памяти.
    GetResources() ->
    m_uiUsedAnalogueInputDescriptionWork +=
        MUVR_ANALOG_INPUT_QUANTITY;


    m_uiBadAnswerCounter = 0;
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

//-----------------------------------------------------------------------------------------------------
uint8_t CInternalModuleMuvr::DataExchange(void)
{
//    std::cout << "CInternalModuleMuvr::DataExchange 1"  << std::endl;
    unsigned short usData;
    float fData;
    unsigned char ucCalibrPlus;
    unsigned char ucCalibrMinus;
    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    memset(auiSpiTxBuffer, 0, 250);
    ucCalibrPlus = 0;
    ucCalibrMinus = 0;
    uint8_t uiCommonIndex = GetCommonIndex();
    // калибруем вход?(если ucCommonIndex не равен 0, то содержит номер калибруемого входа)
    if ((uiCommonIndex > 0) && (uiCommonIndex < (MUVR_TXS_INPUT_NUMBER + 1)))
    {
        std::cout << "CInternalModuleMuvr::DataExchange 12"  << std::endl;
        // получим номер калибруемого входа.
        ucCalibrPlus = uiCommonIndex;
        // установим начало шкалы НШК?
        if (GetCommandControl() == MUVR_COMMAND_CONTROL_SET_BOTTOM_OF_SCALE)
        {
            std::cout << "CInternalModuleMuvr::DataExchange 13"  << std::endl;
            SetCommandControl(0);
            // добавим команду модулю.
            ucCalibrPlus |= MUVR_SET_BOTTOM_OF_SCALE;
        }
        // установим конец шкалы ВШК?
        else if (GetCommandControl() == MUVR_COMMAND_CONTROL_SET_TOP_OF_SCALE)
        {
            std::cout << "CInternalModuleMuvr::DataExchange 14"  << std::endl;
            SetCommandControl(0);
            // добавим команду модулю.
            ucCalibrPlus |= MUVR_SET_TOP_OF_SCALE;
        }
        // данные калибровки передаются дважды, в нормальном виде в байте(ucCalibrPlus) и
        // в инвертированном(!ucCalibrPlus + 1) в байте(ucCalibrMinus).
        // при контроле, сумма (ucCalibrPlus + ucCalibrMinus) должна быть 0.
        ucCalibrMinus = 0x00 - ucCalibrPlus;
    }
    auiSpiTxBuffer[(SPI_PREAMBLE_LENGTH +
                    MUVR_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH)] = ucCalibrPlus;
    auiSpiTxBuffer[(SPI_PREAMBLE_LENGTH +
                    MUVR_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH + 1)] = ucCalibrMinus;
    auiSpiTxBuffer[(SPI_PREAMBLE_LENGTH +
                    MUVR_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH + 2)] = 0x7e;

    auiSpiTxBuffer[0] = MUVR_GET_MEASURE_DATA_COMMAND;
    auiSpiTxBuffer[1] = 0;

    auiSpiTxBuffer[2] = 0;
    auiSpiTxBuffer[3] = 0;
    auiSpiTxBuffer[4] = 0;
    auiSpiTxBuffer[5] = 0;

    usData = usCrcSummTwoByteCalculation(&auiSpiTxBuffer[1],
                                         5);
    auiSpiTxBuffer[6] = (unsigned char)usData;
    auiSpiTxBuffer[7] = (unsigned char)(usData >> 8);

//    {
//        std::cout << "CInternalModuleMuvr::DataExchange auiSpiTxBuffer"  << std::endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)&auiSpiTxBuffer[0];
//        for(int i=0; i<32 ; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//    }

    usleep(10000);
    m_pxCommunicationDevice -> Exchange(GetAddress(),
                                        auiSpiTxBuffer,
                                        auiSpiRxBuffer,
                                        SPI_PREAMBLE_LENGTH +
                                        MUVR_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH +
                                        TWO_BYTE_CRC_LENGTH +
                                        TAIL_ANSWER_LENGTH,
                                        LOW_SPEED_IN_HZ);

//    std::cout << "CInternalModuleMuvr::DataExchange auiSpiRxBuffer"  << std::endl;
//    unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[0];
//    for(int i=0; i<32 ; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }

    // что ответил модуль?
    switch(auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET])
    {
    case MUVR_ANSWER_REPER_POINTS_ADC_DATABASE_ERROR:
        std::cout << "CInternalModuleMuvr::DataExchange 15"  << std::endl;
    // ошибка БД реперных точек, но будет продолжение обмена.
    case MUVR_GET_MEASURE_DATA_COMMAND:
//        std::cout << "CInternalModuleMuvr::DataExchange 16"  << std::endl;
        // данные не повреждены?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   MUVR_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH) > 0)
        {
//            std::cout << "CInternalModuleMuvr::DataExchange 17"  << std::endl;
            // модуль исправен.
            SetBadAnswerCounter(BAD_MODULE_CYCLE_COUNT_DEFAULT);
//            // сбросим флаг отказа модуля.
//            *(m_puiModuleBadStateBuffer) = BAD_MODULE_RESPONDED_OK;

//            // получим данные состояния каналов аналоговых входов.
//            memcpy(m_puiAnalogueInputsState,
//                   &auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET],
//                   MUVR_ANALOG_INPUT_QUANTITY);

            // получим измеренные значения всех аналоговых входов модуля.
            for (uint8_t i = 0; i < MUVR_ANALOG_INPUT_QUANTITY; i++)
            {
//                std::cout << "CInternalModuleMuvr::DataExchange 2"  << std::endl;
                // есть обрыв линии или переполнение при расчётах текущего входа?
                if ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_LINE_BREAK) ||
                        (auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_CALCULATION_OVERFLOW))
                {
//                    std::cout << "CInternalModuleMuvr::DataExchange 3"  << std::endl;
                    // данные входа недостоверны, обнулим их.
                    memset(&(m_pfAnalogueInputsValue[i]),
                           0,
                           sizeof(float));
                    // установим флаг недостоверности - вход недостоверен.
                    m_puiAnalogueInputsBadState[i] = 1;
                    // дискретные данные входа недостоверны, обнулим их.
                    memset(&(m_puiDiscreteInputsState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
                           0,
                           MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                    // установим флаги недостоверности уставок LL, L, H, HH - недостоверны.
                    memset(&(m_puiDiscreteInputsBadState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
                           1,
                           MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                }
                // включен режим калибровки текущего входа?
                else if(
                    (((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_LINE_BREAK) == 0) &&
                     ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_CALCULATION_OVERFLOW) == 0)) &&
                    ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_CHANNEL_CALIBRATION)))
                {
                    std::cout << "CInternalModuleMuvr::DataExchange 4"  << std::endl;
                    // получим измеренное значение и преобразуем.
                    fData = fStep5ToFloat(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                                                                               (i * MUVR_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY)]);
                    // поместим его в рабочий массив.
                    memcpy(&(m_pfAnalogueInputsValue[i]),
                           (unsigned char*)&fData,
                           sizeof(float));
                    // установим флаг недостоверности - вход недостоверен.
                    m_puiAnalogueInputsBadState[i] = 1;
                    // дискретные данные входа недостоверны, обнулим их.
                    memset(&(m_puiDiscreteInputsState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
                           0,
                           MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                    // установим флаги недостоверности уставок LL, L, H, HH - недостоверны.
                    memset(&(m_puiDiscreteInputsBadState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
                           1,
                           MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                }
                else
                {
//                    std::cout << "CInternalModuleMuvr::DataExchange 5"  << std::endl;
                    // аналоговый вход выведен из обработки?
                    if (m_puiAnalogueInputsOff[i])
                    {
                        std::cout << "CInternalModuleMuvr::DataExchange 51"  << std::endl;
                        // данные входов модуля выведены из обработки, обнулим их.
                        memset(&(m_pfAnalogueInputsValue[i]),
                               0,
                               sizeof(float));
                        // установим флаг недостоверности - вход недостоверен.
                        m_puiAnalogueInputsBadState[i] = 1;
                        // дискретные данные входа недостоверны, обнулим их.
                        memset(&(m_puiDiscreteInputsState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
                               0,
                               MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                        // установим флаги недостоверности уставок LL, L, H, HH - недостоверны.
                        memset(&(m_puiDiscreteInputsBadState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
                               1,
                               MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                    }
                    else
                    {
//                        std::cout << "CInternalModuleMuvr::DataExchange 6"  << std::endl;
                        // получим измеренное значение и преобразуем.
                        fData = fStep5ToFloat(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                                                                                   (i * MUVR_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY)]);
                        // поместим его в рабочий массив.
                        memcpy(&(m_pfAnalogueInputsValue[i]),
                               (unsigned char*)&fData,
                               sizeof(float));
                        // сбросим флаг недостоверности - вход достоверен.
                        m_puiAnalogueInputsBadState[i] = 0;
                        // сбросим флаги уставок LL, L, H, HH.
                        memset(&(m_puiDiscreteInputsState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
                               0,
                               MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                        // сбросим флаги недостоверности уставок LL, L, H, HH - достоверны.
                        memset(&(m_puiDiscreteInputsBadState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
                               0,
                               MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                        // нарушена уставка LL + L?
                        if ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                                ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L)
                        {
                            // установим флаг нарушения уставки LL.
                            (m_puiDiscreteInputsState[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                                                             ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L_OFFSET]) = 1;

                            // установим флаг нарушения уставки L.
                            (m_puiDiscreteInputsState[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                                                             ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                        }
                        // нарушена уставка L?
                        if ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                                ANALOGUE_INPUT_SET_POINT_VIOLATION_L)
                        {
                            // установим флаг нарушения уставки L.
                            (m_puiDiscreteInputsState[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                                                             ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                        }
                        // нарушена уставка H?
                        if ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                                ANALOGUE_INPUT_SET_POINT_VIOLATION_H)
                        {
                            // установим флаг нарушения уставки H.
                            (m_puiDiscreteInputsState[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                                                             ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                        }
                        // нарушена уставка HH + H?
                        if ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                                ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H)
                        {
                            // установим флаг нарушения уставки HH.
                            (m_puiDiscreteInputsState[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                                                             ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H_OFFSET]) = 1;

                            // установим флаг нарушения уставки H.
                            (m_puiDiscreteInputsState[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                                                             ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                        }
                    }
                }
            }
            return 0;
        }
        break;

    case MUVR_ANSWER_DATA_NOT_READY:
        std::cout << "CInternalModuleMuvr::DataExchange 7"  << std::endl;
        // данные не готовы.
        return 0;
        break;

    case MUVR_ANSWER_DATABASE_ERROR:
        std::cout << "CInternalModuleMuvr::DataExchange 8"  << std::endl;
//        // ошибка БД обработки, конец связи.
//        // получим код ошибки;
//        SetErrorCode(INTERNAL_MODULE_ERROR_DATA_BASE);
        return 0;
        break;

    default:
        std::cout << "CInternalModuleMuvr::DataExchange 9"  << std::endl;
        break;
    };


    // модуль признан неисправным?
    if (GetBadAnswerCounter() == 0)
    {
        std::cout << "CInternalModuleMuvr::DataExchange 10"  << std::endl;
//        // модуль признан неисправным.
//        *(m_puiModuleBadStateBuffer) = BAD_MODULE_NOT_RESPONDED;

        // данные входов модуля недостоверны, обнулим их.
        memset(m_pfAnalogueInputsValue,
               0,
               MUVR_ANALOG_INPUT_QUANTITY * sizeof(float));
        // установим флаги недостоверности - входы недостоверны.
        memset(m_puiAnalogueInputsBadState,
               1,
               MUVR_ANALOG_INPUT_QUANTITY);
        // дискретные данные входов модуля недостоверны, обнулим их.
        memset(m_puiDiscreteInputsState,
               0,
               MUVR_ANALOG_INPUT_QUANTITY * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
        // установим флаги недостоверности всех дискретных сигналов модуля.
        memset(m_puiDiscreteInputsBadState,
               1,
               MUVR_ANALOG_INPUT_QUANTITY * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
        //iBadModuleBuffUpdate();
    }
    else
    {
        std::cout << "CInternalModuleMuvr::DataExchange 11"  << std::endl;
        // уменьшим значение счётчика - "количество сеансов связи с модулем без ответа".
        SetBadAnswerCounter(GetBadAnswerCounter() - 1);
    }
    return 0;
}

//-------------------------------------------------------------------------------
uint8_t CInternalModuleMuvr::DataBaseRead(void)
{
    std::cout << "CInternalModuleMuvr::DataBaseRead 1"  << std::endl;
    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char aucTempArray[256];

    memset(&aucTempArray[0],
           0,
           sizeof(aucTempArray));
//
    usleep(10000);
    auiSpiTxBuffer[0] = MUVR_GET_DATA_BASE_COMMAND;
    m_pxCommunicationDevice -> Exchange(GetAddress(),
                                        auiSpiTxBuffer,
                                        auiSpiRxBuffer,
                                        SPI_PREAMBLE_LENGTH +
                                        ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
                                        TWO_BYTE_CRC_LENGTH,
                                        LOW_SPEED_IN_HZ);

    {
        std::cout << "CInternalModuleMuvr::DataBaseRead auiSpiRxBuffer"  << std::endl;
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
        std::cout << "CInternalModuleMuvr::DataBaseRead 2"  << std::endl;
        // данные не повреждены?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH) > 0)
        {
            std::cout << "CInternalModuleMuvr::DataBaseRead 3"  << std::endl;
// в модулях аналогового ввода и в памяти прибора, базы данных хранятся в разных форматах.
//  в модулях аналогового ввода сначала идут данные программирования входов(6 входов, по 20 байт) 120 байт, потом
// текстовые реквизиты входов(6 входов, по 8 байт) 48 байт. 6 входов(120 + 48 = 168) байт.
//  в памяти прибора данные программирования и текстовые реквизиты идут отдельными блоками для каждого входа
// (один вход: 20 байт - данные программирования и 8 байт - текстовые реквизиты). один блок - 28 байт. 6 входов(6 * 28 = 168) байт.
// здесь производится преобразование формата базы данных, от модуля к прибору.
            // получим указатель на данные программирования первого входа, принятые из модуля.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET];
            pucDestination = &aucTempArray[0];
// в соответствии с документом: структ и прот ПАС-17.txt
//Перед переформатированием переносим В87-В89 в массив БД RAM B119 - B121, т.к.
//переформатированная БД займет В0-В79 + 32 = 112 байтов
            pucDestination[119] = pucSource[87];
            pucDestination[120] = pucSource[88];
            pucDestination[121] = pucSource[89];
//            std::cout << "CInternalModuleMuvr::DataBaseRead pucSource[87] "  << (int)(pucSource[87]) << std::endl;
//            std::cout << "CInternalModuleMuvr::DataBaseRead pucSource[88] "  << (int)(pucSource[88]) << std::endl;
//            std::cout << "CInternalModuleMuvr::DataBaseRead pucSource[89] "  << (int)(pucSource[89]) << std::endl;

            for (uint8_t i = 0; i < MUVR_ANALOG_INPUT_QUANTITY; i++)
            {
                // скопируем во временный буфер часть блока - данные программирования одного входа(20 байт).
                memcpy(pucDestination, pucSource, 20);
                // перейдём к данным программирования следующего входа в модуле(+20 байт).
                pucSource += 20;
                // перейдём к следующему блоку данных одного входа в приборе(+28 байт).
                pucDestination += 28;
            }
            // получим указатель на данные текстового реквизита первого входа, принятые из модуля.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                                                             ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_TEXT_DESCRIPTOR_OFFSET];
            pucDestination = &aucTempArray[20];
            for (uint8_t i = 0; i < MUVR_ANALOG_INPUT_QUANTITY; i++)
            {
                // скопируем во временный буфер часть блока - текстовые реквизиты одного входа(8 байт).
                memcpy(pucDestination, pucSource, 8);
                // перейдём к данным текстового реквизита следующего входа в модуле(+8 байт).
                pucSource += 8;
                // перейдём к следующему блоку данных одного входа в приборе(+28 байт).
                pucDestination += 28;
            }

            pucSource = &aucTempArray[0];
            memcpy(m_pxOperatingDataContainer -> m_puiDataPointer,
                   pucSource,
                   ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH);
            m_pxOperatingDataContainer -> m_uiDataLength = MUVR_ANALOG_INPUT_QUANTITY;

            cout << "CInternalModuleMuvr::DataBaseRead 4" << endl;
        }
        else
        {
            // ошибка обмена данными.
            cout << "CInternalModuleMuvr::DataBaseRead 5" << endl;
        }
    }
    else
    {
        // модуль не отвечает.
        cout << "iCInternalModuleMuvr::DataBaseRead 6" << endl;
    }
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// записывает базу данных из RAM прибора в EEPROM модуля.
uint8_t CInternalModuleMuvr::DataBaseBlockWrite(void)
{
    unsigned short usData;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned int nuiBusyTimeCounter;
    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];

// в модулях аналогового ввода и в памяти прибора, базы данных хранятся в разных форматах.
//  в модулях аналогового ввода сначала идут данные программирования входов(6 входов, по 20 байт) 120 байт, потом
// текстовые реквизиты входов(6 входов, по 8 байт) 48 байт. 6 входов(120 + 48 = 168) байт.
//  в памяти прибора данные программирования и текстовые реквизиты идут отдельными блоками для каждого входа
// (один вход: 20 байт - данные программирования и 8 байт - текстовые реквизиты). один блок - 28 байт. 6 входов(6 * 28 = 168) байт.
// здесь производится преобразование формата базы данных, от прибора к модулю.
    memset(&aucTempArray[0],
           0,
           sizeof(aucTempArray));
    // получим указатель на блок базы данных, принятый по Modbus во временный буфер.

    pucSource = m_pxOperatingDataContainer -> m_puiDataPointer;

    // получим указатель на данные программирования первого входа, для записи в модуль.
    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET];

// в соответствии с документом: структ и прот ПАС-17.txt
//Перед переформатированием переносим В87-В89 в массив БД RAM B119 - B121, т.к.
//переформатированная БД займет В0-В79 + 32 = 112 байтов
    pucDestination[87] = pucSource[119];
    pucDestination[88] = pucSource[120];
    pucDestination[89] = pucSource[121];

    for (uint8_t i = 0; i < MUVR_ANALOG_INPUT_QUANTITY; i++)
    {
        // скопируем во временный буфер часть блока - данные программирования одного входа(20 байт).
        memcpy(pucDestination, pucSource, 20);
        // перейдём к данным программирования следующего входа в модуле(+20 байт).
        pucDestination += 20;
        // перейдём к следующему блоку данных одного входа в приборе(+28 байт).
        pucSource += 28;
    }
    // получим указатель на блок базы данных, принятый по Modbus во временный буфер.

    pucSource = m_pxOperatingDataContainer -> m_puiDataPointer;

    // перейдём к данным текстового реквизита первого входа(+20 байт).
    pucSource += 20;
    // получим указатель на данные текстового реквизита первого входа, для записи в модуль.
    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET + (120)];
    for (uint8_t i = 0; i < MUVR_ANALOG_INPUT_QUANTITY; i++)
    {
        // скопируем во временный буфер часть блока - текстовые реквизиты одного входа(8 байт).
        memcpy(pucDestination, pucSource, 8);
        // перейдём к данным текстового реквизита следующего входа в модуле(+8 байт).
        pucDestination += 8;
        // перейдём к следующему блоку данных одного входа в приборе(+28 байт).
        pucSource += 28;
    }

    // получим указатель на преобразованную базу данных от прибора к модулю.
    pucSource = &aucTempArray[SPI_DATA_BYTE_OFFSET];
    usData = usCrcSummTwoByteCalculation(pucSource,
                                         ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH);
    // получим указатель на место CRC в пакете.
    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET +
                                                        ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH];
    *pucDestination++ = (unsigned char)usData;
    *pucDestination = (unsigned char)(usData >> 8);
    // скопируем преобразованную базу данных из временного в буфер TX SPI.
    memcpy(&auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET],
           pucSource,
           ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
           TWO_BYTE_CRC_LENGTH);
    // отправим данные в модуль.
    usleep(10000);
    auiSpiTxBuffer[0] = MUVR_SET_DATA_BASE_COMMAND;
    m_pxCommunicationDevice -> Exchange(GetAddress(),
                                        auiSpiTxBuffer,
                                        auiSpiRxBuffer,
                                        SPI_PREAMBLE_LENGTH +
                                        ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
                                        TWO_BYTE_CRC_LENGTH,
                                        LOW_SPEED_IN_HZ);

//    pxModuleContext ->
//    xModuleContextDinamic.
//    usAuxiliaryCounter = 0;

    return 1;
}

////-----------------------------------------------------------------------------------------------------
//int CInternalModuleMuvr::DataBaseBlockWriteError(TModuleContext *pxModuleContext)
//{
//    // База данных не записана?
//    if ((pxModuleContext ->
//            xModuleContextDinamic.
//            usAuxiliaryCounter) >= 20)
//    {
//        // ошибка БД обработки, конец связи.
//        fbAnalogueInputModuleDataBaseError = 1;
//        // получим код ошибки;
//        (pxModuleContext ->
//         xModuleContextDinamic.
//         ucErrorCode) = INTERNAL_MODULE_ERROR_DATA_BASE;
//
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
//
//        return -1;
//    }
//    else
//    {
//        // Увеличим значение счётчика - "количество сеансов связи с модулем без ответа".
//        (pxModuleContext ->
//         xModuleContextDinamic.
//         usAuxiliaryCounter)++;
//
//        return 0;
//    }
//}

//-----------------------------------------------------------------------------------------------------
// записывает базу данных из RAM прибора в EEPROM модуля.
uint8_t CInternalModuleMuvr::DataBaseBlockWriteCheck(void)
{
    unsigned short usData;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned int nuiBusyTimeCounter;
    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // примем данные с результатом программирования.
    auiSpiTxBuffer[0] = MUVR_CHECK_DATA_BASE_WRITE_COMMAND;
    m_pxCommunicationDevice -> Exchange(GetAddress(),
                                        auiSpiTxBuffer,
                                        auiSpiRxBuffer,
                                        SPI_PREAMBLE_LENGTH +
                                        TAIL_ANSWER_LENGTH,
                                        LOW_SPEED_IN_HZ);

    return 0;

    // есть подтверждение записи базы данных в EEPROM модуля?
    if((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == DATA_EXCHANGE_OK)
    {
        // есть подтверждение записи базы данных в EEPROM модуля.
        cout << "CInternalModuleMuvr::DataBaseBlockWriteCheck OK" << endl;
        return DATA_EXCHANGE_OK;
    }
    // модуль занят записью данных в EEPROM?
    else if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == MUVR_CHECK_DATA_BASE_WRITE_ANSWER_BUSY)
    {
        // модуль занят записью данных в EEPROM.
        cout << "CInternalModuleMuvr::DataBaseBlockWriteCheck BUSY" << endl;
        return MUVR_CHECK_DATA_BASE_WRITE_ANSWER_BUSY;
    }
    // произошла ошибка при записи базы данных в EEPROM модуля?
    else if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == DATA_EXCHANGE_ERROR)
    {
        // произошла ошибка при записи базы данных в EEPROM модуля.
        cout << "CInternalModuleMuvr::DataBaseBlockWriteCheck ERROR" << endl;
        return DATA_EXCHANGE_ERROR;
    }
    else
    {
        // произошла ошибка при записи базы данных в EEPROM модуля.
        cout << "CInternalModuleMuvr::DataBaseBlockWriteCheck ERROR" << endl;
        return MUVR_CHECK_DATA_BASE_WRITE_ANSWER_BUSY;
    }
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
        SetFsmState(DONE_OK);
        break;

    case MUVR_DATA_BASE_READ:
        std::cout << "CInternalModuleMuvr::Fsm MUVR_DATA_BASE_READ"  << std::endl;
        DataBaseRead();
        SetFsmState(DONE_OK);
        break;

    case MUVR_WRITE_DATA_BASE:
        std::cout << "CInternalModuleMuvr::Fsm MUVR_WRITE_DATA_BASE"  << std::endl;
        DataBaseBlockWrite();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(MUVR_WRITE_DATA_BASE_CHECK);
        break;

    case MUVR_WRITE_DATA_BASE_CHECK:
        std::cout << "CInternalModuleMuvr::Fsm MUVR_WRITE_DATA_BASE_CHECK 1"  << std::endl;
        {
            uint8_t uiFsmState = DataBaseBlockWriteCheck();

            if (uiFsmState == DATA_EXCHANGE_OK)
            {
                std::cout << "CInternalModuleMuvr::Fsm MUVR_WRITE_DATA_BASE_CHECK 2"  << std::endl;
                SetFsmState(DONE_OK);
            }
            else if (uiFsmState == DATA_EXCHANGE_ERROR)
            {
                std::cout << "CInternalModuleMuvr::Fsm MUVR_WRITE_DATA_BASE_CHECK 3"  << std::endl;
                SetFsmState(DONE_ERROR);
            }
            else
            {
                // Время ожидания выполнения запроса закончилось?
                if (GetTimerPointer() -> IsOverflow())
                {
                    std::cout << "CInternalModuleMuvr::Fsm MUVR_WRITE_DATA_BASE_CHECK 4"  << std::endl;
                    SetFsmState(DONE_ERROR);
                }
            }
        }

//        switch (DataBaseBlockWriteCheck())
//        {
//        case DATA_EXCHANGE_OK:
//            SetFsmState(DONE_OK);
//            break;
//
//        case MUVR_CHECK_DATA_BASE_WRITE_ANSWER_BUSY:
//            break;
//
//        case DATA_EXCHANGE_ERROR:
//            SetFsmState(DONE_ERROR);
//            break;
//
//        default:
//            SetFsmState(DONE_ERROR);
//            break;
//        }

        break;

    case MUVR_DATA_EXCHANGE:
//        std::cout << "CInternalModuleMuvr::Fsm MUVR_DATA_EXCHANGE"  << std::endl;
        DataExchange();
        SetFsmState(DONE_OK);
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------
