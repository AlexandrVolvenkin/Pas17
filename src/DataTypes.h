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

// ��������� ������ ������
struct TOutputData
{
    unsigned char DiscreteOutputModuleDataBase[DISCRETE_OUTPUT_NUMBER];		// ��������� ��-54
    unsigned char ExternalReceiptAddress[DISCRETE_OUTPUT_NUMBER];		// ������ ������ �������� ������������
    unsigned char ExternalResetAddress[DISCRETE_OUTPUT_NUMBER];		// ������ ������ �������� ������
};

struct TDiscreteOutputControl
{
    // ����� ���������.
    uint8_t uiNewActivation;
    // ���������� ��������� ����.
    uint8_t uiRelayActivationRequest;
};

struct TAnalogueSignalsArchiveHourData
{
    float fAin1;       // ���������� ������� �����
    float fAin2;       // ���������� ������� �����
    float fAin3;       // ���������� �������� �����
    float fAin4;       // ���������� ���������� �����
    time_t currentTime; // ���������� ��� �������� �������� �������
};

#endif // DATATYPES_H_INCLUDED
