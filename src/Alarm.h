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
// 2 ���� �� ������.
// 00 � �����, 01 � ������������� ������������, 10 � �������� ������������.
// 128-159 (0x80 - 0x9f) ������ ���������� ������������ ����� ���������� ���-
// ����� ��1-��16, �� 2 ���� �� ������ (32);
//#define ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_LENGTH 32
// 160-663(0xa0 - 0x297); ������ ���������� ������������ ���������� ��������
// ��1-��252, �� 2 ���� �� ������ (504);
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
        // ����� ������ ���������� ��� �������(��������� ��������).
        EVENT_NAME_LENGTH = 14
    };

#pragma pack(push)
#pragma pack(1)
    // ��������� ������ ������� ������������ - "������"(��������� ��������� ���������� ��������).
    struct TAlarmEventBriefPackOne
    {
        // ����� ������� (� �����, ����� �
        // (������ ���������� ������������ ���������� �������� - axAlarmHmi))
        uint16_t ui16Address;
        // ��������� �������.
        // D0-D4 - ��� (������): 0,1-24(0-��� ���.�� �����).
        // D6 - ���1 � ��� ������������ ��������� ����������� �������.
        // D7 - ���2 � ��� ������������ ��������� ����������� �������.
        // 00 � ��� ������������
        // 01 � ����������������� ������������
        // 10 � ��������� ������������
        // 11 � ��������� (������ ���� ��� ��������� �������)
        uint8_t ui8State;
    };
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
    // ��������� ������ ������ �������� ������� �������.
    struct TAlarmEventLogQueryListPackOne
    {
        // ������ ������� �������������� ������� � �������(������ ������� - AlarmEventLog).
        uint16_t ui16Index;
        // ���������� ������������� �������.
        int8_t ui8Quantity;
    };
#pragma pack(pop)

// ��������� ������ ������� ������������ - "������"(��������� ��������� ���������� ��������).
    struct TAlarmEventBrief
    {
        // ����� ������� (� �����, ����� �
        // (������ ���������� ������������ ���������� �������� - axAlarmHmi))
        uint16_t ui16Address;
        // ��������� �������.
        // D0-D4 - ��� (������): 0,1-24(0-��� ���.�� �����).
        // D6 - ���1 � ��� ������������ ��������� ����������� �������.
        // D7 - ���2 � ��� ������������ ��������� ����������� �������.
        // 00 � ��� ������������
        // 01 � ����������������� ������������
        // 10 � ��������� ������������
        // 11 � ��������� (������ ���� ��� ��������� �������)
        uint8_t ui8State;
    };

#pragma pack(push)
#pragma pack(1)
// ��������� ������ ������� ������������(��������� ��������� ���������� ��������).
    struct TAlarmEventPackOne
    {
        // ID � ������� ���� ������.
        uint16_t ui16ID;
        // ������ �������������� ������� � �������(������ ������� - AlarmEventLog).
        uint16_t ui16Index;
        // ����� ������� (� �����, ����� �
        // (������ ���������� ������������ ���������� �������� - axAlarmHmi))
        uint16_t ui16Address;
        // ��������� �������.
        // D0-D4 - ��� (������): 0,1-24(0-��� ���.�� �����).
        // D6 - ���1 � ��� ������������ ��������� ����������� �������.
        // D7 - ���2 � ��� ������������ ��������� ����������� �������.
        // 00 � ��� ������������
        // 01 � ����������������� ������������
        // 10 � ��������� ������������
        // 11 � ��������� (������ ���� ��� ��������� �������)
        uint8_t ui8State;
        // ��������� �������� ��������� �������.
        char acTextDescriptor[CAlarm::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH];
        // ����� �������.
        uint8_t ui8Second;
        uint8_t ui8Minute;
        uint8_t ui8Hour;
        uint8_t ui8MonthDay;
        uint8_t ui8Month;
        uint8_t ui8Year;
    };
#pragma pack(pop)

