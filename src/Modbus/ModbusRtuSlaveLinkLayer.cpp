//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <iostream>
#include <string.h>

#include "ModbusSlaveLinkLayer.h"
#include "ModbusRtuSlaveLinkLayer.h"
#include <Crc.h>

using namespace std;

//-------------------------------------------------------------------------------
CModbusRtuSlaveLinkLayer::CModbusRtuSlaveLinkLayer()
{
    std::cout << "CModbusRtuSlaveLinkLayer constructor"  << std::endl;
    m_pxCommunicationDevice = new CSerialPort();
//    CommunicationDeviceInit("127.0.0.1",
//                            502);
    CommunicationDeviceInit("/dev/ttyO1",
                            9600,
                            8,
                            'N',
                            2);
    SetFsmState(IDDLE);
}

//-------------------------------------------------------------------------------
CModbusRtuSlaveLinkLayer::~CModbusRtuSlaveLinkLayer()
{
    m_pxCommunicationDevice -> Close();
    delete m_pxCommunicationDevice;
}

////-------------------------------------------------------------------------------
//void CModbusRtuSlaveLinkLayer::CommunicationDeviceInit(const char* pccIpAddress,
//        uint16_t uiPort)
//{
////    m_pxCommunicationDevice -> Init();
////    m_pxCommunicationDevice -> SetIpAddress(pccIpAddress);
////    m_pxCommunicationDevice -> SetPort(uiPort);
//
////    m_uiGuardTimeout = 10;
//}
void CModbusRtuSlaveLinkLayer::CommunicationDeviceInit(const char* pccDeviceName,
        uint32_t uiBaudRate,
        uint8_t uiDataBits,
        char cParity,
        uint8_t uiStopBit)
{
    m_pxCommunicationDevice -> Init();
    m_pxCommunicationDevice -> SetDeviceName(pccDeviceName);
    m_pxCommunicationDevice -> SetBaudRate(uiBaudRate);
    m_pxCommunicationDevice -> SetDataBits(uiDataBits);
    m_pxCommunicationDevice -> SetParity(cParity);
    m_pxCommunicationDevice -> SetStopBit(uiStopBit);

    m_uiGuardTimeout = ((((1000000UL / uiBaudRate) * 8UL * 4UL) / 1000UL) + 1);
}

//-------------------------------------------------------------------------------
void CModbusRtuSlaveLinkLayer::ReceiveEnable(void)
{
//    m_pxCommunicationDevice -> Open();
}

//-------------------------------------------------------------------------------
void CModbusRtuSlaveLinkLayer::ReceiveDisable(void)
{
//    m_pxCommunicationDevice -> Close();
}

//-------------------------------------------------------------------------------
void CModbusRtuSlaveLinkLayer::TransmitEnable(void)
{
//    m_pxCommunicationDevice -> Open();
}

//-------------------------------------------------------------------------------
void CModbusRtuSlaveLinkLayer::TransmitDisable(void)
{
//    m_pxCommunicationDevice -> Close();
}

//-------------------------------------------------------------------------------
uint8_t* CModbusRtuSlaveLinkLayer::GetRxBuffer(void)
{
    return &m_auiRxBuffer[0];
}

//-------------------------------------------------------------------------------
uint8_t* CModbusRtuSlaveLinkLayer::GetTxBuffer(void)
{
    return &m_auiTxBuffer[0];
}

//-------------------------------------------------------------------------------
uint8_t* CModbusRtuSlaveLinkLayer::GetRxPdu(void)
{
    return &m_auiRxBuffer[1];
}

//-------------------------------------------------------------------------------
uint8_t* CModbusRtuSlaveLinkLayer::GetTxPdu(void)
{
    return &m_auiTxBuffer[1];
}

//-------------------------------------------------------------------------------
uint8_t CModbusRtuSlaveLinkLayer::GetPduOffset(void)
{
    return 1;
}

//-------------------------------------------------------------------------------
uint8_t CModbusRtuSlaveLinkLayer::GetSlaveAddress(void)
{
    return m_auiRxBuffer[0];
}

//-------------------------------------------------------------------------------
void CModbusRtuSlaveLinkLayer::SetSlaveAddress(uint8_t uiData)
{

}

//-------------------------------------------------------------------------------
uint8_t CModbusRtuSlaveLinkLayer::GetFunctionCode(void)
{
    return m_auiRxBuffer[1];
}

//-------------------------------------------------------------------------------
void CModbusRtuSlaveLinkLayer::SetFunctionCode(uint8_t uiData)
{

}

//-------------------------------------------------------------------------------
uint16_t CModbusRtuSlaveLinkLayer::GetDataAddress(void)
{
    return ((static_cast<uint16_t>(m_auiRxBuffer[2]) << 8) |
            (static_cast<uint16_t>(m_auiRxBuffer[3])));
}

