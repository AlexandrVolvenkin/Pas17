#ifndef ALARM_H_INCLUDED
#define ALARM_H_INCLUDED
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include "Configuration.h"

////#include "AM335xPlatform.h"
//#include "Platform.h"
//#include "PasNewConfig.h"
//#include "SerialMT.h"

//-----------------------------------------------------------------------------------------------------
// 2 бита на сигнал.
// 00 – норма, 01 – квитированная сигнализация, 10 – активная сигнализация.
// 128-159 (0x80 - 0x9f) массив активности сигнализации групп дискретных сиг-
// налов Гр1-Гр16, по 2 бита на группу (32);
//#define ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_LENGTH 32
// 160-663(0xa0 - 0x297); массив активности сигнализации дискретных сигналов
// Дс1-Дс252, по 2 бита на сигнал (504);
//#define ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH ((MAX_HANDLED_DISCRETE_INPUT + MAX_HANDLED_FUNCTION_BLOCK) * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT)
//#define ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT 2
#define ALARM_TYPE_PREVENT 6
#define ALARM_TYPE_ALARM 7
#define ALARM_TYPE_PREVENT_OFFSET 0
#define ALARM_TYPE_ALARM_OFFSET 1

typedef enum
{
    COMMAND_SOUND_SIGNAL_TYPE_NOT = 0,
    COMMAND_SOUND_SIGNAL_TYPE_OFF = 1,
    COMMAND_SOUND_SIGNAL_TYPE_WARNING = 2,
    COMMAND_SOUND_SIGNAL_TYPE_ALARM = 3,
    COMMAND_SOUND_SIGNAL_TYPE_ERROR = 4,
    COMMAND_SOUND_SIGNAL_TYPE_NO_SOUND = 0x80,

    SOUND_SIGNAL_TYPE_NOT = 0,
    SOUND_SIGNAL_TYPE_WARNING,
    SOUND_SIGNAL_TYPE_ALARM,
    SOUND_SIGNAL_TYPE_ERROR,
    SOUND_SIGNAL_TYPE_NO_SOUND = 0x80,
};

//-----------------------------------------------------------------------------------------------------
class CAlarm
{
public:

