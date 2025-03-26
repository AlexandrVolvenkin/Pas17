#ifndef CSHAREDMEMORYCOMMUNICATIONDEVICE_H
#define CSHAREDMEMORYCOMMUNICATIONDEVICE_H
//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <iostream>
#include <random>
#include <iomanip>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <termios.h>
#include <linux/serial.h>
#include <sys/types.h>
#include <sys/select.h>

class CTask;
class CResources;
class CCommunicationDevice;
class CCommunicationDeviceInterface;

//-------------------------------------------------------------------------------
class CSharedMemoryCommunicationDevice : public CCommunicationDevice
{
public:
    enum
    {
        MODBUS_SM_MAX_ADU_LENGTH = 256,
    };

    CSharedMemoryCommunicationDevice();
    virtual ~CSharedMemoryCommunicationDevice();

//-------------------------------------------------------------------------------
    void Init(void);
    void SetDeviceName(const char* pccDeviceName);
    const char* GetDeviceName(void);
    int8_t Open(void);
    int8_t Close(void);
    int16_t Write(uint8_t* puiDestination, uint16_t uiLength);
    int16_t Read(uint8_t* puiSource, uint16_t uiLength);
    int16_t ReceiveStart(uint8_t *puiDestination,
                         uint16_t uiLength,
                         uint32_t uiReceiveTimeout);
    int16_t ReceiveContinue(uint8_t *puiDestination,
                            uint16_t uiLength,
                            uint32_t uiReceiveTimeout);

//-------------------------------------------------------------------------------
//private:
protected:
    const char *m_pccDeviceName;
    /* Socket or file descriptor */
    int32_t m_iDeviceDescriptorServer;

    uint8_t *m_puiClientToServerBuffer;
    uint8_t *m_puiServerToClientBuffer;
};

//-------------------------------------------------------------------------------

#endif // CSHAREDMEMORYCOMMUNICATIONDEVICE_H
