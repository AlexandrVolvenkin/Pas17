//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <iostream>
#include <string.h>
#include <typeinfo>


#include "Platform.h"
#include "Task.h"
#include "Resources.h"
#include "CommunicationDevice.h"
#include <Crc.h>
#include "DataContainer.h"
#include "ModbusSlaveLinkLayer.h"
#include "ModbusTcpSlaveLinkLayer.h"

using namespace std;

//-------------------------------------------------------------------------------
CModbusTcpSlaveLinkLayer::CModbusTcpSlaveLinkLayer()
{
    //std::cout << "CModbusTcpSlaveLinkLayer constructor"  << std::endl;
    m_pxCommunicationDevice = 0;
    SetFsmState(START);

    m_pxThread = new std::thread(CModbusTcpSlaveLinkLayer::Process, this);
    std::thread::id th_id = m_pxThread -> get_id();
    //std::cout << "CModbusTcpSlaveLinkLayer th_id" << " " << th_id << std::endl;
    // не ждем завершения работы функции
    m_pxThread -> detach();
}

//-------------------------------------------------------------------------------
CModbusTcpSlaveLinkLayer::~CModbusTcpSlaveLinkLayer()
{
    delete m_pxCommunicationDevice;

    if (m_pxThread -> joinable())
    {
        m_pxThread -> join();
    }
    delete m_pxThread;
}

//-------------------------------------------------------------------------------
uint8_t CModbusTcpSlaveLinkLayer::Init(void)
{
    //std::cout << "CModbusTcpSlaveLinkLayer Init"  << std::endl;
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));
}

//-------------------------------------------------------------------------------
bool CModbusTcpSlaveLinkLayer::SetTaskData(CDataContainerDataBase* pxDataContainer)
{
//    //std::cout << "CModbusTcpSlaveLinkLayer::SetTaskData 1" << std::endl;

    if (IsTaskReady())
    {
        //std::cout << "CModbusTcpSlaveLinkLayer::SetTaskData 2" << std::endl;
        *m_pxOperatingDataContainer = *pxDataContainer;
        SetFsmState(m_pxOperatingDataContainer -> m_uiFsmCommandState);
        return true;
    }
    else
    {
        //std::cout << "CModbusTcpSlaveLinkLayer::SetTaskData 3" << std::endl;
        return false;
    }
}

//-------------------------------------------------------------------------------
bool CModbusTcpSlaveLinkLayer::GetTaskData(CDataContainerDataBase* pxDataContainer)
{
//    //std::cout << "CModbusTcpSlaveLinkLayer::GetTaskData 1" << std::endl;

    m_pxOperatingDataContainer -> m_uiFsmCommandState = GetFsmState();
    *pxDataContainer = *m_pxOperatingDataContainer;

    return true;
}

//-------------------------------------------------------------------------------
size_t CModbusTcpSlaveLinkLayer::GetObjectLength(void)
{
    //std::cout << "CModbusTcpSlaveLinkLayer GetObjectLength"  << std::endl;
    return sizeof(*this);
}

//-------------------------------------------------------------------------------
void CModbusTcpSlaveLinkLayer::Process(CModbusTcpSlaveLinkLayer* pxModbusSlaveLinkLayer)
{
    cout << "CModbusTcpSlaveLinkLayer::Process 1" << endl;

    while (1)
    {
        usleep(500);
        pxModbusSlaveLinkLayer -> Fsm();
    }
}

//-------------------------------------------------------------------------------
void CModbusTcpSlaveLinkLayer::CommunicationDeviceInit(const char* pccIpAddress,
        uint16_t uiPort)
{
    m_pxCommunicationDevice -> Init();
    m_pxCommunicationDevice -> SetIpAddress(pccIpAddress);
    m_pxCommunicationDevice -> SetPort(uiPort);

    m_uiGuardTimeout = 10;
}

//-------------------------------------------------------------------------------
void CModbusTcpSlaveLinkLayer::ReceiveEnable(void)
{
//    m_pxCommunicationDevice -> Open();
}

