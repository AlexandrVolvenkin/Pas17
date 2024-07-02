//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CMODBUSSLAVELINKLAYER_H
#define CMODBUSSLAVELINKLAYER_H

#include <iostream>
#include <string.h>

#include "Dfa.h"
#include "Configuration.h"
#include "Platform.h"
#include "Timer.h"

//-------------------------------------------------------------------------------
class CModbusSlaveLinkLayerInterface : public CDfa
{
public:
    enum
    {
        COMMUNICATION_START = 0,
        COMMUNICATION_RECEIVE_START,
        COMMUNICATION_RECEIVE_CONTINUE,
        COMMUNICATION_FRAME_RECEIVED,
        COMMUNICATION_RECEIVE_ERROR,
    };

//    CModbusSlaveLinkLayerInterface();
//    virtual ~CModbusSlaveLinkLayerInterface();

    void CommunicationDeviceInit(const char* pccIpAddress,
                                 uint16_t uiPort) {};
    uint8_t Fsm(void) {};

    virtual uint8_t* GetRxBuffer(void) {};
    virtual uint8_t* GetTxBuffer(void) {};
    virtual uint8_t* GetRxPdu(void) {};
    virtual uint8_t* GetTxPdu(void) {};

    virtual uint8_t GetSlaveAddress(void) {};
    virtual void SetSlaveAddress(uint8_t uiData) {};
    virtual uint8_t GetFunctionCode(void) {};
    virtual void SetFunctionCode(uint8_t uiData) {};
    virtual uint16_t GetDataAddress(void) {};
    virtual void SetDataAddress(uint16_t uiData) {};
    virtual uint16_t GetBitNumber(void) {};
    virtual void SetBitNumber(uint16_t uiData) {};

    virtual uint16_t RequestHeader(uint8_t uiSlave) {};
    virtual uint16_t ResponseHeader(uint8_t uiSlave) {};


protected:
private:

//    void ReceiveEnable(void) {};
//    void ReceiveDisable(void) {};
//    void TransmitEnable(void) {};
//    void TransmitDisable(void) {};
//    uint16_t RequestBasis(uint8_t uiSlave,
//                          uint8_t uiFunctionCode,
//                          uint16_t uiAddress,
//                          uint16_t uiBitNumber,
//                          uint8_t *puiRequest) {};
//    uint16_t ResponseBasis(uint8_t , uint8_t , uint8_t * ) {};
//    uint16_t Tail(uint8_t * , uint16_t ) {};
//    uint16_t Send(uint8_t * , uint16_t ) {};
//    int16_t Receive(uint8_t * , uint16_t ) {};
//    uint16_t GetFrameLength(void) {};
//    int8_t FrameCheck(uint8_t * , uint16_t ) {};
//
//    bool IsDataWrited(void)
//    {
//        return m_pxCommunicationDevice -> IsDataWrited();
//    };
//
//    uint16_t GetGuardTimeout(void)
//    {
//        return m_uiGuardTimeout;
//    };
//    void SetGuardTimeout(uint16_t uiData)
//    {
//        m_uiGuardTimeout = uiData;
//    };
//
//    uint16_t HEADER_LENGTH(void)
//    {
//        return 7;
//    };
//    uint16_t CRC_LENGTH(void)
//    {
//        return 0;
//    };

};
//-------------------------------------------------------------------------------








//-------------------------------------------------------------------------------
class CModbusSlaveLinkLayer
{
    public:
        CModbusSlaveLinkLayer();
        virtual ~CModbusSlaveLinkLayer();

    protected:

    private:
};
//-------------------------------------------------------------------------------
#endif // CMODBUSSLAVELINKLAYER_H