// ��������� ������ ������� ������������(��������� ��������� ���������� ��������).
    struct TAlarmEvent
    {
        // ����� ������� (� �����, ����� �
        // (������ ���������� ������������ ���������� �������� - axAlarmHmi))
        uint16_t ui16Address;
        // ��������� �������.
        // D0-D4 - ��� (������): 0,1-24(0-��� ���.�� �����).
        // D6 - ���1 � ��� ������������ ��������� ����������� �������.
        // D7 - ���2 � ��� ������������ ��������� ����������� �������.
        // 00 � ��� ������������
        // 01 � ����������������� ������������
        // 10 � ��������� ������������
        // 11 � ��������� (������ ���� ��� ��������� �������)
        uint8_t ui8State;
        // ��������� �������� ��������� �������.
        char acTextDescriptor[CAlarm::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH];
        // ����� �������.
        struct tm xCurrentTime;
    };

    // ��������� ������ ���������� ���������.
    struct TAlarmEventsLogControl
    {
        // ������ ������� � ������� axAlarmEventsLog.
        int16_t ui16AlarmEventLogIndex;
        // ������ ���������� ������� � ������� axAlarmEventsLog.
        int16_t ui16AlarmEventLogLastIndex;
        // ������ ��������� �������(���� - 0, ������� �����. ���� - 1, ������� �� �����.);
        uint8_t aui8AlarmEventsState[MAX_HANDLED_DISCRETE_INPUT + MAX_HANDLED_FUNCTION_BLOCK];
    };

    typedef enum
    {
        // ����� ������� "������ �������".
//        SHORT_EVENT_LOG_LENGTH = ((CSerialMT::SERIAL_MT_MESSAGE_BAG_MAX_LENGTH -
//                                   CSerialMT::SERIAL_MT_HEADER_LENGTH) /
//                                  sizeof(struct TAlarmEventBrief))
        EVENT_LOG_LENGTH = 24,
        SHORT_EVENT_LOG_LENGTH = 16,
        // ����� ������ �������� �������.
        ALARM_EVENT_QUERY_LIST_LENGTH = 4,
        ALARM_EVENT_BRIEF_STRUCT_BYTE_LENGTH = 3
    };

    static void AlarmEvetnsLogCreate(void);
    static void PlcOnOffEvetnsCreate(void);

    // ���� ������ ������� �������.
    static uint8_t ui8AlarmEventLogBriefIsUpdated;
    // ����� ������� �������, � �������(��������).
    static uint8_t ui8AlarmEventLogLength;
    // ����� ������� ������� "������", � �������(��������).
    static uint8_t ui8AlarmEventLogBriefLength;
    // ������ "������ ������� - ������".
    static CAlarm::TAlarmEvent axAlarmEventsLog[CAlarm::EVENT_LOG_LENGTH];
    // ������ ������ ���������� ���������.
    static CAlarm::TAlarmEventsLogControl xAlarmEventsLogControl;
    // ������ "������ �������".
    static CAlarm::TAlarmEventBrief axAlarmEventsLogBrief[CAlarm::SHORT_EVENT_LOG_LENGTH];
    // ������ �������� �������.
    static CAlarm::TAlarmEventLogQueryListPackOne xAlarmEventsQueryList;
    // ������ ��� ������� ������ ������� ������� "������".
    static const  CAlarm::TAlarmEvent xAlarmEventNull;
    // ���������� ������� � �������.
    static int16_t i16AlarmEventQueryListQuantity;

public:
    CAlarm()
    {
//        // ������� ������ ������� "������" ������������ ���������� ��������.
//        memset(CAlarm::axAlarmEventsLog,
//               0,
//               sizeof(CAlarm::axAlarmEventsLog));
//        // ������� ������ ������ ���������� ���������.
//        memset((void*)&CAlarm::xAlarmEventsLogControl,
//               0,
//               sizeof(CAlarm::xAlarmEventsLogControl));
//        // ��������� ������ ������� �� ������.
//        CAlarm::xAlarmEventsLogControl.ui16AlarmEventLogIndex = 0;
//        // ��������� ������ ���������� ������� �� ������.
//        xAlarmEventsLogControl.ui16AlarmEventLogLastIndex = 0;
//        // ������� ���� - ������ ������� "������" �������.
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

    // ������� �������������� �������(������).
    uint8_t nui8CycleCounter;
    // ��������� ��������.
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
