#ifndef DATABASECREATE_H
#define DATABASECREATE_H
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
class CDataBaseCreate : public CTask
{
public:
    enum
    {
        DATA_BASE_CREATE_START = NEXT_STEP,
//        DATA_BASE_CREATE_EXECUTOR_READY_CHECK_START,
//        DATA_BASE_CREATE_EXECUTOR_READY_CHECK_WAITING,
//        DATA_BASE_CREATE_EXECUTOR_DONE_CHECK_START,
//        DATA_BASE_CREATE_EXECUTOR_DONE_CHECK_WAITING,
        DATA_BASE_CREATE_EXECUTOR_ANSWER_PROCESSING,
        DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_START,
        DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_EXECUTOR_ANSWER_PROCESSING,
        DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_START,
        DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_EXECUTOR_ANSWER_PROCESSING,

        CONFIGURATION_REQUEST_START,
//        CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_START,
//        CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_WAITING,
//        CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_START,
//        CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING,
        CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING,
    };

    CDataBaseCreate();
    virtual ~CDataBaseCreate();

    void SetInternalModuleName(std::string sName);

    uint8_t Init(void);
    uint8_t Fsm(void);

private:
    std::string m_sInternalModuleName;
    uint8_t m_uiInternalModuleId;

    uint8_t* m_puiIntermediateBuff;

};

#endif // DATABASECREATE_H
//-------------------------------------------------------------------------------
