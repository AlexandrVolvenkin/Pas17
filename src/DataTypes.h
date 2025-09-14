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
    // ����������� ���������� �������.
    uint8_t uiModulesQuantity;
    TModuleContext axModulesContext[INTERNAL_MODULE_QUANTITY];
    uint8_t uiLastDiscreteInputModuleAddresPlusOne;
    uint8_t uiLastAnalogueInputModuleAddresPlusOne;
    uint8_t uiDiscreteOutputQuantity;
    uint8_t uiDiscreteInputSignalsQuantity;
    uint8_t uiAnalogueInputSignalsQuantity;
    uint8_t uiDisplayType;
    uint8_t ui16ErrorCode;
    // ��������� ����������. ����������� ��� ������. �������� �� ���������� ����� ������� ������.
    // ������������ ��� ���������� �������, �������� � �.�.
    // ����������� ������ ����������� ����� ������ ������� ������ 100mc.
    uint8_t uiInternalModulesQuantity;
    uint8_t uiExternalModulesQuantity;

    unsigned int uiHandledDiscreteSignalsQuantity;
    unsigned int uiHandledFunctionBlockQuantity;

    uint8_t uiLastDiscreteInputModuleIndex; // ������� ������� ��������� �� ������� � ����� ������.
    uint8_t uiLastAnalogueInputModuleIndex;
    uint8_t uiLastDiscreteOutputModuleIndex;
    uint8_t uiLastAnalogueOutputModuleIndex;

    uint8_t uiServiceDiscreteInputModuleQuantity; // ���������� �������.
    uint8_t uiServiceAnalogueInputModuleQuantity;
    uint8_t uiServiceDiscreteOutputModuleQuantity;
    uint8_t uiServiceAnalogueOutputModuleQuantity;

    uint8_t uiServiceDiscreteInputQuantity; // ���������� ������-�������.
    uint8_t uiServiceAnalogueInputQuantity;
    uint8_t uiServiceDiscreteOutputQuantity;
    uint8_t uiServiceAnalogueOutputQuantity;
};
//#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct TConfigDataProgrammerPackOne
{
    // ����������� ���������� �������.
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
// ��������� ������ ������
struct TOutputData
{
    unsigned char DiscreteOutputModuleDataBase[DISCRETE_OUTPUT_NUMBER];		// ��������� ��-54
    unsigned char ExternalReceiptAddress[DISCRETE_OUTPUT_NUMBER];		// ������ ������ �������� ������������
    unsigned char ExternalResetAddress[DISCRETE_OUTPUT_NUMBER];		// ������ ������ �������� ������
};

//-----------------------------------------------------------------------------------------------------
struct TDiscreteOutputControl
{
    // ����� ���������.
    uint8_t uiNewActivation;
    // ���������� ��������� ����.
    uint8_t uiRelayActivationRequest;
};

//-----------------------------------------------------------------------------------------------------
struct TAnalogueSignalsArchiveHourData
{
    float fAin1;       // ���������� ������� �����
    float fAin2;       // ���������� ������� �����
    float fAin3;       // ���������� �������� �����
    float fAin4;       // ���������� ���������� �����
    time_t currentTime; // ���������� ��� �������� �������� �������
};


////-----------------------------------------------------------------------------------------------------
//#pragma pack(push)
//#pragma pack(1)
//// ��������� ������ �������.
//    struct TEventDataPackOne
//    {
//        // ID � ������� ���� ������.
//        uint16_t ui16ID;
//        // ��� ������������� �������.
//        uint8_t ui8Type;
//        // ������ �������.
//        int16_t i16Index;
//        union
//        {
//            // �������� ������ �������.
//            struct
//            {
//                // ����� ��������� �������.
//                uint16_t ui16Address;
//                // ��������� �������.
//                uint8_t ui8State;
//                // ����� �������.
//                uint8_t ui8Second;
//                uint8_t ui8Minute;
//                uint8_t ui8Hour;
//                uint8_t ui8MonthDay;
//                uint8_t ui8Month;
//                uint8_t ui8Year;
//            };
//            // ��������� ������ �������.
//            char acTextData[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];
//
//            struct
//            {
//                // ����� ������� �������������� ����������� ���������.
//                // ����� ������� (� �����, ����� �
//                // (���������� �������� ������� ���������� �������� - AI_VALUE_BYTE_ARRAY_OFFSET)
//                uint8_t ui8Address;
//                // ��������� ������ ����������� ���������.
//                uint8_t ui8Status;
//                // �������� ����������� ���������.
//                float fData;
//                // ��������� �������� ����������� ����������� ��������� ����.
//                char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
//            };
//        };
//        // ��������� �������� �������.
//        char acTextDescriptor[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];
//    };
//
//// ��������� ����������� ���������� ������ �������.
//    struct TEventDataDinamicPackOne
//    {
//        // ID � ������� ���� ������.
//        uint16_t ui16ID;
////        // ��� ������������� �������.
////        uint8_t ui8Type;
//        // ������ �������.
//        int16_t i16Index;
//        union
//        {
//            struct
//            {
//                //                // ����� ������� �������������� ����������� ���������.
////                // ����� ������� (� �����, ����� �
////                // (���������� �������� ������� ���������� �������� - AI_VALUE_BYTE_ARRAY_OFFSET)
////                uint8_t ui8Address;
//                // ��������� ������ ����������� ���������.
//                uint8_t ui8Status;
//                // �������� ����������� ���������.
//                float fData;
//            };
//        };
//    };
//#pragma pack(pop)
//
//// ��������� ������ �������.
//    struct TEventData
//    {
//        // ������ �������.
//        uint16_t ui16Index;
//        // ����� ��������� �������.
//        uint16_t ui16Address;
//        // ��������� �������.
//        uint8_t ui8State;
//        // ��������� �������� ��������� �������.
//        char acTextDescriptor[CEvents::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH];
//        // ����� �������.
//        struct tm xCurrentTime;
//    };
//
//    // ��������� ������ ������� ������ ����.
//    struct TEventDataCommon
//    {
//        // ID � ������� ���� ������.
//        uint16_t ui16ID;
//        // ��� ������������� �������.
//        uint8_t ui8Type;
//        // ������ �������.
//        int16_t i16Index;
//        union
//        {
//            // �������� ������ �������.
//            struct
//            {
//                // ����� ��������� �������.
//                uint16_t ui16Address;
//                // ��������� �������.
//                uint8_t ui8State;
//                // ����� �������.
//                struct tm xCurrentTime;
//            };
//            // ��������� ������ �������.
//            char acTextData[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];
//
//            struct
//            {
//                // ����� ������� �������������� ����������� ���������.
//                // ����� ������� (� �����, ����� �
//                // (���������� �������� ������� ���������� �������� - AI_VALUE_BYTE_ARRAY_OFFSET)
//                uint8_t ui8Address;
//                // ��������� ������ ����������� ���������.
//                uint8_t ui8Status;
//                // �������� ����������� ���������.
//                float fData;
//                // ��������� �������� ����������� ����������� ��������� ����.
//                char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
//            };
//        };
//        // ��������� �������� �������.
//        char acTextDescriptor[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];
//        // �������������� ��������� �������� �������.
//        char acTextDescriptorAdditional[CEvents::EVENT_TEXT_DATA_LENGTH + END_OF_STRING_LENGTH];
//    };
//
//#pragma pack(push)
//#pragma pack(1)
//    // ��������� ������ ������ �������� ������� �������.
//    struct TEventsLogQueryListPackOne
//    {
//        // ������ ������� �������������� ������� � �������(������� �������).
//        int16_t i16Index;
//        // ��� ������������� �������.
//        uint8_t ui8Type;
//        // ������� ������������� �������.
//        uint8_t ui8Order;
//        // ����� ��������� �������.
//        union
//        {
//            uint8_t ui8Address;
//            uint16_t ui16Address;
//            struct
//            {
//                // �����  ������� �������������� �������.
////        uint8_t ui8Second;
////        uint8_t ui8Minute;
////        uint8_t ui8Hour;
//                uint8_t ui8MonthDay;
//                uint8_t ui8Month;
//                uint8_t ui8Year;
//            };
//        };
//        // ���������� ������������� �������.
//        union
//        {
//            uint8_t ui8Quantity;
//            uint8_t ui8Length;
//        };
//    };
//#pragma pack(pop)
//
//// ��������� ������ ������������� �������.
//    struct TOccuredEventsData
//    {
//        // ��� ������ ���������� �������(�������� ����� ������).
//        uint8_t ui8GroupType;
//        // ������ ������ ���������� �������(�������� ����� ������).
//        uint16_t ui16GroupIndex;
//        // ��� ������������� �������.
//        uint8_t ui8EventType;
//        // ��� ������������� �������.
//        uint8_t ui8EventCode;
//        // ��������� ������������� �������(1 - �������, 0 - ������� � �����).
//        uint8_t ui8State;
//        // ��������� �� ��������� �������� �������.
//        char* pcTextDescriptor;
//    };
//
//// ��������� ������ ������������� �������.
//    struct TOccuredEventsDataBriefly
//    {
//        // ������ ������ ���������� �������(�������� ����� ������).
//        uint16_t ui16GroupIndex;
//        // ��� ������������� �������.
//        uint8_t ui8EventType;
//        // ��������� ������������� �������.
//        uint8_t ui8EventCode;
//        // ��������� �� ��������� �������� �������.
//        char* pcTextDescriptor;
//    };
//
//// ��������� ���������� ������������� ���������.
//    struct TOccuredEventsControl
//    {
//        // ���������� ������� ������������ � ����� �����.
//        uint8_t ui8OccuredEventsNumber;
//        // ������ � ������� ������� ������������ � ����� �����.
//        TOccuredEventsDataBriefly axOccuredEventsData[ONE_CYCLE_OCCURED_EVENTS_MAXIMUM_NUMBER];
//    };
//-------------------------------------------------------------------------------
// ��������� ��������� ����� �����������.
// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ��������� ����� �����������.
// ����� �������������� �� ������ ������� ���� ������.
struct TDimentionParameterPackOne
{
    // ��������� �������� ����������� � 6 �������� ASCII.
    char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH];
};
#pragma pack(pop)

