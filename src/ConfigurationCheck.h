#ifndef CCONFIGURATIONCHECK_H
#define CCONFIGURATIONCHECK_H
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
class CDeviceControl;
class CDataBaseCreate;


//-------------------------------------------------------------------------------
class CConfigurationCheck : public CTask
{
public:
    enum
    {
        CONFIGURATION_CHECK_START = NEXT_STEP,

        CONFIGURATION_CHECK_DIMENTIONS_PARAMETERS_CREATE_START,
        CONFIGURATION_CHECK_DIMENTIONS_PARAMETERS_CREATE_EXECUTOR_ANSWER_PROCESSING,

        CONFIGURATION_CHECK_TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START,
        CONFIGURATION_CHECK_TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING,
        CONFIGURATION_CHECK_TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START,
        CONFIGURATION_CHECK_TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING,
    };

    CConfigurationCheck();
    virtual ~CConfigurationCheck();

    void SetInternalModuleName(std::string sName);
    void SetDeviceControlName(std::string sName);

    uint8_t Init(void);
    uint8_t Fsm(void);

private:

private:
    std::string m_sInternalModuleName;
    uint8_t m_uiInternalModuleId;

    std::string m_sDeviceControlName;
    uint8_t m_uiDeviceControlId;

    uint8_t* m_puiIntermediateBuff;

};

#endif // CCONFIGURATIONCHECK_H
//-------------------------------------------------------------------------------
