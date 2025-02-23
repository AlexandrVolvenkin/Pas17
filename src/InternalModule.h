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

class Timer;
class Platform;
class CTask;
class CResources;
class CCommunicationDevice;
class CCommunicationDeviceInterface;
class CSpiCommunicationDevice;
class CDataContainerInterface;
class CDataContainerDataBase;

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
// начальное значение счётчика запросов модуля без ответа в течении 60 секунд.
    BAD_MODULE_60_SECOND_CYCLE_COUNT_DEFAULT = 600,
    BAD_MODULE_RESPONDED_OK = 0,
// код ошибки когда модуль не отвечает.
// модуль не отвечает.
    BAD_MODULE_NOT_RESPONDED = 1,
// модуль не отвечает при старте.
    BAD_MODULE_NOT_RESPONDED_ON_START = 2,
// сигнатура ошибки. устанавливается если измеренное значение недостоверно.
    ANALOGUE_INPUT_ERROR = 0xFF00,
};


//-------------------------------------------------------------------------------
class CInternalModuleInterface : public CTask
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

    virtual void SetCommunicationDeviceName(std::string sName) {};
    virtual void SetCommunicationDevice(CCommunicationDeviceInterface* pxCommunicationDevice) {};
    void CommunicationDeviceInit(const char* pccIpAddress,
                                 uint16_t uiPort) {};
    virtual uint8_t Init(void) {};
    virtual bool SetTaskData(CDataContainerDataBase* pxDataContainer) {};
    virtual bool GetTaskData(CDataContainerDataBase* pxDataContainer) {};

    enum
    {
        REPLACEMENT_TYPES_NUMBER = 8,
    };

//    virtual uint8_t GetType(void) {};
//    virtual uint8_t GetWorkingStatus(void) {};
//    virtual void SetAddress(uint8_t uiAddress) {};
//    virtual uint8_t GetAddress(void) {};
//    virtual bool IsReadyToStartWork(void) {};
//    virtual bool IsAbleToReplace(uint8_t uiType) {};
//    virtual void SetCommonIndex(uint8_t uiCommonIndex) {};
//    virtual uint8_t GetCommonIndex(void) {};
//    virtual void SetCommandControl(uint8_t uiCommandControl) {};
//    virtual uint8_t GetCommandControl(void) {};
//    virtual void SetErrorCode(uint8_t uiErrorCode) {};
//    virtual uint8_t GetErrorCode(void) {};
//    virtual void SetBadAnswerCounter(uint8_t uiBadAnswerCounter) {};
//    virtual uint8_t GetBadAnswerCounter(void) {};

    virtual void Allocate(void) {};
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

    void SetCommunicationDeviceName(std::string sName);
    void SetCommunicationDevice(CCommunicationDeviceInterface* pxCommunicationDevice);
    uint8_t Init(void);
    bool SetTaskData(CDataContainerDataBase* pxDataContainer);
    bool GetTaskData(CDataContainerDataBase* pxDataContainer);

    uint8_t GetAddress()
    {
        return m_uiAddress;
    };
    void SetAddress(uint8_t uiAddress)
    {
        m_uiAddress = uiAddress;
    };

    uint8_t GetCommonIndex()
    {
        return m_uiCommonIndex;
    };
    void SetCommonIndex(uint8_t uiCommonIndex)
    {
        m_uiCommonIndex = uiCommonIndex;
    };

    uint8_t GetCommandControl()
    {
        return m_uiCommandControl;
    };
    void SetCommandControl(uint8_t uiCommandControl)
    {
        m_uiCommandControl = uiCommandControl;
    };

    uint8_t GetErrorCode()
    {
        return m_uiErrorCode;
    };
    void SetErrorCode(uint8_t uiErrorCode)
    {
        m_uiErrorCode = uiErrorCode;
    };

    uint8_t* GetRxBuffer()
    {
        return m_puiRxBuffer;
    };
    void SetRxBuffer(uint8_t* puiRxBuffer)
    {
        m_puiRxBuffer = puiRxBuffer;
    };

    uint8_t* GetTxBuffer()
    {
        return m_puiTxBuffer;
    };
    void SetTxBuffer(uint8_t* puiTxBuffer)
    {
        m_puiTxBuffer = puiTxBuffer;
    };

    uint8_t* GetDiscreteInputs()
    {
        return m_puiDiscreteInputs;
    };
    void SetDiscreteInputs(uint8_t* puiDiscreteInputs)
    {
        m_puiDiscreteInputs = puiDiscreteInputs;
    };

    uint8_t* GetDiscreteInputsBadState()
    {
        return m_puiDiscreteInputsBadState;
    };
    void SetDiscreteInputsBadState(uint8_t* puiDiscreteInputsBadState)
    {
        m_puiDiscreteInputsBadState = puiDiscreteInputsBadState;
    };

    uint8_t GetBadAnswerCounter()
    {
        return m_uiBadAnswerCounter;
    };
    void SetBadAnswerCounter(uint8_t uiBadAnswerCounter)
    {
        m_uiBadAnswerCounter = uiBadAnswerCounter;
    };

    uint8_t Fsm(void);

//    uint8_t GetType(void);
//
//    uint8_t GetWorkingStatus(void);

//    void SetAddress(uint8_t uiAddress);
//    uint8_t GetAddress(void);
//
//    bool IsReadyToStartWork(void);
//    bool IsAbleToReplace(uint8_t uiType);
//
//    void SetCommonIndex(uint8_t uiCommonIndex);
//    uint8_t GetCommonIndex(void);
//
//    void SetCommandControl(uint8_t uiCommandControl);
//    uint8_t GetCommandControl(void);
//
//    void SetErrorCode(uint8_t uiErrorCode);
//    uint8_t GetErrorCode(void);
//
//    void SetBadAnswerCounter(uint8_t uiBadAnswerCounter);
//    uint8_t GetBadAnswerCounter(void);

    void Allocate(void);
    uint8_t GetModuleType(uint8_t uiAddress);
    void SearchModules(void);

protected:
    std::string m_sCommunicationDeviceName;
    CCommunicationDeviceInterface* m_pxCommunicationDevice;
    CDataContainerDataBase* m_pxOperatingDataContainer;
    uint8_t m_uiAddress;
    uint8_t m_uiCommonIndex;
    uint8_t m_uiCommandControl;
    uint8_t m_uiErrorCode;
    uint8_t* m_puiRxBuffer;
    uint8_t* m_puiTxBuffer;
    uint8_t* m_puiDiscreteInputs;
    uint8_t* m_puiDiscreteInputsBadState;
    uint8_t m_uiBadAnswerCounter;

};

#endif // CINTERNALMODULE_H
