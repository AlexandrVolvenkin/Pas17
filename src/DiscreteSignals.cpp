//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <iostream>
#include <string.h>
#include <typeinfo>

#include "Task.h"
#include "Resources.h"
#include "Platform.h"
#include "DataStore.h"
#include "DeviceControl.h"
#include "DataContainer.h"
#include "InternalModule.h"
#include "ConfigurationCreate.h"
#include "DataBaseCreate.h"
#include "ConfigurationCheck.h"
#include "ModbusSlave.h"
#include "SettingsLoad.h"
#include "DiscreteSignals.h"

using namespace std;

//-------------------------------------------------------------------------------
CDiscreteSignals::CDiscreteSignals()
{
    std::cout << "CDiscreteSignals constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[256];
    m_vpxAlarmHandlers.clear();
    m_xAlarmHandlersIterator = m_vpxAlarmHandlers.begin();
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CDiscreteSignals::~CDiscreteSignals()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
void CDiscreteSignals::SetInternalModuleName(std::string sName)
{
    m_sInternalModuleName = sName;
}

//-------------------------------------------------------------------------------
void CDiscreteSignals::SetDeviceControlName(std::string sName)
{
    m_sDeviceControlName = sName;
}

//-------------------------------------------------------------------------------
uint8_t CDiscreteSignals::Init(void)
{
    std::cout << "CDiscreteSignals Init"  << std::endl;
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
    SetCustomerDataContainer(GetExecutorDataContainerPointer());

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

    Allocate();
}

//-------------------------------------------------------------------------------
void CDiscreteSignals::Allocate(void)
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


    // ������� ��������� �� ����� � ������� ������� ���������� �������� ��� �������� ������.
    m_pxDiscreteSignalsDescriptionWork =
        &(GetResources() ->
          m_pxDiscreteSignalsDescriptionWork[GetResources() ->
                                 m_uiUsedDiscreteSignalsDescriptionWork]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedDiscreteSignalsDescriptionWork +=
//        ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH;
//
//    m_uiBadAnswerCounter = 0;
}

//-------------------------------------------------------------------------------
// ������ ��������� ���� ������ ��������� ���������� ��������.
void CDiscreteSignals::DiscreteSignalsStartDataBaseCreate(void)
{
//    int i;
    uint8_t uiCheck;
    uint8_t nuiBlockCounter;
    uint8_t nuiBlocksInBlockCounter; // ���� ��������� - ���� ����, � ����� �����.
    uint8_t nuiInputCounter;
    uint8_t nuiModuleCounter;
    uint8_t *puiSource;
    uint8_t uiFlowControl;
    TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDescriptionWork;
    TDiscreteSignalsDescriptionWorkPackOne *pxDiscreteSignalsDescriptionWorkPackOne;
    TDiscreteSignalsDescriptionWorkPackOne xDiscreteSignalsDescriptionWorkSourse;
    // ������� ��������� �� ������� ������ ���������� ��������.
    pxDiscreteSignalsDescriptionWork = m_pxDiscreteSignalsDescriptionWork;

    // ����� � ������� �����.
    nuiInputCounter = 0;
    // ����� � ������� ������.
    nuiModuleCounter = 1;
//    i = 0;

    memset((uint8_t*)pxDiscreteSignalsDescriptionWork,
           0,
           (sizeof(struct TDiscreteSignalsDescriptionWork) *
            MAX_HANDLED_DISCRETE_INPUT));

//    // �������� ������ ����� ��������� ���� ������ ���������� ��������.
//    // ��� �������� ����������� �������� ����������� �����.
//    for (int i = 0;
//            i < ((GetResources() ->
//                  GetDeviceConfigSearchPointer()) ->
//                 uiServiceDiscreteInputModuleQuantity);
//            i++)
//    {
//        for (int j = 0;
//                j < DISCRETE_MODULE_INPUT_QUANTITY;
//                j++)
//        {
//            // �������� ����� ������. ���������� � �������� - nuiModuleCounter. ��� ������� ������ ���� ����� ������.
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= (nuiModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
//            // ��� �������� - ��������� �����������.
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= CONTACT_TYPE_DISC_MASK;
//            // ��������� ������������.
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
//            // ������ ����� ���������� ����, ������� ������ ���� ��1.
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].auiRelayOut[0] = 1;
//            // ����� �������� - 5 ������.
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay |= (DELAY_TIME_SETPOINT_DISC_MASK & 5);
//            // ������������ ������� �������� - 1 �������.
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~DELAY_TIME_DISCRETENESS_DISC_MASK;
//            // �� ������������.
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~ARCHIVE_DISC_MASK;
//
//            // �������� CRC ���������.
//            puiSource = (uint8_t*)&pxDiscreteSignalsDescriptionWork[nuiInputCounter];
//            uiCheck = 0xFF;
//            for (int k = 0;
//                    k < (sizeof(struct TDiscreteSignalsDescriptionWork) - ONE_BYTE_CRC_LENGTH);
//                    k++)
//            {
//                uiCheck += puiSource[k];
//            }
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiCrc = uiCheck;
//
//            // ��������� ���� ������ ����������� �����.
//            // ��������� ���������.
//            nuiInputCounter++;
//
//        }
//
//        // ��������� ������.
//        nuiModuleCounter++;
//    }



    // �������� ������ ����� ��������� ���� ������ ���������� ��������.
    // ��� �������� ����������� �������� ����������� �����.
    // ���� ���������� ���� ��������� ������ ���������� ������� - �������: LL, L, H, HH.
    // ����� � �������� ��������� ��� ������� LL+L.
    uiFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;

    for (int i = 0;
            i < ((GetResources() ->
                  GetDeviceConfigSearchPointer()) ->
                 uiServiceAnalogueInputModuleQuantity);
            i++)
    {
        for (int j = 0;
                j < (ANALOG_MODULE_INPUT_QUANTITY *
                     ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                j++)
        {
            switch(uiFlowControl)
            {
            case ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L:
                // �������� ����� ������. ���������� � �������� - nuiModuleCounter. ��� ������� ������ ���� ����� ������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= (nuiModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // ��� �������� - ��������� �����������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // ��������� ������������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
                // ��� ������ ���������� ����, ������� ������ ���� ��1.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].auiRelayOut[0] = 0;
                // ��� ��������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // ������������ ������� �������� - 1 �������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // �� ������������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~ARCHIVE_DISC_MASK;
                // ������� � ���������� ��������� ��������� �������.
                uiFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                break;

            case ANALOGUE_INPUT_SET_POINT_VIOLATION_L:
                // �������� ����� ������. ���������� � �������� - nuiModuleCounter. ��� ������� ������ ���� ����� ������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= (nuiModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // ��� �������� - ��������� �����������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // ����������������� ������������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp &= ~SIGNALING_TYPE_2_DISC_MASK;
                // ��� ������ ���������� ����, ������� ������ ���� ��1.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].auiRelayOut[0] = 0;
                // ��� ��������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // ������������ ������� �������� - 1 �������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // �� ������������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~ARCHIVE_DISC_MASK;
                // ������� � ���������� ��������� ��������� �������.
                uiFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                break;

            case ANALOGUE_INPUT_SET_POINT_VIOLATION_H:
                // �������� ����� ������. ���������� � �������� - nuiModuleCounter. ��� ������� ������ ���� ����� ������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= (nuiModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // ��� �������� - ��������� �����������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // ����������������� ������������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp &= ~SIGNALING_TYPE_2_DISC_MASK;
                // ��� ������ ���������� ����, ������� ������ ���� ��1.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].auiRelayOut[0] = 0;
                // ��� ��������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // ������������ ������� �������� - 1 �������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // �� ������������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~ARCHIVE_DISC_MASK;
                // ������� � ���������� ��������� ��������� �������.
                uiFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                break;

            case ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H:
                // �������� ����� ������. ���������� � �������� - nuiModuleCounter. ��� ������� ������ ���� ����� ������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= (nuiModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // ��� �������� - ��������� �����������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // ��������� ������������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
                // ��� ������ ���������� ����, ������� ������ ���� ��1.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].auiRelayOut[0] = 0;
                // ��� ��������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // ������������ ������� �������� - 1 �������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // �� ������������.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~ARCHIVE_DISC_MASK;
                // ������� � ���������� ��������� ��������� �������.
                uiFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                break;

            default:
                break;
            };

            // �������� CRC ���������.
            puiSource = (uint8_t*)&pxDiscreteSignalsDescriptionWork[nuiInputCounter];
            uiCheck = 0xFF;
            for (int k = 0;
                    k < (sizeof(struct TDiscreteSignalsDescriptionWork) - ONE_BYTE_CRC_LENGTH);
                    k++)
            {
                uiCheck += puiSource[k];
            }
            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiCrc = uiCheck;

            // ��������� ���� ������ ����������� �����.
            // ��������� ���������.
            nuiInputCounter++;

        }

        // ��������� ������.
        nuiModuleCounter++;
    }
}

//-------------------------------------------------------------------------------
// ����������� ���� ��������� ���� ������ ��������� ���������� �������� �� �������� � ����� ������.
void CDiscreteSignals::DiscreteSignalsStartDataBlockWorkToCommonFormat(uint8_t* puiBlockDataPointer,
        TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDescriptionWork)
{
    TDiscreteSignalsDescriptionWorkPackOne *pxDiscreteSignalsDescriptionWorkPackOne;

    memset(m_puiIntermediateBuff,
           0,
           CDataStore::MAX_BLOCK_LENGTH);
    // ������� ��������� �� ���� ������ ������� � ����� �������.
    pxDiscreteSignalsDescriptionWorkPackOne =
        (TDiscreteSignalsDescriptionWorkPackOne*)m_puiIntermediateBuff;

    // ����������� ���� ��������� ���� ������ � ����� ������.
    for (int i = 0;
            i < DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY;
            i++)
    {
        // ��������� ���� ��������� ����������� �������, � ����� ������ �������.
        pxDiscreteSignalsDescriptionWorkPackOne[i].uiTalTkGrp =
            pxDiscreteSignalsDescriptionWork[i].uiTalTkGrp;

        memcpy(pxDiscreteSignalsDescriptionWorkPackOne[i].auiRelayOut,
               pxDiscreteSignalsDescriptionWork[i].auiRelayOut,
               DISCRETE_OUTPUT_MODULE_MAX_NUMBER);

        pxDiscreteSignalsDescriptionWorkPackOne[i].uiDelay =
            pxDiscreteSignalsDescriptionWork[i].uiDelay;
        pxDiscreteSignalsDescriptionWorkPackOne[i].uiCrc =
            pxDiscreteSignalsDescriptionWork[i].uiCrc;
    }
}


//-----------------------------------------------------------------------------------------------------
// ��������������� �� ������ ������� ���� ������, � ������ �������� � RAM.
// ���� ������ � ������� - ��� ������, ������ 100 ������. ������ ���� 256 ����.
// ����� ��� ������ �������, ���������� ��������, �������������� ������ � ��., ����� ��������� �����.
// ���� ������ �������, ���������� ��������, �������������� ������ � ��., ����� �������� ��������� ������.
// ����� ����� ����������� "�������" ��������� � ���������� ��������, �������� ��������������.
void CDiscreteSignals::DiscreteSignalsDataBlockCommonFormatToWork(void)
{
    std::cout << "CDiscreteSignals::DiscreteSignalsDataBlockCommonFormatToWork 1"  << std::endl;
    unsigned char ucCheck;
    unsigned char *pucSource;
    TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDescriptionWork;
    TDiscreteSignalsDescriptionWorkPackOne *pxDiscreteSignalsDescriptionWorkPackOne;

    // ������� ��������� �� ������� ������ ���������� ��������.
    pxDiscreteSignalsDescriptionWork = m_pxDiscreteSignalsDescriptionWork;

    // ������� ��������� �� ���� ������ ������� � ����� �������.
    pxDiscreteSignalsDescriptionWorkPackOne =
        (TDiscreteSignalsDescriptionWorkPackOne*)m_puiIntermediateBuff;

    // ����������� �� ������ ������� ���� ������, � ������ �������� � RAM.
    for (int i = 0;
            i < DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY;
            i++)
    {
        pxDiscreteSignalsDescriptionWork[i].uiTalTkGrp =
            pxDiscreteSignalsDescriptionWorkPackOne[i].uiTalTkGrp;

        memcpy(pxDiscreteSignalsDescriptionWork[i].auiRelayOut,
               pxDiscreteSignalsDescriptionWorkPackOne[i].auiRelayOut,
               DISCRETE_OUTPUT_MODULE_MAX_NUMBER);

        pxDiscreteSignalsDescriptionWork[i].uiDelay =
            pxDiscreteSignalsDescriptionWorkPackOne[i].uiDelay;

        // �������� ����������� ����� ���������.
        pucSource = (unsigned char*)&pxDiscreteSignalsDescriptionWork[i];
        ucCheck = 0xFF;
        for (int j = 0;
                j < (sizeof(struct TDiscreteSignalsDescriptionWork) - ONE_BYTE_CRC_LENGTH);
                j++)
        {
            ucCheck += pucSource[j];
        }
        pxDiscreteSignalsDescriptionWork[i].uiCrc = ucCheck;
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������� ���������� ����������� ����� ������ ��������� ����������� �������.
// �������� ��������� �� ���� ��������� ����������� ������� � ��������������� ���� ������,
// ��������������� �������� vDiscreteSignalsDataBaseExstract().
uint8_t CDiscreteSignals::DiscreteSignalsDataBaseCrcCheck(
    TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDataBase)
{
    unsigned char ucCheck;
    unsigned char *pucSource;
    // ������� ��������� �� ���� ��������� ����������� ������� � ��������������� ���� ������.
    pucSource = (unsigned char*)pxDiscreteSignalsDataBase;
    // ��������� �������� CRC. ����� ����� �� ���� �������.
    ucCheck = 0xFF;
    // ��������� ����� ��������� ���������� struct TDiscreteSignalsDescriptionWork
    // ����� ����� CRC.
    for (int i = 0;
            i < ((sizeof(struct TDiscreteSignalsDescriptionWork)) - ONE_BYTE_CRC_LENGTH);
            i++)
    {
        ucCheck += pucSource[i];
    }
    // CRC ���������?
    if (pxDiscreteSignalsDataBase -> uiCrc == ucCheck)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

//-------------------------------------------------------------------------------
// ��������� ���������� ������������������� ���������� ��������.
void CDiscreteSignals::ProgrammedDiscreteSignalsNumberCount(void)
{
//    std::cout << "CConfigurationCreate::ProgrammedDiscreteSignalsNumberCount 1" << endl;
    unsigned char ucCheck;
    unsigned char *pucSource;
    TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDescriptionWork;

    uint16_t nucModuleCounter = 0;

    // ������� ��������� �� ������� ������ ���������� ��������.
    pxDiscreteSignalsDescriptionWork = m_pxDiscreteSignalsDescriptionWork;

    // ��������� ���������� �������������������, �������������� ���������� ������.
    for (int i = 0;
            i < MAX_HANDLED_DISCRETE_INPUT;
            i++)
    {
        pucSource = (unsigned char*)&pxDiscreteSignalsDescriptionWork[i];
        ucCheck = 0;
        // ��������� ������ ���������, ����, �� ����� ����, �� ���� ����������������.
        for (int j = 0;
                j < (sizeof(struct TDiscreteSignalsDescriptionWork) - ONE_BYTE_CRC_LENGTH);
                j++)
        {
            ucCheck += pucSource[j];
        }

        if (ucCheck)
        {
            // ��� ���� ���������� ������ ����������������.
            nucModuleCounter++;
        }
        else
        {
            break;
        }
    }

    (GetResources() -> GetDeviceConfigSearchPointer()) ->
    uiHandledDiscreteSignalsQuantity =
        nucModuleCounter;
}

//-------------------------------------------------------------------------------
void CDiscreteSignals::CreateAlarmHandlers(void)
{
    std::cout << "CDiscreteSignals::CreateAlarmHandlers 1"  << std::endl;

    // ������� ��������� �� ������ ������������.
    CConfigurationCreate::TConfigDataPackOne* pxDeviceConfigSearch =
        (GetResources() -> GetDeviceConfigSearchPointer());

    for (uint8_t i = 0;
            i < (pxDeviceConfigSearch ->
                 uiHandledDiscreteSignalsQuantity);
            i++)
    {
//        uint8_t uiType = pxDeviceConfigSearch ->
//                         axModulesContext[i].uiType;
//        switch (uiType)
//        {
//        case MODULE_TYPE_MUVR:
//        {
//            std::string sDeviceName = "InternalModuleMuvr0";
////            std::string sDeviceName = "InternalModuleMuvr" + std::to_string(i);
//            std::cout << "CDiscreteSignals::CreateAlarmHandlers sDeviceName " << sDeviceName << std::endl;
//            CInternalModuleMuvr* pxInternalModuleMuvr = 0;
//            //            pxInternalModuleMuvr =
////                static_cast<CInternalModuleMuvr*>(GetResources() ->
////                                                  AddCommonTaskToMap("InternalModuleMuvr" + std::to_string(i),
////                                                          std::make_shared<CInternalModuleMuvr>()));
//            pxInternalModuleMuvr =
//                static_cast<CInternalModuleMuvr*>(GetResources() ->
//                                                  AddCommonTaskToMap(sDeviceName,
//                                                          std::make_shared<CInternalModuleMuvr>()));
//            pxInternalModuleMuvr ->
//            SetResources(GetResources());
//            pxInternalModuleMuvr ->
//            SetCommunicationDeviceName("SpiCommunicationDeviceSpi0");
////            pxInternalModuleMuvr ->
////            Allocate();
//            pxInternalModuleMuvr ->
//            SetAddress(pxDeviceConfigSearch ->
//                       axModulesContext[i].uiAddress);
//            GetResources() -> AddCurrentlyRunningTasksList(pxInternalModuleMuvr);
//            m_vpxDevices.push_back(pxInternalModuleMuvr);
//            m_vuiDevicesId.push_back(GetResources() ->
//                                     GetTaskIdByNameFromMap(sDeviceName));
    }
//        break;
//
//        default:
//            break;
//        }
//    }
}

//-------------------------------------------------------------------------------
void CDiscreteSignals::AlarmHandlersProcessing(void)
{
    if (m_xAlarmHandlersIterator == m_vpxAlarmHandlers.end())
    {
        m_xAlarmHandlersIterator = m_vpxAlarmHandlers.begin();
    }

    (*m_xAlarmHandlersIterator) -> Fsm();
    m_xAlarmHandlersIterator++;
}

//-------------------------------------------------------------------------------
uint8_t CDiscreteSignals::Fsm(void)
{
//    std::cout << "CDiscreteSignals::Fsm 1" << endl;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CDiscreteSignals::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CDiscreteSignals::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CDiscreteSignals::Fsm START"  << std::endl;
        Init();
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CDiscreteSignals::Fsm INIT 1"  << std::endl;
        m_uiInternalModuleId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleName);

        m_uiDeviceControlId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDeviceControlName);

        m_uiDataStoreId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDataStoreName);

        SetFsmState(READY);

        break;

    case READY:
//        std::cout << "CDiscreteSignals::Fsm READY"  << std::endl;
        break;

    case DONE_OK:
//        std::cout << "CDiscreteSignals::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CDiscreteSignals::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START:
        std::cout << "CDiscreteSignals::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING:
//        std::cout << "CDiscreteSignals::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CDiscreteSignals::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else
        {
            std::cout << "CDiscreteSignals::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 3"  << std::endl;
            // ����� �������� ���������� ������� �����������?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDiscreteSignals::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
        std::cout << "CDiscreteSignals::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        std::cout << "CDiscreteSignals::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CDiscreteSignals::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
            std::cout << "CDiscreteSignals::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // ����� �������� ���������� ������� �����������?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDiscreteSignals::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
        std::cout << "CDiscreteSignals::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
        }

        break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        std::cout << "CDiscreteSignals::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CDiscreteSignals::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CDiscreteSignals::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(GetFsmNextStateDoneWaitingDoneError());
        }
        else
        {
            // ����� �������� ���������� ������� �����������?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDiscreteSignals::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateDoneWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case DISCRETE_SIGNALS_CREATE_DATA_BASE_START:
        std::cout << "CDataBaseCreate::Fsm DISCRETE_SIGNALS_CREATE_DATA_BASE_START"  << std::endl;
        {
            // �������� ��������� ���� ������ ���������� �������� � ������� �������.
            DiscreteSignalsStartDataBaseCreate();
            m_uiBlocksCounter = 0;
            SetFsmState(DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_START);
        }
        break;

    case DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_START:
        std::cout << "CDataBaseCreate::Fsm DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_START"  << std::endl;
        {
            DiscreteSignalsStartDataBlockWorkToCommonFormat(m_puiIntermediateBuff,
                    // ������� ��������� �� ���� � ������ � ����� ��������� ��������������� ��������� ����� ������ ���������� ��������.
                    &(m_pxDiscreteSignalsDescriptionWork[(DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY *
                            m_uiBlocksCounter)]));

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
            // ��������� ��������� ���������� �������� ���� 12
            pxDataContainer -> m_uiDataIndex =
                (DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET +
                 m_uiBlocksCounter);
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            m_uiBlocksCounter++;

            if (m_uiBlocksCounter >= (DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_QUANTITY))
            {
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
                SetFsmState(DONE_OK);
            }
            else
            {
                SetFsmState(DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_START);
            }
        }
        break;

    case DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDiscreteSignals::Fsm DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    default:
        break;
    }
}

//-------------------------------------------------------------------------------

