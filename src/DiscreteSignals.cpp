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




//    // ������� ��������� �� ����� � ����� ��������� ��������������� ��������� ����� ������ ���������� ��������.
//    pxDiscreteSignalsDescriptionWork = m_pxDiscreteSignalsDescriptionWork;
////    // ������� ��������� �� ���� ������ ������� � ����� �������.
////    pxDiscreteSignalsDescriptionWorkPackOne = (TDiscreteSignalsDescriptionWorkPackOne*)&(xPlcDataBase.
////            axPlcDataBaseBlocks[DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET].
////            auiPlcDataBaseBlockData[0]);
//    nuiBlockCounter = 0;
//    nuiBlocksInBlockCounter = 0;
//
//    // ����������� ��������� ���� ������ � ����� ������.
//    for (int i = 0;
//            i < MAX_HANDLED_DISCRETE_INPUT;
//            i++)
//    {
//        // ��������� ���� ���� ���� ������?
//        if (nuiBlocksInBlockCounter == DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
//        {
//            // ��������� ���� ���� ������.
//            nuiBlockCounter++;
////            // ������� ��������� �� ��������� ���� � ����� ���� ������ �������.
////            pxDiscreteSignalsDescriptionWorkPackOne = (TDiscreteSignalsDescriptionWorkPackOne*)&(xPlcDataBase.
////                    axPlcDataBaseBlocks[DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + nuiBlockCounter].
////                    auiPlcDataBaseBlockData[0]);
//            nuiBlocksInBlockCounter = 0;
//        }
//
//        // ��������� ���� ��������� ����������� �������, � ����� ������ �������.
//        xDiscreteSignalsDescriptionWorkSourse.uiTalTkGrp =
//            pxDiscreteSignalsDescriptionWork[i].uiTalTkGrp;
//
//        memcpy(xDiscreteSignalsDescriptionWorkSourse.auiRelayOut,
//               pxDiscreteSignalsDescriptionWork[i].auiRelayOut,
//               8);
//
//        xDiscreteSignalsDescriptionWorkSourse.uiDelay =
//            pxDiscreteSignalsDescriptionWork[i].uiDelay;
//        xDiscreteSignalsDescriptionWorkSourse.uiCrc =
//            pxDiscreteSignalsDescriptionWork[i].uiCrc;
//
//        memcpy((uint8_t*)&pxDiscreteSignalsDescriptionWorkPackOne[nuiBlocksInBlockCounter],
//               (uint8_t*)&xDiscreteSignalsDescriptionWorkSourse,
//               sizeof(struct TDiscreteSignalsDescriptionWorkPackOne));
//
//        // ��������� ���������.
//        nuiBlocksInBlockCounter++;
//    }
}

//-------------------------------------------------------------------------------
// ����������� ���� ��������� ���� ������ ��������� ���������� �������� �� �������� � ����� ������.
void CDiscreteSignals::DiscreteSignalsStartDataBlockWorkToCommonFormat(uint8_t* puiBlockDataPointer,
        TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDescriptionWork)
{
//    TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDescriptionWork;
    TDiscreteSignalsDescriptionWorkPackOne *pxDiscreteSignalsDescriptionWorkPackOne;
//    // ������� ��������� �� ���� � ������ � ����� ��������� ��������������� ��������� ����� ������ ���������� ��������.
//    pxDiscreteSignalsDescriptionWork =
//        m_pxDiscreteSignalsDescriptionWork[((sizeof(struct TDiscreteSignalsDescriptionWork) *
//    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY) *
//    uiBlockIndex)];
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
               5);

        pxDiscreteSignalsDescriptionWorkPackOne[i].uiDelay =
            pxDiscreteSignalsDescriptionWork[i].uiDelay;
        pxDiscreteSignalsDescriptionWorkPackOne[i].uiCrc =
            pxDiscreteSignalsDescriptionWork[i].uiCrc;
    }
}

