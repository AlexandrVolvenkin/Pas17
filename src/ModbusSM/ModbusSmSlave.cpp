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

#include "Task.h"
#include "Resources.h"
#include "Platform.h"
#include "DataStore.h"
#include "DeviceControl.h"
#include "Link.h"
#include "DataContainer.h"
#include "ModbusSmSlave.h"

using namespace std;

//-------------------------------------------------------------------------------
CModbusSmSlave::CModbusSmSlave()
{
    std::cout << "CModbusSmSlave constructor"  << std::endl;
    m_pxModbusSlaveLinkLayer = 0;
    m_pxDeviceControl = 0;
    m_pxDeviceControlLink = 0;
    // получим имя класса.
    sprintf(GetTaskNamePointer(),
            "%s",
            typeid(*this).name());
    m_puiIntermediateBuff = new uint8_t[MAX_MODBUS_MESSAGE_LENGTH];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CModbusSmSlave::~CModbusSmSlave()
{
    delete[] m_puiIntermediateBuff;
    WorkingArraysDelete();
}

//-------------------------------------------------------------------------------
void CModbusSmSlave::SetModbusSlaveLinkLayerName(std::string sName)
{
    m_sModbusSlaveLinkLayerName = sName;
}

//-------------------------------------------------------------------------------
void CModbusSmSlave::SetModbusSlaveLinkLayer(CModbusSmSlaveLinkLayer* pxModbusSlaveLinkLayer)
{
    m_pxModbusSlaveLinkLayer = pxModbusSlaveLinkLayer;
}

//-------------------------------------------------------------------------------
void CModbusSmSlave::SetDeviceControlName(std::string sName)
{
    m_sDeviceControlName = sName;
}

//-------------------------------------------------------------------------------
void CModbusSmSlave::SetDeviceControl(CDeviceControl* pxDeviceControl)
{
    m_pxDeviceControl = pxDeviceControl;
}

//-------------------------------------------------------------------------------
void CModbusSmSlave::SetDeviceControlLinkName(const std::string sName)
{
    m_sDeviceControlLinkName = sName;
}

////-------------------------------------------------------------------------------
//std::string CModbusSmSlave::GetDeviceControlLinkName() const
//{
//    return m_sDeviceControlLinkName;
//}

//-------------------------------------------------------------------------------
void CModbusSmSlave::SetDeviceControlLink(CLinkInterface* pxLink)
{
    m_pxDeviceControlLink = pxLink;
}

//-------------------------------------------------------------------------------
CLinkInterface* CModbusSmSlave::GetDeviceControlLink() const
{
    return m_pxDeviceControlLink;
}

//-------------------------------------------------------------------------------
const char *CModbusSmSlave::ModbusStringError(int errnum)
{
    switch (errnum)
    {
    case EMBXILFUN:
        return "Illegal function";
    case EMBXILADD:
        return "Illegal data address";
    case EMBXILVAL:
        return "Illegal data value";
    case EMBXSFAIL:
        return "Slave device or server failure";
    case EMBXACK:
        return "Acknowledge";
    case EMBXSBUSY:
        return "Slave device or server is busy";
    case EMBXNACK:
        return "Negative acknowledge";
    case EMBXMEMPAR:
        return "Memory parity error";
    case EMBXGPATH:
        return "Gateway path unavailable";
    case EMBXGTAR:
        return "Target device failed to respond";
    case EMBBADCRC:
        return "Invalid CRC";
    case EMBBADDATA:
        return "Invalid data";
    case EMBBADEXC:
        return "Invalid exception code";
    case EMBMDATA:
        return "Too many data";
    default:
        return strerror(errnum);
    }
}

//-------------------------------------------------------------------------------
void CModbusSmSlave::ModbusWorkingArraysInit(void)
{
    std::cout << "CModbusSmSlave ModbusWorkingArraysInit 1"  << std::endl;
    m_puiCoils = m_pxResources -> GetCoils();
    m_uiCoilsNumber = m_pxResources -> GetCoilsNumber();
    m_puiDiscreteInputs = m_pxResources -> GetDiscreteInputs();
    m_uiDiscreteInputsNumber = m_pxResources -> GetDiscreteInputsNumber();
    m_puiHoldingRegisters = m_pxResources -> GetHoldingRegisters();
    m_uiHoldingRegistersNumber = m_pxResources -> GetHoldingRegistersNumber();
    m_puiInputRegisters = m_pxResources -> GetInputRegisters();
    m_uiInputRegistersNumber = m_pxResources -> GetInputRegistersNumber();
}

//-------------------------------------------------------------------------------
void CModbusSmSlave::WorkingArraysInit(uint8_t *puiCoils,
                                     uint8_t *puiDiscreteInputs,
                                     uint16_t *puiHoldingRegisters,
                                     uint16_t *puiInputRegisters,
                                     uint16_t uiCoilsNumber,
                                     uint16_t uiDiscreteInputsNumber,
                                     uint16_t uiHoldingRegistersNumber,
                                     uint16_t uiInputRegistersNumber)
{
    m_puiCoils = puiCoils;
    m_puiDiscreteInputs = puiDiscreteInputs;
    m_puiHoldingRegisters = puiHoldingRegisters;
    m_puiInputRegisters = puiInputRegisters;
    m_uiCoilsNumber = uiCoilsNumber;
    m_uiDiscreteInputsNumber = uiDiscreteInputsNumber;
    m_uiHoldingRegistersNumber = uiHoldingRegistersNumber;
    m_uiInputRegistersNumber = uiInputRegistersNumber;
}

//-------------------------------------------------------------------------------
void CModbusSmSlave::WorkingArraysCreate(uint16_t uiCoilsNumber,
                                       uint16_t uiDiscreteInputsNumber,
                                       uint16_t uiHoldingRegistersNumber,
                                       uint16_t uiInputRegistersNumber)
{
    m_puiCoils = new uint8_t(uiCoilsNumber);
    m_puiDiscreteInputs = new uint8_t(uiDiscreteInputsNumber);
    m_puiHoldingRegisters = new uint16_t(uiHoldingRegistersNumber);
    m_puiInputRegisters = new uint16_t(uiInputRegistersNumber);

    m_uiCoilsNumber = uiCoilsNumber;
    m_uiDiscreteInputsNumber = uiDiscreteInputsNumber;
    m_uiHoldingRegistersNumber = uiHoldingRegistersNumber;
    m_uiInputRegistersNumber = uiInputRegistersNumber;
}

//-------------------------------------------------------------------------------
void CModbusSmSlave::WorkingArraysDelete(void)
{
    delete m_puiCoils;
    delete m_puiDiscreteInputs;
    delete m_puiHoldingRegisters;
    delete m_puiInputRegisters;
}

////-------------------------------------------------------------------------------
//void CModbusSmSlave::SlaveSet(uint8_t uiSlave)
//{
//    m_uiOwnAddress = uiSlave;
//}

//-------------------------------------------------------------------------------
/* Builds a TCP request header */
uint16_t CModbusSmSlave::RequestBasis(uint8_t uiSlave,
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

//    return _MODBUS_TCP_PRESET_REQ_LENGTH;
}

//-------------------------------------------------------------------------------
/* Builds a TCP response header */
uint16_t CModbusSmSlave::ResponseBasis(uint8_t uiSlave, uint8_t uiFunctionCode, uint8_t *puiResponse)
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
}

////-------------------------------------------------------------------------------
//int8_t CModbusSmSlave::MessengerIsReady(void)
//{
////    if (GetFsmState() == IDDLE)
////    {
////        return 1;
////    }
////    else
////    {
////        return 0;
////    }
//}
//
////-------------------------------------------------------------------------------
//uint16_t CModbusSmSlave::SendMessage(uint8_t *puiMessage, uint16_t uiLength)
//{
//    uiLength = Tail(puiMessage, uiLength);
//    return Send(puiMessage, uiLength);
//}

