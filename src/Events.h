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

//class CEventsDB;
//class CAlarm;
//class CAlarmDfa;

//-----------------------------------------------------------------------------------------------------
class CEvents
{
public:
    typedef enum
    {
        // ����� ������ ���������� ��� �������(��������� ��������).
        EVENT_NAME_LENGTH = 20,
        // ����� ������ ���������� ��� �������(��������� ��������).
        EVENT_TEXT_DATA_LENGTH = 20,
        // ���������� ������������� ���� �������.
        REQUESTED_EVENTS_QUANTITY = 4,
        // ����� ������ �������� �������.
        EVENT_EVENT_QUERY_LIST_LENGTH = 4,

//-----------------------------------------------------------------------------------------------------
        // ��� �������������� ������� ���������� ��������.
        REQUESTED_EVENTS_ALARM_TYPE = 1,
        // ��� �������������� ������� - ������.
        REQUESTED_EVENTS_ERROR_TYPE = 2,
        // ��� �������������� ������� - �����.
        REQUESTED_EVENTS_ARCHIVE_TYPE = 3,
        // ��� �������������� ������� - ���������� �����.
        REQUESTED_EVENTS_ANALOGUE_TYPE = 4,
        // ��� �������������� ������� - ���������� ������.
        REQUESTED_EVENTS_DISCRETE_TYPE = 5,
        // ��� �������������� ������� - ���������� � �������.
        REQUESTED_EVENTS_INFO_TYPE = 6,

        // ��� �������������� ������� - ���������� �����.
        HANDLED_EVENTS_ANALOGUE_TYPE = 1,
        // ��� �������������� ������� ���������� ��������.
        HANDLED_EVENTS_ALARM_TYPE = 2,
        // ��� �������������� ������� ���������� ������.
        HANDLED_EVENTS_DISCRETE_INPUTS_TYPE = 3,
        // ��� �������������� ������� �������������� ������.
        HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE = 4,
        // ��� �������������� ������� ������� ������� �������.
        HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE = 5,
        // ��� �������������� ������� ������� ������� �������.
        HANDLED_EVENTS_EXTERNAL_MODULES_BAD_TYPE = 6,
        // ��� �������������� ������� ������ ������������.
        HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE = 7,
        // ��� �������������� ������� ������ ��� ������.
        HANDLED_EVENTS_SYSTEM_ERROR_TYPE = 8,
        // ��� �������������� ������� - ���������� � �������.
        HANDLED_EVENTS_INFO_TYPE = 9,
        // ��� �������������� ������� - ���������� � ������� � ��������.
        HANDLED_EVENTS_INFO_TIME_TYPE = 10,
        // ��� �������������� ������� - �������������� ���������.
        HANDLED_EVENTS_SYSTEM_EVENTS_TYPE = 11,
        // ��� �������������� ������� - ������� �������.
        HANDLED_EVENTS_MODULES_EVENTS_TYPE = 12,

        // ������� ������������� ������� - �� �������.
        REQUESTED_EVENTS_ORDER_DEFAULT = 0,
        // ������� ������������� ������� - �� ������.
        REQUESTED_EVENTS_ORDER_BY_NUMBER = 1,
        // ������� ������������� ������� - �� ����.
        REQUESTED_EVENTS_ORDER_BY_DATE = 2,

        // ���������� �������������� ������� ���������� ��������.
        HANDLED_EVENTS_ALARM_QUANTITY = (MAX_HANDLED_DISCRETE_INPUT +
                                         MAX_HANDLED_FUNCTION_BLOCK),
        // ���������� �������������� ������� - ������.
        HANDLED_EVENTS_ERROR_QUANTITY = (INTERNAL_MODULE_QUANTITY +
                                         EXTERNAL_MODULE_QUANTITY),
        // ���������� �������������� ������� - �����.
        HANDLED_EVENTS_ARCHIVE_QUANTITY = 64,

//-----------------------------------------------------------------------------------------------------
        // ������� ������������ � ��������� �����.
        // (Index &= Quantity - 1).
        // bufer[Index++] = Data;
        // ��� ������ ����� ���������, ���������� ����������� �������(ui16RecordedEventsQuantity),
        // ������ ���� ����� ������� ������.
        // ���������� ����������� ������� ���������� ��������.
        RECORDED_EVENTS_ALARM_QUANTITY = 8,
        // ���������� ����������� ������� - ������.
        RECORDED_EVENTS_ERROR_QUANTITY = 8,
        // ���������� �������������� ������� - �����.
        RECORDED_EVENTS_ARCHIVE_QUANTITY = 8,

//-----------------------------------------------------------------------------------------------------
        // ���������� �������������� �������.
        HANDLED_EVENTS_QUANTITY = (HANDLED_EVENTS_ALARM_QUANTITY +
                                   HANDLED_EVENTS_ERROR_QUANTITY +
                                   HANDLED_EVENTS_ARCHIVE_QUANTITY),
        // ���������� ����������� �������.
        RECORDED_EVENTS_QUANTITY = 32,
        // ������������ ���������� ������� ������������ � ����� �����.
        ONE_CYCLE_OCCURED_EVENTS_MAXIMUM_NUMBER = 16,
        // ��� ������ ���������� �������.
        CREATED_EVENT_GROUP_TYPE_DISCRETE_INPUT_MODULE = 1,
        CREATED_EVENT_GROUP_TYPE_ANALOGUE_INPUT_MODULE = 2,
        CREATED_EVENT_GROUP_TYPE_DISCRETE_OUTPUT_MODULE = 3,
        CREATED_EVENT_GROUP_TYPE_ANALOGUE_OUTPUT_MODULE = 4,
        // ���� - ������� ������������.
        HANDLED_EVENTS_IS_ARCHIVE = 0x80,
        // ���� - ������� �� ������.
        HANDLED_EVENTS_IS_SOUND = 0x40,
        // ���� - ������� ��������� �� �������.
        HANDLED_EVENTS_IS_POPUP = 0x20,
//        HANDLED_EVENTS_NO_POPUP_NO_SOUND_NO_ARCHIVE = 0x00,
//        HANDLED_EVENTS_POPUP_SOUND_ARCHIVE = 0x40,
//        HANDLED_EVENTS_POPUP_NO_SOUND_NO_ARCHIVE = 0x40,
//        HANDLED_EVENTS_POPUP_SOUND_ARCHIVE = 0x40,
        // ���� - ������� ��������� ��� ������.
        HANDLED_EVENTS_IS_OCCURED_ON_START = 0x10,
    };

//-----------------------------------------------------------------------------------------------------
    // ����� ��������� �������.
    typedef enum
    {
        // ����.
        // ���� - 0, ������� ����������� ��� ������ ���������.
        // ���� - 1, ������� ����������� ����� ���������� � ������ ���������.
        EVENT_FIXED_ONCE = 0
    };

#pragma pack(push)
#pragma pack(1)
// ��������� ������ �������.
    struct TEventDataPackOne
    {
        // ID � ������� ���� ������.
        uint16_t ui16ID;
        // ��� ������������� �������.
        uint8_t ui8Type;
        // ������ �������.
        int16_t i16Index;
        union
        {
            // �������� ������ �������.
            struct
            {
                // ����� ��������� �������.
                uint16_t ui16Address;
                // ��������� �������.
                uint8_t ui8State;
                // ����� �������.
                uint8_t ui8Second;
                uint8_t ui8Minute;
                uint8_t ui8Hour;
                uint8_t ui8MonthDay;
                uint8_t ui8Month;
                uint8_t ui8Year;
            };
            // ��������� ������ �������.
            char acTextData[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];

