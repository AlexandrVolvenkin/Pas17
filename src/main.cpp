//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------




#include <iostream>

#include "Task.h"
//#include "Dfa.h"
#include "MainProductionCycle.h"

//-------------------------------------------------------------------------------
using namespace std;

int main()
{
    cout << "Hello world!" << endl;

    CMainProductionCycle xMainProductionCycle;

    while (1)
    {
        xMainProductionCycle.Fsm();
    }

    return 0;
}

//-------------------------------------------------------------------------------






////--------------------------------------------------------------------------
//// Name        : N2024.cpp
//// Author      : ADSF
//// Version     :
//// Copyright   : Your copyright notice
//// Description : Hello World in C++, Ansi-style
////--------------------------------------------------------------------------
//// Linux core version 5.10.30
////--------------------------------------------------------------------------
//// Project / Properties / C/C++ Build / Settings / Tool Settings /
//// Gross GCC++ Compiler / Dialect / Lang. standart:  ISO C++14
////--------------------------------------------------------------------------
//// Запуск из терминала + повышение привилегий:  sudo /home/debian/N2024
//// иначе ошибка Errno = 13 (Permission denied)
////--------------------------------------------------------------------------
//#include <linux/spi/spidev.h>
////#include <linux/ioctl.h>
//#include <sys/ioctl.h>
//#include <fcntl.h>
//
//#include <linux/gpio.h>
////#include <gpiod.h>
////#include <mraa.h>
//
//#include <iostream>
//#include <stdio.h>										//// debug
//#include <string.h>
//#include <errno.h>
//
//#include <unistd.h>
//
//using namespace std;
////--------------------------------------------------------------------------
////--------------------------------------------------------------------------
//// Вывод диагностики по ошибке
//void ErrorMessage(const char *String, int Handler = 0xFFFF)
//{
//    cout << String;
//
//    if(Handler != 0xFFFF)
//    {
//        cout << ", Handler = " << Handler;
//    };
//
//    cout << ", Errno = "  << errno;
//    cout << ", " << strerror(errno) << endl;
//};
////--------------------------------------------------------------------------
////--------------------------------------------------------------------------
//int main()
//{
//    cout << "\n\n\n*** Start Programm ***" << endl;
//
////goto GPIO_WORK;
////*******************************************
//// PasNew.cpp : 218:	CSpi xSpiCommunicationDevice("/dev/spidev0.0");
//
//    int Handler = open("/dev/spidev0.0", O_RDWR);		//	"/dev/spidev1.0"
//
//    if(Handler < 0)
//    {
//        ErrorMessage("Port SPI not open!!!", Handler);
//        return 0;
//    }
//    else
//    {
//        cout << "Port SPI opened" << endl;
//    };
//
////*******************************************
//    /* 		Platform.h : 458
//    #define SPI_PATH "/dev/spidev0.0"
//    #define TX_RX_BUFF_SIZE 256
//    #define SPI_MODE SPI_MODE_0
//    #define SPI_MODE32 SPI_3WIRE
//    #define BITS_PER_WORD 8
//    #define SPEED_IN_HZ 150000UL
//    #define LOW_SPEED_IN_HZ SPEED_IN_HZ	     */
//
////********************************************
//    int Mode = SPI_MODE_0;				// spidev.h
//
//    int Responce = ioctl(Handler, SPI_IOC_WR_MODE, &Mode);
//
//    if(Responce < 0)
//    {
//        ErrorMessage("Mode port SPI not set!!!");
//        return 0;
//    }
//    else
//    {
//        cout << "Mode port SPI set" << endl;
//    };
//
////********************************************
//    int DataSize = 8;
//
//    Responce = ioctl(Handler, SPI_IOC_WR_BITS_PER_WORD, &DataSize);
//
//    cout << "Data Size = " << DataSize << endl;
//
//    if(Responce < 0)
//    {
//        ErrorMessage("Error port SPI set data size");
//        return 0;
//    }
//    else
//    {
//        cout << "Set port SPI data size" << endl;
//    };
//
////*******************************************
//// github.com/embeddedcraft/code/blob/master/ecspi.c
//
//    int Frequncy = 2000000L / 2;  // 3686400
//
//    Responce = ioctl(Handler, SPI_IOC_WR_MAX_SPEED_HZ, &Frequncy);
//
//    if(Responce < 0)
//    {
//        ErrorMessage("Speed port SPI not set!!!");
//        return 0;
//    }
//    else
//    {
//        cout << "Speed port SPI set" << endl;
//    };
//
////*******************************************
////*******************************************
//    uint8_t TXBuffer[64];
//    uint8_t RXBuffer[64];
//
//    memset(TXBuffer, 0x00, sizeof (TXBuffer));
//    memset(RXBuffer, 0x00, sizeof (RXBuffer));
//
//    TXBuffer[0] = 0x30;		// Чтение EVE_REG_ID (0x302000) = 0x7C
//    TXBuffer[1] = 0x20;
//    TXBuffer[2] = 0x00;
//    TXBuffer[3] = 0x00;		// Пустой cинхро-байт
//    TXBuffer[4] = 0x00;
//
//    struct spi_ioc_transfer Transfer =
//    {
//        .tx_buf = (unsigned long) TXBuffer,
//        .rx_buf = (unsigned long) RXBuffer,
//        .len    = 5,
//        //.speed_hz = 2000000L / 2,
//        //.delay_usecs = 15
//    };
//
////*******************************************
//    /*
//    const int BuffSize = 64;
//
//    uint8_t *TXPtr = new uint8_t[BuffSize];
//    uint8_t *RXPtr = new uint8_t[BuffSize];
//
//    for(int i = 0; i < BuffSize; i++)
//    {
//     TXPtr[i] = i & 0xFF;
//     RXPtr[i] = 0x00;
//    };
//
//    struct spi_ioc_transfer Transfer;
//
//    memset(&Transfer, 0x00, sizeof(Transfer));
//
//    Transfer.tx_buf = (unsigned long) TXPtr;
//    Transfer.rx_buf = (unsigned long) RXPtr;
//    Transfer.len    = BuffSize;							*/
//
//    int TransferNum = 1;
//
//    while(0)
//    {
//        Responce = ioctl(Handler, SPI_IOC_MESSAGE(TransferNum), &Transfer);
//
//        if(Responce < 0)
//        {
//            ErrorMessage("Transfer port SPI failed!!!", Responce);
//            return 0;
//        }
//        else
//        {
//            //cout << "Transfer port SPI complete" << endl;
//        };
//
//        //usleep(10000);							// Задержка 10 мс (Obsolete)
//        usleep(25000);							// Задержка 25 мс (Obsolete)
//    };
//
//
////*****************************************
////*****************************************
//// www.kernel.org/doc/html/next/userspace-api/gpio/chardev_v1.html
//// developerhelp.microchip.com/xwiki/bin/view/software-tools/linux/apps-gpio/#HGPIOSupport
//// blog.lxsang.me/post/id/33
//
//////GPIO_WORK:
//
//    int GHandler = open("/dev/gpiochip2", O_RDONLY);
//
//    if(GHandler < 0)
//    {
//        ErrorMessage("GPIO not open!!!", GHandler);
//        return 0;
//    }
//    else
//    {
//        cout << "GPIO opened" << endl;
//    };
//
//    struct gpiochip_info cinfo;
//    int ret = ioctl(GHandler, GPIO_GET_CHIPINFO_IOCTL, &cinfo);
//    fprintf(stdout, "GPIO chip: %s, \"%s\", %u GPIO lines\n",
//            cinfo.name, cinfo.label, cinfo.lines);
//
//    struct gpioline_info linfo;
//    ret = ioctl(GHandler, GPIO_GET_LINEINFO_IOCTL, &linfo);
//    fprintf(stdout, "line %2d: %s\n", linfo.line_offset,  linfo.name);
//
////*****************************************
//    struct gpiohandle_request Request;
//
//    Request.lineoffsets[0] = 7;																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																											//// 16;
//    Request.lines = 1;
//    Request.flags = GPIOHANDLE_REQUEST_OUTPUT;  	////GPIOHANDLE_REQUEST_ACTIVE_LOW
//
//    strcpy(Request.consumer_label, "SS0");
//
//
//    Responce = ioctl(GHandler, GPIO_GET_LINEHANDLE_IOCTL, &Request);
//
//    if(Responce < 0)
//    {
//        ErrorMessage("ERROR get line handle");
//        return 0;
//    };
//
//
//    struct gpiohandle_data Data;
//
//    Data.values[0] = 1;
//
//    Responce = ioctl(Request.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &Data);
//
//    if(Responce < 0)
//    {
//        ErrorMessage("ERROR set line value");
//        return 0;
//    };
//
//
////*****************************************
//    while(1)
//    {
//        Data.values[0] = 0;
//        ioctl(Request.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &Data);
//
//
//        if(1)
//        {
//            Responce = ioctl(Handler, SPI_IOC_MESSAGE(1), &Transfer);
//
//            if(Responce < 0)
//            {
//                ErrorMessage("Transfer port SPI failed!!!", Responce);
//                return 0;
//            }
//            else
//            {
//                cout << "Readed: " << std::hex << (int)RXBuffer[4] << endl;
//
//            };
//        }
//        else
//        {
//            usleep(10);
//        };
//
//
//        Data.values[0] = 1;
//        ioctl(Request.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &Data);
//
//        usleep(10000);
//    };
//
////*****************************************
////*****************************************
//    close( Handler);
//    close(GHandler);
//
////delete TXPtr;
////delete RXPtr;
//
//    return 0;
//};
////--------------------------------------------------------------------------
////--------------------------------------------------------------------------
////--------------------------------------------------------------------------
////--------------------------------------------------------------------------

















