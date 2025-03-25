#ifndef TASK_H_INCLUDED
#define TASK_H_INCLUDED

//-------------------------------------------------------------------------------------------------
#include <stdint.h>
#include "SerialMT.h"
#include "Events.h"
#include "Alarm.h"

//-----------------------------------------------------------------------------------------------------
// (ПАС: W - запад, МИНД: E - восток)
class CTaskSerialMT
{
public:
    typedef enum
    {
        TASK_MODE_IDDLE = 0,
        TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY,
        TASK_MODE_MESSAGE_SEND_READ_CONTINUOUSLY,
        TASK_MODE_MESSAGE_ANSWER_WAITING,
        TASK_MODE_MESSAGE_SEND_READ,
        TASK_MODE_MESSAGE_SEND_WRITE,
        TASK_MODE_MESSAGE_SEND_READ_ANSWER,
        TASK_MODE_MESSAGE_SEND_WRITE_ANSWER,

        FIRST_EVENT_DISPLAY,
        BOTH_EVENT_DISPLAY,
        LAST_EVENT_DISPLAY,
        ABSENT_EVENT_DISPLAY,

        START,
        FIRST_STEP,
        SECOND_STEP,
        THIRD_STEP,
        FORTH_STEP,
        CONTINUE,
        SUSPEND,
        IDDLE,
        RESUME,
        STOP
    };

    typedef enum
    {
        TASK_STATE_MESSAGE_IDDLE = 0,
        TASK_STATE_MESSAGE_ANSWER_WAITING = 0,
        TASK_STATE_MESSAGE_RESPONSE_OK,
        TASK_STATE_MESSAGE_RECEIVE_TIMEOUT,
        TASK_STATE_MESSAGE_SEND_ERROR,
        TASK_STATE_MESSAGE_IS_SEND,
        TASK_STATE_MESSAGE_NOT_SEND,
        TASK_STATE_MESSAGE_SEND_OK,
    };