//-------------------------------------------------------------------------------
void CModbusRtuSlaveLinkLayer::SetDataAddress(uint16_t uiData)
{

}

//-------------------------------------------------------------------------------
uint16_t CModbusRtuSlaveLinkLayer::GetBitNumber(void)
{
    return ((static_cast<uint16_t>(m_auiRxBuffer[4]) << 8) |
            (static_cast<uint16_t>(m_auiRxBuffer[5])));
}

//-------------------------------------------------------------------------------
void CModbusRtuSlaveLinkLayer::SetBitNumber(uint16_t uiData)
{

}

//-------------------------------------------------------------------------------
/* Builds a TCP request header */
uint16_t CModbusRtuSlaveLinkLayer::RequestBasis(uint8_t uiSlave,
        uint8_t uiFunctionCode,
        uint16_t uiAddress,
        uint16_t uiBitNumber,
        uint8_t *puiRequest)
{
//    /* Extract from MODBUS Messaging on TCP/IP Implementation Guide V1.0b
//       (page 23/46):
//       The transaction identifier is used to associate the future response
//       with the request. So, at a time, on a TCP connection, this identifier
//       must be unique. */
//
//    /* Transaction ID */
//    if (m_uiRequestTransactionId < UINT16_MAX)
//    {
//        m_uiRequestTransactionId++;
//    }
//    else
//    {
//        m_uiRequestTransactionId = 0;
//    }
//    puiRequest[0] = (m_uiRequestTransactionId >> 8);
//    puiRequest[1] = (m_uiRequestTransactionId & 0x00ff);
//
//    /* Protocol Modbus */
//    puiRequest[2] = 0;
//    puiRequest[3] = 0;
//
//    /* Length will be defined later by set_puiRequest_length_tcp at offsets 4
//       and 5 */
//
//    puiRequest[6] = uiSlave;
//    puiRequest[7] = uiFunctionCode;
//    puiRequest[8] = (static_cast<uint8_t>(uiAddress >> 8));
//    puiRequest[9] = (static_cast<uint8_t>(uiAddress & 0x00ff));
//    puiRequest[10] = (static_cast<uint8_t>(uiBitNumber >> 8));
//    puiRequest[11] = (static_cast<uint8_t>(uiBitNumber & 0x00ff));
//
//    return _MODBUS_TCP_PRESET_REQ_LENGTH;

    puiRequest[0] = uiSlave;
    puiRequest[1] = uiFunctionCode;
    puiRequest[2] = (static_cast<uint8_t>(uiAddress >> 8));
    puiRequest[3] = (static_cast<uint8_t>(uiAddress & 0x00ff));
    puiRequest[4] = (static_cast<uint8_t>(uiBitNumber >> 8));
    puiRequest[5] = (static_cast<uint8_t>(uiBitNumber & 0x00ff));

    return _MODBUS_RTU_PRESET_REQ_LENGTH;
}

//-------------------------------------------------------------------------------
/* Builds a TCP response header */
uint16_t CModbusRtuSlaveLinkLayer::ResponseBasis(uint8_t uiSlave, uint8_t uiFunctionCode, uint8_t *puiResponse)
{
//    /* Extract from MODBUS Messaging on TCP/IP Implementation
//       Guide V1.0b (page 23/46):
//       The transaction identifier is used to associate the future
//       response with the puiRequestuest. */
//    puiResponse[0] = (m_uiResponseTransactionId >> 8);
//    puiResponse[1] = (m_uiResponseTransactionId & 0x00ff);
//
//    /* Protocol Modbus */
//    puiResponse[2] = 0;
//    puiResponse[3] = 0;
//
//    /* Length will be set later by send_msg (4 and 5) */
//
//    /* The slave ID is copied from the indication */
//    puiResponse[6] = uiSlave;
//    puiResponse[7] = uiFunctionCode;
//
//    return _MODBUS_TCP_PRESET_RSP_LENGTH;

    /* In this case, the slave is certainly valid because a check is already
     * done in _modbus_rtu_listen */
    puiResponse[0] = uiSlave;
    puiResponse[1] = uiFunctionCode;

    return _MODBUS_RTU_PRESET_RSP_LENGTH;
}