            struct
            {
                // ����� ������� �������������� ����������� ���������.
                // ����� ������� (� �����, ����� �
                // (���������� �������� ������� ���������� �������� - AI_VALUE_BYTE_ARRAY_OFFSET)
                uint8_t ui8Address;
                // ��������� ������ ����������� ���������.
                uint8_t ui8Status;
                // �������� ����������� ���������.
                float fData;
                // ��������� �������� ����������� ����������� ��������� ����.
                char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
            };
        };
        // ��������� �������� �������.
        char acTextDescriptor[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];
    };

// ��������� ����������� ���������� ������ �������.
    struct TEventDataDinamicPackOne
    {
        // ID � ������� ���� ������.
        uint16_t ui16ID;
//        // ��� ������������� �������.
//        uint8_t ui8Type;
        // ������ �������.
        int16_t i16Index;
        union
        {
            struct
            {
                //                // ����� ������� �������������� ����������� ���������.
//                // ����� ������� (� �����, ����� �
//                // (���������� �������� ������� ���������� �������� - AI_VALUE_BYTE_ARRAY_OFFSET)
//                uint8_t ui8Address;
                // ��������� ������ ����������� ���������.
                uint8_t ui8Status;
                // �������� ����������� ���������.
                float fData;
            };
        };
    };
#pragma pack(pop)

// ��������� ������ �������.
    struct TEventData
    {
        // ������ �������.
        uint16_t ui16Index;
        // ����� ��������� �������.
        uint16_t ui16Address;
        // ��������� �������.
        uint8_t ui8State;
        // ��������� �������� ��������� �������.
        char acTextDescriptor[CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH];
        // ����� �������.
        struct tm xCurrentTime;
    };

