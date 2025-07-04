
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <typeinfo>
#include <stdint.h>
#include <iostream>
#include <stdio.h>



#include "Task.h"
#include "Timer.h"
#include "Platform.h"
#include "Resources.h"
#include "DataStore.h"
#include "DataContainer.h"
//#include "Link.h"
//#include "AnalogueSignals.h"
#include "ConfigurationCreate.h"
#include "InternalModuleMuvr.h"
#include "ModbusSlave.h"
#include "DeviceControl.h"
#include "Alarm.h"

using namespace std;



//-----------------------------------------------------------------------------------------------------
CAlarmDfa::CAlarmDfa()
{
    m_puiIntermediateBuff = new uint8_t[256];
    SetFsmState(START);
}

//-----------------------------------------------------------------------------------------------------
CAlarmDfa::~CAlarmDfa()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
uint8_t CAlarmDfa::Init(void)
{
    std::cout << "CAlarmDfa Init"  << std::endl;
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
    SetCustomerDataContainer(GetExecutorDataContainerPointer());

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
void CAlarmDfa::Allocate(void)
{
    std::cout << "CDiscreteSignals::Allocate 1"  << std::endl;

////    m_uiAddress = xMemoryAllocationContext.uiAddress;
////    m_puiRxBuffer = xMemoryAllocationContext.puiRxBuffer;
////    m_puiTxBuffer = xMemoryAllocationContext.puiTxBuffer;
////    m_puiErrorCode = xMemoryAllocationContext.puiErrorCode;
//
//    // Получим указатель на место в массиве дискретных входов для текущего модуля.
    //    m_puiDiscreteInputsState =
//        &(GetResources() ->
//          m_puiDiscreteInputsState[GetResources() ->
//                                                  m_uiUsedDiscreteInputsState]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedDiscreteInputsState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//
//
//    // Получим указатель на место в массиве достоверности дискретных входов для текущего модуля.
//    m_puiDiscreteInputsBadState =
//        &(GetResources() ->
//          m_puiDiscreteInputsBadState[GetResources() ->
//                                                     m_uiUsedDiscreteInputsBadState]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedDiscreteInputsBadState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;


    // Подключим буфер для управления дискретными выходами.
    m_pxDiscreteOutputControl = (GetResources() -> m_pxDiscreteOutputControl);
//
//
//    // Получим указатель на место в массиве аналоговых входов для текущего модуля.
//    m_pfAnalogueInputsValue =
//        &(GetResources() ->
//          m_pfAnalogueInputsValue[GetResources() ->
//                                                 m_uiUsedAnalogueInputsValue]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedAnalogueInputsValue +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // Получим указатель на место в массиве состояния аналоговых входов для текущего модуля.
//    m_puiAnalogueInputsState =
//        &(GetResources() ->
//          m_puiAnalogueInputsState[GetResources() ->
//                                                  m_uiUsedAnalogueInputsState]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedAnalogueInputsBadState +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // Получим указатель на место в массиве достоверности аналоговых входов для текущего модуля.
//    m_puiAnalogueInputsBadState =
//        &(GetResources() ->
//          m_puiAnalogueInputsBadState[GetResources() ->
//                                                     m_uiUsedAnalogueInputsBadState]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedAnalogueInputsBadState +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // Получим указатель на место в массиве состояний дискретных сигналов порождаемых аналоговыми входами.
//    m_puiAnalogueInputDiscreteInputsState =
//        &(GetResources() ->
//          m_puiAnalogueInputDiscreteInputsState[GetResources() ->
//                                 m_uiUsedAnalogueInputDiscreteInputsState]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedAnalogueInputDiscreteInputsState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//
//
//    // Получим указатель на место в массиве флагов недостоверности состояний дискретных сигналов порождаемых аналоговыми входами.
//    m_puiAnalogueInputDiscreteInputsBadState =
//        &(GetResources() ->
//          m_puiAnalogueInputDiscreteInputsBadState[GetResources() ->
//                                 m_uiUsedAnalogueInputDiscreteInputsBadState]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedAnalogueInputDiscreteInputsBadState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//
//
//    // Получим указатель на место в массиве отключения аналоговых входов для текущего модуля.
//    m_puiAnalogueInputsOff =
//        &(GetResources() ->
//          m_puiAnalogueInputsOff[GetResources() ->
//                                                m_uiUsedAnalogueInputsOff]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedAnalogueInputsOff +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // Получим указатель на место в массиве распакованной во внутренний формат базы данных
//    // аналоговых сигналов.
//    m_pxAnalogueInputDescriptionWork =
//        &(GetResources() ->
//          m_pxAnalogueInputDescriptionWork[GetResources() ->
//                                                          m_uiUsedAnalogueInputDescriptionWork]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedAnalogueInputDescriptionWork +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // Получим указатель на место в массиве аналоговых входов для текущего модуля.
//    m_puiReperPointsAdcBuffer =
//        &(GetResources() ->
//          m_puiReperPointsAdcBuffer[GetResources() ->
//                                                   m_uiUsedReperPointsAdcBuffer]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedReperPointsAdcBuffer +=
//        ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH;


//    // Получим указатель на место в рабочем массиве дискретных сигналов для текущего модуля.
//    m_pxDiscreteSignalsDescriptionWork =
//        &(GetResources() ->
//          m_pxDiscreteSignalsDescriptionWork[GetResources() ->
//                                 m_uiUsedDiscreteSignalsDescriptionWork]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedDiscreteSignalsDescriptionWork +=
//        ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH;
//
//    m_uiBadAnswerCounter = 0;

//    m_puiModbusReceipt = &((uint8_t)(GetResources() -> m_fbIsAcknowledjedGlobal));
//    m_puiModbusReset = &((uint8_t)(GetResources() -> m_fbIsResetededGlobal));
    m_puiModbusReceipt = &(GetResources() -> m_uiModbusReceipt);
    m_puiModbusReset = &(GetResources() -> m_uiModbusReset);
}

//-----------------------------------------------------------------------------------------------------
void CAlarmDfa::DiscreteOutputsSet(uint8_t *puiLinkedDiscreteOutputs, uint8_t uiNewViolation)
{
    // Дискретный сигнал был активен на предыдущем шаге?
    if (!uiNewViolation)
    {
        // Не новое нарушение.
        // установим флаги - требования включения, для запрограммированных реле,
        // в буфере выходов требований включения реле - блокировка.
        // цикл выбора модуля.
        for (int j = 0;
                j < DISCRETE_OUTPUT_MODULE_MAX_NUMBER;
                j++)
        {
            // цикл выбора реле.
            for (int k = 0;
                    k < MUVR_MR_DISCRETE_OUTPUT_NUMBER;
                    k++)
            {
                // Текущий - (j, k) дискретный выход(реле) привязан?
                if (puiLinkedDiscreteOutputs[j] & (0x01 << k))
                {
                    // установим флаг - требование включения реле.
                    m_pxDiscreteOutputControl[(j * MUVR_MR_DISCRETE_OUTPUT_NUMBER) + k].uiRelayActivationRequest = 1;
                }
            }
        }
    }
    else
    {
        // новое нарушение.
        // установим флаги - новое нарушение, для запрограммированных реле
        // в буфере выходов управления реле - новое нарушение.
        // установим флаги - требования включения, для запрограммированных реле,
        // в буфере выходов требований включения реле - блокировка.
        // цикл выбора модуля.
        for (int j = 0;
                j < DISCRETE_OUTPUT_MODULE_MAX_NUMBER;
                j++)
        {
            // цикл выбора реле.
            for (int k = 0;
                    k < MUVR_MR_DISCRETE_OUTPUT_NUMBER;
                    k++)
            {
                // Текущий - (j, k) дискретный выход(реле) привязан?
                if (puiLinkedDiscreteOutputs[j] & (0x01 << k))
                {
                    // установим флаг - новое нарушение.
                    m_pxDiscreteOutputControl[(j * MUVR_MR_DISCRETE_OUTPUT_NUMBER) + k].uiNewActivation = 1;
                    // установим флаг - требование включения реле.
                    m_pxDiscreteOutputControl[(j * MUVR_MR_DISCRETE_OUTPUT_NUMBER) + k].uiRelayActivationRequest = 1;
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CAlarmDfa::DiscreteSignalStateCheck(void)
{
    uint8_t uiDiscreteSignalState = DISCRETE_SIGNAL_IS_INVALID;

    // дискретный вход недостоверен?
    if (GetDiscreteInputsBadState())
    {
        uiDiscreteSignalState = DISCRETE_SIGNAL_IS_INVALID;
    }
    else
    {
        // Дискретный сигнал активен?
        if (GetDiscreteInputsState())
        {
            if (ACTIVE_LEVEL())
            {
                uiDiscreteSignalState = DISCRETE_SIGNAL_IS_ACTIVE;
            }
            else
            {
                uiDiscreteSignalState = DISCRETE_SIGNAL_IS_NOT_ACTIVE;
            }
        }
        else
        {
            if (ACTIVE_LEVEL())
            {
                uiDiscreteSignalState = DISCRETE_SIGNAL_IS_NOT_ACTIVE;
            }
            else
            {
                uiDiscreteSignalState = DISCRETE_SIGNAL_IS_ACTIVE;
            }
        }
    }

    return uiDiscreteSignalState;
}

//-----------------------------------------------------------------------------------------------------
// Автомат обработки сигнализации дискретного сигнала.
uint8_t CAlarmDfa::Fsm(void)
{
    uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();

    switch (GetFsmState())
    {
    case START:
        break;

//    case ACTIVE_STATE_CHECK:
//        break;

    case ACTIVE_STATE_WAITING:
        // Дискретный сигнал активен?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // Установим связанные дискретный выходы - новое нарушение.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NEW_VIOLATION);
//            // Установим тип сигнализации связанному окну в массиве управления окнами извещателя.
//            CPss21::SetAlarmWindowType(GetAlarmWindowIndex(), ALARM_TYPE());
//            CPss21::SetAlarmWindowColor(GetAlarmWindowIndex(), ALARM_TYPE());
//            CPss21::GetAlarmWindowControlPointer(GetAlarmWindowIndex()) -> SetActivityState(1);

            SetFsmState(RECEIPT_OR_RESET_WAITING);

        }
        break;

    case RECEIPT_OR_RESET_WAITING:
        // Дискретный сигнал активен?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // Установим связанные дискретный выходы - не новое нарушение.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }

        // Событие сброшено?
        if (IsModbusReset())
        {
//            // Установим тип сигнализации связанному окну в массиве управления окнами извещателя.
//            CPss21::SetAlarmWindowType(GetAlarmWindowIndex(), INDICATION);
            SetFsmState(RESETED_NOT_ACTIVE_STATE_WAITING);
        }
        // Событие квитировано?
        else if (IsModbusReceipt())
        {
//            // Установим тип сигнализации связанному окну в массиве управления окнами извещателя.
//            CPss21::SetAlarmWindowType(GetAlarmWindowIndex(), INDICATION);
            SetFsmState(RECEIPTED_RESET_OR_NOT_ACTIVE_STATE_WAITING);
        }
        break;

    case RECEIPTED_RESET_OR_NOT_ACTIVE_STATE_WAITING:
        // Событие сброшено?
        if (IsModbusReset())
        {
            SetFsmState(RESETED_NOT_ACTIVE_STATE_WAITING);
        }
        // Дискретный сигнал не активен?
        else if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_NOT_ACTIVE)
        {
            SetFsmState(RECEIPTED_RESET_WAITING);
        }
        else if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // Установим связанные дискретный выходы - не новое нарушение.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }
        break;

    case RESETED_NOT_ACTIVE_STATE_WAITING:
        // Дискретный сигнал не активен?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_NOT_ACTIVE)
        {
//            // Установим тип сигнализации связанному окну в массиве управления окнами извещателя.
//            CPss21::SetAlarmWindowType(GetAlarmWindowIndex(), NORMAL);
//            CPss21::GetAlarmWindowControlPointer(GetAlarmWindowIndex()) -> SetActivityState(0);
            SetFsmState(ACTIVE_STATE_WAITING);
        }
        else if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // Установим связанные дискретный выходы - не новое нарушение.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }
        break;

    case RECEIPTED_RESET_WAITING:
        // Дискретный сигнал активен?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // Установим связанные дискретный выходы - не новое нарушение.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }

        // Событие сброшено?
        if (IsModbusReset())
        {
//            // Установим тип сигнализации связанному окну в массиве управления окнами извещателя.
//            CPss21::SetAlarmWindowType(GetAlarmWindowIndex(), NORMAL);
//            CPss21::GetAlarmWindowControlPointer(GetAlarmWindowIndex()) -> SetActivityState(0);
            SetFsmState(ACTIVE_STATE_WAITING);
        }
        break;

    default:
        break;
    }
}
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
CNormalAlarmDfa::CNormalAlarmDfa()
{
    SetFsmState(START);
}

//-----------------------------------------------------------------------------------------------------
CNormalAlarmDfa::~CNormalAlarmDfa()
{
    //dtor
}

//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
CPreventiveAlarmLowLevelDfa::CPreventiveAlarmLowLevelDfa()
{
    SetFsmState(ACTIVE_STATE_WAITING);
}

//-----------------------------------------------------------------------------------------------------
CPreventiveAlarmLowLevelDfa::~CPreventiveAlarmLowLevelDfa()
{
    //dtor
}

//-----------------------------------------------------------------------------------------------------
CPreventiveAlarmHighLevelDfa::CPreventiveAlarmHighLevelDfa()
{
    SetFsmState(ACTIVE_STATE_WAITING);
}

//-----------------------------------------------------------------------------------------------------
CPreventiveAlarmHighLevelDfa::~CPreventiveAlarmHighLevelDfa()
{
    //dtor
}
//-----------------------------------------------------------------------------------------------------







//-----------------------------------------------------------------------------------------------------
CEmergencyAlarmLowLevelDfa::CEmergencyAlarmLowLevelDfa()
{
    SetFsmState(ACTIVE_STATE_WAITING);
}

//-----------------------------------------------------------------------------------------------------
CEmergencyAlarmLowLevelDfa::~CEmergencyAlarmLowLevelDfa()
{
    //dtor
}

//-----------------------------------------------------------------------------------------------------
CEmergencyAlarmHighLevelDfa::CEmergencyAlarmHighLevelDfa()
{
    SetFsmState(ACTIVE_STATE_WAITING);
}

//-----------------------------------------------------------------------------------------------------
CEmergencyAlarmHighLevelDfa::~CEmergencyAlarmHighLevelDfa()
{
    //dtor
}
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
CIndicationAlarmLowLevelDfa::CIndicationAlarmLowLevelDfa()
{
    SetFsmState(ACTIVE_STATE_WAITING);
}

//-----------------------------------------------------------------------------------------------------
CIndicationAlarmLowLevelDfa::~CIndicationAlarmLowLevelDfa()
{
    //dtor
}

//-----------------------------------------------------------------------------------------------------
// Автомат обработки сигнализации дискретного сигнала.
uint8_t CIndicationAlarmLowLevelDfa::Fsm(void)
{
    uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();

    switch (GetFsmState())
    {
    case START:
        break;

    case ACTIVE_STATE_WAITING:
        // Дискретный сигнал активен?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // Установим связанные дискретный выходы - новое нарушение.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NEW_VIOLATION);
//            // Установим тип сигнализации связанному окну в массиве управления окнами извещателя.
//            CPss21::SetAlarmWindowType(GetAlarmWindowIndex(), ALARM_TYPE());
//            CPss21::SetAlarmWindowColor(GetAlarmWindowIndex(), ALARM_TYPE());
//            // Активизируем окно сигнализации, для отображения извещателем.
//            CPss21::GetAlarmWindowControlPointer(GetAlarmWindowIndex()) -> SetActivityState(1);
            SetFsmState(NOT_ACTIVE_STATE_WAITING);
        }

        break;

    case NOT_ACTIVE_STATE_WAITING:
        // Дискретный сигнал не активен?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_NOT_ACTIVE)
        {
//                // Установим тип сигнализации связанному окну в массиве управления окнами извещателя.
//                CPss21::SetAlarmWindowType(GetAlarmWindowIndex(), NORMAL);
//                // Деактмвируем окно сигнализации, для прекращения отображения извещателем.
//                CPss21::GetAlarmWindowControlPointer(GetAlarmWindowIndex()) -> SetActivityState(0);
            SetFsmState(ACTIVE_STATE_WAITING);
        }
        else if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // Установим связанные дискретный выходы - не новое нарушение.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }
        break;

    default:
        break;
    }
}
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
CIndicationAlarmHighLevelDfa::CIndicationAlarmHighLevelDfa()
{
    SetFsmState(ACTIVE_STATE_WAITING);
}

//-----------------------------------------------------------------------------------------------------
CIndicationAlarmHighLevelDfa::~CIndicationAlarmHighLevelDfa()
{
    //dtor
}

//-----------------------------------------------------------------------------------------------------