    typedef enum
    {
        // длина строки содержащей имя события(текстовый реквизит).
        EVENT_NAME_LENGTH = 14
    };

#pragma pack(push)
#pragma pack(1)
    // структура данных события сигнализации - "кратко"(изменения состояния дискретных сигналов).
    struct TAlarmEventBriefPackOne
    {
        // адрес сигнала (№ входа, адрес в
        // (массив активности сигнализации дискретных сигналов - axAlarmHmi))
        uint16_t ui16Address;
        // состояние события.
        // D0-D4 - ТБЛ (группа): 0,1-24(0-нет вых.на табло).
        // D6 - ТСГ1 – тип сигнализации состояния дискретного сигнала.
        // D7 - ТСГ2 – тип сигнализации состояния дискретного сигнала.
        // 00 – нет сигнализации
        // 01 – предупредительная сигнализация
        // 10 – аварийная сигнализация
        // 11 – индикация (ровный свет без звукового сигнала)
        uint8_t ui8State;
    };
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
    // структура данных списка запросов журнала событий.
    struct TAlarmEventLogQueryListPackOne
    {
        // индекс первого запрашиваемого события в массиве(журнал событий - AlarmEventLog).
        uint16_t ui16Index;
        // количество запрашиваемых событий.
        int8_t ui8Quantity;
    };
#pragma pack(pop)

// структура данных события сигнализации - "кратко"(изменения состояния дискретных сигналов).
    struct TAlarmEventBrief
    {
        // адрес сигнала (№ входа, адрес в
        // (массив активности сигнализации дискретных сигналов - axAlarmHmi))
        uint16_t ui16Address;
        // состояние события.
        // D0-D4 - ТБЛ (группа): 0,1-24(0-нет вых.на табло).
        // D6 - ТСГ1 – тип сигнализации состояния дискретного сигнала.
        // D7 - ТСГ2 – тип сигнализации состояния дискретного сигнала.
        // 00 – нет сигнализации
        // 01 – предупредительная сигнализация
        // 10 – аварийная сигнализация
        // 11 – индикация (ровный свет без звукового сигнала)
        uint8_t ui8State;
    };

#pragma pack(push)
#pragma pack(1)
// структура данных события сигнализации(изменения состояния дискретных сигналов).
    struct TAlarmEventPackOne
    {
        // ID в таблице базы данных.
        uint16_t ui16ID;
        // индекс запрашиваемого события в массиве(журнал событий - AlarmEventLog).
        uint16_t ui16Index;
        // адрес сигнала (№ входа, адрес в
        // (массив активности сигнализации дискретных сигналов - axAlarmHmi))
        uint16_t ui16Address;
        // состояние события.
        // D0-D4 - ТБЛ (группа): 0,1-24(0-нет вых.на табло).
        // D6 - ТСГ1 – тип сигнализации состояния дискретного сигнала.
        // D7 - ТСГ2 – тип сигнализации состояния дискретного сигнала.
        // 00 – нет сигнализации
        // 01 – предупредительная сигнализация
        // 10 – аварийная сигнализация
        // 11 – индикация (ровный свет без звукового сигнала)
        uint8_t ui8State;
        // текстовый реквизит источника события.
        char acTextDescriptor[CAlarm::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH];
        // время события.
        uint8_t ui8Second;
        uint8_t ui8Minute;
        uint8_t ui8Hour;
        uint8_t ui8MonthDay;
        uint8_t ui8Month;
        uint8_t ui8Year;
    };
#pragma pack(pop)

// структура данных события сигнализации(изменения состояния дискретных сигналов).
    struct TAlarmEvent
    {
        // адрес сигнала (№ входа, адрес в
        // (массив активности сигнализации дискретных сигналов - axAlarmHmi))
        uint16_t ui16Address;
        // состояние события.
        // D0-D4 - ТБЛ (группа): 0,1-24(0-нет вых.на табло).
        // D6 - ТСГ1 – тип сигнализации состояния дискретного сигнала.
        // D7 - ТСГ2 – тип сигнализации состояния дискретного сигнала.
        // 00 – нет сигнализации
        // 01 – предупредительная сигнализация
        // 10 – аварийная сигнализация
        // 11 – индикация (ровный свет без звукового сигнала)
        uint8_t ui8State;
        // текстовый реквизит источника события.
        char acTextDescriptor[CAlarm::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH];
        // время события.
        struct tm xCurrentTime;
    };

    // структура данных управления событиями.
    struct TAlarmEventsLogControl
    {
        // индекс события в массиве axAlarmEventsLog.
        int16_t ui16AlarmEventLogIndex;
        // индекс последнего события в массиве axAlarmEventsLog.
        int16_t ui16AlarmEventLogLastIndex;
        // массив состояния события(если - 0, событие новое. если - 1, событие не новое.);
        uint8_t aui8AlarmEventsState[MAX_HANDLED_DISCRETE_INPUT + MAX_HANDLED_FUNCTION_BLOCK];
    };

    typedef enum
    {
        // длина массива "журнал событий".
//        SHORT_EVENT_LOG_LENGTH = ((CSerialMT::SERIAL_MT_MESSAGE_BAG_MAX_LENGTH -
//                                   CSerialMT::SERIAL_MT_HEADER_LENGTH) /
//                                  sizeof(struct TAlarmEventBrief))
        EVENT_LOG_LENGTH = 24,
        SHORT_EVENT_LOG_LENGTH = 16,
        // длина списка запросов событий.
        ALARM_EVENT_QUERY_LIST_LENGTH = 4,
        ALARM_EVENT_BRIEF_STRUCT_BYTE_LENGTH = 3
    };

