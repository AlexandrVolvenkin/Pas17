
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
#include "Events.h"
#include "EventsDB.h"
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
    //std::cout << "CAlarmDfa Init"  << std::endl;
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
    SetCustomerDataContainer(GetExecutorDataContainerPointer());

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

    Allocate();
}

//-------------------------------------------------------------------------------
void CAlarmDfa::Allocate(void)
{
    //std::cout << "CAlarmDfa::Allocate 1"  << std::endl;

////    m_uiAddress = xMemoryAllocationContext.uiAddress;
////    m_puiRxBuffer = xMemoryAllocationContext.puiRxBuffer;
////    m_puiTxBuffer = xMemoryAllocationContext.puiTxBuffer;
////    m_puiErrorCode = xMemoryAllocationContext.puiErrorCode;
//
//    // Получим указатель на место в массиве дискретных входов для текущего обработчика.
//    m_puiDiscreteInputsState =
//        &(GetResources() ->
//          m_puiDiscreteInputsState[GetResources() ->
//                                                  m_uiUsedDiscreteInputsState]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedDiscreteInputsState += 1;
//
//
//    // Получим указатель на место в массиве достоверности дискретных входов для текущего обработчика.
//    m_puiDiscreteInputsBadState =
//        &(GetResources() ->
//          m_puiDiscreteInputsBadState[GetResources() ->
//                                                     m_uiUsedDiscreteInputsBadState]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedDiscreteInputsBadState += 1;


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

//    m_uiBadAnswerCounter = 0;

    m_puiModbusReceipt = &(GetResources() -> m_uiModbusReceipt);
    m_puiModbusReset = &(GetResources() -> m_uiModbusReset);
}

