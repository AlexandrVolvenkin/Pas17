

#include "PasNewConfig.h"
#include "FunctionBlocks.h"
#include "Alarm.h"
#include "Events.h"
#include "EventsDB.h"

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
int16_t CEvents::i16EventsLogQueryListQuantity;
CEvents::TOccuredEventsControl CEvents::xOccuredEventsControl;

//-----------------------------------------------------------------------------------------------------
// снимает признак того, что событие уже было.
void CEvents::EventReset(int16_t ui16EventIndex)
{
    // событие по текущему индексу не активно. сбросим флаг состояния(не новое событие).
    pui8EventsControlState[ui16EventIndex] = 0;
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
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[ui16EventIndex];
    // событие новое?
    // или событие фиксируется при каждом появлении?
    // (если установлен флаг - CEvents::EVENT_FIXED_ONCE, то событие фиксируется
    // только если оно перед этим исчезало).
    if ((!(*pui8EventState)) ||
            !((ui8EventTypeSetting) &
              (1 << CEvents::EVENT_FIXED_ONCE)))
    {
        // событие по текущему индексу активно. установим флаг состояния(не новое событие).
        *pui8EventState = 1;
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
    uint8_t ui8EventOffset)
{
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset];
    // событие неактивно?
    if (!(*pui8EventState))
    {
        // событие по текущему индексу неактивно. изменим состояние на - активно.
        *pui8EventState = 1;
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
    uint8_t ui8EventOffset)
{
//    // получим имя класса.
//    sprintf((char*)pcFileName,
//            "%s%s",
//            typeid(*this).name(),
//            ".map");
//    printf("EventOffIsNotRegistered name %s\n\r", (typeid(*this).name()));
//    printf("EventOffIsNotRegistered sizeof %d\n\r", (sizeof(aui8EventsControlState)));
//    printf("EventOffIsNotRegistered sizeof %d\n\r", (sizeof(xCInternalModuleErrorEvent.aui8EventsControlState)));
//
//    printf("EventOffIsNotRegistered ui8EventOffset %d\n\r", (ui8EventOffset));
//    printf("EventOffIsNotRegistered HANDLED_EVENTS_QUANTITY %d\n\r", (HANDLED_EVENTS_QUANTITY));
//    printf("EventOffIsNotRegistered HANDLED_EVENTS_QUANTITY %d\n\r", (xCInternalModuleErrorEvent.HANDLED_EVENTS_QUANTITY));
//    printf("EventOffIsNotRegistered ui16GroupIndex %d\n\r", ((ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset));
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset];
    // событие активно?
    if (*pui8EventState)
    {
        printf("EventOffIsNotRegistered active %d\n\r", (*pui8EventState));
        // событие по текущему индексу активно. изменим состояние на - неактивно.
        *pui8EventState = 0;
        return 1;
    }
    else
    {
        printf("CEvents EventOffIsNotRegistered no active %d\n\r", (*pui8EventState));
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
CEvents::TEventDataCommon* CEvents::RequestedEventSearch(uint8_t ui8EventIndex)
{
    // между отправкой запроса с требуемыми событиями
    // и получением ответа с данными, проходит время - (MAIN_CYCLE_TIME).
    // эта задержка плюс время обработки клавиатуры - причина относительно
    // долгого ожидания вывода на дисплей запрошенных событий.
    // чтобы этого избежать, за один раз запрашивается 4 события.
    // предыдущее, требуемые и следующее.
    // в нормальном режиме на дисплей выводится требуемое событие.
    // если нажата кнопка вперёд - на дисплей выводится следующее событие,
    // если нажата кнопка назад - на дисплей выводится предыдущее событие,
    // из предыдущего ответа на запрос.
    // индекс-адрес требуемого события в следующем запросе смещается на предыдущий.
    // получим индекс-адрес в массиве событий,
    // с которого начинается отображение в скользящем окне.

    // найдём индекс требуемого события,
    // в массиве принятых событий.
    for (uint8_t i = 0;
            i < CEventsDB::EVENTS_DB_QUERY_ROW_QUANTITY;
            i++)
    {
        // в массиве принятых событий есть требуемое?
        if (axEventDataCommon[i].i16Index == ui8EventIndex)
        {
            // в массиве принятых событий найдено требуемое.
            // получим указатель на найденное событие
            // в массиве принятых событий.
            return &axEventDataCommon[i];
            break;
        }
    }
    // в массиве принятых событий нет требуемого.
    // получим указатель на начало пустого места в массиве принятых событий.
    // оно находится во второй половине массива. опустошается при старте.
    return &axEventDataCommon[CEventsDB::EVENTS_DB_QUERY_ROW_QUANTITY];
}

//-----------------------------------------------------------------------------------------------------
// обнуляет количество событий произошедших в одном цикле.
void CEvents::OccuredEventsControlReset(void)
{
    xOccuredEventsControl.ui8OccuredEventsNumber = 0;
}

//-----------------------------------------------------------------------------------------------------
// регистрирует событие.
void CEvents::EventRegistration(
    uint16_t ui16GroupIndex,
    uint8_t ui8EventType,
    uint8_t ui8EventCode,
    char* pcTextDescriptor)
{
    // не превысили максимальное количество событий произошедших в одном цикле?
    if (xOccuredEventsControl.ui8OccuredEventsNumber <
            (ONE_CYCLE_OCCURED_EVENTS_MAXIMUM_NUMBER - CONVERT_NATURAL_NUMBER_TO_INTEGER))
    {
        // указатель на данные произошедшего события.
        TOccuredEventsDataBriefly* pxOccuredEventsDataBriefly;
        // получим указатель на место для сохранения данных произошедшего события.
        pxOccuredEventsDataBriefly =
            &(xOccuredEventsControl.
              axOccuredEventsData[xOccuredEventsControl.ui8OccuredEventsNumber &
                                  (ONE_CYCLE_OCCURED_EVENTS_MAXIMUM_NUMBER - CONVERT_NATURAL_NUMBER_TO_INTEGER)]);
        // сохраним индекс группы породившей событие(например номер модуля).
        pxOccuredEventsDataBriefly -> ui16GroupIndex = ui16GroupIndex;
        // сохраним тип произошедшего события.
        pxOccuredEventsDataBriefly -> ui8EventType = ui8EventType;
        // сохраним состояние произошедшего события.
        pxOccuredEventsDataBriefly -> ui8EventCode = ui8EventCode;
        // сохраним текстовый описатель произошедшего события.
        pxOccuredEventsDataBriefly -> pcTextDescriptor = pcTextDescriptor;
        // увеличим счётчик событий произошедших в одном цикле.
        xOccuredEventsControl.ui8OccuredEventsNumber += 1;
    }
}

//-----------------------------------------------------------------------------------------------------
// интерпретирует событие.
void CEvents::EventsHandler(void)
{
    uint8_t ui8EventsNumber;
    TOccuredEventsDataBriefly* pxOccuredEventsDataBriefly;
    TModuleContext *pxModuleContext;

    // получим количество событий произошедших в одном цикле.
    ui8EventsNumber = xOccuredEventsControl.ui8OccuredEventsNumber;
    // в текущем цикле произошли события?
    if (ui8EventsNumber)
    {
//        printf("EventsHandler ui8EventsNumber %d\n\r", (ui8EventsNumber));
        CEvents::TEventDataCommon *pxEventData;

        // получим указатель на данные события произошедшего в одном цикле.
        pxOccuredEventsDataBriefly = xOccuredEventsControl.axOccuredEventsData;
        // обработаем события произошедшие в одном цикле.
        for (int i = 0;
                i < (ui8EventsNumber);
                i++)
        {
            // получим указатель на контекст модуля.
            pxModuleContext = &xAllModulesContext.axAllModulesContext[pxOccuredEventsDataBriefly -> ui16GroupIndex];
            // какой тип события?
            switch ((pxOccuredEventsDataBriefly -> ui8EventType) &
                    ~(CEvents::HANDLED_EVENTS_IS_POPUP |
                      HANDLED_EVENTS_IS_SOUND |
                      HANDLED_EVENTS_IS_ARCHIVE |
                      HANDLED_EVENTS_IS_OCCURED_ON_START))
            {
            case HANDLED_EVENTS_ALARM_TYPE:
            case HANDLED_EVENTS_DISCRETE_INPUTS_TYPE:
            case HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE:
                xCAlarmEvent.EventsCompleteInformationCreate(pxOccuredEventsDataBriefly);
                break;

                // для журнала ошибок.
            case HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE:
            case HANDLED_EVENTS_EXTERNAL_MODULES_BAD_TYPE:
            case HANDLED_EVENTS_MODULES_EVENTS_TYPE:
            case HANDLED_EVENTS_SYSTEM_EVENTS_TYPE:
            case HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE:
            case HANDLED_EVENTS_SYSTEM_ERROR_TYPE:
                xCInternalModuleErrorEvent.EventsCompleteInformationCreate(pxOccuredEventsDataBriefly);
                break;

            default:
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
    uint8_t ui8EventOffset)
{
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset];
    // событие неактивно?
    if (!(*pui8EventState))
    {
        // событие по текущему индексу неактивно. изменим состояние на - активно.
        *pui8EventState = 1;
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
    uint8_t ui8EventOffset)
{
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset];
    // событие активно?
    if (*pui8EventState)
    {
        // событие по текущему индексу активно. изменим состояние на - неактивно.
        *pui8EventState = 0;
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
    uint8_t ui8EventOffset,
    uint8_t ui8EventCode)
{
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset];
    // событие неактивно?
    if (!(*pui8EventState & ui8EventCode))
    {
//        printf("EventOnIsNotRegistered *pui8EventState %d\n\r", (*pui8EventState));
//        printf("EventOnIsNotRegistered ui8EventCode %d\n\r", (ui8EventCode));
        // событие по текущему индексу неактивно. изменим состояние на - активно.
        *pui8EventState |= ui8EventCode;
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
    uint8_t ui8EventOffset,
    uint8_t ui8EventCode)
{
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset];
    // событие активно?
    if (*pui8EventState & ui8EventCode)
    {
//        printf("EventOffIsNotRegistered *pui8EventState %d\n\r", (*pui8EventState));
//        printf("EventOffIsNotRegistered ui8EventCode %d\n\r", (ui8EventCode));
        // событие по текущему индексу активно. изменим состояние на - неактивно.
        *pui8EventState &= ~ui8EventCode;
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
    TEventDataCommon *pxEventData;
    uint8_t ui8EventType;

    // получим тип события.
    ui8EventType = ((pxOccuredEventsDataBriefly -> ui8EventType) &
                    ~(CEvents::HANDLED_EVENTS_IS_POPUP |
                      HANDLED_EVENTS_IS_SOUND |
                      HANDLED_EVENTS_IS_ARCHIVE |
                      HANDLED_EVENTS_IS_OCCURED_ON_START));

    // тип обрабатываемых событий - события модулей или системные?
    if ((ui8EventType == HANDLED_EVENTS_MODULES_EVENTS_TYPE) ||
            (ui8EventType == HANDLED_EVENTS_SYSTEM_EVENTS_TYPE))
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

    // событие со звуком и выводом на дисплей?
    if (((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_POPUP) &&
            ((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_SOUND))
    {
        // установим флаг звуковой сигнализации.
        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
    }
    // событие без звука и выводом на дисплей?
    else if (((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_POPUP) &&
             !((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_SOUND))
    {
        // установим флаг типа сигнализации и флаг без звука.
        fucZvkMal = (COMMAND_SOUND_SIGNAL_TYPE_ERROR | COMMAND_SOUND_SIGNAL_TYPE_NO_SOUND);
    }
    else
    {
        // сбросим команды типа звука. на МИНД ничего не отправляем.
        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_NOT;
    }

// по указателю(pxEventData) на место в журнале,
// сохраним данные события.
// установим id события.
    (pxEventData -> ui16ID) =
        1;

// установим тип события.
    (pxEventData -> ui8Type) =
        ((pxOccuredEventsDataBriefly -> ui8EventType) &
         ~(CEvents::HANDLED_EVENTS_IS_POPUP |
           HANDLED_EVENTS_IS_SOUND |
           HANDLED_EVENTS_IS_ARCHIVE |
           HANDLED_EVENTS_IS_OCCURED_ON_START));

    if (((pxEventData -> ui8Type) == HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE) ||
            ((pxEventData -> ui8Type) == HANDLED_EVENTS_MODULES_EVENTS_TYPE))
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
    (pxEventData -> ui8State) =
        pxOccuredEventsDataBriefly -> ui8EventCode;
// отказ текущего модуля при старте?
    if ((pxOccuredEventsDataBriefly -> ui8EventType) &
            HANDLED_EVENTS_IS_OCCURED_ON_START)
    {
        // установим код времени события - при старте.
        (pxEventData -> xCurrentTime.tm_sec) = 0xFF;
    }
    else
    {
        // установим время события.
        (pxEventData -> xCurrentTime) =
            xCurrentTime;
    }
// создадим пустую строку в поле дополнительных текстовых данных таблицы архива в базе данных.
    sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
            "%s",
            " ");

// информационное сообщение?
//    if ((pxEventData -> ui8Type) == HANDLED_EVENTS_SYSTEM_ERROR_TYPE)
    if (((pxEventData -> ui8Type) == HANDLED_EVENTS_SYSTEM_ERROR_TYPE) ||
            ((pxEventData -> ui8Type) == HANDLED_EVENTS_SYSTEM_EVENTS_TYPE))
    {
        // создадим текстовый реквизит события.
        sprintf((char*)(pxEventData -> acTextDescriptor),
                "%s",
                pxOccuredEventsDataBriefly -> pcTextDescriptor);
    }
// информационное сообщение?
    else if ((pxEventData -> ui8Type) == HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE)
    {
        // создадим текстовый реквизит события.
        sprintf((char*)(pxEventData -> acTextDescriptor),
                "%s",
                pxOccuredEventsDataBriefly -> pcTextDescriptor);
        // создадим строку данных события.
        // создадим строку с типами модулей текущей конфигурацией.
        sprintf((char*)(pxEventData -> acTextData),
                "  %X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X",
                (xPlcConfigSearch.axConfigSearch[0].ui8Type),
                (xPlcConfigSearch.axConfigSearch[1].ui8Type),
                (xPlcConfigSearch.axConfigSearch[2].ui8Type),
                (xPlcConfigSearch.axConfigSearch[3].ui8Type),
                (xPlcConfigSearch.axConfigSearch[4].ui8Type),
                (xPlcConfigSearch.axConfigSearch[5].ui8Type),
                (xPlcConfigSearch.axConfigSearch[6].ui8Type),
                (xPlcConfigSearch.axConfigSearch[7].ui8Type),
                (xPlcConfigSearch.axConfigSearch[8].ui8Type),
                (xPlcConfigSearch.axConfigSearch[9].ui8Type),
                (xPlcConfigSearch.axConfigSearch[10].ui8Type),
                (xPlcConfigSearch.axConfigSearch[11].ui8Type),
                (xPlcConfigSearch.axConfigSearch[12].ui8Type),
                (xPlcConfigSearch.axConfigSearch[13].ui8Type),
                (xPlcConfigSearch.axConfigSearch[14].ui8Type),
                (xPlcConfigSearch.axConfigSearch[15].ui8Type));
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
                pxOccuredEventsDataBriefly -> ui8EventCode);
    }

// не тип обрабатываемых событий - события модулей?
    if ((ui8EventType != HANDLED_EVENTS_MODULES_EVENTS_TYPE) &&
            (ui8EventType != HANDLED_EVENTS_SYSTEM_EVENTS_TYPE))
    {
        // поместим в журнал ошибок.
        xCPlcErrorEvent.EventDataPointerPush(pxEventData);
    }

// событие архивируется?
    if ((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_ARCHIVE)
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
    uint8_t ui8EventOffset)
{
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset];
    // событие неактивно?
    if (!(*pui8EventState))
    {
        // событие по текущему индексу неактивно. изменим состояние на - активно.
        *pui8EventState = 1;
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
    uint8_t ui8EventOffset)
{
    uint8_t *pui8EventState;
    pui8EventState = &pui8EventsControlState[(ui16GroupIndex * HANDLED_EVENTS_QUANTITY) + ui8EventOffset];
    // событие активно?
    if (*pui8EventState)
    {
        // событие по текущему индексу активно. изменим состояние на - неактивно.
        *pui8EventState = 0;
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
    TAlarmHmi *pxAlarmHmi;
    int i;
    uint8_t ui8EventType;
    uint8_t ui8EventCode;

    // получим указатель на место в кольцевом буфере.
    pxEventData = EventDataPush();

    // получим тип события.
    ui8EventType = ((pxOccuredEventsDataBriefly -> ui8EventType) &
                    ~(CEvents::HANDLED_EVENTS_IS_POPUP |
                      HANDLED_EVENTS_IS_SOUND |
                      HANDLED_EVENTS_IS_ARCHIVE |
                      HANDLED_EVENTS_IS_OCCURED_ON_START));
    // получим состояние события.
    ui8EventCode = pxOccuredEventsDataBriefly -> ui8EventCode;

    // получим индекс события.
    i = (int)(pxOccuredEventsDataBriefly -> ui16GroupIndex);

    // обрабатываем событие функционального блока?
    if (ui8EventType == HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE)
    {
        // сместимся на место событий функциональных блоков.
        i += (xPlcConfigService.xPlcConfigServiceData.
              uiHandledDiscreteInputQuantity);
    }

    // сигнализация аварийная?
    if ((ui8EventCode & (0x01 << ALARM_TYPE_ALARM)) &&
            (!(ui8EventCode & (0x01 << ALARM_TYPE_PREVENT))))
    {
        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ALARM;
        sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
                "%s",
                "Аварийная");
    }
    // сигнализация предупредительная?
    else if  ((!(ui8EventCode & (0x01 << ALARM_TYPE_ALARM))) &&
              (ui8EventCode & (0x01 << ALARM_TYPE_PREVENT)))
    {
        // предупредительная сигнализация.
        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_WARNING;
        sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
                "%s",
                "Предупредительная");
    }
    // сигнализация в норме?
    else if (((!(ui8EventCode & (0x01 << ALARM_TYPE_ALARM))) &&
              (!(ui8EventCode & (0x01 << ALARM_TYPE_PREVENT)))) ||
             (((ui8EventCode & (0x01 << ALARM_TYPE_ALARM))) &&
              ((ui8EventCode & (0x01 << ALARM_TYPE_PREVENT)))))
    {
        // сигнализация в норме.
        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_WARNING;
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

    // событие со звуком и выводом на дисплей?
    if (((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_POPUP) &&
            ((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_SOUND))
    {
        // установим флаг звуковой сигнализации.
    }
    // событие без звука и выводом на дисплей?
    else if (((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_POPUP) &&
             !((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_SOUND))
    {
        // установим флаг без звука.
        fucZvkMal |= COMMAND_SOUND_SIGNAL_TYPE_NO_SOUND;
    }
    else
    {
        // сбросим команды типа звука. на МИНД ничего не отправляем.
        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_NOT;
    }

    // по указателю(pxEventData) на место в журнале,
    // сохраним данные события.
    // установим id события.
    (pxEventData -> ui16ID) =
        1;
    // установим тип события.
    (pxEventData -> ui8Type) =
        ui8EventType;
    // установим адрес источника события.
    (pxEventData -> ui16Address) =
        (i + CONVERT_INTEGER_TO_NATURAL_NUMBER);
    // установим код события.
    (pxEventData -> ui8State) =
        ui8EventCode;
    // установим время события.
    (pxEventData -> xCurrentTime) =
        xCurrentTime;
    // источник события - дискретный вход?
    if (i <
            (xPlcConfigService.xPlcConfigServiceData.
             uiHandledDiscreteInputQuantity))
    {
        // источник события - дискретный вход.
        // скопируем строку текстового реквизита.
        memcpy(&(pxEventData -> acTextDescriptor),
               (&(axDiscreteSygnalTextDescriptor[i].acTextDescriptor)),
               (CAlarm::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
    }
    else
    {
        // источник события - функциональный блок.
        // создадим строку текстового реквизита.
        sprintf((char*)(pxEventData -> acTextDescriptor),
                "%s%d",
                "ФБЛ #",
                ((i - (xPlcConfigService.xPlcConfigServiceData.
                        uiHandledDiscreteInputQuantity)) +
                 CONVERT_INTEGER_TO_NATURAL_NUMBER));
    }

    // событие архивируется?
    if ((pxOccuredEventsDataBriefly -> ui8EventType) & HANDLED_EVENTS_IS_ARCHIVE)
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


