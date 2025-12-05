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
#include "ModbusRtuSlaveLinkLayer.h"
#include <Crc.h>
#include "DataContainer.h"
#include "ModbusSlaveLinkLayer.h"

using namespace std;

//-------------------------------------------------------------------------------
CModbusRtuSlaveLinkLayer::CModbusRtuSlaveLinkLayer()
{
    //std::cout << "CModbusRtuSlaveLinkLayer constructor"  << std::endl;
    m_pxCommunicationDevice = 0;
    SetFsmState(START);

////    m_pxThread = new std::thread(CModbusRtuSlaveLinkLayer::Process, this);
//    m_pxThread = std::make_shared<std::thread>(CModbusRtuSlaveLinkLayer::Process, this);
////    std::thread::id th_id = m_pxThread -> get_id();
//    //std::cout << "CModbusRtuSlaveLinkLayer th_id" << " " << th_id << std::endl;
//    // не ждем завершения работы функции
//    m_pxThread -> detach();
}

//-------------------------------------------------------------------------------
CModbusRtuSlaveLinkLayer::~CModbusRtuSlaveLinkLayer()
{
    m_pxCommunicationDevice -> Close();
    delete m_pxCommunicationDevice;

    if (m_pxThread -> joinable())
    {
        m_pxThread -> join();
    }
//    delete m_pxThread;
}

//-------------------------------------------------------------------------------
uint8_t CModbusRtuSlaveLinkLayer::Init(void)
{
    //std::cout << "CModbusRtuSlaveLinkLayer Init"  << std::endl;
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));
}

//-------------------------------------------------------------------------------
bool CModbusRtuSlaveLinkLayer::SetTaskData(CDataContainerDataBase* pxDataContainer)
{
//    std::cout << "CModbusRtuSlaveLinkLayer::SetTaskData 1" << std::endl;

//    if (IsTaskReady())
//    {
        //std::cout << "CModbusRtuSlaveLinkLayer::SetTaskData 2" << std::endl;
        *m_pxOperatingDataContainer = *pxDataContainer;
        SetFsmState(m_pxOperatingDataContainer -> m_uiFsmCommandState);
        return true;
//    }
//    else
//    {
//        //std::cout << "CModbusRtuSlaveLinkLayer::SetTaskData 3" << std::endl;
//        return false;
//    }
}

//-------------------------------------------------------------------------------
bool CModbusRtuSlaveLinkLayer::SetTaskDataNoStateCheck(CDataContainerDataBase* pxDataContainer)
{
//    //std::cout << "CModbusRtuSlaveLinkLayer::SetTaskData 1" << std::endl;

    //std::cout << "CModbusRtuSlaveLinkLayer::SetTaskData 2" << std::endl;
    *m_pxOperatingDataContainer = *pxDataContainer;
    SetFsmState(m_pxOperatingDataContainer -> m_uiFsmCommandState);
}

//-------------------------------------------------------------------------------
bool CModbusRtuSlaveLinkLayer::GetTaskData(CDataContainerDataBase* pxDataContainer)
{
//    //std::cout << "CModbusRtuSlaveLinkLayer::GetTaskData 1" << std::endl;

    m_pxOperatingDataContainer -> m_uiFsmCommandState = GetFsmState();
    *pxDataContainer = *m_pxOperatingDataContainer;

    return true;
}

//-------------------------------------------------------------------------------
size_t CModbusRtuSlaveLinkLayer::GetObjectLength(void)
{
    //std::cout << "CModbusRtuSlaveLinkLayer GetObjectLength"  << std::endl;
    return sizeof(*this);
}

//-------------------------------------------------------------------------------
void CModbusRtuSlaveLinkLayer::StartNewThread(void)
{
    cout << "CModbusRtuSlaveLinkLayer::StartNewThread 1" << endl;
    m_uiThreadInProgress = 1;
    m_pxThread = std::make_shared<std::thread>(CModbusRtuSlaveLinkLayer::Process, this);
    // не ждем завершения работы функции
    m_pxThread -> detach();
}

//-------------------------------------------------------------------------------
void CModbusRtuSlaveLinkLayer::DestroyThread(void)
{
    cout << "CModbusRtuSlaveLinkLayer::DestroyThread 1" << endl;

    m_uiThreadInProgress = 0;
    // Ожидаем завершение первого потока (если он еще выполняется)
    if (!m_pxThread || m_pxThread->joinable())
    {
//        m_pxThread -> join();
        // Разорваем связь между потоком и его владелецом
        if (m_pxThread)
        {
            m_pxThread->detach();
        }
        m_pxThread.reset(); // Сбросим указатель
    }
}

