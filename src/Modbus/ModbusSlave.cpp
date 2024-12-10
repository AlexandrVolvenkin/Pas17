//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
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
#include "ModbusSlave.h"

using namespace std;

//-------------------------------------------------------------------------------
CModbusSlave::CModbusSlave()
{
    std::cout << "CModbusSlave constructor"  << std::endl;
    m_pxModbusSlaveLinkLayer = 0;
    // получим имя класса.
    sprintf(GetTaskNamePointer(),
            "%s",
            typeid(*this).name());
    m_puiIntermediateBuff = new uint8_t[MAX_MODBUS_MESSAGE_LENGTH];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CModbusSlave::~CModbusSlave()
{
    delete[] m_puiIntermediateBuff;
    WorkingArraysDelete();
}

//-------------------------------------------------------------------------------
void CModbusSlave::SetModbusSlaveLinkLayerName(std::string sName)
{
    m_sModbusSlaveLinkLayerName = sName;
}

//-------------------------------------------------------------------------------
void CModbusSlave::SetModbusSlaveLinkLayer(CModbusSlaveLinkLayer* pxModbusSlaveLinkLayer)
{
    m_pxModbusSlaveLinkLayer = pxModbusSlaveLinkLayer;
}

//-------------------------------------------------------------------------------
void CModbusSlave::SetDeviceControlName(std::string sName)
{
    m_sDeviceControlName = sName;
}

//-------------------------------------------------------------------------------
void CModbusSlave::SetDeviceControl(CDeviceControl* pxDeviceControl)
{
    m_pxDeviceControl = pxDeviceControl;
}

//-------------------------------------------------------------------------------
void CModbusSlave::SetDeviceControlLinkName(const std::string sName)
{
    m_sDeviceControlLinkName = sName;
}

////-------------------------------------------------------------------------------
//std::string CModbusSlave::GetDeviceControlLinkName() const
//{
//    return m_sDeviceControlLinkName;
//}

//-------------------------------------------------------------------------------
void CModbusSlave::SetDeviceControlLink(CLinkInterface* pxLink)
{
    m_pxDeviceControlLink = pxLink;
}

////-------------------------------------------------------------------------------
//CLinkInterface* CModbusSlave::GetDeviceControlLink() const
//{
//    return m_pxDeviceControlLink;
//}

//-------------------------------------------------------------------------------
const char *CModbusSlave::ModbusStringError(int errnum)
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
void CModbusSlave::ModbusWorkingArraysInit(void)
{
    std::cout << "CModbusSlave ModbusWorkingArraysInit 1"  << std::endl;
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
void CModbusSlave::WorkingArraysInit(uint8_t *puiCoils,
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
void CModbusSlave::WorkingArraysCreate(uint16_t uiCoilsNumber,
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
void CModbusSlave::WorkingArraysDelete(void)
{
    delete m_puiCoils;
    delete m_puiDiscreteInputs;
    delete m_puiHoldingRegisters;
    delete m_puiInputRegisters;
}

////-------------------------------------------------------------------------------
//void CModbusSlave::SlaveSet(uint8_t uiSlave)
//{
//    m_uiOwnAddress = uiSlave;
//}

//-------------------------------------------------------------------------------
/* Builds a TCP request header */
uint16_t CModbusSlave::RequestBasis(uint8_t uiSlave,
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
uint16_t CModbusSlave::ResponseBasis(uint8_t uiSlave, uint8_t uiFunctionCode, uint8_t *puiResponse)
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
//int8_t CModbusSlave::MessengerIsReady(void)
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
//uint16_t CModbusSlave::SendMessage(uint8_t *puiMessage, uint16_t uiLength)
//{
//    uiLength = Tail(puiMessage, uiLength);
//    return Send(puiMessage, uiLength);
//}

//-------------------------------------------------------------------------------
/* Build the exception response */
uint16_t CModbusSlave::ResponseException(uint8_t uiSlave, uint8_t uiFunctionCode, uint8_t uiExceptionCode, uint8_t *puiResponse)
{
    uint16_t uiLength;

    uiLength = m_pxModbusSlaveLinkLayer ->
               ResponseBasis(uiSlave, (uiFunctionCode | 0x80), puiResponse);
    /* Positive exception code */
    puiResponse[uiLength++] = uiExceptionCode;

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ByteToBitPack(uint16_t uiAddress,
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

////-------------------------------------------------------------------------------
//uint16_t CModbusSlave::ReadCoils(void)
//{
//    std::cout << "CModbusSlave::ReadCoils 1" << std::endl;
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
//    std::cout << "CModbusSlave::ReadCoils uiSlave "  << (int)uiSlave << std::endl;
//    std::cout << "CModbusSlave::ReadCoils uiFunctionCode "  << (int)uiFunctionCode << std::endl;
//    std::cout << "CModbusSlave::ReadCoils uiAddress "  << (int)uiAddress << std::endl;
//    std::cout << "CModbusSlave::ReadCoils uiNumberB "  << (int)uiNumberB << std::endl;
//
//    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
//    {
//        std::cout << "CModbusSlave::ReadCoils 2" << std::endl;
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
//                                     puiResponse);
//    }
//    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
//    {
//        std::cout << "CModbusSlave::ReadCoils 3" << std::endl;
//        std::cout << "CModbusSlave::ReadCoils m_uiCoilsNumber "  << (int)m_uiCoilsNumber << std::endl;
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
//                                     puiResponse);
//    }
//    else
//    {
//        std::cout << "CModbusSlave::ReadCoils 4" << std::endl;
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
//
//        if (uiNumberB % 8)
//        {
//            std::cout << "CModbusSlave::ReadCoils 5" << std::endl;
//            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
//        }
//        else
//        {
//            std::cout << "CModbusSlave::ReadCoils 6" << std::endl;
//            puiResponse[uiLength++] = (uiNumberB / 8);
//        }
//        uiLength = ByteToBitPack(uiAddress,
//                                 uiNumberB,
//                                 m_puiCoils,
//                                 puiResponse,
//                                 uiLength);
//    }
//    std::cout << "CModbusSlave::ReadCoils 7" << std::endl;
//    return uiLength;
//}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadCoils(void)
{
    std::cout << "CModbusSlave::ReadCoils 1" << std::endl;

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

    std::cout << "CModbusSlave::ReadCoils uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSlave::ReadCoils uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CModbusSlave::ReadCoils uiAddress "  << (int)uiAddress << std::endl;
    std::cout << "CModbusSlave::ReadCoils uiNumberB "  << (int)uiNumberB << std::endl;

    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
    {
        std::cout << "CModbusSlave::ReadCoils 2" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
    {
        std::cout << "CModbusSlave::ReadCoils 3" << std::endl;
        std::cout << "CModbusSlave::ReadCoils m_uiCoilsNumber "  << (int)m_uiCoilsNumber << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        std::cout << "CModbusSlave::ReadCoils 4" << std::endl;

        m_uiFunctionCode = uiFunctionCode;

        m_pxDeviceControlLink ->
        GetDataContainerPointer() ->
        SetContainerData(uiFunctionCode,
                         0,
                         &puiResponse[uiPduOffset + 3],
                         0,
                         0);





        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

//        if (uiNumberB % 8)
//        {
//            std::cout << "CModbusSlave::ReadCoils 5" << std::endl;
//            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
//        }
//        else
//        {
//            std::cout << "CModbusSlave::ReadCoils 6" << std::endl;
//            puiResponse[uiLength++] = (uiNumberB / 8);
//        }
//        uiLength = ByteToBitPack(uiAddress,
//                                 uiNumberB,
//                                 m_puiCoils,
//                                 puiResponse,
//                                 uiLength);
    }
    std::cout << "CModbusSlave::ReadCoils 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadDiscreteInputs(void)
{
    std::cout << "CModbusSlave::ReadDiscreteInputs 1" << std::endl;

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
        std::cout << "CModbusSlave::ReadDiscreteInputs 4" << std::endl;
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
uint16_t CModbusSlave::ReadHoldingRegisters(void)
{
    std::cout << "CModbusSlave::ReadHoldingRegisters 1" << std::endl;

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

    std::cout << "CModbusSlave::ReadHoldingRegisters uiAddress "  << (int)uiAddress << std::endl;
    std::cout << "CModbusSlave::ReadHoldingRegisters uiNumberB "  << (int)uiNumberB << std::endl;

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

    std::cout << "CModbusSlave::ReadHoldingRegisters uiLength "  << (int)uiLength << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadInputRegisters(void)
{
    std::cout << "CModbusSlave::ReadInputRegisters 1" << std::endl;

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
uint16_t CModbusSlave::WriteSingleCoil(void)
{
    std::cout << "CModbusSlave::WriteSingleCoil 1" << std::endl;

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
uint16_t CModbusSlave::WriteSingleRegister(void)
{
    std::cout << "CModbusSlave::WriteSingleRegister 1" << std::endl;

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
uint16_t CModbusSlave::WriteMultipleCoils(void)
{
    std::cout << "CModbusSlave::WriteMultipleCoils 1" << std::endl;

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
uint16_t CModbusSlave::WriteMultipleRegisters(void)
{
    std::cout << "CModbusSlave::WriteMultipleRegisters 1" << std::endl;

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
uint16_t CModbusSlave::ReadExceptionStatus(void)
{
//    errno = ENOPROTOOPT;
    return -1;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::WriteAndReadRegisters(void)
{

    return 0;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReportSlaveID(void)
{
    std::cout << "CModbusSlave::ReportSlaveID 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    std::cout << "CModbusSlave::ReportSlaveID uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSlave::ReportSlaveID uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    std::cout << "CModbusSlave::ReportSlaveID 4" << std::endl;

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

    std::cout << "CModbusSlave::ReportSlaveID 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::Programming(void)
{
    std::cout << "CModbusSlave::Programming 1" << std::endl;
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
uint16_t CModbusSlave::PollProgramming(void)

{
    std::cout << "CModbusSlave::PollProgramming 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

//    CDeviceControl* pxDeviceControl =
//        (CDeviceControl*)GetResources() ->
//        GetCommonTaskFromMapPointer("DeviceControl");

    CDataStore* pxDataStoreFileSystem =
        (CDataStore*)GetResources() ->
        GetCommonTaskFromMapPointer("DataStoreFileSystem");


//    // Устройство хранения в прцессе записи?
//    if (m_pxDeviceControl ->
//            GetFsmOperationStatus() == CDfa::IN_PROGRESS)
//    {
//        std::cout << "CModbusSlave::PollProgramming 2" << std::endl;
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
//        std::cout << "CModbusSlave::PollProgramming 3" << std::endl;
//
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
//
//    }
//    // Устройство хранения закончило запись не успешно?
//    else if (m_pxDeviceControl ->
//             GetFsmOperationStatus() == CDfa::ERROR_OCCURED)
//    {
//        std::cout << "CModbusSlave::PollProgramming 4" << std::endl;
//
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                      ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ACKNOWLEDGE,
//                                     puiResponse);
//    }

    // Ожидаем окончания записи автоматом устройства хранения.
    // Устройство хранения закончило запись успешно?
    if (pxDataStoreFileSystem -> IsDoneOk())
    {
        std::cout << "CModbusSlave::PollProgramming 2" << std::endl;

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
        std::cout << "CModbusSlave::PollProgramming 3" << std::endl;

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
uint16_t CModbusSlave::DataBaseRead(void)
{
    std::cout << "CModbusSlave::DataBaseRead 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint8_t uiBlockIndex = puiRequest[uiPduOffset + 1];

    std::cout << "CModbusSlave::DataBaseRead uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSlave::DataBaseRead uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CModbusSlave::DataBaseRead uiBlockIndex "  << (int)uiBlockIndex << std::endl;

    if ((uiBlockIndex < 0) ||
            (uiBlockIndex > (CDataStore::MAX_BLOCKS_NUMBER - 1)))
    {
        std::cout << "CModbusSlave::DataBaseRead 2" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else
    {
        std::cout << "CModbusSlave::DataBaseRead 4" << std::endl;

        m_uiFunctionCode = uiFunctionCode;

        m_pxDeviceControlLink ->
        GetDataContainerPointer() ->
        SetContainerData(uiFunctionCode,
                         uiBlockIndex,
                         &puiResponse[uiPduOffset + 3],
                         0,
                         0);

        CDeviceControl* pxDeviceControl =
            (CDeviceControl*)GetResources() ->
            GetCommonTaskFromMapPointer("DeviceControlRtuUpperLevel");

//        if (pxTask != 0)
//        {
//            std::cout << "CModbusSlave::Fsm INIT 2"  << std::endl;
//            if (pxTask -> GetFsmState() >= READY)
//            {
//                SetModbusSlaveLinkLayer((CModbusSlaveLinkLayer*)pxTask);
//                SetFsmState(READY);
//                std::cout << "CModbusSlave::Fsm READY"  << std::endl;
//            }
//        }
//        else
//        {
//            std::cout << "CModbusSlave::Fsm INIT 3"  << std::endl;
//            if (GetTimerPointer() -> IsOverflow())
//            {
//                SetFsmState(STOP);
//                std::cout << "CModbusSlave::Fsm STOP"  << std::endl;
//            }
//        }

        uiLength = pxDeviceControl ->
                   DataBaseBlockRead(&puiResponse[uiPduOffset + 3], uiBlockIndex);
//        uiLength = m_pxResources ->
//                   m_pxDeviceControl ->
//                   DataBaseBlockRead(&puiResponse[uiPduOffset + 3], uiBlockIndex);
        // количество байт в пакете
        puiResponse[uiPduOffset + 1] = uiLength + 1;
        uiLength ++;

        uiLength += m_pxModbusSlaveLinkLayer ->
                    ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        // номер блока базы данных
        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
        uiLength ++;
    }

    std::cout << "CModbusSlave::DataBaseRead 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::DataBaseWrite(void)
{
//    rsp_length == 2. адрес slave + функция
    // (rsp_length - 2) - адрес slave.
    // (rsp_length - 1) - функция.
//        // (rsp_length) - номер блока.
    // (rsp_length + 1) - начало данных.

    std::cout << "CModbusSlave::DataBaseWrite 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint8_t uiBlockIndex = puiRequest[uiPduOffset + 1];

    std::cout << "CModbusSlave::DataBaseWrite uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSlave::DataBaseWrite uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CModbusSlave::DataBaseWrite uiBlockIndex "  << (int)uiBlockIndex << std::endl;


    if ((uiBlockIndex < 0) ||
            (uiBlockIndex > (CDataStore::MAX_BLOCKS_NUMBER - 1)))
    {
        std::cout << "CModbusSlave::DataBaseWrite 2" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        CDataStore* pxDataStoreFileSystem =
            (CDataStore*)GetResources() ->
            GetCommonTaskFromMapPointer("DataStoreFileSystem");

        uiLength = pxDataStoreFileSystem ->
                   GetBlockLength(uiBlockIndex);

        memcpy(m_puiIntermediateBuff,
               &puiRequest[uiPduOffset + 2],
               uiLength);

        // Сохраним блок БД.
        std::cout << "CModbusSlave::DataBaseWrite 3" << std::endl;
        pxDataStoreFileSystem ->
        WriteBlock(m_puiIntermediateBuff,
                   uiLength,
                   uiBlockIndex);

        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        // номер блока базы данных
        puiResponse[uiPduOffset + 1] = 1;
        uiLength ++;
        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
        uiLength ++;
    }

    std::cout << "CModbusSlave::DataBaseWrite 7" << std::endl;
    return uiLength;
}


//-------------------------------------------------------------------------------
uint16_t CModbusSlave::RequestProcessing(void)
{
    std::cout << "CModbusSlave::RequestProcessing 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    std::cout << "CModbusSlave::RequestProcessing uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSlave::RequestProcessing uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    /* Filter on the Modbus unit identifier (slave) in RTU mode */
    if (uiSlave != m_uiOwnAddress && uiSlave != MODBUS_BROADCAST_ADDRESS)
    {
        std::cout << "CModbusSlave::RequestProcessing 2" << std::endl;
        return 0;
    }

    switch (uiFunctionCode)
    {
        std::cout << "CModbusSlave::RequestProcessing 3" << std::endl;
    case _FC_READ_COILS:
        std::cout << "CModbusSlave::RequestProcessing _FC_READ_COILS"  << std::endl;
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
        std::cout << "CModbusSlave::RequestProcessing 4" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_FUNCTION,
                                     puiResponse);
        break;
    }

    std::cout << "CModbusSlave::RequestProcessing 5" << std::endl;
    uiLength = m_pxModbusSlaveLinkLayer -> Tail(puiResponse, uiLength);
    m_pxModbusSlaveLinkLayer -> SetFrameLength(uiLength);
    return uiLength;
}









//-------------------------------------------------------------------------------
////AnswerProcessing
////-------------------------------------------------------------------------------
//uint16_t CModbusSlave::ReadCoilsAnswer(void)
//{
//    std::cout << "CModbusSlave::ReadCoilsAnswer 1" << std::endl;
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
//    std::cout << "CModbusSlave::ReadCoilsAnswer uiSlave "  << (int)uiSlave << std::endl;
//    std::cout << "CModbusSlave::ReadCoilsAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
//    std::cout << "CModbusSlave::ReadCoilsAnswer uiAddress "  << (int)uiAddress << std::endl;
//    std::cout << "CModbusSlave::ReadCoilsAnswer uiNumberB "  << (int)uiNumberB << std::endl;
//
//    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
//    {
//        std::cout << "CModbusSlave::ReadCoilsAnswer 2" << std::endl;
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
//                                     puiResponse);
//    }
//    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
//    {
//        std::cout << "CModbusSlave::ReadCoilsAnswer 3" << std::endl;
//        std::cout << "CModbusSlave::ReadCoilsAnswer m_uiCoilsNumber "  << (int)m_uiCoilsNumber << std::endl;
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
//                                     puiResponse);
//    }
//    else
//    {
//        std::cout << "CModbusSlave::ReadCoilsAnswer 4" << std::endl;
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
//
//        if (uiNumberB % 8)
//        {
//            std::cout << "CModbusSlave::ReadCoilsAnswer 5" << std::endl;
//            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
//        }
//        else
//        {
//            std::cout << "CModbusSlave::ReadCoilsAnswer 6" << std::endl;
//            puiResponse[uiLength++] = (uiNumberB / 8);
//        }
//        uiLength = ByteToBitPack(uiAddress,
//                                 uiNumberB,
//                                 m_puiCoils,
//                                 puiResponse,
//                                 uiLength);
//    }
//    std::cout << "CModbusSlave::ReadCoilsAnswer 7" << std::endl;
//    return uiLength;
//
//}
//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadCoilsAnswer(void)
{
    std::cout << "CModbusSlave::ReadCoilsAnswer 1" << std::endl;

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

    std::cout << "CModbusSlave::ReadCoilsAnswer uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSlave::ReadCoilsAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CModbusSlave::ReadCoilsAnswer uiAddress "  << (int)uiAddress << std::endl;
    std::cout << "CModbusSlave::ReadCoilsAnswer uiNumberB "  << (int)uiNumberB << std::endl;

    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
    {
        std::cout << "CModbusSlave::ReadCoilsAnswer 2" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
    {
        std::cout << "CModbusSlave::ReadCoilsAnswer 3" << std::endl;
        std::cout << "CModbusSlave::ReadCoilsAnswer m_uiCoilsNumber "  << (int)m_uiCoilsNumber << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        std::cout << "CModbusSlave::ReadCoilsAnswer 4" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        if (uiNumberB % 8)
        {
            std::cout << "CModbusSlave::ReadCoilsAnswer 5" << std::endl;
            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
        }
        else
        {
            std::cout << "CModbusSlave::ReadCoilsAnswer 6" << std::endl;
            puiResponse[uiLength++] = (uiNumberB / 8);
        }
        uiLength = ByteToBitPack(uiAddress,
                                 uiNumberB,
                                 m_puiCoils,
                                 puiResponse,
                                 uiLength);
    }
    std::cout << "CModbusSlave::ReadCoilsAnswer 7" << std::endl;
    return uiLength;

}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadDiscreteInputsAnswer(void)
{
    std::cout << "CModbusSlave::ReadDiscreteInputsAnswer 1" << std::endl;

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
        std::cout << "CModbusSlave::ReadDiscreteInputsAnswer 4" << std::endl;
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
uint16_t CModbusSlave::ReadHoldingRegistersAnswer(void)
{
    std::cout << "CModbusSlave::ReadHoldingRegistersAnswer 1" << std::endl;

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

    std::cout << "CModbusSlave::ReadHoldingRegistersAnswer uiAddress "  << (int)uiAddress << std::endl;
    std::cout << "CModbusSlave::ReadHoldingRegistersAnswer uiNumberB "  << (int)uiNumberB << std::endl;

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
        std::cout << "CModbusSlave::ReadHoldingRegistersAnswer 4" << std::endl;
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

    std::cout << "CModbusSlave::ReadHoldingRegistersAnswer uiLength "  << (int)uiLength << std::endl;
    return uiLength;

}//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadInputRegistersAnswer(void)
{
    std::cout << "CModbusSlave::ReadInputRegistersAnswer 1" << std::endl;

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
uint16_t CModbusSlave::WriteSingleCoilAnswer(void)
{
    std::cout << "CModbusSlave::WriteSingleCoilAnswer 1" << std::endl;

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
uint16_t CModbusSlave::WriteSingleRegisterAnswer(void)
{
    std::cout << "CModbusSlave::WriteSingleRegisterAnswer 1" << std::endl;

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
uint16_t CModbusSlave::WriteMultipleCoilsAnswer(void)
{
    std::cout << "CModbusSlave::WriteMultipleCoilsAnswer 1" << std::endl;

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
uint16_t CModbusSlave::WriteMultipleRegistersAnswer(void)
{
    std::cout << "CModbusSlave::WriteMultipleRegistersAnswer 1" << std::endl;

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
uint16_t CModbusSlave::ReadExceptionStatusAnswer(void)
{
//    errno = ENOPROTOOPT;
    return -1;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::WriteAndReadRegistersAnswer(void)
{

    return 0;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReportSlaveIDAnswer(void)
{
    std::cout << "CModbusSlave::ReportSlaveIDAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    std::cout << "CModbusSlave::ReportSlaveIDAnswer uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSlave::ReportSlaveIDAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    std::cout << "CModbusSlave::ReportSlaveIDAnswer 4" << std::endl;

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

    std::cout << "CModbusSlave::ReportSlaveIDAnswer 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ProgrammingAnswer(void)
{
    std::cout << "CModbusSlave::ProgrammingAnswer 1" << std::endl;
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
uint16_t CModbusSlave::PollProgrammingAnswer(void)
{
    std::cout << "CModbusSlave::PollProgrammingAnswer 1" << std::endl;

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
//        std::cout << "CModbusSlave::PollProgrammingAnswer 2" << std::endl;
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
//        std::cout << "CModbusSlave::PollProgrammingAnswer 3" << std::endl;
//
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
//
//    }
//    // Устройство хранения закончило запись не успешно?
//    else if (m_pxDeviceControl ->
//             GetFsmOperationStatus() == CDfa::ERROR_OCCURED)
//    {
//        std::cout << "CModbusSlave::PollProgrammingAnswer 4" << std::endl;
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
        std::cout << "CModbusSlave::PollProgrammingAnswer 2" << std::endl;

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
        std::cout << "CModbusSlave::PollProgrammingAnswer 3" << std::endl;

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
uint16_t CModbusSlave::DataBaseReadAnswer(void)
{
    std::cout << "CModbusSlave::DataBaseReadAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint8_t uiBlockIndex = puiRequest[uiPduOffset + 1];

    std::cout << "CModbusSlave::DataBaseReadAnswer uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSlave::DataBaseReadAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CModbusSlave::DataBaseReadAnswer uiBlockIndex "  << (int)uiBlockIndex << std::endl;

    if ((uiBlockIndex < 0) ||
            (uiBlockIndex > (CDataStore::MAX_BLOCKS_NUMBER - 1)))
    {
        std::cout << "CModbusSlave::DataBaseReadAnswer 2" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else
    {
        CDeviceControl* pxDeviceControl =
            (CDeviceControl*)GetResources() ->
            GetCommonTaskFromMapPointer("DeviceControlRtuUpperLevelAnswer");

//        if (pxTask != 0)
//        {
//            std::cout << "CModbusSlave::Fsm INIT 2"  << std::endl;
//            if (pxTask -> GetFsmState() >= READY)
//            {
//                SetModbusSlaveLinkLayer((CModbusSlaveLinkLayer*)pxTask);
//                SetFsmState(READY);
//                std::cout << "CModbusSlave::Fsm READY"  << std::endl;
//            }
//        }
//        else
//        {
//            std::cout << "CModbusSlave::Fsm INIT 3"  << std::endl;
//            if (GetTimerPointer() -> IsOverflow())
//            {
//                SetFsmState(STOP);
//                std::cout << "CModbusSlave::Fsm STOP"  << std::endl;
//            }
//        }

        uiLength = pxDeviceControl ->
                   DataBaseBlockRead(&puiResponse[uiPduOffset + 3], uiBlockIndex);

        puiResponse[uiPduOffset + 1] = uiLength + 1;
        uiLength ++;

        uiLength += m_pxModbusSlaveLinkLayer ->
                    ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        // номер блока базы данных
        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
        uiLength ++;
    }

    std::cout << "CModbusSlave::DataBaseReadAnswer 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::DataBaseWriteAnswer(void)
{
    std::cout << "CModbusSlave::DataBaseWriteAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint8_t uiBlockIndex = puiRequest[uiPduOffset + 1];

    std::cout << "CModbusSlave::DataBaseWriteAnswer uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSlave::DataBaseWriteAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CModbusSlave::DataBaseWriteAnswer uiBlockIndex "  << (int)uiBlockIndex << std::endl;


    if ((uiBlockIndex < 0) ||
            (uiBlockIndex > (CDataStore::MAX_BLOCKS_NUMBER - 1)))
    {
        std::cout << "CModbusSlave::DataBaseWriteAnswer 2" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        CDataStore* pxDataStoreFileSystem =
            (CDataStore*)GetResources() ->
            GetCommonTaskFromMapPointer("DataStoreFileSystemAnswer");

        uint16_t uiLength = pxDataStoreFileSystem ->
                            GetBlockLength(uiBlockIndex);

        memcpy(m_puiIntermediateBuff,
               &puiRequest[uiPduOffset + 2],
               uiLength);

        // Сохраним блок БД.
        std::cout << "CModbusSlave::DataBaseWriteAnswer 3" << std::endl;
        pxDataStoreFileSystem ->
        WriteBlock(m_puiIntermediateBuff,
                   uiLength,
                   uiBlockIndex);

        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        // номер блока базы данных
        puiResponse[uiPduOffset + 1] = 1;
        uiLength ++;
        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
        uiLength ++;
    }

    std::cout << "CModbusSlave::DataBaseWriteAnswer 7" << std::endl;
    return uiLength;
}

////-------------------------------------------------------------------------------
//uint16_t CModbusSlave::AnswerProcessing(void)
//{
//    std::cout << "CModbusSlave::AnswerProcessing 1" << std::endl;
//
//    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
//    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
//    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
//    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();
//
//    int8_t uiSlave = puiRequest[uiPduOffset - 1];
//    int8_t uiFunctionCode = puiRequest[uiPduOffset];
//
//    std::cout << "CModbusSlave::AnswerProcessing uiSlave "  << (int)uiSlave << std::endl;
//    std::cout << "CModbusSlave::AnswerProcessing uiFunctionCode "  << (int)uiFunctionCode << std::endl;
//
//    /* Filter on the Modbus unit identifier (slave) in RTU mode */
//    if (uiSlave != m_uiOwnAddress && uiSlave != MODBUS_BROADCAST_ADDRESS)
//    {
//        std::cout << "CModbusSlave::AnswerProcessing 2" << std::endl;
//        return 0;
//    }
//
//    switch (uiFunctionCode)
//    {
//        std::cout << "CModbusSlave::AnswerProcessing 3" << std::endl;
//    case _FC_READ_COILS:
//        std::cout << "CModbusSlave::AnswerProcessing _FC_READ_COILS"  << std::endl;
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
//        std::cout << "CModbusSlave::AnswerProcessing 4" << std::endl;
//        uiLength = m_pxModbusSlaveLinkLayer ->
//                   ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_FUNCTION,
//                                     puiResponse);
//        break;
//    }
//
//    std::cout << "CModbusSlave::AnswerProcessing 5" << std::endl;
//    uiLength = m_pxModbusSlaveLinkLayer -> Tail(puiResponse, uiLength);
//    m_pxModbusSlaveLinkLayer -> SetFrameLength(uiLength);
//    return uiLength;
//}
//-------------------------------------------------------------------------------
uint16_t CModbusSlave::AnswerProcessing(void)
{
    std::cout << "CModbusSlave::AnswerProcessing 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    std::cout << "CModbusSlave::AnswerProcessing uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSlave::AnswerProcessing uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    /* Filter on the Modbus unit identifier (slave) in RTU mode */
    if (uiSlave != m_uiOwnAddress && uiSlave != MODBUS_BROADCAST_ADDRESS)
    {
        std::cout << "CModbusSlave::AnswerProcessing 2" << std::endl;
        return 0;
    }

    // проверяем сохранённый локально текущий код функции.
    switch (m_uiFunctionCode)
    {
        std::cout << "CModbusSlave::AnswerProcessing 3" << std::endl;
    case _FC_READ_COILS:
        std::cout << "CModbusSlave::Answer _FC_READ_COILS"  << std::endl;
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
        std::cout << "CModbusSlave::AnswerProcessing 4" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_FUNCTION,
                                     puiResponse);
        break;
    }

    std::cout << "CModbusSlave::AnswerProcessing 5" << std::endl;
    uiLength = m_pxModbusSlaveLinkLayer -> Tail(puiResponse, uiLength);
    m_pxModbusSlaveLinkLayer -> SetFrameLength(uiLength);
    return uiLength;
}

//-------------------------------------------------------------------------------
void CModbusSlave::SetByteFromBits(uint8_t *dest, uint16_t index, const uint8_t value)
{
    uint16_t i;

    for (i=0; i<8; i++)
    {
        dest[index+i] = (value & (1 << i)) ? 1 : 0;
    }
}

//-------------------------------------------------------------------------------
void CModbusSlave::SetBytesFromBits(uint8_t *dest, uint16_t index, uint16_t nb_bits,
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
uint8_t CModbusSlave::GetByteFromBits(const uint8_t *src, uint16_t index,
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
float CModbusSlave::GetFloat(const uint16_t *src)
{
    float f = 0.0f;
    uint32_t i;

    i = (((uint32_t)src[1]) << 16) + src[0];
    memcpy(&f, &i, sizeof(float));

    return f;
}

//-------------------------------------------------------------------------------
void CModbusSlave::SetFloat(float f, uint16_t *dest)
{
    uint32_t i = 0;

    memcpy(&i, &f, sizeof(uint32_t));
    dest[0] = (uint16_t)i;
    dest[1] = (uint16_t)(i >> 16);
}

//-------------------------------------------------------------------------------
uint8_t CModbusSlave::Fsm(void)
{
//    std::cout << "CModbusSlave::Fsm 1" << endl;
    uint8_t uiReadyTaskCounter = 0;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CModbusSlave::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CModbusSlave::Fsm STOP"  << std::endl;[[[]=
        SetFsmState(START);
        break;

    case START:
        std::cout << "CModbusSlave::Fsm START"  << std::endl;
        std::cout << "CModbusSlave::Fsm m_sModbusSlaveLinkLayerName" << " " << (m_sModbusSlaveLinkLayerName) << std::endl;
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:

        std::cout << "CModbusSlave::Fsm INIT 1"  << std::endl;
        {
            CTaskInterface* pxTask =
                GetResources() ->
                GetCommonTaskFromMapPointer(m_sModbusSlaveLinkLayerName);

            if (pxTask != 0)
            {
                std::cout << "CModbusSlave::Fsm INIT 2"  << std::endl;
                if (pxTask -> GetFsmState() >= READY)
                {
                    std::cout << "CModbusSlave::Fsm INIT 3"  << std::endl;
                    SetModbusSlaveLinkLayer((CModbusSlaveLinkLayer*)pxTask);
                    uiReadyTaskCounter += 1;
//                    SetFsmState(READY);
                }
            }
            else
            {
                std::cout << "CModbusSlave::Fsm INIT 4"  << std::endl;
                if (GetTimerPointer() -> IsOverflow())
                {
                    SetFsmState(STOP);
                }
            }
        }


//        {
//            CTaskInterface* pxTask =
//                GetResources() ->
//                GetCommonTaskFromMapPointer(m_sDeviceControlName);
//
//            if (pxTask != 0)
//            {
//                std::cout << "CModbusSlave::Fsm INIT 4"  << std::endl;
//                if (pxTask -> GetFsmState() >= READY)
//                {
//                    SetDeviceControl((CDeviceControl*)pxTask);
//                    SetFsmState(READY);
//                    std::cout << "CModbusSlave::Fsm READY"  << std::endl;
//                }
//            }
//            else
//            {
//                std::cout << "CModbusSlave::Fsm INIT 5"  << std::endl;
//                if (GetTimerPointer() -> IsOverflow())
//                {
//                    SetFsmState(STOP);
//                    std::cout << "CModbusSlave::Fsm STOP"  << std::endl;
//                }
//            }
//        }


        {
            CTaskInterface* pxTask =
                GetResources() ->
                GetCommonTaskFromMapPointer(m_sDeviceControlLinkName);

            if (pxTask != 0)
            {
                std::cout << "CModbusSlave::Fsm INIT 8"  << std::endl;
                if (pxTask -> GetFsmState() >= READY)
                {
                    std::cout << "CModbusSlave::Fsm INIT 9"  << std::endl;
                    SetDeviceControlLink((CLinkInterface*)pxTask);

                    m_pxDeviceControlLink ->
                    GetDataContainerPointer() ->
                    SetContainerData(0,
                                     0,
                                     0,
                                     0,
                                     0);

                    uiReadyTaskCounter += 1;
//                    SetFsmState(READY);
                }
            }
            else
            {
                std::cout << "CModbusSlave::Fsm INIT 10"  << std::endl;
                if (GetTimerPointer() -> IsOverflow())
                {
                    SetFsmState(STOP);
                }
            }
        }

        if (uiReadyTaskCounter > 1)
        {
            SetFsmState(READY);
        }
        break;

    case READY:
        std::cout << "CModbusSlave::Fsm READY"  << std::endl;
//        {
//            if (m_pxCommandDataContainer != 0)
//            {
//                std::cout << "CModbusSlave::Fsm READY 2"  << std::endl;
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
        std::cout << "CModbusSlave::Fsm DONE_OK"  << std::endl;
        SetFsmOperationStatus(DONE_OK);
        SetFsmState(READY);
        break;

    case DONE_ERROR:
        std::cout << "CModbusSlave::Fsm DONE_ERROR"  << std::endl;
        SetFsmOperationStatus(DONE_ERROR);
        SetFsmState(READY);
        break;

    case COMMUNICATION_START:
        std::cout << "CModbusSlave::Fsm COMMUNICATION_START"  << std::endl;
        m_pxModbusSlaveLinkLayer ->
        CommunicationStart();
        SetFsmState(MESSAGE_RECEIVE_WAITING);
        break;

    case COMMUNICATION_RECEIVE_START:
        std::cout << "CModbusSlave::Fsm COMMUNICATION_RECEIVE_START"  << std::endl;
        m_pxModbusSlaveLinkLayer ->
        CommunicationReceiveStart();
        SetFsmState(MESSAGE_RECEIVE_WAITING);
        break;

    case MESSAGE_RECEIVE_WAITING:
//        std::cout << "CModbusSlave::Fsm MESSAGE_RECEIVE_WAITING"  << std::endl;
        if (m_pxModbusSlaveLinkLayer -> IsDoneOk())
        {
            std::cout << "CModbusSlave::Fsm MESSAGE_RECEIVE_WAITING 2"  << std::endl;
            SetFsmState(REQUEST_PROCESSING);
        }
        else if (m_pxModbusSlaveLinkLayer -> IsDoneError())
        {
            std::cout << "CModbusSlave::Fsm MESSAGE_RECEIVE_WAITING 3"  << std::endl;
            SetFsmState(COMMUNICATION_START);
        }
        break;

    case REQUEST_PROCESSING:
        std::cout << "CModbusSlave::Fsm REQUEST_PROCESSING"  << std::endl;
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

        if (m_pxDeviceControlLink ->
                GetDataContainerPointer() ->
                GetFsmCommandState())
        {
            std::cout << "CModbusSlave::Fsm REQUEST_PROCESSING 2"  << std::endl;
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(ANSWER_PROCESSING_WAITING);
//            SetFsmState(BEFORE_ANSWERING_WAITING);
        }
        else
        {
            std::cout << "CModbusSlave::Fsm REQUEST_PROCESSING 3"  << std::endl;
            GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
            SetFsmState(BEFORE_ANSWERING_WAITING);
        }
        break;

    case ANSWER_PROCESSING_WAITING:
        std::cout << "CModbusSlave::Fsm ANSWER_PROCESSING_WAITING"  << std::endl;
        GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
        SetFsmState(ANSWER_PROCESSING);

//        if (m_pxDeviceControlLink ->
//                GetTaskPerformerPointer() ->
//                IsDoneOk())
//        {
//            std::cout << "CModbusSlave::Fsm ANSWER_PROCESSING_WAITING 2"  << std::endl;
//            GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
//            SetFsmState(BEFORE_ANSWERING_WAITING);
//        }
//        else if (m_pxDeviceControlLink ->
//                 GetTaskPerformerPointer() ->
//                 IsDoneError())
//        {
//            std::cout << "CModbusSlave::Fsm ANSWER_PROCESSING_WAITING 3"  << std::endl;
//            GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
//            SetFsmState(BEFORE_ANSWERING_WAITING);
//        }
//        else
//        {
//            // Время ожидания выполнения запроса закончилось?
//            if (GetTimerPointer() -> IsOverflow())
//            {
//                std::cout << "CModbusSlave::Fsm ANSWER_PROCESSING_WAITING 4"  << std::endl;
//                m_pxDeviceControlLink ->
//                GetDataContainerPointer() ->
//                SetFsmCommandState(0);
//                SetFsmState(MESSAGE_RECEIVE_WAITING);
//            }
//        }
        break;

    case ANSWER_PROCESSING:
        std::cout << "CModbusSlave::Fsm ANSWER_PROCESSING"  << std::endl;
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
//            std::cout << "CModbusSlave::Fsm ANSWER_PROCESSING 2"  << std::endl;
//            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
//            SetFsmState(ANSWER_PROCESSING_WAITING);
////            SetFsmState(BEFORE_ANSWERING_WAITING);
//        }
//        else
//        {
//            std::cout << "CModbusSlave::Fsm ANSWER_PROCESSING 3"  << std::endl;
//            GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
//            SetFsmState(BEFORE_ANSWERING_WAITING);
//        }
        break;

    case BEFORE_ANSWERING_WAITING:
//        std::cout << "CModbusSlave::Fsm BEFORE_ANSWERING_WAITING"  << std::endl;
        // Закончилось время паузы между приёмом и передачей(5 милисекунд)?
        if (GetTimerPointer() -> IsOverflow())
        {
//            GetTimerPointer() -> Set(m_uiConfirmationTimeout);
            //            m_pxModbusSlaveLinkLayer ->
//            SetFsmState(CModbusSlaveLinkLayerInterface::COMMUNICATION_TRANSMIT_START);
            m_pxModbusSlaveLinkLayer ->
            TransmitStart();
            SetFsmState(AFTER_ANSWERING_WAITING);
        }
        break;

    case MESSAGE_SEND:
        std::cout << "CModbusSlave::Fsm MESSAGE_SEND"  << std::endl;
        SetFsmState(MESSAGE_RECEIVE_WAITING);
        break;

    case AFTER_ANSWERING_WAITING:
        std::cout << "CModbusSlave::Fsm AFTER_ANSWERING_WAITING"  << std::endl;
        if (m_pxModbusSlaveLinkLayer -> IsDoneOk())
        {
            std::cout << "CModbusSlave::Fsm AFTER_ANSWERING_WAITING 2"  << std::endl;
            m_pxModbusSlaveLinkLayer ->
            CommunicationReceiveStart();
            SetFsmState(MESSAGE_RECEIVE_WAITING);
        }
        else if (m_pxModbusSlaveLinkLayer -> IsDoneError())
        {
            std::cout << "CModbusSlave::Fsm AFTER_ANSWERING_WAITING 3"  << std::endl;
            SetFsmState(COMMUNICATION_START);
        }
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------

