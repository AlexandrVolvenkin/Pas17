
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <typeinfo>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <errno.h>

#include "Task.h"
#include "Platform.h"
#include "Gpio.h"
#include "Resources.h"
#include "CommunicationDevice.h"
#include "Semaphore.h"
#include "SpiCommunicationDevice.h"

using namespace std;

//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
CSpiCommunicationDevice::CSpiCommunicationDevice()
{
    m_pxSpi0Semaphore = new CSemaphore(12345, 1);
}

//-------------------------------------------------------------------------------
CSpiCommunicationDevice::~CSpiCommunicationDevice()
{
    delete m_pxSpi0Semaphore;

//    ChipSelectPinDelete();
    Close();
}

//-------------------------------------------------------------------------------
void CSpiCommunicationDevice::Init(void)
{
    Open();
//    ChipSelectPinSet();

    pxGpioSpiChipEnablePin =
        CGpio::Create(2,
                      12,
                      "SPI_CHIP_ENABLE_PIN");
    pxGpioPrdEnablePin =
        CGpio::Create(0,
                      22,
                      "PRD_EN_PIN");

    SetFsmState(READY);
}

//-------------------------------------------------------------------------------
int8_t CSpiCommunicationDevice::Open(void)
{
    int iBits = BITS_PER_WORD;
    int  iMode = SPI_MODE;             //8-bits per word, SPI mode 3
//    ULONG  ulMode32 = SPI_MODE32;
    unsigned long ulSpeed;
    unsigned int nuiBusyTimeCounter;

    ulSpeed = SPEED_IN_HZ;               //Speed is 1 MHz

    // The following calls set up the SPI bus properties
    nuiBusyTimeCounter = 20;
    while (1)
    {
        if ((m_iDeviceDescriptorServer = open(SPI_PATH, O_RDWR | O_SYNC)) < 0)
        {
            perror("SPI: Can't open device.");
            pxGpioPrdEnablePin -> ClearPin();
            usleep(500000);
            pxGpioPrdEnablePin -> SetPin();
            usleep(500000);
            if (!nuiBusyTimeCounter--)
            {
                return -1;
            }
        }
        else
        {
            break;
        }
    }

    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_WR_MODE, &iMode) == -1)
    {
        perror("SPI: Can't set SPI mode.");
        return -1;
    }
    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_RD_MODE, &iMode) == -1)
    {
        perror("SPI: Can't get SPI mode.");
        return -1;
    }
//    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_WR_MODE32, &ulMode32) == -1)
//    {
//        perror("SPI: Can't set SPI mode32.");
//        return -1;
//    }
//    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_RD_MODE32, &ulMode32) == -1)
//    {
//        perror("SPI: Can't get SPI mode32.");
//        return -1;
//    }
    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_WR_BITS_PER_WORD, &iBits) == -1)
    {
        perror("SPI: Can't set bits per word.");
        return -1;
    }
    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_RD_BITS_PER_WORD, &iBits) == -1)
    {
        perror("SPI: Can't get bits per word.");
        return -1;
    }
    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_WR_MAX_SPEED_HZ, &ulSpeed) == -1)
    {
        perror("SPI: Can't set max speed HZ");
        return -1;
    }
    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_RD_MAX_SPEED_HZ, &ulSpeed) == -1)
    {
        perror("SPI: Can't get max speed HZ.");
        return -1;
    }

//    // Check that the properties have been set
//    printf("SPI Mode is: %d\n", iMode);
//    printf("SPI Bits is: %d\n", iBits);
//    printf("SPI Speed is: %d\n", (int)ulSpeed);






//    std::cout << "CSpiCommunicationDevice::Open 1"  << std::endl;
//    ofstream outdata;
//    // Чтобы добавить и не стереть старые данные откроем файл на чтение и запись.
//    outdata.open("Spi0Semaphore.sem", (ios::binary | ios::in | ios::out));
//    // Файл не существует?
//    if (!outdata)
//    {
//        std::cout << "CSpiCommunicationDevice::Open 2"  << std::endl;
//        cerr << "CSpiCommunicationDevice::Open Error: file could not be opened" << endl;
//        // чтобы создать файл откроем только на запись.
//        outdata.open("Spi0Semaphore.sem", (ios::binary | ios::out));
//        // Файл не создан?
//        if (!outdata)
//        {
//            std::cout << "CSpiCommunicationDevice::Open 3"  << std::endl;
//            cerr << "CSpiCommunicationDevice::Open Error: file could not be created" << endl;
//            return 0;
//        }
//    }
//
//    std::cout << "CSpiCommunicationDevice::Open 5"  << std::endl;
//    // закроем файл.
//    outdata.close();