    // ��������� ������ ������� ������ ����.
    struct TEventDataCommon
    {
        // ID � ������� ���� ������.
        uint16_t ui16ID;
        // ��� ������������� �������.
        uint8_t ui8Type;
        // ������ �������.
        int16_t i16Index;
        union
        {
            // �������� ������ �������.
            struct
            {
                // ����� ��������� �������.
                uint16_t ui16Address;
                // ��������� �������.
                uint8_t ui8State;
                // ����� �������.
                struct tm xCurrentTime;
            };
            // ��������� ������ �������.
            char acTextData[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];

            struct
            {
                // ����� ������� �������������� ����������� ���������.
                // ����� ������� (� �����, ����� �
                // (���������� �������� ������� ���������� �������� - AI_VALUE_BYTE_ARRAY_OFFSET)
                uint8_t ui8Address;
                // ��������� ������ ����������� ���������.
                uint8_t ui8Status;
                // �������� ����������� ���������.
                float fData;
                // ��������� �������� ����������� ����������� ��������� ����.
                char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
            };
        };
        // ��������� �������� �������.
        char acTextDescriptor[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];
        // �������������� ��������� �������� �������.
        char acTextDescriptorAdditional[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];
    };

#pragma pack(push)
#pragma pack(1)
    // ��������� ������ ������ �������� ������� �������.
    struct TEventsLogQueryListPackOne
    {
        // ������ ������� �������������� ������� � �������(������� �������).
        int16_t i16Index;
        // ��� ������������� �������.
        uint8_t ui8Type;
        // ������� ������������� �������.
        uint8_t ui8Order;
        // ����� ��������� �������.
        union
        {
            uint8_t ui8Address;
            uint16_t ui16Address;
            struct
            {
                // �����  ������� �������������� �������.
//        uint8_t ui8Second;
//        uint8_t ui8Minute;
//        uint8_t ui8Hour;
                uint8_t ui8MonthDay;
                uint8_t ui8Month;
                uint8_t ui8Year;
            };
        };
        // ���������� ������������� �������.
        union
        {
            uint8_t ui8Quantity;
            uint8_t ui8Length;
        };
    };
#pragma pack(pop)

// ��������� ������ ������������� �������.
    struct TOccuredEventsData
    {
        // ��� ������ ���������� �������(�������� ����� ������).
        uint8_t ui8GroupType;
        // ������ ������ ���������� �������(�������� ����� ������).
        uint16_t ui16GroupIndex;
        // ��� ������������� �������.
        uint8_t ui8EventType;
        // ��� ������������� �������.
        uint8_t ui8EventCode;
        // ��������� ������������� �������(1 - �������, 0 - ������� � �����).
        uint8_t ui8State;
        // ��������� �� ��������� �������� �������.
        char* pcTextDescriptor;
    };

// ��������� ������ ������������� �������.
    struct TOccuredEventsDataBriefly
    {
        // ������ ������ ���������� �������(�������� ����� ������).
        uint16_t ui16GroupIndex;
        // ��� ������������� �������.
        uint8_t ui8EventType;
        // ��������� ������������� �������.
        uint8_t ui8EventCode;
        // ��������� �� ��������� �������� �������.
        char* pcTextDescriptor;
    };

// ��������� ���������� ������������� ���������.
    struct TOccuredEventsControl
    {
        // ���������� ������� ������������ � ����� �����.
        uint8_t ui8OccuredEventsNumber;
        // ������ � ������� ������� ������������ � ����� �����.
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
    // �������� ���������� ������� ������������ � ����� �����.
    static void OccuredEventsControlReset(void);
    uint8_t EventOnIsNotRegistered(uint16_t , uint8_t );
    uint8_t EventOffIsNotRegistered(uint16_t , uint8_t );
    static void EventRegistration(uint16_t , uint8_t , uint8_t , char* );
    static void EventsHandler(void);

//-----------------------------------------------------------------------------------------------------
    // ���������� �������������� ������� ������ ����.
    int16_t ui16HandledEventsQuantity;
    // ���������� ����������� ������� ������ ����.
    int16_t ui16RecordedEventsQuantity;
    // ������ �������� �������.
    static CEvents::TEventsLogQueryListPackOne xEventsLogQueryList;
    // ���������� ������� � �������.
    static int16_t i16EventsLogQueryListQuantity;

