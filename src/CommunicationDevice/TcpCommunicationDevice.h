#ifndef CTCPCOMMUNICATIONDEVICE_H
#define CTCPCOMMUNICATIONDEVICE_H
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
class CTcpCommunicationDevice : public CCommunicationDevice
{
public:
    enum
    {
        UART_MAX_BUFF_LENGTH = 256,
        UART_INTERMEDIATE_BUFF_LENGTH = 16
    };

    CTcpCommunicationDevice();
    virtual ~CTcpCommunicationDevice();

//-------------------------------------------------------------------------------
    void Init(void);
    void SetIpAddress(const char* pccIpAddress);
    const char* GetIpAddress(void);
    void SetPort(uint16_t uiPort);
    int8_t Listen(void);
    int8_t Accept(void);
//    int8_t Accept(uint32_t uiBlockingTime);
    int8_t Connect(void);
    int8_t Open(void);
    int8_t Close(void);
    void CloseClient(void);
//    void Reset(void);
//    bool IsDataAvailable(void);
    int16_t Write(uint8_t* puiDestination, uint16_t uiLength);
    int16_t Read(uint8_t* puiSource, uint16_t uiLength);
    int16_t ReceiveStart(uint8_t *puiDestination,
                         uint16_t uiLength,
                         uint32_t uiReceiveTimeout);
    int16_t ReceiveContinue(uint8_t *puiDestination,
                            uint16_t uiLength,
                            uint32_t uiReceiveTimeout);
    int Exchange(uint8_t uiAddress,
                 uint8_t *pucTxBuff,
                 uint8_t *pucRxBuff,
                 int iLength,
                 int iSpeed);

//-------------------------------------------------------------------------------
//private:
//protected:
    const char* m_pccDeviceName;
    const char* m_pccIpAddress;
    uint32_t m_uiIpAddress;
    uint16_t m_uiPort;
    /* Socket or file descriptor */
    int32_t m_iDeviceDescriptorServer;
    int32_t m_iDeviceDescriptorClient;
    struct sockaddr_in m_Address;
};

//-------------------------------------------------------------------------------
#endif // CTCPCOMMUNICATIONDEVICE_H