//-------------------------------------------------------------------------------
void CModbusRtuSlaveLinkLayer::Process(CModbusRtuSlaveLinkLayer* pxModbusSlaveLinkLayer)
{
    cout << "CModbusRtuSlaveLinkLayer::Process 1" << endl;

//    while (m_uiThreadInProgress)
//    {
    pxModbusSlaveLinkLayer -> Fsm();
//    }
}

////-------------------------------------------------------------------------------
void CModbusRtuSlaveLinkLayer::CommunicationDeviceInit(const char* pccDeviceName,
        uint32_t uiBaudRate,
        uint8_t uiDataBits,
        char cParity,
        uint8_t uiStopBit)
{
    cout << "CModbusRtuSlaveLinkLayer::CommunicationDeviceInit 1" << endl;
    m_pxCommunicationDevice -> Init();
    m_pxCommunicationDevice -> SetDeviceName(pccDeviceName);
    m_pxCommunicationDevice -> SetBaudRate(uiBaudRate);
    m_pxCommunicationDevice -> SetDataBits(uiDataBits);
    m_pxCommunicationDevice -> SetParity(cParity);
    m_pxCommunicationDevice -> SetStopBit(uiStopBit);

    m_uiGuardTimeout = 10;//((((1000000UL / uiBaudRate) * 8UL * 4UL) / 1000UL) + 1);
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
void CModbusRtuSlaveLinkLayer::CommunicationStart(void)
{
    SetFsmCommandState(COMMUNICATION_START);
}

//-------------------------------------------------------------------------------
void CModbusRtuSlaveLinkLayer::CommunicationReceiveStart(void)
{
    SetFsmCommandState(COMMUNICATION_RECEIVE_START);
}

//-------------------------------------------------------------------------------
void CModbusRtuSlaveLinkLayer::ReceiveStart(void)
{
    SetFsmCommandState(COMMUNICATION_RECEIVE_START);
}

//-------------------------------------------------------------------------------
void CModbusRtuSlaveLinkLayer::TransmitStart(void)
{
    SetFsmCommandState(COMMUNICATION_TRANSMIT_START);
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
/* Builds a RTU request header */
uint16_t CModbusRtuSlaveLinkLayer::RequestBasis(uint8_t uiSlave,
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

    return _MODBUS_RTU_PRESET_REQ_LENGTH;
}

//-------------------------------------------------------------------------------
/* Builds a RTU response header */
uint16_t CModbusRtuSlaveLinkLayer::ResponseBasis(uint8_t uiSlave, uint8_t uiFunctionCode, uint8_t *puiResponse)
{
    /* In this case, the slave is certainly valid because a check is already
     * done in _modbus_rtu_listen */
    puiResponse[0] = uiSlave;
    puiResponse[1] = uiFunctionCode;

    return _MODBUS_RTU_PRESET_RSP_LENGTH;
}

//-------------------------------------------------------------------------------
/* Build the exception response */
uint16_t CModbusRtuSlaveLinkLayer::ResponseException(uint8_t uiSlave, uint8_t uiFunctionCode, uint8_t uiExceptionCode, uint8_t *puiResponse)
{
    uint16_t uiLength;

    uiLength = ResponseBasis(uiSlave, (uiFunctionCode | 0x80), puiResponse);
    /* Positive exception code */
    puiResponse[uiLength++] = uiExceptionCode;

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusRtuSlaveLinkLayer::Tail(uint8_t *puiMessage, uint16_t uiLength)
{
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
    //std::cout << "CModbusRtuSlaveLinkLayer::FrameCheck 1"  << std::endl;
    if (uiLength < _MIN_MESSAGE_LENGTH)
    {
        //std::cout << "CModbusRtuSlaveLinkLayer::FrameCheck 2"  << std::endl;
        return 0;
    }

    uint16_t uiCrc = ((static_cast<uint16_t>(puiSourse[uiLength - 1]) << 8) |
                      (static_cast<uint16_t>(puiSourse[uiLength - 2])));
    uint16_t uiCrcTemp = usCrc16(puiSourse,
                                 (uiLength - _MODBUS_RTU_CHECKSUM_LENGTH));

    //std::cout << "CModbusRtuSlaveLinkLayer::FrameCheck uiLength "  << (int)(uiLength) << std::endl;
    //std::cout << "CModbusRtuSlaveLinkLayer::FrameCheck uiCrc "  << (int)(uiCrc) << std::endl;
    //std::cout << "CModbusRtuSlaveLinkLayer::FrameCheck uiCrcTemp "  << (int)(uiCrcTemp) << std::endl;
    if (usCrc16(puiSourse,
                (uiLength - _MODBUS_RTU_CHECKSUM_LENGTH)) == uiCrc)
    {
        //std::cout << "CModbusRtuSlaveLinkLayer::FrameCheck 3"  << std::endl;
        return 1;
    }
    else
    {
        //std::cout << "CModbusRtuSlaveLinkLayer::FrameCheck 4"  << std::endl;

//        {
//            cout << "CModbusRtuSlaveLinkLayer::FrameCheck puiSourse" << endl;
//            uint8_t *pucSourceTemp;
//            pucSourceTemp = (uint8_t*)puiSourse;
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
        return 0;
    }
}

//-------------------------------------------------------------------------------
uint8_t CModbusRtuSlaveLinkLayer::Fsm(void)
{
//    std::cout << "CModbusRtuSlaveLinkLayer::Fsm 1"  << std::endl;
    while (m_uiThreadInProgress)
    {
        switch (GetFsmState())
        {
            int16_t iBytesNumber;

        case IDDLE:
//        //std::cout << "CModbusRtuSlaveLinkLayer::Fsm IDDLE"  << std::endl;
            break;

        case STOP:
//        ////std::cout << "CModbusRtuSlaveLinkLayer::Fsm STOP"  << std::endl;
            break;

        case START:
            //std::cout << "CModbusRtuSlaveLinkLayer::Fsm START"  << std::endl;
            //std::cout << "CModbusRtuSlaveLinkLayer::Fsm m_sCommunicationDeviceName" << " " << (m_sCommunicationDeviceName) << std::endl;
            Init();
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(INIT);
            break;

        case INIT:
//        //std::cout << "CModbusRtuSlaveLinkLayer::Fsm INIT 1"  << std::endl;
        {
            CTaskInterface* pxTask =
                GetResources() ->
                GetTaskPointerByNameFromMap(m_sCommunicationDeviceName);

            if (pxTask != 0)
            {
//                //std::cout << "CModbusRtuSlaveLinkLayer::Fsm INIT 2"  << std::endl;
                if (pxTask -> GetFsmState() >= READY)
                {
//                    //std::cout << "CModbusRtuSlaveLinkLayer::Fsm INIT 3"  << std::endl;
                    SetCommunicationDevice((CCommunicationDeviceInterface*)pxTask);
                    SetFsmState(READY);
                }
            }
            else
            {
//                //std::cout << "CModbusRtuSlaveLinkLayer::Fsm INIT 4"  << std::endl;
                if (GetTimerPointer() -> IsOverflow())
                {
                    //std::cout << "CModbusRtuSlaveLinkLayer::Fsm INIT 5"  << std::endl;
                    SetFsmState(STOP);
                }
            }
        }
        break;

        case READY:
//            std::cout << "CModbusRtuSlaveLinkLayer::Fsm READY"  << std::endl;
            break;

        case DONE_OK:
//        //std::cout << "CModbusRtuSlaveLinkLayer::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
            break;

        case DONE_ERROR:
//        //std::cout << "CModbusRtuSlaveLinkLayer::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
            break;

        case COMMUNICATION_START:
//            std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_START"  << std::endl;
            m_pxCommunicationDevice -> Open();
            m_uiFrameLength = 0;
            SetFsmState(COMMUNICATION_RECEIVE_START);
            break;

        case COMMUNICATION_RECEIVE_START:
//            std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START"  << std::endl;
            m_uiFrameLength = 0;
            iBytesNumber =
                m_pxCommunicationDevice ->
                ReceiveStart((m_auiRxBuffer + m_uiFrameLength),
                             (MODBUS_RTU_MAX_ADU_LENGTH - m_uiFrameLength),
                             m_uiReceiveTimeout);
//            std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START iBytesNumber "  << (int)(iBytesNumber) << std::endl;
            if (iBytesNumber > 0)
            {
//                std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START 2"  << std::endl;
                m_uiFrameLength = m_uiFrameLength + iBytesNumber;
                SetFsmState(COMMUNICATION_RECEIVE_END);
            }
            else if (iBytesNumber < 0)
            {
//                std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START 3"  << std::endl;
//            cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START errno " << errno << endl;
                SetFsmState(COMMUNICATION_RECEIVE_ERROR);
            }
            else
            {
//                std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START 4"  << std::endl;
//            cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START errno " << errno << endl;
            }
            break;

        case COMMUNICATION_RECEIVE_CONTINUE:
//        std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_CONTINUE"  << std::endl;
            m_uiFrameLength = 0;
            iBytesNumber =
                m_pxCommunicationDevice ->
                ReceiveContinue((m_auiRxBuffer + m_uiFrameLength),
                                (MODBUS_RTU_MAX_ADU_LENGTH - m_uiFrameLength),
                                m_uiReceiveTimeout);

//            std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_CONTINUE iBytesNumber "  << (int)(iBytesNumber) << std::endl;
            if (iBytesNumber > 0)
            {
//            //std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_CONTINUE 2"  << std::endl;
                m_uiFrameLength = m_uiFrameLength + iBytesNumber;
                SetFsmState(COMMUNICATION_RECEIVE_END);
            }
            else if (iBytesNumber < 0)
            {
                //std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_CONTINUE 3"  << std::endl;
//            cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_CONTINUE errno " << errno << endl;
                SetFsmState(COMMUNICATION_RECEIVE_ERROR);
            }
            else
            {
                //std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_CONTINUE 4"  << std::endl;
//            cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_CONTINUE errno " << errno << endl;
            }
            break;

        case COMMUNICATION_RECEIVE_END:
//        std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END"  << std::endl;
            iBytesNumber =
                m_pxCommunicationDevice ->
                ReceiveContinue((m_auiRxBuffer + m_uiFrameLength),
                                (MODBUS_RTU_MAX_ADU_LENGTH - m_uiFrameLength),
                                m_uiGuardTimeout);
//            std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END iBytesNumber "  << (int)(iBytesNumber) << std::endl;
            if (iBytesNumber > 0)
            {
//            //std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END 2"  << std::endl;
//            cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END errno " << errno << endl;
                m_uiFrameLength = m_uiFrameLength + iBytesNumber;
            }
            else if (iBytesNumber < 0)
            {
                //std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END 3"  << std::endl;
//            cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END errno " << errno << endl;
                SetFsmState(COMMUNICATION_RECEIVE_ERROR);
            }
            else
            {
                //std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END 4"  << std::endl;
//            cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END errno " << errno << endl;
//            SetFsmState(COMMUNICATION_FRAME_CHECK);
                if (FrameCheck(m_auiRxBuffer, m_uiFrameLength))
                {
//                    std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_FRAME_CHECK 2"  << std::endl;
                    SetFsmState(COMMUNICATION_FRAME_RECEIVED);
                }
                else
                {
//                    std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_FRAME_CHECK 3"  << std::endl;
                    SetFsmState(COMMUNICATION_RECEIVE_CONTINUE);
                }
            }
            break;

        case COMMUNICATION_FRAME_CHECK:
            //std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_FRAME_CHECK 1"  << std::endl;

//            {
//                cout << "CModbusRtuSlaveLinkLayer::Fsm m_auiRxBuffer" << endl;
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
                //std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_FRAME_CHECK 2"  << std::endl;
//            SetFsmState(COMMUNICATION_FRAME_RECEIVED);
                SetFsmState(DONE_OK);
            }
            else
            {
                //std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_FRAME_CHECK 3"  << std::endl;
                SetFsmState(COMMUNICATION_RECEIVE_CONTINUE);
            }
            break;

        case COMMUNICATION_FRAME_RECEIVED:
            //std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_FRAME_RECEIVED"  << std::endl;
            SetFsmState(DONE_OK);
            break;

        case COMMUNICATION_TRANSMIT_START:
            //std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_TRANSMIT_START"  << std::endl;
            //std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_TRANSMIT_START m_uiFrameLength "  << (int)m_uiFrameLength << std::endl;
            m_pxCommunicationDevice -> Write(m_auiTxBuffer, m_uiFrameLength);
//        SetFsmState(COMMUNICATION_FRAME_TRANSMITED);
            SetFsmState(DONE_OK);
            break;

        case COMMUNICATION_FRAME_TRANSMITED:
            //std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_FRAME_TRANSMITED"  << std::endl;
        {
            //std::cout << "CModbusRtuSlaveLinkLayer::Fsm m_auiTxBuffer" << endl;
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
        SetFsmState(DONE_OK);
        break;

        case COMMUNICATION_RECEIVE_ERROR:
            //std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_ERROR"  << std::endl;
            m_pxCommunicationDevice -> Close();
            SetFsmState(DONE_ERROR);
            break;

        case COMMUNICATION_STOP:
            std::cout << "CModbusRtuSlaveLinkLayer::Fsm COMMUNICATION_STOP"  << std::endl;
            m_pxCommunicationDevice -> Close();
            SetFsmState(READY);
//        SetFsmState(DONE_OK);
            break;

        default:
            break;
        }
    }
}

//-------------------------------------------------------------------------------