    // ������ ��������� �������(���� - 0, ������� �����. ���� - 1, ������� �� �����.);
    uint8_t aui8EventsControlState[HANDLED_EVENTS_QUANTITY];
    // ��������� �� ������ ��������� �������.
    uint8_t *pui8EventsControlState;
    // ��������� ���� �������.
    uint8_t ui8EventTypeSetting;
    // ������ ����� �������.
    CEvents::TEventDataCommon axEventDataCommon[RECORDED_EVENTS_QUANTITY];
    // ��������� �� ������ ����� �������.
    TEventDataCommon *pxEventDataCommon;
    // ������ � ������� �������.
    int16_t ui16EventDataCommonPushIndex;
    int16_t ui16EventDataCommonPopIndex;
    // ������ ���������� �� ������� � ������� ����� �������.
    // � �� ��������� ��������� �� ������� � ������� ����� �������(axEventDataCommon[CEvents::RECORDED_EVENTS_QUANTITY]),
    // �� ��������������� ����������� ������� ������ �����.
    // �� ��� �� ������� ����� �������(axEventDataCommon[CEvents::RECORDED_EVENTS_QUANTITY]),
    // ����� �������� ��� ��������������� ����������� ������� ������ �����.
////    static CEvents::TEventDataCommon* apxEventDataCommonPointers[RECORDED_EVENTS_QUANTITY];
    // ��������� �� ������ �������� ������� � ������� ����� �������.
    CEvents::TEventDataCommon** ppxEventDataCommonPointers;
    // ������ ��������������� ������������ ������� ������ �����.
    // � ������� �������� �������(apxEventDataCommonPointers[CEvents::RECORDED_EVENTS_QUANTITY]).
    int16_t ui16EventDataCommonPointersPushIndex;
    int16_t ui16EventDataCommonPointersPopIndex;
    // ������ ���������� ������������� ���������.
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
        // ���������� �������������� ������� � ����� ������.
        HANDLED_EVENTS_QUANTITY = 16,
        // ���������� ����������� �������.
        RECORDED_EVENTS_QUANTITY = 16,
    };

    // ��������� ���������� ��������� ������ ������.
    struct TOneGroupEventsControl
    {
        // ������ ��������� ����� �������(���� - 0, ������� �����. ���� - 1, ������� �� �����.);
        uint8_t aui8EventsTypesControlState[HANDLED_EVENTS_QUANTITY];
    };

//-----------------------------------------------------------------------------------------------------
//private:
    // ������ ��������� �������.
    uint8_t aui8EventsControlState[MAX_MODULE_QUANTITY * HANDLED_EVENTS_QUANTITY];
    TOneGroupEventsControl axAllGroupsEventsControl[MAX_MODULE_QUANTITY];
    // ������ ����� �������.
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
        // ������� ������� �������� - ��� �������.
        for (int i = 0;
                i < (ui16RecordedEventsQuantity);
                i++)
        {
            pxEventDataCommon[i].ui16ID = 0;
        }
        // ������� ������ ����������� ��������� �������.
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
        // ���������� �������������� �������.
        HANDLED_EVENTS_QUANTITY = EXTERNAL_MODULE_QUANTITY,
        // ���������� ����������� �������.
        RECORDED_EVENTS_QUANTITY = 8,
    };

//-----------------------------------------------------------------------------------------------------
private:
    // ������ ��������� �������(���� - 0, ������� �����. ���� - 1, ������� �� �����.);
    uint8_t aui8EventsControlState[HANDLED_EVENTS_QUANTITY];
    // ������ ����� �������.
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
        // ������� ������� �������� - ��� �������.
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
        // ���������� �������������� �������.
        HANDLED_EVENTS_QUANTITY = 2,
        // ���������� ����������� �������.
        RECORDED_EVENTS_QUANTITY = 2,
    };

