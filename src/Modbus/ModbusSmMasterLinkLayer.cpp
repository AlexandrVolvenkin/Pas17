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
#include "ModbusSmMasterLinkLayer.h"

using namespace std;

//-------------------------------------------------------------------------------
CModbusSmMasterLinkLayer::CModbusSmMasterLinkLayer()
{
    std::cout << "CModbusSmMasterLinkLayer constructor"  << std::endl;
    m_pxCommunicationDevice = 0;
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CModbusSmMasterLinkLayer::~CModbusSmMasterLinkLayer()
{
    m_pxCommunicationDevice -> Close();
    delete m_pxCommunicationDevice;
}

//-------------------------------------------------------------------------------
uint8_t CModbusSmMasterLinkLayer::Init(void)
{
    std::cout << "CModbusSmMasterLinkLayer Init"  << std::endl;
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));
}

//-------------------------------------------------------------------------------
void CModbusSmMasterLinkLayer::Process(CModbusSmMasterLinkLayer* pxModbusSlaveLinkLayer)
{
    cout << "CModbusSmMasterLinkLayer::Process 1" << endl;

    while (1)
    {
        pxModbusSlaveLinkLayer -> Fsm();
    }
}

//////-------------------------------------------------------------------------------
//void CModbusSmMasterLinkLayer::CommunicationDeviceInit(const char* pccDeviceName,
//        uint32_t uiBaudRate,
//        uint8_t uiDataBits,
//        char cParity,
//        uint8_t uiStopBit)
//{
//    cout << "CModbusSmMasterLinkLayer::CommunicationDeviceInit 1" << endl;
//    m_pxCommunicationDevice -> Init();
//    m_pxCommunicationDevice -> SetDeviceName(pccDeviceName);
//    m_pxCommunicationDevice -> SetBaudRate(uiBaudRate);
//    m_pxCommunicationDevice -> SetDataBits(uiDataBits);
//    m_pxCommunicationDevice -> SetParity(cParity);
//    m_pxCommunicationDevice -> SetStopBit(uiStopBit);
//
//    m_uiGuardTimeout = 10;//((((1000000UL / uiBaudRate) * 8UL * 4UL) / 1000UL) + 1);
//}

//-------------------------------------------------------------------------------
bool CModbusSmMasterLinkLayer::SetTaskData(CDataContainerDataBase* pxDataContainer)
{
//    std::cout << "CModbusSmMasterLinkLayer::SetTaskData 1" << std::endl;

    if (IsTaskReady())
    {
        std::cout << "CModbusSmMasterLinkLayer::SetTaskData 2" << std::endl;
        *m_pxOperatingDataContainer = *pxDataContainer;
        SetFsmState(m_pxOperatingDataContainer -> m_uiFsmCommandState);
        return true;
    }
    else
    {
        std::cout << "CModbusSmMasterLinkLayer::SetTaskData 3" << std::endl;
        return false;
    }
}

//-------------------------------------------------------------------------------
bool CModbusSmMasterLinkLayer::GetTaskData(CDataContainerDataBase* pxDataContainer)
{
//    std::cout << "CModbusSmMasterLinkLayer::GetTaskData 1" << std::endl;

    m_pxOperatingDataContainer -> m_uiFsmCommandState = GetFsmState();
    *pxDataContainer = *m_pxOperatingDataContainer;

    return true;
}

//-------------------------------------------------------------------------------
void CModbusSmMasterLinkLayer::ReceiveEnable(void)
{
//    m_pxCommunicationDevice -> Open();
}

//-------------------------------------------------------------------------------
void CModbusSmMasterLinkLayer::ReceiveDisable(void)
{
//    m_pxCommunicationDevice -> Close();
}

//-------------------------------------------------------------------------------
void CModbusSmMasterLinkLayer::TransmitEnable(void)
{
//    m_pxCommunicationDevice -> Open();
}

//-------------------------------------------------------------------------------
void CModbusSmMasterLinkLayer::TransmitDisable(void)
{
//    m_pxCommunicationDevice -> Close();
}

