#ifndef TASK_H_INCLUDED
#define TASK_H_INCLUDED

//-------------------------------------------------------------------------------------------------
#include <stdint.h>
#include "SerialMT.h"
#include "Events.h"
#include "Alarm.h"

//-----------------------------------------------------------------------------------------------------
// (���: W - �����, ����: E - ������)
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
        // ������������ �����(���������� ������) �������� ������.
        TASK_NO_ANSWER_TIMEOUT = 3,
        // ���������� ������ ��������� ���������.
        TASK_MESSAGE_SEND_ATTEMPTS_NUMBER = 60,
        // ������������ �����(���������� ������) �������� ������ ��� �������� �����.
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
        // ������ ������ �������� ���������� ���������.
        ANALOGUE_QUERY_MEASURE_LIST_BEGIN = 0,
        // ����� ������ �������� ���������� ���������.
        ANALOGUE_QUERY_MEASURE_LIST_LENGTH = 6,
        // ����� ������ �������� ��������� ���������� �������.
        DISCRETE_INPUT_QUERY_LIST_LENGTH = 3,
//        // ����� ������ ���������� ���������.
//        ANALOGUE_INPUT_NAME_LENGTH = 8,
//        // ����� ������ ���������� ��������� ����������� ����������� ��������� ����.
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
    // ������� �������������� �������(������).
    uint8_t nui8CycleCounter;
    // ��������� ������.
    uint8_t ui8Status;
    // ��������������� ������� �������������� �������(������).
    uint8_t nui8TimeCounter;
    // ��������������� ������� ���������� ������� ��������.
    uint8_t nui8AttemptsNumber;
    // ��������� ��������.
    uint8_t ui8DataExchangeFsmState;
    // ��������� ��������� ��������.
    uint8_t ui8InitFsmState;
    // ��������� ����� ��� �������.
    uint8_t ui8LastAddress;
    // ������ ���������� �������.
    int16_t i16LastRequestIndex;
    // ������ � ����������� �� ������ ������ �������������� �����.
    static CTaskSerialMT *apxTasksProcessed[];
    uint8_t ui8TasksProcessedNumber;
    struct TModuleContext *pxModuleContext;
    // id � ���������� �������.
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
    // ������ ����������� �������.
    // ������ ����� �����������. ������� ��� ������ ������.
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
// ������ ����������, ���� ����������� ������� �� ���������� � ������� ������
// � ������� �� ���������� ��� ������ �����������(virtual int function(void) = 0).
};

//-----------------------------------------------------------------------------------------------------
// ������������ ����� � ����. ���� ���� �� �������� - ������.
// �������� �� ������ �������� ����� �� ����.
// (���: W - �����, ����: E - ������)
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
// ���������� ����� �������� ������ � ����(���: W - �����, ����: E - ������)
class CHighLevelFlagsSetWE : public CTaskSerialMT
{
public:

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
    struct THighLevelFlagsExchangePackOne
    {
        //  ����� ���������� �� �� � ���� - F_HLEV (1�): D0-��. ��, D1-����� ��, D2-��.���/���� - ����. ����.;
        uint8_t ui8HighLevelFlagsWE;
        // ��� ��������� �������.
        uint8_t ui8SoundSignalType;
    };
#pragma pack(pop)

    void StatusCheck(void);
    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );
    //  ����� ���������� �� �� � ���� - F_HLEV (1�): D0-��. ��, D1-����� ��, D2-��.���/���� - ����. ����.;
//    uint8_t ui8HighLevelFlagsWE;
    THighLevelFlagsExchangePackOne xTHighLevelFlagsExchangePackOne;