    typedef enum
    {
        TASK_ANSWER_OK = 0x7E,
        TASK_ANSWER_ERROR = 0xE7,
        TASK_CONFIRMATION_LENGTH = 1,
        // максимальное время(количество циклов) ожидания ответа.
        TASK_NO_ANSWER_TIMEOUT = 3,
        // количество поыток отправить сообщение.
        TASK_MESSAGE_SEND_ATTEMPTS_NUMBER = 60,
        // максимальное время(количество циклов) ожидания ответа при контроле связи.
        TASK_PING_NO_ANSWER_TIMEOUT = 50,
        TASK_HIGH_LEVEL_FLAGS_MESSAGE_LENGTH = 1,
        TASK_CONFIGURATION_REQUEST_MESSAGE_LENGTH = 1,
        TASK_PING_REQUEST_MESSAGE_LENGTH = 1,
        TASK_PROCESSED_MAX_NUMBER = 17,
        HIGH_LEVEL_FLAGS_KVIT_BIT = 0,
        HIGH_LEVEL_FLAGS_RESET_BIT = 1,
        HIGH_LEVEL_FLAGS_BLOCK_BIT = 2,
        HIGH_LEVEL_FLAGS_MMC_COPY_BIT = 3,
        HIGH_LEVEL_FLAGS_MMC_COPY_END_BIT = 4,
        // начало списка запросов аналоговых измерений.
        ANALOGUE_QUERY_MEASURE_LIST_BEGIN = 0,
        // длина списка запросов аналоговых измерений.
        ANALOGUE_QUERY_MEASURE_LIST_LENGTH = 6,
        // длина списка запросов состояния дискретных выходов.
        DISCRETE_INPUT_QUERY_LIST_LENGTH = 3,
//        // длина строки текстового реквизита.
//        ANALOGUE_INPUT_NAME_LENGTH = 8,
//        // длина строки текстового реквизита размерности измеряемого параметра КРЗМ.
//        DIMENSIONS_PARAMETERS_NAME_LENGTH = 6,
//        CURRENT_TIME_BYTE_QUANTITY = 7,
//        CURRENT_TIME_SECOND_OFFSET = 0,
//        CURRENT_TIME_MINUTE_OFFSET = 1,
//        CURRENT_TIME_HOUR_OFFSET = 2,
//        CURRENT_TIME_MONTH_DAY_OFFSET = 3,
//        CURRENT_TIME_MONTH_OFFSET = 4,
//        CURRENT_TIME_YEAR_OFFSET = 5,
//        CURRENT_TIME_WEEK_DAY_OFFSET = 6
    };

//-----------------------------------------------------------------------------------------------------
//private:
    // счётчик относительного времени(циклов).
    uint8_t nui8CycleCounter;
    // состояние задачи.
    uint8_t ui8Status;
    // вспомогательный счётчик относительного времени(циклов).
    uint8_t nui8TimeCounter;
    // вспомогательный счётчик количества попыток отправки.
    uint8_t nui8AttemptsNumber;
    // положение автомата.
    uint8_t ui8DataExchangeFsmState;
    // начальное положение автомата.
    uint8_t ui8InitFsmState;
    // последний адрес для повтора.
    uint8_t ui8LastAddress;
    // индекс последнего запроса.
    int16_t i16LastRequestIndex;
    // массив с указателями на объект класса обрабатываемых задач.
    static CTaskSerialMT *apxTasksProcessed[];
    uint8_t ui8TasksProcessedNumber;
    struct TModuleContext *pxModuleContext;
    // id в предыдущем запросе.
    int16_t i16LastID;


public:
    uint8_t SendMessage(CSerialMT *, uint8_t );
    uint8_t ReceiveMessage(CSerialMT * , uint8_t );
    uint8_t StateGet(void);
    uint8_t StatusGet(void);
    void StateSet(uint8_t );
    void StatusSet(uint8_t );
    void IndexSet(uint8_t );
    uint8_t DataExchange(CTaskSerialMT **, CSerialMT *);
    // чистая виртуальная функция.
    // делает класс абстрактным. создать его объект нельзя.
    virtual void StatusCheck(void);
//    virtual void FillingMessage(CEvents::TEventDataPackOne *, CEvents::TEventDataCommon *, int16_t );
//    virtual int8_t InfoEvetnsLogCreate(void);
    virtual uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    virtual uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    virtual uint8_t ReceiveProcessing(CSerialMT *, uint8_t );

//-----------------------------------------------------------------------------------------------------
    CTaskSerialMT()
    {
        nui8CycleCounter = 20;

    }
    virtual ~CTaskSerialMT()
    {

    }
// Error[e46]: Undefined external "CTaskSerialMT::__vtbl" referred in Task
// ошибка появляется, если виртуальная функция не определена в базовом классе
// и функция не определена как чистая виртуальная(virtual int function(void) = 0).
};

//-----------------------------------------------------------------------------------------------------
// контролирует связь с МИНД. если МИНД не отвечает - авария.
// отвечает на запрос контроля связи от МИНД.
// (ПАС: W - запад, МИНД: E - восток)
class CPingTaskEWWE : public CTaskSerialMT
{
public:

    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );

//-----------------------------------------------------------------------------------------------------
    CPingTaskEWWE()
    {
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8TasksProcessedNumber = 0;
        ui8InitFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        nui8CycleCounter = 0;
    }
    ~CPingTaskEWWE()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
class CPingTaskWEEW : public CTaskSerialMT
{
public:

    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );

//-----------------------------------------------------------------------------------------------------
    CPingTaskWEEW()
    {
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_IDDLE;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_READ;
        ui8TasksProcessedNumber = 0;
        nui8CycleCounter = 1;
    }
    ~CPingTaskWEEW()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
// отправляет флаги верхнего уровня в МИНД(ПАС: W - запад, МИНД: E - восток)
class CHighLevelFlagsSetWE : public CTaskSerialMT
{
public:

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
    struct THighLevelFlagsExchangePackOne
    {
        //  флаги управления от РС к МИНД - F_HLEV (1б): D0-кв. РС, D1-сброс РС, D2-бл.ВКЛ/ОТКЛ - факт. сост.;
        uint8_t ui8HighLevelFlagsWE;
        // тип звукового сигнала.
        uint8_t ui8SoundSignalType;
    };
#pragma pack(pop)

    void StatusCheck(void);
    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );
    //  флаги управления от РС к МИНД - F_HLEV (1б): D0-кв. РС, D1-сброс РС, D2-бл.ВКЛ/ОТКЛ - факт. сост.;