////--------------------------------------------------------------------------
//// Name        : N2024.cpp
//// Author      : ADSF
//// Version     :
//// Copyright   : Your copyright notice
//// Description : Hello World in C++, Ansi-style
////--------------------------------------------------------------------------
//// Linux core version 5.10.30
////--------------------------------------------------------------------------
//// Project / Properties / C/C++ Build / Settings / Tool Settings /
//// Gross GCC++ Compiler / Dialect / Lang. standart:  ISO C++14
////--------------------------------------------------------------------------
//// Запуск из терминала + повышение привилегий:  sudo /home/debian/N2024
//// иначе ошибка Errno = 13 (Permission denied)
////--------------------------------------------------------------------------
//#include <linux/spi/spidev.h>
////#include <linux/ioctl.h>
//#include <sys/ioctl.h>
//#include <fcntl.h>
//
//#include <linux/gpio.h>
////#include <gpiod.h>
////#include <mraa.h>
//
//#include <iostream>
//#include <stdio.h>										//// debug
//#include <string.h>
//#include <errno.h>
//
//#include <unistd.h>
//
//using namespace std;
////--------------------------------------------------------------------------
////--------------------------------------------------------------------------
//// Вывод диагностики по ошибке
//void ErrorMessage(const char *String, int Handler = 0xFFFF)
//{
//    cout << String;
//
//    if(Handler != 0xFFFF)
//    {
//        cout << ", Handler = " << Handler;
//    };
//
//    cout << ", Errno = "  << errno;
//    cout << ", " << strerror(errno) << endl;
//};
////--------------------------------------------------------------------------
////--------------------------------------------------------------------------
//int main()
//{
//    cout << "\n\n\n*** Start Programm ***" << endl;
//
////goto GPIO_WORK;
////*******************************************
//// PasNew.cpp : 218:	CSpi xSpiCommunicationDevice("/dev/spidev0.0");
//
//    int Handler = open("/dev/spidev0.0", O_RDWR);		//	"/dev/spidev1.0"
//
//    if(Handler < 0)
//    {
//        ErrorMessage("Port SPI not open!!!", Handler);
//        return 0;
//    }
//    else
//    {
//        cout << "Port SPI opened" << endl;
//    };
//
////*******************************************
//    /* 		Platform.h : 458
//    #define SPI_PATH "/dev/spidev0.0"
//    #define TX_RX_BUFF_SIZE 256
//    #define SPI_MODE SPI_MODE_0
//    #define SPI_MODE32 SPI_3WIRE
//    #define BITS_PER_WORD 8
//    #define SPEED_IN_HZ 150000UL
//    #define LOW_SPEED_IN_HZ SPEED_IN_HZ	     */
//
////********************************************
//    int Mode = SPI_MODE_0;				// spidev.h
//
//    int Responce = ioctl(Handler, SPI_IOC_WR_MODE, &Mode);
//
//    if(Responce < 0)
//    {
//        ErrorMessage("Mode port SPI not set!!!");
//        return 0;
//    }
//    else
//    {
//        cout << "Mode port SPI set" << endl;
//    };
//
////********************************************
//    int DataSize = 8;
//
//    Responce = ioctl(Handler, SPI_IOC_WR_BITS_PER_WORD, &DataSize);
//
//    cout << "Data Size = " << DataSize << endl;
//
//    if(Responce < 0)
//    {
//        ErrorMessage("Error port SPI set data size");
//        return 0;
//    }
//    else
//    {
//        cout << "Set port SPI data size" << endl;
//    };
//
////*******************************************
//// github.com/embeddedcraft/code/blob/master/ecspi.c
//
//    int Frequncy = 2000000L / 2;  // 3686400
//
//    Responce = ioctl(Handler, SPI_IOC_WR_MAX_SPEED_HZ, &Frequncy);
//
//    if(Responce < 0)
//    {
//        ErrorMessage("Speed port SPI not set!!!");
//        return 0;
//    }
//    else
//    {
//        cout << "Speed port SPI set" << endl;
//    };
//
////*******************************************
////*******************************************
//    uint8_t TXBuffer[64];
//    uint8_t RXBuffer[64];
//
//    memset(TXBuffer, 0x00, sizeof (TXBuffer));
//    memset(RXBuffer, 0x00, sizeof (RXBuffer));
//
//    TXBuffer[0] = 0x30;		// Чтение EVE_REG_ID (0x302000) = 0x7C
//    TXBuffer[1] = 0x20;
//    TXBuffer[2] = 0x00;
//    TXBuffer[3] = 0x00;		// Пустой cинхро-байт
//    TXBuffer[4] = 0x00;
//
//    TXBuffer[5] = 0x30;		// Чтение EVE_REG_ID (0x302000) = 0x7C
//    TXBuffer[6] = 0x20;
//    TXBuffer[7] = 0x00;
//    TXBuffer[8] = 0x00;		// Пустой cинхро-байт
//    TXBuffer[4] = 0x00;
//
//    TXBuffer[9] = 0x30;		// Чтение EVE_REG_ID (0x302000) = 0x7C
//    TXBuffer[10] = 0x20;
//    TXBuffer[11] = 0x00;
//    TXBuffer[12] = 0x00;		// Пустой cинхро-байт
//    TXBuffer[13] = 0x00;
//
//    TXBuffer[14] = 0x30;		// Чтение EVE_REG_ID (0x302000) = 0x7C
//    TXBuffer[15] = 0x20;
//    TXBuffer[16] = 0x00;
//    TXBuffer[17] = 0x00;		// Пустой cинхро-байт
//    TXBuffer[18] = 0x00;
//
//    struct spi_ioc_transfer Transfer =
//    {
//        .tx_buf = (unsigned long) TXBuffer,
//        .rx_buf = (unsigned long) RXBuffer,
//        .len    = 15,
//        //.speed_hz = 2000000L / 2,
//        //.delay_usecs = 15
//    };
//
////*******************************************
//    /*
//    const int BuffSize = 64;
//
//    uint8_t *TXPtr = new uint8_t[BuffSize];
//    uint8_t *RXPtr = new uint8_t[BuffSize];
//
//    for(int i = 0; i < BuffSize; i++)
//    {
//     TXPtr[i] = i & 0xFF;
//     RXPtr[i] = 0x00;
//    };
//
//    struct spi_ioc_transfer Transfer;
//
//    memset(&Transfer, 0x00, sizeof(Transfer));
//
//    Transfer.tx_buf = (unsigned long) TXPtr;
//    Transfer.rx_buf = (unsigned long) RXPtr;
//    Transfer.len    = BuffSize;							*/
//
//    int TransferNum = 1;
//
//    while(0)
//    {
//        Responce = ioctl(Handler, SPI_IOC_MESSAGE(TransferNum), &Transfer);
//
//        if(Responce < 0)
//        {
//            ErrorMessage("Transfer port SPI failed!!!", Responce);
//            return 0;
//        }
//        else
//        {
//            //cout << "Transfer port SPI complete" << endl;
//        };
//
//        //usleep(10000);							// Задержка 10 мс (Obsolete)
//        usleep(25000);							// Задержка 25 мс (Obsolete)
//    };
//
//
////*****************************************
////*****************************************
//// www.kernel.org/doc/html/next/userspace-api/gpio/chardev_v1.html
//// developerhelp.microchip.com/xwiki/bin/view/software-tools/linux/apps-gpio/#HGPIOSupport
//// blog.lxsang.me/post/id/33
//
//////GPIO_WORK:
//
//    int GHandler = open("/dev/gpiochip2", O_RDONLY);
//
//    if(GHandler < 0)
//    {
//        ErrorMessage("GPIO not open!!!", GHandler);
//        return 0;
//    }
//    else
//    {
//        cout << "GPIO opened" << endl;
//    };
//
//    struct gpiochip_info cinfo;
//    int ret = ioctl(GHandler, GPIO_GET_CHIPINFO_IOCTL, &cinfo);
//    fprintf(stdout, "GPIO chip: %s, \"%s\", %u GPIO lines\n",
//            cinfo.name, cinfo.label, cinfo.lines);
//
//    struct gpioline_info linfo;
//    ret = ioctl(GHandler, GPIO_GET_LINEINFO_IOCTL, &linfo);
//    fprintf(stdout, "line %2d: %s\n", linfo.line_offset,  linfo.name);
//
////*****************************************
//    struct gpiohandle_request Request;
//
//    Request.lineoffsets[0] = 7;																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																											//// 16;
//    Request.lines = 1;
//    Request.flags = GPIOHANDLE_REQUEST_OUTPUT;  	////GPIOHANDLE_REQUEST_ACTIVE_LOW
//
//    strcpy(Request.consumer_label, "SS0");
//
//
//    Responce = ioctl(GHandler, GPIO_GET_LINEHANDLE_IOCTL, &Request);
//
//    if(Responce < 0)
//    {
//        ErrorMessage("ERROR get line handle");
//        return 0;
//    };
//
//
//    struct gpiohandle_data Data;
//
//    Data.values[0] = 1;
//
//    Responce = ioctl(Request.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &Data);
//
//    if(Responce < 0)
//    {
//        ErrorMessage("ERROR set line value");
//        return 0;
//    };
//
//
////*****************************************
//    while(1)
//    {
//        Data.values[0] = 0;
//        ioctl(Request.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &Data);
//
//
//        if(1)
//        {
//            Responce = ioctl(Handler, SPI_IOC_MESSAGE(1), &Transfer);
//
//            if(Responce < 0)
//            {
//                ErrorMessage("Transfer port SPI failed!!!", Responce);
//                return 0;
//            }
//            else
//            {
////                cout << "Readed: " << std::hex << (int)RXBuffer[4] << endl;
//
//            };
//        }
//        else
//        {
//            usleep(10);
//        };
//
//
//        Data.values[0] = 1;
//        ioctl(Request.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &Data);
//
////        cout << "Request.fd" << endl;
//        usleep(10000);
//    };
//
////*****************************************
////*****************************************
//    close( Handler);
//    close(GHandler);
//
////delete TXPtr;
////delete RXPtr;
//
//    return 0;
//};
////--------------------------------------------------------------------------
////--------------------------------------------------------------------------
////--------------------------------------------------------------------------
////--------------------------------------------------------------------------