////    key_t key = ftok("keyfile", 'A'); // Используем ключ файлового устройства
//    key_t key = 12345;
////    key_t key = ftok("Spi0Semaphore.sem", 1);
//    if (key == -1)
//    {
//        perror("ftok");
//        std::cout << "CSpiCommunicationDevice::Open 6"  << std::endl;
//    }
//    else
//    {
//        std::cout << "CSpiCommunicationDevice::Open 7"  << std::endl;
//    }
//
////    int semid;
//    // Создание семафора с заданным ключом и разрешениями
//    m_iSpi0SemaphoreId = semget(key, 1, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
//    if (m_iSpi0SemaphoreId == -1)
//    {
//        std::cout << "CSpiCommunicationDevice::Open 8"  << std::endl;
//        if (errno == EEXIST)
//            printf("Semaphore already exists.\n");
//        else
//            perror("Error creating semaphore");
//    }
//    else
//    {
//        std::cout << "CSpiCommunicationDevice::Open 9"  << std::endl;
//    }
//
//    semctl(m_iSpi0SemaphoreId, 0, SETVAL, 1); // Установка начального значения семафора в 1
//    std::cout << "Semaphore created with ID: " << m_iSpi0SemaphoreId << "\n";
//
//    // Теперь m_iSpi0SemaphoreId содержит идентификатор созданного семафора
//
//    // Пример использования семафора
//
////    // Закрываем семафор (если это необходимо)
////    semctl(m_iSpi0SemaphoreId, 0, IPC_RMID);
//    int result = semctl(m_iSpi0SemaphoreId, 0, IPC_RMID); // Удаление семафора
//    if (result == -1)
//    {
//        perror("semctl");
//    }
//    else
//    {
//        std::cout << "Semaphore destroyed with ID: " << m_iSpi0SemaphoreId << "\n";
//    }

//    m_pxSpi0Semaphore = new CSemaphore(12345, 1);
//
//
////    m_pxSpi0Semaphore -> Acquire();
////    sleep(5); // Пример работы с ресурсом
////    m_pxSpi0Semaphore -> Release();
////    CSemaphore xSpi0Semaphore(12345, 1);
////    xSpi0Semaphore.Acquire();
////    sleep(5); // Пример работы с ресурсом
////    xSpi0Semaphore.Release();

    return 0;
}

//-------------------------------------------------------------------------------
int8_t CSpiCommunicationDevice::ModeSet(void)
{
    int iBits = BITS_PER_WORD;
    int  iMode = SPI_MODE;             //8-bits per word, SPI mode 3
//    ULONG  ulMode32 = SPI_MODE32;
    unsigned long ulSpeed;
    unsigned int nuiBusyTimeCounter;

    ulSpeed = SPEED_IN_HZ;               //Speed is 1 MHz

    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_WR_MODE, &iMode) == -1)
    {
        perror("SPI: Can't set SPI mode.");
        return -1;
    }
    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_RD_MODE, &iMode) == -1)
    {
        perror("SPI: Can't get SPI mode.");
        return -1;
    }
//    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_WR_MODE32, &ulMode32) == -1)
//    {
//        perror("SPI: Can't set SPI mode32.");
//        return -1;
//    }
//    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_RD_MODE32, &ulMode32) == -1)
//    {
//        perror("SPI: Can't get SPI mode32.");
//        return -1;
//    }
    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_WR_BITS_PER_WORD, &iBits) == -1)
    {
        perror("SPI: Can't set bits per word.");
        return -1;
    }
    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_RD_BITS_PER_WORD, &iBits) == -1)
    {
        perror("SPI: Can't get bits per word.");
        return -1;
    }
    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_WR_MAX_SPEED_HZ, &ulSpeed) == -1)
    {
        perror("SPI: Can't set max speed HZ");
        return -1;
    }
    if (ioctl(m_iDeviceDescriptorServer, SPI_IOC_RD_MAX_SPEED_HZ, &ulSpeed) == -1)
    {
        perror("SPI: Can't get max speed HZ.");
        return -1;
    }

    return 0;
}

//-------------------------------------------------------------------------------
int8_t CSpiCommunicationDevice::Close(void)
{
    close(m_iDeviceDescriptorServer);
}

//-------------------------------------------------------------------------------
void CSpiCommunicationDevice::ChipSelectPinSet(void)
{
    //CGpio::SetPinOutput(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    //CGpio::SetPinOutput(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    //CGpio::SetPinOutput(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    //CGpio::SetPinOutput(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    //CGpio::SetPinOutput(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
    //CGpio::SetPinOutput(SPI_CHIP_MIND_ENABLE_PIN_PORT, SPI_CHIP_MIND_ENABLE_PIN);

    //CGpio::SetPin(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    //CGpio::SetPin(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    //CGpio::SetPin(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    //CGpio::SetPin(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    //CGpio::ClearPin(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
    //CGpio::SetPin(SPI_CHIP_MIND_ENABLE_PIN_PORT, SPI_CHIP_MIND_ENABLE_PIN);
}