////-------------------------------------------------------------------------------
//// ������ ��������� ���� ������ ��������� ���������� ���������� ��������.
//void CDiscreteSignals::DiscreteSygnalsTextDescriptorStartDataBaseCreate(uint8_t* puiBlockDataPointer)
//{
//    int i;
//    uint8_t nucBlockCounter;
//    uint8_t nucBlocksInBlockCounter; // ���� ��������� - ���� ����, � ����� �����.
//    uint8_t nucDiscreteSignalsCounter;
//    uint8_t nucModuleInputCounter;
//    // ���������� ������� �� ������ �����(4 - LL, L, H, HH).
//    uint8_t nucInputSetPointCounter;
//    uint8_t nucModuleCounter;
//    uint8_t ucFlowControl;
//    TDiscreteSygnalTextDescriptorPackOne *pxDiscreteSygnalTextDescriptorPackOne;
//    TDiscreteSygnalTextDescriptorPackOne xDiscreteSygnalTextDescriptorPackOne;
//    TDiscreteSygnalTextDescriptor *pxDiscreteSygnalTextDescriptor;
//
//
//    // ������� ��������� �� ����� ��� ��������������� ��������� ���� ������ �������.
//    pxDiscreteSygnalTextDescriptor = axDiscreteSygnalTextDescriptor;
//    // ����� � ������� �����.
//    nucDiscreteSignalsCounter = 0;
//    nucModuleInputCounter = 0;
//    // ����� � ������� ������.
//    nucModuleCounter = 1;
//    i = 0;
//
//    memset((uint8_t*)pxDiscreteSygnalTextDescriptor,
//           0,
//           (sizeof(struct TDiscreteSygnalTextDescriptor)));
//
//    // �������� ������ ����� ��������� ���� ������ ��������� ���������� ���������� ��������.
//    // ��� �������� ����������� �������� ����������� �����.
//    while (i < (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity *
//                DISCRETE_MODULE_INPUT_QUANTITY))
//    {
//        // �������� ������ ���������� ��������� ����������� �������.
//        sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
//                "%s_%02d_%02d  ",
//                "��. DI",
//                nucModuleCounter,
//                nucDiscreteSignalsCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
//        // ��������� ���� ������ ����������� �����.
//        nucDiscreteSignalsCounter++;
//        // ��������� ���������.
//        i++;
//        // ���������� ��� ����� ������ ����������� �����?
//        if (nucDiscreteSignalsCounter == DISCRETE_MODULE_INPUT_QUANTITY)
//        {
//            // ����� � ������� �����.
//            nucDiscreteSignalsCounter = 0;
//            // ��������� ������.
//            nucModuleCounter++;
//        }
//    }
//
//    // ����� � ������� �����.
//    nucDiscreteSignalsCounter = 0;
//    nucModuleInputCounter = 0;
//    // ����� � ������ ������� - LL+L.
//    nucInputSetPointCounter = 0;
//    // ����� � ������� ������.
//    nucModuleCounter = 1;
//
//    // �������� ������ ����� ��������� ���� ������ ��������� ���������� ���������� ��������.
//    // ��� �������� ����������� �������� ����������� �����.
//    // ���� ���������� ���� ��������� ������ ���������� ������� - �������: LL, L, H, HH.
//    // ����� � �������� ��������� ��� ������� LL+L.
//    ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
//    while (i < ((xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity *
//                 DISCRETE_MODULE_INPUT_QUANTITY) +
//                (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity *
//                 ANALOG_MODULE_INPUT_QUANTITY *
//                 ANALOGUE_INPUT_DI_VALUE_QUANTITY)))
//    {
//        switch(ucFlowControl)
//        {
//        case ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L:
//            // �������� ������ ���������� ��������� ����������� �������.
//            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
//                    "%s_%02d_%01d_LL",
//                    "��. AI",
//                    nucModuleCounter,
//                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
//            // ������� � ���������� ��������� ��������� �������.
//            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
//            break;
//
//        case ANALOGUE_INPUT_SET_POINT_VIOLATION_L:
//            // �������� ������ ���������� ��������� ����������� �������.
//            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
//                    "%s_%02d_%01d_L ",
//                    "��. AI",
//                    nucModuleCounter,
//                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
//
//            // ������� � ���������� ��������� ��������� �������.
//            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
//            break;
//
//        case ANALOGUE_INPUT_SET_POINT_VIOLATION_H:
//            // �������� ������ ���������� ��������� ����������� �������.
//            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
//                    "%s_%02d_%01d_H ",
//                    "��. AI",
//                    nucModuleCounter,
//                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
//
//            // ������� � ���������� ��������� ��������� �������.
//            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
//            break;
//
//        case ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H:
//            // �������� ������ ���������� ��������� ����������� �������.
//            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
//                    "%s_%02d_%01d_HH",
//                    "��. AI",
//                    nucModuleCounter,
//                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
//
//            // ������� � ���������� ��������� ��������� �������.
//            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
//            break;
//
//        default:
//            break;
//        };
//
//        nucDiscreteSignalsCounter++;
//        nucInputSetPointCounter++;
//        // ���������� ������� �� ������ �����(4 - LL, L, H, HH).
//        // ������ �� ���� �������� ������ �����?
//        if (nucInputSetPointCounter >= 4)
//        {
//            nucInputSetPointCounter = 0;
//            // ��������� ����.
//            nucModuleInputCounter++;
//        }
//        // ��������� ���������.
//        i++;
//
//        // ���������� ��� ���������� ������� ���������� ������� ������ ����������� �����?
//        if (nucDiscreteSignalsCounter == (ANALOG_MODULE_INPUT_QUANTITY *
//                                        ANALOGUE_INPUT_DI_VALUE_QUANTITY))
//        {
//            // ����� � ������� �����.
//            nucDiscreteSignalsCounter = 0;
//            nucModuleInputCounter = 0;
//            // ����� � ������ ������� - LL+L.
//            nucInputSetPointCounter = 0;
//            // ��������� ������.
//            nucModuleCounter++;
//        }
//    }
//
//    // ������� ��������� �� ����� � ����� ��������� ��������������� ��������� ����� ������ �������.
//    pxDiscreteSygnalTextDescriptor = axDiscreteSygnalTextDescriptor;
//    // ������� ��������� �� ���� ������ ������� � ����� �������.
//    pxDiscreteSygnalTextDescriptorPackOne = (TDiscreteSygnalTextDescriptorPackOne*)&(xPlcDataBase.
//                                            axPlcDataBaseBlocks[TEXT_TITLES_DATA_BASE_BLOCK_OFFSET].
//                                            aucPlcDataBaseBlockData[0]);
//    nucBlockCounter = 0;
//    nucBlocksInBlockCounter = 0;
//
//    // ����������� ��������� ���� ������ � ����� ������.
//    for (int i = 0;
//            i < MAX_HANDLED_DISCRETE_INPUT;
//            i++)
//    {
//        // ��������� ���� ���� ���� ������?
//        if (nucBlocksInBlockCounter == TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
//        {
//            // ��������� ���� ���� ������.
//            nucBlockCounter++;
//            // ������� ��������� �� ��������� ���� � ����� ���� ������ �������.
//            pxDiscreteSygnalTextDescriptorPackOne = (TDiscreteSygnalTextDescriptorPackOne*)&(xPlcDataBase.
//                                                    axPlcDataBaseBlocks[TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
//                                                    aucPlcDataBaseBlockData[0]);
//            nucBlocksInBlockCounter = 0;
//        }
//
//        // ��������� ���� ��������� ��������� ���������� ����������� �������, � ����� ������ �������.
//        memcpy((&xDiscreteSygnalTextDescriptorPackOne.
//                acTextDescriptor),
//               (&pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
//               DISCRETE_SYGNAL_NAME_LENGTH);
//
//        memcpy((uint8_t*)&pxDiscreteSygnalTextDescriptorPackOne[nucBlocksInBlockCounter],
//               (uint8_t*)&xDiscreteSygnalTextDescriptorPackOne,
//               sizeof(struct TDiscreteSygnalTextDescriptorPackOne));
//
//        // ��������� ���������.
//        nucBlocksInBlockCounter++;
//    }
//}

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

