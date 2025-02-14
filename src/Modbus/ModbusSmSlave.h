#ifndef MODBUSSMSLAVE_H
#define MODBUSSMSLAVE_H
//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
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

#include "Modbus.h"
#include "ModbusSmSlaveLinkLayer.h"

class CTimer;
class CPlatform;
class CTask;
class CResources;
class CDeviceControl;
class CLinkInterface;
class CLink;

//-------------------------------------------------------------------------------
class CModbusSmSlave : public CTask
{
public:
    enum
    {
        COMMUNICATION_START = NEXT_STEP,
        COMMUNICATION_RECEIVE_START,
        MESSAGE_RECEIVE_WAITING,
        REQUEST_PROCESSING,
        ANSWER_PROCESSING_WAITING,
        ANSWER_PROCESSING,
        BEFORE_ANSWERING_WAITING,
        MESSAGE_SEND,
        AFTER_ANSWERING_WAITING,
    };

    enum
    {
        MODBUS_EXCEPTION_CODE_OFFSET = 1,
        MAX_MODBUS_MESSAGE_LENGTH = 256,
    };

    CModbusSmSlave();
    CModbusSmSlave(CResources* pxResources);
    virtual ~CModbusSmSlave();

    uint8_t Init(void);
//    size_t GetObjectLength(void);

    void SetModbusSlaveLinkLayerName(std::string sName);
    void SetModbusSlaveLinkLayer(CModbusSmSlaveLinkLayer* pxModbusSlaveLinkLayer);

    void SetDeviceControlName(std::string sName);
    void SetDeviceControl(CDeviceControl* pxDeviceControl);
    CDeviceControl* GetDeviceContro(void);

    void WorkingArraysInit(void);

    void WorkingArraysInit(uint8_t *puiCoils,
                           uint8_t *puiDiscreteInputs,
                           uint16_t *puiHoldingRegisters,
                           uint16_t *puiInputRegisters,
                           uint16_t uiCoilsNumber,
                           uint16_t uiDiscreteInputsNumber,
                           uint16_t uiHoldingRegistersNumber,
                           uint16_t uiInputRegistersNumber);
    void WorkingArraysCreate(uint16_t uiCoilsNumber,
                             uint16_t uiDiscreteInputsNumber,
                             uint16_t uiHoldingRegistersNumber,
                             uint16_t uiInputRegistersNumber);
    void WorkingArraysDelete(void);
    static const char *ModbusStringError(int errnum);

//-------------------------------------------------------------------------------
    uint16_t ReadExceptionStatus(void);
    uint16_t ReportSlaveID(void);
    uint16_t RequestProcessing(void);

//-------------------------------------------------------------------------------
    uint16_t ReadExceptionStatusAnswer(void);
    uint16_t ReportSlaveIDAnswer(void);
    uint16_t AnswerProcessing(void);

//-------------------------------------------------------------------------------
    uint8_t Fsm(void);

//protected:
//private:

    uint16_t ByteToBitPack(uint16_t,
                           uint16_t,
                           uint8_t *,
                           uint8_t *,
                           uint16_t );
    void SetByteFromBits(uint8_t *, uint16_t, const uint8_t );
    void SetBytesFromBits(uint8_t *, uint16_t, uint16_t,
                          const uint8_t * );
    uint8_t GetByteFromBits(const uint8_t *, uint16_t,
                            uint16_t );
    float GetFloat(const uint16_t * );
    void SetFloat(float, uint16_t * );

public:


//private:
//protected:

    uint16_t AnswerProcessing(uint8_t *puiResponse, uint16_t uiFrameLength);

    uint8_t GetOwnAddress(void)
    {
        return m_uiOwnAddress;
    };
    void SetOwnAddress(uint8_t uiData)
    {
        m_uiOwnAddress = uiData;
    };

    uint8_t GetMessageLength(void)
    {
        return m_uiMessageLength;
    };
    void SetMessageLength(uint8_t uiData)
    {
        m_uiMessageLength = uiData;
    };
    virtual uint16_t CRC_LENGTH(void)
    {
        return 2;
    };


//-------------------------------------------------------------------------------
    std::string m_sModbusSlaveLinkLayerName;
    CModbusSlaveLinkLayerInterface* m_pxModbusSlaveLinkLayer;

    std::string m_sDeviceControlName;
    CDeviceControl* m_pxDeviceControl;

    uint8_t m_uiOwnAddress;
    uint8_t m_uiSlaveAddress;
    uint8_t m_uiFunctionCode;
    uint16_t  m_uiLength;
    uint16_t m_uiAddress;
    uint16_t m_uiQuantity;
    uint16_t m_uiMessageLength;
    // таймоут по отсутствию подтверждения.
    const static uint16_t m_uiConfirmationTimeout = 500;
    const static uint16_t m_uiTransmitDelayTimeout = 5;

    uint8_t *m_puiRxBuffer;
    uint8_t *m_puiTxBuffer;
    // Вспомогательный буфер.
    uint8_t* m_puiIntermediateBuff;

    uint8_t *m_puiCoils;
    uint8_t *m_puiDiscreteInputs;
    uint16_t *m_puiHoldingRegisters;
    uint16_t *m_puiInputRegisters;
    uint16_t m_uiCoilsNumber;
    uint16_t m_uiDiscreteInputsNumber;
    uint16_t m_uiHoldingRegistersNumber;
    uint16_t m_uiInputRegistersNumber;

    CDataContainerDataBase* m_pxOperatingDataContainer;
};

//-------------------------------------------------------------------------------
#endif // MODBUSSMSLAVE_H
