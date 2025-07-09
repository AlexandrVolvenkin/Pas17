
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
//    // ������� ��������� �� ����� � ������� ���������� ������ ��� �������� �����������.
//    m_puiDiscreteInputsState =
//        &(GetResources() ->
//          m_puiDiscreteInputsState[GetResources() ->
//                                                  m_uiUsedDiscreteInputsState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedDiscreteInputsState += 1;
//
//
//    // ������� ��������� �� ����� � ������� ������������� ���������� ������ ��� �������� �����������.
//    m_puiDiscreteInputsBadState =
//        &(GetResources() ->
//          m_puiDiscreteInputsBadState[GetResources() ->
//                                                     m_uiUsedDiscreteInputsBadState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedDiscreteInputsBadState += 1;


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
        // �����.
        // ������� ����� - ����� ���������, ��� ������������������� ����
        // � ������ ������� ���������� ���� - ����� ���������.
        // ������� ����� - ���������� ���������, ��� ������������������� ����,
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
    //std::cout << "CAlarmDfa::DiscreteOutputsSet 2"  << std::endl;
                    // ��������� ���� - ����� ���������.
                    m_pxDiscreteOutputControl[(j * MUVR_MR_DISCRETE_OUTPUT_NUMBER) + k].uiNewActivation = 0;
                    // ��������� ���� - ���������� ��������� ����.
                    m_pxDiscreteOutputControl[(j * MUVR_MR_DISCRETE_OUTPUT_NUMBER) + k].uiRelayActivationRequest = 0;
                }
            }
        }
    }
    break;

    case NEW_VIOLATION:
    {
        //std::cout << "CAlarmDfa::DiscreteOutputsSet NEW_VIOLATION"  << std::endl;
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
    //std::cout << "CAlarmDfa::DiscreteOutputsSet 3"  << std::endl;
                    // ��������� ���� - ����� ���������.
                    m_pxDiscreteOutputControl[(j * MUVR_MR_DISCRETE_OUTPUT_NUMBER) + k].uiNewActivation = 1;
                    // ��������� ���� - ���������� ��������� ����.
                    m_pxDiscreteOutputControl[(j * MUVR_MR_DISCRETE_OUTPUT_NUMBER) + k].uiRelayActivationRequest = 1;
                }
            }
        }
    }
    break;

    case NOT_NEW_VIOLATION:
    {
        //std::cout << "CAlarmDfa::DiscreteOutputsSet NOT_NEW_VIOLATION"  << std::endl;
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
    //std::cout << "CAlarmDfa::DiscreteOutputsSet 4"  << std::endl;
                    // ��������� ���� - ���������� ��������� ����.
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

    // ���������� ���� ������������?
    if (*GetDiscreteInputsBadState())
    {
//        //std::cout << "CAlarmDfa::DiscreteSignalStateCheck 2 " << m_uiAlarmHandlerIndex << std::endl;
        uiDiscreteSignalState = DISCRETE_SIGNAL_IS_INVALID;
    }
    else
    {
//        //std::cout << "CAlarmDfa::DiscreteSignalStateCheck 3"  << std::endl;
        // ���������� ������ �������?
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
// ������� ��������� ������������ ����������� �������.
uint8_t CAlarmDfa::Fsm(void)
{
//    //std::cout << "CAlarmDfa::Fsm 1"  << std::endl;

    switch (GetFsmState())
    {
    case START:
        Init();
        // ������� ��������� ���������� ������ - �� � �����.
        DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NORMA);
        SetFsmState(ACTIVE_STATE_WAITING);
        break;

//    case ACTIVE_STATE_CHECK:
//        break;

    case ACTIVE_STATE_WAITING:
    {
        uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();
        // ���������� ������ �������?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            //std::cout << "CAlarmDfa::Fsm 2"  << std::endl;
            // ��������� ��������� ���������� ������ - ����� ���������.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NEW_VIOLATION);

            // ���� �������� ��������� ����?
            if (m_uiRelayOnDelay)
            {
                //std::cout << "CAlarmDfa::Fsm 3"  << std::endl;
                m_uiDelay = 0;
                // ��������� ����� �������� 1 �������.
                GetTimerPointer() -> Set(RELAY_ON_DELAY_ONE_SECOND);
                SetFsmState(RELAY_ON_DELAY_END_WAITING);
            }
            else
            {
                //std::cout << "CAlarmDfa::Fsm 4"  << std::endl;
                // ��������� ��������� ���������� ������ - �� ����� ���������.
                DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
                SetFsmState(RECEIPT_OR_RESET_WAITING);
            }
        }
    }
    break;

    case RELAY_ON_DELAY_END_WAITING:
    {
        uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();
        // ���������� ������ �������?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
                //std::cout << "CAlarmDfa::Fsm 5"  << std::endl;
            // ����� �������� 1 ������� �����������?
            if (GetTimerPointer() -> IsOverflow())
            {
                //std::cout << "CAlarmDfa::Fsm 6"  << std::endl;
                // �������� �����������?
                if (m_uiDelay < m_uiRelayOnDelay)
                {
                //std::cout << "CAlarmDfa::Fsm 7"  << std::endl;
                    m_uiDelay++;
                    // ��������� ����� �������� 1 �������.
                    GetTimerPointer() -> Set(RELAY_ON_DELAY_ONE_SECOND);
                }
                else
                {
                //std::cout << "CAlarmDfa::Fsm 8"  << std::endl;
                    // ��������� ��������� ���������� ������ - �� ����� ���������.
                    DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
                    SetFsmState(RECEIPT_OR_RESET_WAITING);
                }
            }
        }
        else
        {
                //std::cout << "CAlarmDfa::Fsm 9"  << std::endl;
            // ������� ��������� ���������� ������ - �� � �����.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NORMA);
            SetFsmState(ACTIVE_STATE_WAITING);
        }
    }
    break;

    case RECEIPT_OR_RESET_WAITING:
    {
        uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();
        // ���������� ������ �������?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            //std::cout << "CAlarmDfa::Fsm 101"  << std::endl;
            // ��������� ��������� ���������� ������ - �� ����� ���������.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }

        // ������� ��������?
        if (IsModbusReset())
        {
            //std::cout << "CAlarmDfa::Fsm 10"  << std::endl;
            SetFsmState(RESETED_NOT_ACTIVE_STATE_WAITING);
        }
        // ������� �����������?
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
        // ������� ��������?
        if (IsModbusReset())
        {
            //std::cout << "CAlarmDfa::Fsm 12"  << std::endl;
            SetFsmState(RESETED_NOT_ACTIVE_STATE_WAITING);
        }
        // ���������� ������ �� �������?
        else if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_NOT_ACTIVE)
        {
            //std::cout << "CAlarmDfa::Fsm 13"  << std::endl;
            SetFsmState(RECEIPTED_RESET_WAITING);
        }
        else if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            //std::cout << "CAlarmDfa::Fsm 14"  << std::endl;
            // ��������� ��������� ���������� ������ - �� ����� ���������.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }
    }
    break;

    case RESETED_NOT_ACTIVE_STATE_WAITING:
    {
        uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();
        // ���������� ������ �� �������?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_NOT_ACTIVE)
        {
            //std::cout << "CAlarmDfa::Fsm 15"  << std::endl;
            // ������� ��������� ���������� ������ - �� � �����.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NORMA);
            SetFsmState(ACTIVE_STATE_WAITING);
        }
        else if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            //std::cout << "CAlarmDfa::Fsm 16"  << std::endl;
            // ��������� ��������� ���������� ������ - �� ����� ���������.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }
    }
    break;

    case RECEIPTED_RESET_WAITING:
    {
        uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();
        // ���������� ������ �������?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // ��������� ��������� ���������� ������ - �� ����� ���������.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
        }

        // ������� ��������?
        if (IsModbusReset())
        {
            // ������� ��������� ���������� ������ - �� � �����.
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
// ������� ��������� ������������ ����������� �������.
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
        // ���������� ������ �������?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // ��������� ��������� ���������� ������ - ����� ���������.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NEW_VIOLATION);

            // ���� �������� ��������� ����?
            if (m_uiRelayOnDelay)
            {
                m_uiDelay = 0;
                // ��������� ����� �������� 1 �������.
                GetTimerPointer() -> Set(RELAY_ON_DELAY_ONE_SECOND);
                SetFsmState(RELAY_ON_DELAY_END_WAITING);
            }
            else
            {
                // ��������� ��������� ���������� ������ - �� ����� ���������.
                DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
                SetFsmState(NOT_ACTIVE_STATE_WAITING);
            }
        }
    }
    break;

    case RELAY_ON_DELAY_END_WAITING:
    {
        uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();
        // ���������� ������ �������?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // ����� �������� 1 ������� �����������?
            if (GetTimerPointer() -> IsOverflow())
            {
                // �������� �����������?
                if (m_uiDelay < m_uiRelayOnDelay)
                {
                    m_uiDelay++;
                    // ��������� ����� �������� 1 �������.
                    GetTimerPointer() -> Set(RELAY_ON_DELAY_ONE_SECOND);
                }
                else
                {
                    // ��������� ��������� ���������� ������ - �� ����� ���������.
                    DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NOT_NEW_VIOLATION);
                    SetFsmState(NOT_ACTIVE_STATE_WAITING);
                }
            }
        }
        else
        {
            // ������� ��������� ���������� ������ - �� � �����.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NORMA);
            SetFsmState(ACTIVE_STATE_WAITING);
        }
    }
    break;

    case NOT_ACTIVE_STATE_WAITING:
    {
        uint8_t uiDiscreteSignalState = DiscreteSignalStateCheck();
        // ���������� ������ �� �������?
        if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_NOT_ACTIVE)
        {
            // ������� ��������� ���������� ������ - �� � �����.
            DiscreteOutputsSet(GetLinkedDiscreteOutputsPointer(), NORMA);
            SetFsmState(ACTIVE_STATE_WAITING);
        }
        else if (uiDiscreteSignalState == DISCRETE_SIGNAL_IS_ACTIVE)
        {
            // ��������� ��������� ���������� ������ - �� ����� ���������.
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
