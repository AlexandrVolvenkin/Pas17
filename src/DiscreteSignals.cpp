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
#include "Alarm.h"
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
    // Получим указатель на место в массиве дискретных входов для текущего модуля.
    m_puiDiscreteInputsState =
        (GetResources() ->
         m_puiDiscreteInputsState);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedDiscreteInputsState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//    memset(m_puiDiscreteInputsState, 0, 16);
//
//
    // Получим указатель на место в массиве достоверности дискретных входов для текущего модуля.
    m_puiDiscreteInputsBadState =
        (GetResources() ->
         m_puiDiscreteInputsBadState);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedDiscreteInputsBadState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
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


    // Получим указатель на место в рабочем массиве дискретных сигналов для текущего модуля.
    m_pxDiscreteSignalsDescriptionWork =
        &(GetResources() ->
          m_pxDiscreteSignalsDescriptionWork[GetResources() ->
                                 m_uiUsedDiscreteSignalsDescriptionWork]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedDiscreteSignalsDescriptionWork +=
//        ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH;


    // Получим указатель на место в рабочем массиве текстовых реквизитов дискретных сигналов для текущего модуля.
    m_pxDiscreteSygnalTextTitlesWork =
        &(GetResources() ->
          m_pxDiscreteSygnalTextTitlesWork[GetResources() ->
                                                          m_uiUsedDiscreteSygnalTextTitlesWork]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedDiscreteSygnalTextTitlesWork +=
//        ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH;
//
//    m_uiBadAnswerCounter = 0;
}

//-------------------------------------------------------------------------------
// создаёт стартовую базу данных обработки дискретных сигналов.
void CDiscreteSignals::DiscreteSignalsStartDataBaseCreate(void)
{
//    int i;
    uint8_t uiCheck;
    uint8_t nuiBlockCounter;
    uint8_t nuiBlocksInBlockCounter; // один описатель - один блок, в общем блоке.
    uint8_t nuiInputCounter;
    uint8_t nuiModuleCounter;
    uint8_t *puiSource;
    uint8_t uiFlowControl;
    TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDescriptionWork;
    TDiscreteSignalsDescriptionWorkPackOne *pxDiscreteSignalsDescriptionWorkPackOne;
    TDiscreteSignalsDescriptionWorkPackOne xDiscreteSignalsDescriptionWorkSourse;
    // получим указатель на рабочий массив дискретных сигналов.
    pxDiscreteSignalsDescriptionWork = m_pxDiscreteSignalsDescriptionWork;

    // начнём с первого входа.
    nuiInputCounter = 0;
    // начнём с первого модуля.
    nuiModuleCounter = 1;
//    i = 0;

    memset((uint8_t*)pxDiscreteSignalsDescriptionWork,
           0,
           (sizeof(struct TDiscreteSignalsDescriptionWork) *
            MAX_HANDLED_DISCRETE_INPUT));

//    // создадим первую часть стартовой базы данных дискретных сигналов.
//    // для сигналов пораждаемых модулями дискретного ввода.
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
//            // заполним номер группы. содержится в счётчике - nuiModuleCounter. для каждого модуля свой номер группы.
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= (nuiModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
//            // тип контакта - нормально разомкнутый.
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= CONTACT_TYPE_DISC_MASK;
//            // аварийная сигнализация.
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
//            // первый выход управления реле, превого модуля реле МР1.
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].auiRelayOut[0] = 1;
//            // время задержки - 5 секунд.
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay |= (DELAY_TIME_SETPOINT_DISC_MASK & 5);
//            // дискретность времени задержки - 1 секунда.
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~DELAY_TIME_DISCRETENESS_DISC_MASK;
//            // Не архивировать.
//            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~ARCHIVE_DISC_MASK;
//
//            // вычислим CRC описателя.
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
//            // следующий вход модуля дискретного ввода.
//            // следующий описатель.
//            nuiInputCounter++;
//
//        }
//
//        // следующий модуль.
//        nuiModuleCounter++;
//    }



    // создадим вторую часть стартовой базы данных дискретных сигналов.
    // для сигналов пораждаемых модулями аналогового ввода.
    // один аналоговый вход пораждает четыре дискретных сигнала - уставки: LL, L, H, HH.
    // начнём с создания описателя для уставки LL+L.
    uiFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;

    for (int i = 0;
            i < (1);
            i++)
//    for (int i = 0;
//            i < ((GetResources() ->
//                  GetDeviceConfigSearchPointer()) ->
//                 uiServiceAnalogueInputModuleQuantity);
//            i++)
    {
        for (int j = 0, k = 0;
                j < ((ANALOG_MODULE_INPUT_QUANTITY / 2) *
                     ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                j++)
        {
            switch(uiFlowControl)
            {
            case ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L:
                // заполним номер группы. содержится в счётчике - nuiModuleCounter. для каждого модуля свой номер группы.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= (nuiModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // тип контакта - нормально разомкнутый.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // аварийная сигнализация.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
                // нет выхода управления реле, превого модуля реле МР1.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].auiRelayOut[0] = 0;
                // нет задержки.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // дискретность времени задержки - 1 секунда.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // Не архивировать.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~ARCHIVE_DISC_MASK;
                // перейдём к заполнению описателя следующей уставки.
                uiFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                break;

            case ANALOGUE_INPUT_SET_POINT_VIOLATION_L:
                // заполним номер группы. содержится в счётчике - nuiModuleCounter. для каждого модуля свой номер группы.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= (nuiModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // тип контакта - нормально разомкнутый.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // предупредительная сигнализация.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp &= ~SIGNALING_TYPE_2_DISC_MASK;
                // нет выхода управления реле, превого модуля реле МР1.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].auiRelayOut[0] = 0;
                // нет задержки.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // дискретность времени задержки - 1 секунда.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // Не архивировать.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~ARCHIVE_DISC_MASK;
                // перейдём к заполнению описателя следующей уставки.
                uiFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                break;

            case ANALOGUE_INPUT_SET_POINT_VIOLATION_H:
                // заполним номер группы. содержится в счётчике - nuiModuleCounter. для каждого модуля свой номер группы.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= (nuiModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // тип контакта - нормально разомкнутый.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // предупредительная сигнализация.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp &= ~SIGNALING_TYPE_2_DISC_MASK;
                // нет выхода управления реле, превого модуля реле МР1.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].auiRelayOut[0] = 0;
                // нет задержки.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // дискретность времени задержки - 1 секунда.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // Не архивировать.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~ARCHIVE_DISC_MASK;
                // перейдём к заполнению описателя следующей уставки.
                uiFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                break;

            case ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H:
                // заполним номер группы. содержится в счётчике - nuiModuleCounter. для каждого модуля свой номер группы.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= (nuiModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // тип контакта - нормально разомкнутый.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // аварийная сигнализация.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
                // нет выхода управления реле, превого модуля реле МР1.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].auiRelayOut[0] = 0;
                // нет задержки.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // дискретность времени задержки - 1 секунда.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // Не архивировать.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~ARCHIVE_DISC_MASK;
                // перейдём к заполнению описателя следующей уставки.
                uiFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                break;

            default:
                break;
            };

            // вычислим CRC описателя.
            puiSource = (uint8_t*)&pxDiscreteSignalsDescriptionWork[nuiInputCounter];
            uiCheck = 0xFF;
            for (int k = 0;
                    k < (sizeof(struct TDiscreteSignalsDescriptionWork) - ONE_BYTE_CRC_LENGTH);
                    k++)
            {
                uiCheck += puiSource[k];
            }
            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiCrc = uiCheck;

            // следующий вход модуля дискретного ввода.
            // следующий описатель.
            nuiInputCounter++;

        }



        for (int j = 0, k = 0;
                j < ((ANALOG_MODULE_INPUT_QUANTITY / 2) *
                     ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                j++)
        {
            switch(uiFlowControl)
            {
            case ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L:
                // заполним номер группы. содержится в счётчике - nuiModuleCounter. для каждого модуля свой номер группы.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= (nuiModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // тип контакта - нормально разомкнутый.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // аварийная сигнализация.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
                // нет выхода управления реле, превого модуля реле МР1.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].auiRelayOut[0] = 0;
                // нет задержки.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // дискретность времени задержки - 1 секунда.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // Не архивировать.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~ARCHIVE_DISC_MASK;
                // перейдём к заполнению описателя следующей уставки.
                uiFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                break;

            case ANALOGUE_INPUT_SET_POINT_VIOLATION_L:
                // заполним номер группы. содержится в счётчике - nuiModuleCounter. для каждого модуля свой номер группы.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= (nuiModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // тип контакта - нормально разомкнутый.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // предупредительная сигнализация.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp &= ~SIGNALING_TYPE_2_DISC_MASK;
                // нет выхода управления реле, превого модуля реле МР1.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].auiRelayOut[0] = (1 << (k));
                k++;
                // нет задержки.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // дискретность времени задержки - 1 секунда.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // Не архивировать.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~ARCHIVE_DISC_MASK;
                // перейдём к заполнению описателя следующей уставки.
                uiFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                break;

            case ANALOGUE_INPUT_SET_POINT_VIOLATION_H:
                // заполним номер группы. содержится в счётчике - nuiModuleCounter. для каждого модуля свой номер группы.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= (nuiModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // тип контакта - нормально разомкнутый.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // предупредительная сигнализация.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp &= ~SIGNALING_TYPE_2_DISC_MASK;
                // нет выхода управления реле, превого модуля реле МР1.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].auiRelayOut[0] = (1 << (k));
                k++;
                // нет задержки.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // дискретность времени задержки - 1 секунда.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // Не архивировать.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~ARCHIVE_DISC_MASK;
                // перейдём к заполнению описателя следующей уставки.
                uiFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                break;

            case ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H:
                // заполним номер группы. содержится в счётчике - nuiModuleCounter. для каждого модуля свой номер группы.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= (nuiModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // тип контакта - нормально разомкнутый.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // аварийная сигнализация.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
                // нет выхода управления реле, превого модуля реле МР1.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].auiRelayOut[0] = 0;
                // нет задержки.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // дискретность времени задержки - 1 секунда.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay = 0;
                // Не архивировать.
                pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~ARCHIVE_DISC_MASK;
                // перейдём к заполнению описателя следующей уставки.
                uiFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                break;

            default:
                break;
            };

            // вычислим CRC описателя.
            puiSource = (uint8_t*)&pxDiscreteSignalsDescriptionWork[nuiInputCounter];
            uiCheck = 0xFF;
            for (int k = 0;
                    k < (sizeof(struct TDiscreteSignalsDescriptionWork) - ONE_BYTE_CRC_LENGTH);
                    k++)
            {
                uiCheck += puiSource[k];
            }
            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiCrc = uiCheck;

            // следующий вход модуля дискретного ввода.
            // следующий описатель.
            nuiInputCounter++;

        }

        // следующий модуль.
        nuiModuleCounter++;
    }
}

//-------------------------------------------------------------------------------
// преобразует блок стартовой базы данных обработки дискретных сигналов из рабочего в общий формат.
void CDiscreteSignals::DiscreteSignalsStartDataBlockWorkToCommonFormat(uint8_t* puiBlockDataPointer,
        TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDescriptionWork)
{
    TDiscreteSignalsDescriptionWorkPackOne *pxDiscreteSignalsDescriptionWorkPackOne;

    memset(m_puiIntermediateBuff,
           0,
           CDataStore::MAX_BLOCK_LENGTH);
    // получим указатель на базу данных прибора в общем формате.
    pxDiscreteSignalsDescriptionWorkPackOne =
        (TDiscreteSignalsDescriptionWorkPackOne*)m_puiIntermediateBuff;

    // преобразуем блок созданной базы данных в общий формат.
    for (int i = 0;
            i < DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY;
            i++)
    {
        // скопируем один описатель дискретного сигнала, в буфер общего формата.
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
// преобразовывает из общего формата базы данных, в формат хранения в RAM.
// база данных в приборе - это массив, длиной 100 блоков. каждый блок 256 байт.
// блоки баз данных модулей, дискретных сигналов, функциональных блоков и др., имеют различный рамер.
// базы данных модулей, дискретных сигналов, функциональных блоков и др., могут занимать несколько блоков.
// чтобы иметь возможность "плоской" адресации к описателям объектов, делается преобразование.
void CDiscreteSignals::DiscreteSignalsDataBlockCommonFormatToWork(void)
{
    std::cout << "CDiscreteSignals::DiscreteSignalsDataBlockCommonFormatToWork 1"  << std::endl;
    unsigned char ucCheck;
    unsigned char *pucSource;
    TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDescriptionWork;
    TDiscreteSignalsDescriptionWorkPackOne *pxDiscreteSignalsDescriptionWorkPackOne;



    {
        std::cout << "CDiscreteSignals::DiscreteSignalsDataBlockCommonFormatToWork m_puiIntermediateBuff 1"  << std::endl;
        uint8_t *pucSourceTemp;
        pucSourceTemp = (uint8_t*)(m_puiIntermediateBuff);
        for(int i=0; i<128 ; )
        {
            for(int j=0; j<8; j++)
            {
                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
            }
            cout << endl;
            i += 8;
        }
    }

    // получим указатель на рабочий массив дискретных сигналов.
    pxDiscreteSignalsDescriptionWork = m_pxDiscreteSignalsDescriptionWork;

    // получим указатель на базу данных прибора в общем формате.
    pxDiscreteSignalsDescriptionWorkPackOne =
        (TDiscreteSignalsDescriptionWorkPackOne*)m_puiIntermediateBuff;

    // преобразуем из общего формата базы данных, в формат хранения в RAM.
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

        // вычислим контрольную сумму описателя.
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



    {
        std::cout << "CDiscreteSignals::DiscreteSignalsDataBlockCommonFormatToWork DiscreteSignalsDataBlockCommonFormatToWork 1"  << std::endl;
        uint8_t *pucSourceTemp;
        pucSourceTemp = (uint8_t*)(pxDiscreteSignalsDescriptionWorkPackOne[8].auiRelayOut);
        for(int i=0; i<32 ; )
        {
            for(int j=0; j<8; j++)
            {
                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
            }
            cout << endl;
            i += 8;
        }
    }
}

//-----------------------------------------------------------------------------------------------------
// проверяет совпадение контрольной суммы одного описателя дискретного сигнала.
// получает указатель на один описатель дискретного сигнала в нормализованной базе данных,
// преобразованной функцией vDiscreteSignalsDataBaseExstract().
uint8_t CDiscreteSignals::DiscreteSignalsDataBaseCrcCheck(
    TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDataBase)
{
    unsigned char ucCheck;
    unsigned char *pucSource;
    // получим указатель на один описатель дискретного сигнала в нормализованной базе данных.
    pucSource = (unsigned char*)pxDiscreteSignalsDataBase;
    // начальное значение CRC. чтобы сумма не была нулевой.
    ucCheck = 0xFF;
    // суммируем байты структуры описателей struct TDiscreteSignalsDescriptionWork
    // кроме байта CRC.
    for (int i = 0;
            i < ((sizeof(struct TDiscreteSignalsDescriptionWork)) - ONE_BYTE_CRC_LENGTH);
            i++)
    {
        ucCheck += pucSource[i];
    }
    // CRC совпадает?
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
// вычисляет количество запрограммированных дискретных сигналов.
void CDiscreteSignals::ProgrammedDiscreteSignalsNumberCount(void)
{
    std::cout << "CConfigurationCreate::ProgrammedDiscreteSignalsNumberCount 1" << endl;

    // получим указатель на объект конфигурации.
    TConfigDataPackOne* pxDeviceConfigSearch =
        (TConfigDataPackOne*)
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);

    unsigned char ucCheck;
    unsigned char *pucSource;
    TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDescriptionWork;

    uint16_t nucModuleCounter = 0;

    // получим указатель на рабочий массив дискретных сигналов.
    pxDiscreteSignalsDescriptionWork = m_pxDiscreteSignalsDescriptionWork;

    // посчитаем количество запрограммированных, обрабатываемых дискретных входов.
    for (int i = 0;
            i < MAX_HANDLED_DISCRETE_INPUT;
            i++)
    {
        pucSource = (unsigned char*)&pxDiscreteSignalsDescriptionWork[i];
        ucCheck = 0;
        // суммируем данные описателя, если, не равно нулю, то вход запрограммирован.
        for (int j = 0;
                j < (sizeof(struct TDiscreteSignalsDescriptionWork) - ONE_BYTE_CRC_LENGTH);
                j++)
        {
            ucCheck += pucSource[j];
        }

        if (ucCheck)
        {
            // ещё один дискретный сигнал запрограммирован.
            nucModuleCounter++;
        }
        else
        {
            break;
        }
    }

    std::cout << "CConfigurationCreate::ProgrammedDiscreteSignalsNumberCount nucModuleCounter " << nucModuleCounter << endl;
    pxDeviceConfigSearch ->
    uiHandledDiscreteSignalsQuantity = nucModuleCounter;

//    (GetResources() -> GetDeviceConfigSearchPointer()) ->
//    uiHandledDiscreteSignalsQuantity =
//        nucModuleCounter;
}

//-----------------------------------------------------------------------------------------------------
// преобразовывает из общего формата базы данных, в формат хранения в RAM.
// база данных в приборе - это массив, длиной 100 блоков. каждый блок 256 байт.
// блоки баз данных модулей, дискретных сигналов, функциональных блоков и др., имеют различный рамер.
// базы данных модулей, дискретных сигналов, функциональных блоков и др., могут занимать несколько блоков.
// чтобы иметь возможность "плоской" адресации к описателям объектов, делается преобразование.
void CDiscreteSignals::DiscreteSignalsTextTitlesDataBlockCommonFormatToWork(void)
{
    std::cout << "CDiscreteSignals::DiscreteSignalsTextTitlesDataBlockCommonFormatToWork 1"  << std::endl;
    TDiscreteSygnalTextTitle *pxDiscreteSygnalTextTitlesWork;
    TDiscreteSygnalTextTitlePackOne *pxDiscreteSygnalTextTitlesWorkPackOne;

    // получим указатель на рабочий массив текстовых реквизитов дискретных сигналов.
    pxDiscreteSygnalTextTitlesWork = m_pxDiscreteSygnalTextTitlesWork;

    // получим указатель на базу данных прибора в общем формате.
    pxDiscreteSygnalTextTitlesWorkPackOne =
        (TDiscreteSygnalTextTitlePackOne*)m_puiIntermediateBuff;

    // преобразуем из общего формата базы данных, в формат хранения в RAM.
    for (int i = 0;
            i < 16;
            i++)
    {
        memset(pxDiscreteSygnalTextTitlesWork[i].acTextDescriptor,
               0,
               (DISCRETE_SYGNAL_NAME_LENGTH + END_OF_STRING_LENGTH));

        memcpy(pxDiscreteSygnalTextTitlesWork[i].acTextDescriptor,
               pxDiscreteSygnalTextTitlesWorkPackOne[i].acTextDescriptor,
               DISCRETE_SYGNAL_NAME_LENGTH);
    }
}

//-------------------------------------------------------------------------------
void CDiscreteSignals::ServiceDataCreate(void)
{
    std::cout << "CInternalModule::CDiscreteSignals 1"  << std::endl;

    ProgrammedDiscreteSignalsNumberCount();
}

//-------------------------------------------------------------------------------
template <typename T>
void CDiscreteSignals::CreateAlarmHandler(CResources* res,
        uint16_t uiAlarmHandlerIndex,
        std::string sDeviceName)
{
    sDeviceName = sDeviceName + std::to_string(uiAlarmHandlerIndex);
    std::cout << "CDiscreteSignals::CreateAlarmHandler sDeviceName " << sDeviceName << std::endl;
    T* pxAlarmHandler = nullptr;
    pxAlarmHandler =
        static_cast<T*>(res ->
                        AddCommonTaskToMap(sDeviceName,
                                           std::make_shared<T>()));
    pxAlarmHandler ->
    SetResources(res);
    pxAlarmHandler ->
    SetAlarmHandlerIndex(uiAlarmHandlerIndex);

    for (uint8_t j = 0; j < DISCRETE_OUTPUT_MODULE_MAX_NUMBER; j++)
    {
        pxAlarmHandler ->
        GetLinkedDiscreteOutputsPointer()[j] =
            (m_pxDiscreteSignalsDescriptionWork[uiAlarmHandlerIndex].
             auiRelayOut[j]);
    }

    // дискретность времени задержки включения реле 10 секунд?
    if (((m_pxDiscreteSignalsDescriptionWork[uiAlarmHandlerIndex].uiDelay >> 6) & 0x01))
    {
        pxAlarmHandler ->
        SetRelayOnDelay(((m_pxDiscreteSignalsDescriptionWork[uiAlarmHandlerIndex].uiDelay) & 0x3f) * 10);
    }
    else
    {
        // дискретность времени задержки включения реле 1 секунда.
        pxAlarmHandler ->
        SetRelayOnDelay(((m_pxDiscreteSignalsDescriptionWork[uiAlarmHandlerIndex].uiDelay) & 0x3f));
    }

    pxAlarmHandler ->
    SetDiscreteInputsState(&m_puiDiscreteInputsState[uiAlarmHandlerIndex]);
    pxAlarmHandler ->
    SetDiscreteInputsBadState(&m_puiDiscreteInputsBadState[uiAlarmHandlerIndex]);

    m_vpxAlarmHandlers.push_back(pxAlarmHandler);
    m_vuiAlarmHandlersId.push_back(GetResources() ->
                                   GetTaskIdByNameFromMap(sDeviceName));
}

//-------------------------------------------------------------------------------
void CDiscreteSignals::CreateAlarmHandlers(void)
{
    std::cout << "CDiscreteSignals::CreateAlarmHandlers 1"  << std::endl;

    // получим указатель на объект конфигурации.
    TConfigDataPackOne* pxDeviceConfigSearch =
        (GetResources() -> GetDeviceConfigSearchPointer());

    TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDescriptionWork;
    // получим указатель на рабочий массив описателей дискретных сигналов.
    pxDiscreteSignalsDescriptionWork = m_pxDiscreteSignalsDescriptionWork;

    for (uint16_t i = 0;
            i < (pxDeviceConfigSearch ->
                 uiHandledDiscreteSignalsQuantity);
            i++)
    {
        // Какой тип сигнализации связан с дискретным сигналом?
        switch ((pxDiscreteSignalsDescriptionWork[i].uiTalTkGrp >> 6) & 0x03)
        {
        case NORMAL:
        {
    std::cout << "CDiscreteSignals::CreateAlarmHandlers 2"  << std::endl;
            CreateAlarmHandler<CNormalAlarmDfa>(GetResources(),
                                                i,
                                                "CNormalAlarmDfa");
        }
        break;

        case INDICATION:
            // Уровень дискретного сигнала интерпретируемый как активный - высокий?
            if ((pxDiscreteSignalsDescriptionWork[i].uiTalTkGrp >> 5) & 0x01)
            {
    std::cout << "CDiscreteSignals::CreateAlarmHandlers 3"  << std::endl;
                CreateAlarmHandler<CIndicationAlarmHighLevelDfa>(GetResources(),
                        i,
                        "CIndicationAlarmHighLevelDfa");
            }
            else
            {
    std::cout << "CDiscreteSignals::CreateAlarmHandlers 4"  << std::endl;
                CreateAlarmHandler<CIndicationAlarmLowLevelDfa>(GetResources(),
                        i,
                        "CIndicationAlarmLowLevelDfa");
            }
            break;

        case PREVENTIVE:
            // Уровень дискретного сигнала интерпретируемый как активный - высокий?
            if ((pxDiscreteSignalsDescriptionWork[i].uiTalTkGrp >> 5) & 0x01)
            {
    std::cout << "CDiscreteSignals::CreateAlarmHandlers 5"  << std::endl;
                CreateAlarmHandler<CPreventiveAlarmHighLevelDfa>(GetResources(),
                        i,
                        "CPreventiveAlarmHighLevelDfa");
            }
            else
            {
    std::cout << "CDiscreteSignals::CreateAlarmHandlers 6"  << std::endl;
                CreateAlarmHandler<CPreventiveAlarmLowLevelDfa>(GetResources(),
                        i,
                        "CPreventiveAlarmLowLevelDfa");
            }
            break;

        case EMERGENCY:
            // Уровень дискретного сигнала интерпретируемый как активный - высокий?
            if ((pxDiscreteSignalsDescriptionWork[i].uiTalTkGrp >> 5) & 0x01)
            {
    std::cout << "CDiscreteSignals::CreateAlarmHandlers 7"  << std::endl;
                CreateAlarmHandler<CEmergencyAlarmHighLevelDfa>(GetResources(),
                        i,
                        "CEmergencyAlarmHighLevelDfa");
            }
            else
            {
    std::cout << "CDiscreteSignals::CreateAlarmHandlers 8"  << std::endl;
                CreateAlarmHandler<CEmergencyAlarmLowLevelDfa>(GetResources(),
                        i,
                        "CEmergencyAlarmLowLevelDfa");
            }
            break;

        case IND_PREVENTIVE:
        {
    std::cout << "CDiscreteSignals::CreateAlarmHandlers 9"  << std::endl;
            CreateAlarmHandler<CNormalAlarmDfa>(GetResources(),
                                                i,
                                                "CNormalAlarmDfa");
        }

        break;

        case IND_EMERGENCY:
        {
    std::cout << "CDiscreteSignals::CreateAlarmHandlers 10"  << std::endl;
            CreateAlarmHandler<CNormalAlarmDfa>(GetResources(),
                                                i,
                                                "CNormalAlarmDfa");
        }

        break;

        default:
        {
    std::cout << "CDiscreteSignals::CreateAlarmHandlers 11"  << std::endl;
            CreateAlarmHandler<CNormalAlarmDfa>(GetResources(),
                                                i,
                                                "CNormalAlarmDfa");
        }

        break;
        }
    }
}

//-------------------------------------------------------------------------------
void CDiscreteSignals::AlarmHandlersProcessing(void)
{
//    std::cout << "CDiscreteSignals::AlarmHandlersProcessing 1" << endl;
//    if (m_xAlarmHandlersIterator == m_vpxAlarmHandlers.end())
//    {
//        m_xAlarmHandlersIterator = m_vpxAlarmHandlers.begin();
//    }
//
//    (*m_xAlarmHandlersIterator) -> Fsm();
//    m_xAlarmHandlersIterator++;


//    {
//        std::cout << "CDiscreteSignals::AlarmHandlersProcessing m_puiDiscreteInputsState"  << std::endl;
//        uint8_t *pucSourceTemp;
//        pucSourceTemp = (uint8_t*)(m_puiDiscreteInputsState);
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

//    {
//        std::cout << "CDiscreteSignals::AlarmHandlersProcessing m_puiDiscreteInputsBadState"  << std::endl;
//        uint8_t *pucSourceTemp;
//        pucSourceTemp = (uint8_t*)(m_puiDiscreteInputsBadState);
//        for(int i=0; i<32 ; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//
//    }

//    {
//        std::cout << "CDiscreteSignals::AlarmHandlersProcessing m_pxDiscreteOutputControl 1"  << std::endl;
//        uint8_t *pucSourceTemp;
//        pucSourceTemp = (uint8_t*)(GetResources() -> m_pxDiscreteOutputControl);
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

    for (auto m_xAlarmHandlersIterator = m_vpxAlarmHandlers.begin();
            m_xAlarmHandlersIterator != m_vpxAlarmHandlers.end(); ++m_xAlarmHandlersIterator)
    {
        (*m_xAlarmHandlersIterator) -> Fsm();
    }

//    {
//        std::cout << "CDiscreteSignals::AlarmHandlersProcessing m_pxDiscreteOutputControl 2"  << std::endl;
//        uint8_t *pucSourceTemp;
//        pucSourceTemp = (uint8_t*)(GetResources() -> m_pxDiscreteOutputControl);
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
            // Время ожидания выполнения запроса закончилось?
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
            // Время ожидания выполнения запроса закончилось?
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
            // Время ожидания выполнения запроса закончилось?
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
        std::cout << "CDiscreteSignals::Fsm DISCRETE_SIGNALS_CREATE_DATA_BASE_START"  << std::endl;
        {
            // создадим стартовую базу данных дискретных сигналов в рабочем формате.
            DiscreteSignalsStartDataBaseCreate();
            m_uiBlocksCounter = 0;
            SetFsmState(DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_START);
        }
        break;

    case DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_START:
        std::cout << "CDiscreteSignals::Fsm DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_START"  << std::endl;
        {
            DiscreteSignalsStartDataBlockWorkToCommonFormat(m_puiIntermediateBuff,
                    // получим указатель на блок в буфере с вновь созданной нормализованной стартовой базой данных дискретных сигналов.
                    &(m_pxDiscreteSignalsDescriptionWork[(DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY *
                            m_uiBlocksCounter)]));

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
            // стартовые описатели дискретных сигналов блок 12
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
        std::cout << "CDiscreteSignals::Fsm DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
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
    case DISCRETE_SIGNALS_CREATE_SERVICE_DATA_CREATE_START:
        std::cout << "CDiscreteSignals::Fsm DISCRETE_SIGNALS_CREATE_SERVICE_DATA_CREATE_START"  << std::endl;
        {
            uint8_t uiDataStoreId =
                GetResources() ->
                GetTaskIdByNameFromMap("DataStoreFileSystem");

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::READ_BLOCK_DATA_START;
            // стартовые описатели дискретных сигналов блок 12
            pxDataContainer -> m_uiDataIndex = DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DISCRETE_SIGNALS_CREATE_SERVICE_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DISCRETE_SIGNALS_CREATE_SERVICE_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(DISCRETE_SIGNALS_CREATE_SERVICE_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(DISCRETE_SIGNALS_CREATE_SERVICE_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case DISCRETE_SIGNALS_CREATE_SERVICE_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDiscreteSignals::Fsm DISCRETE_SIGNALS_CREATE_SERVICE_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            DiscreteSignalsDataBlockCommonFormatToWork();
            ServiceDataCreate();

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case DISCRETE_SIGNALS_CREATE_SERVICE_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDiscreteSignals::Fsm DISCRETE_SIGNALS_CREATE_SERVICE_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case DISCRETE_SIGNALS_TEXT_TITLES_COMMON_TO_WORK_START:
        std::cout << "CDiscreteSignals::Fsm DISCRETE_SIGNALS_TEXT_TITLES_COMMON_TO_WORK_START"  << std::endl;
        {
            uint8_t uiDataStoreId =
                GetResources() ->
                GetTaskIdByNameFromMap("DataStoreFileSystem");

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::READ_BLOCK_DATA_START;
            // текстовые реквизиты дискретных сигналов блок 40
            pxDataContainer -> m_uiDataIndex = TEXT_TITLES_DATA_BASE_BLOCK_OFFSET;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DISCRETE_SIGNALS_TEXT_TITLES_COMMON_TO_WORK_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DISCRETE_SIGNALS_TEXT_TITLES_COMMON_TO_WORK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(DISCRETE_SIGNALS_TEXT_TITLES_COMMON_TO_WORK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(DISCRETE_SIGNALS_TEXT_TITLES_COMMON_TO_WORK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case DISCRETE_SIGNALS_TEXT_TITLES_COMMON_TO_WORK_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDiscreteSignals::Fsm DISCRETE_SIGNALS_TEXT_TITLES_COMMON_TO_WORK_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            DiscreteSignalsTextTitlesDataBlockCommonFormatToWork();

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case DISCRETE_SIGNALS_TEXT_TITLES_COMMON_TO_WORK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDiscreteSignals::Fsm DISCRETE_SIGNALS_TEXT_TITLES_COMMON_TO_WORK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case DISCRETE_SIGNALS_CREATE_ALARM_HANDLERS_START:
        std::cout << "CDiscreteSignals::Fsm DISCRETE_SIGNALS_CREATE_ALARM_HANDLERS_START"  << std::endl;
        {
            CreateAlarmHandlers();

            SetFsmState(DISCRETE_SIGNALS_CREATE_ALARM_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        }
        break;

    case DISCRETE_SIGNALS_CREATE_ALARM_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDiscreteSignals::Fsm DISCRETE_SIGNALS_CREATE_ALARM_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case DISCRETE_SIGNALS_CREATE_ALARM_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDiscreteSignals::Fsm DISCRETE_SIGNALS_CREATE_ALARM_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_START:
//        std::cout << "CDiscreteSignals::Fsm DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_START"  << std::endl;
    {
        AlarmHandlersProcessing();

        SetFsmState(DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
    }
    break;

    case DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
//        std::cout << "CDiscreteSignals::Fsm DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDiscreteSignals::Fsm DISCRETE_SIGNALS_PROCESSING_ALARM_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
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

