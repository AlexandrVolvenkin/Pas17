#ifndef MODULEMIND_H_INCLUDED
#define MODULEMIND_H_INCLUDED

//#include "AM335xPlatform.h"
#include "PasNewConfig.h"
#include <stdint.h>
#include "SerialMT.h"

//-----------------------------------------------------------------------------------------------------
// MIND
// коды типа модуля.
// временно. //?
#define MODULE_MIND_MODBUS_ADDRESS 0x11
#define MODULE_TYPE_MIND 0x11
#define MODULE_MIND_INDEX (INTERNAL_MODULE_QUANTITY + EXTERNAL_MODULE_QUANTITY)
// $42 - запрос на основной (циклический) обмен данными.
#define MIND_DATA_EXCHANGE_COMMAND 0x42
#define MIND_DATA_EXCHANGE_READY 0x42
#define MIND_DATA_EXCHANGE_NOT_READY 0x24
#define MIND_DATA_EXCHANGE_COMMAND_ANSWER_LENGTH 5
// $88 - запрос на передачу конфигурации.
#define MIND_CONFIGURATION_LOAD_COMMAND 0x88
#define MIND_CONFIGURATION_LOAD_COMMAND_ANSWER_LENGTH 15
#define MIND_INDICATION_MODULE_TYPE_12_CELLS_LCD_4x24 0x88
#define MIND_INDICATION_MODULE_TYPE_24_CELLS_LCD_4x24 0x89
#define MIND_INDICATION_MODULE_TYPE_LCD_320x240 0x8A
#define MIND_INDICATION_MODULE_TYPE_LCD_4x24 0x8B

//-----------------------------------------------------------------------------------------------------
// (ПАС: W - запад, МИНД: E - восток)
class CMind
{
public:
    typedef enum
    {
        TASK_MODE_SERVER = 0,
        TASK_MODE_CLIENT,
        TASK_MODE_MESSAGE_IS_SEND = 1,
        TASK_MODE_MESSAGE_SEND_OK = 2,
        TASK_MODE_MESSAGE_SEND_ERROR = 3,
        TASK_MODE_MESSAGE_SEND_TIMEOUT = 4,
        TASK_MODE_MESSAGE_IS_HANDLED = 5,
        TASK_MODE_MESSAGE_SEND_READY = 6,
        TASK_MODE_MESSAGE_SEND_START = 0,
    };

    typedef enum
    {
        TASK_ANSWER_OK = 0x7E,
        TASK_ANSWER_ERROR = 0xE7,
        TASK_CONFIRMATION_LENGTH = 1,
        // максимальное время(количество циклов) ожидания ответа.
        TASK_NO_ANSWER_TIMEOUT = 3,
        // максимальное время(количество циклов) ожидания ответа при контроле связи.
        TASK_PING_NO_ANSWER_TIMEOUT = 50,
        TASK_HIGH_LEVEL_FLAGS_MESSAGE_LENGTH = 1,
        TASK_CONFIGURATION_REQUEST_MESSAGE_LENGTH = 1,
        TASK_PING_REQUEST_MESSAGE_LENGTH = 1,
        HIGH_LEVEL_FLAGS_KVIT_BIT = 0,
        HIGH_LEVEL_FLAGS_RESET_BIT = 1,
        HIGH_LEVEL_FLAGS_BLOCK_BIT = 2,
        // длина списка запросов аналоговых измерений.
        ANALOGUE_QUERY_MEASURE_LIST_LENGTH = 6,
        // длина списка запросов состояния дискретных выходов.
        DISCRETE_INPUT_QUERY_LIST_LENGTH = 3,
//        // длина строки текстового реквизита.
//        ANALOGUE_INPUT_NAME_LENGTH = 8,
//        // длина строки текстового реквизита размерности измеряемого параметра КРЗМ.
//        DIMENSIONS_PARAMETERS_NAME_LENGTH = 6,
    };

//-----------------------------------------------------------------------------------------------------
    struct TTaskContext
    {
        // флаг - сообщение отправлено.
        uint8_t fui8MessageIsSent;
        // состояние задачи.
        uint8_t ui8Status;
        // вспомогательный счётчик относительного времени(циклов).
        uint8_t nui8TimeCounter;
        // указатель на функцию-задачу, связанную с адресом.
        int16_t (*pfi16Task)(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
    };

#pragma pack(push)
#pragma pack(1)
    // структура данных аналоговых измерений.
    struct TAnalogueMeasurePackOne
    {
        // адрес первого запрашиваемого аналогового измерения.
        // адрес сигнала (№ входа, адрес в
        // (измеренные значения входных аналоговых сигналов - AI_VALUE_BYTE_ARRAY_OFFSET)
        uint8_t ui8Address;
        // состояние канала аналогового измерения.
        uint8_t ui8Status;
        // значение аналогового измерения.
        float fData;
        // Текстовый реквизит входа – 8 символов ASCII, плюс нуль - признак конца строки.
        char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
        // Текстовый реквизит размерности – 6 символов ASCII, плюс нуль - признак конца строки.
        char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
    };