//-------------------------------------------------------------------------------
void CSpiCommunicationDevice::ChipSelectPinDelete(void)
{
    //CGpio::SetPinInput(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    //CGpio::SetPinInput(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    //CGpio::SetPinInput(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    //CGpio::SetPinInput(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    //CGpio::SetPinInput(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
    //CGpio::SetPinInput(SPI_CHIP_MIND_ENABLE_PIN_PORT, SPI_CHIP_MIND_ENABLE_PIN);

    //CGpio::ClearPin(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    //CGpio::ClearPin(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    //CGpio::ClearPin(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    //CGpio::ClearPin(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    //CGpio::ClearPin(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
    //CGpio::ClearPin(SPI_CHIP_MIND_ENABLE_PIN_PORT, SPI_CHIP_MIND_ENABLE_PIN);
}

//-------------------------------------------------------------------------------
// массив сопоставления номера посадочного слота модуля с адресом на шине SPI.
const uint8_t CSpiCommunicationDevice::aui8ModuleSlotNumberToSpiAddressMatching[] =
{
//    0x0F,
//    0x07,
//    0x0B,
//    0x03,
//    0x0D,
//    0x05,
//    0x09,
//    0x01,
//    0x0E,
//    0x06,
//    0x0A,
//    0x02,
//    0x0C,
//    0x04,
//    0x08,
    0x00
};

//-------------------------------------------------------------------------------
// устанавливает адрес на шине SPI. зажигает и гасит светодиод контроля обмена данными.
void CSpiCommunicationDevice::ChipSelectAddressSet(uint8_t ucAddress)
{
    uint8_t ucAddressLocal;

    pxGpioPrdEnablePin -> SetPin();

    ucAddressLocal = aui8ModuleSlotNumberToSpiAddressMatching[ucAddress];

    if (ucAddressLocal & 0x01)
    {
        //CGpio::SetPin(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    }
    else
    {
        //CGpio::ClearPin(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    }

    ucAddressLocal = (ucAddressLocal >> 1);
    if (ucAddressLocal & 0x01)
    {
        //CGpio::SetPin(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    }
    else
    {
        //CGpio::ClearPin(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    }

    ucAddressLocal = (ucAddressLocal >> 1);
    if (ucAddressLocal & 0x01)
    {
        //CGpio::SetPin(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    }
    else
    {
        //CGpio::ClearPin(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    }

    ucAddressLocal = (ucAddressLocal >> 1);
    if (ucAddressLocal & 0x01)
    {
        //CGpio::SetPin(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    }
    else
    {
        //CGpio::ClearPin(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    }

    pxGpioSpiChipEnablePin -> SetPin();
    usleep(380);
    pxGpioPrdEnablePin -> ClearPin();
}
uint8_t uiExchangeCounter = 0;
//-------------------------------------------------------------------------------
// производит обмен данными по SPI.
int CSpiCommunicationDevice::Exchange(uint8_t uiAddress,
                                      uint8_t *pucTxBuff,
                                      uint8_t *pucRxBuff,
                                      int iLength,
                                      int iSpeed)
{
//    std::cout << "CSpiCommunicationDevice::Exchange 1"  << std::endl;

//    if (uiExchangeCounter >= 10)
//    {
//        return -1;
//    }
//    else
//    {
//        uiExchangeCounter++;
//    }

    while (m_pxSpi0Semaphore -> Acquire() == false);
//    if (!(m_pxSpi0Semaphore -> Acquire()))
//    {
//        std::cout << "CSpiCommunicationDevice::Exchange 2"  << std::endl;
//        return -1;
//    }
//    usleep(5000);
//
//    Open();

    ModeSet();
    ChipSelectAddressSet(uiAddress);

//    memset(aucSpiTxBuffer, 0, iLength);
//    memset(aucSpiRxBuffer, 0, iLength);
//    memset(aucSpiRxBuffer, 0, 4);

    struct spi_ioc_transfer xTransfer =
    {
        //the xTransfer structure
        (unsigned long) pucTxBuff,	    // tx_buf;        //the buffer for sending data
        (unsigned long) pucRxBuff,    // rx_buf;         //the buffer for receiving data
        (unsigned long) iLength,      // len;           //the length of buffer
        (unsigned long) iSpeed,       // speed_hz;      //the speed in Hz
        (unsigned int) 0,             // delay_usecs;   //delay in us
        (uint8_t) 8             // bits_per_word; //bits per word
        //(uint8_t)               // cs_change;
        //(uint8_t)               // tx_nbits;
        //(uint8_t)               // rx_nbits;
        //(unsigned int)                // pad;

    };
    // send the SPI message (all of the above fields, inc. buffers)
    int iStatus = ioctl(m_iDeviceDescriptorServer, SPI_IOC_MESSAGE(1), &xTransfer);

    pxGpioSpiChipEnablePin -> ClearPin();

//    Close();
    m_pxSpi0Semaphore -> Release();

    if (iStatus < 0)
    {
        perror("SPI: SPI_IOC_MESSAGE Failed");
        return -1;
    }

    return iStatus;
}

//-------------------------------------------------------------------------------
