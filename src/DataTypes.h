//-----------------------------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#ifndef DATATYPES_H_INCLUDED
#define DATATYPES_H_INCLUDED

#include "Configuration.h"

//-----------------------------------------------------------------------------------------------------
#pragma pack(push)
#pragma pack(1)
struct TModuleContext
{
    uint8_t uiType;
    uint8_t uiAddress;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------------------------------
//#pragma pack(push)
//#pragma pack(1)
struct TConfigDataPackOne
{
    // фактическое количество модулей.
    uint8_t uiModulesQuantity;
    TModuleContext axModulesContext[INTERNAL_MODULE_QUANTITY];
    uint8_t uiLastDiscreteInputModuleAddresPlusOne;
    uint8_t uiLastAnalogueInputModuleAddresPlusOne;
    uint8_t uiDiscreteOutputQuantity;
    uint8_t uiDiscreteInputSignalsQuantity;
    uint8_t uiAnalogueInputSignalsQuantity;
    uint8_t uiDisplayType;
    uint8_t ui16ErrorCode;
    // служебные переменные. вычисляются при старте. хранятся на протяжении всего времени работы.
    // используются для вычисления адресов, смещений и т.д.
    // целостность данных проверяется перед каждым рабочим циклом 100mc.
    uint8_t uiInternalModulesQuantity;
    uint8_t uiExternalModulesQuantity;

    unsigned int uiHandledDiscreteSignalsQuantity;
    unsigned int uiHandledFunctionBlockQuantity;

    uint8_t uiLastDiscreteInputModuleIndex; // индексы модулей последних по порядку в своей группе.
    uint8_t uiLastAnalogueInputModuleIndex;
    uint8_t uiLastDiscreteOutputModuleIndex;
    uint8_t uiLastAnalogueOutputModuleIndex;

    uint8_t uiServiceDiscreteInputModuleQuantity; // количество модулей.
    uint8_t uiServiceAnalogueInputModuleQuantity;
    uint8_t uiServiceDiscreteOutputModuleQuantity;
    uint8_t uiServiceAnalogueOutputModuleQuantity;

