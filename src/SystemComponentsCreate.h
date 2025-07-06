#ifndef CSYSTEMCOMPONENTSCREATE_H
#define CSYSTEMCOMPONENTSCREATE_H
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
class CDiscreteSignals;

//-------------------------------------------------------------------------------
class CSystemComponentsCreate : public CTask
{
public:
    enum
    {
        SYSTEM_COMPONENTS_CREATE_INTERNAL_MODULES_HANDLERS_CREATE_START = NEXT_STEP,
        SYSTEM_COMPONENTS_CREATE_INTERNAL_MODULES_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        SYSTEM_COMPONENTS_CREATE_INTERNAL_MODULES_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        SYSTEM_COMPONENTS_CREATE_DISCRETE_SIGNALS_ALARM_HANDLERS_CREATE_START,
        SYSTEM_COMPONENTS_CREATE_DISCRETE_SIGNALS_ALARM_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        SYSTEM_COMPONENTS_CREATE_DISCRETE_SIGNALS_ALARM_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,
    };

    CSystemComponentsCreate();
    virtual ~CSystemComponentsCreate();

    void SetInternalModuleName(std::string sName);

    uint8_t Init(void);
    uint8_t Fsm(void);

private:
    std::string m_sInternalModuleName;
    uint8_t m_uiInternalModuleId;

    uint8_t* m_puiIntermediateBuff;
};

//-------------------------------------------------------------------------------
#endif // CSYSTEMCOMPONENTSCREATE_H
