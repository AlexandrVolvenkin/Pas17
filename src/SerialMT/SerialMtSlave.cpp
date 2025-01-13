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
#include "SerialMtSlaveLinkLayer.h"
#include "SerialMtSpiSlaveLinkLayer.h"
#include "SerialMtSlave.h"

using namespace std;

//-------------------------------------------------------------------------------
CSerialMtSlave::CSerialMtSlave()
{
    std::cout << "CSerialMtSlave constructor"  << std::endl;
    m_pxSerialMtSlaveLinkLayer = 0;
    m_pxDeviceControl = 0;
    // получим имя класса.
    sprintf(GetTaskNamePointer(),
            "%s",
            typeid(*this).name());
    m_puiIntermediateBuff = new uint8_t[MAX_MODBUS_MESSAGE_LENGTH];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CSerialMtSlave::~CSerialMtSlave()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
uint8_t CSerialMtSlave::Init(void)
{
    std::cout << "CSerialMtSlave Init"  << std::endl;
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));
}

//-------------------------------------------------------------------------------
size_t CSerialMtSlave::GetObjectLength(void)
{
    std::cout << "CSerialMtSlave GetObjectLength"  << std::endl;
    return sizeof(*this);
}

//-------------------------------------------------------------------------------
void CSerialMtSlave::SetSerialMtSlaveLinkLayerName(std::string sName)
{
    m_sSerialMtSlaveLinkLayerName = sName;
}

//-------------------------------------------------------------------------------
void CSerialMtSlave::SetSerialMtSlaveLinkLayer(CSerialMtSlaveLinkLayer* pxSerialMtSlaveLinkLayer)
{
    m_pxSerialMtSlaveLinkLayer = pxSerialMtSlaveLinkLayer;
}

//-------------------------------------------------------------------------------
void CSerialMtSlave::SetDeviceControlName(std::string sName)
{
    m_sDeviceControlName = sName;
}

//-------------------------------------------------------------------------------
void CSerialMtSlave::SetDeviceControl(CDeviceControl* pxDeviceControl)
{
    m_pxDeviceControl = pxDeviceControl;
}

//-------------------------------------------------------------------------------
CDeviceControl* CSerialMtSlave::GetDeviceContro(void)
{
    return m_pxDeviceControl;
}