//    uint8_t ui8HighLevelFlagsWE;
    THighLevelFlagsExchangePackOne xTHighLevelFlagsExchangePackOne;

//-----------------------------------------------------------------------------------------------------
    CHighLevelFlagsSetWE()
    {
        xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE = 0;
        // сбросим признаки звукового сигнала.
        (xTHighLevelFlagsExchangePackOne.ui8SoundSignalType) = SOUND_SIGNAL_TYPE_NOT;
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_IDDLE;
        ui8DataExchangeFsmState = TASK_MODE_IDDLE;
        nui8CycleCounter = 2;
    }
    ~CHighLevelFlagsSetWE()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
// принимает флаги верхнего уровня от МИНД(ПАС: W - запад, МИНД: E - восток)
class CHighLevelFlagsSetEW : public CTaskSerialMT
{
public:

    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );

//-----------------------------------------------------------------------------------------------------
    CHighLevelFlagsSetEW()
    {
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        nui8CycleCounter = 3;
    }
    ~CHighLevelFlagsSetEW()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
// получает запросы на передачу журнала событий от МИНД(ПАС: W - запад, МИНД: E - восток)
// передаёт журнал событий в МИНД.
class CEventLogQuery : public CTaskSerialMT
{
public:

    void FillingMessage(CEvents::TEventDataPackOne *, CEvents::TEventDataCommon *, int16_t );
//    int8_t InfoEvetnsLogCreate(void);
    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );

//    // индекс последнего запроса.
//    static int16_t i16LastRequestIndex;
    // направление чтения данных.
    static uint8_t ui8DirectionIsForward;

//-----------------------------------------------------------------------------------------------------
    CEventLogQuery()
    {
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        nui8CycleCounter = 4;
    }
    ~CEventLogQuery()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
class CErrorEventLogQuery : public CEventLogQuery
{
public:

//    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
//    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
//    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );
//    void DisplayModeSet(void);
//    static void Display(void);

//-----------------------------------------------------------------------------------------------------
    CErrorEventLogQuery()
    {
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        nui8CycleCounter = 5;
    }
    ~CErrorEventLogQuery()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
class CArchiveDefaultEventLogQuery : public CEventLogQuery
{
public:

//    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
//    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
//    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );
//    void DisplayModeSet(void);
//    static void Display(void);

//-----------------------------------------------------------------------------------------------------
    CArchiveDefaultEventLogQuery()
    {
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        nui8CycleCounter = 6;
    }
    ~CArchiveDefaultEventLogQuery()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
class CArchiveByNumberEventLogQuery : public CEventLogQuery
{
public:

//    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
//    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
//    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );
//    void DisplayModeSet(void);
//    static void Display(void);

//-----------------------------------------------------------------------------------------------------
    CArchiveByNumberEventLogQuery()
    {
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        nui8CycleCounter = 7;
    }
    ~CArchiveByNumberEventLogQuery()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
class CArchiveByDateEventLogQuery : public CEventLogQuery
{
public:

//    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
//    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
//    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );
//    void DisplayModeSet(void);
//    static void Display(void);

//-----------------------------------------------------------------------------------------------------
    CArchiveByDateEventLogQuery()
    {
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        nui8CycleCounter = 8;
    }
    ~CArchiveByDateEventLogQuery()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
// получает запросы на передачу журнала информации о приборе от МИНД(ПАС: W - запад, МИНД: E - восток)
// передаёт журнал информации о приборе в МИНД.
//-----------------------------------------------------------------------------------------------------
class CInfoLogQuery : public CTaskSerialMT
{
public:

//    void FillingMessage(CEvents::TEventDataPackOne *, CEvents::TEventDataCommon *, int16_t );
    int8_t InfoEvetnsLogCreate(void);
    int8_t InfoEvetnsLogUpdate(void);
    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );

//-----------------------------------------------------------------------------------------------------
    CInfoLogQuery()
    {
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        nui8CycleCounter = 10;
    }
    ~CInfoLogQuery()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
// получает запросы на передачу журнала дискретных выходов от МИНД(ПАС: W - запад, МИНД: E - восток)
// передаёт журнал дискретных выходов в МИНД.
class CDiscreteOutputLogQuery : public CTaskSerialMT
{
public:
    typedef enum
    {
        // длина списка запросов состояния дискретных выходов.
        DISCRETE_INPUT_QUERY_LIST_LENGTH = 3,
    };

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
    // структура данных состояния дискретных выходов.
    struct TDiscreteOutputsPackOne
    {
        // ID.
        uint16_t ui16ID;
        // индекс запрашиваемого байта(модуля) состояния дискретных выходов
        // в массиве(состояние дискретных выходов).
        int16_t i16Index;
        // состояние дискретных выходов.
        uint8_t ui8State;
    };
#pragma pack(pop)

    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );

