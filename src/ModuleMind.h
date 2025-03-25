#ifndef MODULEMIND_H_INCLUDED
#define MODULEMIND_H_INCLUDED

//#include "AM335xPlatform.h"
#include "PasNewConfig.h"
#include <stdint.h>
#include "SerialMT.h"

//-----------------------------------------------------------------------------------------------------
// MIND
// ���� ���� ������.
// ��������. //?
#define MODULE_MIND_MODBUS_ADDRESS 0x11
#define MODULE_TYPE_MIND 0x11
#define MODULE_MIND_INDEX (INTERNAL_MODULE_QUANTITY + EXTERNAL_MODULE_QUANTITY)
// $42 - ������ �� �������� (�����������) ����� �������.
#define MIND_DATA_EXCHANGE_COMMAND 0x42
#define MIND_DATA_EXCHANGE_READY 0x42
#define MIND_DATA_EXCHANGE_NOT_READY 0x24
#define MIND_DATA_EXCHANGE_COMMAND_ANSWER_LENGTH 5
// $88 - ������ �� �������� ������������.
#define MIND_CONFIGURATION_LOAD_COMMAND 0x88
#define MIND_CONFIGURATION_LOAD_COMMAND_ANSWER_LENGTH 15
#define MIND_INDICATION_MODULE_TYPE_12_CELLS_LCD_4x24 0x88
#define MIND_INDICATION_MODULE_TYPE_24_CELLS_LCD_4x24 0x89
#define MIND_INDICATION_MODULE_TYPE_LCD_320x240 0x8A
#define MIND_INDICATION_MODULE_TYPE_LCD_4x24 0x8B

//-----------------------------------------------------------------------------------------------------
// (���: W - �����, ����: E - ������)
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
        // ������������ �����(���������� ������) �������� ������.
        TASK_NO_ANSWER_TIMEOUT = 3,
        // ������������ �����(���������� ������) �������� ������ ��� �������� �����.
        TASK_PING_NO_ANSWER_TIMEOUT = 50,
        TASK_HIGH_LEVEL_FLAGS_MESSAGE_LENGTH = 1,
        TASK_CONFIGURATION_REQUEST_MESSAGE_LENGTH = 1,
        TASK_PING_REQUEST_MESSAGE_LENGTH = 1,
        HIGH_LEVEL_FLAGS_KVIT_BIT = 0,
        HIGH_LEVEL_FLAGS_RESET_BIT = 1,
        HIGH_LEVEL_FLAGS_BLOCK_BIT = 2,
        // ����� ������ �������� ���������� ���������.
        ANALOGUE_QUERY_MEASURE_LIST_LENGTH = 6,
        // ����� ������ �������� ��������� ���������� �������.
        DISCRETE_INPUT_QUERY_LIST_LENGTH = 3,
//        // ����� ������ ���������� ���������.
//        ANALOGUE_INPUT_NAME_LENGTH = 8,
//        // ����� ������ ���������� ��������� ����������� ����������� ��������� ����.
//        DIMENSIONS_PARAMETERS_NAME_LENGTH = 6,
    };