//-------------------------------------------------------------------------------
/* Builds a TCP request header */
uint16_t CModbusRtuSlaveLinkLayer::RequestHeader(uint8_t uiSlave)
{
//    /* Extract from MODBUS Messaging on TCP/IP Implementation Guide V1.0b
//       (page 23/46):
//       The transaction identifier is used to associate the future response
//       with the request. So, at a time, on a TCP connection, this identifier
//       must be unique. */
//
//    /* Transaction ID */
//    if (m_uiRequestTransactionId < UINT16_MAX)
//    {
//        m_uiRequestTransactionId++;
//    }
//    else
//    {
//        m_uiRequestTransactionId = 0;
//    }
//    m_auiTxBuffer[0] = (m_uiRequestTransactionId >> 8);
//    m_auiTxBuffer[1] = (m_uiRequestTransactionId & 0x00ff);
//
//    /* Protocol Modbus */
//    m_auiTxBuffer[2] = 0;
//    m_auiTxBuffer[3] = 0;
//
//    /* Length will be defined later by set_puiRequest_length_tcp at offsets 4
//       and 5 */
//
//    m_auiTxBuffer[6] = uiSlave;
//
//    // tcp header length
//    return 7;
}

//-------------------------------------------------------------------------------
/* Builds a TCP response header */
uint16_t CModbusRtuSlaveLinkLayer::ResponseHeader(uint8_t uiSlave)
{
//    /* Extract from MODBUS Messaging on TCP/IP Implementation
//       Guide V1.0b (page 23/46):
//       The transaction identifier is used to associate the future
//       response with the puiRequestuest. */
//    m_auiTxBuffer[0] = (m_uiResponseTransactionId >> 8);
//    m_auiTxBuffer[1] = (m_uiResponseTransactionId & 0x00ff);
//
//    /* Protocol Modbus */
//    m_auiTxBuffer[2] = 0;
//    m_auiTxBuffer[3] = 0;
//
//    /* Length will be set later by send_msg (4 and 5) */
//
//    /* The slave ID is copied from the indication */
//    m_auiTxBuffer[6] = uiSlave;
//
//    // tcp header length
//    return 7;
}