    // массив состояния дискретных выходов.
    TDiscreteOutputsPackOne axDiscreteOutputsLog[DISCRETE_INPUT_QUERY_LIST_LENGTH];

//-----------------------------------------------------------------------------------------------------
    CDiscreteOutputLogQuery()
    {
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        nui8CycleCounter = 11;
    }
    ~CDiscreteOutputLogQuery()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
// получает запрос на передачу конфигкрации от МИНД(ПАС: W - запад, МИНД: E - восток)
// передаёт конфигкрацию в МИНД.
class CConfigurationQueryEW : public CTaskSerialMT
{
public:

    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );

//-----------------------------------------------------------------------------------------------------
    CConfigurationQueryEW()
    {
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        nui8CycleCounter = 12;
    }
    ~CConfigurationQueryEW()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
// получает запрос на передачу текущего времени от МИНД(ПАС: W - запад, МИНД: E - восток)
// передаёт текущее время в МИНД.
class CCurrentTimeQueryEW : public CTaskSerialMT
{
public:

    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );

//-----------------------------------------------------------------------------------------------------
    CCurrentTimeQueryEW()
    {
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        nui8CycleCounter = 13;
    }
    ~CCurrentTimeQueryEW()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
// получает запрос на передачу текущего времени от МИНД(ПАС: W - запад, МИНД: E - восток)
// передаёт текущее время в МИНД.
class CCurrentTimeSetEW : public CTaskSerialMT
{
public:

    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );

//-----------------------------------------------------------------------------------------------------
    CCurrentTimeSetEW()
    {
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        nui8CycleCounter = 14;
    }
    ~CCurrentTimeSetEW()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
class CSettingsRW : public CTaskSerialMT
{
public:
    typedef enum
    {
        RS485_HIGH_LEVEL_SET = 1,
        RS485_LOW_LEVEL_SET,
        ARCHIVE_LOG_SAVE,
        ETHERNET_SET,
        PROGRAMM_FILE_UPDATE,
        DEBUG_LOG_FILE_SAVE
    };

    typedef enum
    {
        CHOICE_OPTION_TEXT_LENGTH = 14,
        CHOICE_OPTION_TEXT_NUMBER = 2,
        ETHERNET_IP_TEXT_LENGTH = 16,
    };

    typedef enum
    {
        WRITE_IDDLE = 0,
        WRITE_ERROR = 1,
        WRITE_OK = 2,
        WRITE_BUSY = 3
    };

    typedef enum
    {
        BIT_RATE_9600 = 0,
        BIT_RATE_19200,
        BIT_RATE_57600,
        BIT_RATE_115200,
    };