//-----------------------------------------------------------------------------------------------------
void CAlarmDfa::DiscreteOutputsSet(uint8_t *puiLinkedDiscreteOutputs, uint8_t uiNewViolation)
{
    //std::cout << "CAlarmDfa::DiscreteOutputsSet 1"  << std::endl;
    switch (uiNewViolation)
    {
    case NORMA:
    {
        //std::cout << "CAlarmDfa::DiscreteOutputsSet NORMA"  << std::endl;
        // норма.
        // сбросим флаги - новое нарушение, для запрограммированных реле
        // в буфере выходов управления реле - новое нарушение.
        // сбросим флаги - требования включения, для запрограммированных реле,
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
    //std::cout << "CAlarmDfa::DiscreteOutputsSet 2"  << std::endl;
                    // установим флаг - новое нарушение.
                    m_pxDiscreteOutputControl[(j * MUVR_MR_DISCRETE_OUTPUT_NUMBER) + k].uiNewActivation = 0;
                    // установим флаг - требование включения реле.
                    m_pxDiscreteOutputControl[(j * MUVR_MR_DISCRETE_OUTPUT_NUMBER) + k].uiRelayActivationRequest = 0;
                }
            }
        }
    }
    break;

    case NEW_VIOLATION:
    {
        //std::cout << "CAlarmDfa::DiscreteOutputsSet NEW_VIOLATION"  << std::endl;
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
    //std::cout << "CAlarmDfa::DiscreteOutputsSet 3"  << std::endl;
                    // установим флаг - новое нарушение.
                    m_pxDiscreteOutputControl[(j * MUVR_MR_DISCRETE_OUTPUT_NUMBER) + k].uiNewActivation = 1;
                    // установим флаг - требование включения реле.
                    m_pxDiscreteOutputControl[(j * MUVR_MR_DISCRETE_OUTPUT_NUMBER) + k].uiRelayActivationRequest = 1;
                }
            }
        }
    }
    break;

    case NOT_NEW_VIOLATION:
    {
        //std::cout << "CAlarmDfa::DiscreteOutputsSet NOT_NEW_VIOLATION"  << std::endl;
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
    //std::cout << "CAlarmDfa::DiscreteOutputsSet 4"  << std::endl;
                    // установим флаг - требование включения реле.
                    m_pxDiscreteOutputControl[(j * MUVR_MR_DISCRETE_OUTPUT_NUMBER) + k].uiRelayActivationRequest = 1;
                }
            }
        }
    }
    break;

    default:
        break;
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CAlarmDfa::DiscreteSignalStateCheck(void)
{
//    //std::cout << "CAlarmDfa::DiscreteSignalStateCheck 1"  << std::endl;
    uint8_t uiDiscreteSignalState = DISCRETE_SIGNAL_IS_INVALID;

    // дискретный вход недостоверен?
    if (*GetDiscreteInputsBadState())
    {
//        //std::cout << "CAlarmDfa::DiscreteSignalStateCheck 2 " << m_uiAlarmHandlerIndex << std::endl;
        uiDiscreteSignalState = DISCRETE_SIGNAL_IS_INVALID;
    }
    else
    {
//        //std::cout << "CAlarmDfa::DiscreteSignalStateCheck 3"  << std::endl;
        // Дискретный сигнал активен?
        if (*GetDiscreteInputsState())
        {
            //std::cout << "CAlarmDfa::DiscreteSignalStateCheck 4 " << m_uiAlarmHandlerIndex << std::endl;
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
//            //std::cout << "CAlarmDfa::DiscreteSignalStateCheck 5 " << m_uiAlarmHandlerIndex << std::endl;
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
//    //std::cout << "CAlarmDfa::Fsm 1"  << std::endl;

    switch (GetFsmState())
    {
    case START:
        Init();
        // сбросим связанные дискретный выходы - всё в норму.
        DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NORMA);
        SetFsmState(ACTIVE_STATE_WAITING);
        break;

//    case ACTIVE_STATE_CHECK:
//        break;

    case ACTIVE_STATE_WAITING:
    {
        uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();
        // Дискретный сигнал активен?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            //std::cout << "CAlarmDfa::Fsm 2"  << std::endl;
            // Установим связанные дискретный выходы - новое нарушение.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NEW_VIOLATION);

            // есть задержка включения реле?
            if (m_uiRelayOnDelay)
            {
                //std::cout << "CAlarmDfa::Fsm 3"  << std::endl;
                m_uiDelay = 0;
                // Установим время ожидания 1 секунда.
                GetTimerPointer() -> Set(RELAY_ON_DELAY_ONE_SECOND);
                SetFsmState(RELAY_ON_DELAY_END_WAITING);
            }
            else
            {
                //std::cout << "CAlarmDfa::Fsm 4"  << std::endl;
                // Установим связанные дискретный выходы - не новое нарушение.
                DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
                SetFsmState(RECEIPT_OR_RESET_WAITING);
            }
        }
    }
    break;

    case RELAY_ON_DELAY_END_WAITING:
    {
        uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();
        // Дискретный сигнал активен?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
                //std::cout << "CAlarmDfa::Fsm 5"  << std::endl;
            // Время ожидания 1 секунды закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                //std::cout << "CAlarmDfa::Fsm 6"  << std::endl;
                // задержка закончилась?
                if (m_uiDelay < m_uiRelayOnDelay)
                {
                //std::cout << "CAlarmDfa::Fsm 7"  << std::endl;
                    m_uiDelay++;
                    // Установим время ожидания 1 секунда.
                    GetTimerPointer() -> Set(RELAY_ON_DELAY_ONE_SECOND);
                }
                else
                {
                //std::cout << "CAlarmDfa::Fsm 8"  << std::endl;
                    // Установим связанные дискретный выходы - не новое нарушение.
                    DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
                    SetFsmState(RECEIPT_OR_RESET_WAITING);
                }
            }
        }
        else
        {
                //std::cout << "CAlarmDfa::Fsm 9"  << std::endl;
            // сбросим связанные дискретный выходы - всё в норму.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NORMA);
            SetFsmState(ACTIVE_STATE_WAITING);
        }
    }
    break;

    case RECEIPT_OR_RESET_WAITING:
    {
        uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();
        // Дискретный сигнал активен?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            //std::cout << "CAlarmDfa::Fsm 101"  << std::endl;
            // Установим связанные дискретный выходы - не новое нарушение.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }

        // Событие сброшено?
        if (IsModbusReset())
        {
            //std::cout << "CAlarmDfa::Fsm 10"  << std::endl;
            SetFsmState(RESETED_NOT_ACTIVE_STATE_WAITING);
        }
        // Событие квитировано?
        else if (IsModbusReceipt())
        {
            //std::cout << "CAlarmDfa::Fsm 11"  << std::endl;
            SetFsmState(RECEIPTED_RESET_OR_NOT_ACTIVE_STATE_WAITING);
        }
    }
    break;

    case RECEIPTED_RESET_OR_NOT_ACTIVE_STATE_WAITING:
    {
        uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();
        // Событие сброшено?
        if (IsModbusReset())
        {
            //std::cout << "CAlarmDfa::Fsm 12"  << std::endl;
            SetFsmState(RESETED_NOT_ACTIVE_STATE_WAITING);
        }
        // Дискретный сигнал не активен?
        else if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_NOT_ACTIVE)
        {
            //std::cout << "CAlarmDfa::Fsm 13"  << std::endl;
            SetFsmState(RECEIPTED_RESET_WAITING);
        }
        else if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            //std::cout << "CAlarmDfa::Fsm 14"  << std::endl;
            // Установим связанные дискретный выходы - не новое нарушение.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }
    }
    break;

    case RESETED_NOT_ACTIVE_STATE_WAITING:
    {
        uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();
        // Дискретный сигнал не активен?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_NOT_ACTIVE)
        {
            //std::cout << "CAlarmDfa::Fsm 15"  << std::endl;
            // сбросим связанные дискретный выходы - всё в норму.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NORMA);
            SetFsmState(ACTIVE_STATE_WAITING);
        }
        else if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            //std::cout << "CAlarmDfa::Fsm 16"  << std::endl;
            // Установим связанные дискретный выходы - не новое нарушение.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }
    }
    break;

    case RECEIPTED_RESET_WAITING:
    {
        uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();
        // Дискретный сигнал активен?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // Установим связанные дискретный выходы - не новое нарушение.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }

        // Событие сброшено?
        if (IsModbusReset())
        {
            // сбросим связанные дискретный выходы - всё в норму.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NORMA);
            SetFsmState(ACTIVE_STATE_WAITING);
        }
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
    SetFsmState(START);
}