//-------------------------------------------------------------------------------
uint16_t CModbusRtuSlaveLinkLayer::Tail(uint8_t *puiMessage, uint16_t uiLength)
{
//    m_auiTxBuffer[4] = ((m_uiFrameLength - 6) >> 8);
//    m_auiTxBuffer[5] = ((m_uiFrameLength - 6) & 0x00ff);
//    return uiLength;

    uint16_t uiCrc = usCrc16(puiMessage, uiLength);
    puiMessage[uiLength++] = uiCrc & 0x00FF;
    puiMessage[uiLength++] = uiCrc >> 8;

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusRtuSlaveLinkLayer::Send(uint8_t *puiMessage, uint16_t uiLength)
{
    return m_pxCommunicationDevice -> Write(puiMessage, uiLength);
}

//-------------------------------------------------------------------------------
int16_t CModbusRtuSlaveLinkLayer::Receive(uint8_t *puiDestination, uint16_t uiLength)
{
    return m_pxCommunicationDevice -> Read(puiDestination, uiLength);
}

//-------------------------------------------------------------------------------
int8_t CModbusRtuSlaveLinkLayer::FrameCheck(uint8_t *puiSourse, uint16_t uiLength)
{
//    if (uiLength < _MIN_MESSAGE_LENGTH)
//    {
//        return 0;
//    }
//
//    return 1;

    if (uiLength < _MIN_MESSAGE_LENGTH)
    {
        return 0;
    }

    uint16_t uiCrc = ((static_cast<uint16_t>(puiSourse[uiLength - 1]) << 8) |
                      (static_cast<uint16_t>(puiSourse[uiLength - 2])));
    uint16_t uiCrcTemp = usCrc16(puiSourse,
                                 (uiLength - _MODBUS_RTU_CHECKSUM_LENGTH));
    if (usCrc16(puiSourse,
                (uiLength - _MODBUS_RTU_CHECKSUM_LENGTH)) == uiCrc)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//-------------------------------------------------------------------------------
uint8_t CModbusRtuSlaveLinkLayer::Fsm(void)
{
    std::cout << "CModbusRtuSlaveLinkLayer::Fsm 1"  << std::endl;
    switch (GetFsmState())
    {
        int16_t iBytesNumber;

    case START:
        std::cout << "CModbusRtuSlaveLinkLayer::Fsm START"  << std::endl;
        SetFsmState(COMMUNICATION_START);
        break;

    case READY:
        std::cout << "CModbusRtuSlaveLinkLayer::Fsm READY"  << std::endl;
        break;

    case IDDLE:
        std::cout << "CModbusRtuSlaveLinkLayer::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
        std::cout << "CModbusRtuSlaveLinkLayer::Fsm STOP"  << std::endl;
        SetFsmState(START);
        break;

    case COMMUNICATION_START:
        std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_START"  << std::endl;
        m_pxCommunicationDevice -> Open();
        m_uiFrameLength = 0;
        SetFsmState(COMMUNICATION_RECEIVE_START);
        break;

    case COMMUNICATION_RECEIVE_START:
        std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START"  << std::endl;
        m_uiFrameLength = 0;
        iBytesNumber =
            m_pxCommunicationDevice ->
            ReceiveStart((m_auiRxBuffer + m_uiFrameLength),
                         (MODBUS_RTU_MAX_ADU_LENGTH - m_uiFrameLength),
                         1000000);
        if (iBytesNumber > 0)
        {
            m_uiFrameLength = m_uiFrameLength + iBytesNumber;
            SetFsmState(COMMUNICATION_RECEIVE_END);
        }
        else if (iBytesNumber < 0)
        {
            cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START errno " << errno << endl;
            SetFsmState(COMMUNICATION_RECEIVE_ERROR);
        }
        break;

    case COMMUNICATION_RECEIVE_CONTINUE:
        std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_CONTINUE"  << std::endl;
        m_uiFrameLength = 0;
        iBytesNumber =
            m_pxCommunicationDevice ->
            ReceiveContinue((m_auiRxBuffer + m_uiFrameLength),
                            (MODBUS_RTU_MAX_ADU_LENGTH - m_uiFrameLength),
                            1000000);
//        if (iBytesNumber > 0)
//        {
//            m_uiFrameLength = m_uiFrameLength + iBytesNumber;
//        }
//        else if (iBytesNumber < 0)
//        {
//            SetFsmState(COMMUNICATION_RECEIVE_ERROR);
//        }
//        else
//        {
//            SetFsmState(COMMUNICATION_FRAME_RECEIVED);
//
//            cout << "ReceiveContinue" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)m_auiRxBuffer;
//            for(int i=0; i<32; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
//        }
        if (iBytesNumber > 0)
        {
            m_uiFrameLength = m_uiFrameLength + iBytesNumber;
            SetFsmState(COMMUNICATION_RECEIVE_END);
        }
        else if (iBytesNumber < 0)
        {
            cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_CONTINUE errno " << errno << endl;
            SetFsmState(COMMUNICATION_RECEIVE_ERROR);
        }
        break;

    case COMMUNICATION_RECEIVE_END:
        std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END"  << std::endl;
        iBytesNumber =
            m_pxCommunicationDevice ->
            ReceiveContinue((m_auiRxBuffer + m_uiFrameLength),
                            (MODBUS_RTU_MAX_ADU_LENGTH - m_uiFrameLength),
                            10000);
        if (iBytesNumber > 0)
        {
            m_uiFrameLength = m_uiFrameLength + iBytesNumber;
        }
        else if (iBytesNumber < 0)
        {
            SetFsmState(COMMUNICATION_RECEIVE_ERROR);
        }
        else
        {
//            m_uiResponseTransactionId = ((static_cast<uint16_t>(m_auiRxBuffer[0]) << 8) |
//                                         (static_cast<uint16_t>(m_auiRxBuffer[1])));
            SetFsmState(COMMUNICATION_FRAME_RECEIVED);

            cout << "ReceiveContinue" << endl;
            unsigned char *pucSourceTemp;
            pucSourceTemp = (unsigned char*)m_auiRxBuffer;
            for(int i=0; i<32; )
            {
                for(int j=0; j<8; j++)
                {
                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
                }
                cout << endl;
                i += 8;
            }
        }
        break;

    case COMMUNICATION_FRAME_RECEIVED:
        std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_FRAME_RECEIVED"  << std::endl;

//        cout << "ReceiveContinue" << endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)m_auiRxBuffer;
//        for(int i=0; i<32; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
        break;

    case COMMUNICATION_TRANSMIT_START:
        std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_TRANSMIT_START"  << std::endl;

        {
            cout << "m_auiTxBuffer" << endl;
            unsigned char *pucSourceTemp;
            pucSourceTemp = (unsigned char*)m_auiTxBuffer;
            for(int i=0; i<32; )
            {
                for(int j=0; j<8; j++)
                {
                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
                }
                cout << endl;
                i += 8;
            }
        }
        std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_TRANSMIT_START m_uiFrameLength "  << (int)m_uiFrameLength << std::endl;
        m_pxCommunicationDevice -> Write(m_auiTxBuffer, m_uiFrameLength);
        SetFsmState(COMMUNICATION_FRAME_TRANSMITED);
        break;

    case COMMUNICATION_FRAME_TRANSMITED:
        std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_FRAME_TRANSMITED"  << std::endl;
        {
            cout << "m_auiTxBuffer" << endl;
            unsigned char *pucSourceTemp;
            pucSourceTemp = (unsigned char*)m_auiTxBuffer;
            for(int i=0; i<32; )
            {
                for(int j=0; j<8; j++)
                {
                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
                }
                cout << endl;
                i += 8;
            }
        }
        break;

    case COMMUNICATION_RECEIVE_ERROR:
        std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_ERROR"  << std::endl;
//        m_pxCommunicationDevice -> Close();
//        SetFsmState(COMMUNICATION_START);
        SetFsmState(COMMUNICATION_RECEIVE_CONTINUE);
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------
