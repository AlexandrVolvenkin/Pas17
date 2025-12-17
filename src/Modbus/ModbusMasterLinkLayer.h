#ifndef CMODBUSMASTERLINKLAYER_H
#define CMODBUSMASTERLINKLAYER_H
//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <iostream>
#include <string.h>

//#include "Dfa.h"
#include "Configuration.h"

class CTimer;
class CPlatform;
class CTask;
class CResources;
class CCommunicationDevice;
class CCommunicationDeviceInterface;
class CDataContainerInterface;
class CDataContainerDataBase;

//-------------------------------------------------------------------------------
class CModbusMasterLinkLayerInterface : public CTask
{
public:
//    enum
//    {
//        COMMUNICATION_START = NEXT_STEP,
//        COMMUNICATION_RECEIVE_START,
//        COMMUNICATION_RECEIVE_END,
//        COMMUNICATION_FRAME_CHECK,
//        COMMUNICATION_FRAME_RECEIVED,
//        COMMUNICATION_TRANSMIT_START,
//        COMMUNICATION_FRAME_TRANSMITED,
//        COMMUNICATION_RECEIVE_ERROR,
//    };

//    CModbusMasterLinkLayerInterface();
//    virtual ~CModbusMasterLinkLayerInterface();

    virtual uint8_t Init(void) {};
    virtual void SetCommunicationDeviceName(std::string sName) {};
    virtual void SetCommunicationDevice(CCommunicationDeviceInterface* pxCommunicationDevice) {};
    void CommunicationDeviceInit(const char* pccIpAddress,
                                 uint16_t uiPort) {};
    virtual bool SetTaskData(CDataContainerDataBase* pxDataContainer) {};
    virtual bool GetTaskData(CDataContainerDataBase* pxDataContainer) {};

    uint8_t Fsm(void) {};

    virtual uint8_t* GetRxBuffer(void) {};
    virtual uint8_t* GetTxBuffer(void) {};
    virtual uint8_t* GetRxPdu(void) {};
    virtual uint8_t* GetTxPdu(void) {};

    virtual uint8_t GetPduOffset(void) {};

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


//    void ReceiveEnable(void) {};
//    void ReceiveDisable(void) {};
//    void TransmitEnable(void) {};
//    void TransmitDisable(void) {};

    virtual void CommunicationStart(void) {};
    virtual void CommunicationReceiveStart(void) {};
    virtual void ReceiveStart(void) {};
    virtual void TransmitStart(void) {};

    virtual uint16_t RequestBasis(uint8_t uiSlave,
                                  uint8_t uiFunctionCode,
                                  uint16_t uiAddress,
                                  uint16_t uiBitNumber,
                                  uint8_t *puiRequest) {};
    virtual uint16_t ResponseBasis(uint8_t, uint8_t, uint8_t* ) {};
    virtual uint16_t ResponseException(uint8_t, uint8_t, uint8_t, uint8_t* ) {};
    virtual uint16_t Tail(uint8_t *, uint16_t ) {};
    virtual uint16_t Send(uint8_t *, uint16_t ) {};
    virtual int8_t FrameCheck(uint8_t *, uint16_t ) {};

    virtual uint16_t GetFrameLength(void) {};
    virtual void SetFrameLength(uint16_t uiData) {};

protected:
private:

};
//-------------------------------------------------------------------------------








//-------------------------------------------------------------------------------
class CModbusMasterLinkLayer : public CModbusMasterLinkLayerInterface
{
public:
    CModbusMasterLinkLayer();
    virtual ~CModbusMasterLinkLayer();

    void SetCommunicationDeviceName(std::string sName);
    void SetCommunicationDevice(CCommunicationDeviceInterface* pxCommunicationDevice);

protected:
    std::string m_sCommunicationDeviceName;
    CCommunicationDeviceInterface* m_pxCommunicationDevice;
private:
};
//-------------------------------------------------------------------------------


#endif // CMODBUSMASTERLINKLAYER_H
