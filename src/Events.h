#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include "Configuration.h"
//#include "EventsDB.h"

class CEventsDB;
//class CAlarm;
class CAlarmDfa;

//-----------------------------------------------------------------------------------------------------
class CEvents
{
public:
    typedef enum
    {
        // длина строки содержащей имя события(текстовый реквизит).
        EVENT_NAME_LENGTH = 20,
        // длина строки содержащей имя события(текстовый реквизит).
        EVENT_TEXT_DATA_LENGTH = 20,
        // количество запрашиваемых МИНД событий.
        REQUESTED_EVENTS_QUANTITY = 4,
        // длина списка запросов событий.
        EVENT_EVENT_QUERY_LIST_LENGTH = 4,

//-----------------------------------------------------------------------------------------------------
        // тип обрабатываемых событий дискретных сигналов.
        REQUESTED_EVENTS_ALARM_TYPE = 1,
        // тип обрабатываемых событий - ошибки.
        REQUESTED_EVENTS_ERROR_TYPE = 2,
        // тип обрабатываемых событий - архив.
        REQUESTED_EVENTS_ARCHIVE_TYPE = 3,
        // тип обрабатываемых событий - аналоговые входы.
        REQUESTED_EVENTS_ANALOGUE_TYPE = 4,
        // тип обрабатываемых событий - дискретные выходы.
        REQUESTED_EVENTS_DISCRETE_TYPE = 5,
        // тип обрабатываемых событий - информация о приборе.
        REQUESTED_EVENTS_INFO_TYPE = 6,

        // тип обрабатываемых событий - аналоговые входы.
        HANDLED_EVENTS_ANALOGUE_TYPE = 1,
        // тип обрабатываемых событий дискретных сигналов.
        HANDLED_EVENTS_ALARM_TYPE = 2,
        // тип обрабатываемых событий дискретных входов.
        HANDLED_EVENTS_DISCRETE_INPUTS_TYPE = 3,
        // тип обрабатываемых событий функциональных блоков.
        HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE = 4,
        // тип обрабатываемых событий отказов внешних модулей.
        HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE = 5,
        // тип обрабатываемых событий отказов внешних модулей.
        HANDLED_EVENTS_EXTERNAL_MODULES_BAD_TYPE = 6,
        // тип обрабатываемых событий ошибка конфигурации.
        HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE = 7,
        // тип обрабатываемых событий ошибка баз данных.
        HANDLED_EVENTS_SYSTEM_ERROR_TYPE = 8,
        // тип обрабатываемых событий - информация о приборе.
        HANDLED_EVENTS_INFO_TYPE = 9,
        // тип обрабатываемых событий - информация о приборе с временем.
        HANDLED_EVENTS_INFO_TIME_TYPE = 10,
        // тип обрабатываемых событий - информационное сообщение.
        HANDLED_EVENTS_SYSTEM_EVENTS_TYPE = 11,
        // тип обрабатываемых событий - события модулей.
        HANDLED_EVENTS_MODULES_EVENTS_TYPE = 12,

        // порядок представления события - по порядку.
        REQUESTED_EVENTS_ORDER_DEFAULT = 0,
        // порядок представления события - по номеру.
        REQUESTED_EVENTS_ORDER_BY_NUMBER = 1,
        // порядок представления события - по дате.
        REQUESTED_EVENTS_ORDER_BY_DATE = 2,

        // количество обрабатываемых событий дискретных сигналов.
        HANDLED_EVENTS_ALARM_QUANTITY = (MAX_HANDLED_DISCRETE_INPUT +
                                         MAX_HANDLED_FUNCTION_BLOCK),
        // количество обрабатываемых событий - ошибки.
        HANDLED_EVENTS_ERROR_QUANTITY = (INTERNAL_MODULE_QUANTITY +
                                         EXTERNAL_MODULE_QUANTITY),
        // количество обрабатываемых событий - архив.
        HANDLED_EVENTS_ARCHIVE_QUANTITY = 64,

//-----------------------------------------------------------------------------------------------------
        // события записываются в кольцевой буфер.
        // (Index &= Quantity - 1).
        // bufer[Index++] = Data;
        // для работы этого механизма, количество фиксируемых событий(ui16RecordedEventsQuantity),
        // должно быть равно степени двойки.
        // количество фиксируемых событий дискретных сигналов.
        RECORDED_EVENTS_ALARM_QUANTITY = 8,
        // количество фиксируемых событий - ошибки.
        RECORDED_EVENTS_ERROR_QUANTITY = 8,
        // количество обрабатываемых событий - архив.
        RECORDED_EVENTS_ARCHIVE_QUANTITY = 8,

//-----------------------------------------------------------------------------------------------------
        // количество обрабатываемых событий.
        HANDLED_EVENTS_QUANTITY = (HANDLED_EVENTS_ALARM_QUANTITY +
                                   HANDLED_EVENTS_ERROR_QUANTITY +
                                   HANDLED_EVENTS_ARCHIVE_QUANTITY),
        // количество фиксируемых событий.
        RECORDED_EVENTS_QUANTITY = 32,
        // максимальное количество событий произошедших в одном цикле.
        ONE_CYCLE_OCCURED_EVENTS_MAXIMUM_NUMBER = 16,
        // тип группы породившей событие.
        CREATED_EVENT_GROUP_TYPE_DISCRETE_INPUT_MODULE = 1,
        CREATED_EVENT_GROUP_TYPE_ANALOGUE_INPUT_MODULE = 2,
        CREATED_EVENT_GROUP_TYPE_DISCRETE_OUTPUT_MODULE = 3,
        CREATED_EVENT_GROUP_TYPE_ANALOGUE_OUTPUT_MODULE = 4,
        // флаг - событие архивируется.
        HANDLED_EVENTS_IS_ARCHIVE = 0x80,
        // флаг - событие со звуком.
        HANDLED_EVENTS_IS_SOUND = 0x40,
        // флаг - событие выводится на дисплей.
        HANDLED_EVENTS_IS_POPUP = 0x20,
//        HANDLED_EVENTS_NO_POPUP_NO_SOUND_NO_ARCHIVE = 0x00,
//        HANDLED_EVENTS_POPUP_SOUND_ARCHIVE = 0x40,
//        HANDLED_EVENTS_POPUP_NO_SOUND_NO_ARCHIVE = 0x40,
//        HANDLED_EVENTS_POPUP_SOUND_ARCHIVE = 0x40,
        // флаг - событие произошло при старте.
        HANDLED_EVENTS_IS_OCCURED_ON_START = 0x10,
    };

//-----------------------------------------------------------------------------------------------------
    // флаги установок события.
    typedef enum
    {
        // флаг.
        // если - 0, событие фиксируется при каждом появлении.
        // если - 1, событие фиксируется после пропадания и нового появления.
        EVENT_FIXED_ONCE = 0
    };

#pragma pack(push)
#pragma pack(1)
// структура данных события.
    struct TEventDataPackOne
    {
        // ID в таблице базы данных.
        uint16_t ui16ID;
        // тип запрашиваемых событий.
        uint8_t ui8Type;
        // индекс события.
        int16_t i16Index;
        union
        {
            // числовые данные события.
            struct
            {
                // адрес источника события.
                uint16_t ui16Address;
                // состояние события.
                uint8_t ui8State;
                // время события.
                uint8_t ui8Second;
                uint8_t ui8Minute;
                uint8_t ui8Hour;
                uint8_t ui8MonthDay;
                uint8_t ui8Month;
                uint8_t ui8Year;
            };
            // текстовые данные события.
            char acTextData[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];

