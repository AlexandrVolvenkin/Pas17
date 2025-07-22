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

#include "DataTypes.h"

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
class CDiscreteSignals;

//-------------------------------------------------------------------------------
class CConfigurationCreate : public CTask
{
public:
    enum
    {
        CONFIGURATION_CREATE_START = NEXT_STEP,
//        CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_START,
//        CONFIGURATION_CREATE_EXECUTOR_READY_CHECK_WAITING,
//        CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_START,
//        CONFIGURATION_CREATE_EXECUTOR_DONE_CHECK_WAITING,
        CONFIGURATION_CREATE_EXECUTOR_ANSWER_PROCESSING,
        CONFIGURATION_CREATE_INTERNAL_MODULES_SEARCH_MODULES_START,
        CONFIGURATION_CREATE_INTERNAL_MODULES_SEARCH_MODULES_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        CONFIGURATION_CREATE_INTERNAL_MODULES_SEARCH_MODULES_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        CONFIGURATION_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_START,
        CONFIGURATION_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_EXECUTOR_ANSWER_PROCESSING,

		CONFIGURATION_CREATE_INTERNAL_MODULES_HANDLERS_CREATE_START,
        CONFIGURATION_CREATE_INTERNAL_MODULES_HANDLERS_EXECUTOR_CREATE_DONE_OK_ANSWER_PROCESSING,
        CONFIGURATION_CREATE_INTERNAL_MODULES_HANDLERS_EXECUTOR_CREATE_DONE_ERROR_ANSWER_PROCESSING,

        CONFIGURATION_CREATE_DISCRETE_SIGNALS_SERVICE_DATA_CREATE_START,
        CONFIGURATION_CREATE_DISCRETE_SIGNALS_SERVICE_DATA_CREATE_EXECUTOR_ANSWER_PROCESSING,

        CONFIGURATION_REQUEST_START,
//        CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_START,
//        CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_WAITING,
//        CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_START,
//        CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING,
        CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING,
    };

    CConfigurationCreate();
    virtual ~CConfigurationCreate();

    void SetInternalModuleName(std::string sName);

    static void ConfigurationToProgrammerFormat(TConfigDataProgrammerPackOne* pxDestination,
            TConfigDataPackOne* pxSourse);

    uint8_t Init(void);
    uint8_t Fsm(void);

private:
    std::string m_sInternalModuleName;
    uint8_t m_uiInternalModuleId;

    uint8_t* m_puiIntermediateBuff;
//    // объект с конфигурацией во внутреннем формате.
//    TConfigDataPackOne m_xDeviceConfigSearch;

};

#endif // CCONFIGURATIONCREATE_H
//-------------------------------------------------------------------------------