//-----------------------------------------------------------------------------------------------------
    struct TTaskContext
    {
        // ���� - ��������� ����������.
        uint8_t fui8MessageIsSent;
        // ��������� ������.
        uint8_t ui8Status;
        // ��������������� ������� �������������� �������(������).
        uint8_t nui8TimeCounter;
        // ��������� �� �������-������, ��������� � �������.
        int16_t (*pfi16Task)(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
    };

#pragma pack(push)
#pragma pack(1)
    // ��������� ������ ���������� ���������.
    struct TAnalogueMeasurePackOne
    {
        // ����� ������� �������������� ����������� ���������.
        // ����� ������� (� �����, ����� �
        // (���������� �������� ������� ���������� �������� - AI_VALUE_BYTE_ARRAY_OFFSET)
        uint8_t ui8Address;
        // ��������� ������ ����������� ���������.
        uint8_t ui8Status;
        // �������� ����������� ���������.
        float fData;
        // ��������� �������� ����� � 8 �������� ASCII, ���� ���� - ������� ����� ������.
        char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
        // ��������� �������� ����������� � 6 �������� ASCII, ���� ���� - ������� ����� ������.
        char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
    };

    // ��������� ������ ������ �������� ���������� ���������.
    struct TAnalogueMeasureQueryListPackOne
    {
        // ����� ������� �������������� ����������� ���������.
        // ����� ������� (� �����, ����� �
        // (���������� �������� ������� ���������� ��������)
        uint8_t ui8Address;
        // ���������� ������������� ���������� ���������.
        int8_t ui8Quantity;
    };

    // ��������� ������ ������ �������� ��������� ���������� �������.
    struct TDiscreteOutputsQueryListPackOne
    {
        // ������ ������� �������������� �����(������) ��������� ���������� �������
        // � �������(��������� ���������� �������).
        uint16_t ui16Index;
        // ���������� ������������� ����(�������) ��������� ���������� �������.
        int8_t ui8Quantity;
    };

    // ��������� ������ ��������� ���������� �������.
    struct TDiscreteOutputsPackOne
    {
        // ������ �������������� �����(������) ��������� ���������� �������
        // � �������(��������� ���������� �������).
        uint16_t ui16Index;
        // ��������� ���������� �������.
        uint8_t ui8State;
    };
#pragma pack(pop)

// ��������� ������ ���������� ���������.
    struct TAnalogueMeasure
    {
        // ����� ������� �������������� ����������� ���������.
        // ����� ������� (� �����, ����� �
        // (���������� �������� ������� ���������� �������� - AI_VALUE_BYTE_ARRAY_OFFSET)
        uint8_t ui8Address;
        // ��������� ������ ����������� ���������.
        uint8_t ui8Status;
        // �������� ����������� ���������.
        float fData;
        // ��������� �������� ����� � 8 �������� ASCII, ���� ���� - ������� ����� ������.
        char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
        // ��������� �������� ����������� ����������� ��������� ����.
        char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
    };

//-----------------------------------------------------------------------------------------------------
private:
    // ������ � ���������� �����.
    // ������ ��������� ������ ������������ ������� ������ �� ��������� ���������.
    static CMind::TTaskContext axAllTasksContext[];
    // ���������� �������-����� � ������� ��������� �����.
    static int16_t i16TasksQuantity;
    //  ����� ���������� �� �� - F_HLEV (1�): D0-��. ��, D1-����� ��, D2-��.���/���� - ����. ����.;
    static uint8_t ui8HighLevelFlags;

public:
    //  ����� ���������� �� �� � ���� - F_HLEV (1�): D0-��. ��, D1-����� ��, D2-��.���/���� - ����. ����.;
    static uint8_t ui8HighLevelFlagsGet;
    //  ����� ���������� �� ���� � �� - F_HLEV (1�): D0-��. ��, D1-����� ��, D2-��.���/���� - ����. ����.;
    static uint8_t ui8HighLevelFlagsSet;
    static uint8_t ui8HighLevelFlagsSetSave;
    // ��������� ������ ���������� �� ���� � �� - F_HLEV ���������.
    static uint8_t ui8HighLevelFlagsIsUpdated;
    // ������ �������� ���������� ���������.
    static CMind::TAnalogueMeasureQueryListPackOne xAnalogueMeasureQueryList;
    static CMind::TAnalogueMeasureQueryListPackOne axAnalogueMeasureQueryList[CMind::ANALOGUE_QUERY_MEASURE_LIST_LENGTH];
//    // ������ ���������� ���������.
//    static CMind::TAnalogueMeasure axAnalogueMeasure[CMind::ANALOGUE_QUERY_MEASURE_LIST_LENGTH];
    // ���������� ���������� ��������� � �������.
    static int16_t i16AnalogueMeasureQueryListQuantity;
    // ���������� ���������� ���������� ���������.
    static int16_t i16AnalogueMeasureQuantity;
    // ���� ������ �������� ���������� ��������� �������.
    static uint8_t ui8AnalogueMeasureQueryListIsUpdated;
    // ���� ������������ ���������.
    static uint8_t ui8ConfigurationIsUpdated;
    // ������ �������� ��������� ���������� �������.
    static CMind::TDiscreteOutputsQueryListPackOne xDiscreteOutputsQueryList;
    // ������ ��������� ���������� �������.
    static uint8_t ui8DiscreteOutputsLog[CMind::DISCRETE_INPUT_QUERY_LIST_LENGTH];
    // ���������� ����(�������) ��������� ���������� ������� � �������.
    static int16_t i16DiscreteOutputsQueryListQuantity;
    // ���� ��������� ���������� ������� ���������.
    static uint8_t ui8DiscreteOutputsIsUpdated;
    // ��������� ������ ������� � ������ ��������.
    static uint16_t ui16LastIndexQuery;
    // ����� ���������� ������� ���������� �� ����.
    static int16_t i16AllReceivedRecordsQuantity;
    // ����� ���������� ������� ���������� �� ���� � ���������� �������.
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
    // ������-��������.
    // ���������� �� ��������� �� ������� � ���������� ����� �� ������-������� 0.
    // ����� ��� ������ ������� �� ��������� �� �������� ������.
    // �����: 0 - �� �������� ����������(SerialMT).
    static int16_t NopTask(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);
    // ������ ��������� ������ ��������� �������� ������������ ����� ������������ �����
    // CSpiMT::MAX_RECEIVED_FRAME_LENGTH.
    // �������� � ���� �� ���� SPI ���������� � ���������� ������.
    // ���� ����� ������������� ����� ������ ����� ������������ �����(CSpiMT::MAX_RECEIVED_FRAME_LENGTH), ��
    // ����������� ���� ����� �������.
    // ������� ����� ������������� ����� ������ ���� ������ ������������ �����.
    // ������������ ����� ������������ ����� �������� �� ������ ��������������.
    // � ������ ������������ ����� ������ ��������� ��������� SerialMT �
    // ������������ ����� ������������ �����.
    // ���� ����� ������ ��������� ��������� SerialMT ������ ������������ �����
    // ������������ �����, �� ������ ��������� ������ ���������
    // �������� ������������ ����� ������������ �����.
    static int16_t ReceiveRequest(struct TModuleContext *, CMind::TTaskContext *, CSerialMT::TSerialMTData *);

//-----------------------------------------------------------------------------------------------------
    CMind()
    {
        // ������� ����� ���������� �� ��.
        CMind::ui8HighLevelFlags = 0;
    }
    ~CMind()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
#endif // MODULEMIND_H_INCLUDED