            struct
            {
                // адрес первого запрашиваемого аналогового измерения.
                // адрес сигнала (№ входа, адрес в
                // (измеренные значения входных аналоговых сигналов - AI_VALUE_BYTE_ARRAY_OFFSET)
                uint8_t ui8Address;
                // состояние канала аналогового измерения.
                uint8_t ui8Status;
                // значение аналогового измерения.
                float fData;
                // Текстовый реквизит размерности измеряемого параметра КРЗМ.
                char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
            };
        };
        // текстовое описание события.
        char acTextDescriptor[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];
    };

// структура динамически изменяемых данных события.
    struct TEventDataDinamicPackOne
    {
        // ID в таблице базы данных.
        uint16_t ui16ID;
//        // тип запрашиваемых событий.
//        uint8_t ui8Type;
        // индекс события.
        int16_t i16Index;
        union
        {
            struct
            {
                //                // адрес первого запрашиваемого аналогового измерения.
//                // адрес сигнала (№ входа, адрес в
//                // (измеренные значения входных аналоговых сигналов - AI_VALUE_BYTE_ARRAY_OFFSET)
//                uint8_t ui8Address;
                // состояние канала аналогового измерения.
                uint8_t ui8Status;
                // значение аналогового измерения.
                float fData;
            };
        };
    };
#pragma pack(pop)

// структура данных события.
    struct TEventData
    {
        // индекс события.
        uint16_t ui16Index;
        // адрес источника события.
        uint16_t ui16Address;
        // состояние события.
        uint8_t ui8State;
        // текстовый реквизит источника события.
        char acTextDescriptor[CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH];
        // время события.
        struct tm xCurrentTime;
    };

