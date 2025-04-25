#ifndef CMODBUSSMMASTER_H
#define CMODBUSSMMASTER_H
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

class CTimer;
class CPlatform;
class CTask;
class CResources;
class CDeviceControl;
class CLinkInterface;
class CLink;
class CModbusMasterLinkLayer;
class CModbusMasterLinkLayerInterface;
class CModbusSmMasterLinkLayer;

//-------------------------------------------------------------------------------
class CModbusSmMaster : public CTask
{
public:
    enum
    {
        COMMUNICATION_START = NEXT_STEP,
        COMMUNICATION_RECEIVE_START,
        MESSAGE_RECEIVE_WAITING,
        REQUEST_PROCESSING,
        EXECUTOR_ANSWER_PROCESSING_WAITING,
        EXECUTOR_ANSWER_PROCESSING,
        BEFORE_ANSWERING_WAITING,
        MESSAGE_SEND,
        AFTER_ANSWERING_WAITING,

        REQUEST_START,
        BEFORE_REQUEST_WAITING,
        AFTER_REQUEST_WAITING,
    };

    enum
    {
        MODBUS_EXCEPTION_CODE_OFFSET = 1,
        MAX_MODBUS_MESSAGE_LENGTH = 256,
    };

    CModbusSmMaster();
    CModbusSmMaster(CResources* pxResources);
    virtual ~CModbusSmMaster();

    uint8_t Init(void);
//    size_t GetObjectLength(void);

    void SetModbusMasterLinkLayerName(std::string sName);
    void SetModbusMasterLinkLayer(CModbusSmMasterLinkLayer* pxModbusMasterLinkLayer);

    void SetDeviceControlName(std::string sName);
    void SetDeviceControl(CDeviceControl* pxDeviceControl);
    CDeviceControl* GetDeviceControl(void);

    void WorkingArraysInit(void);
    static const char *ModbusStringError(int errnum);

//-------------------------------------------------------------------------------
    int8_t ReadDiscreteInputsRequest(uint8_t uiSlaveAddress,
                                     uint16_t uiAddress,
                                     uint16_t uiNumberB);
//    int8_t ReadCoilsRequest(uint16_t uiAddress,
//                            uint16_t uiBitNumber);
//    uint16_t ReadCoilsReply(uint8_t *puiDestination);
//    uint8_t CheckConfirmation(uint8_t *puiDestination, uint16_t uiLength);
//    int8_t ReadDiscreteInputsRequest(uint8_t uiSlaveAddress,
//                                     uint16_t uiAddress,
//                                     uint16_t uiBitNumber);
//    uint16_t ReadDiscreteInputsReceive(uint8_t *puiMessage, uint16_t uiLength);

//-------------------------------------------------------------------------------
    uint16_t ReadDiscreteInputs(void);
    uint16_t ReadExceptionStatus(void);
    uint16_t ReportSlaveID(void);
    uint16_t RequestProcessing(void);

//-------------------------------------------------------------------------------
    uint16_t ReadDiscreteInputsAnswer(void);
    uint16_t ReadExceptionStatusAnswer(void);
    uint16_t ReportSlaveIDAnswer(void);
    uint16_t AnswerProcessing(void);

//-------------------------------------------------------------------------------
    uint8_t Fsm(void);

//protected:
//private:

    uint16_t RequestBasis(uint8_t uiSlave,
                          uint8_t uiFunctionCode,
                          uint16_t uiAddress,
                          uint16_t uiBitNumber,
                          uint8_t *puiRequest);
    uint16_t ResponseBasis(uint8_t, uint8_t, uint8_t * );
    uint16_t ResponseException(uint8_t, uint8_t, uint8_t, uint8_t * );

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
    std::string m_sModbusMasterLinkLayerName;
    CModbusMasterLinkLayerInterface* m_pxModbusMasterLinkLayer;

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


#endif // CMODBUSSMMASTER_H
