#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

//#include "AM335xPlatform.h"
#include "Platform.h"
#include "PasNewConfig.h"
#include "SerialMT.h"

//-----------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------
class CError
{
public:

    typedef enum
    {
        // ����� ������ ���������� ��� ������(��������� ��������).
        ERROR_NAME_LENGTH = 20,
        // ����� ������� "������ ������".
        ERROR_LOG_LENGTH = 24,
        // ���������� ������.
        ERROR_TEXT_DESCRITOR_QUANTITY = 24,
        // ����� ������ �������� ������.
        ERROR_QUERY_LIST_LENGTH = 4,
        ERROR_BRIEF_STRUCT_BYTE_LENGTH = 3
    };

#pragma pack(push)
#pragma pack(1)
    // ��������� ������ �������.
    struct TTime
    {
        uint8_t ui8Second;
        uint8_t ui8Minute;
        uint8_t ui8Hour;
        uint8_t ui8MonthDay;
        uint8_t ui8Month;
        uint8_t ui8Year;
        uint8_t ui8WeekDay;
    };
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
    // ��������� ������ ������ �������� ������� ������.
    struct TErrorLogQueryListPackOne
    {
        // ������ ������ ������������� ������ � �������(������� ������ - ErrorLog).
        uint16_t ui16Index;
        // ���������� ������������� ������.
        int8_t ui8Quantity;
    };
#pragma pack(pop)

//// ��������� ������ ������.
//    struct TErrorBrief
//    {
//
//    };

#pragma pack(push)
#pragma pack(1)
// ��������� ������ ������.
    struct TErrorPackOne
    {
        // ID � ������� ���� ������.
        uint16_t ui16ID;
        // ��� ������������� �������.
        uint8_t ui8Type;
        // ������ �������.
        uint16_t ui16Index;
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
            char acTextData[CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH];
        };
        // ��������� �������� �������.
        char acTextDescriptor[CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH];
//        // ������ ������������� ������ � �������(������� ������ - ErrorLog).
//        uint16_t ui16Index;
////        // ����� ��������� ������.
////        uint16_t ui16Address;
////        // ��������� ������.
////        uint8_t ui8State;
//        // ��������� �������� ������.
//        char acTextDescriptor[CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH];
//        // ����� ������ ������.
//        char acTextData[CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH];
    };
#pragma pack(pop)

// ��������� ������ ������.
    struct TError
    {
        // ����� ��������� ������.
        uint16_t ui16Address;
        // ��������� ������.
        uint8_t ui8State;
        // ��������� �������� ������.
        char acTextDescriptor[CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH];
        // ����� ������ ������.
        char acTextData[CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH];
        // ����� ������.
        struct tm xCurrentTime;
    };

    // ��������� ������ ���������� ����� �������.
    struct TErrorControl
    {
        // ����� ��������� ������.
        int8_t ui16Address;
        // ������� ������������� ������.
        // ��������� ������(���� - 0, ������ �����.).
        int16_t ui16ErrorCounter;
    };

    // ��������� ������ ���������� ��������.
    struct TErrorLogControl
    {
        // ������ ������ � ������� axErrorLog.
        int16_t ui16ErrorLogIndex;
        // ������ ��������� ������ � ������� axErrorLog.
        int16_t ui16ErrorLogLastIndex;
        // ������ ��������� ������(���� - 0, ������ �����. ���� - 1, ������ �� �����.);
        uint8_t aui8ErrorState[CError::ERROR_LOG_LENGTH];
    };

    struct TErrorTextDescriptor
    {
        // ��������� �������� ������.
        char acTextDescriptor[CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH];
    };

    // ���� ������ ������ �������.
    static uint8_t ui8ErrorLogBriefIsUpdated;
    // ����� ������� ������, � �������(�������).
    static uint8_t ui8ErrorLogLength;
    // ����� ������� ������ "������", � �������(�������).
    static uint8_t ui8ErrorLogBriefLength;
    // ������ "������ ������".
    static CError::TError axErrorLog[CError::ERROR_LOG_LENGTH];
    // ������ ������.
    static CError::TErrorPackOne xErrorLog;
    // ������ ������ ���������� ������.
    static CError::TErrorLogControl xErrorLogControl;
    // ������ �������� ������.
    static CError::TErrorLogQueryListPackOne xErrorQueryList;
    // ������ ��� ������� ������ ������� ������.
    static const  CError::TError xErrorNull;
    // ���������� ������ � �������.
    static int16_t i16ErrorQueryListQuantity;
    // ������ ��������� ���������� ������.
    static TErrorTextDescriptor axErrorTextDescriptor[CError::ERROR_TEXT_DESCRITOR_QUANTITY];
    // ������ ��������� ���������� ������ ��� ������.
    static TErrorTextDescriptor axDataBaseErrorTextDescriptor[];//PLC_DATA_BASE_ERROR_LENGTH
    // ���������� ��������� �������.
    static float fBadPacketQuantity;
    // ���������� �������� �������.
    static float fOkPacketQuantity;


    static void ErrorLogCreate(void);
    static uint8_t ModbusErrorToPlcError(int32_t);

public:
    CError()
    {
        // ������� ������ ������.
        memset(CError::axErrorLog,
               0,
               sizeof(CError::axErrorLog));
//
//        for (int i = 0;
//                i < CError::ERROR_LOG_LENGTH;
//                i++)
//        {
//            axErrorLog[i].ui16Address = 7;
//            // ��������� ������ ���������� ���������.
//            memcpy(&(axErrorLog[i].acTextDescriptor),
//                   (&(axErrorTextDescriptor[i].acTextDescriptor)),
//                   (CError::ERROR_LOG_LENGTH));
//        }


        // ������� ������ ������ ���������� ��������.
        memset((void*)&CError::xErrorLogControl,
               0,
               sizeof(CError::xErrorLogControl));
        // ��������� ������ ������ �� ������.
        CError::xErrorLogControl.ui16ErrorLogIndex = 0;
        // ��������� ������ ��������� ������ �� ������.
        xErrorLogControl.ui16ErrorLogLastIndex = 0;
        // ������� ���� - ������ ������ �������.
        CError::ui8ErrorLogBriefIsUpdated = 0;
    }
    ~CError()
    {

    }

private:
};



#endif // ERROR_H_INCLUDED