    // структура данных события общего типа.
    struct TEventDataCommon
    {
        // ID в таблице базы данных.
        uint16_t ui16ID;
        // тип запрашиваемых событий.
        uint8_t ui8Type;
        // индекс события.
        int16_t i16Index;
        union
        {
            // числовые данные события.
            struct
            {
                // адрес источника события.
                uint16_t ui16Address;
                // состояние события.
                uint8_t ui8State;
                // время события.
                struct tm xCurrentTime;
            };
            // текстовые данные события.
            char acTextData[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];

            struct
            {
                // адрес первого запрашиваемого аналогового измерения.
                // адрес сигнала (№ входа, адрес в
                // (измеренные значения входных аналоговых сигналов - AI_VALUE_BYTE_ARRAY_OFFSET)
                uint8_t ui8Address;
                // состояние канала аналогового измерения.
                uint8_t ui8Status;
                // значение аналогового измерения.
                float fData;
                // Текстовый реквизит размерности измеряемого параметра КРЗМ.
                char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
            };
        };
        // текстовое описание события.
        char acTextDescriptor[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];
        // дополнительное текстовое описание события.
        char acTextDescriptorAdditional[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];
    };

#pragma pack(push)
#pragma pack(1)
    // структура данных списка запросов журнала событий.
    struct TEventsLogQueryListPackOne
    {
        // индекс первого запрашиваемого события в массиве(журнала событий).
        int16_t i16Index;
        // тип запрашиваемых событий.
        uint8_t ui8Type;
        // порядок представления события.
        uint8_t ui8Order;
        // адрес источника события.
        union
        {
            uint8_t ui8Address;
            uint16_t ui16Address;
            struct
            {
                // время  первого запрашиваемого события.
//        uint8_t ui8Second;
//        uint8_t ui8Minute;
//        uint8_t ui8Hour;
                uint8_t ui8MonthDay;
                uint8_t ui8Month;
                uint8_t ui8Year;
            };
        };
        // количество запрашиваемых событий.
        union
        {
            uint8_t ui8Quantity;
            uint8_t ui8Length;
        };
    };
#pragma pack(pop)

// структура данных произошедшего события.
    struct TOccuredEventsData
    {
        // тип группы породившей событие(например номер модуля).
        uint8_t ui8GroupType;
        // индекс группы породившей событие(например номер модуля).
        uint16_t ui16GroupIndex;
        // тип произошедшего события.
        uint8_t ui8EventType;
        // код произошедшего события.
        uint8_t ui8EventCode;
        // состояние произошедшего события(1 - активно, 0 - переход в норму).
        uint8_t ui8State;
        // указатель на текстовое описание события.
        char* pcTextDescriptor;
    };

// структура данных произошедшего события.
    struct TOccuredEventsDataBriefly
    {
        // индекс группы породившей событие(например номер модуля).
        uint16_t ui16GroupIndex;
        // тип произошедшего события.
        uint8_t ui8EventType;
        // состояние произошедшего события.
        uint8_t ui8EventCode;
        // указатель на текстовое описание события.
        char* pcTextDescriptor;
    };

// структура управления произошедшими событиями.
    struct TOccuredEventsControl
    {
        // количество событий произошедших в одном цикле.
        uint8_t ui8OccuredEventsNumber;
        // массив с данными событий произошедших в одном цикле.
        TOccuredEventsDataBriefly axOccuredEventsData[ONE_CYCLE_OCCURED_EVENTS_MAXIMUM_NUMBER];
    };

//-----------------------------------------------------------------------------------------------------
    void EventReset(int16_t );
    CEvents::TEventDataCommon*
    EventGetBuffer(void);
    CEvents::TEventDataCommon*
    EventDataPushRequest(int16_t );
    virtual CEvents::TEventDataCommon*
    EventDataPush(void);
    void EventDataPointerPush(CEvents::TEventDataCommon * );
    virtual void EventDataPopIndexSet(int8_t );
    void EventDataPointerPopIndexSet(int8_t );
    virtual CEvents::TEventDataCommon*
    EventDataPop(void);
    CEvents::TEventDataCommon*
    EventDataPointerPop(void);
    void NoEventsFlagSet(void);
    CEvents::TEventDataCommon* RequestedEventSearch(uint8_t );
    // обнуляет количество событий произошедших в одном цикле.
    static void OccuredEventsControlReset(void);
    uint8_t EventOnIsNotRegistered(uint16_t , uint8_t );
    uint8_t EventOffIsNotRegistered(uint16_t , uint8_t );
    static void EventRegistration(uint16_t , uint8_t , uint8_t , char* );
    static void EventsHandler(void);

//-----------------------------------------------------------------------------------------------------
//    // количество обрабатываемых событий одного типа.
//    int16_t ui16HandledEventsQuantity;
    // количество фиксируемых событий одного типа.
    int16_t ui16RecordedEventsQuantity;
    // список запросов событий.
    static CEvents::TEventsLogQueryListPackOne xEventsLogQueryList;
//    // количество событий в запросе.
//    static int16_t i16EventsLogQueryListQuantity;

