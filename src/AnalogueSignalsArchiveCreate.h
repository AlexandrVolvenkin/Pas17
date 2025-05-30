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
        ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,
    };

    CAnalogueSignalsArchiveCreate();
    virtual ~CAnalogueSignalsArchiveCreate();

    void SetInternalModuleName(std::string sName);

    uint8_t Init(void);
    uint8_t Fsm(void);
    void Allocate(void);
    void CreateArchiveEntry(void);

private:
    std::string m_sInternalModuleName;
    uint8_t m_uiInternalModuleId;

    float* m_pfAnalogueInputsValue;
//    uint8_t* m_puiAnalogueInputsState;
//    uint8_t* m_puiAnalogueInputsOff;
//    uint8_t* m_puiAnalogueInputsBadState;
    uint16_t *m_puiHoldingRegisters;
    uint16_t *m_puiInputRegisters;

    uint8_t* m_puiIntermediateBuff;
    std::string m_sLastDailyArchveFlashFile;
    std::string m_sCurrentDailyArchveFlashFile;
    bool bIsDailyArchveFlashFileExist = false;
    uint32_t m_uiCurrentOffset = 0;
    int m_iLastSecond = 100;
    int m_iLastHour = 100;
    int m_iLastDay = 100;
};

#endif // ANALOGUESIGNALSARCHIVECREATE_H
//-------------------------------------------------------------------------------
