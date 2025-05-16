#ifndef CSPICOMMUNICATIONDEVICE_H
#define CSPICOMMUNICATIONDEVICE_H
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
class CSemaphore;
class CCommunicationDevice;
class CCommunicationDeviceInterface;
class CGpio;
//class CPlatform;

//-------------------------------------------------------------------------------
#define SPI_PATH "/dev/spidev0.0"
#define TX_RX_BUFF_SIZE 256
#define SPI_MODE SPI_MODE_0
#define SPI_MODE32 SPI_3WIRE
#define BITS_PER_WORD 8
#define SPEED_IN_HZ 15000UL
#define LOW_SPEED_IN_HZ SPEED_IN_HZ

#define SPI_CHIP_SELECT_PIN_0  7 /* (R2) lcd_data1.gpio2[7] A0*/
#define SPI_CHIP_SELECT_PIN_0_PORT  GPIO_PORT_2
#define SPI_CHIP_SELECT_PIN_1  11  /* (T2) lcd_data5.gpio2[11] A1*/
#define SPI_CHIP_SELECT_PIN_1_PORT  GPIO_PORT_2
#define SPI_CHIP_SELECT_PIN_2  8  /* (R3) lcd_data2.gpio2[8] A2*/
#define SPI_CHIP_SELECT_PIN_2_PORT  GPIO_PORT_2
#define SPI_CHIP_SELECT_PIN_3  6  /* (R1) lcd_data0.gpio2[6] A3*/
#define SPI_CHIP_SELECT_PIN_3_PORT  GPIO_PORT_2
#define SPI_CHIP_ENABLE_PIN  12 /* (T3) lcd_data6.gpio2[12] ENABLE*/
#define SPI_CHIP_ENABLE_PIN_PORT  GPIO_PORT_2
#define SPI_CHIP_MIND_ENABLE_PIN  16 /* (U3) lcd_data10.gpio2[16] SS0*/
#define SPI_CHIP_MIND_ENABLE_PIN_PORT  GPIO_PORT_2

//-------------------------------------------------------------------------------
class CSpiCommunicationDevice : public CCommunicationDevice
{
public:
    CSpiCommunicationDevice();
    virtual ~CSpiCommunicationDevice();

    void Init(void);
    int8_t Open(void);
    int8_t ModeSet(void);
    int8_t Close(void);
    int Exchange(uint8_t uiAddress,
                 uint8_t *pucTxBuff,
                 uint8_t *pucRxBuff,
                 int iLength,
                 int iSpeed);
    void ChipSelectPinSet(void);
    void ChipSelectPinDelete(void);
    void ChipSelectAddressSet(uint8_t ucAddress);

    static const uint8_t BUFFER_LENGTH = 64;
private:
    const char *m_pccDeviceName;
    /* Socket or file descriptor */
    int32_t m_iDeviceDescriptorServer;
    static const uint8_t aui8ModuleSlotNumberToSpiAddressMatching[SPI_MAX_BUS_ADDRESS];
    const char *m_pccSemaphoreKeyFileName = "Spi0Semaphore.sem";
    int m_iSpi0SemaphoreId;
//    CSemaphore xSpi0Semaphore(12345, 1);
    CSemaphore* m_pxSpi0Semaphore;
    CGpio* pxGpioSpiChipEnablePin;
    CGpio* pxGpioPrdEnablePin;

};
//-------------------------------------------------------------------------------
#endif // CSPICOMMUNICATIONDEVICE_H
