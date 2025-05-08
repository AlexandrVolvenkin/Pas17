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
           MAX_HANDLED_DISCRETE_INPUT);

    // создадим первую часть стартовой базы данных дискретных сигналов.
    // для сигналов пораждаемых модулями дискретного ввода.
    for (int i = 0;
            i < ((GetResources() ->
                  GetDeviceConfigSearchPointer()) ->
                 uiServiceDiscreteInputModuleQuantity);
            i++)
    {
        for (int j = 0;
                j < DISCRETE_MODULE_INPUT_QUANTITY;
                j++)
        {
            // заполним номер группы. содержится в счётчике - nuiModuleCounter. для каждого модуля свой номер группы.
            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= (nuiModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
            // тип контакта - нормально разомкнутый.
            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= CONTACT_TYPE_DISC_MASK;
            // аварийная сигнализация.
            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
            // первый выход управления реле, превого модуля реле МР1.
            pxDiscreteSignalsDescriptionWork[nuiInputCounter].auiRelayOut[0] = 1;
            // время задержки - 5 секунд.
            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay |= (DELAY_TIME_SETPOINT_DISC_MASK & 5);
            // дискретность времени задержки - 1 секунда.
            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~DELAY_TIME_DISCRETENESS_DISC_MASK;
            // Не архивировать.
            pxDiscreteSignalsDescriptionWork[nuiInputCounter].uiDelay &= ~ARCHIVE_DISC_MASK;

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



    // создадим вторую часть стартовой базы данных дискретных сигналов.
    // для сигналов пораждаемых модулями аналогового ввода.
    // один аналоговый вход пораждает четыре дискретных сигнала - уставки: LL, L, H, HH.
    // начнём с создания описателя для уставки LL+L.
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

        // следующий модуль.
        nuiModuleCounter++;
    }




    // получим указатель на буфер с вновь созданной нормализованной стартовой базой данных дискретных сигналов.
    pxDiscreteSignalsDescriptionWork = m_pxDiscreteSignalsDescriptionWork;
//    // получим указатель на базу данных прибора в общем формате.
//    pxDiscreteSignalsDescriptionWorkPackOne = (TDiscreteSignalsDescriptionWorkPackOne*)&(xPlcDataBase.
//            axPlcDataBaseBlocks[DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET].
//            auiPlcDataBaseBlockData[0]);
    nuiBlockCounter = 0;
    nuiBlocksInBlockCounter = 0;

    // преобразуем созданную базу данных в общий формат.
    for (int i = 0;
            i < MAX_HANDLED_DISCRETE_INPUT;
            i++)
    {
        // обработан весь блок базы данных?
        if (nuiBlocksInBlockCounter == DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // следующий блок базы данных.
            nuiBlockCounter++;
//            // получим указатель на следующий блок в общей базе данных прибора.
//            pxDiscreteSignalsDescriptionWorkPackOne = (TDiscreteSignalsDescriptionWorkPackOne*)&(xPlcDataBase.
//                    axPlcDataBaseBlocks[DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + nuiBlockCounter].
//                    auiPlcDataBaseBlockData[0]);
            nuiBlocksInBlockCounter = 0;
        }

        // скопируем один описатель дискретного сигнала, в буфер общего формата.
        xDiscreteSignalsDescriptionWorkSourse.uiTalTkGrp =
            pxDiscreteSignalsDescriptionWork[i].uiTalTkGrp;

        memcpy(xDiscreteSignalsDescriptionWorkSourse.auiRelayOut,
               pxDiscreteSignalsDescriptionWork[i].auiRelayOut,
               8);

        xDiscreteSignalsDescriptionWorkSourse.uiDelay =
            pxDiscreteSignalsDescriptionWork[i].uiDelay;
        xDiscreteSignalsDescriptionWorkSourse.uiCrc =
            pxDiscreteSignalsDescriptionWork[i].uiCrc;

        memcpy((uint8_t*)&pxDiscreteSignalsDescriptionWorkPackOne[nuiBlocksInBlockCounter],
               (uint8_t*)&xDiscreteSignalsDescriptionWorkSourse,
               sizeof(struct TDiscreteSignalsDescriptionWorkPackOne));

        // следующий описатель.
        nuiBlocksInBlockCounter++;
    }
}

//-------------------------------------------------------------------------------
// преобразует блок стартовой базы данных обработки дискретных сигналов из рабочего в общий формат.
void CDiscreteSignals::DiscreteSignalsStartDataBlockWorkToCommonFormat(uint8_t* puiBlockDataPointer,
        TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDescriptionWork)
{
//    TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDescriptionWork;
    TDiscreteSignalsDescriptionWorkPackOne *pxDiscreteSignalsDescriptionWorkPackOne;
//    // получим указатель на блок в буфере с вновь созданной нормализованной стартовой базой данных дискретных сигналов.
//    pxDiscreteSignalsDescriptionWork =
//        m_pxDiscreteSignalsDescriptionWork[((sizeof(struct TDiscreteSignalsDescriptionWork) *
//    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY) *
//    uiBlockIndex)];
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
               8);

        pxDiscreteSignalsDescriptionWorkPackOne[i].uiDelay =
            pxDiscreteSignalsDescriptionWork[i].uiDelay;
        pxDiscreteSignalsDescriptionWorkPackOne[i].uiCrc =
            pxDiscreteSignalsDescriptionWork[i].uiCrc;
    }
}