//-----------------------------------------------------------------------------------------------------
    CHighLevelFlagsSetWE()
    {
        xTHighLevelFlagsExchangePackOne.ui8HighLevelFlagsWE = 0;
        // ������� �������� ��������� �������.
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
// ��������� ����� �������� ������ �� ����(���: W - �����, ����: E - ������)
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
// �������� ������� �� �������� ������� ������� �� ����(���: W - �����, ����: E - ������)
// ������� ������ ������� � ����.
class CEventLogQuery : public CTaskSerialMT
{
public:

    void FillingMessage(CEvents::TEventDataPackOne *, CEvents::TEventDataCommon *, int16_t );
//    int8_t InfoEvetnsLogCreate(void);
    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );

//    // ������ ���������� �������.
//    static int16_t i16LastRequestIndex;
    // ����������� ������ ������.
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
// �������� ������� �� �������� ������� ���������� � ������� �� ����(���: W - �����, ����: E - ������)
// ������� ������ ���������� � ������� � ����.
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
// �������� ������� �� �������� ������� ���������� ������� �� ����(���: W - �����, ����: E - ������)
// ������� ������ ���������� ������� � ����.
class CDiscreteOutputLogQuery : public CTaskSerialMT
{
public:
    typedef enum
    {
        // ����� ������ �������� ��������� ���������� �������.
        DISCRETE_INPUT_QUERY_LIST_LENGTH = 3,
    };

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
    // ��������� ������ ��������� ���������� �������.
    struct TDiscreteOutputsPackOne
    {
        // ID.
        uint16_t ui16ID;
        // ������ �������������� �����(������) ��������� ���������� �������
        // � �������(��������� ���������� �������).
        int16_t i16Index;
        // ��������� ���������� �������.
        uint8_t ui8State;
    };
#pragma pack(pop)

    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );

    // ������ ��������� ���������� �������.
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
// �������� ������ �� �������� ������������ �� ����(���: W - �����, ����: E - ������)
// ������� ������������ � ����.
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
// �������� ������ �� �������� �������� ������� �� ����(���: W - �����, ����: E - ������)
// ������� ������� ����� � ����.
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
// �������� ������ �� �������� �������� ������� �� ����(���: W - �����, ����: E - ������)
// ������� ������� ����� � ����.
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
    // ��������� ������ ��������.
    struct TPlcSettingsQueryPackOne
    {
        // ��� ������������� ������.
        uint8_t ui8Type;
    };
#pragma pack(pop)

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
    struct TPortSettingsPackOne
    {
        uint8_t ui8BaudRate;
        uint8_t ui8StopBits;
        uint8_t ui8Parity;
    };
#pragma pack(pop)

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
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

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
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
        // ��������� ������ ����������� ����.
        char acChoiceOptionTextData[CHOICE_OPTION_TEXT_LENGTH + END_OF_STRING_LENGTH];
    };
#pragma pack(pop)

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
    struct TPlcSettingsExchangePackOne
    {
        // ��� ������������� ������.
        uint8_t ui8Type;
        // ��� ������.
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
                // ����� �����������.
                uint8_t ui8ChoiceOption;
                // ���������� �����������.
                uint8_t ui8ChoiceOptionNumber;
                // ��������� ������ ����������� ����.
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
        // ��� ������������� ������.
        uint8_t ui8Type;
        // ��� ������.
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
                // ����� �����������.
                uint8_t ui8ChoiceOption;
                // ���������� �����������.
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
// �������� ������� �� �������� ���������� ���������� �������� �� ����(���: W - �����, ����: E - ������)
// ������� ���������� ���������� �������� � ����.
class CAnalogueMeasureQuery : public CTaskSerialMT
{
public:

    uint8_t TransmitProcessing(CSerialMT *, uint8_t );
    uint8_t LoadAnswerData(CSerialMT *, uint8_t );
    uint8_t ReceiveProcessing(CSerialMT *, uint8_t );

//    // ������ ���������� �������.
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
// ������ ��������� ������ ��������� �������� ������������ ����� ������������ �����
// CSerialMT::MAX_RECEIVED_FRAME_LENGTH.
// �������� � ���� �� ���� SPI ���������� � ���������� ������.
// ���� ����� ������������� ����� ������ ����� ������������ �����(CSerialMT::MAX_RECEIVED_FRAME_LENGTH), ��
// ����������� ���� ����� �������.
// ������� ����� ������������� ����� ������ ���� ������ ������������ �����.
// ������������ ����� ������������ ����� �������� �� ������ ��������������.
// � ������ ������������ ����� ������ ��������� ��������� SerialMT �
// ������������ ����� ������������ �����.
// ���� ����� ������ ��������� ��������� SerialMT ������ ������������ �����
// ������������ �����, �� ������ ��������� ������ ���������
// �������� ������������ ����� ������������ �����.
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