    typedef enum
    {
        PARITY_NO = 0,
        PARITY_EVEN,
        PARITY_ODD,
        PARITY_NO_ONE_STOP,
    };

#pragma pack(push)
#pragma pack(1)
    // структура данных запросов.
    struct TPlcSettingsQueryPackOne
    {
        // тип запрашиваемых данных.
        uint8_t ui8Type;
    };
#pragma pack(pop)

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
    struct TPortSettingsPackOne
    {
        uint8_t ui8BaudRate;
        uint8_t ui8StopBits;
        uint8_t ui8Parity;
    };
#pragma pack(pop)

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
    struct TEthernetSettingsPackOne
    {
        uint8_t ui8IpByte0;
        uint8_t ui8IpByte1;
        uint8_t ui8IpByte2;
        uint8_t ui8IpByte3;
        uint16_t ui16Port;
    };
#pragma pack(pop)

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
    struct TPeripherySettingsPackOne
    {
        uint8_t ui8ZummerIsOn;
    };
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
    struct TPlcSettingsPackOne
    {
        TPortSettingsPackOne xTRs485HighLevelSettingsPackOne;
        TPortSettingsPackOne xTRs485LowLevelSettingsPackOne;
        TEthernetSettingsPackOne xTEthernetSettingsPackOne;
        TPeripherySettingsPackOne xTPeripherySettingsPackOne;
        uint16_t ui16Crc;
    };
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
    struct TChoiceOptionTextData
    {
        // текстовые данные предложения меню.
        char acChoiceOptionTextData[CHOICE_OPTION_TEXT_LENGTH + END_OF_STRING_LENGTH];
    };
#pragma pack(pop)

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
    struct TPlcSettingsExchangePackOne
    {
        // тип запрашиваемых данных.
        uint8_t ui8Type;
        // код ответа.
        uint8_t ui8AnswerCode;
        union
        {
            struct
            {
                uint8_t ui8BaudRate;
                uint8_t ui8StopBits;
                uint8_t ui8Parity;
            };
            struct
            {
                uint8_t ui8IpByte0;
                uint8_t ui8IpByte1;
                uint8_t ui8IpByte2;
                uint8_t ui8IpByte3;
                uint16_t ui16Port;
            };
            struct
            {
                // номер предложения.
                uint8_t ui8ChoiceOption;
                // количество предложений.
                uint8_t ui8ChoiceOptionNumber;
                // текстовые данные предложения меню.
                TChoiceOptionTextData axChoiceOptionTextData[CHOICE_OPTION_TEXT_NUMBER];
            };
            struct
            {
                uint8_t ui8ZummerIsOn;
            };
        };
    };

    struct TPlcSettingsExchangeClientPackOne
    {
        // тип запрашиваемых данных.
        uint8_t ui8Type;
        // код ответа.
        uint8_t ui8AnswerCode;
        union
        {
            struct
            {
                uint8_t ui8BaudRate;
                uint8_t ui8StopBits;
                uint8_t ui8Parity;
            };
            struct
            {
                uint8_t ui8IpByte0;
                uint8_t ui8IpByte1;
                uint8_t ui8IpByte2;
                uint8_t ui8IpByte3;
                uint16_t ui16Port;
            };
            struct
            {
                // номер предложения.
                uint8_t ui8ChoiceOption;
                // количество предложений.
                uint8_t ui8ChoiceOptionNumber;
            };
            struct
            {
                uint8_t ui8ZummerIsOn;
            };
        };
    };
#pragma pack(pop)

