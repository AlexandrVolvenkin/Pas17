//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CCOMMUNICATIONDEVICE_H
#define CCOMMUNICATIONDEVICE_H

#include <stdint.h>

class CTask;
class CResources;

//-------------------------------------------------------------------------------
class CCommunicationDeviceInterface : public CTask
{
public:
    virtual void Init(void) {};

    virtual void SetSocketLowAddress(struct sockaddr_ll* pxSocketLowAddress) {};
    virtual struct sockaddr_ll* GetSocketLowAddress(void) {};

    virtual void SetDeviceName(const char* pccDeviceName) {};
    virtual const char* GetDeviceName(void) {};
    virtual void SetBaudRate(uint32_t uiBaudRate) {};
    virtual void SetDataBits(uint8_t uiDataBits) {};
    virtual void SetParity(char cParity) {};
    virtual void SetStopBit(uint8_t uiStopBit) {};

    virtual void SetIpAddress(const char* pccIpAddress) {};
    virtual const char* GetIpAddress(void) {};
    virtual void SetPort(uint16_t uiPort) {};
    virtual void SetDestinationMacAddress(uint8_t* puiMacAddress) {};
    virtual uint8_t* GetDestnationMacAddress(void) {};
    virtual void SetSourseMacAddress(uint8_t* puiMacAddress) {};
    virtual uint8_t* GetSourseMacAddress(void) {};

    virtual int8_t Open(void) {};
    virtual int8_t Close(void) {};
    virtual void CloseClient(void) {};
    virtual int16_t Write(uint8_t* puiDestination, uint16_t uiLength) {};
    virtual int16_t Read(uint8_t* puiSource, uint16_t uiLength) {};
    virtual int16_t ReceiveStart(uint8_t *puiDestination,
                                 uint16_t uiLength,
                                 uint32_t uiReceiveTimeout) {};
    virtual int16_t ReceiveContinue(uint8_t *puiDestination,
                                    uint16_t uiLength,
                                    uint32_t uiReceiveTimeout) {};
    virtual int Exchange(uint8_t uiAddress,
                         unsigned char* pucTxBuff,
                         unsigned char* pucRxBuff,
                         int iLength,
                         int iSpeed) {};
};

//-------------------------------------------------------------------------------







//-------------------------------------------------------------------------------
class CCommunicationDevice : public CCommunicationDeviceInterface
{
public:
    CCommunicationDevice();
    virtual ~CCommunicationDevice();

protected:

private:
};

//-------------------------------------------------------------------------------
#endif // CCOMMUNICATIONDEVICE_H
