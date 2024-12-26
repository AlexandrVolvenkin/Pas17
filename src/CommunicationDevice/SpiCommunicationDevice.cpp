
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <typeinfo>

#include "Task.h"
#include "Platform.h"
#include "Resources.h"
#include "CommunicationDevice.h"
#include "SpiCommunicationDevice.h"

using namespace std;

//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
CSpiCommunicationDevice::CSpiCommunicationDevice()
{

}

//-------------------------------------------------------------------------------
CSpiCommunicationDevice::~CSpiCommunicationDevice()
{
    ChipSelectPinDelete();
    Close();
}

//-------------------------------------------------------------------------------
void CSpiCommunicationDevice::Init(void)
{
    Open();
//    ChipSelectPinSet();
};

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
            CGpio::SetPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
            usleep(500000);
            CGpio::ClearPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
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

    // Check that the properties have been set
    printf("SPI Mode is: %d\n", iMode);
    printf("SPI Bits is: %d\n", iBits);
    printf("SPI Speed is: %d\n", (int)ulSpeed);
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
    CGpio::SetPinOutput(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    CGpio::SetPinOutput(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    CGpio::SetPinOutput(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    CGpio::SetPinOutput(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    CGpio::SetPinOutput(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
    CGpio::SetPinOutput(SPI_CHIP_MIND_ENABLE_PIN_PORT, SPI_CHIP_MIND_ENABLE_PIN);

    CGpio::SetPin(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    CGpio::SetPin(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    CGpio::SetPin(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    CGpio::SetPin(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    CGpio::ClearPin(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
    CGpio::SetPin(SPI_CHIP_MIND_ENABLE_PIN_PORT, SPI_CHIP_MIND_ENABLE_PIN);
}

//-------------------------------------------------------------------------------
void CSpiCommunicationDevice::ChipSelectPinDelete(void)
{
    CGpio::SetPinInput(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    CGpio::SetPinInput(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    CGpio::SetPinInput(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    CGpio::SetPinInput(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    CGpio::SetPinInput(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
    CGpio::SetPinInput(SPI_CHIP_MIND_ENABLE_PIN_PORT, SPI_CHIP_MIND_ENABLE_PIN);

    CGpio::ClearPin(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    CGpio::ClearPin(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    CGpio::ClearPin(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    CGpio::ClearPin(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    CGpio::ClearPin(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
    CGpio::ClearPin(SPI_CHIP_MIND_ENABLE_PIN_PORT, SPI_CHIP_MIND_ENABLE_PIN);
}

//-------------------------------------------------------------------------------
// массив сопоставления номера посадочного слота модуля с адресом на шине SPI.
const uint8_t CSpiCommunicationDevice::aui8ModuleSlotNumberToSpiAddressMatching[] =
{
    0x0F,
    0x07,
    0x0B,
    0x03,
    0x0D,
    0x05,
    0x09,
    0x01,
    0x0E,
    0x06,
    0x0A,
    0x02,
    0x0C,
    0x04,
    0x08,
    0x00
};

//-------------------------------------------------------------------------------
// устанавливает адрес на шине SPI. зажигает и гасит светодиод контроля обмена данными.
void CSpiCommunicationDevice::ChipSelectAddressSet(unsigned char ucAddress)
{
    unsigned char ucAddressLocal;

    CGpio::ClearPin(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
    CGpio::SetPin(PRD_EN_PIN_PORT, PRD_EN_PIN);

    ucAddressLocal = aui8ModuleSlotNumberToSpiAddressMatching[ucAddress];

    if (ucAddressLocal & 0x01)
    {
        CGpio::SetPin(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    }
    else
    {
        CGpio::ClearPin(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    }

    ucAddressLocal = (ucAddressLocal >> 1);
    if (ucAddressLocal & 0x01)
    {
        CGpio::SetPin(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    }
    else
    {
        CGpio::ClearPin(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    }

    ucAddressLocal = (ucAddressLocal >> 1);
    if (ucAddressLocal & 0x01)
    {
        CGpio::SetPin(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    }
    else
    {
        CGpio::ClearPin(SPI_CHIP_SELECT_PIN_2_PORT, SPI_CHIP_SELECT_PIN_2);
    }

    ucAddressLocal = (ucAddressLocal >> 1);
    if (ucAddressLocal & 0x01)
    {
        CGpio::SetPin(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    }
    else
    {
        CGpio::ClearPin(SPI_CHIP_SELECT_PIN_3_PORT, SPI_CHIP_SELECT_PIN_3);
    }

    usleep(100);
    CGpio::SetPin(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
    CGpio::ClearPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
    usleep(380);
}

//-------------------------------------------------------------------------------
// производит обмен данными по SPI.
int CSpiCommunicationDevice::Exchange(uint8_t uiAddress,
                   unsigned char *pucTxBuff,
                   unsigned char *pucRxBuff,
                   int iLength,
                   int iSpeed)
{
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
        (unsigned char) 8             // bits_per_word; //bits per word
        //(unsigned char)               // cs_change;
        //(unsigned char)               // tx_nbits;
        //(unsigned char)               // rx_nbits;
        //(unsigned int)                // pad;

    };
    // send the SPI message (all of the above fields, inc. buffers)
    int iStatus = ioctl(m_iDeviceDescriptorServer, SPI_IOC_MESSAGE(1), &xTransfer);

//    CGpio::ClearPin(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);

    if (iStatus < 0)
    {
        perror("SPI: SPI_IOC_MESSAGE Failed");
        return -1;
    }
    return iStatus;
}

//-------------------------------------------------------------------------------
