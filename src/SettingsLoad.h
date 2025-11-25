#ifndef SETTINGSLOAD_H
#define SETTINGSLOAD_H
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
class CSerialPortCommunicationDevice;
class CDataContainerInterface;
class CDataContainerDataBase;
class CInternalModule;
class CConfigurationCreate;
class CDeviceControl;
class CDataBaseCreate;


//-------------------------------------------------------------------------------
class CSettingsLoad : public CTask
{
public:
    enum
    {
        SETTINGS_LOAD_START = NEXT_STEP,
        SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_START,
        SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        SETTINGS_LOAD_NETWORK_ADDRESS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,
        SETTINGS_LOAD_NETWORK_ADDRESS_SET_START,

        SETTINGS_LOAD_SETTINGS_DATA_BASE_BLOCKS_READ_START,
        SETTINGS_LOAD_SETTINGS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        SETTINGS_LOAD_SETTINGS_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,
        SETTINGS_LOAD_SETTINGS_SET_START,
    };

    CSettingsLoad();
    virtual ~CSettingsLoad();

    void SetInternalModuleName(std::string sName);
    void SetDeviceControlName(std::string sName);
    // Сеттер для m_sDataStoreName
    void SetDataStoreName(const std::string& sName)
    {
        m_sDataStoreName = sName;
    };

    uint8_t Init(void);
    uint8_t Fsm(void);

private:

private:
    std::string m_sInternalModuleName;
    uint16_t m_uiInternalModuleId;

    std::string m_sDeviceControlName;
    uint16_t m_uiDeviceControlId;

    std::string m_sDataStoreName;
    uint16_t m_uiDataStoreId;

    uint8_t* m_puiIntermediateBuff;

};

#endif // SETTINGSLOAD_H
//-------------------------------------------------------------------------------