//-------------------------------------------------------------------------------
/* Build the exception response */
uint16_t CModbusSmSlave::ResponseException(uint8_t uiSlave, uint8_t uiFunctionCode, uint8_t uiExceptionCode, uint8_t *puiResponse)
{
    uint16_t uiLength;

    uiLength = m_pxModbusSlaveLinkLayer ->
               ResponseBasis(uiSlave, (uiFunctionCode | 0x80), puiResponse);
    /* Positive exception code */
    puiResponse[uiLength++] = uiExceptionCode;

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::ByteToBitPack(uint16_t uiAddress,
                                     uint16_t uiNumberB,
                                     uint8_t *m_puiCoils,
                                     uint8_t *puiResponse,
                                     uint16_t uiLength)
{
    uint8_t uiData = 0;
    uint8_t uiShift = 0;
    for (uint16_t i = 0; i < uiNumberB; i++)
    {
        if (m_puiCoils[uiAddress++])
        {
            uiData |= (1 << uiShift);
        }
        // Byte is full?
        if (uiShift == 7)
        {
            puiResponse[uiLength++] = uiData;
            uiData = 0;
            uiShift = 0;
        }
        else
        {
            uiShift++;
        }
    }

    if (uiShift != 0)
    {
        puiResponse[uiLength++] = uiData;
    }

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::ReadCoils(void)
{
    std::cout << "CModbusSmSlave::ReadCoils 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    std::cout << "CModbusSmSlave::ReadCoils uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSmSlave::ReadCoils uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CModbusSmSlave::ReadCoils uiAddress "  << (int)uiAddress << std::endl;
    std::cout << "CModbusSmSlave::ReadCoils uiNumberB "  << (int)uiNumberB << std::endl;

    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
    {
        std::cout << "CModbusSmSlave::ReadCoils 2" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
    {
        std::cout << "CModbusSmSlave::ReadCoils 3" << std::endl;
        std::cout << "CModbusSmSlave::ReadCoils m_uiCoilsNumber "  << (int)m_uiCoilsNumber << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        std::cout << "CModbusSmSlave::ReadCoils 4" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        if (uiNumberB % 8)
        {
            std::cout << "CModbusSmSlave::ReadCoils 5" << std::endl;
            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
        }
        else
        {
            std::cout << "CModbusSmSlave::ReadCoils 6" << std::endl;
            puiResponse[uiLength++] = (uiNumberB / 8);
        }
        uiLength = ByteToBitPack(uiAddress,
                                 uiNumberB,
                                 m_puiCoils,
                                 puiResponse,
                                 uiLength);
    }
    std::cout << "CModbusSmSlave::ReadCoils 7" << std::endl;
    return uiLength;
}

////-------------------------------------------------------------------------------
//uint16_t CModbusSmSlave::ReadCoils(void)
//{
//    std::cout << "CModbusSmSlave::ReadCoils 1" << std::endl;
//
//    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
//    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
//    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
//    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();
//
//    int8_t uiSlave = puiRequest[uiPduOffset - 1];
//    int8_t uiFunctionCode = puiRequest[uiPduOffset];
//    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
//                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));
//
//    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
//                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));
//
//    std::cout << "CModbusSmSlave::ReadCoils uiSlave "  << (int)uiSlave << std::endl;
//    std::cout << "CModbusSmSlave::ReadCoils uiFunctionCode "  << (int)uiFunctionCode << std::endl;
//    std::cout << "CModbusSmSlave::ReadCoils uiAddress "  << (int)uiAddress << std::endl;
//    std::cout << "CModbusSmSlave::ReadCoils uiNumberB "  << (int)uiNumberB << std::endl;
//
//    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
//    {
//        std::cout << "CModbusSmSlave::ReadCoils 2" << std::endl;
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
//                                     puiResponse);
//    }
//    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
//    {
//        std::cout << "CModbusSmSlave::ReadCoils 3" << std::endl;
//        std::cout << "CModbusSmSlave::ReadCoils m_uiCoilsNumber "  << (int)m_uiCoilsNumber << std::endl;
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
//                                     puiResponse);
//    }
//    else
//    {
//        std::cout << "CModbusSmSlave::ReadCoils 4" << std::endl;
//
//        m_uiFunctionCode = uiFunctionCode;
//
//        m_pxDeviceControlLink ->
//        GetDataContainerPointer() ->
//        SetContainerData(uiFunctionCode,
//                         0,
//                         &puiResponse[uiPduOffset + 3],
//                         0,
//                         0);
//
//
//
//
//
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
//
////        if (uiNumberB % 8)
////        {
////            std::cout << "CModbusSmSlave::ReadCoils 5" << std::endl;
////            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
////        }
////        else
////        {
////            std::cout << "CModbusSmSlave::ReadCoils 6" << std::endl;
////            puiResponse[uiLength++] = (uiNumberB / 8);
////        }
////        uiLength = ByteToBitPack(uiAddress,
////                                 uiNumberB,
////                                 m_puiCoils,
////                                 puiResponse,
////                                 uiLength);
//    }
//    std::cout << "CModbusSmSlave::ReadCoils 7" << std::endl;
//    return uiLength;
//}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::ReadDiscreteInputs(void)
{
    std::cout << "CModbusSmSlave::ReadDiscreteInputs 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiDiscreteInputsNumber)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        std::cout << "CModbusSmSlave::ReadDiscreteInputs 4" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        if (uiNumberB % 8)
        {
            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
        }
        else
        {
            puiResponse[uiLength++] = (uiNumberB / 8);
        }
        uiLength = ByteToBitPack(uiAddress,
                                 uiNumberB,
                                 m_puiDiscreteInputs,
                                 puiResponse,
                                 uiLength);
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::ReadHoldingRegisters(void)
{
    std::cout << "CModbusSmSlave::ReadHoldingRegisters 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    std::cout << "CModbusSmSlave::ReadHoldingRegisters uiAddress "  << (int)uiAddress << std::endl;
    std::cout << "CModbusSmSlave::ReadHoldingRegisters uiNumberB "  << (int)uiNumberB << std::endl;

    if (uiNumberB < 1 || MODBUS_MAX_READ_REGISTERS < uiNumberB)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiHoldingRegistersNumber)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
        puiResponse[uiLength++] = (uiNumberB << 1);
        for (uint16_t i = 0; i < uiNumberB; i++)
        {
            int8_t uiData = 0;
            uiData = (static_cast<uint8_t>(m_puiHoldingRegisters[uiAddress] >> 8));
            uiData = (static_cast<uint8_t>(m_puiHoldingRegisters[uiAddress] & 0x00FF));
            puiResponse[uiLength++] = (static_cast<uint8_t>(m_puiHoldingRegisters[uiAddress] >> 8));
            puiResponse[uiLength++] = (static_cast<uint8_t>(m_puiHoldingRegisters[uiAddress] & 0x00FF));
            uiAddress++;
        }
    }

    std::cout << "CModbusSmSlave::ReadHoldingRegisters uiLength "  << (int)uiLength << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::ReadInputRegisters(void)
{
    std::cout << "CModbusSmSlave::ReadInputRegisters 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_READ_REGISTERS < uiNumberB)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiInputRegistersNumber)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
        puiResponse[uiLength++] = (uiNumberB << 1);
        for (uint16_t i = 0; i < uiNumberB; i++)
        {
            puiResponse[uiLength++] = (static_cast<uint8_t>(m_puiInputRegisters[uiAddress] >> 8));
            puiResponse[uiLength++] = (static_cast<uint8_t>(m_puiInputRegisters[uiAddress] & 0x00FF));
            uiAddress++;
        }
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::WriteSingleCoil(void)
{
    std::cout << "CModbusSmSlave::WriteSingleCoil 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    if (uiAddress >= m_uiCoilsNumber)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        uint16_t uiData = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                           (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

        if (uiData == 0xFF00 || uiData == 0x0)
        {
            if (uiData)
            {
                m_puiCoils[uiAddress] = 1;
            }
            else
            {
                m_puiCoils[uiAddress] = 0;
            }
            memcpy(puiResponse, puiRequest, uiLength);
        }
        else
        {
            uiLength = m_pxModbusSlaveLinkLayer ->
                       ResponseException(uiSlave,
                                         uiFunctionCode,
                                         MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                         puiResponse);
        }

    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::WriteSingleRegister(void)
{
    std::cout << "CModbusSmSlave::WriteSingleRegister 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    if (uiAddress >= m_uiHoldingRegistersNumber)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        m_puiHoldingRegisters[uiAddress] =
            ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
             (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));
        memcpy(puiResponse, puiRequest, uiLength);
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::WriteMultipleCoils(void)
{
    std::cout << "CModbusSmSlave::WriteMultipleCoils 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_WRITE_BITS < uiNumberB)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        SetBytesFromBits(m_puiCoils, uiAddress, uiNumberB, &puiRequest[uiPduOffset + 6]);

        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
        /* 4 to copy the bit address (2) and the quantity of bits */
        memcpy(puiResponse + uiLength, puiRequest + uiLength, 4);
        uiLength += 4;
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::WriteMultipleRegisters(void)
{
    std::cout << "CModbusSmSlave::WriteMultipleRegisters 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_WRITE_REGISTERS < uiNumberB)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiHoldingRegistersNumber)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        for (uint16_t i = 0; i < uiNumberB; i++)
        {
            m_puiHoldingRegisters[uiAddress++] = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 6]) << 8) |
                                                  (static_cast<uint16_t>(puiRequest[uiPduOffset + 6 + 1])));
            uiPduOffset += 2;
        }

        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
        /* 4 to copy the bit address (2) and the quantity of bits */
        memcpy(puiResponse + uiLength, puiRequest + uiLength, 4);
        uiLength += 4;

    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::ReadExceptionStatus(void)
{
//    errno = ENOPROTOOPT;
    return -1;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::WriteAndReadRegisters(void)
{

    return 0;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::ReportSlaveID(void)
{
    std::cout << "CModbusSmSlave::ReportSlaveID 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    std::cout << "CModbusSmSlave::ReportSlaveID uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSmSlave::ReportSlaveID uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    std::cout << "CModbusSmSlave::ReportSlaveID 4" << std::endl;

    CDeviceControl* pxDeviceControl =
        (CDeviceControl*)GetResources() ->
        GetCommonTaskFromMapPointer("DeviceControlRtuUpperLevel");

    uiLength = pxDeviceControl ->
               ConfigurationRead(&puiResponse[uiPduOffset + 2]);
//    uiLength = m_pxResources ->
//               m_pxDeviceControl ->
//               ConfigurationRead(&puiResponse[uiPduOffset + 2]);

//    uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
//    memcpy(&puiResponse[uiPduOffset + 2], auiTempData, sizeof(auiTempData));
//    uiLength += sizeof(auiTempData);

    // количество байт в прикладном сообщении массиве конфигурации, не включая остальные.
    puiResponse[uiPduOffset + 1] = uiLength;//sizeof(auiTempData);// + 1;
    uiLength ++;
    uiLength += m_pxModbusSlaveLinkLayer ->
                ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

    std::cout << "CModbusSmSlave::ReportSlaveID 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::Programming(void)
{
    std::cout << "CModbusSmSlave::Programming 1" << std::endl;
//// Функция 0x46 "программирование". Описание протокола обмена:
//// Buff[0] - адрес Slave.
//// Buff[1] - код функции.
//// Buff[2] - количество байт в запросе команды старший байт.
//// Buff[3] - количество байт в запросе команды младший байт.
//// Buff[4] - код операции(0 - запрос версии ПО, 1 - чтение блокаБД, 2 - запись блокаБД).
//// Buff[5] - номер блока БД.
//
//    // Смещения в принятом буфере.
//    enum
//    {
//        REQUEST_LENGTH = 2,
//        REQUEST_COMMAND = 3,
//        BLOCK_NUMBER = 4,
//        DATA_BEGIN = 5,
//    };

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

//    uint16_t uiNumberB = (static_cast<uint16_t>(puiRequest[uiPduOffset + REQUEST_LENGTH]));
//
//    if (uiNumberB != (uiLength - 6))
//    {
//        uiLength = m_pxModbusSlaveLinkLayer ->
//    ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
//                                     puiResponse);
//    }
//    else
//    {
//        uint8_t uiBlockNumber;
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
//        // Какой код операции?
//        switch(puiRequest[uiPduOffset + REQUEST_COMMAND])
//        {
//        case 0x00:
//            // Получение версии ПО.
//            puiResponse[uiLength++] = 0;
//            puiResponse[uiLength++] = 4;
//            puiResponse[uiLength++] = puiRequest[uiPduOffset + REQUEST_COMMAND];
//            puiResponse[uiLength++] = 2; // 1.7.01
//            puiResponse[uiLength++] = 0;
//            puiResponse[uiLength++] = 0x02;
//            break;
//
//        case 0x01:
//            // Чтение блока БД.
//            uiBlockNumber = puiRequest[uiPduOffset + BLOCK_NUMBER];
//            // Блок не существует?
//            if (uiBlockNumber > CDataBase::BLOCKS_QUANTITY)
//            {
//                uiLength = m_pxModbusSlaveLinkLayer ->
//                      ResponseException(uiSlave,
//                                             uiFunctionCode,
//                                             MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
//                                             puiResponse);
//            }
//            else
//            {
//                // Прочитаем блок БД.
//                uint16_t uiLengthLocal = CDataBase::Read(&puiResponse[uiPduOffset + DATA_BEGIN], uiBlockNumber);
//                puiResponse[uiLength++] = 0;
//                puiResponse[uiLength++] = uiLengthLocal;
//                puiResponse[uiLength++] = puiRequest[uiPduOffset + REQUEST_COMMAND];
//                puiResponse[uiLength++] = uiBlockNumber;
//                uiLength += uiLengthLocal;
//            }
//            break;
//
//
//        case 0x02:
//            // Запись блока БД.
//            CDataBase::SetStatus(0);
//            uiBlockNumber = puiRequest[uiPduOffset + BLOCK_NUMBER];
//            // Блок не существует?
//            if (uiBlockNumber > CDataBase::BLOCKS_QUANTITY)
//            {
//                uiLength = m_pxModbusSlaveLinkLayer ->
//                                        ResponseException(uiSlave,
//                                             uiFunctionCode,
//                                             MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
//                                             puiResponse);
//            }
//            // Размер блока не соответствует?
//            else if ((puiRequest[uiPduOffset + REQUEST_LENGTH] - 2) != CDataBase::m_DStruct[uiBlockNumber].Size)
//            {
//                uiLength = m_pxModbusSlaveLinkLayer ->
//                                                ResponseException(uiSlave,
//                                             uiFunctionCode,
//                                             MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
//                                             puiResponse);
//            }
//            else
//            {
//                // Сохраним блок БД.
//                // Успешно?
//                if (CDataBase::Write(&puiRequest[uiPduOffset + DATA_BEGIN], uiBlockNumber))
//                {
//                    CDataBase::SetStatus(1);
//
//                    uiLength = m_pxModbusSlaveLinkLayer ->
//                                                        ResponseException(uiSlave,
//                                                 uiFunctionCode,
//                                                 MODBUS_EXCEPTION_ACKNOWLEDGE,
//                                                 puiResponse);
//                }
//                else
//                {
//                    CDataBase::SetStatus(0);
//
//                    uiLength = m_pxModbusSlaveLinkLayer ->
//                                                                ResponseException(uiSlave,
//                                                 uiFunctionCode,
//                                                 MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE,
//                                                 puiResponse);
//                }
//            }
//            break;
//
//        case 0x03:
//            if(!(CDataBase::GetStatus()))
//            {
//                uiLength = m_pxModbusSlaveLinkLayer ->
//                                                                        ResponseException(uiSlave,
//                                             uiFunctionCode,
//                                             MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
//                                             puiResponse);
//            }
//            break;
//
//        default:
//            uiLength = m_pxModbusSlaveLinkLayer ->
    //                                                                                ResponseException(uiSlave,
//                                         uiFunctionCode,
//                                         MODBUS_EXCEPTION_ILLEGAL_FUNCTION,
//                                         puiResponse);
//            break;
//        };
//    }

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::PollProgramming(void)

{
    std::cout << "CModbusSmSlave::PollProgramming 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    CDeviceControl* pxDeviceControl =
        (CDeviceControl*)GetResources() ->
        GetCommonTaskFromMapPointer("DeviceControlRtuUpperLevel");

    // Ожидаем окончания записи автоматом устройства хранения.
    // Устройство хранения закончило запись успешно?
    if (pxDeviceControl -> IsDoneOk())
    {
        std::cout << "CModbusSmSlave::PollProgramming 2" << std::endl;

        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

//        // номер блока базы данных
//        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
//        uiLength ++;

//        uiLength = m_pxModbusSlaveLinkLayer ->
//        ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ACKNOWLEDGE,
//                                     puiResponse);
    }
//    // Устройство хранения закончило запись не успешно?
//    else if (pxDataStoreFileSystem -> IsDoneError())
//    {
//        uiLength = m_pxModbusSlaveLinkLayer ->
    //    ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
//                                     puiResponse);
//    }
    else
    {
        std::cout << "CModbusSmSlave::PollProgramming 3" << std::endl;;

        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
                                     puiResponse);
    }

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::DataBaseRead(void)
{
    std::cout << "CModbusSmSlave::DataBaseRead 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint8_t uiBlockIndex = puiRequest[uiPduOffset + 1];

    std::cout << "CModbusSmSlave::DataBaseRead uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSmSlave::DataBaseRead uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CModbusSmSlave::DataBaseRead uiBlockIndex "  << (int)uiBlockIndex << std::endl;

    if ((uiBlockIndex < 0) ||
            (uiBlockIndex > (CDataStore::MAX_BLOCKS_NUMBER - 1)))
    {
        std::cout << "CModbusSmSlave::DataBaseRead 2" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else
    {
        std::cout << "CModbusSmSlave::DataBaseRead 4" << std::endl;

        m_uiFunctionCode = uiFunctionCode;

        m_pxDeviceControlLink ->
        GetDataContainerPointer() ->
        SetContainerData(CDeviceControl::DATA_BASE_BLOCK_READ,
                         uiBlockIndex,
                         m_puiIntermediateBuff,
                         0,
                         0);

        ((CDeviceControl*)(m_pxDeviceControlLink ->
                           GetTaskPerformerPointer())) ->
        SetCommandDataLink(m_pxDeviceControlLink);

        uiLength = 5;

        // не нуль, ожидаем ответ
        m_uiLength = uiLength;
    }

    std::cout << "CModbusSmSlave::DataBaseRead 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::DataBaseWrite(void)
{
//    rsp_length == 2. адрес slave + функция
    // (rsp_length - 2) - адрес slave.
    // (rsp_length - 1) - функция.
//        // (rsp_length) - номер блока.
    // (rsp_length + 1) - начало данных.

    std::cout << "CModbusSmSlave::DataBaseWrite 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint8_t uiBlockIndex = puiRequest[uiPduOffset + 1];

    std::cout << "CModbusSmSlave::DataBaseWrite uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSmSlave::DataBaseWrite uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CModbusSmSlave::DataBaseWrite uiBlockIndex "  << (int)uiBlockIndex << std::endl;


    if ((uiBlockIndex < 0) ||
            (uiBlockIndex > (CDataStore::MAX_BLOCKS_NUMBER - 1)))
    {
        std::cout << "CModbusSmSlave::DataBaseWrite 2" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {

        std::cout << "CModbusSmSlave::DataBaseRead 4" << std::endl;

        memcpy(m_puiIntermediateBuff,
               &puiRequest[uiPduOffset + 2],
               MAX_MODBUS_MESSAGE_LENGTH);

        m_pxDeviceControlLink ->
        GetDataContainerPointer() ->
        SetContainerData(CDeviceControl::DATA_BASE_BLOCK_START_WRITE,
                         uiBlockIndex,
                         m_puiIntermediateBuff,
                         0,
                         0);

        // отправим сообщение задача-исполнителю
        ((CDeviceControl*)(m_pxDeviceControlLink ->
                           GetTaskPerformerPointer())) ->
        SetCommandDataLink(m_pxDeviceControlLink);

        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        // номер блока базы данных
        puiResponse[uiPduOffset + 1] = 1;
        uiLength ++;
        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
        uiLength ++;

        // не ожидаем ответа
        m_uiLength = 0;
    }

    std::cout << "CModbusSmSlave::DataBaseWrite 7" << std::endl;
    return uiLength;
}


//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::RequestProcessing(void)
{
    std::cout << "CModbusSmSlave::RequestProcessing 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    std::cout << "CModbusSmSlave::RequestProcessing uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSmSlave::RequestProcessing uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    /* Filter on the Modbus unit identifier (slave) in RTU mode */
    if (uiSlave != m_uiOwnAddress && uiSlave != MODBUS_BROADCAST_ADDRESS)
    {
        std::cout << "CModbusSmSlave::RequestProcessing 2" << std::endl;
        return 0;
    }

    // не ожидаем ответа
    m_uiLength = 0;

    switch (uiFunctionCode)
    {
        std::cout << "CModbusSmSlave::RequestProcessing 3" << std::endl;
    case _FC_READ_COILS:
        std::cout << "CModbusSmSlave::RequestProcessing _FC_READ_COILS"  << std::endl;
        uiLength = ReadCoils();
        break;

    case _FC_READ_DISCRETE_INPUTS:
        uiLength = ReadDiscreteInputs();
        break;

    case _FC_READ_HOLDING_REGISTERS:
        uiLength = ReadHoldingRegisters();
        break;

    case _FC_READ_INPUT_REGISTERS:
        uiLength = ReadInputRegisters();
        break;

    case _FC_WRITE_SINGLE_COIL:
        uiLength = WriteSingleCoil();
        break;

    case _FC_WRITE_SINGLE_REGISTER:
        uiLength = WriteSingleRegister();
        break;

    case _FC_READ_EXCEPTION_STATUS:
        uiLength = ReadExceptionStatus();
        break;

    case _FC_WRITE_MULTIPLE_COILS:
        uiLength = WriteMultipleCoils();
        break;

    case _FC_PROGRAMMING_COMPLETION_REQUEST:
        uiLength = PollProgramming();
        break;

    case _FC_WRITE_MULTIPLE_REGISTERS:
        uiLength = WriteMultipleRegisters();
        break;

    case _FC_REPORT_SLAVE_ID:
        uiLength = ReportSlaveID();
        break;

    case _FC_WRITE_AND_READ_REGISTERS:
        uiLength = WriteAndReadRegisters();
        break;

    case _FC_DATA_EXCHANGE:
        break;

    case _FC_DATA_BASE_READ:
        uiLength = DataBaseRead();
        break;

    case _FC_DATA_BASE_WRITE:
        uiLength = DataBaseWrite();
        break;

    case _FC_PROGRAMMING:
        uiLength = Programming();
        break;

    default:
        std::cout << "CModbusSmSlave::RequestProcessing 4" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_FUNCTION,
                                     puiResponse);
        break;
    }

    std::cout << "CModbusSmSlave::RequestProcessing 5" << std::endl;
    uiLength = m_pxModbusSlaveLinkLayer -> Tail(puiResponse, uiLength);
    m_pxModbusSlaveLinkLayer -> SetFrameLength(uiLength);
    return uiLength;
}









//-------------------------------------------------------------------------------
////AnswerProcessing
////-------------------------------------------------------------------------------
//uint16_t CModbusSmSlave::ReadCoilsAnswer(void)
//{
//    std::cout << "CModbusSmSlave::ReadCoilsAnswer 1" << std::endl;
//
//    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
//    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
//    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
//    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();
//
//    int8_t uiSlave = puiRequest[uiPduOffset - 1];
//    int8_t uiFunctionCode = puiRequest[uiPduOffset];
//    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
//                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));
//
//    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
//                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));
//
//    std::cout << "CModbusSmSlave::ReadCoilsAnswer uiSlave "  << (int)uiSlave << std::endl;
//    std::cout << "CModbusSmSlave::ReadCoilsAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
//    std::cout << "CModbusSmSlave::ReadCoilsAnswer uiAddress "  << (int)uiAddress << std::endl;
//    std::cout << "CModbusSmSlave::ReadCoilsAnswer uiNumberB "  << (int)uiNumberB << std::endl;
//
//    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
//    {
//        std::cout << "CModbusSmSlave::ReadCoilsAnswer 2" << std::endl;
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
//                                     puiResponse);
//    }
//    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
//    {
//        std::cout << "CModbusSmSlave::ReadCoilsAnswer 3" << std::endl;
//        std::cout << "CModbusSmSlave::ReadCoilsAnswer m_uiCoilsNumber "  << (int)m_uiCoilsNumber << std::endl;
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
//                                     puiResponse);
//    }
//    else
//    {
//        std::cout << "CModbusSmSlave::ReadCoilsAnswer 4" << std::endl;
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
//
//        if (uiNumberB % 8)
//        {
//            std::cout << "CModbusSmSlave::ReadCoilsAnswer 5" << std::endl;
//            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
//        }
//        else
//        {
//            std::cout << "CModbusSmSlave::ReadCoilsAnswer 6" << std::endl;
//            puiResponse[uiLength++] = (uiNumberB / 8);
//        }
//        uiLength = ByteToBitPack(uiAddress,
//                                 uiNumberB,
//                                 m_puiCoils,
//                                 puiResponse,
//                                 uiLength);
//    }
//    std::cout << "CModbusSmSlave::ReadCoilsAnswer 7" << std::endl;
//    return uiLength;
//
//}
//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::ReadCoilsAnswer(void)
{
    std::cout << "CModbusSmSlave::ReadCoilsAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    std::cout << "CModbusSmSlave::ReadCoilsAnswer uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSmSlave::ReadCoilsAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CModbusSmSlave::ReadCoilsAnswer uiAddress "  << (int)uiAddress << std::endl;
    std::cout << "CModbusSmSlave::ReadCoilsAnswer uiNumberB "  << (int)uiNumberB << std::endl;

    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
    {
        std::cout << "CModbusSmSlave::ReadCoilsAnswer 2" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
    {
        std::cout << "CModbusSmSlave::ReadCoilsAnswer 3" << std::endl;
        std::cout << "CModbusSmSlave::ReadCoilsAnswer m_uiCoilsNumber "  << (int)m_uiCoilsNumber << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        std::cout << "CModbusSmSlave::ReadCoilsAnswer 4" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        if (uiNumberB % 8)
        {
            std::cout << "CModbusSmSlave::ReadCoilsAnswer 5" << std::endl;
            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
        }
        else
        {
            std::cout << "CModbusSmSlave::ReadCoilsAnswer 6" << std::endl;
            puiResponse[uiLength++] = (uiNumberB / 8);
        }
        uiLength = ByteToBitPack(uiAddress,
                                 uiNumberB,
                                 m_puiCoils,
                                 puiResponse,
                                 uiLength);
    }
    std::cout << "CModbusSmSlave::ReadCoilsAnswer 7" << std::endl;
    return uiLength;

}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::ReadDiscreteInputsAnswer(void)
{
    std::cout << "CModbusSmSlave::ReadDiscreteInputsAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiDiscreteInputsNumber)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        std::cout << "CModbusSmSlave::ReadDiscreteInputsAnswer 4" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        if (uiNumberB % 8)
        {
            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
        }
        else
        {
            puiResponse[uiLength++] = (uiNumberB / 8);
        }
        uiLength = ByteToBitPack(uiAddress,
                                 uiNumberB,
                                 m_puiDiscreteInputs,
                                 puiResponse,
                                 uiLength);
    }
    return uiLength;

}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::ReadHoldingRegistersAnswer(void)
{
    std::cout << "CModbusSmSlave::ReadHoldingRegistersAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    std::cout << "CModbusSmSlave::ReadHoldingRegistersAnswer uiAddress "  << (int)uiAddress << std::endl;
    std::cout << "CModbusSmSlave::ReadHoldingRegistersAnswer uiNumberB "  << (int)uiNumberB << std::endl;

    if (uiNumberB < 1 || MODBUS_MAX_READ_REGISTERS < uiNumberB)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiHoldingRegistersNumber)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        std::cout << "CModbusSmSlave::ReadHoldingRegistersAnswer 4" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        puiResponse[uiLength++] = (uiNumberB << 1);
        for (uint16_t i = 0; i < uiNumberB; i++)
        {
            int8_t uiData = 0;
            uiData = (static_cast<uint8_t>(m_puiHoldingRegisters[uiAddress] >> 8));
            uiData = (static_cast<uint8_t>(m_puiHoldingRegisters[uiAddress] & 0x00FF));
            puiResponse[uiLength++] = (static_cast<uint8_t>(m_puiHoldingRegisters[uiAddress] >> 8));
            puiResponse[uiLength++] = (static_cast<uint8_t>(m_puiHoldingRegisters[uiAddress] & 0x00FF));
            uiAddress++;
        }
    }

    std::cout << "CModbusSmSlave::ReadHoldingRegistersAnswer uiLength "  << (int)uiLength << std::endl;
    return uiLength;

}//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::ReadInputRegistersAnswer(void)
{
    std::cout << "CModbusSmSlave::ReadInputRegistersAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_READ_REGISTERS < uiNumberB)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiInputRegistersNumber)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
        puiResponse[uiLength++] = (uiNumberB << 1);
        for (uint16_t i = 0; i < uiNumberB; i++)
        {
            puiResponse[uiLength++] = (static_cast<uint8_t>(m_puiInputRegisters[uiAddress] >> 8));
            puiResponse[uiLength++] = (static_cast<uint8_t>(m_puiInputRegisters[uiAddress] & 0x00FF));
            uiAddress++;
        }
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::WriteSingleCoilAnswer(void)
{
    std::cout << "CModbusSmSlave::WriteSingleCoilAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    if (uiAddress >= m_uiCoilsNumber)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        uint16_t uiData = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                           (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

        if (uiData == 0xFF00 || uiData == 0x0)
        {
            if (uiData)
            {
                m_puiCoils[uiAddress] = 1;
            }
            else
            {
                m_puiCoils[uiAddress] = 0;
            }
            memcpy(puiResponse, puiRequest, uiLength);
        }
        else
        {
            uiLength = m_pxModbusSlaveLinkLayer ->
                       ResponseException(uiSlave,
                                         uiFunctionCode,
                                         MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                         puiResponse);
        }

    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::WriteSingleRegisterAnswer(void)
{
    std::cout << "CModbusSmSlave::WriteSingleRegisterAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    if (uiAddress >= m_uiHoldingRegistersNumber)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        m_puiHoldingRegisters[uiAddress] =
            ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
             (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));
        memcpy(puiResponse, puiRequest, uiLength);
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::WriteMultipleCoilsAnswer(void)
{
    std::cout << "CModbusSmSlave::WriteMultipleCoilsAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_WRITE_BITS < uiNumberB)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        SetBytesFromBits(m_puiCoils, uiAddress, uiNumberB, &puiRequest[uiPduOffset + 6]);

        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
        /* 4 to copy the bit address (2) and the quantity of bits */
        memcpy(puiResponse + uiLength, puiRequest + uiLength, 4);
        uiLength += 4;
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::WriteMultipleRegistersAnswer(void)
{
    std::cout << "CModbusSmSlave::WriteMultipleRegistersAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_WRITE_REGISTERS < uiNumberB)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiHoldingRegistersNumber)
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        for (uint16_t i = 0; i < uiNumberB; i++)
        {
            m_puiHoldingRegisters[uiAddress++] = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 6]) << 8) |
                                                  (static_cast<uint16_t>(puiRequest[uiPduOffset + 6 + 1])));
            uiPduOffset += 2;
        }

        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
        /* 4 to copy the bit address (2) and the quantity of bits */
        memcpy(puiResponse + uiLength, puiRequest + uiLength, 4);
        uiLength += 4;
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::ReadExceptionStatusAnswer(void)
{
//    errno = ENOPROTOOPT;
    return -1;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::WriteAndReadRegistersAnswer(void)
{

    return 0;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::ReportSlaveIDAnswer(void)
{
    std::cout << "CModbusSmSlave::ReportSlaveIDAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    std::cout << "CModbusSmSlave::ReportSlaveIDAnswer uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSmSlave::ReportSlaveIDAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    std::cout << "CModbusSmSlave::ReportSlaveIDAnswer 4" << std::endl;

    CDeviceControl* pxDeviceControl =
        (CDeviceControl*)GetResources() ->
        GetCommonTaskFromMapPointer("DeviceControlRtuUpperLevel");

    uiLength = pxDeviceControl ->
               ConfigurationRead(&puiResponse[uiPduOffset + 2]);
//    uiLength = m_pxResources ->
//               m_pxDeviceControl ->
//               ConfigurationRead(&puiResponse[uiPduOffset + 2]);

//    uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
//    memcpy(&puiResponse[uiPduOffset + 2], auiTempData, sizeof(auiTempData));
//    uiLength += sizeof(auiTempData);

    // количество байт в прикладном сообщении массиве конфигурации, не включая остальные.
    puiResponse[uiPduOffset + 1] = uiLength;//sizeof(auiTempData);// + 1;
    uiLength ++;
    uiLength += m_pxModbusSlaveLinkLayer ->
                ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

    std::cout << "CModbusSmSlave::ReportSlaveIDAnswer 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::ProgrammingAnswer(void)
{
    std::cout << "CModbusSmSlave::ProgrammingAnswer 1" << std::endl;
//// Функция 0x46 "программирование". Описание протокола обмена:
//// Buff[0] - адрес Slave.
//// Buff[1] - код функции.
//// Buff[2] - количество байт в запросе команды старший байт.
//// Buff[3] - количество байт в запросе команды младший байт.
//// Buff[4] - код операции(0 - запрос версии ПО, 1 - чтение блокаБД, 2 - запись блокаБД).
//// Buff[5] - номер блока БД.

    // Смещения в принятом буфере.
    enum
    {
        REQUEST_LENGTH = 2,
        REQUEST_COMMAND = 3,
        BLOCK_NUMBER = 4,
        DATA_BEGIN = 5,
    };

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

//    uint16_t uiNumberB = (static_cast<uint16_t>(puiRequest[uiPduOffset + REQUEST_LENGTH]));

//    if (uiNumberB != (uiLength - 6))
//    {
//        uiLength = m_pxModbusSlaveLinkLayer ->
//    ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
//                                     puiResponse);
//    }
//    else
//    {
//        uint8_t uiBlockNumber;
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
//        // Какой код операции?
//        switch(puiRequest[uiPduOffset + REQUEST_COMMAND])
//        {
//        case 0x00:
//            // Получение версии ПО.
//            puiResponse[uiLength++] = 0;
//            puiResponse[uiLength++] = 4;
//            puiResponse[uiLength++] = puiRequest[uiPduOffset + REQUEST_COMMAND];
//            puiResponse[uiLength++] = 2; // 1.7.01
//            puiResponse[uiLength++] = 0;
//            puiResponse[uiLength++] = 0x02;
//            break;
//
//        case 0x01:
//            // Чтение блока БД.
//            uiBlockNumber = puiRequest[uiPduOffset + BLOCK_NUMBER];
//            // Блок не существует?
//            if (uiBlockNumber > CDataBase::BLOCKS_QUANTITY)
//            {
//                uiLength = m_pxModbusSlaveLinkLayer ->
//                      ResponseException(uiSlave,
//                                             uiFunctionCode,
//                                             MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
//                                             puiResponse);
//            }
//            else
//            {
//                // Прочитаем блок БД.
//                uint16_t uiLengthLocal = CDataBase::Read(&puiResponse[uiPduOffset + DATA_BEGIN], uiBlockNumber);
//                puiResponse[uiLength++] = 0;
//                puiResponse[uiLength++] = uiLengthLocal;
//                puiResponse[uiLength++] = puiRequest[uiPduOffset + REQUEST_COMMAND];
//                puiResponse[uiLength++] = uiBlockNumber;
//                uiLength += uiLengthLocal;
//            }
//            break;
//
//
//        case 0x02:
//            // Запись блока БД.
//            CDataBase::SetStatus(0);
//            uiBlockNumber = puiRequest[uiPduOffset + BLOCK_NUMBER];
//            // Блок не существует?
//            if (uiBlockNumber > CDataBase::BLOCKS_QUANTITY)
//            {
//                uiLength = m_pxModbusSlaveLinkLayer ->
//                                        ResponseException(uiSlave,
//                                             uiFunctionCode,
//                                             MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
//                                             puiResponse);
//            }
//            // Размер блока не соответствует?
//            else if ((puiRequest[uiPduOffset + REQUEST_LENGTH] - 2) != CDataBase::m_DStruct[uiBlockNumber].Size)
//            {
//                uiLength = m_pxModbusSlaveLinkLayer ->
//                                                ResponseException(uiSlave,
//    uiFunctionCode,
//    MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
//    puiResponse);
//            }
//            else
//            {
//                // Сохраним блок БД.
//                // Успешно?
//                if (CDataBase::Write(&puiRequest[uiPduOffset + DATA_BEGIN], uiBlockNumber))
//                {
//                    CDataBase::SetStatus(1);
//
//                    uiLength = m_pxModbusSlaveLinkLayer ->
//                                                        ResponseException(uiSlave,
//    uiFunctionCode,
//    MODBUS_EXCEPTION_ACKNOWLEDGE,
//    puiResponse);
//                }
//                else
//                {
//                    CDataBase::SetStatus(0);
//
//                    uiLength = m_pxModbusSlaveLinkLayer ->
//                                                                ResponseException(uiSlave,
//    uiFunctionCode,
//    MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE,
//    puiResponse);
//                }
//            }
//            break;
//
//        case 0x03:
//            if(!(CDataBase::GetStatus()))
//            {
//                uiLength = m_pxModbusSlaveLinkLayer ->
//                                                                        ResponseException(uiSlave,
//    uiFunctionCode,
//    MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
//    puiResponse);
//            }
//            break;
//
//        default:
//            uiLength = m_pxModbusSlaveLinkLayer ->
    //                                                                                ResponseException(uiSlave,
    //    uiFunctionCode,
//    MODBUS_EXCEPTION_ILLEGAL_FUNCTION,
//    puiResponse);
//            break;
//        };
//    }

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::PollProgrammingAnswer(void)
{
    std::cout << "CModbusSmSlave::PollProgrammingAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

//    CDeviceControl* pxDeviceControl =
//        (CDeviceControl*)GetResources() ->
//        GetCommonTaskFromMapPointer("DeviceControl");

    CDataStore* pxDataStoreFileSystemAnswer =
        (CDataStore*)GetResources() ->
        GetCommonTaskFromMapPointer("DataStoreFileSystemAnswer");


//    // Устройство хранения в прцессе записи?
//    if (m_pxDeviceControl ->
//            GetFsmOperationStatus() == CDfa::IN_PROGRESS)
//    {
//        std::cout << "CModbusSmSlave::PollProgrammingAnswer 2" << std::endl;
//
//        uiLength = m_pxModbusSlaveLinkLayer ->
//    ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
//                                     puiResponse);
//    }
//    // Устройство хранения закончило запись успешно?
//    else if (m_pxDeviceControl ->
//             GetFsmOperationStatus() == CDfa::DONE_SUCCESSFULLY)
//    {
//        std::cout << "CModbusSmSlave::PollProgrammingAnswer 3" << std::endl;
//
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
//
//    }
//    // Устройство хранения закончило запись не успешно?
//    else if (m_pxDeviceControl ->
//             GetFsmOperationStatus() == CDfa::ERROR_OCCURED)
//    {
//        std::cout << "CModbusSmSlave::PollProgrammingAnswer 4" << std::endl;
//
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                      ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ACKNOWLEDGE,
//                                     puiResponse);
//    }

    // Ожидаем окончания записи автоматом устройства хранения.
    // Устройство хранения закончило запись успешно?
    if (pxDataStoreFileSystemAnswer -> IsDoneOk())
    {
        std::cout << "CModbusSmSlave::PollProgrammingAnswer 2" << std::endl;

        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

//        // номер блока базы данных
//        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
//        uiLength ++;

//        uiLength = m_pxModbusSlaveLinkLayer ->
//        ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ACKNOWLEDGE,
//                                     puiResponse);
    }
//    // Устройство хранения закончило запись не успешно?
//    else if (pxDataStoreFileSystemAnswer -> IsDoneError())
//    {
//        uiLength = m_pxModbusSlaveLinkLayer ->
    //    ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
//                                     puiResponse);
//    }
    else
    {
        std::cout << "CModbusSmSlave::PollProgrammingAnswer 3" << std::endl;

//        uiLength = m_pxModbusSlaveLinkLayer ->
//                    ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
                                     puiResponse);
    }

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::DataBaseReadAnswer(void)
{
    std::cout << "CModbusSmSlave::DataBaseReadAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint8_t uiBlockIndex = puiRequest[uiPduOffset + 1];

    std::cout << "CModbusSmSlave::DataBaseReadAnswer uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSmSlave::DataBaseReadAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CModbusSmSlave::DataBaseReadAnswer uiBlockIndex "  << (int)uiBlockIndex << std::endl;

    if ((uiBlockIndex < 0) ||
            (uiBlockIndex > (CDataStore::MAX_BLOCKS_NUMBER - 1)))
    {
        std::cout << "CModbusSmSlave::DataBaseReadAnswer 2" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else
    {
        uiLength = m_pxDeviceControlLink ->
                   GetDataContainerPointer() ->
                   GetDataLength();
        std::cout << "CModbusSmSlave::DataBaseReadAnswer uiLength "  << (int)uiLength << std::endl;

        memcpy(&puiResponse[uiPduOffset + 3], m_puiIntermediateBuff, uiLength);

        puiResponse[uiPduOffset + 1] = uiLength + 1;
        uiLength ++;

        uiLength += m_pxModbusSlaveLinkLayer ->
                    ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        // номер блока базы данных
        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
        uiLength ++;
    }

    std::cout << "CModbusSmSlave::DataBaseReadAnswer 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::DataBaseWriteAnswer(void)
{
    std::cout << "CModbusSmSlave::DataBaseWriteAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint8_t uiBlockIndex = puiRequest[uiPduOffset + 1];

    std::cout << "CModbusSmSlave::DataBaseWriteAnswer uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSmSlave::DataBaseWriteAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CModbusSmSlave::DataBaseWriteAnswer uiBlockIndex "  << (int)uiBlockIndex << std::endl;


    if ((uiBlockIndex < 0) ||
            (uiBlockIndex > (CDataStore::MAX_BLOCKS_NUMBER - 1)))
    {
        std::cout << "CModbusSmSlave::DataBaseWriteAnswer 2" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        std::cout << "CModbusSmSlave::DataBaseWriteAnswer 3" << std::endl;
        //        CDataStore* pxDataStoreFileSystem =
//            (CDataStore*)GetResources() ->
//            GetCommonTaskFromMapPointer("DataStoreFileSystemAnswer");
//
//        uint16_t uiLength = pxDataStoreFileSystem ->
//                            GetBlockLength(uiBlockIndex);
//
//        memcpy(m_puiIntermediateBuff,
//               &puiRequest[uiPduOffset + 2],
//               uiLength);
//
//        // Сохраним блок БД.
//        std::cout << "CModbusSmSlave::DataBaseWriteAnswer 3" << std::endl;
//        pxDataStoreFileSystem ->
//        WriteBlock(m_puiIntermediateBuff,
//                   uiLength,
//                   uiBlockIndex);

        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        // номер блока базы данных
        puiResponse[uiPduOffset + 1] = 1;
        uiLength ++;
        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
        uiLength ++;
    }

    std::cout << "CModbusSmSlave::DataBaseWriteAnswer 7" << std::endl;
    return uiLength;
}

////-------------------------------------------------------------------------------
//uint16_t CModbusSmSlave::AnswerProcessing(void)
//{
//    std::cout << "CModbusSmSlave::AnswerProcessing 1" << std::endl;
//
//    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
//    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
//    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
//    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();
//
//    int8_t uiSlave = puiRequest[uiPduOffset - 1];
//    int8_t uiFunctionCode = puiRequest[uiPduOffset];
//
//    std::cout << "CModbusSmSlave::AnswerProcessing uiSlave "  << (int)uiSlave << std::endl;
//    std::cout << "CModbusSmSlave::AnswerProcessing uiFunctionCode "  << (int)uiFunctionCode << std::endl;
//
//    /* Filter on the Modbus unit identifier (slave) in RTU mode */
//    if (uiSlave != m_uiOwnAddress && uiSlave != MODBUS_BROADCAST_ADDRESS)
//    {
//        std::cout << "CModbusSmSlave::AnswerProcessing 2" << std::endl;
//        return 0;
//    }
//
//    switch (uiFunctionCode)
//    {
//        std::cout << "CModbusSmSlave::AnswerProcessing 3" << std::endl;
//    case _FC_READ_COILS:
//        std::cout << "CModbusSmSlave::AnswerProcessing _FC_READ_COILS"  << std::endl;
//        uiLength = ReadCoils();
//        break;
//
//    case _FC_READ_DISCRETE_INPUTS:
//        uiLength = ReadDiscreteInputs();
//        break;
//
//    case _FC_READ_HOLDING_REGISTERS:
//        uiLength = ReadHoldingRegisters();
//        break;
//
//    case _FC_READ_INPUT_REGISTERS:
//        uiLength = ReadInputRegisters();
//        break;
//
//    case _FC_WRITE_SINGLE_COIL:
//        uiLength = WriteSingleCoil();
//        break;
//
//    case _FC_WRITE_SINGLE_REGISTER:
//        uiLength = WriteSingleRegister();
//        break;
//
//    case _FC_READ_EXCEPTION_STATUS:
//        uiLength = ReadExceptionStatus();
//        break;
//
//    case _FC_WRITE_MULTIPLE_COILS:
//        uiLength = WriteMultipleCoils();
//        break;
//
//    case _FC_PROGRAMMING_COMPLETION_REQUEST:
//        uiLength = PollProgramming();
//        break;
//
//    case _FC_WRITE_MULTIPLE_REGISTERS:
//        uiLength = WriteMultipleRegisters();
//        break;
//
//    case _FC_REPORT_SLAVE_ID:
//        uiLength = ReportSlaveID();
//        break;
//
//    case _FC_WRITE_AND_READ_REGISTERS:
//        uiLength = WriteAndReadRegisters();
//        break;
//
//    case _FC_DATA_EXCHANGE:
//        break;
//
//    case _FC_DATA_BASE_READ:
//        uiLength = DataBaseRead();
//        break;
//
//    case _FC_DATA_BASE_WRITE:
//        uiLength = DataBaseWrite();
//        break;
//
//    case _FC_PROGRAMMING:
//        uiLength = Programming();
//        break;
//
//    default:
//        std::cout << "CModbusSmSlave::AnswerProcessing 4" << std::endl;
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_FUNCTION,
//                                     puiResponse);
//        break;
//    }
//
//    std::cout << "CModbusSmSlave::AnswerProcessing 5" << std::endl;
//    uiLength = m_pxModbusSlaveLinkLayer -> Tail(puiResponse, uiLength);
//    m_pxModbusSlaveLinkLayer -> SetFrameLength(uiLength);
//    return uiLength;
//}
//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::AnswerProcessing(void)
{
    std::cout << "CModbusSmSlave::AnswerProcessing 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    std::cout << "CModbusSmSlave::AnswerProcessing uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSmSlave::AnswerProcessing uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    /* Filter on the Modbus unit identifier (slave) in RTU mode */
    if (uiSlave != m_uiOwnAddress && uiSlave != MODBUS_BROADCAST_ADDRESS)
    {
        std::cout << "CModbusSmSlave::AnswerProcessing 2" << std::endl;
        return 0;
    }

    // проверяем сохранённый локально текущий код функции.
    switch (m_uiFunctionCode)
    {
        std::cout << "CModbusSmSlave::AnswerProcessing 3" << std::endl;
    case _FC_READ_COILS:
        std::cout << "CModbusSmSlave::Answer _FC_READ_COILS"  << std::endl;
        uiLength = ReadCoilsAnswer();
        break;

    case _FC_READ_DISCRETE_INPUTS:
        uiLength = ReadDiscreteInputsAnswer();
        break;

    case _FC_READ_HOLDING_REGISTERS:
        uiLength = ReadHoldingRegistersAnswer();
        break;

    case _FC_READ_INPUT_REGISTERS:
        uiLength = ReadInputRegistersAnswer();
        break;

    case _FC_WRITE_SINGLE_COIL:
        uiLength = WriteSingleCoilAnswer();
        break;

    case _FC_WRITE_SINGLE_REGISTER:
        uiLength = WriteSingleRegisterAnswer();
        break;

    case _FC_READ_EXCEPTION_STATUS:
        uiLength = ReadExceptionStatusAnswer();
        break;

    case _FC_WRITE_MULTIPLE_COILS:
        uiLength = WriteMultipleCoilsAnswer();
        break;

    case _FC_PROGRAMMING_COMPLETION_REQUEST:
        uiLength = PollProgrammingAnswer();
        break;

    case _FC_WRITE_MULTIPLE_REGISTERS:
        uiLength = WriteMultipleRegistersAnswer();
        break;

    case _FC_REPORT_SLAVE_ID:
        uiLength = ReportSlaveIDAnswer();
        break;

    case _FC_WRITE_AND_READ_REGISTERS:
        uiLength = WriteAndReadRegistersAnswer();
        break;

    case _FC_DATA_EXCHANGE:
        break;

    case _FC_DATA_BASE_READ:
        uiLength = DataBaseReadAnswer();
        break;

    case _FC_DATA_BASE_WRITE:
        uiLength = DataBaseWriteAnswer();
        break;

    case _FC_PROGRAMMING:
        uiLength = ProgrammingAnswer();
        break;

    default:
        std::cout << "CModbusSmSlave::AnswerProcessing 4" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_FUNCTION,
                                     puiResponse);
        break;
    }

    std::cout << "CModbusSmSlave::AnswerProcessing 5" << std::endl;
    uiLength = m_pxModbusSlaveLinkLayer -> Tail(puiResponse, uiLength);
    m_pxModbusSlaveLinkLayer -> SetFrameLength(uiLength);
    return uiLength;
}

//-------------------------------------------------------------------------------
void CModbusSmSlave::SetByteFromBits(uint8_t *dest, uint16_t index, const uint8_t value)
{
    uint16_t i;

    for (i=0; i<8; i++)
    {
        dest[index+i] = (value & (1 << i)) ? 1 : 0;
    }
}

//-------------------------------------------------------------------------------
void CModbusSmSlave::SetBytesFromBits(uint8_t *dest, uint16_t index, uint16_t nb_bits,
                                    const uint8_t *tab_byte)
{
    uint16_t i;
    uint16_t shift = 0;

    for (i = index; i < index + nb_bits; i++)
    {
        dest[i] = tab_byte[(i - index) / 8] & (1 << shift) ? 1 : 0;
        /* gcc doesn't like: shift = (++shift) % 8; */
        shift++;
        shift %= 8;
    }
}

//-------------------------------------------------------------------------------
uint8_t CModbusSmSlave::GetByteFromBits(const uint8_t *src, uint16_t index,
                                      uint16_t nb_bits)
{
    uint16_t i;
    uint8_t value = 0;

    if (nb_bits > 8)
    {
        /* Assert is ignored if NDEBUG is set */
//        assert(nb_bits < 8); //?
        nb_bits = 8;
    }

    for (i=0; i < nb_bits; i++)
    {
        value |= (src[index+i] << i);
    }

    return value;
}

//-------------------------------------------------------------------------------
float CModbusSmSlave::GetFloat(const uint16_t *src)
{
    float f = 0.0f;
    uint32_t i;

    i = (((uint32_t)src[1]) << 16) + src[0];
    memcpy(&f, &i, sizeof(float));

    return f;
}

//-------------------------------------------------------------------------------
void CModbusSmSlave::SetFloat(float f, uint16_t *dest)
{
    uint32_t i = 0;

    memcpy(&i, &f, sizeof(uint32_t));
    dest[0] = (uint16_t)i;
    dest[1] = (uint16_t)(i >> 16);
}

//-------------------------------------------------------------------------------
uint8_t CModbusSmSlave::Fsm(void)
{
//    std::cout << "CModbusSmSlave::Fsm 1" << endl;
    uint8_t uiReadyTaskCounter = 0;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CModbusSmSlave::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CModbusSmSlave::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CModbusSmSlave::Fsm START"  << std::endl;
        std::cout << "CModbusSmSlave::Fsm m_sModbusSlaveLinkLayerName" << " " << (m_sModbusSlaveLinkLayerName) << std::endl;
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:

        std::cout << "CModbusSmSlave::Fsm INIT 1"  << std::endl;
        {
            CTaskInterface* pxTask =
                GetResources() ->
                GetCommonTaskFromMapPointer(m_sModbusSlaveLinkLayerName);

            if (pxTask != 0)
            {
                std::cout << "CModbusSmSlave::Fsm INIT 2"  << std::endl;
                if (pxTask -> GetFsmState() >= READY)
                {
                    std::cout << "CModbusSmSlave::Fsm INIT 3"  << std::endl;
                    SetModbusSlaveLinkLayer((CModbusSmSlaveLinkLayer*)pxTask);
                    uiReadyTaskCounter += 1;
//                    SetFsmState(READY);
                }
            }
            else
            {
                std::cout << "CModbusSmSlave::Fsm INIT 4"  << std::endl;
                if (GetTimerPointer() -> IsOverflow())
                {
                    SetFsmState(STOP);
                }
            }
        }


        {
            if (GetDeviceControlLink() == 0)
            {
                std::cout << "CModbusSmSlave::Fsm INIT 5"  << std::endl;
                CLinkInterface* pxLink = GetResources() ->
                                         CreateLinkByPerformerName(m_sDeviceControlName);
                if (pxLink != 0)
                {
                    std::cout << "CModbusSmSlave::Fsm INIT 6"  << std::endl;
                    SetDeviceControlLink(pxLink);
                    CDataContainerDataBase* pxDataContainerDataBase = new CDataContainerDataBase();
                    pxLink ->
                    SetDataContainer(pxDataContainerDataBase);

//                    auto pxDataContainer = std::make_shared<CDataContainerDataBase>();
//                    pxLink ->
//                    SetDataContainer(pxDataContainer.get());
                    pxLink ->
                    GetDataContainerPointer() ->
                    SetContainerData(0,
                                     0,
                                     0,
                                     0,
                                     0);
                }
                else
                {
                    std::cout << "CModbusSmSlave::Fsm INIT 7"  << std::endl;
                    if (GetTimerPointer() -> IsOverflow())
                    {
                        SetFsmState(STOP);
                    }
                }
            }
            else
            {
                std::cout << "CModbusSmSlave::Fsm INIT 8"  << std::endl;
                uiReadyTaskCounter += 1;
            }
        }

        if (uiReadyTaskCounter > 1)
        {
            SetFsmState(READY);
        }
        break;

//    case READY_WAITING:
//        std::cout << "CModbusSmSlave::Fsm READY_WAITING"  << std::endl;
//        SetFsmState(READY);
//        break;

    case READY:
        std::cout << "CModbusSmSlave::Fsm READY"  << std::endl;
//        {
//            if (m_pxCommandDataContainer != 0)
//            {
//                std::cout << "CModbusSmSlave::Fsm READY 2"  << std::endl;
//                m_pxOperatingDataContainer = m_pxCommandDataContainer;
//                SetFsmState(GetFsmCommandState());
//                SetFsmCommandState(0);
//                m_pxCommandDataContainer = 0;
//            }
//        }


//        {
//            if (GetFsmCommandState() != 0)
//            {
//                SetFsmState(GetFsmCommandState());
//                SetFsmCommandState(0);
//            }
//        }
        SetFsmState(COMMUNICATION_START);
        break;

    case DONE_OK:
        std::cout << "CModbusSmSlave::Fsm DONE_OK"  << std::endl;
        SetFsmOperationStatus(DONE_OK);
        SetFsmState(READY);
        break;

    case DONE_ERROR:
        std::cout << "CModbusSmSlave::Fsm DONE_ERROR"  << std::endl;
        SetFsmOperationStatus(DONE_ERROR);
        SetFsmState(READY);
        break;

    case COMMUNICATION_START:
        std::cout << "CModbusSmSlave::Fsm COMMUNICATION_START"  << std::endl;
        m_pxModbusSlaveLinkLayer ->
        CommunicationStart();
        SetFsmState(MESSAGE_RECEIVE_WAITING);
        break;

    case COMMUNICATION_RECEIVE_START:
        std::cout << "CModbusSmSlave::Fsm COMMUNICATION_RECEIVE_START"  << std::endl;
        m_pxModbusSlaveLinkLayer ->
        CommunicationReceiveStart();
        SetFsmState(MESSAGE_RECEIVE_WAITING);
        break;

    case MESSAGE_RECEIVE_WAITING:
//        std::cout << "CModbusSmSlave::Fsm MESSAGE_RECEIVE_WAITING"  << std::endl;
        if (m_pxModbusSlaveLinkLayer -> IsDoneOk())
        {
            std::cout << "CModbusSmSlave::Fsm MESSAGE_RECEIVE_WAITING 2"  << std::endl;
            SetFsmState(REQUEST_PROCESSING);
        }
        else if (m_pxModbusSlaveLinkLayer -> IsDoneError())
        {
            std::cout << "CModbusSmSlave::Fsm MESSAGE_RECEIVE_WAITING 3"  << std::endl;
            SetFsmState(COMMUNICATION_START);
        }
        break;

    case REQUEST_PROCESSING:
        std::cout << "CModbusSmSlave::Fsm REQUEST_PROCESSING"  << std::endl;
        if (RequestProcessing())
        {
            //            GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
//            SetFsmState(BEFORE_ANSWERING_WAITING);
        }
        else
        {
            SetFsmState(MESSAGE_RECEIVE_WAITING);
            break;
        }

//        RequestProcessing();
//            SetFsmState(BEFORE_ANSWERING_WAITING);

//        if (m_pxDeviceControlLink ->
//                GetDataContainerPointer() ->
//                GetFsmCommandState())
        if (m_uiLength)
        {
            std::cout << "CModbusSmSlave::Fsm REQUEST_PROCESSING 2"  << std::endl;
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(ANSWER_PROCESSING_WAITING);
//            SetFsmState(BEFORE_ANSWERING_WAITING);
        }
        else
        {
            std::cout << "CModbusSmSlave::Fsm REQUEST_PROCESSING 3"  << std::endl;
            GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
            SetFsmState(BEFORE_ANSWERING_WAITING);
        }
        break;

    case ANSWER_PROCESSING_WAITING:
//        std::cout << "CModbusSmSlave::Fsm ANSWER_PROCESSING_WAITING"  << std::endl;
//        GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
//        SetFsmState(ANSWER_PROCESSING);

        if (m_pxDeviceControlLink ->
                GetTaskPerformerPointer() ->
                IsDoneOk())
        {
            std::cout << "CModbusSmSlave::Fsm ANSWER_PROCESSING_WAITING 2"  << std::endl;
            GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
            SetFsmState(ANSWER_PROCESSING);
        }
        else if (m_pxDeviceControlLink ->
                 GetTaskPerformerPointer() ->
                 IsDoneError())
        {
            std::cout << "CModbusSmSlave::Fsm ANSWER_PROCESSING_WAITING 3"  << std::endl;
            GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
            SetFsmState(MESSAGE_RECEIVE_WAITING);
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CModbusSmSlave::Fsm ANSWER_PROCESSING_WAITING 4"  << std::endl;
                m_pxDeviceControlLink ->
                GetDataContainerPointer() ->
                SetFsmCommandState(0);
                SetFsmState(MESSAGE_RECEIVE_WAITING);
            }
        }
        break;

    case ANSWER_PROCESSING:
        std::cout << "CModbusSmSlave::Fsm ANSWER_PROCESSING"  << std::endl;
        if (AnswerProcessing())
        {
            GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
            SetFsmState(BEFORE_ANSWERING_WAITING);
        }
        else
        {
            SetFsmState(MESSAGE_RECEIVE_WAITING);
            break;
        }

//        RequestProcessing();
//            SetFsmState(BEFORE_ANSWERING_WAITING);

//        if (m_pxDeviceControlLink ->
//                GetDataContainerPointer() ->
//                GetFsmCommandState())
//        {
//            std::cout << "CModbusSmSlave::Fsm ANSWER_PROCESSING 2"  << std::endl;
//            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
//            SetFsmState(ANSWER_PROCESSING_WAITING);
////            SetFsmState(BEFORE_ANSWERING_WAITING);
//        }
//        else
//        {
//            std::cout << "CModbusSmSlave::Fsm ANSWER_PROCESSING 3"  << std::endl;
//            GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
//            SetFsmState(BEFORE_ANSWERING_WAITING);
//        }
        break;

    case BEFORE_ANSWERING_WAITING:
//        std::cout << "CModbusSmSlave::Fsm BEFORE_ANSWERING_WAITING"  << std::endl;
        // Закончилось время паузы между приёмом и передачей(5 милисекунд)?
        if (GetTimerPointer() -> IsOverflow())
        {
//            GetTimerPointer() -> Set(m_uiConfirmationTimeout);
            //            m_pxModbusSlaveLinkLayer ->
//            SetFsmState(CModbusSmSlaveLinkLayerInterface::COMMUNICATION_TRANSMIT_START);
            m_pxModbusSlaveLinkLayer ->
            TransmitStart();
            SetFsmState(AFTER_ANSWERING_WAITING);
        }
        break;

    case MESSAGE_SEND:
        std::cout << "CModbusSmSlave::Fsm MESSAGE_SEND"  << std::endl;
        SetFsmState(MESSAGE_RECEIVE_WAITING);
        break;

    case AFTER_ANSWERING_WAITING:
        std::cout << "CModbusSmSlave::Fsm AFTER_ANSWERING_WAITING"  << std::endl;
        if (m_pxModbusSlaveLinkLayer -> IsDoneOk())
        {
            std::cout << "CModbusSmSlave::Fsm AFTER_ANSWERING_WAITING 2"  << std::endl;
            m_pxModbusSlaveLinkLayer ->
            CommunicationReceiveStart();
            SetFsmState(MESSAGE_RECEIVE_WAITING);
        }
        else if (m_pxModbusSlaveLinkLayer -> IsDoneError())
        {
            std::cout << "CModbusSmSlave::Fsm AFTER_ANSWERING_WAITING 3"  << std::endl;
            SetFsmState(COMMUNICATION_START);
        }
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------

