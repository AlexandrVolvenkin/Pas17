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

    void ErrorMessage(const std::string& message, int errorCode);
    bool GetLineHandler(int chipNumber,
                        int lineOffset,
                        std::string consumerLabel);
    int8_t Open(void);
    int8_t Close(void);
    void SetPinOutput(unsigned char ucPortN, unsigned char ucPinN);
    void SetPinInput(unsigned char ucPortN, unsigned char ucPinN);
    void SetPin(unsigned char ucPortN, unsigned char ucPinN);
    void ClearPin(unsigned char ucPortN, unsigned char ucPinN);

protected:

private:
};

//-------------------------------------------------------------------------------
#endif // CGPIO_H