//-------------------------------------------------------------------------------
void CModbusSmMasterLinkLayer::CommunicationStart(void)
{
    SetFsmCommandState(COMMUNICATION_START);
}

//-------------------------------------------------------------------------------
void CModbusSmMasterLinkLayer::CommunicationReceiveStart(void)
{
    SetFsmCommandState(COMMUNICATION_RECEIVE_START);
}

//-------------------------------------------------------------------------------
void CModbusSmMasterLinkLayer::ReceiveStart(void)
{
    SetFsmCommandState(COMMUNICATION_RECEIVE_START);
}

//-------------------------------------------------------------------------------
void CModbusSmMasterLinkLayer::TransmitStart(void)
{
    cout << "CModbusSmMasterLinkLayer::TransmitStart 1" << endl;
    SetFsmCommandState(COMMUNICATION_TRANSMIT_START);
}

//-------------------------------------------------------------------------------
uint8_t* CModbusSmMasterLinkLayer::GetRxBuffer(void)
{
    return &m_auiRxBuffer[0];
}

//-------------------------------------------------------------------------------
uint8_t* CModbusSmMasterLinkLayer::GetTxBuffer(void)
{
    return &m_auiTxBuffer[0];
}

//-------------------------------------------------------------------------------
uint8_t* CModbusSmMasterLinkLayer::GetRxPdu(void)
{
    return &m_auiRxBuffer[1];
}

//-------------------------------------------------------------------------------
uint8_t* CModbusSmMasterLinkLayer::GetTxPdu(void)
{
    return &m_auiTxBuffer[1];
}

//-------------------------------------------------------------------------------
uint8_t CModbusSmMasterLinkLayer::GetPduOffset(void)
{
    return 1;
}

//-------------------------------------------------------------------------------
uint8_t CModbusSmMasterLinkLayer::GetSlaveAddress(void)
{
    return m_auiRxBuffer[0];
}

//-------------------------------------------------------------------------------
void CModbusSmMasterLinkLayer::SetSlaveAddress(uint8_t uiData)
{

}

//-------------------------------------------------------------------------------
uint8_t CModbusSmMasterLinkLayer::GetFunctionCode(void)
{
    return m_auiRxBuffer[1];
}

//-------------------------------------------------------------------------------
void CModbusSmMasterLinkLayer::SetFunctionCode(uint8_t uiData)
{

}

//-------------------------------------------------------------------------------
uint16_t CModbusSmMasterLinkLayer::GetDataAddress(void)
{
    return ((static_cast<uint16_t>(m_auiRxBuffer[2]) << 8) |
            (static_cast<uint16_t>(m_auiRxBuffer[3])));
}

//-------------------------------------------------------------------------------
void CModbusSmMasterLinkLayer::SetDataAddress(uint16_t uiData)
{

}

//-------------------------------------------------------------------------------
uint16_t CModbusSmMasterLinkLayer::GetBitNumber(void)
{
    return ((static_cast<uint16_t>(m_auiRxBuffer[4]) << 8) |
            (static_cast<uint16_t>(m_auiRxBuffer[5])));
}

//-------------------------------------------------------------------------------
void CModbusSmMasterLinkLayer::SetBitNumber(uint16_t uiData)
{

}

//-------------------------------------------------------------------------------
/* Builds a RTU request header */
uint16_t CModbusSmMasterLinkLayer::RequestBasis(uint8_t uiSlave,
        uint8_t uiFunctionCode,
        uint16_t uiAddress,
        uint16_t uiBitNumber,
        uint8_t *puiRequest)
{
    puiRequest[0] = uiSlave;
    puiRequest[1] = uiFunctionCode;
    puiRequest[2] = (static_cast<uint8_t>(uiAddress >> 8));
    puiRequest[3] = (static_cast<uint8_t>(uiAddress & 0x00ff));
    puiRequest[4] = (static_cast<uint8_t>(uiBitNumber >> 8));
    puiRequest[5] = (static_cast<uint8_t>(uiBitNumber & 0x00ff));

    return _MODBUS_SM_PRESET_REQ_LENGTH;
}

