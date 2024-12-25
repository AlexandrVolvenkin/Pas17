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
#include "ModbusSlaveLinkLayer.h"
#include "ModbusSmSlaveLinkLayer.h"

using namespace std;

//-------------------------------------------------------------------------------
CModbusSmSlaveLinkLayer::CModbusSmSlaveLinkLayer()
{
    std::cout << "CModbusSmSlaveLinkLayer constructor"  << std::endl;
    m_pxCommunicationDevice = 0;
    SetFsmState(START);

////    // Создание или открытие разделяемой памяти
////    addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
////
////    if (addr == MAP_FAILED)
////    {
////        perror("mmap");
////        exit(EXIT_FAILURE);
////    }
////
////    cout << "Shared memory created successfully at address: " << addr << endl;
//
////-------------------------------------------------------------------------------
//    int fd;
//
//    fd = shm_open("SharedMemoryTxBuffer",
//                  O_CREAT | O_RDWR,
//                  S_IRWXU | S_IRWXG);
//    if(fd < 0)
//    {
//        printf("%s\n", strerror(errno));
//    }
//    printf("Shared Memory Open: %s %d\n","SharedMemoryTxBuffer", fd);
//    ftruncate(fd, MODBUS_SM_MAX_ADU_LENGTH);
//    m_puiTxBuffer = (uint8_t*)mmap(0,
//                                   MODBUS_SM_MAX_ADU_LENGTH,
//                                   PROT_READ | PROT_WRITE, MAP_SHARED,
//                                   fd,
//                                   0);
//    close(fd);
//
////-------------------------------------------------------------------------------
//    fd = shm_open("SharedMemoryRxBuffer",
//                  O_CREAT | O_RDWR,
//                  S_IRWXU | S_IRWXG);
//    if(fd < 0)
//    {
//        printf("%s\n", strerror(errno));
//    }
//    printf("Shared Memory Open: %s %d\n","SharedMemoryRxBuffer", fd);
//    ftruncate(fd, MODBUS_SM_MAX_ADU_LENGTH);
//    m_puiRxBuffer = (uint8_t*)mmap(0,
//                                   MODBUS_SM_MAX_ADU_LENGTH,
//                                   PROT_READ | PROT_WRITE, MAP_SHARED,
//                                   fd,
//                                   0);
//    close(fd);
}

//-------------------------------------------------------------------------------
CModbusSmSlaveLinkLayer::~CModbusSmSlaveLinkLayer()
{
    m_pxCommunicationDevice -> Close();
    delete m_pxCommunicationDevice;

//    munmap(m_puiTxBuffer,
//           MODBUS_SM_MAX_ADU_LENGTH);
//    munmap(m_puiRxBuffer,
//           MODBUS_SM_MAX_ADU_LENGTH);
}

//-------------------------------------------------------------------------------
void CModbusSmSlaveLinkLayer::Process(CModbusSmSlaveLinkLayer* pxModbusSlaveLinkLayer)
{
    cout << "CModbusSmSlaveLinkLayer::Process 1" << endl;

    while (1)
    {
        pxModbusSlaveLinkLayer -> Fsm();
    }
}

//////-------------------------------------------------------------------------------
//void CModbusSmSlaveLinkLayer::CommunicationDeviceInit(const char* pccDeviceName,
//        uint32_t uiBaudRate,
//        uint8_t uiDataBits,
//        char cParity,
//        uint8_t uiStopBit)
//{
//    cout << "CModbusSmSlaveLinkLayer::CommunicationDeviceInit 1" << endl;
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
void CModbusSmSlaveLinkLayer::ReceiveEnable(void)
{
//    m_pxCommunicationDevice -> Open();
}

//-------------------------------------------------------------------------------
void CModbusSmSlaveLinkLayer::ReceiveDisable(void)
{
//    m_pxCommunicationDevice -> Close();
}

//-------------------------------------------------------------------------------
void CModbusSmSlaveLinkLayer::TransmitEnable(void)
{
//    m_pxCommunicationDevice -> Open();
}