    // структура данных списка запросов аналоговых измерений.
    struct TAnalogueMeasureQueryListPackOne
    {
        // адрес первого запрашиваемого аналогового измерения.
        // адрес сигнала (№ входа, адрес в
        // (измеренные значения входных аналоговых сигналов)
        uint8_t ui8Address;
        // количество запрашиваемых аналоговых измерений.
        int8_t ui8Quantity;
    };

    // структура данных списка запросов состояния дискретных выходов.
    struct TDiscreteOutputsQueryListPackOne
    {
        // индекс первого запрашиваемого байта(модуля) состояния дискретных выходов
        // в массиве(состояние дискретных выходов).
        uint16_t ui16Index;
        // количество запрашиваемых байт(модулей) состояния дискретных выходов.
        int8_t ui8Quantity;
    };

    // структура данных состояния дискретных выходов.
    struct TDiscreteOutputsPackOne
    {
        // индекс запрашиваемого байта(модуля) состояния дискретных выходов
        // в массиве(состояние дискретных выходов).
        uint16_t ui16Index;
        // состояние дискретных выходов.
        uint8_t ui8State;
    };
#pragma pack(pop)

// структура данных аналоговых измерений.
    struct TAnalogueMeasure
    {
        // адрес первого запрашиваемого аналогового измерения.
        // адрес сигнала (№ входа, адрес в
        // (измеренные значения входных аналоговых сигналов - AI_VALUE_BYTE_ARRAY_OFFSET)
        uint8_t ui8Address;
        // состояние канала аналогового измерения.
        uint8_t ui8Status;
        // значение аналогового измерения.
        float fData;
        // Текстовый реквизит входа – 8 символов ASCII, плюс нуль - признак конца строки.
        char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
        // Текстовый реквизит размерности измеряемого параметра КРЗМ.
        char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
    };

//-----------------------------------------------------------------------------------------------------
private:
    // массив с контекстом задач.
    // индекс контекста задачи определяется адресом задачи во вложенном сообщении.
    static CMind::TTaskContext axAllTasksContext[];
    // количество функций-задач в массиве контекста задач.
    static int16_t i16TasksQuantity;
    //  флаги управления от РС - F_HLEV (1б): D0-кв. РС, D1-сброс РС, D2-бл.ВКЛ/ОТКЛ - факт. сост.;
    static uint8_t ui8HighLevelFlags;

public:
    //  флаги управления от РС к МИНД - F_HLEV (1б): D0-кв. РС, D1-сброс РС, D2-бл.ВКЛ/ОТКЛ - факт. сост.;
    static uint8_t ui8HighLevelFlagsGet;
    //  флаги управления от МИНД к РС - F_HLEV (1б): D0-кв. РС, D1-сброс РС, D2-бл.ВКЛ/ОТКЛ - факт. сост.;
    static uint8_t ui8HighLevelFlagsSet;
    static uint8_t ui8HighLevelFlagsSetSave;
    // состояние флагов управления от МИНД к РС - F_HLEV обновлено.
    static uint8_t ui8HighLevelFlagsIsUpdated;
    // список запросов аналоговых измерений.
    static CMind::TAnalogueMeasureQueryListPackOne xAnalogueMeasureQueryList;
    static CMind::TAnalogueMeasureQueryListPackOne axAnalogueMeasureQueryList[CMind::ANALOGUE_QUERY_MEASURE_LIST_LENGTH];
//    // массив аналоговых измерений.
//    static CMind::TAnalogueMeasure axAnalogueMeasure[CMind::ANALOGUE_QUERY_MEASURE_LIST_LENGTH];
    // количество аналоговых измерений в запросе.
    static int16_t i16AnalogueMeasureQueryListQuantity;
    // количество полученных аналоговых измерений.
    static int16_t i16AnalogueMeasureQuantity;
    // флаг список запросов аналоговых измерений обновлён.
    static uint8_t ui8AnalogueMeasureQueryListIsUpdated;
    // флаг конфигурация обновлена.
    static uint8_t ui8ConfigurationIsUpdated;
    // список запросов состояния дискретных выходов.
    static CMind::TDiscreteOutputsQueryListPackOne xDiscreteOutputsQueryList;
    // массив состояния дискретных выходов.
    static uint8_t ui8DiscreteOutputsLog[CMind::DISCRETE_INPUT_QUERY_LIST_LENGTH];
    // количество байт(модулей) состояния дискретных выходов в запросе.
    static int16_t i16DiscreteOutputsQueryListQuantity;
    // флаг состояние дискретных выходов обновлено.
    static uint8_t ui8DiscreteOutputsIsUpdated;
    // последний индекс события в списке запросов.
    static uint16_t ui16LastIndexQuery;
    // общее количество записей полученных от СУБД.
    static int16_t i16AllReceivedRecordsQuantity;
    // общее количество записей полученных от СУБД в предыдущем запросе.
    static int16_t i16LastAllReceivedRecordsQuantity;


//-----------------------------------------------------------------------------------------------------
    static int16_t Init(void);
    static int16_t DataExchangeSpi(struct TModuleContext *);
    static int16_t DataExchangeModbus(struct TModuleContext *);
    static int16_t RequestHandler(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
    static int16_t AnswerHandler(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
    static int16_t ConfigurationQueryEW(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
//    static int16_t TimeSetWE(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
    static int16_t CurrentTimeQueryEW(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
    static int16_t CurrentTimeSetEW(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
    static int16_t HighLevelFlagsSetWE(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
    static int16_t HighLevelFlagsSetEW(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
//    static int16_t AlarmEventBriefLogSendWE(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
    static int16_t EventLogQueryEW(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
//    static int16_t AlarmEventLogQueryEW(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
//    static int16_t ArchiveEventLogQueryEW(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
//    static int16_t ArchiveEventLogQueryEWLite(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
    static int16_t ErrorLogQueryEW(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
    static int16_t DiscreteOutputsLogQueryEW(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
    static int16_t AnalogueMeasureQueryEW(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
    static int16_t PingTaskEWWE(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
    // задача-заглушка.
    // вызывается по указателю из массива с контекстом задач по адресу-индексу 0.
    // чтобы при вызове функции не обратится по нулевому адресу.
    // адрес: 0 - не определён протоколом(SerialMT).
    static int16_t NopTask(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
    // задача формирует пустое сообщение размером максимальной длины принимаемого кадра
    // CSpiMT::MAX_RECEIVED_FRAME_LENGTH.
    // передача и приём на шине SPI происходит в дуплексном режиме.
    // если длина передаваемого кадра меньше длины принимаемого кадра(CSpiMT::MAX_RECEIVED_FRAME_LENGTH), то
    // принимаемый кадр будет потерян.
    // поэтому длина передаваемого кадра должна быть больше принимаемого кадра.
    // максимальная длина принимаемого кадра известна на стадии проектирования.
    // в задаче сравнивается длина общего сообщения протокола SerialMT и
    // максимальная длина принимаемого кадра.
    // если длина общего сообщения протокола SerialMT меньше максимальной длины
    // принимаемого кадра, то задача формирует пустое сообщение
    // размером максимальной длины принимаемого кадра.
    static int16_t ReceiveRequest(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);

//-----------------------------------------------------------------------------------------------------
    CMind()
    {
        // очистим флаги управления от РС.
        CMind::ui8HighLevelFlags = 0;
    }
    ~CMind()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
#endif // MODULEMIND_H_INCLUDED
