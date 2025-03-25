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
        // длина строки содержащей имя ошибки(текстовый реквизит).
        ERROR_NAME_LENGTH = 20,
        // длина массива "журнал ошибок".
        ERROR_LOG_LENGTH = 24,
        // количество ошибок.
        ERROR_TEXT_DESCRITOR_QUANTITY = 24,
        // длина списка запросов ошибок.
        ERROR_QUERY_LIST_LENGTH = 4,
        ERROR_BRIEF_STRUCT_BYTE_LENGTH = 3
    };

#pragma pack(push)
#pragma pack(1)
    // структура данных времени.
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
    // структура данных списка запросов журнала ошибок.
    struct TErrorLogQueryListPackOne
    {
        // индекс первой запрашиваемой ошибки в массиве(журнала ошибок - ErrorLog).
        uint16_t ui16Index;
        // количество запрашиваемых ошибок.
        int8_t ui8Quantity;
    };
#pragma pack(pop)

//// структура данных ошибки.
//    struct TErrorBrief
//    {
//
//    };

#pragma pack(push)
#pragma pack(1)
// структура данных ошибки.
    struct TErrorPackOne
    {
        // ID в таблице базы данных.
        uint16_t ui16ID;
        // тип запрашиваемых событий.
        uint8_t ui8Type;
        // индекс события.
        uint16_t ui16Index;
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
            char acTextData[CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH];
        };
        // текстовое описание события.
        char acTextDescriptor[CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH];
//        // индекс запрашиваемой ошибки в массиве(журнала ошибок - ErrorLog).
//        uint16_t ui16Index;
////        // адрес источника ошибки.
////        uint16_t ui16Address;
////        // состояние ошибки.
////        uint8_t ui8State;
//        // текстовый реквизит ошибки.
//        char acTextDescriptor[CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH];
//        // текст данных ошибки.
//        char acTextData[CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH];
    };
#pragma pack(pop)

// структура данных ошибки.
    struct TError
    {
        // адрес источника ошибки.
        uint16_t ui16Address;
        // состояние ошибки.
        uint8_t ui8State;
        // текстовый реквизит ошибки.
        char acTextDescriptor[CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH];
        // текст данных ошибки.
        char acTextData[CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH];
        // время ошибки.
        struct tm xCurrentTime;
    };

    // структура данных управления одной ошибкой.
    struct TErrorControl
    {
        // адрес источника ошибки.
        int8_t ui16Address;
        // счётчик возникновения ошибки.
        // состояния ошибки(если - 0, ошибка новая.).
        int16_t ui16ErrorCounter;
    };

    // структура данных управления ошибками.
    struct TErrorLogControl
    {
        // индекс ошибки в массиве axErrorLog.
        int16_t ui16ErrorLogIndex;
        // индекс последней ошибки в массиве axErrorLog.
        int16_t ui16ErrorLogLastIndex;
        // массив состояния ошибки(если - 0, ошибка новая. если - 1, ошибка не новая.);
        uint8_t aui8ErrorState[CError::ERROR_LOG_LENGTH];
    };

    struct TErrorTextDescriptor
    {
        // текстовый реквизит ошибки.
        char acTextDescriptor[CError::ERROR_NAME_LENGTH + END_OF_STRING_LENGTH];
    };

    // флаг журнал ошибок обновлён.
    static uint8_t ui8ErrorLogBriefIsUpdated;
    // длина журнала ошибок, в строках(ошибках).
    static uint8_t ui8ErrorLogLength;
    // длина журнала ошибок "кратко", в строках(ошибках).
    static uint8_t ui8ErrorLogBriefLength;
    // массив "журнал ошибок".
    static CError::TError axErrorLog[CError::ERROR_LOG_LENGTH];
    // объект ошибка.
    static CError::TErrorPackOne xErrorLog;
    // объект данных управления ошибок.
    static CError::TErrorLogControl xErrorLogControl;
    // список запросов ошибок.
    static CError::TErrorLogQueryListPackOne xErrorQueryList;
    // объект для очистки строки журнала ошибок.
    static const  CError::TError xErrorNull;
    // количество ошибок в запросе.
    static int16_t i16ErrorQueryListQuantity;
    // массив текстовых реквизитов ошибок.
    static TErrorTextDescriptor axErrorTextDescriptor[CError::ERROR_TEXT_DESCRITOR_QUANTITY];
    // массив текстовых реквизитов ошибок баз данных.
    static TErrorTextDescriptor axDataBaseErrorTextDescriptor[];//PLC_DATA_BASE_ERROR_LENGTH
    // количество потеряных пакетов.
    static float fBadPacketQuantity;
    // количество принятых пакетов.
    static float fOkPacketQuantity;


    static void ErrorLogCreate(void);
    static uint8_t ModbusErrorToPlcError(int32_t);

public:
    CError()
    {
        // очистим журнал ошибок.
        memset(CError::axErrorLog,
               0,
               sizeof(CError::axErrorLog));
//
//        for (int i = 0;
//                i < CError::ERROR_LOG_LENGTH;
//                i++)
//        {
//            axErrorLog[i].ui16Address = 7;
//            // скопируем строку текстового реквизита.
//            memcpy(&(axErrorLog[i].acTextDescriptor),
//                   (&(axErrorTextDescriptor[i].acTextDescriptor)),
//                   (CError::ERROR_LOG_LENGTH));
//        }


        // очистим объект данных управления ошибками.
        memset((void*)&CError::xErrorLogControl,
               0,
               sizeof(CError::xErrorLogControl));
        // установим индекс ошибки на начало.
        CError::xErrorLogControl.ui16ErrorLogIndex = 0;
        // установим индекс последней ошибки на начало.
        xErrorLogControl.ui16ErrorLogLastIndex = 0;
        // очистим флаг - журнал ошибок обновлён.
        CError::ui8ErrorLogBriefIsUpdated = 0;
    }
    ~CError()
    {

    }

private:
};



#endif // ERROR_H_INCLUDED
