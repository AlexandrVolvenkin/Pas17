//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <iostream>
#include <string.h>
#include <typeinfo>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/gpio.h>

#include "Task.h"
#include "Resources.h"
#include "Platform.h"
#include "DataStore.h"
#include "DataContainer.h"
#include "InternalModule.h"
#include "Gpio.h"

class CGpio;

using namespace std;

//-------------------------------------------------------------------------------
CGpio::CGpio()
{

}

//-------------------------------------------------------------------------------
CGpio::~CGpio()
{
    close(m_Fd);
}

//-------------------------------------------------------------------------------
std::unique_ptr<CGpio> CGpio::Create(int chipNumber,
                                     int lineOffset,
                                     const std::string& consumerLabel)
{
    std::cout << "CGpio::Create 1" << std::endl;

    // Используем std::make_unique для создания инициализированного объекта
    auto pxGpio = std::make_unique<CGpio>();

    if (pxGpio != nullptr)
    {
        int fd = GetLineHandler(chipNumber, lineOffset, consumerLabel);
        if (fd != -1)
        {
            pxGpio->m_Fd = fd;
            return pxGpio;
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
}

//-------------------------------------------------------------------------------
void CGpio::ErrorMessage(const std::string& message, int errorCode)
{
    cerr << "CGpio: " << message << " Error Code: " << errorCode << endl;
}

//-------------------------------------------------------------------------------
int CGpio::GetLineHandler(int chipNumber,
                          int lineOffset,
                          std::string consumerLabel)
{
    std::cout << "CGpio::GetLineHandler 1"  << std::endl;
    int GPIO_Handler;

    GPIO_Handler = open((const char*)("/dev/gpiochip" + std::to_string(chipNumber)).c_str(), O_RDONLY);
    if (GPIO_Handler < 0)
    {
        ErrorMessage("GPIO not open!!!", GPIO_Handler);
        return -1;
    }

    cout << "GPIO port open, GPIO_Handler = " << GPIO_Handler << endl;

    struct gpiohandle_request Request;
    memset(&Request, 0x00, sizeof(Request));

    Request.lineoffsets[0] = lineOffset;
    Request.lines = 1;
//    Request.flags = (GPIOHANDLE_REQUEST_OUTPUT | GPIOHANDLE_REQUEST_ACTIVE_LOW);
    Request.flags = (GPIOHANDLE_REQUEST_OUTPUT);
    Request.default_values[0] = 0;
    strcpy(Request.consumer_label, consumerLabel.c_str());

    int Responce = ioctl(GPIO_Handler, GPIO_GET_LINEHANDLE_IOCTL, &Request);
    if (Responce < 0)
    {
        ErrorMessage("ERROR get line handle!!!", lineOffset);
        return -1;
    }

    close(GPIO_Handler);

    return Request.fd;
}

////-------------------------------------------------------------------------------
//int8_t CGpio::Open(void)
//{
//    std::cout << "CGpio::Open 1"  << std::endl;
//
//}
//
////-------------------------------------------------------------------------------
//int8_t CGpio::Close(void)
//{
//
//}
//
////-------------------------------------------------------------------------------
//void CGpio::SetPinOutput(uint8_t ucPortN, uint8_t ucPinN)
//{
//
//}
//
////-------------------------------------------------------------------------------
//void CGpio::SetPinInput(uint8_t ucPortN, uint8_t ucPinN)
//{
//
//}

//-------------------------------------------------------------------------------
void CGpio::SetPin(void)
{
//    std::cout << "CGpio::SetPin 1"  << std::endl;
    struct gpiohandle_data GPIO;

    GPIO.values[0] = 1;

    ioctl(m_Fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &GPIO);
}

//-------------------------------------------------------------------------------
void CGpio::ClearPin(void)
{
//    std::cout << "CGpio::ClearPin 1"  << std::endl;
    struct gpiohandle_data GPIO;

    GPIO.values[0] = 0;

    ioctl(m_Fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &GPIO);
}

//-------------------------------------------------------------------------------
