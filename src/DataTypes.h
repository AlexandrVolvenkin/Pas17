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

// Описатель канала вывода
struct TOutputData
{
    unsigned char DiscreteOutputModuleDataBase[DISCRETE_OUTPUT_NUMBER];		// Настройки МР-54
    unsigned char ExternalReceiptAddress[DISCRETE_OUTPUT_NUMBER];		// Адреса входов внешнего квитирования
    unsigned char ExternalResetAddress[DISCRETE_OUTPUT_NUMBER];		// Адреса входов внешнего сброса
};

struct TDiscreteOutputControl
{
    // Новое нарушение.
    uint8_t uiNewActivation;
    // Требование включения реле.
    uint8_t uiRelayActivationRequest;
};

struct TAnalogueSignalsArchiveHourData
{
    float fAin1;       // Переменная первого входа
    float fAin2;       // Переменная второго входа
    float fAin3;       // Переменная третьего входа
    float fAin4;       // Переменная четвертого входа
    time_t currentTime; // Переменная для хранения текущего времени
};

#endif // DATATYPES_H_INCLUDED
