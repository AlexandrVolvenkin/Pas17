#ifndef CMODBUSSMSLAVELINKLAYER_H
#define CMODBUSSMSLAVELINKLAYER_H
//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <stdint.h>
#include <thread>

#include "Modbus.h"
#include "ModbusSlaveLinkLayer.h"
#include "Configuration.h"

/* Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5
 * RS232 / RS485 ADU = 253 bytes + slave (1 byte) + CRC (2 bytes) = 256 bytes
 */
#define MODBUS_SM_MAX_ADU_LENGTH  256

#define _MODBUS_SM_HEADER_LENGTH      1
#define _MODBUS_SM_PRESET_REQ_LENGTH  6
#define _MODBUS_SM_PRESET_RSP_LENGTH  2

#define _MODBUS_SM_CHECKSUM_LENGTH    2

#define MODBUS_SM_RS232 0
#define MODBUS_SM_RS485 1

class CTimer;
class CPlatform;
class CTask;
class CResources;
class CCommunicationDevice;
class CCommunicationDeviceInterface;
class CModbusSlaveLinkLayer;

//-------------------------------------------------------------------------------
class CModbusSmSlaveLinkLayer : public CModbusSlaveLinkLayer
{
public:
    enum
    {
        COMMUNICATION_START = NEXT_STEP,
        COMMUNICATION_RECEIVE_START,
        COMMUNICATION_RECEIVE_CONTINUE,
        COMMUNICATION_RECEIVE_END,
        COMMUNICATION_FRAME_CHECK,
        COMMUNICATION_FRAME_RECEIVED,
        COMMUNICATION_TRANSMIT_START,
        COMMUNICATION_FRAME_TRANSMITED,
        COMMUNICATION_RECEIVE_ERROR,
        COMMUNICATION_STOP,
    };

    CModbusSmSlaveLinkLayer();
    virtual ~CModbusSmSlaveLinkLayer();

    uint8_t Init(void);
    size_t GetObjectLength(void);
    static void Process(CModbusSmSlaveLinkLayer* pxModbusSlaveLinkLayer);
//    void CommunicationDeviceInit(const char* pccDeviceName,
//                                 uint32_t uiBaudRate,
//                                 uint8_t uiDataBits,
//                                 char cParity,
//                                 uint8_t uiStopBit);
    bool SetTaskData(CDataContainerDataBase* pxDataContainer);
    bool GetTaskData(CDataContainerDataBase* pxDataContainer);
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

    void ReceiveEnable(void);
    void ReceiveDisable(void);
    void TransmitEnable(void);
    void TransmitDisable(void);

    void CommunicationStart(void);
    void CommunicationReceiveStart(void);
    void ReceiveStart(void);
    void TransmitStart(void);

    uint16_t RequestBasis(uint8_t uiSlave,
                          uint8_t uiFunctionCode,
                          uint16_t uiAddress,
                          uint16_t uiBitNumber,
                          uint8_t *puiRequest);
    uint16_t ResponseBasis(uint8_t, uint8_t, uint8_t * );
    uint16_t ResponseException(uint8_t, uint8_t, uint8_t, uint8_t * );
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

    uint16_t GetGuardTimeout(void)
    {
        return m_uiGuardTimeout;
    };
    void SetGuardTimeout(uint16_t uiData)
    {
        m_uiGuardTimeout = uiData;
    };

    uint16_t CRC_LENGTH(void)
    {
        return 0;
    };

    uint16_t m_uiRequestTransactionId = 0;
    uint16_t m_uiResponseTransactionId = 0;
    // ������� �� ���������� ���������� ����� 3.5 ����.
    uint16_t m_uiGuardTimeout = 10;
    // ������� �� ���������� �������.
    const static uint16_t m_uiReceiveTimeout = 15000;
    // ������� �� ���������� �������������.
    const static uint16_t m_uiConfirmationTimeout = 500;
    const static uint16_t m_uiTransmitDelayTimeout = 5;

    uint8_t m_auiRxBuffer[MODBUS_SM_MAX_ADU_LENGTH];
    uint8_t m_auiTxBuffer[MODBUS_SM_MAX_ADU_LENGTH];
//    uint16_t m_uiRxBytesNumber;
    uint16_t m_uiFrameLength;
    std::thread* m_pxThread;
    CDataContainerDataBase* m_pxOperatingDataContainer;
};

//-------------------------------------------------------------------------------
#endif // CMODBUSSMSLAVELINKLAYER_H