    static uint8_t PlcSettingsCheck(void);
    static uint8_t PlcSettingsSave(void);
    static uint8_t PlcSettingsLoad(void);
    static void ArchiveSave(uint8_t );
    static void DebugLogFileSave(uint8_t );
    static uint8_t ProgrammFileUpdate(uint8_t );
    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );
//    void DisplayModeSet(void);
//    static void Display(void);

//-----------------------------------------------------------------------------------------------------
    static const char *pccSettingsFileName;
    static TPlcSettingsPackOne xTPlcSettingsPackOne;
    static TPlcSettingsPackOne xTPlcSettingsDefaultPackOne;
    static TPlcSettingsQueryPackOne xTPlcSettingsQueryPackOne;
    static TPlcSettingsExchangePackOne xTPlcSettingsExchangePackOne;
    static uint8_t ui8ModbusRtuDataBits;
    static uint8_t ui8ModbusRtuStopBits;
    static uint32_t ui32ModbusRtuBaudRate;
    static char cModbusRtuParity;
    static uint8_t ui8ModbusRtuMasterDataBits;
    static uint8_t ui8ModbusRtuMasterStopBits;
    static uint32_t ui32ModbusRtuMasterBaudRate;
    static char cModbusRtuMasterParity;
    static char acModbusTcpIP[ETHERNET_IP_TEXT_LENGTH];
    static uint16_t ui16ModbusTcpPort;
    static uint8_t ui8ArchiveFileIsSaveState;
    static uint8_t ui8DebugLogFileSaveState;


//-----------------------------------------------------------------------------------------------------
    CSettingsRW()
    {
        pccSettingsFileName = "/home/debian/PasSettings.dat";
//        pccSettingsFileName = "PasSettings.dat";
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        xTPlcSettingsPackOne.xTRs485HighLevelSettingsPackOne.ui8BaudRate = 0;
        xTPlcSettingsPackOne.xTRs485HighLevelSettingsPackOne.ui8StopBits = 1;
        xTPlcSettingsPackOne.xTRs485HighLevelSettingsPackOne.ui8Parity = 0;
        nui8CycleCounter = 15;
        ui8ArchiveFileIsSaveState = WRITE_IDDLE;
        ui8DebugLogFileSaveState = WRITE_IDDLE;
    }
    ~CSettingsRW()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
class CRs485Set : public CSettingsRW
{
public:
    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );

//-----------------------------------------------------------------------------------------------------
    CRs485Set()
    {
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
    }
    ~CRs485Set()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
// получает запросы на передачу измеренных аналоговых значений от МИНД(ПАС: W - запад, МИНД: E - восток)
// передаёт измеренные аналоговые значения в МИНД.
class CAnalogueMeasureQuery : public CTaskSerialMT
{
public:

    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );

//    // индекс последнего запроса.
//    static int16_t i16LastRequestIndex;
//-----------------------------------------------------------------------------------------------------
    CAnalogueMeasureQuery()
    {
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_RECEIVE_CONTINUOUSLY;
        i16LastRequestIndex = 1;
        nui8CycleCounter = 9;
    }
    ~CAnalogueMeasureQuery()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
// задача формирует пустое сообщение размером максимальной длины принимаемого кадра
// CSerialMT::MAX_RECEIVED_FRAME_LENGTH.
// передача и приём на шине SPI происходит в дуплексном режиме.
// если длина передаваемого кадра меньше длины принимаемого кадра(CSerialMT::MAX_RECEIVED_FRAME_LENGTH), то
// принимаемый кадр будет потерян.
// поэтому длина передаваемого кадра должна быть больше принимаемого кадра.
// максимальная длина принимаемого кадра известна на стадии проектирования.
// в задаче сравнивается длина общего сообщения протокола SerialMT и
// максимальная длина принимаемого кадра.
// если длина общего сообщения протокола SerialMT меньше максимальной длины
// принимаемого кадра, то задача формирует пустое сообщение
// размером максимальной длины принимаемого кадра.
class CReceiveRequest : public CTaskSerialMT
{
public:

    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );

//-----------------------------------------------------------------------------------------------------
    CReceiveRequest()
    {
        pxModuleContext = xAllModulesContext.axAllModulesContext;
        ui8InitFsmState = TASK_MODE_MESSAGE_SEND_READ_CONTINUOUSLY;
        ui8DataExchangeFsmState = TASK_MODE_MESSAGE_SEND_READ_CONTINUOUSLY;
        nui8CycleCounter = 16;
    }
    ~CReceiveRequest()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
extern CPingTaskEWWE xCPingTaskEWWE;
extern CPingTaskWEEW xCPingTaskWEEW;
extern CHighLevelFlagsSetWE xCHighLevelFlagsSetWE;
extern CHighLevelFlagsSetEW xCHighLevelFlagsSetEW;
extern CConfigurationQueryEW xCConfigurationQueryEW;
extern CCurrentTimeQueryEW xCCurrentTimeQueryEW;
extern CCurrentTimeSetEW xCCurrentTimeSetEW;
extern CSettingsRW xCSettingsRW;
extern CEventLogQuery xCEventLogQuery;
extern CErrorEventLogQuery xCErrorEventLogQuery;
extern CArchiveDefaultEventLogQuery xCArchiveDefaultEventLogQuery;
extern CArchiveByNumberEventLogQuery xCArchiveByNumberEventLogQuery;
extern CArchiveByDateEventLogQuery xCArchiveByDateEventLogQuery;
extern CDiscreteOutputLogQuery xCDiscreteOutputLogQuery;
extern CAnalogueMeasureQuery xCAnalogueMeasureQuery;
extern CReceiveRequest xCReceiveRequest;
extern CInfoLogQuery xCInfoLogQuery;
//extern CTaskSerialMT xAlarmCTaskSerialMT;

#endif // TASK_H_INCLUDED
