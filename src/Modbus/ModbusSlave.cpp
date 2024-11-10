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
#include "ModbusSlave.h"
#include "Platform.h"
#include "DataStore.h"
#include "DeviceControl.h"

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
    ModbusWorkingArraysInit();
//    m_pxModbusSlaveLinkLayer = new CModbusSlaveLinkLayerInterface();
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CModbusSlave::CModbusSlave(CResources* pxResources)
{
    std::cout << "CModbusSlave constructor 2"  << std::endl;
    m_pxModbusSlaveLinkLayer = 0;
    // получим имя класса.
    sprintf(GetTaskNamePointer(),
            "%s",
            typeid(*this).name());
    // получим имя класса.
//    sprintf(GetTaskNamePointer(),
//            "%s",
//            this -> std::type_info::name());
    SetResources(pxResources);
//    m_pxResources ->
//    AddCommonListTask(this);
//    m_pxResources ->
//    AddCommonTaskToMap("ModbusTcpSlaveUpperLevel", this);
//    m_pxResources ->
//    GetCommonListTaskPointer("CModbusSlave");
//    m_pxResources ->
//    GetCommonTaskFromMapPointer("ModbusTcpSlaveUpperLevel");
    ModbusWorkingArraysInit();
//    m_pxModbusSlaveLinkLayer = new CModbusSlaveLinkLayerInterface();
    SetFsmState(START);
}
//std::type_info::name() или std::get_class_name().
//-------------------------------------------------------------------------------
CModbusSlave::~CModbusSlave()
{
//    delete pxModbusSlaveLinkLayer;
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

////-------------------------------------------------------------------------------
//void CModbusSlave::SetResources(CResources* pxResources)
//{
//    m_pxResources = pxResources;
//}
//
////-------------------------------------------------------------------------------
//CResources* CModbusSlave::GetResources(void)
//{
//    return m_pxResources;
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

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadCoils(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)
{
    std::cout << "CModbusSlave::ReadCoils 1" << std::endl;
    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
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

//    puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
//    puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
//    puiRequest = m_pxModbusSlaveLinkLayer -> GetRxPdu();
//    puiResponse = m_pxModbusSlaveLinkLayer -> GetTxPdu();
//    int8_t uiSlave = m_pxModbusSlaveLinkLayer -> GetSlaveAddress();
//    int8_t uiFunctionCode = m_pxModbusSlaveLinkLayer -> GetFunctionCode();
//    uint16_t uiAddress = m_pxModbusSlaveLinkLayer -> GetDataAddress();
//    uint16_t uiNumberB = m_pxModbusSlaveLinkLayer -> GetBitNumber();

    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
    {
        std::cout << "CModbusSlave::ReadCoils 2" << std::endl;
        uiLength = ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
    {
        std::cout << "CModbusSlave::ReadCoils 3" << std::endl;
        std::cout << "CModbusSlave::ReadCoils m_uiCoilsNumber "  << (int)m_uiCoilsNumber << std::endl;
        uiLength = ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        std::cout << "CModbusSlave::ReadCoils 4" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

//        uiLength = m_pxModbusSlaveLinkLayer -> ResponseHeader(uiSlave);
//        puiResponse[uiLength++] = uiFunctionCode;

        if (uiNumberB % 8)
        {
            std::cout << "CModbusSlave::ReadCoils 5" << std::endl;
            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
        }
        else
        {
            std::cout << "CModbusSlave::ReadCoils 6" << std::endl;
            puiResponse[uiLength++] = (uiNumberB / 8);
        }
        uiLength = ByteToBitPack(uiAddress,
                                 uiNumberB,
                                 m_puiCoils,
                                 puiResponse,
                                 uiLength);
    }
    std::cout << "CModbusSlave::ReadCoils 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadDiscreteInputs(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)
{
    std::cout << "CModbusSlave::ReadDiscreteInputs 1" << std::endl;
    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
//    uint16_t uiPduOffset = HEADER_LENGTH();
    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
    {
        uiLength = ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiDiscreteInputsNumber)
    {
        uiLength = ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
//        uiLength = ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
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
uint16_t CModbusSlave::ReadHoldingRegisters(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)
{
    uint16_t uiPduOffset = HEADER_LENGTH();
    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_READ_REGISTERS < uiNumberB)
    {
        uiLength = ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiHoldingRegistersNumber)
    {
        uiLength = ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        uiLength = ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
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

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadInputRegisters(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)
{
    uint16_t uiPduOffset = HEADER_LENGTH();
    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_READ_REGISTERS < uiNumberB)
    {
        uiLength = ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiInputRegistersNumber)
    {
        uiLength = ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        uiLength = ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
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
uint16_t CModbusSlave::WriteSingleCoil(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)
{
    uint16_t uiPduOffset = HEADER_LENGTH();
    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    if (uiAddress >= m_uiCoilsNumber)
    {
        uiLength = ResponseException(uiSlave,
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
            uiLength = ResponseException(uiSlave,
                                         uiFunctionCode,
                                         MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                         puiResponse);
        }

    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::WriteSingleRegister(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)
{
    uint16_t uiPduOffset = HEADER_LENGTH();
    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    if (uiAddress >= m_uiHoldingRegistersNumber)
    {
        uiLength = ResponseException(uiSlave,
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
uint16_t CModbusSlave::WriteMultipleCoils(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)
{
    uint16_t uiPduOffset = HEADER_LENGTH();
    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_WRITE_BITS < uiNumberB)
    {
        uiLength = ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
    {
        uiLength = ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        SetBytesFromBits(m_puiCoils, uiAddress, uiNumberB, &puiRequest[uiPduOffset + 6]);

        uiLength = ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
        /* 4 to copy the bit address (2) and the quantity of bits */
        memcpy(puiResponse + uiLength, puiRequest + uiLength, 4);
        uiLength += 4;
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::WriteMultipleRegisters(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)
{
    uint16_t uiPduOffset = HEADER_LENGTH();
    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_WRITE_REGISTERS < uiNumberB)
    {
        uiLength = ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiHoldingRegistersNumber)
    {
        uiLength = ResponseException(uiSlave,
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

        uiLength = ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
        /* 4 to copy the bit address (2) and the quantity of bits */
        memcpy(puiResponse + uiLength, puiRequest + uiLength, 4);
        uiLength += 4;

    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadExceptionStatus(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)
{
//    errno = ENOPROTOOPT;
    return -1;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::WriteAndReadRegisters(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)
{

    return 0;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReportSlaveID(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)
{
    std::cout << "CModbusSlave::ReportSlaveID 1" << std::endl;
    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    std::cout << "CModbusSlave::ReportSlaveID uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSlave::ReportSlaveID uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    std::cout << "CModbusSlave::ReportSlaveID 4" << std::endl;
    uiLength = m_pxResources ->
               m_pxDeviceControl ->
               ConfigurationRead(&puiResponse[uiPduOffset + 2]);

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
uint16_t CModbusSlave::Programming(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)
{
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
//
//    uint16_t uiPduOffset = HEADER_LENGTH();
//    int8_t uiSlave = puiRequest[uiPduOffset - 1];
//    int8_t uiFunctionCode = puiRequest[uiPduOffset];
//
//    uint16_t uiNumberB = (static_cast<uint16_t>(puiRequest[uiPduOffset + REQUEST_LENGTH]));
//
//    if (uiNumberB != (uiLength - 6))
//    {
//        uiLength = ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
//                                     puiResponse);
//    }
//    else
//    {
//        uint8_t uiBlockNumber;
//        uiLength = ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
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
//                uiLength = ResponseException(uiSlave,
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
//                uiLength = ResponseException(uiSlave,
//                                             uiFunctionCode,
//                                             MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
//                                             puiResponse);
//            }
//            // Размер блока не соответствует?
//            else if ((puiRequest[uiPduOffset + REQUEST_LENGTH] - 2) != CDataBase::m_DStruct[uiBlockNumber].Size)
//            {
//                uiLength = ResponseException(uiSlave,
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
//                    uiLength = ResponseException(uiSlave,
//                                                 uiFunctionCode,
//                                                 MODBUS_EXCEPTION_ACKNOWLEDGE,
//                                                 puiResponse);
//                }
//                else
//                {
//                    CDataBase::SetStatus(0);
//
//                    uiLength = ResponseException(uiSlave,
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
//                uiLength = ResponseException(uiSlave,
//                                             uiFunctionCode,
//                                             MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
//                                             puiResponse);
//            }
//            break;
//
//        default:
//            uiLength = ResponseException(uiSlave,
//                                         uiFunctionCode,
//                                         MODBUS_EXCEPTION_ILLEGAL_FUNCTION,
//                                         puiResponse);
//            break;
//        };
//    }

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::PollProgramming(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)

{
    uint16_t uiPduOffset = HEADER_LENGTH();
    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

//    if(!(CDataBase::GetStatus()))
//    {
//        uiLength = ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
//                                     puiResponse);
//    }
//    else
//    {
//        uiLength = ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
//        // После приёма последнего блока базы данных произойдёт перезагрузка
//        // через "PROGRAMMING_TIME + WDTO_2S", чтобы применить изменения конфигурации.
//        CPss21::SetFsmState(CPss21::PROGRAMMING_START);
//    }

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::DataBaseRead(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)
{
    std::cout << "CModbusSlave::DataBaseRead 1" << std::endl;
    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
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
        uiLength = ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else
    {
        std::cout << "CModbusSlave::DataBaseRead 4" << std::endl;
        uiLength = m_pxResources ->
                   m_pxDeviceControl ->
                   DataBaseBlockRead(&puiResponse[uiPduOffset + 3], uiBlockIndex);
        // количество байт в пакете
        puiResponse[uiPduOffset + 1] = uiLength + 1;
        uiLength ++;

        uiLength += m_pxModbusSlaveLinkLayer ->
                    ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        // номер блока базы данных
        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
        uiLength ++;



//        uiLength = m_pxResources ->
//                   m_pxDeviceControl ->
//                   ConfigurationRead(&puiResponse[uiPduOffset + 2]);
//
////    uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
////    memcpy(&puiResponse[uiPduOffset + 2], auiTempData, sizeof(auiTempData));
////    uiLength += sizeof(auiTempData);
//
//        // количество байт в прикладном сообщении массиве конфигурации, не включая остальные.
//        puiResponse[uiPduOffset + 1] = uiLength;//sizeof(auiTempData);// + 1;
//        uiLength ++;
//        uiLength += m_pxModbusSlaveLinkLayer ->
//                    ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

//        if (uiNumberB % 8)
//        {
//            std::cout << "CModbusSlave::DataBaseRead 5" << std::endl;
//            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
//        }
//        else
//        {
//            std::cout << "CModbusSlave::DataBaseRead 6" << std::endl;
//            puiResponse[uiLength++] = (uiNumberB / 8);
//        }
//        uiLength = ByteToBitPack(uiAddress,
//                                 uiNumberB,
//                                 m_puiCoils,
//                                 puiResponse,
//                                 uiLength);


//        nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
//        while (mb_mapping->message_ready)
//        {
//            usleep(COMMON_DELAY_TIME);
//            if (!nuiBusyTimeCounter--)
//            {
//                cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
//                rsp_length = response_exception(
//                                 ctx, &sft,
//                                 MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
//                break;
//            }
//        }
//
//        mb_mapping->message_ready = 1;
//        mb_mapping->current_message_address_common = puiRequest[offset + 1];
//        mb_mapping->function_code = _FC_DATA_BASE_READ;
//        rsp_length = ctx->backend->build_response_basis(&sft, rsp);
//        // (rsp_length - 2) - адрес slave.
//        // (rsp_length - 1) - функция.
//        // (rsp_length) - количество байт в ответе.
//        // (rsp_length + 1) - номер блока.
//        // (rsp_length + 2) - начало данных в ответе.
//        mb_mapping->buffer_pointer = mb_mapping->tab_auxiliary;
//
//        nuiBusyTimeCounter = MAX_MODBUS_BUFFER_BUSY_WAITING_TIME;
//        while (mb_mapping->message_ready)
//        {
//            usleep(COMMON_DELAY_TIME);
//            if (!nuiBusyTimeCounter--)
//            {
//                cout << "MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY 2" << endl;
//                rsp_length = response_exception(
//                                 ctx, &sft,
//                                 MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY, rsp);
//                break;
//            }
//        }
////                    // при чтении базы данных из модуля произошла ошибка?
////                    if (mb_mapping->current_message_address_common)
////                    {
////                        rsp_length = response_exception(
////                                         ctx, &sft,
////                                         (mb_mapping->current_message_address_common), rsp);
////
////                    mb_mapping->message_ready = 0;
////                        break;
////                    }
//
//        memcpy((&(rsp[rsp_length + 2])),
//               mb_mapping->buffer_pointer,
//               mb_mapping -> current_message_nb_common);
//
//        rsp[rsp_length++] = mb_mapping->current_message_nb_common +
//                            DATA_BASE_BLOCK_NUMBER_DATA_LENGTH;
//        rsp[rsp_length++] = puiRequest[offset + 1];
//        rsp_length += mb_mapping->current_message_nb_common;
//
//        mb_mapping->message_ready = 0;
    }

    std::cout << "CModbusSlave::DataBaseRead 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::DataBaseWrite(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)
{
    std::cout << "CModbusSlave::DataBaseWrite 1" << std::endl;
    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    std::cout << "CModbusSlave::DataBaseWrite uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusSlave::DataBaseWrite uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CModbusSlave::DataBaseWrite uiAddress "  << (int)uiAddress << std::endl;
    std::cout << "CModbusSlave::DataBaseWrite uiNumberB "  << (int)uiNumberB << std::endl;

//    puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
//    puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
//    puiRequest = m_pxModbusSlaveLinkLayer -> GetRxPdu();
//    puiResponse = m_pxModbusSlaveLinkLayer -> GetTxPdu();
//    int8_t uiSlave = m_pxModbusSlaveLinkLayer -> GetSlaveAddress();
//    int8_t uiFunctionCode = m_pxModbusSlaveLinkLayer -> GetFunctionCode();
//    uint16_t uiAddress = m_pxModbusSlaveLinkLayer -> GetDataAddress();
//    uint16_t uiNumberB = m_pxModbusSlaveLinkLayer -> GetBitNumber();

    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
    {
        std::cout << "CModbusSlave::DataBaseRead 2" << std::endl;
        uiLength = ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
    {
        std::cout << "CModbusSlave::DataBaseRead 3" << std::endl;
        std::cout << "CModbusSlave::DataBaseRead m_uiCoilsNumber "  << (int)m_uiCoilsNumber << std::endl;
        uiLength = ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        std::cout << "CModbusSlave::DataBaseRead 4" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

//        uiLength = m_pxModbusSlaveLinkLayer -> ResponseHeader(uiSlave);
//        puiResponse[uiLength++] = uiFunctionCode;

        if (uiNumberB % 8)
        {
            std::cout << "CModbusSlave::DataBaseRead 5" << std::endl;
            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
        }
        else
        {
            std::cout << "CModbusSlave::DataBaseRead 6" << std::endl;
            puiResponse[uiLength++] = (uiNumberB / 8);
        }
        uiLength = ByteToBitPack(uiAddress,
                                 uiNumberB,
                                 m_puiCoils,
                                 puiResponse,
                                 uiLength);
    }
    std::cout << "CModbusSlave::DataBaseRead 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::RequestProcessing(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiFrameLength)
{
    std::cout << "CModbusSlave::RequestProcessing 1" << std::endl;
    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiLength = uiFrameLength;

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
        uiLength = ReadCoils(puiRequest, puiResponse, uiLength);
        break;

    case _FC_READ_DISCRETE_INPUTS:
        uiLength = ReadDiscreteInputs(puiRequest, puiResponse, uiLength);
        break;

    case _FC_READ_HOLDING_REGISTERS:
        uiLength = ReadHoldingRegisters(puiRequest, puiResponse, uiLength);
        break;

    case _FC_READ_INPUT_REGISTERS:
        uiLength = ReadInputRegisters(puiRequest, puiResponse, uiLength);
        break;

    case _FC_WRITE_SINGLE_COIL:
        uiLength = WriteSingleCoil(puiRequest, puiResponse, uiLength);
        break;

    case _FC_WRITE_SINGLE_REGISTER:
        uiLength = WriteSingleRegister(puiRequest, puiResponse, uiLength);
        break;

    case _FC_READ_EXCEPTION_STATUS:
        uiLength = ReadExceptionStatus(puiRequest, puiResponse, uiLength);
        break;

    case _FC_WRITE_MULTIPLE_COILS:
        uiLength = WriteMultipleCoils(puiRequest, puiResponse, uiLength);
        break;

    case _FC_PROGRAMMING_COMPLETION_REQUEST:
        uiLength = PollProgramming(puiRequest, puiResponse, uiLength);
        break;

    case _FC_WRITE_MULTIPLE_REGISTERS:
        uiLength = WriteMultipleRegisters(puiRequest, puiResponse, uiLength);
        break;

    case _FC_REPORT_SLAVE_ID:
        uiLength = ReportSlaveID(puiRequest, puiResponse, uiLength);
        break;

    case _FC_WRITE_AND_READ_REGISTERS:
        uiLength = WriteAndReadRegisters(puiRequest, puiResponse, uiLength);
        break;

    case _FC_DATA_EXCHANGE:
        break;

    case _FC_DATA_BASE_READ:
        uiLength = DataBaseRead(puiRequest, puiResponse, uiLength);
        break;

    case _FC_DATA_BASE_WRITE:
        uiLength = DataBaseWrite(puiRequest, puiResponse, uiLength);
        break;

    case _FC_PROGRAMMING:
        uiLength = Programming(puiRequest, puiResponse, uiLength);
        break;

    default:
        std::cout << "CModbusSlave::RequestProcessing 4" << std::endl;
        uiLength = ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_FUNCTION,
                                     puiResponse);
        break;
    }

    std::cout << "CModbusSlave::RequestProcessing 5" << std::endl;
//    m_uiMessageLength = uiLength;
    uiLength = m_pxModbusSlaveLinkLayer -> Tail(puiResponse, uiLength);
    m_pxModbusSlaveLinkLayer -> SetFrameLength(uiLength);
    return uiLength;
//    return SendMessage(puiResponse, uiLength);
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
    switch (GetFsmState())
    {
    case IDDLE:
        std::cout << "CModbusSlave::Fsm IDDLE"  << std::endl;
        break;

    case START:
        std::cout << "CModbusSlave::Fsm START"  << std::endl;
        std::cout << "CModbusSlave::Fsm m_sCommunicationDeviceName" << " " << (m_sModbusSlaveLinkLayerName) << std::endl;
        SetModbusSlaveLinkLayer((CModbusSlaveLinkLayerInterface*)
                                (GetResources() ->
                                 GetCommonTaskFromMapPointer(m_sModbusSlaveLinkLayerName)));
        SetFsmState(COMMUNICATION_START);
        break;

    case READY:
        std::cout << "CModbusSlave::Fsm READY"  << std::endl;
        break;

    case STOP:
        std::cout << "CModbusSlave::Fsm STOP"  << std::endl;
        SetFsmState(START);
        break;

    case COMMUNICATION_START:
        std::cout << "CModbusSlave::Fsm COMMUNICATION_START"  << std::endl;
//        m_pxModbusSlaveLinkLayer ->
//        SetFsmState(CModbusSlaveLinkLayerInterface::COMMUNICATION_START);
        m_pxModbusSlaveLinkLayer ->
        SetFsmState(CModbusSlaveLinkLayerInterface::START);
        SetFsmState(MESSAGE_RECEIVE_WAITING);
        break;

    case MESSAGE_RECEIVE_WAITING:
//        std::cout << "CModbusSlave::Fsm MESSAGE_RECEIVE_WAITING"  << std::endl;
        if (m_pxModbusSlaveLinkLayer ->
                GetFsmState() ==
                CModbusSlaveLinkLayerInterface::COMMUNICATION_FRAME_RECEIVED)
        {
            SetFsmState(REQUEST_PROCESSING);
        }
        break;

    case REQUEST_PROCESSING:
        std::cout << "CModbusSlave::Fsm REQUEST_PROCESSING"  << std::endl;
        if (RequestProcessing(m_puiRxBuffer, m_puiTxBuffer, GetMessageLength()))
        {
            GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
            SetFsmState(BEFORE_ANSWERING_WAITING);
        }
        else
        {
            SetFsmState(MESSAGE_RECEIVE_WAITING);
        }
        break;

    case BEFORE_ANSWERING_WAITING:
//        std::cout << "CModbusSlave::Fsm BEFORE_ANSWERING_WAITING"  << std::endl;
        // Закончилось время паузы между приёмом и передачей(5 милисекунд)?
        if (GetTimerPointer() -> IsOverflow())
        {
//            GetTimerPointer() -> Set(m_uiConfirmationTimeout);
            m_pxModbusSlaveLinkLayer ->
            SetFsmState(CModbusSlaveLinkLayerInterface::COMMUNICATION_TRANSMIT_START);
            SetFsmState(AFTER_ANSWERING_WAITING);
        }
        break;

    case MESSAGE_SEND:
        std::cout << "CModbusSlave::Fsm MESSAGE_SEND"  << std::endl;
        SetFsmState(MESSAGE_RECEIVE_WAITING);
        break;

    case AFTER_ANSWERING_WAITING:
//        std::cout << "CModbusSlave::Fsm COMMUNICATION_START"  << std::endl;
        if (m_pxModbusSlaveLinkLayer ->
                GetFsmState() ==
                CModbusSlaveLinkLayerInterface::COMMUNICATION_FRAME_TRANSMITED)
        {
            m_pxModbusSlaveLinkLayer ->
            SetFsmState(CModbusSlaveLinkLayerInterface::COMMUNICATION_RECEIVE_CONTINUE);
            SetFsmState(MESSAGE_RECEIVE_WAITING);
        }
        break;

    default:
        break;
    }
}
//-------------------------------------------------------------------------------

