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

#include "Task.h"
#include "Resources.h"
#include "Platform.h"
#include "DataStore.h"
#include "DataContainer.h"
#include "InternalModule.h"
#include "Gpio.h"

using namespace std;

//-------------------------------------------------------------------------------
CGpio::CGpio()
{

}

//-------------------------------------------------------------------------------
CGpio::~CGpio()
{

}

//-------------------------------------------------------------------------------
void CGpio::ErrorMessage(const std::string& message, int errorCode)
{
    cerr << "CGpio: " << message << " Error Code: " << errorCode << endl;
}

//-------------------------------------------------------------------------------
bool CGpio::GetLineHandler(int chipNumber,
                           int lineOffset,
                           std::string consumerLabel)
{
    int GPIO_Handler;

    GPIO_Handler = open((const char*)("/dev/gpiochip" + std::to_string(chipNumber)).c_str(), O_RDONLY);
    if (GPIO_Handler < 0)
    {
        ErrorMessage("GPIO not open!!!", GPIO_Handler);
        return false;
    }


    close(GPIO_Handler);

    return true;
}

//-------------------------------------------------------------------------------
int8_t CGpio::Open(void)
{
    std::cout << "CGpio::Open 1"  << std::endl;

}

//-------------------------------------------------------------------------------
int8_t CGpio::Close(void)
{

}

//-------------------------------------------------------------------------------
void CGpio::SetPinOutput(unsigned char ucPortN, unsigned char ucPinN)
{

}

//-------------------------------------------------------------------------------
void CGpio::SetPinInput(unsigned char ucPortN, unsigned char ucPinN)
{

}

//-------------------------------------------------------------------------------
void CGpio::SetPin(unsigned char ucPortN, unsigned char ucPinN)
{

}

//-------------------------------------------------------------------------------
void CGpio::ClearPin(unsigned char ucPortN, unsigned char ucPinN)
{

}

//-------------------------------------------------------------------------------
