#ifndef CGPIO_H
#define CGPIO_H
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
#include <memory>


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

//-------------------------------------------------------------------------------
class CGpio
{
public:
    CGpio();
    virtual ~CGpio();

    static std::unique_ptr<CGpio> Create(int chipNumber,
                                         int lineOffset,
                                         const std::string& consumerLabel);
    static void ErrorMessage(const std::string& message, int errorCode);
    static int GetLineHandler(int chipNumber,
                              int lineOffset,
                              std::string consumerLabel);
//    int8_t Open(void);
//    int8_t Close(void);
//    void SetPinOutput(uint8_t ucPortN, uint8_t ucPinN);
//    void SetPinInput(uint8_t ucPortN, uint8_t ucPinN);
    void SetPin(void);
    void ClearPin(void);

protected:

private:
    int m_Fd;
    CGpio* m_pxGpio;
};

//-------------------------------------------------------------------------------
#endif // CGPIO_H
