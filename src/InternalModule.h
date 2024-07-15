#ifndef CINTERNALMODULE_H
#define CINTERNALMODULE_H
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
#include <string.h>

#include "Platform.h"
#include "Dfa.h"
#include "Timer.h"


//#define GET_MODULE_TYPE_COMMAND 0x91 // команда - чтение типа модуля.
#define CHANGE_PROTOCOL_COMMAND 0x92 // команда - переключить на новый протокол.
//#define GET_MODULE_TYPE_COMMAND_LENGTH 1
//#define GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH 1
// смещение в массиве RX SPI. 0 - команда запроса, 1 - "эхо" команды запроса.
#define GET_MODULE_TYPE_COMMAND_OFFSET 1

//#define ONE_BYTE_CRC_LENGTH 1
//#define TWO_BYTE_CRC_LENGTH 2
// смещение на контрольную сумму блока базы данных.
#define PLC_DATA_BASE_BLOCK_CRC_OFFSET (PLC_DATA_BASE_BLOCK_LENGTH - TWO_BYTE_CRC_LENGTH)
// 2 - смещение на тип модуля в массиве RX SPI. 0 - команда запроса, 1 - "эхо" команды запроса.
//#define MODULE_TYPE_OFFSET 2
#define MODULE_COMMAND_LENGTH 1
// количество байт в ответе на запрос типа модуля.
#define MODULE_COMMAND_ANSWER_LENGTH 1
// количество байт в "хвосте" ответа модуля. 0x7E - OK, 0xE7 - error.
#define TAIL_ANSWER_LENGTH 1
#define DATA_EXCHANGE_OK 0x7E
#define DATA_EXCHANGE_ERROR 0xE7
#define DATA_EXCHANGE_CRC_ERROR 0x03
// MVAI5_REPER_POINTS_ADC_CHANNEL_QUANTITY + MVAI5_ANALOG_INPUT_QUANTITY// 6х7=42.
#define ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH 42

// максимальное количество адресов на шине SPI.
#define SPI_CHIP_SELECT_MAX_ADDRESS 16
// 2 - смещение на данные в массиве RX SPI. 0 - команда запроса, 1 - "эхо" команды запроса.
//#define SPI_DATA_BYTE_OFFSET 2
// 1 - смещение на команду в массиве RX SPI.
//#define SPI_COMMAND_BYTE_OFFSET 1
//#define SPI_COMMAND_BYTE_LENGTH 1

class CResources;
class CCommunicationDeviceInterface;

enum
{
    SPI_PREAMBLE_LENGTH = 2,
    SPI_BUFFER_LENGTH = 64,
//        SPI_MAX_BUS_ADDRESS = 16,
// 2 - смещение на данные в массиве RX SPI. 0 - команда запроса, 1 - "эхо" команды запроса.
    SPI_DATA_BYTE_OFFSET = 2,
// 1 - смещение на команду в массиве RX SPI.
    SPI_COMMAND_BYTE_OFFSET = 1,
    SPI_COMMAND_BYTE_LENGTH = 1,
    ONE_BYTE_CRC_LENGTH = 1,
    TWO_BYTE_CRC_LENGTH = 2,
    GET_MODULE_TYPE_COMMAND = 0x91,
    GET_MODULE_TYPE_COMMAND_LENGTH = 1,
    GET_MODULE_TYPE_REQUEST_LENGTH = 4,
    GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH = 1,
    MODULE_REQUEST_QUANTITY = 5,
    MODULE_TYPE_OFFSET = SPI_DATA_BYTE_OFFSET,
// начальное значение счётчика запросов модуля без ответа.
// если равно нулю - модуль считается неисправным. его данные признаются недостоверными.
// устанавливается флаг сигнализации в рабочем массиве aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET + индекс модуля].
    BAD_MODULE_CYCLE_COUNT_DEFAULT = 10,
};


//-------------------------------------------------------------------------------
class CInternalModuleInterface : public CDfa
{
public:
    enum
    {
        MISSING_COMPONENT = 0,
        NORMAL_WORK,
        // В отсутствие компонента можно продолжать работу.
        VACATION_COMPONENT,
    };

    CInternalModuleInterface();
    CInternalModuleInterface(uint8_t muiAddress);
    virtual ~CInternalModuleInterface();

    virtual void SetResources(CResources* pxResources) {};
    virtual CResources* GetResources(void) {};

    enum
    {
        REPLACEMENT_TYPES_NUMBER = 8,
    };

    virtual uint8_t GetType(void) {};
    virtual uint8_t GetWorkingStatus(void) {};
    virtual void SetAddress(uint8_t uiAddress) {};
    virtual uint8_t GetAddress(void) {};
    virtual bool IsReadyToStartWork(void) {};
    virtual bool IsAbleToReplace(uint8_t uiType) {};
    virtual void SetCommunicationDevice(CCommunicationDeviceInterface* pxCommunicationDevice) {};
    virtual uint8_t GetModuleType(uint8_t uiAddress) {};
    virtual uint8_t DataBaseRead(uint8_t uiAddress) {};
};
//-------------------------------------------------------------------------------








//-------------------------------------------------------------------------------
class CInternalModule : public CInternalModuleInterface
{
public:
    CInternalModule();
    CInternalModule(uint8_t muiAddress);
    virtual ~CInternalModule();

    void SetResources(CResources* pxResources);
    CResources* GetResources(void);

    uint8_t GetType(void);

    uint8_t GetWorkingStatus(void);

    void SetAddress(uint8_t uiAddress);
    uint8_t GetAddress(void);

    bool IsReadyToStartWork(void);
    bool IsAbleToReplace(uint8_t uiType);
    void SetCommunicationDevice(CCommunicationDeviceInterface* pxCommunicationDevice);
    uint8_t GetModuleType(uint8_t uiAddress);
//    uint8_t DataBaseRead(uint8_t uiAddress);

private:
    uint8_t m_uiAddress;
    CCommunicationDeviceInterface* m_pxCommunicationDevice;
    CResources* m_pxResources;
};

#endif // CINTERNALMODULE_H