//-----------------------------------------------------------------------------------------------------
CPreventiveAlarmLowLevelDfa::~CPreventiveAlarmLowLevelDfa()
{
    //dtor
}

//-----------------------------------------------------------------------------------------------------
CPreventiveAlarmHighLevelDfa::CPreventiveAlarmHighLevelDfa()
{
    SetFsmState(START);
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
    SetFsmState(START);
}

//-----------------------------------------------------------------------------------------------------
CEmergencyAlarmLowLevelDfa::~CEmergencyAlarmLowLevelDfa()
{
    //dtor
}

//-----------------------------------------------------------------------------------------------------
CEmergencyAlarmHighLevelDfa::CEmergencyAlarmHighLevelDfa()
{
    SetFsmState(START);
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
    SetFsmState(START);
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
    std::cout << "CIndicationAlarmLowLevelDfa::Fsm 1"  << std::endl;
    uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();

    switch (GetFsmState())
    {
    case START:
        Init();
        SetFsmState(ACTIVE_STATE_WAITING);
        break;

    case ACTIVE_STATE_WAITING:
    {
        uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();
        // Дискретный сигнал активен?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // Установим связанные дискретный выходы - новое нарушение.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NEW_VIOLATION);

            // есть задержка включения реле?
            if (m_uiRelayOnDelay)
            {
                m_uiDelay = 0;
                // Установим время ожидания 1 секунда.
                GetTimerPointer() -> Set(RELAY_ON_DELAY_ONE_SECOND);
                SetFsmState(RELAY_ON_DELAY_END_WAITING);
            }
            else
            {
                // Установим связанные дискретный выходы - не новое нарушение.
                DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
                SetFsmState(NOT_ACTIVE_STATE_WAITING);
            }
        }
    }
    break;

    case RELAY_ON_DELAY_END_WAITING:
    {
        uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();
        // Дискретный сигнал активен?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // Время ожидания 1 секунды закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                // задержка закончилась?
                if (m_uiDelay < m_uiRelayOnDelay)
                {
                    m_uiDelay++;
                    // Установим время ожидания 1 секунда.
                    GetTimerPointer() -> Set(RELAY_ON_DELAY_ONE_SECOND);
                }
                else
                {
                    // Установим связанные дискретный выходы - не новое нарушение.
                    DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
                    SetFsmState(NOT_ACTIVE_STATE_WAITING);
                }
            }
        }
        else
        {
            // сбросим связанные дискретный выходы - всё в норму.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NORMA);
            SetFsmState(ACTIVE_STATE_WAITING);
        }
    }
    break;

    case NOT_ACTIVE_STATE_WAITING:
    {
        uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();
        // Дискретный сигнал не активен?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_NOT_ACTIVE)
        {
            // сбросим связанные дискретный выходы - всё в норму.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NORMA);
            SetFsmState(ACTIVE_STATE_WAITING);
        }
        else if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // Установим связанные дискретный выходы - не новое нарушение.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }
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
    SetFsmState(START);
}

//-----------------------------------------------------------------------------------------------------
CIndicationAlarmHighLevelDfa::~CIndicationAlarmHighLevelDfa()
{
    //dtor
}

//-----------------------------------------------------------------------------------------------------