//-----------------------------------------------------------------------------------------------------
private:
    // ������ ��������� �������(���� - 0, ������� �����. ���� - 1, ������� �� �����.);
    uint8_t aui8EventsControlState[HANDLED_EVENTS_QUANTITY];
    // ������ ����� �������.
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
        // ������� ������� �������� - ��� �������.
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
        // ���������� �������������� �������.
        HANDLED_EVENTS_QUANTITY = 16,
        // ���������� ����������� �������.
        RECORDED_EVENTS_QUANTITY = 16,
    };

//-----------------------------------------------------------------------------------------------------
private:
    // ������ ��������� �������(���� - 0, ������� �����. ���� - 1, ������� �� �����.);
    uint8_t aui8EventsControlState[HANDLED_EVENTS_QUANTITY];
    // ������ ����� �������.
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
        // ������� ������� �������� - ��� �������.
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
        // ���������� �������������� ������� � ����� ������.
        HANDLED_EVENTS_QUANTITY = 1,
        // ���������� ����������� �������.
        RECORDED_EVENTS_QUANTITY = 16,
    };
//-----------------------------------------------------------------------------------------------------
private:
    // ������ ��������� �������(���� - 0, ������� �����. ���� - 1, ������� �� �����.);
    uint8_t aui8EventsControlState[(MAX_HANDLED_DISCRETE_INPUT +
                                    MAX_HANDLED_FUNCTION_BLOCK) * HANDLED_EVENTS_QUANTITY];
    // ������ ����� �������.
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
        // ������� ������� �������� - ��� �������.
        for (int i = 0;
                i < (ui16RecordedEventsQuantity);
                i++)
        {
            pxEventDataCommon[i].ui16ID = 0;
        }
        // ������� ������ ����������� ��������� �������.
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
        // ���������� �������������� �������.
        HANDLED_EVENTS_QUANTITY = 8,
        // ���������� ����������� �������.
        RECORDED_EVENTS_QUANTITY = 8,
    };
//-----------------------------------------------------------------------------------------------------
private:
    // ������ ��������� �������(���� - 0, ������� �����. ���� - 1, ������� �� �����.);
    uint8_t aui8EventsControlState[HANDLED_EVENTS_QUANTITY];
    // ������ ����� �������.
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
        // ������� ������� �������� - ��� �������.
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
        // ���������� �������������� ������� - ������.
        HANDLED_EVENTS_QUANTITY = (INTERNAL_MODULE_QUANTITY +
                                   EXTERNAL_MODULE_QUANTITY),
        // ���������� ����������� �������.
        RECORDED_EVENTS_QUANTITY = 16,
    };

//-----------------------------------------------------------------------------------------------------
// ������������� ������ �� ������ �������.
// � �������� (ui16EventDataCommonPushIndex - 1) ���������� ������ �� ������ ���������� �������.
    void EventDataPopIndexSet(int8_t i8Length)
    {
        ui16EventDataCommonPointersPopIndex = ui16EventDataCommonPointersPushIndex;
        // ������� ������ �� ��������� �������.
        ui16EventDataCommonPointersPopIndex -= i8Length;
        // ��������, ������ ������� ����� �� ������� ������� - ������ �������.
        // ���� ��, �� ��������� ������ ������� �� ������ ������� - ������ �������.
        // ��� ������ ����� ���������, ���������� ����������� �������(ui16RecordedEventsQuantity),
        // ������ ���� ����� ������� ������.
        // �� ����� �� ������� �������? ���� �����, ������� ������.
        ui16EventDataCommonPointersPopIndex &= (ui16RecordedEventsQuantity - 1);
    }

//-----------------------------------------------------------------------------------------------------
// ��������� ��������� �� ��������� �� ������ �������.
    CEvents::TEventDataCommon*
    EventDataPop(void)
    {
        // ��������, ������ ������� ����� �� ������� ������� - ������ �������.
        // ���� ��, �� ��������� ������ ������� �� ������ ������� - ������ �������.
        // ��� ������ ����� ���������, ���������� ����������� �������(ui16RecordedEventsQuantity),
        // ������ ���� ����� ������� ������.
        // �� ����� �� ������� �������? ���� �����, ������� ������.
        ui16EventDataCommonPointersPopIndex &= (ui16RecordedEventsQuantity - 1);
        // ������� ��������� �� ���������, �� ������ �������.
        return (ppxEventDataCommonPointers[ui16EventDataCommonPointersPopIndex--]);
    }