// ��������� ��������� ����� �����������.
struct TDimentionParameter
{
    // ��������� �������� ����������� � 6 �������� ASCII, ���� ���� - ������� ����� ������.
    char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
};

//-------------------------------------------------------------------------------
// ��������� ���������� - "��������� �������� ����������� �������", � ���� ������.
// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
struct TDiscreteSygnalTextTitlePackOne
{
    // ��������� �������� ����������� ������� � 14 �������� ASCII.
    char acTextDescriptor[DISCRETE_SYGNAL_NAME_LENGTH];
};
#pragma pack(pop)

// ��������� ���������� - "��������� �������� ����������� �������", � ���� ������.
struct TDiscreteSygnalTextTitle
{
// ��������� �������� ����������� ������� � 14 �������� ASCII, ���� ���� - ������� ����� ������.
    char acTextDescriptor[DISCRETE_SYGNAL_NAME_LENGTH + END_OF_STRING_LENGTH];
};

//-------------------------------------------------------------------------------
// ��������� ���������� - "��������� �������� ����������� �������", � ���� ������.
// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
struct TAnalogoueSignalsTextTitlePackOne
{
    // ��������� �������� ����������� ������� � 35 �������� ASCII.
    char acTextDescriptor[ANALOGUE_SYGNAL_NAME_LENGTH];
};
#pragma pack(pop)

// ��������� ���������� - "��������� �������� ����������� �������", � ���� ������.
struct TAnalogoueSignalsTextTitle
{
// ��������� �������� ����������� ������� � 35 �������� ASCII, ���� ���� - ������� ����� ������.
    char acTextDescriptor[ANALOGUE_SYGNAL_NAME_LENGTH + END_OF_STRING_LENGTH];
};

#endif // DATATYPES_H_INCLUDED