    // массив состояния событий(если - 0, событие новое. если - 1, событие не новое.);
    uint8_t aui8EventsControlState[HANDLED_EVENTS_QUANTITY];
    // указатель на массив состояния событий.
    uint8_t *pui8EventsControlState;
    // установки типа события.
    uint8_t ui8EventTypeSetting;
    // массив даных событий.
    CEvents::TEventDataCommon axEventDataCommon[RECORDED_EVENTS_QUANTITY];
    // указатель на массив даных событий.
    TEventDataCommon *pxEventDataCommon;
    // индекс в массиве событий.
    int16_t ui16EventDataCommonPushIndex;
    int16_t ui16EventDataCommonPopIndex;
    // массив указателей на события в массиве даных событий.
    // в нём находятся указатели на события в массиве даных событий(axEventDataCommon[CEvents::RECORDED_EVENTS_QUANTITY]),
    // на последовательно поступающие события разных типов.
    // по ним из массива даных событий(axEventDataCommon[CEvents::RECORDED_EVENTS_QUANTITY]),
    // будем получать все последовательно поступившие события разных типов.
////    static CEvents::TEventDataCommon* apxEventDataCommonPointers[RECORDED_EVENTS_QUANTITY];
    // указатель на массив индексов событий в массиве даных событий.
    CEvents::TEventDataCommon** ppxEventDataCommonPointers;
    // индекс последовательно поступившего события разных типов.
    // в массиве индексов событий(apxEventDataCommonPointers[CEvents::RECORDED_EVENTS_QUANTITY]).
    int16_t ui16EventDataCommonPointersPushIndex;
    int16_t ui16EventDataCommonPointersPopIndex;
    // объект управления произошедшими событиями.
    static TOccuredEventsControl xOccuredEventsControl;
};

//-----------------------------------------------------------------------------------------------------
class CEventsGroup
{
public:
    typedef enum
    {

    };
};

//-----------------------------------------------------------------------------------------------------
class CInternalModuleErrorEvent : public CEvents
{
public:
    typedef enum
    {
//-----------------------------------------------------------------------------------------------------
        // количество обрабатываемых событий в одной группе.
        HANDLED_EVENTS_QUANTITY = 16,
        // количество фиксируемых событий.
        RECORDED_EVENTS_QUANTITY = 16,
    };