//-------------------------------------------------------------------------------
/* Builds a RTU response header */
uint16_t CModbusSmMasterLinkLayer::ResponseBasis(uint8_t uiSlave, uint8_t uiFunctionCode, uint8_t *puiResponse)
{
    /* In this case, the slave is certainly valid because a check is already
     * done in _modbus_rtu_listen */
    puiResponse[0] = uiSlave;
    puiResponse[1] = uiFunctionCode;

    return _MODBUS_SM_PRESET_RSP_LENGTH;
}

//-------------------------------------------------------------------------------
/* Build the exception response */
uint16_t CModbusSmMasterLinkLayer::ResponseException(uint8_t uiSlave, uint8_t uiFunctionCode, uint8_t uiExceptionCode, uint8_t *puiResponse)
{
    uint16_t uiLength;

    uiLength = ResponseBasis(uiSlave, (uiFunctionCode | 0x80), puiResponse);
    /* Positive exception code */
    puiResponse[uiLength++] = uiExceptionCode;

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmMasterLinkLayer::Tail(uint8_t *puiMessage, uint16_t uiLength)
{
    uint16_t uiCrc = usCrc16(puiMessage, uiLength);
    puiMessage[uiLength++] = uiCrc & 0x00FF;
    puiMessage[uiLength++] = uiCrc >> 8;

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmMasterLinkLayer::Send(uint8_t *puiMessage, uint16_t uiLength)
{
    return m_pxCommunicationDevice -> Write(puiMessage, uiLength);
}

//-------------------------------------------------------------------------------
int16_t CModbusSmMasterLinkLayer::Receive(uint8_t *puiDestination, uint16_t uiLength)
{
    return m_pxCommunicationDevice -> Read(puiDestination, uiLength);
}

//-------------------------------------------------------------------------------
int8_t CModbusSmMasterLinkLayer::FrameCheck(uint8_t *puiSourse, uint16_t uiLength)
{
    if (uiLength < _MIN_MESSAGE_LENGTH)
    {
        return 0;
    }

    uint16_t uiCrc = ((static_cast<uint16_t>(puiSourse[uiLength - 1]) << 8) |
                      (static_cast<uint16_t>(puiSourse[uiLength - 2])));
    uint16_t uiCrcTemp = usCrc16(puiSourse,
                                 (uiLength - _MODBUS_SM_CHECKSUM_LENGTH));
    if (usCrc16(puiSourse,
                (uiLength - _MODBUS_SM_CHECKSUM_LENGTH)) == uiCrc)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//-------------------------------------------------------------------------------
uint8_t CModbusSmMasterLinkLayer::Fsm(void)
{
//    std::cout << "CModbusSmMasterLinkLayer::Fsm 1"  << std::endl;
    switch (GetFsmState())
    {
        int16_t iBytesNumber;

    case IDDLE:
//        std::cout << "CModbusSmMasterLinkLayer::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CModbusSmMasterLinkLayer::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CModbusSmMasterLinkLayer::Fsm START"  << std::endl;
        std::cout << "CModbusSmMasterLinkLayer::Fsm m_sCommunicationDeviceName" << " " << (m_sCommunicationDeviceName) << std::endl;
        Init();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
//        std::cout << "CModbusSmMasterLinkLayer::Fsm INIT 1"  << std::endl;
    {
        CTaskInterface* pxTask =
            GetResources() ->
            GetTaskPointerByNameFromMap(m_sCommunicationDeviceName);

        if (pxTask != 0)
        {
//                std::cout << "CModbusSmMasterLinkLayer::Fsm INIT 2"  << std::endl;
            if (pxTask -> GetFsmState() >= READY)
            {
//                    std::cout << "CModbusSmMasterLinkLayer::Fsm INIT 3"  << std::endl;
                SetCommunicationDevice((CCommunicationDeviceInterface*)pxTask);
                SetFsmState(READY);
            }
        }
        else
        {
//                std::cout << "CModbusSmMasterLinkLayer::Fsm INIT 4"  << std::endl;
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CModbusSmMasterLinkLayer::Fsm INIT 5"  << std::endl;
                SetFsmState(STOP);
            }
        }
    }
    break;

    case READY:
//        std::cout << "CModbusSmMasterLinkLayer::Fsm READY"  << std::endl;
        break;

    case DONE_OK:
//        std::cout << "CModbusSmMasterLinkLayer::Fsm DONE_OK"  << std::endl;
        SetFsmOperationStatus(DONE_OK);
        break;

    case DONE_ERROR:
//        std::cout << "CModbusSmMasterLinkLayer::Fsm DONE_ERROR"  << std::endl;
        SetFsmOperationStatus(DONE_ERROR);
        break;

    case COMMUNICATION_START:
        std::cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_START"  << std::endl;
        m_pxCommunicationDevice -> Open();
        m_uiFrameLength = 0;
        SetFsmState(READY);
        break;

    case COMMUNICATION_RECEIVE_START:
        std::cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_RECEIVE_START"  << std::endl;
        m_uiFrameLength = 0;
        GetTimerPointer() -> Set(m_uiConfirmationTimeout);
        SetFsmState(COMMUNICATION_RECEIVE);
        break;

    case COMMUNICATION_RECEIVE:
//        std::cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_RECEIVE"  << std::endl;
        iBytesNumber =
            m_pxCommunicationDevice ->
            ReceiveStart((m_auiRxBuffer + m_uiFrameLength),
                         (MODBUS_SM_MAX_ADU_LENGTH - m_uiFrameLength),
                         m_uiReceiveTimeout);
        if (iBytesNumber > 0)
        {
            std::cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_RECEIVE 2"  << std::endl;
            m_uiFrameLength = m_uiFrameLength + iBytesNumber;
//            {
//                cout << "CModbusSmMasterLinkLayer::Fsm m_auiRxBuffer" << endl;
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
            SetFsmState(COMMUNICATION_FRAME_CHECK);
        }
        else if (iBytesNumber < 0)
        {
            std::cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_RECEIVE 3"  << std::endl;
            cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_RECEIVE errno " << errno << endl;
            SetFsmState(COMMUNICATION_RECEIVE_ERROR);
        }
        else
        {
//            std::cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_RECEIVE 4"  << std::endl;
//            cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_RECEIVE errno " << errno << endl;
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_RECEIVE 5"  << std::endl;
                cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_RECEIVE errno " << errno << endl;
                SetFsmState(DONE_ERROR);
            }
        }
        break;

    case COMMUNICATION_FRAME_CHECK:
        std::cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_FRAME_CHECK 1"  << std::endl;
        if (FrameCheck(m_auiRxBuffer, m_uiFrameLength))
        {
            std::cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_FRAME_CHECK 2"  << std::endl;
            SetFsmState(COMMUNICATION_FRAME_RECEIVED);
        }
        else
        {
            std::cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_FRAME_CHECK 3"  << std::endl;
            SetFsmState(COMMUNICATION_RECEIVE_ERROR);
        }
        break;

    case COMMUNICATION_FRAME_RECEIVED:
        std::cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_FRAME_RECEIVED"  << std::endl;

        SetFsmState(DONE_OK);
        break;

    case COMMUNICATION_TRANSMIT_START:
        std::cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_TRANSMIT_START"  << std::endl;

//        {
//            cout << "CModbusSmMasterLinkLayer::Fsm m_auiTxBuffer" << endl;
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
//        }
        std::cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_TRANSMIT_START m_uiFrameLength "  << (int)m_uiFrameLength << std::endl;
        m_pxCommunicationDevice -> Write(m_auiTxBuffer, m_uiFrameLength);
        SetFsmState(COMMUNICATION_FRAME_TRANSMITED);
        break;

    case COMMUNICATION_FRAME_TRANSMITED:
        std::cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_FRAME_TRANSMITED"  << std::endl;
        SetFsmState(DONE_OK);
        {
            cout << "CModbusSmMasterLinkLayer::Fsm m_auiTxBuffer" << endl;
            uint8_t *pucSourceTemp;
            pucSourceTemp = (uint8_t*)m_auiTxBuffer;
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
        std::cout << "CModbusSmMasterLinkLayer::Fsm COMMUNICATION_RECEIVE_ERROR"  << std::endl;
        SetFsmState(DONE_ERROR);
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------

