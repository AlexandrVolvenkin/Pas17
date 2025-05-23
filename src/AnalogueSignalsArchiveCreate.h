#ifndef ANALOGUESIGNALSARCHIVECREATE_H
#define ANALOGUESIGNALSARCHIVECREATE_H
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <time.h>


class Timer;
class Platform;
class CTask;
class CResources;
class CCommunicationDevice;
class CCommunicationDeviceInterface;
class CSpiCommunicationDevice;
class CDataContainerInterface;
class CDataContainerDataBase;
class CInternalModule;
class CConfigurationCreate;

//-------------------------------------------------------------------------------
class CAnalogueSignalsArchiveCreate : public CTask
{
public:
    enum
    {
        ANALOGUE_SIGNALS_ARCHIVE_CREATE_START = NEXT_STEP,
        ANALOGUE_SIGNALS_ARCHIVE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_START,
        ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,
    };

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

        unsigned int uiHandledDiscreteInputQuantity;
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

    CAnalogueSignalsArchiveCreate();
    virtual ~CAnalogueSignalsArchiveCreate();

    void SetInternalModuleName(std::string sName);

    uint8_t Init(void);
    uint8_t Fsm(void);
    void Allocate(void);

private:
    std::string m_sInternalModuleName;
    uint8_t m_uiInternalModuleId;

    float* m_pfAnalogueInputsValue;
    uint8_t* m_puiAnalogueInputsState;
    uint8_t* m_puiAnalogueInputsOff;
    uint8_t* m_puiAnalogueInputsBadState;

    uint8_t* m_puiIntermediateBuff;

};

#endif // ANALOGUESIGNALSARCHIVECREATE_H
//-------------------------------------------------------------------------------