    // структура управления событиями одного модуля.
    struct TOneGroupEventsControl
    {
        // массив состояния типов событий(если - 0, событие новое. если - 1, событие не новое.);
        uint8_t aui8EventsTypesControlState[HANDLED_EVENTS_QUANTITY];
    };

//-----------------------------------------------------------------------------------------------------
//private:
    // массив состояния событий.
    uint8_t aui8EventsControlState[MAX_MODULE_QUANTITY * HANDLED_EVENTS_QUANTITY];
    TOneGroupEventsControl axAllGroupsEventsControl[MAX_MODULE_QUANTITY];
    // массив даных событий.
    CEvents::TEventDataCommon axEventDataCommon[RECORDED_EVENTS_QUANTITY];

//-----------------------------------------------------------------------------------------------------
public:
    uint8_t EventOnIsNotRegistered(uint16_t , uint8_t );
    uint8_t EventOffIsNotRegistered(uint16_t , uint8_t );
    uint8_t EventOnIsNotRegistered(uint16_t , uint8_t , uint8_t );
    uint8_t EventOffIsNotRegistered(uint16_t , uint8_t , uint8_t );
    void EventsCompleteInformationCreate(TOccuredEventsDataBriefly* );

//-----------------------------------------------------------------------------------------------------
public:
    CInternalModuleErrorEvent()
    {
        ui16RecordedEventsQuantity = RECORDED_EVENTS_QUANTITY;
        pui8EventsControlState = aui8EventsControlState;
        pxEventDataCommon = axEventDataCommon;
        ui16EventDataCommonPushIndex = 0;
        ui8EventTypeSetting = (1 << CEvents::EVENT_FIXED_ONCE);
        // пометим события маркером - нет события.
        for (int i = 0;
                i < (ui16RecordedEventsQuantity);
                i++)
        {
            pxEventDataCommon[i].ui16ID = 0;
        }
        // обнулим массив регистрации состояния события.
        for (int i = 0;
                i < (MAX_MODULE_QUANTITY * HANDLED_EVENTS_QUANTITY);
                i++)
        {
            aui8EventsControlState[i] = 0;
        }

//        for (int i = 0;
//                i < (MAX_MODULE_QUANTITY);
//                i++)
//        {
//            for (int j = 0;
//                    j < (HANDLED_EVENTS_QUANTITY);
//                    j++)
//            {
//                axAllGroupsEventsControl[i].aui8EventsTypesControlState[j] = 0;
//            }
//        }
    }
};

//-----------------------------------------------------------------------------------------------------
class CExternalModuleErrorEvent : public CEvents
{
public:
    typedef enum
    {
//-----------------------------------------------------------------------------------------------------
        // количество обрабатываемых событий.
        HANDLED_EVENTS_QUANTITY = EXTERNAL_MODULE_QUANTITY,
        // количество фиксируемых событий.
        RECORDED_EVENTS_QUANTITY = 8,
    };

//-----------------------------------------------------------------------------------------------------
private:
    // массив состояния событий(если - 0, событие новое. если - 1, событие не новое.);
    uint8_t aui8EventsControlState[HANDLED_EVENTS_QUANTITY];
    // массив даных событий.
    CEvents::TEventDataCommon axEventDataCommon[RECORDED_EVENTS_QUANTITY];

//-----------------------------------------------------------------------------------------------------

public:
    CExternalModuleErrorEvent()
    {
        ui16RecordedEventsQuantity = RECORDED_EVENTS_QUANTITY;
        pui8EventsControlState = aui8EventsControlState;
        pxEventDataCommon = axEventDataCommon;
        ui16EventDataCommonPushIndex = 0;
        ui8EventTypeSetting = (1 << CEvents::EVENT_FIXED_ONCE);
        // пометим события маркером - нет события.
        for (int i = 0;
                i < (ui16RecordedEventsQuantity);
                i++)
        {
            pxEventDataCommon[i].ui16ID = 0;
        }
    }
};

