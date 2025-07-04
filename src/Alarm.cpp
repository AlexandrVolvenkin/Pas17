
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
//    // ������� ��������� �� ����� � ������� ���������� ������ ��� �������� ������.
    //    m_puiDiscreteInputsState =
//        &(GetResources() ->
//          m_puiDiscreteInputsState[GetResources() ->
//                                                  m_uiUsedDiscreteInputsState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedDiscreteInputsState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ������������� ���������� ������ ��� �������� ������.
//    m_puiDiscreteInputsBadState =
//        &(GetResources() ->
//          m_puiDiscreteInputsBadState[GetResources() ->
//                                                     m_uiUsedDiscreteInputsBadState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedDiscreteInputsBadState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;


    // ��������� ����� ��� ���������� ����������� ��������.
    m_pxDiscreteOutputControl = (GetResources() -> m_pxDiscreteOutputControl);
//
//
//    // ������� ��������� �� ����� � ������� ���������� ������ ��� �������� ������.
//    m_pfAnalogueInputsValue =
//        &(GetResources() ->
//          m_pfAnalogueInputsValue[GetResources() ->
//                                                 m_uiUsedAnalogueInputsValue]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputsValue +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ��������� ���������� ������ ��� �������� ������.
//    m_puiAnalogueInputsState =
//        &(GetResources() ->
//          m_puiAnalogueInputsState[GetResources() ->
//                                                  m_uiUsedAnalogueInputsState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputsBadState +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ������������� ���������� ������ ��� �������� ������.
//    m_puiAnalogueInputsBadState =
//        &(GetResources() ->
//          m_puiAnalogueInputsBadState[GetResources() ->
//                                                     m_uiUsedAnalogueInputsBadState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputsBadState +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ��������� ���������� �������� ����������� ����������� �������.
//    m_puiAnalogueInputDiscreteInputsState =
//        &(GetResources() ->
//          m_puiAnalogueInputDiscreteInputsState[GetResources() ->
//                                 m_uiUsedAnalogueInputDiscreteInputsState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputDiscreteInputsState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ������ ��������������� ��������� ���������� �������� ����������� ����������� �������.
//    m_puiAnalogueInputDiscreteInputsBadState =
//        &(GetResources() ->
//          m_puiAnalogueInputDiscreteInputsBadState[GetResources() ->
//                                 m_uiUsedAnalogueInputDiscreteInputsBadState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputDiscreteInputsBadState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ���������� ���������� ������ ��� �������� ������.
//    m_puiAnalogueInputsOff =
//        &(GetResources() ->
//          m_puiAnalogueInputsOff[GetResources() ->
//                                                m_uiUsedAnalogueInputsOff]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputsOff +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ������������� �� ���������� ������ ���� ������
//    // ���������� ��������.
//    m_pxAnalogueInputDescriptionWork =
//        &(GetResources() ->
//          m_pxAnalogueInputDescriptionWork[GetResources() ->
//                                                          m_uiUsedAnalogueInputDescriptionWork]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputDescriptionWork +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ���������� ������ ��� �������� ������.
//    m_puiReperPointsAdcBuffer =
//        &(GetResources() ->
//          m_puiReperPointsAdcBuffer[GetResources() ->
//                                                   m_uiUsedReperPointsAdcBuffer]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedReperPointsAdcBuffer +=
//        ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH;