//-----------------------------------------------------------------------------------------------------
private:
    // ������ ���������� �� ������� � ������� ����� �������.
    // � �� ��������� ��������� �� ������� � ������� ����� �������(axEventDataCommon[CEvents::RECORDED_EVENTS_QUANTITY]),
    // �� ��������������� ����������� ������� ������ �����.
    // �� ��� �� ������� ����� �������(axEventDataCommon[CEvents::RECORDED_EVENTS_QUANTITY]),
    // ����� �������� ��� ��������������� ����������� ������� ������ �����.
    CEvents::TEventDataCommon* apxEventDataCommonPointers[RECORDED_EVENTS_QUANTITY];

//-----------------------------------------------------------------------------------------------------

public:
    CPlcErrorEvent()
    {
        ui16RecordedEventsQuantity = RECORDED_EVENTS_QUANTITY;
        ppxEventDataCommonPointers = apxEventDataCommonPointers;
        ui16EventDataCommonPushIndex = 0;
        ui16EventDataCommonPointersPushIndex = 0;
        // �������� ������ ���������� �� ������ ������� ������.
        // �������, ��� ��� �������.
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
        // ���������� ����������� �������.
        RECORDED_EVENTS_QUANTITY = 16,
    };
//-----------------------------------------------------------------------------------------------------
    // ��������� ��������� �� ������ �������.
    // ������������� �����:
    // ����� �������, ������� ������ ���������� � ��������� �����.
    // � ����������(ui16EventDataCommonPushIndex) ��������� ������ �� ��������� �������,
    // ��������� �� ������� �� ���� SQLite3. ��� ����� ������ ������� � ������� �������.
    // �������, ��������� ������ � �������(EventDataPopIndexSet) �� ����� ����� �������
    // � ������� �������, ����� �� ����������(ui16EventDataCommonPushIndex) ����������
    // ��������� �������. � ��������������� �������� ��������� �� ������� �� �������� �
    // ��������, ������������� ����������(ui16EventDataCommonPopIndex).
    CEvents::TEventDataCommon*
    EventDataPop(void)
    {
        // ��������, ������ ������� ����� �� ������� ������� - ������ �������.
        // ���� ��, �� ��������� ������ ������� �� ������ ������� - ������ �������.
        // ��� ������ ����� ���������, ���������� ����������� �������(ui16RecordedEventsQuantity),
        // ������ ���� ����� ������� ������.
        // �� ����� �� ������� �������? ���� �����, ������� ������.
        ui16EventDataCommonPopIndex &= (ui16RecordedEventsQuantity - 1);
        // ������� ��������� �� ������ �������.
        return &(pxEventDataCommon[ui16EventDataCommonPopIndex++]);
    }
//-----------------------------------------------------------------------------------------------------
//    CEvents::TEventDataCommon*
//    EventDataPush(void)
//    {
//        // ��������, ������ ������� ����� �� ������� ������� - ������ �������.
//        // ���� ��, �� ��������� ������ ������� �� ������ ������� - ������ �������.
//        // ��� ������ ����� ���������, ���������� ����������� �������(ui16RecordedEventsQuantity),
//        // ������ ���� ����� ������� ������.
//        // �� ����� �� ������� �������? ���� �����, ������� ������.
//        ui16EventDataCommonPushIndex &=
//            ui16RecordedEventsQuantity - 1;
//        TEventDataCommon *pxEventData;
//        pxEventData = &(pxEventDataCommon[ui16EventDataCommonPushIndex]);
//        // ��������� ������ �� ��������� �������.
//        ui16EventDataCommonPushIndex--;
//        return pxEventData;
//    }

//-----------------------------------------------------------------------------------------------------
private:
    // ������ ����� �������.
    CEvents::TEventDataCommon axEventDataCommon[RECORDED_EVENTS_QUANTITY];
    // ������ ���������� �� ������� � ������� ����� �������.
    // � �� ��������� ��������� �� ������� � ������� ����� �������(axEventDataCommon[CEvents::RECORDED_EVENTS_QUANTITY]),
    // �� ��������������� ����������� ������� ������ �����.
    // �� ��� �� ������� ����� �������(axEventDataCommon[CEvents::RECORDED_EVENTS_QUANTITY]),
    // ����� �������� ��� ��������������� ����������� ������� ������ �����.
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
        // �������� ������ ���������� �� ������ ������� ������.
        // �������, ��� ��� �������.
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