//-----------------------------------------------------------------------------------------------------
class CPlcConfigurationErrorEvent : public CEvents
{
public:
    typedef enum
    {
//-----------------------------------------------------------------------------------------------------
        // количество обрабатываемых событий.
        HANDLED_EVENTS_QUANTITY = 2,
        // количество фиксируемых событий.
        RECORDED_EVENTS_QUANTITY = 2,
    };

//-----------------------------------------------------------------------------------------------------
private:
    // массив состояния событий(если - 0, событие новое. если - 1, событие не новое.);
    uint8_t aui8EventsControlState[HANDLED_EVENTS_QUANTITY];
    // массив даных событий.
    CEvents::TEventDataCommon axEventDataCommon[RECORDED_EVENTS_QUANTITY];

//-----------------------------------------------------------------------------------------------------

public:
    CPlcConfigurationErrorEvent()
    {
        ui16RecordedEventsQuantity = RECORDED_EVENTS_QUANTITY;
        pui8EventsControlState = aui8EventsControlState;
        pxEventDataCommon = axEventDataCommon;
        ui16EventDataCommonPushIndex = 0;
        ui8EventTypeSetting = (1 << CEvents::EVENT_FIXED_ONCE);
        // пометим события маркером - нет события.
        for (int i = 0;
                i < (ui16RecordedEventsQuantity);
                i++)
        {
            pxEventDataCommon[i].ui16ID = 0;
        }
    }
};

//-----------------------------------------------------------------------------------------------------
class CPlcDataBaseErrorEvent : public CEvents
{
public:
    typedef enum
    {
//-----------------------------------------------------------------------------------------------------
        // количество обрабатываемых событий.
        HANDLED_EVENTS_QUANTITY = 16,
        // количество фиксируемых событий.
        RECORDED_EVENTS_QUANTITY = 16,
    };

//-----------------------------------------------------------------------------------------------------
private:
    // массив состояния событий(если - 0, событие новое. если - 1, событие не новое.);
    uint8_t aui8EventsControlState[HANDLED_EVENTS_QUANTITY];
    // массив даных событий.
    CEvents::TEventDataCommon axEventDataCommon[RECORDED_EVENTS_QUANTITY];

//-----------------------------------------------------------------------------------------------------

public:
    CPlcDataBaseErrorEvent()
    {
        ui16RecordedEventsQuantity = RECORDED_EVENTS_QUANTITY;
        pui8EventsControlState = aui8EventsControlState;
        pxEventDataCommon = axEventDataCommon;
        ui16EventDataCommonPushIndex = 0;
        ui8EventTypeSetting = (1 << CEvents::EVENT_FIXED_ONCE);
        // пометим события маркером - нет события.
        for (int i = 0;
                i < (ui16RecordedEventsQuantity);
                i++)
        {
            pxEventDataCommon[i].ui16ID = 0;
        }
    }
};

//-----------------------------------------------------------------------------------------------------
class CAlarmEvent : public CEvents
{
public:
    typedef enum
    {
//-----------------------------------------------------------------------------------------------------
        // количество обрабатываемых событий в одной группе.
        HANDLED_EVENTS_QUANTITY = 1,
        // количество фиксируемых событий.
        RECORDED_EVENTS_QUANTITY = 16,
    };
//-----------------------------------------------------------------------------------------------------
private:
    // массив состояния событий(если - 0, событие новое. если - 1, событие не новое.);
    uint8_t aui8EventsControlState[(MAX_HANDLED_DISCRETE_INPUT +
                                    MAX_HANDLED_FUNCTION_BLOCK) * HANDLED_EVENTS_QUANTITY];
    // массив даных событий.
    CEvents::TEventDataCommon axEventDataCommon[RECORDED_EVENTS_QUANTITY];

public:
    uint8_t EventOnIsNotRegistered(uint16_t , uint8_t );
    uint8_t EventOffIsNotRegistered(uint16_t , uint8_t );
    void EventsCompleteInformationCreate(TOccuredEventsDataBriefly* );

//-----------------------------------------------------------------------------------------------------

public:
    CAlarmEvent()
    {
        ui16RecordedEventsQuantity = RECORDED_EVENTS_QUANTITY;
        pui8EventsControlState = aui8EventsControlState;
        pxEventDataCommon = axEventDataCommon;
        ui16EventDataCommonPushIndex = 0;
        ui8EventTypeSetting = (1 << CEvents::EVENT_FIXED_ONCE);
        // пометим события маркером - нет события.
        for (int i = 0;
                i < (ui16RecordedEventsQuantity);
                i++)
        {
            pxEventDataCommon[i].ui16ID = 0;
        }
        // обнулим массив регистрации состояния события.
        for (int i = 0;
                i < (MAX_HANDLED_DISCRETE_INPUT +
                     MAX_HANDLED_FUNCTION_BLOCK);
                i++)
        {
            aui8EventsControlState[i] = 0;
        }
    }
};