//-------------------------------------------------------------------------------
void CModbusSmSlaveLinkLayer::TransmitDisable(void)
{
//    m_pxCommunicationDevice -> Close();
}

//-------------------------------------------------------------------------------
void CModbusSmSlaveLinkLayer::CommunicationStart(void)
{
    SetFsmCommandState(COMMUNICATION_START);
}

//-------------------------------------------------------------------------------
void CModbusSmSlaveLinkLayer::CommunicationReceiveStart(void)
{
    SetFsmCommandState(COMMUNICATION_RECEIVE_START);
}

//-------------------------------------------------------------------------------
void CModbusSmSlaveLinkLayer::ReceiveStart(void)
{
    SetFsmCommandState(COMMUNICATION_RECEIVE_START);
}

//-------------------------------------------------------------------------------
void CModbusSmSlaveLinkLayer::TransmitStart(void)
{
    cout << "CModbusSmSlaveLinkLayer::TransmitStart 1" << endl;
    SetFsmCommandState(COMMUNICATION_TRANSMIT_START);
}

//-------------------------------------------------------------------------------
uint8_t* CModbusSmSlaveLinkLayer::GetRxBuffer(void)
{
    return &m_auiRxBuffer[0];
}

//-------------------------------------------------------------------------------
uint8_t* CModbusSmSlaveLinkLayer::GetTxBuffer(void)
{
    return &m_auiTxBuffer[0];
}

//-------------------------------------------------------------------------------
uint8_t* CModbusSmSlaveLinkLayer::GetRxPdu(void)
{
    return &m_auiRxBuffer[1];
}

//-------------------------------------------------------------------------------
uint8_t* CModbusSmSlaveLinkLayer::GetTxPdu(void)
{
    return &m_auiTxBuffer[1];
}

//-------------------------------------------------------------------------------
uint8_t CModbusSmSlaveLinkLayer::GetPduOffset(void)
{
    return 1;
}

//-------------------------------------------------------------------------------
uint8_t CModbusSmSlaveLinkLayer::GetSlaveAddress(void)
{
    return m_auiRxBuffer[0];
}

//-------------------------------------------------------------------------------
void CModbusSmSlaveLinkLayer::SetSlaveAddress(uint8_t uiData)
{

}

//-------------------------------------------------------------------------------
uint8_t CModbusSmSlaveLinkLayer::GetFunctionCode(void)
{
    return m_auiRxBuffer[1];
}

//-------------------------------------------------------------------------------
void CModbusSmSlaveLinkLayer::SetFunctionCode(uint8_t uiData)
{

}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlaveLinkLayer::GetDataAddress(void)
{
    return ((static_cast<uint16_t>(m_auiRxBuffer[2]) << 8) |
            (static_cast<uint16_t>(m_auiRxBuffer[3])));
}

//-------------------------------------------------------------------------------
void CModbusSmSlaveLinkLayer::SetDataAddress(uint16_t uiData)
{

}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlaveLinkLayer::GetBitNumber(void)
{
    return ((static_cast<uint16_t>(m_auiRxBuffer[4]) << 8) |
            (static_cast<uint16_t>(m_auiRxBuffer[5])));
}

//-------------------------------------------------------------------------------
void CModbusSmSlaveLinkLayer::SetBitNumber(uint16_t uiData)
{

}