//-------------------------------------------------------------------------------
void CModbusTcpSlaveLinkLayer::ReceiveDisable(void)
{
//    m_pxCommunicationDevice -> Close();
}

//-------------------------------------------------------------------------------
void CModbusTcpSlaveLinkLayer::TransmitEnable(void)
{
//    m_pxCommunicationDevice -> Open();
}

//-------------------------------------------------------------------------------
void CModbusTcpSlaveLinkLayer::TransmitDisable(void)
{
//    m_pxCommunicationDevice -> Close();
}

//-------------------------------------------------------------------------------
void CModbusTcpSlaveLinkLayer::CommunicationStart(void)
{
    SetFsmCommandState(COMMUNICATION_START);
}

//-------------------------------------------------------------------------------
void CModbusTcpSlaveLinkLayer::CommunicationReceiveStart(void)
{
    SetFsmCommandState(COMMUNICATION_RECEIVE_CONTINUE);
}

//-------------------------------------------------------------------------------
void CModbusTcpSlaveLinkLayer::ReceiveStart(void)
{
    SetFsmCommandState(COMMUNICATION_RECEIVE_START);
}

//-------------------------------------------------------------------------------
void CModbusTcpSlaveLinkLayer::TransmitStart(void)
{
    SetFsmCommandState(COMMUNICATION_TRANSMIT_START);
}

//-------------------------------------------------------------------------------
uint8_t* CModbusTcpSlaveLinkLayer::GetRxBuffer(void)
{
    return &m_auiRxBuffer[0];
}

//-------------------------------------------------------------------------------
uint8_t* CModbusTcpSlaveLinkLayer::GetTxBuffer(void)
{
    return &m_auiTxBuffer[0];
}

//-------------------------------------------------------------------------------
uint8_t* CModbusTcpSlaveLinkLayer::GetRxPdu(void)
{
    return &m_auiRxBuffer[7];
}

//-------------------------------------------------------------------------------
uint8_t* CModbusTcpSlaveLinkLayer::GetTxPdu(void)
{
    return &m_auiTxBuffer[7];
}

//-------------------------------------------------------------------------------
uint8_t CModbusTcpSlaveLinkLayer::GetPduOffset(void)
{
    return 7;
}

//-------------------------------------------------------------------------------
uint8_t CModbusTcpSlaveLinkLayer::GetSlaveAddress(void)
{
    return m_auiRxBuffer[6];
}

//-------------------------------------------------------------------------------
void CModbusTcpSlaveLinkLayer::SetSlaveAddress(uint8_t uiData)
{

}

//-------------------------------------------------------------------------------
uint8_t CModbusTcpSlaveLinkLayer::GetFunctionCode(void)
{
    return m_auiRxBuffer[7];
}

//-------------------------------------------------------------------------------
void CModbusTcpSlaveLinkLayer::SetFunctionCode(uint8_t uiData)
{

}

//-------------------------------------------------------------------------------
uint16_t CModbusTcpSlaveLinkLayer::GetDataAddress(void)
{
    return ((static_cast<uint16_t>(m_auiRxBuffer[8]) << 8) |
            (static_cast<uint16_t>(m_auiRxBuffer[9])));
}

//-------------------------------------------------------------------------------
void CModbusTcpSlaveLinkLayer::SetDataAddress(uint16_t uiData)
{

}

//-------------------------------------------------------------------------------
uint16_t CModbusTcpSlaveLinkLayer::GetBitNumber(void)
{
    return ((static_cast<uint16_t>(m_auiRxBuffer[10]) << 8) |
            (static_cast<uint16_t>(m_auiRxBuffer[11])));
}

//-------------------------------------------------------------------------------
void CModbusTcpSlaveLinkLayer::SetBitNumber(uint16_t uiData)
{

}