//-----------------------------------------------------------------------------------------------------
class CInfoEvent : public CEvents
{
public:
    typedef enum
    {
//-----------------------------------------------------------------------------------------------------
        // количество обрабатываемых событий.
        HANDLED_EVENTS_QUANTITY = 8,
        // количество фиксируемых событий.
        RECORDED_EVENTS_QUANTITY = 8,
    };
//-----------------------------------------------------------------------------------------------------
private:
    // массив состояния событий(если - 0, событие новое. если - 1, событие не новое.);
    uint8_t aui8EventsControlState[HANDLED_EVENTS_QUANTITY];
    // массив даных событий.
    CEvents::TEventDataCommon axEventDataCommon[RECORDED_EVENTS_QUANTITY];

//-----------------------------------------------------------------------------------------------------

public:
    CInfoEvent()
    {
        ui16RecordedEventsQuantity = RECORDED_EVENTS_QUANTITY;
        pui8EventsControlState = aui8EventsControlState;
        pxEventDataCommon = axEventDataCommon;
        ui16EventDataCommonPushIndex = 0;
//        ui8EventTypeSetting = (1 << CEvents::EVENT_FIXED_ONCE);
        // пометим события маркером - нет события.
        for (int i = 0;
                i < (ui16RecordedEventsQuantity);
                i++)
        {
            pxEventDataCommon[i].ui16ID = 0;
        }
    }
};

//-----------------------------------------------------------------------------------------------------
class CPlcErrorEvent : public CEvents
{
public:
    typedef enum
    {
//-----------------------------------------------------------------------------------------------------
        // количество обрабатываемых событий - ошибки.
        HANDLED_EVENTS_QUANTITY = (INTERNAL_MODULE_QUANTITY +
                                   EXTERNAL_MODULE_QUANTITY),
        // количество фиксируемых событий.
        RECORDED_EVENTS_QUANTITY = 16,
    };

//-----------------------------------------------------------------------------------------------------
// устанавливает индекс на данные события.
// в регистре (ui16EventDataCommonPushIndex - 1) содержится индекс на данные последнего события.
    void EventDataPopIndexSet(int8_t i8Length)
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
// извлекает указатель на указатель на данные события.
    CEvents::TEventDataCommon*
    EventDataPop(void)
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
private:
    // массив указателей на события в массиве даных событий.
    // в нём находятся указатели на события в массиве даных событий(axEventDataCommon[CEvents::RECORDED_EVENTS_QUANTITY]),
    // на последовательно поступающие события разных типов.
    // по ним из массива даных событий(axEventDataCommon[CEvents::RECORDED_EVENTS_QUANTITY]),
    // будем получать все последовательно поступившие события разных типов.
    CEvents::TEventDataCommon* apxEventDataCommonPointers[RECORDED_EVENTS_QUANTITY];

//-----------------------------------------------------------------------------------------------------

public:
    CPlcErrorEvent()
    {
        ui16RecordedEventsQuantity = RECORDED_EVENTS_QUANTITY;
        ppxEventDataCommonPointers = apxEventDataCommonPointers;
        ui16EventDataCommonPushIndex = 0;
        ui16EventDataCommonPointersPushIndex = 0;
        // заполним массив указателей на данные событий нулями.
        // сообщим, что нет событий.
        for (int i = 0;
                i < (ui16RecordedEventsQuantity);
                i++)
        {
            ppxEventDataCommonPointers[i] = 0;
        }
    }
};