//-------------------------------------------------------------------------------
/* Builds a RTU request header */
uint16_t CModbusSmSlaveLinkLayer::RequestBasis(uint8_t uiSlave,
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
uint16_t CModbusSmSlaveLinkLayer::ResponseBasis(uint8_t uiSlave, uint8_t uiFunctionCode, uint8_t *puiResponse)
{
    /* In this case, the slave is certainly valid because a check is already
     * done in _modbus_rtu_listen */
    puiResponse[0] = uiSlave;
    puiResponse[1] = uiFunctionCode;

    return _MODBUS_SM_PRESET_RSP_LENGTH;
}

//-------------------------------------------------------------------------------
/* Build the exception response */
uint16_t CModbusSmSlaveLinkLayer::ResponseException(uint8_t uiSlave, uint8_t uiFunctionCode, uint8_t uiExceptionCode, uint8_t *puiResponse)
{
    uint16_t uiLength;

    uiLength = ResponseBasis(uiSlave, (uiFunctionCode | 0x80), puiResponse);
    /* Positive exception code */
    puiResponse[uiLength++] = uiExceptionCode;

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlaveLinkLayer::Tail(uint8_t *puiMessage, uint16_t uiLength)
{
    uint16_t uiCrc = usCrc16(puiMessage, uiLength);
    puiMessage[uiLength++] = uiCrc & 0x00FF;
    puiMessage[uiLength++] = uiCrc >> 8;

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlaveLinkLayer::Send(uint8_t *puiMessage, uint16_t uiLength)
{
    return m_pxCommunicationDevice -> Write(puiMessage, uiLength);
}

//-------------------------------------------------------------------------------
int16_t CModbusSmSlaveLinkLayer::Receive(uint8_t *puiDestination, uint16_t uiLength)
{
    return m_pxCommunicationDevice -> Read(puiDestination, uiLength);
}

//-------------------------------------------------------------------------------
int8_t CModbusSmSlaveLinkLayer::FrameCheck(uint8_t *puiSourse, uint16_t uiLength)
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
uint8_t CModbusSmSlaveLinkLayer::Fsm(void)
{
//    std::cout << "CModbusSmSlaveLinkLayer::Fsm 1"  << std::endl;
    switch (GetFsmState())
    {
        int16_t iBytesNumber;

    case IDDLE:
//        std::cout << "CModbusSmSlaveLinkLayer::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CModbusSmSlaveLinkLayer::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CModbusSmSlaveLinkLayer::Fsm START"  << std::endl;
        std::cout << "CModbusSmSlaveLinkLayer::Fsm m_sCommunicationDeviceName" << " " << (m_sCommunicationDeviceName) << std::endl;
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
//        std::cout << "CModbusSmSlaveLinkLayer::Fsm INIT 1"  << std::endl;
        {
            CTaskInterface* pxTask =
                GetResources() ->
                GetTaskPointerByNameFromMap(m_sCommunicationDeviceName);

            if (pxTask != 0)
            {
//                std::cout << "CModbusSmSlaveLinkLayer::Fsm INIT 2"  << std::endl;
                if (pxTask -> GetFsmState() >= READY)
                {
//                    std::cout << "CModbusSmSlaveLinkLayer::Fsm INIT 3"  << std::endl;
                    SetCommunicationDevice((CCommunicationDeviceInterfaceNew*)pxTask);
                    SetFsmCommandState(0);
                    SetFsmState(READY);
                }
            }
            else
            {
//                std::cout << "CModbusSmSlaveLinkLayer::Fsm INIT 4"  << std::endl;
                if (GetTimerPointer() -> IsOverflow())
                {
                    std::cout << "CModbusSmSlaveLinkLayer::Fsm INIT 5"  << std::endl;
                    SetFsmState(STOP);
                }
            }
        }
        break;

    case READY:
//        std::cout << "CModbusSmSlaveLinkLayer::Fsm READY"  << std::endl;

        if (GetFsmCommandState() != 0)
        {
            std::cout << "CModbusSmSlaveLinkLayer::Fsm READY 2"  << std::endl;
            SetFsmState(GetFsmCommandState());
            SetFsmCommandState(0);
        }
        break;

    case DONE_OK:
        std::cout << "CModbusSmSlaveLinkLayer::Fsm DONE_OK"  << std::endl;
        SetFsmOperationStatus(DONE_OK);
        SetFsmState(READY);
        break;

    case DONE_ERROR:
        std::cout << "CModbusSmSlaveLinkLayer::Fsm DONE_ERROR"  << std::endl;
        SetFsmOperationStatus(DONE_ERROR);
        SetFsmState(READY);
        break;

    case COMMUNICATION_START:
        std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_START"  << std::endl;
        m_pxCommunicationDevice -> Open();
        m_uiFrameLength = 0;
//        SetFsmState(COMMUNICATION_RECEIVE_START);
        SetFsmState(COMMUNICATION_FRAME_RECEIVED);
        break;

    case COMMUNICATION_RECEIVE_START:
//        std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START"  << std::endl;
        m_uiFrameLength = 0;
        iBytesNumber =
            m_pxCommunicationDevice ->
            ReceiveStart((m_auiRxBuffer + m_uiFrameLength),
                         (MODBUS_SM_MAX_ADU_LENGTH - m_uiFrameLength),
                         m_uiReceiveTimeout);
        if (iBytesNumber > 0)
        {
            std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START 2"  << std::endl;
            m_uiFrameLength = m_uiFrameLength + iBytesNumber;
//            {
//                cout << "CModbusSmSlaveLinkLayer::Fsm m_auiRxBuffer" << endl;
//                unsigned char *pucSourceTemp;
//                pucSourceTemp = (unsigned char*)m_auiRxBuffer;
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
            std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START 3"  << std::endl;
            cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START errno " << errno << endl;
            SetFsmState(COMMUNICATION_RECEIVE_ERROR);
        }
        else
        {
//            std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START 4"  << std::endl;
//            cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_START errno " << errno << endl;
        }
        break;

    case COMMUNICATION_RECEIVE_END:
        std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END"  << std::endl;
//        iBytesNumber =
//            m_pxCommunicationDevice ->
//            ReceiveContinue((m_auiRxBuffer + m_uiFrameLength),
//                            (MODBUS_SM_MAX_ADU_LENGTH - m_uiFrameLength),
//                            m_uiGuardTimeout);
//        if (iBytesNumber > 0)
//        {
//            std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END 2"  << std::endl;
//            cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END errno " << errno << endl;
//            m_uiFrameLength = m_uiFrameLength + iBytesNumber;
//        }
//        else if (iBytesNumber < 0)
//        {
//            std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END 3"  << std::endl;
//            cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END errno " << errno << endl;
//            SetFsmState(COMMUNICATION_RECEIVE_ERROR);
//        }
//        else
//        {
//            std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END 4"  << std::endl;
//            cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_END errno " << errno << endl;
//            SetFsmState(COMMUNICATION_FRAME_CHECK);
//
//            {
//                cout << "CModbusSmSlaveLinkLayer::Fsm m_auiRxBuffer" << endl;
//                unsigned char *pucSourceTemp;
//                pucSourceTemp = (unsigned char*)m_auiRxBuffer;
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
//        }
        break;

    case COMMUNICATION_FRAME_CHECK:
        std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_FRAME_CHECK 1"  << std::endl;
        if (FrameCheck(m_auiRxBuffer, m_uiFrameLength))
        {
            std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_FRAME_CHECK 2"  << std::endl;
            SetFsmState(COMMUNICATION_FRAME_RECEIVED);
        }
        else
        {
            std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_FRAME_CHECK 3"  << std::endl;
            SetFsmState(COMMUNICATION_RECEIVE_ERROR);
        }
        break;

    case COMMUNICATION_FRAME_RECEIVED:
        std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_FRAME_RECEIVED"  << std::endl;

        SetFsmState(DONE_OK);
        break;

    case COMMUNICATION_TRANSMIT_START:
        std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_TRANSMIT_START"  << std::endl;

//        {
//            cout << "CModbusSmSlaveLinkLayer::Fsm m_auiTxBuffer" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)m_auiTxBuffer;
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
        std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_TRANSMIT_START m_uiFrameLength "  << (int)m_uiFrameLength << std::endl;
        m_pxCommunicationDevice -> Write(m_auiTxBuffer, m_uiFrameLength);
        SetFsmState(COMMUNICATION_FRAME_TRANSMITED);
        break;

    case COMMUNICATION_FRAME_TRANSMITED:
        std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_FRAME_TRANSMITED"  << std::endl;
        SetFsmState(DONE_OK);
        {
            cout << "CModbusSmSlaveLinkLayer::Fsm m_auiTxBuffer" << endl;
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
        std::cout << "CModbusSmSlaveLinkLayer::Fsm COMMUNICATION_RECEIVE_ERROR"  << std::endl;
        SetFsmState(DONE_ERROR);
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------

