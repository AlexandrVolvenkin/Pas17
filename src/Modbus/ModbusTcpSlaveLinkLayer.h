//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CMODBUSTCPSLAVELINKLAYER_H
#define CMODBUSTCPSLAVELINKLAYER_H

#include <stdint.h>
#include "Modbus.h"
#include "Configuration.h"
#include "Platform.h"
#include "Timer.h"
#include "Task.h"

/* Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5
 * RS232 / RS485 ADU = 253 bytes + slave (1 byte) + CRC (2 bytes) = 256 bytes
 */
//#define MODBUS_RTU_MAX_ADU_LENGTH  256


#define MODBUS_TCP_DEFAULT_PORT   502
#define MODBUS_TCP_SLAVE         0x11

/* Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5
 * TCP MODBUS ADU = 253 bytes + MBAP (7 bytes) = 260 bytes
 */
#define MODBUS_TCP_MAX_ADU_LENGTH  260

#define _MODBUS_TCP_HEADER_LENGTH      7
#define _MODBUS_TCP_PRESET_REQ_LENGTH 12
#define _MODBUS_TCP_PRESET_RSP_LENGTH  8

#define _MODBUS_TCP_CHECKSUM_LENGTH    0

//-------------------------------------------------------------------------------
class CModbusTcpSlaveLinkLayer : public CModbusSlaveLinkLayerInterface,
    public CTaskInterface
{
public:
    enum
    {
        START = 0,
        READY,
        IDDLE,
        STOP,

        COMMUNICATION_START,
        COMMUNICATION_RECEIVE_START,
        COMMUNICATION_RECEIVE_CONTINUE,
        COMMUNICATION_RECEIVE_END,
        COMMUNICATION_FRAME_RECEIVED,
        COMMUNICATION_TRANSMIT_START,
        COMMUNICATION_FRAME_TRANSMITED,
        COMMUNICATION_RECEIVE_ERROR,



//        REQUEST_ENABLE,
//        WAITING_ACCEPT,
//        START_REQUEST,
//        WAITING_MESSAGE_REQUEST,
//        RECEIVE_MESSAGE_REQUEST,
//        REQUEST_PROCESSING_REQUEST,
//        FRAME_TRANSMIT_CONFIRMATION,
//        WAITING_FRAME_TRANSMIT_CONFIRMATION,
//        END_WAITING_FRAME_TRANSMIT_CONFIRMATION,
//        STOP_REQUEST,
//        REQUEST_ERROR,
//
//        RESTART,
    };

    CModbusTcpSlaveLinkLayer();
    virtual ~CModbusTcpSlaveLinkLayer();


    void CommunicationDeviceInit(const char* pccIpAddress,
                                 uint16_t uiPort);
    uint8_t Fsm(void);

    uint8_t* GetRxBuffer(void);
    uint8_t* GetTxBuffer(void);
    uint8_t* GetRxPdu(void);
    uint8_t* GetTxPdu(void);
    uint8_t GetPduOffset(void);

    uint8_t GetSlaveAddress(void);
    void SetSlaveAddress(uint8_t uiData);
    uint8_t GetFunctionCode(void);
    void SetFunctionCode(uint8_t uiData);
    uint16_t GetDataAddress(void);
    void SetDataAddress(uint16_t uiData);
    uint16_t GetBitNumber(void);
    void SetBitNumber(uint16_t uiData);

protected:
private:

//    void Reset(void);
    void ReceiveEnable(void);
    void ReceiveDisable(void);
    void TransmitEnable(void);
    void TransmitDisable(void);
    uint16_t RequestBasis(uint8_t uiSlave,
                          uint8_t uiFunctionCode,
                          uint16_t uiAddress,
                          uint16_t uiBitNumber,
                          uint8_t *puiRequest);
    uint16_t ResponseBasis(uint8_t, uint8_t, uint8_t * );
    uint16_t RequestHeader(uint8_t uiSlave);
    uint16_t ResponseHeader(uint8_t uiSlave);
    uint16_t Tail(uint8_t *, uint16_t );
    uint16_t Send(uint8_t *, uint16_t );
    int16_t Receive(uint8_t *, uint16_t );
    int8_t FrameCheck(uint8_t *, uint16_t );

    uint16_t GetFrameLength(void)
    {
        return m_uiFrameLength;
    };
    void SetFrameLength(uint16_t uiData)
    {
        m_uiFrameLength = uiData;
    };

    bool IsDataWrited(void)
    {
        return m_pxCommunicationDevice -> IsDataWrited();
    };

    uint16_t GetGuardTimeout(void)
    {
        return m_uiGuardTimeout;
    };
    void SetGuardTimeout(uint16_t uiData)
    {
        m_uiGuardTimeout = uiData;
    };

    uint16_t HEADER_LENGTH(void)
    {
        return 7;
    };
    uint16_t CRC_LENGTH(void)
    {
        return 0;
    };

    CTcpCommunicationDevice* m_pxCommunicationDevice;
    uint16_t m_uiRequestTransactionId = 0;
    uint16_t m_uiResponseTransactionId = 0;
    // таймоут по отсутствию следующего байта 3.5 бода.
    uint16_t m_uiGuardTimeout;
    // таймоут по отсутствию запроса.
    const static uint16_t m_uiReceiveTimeout = 15000;
    // таймоут по отсутствию подтверждения.
    const static uint16_t m_uiConfirmationTimeout = 500;
    const static uint16_t m_uiTransmitDelayTimeout = 5;

    uint8_t m_auiRxBuffer[MODBUS_TCP_MAX_ADU_LENGTH];
    uint8_t m_auiTxBuffer[MODBUS_TCP_MAX_ADU_LENGTH];
//    uint16_t m_uiRxBytesNumber;
    uint16_t m_uiFrameLength;
};

//-------------------------------------------------------------------------------
#endif // CMODBUSTCPSLAVELINKLAYER_H