    static void AlarmEvetnsLogCreate(void);
    static void PlcOnOffEvetnsCreate(void);

    // флаг журнал событий обновлён.
    static uint8_t ui8AlarmEventLogBriefIsUpdated;
    // длина журнала событий, в строках(событиях).
    static uint8_t ui8AlarmEventLogLength;
    // длина журнала событий "кратко", в строках(событиях).
    static uint8_t ui8AlarmEventLogBriefLength;
    // массив "журнал событий - кратко".
    static CAlarm::TAlarmEvent axAlarmEventsLog[CAlarm::EVENT_LOG_LENGTH];
    // объект данных управления событиями.
    static CAlarm::TAlarmEventsLogControl xAlarmEventsLogControl;
    // массив "журнал событий".
    static CAlarm::TAlarmEventBrief axAlarmEventsLogBrief[CAlarm::SHORT_EVENT_LOG_LENGTH];
    // список запросов событий.
    static CAlarm::TAlarmEventLogQueryListPackOne xAlarmEventsQueryList;
    // объект для очистки строки журнала событий "кратко".
    static const  CAlarm::TAlarmEvent xAlarmEventNull;
    // количество событий в запросе.
    static int16_t i16AlarmEventQueryListQuantity;

public:
    CAlarm()
    {
//        // очистим журнал событий "кратко" сигнализации дискретных сигналов.
//        memset(CAlarm::axAlarmEventsLog,
//               0,
//               sizeof(CAlarm::axAlarmEventsLog));
//        // очистим объект данных управления событиями.
//        memset((void*)&CAlarm::xAlarmEventsLogControl,
//               0,
//               sizeof(CAlarm::xAlarmEventsLogControl));
//        // установим индекс события на начало.
//        CAlarm::xAlarmEventsLogControl.ui16AlarmEventLogIndex = 0;
//        // установим индекс последнего события на начало.
//        xAlarmEventsLogControl.ui16AlarmEventLogLastIndex = 0;
//        // очистим флаг - журнал событий "кратко" обновлён.
//        CAlarm::ui8AlarmEventLogBriefIsUpdated = 0;
    }
    ~CAlarm()
    {

    }

private:
};

//-----------------------------------------------------------------------------------------------------
class CSoundSignal
{
public:

    typedef enum
    {
        IDDLE = 0,
        WARNING_SIGNAL_ON,
        WARNING_SIGNAL_OFF,
        ALARM_SIGNAL_ON,
        ALARM_SIGNAL_OFF,
        ERROR_SIGNAL_ON,
        ERROR_SIGNAL_OFF,
    };

    typedef enum
    {
//        WARNING_SIGNAL_ON_TIME = (750 / PERIPHERY_SCAN_TIME),
//        WARNING_SIGNAL_OFF_TIME = (250 / PERIPHERY_SCAN_TIME),
//        ALARM_SIGNAL_ON_TIME = (250 / PERIPHERY_SCAN_TIME),
//        ALARM_SIGNAL_OFF_TIME = (250 / PERIPHERY_SCAN_TIME),
//        ERROR_SIGNAL_ON_TIME = (100 / PERIPHERY_SCAN_TIME),
//        ERROR_SIGNAL_OFF_TIME = (200 / PERIPHERY_SCAN_TIME),
    };

//    void Scan(void);
//    void WarningSignalOn(void);
//    void AlarmSignalOn(void);
//    void ErrorSignalOn(void);
//    void SoundSignalOff(void);

    // счётчик относительного времени(циклов).
    uint8_t nui8CycleCounter;
    // положение автомата.
    uint8_t ui8FsmState;

public:
    CSoundSignal()
    {
//        BEEP_INIT();
        ui8FsmState = IDDLE;
    }
    ~CSoundSignal()
    {

    }

private:
};

#endif // ALARM_H_INCLUDED