//-------------------------------------------------------------------------------
/* Builds a TCP request header */
uint16_t CModbusTcpSlaveLinkLayer::RequestBasis(uint8_t uiSlave,
        uint8_t uiFunctionCode,
        uint16_t uiAddress,
        uint16_t uiBitNumber,
        uint8_t *puiRequest)
{
    /* Extract from MODBUS Messaging on TCP/IP Implementation Guide V1.0b
       (page 23/46):
       The transaction identifier is used to associate the future response
       with the request. So, at a time, on a TCP connection, this identifier
       must be unique. */

    /* Transaction ID */
    if (m_uiRequestTransactionId < UINT16_MAX)
    {
        m_uiRequestTransactionId++;
    }
    else
    {
        m_uiRequestTransactionId = 0;
    }
    puiRequest[0] = (m_uiRequestTransactionId >> 8);
    puiRequest[1] = (m_uiRequestTransactionId & 0x00ff);

    /* Protocol Modbus */
    puiRequest[2] = 0;
    puiRequest[3] = 0;

    /* Length will be defined later by set_puiRequest_length_tcp at offsets 4
       and 5 */

    puiRequest[6] = uiSlave;
    puiRequest[7] = uiFunctionCode;
    puiRequest[8] = (static_cast<uint8_t>(uiAddress >> 8));
    puiRequest[9] = (static_cast<uint8_t>(uiAddress & 0x00ff));
    puiRequest[10] = (static_cast<uint8_t>(uiBitNumber >> 8));
    puiRequest[11] = (static_cast<uint8_t>(uiBitNumber & 0x00ff));

    return _MODBUS_TCP_PRESET_REQ_LENGTH;
}

//-------------------------------------------------------------------------------
/* Builds a TCP response header */
uint16_t CModbusTcpSlaveLinkLayer::ResponseBasis(uint8_t uiSlave, uint8_t uiFunctionCode, uint8_t *puiResponse)
{
    /* Extract from MODBUS Messaging on TCP/IP Implementation
       Guide V1.0b (page 23/46):
       The transaction identifier is used to associate the future
       response with the puiRequestuest. */
    puiResponse[0] = (m_uiResponseTransactionId >> 8);
    puiResponse[1] = (m_uiResponseTransactionId & 0x00ff);

    /* Protocol Modbus */
    puiResponse[2] = 0;
    puiResponse[3] = 0;

    /* Length will be set later by send_msg (4 and 5) */

    /* The slave ID is copied from the indication */
    puiResponse[6] = uiSlave;
    puiResponse[7] = uiFunctionCode;

    return _MODBUS_TCP_PRESET_RSP_LENGTH;
}

