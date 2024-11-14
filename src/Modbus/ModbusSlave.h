//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CMODBUSSLAVE_H
#define CMODBUSSLAVE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "Modbus.h"
#include "ModbusSlaveLinkLayer.h"

class CTimer;
class CPlatform;
class CTask;
class CResources;

//-------------------------------------------------------------------------------
class CModbusSlave : public CTask
{
public:
    enum
    {
        COMMUNICATION_START = NEXT_STEP,
        MESSAGE_RECEIVE_WAITING,
        REQUEST_PROCESSING,
        BEFORE_ANSWERING_WAITING,
        MESSAGE_SEND,
        AFTER_ANSWERING_WAITING,
    };

    enum
    {
        MODBUS_EXCEPTION_CODE_OFFSET = 1,
    };

    CModbusSlave();
    CModbusSlave(CResources* pxResources);
    virtual ~CModbusSlave();

    void SetModbusSlaveLinkLayerName(std::string sName);
    void SetModbusSlaveLinkLayer(CModbusSlaveLinkLayer* pxModbusSlaveLinkLayer);
//    void SetResources(CResources* pxResources);
//    CResources* GetResources(void);
    void ModbusWorkingArraysInit(void);

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
//    void SlaveSet(uint8_t );

    uint16_t ReadCoils(void);
    uint16_t ReadDiscreteInputs(void);
    uint16_t ReadHoldingRegisters(void);
    uint16_t ReadInputRegisters(void);
    uint16_t WriteSingleCoil(void);
    uint16_t WriteSingleRegister(void);
    uint16_t WriteMultipleCoils(void);
    uint16_t WriteMultipleRegisters(void);
    uint16_t ReadExceptionStatus(void);
    uint16_t ReportSlaveID(void);
    uint16_t WriteAndReadRegisters(void);
    uint16_t Programming(void);
    uint16_t PollProgramming(void);
    uint16_t DataBaseRead(void);
    uint16_t DataBaseWrite(void);
    uint16_t RequestProcessing(void);

    uint8_t Fsm(void);

    void SetModbusSlaveLinkLayer(CModbusSlaveLinkLayerInterface* pxModbusSlaveLinkLayer)
    {
        m_pxModbusSlaveLinkLayer = pxModbusSlaveLinkLayer;
    };

//protected:
//private:

//    virtual bool IsDataWrited(void) = 0;
//    int8_t MessengerIsReady(void);
//    virtual uint16_t Tail(uint8_t *, uint16_t ) = 0;
    uint16_t RequestBasis(uint8_t uiSlave,
                          uint8_t uiFunctionCode,
                          uint16_t uiAddress,
                          uint16_t uiBitNumber,
                          uint8_t *puiRequest);
    uint16_t ResponseBasis(uint8_t, uint8_t, uint8_t * );
    uint16_t ResponseException(uint8_t, uint8_t, uint8_t, uint8_t * );
//    uint16_t SendMessage(uint8_t *, uint16_t );
//    virtual uint16_t Send(uint8_t *, uint16_t ) = 0;

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
//-------------------------------------------------------------------------------
// ModbusMaster
//    int8_t ReadCoilsRequest(uint16_t uiAddress,
//                            uint16_t uiBitNumber);
//    uint16_t ReadCoilsReply(uint8_t *puiDestination);
//    uint8_t CheckConfirmation(uint8_t *puiDestination, uint16_t uiLength);
    int8_t ReadDiscreteInputsRequest(uint8_t uiSlaveAddress,
                                     uint16_t uiAddress,
                                     uint16_t uiBitNumber);
    uint16_t ReadDiscreteInputsReceive(uint8_t *puiMessage, uint16_t uiLength);


//private:
//protected:

//    static uint8_t CheckConfirmation(uint8_t *puiResponse, uint16_t uiLength);
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

    virtual uint16_t HEADER_LENGTH(void)
    {
        return 1;
    };
    virtual uint16_t CRC_LENGTH(void)
    {
        return 2;
    };


//-------------------------------------------------------------------------------
    std::string m_sModbusSlaveLinkLayerName;
    CModbusSlaveLinkLayerInterface* m_pxModbusSlaveLinkLayer;

    uint8_t m_uiOwnAddress;
    uint8_t m_uiSlaveAddress;
    uint8_t m_uiFunctionCode;
    uint16_t m_uiQuantity;
    uint16_t m_uiMessageLength;
    // таймоут по отсутствию подтверждения.
    const static uint16_t m_uiConfirmationTimeout = 500;
    const static uint16_t m_uiTransmitDelayTimeout = 5;

    uint8_t *m_puiRxBuffer;
    uint8_t *m_puiTxBuffer;

    uint8_t *m_puiCoils;
    uint8_t *m_puiDiscreteInputs;
    uint16_t *m_puiHoldingRegisters;
    uint16_t *m_puiInputRegisters;
    uint16_t m_uiCoilsNumber;
    uint16_t m_uiDiscreteInputsNumber;
    uint16_t m_uiHoldingRegistersNumber;
    uint16_t m_uiInputRegistersNumber;

    friend class CModbusRtu;
    friend class CModbusTcp;
};

//-------------------------------------------------------------------------------

#endif // CMODBUSSLAVE_H