//    // ������� ��������� �� ����� � ������� ������� ���������� �������� ��� �������� ������.
//    m_pxDiscreteSignalsDescriptionWork =
//        &(GetResources() ->
//          m_pxDiscreteSignalsDescriptionWork[GetResources() ->
//                                 m_uiUsedDiscreteSignalsDescriptionWork]);
//    // �������� ����� ����� ���������� ������.
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
    // ���������� ������ ��� ������� �� ���������� ����?
    if (!uiNewViolation)
    {
        // �� ����� ���������.
        // ��������� ����� - ���������� ���������, ��� ������������������� ����,
        // � ������ ������� ���������� ��������� ���� - ����������.
        // ���� ������ ������.
        for (int j = 0;
                j < DISCRETE_OUTPUT_MODULE_MAX_NUMBER;
                j++)
        {
            // ���� ������ ����.
            for (int k = 0;
                    k < MUVR_MR_DISCRETE_OUTPUT_NUMBER;
                    k++)
            {
                // ������� - (j, k) ���������� �����(����) ��������?
                if (puiLinkedDiscreteOutputs[j] & (0x01 << k))
                {
                    // ��������� ���� - ���������� ��������� ����.
                    m_pxDiscreteOutputControl[(j * MUVR_MR_DISCRETE_OUTPUT_NUMBER) + k].uiRelayActivationRequest = 1;
                }
            }
        }
    }
    else
    {
        // ����� ���������.
        // ��������� ����� - ����� ���������, ��� ������������������� ����
        // � ������ ������� ���������� ���� - ����� ���������.
        // ��������� ����� - ���������� ���������, ��� ������������������� ����,
        // � ������ ������� ���������� ��������� ���� - ����������.
        // ���� ������ ������.
        for (int j = 0;
                j < DISCRETE_OUTPUT_MODULE_MAX_NUMBER;
                j++)
        {
            // ���� ������ ����.
            for (int k = 0;
                    k < MUVR_MR_DISCRETE_OUTPUT_NUMBER;
                    k++)
            {
                // ������� - (j, k) ���������� �����(����) ��������?
                if (puiLinkedDiscreteOutputs[j] & (0x01 << k))
                {
                    // ��������� ���� - ����� ���������.
                    m_pxDiscreteOutputControl[(j * MUVR_MR_DISCRETE_OUTPUT_NUMBER) + k].uiNewActivation = 1;
                    // ��������� ���� - ���������� ��������� ����.
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

    // ���������� ���� ������������?
    if (GetDiscreteInputsBadState())
    {
        uiDiscreteSignalState = DISCRETE_SIGNAL_IS_INVALID;
    }
    else
    {
        // ���������� ������ �������?
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
// ������� ��������� ������������ ����������� �������.
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
        // ���������� ������ �������?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // ��������� ��������� ���������� ������ - ����� ���������.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NEW_VIOLATION);
//            // ��������� ��� ������������ ���������� ���� � ������� ���������� ������ ����������.
//            CPss21::SetAlarmWindowType(GetAlarmWindowIndex(), ALARM_TYPE());
//            CPss21::SetAlarmWindowColor(GetAlarmWindowIndex(), ALARM_TYPE());
//            CPss21::GetAlarmWindowControlPointer(GetAlarmWindowIndex()) -> SetActivityState(1);

            SetFsmState(RECEIPT_OR_RESET_WAITING);

        }
        break;

    case RECEIPT_OR_RESET_WAITING:
        // ���������� ������ �������?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // ��������� ��������� ���������� ������ - �� ����� ���������.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }

        // ������� ��������?
        if (IsModbusReset())
        {
//            // ��������� ��� ������������ ���������� ���� � ������� ���������� ������ ����������.
//            CPss21::SetAlarmWindowType(GetAlarmWindowIndex(), INDICATION);
            SetFsmState(RESETED_NOT_ACTIVE_STATE_WAITING);
        }
        // ������� �����������?
        else if (IsModbusReceipt())
        {
//            // ��������� ��� ������������ ���������� ���� � ������� ���������� ������ ����������.
//            CPss21::SetAlarmWindowType(GetAlarmWindowIndex(), INDICATION);
            SetFsmState(RECEIPTED_RESET_OR_NOT_ACTIVE_STATE_WAITING);
        }
        break;

    case RECEIPTED_RESET_OR_NOT_ACTIVE_STATE_WAITING:
        // ������� ��������?
        if (IsModbusReset())
        {
            SetFsmState(RESETED_NOT_ACTIVE_STATE_WAITING);
        }
        // ���������� ������ �� �������?
        else if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_NOT_ACTIVE)
        {
            SetFsmState(RECEIPTED_RESET_WAITING);
        }
        else if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // ��������� ��������� ���������� ������ - �� ����� ���������.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }
        break;

    case RESETED_NOT_ACTIVE_STATE_WAITING:
        // ���������� ������ �� �������?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_NOT_ACTIVE)
        {
//            // ��������� ��� ������������ ���������� ���� � ������� ���������� ������ ����������.
//            CPss21::SetAlarmWindowType(GetAlarmWindowIndex(), NORMAL);
//            CPss21::GetAlarmWindowControlPointer(GetAlarmWindowIndex()) -> SetActivityState(0);
            SetFsmState(ACTIVE_STATE_WAITING);
        }
        else if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // ��������� ��������� ���������� ������ - �� ����� ���������.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }
        break;

    case RECEIPTED_RESET_WAITING:
        // ���������� ������ �������?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // ��������� ��������� ���������� ������ - �� ����� ���������.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }

        // ������� ��������?
        if (IsModbusReset())
        {
//            // ��������� ��� ������������ ���������� ���� � ������� ���������� ������ ����������.
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
// ������� ��������� ������������ ����������� �������.
uint8_t CIndicationAlarmLowLevelDfa::Fsm(void)
{
    uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();

    switch (GetFsmState())
    {
    case START:
        break;

    case ACTIVE_STATE_WAITING:
        // ���������� ������ �������?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // ��������� ��������� ���������� ������ - ����� ���������.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NEW_VIOLATION);
//            // ��������� ��� ������������ ���������� ���� � ������� ���������� ������ ����������.
//            CPss21::SetAlarmWindowType(GetAlarmWindowIndex(), ALARM_TYPE());
//            CPss21::SetAlarmWindowColor(GetAlarmWindowIndex(), ALARM_TYPE());
//            // ������������ ���� ������������, ��� ����������� �����������.
//            CPss21::GetAlarmWindowControlPointer(GetAlarmWindowIndex()) -> SetActivityState(1);
            SetFsmState(NOT_ACTIVE_STATE_WAITING);
        }

        break;

    case NOT_ACTIVE_STATE_WAITING:
        // ���������� ������ �� �������?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_NOT_ACTIVE)
        {
//                // ��������� ��� ������������ ���������� ���� � ������� ���������� ������ ����������.
//                CPss21::SetAlarmWindowType(GetAlarmWindowIndex(), NORMAL);
//                // ������������ ���� ������������, ��� ����������� ����������� �����������.
//                CPss21::GetAlarmWindowControlPointer(GetAlarmWindowIndex()) -> SetActivityState(0);
            SetFsmState(ACTIVE_STATE_WAITING);
        }
        else if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // ��������� ��������� ���������� ������ - �� ����� ���������.
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