//-------------------------------------------------------------------------------
const char *CSerialMtSlave::ModbusStringError(int errnum)
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
void CSerialMtSlave::ModbusWorkingArraysInit(void)
{
    std::cout << "CSerialMtSlave ModbusWorkingArraysInit 1"  << std::endl;
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
uint16_t CSerialMtSlave::ByteToBitPack(uint16_t uiAddress,
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
uint16_t CSerialMtSlave::ReadCoils(void)
{
    std::cout << "CSerialMtSlave::ReadCoils 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    std::cout << "CSerialMtSlave::ReadCoils uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CSerialMtSlave::ReadCoils uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CSerialMtSlave::ReadCoils uiAddress "  << (int)uiAddress << std::endl;
    std::cout << "CSerialMtSlave::ReadCoils uiNumberB "  << (int)uiNumberB << std::endl;

    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
    {
        std::cout << "CSerialMtSlave::ReadCoils 2" << std::endl;
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
    {
        std::cout << "CSerialMtSlave::ReadCoils 3" << std::endl;
        std::cout << "CSerialMtSlave::ReadCoils m_uiCoilsNumber "  << (int)m_uiCoilsNumber << std::endl;
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        std::cout << "CSerialMtSlave::ReadCoils 4" << std::endl;
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        if (uiNumberB % 8)
        {
            std::cout << "CSerialMtSlave::ReadCoils 5" << std::endl;
            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
        }
        else
        {
            std::cout << "CSerialMtSlave::ReadCoils 6" << std::endl;
            puiResponse[uiLength++] = (uiNumberB / 8);
        }
        uiLength = ByteToBitPack(uiAddress,
                                 uiNumberB,
                                 m_puiCoils,
                                 puiResponse,
                                 uiLength);
    }
    std::cout << "CSerialMtSlave::ReadCoils 7" << std::endl;
    return uiLength;
}

////-------------------------------------------------------------------------------
//uint16_t CSerialMtSlave::ReadCoils(void)
//{
//    std::cout << "CSerialMtSlave::ReadCoils 1" << std::endl;
//
//    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
//    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
//    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
//    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();
//
//    int8_t uiSlave = puiRequest[uiPduOffset - 1];
//    int8_t uiFunctionCode = puiRequest[uiPduOffset];
//    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
//                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));
//
//    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
//                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));
//
//    std::cout << "CSerialMtSlave::ReadCoils uiSlave "  << (int)uiSlave << std::endl;
//    std::cout << "CSerialMtSlave::ReadCoils uiFunctionCode "  << (int)uiFunctionCode << std::endl;
//    std::cout << "CSerialMtSlave::ReadCoils uiAddress "  << (int)uiAddress << std::endl;
//    std::cout << "CSerialMtSlave::ReadCoils uiNumberB "  << (int)uiNumberB << std::endl;
//
//    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
//    {
//        std::cout << "CSerialMtSlave::ReadCoils 2" << std::endl;
//        uiLength = m_pxSerialMtSlaveLinkLayer ->
//                   ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
//                                     puiResponse);
//    }
//    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
//    {
//        std::cout << "CSerialMtSlave::ReadCoils 3" << std::endl;
//        std::cout << "CSerialMtSlave::ReadCoils m_uiCoilsNumber "  << (int)m_uiCoilsNumber << std::endl;
//        uiLength = m_pxSerialMtSlaveLinkLayer ->
//                   ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
//                                     puiResponse);
//    }
//    else
//    {
//        std::cout << "CSerialMtSlave::ReadCoils 4" << std::endl;
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
//        uiLength = m_pxSerialMtSlaveLinkLayer ->
//                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
//
////        if (uiNumberB % 8)
////        {
////            std::cout << "CSerialMtSlave::ReadCoils 5" << std::endl;
////            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
////        }
////        else
////        {
////            std::cout << "CSerialMtSlave::ReadCoils 6" << std::endl;
////            puiResponse[uiLength++] = (uiNumberB / 8);
////        }
////        uiLength = ByteToBitPack(uiAddress,
////                                 uiNumberB,
////                                 m_puiCoils,
////                                 puiResponse,
////                                 uiLength);
//    }
//    std::cout << "CSerialMtSlave::ReadCoils 7" << std::endl;
//    return uiLength;
//}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::ReadDiscreteInputs(void)
{
    std::cout << "CSerialMtSlave::ReadDiscreteInputs 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiDiscreteInputsNumber)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        std::cout << "CSerialMtSlave::ReadDiscreteInputs 4" << std::endl;
        uiLength = m_pxSerialMtSlaveLinkLayer ->
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
uint16_t CSerialMtSlave::ReadHoldingRegisters(void)
{
    std::cout << "CSerialMtSlave::ReadHoldingRegisters 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    std::cout << "CSerialMtSlave::ReadHoldingRegisters uiAddress "  << (int)uiAddress << std::endl;
    std::cout << "CSerialMtSlave::ReadHoldingRegisters uiNumberB "  << (int)uiNumberB << std::endl;

    if (uiNumberB < 1 || MODBUS_MAX_READ_REGISTERS < uiNumberB)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiHoldingRegistersNumber)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
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

    std::cout << "CSerialMtSlave::ReadHoldingRegisters uiLength "  << (int)uiLength << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::ReadInputRegisters(void)
{
    std::cout << "CSerialMtSlave::ReadInputRegisters 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_READ_REGISTERS < uiNumberB)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiInputRegistersNumber)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
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
uint16_t CSerialMtSlave::WriteSingleCoil(void)
{
    std::cout << "CSerialMtSlave::WriteSingleCoil 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    if (uiAddress >= m_uiCoilsNumber)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
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
            uiLength = m_pxSerialMtSlaveLinkLayer ->
                       ResponseException(uiSlave,
                                         uiFunctionCode,
                                         MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                         puiResponse);
        }

    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::WriteSingleRegister(void)
{
    std::cout << "CSerialMtSlave::WriteSingleRegister 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    if (uiAddress >= m_uiHoldingRegistersNumber)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
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
uint16_t CSerialMtSlave::WriteMultipleCoils(void)
{
    std::cout << "CSerialMtSlave::WriteMultipleCoils 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_WRITE_BITS < uiNumberB)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        SetBytesFromBits(m_puiCoils, uiAddress, uiNumberB, &puiRequest[uiPduOffset + 6]);

        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
        /* 4 to copy the bit address (2) and the quantity of bits */
        memcpy(puiResponse + uiLength, puiRequest + uiLength, 4);
        uiLength += 4;
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::WriteMultipleRegisters(void)
{
    std::cout << "CSerialMtSlave::WriteMultipleRegisters 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_WRITE_REGISTERS < uiNumberB)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiHoldingRegistersNumber)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
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

        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
        /* 4 to copy the bit address (2) and the quantity of bits */
        memcpy(puiResponse + uiLength, puiRequest + uiLength, 4);
        uiLength += 4;

    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::ReadExceptionStatus(void)
{
//    errno = ENOPROTOOPT;
    return -1;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::WriteAndReadRegisters(void)
{

    return 0;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::ReportSlaveID(void)
{
    std::cout << "CSerialMtSlave::ReportSlaveID 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    std::cout << "CSerialMtSlave::ReportSlaveID uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CSerialMtSlave::ReportSlaveID uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    std::cout << "CSerialMtSlave::ReportSlaveID 4" << std::endl;

    CDeviceControl* pxDeviceControl =
        (CDeviceControl*)GetResources() ->
        GetTaskPointerByNameFromMap("DeviceControlRtuUpperLevel");

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
    uiLength += m_pxSerialMtSlaveLinkLayer ->
                ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

    std::cout << "CSerialMtSlave::ReportSlaveID 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::PollProgramming(void)

{
    std::cout << "CSerialMtSlave::PollProgramming 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    CDeviceControl* pxDeviceControl =
        (CDeviceControl*)GetResources() ->
        GetTaskPointerByNameFromMap("DeviceControlRtuUpperLevel");

    // Ожидаем окончания записи автоматом устройства хранения.
    // Устройство хранения закончило запись успешно?
    if (pxDeviceControl -> IsDoneOk())
    {
        std::cout << "CSerialMtSlave::PollProgramming 2" << std::endl;

        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

//        // номер блока базы данных
//        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
//        uiLength ++;

//        uiLength = m_pxSerialMtSlaveLinkLayer ->
//        ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ACKNOWLEDGE,
//                                     puiResponse);
    }
//    // Устройство хранения закончило запись не успешно?
//    else if (pxDataStoreFileSystem -> IsDoneError())
//    {
//        uiLength = m_pxSerialMtSlaveLinkLayer ->
    //    ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
//                                     puiResponse);
//    }
    else
    {
        std::cout << "CSerialMtSlave::PollProgramming 3" << std::endl;;

        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
                                     puiResponse);
    }

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::DataBaseRead(void)
{
    std::cout << "CSerialMtSlave::DataBaseRead 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint8_t uiBlockIndex = puiRequest[uiPduOffset + 1];

    std::cout << "CSerialMtSlave::DataBaseRead uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CSerialMtSlave::DataBaseRead uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CSerialMtSlave::DataBaseRead uiBlockIndex "  << (int)uiBlockIndex << std::endl;

    if ((uiBlockIndex < 0) ||
            (uiBlockIndex > (CDataStore::MAX_BLOCKS_NUMBER - 1)))
    {
        std::cout << "CSerialMtSlave::DataBaseRead 2" << std::endl;
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else
    {
        std::cout << "CSerialMtSlave::DataBaseRead 4" << std::endl;

        m_uiFunctionCode = uiFunctionCode;

//        CDataContainerDataBase xDataContainer;
//        xDataContainer.m_uiFsmCommandState = CDeviceControl::DATA_BASE_BLOCK_READ;
//        xDataContainer.m_uiDataIndex = uiBlockIndex;
//        m_pxDeviceControl ->
//        SetTaskData(&xDataContainer);

        m_pxOperatingDataContainer -> m_uiFsmCommandState = CDeviceControl::DATA_BASE_BLOCK_READ;
        m_pxOperatingDataContainer -> m_uiDataIndex = uiBlockIndex;
        m_pxOperatingDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;
        m_pxDeviceControl ->
        SetTaskData(m_pxOperatingDataContainer);

//        uint8_t m_uiFsmCommandState;
//        uint8_t m_uiFsmOperationStatus;
//        uint16_t m_uiDataIndex;
//        uint8_t* m_puiDataPointer;
//        uint32_t m_uiDataOffset;
//        uint32_t m_uiDataLength;
//        m_pxDeviceControlLink ->
//        GetDataContainerPointer() ->
//        SetContainerData(CDeviceControl::DATA_BASE_BLOCK_READ,
//                         uiBlockIndex,
//                         m_puiIntermediateBuff,
//                         0,
//                         0);
//
//        ((CDeviceControl*)(m_pxDeviceControlLink ->
//                           GetTaskPerformerPointer())) ->
//        SetCommandDataLink(m_pxDeviceControlLink);

        uiLength = 5;

        // не нуль, ожидаем ответ
        m_uiLength = uiLength;
    }

    std::cout << "CSerialMtSlave::DataBaseRead 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::DataBaseWrite(void)
{
//    rsp_length == 2. адрес slave + функция
    // (rsp_length - 2) - адрес slave.
    // (rsp_length - 1) - функция.
//        // (rsp_length) - номер блока.
    // (rsp_length + 1) - начало данных.

    std::cout << "CSerialMtSlave::DataBaseWrite 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint8_t uiBlockIndex = puiRequest[uiPduOffset + 1];

    std::cout << "CSerialMtSlave::DataBaseWrite uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CSerialMtSlave::DataBaseWrite uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CSerialMtSlave::DataBaseWrite uiBlockIndex "  << (int)uiBlockIndex << std::endl;


    if ((uiBlockIndex < 0) ||
            (uiBlockIndex > (CDataStore::MAX_BLOCKS_NUMBER - 1)))
    {
        std::cout << "CSerialMtSlave::DataBaseWrite 2" << std::endl;
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {

        std::cout << "CSerialMtSlave::DataBaseWrite 4" << std::endl;

        memcpy(m_puiIntermediateBuff,
               &puiRequest[uiPduOffset + 2],
               MAX_MODBUS_MESSAGE_LENGTH);

        m_pxOperatingDataContainer -> m_uiFsmCommandState = CDeviceControl::DATA_BASE_BLOCK_START_WRITE;
        m_pxOperatingDataContainer -> m_uiDataIndex = uiBlockIndex;
        m_pxOperatingDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;
        m_pxDeviceControl ->
        SetTaskData(m_pxOperatingDataContainer);

//        m_pxDeviceControlLink ->
//        GetDataContainerPointer() ->
//        SetContainerData(CDeviceControl::DATA_BASE_BLOCK_START_WRITE,
//                         uiBlockIndex,
//                         m_puiIntermediateBuff,
//                         0,
//                         0);
//
//        // отправим сообщение задаче-исполнителю
//        ((CDeviceControl*)(m_pxDeviceControlLink ->
//                           GetTaskPerformerPointer())) ->
//        SetCommandDataLink(m_pxDeviceControlLink);

        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        // номер блока базы данных
        puiResponse[uiPduOffset + 1] = 1;
        uiLength ++;
        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
        uiLength ++;

        // не ожидаем ответа
        m_uiLength = 0;
    }

    std::cout << "CSerialMtSlave::DataBaseWrite 7" << std::endl;
    return uiLength;
}


//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::RequestProcessing(void)
{
    std::cout << "CSerialMtSlave::RequestProcessing 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    std::cout << "CSerialMtSlave::RequestProcessing uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CSerialMtSlave::RequestProcessing uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    /* Filter on the Modbus unit identifier (slave) in RTU mode */
    if (uiSlave != m_uiOwnAddress && uiSlave != MODBUS_BROADCAST_ADDRESS)
    {
        std::cout << "CSerialMtSlave::RequestProcessing 2" << std::endl;
        return 0;
    }

    // не ожидаем ответа
    m_uiLength = 0;

    switch (uiFunctionCode)
    {
        std::cout << "CSerialMtSlave::RequestProcessing 3" << std::endl;
    case _FC_READ_COILS:
        std::cout << "CSerialMtSlave::RequestProcessing _FC_READ_COILS"  << std::endl;
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
        break;

    default:
        std::cout << "CSerialMtSlave::RequestProcessing 4" << std::endl;
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_FUNCTION,
                                     puiResponse);
        break;
    }

    std::cout << "CSerialMtSlave::RequestProcessing 5" << std::endl;
    uiLength = m_pxSerialMtSlaveLinkLayer -> Tail(puiResponse, uiLength);
    m_pxSerialMtSlaveLinkLayer -> SetFrameLength(uiLength);
    return uiLength;
}









//-------------------------------------------------------------------------------
////AnswerProcessing
////-------------------------------------------------------------------------------
//uint16_t CSerialMtSlave::ReadCoilsAnswer(void)
//{
//    std::cout << "CSerialMtSlave::ReadCoilsAnswer 1" << std::endl;
//
//    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
//    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
//    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
//    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();
//
//    int8_t uiSlave = puiRequest[uiPduOffset - 1];
//    int8_t uiFunctionCode = puiRequest[uiPduOffset];
//    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
//                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));
//
//    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
//                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));
//
//    std::cout << "CSerialMtSlave::ReadCoilsAnswer uiSlave "  << (int)uiSlave << std::endl;
//    std::cout << "CSerialMtSlave::ReadCoilsAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
//    std::cout << "CSerialMtSlave::ReadCoilsAnswer uiAddress "  << (int)uiAddress << std::endl;
//    std::cout << "CSerialMtSlave::ReadCoilsAnswer uiNumberB "  << (int)uiNumberB << std::endl;
//
//    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
//    {
//        std::cout << "CSerialMtSlave::ReadCoilsAnswer 2" << std::endl;
//        uiLength = m_pxSerialMtSlaveLinkLayer ->
//                   ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
//                                     puiResponse);
//    }
//    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
//    {
//        std::cout << "CSerialMtSlave::ReadCoilsAnswer 3" << std::endl;
//        std::cout << "CSerialMtSlave::ReadCoilsAnswer m_uiCoilsNumber "  << (int)m_uiCoilsNumber << std::endl;
//        uiLength = m_pxSerialMtSlaveLinkLayer ->
//                   ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
//                                     puiResponse);
//    }
//    else
//    {
//        std::cout << "CSerialMtSlave::ReadCoilsAnswer 4" << std::endl;
//        uiLength = m_pxSerialMtSlaveLinkLayer ->
//                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
//
//        if (uiNumberB % 8)
//        {
//            std::cout << "CSerialMtSlave::ReadCoilsAnswer 5" << std::endl;
//            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
//        }
//        else
//        {
//            std::cout << "CSerialMtSlave::ReadCoilsAnswer 6" << std::endl;
//            puiResponse[uiLength++] = (uiNumberB / 8);
//        }
//        uiLength = ByteToBitPack(uiAddress,
//                                 uiNumberB,
//                                 m_puiCoils,
//                                 puiResponse,
//                                 uiLength);
//    }
//    std::cout << "CSerialMtSlave::ReadCoilsAnswer 7" << std::endl;
//    return uiLength;
//
//}
//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::ReadCoilsAnswer(void)
{
    std::cout << "CSerialMtSlave::ReadCoilsAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    std::cout << "CSerialMtSlave::ReadCoilsAnswer uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CSerialMtSlave::ReadCoilsAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CSerialMtSlave::ReadCoilsAnswer uiAddress "  << (int)uiAddress << std::endl;
    std::cout << "CSerialMtSlave::ReadCoilsAnswer uiNumberB "  << (int)uiNumberB << std::endl;

    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
    {
        std::cout << "CSerialMtSlave::ReadCoilsAnswer 2" << std::endl;
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
    {
        std::cout << "CSerialMtSlave::ReadCoilsAnswer 3" << std::endl;
        std::cout << "CSerialMtSlave::ReadCoilsAnswer m_uiCoilsNumber "  << (int)m_uiCoilsNumber << std::endl;
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        std::cout << "CSerialMtSlave::ReadCoilsAnswer 4" << std::endl;
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        if (uiNumberB % 8)
        {
            std::cout << "CSerialMtSlave::ReadCoilsAnswer 5" << std::endl;
            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
        }
        else
        {
            std::cout << "CSerialMtSlave::ReadCoilsAnswer 6" << std::endl;
            puiResponse[uiLength++] = (uiNumberB / 8);
        }
        uiLength = ByteToBitPack(uiAddress,
                                 uiNumberB,
                                 m_puiCoils,
                                 puiResponse,
                                 uiLength);
    }
    std::cout << "CSerialMtSlave::ReadCoilsAnswer 7" << std::endl;
    return uiLength;

}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::ReadDiscreteInputsAnswer(void)
{
    std::cout << "CSerialMtSlave::ReadDiscreteInputsAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiDiscreteInputsNumber)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        std::cout << "CSerialMtSlave::ReadDiscreteInputsAnswer 4" << std::endl;
        uiLength = m_pxSerialMtSlaveLinkLayer ->
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
uint16_t CSerialMtSlave::ReadHoldingRegistersAnswer(void)
{
    std::cout << "CSerialMtSlave::ReadHoldingRegistersAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    std::cout << "CSerialMtSlave::ReadHoldingRegistersAnswer uiAddress "  << (int)uiAddress << std::endl;
    std::cout << "CSerialMtSlave::ReadHoldingRegistersAnswer uiNumberB "  << (int)uiNumberB << std::endl;

    if (uiNumberB < 1 || MODBUS_MAX_READ_REGISTERS < uiNumberB)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiHoldingRegistersNumber)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        std::cout << "CSerialMtSlave::ReadHoldingRegistersAnswer 4" << std::endl;
        uiLength = m_pxSerialMtSlaveLinkLayer ->
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

    std::cout << "CSerialMtSlave::ReadHoldingRegistersAnswer uiLength "  << (int)uiLength << std::endl;
    return uiLength;

}//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::ReadInputRegistersAnswer(void)
{
    std::cout << "CSerialMtSlave::ReadInputRegistersAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_READ_REGISTERS < uiNumberB)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiInputRegistersNumber)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
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
uint16_t CSerialMtSlave::WriteSingleCoilAnswer(void)
{
    std::cout << "CSerialMtSlave::WriteSingleCoilAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    if (uiAddress >= m_uiCoilsNumber)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
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
            uiLength = m_pxSerialMtSlaveLinkLayer ->
                       ResponseException(uiSlave,
                                         uiFunctionCode,
                                         MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                         puiResponse);
        }

    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::WriteSingleRegisterAnswer(void)
{
    std::cout << "CSerialMtSlave::WriteSingleRegisterAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    if (uiAddress >= m_uiHoldingRegistersNumber)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
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
uint16_t CSerialMtSlave::WriteMultipleCoilsAnswer(void)
{
    std::cout << "CSerialMtSlave::WriteMultipleCoilsAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_WRITE_BITS < uiNumberB)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        SetBytesFromBits(m_puiCoils, uiAddress, uiNumberB, &puiRequest[uiPduOffset + 6]);

        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
        /* 4 to copy the bit address (2) and the quantity of bits */
        memcpy(puiResponse + uiLength, puiRequest + uiLength, 4);
        uiLength += 4;
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::WriteMultipleRegistersAnswer(void)
{
    std::cout << "CSerialMtSlave::WriteMultipleRegistersAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    uint16_t uiNumberB = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

    if (uiNumberB < 1 || MODBUS_MAX_WRITE_REGISTERS < uiNumberB)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiHoldingRegistersNumber)
    {
        uiLength = m_pxSerialMtSlaveLinkLayer ->
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

        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
        /* 4 to copy the bit address (2) and the quantity of bits */
        memcpy(puiResponse + uiLength, puiRequest + uiLength, 4);
        uiLength += 4;
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::ReadExceptionStatusAnswer(void)
{
//    errno = ENOPROTOOPT;
    return -1;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::WriteAndReadRegistersAnswer(void)
{

    return 0;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::ReportSlaveIDAnswer(void)
{
    std::cout << "CSerialMtSlave::ReportSlaveIDAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    std::cout << "CSerialMtSlave::ReportSlaveIDAnswer uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CSerialMtSlave::ReportSlaveIDAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    std::cout << "CSerialMtSlave::ReportSlaveIDAnswer 4" << std::endl;

    CDeviceControl* pxDeviceControl =
        (CDeviceControl*)GetResources() ->
        GetTaskPointerByNameFromMap("DeviceControlRtuUpperLevel");

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
    uiLength += m_pxSerialMtSlaveLinkLayer ->
                ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

    std::cout << "CSerialMtSlave::ReportSlaveIDAnswer 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::PollProgrammingAnswer(void)
{
    std::cout << "CSerialMtSlave::PollProgrammingAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

//    CDeviceControl* pxDeviceControl =
//        (CDeviceControl*)GetResources() ->
//        GetTaskPointerByNameFromMap("DeviceControl");

    CDataStore* pxDataStoreFileSystemAnswer =
        (CDataStore*)GetResources() ->
        GetTaskPointerByNameFromMap("DataStoreFileSystemAnswer");


//    // Устройство хранения в прцессе записи?
//    if (m_pxDeviceControl ->
//            GetFsmOperationStatus() == CDfa::IN_PROGRESS)
//    {
//        std::cout << "CSerialMtSlave::PollProgrammingAnswer 2" << std::endl;
//
//        uiLength = m_pxSerialMtSlaveLinkLayer ->
//    ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
//                                     puiResponse);
//    }
//    // Устройство хранения закончило запись успешно?
//    else if (m_pxDeviceControl ->
//             GetFsmOperationStatus() == CDfa::DONE_SUCCESSFULLY)
//    {
//        std::cout << "CSerialMtSlave::PollProgrammingAnswer 3" << std::endl;
//
//        uiLength = m_pxSerialMtSlaveLinkLayer ->
//                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
//
//    }
//    // Устройство хранения закончило запись не успешно?
//    else if (m_pxDeviceControl ->
//             GetFsmOperationStatus() == CDfa::ERROR_OCCURED)
//    {
//        std::cout << "CSerialMtSlave::PollProgrammingAnswer 4" << std::endl;
//
//        uiLength = m_pxSerialMtSlaveLinkLayer ->
//                      ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ACKNOWLEDGE,
//                                     puiResponse);
//    }

    // Ожидаем окончания записи автоматом устройства хранения.
    // Устройство хранения закончило запись успешно?
    if (pxDataStoreFileSystemAnswer -> IsDoneOk())
    {
        std::cout << "CSerialMtSlave::PollProgrammingAnswer 2" << std::endl;

        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

//        // номер блока базы данных
//        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
//        uiLength ++;

//        uiLength = m_pxSerialMtSlaveLinkLayer ->
//        ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_ACKNOWLEDGE,
//                                     puiResponse);
    }
//    // Устройство хранения закончило запись не успешно?
//    else if (pxDataStoreFileSystemAnswer -> IsDoneError())
//    {
//        uiLength = m_pxSerialMtSlaveLinkLayer ->
    //    ResponseException(uiSlave,
//                                     uiFunctionCode,
//                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
//                                     puiResponse);
//    }
    else
    {
        std::cout << "CSerialMtSlave::PollProgrammingAnswer 3" << std::endl;

//        uiLength = m_pxSerialMtSlaveLinkLayer ->
//                    ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
                                     puiResponse);
    }

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::DataBaseReadAnswer(void)
{
    std::cout << "CSerialMtSlave::DataBaseReadAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint8_t uiBlockIndex = puiRequest[uiPduOffset + 1];

    std::cout << "CSerialMtSlave::DataBaseReadAnswer uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CSerialMtSlave::DataBaseReadAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CSerialMtSlave::DataBaseReadAnswer uiBlockIndex "  << (int)uiBlockIndex << std::endl;

    if ((uiBlockIndex < 0) ||
            (uiBlockIndex > (CDataStore::MAX_BLOCKS_NUMBER - 1)))
    {
        std::cout << "CSerialMtSlave::DataBaseReadAnswer 2" << std::endl;
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else
    {
//        CDataContainerDataBase* pxDataContainer =
//            m_pxDeviceControl ->
//            GetTaskData();
//        uiLength = pxDataContainer -> m_uiDataLength;

        m_pxDeviceControl ->
        GetTaskData(m_pxOperatingDataContainer);

        uiLength = m_pxOperatingDataContainer -> m_uiDataLength;

//        uiLength = m_pxDeviceControlLink ->
//                   GetDataContainerPointer() ->
//                   GetDataLength();
        std::cout << "CSerialMtSlave::DataBaseReadAnswer uiLength "  << (int)uiLength << std::endl;

        memcpy(&puiResponse[uiPduOffset + 3],
               (m_pxOperatingDataContainer -> m_puiDataPointer),
               uiLength);

        puiResponse[uiPduOffset + 1] = uiLength + 1;
        uiLength ++;

        uiLength += m_pxSerialMtSlaveLinkLayer ->
                    ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        // номер блока базы данных
        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
        uiLength ++;
    }

    std::cout << "CSerialMtSlave::DataBaseReadAnswer 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::DataBaseWriteAnswer(void)
{
    std::cout << "CSerialMtSlave::DataBaseWriteAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint8_t uiBlockIndex = puiRequest[uiPduOffset + 1];

    std::cout << "CSerialMtSlave::DataBaseWriteAnswer uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CSerialMtSlave::DataBaseWriteAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    std::cout << "CSerialMtSlave::DataBaseWriteAnswer uiBlockIndex "  << (int)uiBlockIndex << std::endl;


    if ((uiBlockIndex < 0) ||
            (uiBlockIndex > (CDataStore::MAX_BLOCKS_NUMBER - 1)))
    {
        std::cout << "CSerialMtSlave::DataBaseWriteAnswer 2" << std::endl;
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        std::cout << "CSerialMtSlave::DataBaseWriteAnswer 3" << std::endl;
        //        CDataStore* pxDataStoreFileSystem =
//            (CDataStore*)GetResources() ->
//            GetTaskPointerByNameFromMap("DataStoreFileSystemAnswer");
//
//        uint16_t uiLength = pxDataStoreFileSystem ->
//                            GetBlockLength(uiBlockIndex);
//
//        memcpy(m_puiIntermediateBuff,
//               &puiRequest[uiPduOffset + 2],
//               uiLength);
//
//        // Сохраним блок БД.
//        std::cout << "CSerialMtSlave::DataBaseWriteAnswer 3" << std::endl;
//        pxDataStoreFileSystem ->
//        WriteBlock(m_puiIntermediateBuff,
//                   uiLength,
//                   uiBlockIndex);

        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        // номер блока базы данных
        puiResponse[uiPduOffset + 1] = 1;
        uiLength ++;
        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
        uiLength ++;
    }

    std::cout << "CSerialMtSlave::DataBaseWriteAnswer 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtSlave::AnswerProcessing(void)
{
    std::cout << "CSerialMtSlave::AnswerProcessing 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    std::cout << "CSerialMtSlave::AnswerProcessing uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CSerialMtSlave::AnswerProcessing uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    /* Filter on the Modbus unit identifier (slave) in RTU mode */
    if (uiSlave != m_uiOwnAddress && uiSlave != MODBUS_BROADCAST_ADDRESS)
    {
        std::cout << "CSerialMtSlave::AnswerProcessing 2" << std::endl;
        return 0;
    }

    // проверяем сохранённый локально текущий код функции.
    switch (m_uiFunctionCode)
    {
        std::cout << "CSerialMtSlave::AnswerProcessing 3" << std::endl;
    case _FC_READ_COILS:
        std::cout << "CSerialMtSlave::Answer _FC_READ_COILS"  << std::endl;
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
        break;

    default:
        std::cout << "CSerialMtSlave::AnswerProcessing 4" << std::endl;
        uiLength = m_pxSerialMtSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_FUNCTION,
                                     puiResponse);
        break;
    }

    std::cout << "CSerialMtSlave::AnswerProcessing 5" << std::endl;
    uiLength = m_pxSerialMtSlaveLinkLayer -> Tail(puiResponse, uiLength);
    m_pxSerialMtSlaveLinkLayer -> SetFrameLength(uiLength);
    return uiLength;
}

//-------------------------------------------------------------------------------
void CSerialMtSlave::SetByteFromBits(uint8_t *dest, uint16_t index, const uint8_t value)
{
    uint16_t i;

    for (i=0; i<8; i++)
    {
        dest[index+i] = (value & (1 << i)) ? 1 : 0;
    }
}

//-------------------------------------------------------------------------------
void CSerialMtSlave::SetBytesFromBits(uint8_t *dest, uint16_t index, uint16_t nb_bits,
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
uint8_t CSerialMtSlave::GetByteFromBits(const uint8_t *src, uint16_t index,
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
float CSerialMtSlave::GetFloat(const uint16_t *src)
{
    float f = 0.0f;
    uint32_t i;

    i = (((uint32_t)src[1]) << 16) + src[0];
    memcpy(&f, &i, sizeof(float));

    return f;
}

//-------------------------------------------------------------------------------
void CSerialMtSlave::SetFloat(float f, uint16_t *dest)
{
    uint32_t i = 0;

    memcpy(&i, &f, sizeof(uint32_t));
    dest[0] = (uint16_t)i;
    dest[1] = (uint16_t)(i >> 16);
}

//-------------------------------------------------------------------------------
uint8_t CSerialMtSlave::Fsm(void)
{
//    std::cout << "CSerialMtSlave::Fsm 1" << endl;
    uint8_t uiReadyTaskCounter = 0;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CSerialMtSlave::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CSerialMtSlave::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CSerialMtSlave::Fsm START"  << std::endl;
        std::cout << "CSerialMtSlave::Fsm m_sSerialMtSlaveLinkLayerName" << " " << (m_sSerialMtSlaveLinkLayerName) << std::endl;
        Init();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
//        std::cout << "CSerialMtSlave::Fsm INIT 1"  << std::endl;
    {
        CTaskInterface* pxTask =
            GetResources() ->
            GetTaskPointerByNameFromMap(m_sSerialMtSlaveLinkLayerName);

        if (pxTask != 0)
        {
//                std::cout << "CSerialMtSlave::Fsm INIT 2"  << std::endl;
            if (pxTask -> GetFsmState() >= READY)
            {
//                    std::cout << "CSerialMtSlave::Fsm INIT 3"  << std::endl;
                SetSerialMtSlaveLinkLayer((CSerialMtSlaveLinkLayer*)pxTask);
                uiReadyTaskCounter += 1;
            }
        }
        else
        {
//                std::cout << "CSerialMtSlave::Fsm INIT 4"  << std::endl;
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CSerialMtSlave::Fsm INIT 5"  << std::endl;
                SetFsmState(STOP);
            }
        }
    }


    {
        CTaskInterface* pxTask =
            GetResources() ->
            GetTaskPointerByNameFromMap(m_sDeviceControlName);

        if (pxTask != 0)
        {
//                std::cout << "CSerialMtSlave::Fsm INIT 2"  << std::endl;
            if (pxTask -> GetFsmState() >= READY)
            {
//                    std::cout << "CSerialMtSlave::Fsm INIT 3"  << std::endl;
                SetDeviceControl((CDeviceControl*)pxTask);
                uiReadyTaskCounter += 1;
            }
        }
        else
        {
//                std::cout << "CSerialMtSlave::Fsm INIT 4"  << std::endl;
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CSerialMtSlave::Fsm INIT 5"  << std::endl;
                SetFsmState(STOP);
            }
        }
    }

    if (uiReadyTaskCounter > 1)
    {
        std::cout << "CSerialMtSlave::Fsm INIT 9"  << std::endl;
        SetFsmState(READY);
    }
    break;

    case READY:
        std::cout << "CSerialMtSlave::Fsm READY"  << std::endl;
        SetFsmState(COMMUNICATION_START);
        break;

    case DONE_OK:
        std::cout << "CSerialMtSlave::Fsm DONE_OK"  << std::endl;
        SetFsmOperationStatus(DONE_OK);
        break;

    case DONE_ERROR:
        std::cout << "CSerialMtSlave::Fsm DONE_ERROR"  << std::endl;
        SetFsmOperationStatus(DONE_ERROR);
        break;

    case COMMUNICATION_START:
        std::cout << "CSerialMtSlave::Fsm COMMUNICATION_START"  << std::endl;
        m_pxOperatingDataContainer -> m_uiFsmCommandState =
            CSerialMtSlaveLinkLayer::COMMUNICATION_START;
        m_pxSerialMtSlaveLinkLayer ->
        SetTaskData(m_pxOperatingDataContainer);
        SetFsmState(MESSAGE_RECEIVE_WAITING);
        break;

    case COMMUNICATION_RECEIVE_START:
        std::cout << "CSerialMtSlave::Fsm COMMUNICATION_RECEIVE_START"  << std::endl;
        m_pxOperatingDataContainer -> m_uiFsmCommandState =
            CSerialMtSlaveLinkLayer::COMMUNICATION_RECEIVE_START;
        m_pxSerialMtSlaveLinkLayer ->
        SetTaskData(m_pxOperatingDataContainer);
        SetFsmState(MESSAGE_RECEIVE_WAITING);
        break;

    case COMMUNICATION_RECEIVE_CONTINUE:
        std::cout << "CSerialMtSlave::Fsm COMMUNICATION_RECEIVE_CONTINUE"  << std::endl;
        m_pxOperatingDataContainer -> m_uiFsmCommandState =
            CSerialMtSlaveLinkLayer::COMMUNICATION_RECEIVE_CONTINUE;
        m_pxSerialMtSlaveLinkLayer ->
        SetTaskData(m_pxOperatingDataContainer);
        SetFsmState(MESSAGE_RECEIVE_WAITING);
        break;

    case MESSAGE_RECEIVE_WAITING:
//        std::cout << "CSerialMtSlave::Fsm MESSAGE_RECEIVE_WAITING"  << std::endl;
    {
        m_pxSerialMtSlaveLinkLayer ->
        GetTaskData(m_pxOperatingDataContainer);

        uint8_t uiFsmState = m_pxOperatingDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CSerialMtSlave::Fsm MESSAGE_RECEIVE_WAITING 2"  << std::endl;
            SetFsmState(REQUEST_PROCESSING);
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CSerialMtSlave::Fsm MESSAGE_RECEIVE_WAITING 3"  << std::endl;
            SetFsmState(COMMUNICATION_START);
        }
    }
    break;

    case REQUEST_PROCESSING:
        std::cout << "CSerialMtSlave::Fsm REQUEST_PROCESSING"  << std::endl;
        if (RequestProcessing())
        {
            // ожидаем ответ от асинхронной функции модбас?
            if (m_uiLength)
            {
                std::cout << "CSerialMtSlave::Fsm REQUEST_PROCESSING 2"  << std::endl;
                GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
                SetFsmState(ANSWER_PROCESSING_WAITING);
            }
            else
            {
                // функция модбас не асинхронная и не требует ожидать результат её работы.
                // идём сразу на передачу ответа модбас
                std::cout << "CSerialMtSlave::Fsm REQUEST_PROCESSING 3"  << std::endl;
                GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
                SetFsmState(BEFORE_ANSWERING_WAITING);
            }
        }
        else
        {
            SetFsmState(MESSAGE_RECEIVE_WAITING);
        }

        break;

    case ANSWER_PROCESSING_WAITING:
//        std::cout << "CSerialMtSlave::Fsm ANSWER_PROCESSING_WAITING"  << std::endl;
    {
        m_pxDeviceControl ->
        GetTaskData(m_pxOperatingDataContainer);

        uint8_t uiFsmState = m_pxOperatingDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CSerialMtSlave::Fsm ANSWER_PROCESSING_WAITING 2"  << std::endl;
            GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
            SetFsmState(ANSWER_PROCESSING);
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CSerialMtSlave::Fsm ANSWER_PROCESSING_WAITING 3"  << std::endl;
            GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
            SetFsmState(MESSAGE_RECEIVE_WAITING);
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CSerialMtSlave::Fsm ANSWER_PROCESSING_WAITING 4"  << std::endl;
                SetFsmState(MESSAGE_RECEIVE_WAITING);
            }
        }
    }
    break;

    case ANSWER_PROCESSING:
        std::cout << "CSerialMtSlave::Fsm ANSWER_PROCESSING"  << std::endl;
        if (AnswerProcessing())
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
//        std::cout << "CSerialMtSlave::Fsm BEFORE_ANSWERING_WAITING"  << std::endl;
        // Закончилось время паузы между приёмом и передачей(5 милисекунд)?
        if (GetTimerPointer() -> IsOverflow())
        {
            GetTimerPointer() -> Set(m_uiConfirmationTimeout);
            m_pxOperatingDataContainer -> m_uiFsmCommandState =
                CSerialMtSlaveLinkLayer::COMMUNICATION_TRANSMIT_START;
            m_pxSerialMtSlaveLinkLayer ->
            SetTaskData(m_pxOperatingDataContainer);
            SetFsmState(AFTER_ANSWERING_WAITING);
        }
        break;

    case AFTER_ANSWERING_WAITING:
//        std::cout << "CSerialMtSlave::Fsm AFTER_ANSWERING_WAITING"  << std::endl;
    {
        m_pxSerialMtSlaveLinkLayer ->
        GetTaskData(m_pxOperatingDataContainer);

        uint8_t uiFsmState = m_pxOperatingDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CSerialMtSlave::Fsm AFTER_ANSWERING_WAITING 2"  << std::endl;
            SetFsmState(COMMUNICATION_RECEIVE_CONTINUE);
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CSerialMtSlave::Fsm AFTER_ANSWERING_WAITING 3"  << std::endl;
            SetFsmState(COMMUNICATION_START);
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CSerialMtSlave::Fsm AFTER_ANSWERING_WAITING 4"  << std::endl;
                SetFsmState(MESSAGE_RECEIVE_WAITING);
            }
        }
    }
    break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------

