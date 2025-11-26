
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
#include "Resources.h"
#include "DataContainer.h"
#include "Alarm.h"
#include "EventsDB.h"
#include "Events.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
CInternalModuleErrorEvent xCInternalModuleErrorEvent;
//CExternalModuleErrorEvent xCExternalModuleErrorEvent;
//CPlcConfigurationErrorEvent xCPlcConfigurationErrorEvent;
//CPlcDataBaseErrorEvent xCPlcDataBaseErrorEvent;
CAlarmEvent xCAlarmEvent;
CPlcErrorEvent xCPlcErrorEvent;
CPlcArchiveEvent xCPlcArchiveEvent;
CInfoEvent xCInfoEvent;

//-----------------------------------------------------------------------------------------------------
CEvents::TEventsLogQueryListPackOne CEvents::xEventsLogQueryList;
//int16_t CEvents::i16EventsLogQueryListQuantity;
CEvents::TOccuredEventsControl CEvents::xOccuredEventsControl;

//-------------------------------------------------------------------------------
CEvents::CEvents()
{
    std::cout << "CEvents constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[256];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CEvents::~CEvents()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
uint8_t CEvents::Init(void)
{
    std::cout << "CEvents Init"  << std::endl;
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
    SetCustomerDataContainer(GetExecutorDataContainerPointer());

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

    Allocate();
}

//-------------------------------------------------------------------------------
void CEvents::Allocate(void)
{
    std::cout << "CEvents::Allocate 1"  << std::endl;

////    m_uiAddress = xMemoryAllocationContext.uiAddress;
////    m_puiRxBuffer = xMemoryAllocationContext.puiRxBuffer;
////    m_puiTxBuffer = xMemoryAllocationContext.puiTxBuffer;
////    m_puiErrorCode = xMemoryAllocationContext.puiErrorCode;
//
//    // Получим указатель на место в массиве дискретных входов для текущего модуля.
//    m_puiDiscreteInputsState =
//        (GetResources() ->
//         m_puiDiscreteInputsState);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedDiscreteInputsState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//    memset(m_puiDiscreteInputsState, 0, 16);
//
//
//    // Получим указатель на место в массиве достоверности дискретных входов для текущего модуля.
//    m_puiDiscreteInputsBadState =
//        (GetResources() ->
//         m_puiDiscreteInputsBadState);
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


//    // Получим указатель на место в рабочем массиве дискретных сигналов для текущего модуля.
//    m_pxDiscreteSignalsDescriptionWork =
//        &(GetResources() ->
//          m_pxDiscreteSignalsDescriptionWork[GetResources() ->
//                                 m_uiUsedDiscreteSignalsDescriptionWork]);
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

//-----------------------------------------------------------------------------------------------------
// снимает признак того, что событие уже было.
void CEvents::EventReset(int16_t ui16EventIndex)
{
    // событие по текущему индексу не активно. сбросим флаг состояния(не новое событие).
    puiEventsControlState[ui16EventIndex] = 0;
}

//-----------------------------------------------------------------------------------------------------
// возвращает указатель на массив событий..
CEvents::TEventDataCommon*
CEvents::EventGetBuffer(void)
{
    return axEventDataCommon;
}

//-----------------------------------------------------------------------------------------------------
// проверяет, событие уже зафиксировано?
// если нет, то возвращает указатель на свободное место в массиве событий.
CEvents::TEventDataCommon*
CEvents::EventDataPushRequest(int16_t ui16EventIndex)
{
    uint8_t *puiEventState;
    puiEventState = &puiEventsControlState[ui16EventIndex];
    // событие новое?
    // или событие фиксируется при каждом появлении?
    // (если установлен флаг - CEvents::EVENT_FIXED_ONCE, то событие фиксируется
    // только если оно перед этим исчезало).
    if ((!(*puiEventState)) ||
            !((uiEventTypeSetting) &
              (1 << CEvents::EVENT_FIXED_ONCE)))
    {
        // событие по текущему индексу активно. установим флаг состояния(не новое событие).
        *puiEventState = 1;
        // проверим, индекс события вышел за границы массива - журнал событий.
        // если да, то установим индекс события на начало массива - журнал событий.
        // для работы этого механизма, количество сохраняемых событий(ui16RecordedEventsQuantity),
        // должно быть равно степени двойки.
        // не вышли за границы массива? если вышли, обнулим индекс.
        ui16EventDataCommonPushIndex &=
            ui16RecordedEventsQuantity - 1;
        TEventDataCommon *pxEventData;
        pxEventData = &(pxEventDataCommon[ui16EventDataCommonPushIndex]);
        // установим индекс на следующее событие.
        ui16EventDataCommonPushIndex++;
        return pxEventData;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// проверяет, активное состояние события ещё не зарегистрировано?
uint8_t CEvents::EventOnIsNotRegistered(
    uint16_t ui16GroupIndex,
    uint8_t uiEventOffset)
{
    uint8_t *puiEventState;
    puiEventState = &puiEventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + uiEventOffset];
    // событие неактивно?
    if (!(*puiEventState))
    {
        // событие по текущему индексу неактивно. изменим состояние на - активно.
        *puiEventState = 1;
        return 1;
    }
    else
    {
        return 0;
    }
}

#include <typeinfo>
//-----------------------------------------------------------------------------------------------------
// проверяет, неактивное состояние события ещё не зарегистрировано?
uint8_t CEvents::EventOffIsNotRegistered(
    uint16_t ui16GroupIndex,
    uint8_t uiEventOffset)
{
//    // получим имя класса.
//    sprintf((char*)pcFileName,
//            "%s%s",
//            typeid(*this).name(),
//            ".map");
//    printf("EventOffIsNotRegistered name %s\n\r", (typeid(*this).name()));
//    printf("EventOffIsNotRegistered sizeof %d\n\r", (sizeof(auiEventsControlState)));
//    printf("EventOffIsNotRegistered sizeof %d\n\r", (sizeof(xCInternalModuleErrorEvent.auiEventsControlState)));
//
//    printf("EventOffIsNotRegistered uiEventOffset %d\n\r", (uiEventOffset));
//    printf("EventOffIsNotRegistered HANDLED_EVENTS_QUANTITY %d\n\r", (HANDLED_EVENTS_QUANTITY));
//    printf("EventOffIsNotRegistered HANDLED_EVENTS_QUANTITY %d\n\r", (xCInternalModuleErrorEvent.HANDLED_EVENTS_QUANTITY));
//    printf("EventOffIsNotRegistered ui16GroupIndex %d\n\r", ((ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + uiEventOffset));
    uint8_t *puiEventState;
    puiEventState = &puiEventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + uiEventOffset];
    // событие активно?
    if (*puiEventState)
    {
        printf("EventOffIsNotRegistered active %d\n\r", (*puiEventState));
        // событие по текущему индексу активно. изменим состояние на - неактивно.
        *puiEventState = 0;
        return 1;
    }
    else
    {
        printf("CEvents EventOffIsNotRegistered no active %d\n\r", (*puiEventState));
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
CEvents::TEventDataCommon*
CEvents::EventDataPush(void)
{
    // проверим, индекс события вышел за границы массива - журнал событий.
    // если да, то установим индекс события на начало массива - журнал событий.
    // для работы этого механизма, количество сохраняемых событий(ui16RecordedEventsQuantity),
    // должно быть равно степени двойки.
    // не вышли за границы массива? если вышли, обнулим индекс.
    ui16EventDataCommonPushIndex &=
        ui16RecordedEventsQuantity - 1;
    // получим указатель на данные события.
    return &(pxEventDataCommon[ui16EventDataCommonPushIndex++]);
}

//-----------------------------------------------------------------------------------------------------
void CEvents::EventDataPointerPush(CEvents::TEventDataCommon *pxEventDataCommon)
{
    // проверим, индекс события вышел за границы массива - журнал событий.
    // если да, то установим индекс события на начало массива - журнал событий.
    // для работы этого механизма, количество сохраняемых событий(ui16RecordedEventsQuantity),
    // должно быть равно степени двойки.
    // не вышли за границы массива? если вышли, обнулим индекс.
    ui16EventDataCommonPointersPushIndex &= (ui16RecordedEventsQuantity - 1);
    // сохраним указатель на данные зафиксированного события, в массиве указателей на данные события.
    // чтобы потом получать данные событий разного типа, происходящих по порядку.
    ppxEventDataCommonPointers[ui16EventDataCommonPointersPushIndex++] =
        pxEventDataCommon;
}

//-----------------------------------------------------------------------------------------------------
// устанавливает индекс на данные события.
// в регистре (ui16EventDataCommonPushIndex - 1) содержится индекс на данные последнего события.
void CEvents::EventDataPopIndexSet(int8_t i8Length)
{
    ui16EventDataCommonPopIndex = ui16EventDataCommonPushIndex;
    // сместим индекс на требуемое событие.
    ui16EventDataCommonPopIndex -= i8Length;
    // проверим, индекс события вышел за границы массива - журнал событий.
    // если да, то установим индекс события на начало массива - журнал событий.
    // для работы этого механизма, количество сохраняемых событий(ui16RecordedEventsQuantity),
    // должно быть равно степени двойки.
    // не вышли за границы массива? если вышли, обнулим индекс.
    ui16EventDataCommonPopIndex &= (ui16RecordedEventsQuantity - 1);
}

//-----------------------------------------------------------------------------------------------------
// устанавливает индекс на данные события.
// в регистре (ui16EventDataCommonPushIndex - 1) содержится индекс на данные последнего события.
void CEvents::EventDataPointerPopIndexSet(int8_t i8Length)
{
    ui16EventDataCommonPointersPopIndex = ui16EventDataCommonPointersPushIndex;
    // сместим индекс на требуемое событие.
    ui16EventDataCommonPointersPopIndex -= i8Length;
    // проверим, индекс события вышел за границы массива - журнал событий.
    // если да, то установим индекс события на начало массива - журнал событий.
    // для работы этого механизма, количество сохраняемых событий(ui16RecordedEventsQuantity),
    // должно быть равно степени двойки.
    // не вышли за границы массива? если вышли, обнулим индекс.
    ui16EventDataCommonPointersPopIndex &= (ui16RecordedEventsQuantity - 1);
}

//-----------------------------------------------------------------------------------------------------
// извлекает указатель на данные события.
CEvents::TEventDataCommon*
CEvents::EventDataPop(void)
{
    // проверим, индекс события вышел за границы массива - журнал событий.
    // если да, то установим индекс события на начало массива - журнал событий.
    // для работы этого механизма, количество сохраняемых событий(ui16RecordedEventsQuantity),
    // должно быть равно степени двойки.
    // не вышли за границы массива? если вышли, обнулим индекс.
    ui16EventDataCommonPopIndex &= (ui16RecordedEventsQuantity - 1);
    // получим указатель на данные события.
    return &(pxEventDataCommon[ui16EventDataCommonPopIndex--]);
}

//-----------------------------------------------------------------------------------------------------
// извлекает указатель на данные события.
CEvents::TEventDataCommon*
CEvents::EventDataPointerPop(void)
{
    // проверим, индекс события вышел за границы массива - журнал событий.
    // если да, то установим индекс события на начало массива - журнал событий.
    // для работы этого механизма, количество сохраняемых событий(ui16RecordedEventsQuantity),
    // должно быть равно степени двойки.
    // не вышли за границы массива? если вышли, обнулим индекс.
    ui16EventDataCommonPointersPopIndex &= (ui16RecordedEventsQuantity - 1);
    // получим указатель на указатель, на данные события.
    return (ppxEventDataCommonPointers[ui16EventDataCommonPointersPopIndex--]);
}

//-----------------------------------------------------------------------------------------------------
// устанавливает в массиве признак - нет событий.
void CEvents::NoEventsFlagSet(void)
{
    // пометим события маркером(0xFF) - нет события.
    for (int i = 0;
            i < (ui16RecordedEventsQuantity);
            i++)
    {
        pxEventDataCommon[i].ui16ID = 0;
    }
}

//-------------------------------------------------------------------------------------------------
// ищет требуемое событие в массиве данных полученных по запросу.
CEvents::TEventDataCommon* CEvents::RequestedEventSearch(uint8_t uiEventIndex)
{
//    // между отправкой запроса с требуемыми событиями
//    // и получением ответа с данными, проходит время - (MAIN_CYCLE_TIME).
//    // эта задержка плюс время обработки клавиатуры - причина относительно
//    // долгого ожидания вывода на дисплей запрошенных событий.
//    // чтобы этого избежать, за один раз запрашивается 4 события.
//    // предыдущее, требуемые и следующее.
//    // в нормальном режиме на дисплей выводится требуемое событие.
//    // если нажата кнопка вперёд - на дисплей выводится следующее событие,
//    // если нажата кнопка назад - на дисплей выводится предыдущее событие,
//    // из предыдущего ответа на запрос.
//    // индекс-адрес требуемого события в следующем запросе смещается на предыдущий.
//    // получим индекс-адрес в массиве событий,
//    // с которого начинается отображение в скользящем окне.
//
//    // найдём индекс требуемого события,
//    // в массиве принятых событий.
//    for (uint8_t i = 0;
//            i < CEventsDB::EVENTS_DB_QUERY_ROW_QUANTITY;
//            i++)
//    {
//        // в массиве принятых событий есть требуемое?
//        if (axEventDataCommon[i].i16Index == uiEventIndex)
//        {
//            // в массиве принятых событий найдено требуемое.
//            // получим указатель на найденное событие
//            // в массиве принятых событий.
//            return &axEventDataCommon[i];
//            break;
//        }
//    }
//    // в массиве принятых событий нет требуемого.
//    // получим указатель на начало пустого места в массиве принятых событий.
//    // оно находится во второй половине массива. опустошается при старте.
//    return &axEventDataCommon[CEventsDB::EVENTS_DB_QUERY_ROW_QUANTITY];
}

//-----------------------------------------------------------------------------------------------------
// обнуляет количество событий произошедших в одном цикле.
void CEvents::OccuredEventsControlReset(void)
{
    xOccuredEventsControl.uiOccuredEventsNumber = 0;
}

//-----------------------------------------------------------------------------------------------------
// регистрирует событие.
void CEvents::EventRegistration(
    uint16_t ui16GroupIndex,
    uint8_t uiEventType,
    uint8_t uiEventCode,
    char* pcTextDescriptor)
{
    std::cout << "CEvents::EventRegistration 1"  << std::endl;
    // не превысили максимальное количество событий произошедших в одном цикле?
    if (xOccuredEventsControl.uiOccuredEventsNumber <
            (ONE_CYCLE_OCCURED_EVENTS_MAXIMUM_NUMBER - CONVERT_NATURAL_NUMBER_TO_INTEGER))
    {
        std::cout << "CEvents::EventRegistration 2"  << std::endl;
        // указатель на данные произошедшего события.
        TOccuredEventsDataBriefly* pxOccuredEventsDataBriefly;
        // получим указатель на место для сохранения данных произошедшего события.
        pxOccuredEventsDataBriefly =
            &(xOccuredEventsControl.
              axOccuredEventsData[xOccuredEventsControl.uiOccuredEventsNumber &
                                                                               (ONE_CYCLE_OCCURED_EVENTS_MAXIMUM_NUMBER - CONVERT_NATURAL_NUMBER_TO_INTEGER)]);
        // сохраним индекс группы породившей событие(например номер модуля).
        pxOccuredEventsDataBriefly -> ui16GroupIndex = ui16GroupIndex;
        // сохраним тип произошедшего события.
        pxOccuredEventsDataBriefly -> uiEventType = uiEventType;
        // сохраним состояние произошедшего события.
        pxOccuredEventsDataBriefly -> uiEventCode = uiEventCode;
        // сохраним текстовый описатель произошедшего события.
        pxOccuredEventsDataBriefly -> pcTextDescriptor = pcTextDescriptor;
        // увеличим счётчик событий произошедших в одном цикле.
        xOccuredEventsControl.uiOccuredEventsNumber += 1;
    }
}

//-----------------------------------------------------------------------------------------------------
// интерпретирует событие.
void CEvents::EventsHandler(void)
{
    //std::cout << "CEvents::EventsHandler 1"  << std::endl;
    uint8_t uiEventsNumber;
    TOccuredEventsDataBriefly* pxOccuredEventsDataBriefly;
//    TModuleContext *pxModuleContext;

    // получим количество событий произошедших в одном цикле.
    uiEventsNumber = xOccuredEventsControl.uiOccuredEventsNumber;
    // в текущем цикле произошли события?
    if (uiEventsNumber)
    {
        //std::cout << "CEvents::EventsHandler 2"  << std::endl;
        printf("EventsHandler uiEventsNumber %d\n\r", (uiEventsNumber));
        CEvents::TEventDataCommon *pxEventData;

        // получим указатель на данные события произошедшего в одном цикле.
        pxOccuredEventsDataBriefly = xOccuredEventsControl.axOccuredEventsData;
        // обработаем события произошедшие в одном цикле.
        for (int i = 0;
                i < (uiEventsNumber);
                i++)
        {
            //std::cout << "CEvents::EventsHandler 3"  << std::endl;
//            // получим указатель на контекст модуля.
//            pxModuleContext = &xAllModulesContext.axAllModulesContext[pxOccuredEventsDataBriefly -> ui16GroupIndex];
            // какой тип события?
            switch ((pxOccuredEventsDataBriefly -> uiEventType) &
                    ~(CEvents::HANDLED_EVENTS_IS_POPUP |
                      HANDLED_EVENTS_IS_SOUND |
                      HANDLED_EVENTS_IS_ARCHIVE |
                      HANDLED_EVENTS_IS_OCCURED_ON_START))
            {
            case HANDLED_EVENTS_ALARM_TYPE:
            case HANDLED_EVENTS_DISCRETE_INPUTS_TYPE:
            case HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE:
                //std::cout << "CEvents::EventsHandler 4"  << std::endl;
                xCAlarmEvent.EventsCompleteInformationCreate(pxOccuredEventsDataBriefly);
                break;

            // для журнала ошибок.
            case HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE:
            case HANDLED_EVENTS_EXTERNAL_MODULES_BAD_TYPE:
            case HANDLED_EVENTS_MODULES_EVENTS_TYPE:
            case HANDLED_EVENTS_SYSTEM_EVENTS_TYPE:
            case HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE:
            case HANDLED_EVENTS_SYSTEM_ERROR_TYPE:
                //std::cout << "CEvents::EventsHandler 5"  << std::endl;
                xCInternalModuleErrorEvent.EventsCompleteInformationCreate(pxOccuredEventsDataBriefly);
                break;

            default:
                //std::cout << "CEvents::EventsHandler 6"  << std::endl;
                break;
            }
            // следующее событие.
            pxOccuredEventsDataBriefly++;
        }

        // обнулим счётчик событий произошедших в цикле.
        OccuredEventsControlReset();
    }
}

//-----------------------------------------------------------------------------------------------------
// проверяет, активное состояние события ещё не зарегистрировано?
uint8_t CInternalModuleErrorEvent::EventOnIsNotRegistered(
    uint16_t ui16GroupIndex,
    uint8_t uiEventOffset)
{
    uint8_t *puiEventState;
    puiEventState = &puiEventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + uiEventOffset];
    // событие неактивно?
    if (!(*puiEventState))
    {
        // событие по текущему индексу неактивно. изменим состояние на - активно.
        *puiEventState = 1;
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// проверяет, неактивное состояние события ещё не зарегистрировано?
uint8_t CInternalModuleErrorEvent::EventOffIsNotRegistered(
    uint16_t ui16GroupIndex,
    uint8_t uiEventOffset)
{
    uint8_t *puiEventState;
    puiEventState = &puiEventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + uiEventOffset];
    // событие активно?
    if (*puiEventState)
    {
        // событие по текущему индексу активно. изменим состояние на - неактивно.
        *puiEventState = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// проверяет, активное состояние события ещё не зарегистрировано?
uint8_t CInternalModuleErrorEvent::EventOnIsNotRegistered(
    uint16_t ui16GroupIndex,
    uint8_t uiEventOffset,
    uint8_t uiEventCode)
{
    uint8_t *puiEventState;
    puiEventState = &puiEventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + uiEventOffset];
    // событие неактивно?
    if (!(*puiEventState & uiEventCode))
    {
//        printf("EventOnIsNotRegistered *puiEventState %d\n\r", (*puiEventState));
//        printf("EventOnIsNotRegistered uiEventCode %d\n\r", (uiEventCode));
        // событие по текущему индексу неактивно. изменим состояние на - активно.
        *puiEventState |= uiEventCode;
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// проверяет, неактивное состояние события ещё не зарегистрировано?
uint8_t CInternalModuleErrorEvent::EventOffIsNotRegistered(
    uint16_t ui16GroupIndex,
    uint8_t uiEventOffset,
    uint8_t uiEventCode)
{
    uint8_t *puiEventState;
    puiEventState = &puiEventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + uiEventOffset];
    // событие активно?
    if (*puiEventState & uiEventCode)
    {
//        printf("EventOffIsNotRegistered *puiEventState %d\n\r", (*puiEventState));
//        printf("EventOffIsNotRegistered uiEventCode %d\n\r", (uiEventCode));
        // событие по текущему индексу активно. изменим состояние на - неактивно.
        *puiEventState &= ~uiEventCode;
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// создаёт подробный отчёт о событии.
void CInternalModuleErrorEvent::EventsCompleteInformationCreate(TOccuredEventsDataBriefly* pxOccuredEventsDataBriefly)
{
    std::cout << "CInternalModuleErrorEvent::EventsCompleteInformationCreate 1"  << std::endl;
    TEventDataCommon *pxEventData;
    uint8_t uiEventType;

    // получим тип события.
    uiEventType = ((pxOccuredEventsDataBriefly -> uiEventType) &
                    ~(CEvents::HANDLED_EVENTS_IS_POPUP |
                      HANDLED_EVENTS_IS_SOUND |
                      HANDLED_EVENTS_IS_ARCHIVE |
                      HANDLED_EVENTS_IS_OCCURED_ON_START));

    // тип обрабатываемых событий - события модулей или системные?
    if ((uiEventType == HANDLED_EVENTS_MODULES_EVENTS_TYPE) ||
            (uiEventType == HANDLED_EVENTS_SYSTEM_EVENTS_TYPE))
    {
        // получим указатель на место в кольцевом буфере журнала событий.
        pxEventData = xCAlarmEvent.EventDataPush();
    }
    // событие без звука и выводом на дисплей?
    else
    {
        // получим указатель на место в кольцевом буфере журнала ошибок.
        pxEventData = EventDataPush();
    }

//    // событие со звуком и выводом на дисплей?
//    if (((pxOccuredEventsDataBriefly -> uiEventType) & HANDLED_EVENTS_IS_POPUP) &&
//            ((pxOccuredEventsDataBriefly -> uiEventType) & HANDLED_EVENTS_IS_SOUND))
//    {
//        // установим флаг звуковой сигнализации.
//        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//    }
//    // событие без звука и выводом на дисплей?
//    else if (((pxOccuredEventsDataBriefly -> uiEventType) & HANDLED_EVENTS_IS_POPUP) &&
//             !((pxOccuredEventsDataBriefly -> uiEventType) & HANDLED_EVENTS_IS_SOUND))
//    {
//        // установим флаг типа сигнализации и флаг без звука.
//        fucZvkMal = (COMMAND_SOUND_SIGNAL_TYPE_ERROR | COMMAND_SOUND_SIGNAL_TYPE_NO_SOUND);
//    }
//    else
//    {
//        // сбросим команды типа звука. на МИНД ничего не отправляем.
//        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_NOT;
//    }

// по указателю(pxEventData) на место в журнале,
// сохраним данные события.
// установим id события.
    (pxEventData -> ui16ID) =
        1;

// установим тип события.
    (pxEventData -> uiType) =
        ((pxOccuredEventsDataBriefly -> uiEventType) &
         ~(CEvents::HANDLED_EVENTS_IS_POPUP |
           HANDLED_EVENTS_IS_SOUND |
           HANDLED_EVENTS_IS_ARCHIVE |
           HANDLED_EVENTS_IS_OCCURED_ON_START));

    if (((pxEventData -> uiType) == HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE) ||
            ((pxEventData -> uiType) == HANDLED_EVENTS_MODULES_EVENTS_TYPE))
    {
        // установим адрес источника события.
        (pxEventData -> ui16Address) =
            ((pxOccuredEventsDataBriefly -> ui16GroupIndex) + CONVERT_INTEGER_TO_NATURAL_NUMBER);
    }
    else
    {
        // установим адрес источника события.
        (pxEventData -> ui16Address) =
            pxOccuredEventsDataBriefly -> ui16GroupIndex;
    }

// установим код события.
    (pxEventData -> uiState) =
        pxOccuredEventsDataBriefly -> uiEventCode;
// отказ текущего модуля при старте?
    if ((pxOccuredEventsDataBriefly -> uiEventType) &
            HANDLED_EVENTS_IS_OCCURED_ON_START)
    {
        // Получаем текущее время
        time_t now = time(nullptr);
        // Получаем текущую дату
        // установим время события.
        (pxEventData -> xCurrentTime) = *gmtime(&now);
//        // установим код времени события - при старте.
//        (pxEventData -> xCurrentTime.tm_sec) = 0xFF;
    }
    else
    {
        // Получаем текущее время
        time_t now = time(nullptr);
        // Получаем текущую дату
        // установим время события.
        (pxEventData -> xCurrentTime) = *gmtime(&now);
    }
// создадим пустую строку в поле дополнительных текстовых данных таблицы архива в базе данных.
    sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
            "%s",
            " ");

// информационное сообщение?
//    if ((pxEventData -> uiType) == HANDLED_EVENTS_SYSTEM_ERROR_TYPE)
    if (((pxEventData -> uiType) == HANDLED_EVENTS_SYSTEM_ERROR_TYPE) ||
            ((pxEventData -> uiType) == HANDLED_EVENTS_SYSTEM_EVENTS_TYPE))
    {
        // создадим текстовый реквизит события.
        sprintf((char*)(pxEventData -> acTextDescriptor),
                "%s",
                pxOccuredEventsDataBriefly -> pcTextDescriptor);
    }
// информационное сообщение?
    else if ((pxEventData -> uiType) == HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE)
    {
        // создадим текстовый реквизит события.
        sprintf((char*)(pxEventData -> acTextDescriptor),
                "%s",
                pxOccuredEventsDataBriefly -> pcTextDescriptor);
//        // создадим строку данных события.
//        // создадим строку с типами модулей текущей конфигурацией.
//        sprintf((char*)(pxEventData -> acTextData),
//                "  %X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X",
//                (xPlcConfigSearch.axConfigSearch[0].uiType),
//                (xPlcConfigSearch.axConfigSearch[1].uiType),
//                (xPlcConfigSearch.axConfigSearch[2].uiType),
//                (xPlcConfigSearch.axConfigSearch[3].uiType),
//                (xPlcConfigSearch.axConfigSearch[4].uiType),
//                (xPlcConfigSearch.axConfigSearch[5].uiType),
//                (xPlcConfigSearch.axConfigSearch[6].uiType),
//                (xPlcConfigSearch.axConfigSearch[7].uiType),
//                (xPlcConfigSearch.axConfigSearch[8].uiType),
//                (xPlcConfigSearch.axConfigSearch[9].uiType),
//                (xPlcConfigSearch.axConfigSearch[10].uiType),
//                (xPlcConfigSearch.axConfigSearch[11].uiType),
//                (xPlcConfigSearch.axConfigSearch[12].uiType),
//                (xPlcConfigSearch.axConfigSearch[13].uiType),
//                (xPlcConfigSearch.axConfigSearch[14].uiType),
//                (xPlcConfigSearch.axConfigSearch[15].uiType));
    }
    else
    {
        // создадим текстовый реквизит события.
        sprintf((char*)(pxEventData -> acTextDescriptor),
                "%s %s%02d %s%02X",
                pxOccuredEventsDataBriefly -> pcTextDescriptor,
                "#",
                (pxEventData -> ui16Address),
                "E",
                pxOccuredEventsDataBriefly -> uiEventCode);
    }

// не тип обрабатываемых событий - события модулей?
    if ((uiEventType != HANDLED_EVENTS_MODULES_EVENTS_TYPE) &&
            (uiEventType != HANDLED_EVENTS_SYSTEM_EVENTS_TYPE))
    {
        // поместим в журнал ошибок.
        xCPlcErrorEvent.EventDataPointerPush(pxEventData);
    }

// событие архивируется?
    if ((pxOccuredEventsDataBriefly -> uiEventType) & HANDLED_EVENTS_IS_ARCHIVE)
    {
        // архивируем событие.
        xCArchiveEventsDB.DataBaseDataPush(pxEventData);
    }

// модуль индикации МИНД последовательно запрашивает данные событий из кольцевого буфера.
// когда МИНД получает данные события с маркером - нет события, он перестаёт посылать
// запросы. на дисплее появляется информация о том, что событий больше нет.
// чтобы сообщить модулю индикации МИНД о том, что событий больше нет - вытесним из
// кольцевого буфера самое давнее событие. пометим его маркером - нет события(ui16ID = 0).
// установим индекс (ui16EventDataCommonPopIndex) на место следующего в кольцевом буфере самого давнего события.
    EventDataPopIndexSet(0);
// получим указатель на место следующего в кольцевом буфере самого давнего события.
    pxEventData = EventDataPop();
// пометим событие маркером - нет события.
    pxEventData -> ui16ID = 0;
}




//-----------------------------------------------------------------------------------------------------
// проверяет, активное состояние события ещё не зарегистрировано?
uint8_t CAlarmEvent::EventOnIsNotRegistered(
    uint16_t ui16GroupIndex,
    uint8_t uiEventOffset)
{
    uint8_t *puiEventState;
    puiEventState = &puiEventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + uiEventOffset];
    // событие неактивно?
    if (!(*puiEventState))
    {
        // событие по текущему индексу неактивно. изменим состояние на - активно.
        *puiEventState = 1;
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// проверяет, неактивное состояние события ещё не зарегистрировано?
uint8_t CAlarmEvent::EventOffIsNotRegistered(
    uint16_t ui16GroupIndex,
    uint8_t uiEventOffset)
{
    uint8_t *puiEventState;
    puiEventState = &puiEventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + uiEventOffset];
    // событие активно?
    if (*puiEventState)
    {
        // событие по текущему индексу активно. изменим состояние на - неактивно.
        *puiEventState = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// создаёт подробный отчёт о событии.
void CAlarmEvent::EventsCompleteInformationCreate(TOccuredEventsDataBriefly* pxOccuredEventsDataBriefly)
{
    TEventDataCommon *pxEventData;
//    TAlarmHmi *pxAlarmHmi;
    int i;
    uint8_t uiEventType;
    uint8_t uiEventCode;
//    // получим указатель на объект конфигурации.
//    TConfigDataPackOne* pxDeviceConfigSearch =
//        (GetResources() -> GetDeviceConfigSearchPointer());

    // получим указатель на место в кольцевом буфере.
    pxEventData = EventDataPush();

    // получим тип события.
    uiEventType = ((pxOccuredEventsDataBriefly -> uiEventType) &
                    ~(CEvents::HANDLED_EVENTS_IS_POPUP |
                      HANDLED_EVENTS_IS_SOUND |
                      HANDLED_EVENTS_IS_ARCHIVE |
                      HANDLED_EVENTS_IS_OCCURED_ON_START));
    // получим состояние события.
    uiEventCode = pxOccuredEventsDataBriefly -> uiEventCode;

    // получим индекс события.
    i = (int)(pxOccuredEventsDataBriefly -> ui16GroupIndex);

//    // обрабатываем событие функционального блока?
//    if (uiEventType == HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE)
//    {
//        // сместимся на место событий функциональных блоков.
//        i += (xPlcConfigService.xPlcConfigServiceData.
//              uiHandledDiscreteInputQuantity);
//    }

    // сигнализация аварийная?
    if (uiEventCode == EMERGENCY)
    {
//        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ALARM;
        sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
                "%s",
                "Аварийная");
    }
    // сигнализация предупредительная?
    else if (uiEventCode == PREVENTIVE)
    {
        // предупредительная сигнализация.
//        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_WARNING;
        sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
                "%s",
                "Предупредительная");
    }
    // сигнализация в норме?
    else if (uiEventCode == NORMAL)
    {
        // сигнализация в норме.
//        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_WARNING;
        sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
                "%s",
                "Норма");
    }
//    else
//    {
//        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ALARM;
//    // создадим пустую строку в поле дополнительных текстовых данных таблицы архива в базе данных.
//        sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
//                "%s",
//                "Аварийная");
//    }

//    // событие со звуком и выводом на дисплей?
//    if (((pxOccuredEventsDataBriefly -> uiEventType) & HANDLED_EVENTS_IS_POPUP) &&
//            ((pxOccuredEventsDataBriefly -> uiEventType) & HANDLED_EVENTS_IS_SOUND))
//    {
//        // установим флаг звуковой сигнализации.
//    }
//    // событие без звука и выводом на дисплей?
//    else if (((pxOccuredEventsDataBriefly -> uiEventType) & HANDLED_EVENTS_IS_POPUP) &&
//             !((pxOccuredEventsDataBriefly -> uiEventType) & HANDLED_EVENTS_IS_SOUND))
//    {
//        // установим флаг без звука.
//        fucZvkMal |= COMMAND_SOUND_SIGNAL_TYPE_NO_SOUND;
//    }
//    else
//    {
//        // сбросим команды типа звука. на МИНД ничего не отправляем.
//        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_NOT;
//    }

    // по указателю(pxEventData) на место в журнале,
    // сохраним данные события.
    // установим id события.
    (pxEventData -> ui16ID) =
        1;
    // установим тип события.
    (pxEventData -> uiType) =
        uiEventType;
    // установим адрес источника события.
    (pxEventData -> ui16Address) =
        (i + CONVERT_INTEGER_TO_NATURAL_NUMBER);
    // установим код события.
    (pxEventData -> uiState) =
        uiEventCode;
    // Получаем текущее время
    time_t now = time(nullptr);
    // Получаем текущую дату
    // установим время события.
    (pxEventData -> xCurrentTime) = *gmtime(&now);
//    // источник события - дискретный вход?
//    if (i < (pxDeviceConfigSearch ->
//             uiHandledDiscreteSignalsQuantity))
//    {
        // источник события - дискретный вход.
        // скопируем строку текстового реквизита.
        memcpy(&(pxEventData -> acTextDescriptor),
               (&(m_pxDiscreteSygnalTextTitlesWork[i].acTextDescriptor)),
               (DISCRETE_SYGNAL_NAME_LENGTH + END_OF_STRING_LENGTH));
//    }
//    else
//    {
//        // источник события - функциональный блок.
//        // создадим строку текстового реквизита.
//        sprintf((char*)(pxEventData -> acTextDescriptor),
//                "%s%d",
//                "ФБЛ #",
//                ((i - (xPlcConfigService.xPlcConfigServiceData.
//                       uiHandledDiscreteInputQuantity)) +
//                 CONVERT_INTEGER_TO_NATURAL_NUMBER));
//    }

    // событие архивируется?
    if ((pxOccuredEventsDataBriefly -> uiEventType) & HANDLED_EVENTS_IS_ARCHIVE)
    {
        // архивируем событие.
        xCArchiveEventsDB.DataBaseDataPush(pxEventData);
    }

    // модуль индикации МИНД последовательно запрашивает данные событий из кольцевого буфера.
    // когда МИНД получает данные события с маркером - нет события, он перестаёт посылать
    // запросы. на дисплее появляется информация о том, что событий больше нет.
    // чтобы сообщить модулю индикации МИНД о том, что событий больше нет - вытесним из
    // кольцевого буфера самое давнее событие. пометим его маркером - нет события(ui16ID = 0).
    // установим индекс (ui16EventDataCommonPopIndex) на место следующего в кольцевом буфере самого давнего события.
    EventDataPopIndexSet(0);
    // получим указатель на место следующего в кольцевом буфере самого давнего события.
    pxEventData = EventDataPop();
    // пометим событие маркером - нет события.
    pxEventData -> ui16ID = 0;
}


