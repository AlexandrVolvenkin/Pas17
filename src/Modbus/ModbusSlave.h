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

#include "../Platform.h"
//#include "Resources.h"
//#include "../Task.h"
//#include "../Dfa.h"
#include "../Timer.h"
#include "Modbus.h"
#include "ModbusSlaveLinkLayer.h"

class CTask;
class CResources;

//-------------------------------------------------------------------------------
class CModbusSlave : public CTask//, public CDfa
{
public:
    enum
    {
        START = 0,
        READY,
        IDDLE,
        STOP,

        COMMUNICATION_START,
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

    uint16_t ReadCoils(uint8_t *, uint8_t *, uint16_t );
    uint16_t ReadDiscreteInputs(uint8_t *, uint8_t *, uint16_t );
    uint16_t ReadHoldingRegisters(uint8_t *, uint8_t *, uint16_t );
    uint16_t ReadInputRegisters(uint8_t *, uint8_t *, uint16_t );
    uint16_t WriteSingleCoil(uint8_t *, uint8_t *, uint16_t );
    uint16_t WriteSingleRegister(uint8_t *, uint8_t *, uint16_t );
    uint16_t WriteMultipleCoils(uint8_t *, uint8_t *, uint16_t );
    uint16_t WriteMultipleRegisters(uint8_t *, uint8_t *, uint16_t );
    uint16_t ReadExceptionStatus(uint8_t *, uint8_t *, uint16_t );
    uint16_t ReportSlaveID(uint8_t *, uint8_t *, uint16_t );
    uint16_t WriteAndReadRegisters(uint8_t *, uint8_t *, uint16_t );
    uint16_t Programming(uint8_t *, uint8_t *, uint16_t );
    uint16_t PollProgramming(uint8_t *, uint8_t *, uint16_t );
    uint16_t DataBaseRead(uint8_t *, uint8_t *, uint16_t );
    uint16_t DataBaseWrite(uint8_t *, uint8_t *, uint16_t );
    uint16_t RequestProcessing(uint8_t *, uint8_t *, uint16_t );
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
    CModbusSlaveLinkLayerInterface* m_pxModbusSlaveLinkLayer;
//    CResources* m_pxResources;

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



