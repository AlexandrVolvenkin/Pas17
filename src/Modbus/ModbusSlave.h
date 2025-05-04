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
class CDeviceControl;
class CLinkInterface;
class CLink;

//-------------------------------------------------------------------------------
class CModbusSlave : public CTask
{
public:
    enum
    {
        COMMUNICATION_START = NEXT_STEP,
        COMMUNICATION_RECEIVE_START,
        COMMUNICATION_RECEIVE_CONTINUE,
        MESSAGE_RECEIVE_WAITING,

        REQUEST_PROCESSING,

        EXECUTOR_ANSWER_PROCESSING,

        MESSAGE_TRANSMIT_START,
        MESSAGE_TRANSMIT_BEFORE_WAITING,
        MESSAGE_TRANSMIT_AFTER_WAITING,

        RESPONSE_EXCEPTION_ILLEGAL_FUNCTION,
        RESPONSE_EXCEPTION_ILLEGAL_DATA_ADDRESS,
        RESPONSE_EXCEPTION_ILLEGAL_DATA_VALUE,
        RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE,
        RESPONSE_EXCEPTION_ACKNOWLEDGE,
        RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY,
        RESPONSE_EXCEPTION_NEGATIVE_ACKNOWLEDGE,
        RESPONSE_EXCEPTION_MEMORY_PARITY,
        RESPONSE_EXCEPTION_NOT_DEFINED,
        RESPONSE_EXCEPTION_GATEWAY_PATH,
        RESPONSE_EXCEPTION_GATEWAY_TARGET,
        RESPONSE_EXCEPTION_MAX,

        COMMUNICATION_STOP,
    };

    enum
    {
        MODBUS_EXCEPTION_CODE_OFFSET = 1,
        MAX_MODBUS_MESSAGE_LENGTH = 256,
    };


    CModbusSlave();
    CModbusSlave(CResources* pxResources);
    virtual ~CModbusSlave();

    uint8_t Init(void);
    size_t GetObjectLength(void);

    void SetModbusSlaveLinkLayerName(std::string sName);
    void SetModbusSlaveLinkLayer(CModbusSlaveLinkLayer* pxModbusSlaveLinkLayer);

    void SetDeviceControlName(std::string sName);
    void SetDeviceControl(CDeviceControl* pxDeviceControl);
    CDeviceControl* GetDeviceControl(void);

    void WorkingArraysInit(void);
    static const char *ModbusStringError(int errnum);

//-------------------------------------------------------------------------------
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
    uint16_t OnlineDataRead(void);
    uint16_t RequestProcessing(void);

//-------------------------------------------------------------------------------
    uint16_t ReadCoilsAnswer(void);
    uint16_t ReadDiscreteInputsAnswer(void);
    uint16_t ReadHoldingRegistersAnswer(void);
    uint16_t ReadInputRegistersAnswer(void);
    uint16_t WriteSingleCoilAnswer(void);
    uint16_t WriteSingleRegisterAnswer(void);
    uint16_t WriteMultipleCoilsAnswer(void);
    uint16_t WriteMultipleRegistersAnswer(void);
    uint16_t ReadExceptionStatusAnswer(void);
    uint16_t ReportSlaveIDAnswer(void);
    uint16_t WriteAndReadRegistersAnswer(void);
    uint16_t ProgrammingAnswer(void);
    uint16_t PollProgrammingAnswer(void);
    uint16_t DataBaseReadAnswer(void);
    uint16_t DataBaseWriteAnswer(void);
    uint16_t OnlineDataReadAnswer(void);
    uint16_t AnswerProcessing(void);

//-------------------------------------------------------------------------------
    uint8_t Fsm(void);

//protected:
//private:

    uint16_t ResponseException(uint8_t uiExceptionCode);
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
    uint8_t m_uiModbusSlaveLinkLayerId;

    std::string m_sDeviceControlName;
    CDeviceControl* m_pxDeviceControl;
    uint8_t m_uiDeviceControlId;

    uint8_t m_uiOwnAddress;
    uint8_t m_uiSlaveAddress;
    uint8_t m_uiFunctionCode;
    uint16_t  m_uiLength;
    uint16_t m_uiAddress;
    uint16_t m_uiQuantity;
    uint16_t m_uiMessageLength;
    // ������� �� ���������� �������������.
    const static uint16_t m_uiConfirmationTimeout = 500;
    const static uint16_t m_uiTransmitDelayTimeout = 5;

    uint8_t *m_puiRxBuffer;
    uint8_t *m_puiTxBuffer;
    // ��������������� �����.
    uint8_t* m_puiIntermediateBuff;

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

    CDataContainerDataBase* m_pxOperatingDataContainer;
};

//-------------------------------------------------------------------------------

#endif // CMODBUSSLAVE_H



