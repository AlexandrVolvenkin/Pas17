//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <iostream>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

//#include "Timer.h"
#include "Platform.h"

using namespace std;

////-------------------------------------------------------------------------------
int32_t CGpio::m_iDeviceDescriptorServer;
TGpioControl CGpio::m_xGpioControl;

////-------------------------------------------------------------------------------
//CGpio::CGpio()
//{
//
//}
//
////-------------------------------------------------------------------------------
//CGpio::~CGpio()
//{
//
//}

//-------------------------------------------------------------------------------
void CGpio::Init(void)
{
    Open();
};

//-------------------------------------------------------------------------------
int8_t CGpio::Open(void)
{
    std::cout << "CGpio::Open 1"  << std::endl;
    m_iDeviceDescriptorServer = open("/dev/mem", O_RDWR | O_SYNC);
    if(m_iDeviceDescriptorServer < 0)
    {
        std::cout << "CGpio::Open 2"  << std::endl;
        printf("%s\n", strerror(errno));
        close(m_iDeviceDescriptorServer);
        return -1;
    }

    m_xGpioControl.aulGpioMmapAddresses[GPIO_PORT_0] = GPIO0_ADDR;
    m_xGpioControl.aulGpioMmapAddresses[GPIO_PORT_1] = GPIO1_ADDR;
    m_xGpioControl.aulGpioMmapAddresses[GPIO_PORT_2] = GPIO2_ADDR;
    m_xGpioControl.aulGpioMmapAddresses[GPIO_PORT_3] = GPIO3_ADDR;

    int i;
    for(i = 0; i < GPIO_PORT_QUANTITY; i++)
    {
        m_xGpioControl.apulGpioMmapPointers[i] =  (ulong*) mmap(NULL,
                0x1000,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                m_iDeviceDescriptorServer,
                m_xGpioControl.aulGpioMmapAddresses[i]);
    }

// GPIO clock configuration
    m_xGpioControl.pulCmPerMap =  (ulong*) mmap(NULL,
                                  0x400,
                                  PROT_READ | PROT_WRITE,
                                  MAP_SHARED,
                                  m_iDeviceDescriptorServer,
                                  CM_PER);

//    *(m_xGpioControl.pulCmPerMap + (CM_PER_L4LS_CLKSTCTRL_OFFSET / 4)) |= (1 << CLKACTIVITY_GPIO_1_GDBCLK) |
//            (1 << CLKACTIVITY_GPIO_2_GDBCLK) |
//            (1 << CLKACTIVITY_GPIO_3_GDBCLK);
//
    *(m_xGpioControl.pulCmPerMap + (CM_PER_GPIO1_CLKCTRL_OFFSET / 4)) = (1 << OPTFCLKEN_GPIO_X_GDBCLK) | (1 << MODULEMODE_BIT_1);
    *(m_xGpioControl.pulCmPerMap + (CM_PER_GPIO2_CLKCTRL_OFFSET / 4)) = (1 << OPTFCLKEN_GPIO_X_GDBCLK) | (1 << MODULEMODE_BIT_1);
    *(m_xGpioControl.pulCmPerMap + (CM_PER_GPIO3_CLKCTRL_OFFSET / 4)) = (1 << OPTFCLKEN_GPIO_X_GDBCLK) | (1 << MODULEMODE_BIT_1);

    std::cout << "CGpio::Open 3"  << std::endl;
    return 0;
};

//-------------------------------------------------------------------------------
int8_t CGpio::Close(void)
{
    close(m_iDeviceDescriptorServer);
}

//-------------------------------------------------------------------------------
void CGpio::SetPinOutput(unsigned char ucPortN, unsigned char ucPinN)
{
    *(m_xGpioControl.apulGpioMmapPointers[ucPortN] + (GPIO_OE_OFFSET / 4)) &= ~(1 << ucPinN);
}

//-------------------------------------------------------------------------------
void CGpio::SetPinInput(unsigned char ucPortN, unsigned char ucPinN)
{
    *(m_xGpioControl.apulGpioMmapPointers[ucPortN] + (GPIO_OE_OFFSET / 4)) |= (1 << ucPinN);
}

//-------------------------------------------------------------------------------
void CGpio::SetPin(unsigned char ucPortN, unsigned char ucPinN)
{
    *(m_xGpioControl.apulGpioMmapPointers[ucPortN] + (GPIO_DATAOUT_OFFSET / 4)) |= (1 << ucPinN);
}

//-------------------------------------------------------------------------------
void CGpio::ClearPin(unsigned char ucPortN, unsigned char ucPinN)
{
    *(m_xGpioControl.apulGpioMmapPointers[ucPortN] + (GPIO_DATAOUT_OFFSET / 4)) &= ~(1 << ucPinN);
}
//-------------------------------------------------------------------------------













////-------------------------------------------------------------------------------
//CPlatform::CPlatform()
//{
//
//}
//
////-------------------------------------------------------------------------------
//CPlatform::~CPlatform()
//{
//
//}
//
////-------------------------------------------------------------------------------
//void CPlatform::Init(void)
//{
//
//}
//
////-------------------------------------------------------------------------------
//uint16_t CPlatform::GetCurrentTime(void)
//{
//    struct timeval xCurrentTime;
//
//    gettimeofday( &xCurrentTime, NULL );
//
//    return static_cast<uint16_t>(((xCurrentTime.tv_sec * 1000) +
//                                  (xCurrentTime.tv_usec / 1000)));
//}

//-------------------------------------------------------------------------------
