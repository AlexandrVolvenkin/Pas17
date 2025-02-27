#ifndef CCONFIGURATIONCREATE_H
#define CCONFIGURATIONCREATE_H
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

//-------------------------------------------------------------------------------
class CConfigurationCreate : public CTask
{
public:
    enum
    {
        CONFIGURATION_CREATE_START = NEXT_STEP,
        CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_START,
        CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_WAITING,
        CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_START,
        CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_WAITING,

        CONFIGURATION_REQUEST_START,
//        CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_START,
//        CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_WAITING,
//        CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_START,
//        CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING,
        CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING,
    };

#pragma pack(push)
#pragma pack(1)
    struct TModuleContext
    {
        uint8_t uiType;
        uint8_t uiAddress;
    };
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
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
    };
#pragma pack(pop)

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

    CConfigurationCreate();
    virtual ~CConfigurationCreate();

    void SetInternalModuleName(std::string sName);
    void ConfigurationToProgrammerFormat(TConfigDataProgrammerPackOne *pxDestination,
                                         TConfigDataPackOne *pxSourse);
    uint8_t Init(void);
    uint8_t Fsm(void);

private:
    std::string m_sInternalModuleName;
    uint8_t m_uiInternalModuleId;

    uint8_t* m_puiIntermediateBuff;
    // объект с конфигурацией во внутреннем формате.
    TConfigDataPackOne m_xDeviceConfigSearch;

};

#endif // CCONFIGURATIONCREATE_H
//-------------------------------------------------------------------------------