//-------------------------------------------------------------------------------
/* Build the exception response */
uint16_t CModbusTcpSlaveLinkLayer::ResponseException(uint8_t uiSlave, uint8_t uiFunctionCode, uint8_t uiExceptionCode, uint8_t *puiResponse)
{
    uint16_t uiLength;

    uiLength = ResponseBasis(uiSlave, (uiFunctionCode | 0x80), puiResponse);
    /* Positive exception code */
    puiResponse[uiLength++] = uiExceptionCode;

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusTcpSlaveLinkLayer::Tail(uint8_t *puiMessage, uint16_t uiLength)
{
    m_auiTxBuffer[4] = ((m_uiFrameLength - 6) >> 8);
    m_auiTxBuffer[5] = ((m_uiFrameLength - 6) & 0x00ff);
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusTcpSlaveLinkLayer::Send(uint8_t *puiMessage, uint16_t uiLength)
{
    return m_pxCommunicationDevice -> Write(puiMessage, uiLength);
}

//-------------------------------------------------------------------------------
int16_t CModbusTcpSlaveLinkLayer::Receive(uint8_t *puiDestination, uint16_t uiLength)
{
    return m_pxCommunicationDevice -> Read(puiDestination, uiLength);
}

//-------------------------------------------------------------------------------
int8_t CModbusTcpSlaveLinkLayer::FrameCheck(uint8_t *puiSourse, uint16_t uiLength)
{
    if (uiLength < _MIN_MESSAGE_LENGTH)
    {
        return 0;
    }
    else
    {
        return 1;
    }

}

//-------------------------------------------------------------------------------
uint8_t CModbusTcpSlaveLinkLayer::Fsm(void)
{
//    //std::cout << "CModbusTcpSlaveLinkLayer::Fsm 1"  << std::endl;
    switch (GetFsmState())
    {
        int16_t iBytesNumber;

    case IDDLE:
//        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        ////std::cout << "CModbusTcpSlaveLinkLayer::Fsm STOP"  << std::endl;
        break;

    case START:
        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm START"  << std::endl;
        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm m_sCommunicationDeviceName" << " " << (m_sCommunicationDeviceName) << std::endl;
        Init();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
//        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm INIT 1"  << std::endl;
    {
        CTaskInterface* pxTask =
            GetResources() ->
            GetTaskPointerByNameFromMap(m_sCommunicationDeviceName);

        if (pxTask != 0)
        {
//                //std::cout << "CModbusTcpSlaveLinkLayer::Fsm INIT 2"  << std::endl;
            if (pxTask -> GetFsmState() >= READY)
            {
//                    //std::cout << "CModbusTcpSlaveLinkLayer::Fsm INIT 3"  << std::endl;
                SetCommunicationDevice((CCommunicationDeviceInterface*)pxTask);
                SetFsmState(READY);
            }
        }
        else
        {
//                //std::cout << "CModbusTcpSlaveLinkLayer::Fsm INIT 4"  << std::endl;
            if (GetTimerPointer() -> IsOverflow())
            {
                //std::cout << "CModbusTcpSlaveLinkLayer::Fsm INIT 5"  << std::endl;
                SetFsmState(STOP);
            }
        }
    }
    break;

    case READY:
//        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm READY"  << std::endl;
        break;

    case DONE_OK:
//        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm DONE_OK"  << std::endl;
        break;

    case DONE_ERROR:
//        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm DONE_ERROR"  << std::endl;
        break;

    case COMMUNICATION_START:
        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_START"  << std::endl;
        m_pxCommunicationDevice -> Open();
        m_uiFrameLength = 0;
        SetFsmState(COMMUNICATION_RECEIVE_START);
        break;

    case COMMUNICATION_RECEIVE_START:
        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START"  << std::endl;
        m_uiFrameLength = 0;
        iBytesNumber =
            m_pxCommunicationDevice ->
            ReceiveStart((m_auiRxBuffer + m_uiFrameLength),
                         (MODBUS_TCP_MAX_ADU_LENGTH - m_uiFrameLength),
                         m_uiReceiveTimeout);
        if (iBytesNumber > 0)
        {
            //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START 2"  << std::endl;
            m_uiFrameLength = m_uiFrameLength + iBytesNumber;
            m_uiResponseTransactionId = ((static_cast<uint16_t>(m_auiRxBuffer[0]) << 8) |
                                         (static_cast<uint16_t>(m_auiRxBuffer[1])));
            SetFsmState(COMMUNICATION_FRAME_CHECK);
        }
        else if (iBytesNumber < 0)
        {
//            //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START 3"  << std::endl;
//            cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START errno " << errno << endl;
            SetFsmState(COMMUNICATION_RECEIVE_ERROR);
        }
        else
        {
            //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START 4"  << std::endl;
//            cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START errno " << errno << endl;
        }
        break;

    case COMMUNICATION_RECEIVE_CONTINUE:
        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_CONTINUE"  << std::endl;
        m_uiFrameLength = 0;
        iBytesNumber =
            m_pxCommunicationDevice ->
            ReceiveContinue((m_auiRxBuffer + m_uiFrameLength),
                            (MODBUS_TCP_MAX_ADU_LENGTH - m_uiFrameLength),
                            m_uiReceiveTimeout);
        if (iBytesNumber > 0)
        {
            //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_CONTINUE 2"  << std::endl;
            m_uiFrameLength = m_uiFrameLength + iBytesNumber;
            m_uiResponseTransactionId = ((static_cast<uint16_t>(m_auiRxBuffer[0]) << 8) |
                                         (static_cast<uint16_t>(m_auiRxBuffer[1])));
            SetFsmState(COMMUNICATION_FRAME_CHECK);
        }
        else if (iBytesNumber < 0)
        {
            //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_CONTINUE 3"  << std::endl;
            cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_CONTINUE errno " << errno << endl;
            SetFsmState(COMMUNICATION_RECEIVE_ERROR);
        }
        else
        {
            //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_CONTINUE 4"  << std::endl;
            cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_CONTINUE errno " << errno << endl;
        }
        break;

    case COMMUNICATION_RECEIVE_END:
        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END"  << std::endl;
        iBytesNumber =
            m_pxCommunicationDevice ->
            ReceiveContinue((m_auiRxBuffer + m_uiFrameLength),
                            (MODBUS_TCP_MAX_ADU_LENGTH - m_uiFrameLength),
                            m_uiGuardTimeout);
        if (iBytesNumber > 0)
        {
            //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END 2"  << std::endl;
            m_uiFrameLength = m_uiFrameLength + iBytesNumber;
        }
        else if (iBytesNumber < 0)
        {
            //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END 3"  << std::endl;
            SetFsmState(COMMUNICATION_RECEIVE_ERROR);
        }
        else
        {
            //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END 4"  << std::endl;
            m_uiResponseTransactionId = ((static_cast<uint16_t>(m_auiRxBuffer[0]) << 8) |
                                         (static_cast<uint16_t>(m_auiRxBuffer[1])));
//            SetFsmState(COMMUNICATION_FRAME_CHECK);
            if (FrameCheck(m_auiRxBuffer, m_uiFrameLength))
            {
                //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_FRAME_CHECK 2"  << std::endl;
//            SetFsmState(COMMUNICATION_FRAME_RECEIVED);
                SetFsmState(DONE_OK);
            }
            else
            {
                //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_FRAME_CHECK 3"  << std::endl;
                SetFsmState(COMMUNICATION_RECEIVE_CONTINUE);
            }
        }
        break;

    case COMMUNICATION_FRAME_CHECK:
        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_FRAME_CHECK 1"  << std::endl;

//            {
//                cout << "CModbusTcpSlaveLinkLayer::Fsm m_auiRxBuffer" << endl;
//                uint8_t *pucSourceTemp;
//                pucSourceTemp = (uint8_t*)m_auiRxBuffer;
//                for(int i=0; i<32; )
//                {
//                    for(int j=0; j<8; j++)
//                    {
//                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                    }
//                    cout << endl;
//                    i += 8;
//                }
//            }

        if (FrameCheck(m_auiRxBuffer, m_uiFrameLength))
        {
            //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_FRAME_CHECK 2"  << std::endl;
            SetFsmState(COMMUNICATION_FRAME_RECEIVED);
        }
        else
        {
            //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_FRAME_CHECK 3"  << std::endl;
            SetFsmState(COMMUNICATION_RECEIVE_CONTINUE);
        }
        break;

    case COMMUNICATION_FRAME_RECEIVED:
        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_FRAME_RECEIVED"  << std::endl;
        SetFsmState(DONE_OK);
        break;

    case COMMUNICATION_TRANSMIT_START:
        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_TRANSMIT_START"  << std::endl;
        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_TRANSMIT_START m_uiFrameLength "  << (int)m_uiFrameLength << std::endl;
        m_pxCommunicationDevice -> Write(m_auiTxBuffer, m_uiFrameLength);
//        SetFsmState(COMMUNICATION_FRAME_TRANSMITED);
        SetFsmState(DONE_OK);
        break;

    case COMMUNICATION_FRAME_TRANSMITED:
        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_FRAME_TRANSMITED"  << std::endl;
        SetFsmState(DONE_OK);
        {
//            cout << "CModbusTcpSlaveLinkLayer::Fsm m_auiTxBuffer" << endl;
//            uint8_t *pucSourceTemp;
//            pucSourceTemp = (uint8_t*)m_auiTxBuffer;
//            for(int i=0; i<32; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
        }
        break;

    case COMMUNICATION_RECEIVE_ERROR:
        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_ERROR"  << std::endl;
        m_pxCommunicationDevice -> Close();
        SetFsmState(DONE_ERROR);
        break;

    case COMMUNICATION_STOP:
        //std::cout << "CModbusTcpSlaveLinkLayer::Fsm COMMUNICATION_STOP"  << std::endl;
        m_pxCommunicationDevice -> Close();
        SetFsmState(READY);
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------