////-------------------------------------------------------------------------------
//// создаёт стартовую базу данных текстовых реквизитов дискретных сигналов.
//void CDiscreteSignals::DiscreteSygnalsTextDescriptorStartDataBaseCreate(uint8_t* puiBlockDataPointer)
//{
//    int i;
//    uint8_t nucBlockCounter;
//    uint8_t nucBlocksInBlockCounter; // один описатель - один блок, в общем блоке.
//    uint8_t nucDiscreteSignalsCounter;
//    uint8_t nucModuleInputCounter;
//    // количество уставок по одному входу(4 - LL, L, H, HH).
//    uint8_t nucInputSetPointCounter;
//    uint8_t nucModuleCounter;
//    uint8_t ucFlowControl;
//    TDiscreteSygnalTextDescriptorPackOne *pxDiscreteSygnalTextDescriptorPackOne;
//    TDiscreteSygnalTextDescriptorPackOne xDiscreteSygnalTextDescriptorPackOne;
//    TDiscreteSygnalTextDescriptor *pxDiscreteSygnalTextDescriptor;
//
//
//    // получим указатель на буфер для нормализованной стартовой базы данных прибора.
//    pxDiscreteSygnalTextDescriptor = axDiscreteSygnalTextDescriptor;
//    // начнём с первого входа.
//    nucDiscreteSignalsCounter = 0;
//    nucModuleInputCounter = 0;
//    // начнём с первого модуля.
//    nucModuleCounter = 1;
//    i = 0;
//
//    memset((uint8_t*)pxDiscreteSygnalTextDescriptor,
//           0,
//           (sizeof(struct TDiscreteSygnalTextDescriptor)));
//
//    // создадим первую часть стартовой базы данных текстовых реквизитов дискретных сигналов.
//    // для сигналов пораждаемых модулями дискретного ввода.
//    while (i < (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity *
//                DISCRETE_MODULE_INPUT_QUANTITY))
//    {
//        // создадим строку текстового реквизита дискретного сигнала.
//        sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
//                "%s_%02d_%02d  ",
//                "Вх. DI",
//                nucModuleCounter,
//                nucDiscreteSignalsCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
//        // следующий вход модуля дискретного ввода.
//        nucDiscreteSignalsCounter++;
//        // следующий описатель.
//        i++;
//        // обработаны все входы модуля дискретного ввода?
//        if (nucDiscreteSignalsCounter == DISCRETE_MODULE_INPUT_QUANTITY)
//        {
//            // начнём с первого входа.
//            nucDiscreteSignalsCounter = 0;
//            // следующий модуль.
//            nucModuleCounter++;
//        }
//    }
//
//    // начнём с первого входа.
//    nucDiscreteSignalsCounter = 0;
//    nucModuleInputCounter = 0;
//    // начнём с первой уставки - LL+L.
//    nucInputSetPointCounter = 0;
//    // начнём с первого модуля.
//    nucModuleCounter = 1;
//
//    // создадим вторую часть стартовой базы данных текстовых реквизитов дискретных сигналов.
//    // для сигналов пораждаемых модулями аналогового ввода.
//    // один аналоговый вход пораждает четыре дискретных сигнала - уставки: LL, L, H, HH.
//    // начнём с создания описателя для уставки LL+L.
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
//            // создадим строку текстового реквизита дискретного сигнала.
//            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
//                    "%s_%02d_%01d_LL",
//                    "Вх. AI",
//                    nucModuleCounter,
//                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
//            // перейдём к заполнению описателя следующей уставки.
//            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
//            break;
//
//        case ANALOGUE_INPUT_SET_POINT_VIOLATION_L:
//            // создадим строку текстового реквизита дискретного сигнала.
//            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
//                    "%s_%02d_%01d_L ",
//                    "Вх. AI",
//                    nucModuleCounter,
//                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
//
//            // перейдём к заполнению описателя следующей уставки.
//            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
//            break;
//
//        case ANALOGUE_INPUT_SET_POINT_VIOLATION_H:
//            // создадим строку текстового реквизита дискретного сигнала.
//            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
//                    "%s_%02d_%01d_H ",
//                    "Вх. AI",
//                    nucModuleCounter,
//                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
//
//            // перейдём к заполнению описателя следующей уставки.
//            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
//            break;
//
//        case ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H:
//            // создадим строку текстового реквизита дискретного сигнала.
//            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
//                    "%s_%02d_%01d_HH",
//                    "Вх. AI",
//                    nucModuleCounter,
//                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
//
//            // перейдём к заполнению описателя следующей уставки.
//            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
//            break;
//
//        default:
//            break;
//        };
//
//        nucDiscreteSignalsCounter++;
//        nucInputSetPointCounter++;
//        // количество уставок по одному входу(4 - LL, L, H, HH).
//        // прошли по всем уставкам одного входа?
//        if (nucInputSetPointCounter >= 4)
//        {
//            nucInputSetPointCounter = 0;
//            // следующий вход.
//            nucModuleInputCounter++;
//        }
//        // следующий описатель.
//        i++;
//
//        // обработаны все дискретные сигналы пораждённые входами модуля аналогового ввода?
//        if (nucDiscreteSignalsCounter == (ANALOG_MODULE_INPUT_QUANTITY *
//                                        ANALOGUE_INPUT_DI_VALUE_QUANTITY))
//        {
//            // начнём с первого входа.
//            nucDiscreteSignalsCounter = 0;
//            nucModuleInputCounter = 0;
//            // начнём с первой уставки - LL+L.
//            nucInputSetPointCounter = 0;
//            // следующий модуль.
//            nucModuleCounter++;
//        }
//    }
//
//    // получим указатель на буфер с вновь созданной нормализованной стартовой базой данных прибора.
//    pxDiscreteSygnalTextDescriptor = axDiscreteSygnalTextDescriptor;
//    // получим указатель на базу данных прибора в общем формате.
//    pxDiscreteSygnalTextDescriptorPackOne = (TDiscreteSygnalTextDescriptorPackOne*)&(xPlcDataBase.
//                                            axPlcDataBaseBlocks[TEXT_TITLES_DATA_BASE_BLOCK_OFFSET].
//                                            aucPlcDataBaseBlockData[0]);
//    nucBlockCounter = 0;
//    nucBlocksInBlockCounter = 0;
//
//    // преобразуем созданную базу данных в общий формат.
//    for (int i = 0;
//            i < MAX_HANDLED_DISCRETE_INPUT;
//            i++)
//    {
//        // обработан весь блок базы данных?
//        if (nucBlocksInBlockCounter == TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
//        {
//            // следующий блок базы данных.
//            nucBlockCounter++;
//            // получим указатель на следующий блок в общей базе данных прибора.
//            pxDiscreteSygnalTextDescriptorPackOne = (TDiscreteSygnalTextDescriptorPackOne*)&(xPlcDataBase.
//                                                    axPlcDataBaseBlocks[TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
//                                                    aucPlcDataBaseBlockData[0]);
//            nucBlocksInBlockCounter = 0;
//        }
//
//        // скопируем один описатель текстовых реквизитов дискретного сигнала, в буфер общего формата.
//        memcpy((&xDiscreteSygnalTextDescriptorPackOne.
//                acTextDescriptor),
//               (&pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
//               DISCRETE_SYGNAL_NAME_LENGTH);
//
//        memcpy((uint8_t*)&pxDiscreteSygnalTextDescriptorPackOne[nucBlocksInBlockCounter],
//               (uint8_t*)&xDiscreteSygnalTextDescriptorPackOne,
//               sizeof(struct TDiscreteSygnalTextDescriptorPackOne));
//
//        // следующий описатель.
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
        std::cout << "CDataBaseCreate::Fsm DISCRETE_SIGNALS_CREATE_DATA_BASE_START"  << std::endl;
        {
            // создадим стартовую базу данных дискретных сигналов в рабочем формате.
            DiscreteSignalsStartDataBaseCreate();
            m_uiBlocksCounter = 0;
            SetFsmState(DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_START);
        }
        break;

    case DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_START:
        std::cout << "CDataBaseCreate::Fsm DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_START"  << std::endl;
        {
            DiscreteSignalsStartDataBlockWorkToCommonFormat(m_puiIntermediateBuff,
                    // получим указатель на блок в буфере с вновь созданной нормализованной стартовой базой данных дискретных сигналов.
                    &(m_pxDiscreteSignalsDescriptionWork[((sizeof(struct TDiscreteSignalsDescriptionWork) *
                                                                             DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY) *
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