//-----------------------------------------------------------------------------------------------------
class CPlcArchiveEvent : public CEvents
{
public:
    typedef enum
    {
//-----------------------------------------------------------------------------------------------------
        // количество фиксируемых событий.
        RECORDED_EVENTS_QUANTITY = 16,
    };
//-----------------------------------------------------------------------------------------------------
    // извлекает указатель на данные события.
    // переопределим метод:
    // после запроса, события архива помещаются в кольцевой буфер.
    // в переменной(ui16EventDataCommonPushIndex) находится индекс на последнее событие,
    // пришедшее по запросу от СУБД SQLite3. Это самое давнее событие в текущем запросе.
    // поэтому, установим индекс в функции(EventDataPopIndexSet) на самое новое событие
    // в текущем запросе, вычтя из переменной(ui16EventDataCommonPushIndex) количество
    // требуемых событий. и последовательно извлечём указатели на события от ближнего к
    // дальнему, инкрементируя переменную(ui16EventDataCommonPopIndex).
    CEvents::TEventDataCommon*
    EventDataPop(void)
    {
        // проверим, индекс события вышел за границы массива - журнал событий.
        // если да, то установим индекс события на начало массива - журнал событий.
        // для работы этого механизма, количество сохраняемых событий(ui16RecordedEventsQuantity),
        // должно быть равно степени двойки.
        // не вышли за границы массива? если вышли, обнулим индекс.
        ui16EventDataCommonPopIndex &= (ui16RecordedEventsQuantity - 1);
        // получим указатель на данные события.
        return &(pxEventDataCommon[ui16EventDataCommonPopIndex++]);
    }
//-----------------------------------------------------------------------------------------------------
//    CEvents::TEventDataCommon*
//    EventDataPush(void)
//    {
//        // проверим, индекс события вышел за границы массива - журнал событий.
//        // если да, то установим индекс события на начало массива - журнал событий.
//        // для работы этого механизма, количество сохраняемых событий(ui16RecordedEventsQuantity),
//        // должно быть равно степени двойки.
//        // не вышли за границы массива? если вышли, обнулим индекс.
//        ui16EventDataCommonPushIndex &=
//            ui16RecordedEventsQuantity - 1;
//        TEventDataCommon *pxEventData;
//        pxEventData = &(pxEventDataCommon[ui16EventDataCommonPushIndex]);
//        // установим индекс на следующее событие.
//        ui16EventDataCommonPushIndex--;
//        return pxEventData;
//    }

//-----------------------------------------------------------------------------------------------------
private:
    // массив даных событий.
    CEvents::TEventDataCommon axEventDataCommon[RECORDED_EVENTS_QUANTITY];
    // массив указателей на события в массиве даных событий.
    // в нём находятся указатели на события в массиве даных событий(axEventDataCommon[CEvents::RECORDED_EVENTS_QUANTITY]),
    // на последовательно поступающие события разных типов.
    // по ним из массива даных событий(axEventDataCommon[CEvents::RECORDED_EVENTS_QUANTITY]),
    // будем получать все последовательно поступившие события разных типов.
    CEvents::TEventDataCommon* apxEventDataCommonPointers[RECORDED_EVENTS_QUANTITY];

//-----------------------------------------------------------------------------------------------------

public:
    CPlcArchiveEvent()
    {
        ui16RecordedEventsQuantity = RECORDED_EVENTS_QUANTITY;
        pxEventDataCommon = axEventDataCommon;
        ppxEventDataCommonPointers = apxEventDataCommonPointers;
        ui16EventDataCommonPointersPushIndex = 0;
        ui16EventDataCommonPushIndex = 0;
        // заполним массив указателей на данные событий нулями.
        // сообщим, что нет событий.
        for (int i = 0;
                i < (ui16RecordedEventsQuantity);
                i++)
        {
            pxEventDataCommon[i].ui16ID = 0;
            ppxEventDataCommonPointers[i] = 0;
        }
    }
};

//-----------------------------------------------------------------------------------------------------
extern CInternalModuleErrorEvent xCInternalModuleErrorEvent;
//extern CExternalModuleErrorEvent xCExternalModuleErrorEvent;
//extern CPlcConfigurationErrorEvent xCPlcConfigurationErrorEvent;
//extern CPlcDataBaseErrorEvent xCPlcDataBaseErrorEvent;
extern CAlarmEvent xCAlarmEvent;
extern CPlcErrorEvent xCPlcErrorEvent;
extern CInfoEvent xCInfoEvent;
extern CPlcArchiveEvent xCPlcArchiveEvent;

//-----------------------------------------------------------------------------------------------------
#endif // EVENTS_H_INCLUDED

