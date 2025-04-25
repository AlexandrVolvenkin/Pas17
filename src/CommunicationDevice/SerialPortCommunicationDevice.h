
#ifndef CSERIALPORTCOMMUNICATIONDEVICE_H
#define CSERIALPORTCOMMUNICATIONDEVICE_H
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
class CSerialPortCommunicationDevice : public CCommunicationDevice
{
public:
    enum
    {
        UART_MAX_BUFF_LENGTH = 256,
        UART_INTERMEDIATE_BUFF_LENGTH = 16
    };

    CSerialPortCommunicationDevice();
    virtual ~CSerialPortCommunicationDevice();

//-------------------------------------------------------------------------------
    void Init(void);
    void SetDeviceName(const char* pccDeviceName);
    const char* GetDeviceName(void);
    void SetBaudRate(uint32_t uiBaudRate);
    void SetDataBits(uint8_t uiDataBits);
    void SetParity(char cParity);
    void SetStopBit(uint8_t uiStopBit);
    int8_t Open(void);
    int8_t Close(void);
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
                 unsigned char *pucTxBuff,
                 unsigned char *pucRxBuff,
                 int iLength,
                 int iSpeed);

//-------------------------------------------------------------------------------
//private:
protected:
    const char *m_pccDeviceName;
    /* Socket or file descriptor */
    int32_t m_iDeviceDescriptorServer;
    struct termios m_xTios;
    struct serial_rs485 m_xRs485Conf;
//    struct timeval *pto;
//    fd_set readfds, writefds;
//    uint32_t m_uiBaudRate;
//    /* Parity: 'N', 'O', 'E' */
//    char m_uiParity;
////    uint8_t m_uiParity;
//    uint8_t m_uiDataBits;
//    uint8_t m_uiStopBit;
//    // Флаг - данные записаны.
//    bool m_bDataIsWrited = false;
//    uint8_t* m_puiTxBuffer;
//    uint16_t m_nuiTxBuffByteCounter;
//    uint8_t* m_puiRxBuffer;
//    uint16_t m_nuiRxBuffByteCounter;
////    uint8_t m_auiIntermediateBuff[UART_INTERMEDIATE_BUFF_LENGTH];
};

//-------------------------------------------------------------------------------
#endif // CSERIALPORTCOMMUNICATIONDEVICE_H