    uint8_t uiServiceDiscreteInputQuantity; // количество входов-выходов.
    uint8_t uiServiceAnalogueInputQuantity;
    uint8_t uiServiceDiscreteOutputQuantity;
    uint8_t uiServiceAnalogueOutputQuantity;
};
//#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct TConfigDataProgrammerPackOne
{
    // фактическое количество модулей.
    uint8_t uiModulesQuantity;
    uint8_t auiModulesContext[INTERNAL_MODULE_QUANTITY];
    uint8_t uiLastDiscreteInputModuleAddresPlusOne;
    uint8_t uiLastAnalogueInputModuleAddresPlusOne;
    uint8_t uiDiscreteOutputQuantity;
    uint8_t uiDiscreteInputSignalsQuantity;
    uint8_t uiAnalogueInputSignalsQuantity;
    uint8_t uiDisplayType;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------------------------------
enum
{
    FLASH_CONNECTOR_EMPTY = 0,
    FLASH_CONNECTOR_INSERTED,
};

#pragma pack(push)
#pragma pack(1)
struct TDeviceStateDataPackOne
{
    uint8_t uiFlashConnectorStatus;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------------------------------
#pragma pack(push)
#pragma pack(1)
struct TRegulatorsDacDataPackOne
{
    uint16_t uiRegulatorDacData1;
    uint16_t uiRegulatorDacData2;
};
#pragma pack(pop)


//-----------------------------------------------------------------------------------------------------
typedef enum
{
    WRITE_IDDLE = 0,
    // запись завершена с ошибкой
    WRITE_ERROR = 1,
    // запись завершена успешно
    WRITE_OK = 2,
    // идёт процесс записи
    WRITE_BUSY = 3
};

#pragma pack(push)
#pragma pack(1)
struct TFileSaveStateDataPackOne
{
    uint8_t uiFileSaveState;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------------------------------
// Описатель канала вывода
struct TOutputData
{
    unsigned char DiscreteOutputModuleDataBase[DISCRETE_OUTPUT_NUMBER];		// Настройки МР-54
    unsigned char ExternalReceiptAddress[DISCRETE_OUTPUT_NUMBER];		// Адреса входов внешнего квитирования
    unsigned char ExternalResetAddress[DISCRETE_OUTPUT_NUMBER];		// Адреса входов внешнего сброса
};

//-----------------------------------------------------------------------------------------------------
struct TDiscreteOutputControl
{
    // Новое нарушение.
    uint8_t uiNewActivation;
    // Требование включения реле.
    uint8_t uiRelayActivationRequest;
};

//-----------------------------------------------------------------------------------------------------
struct TAnalogueSignalsArchiveHourData
{
    float fAin1;       // Переменная первого входа
    float fAin2;       // Переменная второго входа
    float fAin3;       // Переменная третьего входа
    float fAin4;       // Переменная четвертого входа
    time_t currentTime; // Переменная для хранения текущего времени
};


////-----------------------------------------------------------------------------------------------------
//#pragma pack(push)
//#pragma pack(1)
//// структура данных события.
//    struct TEventDataPackOne
//    {
//        // ID в таблице базы данных.
//        uint16_t ui16ID;
//        // тип запрашиваемых событий.
//        uint8_t ui8Type;
//        // индекс события.
//        int16_t i16Index;
//        union
//        {
//            // числовые данные события.
//            struct
//            {
//                // адрес источника события.
//                uint16_t ui16Address;
//                // состояние события.
//                uint8_t ui8State;
//                // время события.
//                uint8_t ui8Second;
//                uint8_t ui8Minute;
//                uint8_t ui8Hour;
//                uint8_t ui8MonthDay;
//                uint8_t ui8Month;
//                uint8_t ui8Year;
//            };
//            // текстовые данные события.
//            char acTextData[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];
//
//            struct
//            {
//                // адрес первого запрашиваемого аналогового измерения.
//                // адрес сигнала (№ входа, адрес в
//                // (измеренные значения входных аналоговых сигналов - AI_VALUE_BYTE_ARRAY_OFFSET)
//                uint8_t ui8Address;
//                // состояние канала аналогового измерения.
//                uint8_t ui8Status;
//                // значение аналогового измерения.
//                float fData;
//                // Текстовый реквизит размерности измеряемого параметра КРЗМ.
//                char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
//            };
//        };
//        // текстовое описание события.
//        char acTextDescriptor[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];
//    };
//
//// структура динамически изменяемых данных события.
//    struct TEventDataDinamicPackOne
//    {
//        // ID в таблице базы данных.
//        uint16_t ui16ID;
////        // тип запрашиваемых событий.
////        uint8_t ui8Type;
//        // индекс события.
//        int16_t i16Index;
//        union
//        {
//            struct
//            {
//                //                // адрес первого запрашиваемого аналогового измерения.
////                // адрес сигнала (№ входа, адрес в
////                // (измеренные значения входных аналоговых сигналов - AI_VALUE_BYTE_ARRAY_OFFSET)
////                uint8_t ui8Address;
//                // состояние канала аналогового измерения.
//                uint8_t ui8Status;
//                // значение аналогового измерения.
//                float fData;
//            };
//        };
//    };
//#pragma pack(pop)
//
//// структура данных события.
//    struct TEventData
//    {
//        // индекс события.
//        uint16_t ui16Index;
//        // адрес источника события.
//        uint16_t ui16Address;
//        // состояние события.
//        uint8_t ui8State;
//        // текстовый реквизит источника события.
//        char acTextDescriptor[CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH];
//        // время события.
//        struct tm xCurrentTime;
//    };
//
//    // структура данных события общего типа.
//    struct TEventDataCommon
//    {
//        // ID в таблице базы данных.
//        uint16_t ui16ID;
//        // тип запрашиваемых событий.
//        uint8_t ui8Type;
//        // индекс события.
//        int16_t i16Index;
//        union
//        {
//            // числовые данные события.
//            struct
//            {
//                // адрес источника события.
//                uint16_t ui16Address;
//                // состояние события.
//                uint8_t ui8State;
//                // время события.
//                struct tm xCurrentTime;
//            };
//            // текстовые данные события.
//            char acTextData[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];
//
//            struct
//            {
//                // адрес первого запрашиваемого аналогового измерения.
//                // адрес сигнала (№ входа, адрес в
//                // (измеренные значения входных аналоговых сигналов - AI_VALUE_BYTE_ARRAY_OFFSET)
//                uint8_t ui8Address;
//                // состояние канала аналогового измерения.
//                uint8_t ui8Status;
//                // значение аналогового измерения.
//                float fData;
//                // Текстовый реквизит размерности измеряемого параметра КРЗМ.
//                char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
//            };
//        };
//        // текстовое описание события.
//        char acTextDescriptor[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];
//        // дополнительное текстовое описание события.
//        char acTextDescriptorAdditional[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];
//    };
//
//#pragma pack(push)
//#pragma pack(1)
//    // структура данных списка запросов журнала событий.
//    struct TEventsLogQueryListPackOne
//    {
//        // индекс первого запрашиваемого события в массиве(журнала событий).
//        int16_t i16Index;
//        // тип запрашиваемых событий.
//        uint8_t ui8Type;
//        // порядок представления события.
//        uint8_t ui8Order;
//        // адрес источника события.
//        union
//        {
//            uint8_t ui8Address;
//            uint16_t ui16Address;
//            struct
//            {
//                // время  первого запрашиваемого события.
////        uint8_t ui8Second;
////        uint8_t ui8Minute;
////        uint8_t ui8Hour;
//                uint8_t ui8MonthDay;
//                uint8_t ui8Month;
//                uint8_t ui8Year;
//            };
//        };
//        // количество запрашиваемых событий.
//        union
//        {
//            uint8_t ui8Quantity;
//            uint8_t ui8Length;
//        };
//    };
//#pragma pack(pop)
//
//// структура данных произошедшего события.
//    struct TOccuredEventsData
//    {
//        // тип группы породившей событие(например номер модуля).
//        uint8_t ui8GroupType;
//        // индекс группы породившей событие(например номер модуля).
//        uint16_t ui16GroupIndex;
//        // тип произошедшего события.
//        uint8_t ui8EventType;
//        // код произошедшего события.
//        uint8_t ui8EventCode;
//        // состояние произошедшего события(1 - активно, 0 - переход в норму).
//        uint8_t ui8State;
//        // указатель на текстовое описание события.
//        char* pcTextDescriptor;
//    };
//
//// структура данных произошедшего события.
//    struct TOccuredEventsDataBriefly
//    {
//        // индекс группы породившей событие(например номер модуля).
//        uint16_t ui16GroupIndex;
//        // тип произошедшего события.
//        uint8_t ui8EventType;
//        // состояние произошедшего события.
//        uint8_t ui8EventCode;
//        // указатель на текстовое описание события.
//        char* pcTextDescriptor;
//    };
//
//// структура управления произошедшими событиями.
//    struct TOccuredEventsControl
//    {
//        // количество событий произошедших в одном цикле.
//        uint8_t ui8OccuredEventsNumber;
//        // массив с данными событий произошедших в одном цикле.
//        TOccuredEventsDataBriefly axOccuredEventsData[ONE_CYCLE_OCCURED_EVENTS_MAXIMUM_NUMBER];
//    };
//-------------------------------------------------------------------------------
// структура описателя одной размерности.
// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура описателя одной размерности.
// после преобразования из общего формата базы данных.
struct TDimentionParameterPackOne
{
    // Текстовый реквизит размерности – 6 символов ASCII.
    char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH];
};
#pragma pack(pop)

// структура описателя одной размерности.
struct TDimentionParameter
{
    // Текстовый реквизит размерности – 6 символов ASCII, плюс нуль - признак конца строки.
    char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
};

//-------------------------------------------------------------------------------
// Структура описателей - "Текстовый реквизит дискретного сигнала", в базе данных.
// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
struct TDiscreteSygnalTextTitlePackOne
{
    // Текстовый реквизит дискретного сигнала – 14 символов ASCII.
    char acTextDescriptor[DISCRETE_SYGNAL_NAME_LENGTH];
};
#pragma pack(pop)

// Структура описателей - "Текстовый реквизит дискретного сигнала", в базе данных.
struct TDiscreteSygnalTextTitle
{
// Текстовый реквизит дискретного сигнала – 14 символов ASCII, плюс нуль - признак конца строки.
    char acTextDescriptor[DISCRETE_SYGNAL_NAME_LENGTH + END_OF_STRING_LENGTH];
};

//-------------------------------------------------------------------------------
// Структура описателей - "Текстовый реквизит аналогового сигнала", в базе данных.
// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
struct TAnalogoueSignalsTextTitlePackOne
{
    // Текстовый реквизит аналогового сигнала – 35 символов ASCII.
    char acTextDescriptor[ANALOGUE_SYGNAL_NAME_LENGTH];
};
#pragma pack(pop)

// Структура описателей - "Текстовый реквизит аналогового сигнала", в базе данных.
struct TAnalogoueSignalsTextTitle
{
// Текстовый реквизит аналогового сигнала – 35 символов ASCII, плюс нуль - признак конца строки.
    char acTextDescriptor[ANALOGUE_SYGNAL_NAME_LENGTH + END_OF_STRING_LENGTH];
};

//-----------------------------------------------------------------------------------------------------
// структуры сохранения и обмена параметров настроек

//typedef enum
//{
//    RS485_HIGH_LEVEL_SET = 1,
//    RS485_LOW_LEVEL_SET,
//    ARCHIVE_LOG_SAVE,
//    ETHERNET_SET,
//    PROGRAMM_FILE_UPDATE,
//    DEBUG_LOG_FILE_SAVE
//};
//
//typedef enum
//{
//    CHOICE_OPTION_TEXT_LENGTH = 14,
//    CHOICE_OPTION_TEXT_NUMBER = 2,
//    ETHERNET_IP_TEXT_LENGTH = 16,
//};

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

//#pragma pack(push)
//#pragma pack(1)
//// структура данных запросов.
//struct TPlcSettingsQueryPackOne
//{
//    // тип запрашиваемых данных.
//    uint8_t ui8Type;
//};
//#pragma pack(pop)

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
    uint8_t ui8IpByte3;
    uint8_t ui8IpByte2;
    uint8_t ui8IpByte1;
    uint8_t ui8IpByte0;
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
    TEthernetSettingsPackOne xTEthernetSettingsPackOne;
//    TPeripherySettingsPackOne xTPeripherySettingsPackOne;
};
#pragma pack(pop)

//#pragma pack(push)
//#pragma pack(1)
//struct TChoiceOptionTextData
//{
//    // текстовые данные предложения меню.
//    char acChoiceOptionTextData[CHOICE_OPTION_TEXT_LENGTH + END_OF_STRING_LENGTH];
//};
//#pragma pack(pop)
//
//// для сериализации, разбора данных полученных по интерфейсам передачи данных.
//#pragma pack(push)
//#pragma pack(1)
//struct TPlcSettingsExchangePackOne
//{
//    // тип запрашиваемых данных.
//    uint8_t ui8Type;
//    // код ответа.
//    uint8_t ui8AnswerCode;
//    union
//    {
//        struct
//        {
//            uint8_t ui8BaudRate;
//            uint8_t ui8StopBits;
//            uint8_t ui8Parity;
//        };
//        struct
//        {
//            uint8_t ui8IpByte0;
//            uint8_t ui8IpByte1;
//            uint8_t ui8IpByte2;
//            uint8_t ui8IpByte3;
//            uint16_t ui16Port;
//        };
//        struct
//        {
//            // номер предложения.
//            uint8_t ui8ChoiceOption;
//            // количество предложений.
//            uint8_t ui8ChoiceOptionNumber;
//            // текстовые данные предложения меню.
//            TChoiceOptionTextData axChoiceOptionTextData[CHOICE_OPTION_TEXT_NUMBER];
//        };
//        struct
//        {
//            uint8_t ui8ZummerIsOn;
//        };
//    };
//};
//
//struct TPlcSettingsExchangeClientPackOne
//{
//    // тип запрашиваемых данных.
//    uint8_t ui8Type;
//    // код ответа.
//    uint8_t ui8AnswerCode;
//    union
//    {
//        struct
//        {
//            uint8_t ui8BaudRate;
//            uint8_t ui8StopBits;
//            uint8_t ui8Parity;
//        };
//        struct
//        {
//            uint8_t ui8IpByte0;
//            uint8_t ui8IpByte1;
//            uint8_t ui8IpByte2;
//            uint8_t ui8IpByte3;
//            uint16_t ui16Port;
//        };
//        struct
//        {
//            // номер предложения.
//            uint8_t ui8ChoiceOption;
//            // количество предложений.
//            uint8_t ui8ChoiceOptionNumber;
//        };
//        struct
//        {
//            uint8_t ui8ZummerIsOn;
//        };
//    };
//};
//#pragma pack(pop)

#endif // DATATYPES_H_INCLUDED
