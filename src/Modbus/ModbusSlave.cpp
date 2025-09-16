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

#include "Configuration.h"
#include "Task.h"
#include "Timer.h"
#include "Resources.h"
#include "Platform.h"
#include "DataStore.h"
#include "DeviceControl.h"
#include "Link.h"
#include "DataContainer.h"
#include "InternalModuleMuvr.h"
#include "ModbusRtuSlaveLinkLayer.h"
#include "ModbusSlaveLinkLayer.h"
#include "ModbusSlave.h"

using namespace std;

CTimeMeasure xTimeMeasure;

//-------------------------------------------------------------------------------
CModbusSlave::CModbusSlave()
{
    //std::cout << "CModbusSlave constructor"  << std::endl;
    m_pxModbusSlaveLinkLayer = 0;
    m_pxDeviceControl = 0;
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
}

//-------------------------------------------------------------------------------
uint8_t CModbusSlave::Init(void)
{
    //std::cout << "CModbusSlave Init"  << std::endl;
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));

    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));

    SetCustomerDataContainer(GetExecutorDataContainerPointer());
}

//-------------------------------------------------------------------------------
size_t CModbusSlave::GetObjectLength(void)
{
    //std::cout << "CModbusSlave GetObjectLength"  << std::endl;
    return sizeof(*this);
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
CDeviceControl* CModbusSlave::GetDeviceControl(void)
{
    return m_pxDeviceControl;
}

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
void CModbusSlave::WorkingArraysInit(void)
{
    //std::cout << "CModbusSlave WorkingArraysInit 1"  << std::endl;
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
uint16_t CModbusSlave::ByteToBitPack(uint16_t uiAddress,
                                     uint16_t uiNumberB,
                                     uint8_t* puiCoils,
                                     uint8_t *puiResponse,
                                     uint16_t uiLength)
{
    uint8_t uiData = 0;
    uint8_t uiShift = 0;
    for (uint16_t i = 0; i < uiNumberB; i++)
    {
        if (puiCoils[uiAddress++])
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
uint16_t CModbusSlave::ResponseException(uint8_t uiExceptionCode)
{
    //std::cout << "CModbusSlave ResponseException 1"  << std::endl;
    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    uiLength = m_pxModbusSlaveLinkLayer ->
               ResponseException(uiSlave,
                                 uiFunctionCode,
                                 uiExceptionCode,
                                 puiResponse);

    uiLength = m_pxModbusSlaveLinkLayer -> Tail(puiResponse, uiLength);
    m_pxModbusSlaveLinkLayer -> SetFrameLength(uiLength);

    SetFsmState(MESSAGE_TRANSMIT_START);

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadCoils(void)
{
    //std::cout << "CModbusSlave::ReadCoils 1" << std::endl;

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

    //std::cout << "CModbusSlave::ReadCoils uiSlave "  << (int)uiSlave << std::endl;
    //std::cout << "CModbusSlave::ReadCoils uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    //std::cout << "CModbusSlave::ReadCoils uiAddress "  << (int)uiAddress << std::endl;
    //std::cout << "CModbusSlave::ReadCoils uiNumberB "  << (int)uiNumberB << std::endl;

    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
    {
        //std::cout << "CModbusSlave::ReadCoils 2" << std::endl;
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_VALUE);
    }
    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
    {
        //std::cout << "CModbusSlave::ReadCoils 3" << std::endl;
        //std::cout << "CModbusSlave::ReadCoils m_uiCoilsNumber "  << (int)m_uiCoilsNumber << std::endl;
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_ADDRESS);
    }
    else
    {
        //std::cout << "CModbusSlave::ReadCoils 4" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        if (uiNumberB % 8)
        {
            //std::cout << "CModbusSlave::ReadCoils 5" << std::endl;
            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
        }
        else
        {
            //std::cout << "CModbusSlave::ReadCoils 6" << std::endl;
            puiResponse[uiLength++] = (uiNumberB / 8);
        }
        uiLength = ByteToBitPack(uiAddress,
                                 uiNumberB,
                                 m_puiCoils,
                                 puiResponse,
                                 uiLength);

        SetFsmState(MESSAGE_TRANSMIT_START);
    }
    //std::cout << "CModbusSlave::ReadCoils 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadDiscreteInputs(void)
{
    //std::cout << "CModbusSlave::ReadDiscreteInputs 1" << std::endl;

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
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_VALUE);
    }
    else if ((uiAddress + uiNumberB) > m_uiDiscreteInputsNumber)
    {
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_ADDRESS);
    }
    else
    {
        //std::cout << "CModbusSlave::ReadDiscreteInputs 4" << std::endl;
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

        SetFsmState(MESSAGE_TRANSMIT_START);
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadHoldingRegisters(void)
{
    //std::cout << "CModbusSlave::ReadHoldingRegisters 1" << std::endl;

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

    //std::cout << "CModbusSlave::ReadHoldingRegisters uiAddress "  << (int)uiAddress << std::endl;
    //std::cout << "CModbusSlave::ReadHoldingRegisters uiNumberB "  << (int)uiNumberB << std::endl;

    if (uiNumberB < 1 || MODBUS_MAX_READ_REGISTERS < uiNumberB)
    {
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_VALUE);
    }
    else if ((uiAddress + uiNumberB) > m_uiHoldingRegistersNumber)
    {
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_ADDRESS);
    }
    else
    {
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
        puiResponse[uiLength++] = (uiNumberB << 1);
        for (uint16_t i = 0; i < uiNumberB; i++)
        {
//            int8_t uiData = 0;
//            uiData = (static_cast<uint8_t>(m_puiHoldingRegisters[uiAddress] >> 8));
//            uiData = (static_cast<uint8_t>(m_puiHoldingRegisters[uiAddress] & 0x00FF));
            puiResponse[uiLength++] = (static_cast<uint8_t>(m_puiHoldingRegisters[uiAddress] >> 8));
            puiResponse[uiLength++] = (static_cast<uint8_t>(m_puiHoldingRegisters[uiAddress] & 0x00FF));
            uiAddress++;
        }

        SetFsmState(MESSAGE_TRANSMIT_START);
    }

    //std::cout << "CModbusSlave::ReadHoldingRegisters uiLength "  << (int)uiLength << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadInputRegisters(void)
{
    //std::cout << "CModbusSlave::ReadInputRegisters 1" << std::endl;

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
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_VALUE);
    }
    else if ((uiAddress + uiNumberB) > m_uiInputRegistersNumber)
    {
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_ADDRESS);
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

        SetFsmState(MESSAGE_TRANSMIT_START);
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::WriteSingleCoil(void)
{
    //std::cout << "CModbusSlave::WriteSingleCoil 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    //std::cout << "CModbusSlave::WriteSingleCoil uiSlave "  << (int)uiSlave << std::endl;
    //std::cout << "CModbusSlave::WriteSingleCoil uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    //std::cout << "CModbusSlave::WriteSingleCoil uiAddress "  << (int)uiAddress << std::endl;

    if (uiAddress >= m_uiCoilsNumber)
    {
        //std::cout << "CModbusSlave::WriteSingleCoil 2" << std::endl;
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_ADDRESS);
    }
    else
    {
        //std::cout << "CModbusSlave::WriteSingleCoil 3" << std::endl;
        uint16_t uiData = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
                           (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));

        if (uiData == 0xFF00 || uiData == 0x0)
        {
            //std::cout << "CModbusSlave::WriteSingleCoil 4" << std::endl;
            if (uiData)
            {
                //std::cout << "CModbusSlave::WriteSingleCoil 5" << std::endl;
                m_puiIntermediateBuff[BIT_STATE_OFFSET] = 1;
//                m_puiCoils[uiAddress] = 1;
            }
            else
            {
                //std::cout << "CModbusSlave::WriteSingleCoil 6" << std::endl;
                m_puiIntermediateBuff[BIT_STATE_OFFSET] = 0;
//                m_puiCoils[uiAddress] = 0;
            }
            memcpy(puiResponse, puiRequest, uiLength);



            m_uiFunctionCode = uiFunctionCode;

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
            pxDataContainer -> m_uiFsmCommandState =
                CDeviceControl::MODBUS_FUNCTION_5_HANDLER_START;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;
            pxDataContainer -> m_uiDataOffset = uiAddress;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
            SetFsmNextStateDoneWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE);
            SetFsmNextStateDoneWaitingDoneError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE);

//            SetFsmState(MESSAGE_TRANSMIT_START);
        }
        else
        {
            //std::cout << "CModbusSlave::WriteSingleCoil 7" << std::endl;
            SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_VALUE);
        }
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::WriteSingleRegister(void)
{
    //std::cout << "CModbusSlave::WriteSingleRegister 1" << std::endl;

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
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_ADDRESS);
    }
    else
    {
        m_puiHoldingRegisters[uiAddress] =
            ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
             (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));
        memcpy(puiResponse, puiRequest, uiLength);

        SetFsmState(MESSAGE_TRANSMIT_START);
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::WriteMultipleCoils(void)
{
    //std::cout << "CModbusSlave::WriteMultipleCoils 1" << std::endl;

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
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_VALUE);
    }
    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
    {
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_ADDRESS);
    }
    else
    {
        SetBytesFromBits(m_puiCoils, uiAddress, uiNumberB, &puiRequest[uiPduOffset + 6]);

        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
        /* 4 to copy the bit address (2) and the quantity of bits */
        memcpy(puiResponse + uiLength, puiRequest + uiLength, 4);
        uiLength += 4;

        SetFsmState(MESSAGE_TRANSMIT_START);
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::WriteMultipleRegisters(void)
{
    //std::cout << "CModbusSlave::WriteMultipleRegisters 1" << std::endl;

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
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_VALUE);
    }
    else if ((uiAddress + uiNumberB) > m_uiHoldingRegistersNumber)
    {
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_ADDRESS);
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

        SetFsmState(MESSAGE_TRANSMIT_START);
    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadExceptionStatus(void)
{
//    errno = ENOPROTOOPT;
    return 0;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::WriteAndReadRegisters(void)
{

    return 0;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReportSlaveID(void)
{
    //std::cout << "CModbusSlave::ReportSlaveID 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    //std::cout << "CModbusSlave::ReportSlaveID uiSlave "  << (int)uiSlave << std::endl;
    //std::cout << "CModbusSlave::ReportSlaveID uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    //std::cout << "CModbusSlave::ReportSlaveID 4" << std::endl;

    m_uiFunctionCode = uiFunctionCode;

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
    pxDataContainer -> m_uiFsmCommandState =
        CDeviceControl::CONFIGURATION_REQUEST_START;
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

    SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
    SetFsmNextStateDoneOk(EXECUTOR_ANSWER_PROCESSING);
    SetFsmNextStateReadyWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
    SetFsmNextStateDoneWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE);
    SetFsmNextStateDoneWaitingDoneError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE);

    //std::cout << "CModbusSlave::ReportSlaveID 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::PollProgramming(void)

{
    //std::cout << "CModbusSlave::PollProgramming 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    m_uiFunctionCode = uiFunctionCode;

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
    pxDataContainer -> m_uiFsmCommandState =
        CDeviceControl::DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_START;

    SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
    SetFsmNextStateDoneOk(EXECUTOR_ANSWER_PROCESSING);
    SetFsmNextStateReadyWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
    SetFsmNextStateDoneWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
    SetFsmNextStateDoneWaitingDoneError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE);

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::StateDataRead(void)

{
    //std::cout << "CModbusSlave::StateDataRead 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    m_uiFunctionCode = uiFunctionCode;

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
    pxDataContainer -> m_uiFsmCommandState =
        CDeviceControl::STATE_DATA_READ_START;
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

    SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
    SetFsmNextStateDoneOk(EXECUTOR_ANSWER_PROCESSING);
    SetFsmNextStateReadyWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
    SetFsmNextStateDoneWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
    SetFsmNextStateDoneWaitingDoneError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE);

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::AnalogueMeasureArchiveWrite(void)

{
    //std::cout << "CModbusSlave::AnalogueMeasureArchiveWrite 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    m_uiFunctionCode = uiFunctionCode;

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
    pxDataContainer -> m_uiFsmCommandState =
        CDeviceControl::ANALOGUE_MEASURE_ARCHIVE_WRITE_START;
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

    SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
    SetFsmNextStateDoneOk(EXECUTOR_ANSWER_PROCESSING);
    SetFsmNextStateReadyWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
    SetFsmNextStateDoneWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
    SetFsmNextStateDoneWaitingDoneError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE);

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::AnalogueMeasureArchiveWriteStateRequest(void)

{
    //std::cout << "CModbusSlave::AnalogueMeasureArchiveWriteStateRequest 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    m_uiFunctionCode = uiFunctionCode;

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
    pxDataContainer -> m_uiFsmCommandState =
        CDeviceControl::ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST_START;
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

    SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
    SetFsmNextStateDoneOk(EXECUTOR_ANSWER_PROCESSING);
    SetFsmNextStateReadyWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
    SetFsmNextStateDoneWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
    SetFsmNextStateDoneWaitingDoneError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE);

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::DataBaseRead(void)
{
    //std::cout << "CModbusSlave::DataBaseRead 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint8_t uiBlockIndex = puiRequest[uiPduOffset + 1];

    //std::cout << "CModbusSlave::DataBaseRead uiSlave "  << (int)uiSlave << std::endl;
    //std::cout << "CModbusSlave::DataBaseRead uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    //std::cout << "CModbusSlave::DataBaseRead uiBlockIndex "  << (int)uiBlockIndex << std::endl;

    if ((uiBlockIndex < 0) ||
            (uiBlockIndex > (CDataStore::MAX_BLOCKS_NUMBER - 1)))
    {
        //std::cout << "CModbusSlave::DataBaseRead 2" << std::endl;
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_VALUE);
    }
    else
    {
        //std::cout << "CModbusSlave::DataBaseRead 3" << std::endl;

        m_uiFunctionCode = uiFunctionCode;

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
        pxDataContainer -> m_uiFsmCommandState =
            CDeviceControl::DATA_BASE_BLOCK_READ;
        pxDataContainer -> m_uiDataIndex = uiBlockIndex;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(EXECUTOR_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
        SetFsmNextStateDoneWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE);
        SetFsmNextStateDoneWaitingDoneError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE);
    }

    //std::cout << "CModbusSlave::DataBaseRead 7" << std::endl;
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

    //std::cout << "CModbusSlave::DataBaseWrite 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint8_t uiBlockIndex = puiRequest[uiPduOffset + 1];

    //std::cout << "CModbusSlave::DataBaseWrite uiSlave "  << (int)uiSlave << std::endl;
    //std::cout << "CModbusSlave::DataBaseWrite uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    //std::cout << "CModbusSlave::DataBaseWrite uiBlockIndex "  << (int)uiBlockIndex << std::endl;


    if ((uiBlockIndex < 0) ||
            (uiBlockIndex > (CDataStore::MAX_BLOCKS_NUMBER - 1)))
    {
        //std::cout << "CModbusSlave::DataBaseWrite 2" << std::endl;
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_ADDRESS);
    }
    else
    {
        //std::cout << "CModbusSlave::DataBaseWrite 3" << std::endl;

        m_uiFunctionCode = uiFunctionCode;

        memcpy(m_puiIntermediateBuff,
               &puiRequest[uiPduOffset + 2],
               MAX_MODBUS_MESSAGE_LENGTH);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
        pxDataContainer -> m_uiFsmCommandState =
            CDeviceControl::DATA_BASE_BLOCK_WRITE_START;
        pxDataContainer -> m_uiDataIndex = uiBlockIndex;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(EXECUTOR_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
        SetFsmNextStateDoneWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
        SetFsmNextStateDoneWaitingDoneError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE);
    }

    //std::cout << "CModbusSlave::DataBaseWrite 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::TimeSet(void)
{
    //std::cout << "CModbusSlave::TimeSet 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    //std::cout << "CModbusSlave::TimeSet uiSlave "  << (int)uiSlave << std::endl;
    //std::cout << "CModbusSlave::TimeSet uiFunctionCode "  << (int)uiFunctionCode << std::endl;


    //std::cout << "CModbusSlave::TimeSet 3" << std::endl;

    m_uiFunctionCode = uiFunctionCode;

    memcpy(m_puiIntermediateBuff,
           &puiRequest[uiPduOffset + 1],
           MAX_MODBUS_MESSAGE_LENGTH);

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
    pxDataContainer -> m_uiFsmCommandState =
        CDeviceControl::TIME_SET_START;
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

    SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
    // не отвечаем. переходим к приёму следующего сообщения.
    SetFsmNextStateDoneOk(COMMUNICATION_RECEIVE_CONTINUE);
    SetFsmNextStateReadyWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
    SetFsmNextStateDoneWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
    SetFsmNextStateDoneWaitingDoneError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE);

    //std::cout << "CModbusSlave::TimeSet 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::OnlineDataRead(void)
{
    //std::cout << "CModbusSlave::OnlineDataRead 1" << std::endl;
    // puiRequest[uiPduOffset + 1] -
    // если бит7 = 0, то запрашиваются реперные точки - (бит0 - бит6) - адрес аналогового входа.
    // если бит7 = 1, то запрашивается ТХС и (бит0 - бит2) - относительный адрес модуля МВСТ3.
    // puiRequest[uiPduOffset + 2] - требуемое количество аналоговых входов.
    //        cout << "_FC_ONLINE_DATA_READ" << endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));

    //std::cout << "CModbusSlave::OnlineDataRead uiSlave "  << (int)uiSlave << std::endl;
    //std::cout << "CModbusSlave::OnlineDataRead uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    //std::cout << "CModbusSlave::OnlineDataRead uiAddress "  << (int)uiAddress << std::endl;

    if ((((puiRequest[uiPduOffset + 1]) & ANALOGUE_INPUT_ADDRESS_MASK) < MAX_HANDLED_ANALOGUE_INPUT) &&
            ((puiRequest[uiPduOffset + 2]) <= (MUVR_TXS_INPUT_QUANTITY + MUVR_ANALOG_INPUT_QUANTITY + 2)))
    {
        //std::cout << "CModbusSlave::OnlineDataRead 2" << std::endl;

        m_uiFunctionCode = uiFunctionCode;

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
        pxDataContainer -> m_uiFsmCommandState =
            CDeviceControl::ONLINE_DATA_READ_START;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;
        pxDataContainer -> m_uiDataOffset = uiAddress;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(EXECUTOR_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
        SetFsmNextStateDoneWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE);
        SetFsmNextStateDoneWaitingDoneError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE);
    }
    else
    {
        //std::cout << "CModbusSlave::OnlineDataRead 3" << std::endl;

        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_ADDRESS);
    }

    //std::cout << "CModbusSlave::OnlineDataRead 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::RequestProcessing(void)
{
    //std::cout << "CModbusSlave::RequestProcessing 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    //std::cout << "CModbusSlave::RequestProcessing uiSlave "  << (int)uiSlave << std::endl;
    //std::cout << "CModbusSlave::RequestProcessing m_uiOwnAddress "  << (int)m_uiOwnAddress << std::endl;
    //std::cout << "CModbusSlave::RequestProcessing uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    /* Filter on the Modbus unit identifier (slave) in RTU mode */
    if (uiSlave != m_uiOwnAddress && uiSlave != MODBUS_BROADCAST_ADDRESS)
    {
        //std::cout << "CModbusSlave::RequestProcessing 2" << std::endl;
        return 0;
    }

    switch (uiFunctionCode)
    {
    //std::cout << "CModbusSlave::RequestProcessing 3" << std::endl;
    case _FC_READ_COILS:
        //std::cout << "CModbusSlave::RequestProcessing _FC_READ_COILS"  << std::endl;
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

    case _FC_STATE_DATA_READ:
        uiLength = StateDataRead();
        break;

    case _FC_ANALOGUE_MEASURE_ARCHIVE_WRITE:
        uiLength = AnalogueMeasureArchiveWrite();
        break;

    case _FC_ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST:
        uiLength = AnalogueMeasureArchiveWriteStateRequest();
        break;

    case _FC_DATA_BASE_READ:
        uiLength = DataBaseRead();
        break;

    case _FC_DATA_BASE_WRITE:
        uiLength = DataBaseWrite();
        break;

//    case _FC_PROGRAMMING:
//        break;

    case _FC_TIME_SET:
        uiLength = TimeSet();
        break;

    case _FC_ONLINE_DATA_READ:
        uiLength = OnlineDataRead();
        break;

    default:
        //std::cout << "CModbusSlave::RequestProcessing 4" << std::endl;
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_FUNCTION);
        break;
    }

    //std::cout << "CModbusSlave::RequestProcessing 5" << std::endl;
    uiLength = m_pxModbusSlaveLinkLayer -> Tail(puiResponse, uiLength);
    m_pxModbusSlaveLinkLayer -> SetFrameLength(uiLength);
    return uiLength;
}









//-------------------------------------------------------------------------------
//AnswerProcessing
//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadCoilsAnswer(void)
{
    //std::cout << "CModbusSlave::ReadCoilsAnswer 1" << std::endl;

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

    //std::cout << "CModbusSlave::ReadCoilsAnswer uiSlave "  << (int)uiSlave << std::endl;
    //std::cout << "CModbusSlave::ReadCoilsAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    //std::cout << "CModbusSlave::ReadCoilsAnswer uiAddress "  << (int)uiAddress << std::endl;
    //std::cout << "CModbusSlave::ReadCoilsAnswer uiNumberB "  << (int)uiNumberB << std::endl;

    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
    {
        //std::cout << "CModbusSlave::ReadCoilsAnswer 2" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
                                     puiResponse);
    }
    else if ((uiAddress + uiNumberB) > m_uiCoilsNumber)
    {
        //std::cout << "CModbusSlave::ReadCoilsAnswer 3" << std::endl;
        //std::cout << "CModbusSlave::ReadCoilsAnswer m_uiCoilsNumber "  << (int)m_uiCoilsNumber << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
                                     puiResponse);
    }
    else
    {
        //std::cout << "CModbusSlave::ReadCoilsAnswer 4" << std::endl;
        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        if (uiNumberB % 8)
        {
            //std::cout << "CModbusSlave::ReadCoilsAnswer 5" << std::endl;
            puiResponse[uiLength++] = ((uiNumberB / 8) + 1);
        }
        else
        {
            //std::cout << "CModbusSlave::ReadCoilsAnswer 6" << std::endl;
            puiResponse[uiLength++] = (uiNumberB / 8);
        }
        uiLength = ByteToBitPack(uiAddress,
                                 uiNumberB,
                                 m_puiCoils,
                                 puiResponse,
                                 uiLength);
    }
    //std::cout << "CModbusSlave::ReadCoilsAnswer 7" << std::endl;
    return uiLength;

}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadDiscreteInputsAnswer(void)
{
    //std::cout << "CModbusSlave::ReadDiscreteInputsAnswer 1" << std::endl;

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
        //std::cout << "CModbusSlave::ReadDiscreteInputsAnswer 4" << std::endl;
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
    //std::cout << "CModbusSlave::ReadHoldingRegistersAnswer 1" << std::endl;

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

    //std::cout << "CModbusSlave::ReadHoldingRegistersAnswer uiAddress "  << (int)uiAddress << std::endl;
    //std::cout << "CModbusSlave::ReadHoldingRegistersAnswer uiNumberB "  << (int)uiNumberB << std::endl;

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
        //std::cout << "CModbusSlave::ReadHoldingRegistersAnswer 4" << std::endl;
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

    //std::cout << "CModbusSlave::ReadHoldingRegistersAnswer uiLength "  << (int)uiLength << std::endl;
    return uiLength;

}//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReadInputRegistersAnswer(void)
{
    //std::cout << "CModbusSlave::ReadInputRegistersAnswer 1" << std::endl;

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
    //std::cout << "CModbusSlave::WriteSingleCoilAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

//    int8_t uiSlave = puiRequest[uiPduOffset - 1];
//    int8_t uiFunctionCode = puiRequest[uiPduOffset];
////    uint16_t uiAddress = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 1]) << 8) |
////                          (static_cast<uint16_t>(puiRequest[uiPduOffset + 2])));
////
////    if (uiAddress >= m_uiCoilsNumber)
////    {
////        uiLength = m_pxModbusSlaveLinkLayer ->
////                   ResponseException(uiSlave,
////                                     uiFunctionCode,
////                                     MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
////                                     puiResponse);
////    }
////    else
////    {
////        uint16_t uiData = ((static_cast<uint16_t>(puiRequest[uiPduOffset + 3]) << 8) |
////                           (static_cast<uint16_t>(puiRequest[uiPduOffset + 4])));
////
////        if (uiData == 0xFF00 || uiData == 0x0)
////        {
////    if (uiData)
////    {
////        m_puiCoils[uiAddress] = 1;
////    }
////    else
////    {
////        m_puiCoils[uiAddress] = 0;
////    }
////    memcpy(puiResponse, puiRequest, uiLength);
//
//
//
//    CDataContainerDataBase* pxDataContainer =
//        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
////    uiLength = pxDataContainer -> m_uiDataLength;
////
////    //std::cout << "CModbusSlave::DataBaseReadAnswer uiLength "  << (int)uiLength << std::endl;
////
////    memcpy(&puiResponse[uiPduOffset + 3],
////           (pxDataContainer -> m_puiDataPointer),
////           uiLength);
//
//    if ((pxDataContainer -> m_puiDataPointer[0]))
//    {
//        m_puiCoils[(pxDataContainer -> m_uiDataOffset)] = 1;
//    }
//    else
//    {
//        m_puiCoils[(pxDataContainer -> m_uiDataOffset)] = 0;
//    }
////    memcpy(puiResponse, puiRequest, uiLength);

    SetFsmState(MESSAGE_TRANSMIT_START);



//        }
//        else
//        {
//            uiLength = m_pxModbusSlaveLinkLayer ->
//                       ResponseException(uiSlave,
//                                         uiFunctionCode,
//                                         MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
//                                         puiResponse);
//        }
//
//    }
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::WriteSingleRegisterAnswer(void)
{
    //std::cout << "CModbusSlave::WriteSingleRegisterAnswer 1" << std::endl;

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
    //std::cout << "CModbusSlave::WriteMultipleCoilsAnswer 1" << std::endl;

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
    //std::cout << "CModbusSlave::WriteMultipleRegistersAnswer 1" << std::endl;

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
    return 0;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::WriteAndReadRegistersAnswer(void)
{

    return 0;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::ReportSlaveIDAnswer(void)
{
    //std::cout << "CModbusSlave::ReportSlaveIDAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    //std::cout << "CModbusSlave::ReportSlaveIDAnswer uiSlave "  << (int)uiSlave << std::endl;
    //std::cout << "CModbusSlave::ReportSlaveIDAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    //std::cout << "CModbusSlave::ReportSlaveIDAnswer 4" << std::endl;

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    uiLength = pxDataContainer -> m_uiDataLength;

    //std::cout << "CModbusSlave::ReportSlaveIDAnswer uiLength "  << (int)uiLength << std::endl;

    memcpy(&puiResponse[uiPduOffset + 2],
           (pxDataContainer -> m_puiDataPointer),
           uiLength);

    // количество байт в прикладном сообщении массиве конфигурации, не включая остальные.
    puiResponse[uiPduOffset + 1] = uiLength;
    uiLength++;
    uiLength += m_pxModbusSlaveLinkLayer ->
                ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

    SetFsmState(MESSAGE_TRANSMIT_START);

    //std::cout << "CModbusSlave::ReportSlaveIDAnswer 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::PollProgrammingAnswer(void)
{
    //std::cout << "CModbusSlave::PollProgrammingAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    //std::cout << "CModbusSlave::PollProgrammingAnswer 2" << std::endl;

    uiLength = m_pxModbusSlaveLinkLayer ->
               ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

    SetFsmState(MESSAGE_TRANSMIT_START);

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::StateDataReadAnswer(void)
{
    //std::cout << "CModbusSlave::StateDataReadAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    //std::cout << "CModbusSlave::StateDataReadAnswer 2" << std::endl;

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    uiLength = pxDataContainer -> m_uiDataLength;

    //std::cout << "CModbusSlave::StateDataReadAnswer uiLength "  << (int)uiLength << std::endl;

    memcpy(&puiResponse[uiPduOffset + 2],
           (pxDataContainer -> m_puiDataPointer),
           uiLength);

    // количество байт в прикладном сообщении массиве конфигурации, не включая остальные.
    puiResponse[uiPduOffset + 1] = uiLength;
    uiLength++;
    uiLength += m_pxModbusSlaveLinkLayer ->
                ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

    SetFsmState(MESSAGE_TRANSMIT_START);

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::AnalogueMeasureArchiveWriteAnswer(void)
{
    //std::cout << "CModbusSlave::AnalogueMeasureArchiveWriteAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

//    //std::cout << "CModbusSlave::AnalogueMeasureArchiveWriteAnswer 2" << std::endl;
//
//    //std::cout << "CModbusSlave::AnalogueMeasureArchiveWriteAnswer uiSlave "  << (int)uiSlave << std::endl;
//    //std::cout << "CModbusSlave::AnalogueMeasureArchiveWriteAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
//
//
//    //std::cout << "CModbusSlave::AnalogueMeasureArchiveWriteAnswer 3" << std::endl;

//    uiLength = m_pxModbusSlaveLinkLayer ->
//               ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
    memcpy(puiResponse, puiRequest, uiLength);

    SetFsmState(MESSAGE_TRANSMIT_START);


//    CDataContainerDataBase* pxDataContainer =
//        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//    uiLength = pxDataContainer -> m_uiDataLength;
//
//    //std::cout << "CModbusSlave::AnalogueMeasureArchiveWriteAnswer uiLength "  << (int)uiLength << std::endl;
//
//    memcpy(&puiResponse[uiPduOffset + 2],
//           (pxDataContainer -> m_puiDataPointer),
//           uiLength);
//
//    // количество байт в прикладном сообщении массиве конфигурации, не включая остальные.
//    puiResponse[uiPduOffset + 1] = uiLength;
//    uiLength++;
//    uiLength += m_pxModbusSlaveLinkLayer ->
//                ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
//
//    SetFsmState(MESSAGE_TRANSMIT_START);

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::AnalogueMeasureArchiveWriteStateRequestAnswer(void)
{
    //std::cout << "CModbusSlave::AnalogueMeasureArchiveWriteStateRequestAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    //std::cout << "CModbusSlave::AnalogueMeasureArchiveWriteStateRequestAnswer 2" << std::endl;

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    uiLength = pxDataContainer -> m_uiDataLength;

    //std::cout << "CModbusSlave::AnalogueMeasureArchiveWriteStateRequestAnswer uiLength "  << (int)uiLength << std::endl;

    memcpy(&puiResponse[uiPduOffset + 2],
           (pxDataContainer -> m_puiDataPointer),
           uiLength);

    // количество байт в прикладном сообщении массиве конфигурации, не включая остальные.
    puiResponse[uiPduOffset + 1] = uiLength;
    uiLength++;
    uiLength += m_pxModbusSlaveLinkLayer ->
                ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

    SetFsmState(MESSAGE_TRANSMIT_START);

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::DataBaseReadAnswer(void)
{
    //std::cout << "CModbusSlave::DataBaseReadAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint8_t uiBlockIndex = puiRequest[uiPduOffset + 1];

    //std::cout << "CModbusSlave::DataBaseReadAnswer uiSlave "  << (int)uiSlave << std::endl;
    //std::cout << "CModbusSlave::DataBaseReadAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    //std::cout << "CModbusSlave::DataBaseReadAnswer uiBlockIndex "  << (int)uiBlockIndex << std::endl;

    if ((uiBlockIndex < 0) ||
            (uiBlockIndex > (CDataStore::MAX_BLOCKS_NUMBER - 1)))
    {
        //std::cout << "CModbusSlave::DataBaseReadAnswer 2" << std::endl;
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_VALUE);
    }
    else
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        uiLength = pxDataContainer -> m_uiDataLength;

        //std::cout << "CModbusSlave::DataBaseReadAnswer uiLength "  << (int)uiLength << std::endl;

        memcpy(&puiResponse[uiPduOffset + 3],
               (pxDataContainer -> m_puiDataPointer),
               uiLength);

        puiResponse[uiPduOffset + 1] = uiLength + 1;
        uiLength++;

        uiLength += m_pxModbusSlaveLinkLayer ->
                    ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        // номер блока базы данных
        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
        uiLength++;

        SetFsmState(MESSAGE_TRANSMIT_START);
    }

    //std::cout << "CModbusSlave::DataBaseReadAnswer 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::DataBaseWriteAnswer(void)
{
    //std::cout << "CModbusSlave::DataBaseWriteAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];
    uint8_t uiBlockIndex = puiRequest[uiPduOffset + 1];

    //std::cout << "CModbusSlave::DataBaseWriteAnswer uiSlave "  << (int)uiSlave << std::endl;
    //std::cout << "CModbusSlave::DataBaseWriteAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
    //std::cout << "CModbusSlave::DataBaseWriteAnswer uiBlockIndex "  << (int)uiBlockIndex << std::endl;


    if ((uiBlockIndex < 0) ||
            (uiBlockIndex > (CDataStore::MAX_BLOCKS_NUMBER - 1)))
    {
        //std::cout << "CModbusSlave::DataBaseWriteAnswer 2" << std::endl;
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_DATA_ADDRESS);
    }
    else
    {
        //std::cout << "CModbusSlave::DataBaseWriteAnswer 3" << std::endl;

        uiLength = m_pxModbusSlaveLinkLayer ->
                   ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

        // номер блока базы данных
        puiResponse[uiPduOffset + 1] = 1;
        uiLength++;
        puiResponse[uiPduOffset + 2] = puiRequest[uiPduOffset + 1];
        uiLength++;

        SetFsmState(MESSAGE_TRANSMIT_START);
    }

    //std::cout << "CModbusSlave::DataBaseWriteAnswer 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::TimeSetAnswer(void)
{
    //std::cout << "CModbusSlave::TimeSetAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    //std::cout << "CModbusSlave::TimeSetAnswer uiSlave "  << (int)uiSlave << std::endl;
    //std::cout << "CModbusSlave::TimeSetAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    uiLength = pxDataContainer -> m_uiDataLength;
    puiResponse[uiPduOffset + 1] = uiLength;
    uiLength++;
    uiLength += m_pxModbusSlaveLinkLayer ->
                ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

    SetFsmState(MESSAGE_TRANSMIT_START);

    //std::cout << "CModbusSlave::TimeSetAnswer 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::OnlineDataReadAnswer(void)
{
    //std::cout << "CModbusSlave::OnlineDataReadAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    //std::cout << "CModbusSlave::OnlineDataReadAnswer uiSlave "  << (int)uiSlave << std::endl;
    //std::cout << "CModbusSlave::OnlineDataReadAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    // (rsp_length) = 2.
    // (rsp_length - 2) - адрес slave.
    // (rsp_length - 1) - функция.
    // (rsp_length) - количество байт в ответе.
    // (rsp_length + 1) - начало данных в ответе.

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    uiLength = pxDataContainer -> m_uiDataLength;

    //std::cout << "CModbusSlave::OnlineDataReadAnswer uiLength "  << (int)uiLength << std::endl;

    memcpy(&puiResponse[uiPduOffset + 2],
           (pxDataContainer -> m_puiDataPointer),
           uiLength);

    puiResponse[uiPduOffset + 1] = uiLength;
    uiLength++;

    uiLength += m_pxModbusSlaveLinkLayer ->
                ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

    SetFsmState(MESSAGE_TRANSMIT_START);

    //std::cout << "CModbusSlave::OnlineDataReadAnswer 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusSlave::AnswerProcessing(void)
{
    //std::cout << "CModbusSlave::AnswerProcessing 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxModbusSlaveLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    //std::cout << "CModbusSlave::AnswerProcessing uiSlave "  << (int)uiSlave << std::endl;
    //std::cout << "CModbusSlave::AnswerProcessing uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    /* Filter on the Modbus unit identifier (slave) in RTU mode */
    if (uiSlave != m_uiOwnAddress && uiSlave != MODBUS_BROADCAST_ADDRESS)
    {
        //std::cout << "CModbusSlave::AnswerProcessing 2" << std::endl;
        return 0;
    }

    // проверяем сохранённый локально текущий код функции.
    switch (m_uiFunctionCode)
    {
    //std::cout << "CModbusSlave::AnswerProcessing 3" << std::endl;
    case _FC_READ_COILS:
        //std::cout << "CModbusSlave::Answer _FC_READ_COILS"  << std::endl;
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

    case _FC_STATE_DATA_READ:
        uiLength = StateDataReadAnswer();
        break;

    case _FC_ANALOGUE_MEASURE_ARCHIVE_WRITE:
        uiLength = AnalogueMeasureArchiveWriteAnswer();
        break;

    case _FC_ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST:
        uiLength = AnalogueMeasureArchiveWriteStateRequestAnswer();
        break;

    case _FC_DATA_BASE_READ:
        uiLength = DataBaseReadAnswer();
        break;

    case _FC_DATA_BASE_WRITE:
        uiLength = DataBaseWriteAnswer();
        break;

    case _FC_TIME_SET:
        uiLength = TimeSetAnswer();
        break;

    case _FC_ONLINE_DATA_READ:
        uiLength = OnlineDataReadAnswer();
        break;

    default:
        //std::cout << "CModbusSlave::AnswerProcessing 4" << std::endl;
        SetFsmState(RESPONSE_EXCEPTION_ILLEGAL_FUNCTION);
        break;
    }

    //std::cout << "CModbusSlave::AnswerProcessing 5" << std::endl;
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
//    //std::cout << "CModbusSlave::Fsm 1" << endl;
    uint8_t uiReadyTaskCounter = 0;
    switch (GetFsmState())
    {
    case IDDLE:
//        //std::cout << "CModbusSlave::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        ////std::cout << "CModbusSlave::Fsm STOP"  << std::endl;
        break;

    case START:
        //std::cout << "CModbusSlave::Fsm START"  << std::endl;
        //std::cout << "CModbusSlave::Fsm m_sModbusSlaveLinkLayerName" << " " << (m_sModbusSlaveLinkLayerName) << std::endl;
        Init();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
        //std::cout << "CModbusSlave::Fsm INIT 1"  << std::endl;
    {
        m_uiModbusSlaveLinkLayerId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sModbusSlaveLinkLayerName);
        CModbusSlaveLinkLayer* m_pxModbusSlaveLinkLayer =
            (CModbusSlaveLinkLayer*)GetResources() ->
            GetTaskPointerById(m_uiModbusSlaveLinkLayerId);
        SetModbusSlaveLinkLayer(((CModbusSlaveLinkLayer*)(GetResources() ->
                                 GetTaskPointerById(m_uiModbusSlaveLinkLayerId))));

        m_uiDeviceControlId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDeviceControlName);

        SetFsmState(READY);
    }
    break;

    case READY:
        //std::cout << "CModbusSlave::Fsm READY"  << std::endl;
        SetFsmState(COMMUNICATION_START);
        break;

    case DONE_OK:
        //std::cout << "CModbusSlave::Fsm DONE_OK"  << std::endl;
        SetFsmOperationStatus(DONE_OK);
        break;

    case DONE_ERROR:
        //std::cout << "CModbusSlave::Fsm DONE_ERROR"  << std::endl;
        SetFsmOperationStatus(DONE_ERROR);
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START:
        //std::cout << "CModbusSlave::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START"  << std::endl;
    {
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING);
    }
    break;

    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING:
//        //std::cout << "CModbusSlave::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            //std::cout << "CModbusSlave::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else
        {
            //std::cout << "CModbusSlave::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                //std::cout << "CModbusSlave::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
        //std::cout << "CModbusSlave::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
    {
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
    }
    break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        //std::cout << "CModbusSlave::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            //std::cout << "CModbusSlave::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
//            //std::cout << "CModbusSlave::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                //std::cout << "CModbusSlave::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
        //std::cout << "CModbusSlave::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
    {
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
    }

    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        //std::cout << "CModbusSlave::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            //std::cout << "CModbusSlave::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else if (uiFsmState == DONE_ERROR)
        {
            //std::cout << "CModbusSlave::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(GetFsmNextStateDoneWaitingDoneError());
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                //std::cout << "CModbusSlave::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateDoneWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case COMMUNICATION_START:
        //std::cout << "CModbusSlave::Fsm COMMUNICATION_START"  << std::endl;
//        CDataContainerDataBase* pxDataContainer =
//            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//        pxDataContainer -> m_uiTaskId = m_uiModbusSlaveLinkLayerId;
//        pxDataContainer -> m_uiFsmCommandState =
//            CModbusRtuSlaveLinkLayer::COMMUNICATION_START;
//        SetFsmState(MESSAGE_RECEIVE_WAITING);

        //        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
//        SetFsmNextStateDoneOk(EXECUTOR_ANSWER_PROCESSING);
////        SetFsmNextStateDoneWaitingDoneOk(EXECUTOR_ANSWER_PROCESSING);
////        SetFsmNextStateDoneError(DONE_ERROR);
//        SetFsmNextStateReadyWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY);
//        SetFsmNextStateDoneWaitingError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE);
//        SetFsmNextStateDoneWaitingDoneError(RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE);
//
        m_pxOperatingDataContainer -> m_uiFsmCommandState =
            CModbusRtuSlaveLinkLayer::COMMUNICATION_START;
        m_pxModbusSlaveLinkLayer ->
        SetTaskData(m_pxOperatingDataContainer);
        SetFsmState(MESSAGE_RECEIVE_WAITING);
        break;

    case COMMUNICATION_RECEIVE_START:
        //std::cout << "CModbusSlave::Fsm COMMUNICATION_RECEIVE_START"  << std::endl;
//        CDataContainerDataBase* pxDataContainer =
//            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//        pxDataContainer -> m_uiTaskId = m_uiModbusSlaveLinkLayerId;
//        pxDataContainer -> m_uiFsmCommandState =
//            CModbusRtuSlaveLinkLayer::COMMUNICATION_RECEIVE_START;
//        SetFsmState(MESSAGE_RECEIVE_WAITING);

        m_pxOperatingDataContainer -> m_uiFsmCommandState =
            CModbusRtuSlaveLinkLayer::COMMUNICATION_RECEIVE_START;
        m_pxModbusSlaveLinkLayer ->
        SetTaskData(m_pxOperatingDataContainer);
        SetFsmState(MESSAGE_RECEIVE_WAITING);
        break;

    case COMMUNICATION_RECEIVE_CONTINUE:
        //std::cout << "CModbusSlave::Fsm COMMUNICATION_RECEIVE_CONTINUE"  << std::endl;
//        CDataContainerDataBase* pxDataContainer =
//            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//        pxDataContainer -> m_uiTaskId = m_uiModbusSlaveLinkLayerId;
//        pxDataContainer -> m_uiFsmCommandState =
//            CModbusRtuSlaveLinkLayer::COMMUNICATION_RECEIVE_CONTINUE;
//        SetFsmState(MESSAGE_RECEIVE_WAITING);

        m_pxOperatingDataContainer -> m_uiFsmCommandState =
            CModbusRtuSlaveLinkLayer::COMMUNICATION_RECEIVE_CONTINUE;
        m_pxModbusSlaveLinkLayer ->
        SetTaskData(m_pxOperatingDataContainer);
        SetFsmState(MESSAGE_RECEIVE_WAITING);
        break;

    case MESSAGE_RECEIVE_WAITING:
//        //std::cout << "CModbusSlave::Fsm MESSAGE_RECEIVE_WAITING"  << std::endl;
    {
//        CDataContainerDataBase* pxDataContainer =
//            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//
//        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        m_pxModbusSlaveLinkLayer ->
        GetTaskData(m_pxOperatingDataContainer);

        uint8_t uiFsmState = m_pxOperatingDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            xTimeMeasure.Begin();
//            //std::cout << "CModbusSlave::Fsm MESSAGE_RECEIVE_WAITING 2"  << std::endl;
//            SetFsmState(REQUEST_PROCESSING);
            if (RequestProcessing())
            {
                // состояние автомата изменяют вызываемые методы обработчики функций модбас.
            }
            else
            {
                SetFsmState(MESSAGE_RECEIVE_WAITING);
            }
//        xTimeMeasure.End();
        }
        else if (uiFsmState == DONE_ERROR)
        {
            //std::cout << "CModbusSlave::Fsm MESSAGE_RECEIVE_WAITING 3"  << std::endl;
            SetFsmState(COMMUNICATION_START);
        }
    }
    break;

    case REQUEST_PROCESSING:
//        //std::cout << "CModbusSlave::Fsm REQUEST_PROCESSING"  << std::endl;
//        xTimeMeasure.End();
        if (RequestProcessing())
        {
            // состояние автомата изменяют вызываемые методы обработчики функций модбас.
        }
        else
        {
            SetFsmState(MESSAGE_RECEIVE_WAITING);
        }

        break;

    case EXECUTOR_ANSWER_PROCESSING:
        //std::cout << "CModbusSlave::Fsm EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        if (AnswerProcessing())
        {
            // состояние автомата изменяют вызываемые методы обработчики функций модбас.
        }
        else
        {
            SetFsmState(MESSAGE_RECEIVE_WAITING);
        }
        break;

    case MESSAGE_TRANSMIT_START:
        //std::cout << "CModbusSlave::Fsm MESSAGE_TRANSMIT_START"  << std::endl;
//    xTimeMeasure.Begin();
//    xTimeMeasure.End();
        GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
//        SetFsmState(MESSAGE_TRANSMIT_BEFORE_WAITING);
            GetTimerPointer() -> Set(m_uiConfirmationTimeout);
            m_pxOperatingDataContainer -> m_uiFsmCommandState =
                CModbusRtuSlaveLinkLayer::COMMUNICATION_TRANSMIT_START;
            m_pxModbusSlaveLinkLayer ->
            SetTaskData(m_pxOperatingDataContainer);
            SetFsmState(MESSAGE_TRANSMIT_AFTER_WAITING);
        break;

    case MESSAGE_TRANSMIT_BEFORE_WAITING:
//        //std::cout << "CModbusSlave::Fsm MESSAGE_TRANSMIT_BEFORE_WAITING"  << std::endl;
        // Закончилось время паузы между приёмом и передачей(5 милисекунд)?
        if (GetTimerPointer() -> IsOverflow())
        {
            GetTimerPointer() -> Set(m_uiConfirmationTimeout);
            m_pxOperatingDataContainer -> m_uiFsmCommandState =
                CModbusRtuSlaveLinkLayer::COMMUNICATION_TRANSMIT_START;
            m_pxModbusSlaveLinkLayer ->
            SetTaskData(m_pxOperatingDataContainer);
            SetFsmState(MESSAGE_TRANSMIT_AFTER_WAITING);
        }
        break;

    case MESSAGE_TRANSMIT_AFTER_WAITING:
//        //std::cout << "CModbusSlave::Fsm MESSAGE_TRANSMIT_AFTER_WAITING"  << std::endl;
    {
        m_pxModbusSlaveLinkLayer ->
        GetTaskData(m_pxOperatingDataContainer);

        uint8_t uiFsmState = m_pxOperatingDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            xTimeMeasure.End();
            //std::cout << "CModbusSlave::Fsm MESSAGE_TRANSMIT_AFTER_WAITING 2"  << std::endl;
            SetFsmState(COMMUNICATION_RECEIVE_CONTINUE);
        }
        else if (uiFsmState == DONE_ERROR)
        {
            //std::cout << "CModbusSlave::Fsm MESSAGE_TRANSMIT_AFTER_WAITING 3"  << std::endl;
            SetFsmState(COMMUNICATION_START);
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                //std::cout << "CModbusSlave::Fsm MESSAGE_TRANSMIT_AFTER_WAITING 4"  << std::endl;
                SetFsmState(MESSAGE_RECEIVE_WAITING);
            }
        }
    }
    break;

    case RESPONSE_EXCEPTION_ILLEGAL_FUNCTION:
        //std::cout << "CModbusSlave::Fsm RESPONSE_EXCEPTION_ILLEGAL_FUNCTION"  << std::endl;
        ResponseException(MODBUS_EXCEPTION_ILLEGAL_FUNCTION);
        break;

    case RESPONSE_EXCEPTION_ILLEGAL_DATA_ADDRESS:
        //std::cout << "CModbusSlave::Fsm RESPONSE_EXCEPTION_ILLEGAL_DATA_ADDRESS"  << std::endl;
        ResponseException(MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        break;

    case RESPONSE_EXCEPTION_ILLEGAL_DATA_VALUE:
        //std::cout << "CModbusSlave::Fsm RESPONSE_EXCEPTION_ILLEGAL_DATA_VALUE"  << std::endl;
        ResponseException(MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE);
        break;

    case RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE:
        //std::cout << "CModbusSlave::Fsm RESPONSE_EXCEPTION_SLAVE_OR_SERVER_FAILURE"  << std::endl;
        ResponseException(MODBUS_EXCEPTION_SLAVE_OR_SERVER_FAILURE);
        break;

    case RESPONSE_EXCEPTION_ACKNOWLEDGE:
        //std::cout << "CModbusSlave::Fsm RESPONSE_EXCEPTION_ACKNOWLEDGE"  << std::endl;
        ResponseException(MODBUS_EXCEPTION_ACKNOWLEDGE);
        break;

    case RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY:
        //std::cout << "CModbusSlave::Fsm RESPONSE_EXCEPTION_SLAVE_OR_SERVER_BUSY"  << std::endl;
        ResponseException(MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY);
        break;

    case RESPONSE_EXCEPTION_NEGATIVE_ACKNOWLEDGE:
        //std::cout << "CModbusSlave::Fsm RESPONSE_EXCEPTION_NEGATIVE_ACKNOWLEDGE"  << std::endl;
        ResponseException(MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE);
        break;

    case RESPONSE_EXCEPTION_MEMORY_PARITY:
        //std::cout << "CModbusSlave::Fsm RESPONSE_EXCEPTION_MEMORY_PARITY"  << std::endl;
        ResponseException(MODBUS_EXCEPTION_MEMORY_PARITY);
        break;

    case RESPONSE_EXCEPTION_NOT_DEFINED:
        //std::cout << "CModbusSlave::Fsm RESPONSE_EXCEPTION_NOT_DEFINED"  << std::endl;
        ResponseException(MODBUS_EXCEPTION_NOT_DEFINED);
        break;

    case RESPONSE_EXCEPTION_GATEWAY_PATH:
        //std::cout << "CModbusSlave::Fsm RESPONSE_EXCEPTION_GATEWAY_PATH"  << std::endl;
        ResponseException(MODBUS_EXCEPTION_GATEWAY_PATH);
        break;

    case RESPONSE_EXCEPTION_GATEWAY_TARGET:
        //std::cout << "CModbusSlave::Fsm RESPONSE_EXCEPTION_GATEWAY_TARGET"  << std::endl;
        ResponseException(MODBUS_EXCEPTION_GATEWAY_TARGET);
        break;

    case RESPONSE_EXCEPTION_MAX:
        //std::cout << "CModbusSlave::Fsm RESPONSE_EXCEPTION_MAX"  << std::endl;
        ResponseException(MODBUS_EXCEPTION_MAX);
        break;

    case COMMUNICATION_STOP:
        ////std::cout << "CModbusSlave::Fsm COMMUNICATION_STOP"  << std::endl;
        m_pxOperatingDataContainer -> m_uiFsmCommandState =
            CModbusRtuSlaveLinkLayer::COMMUNICATION_STOP;
        m_pxModbusSlaveLinkLayer ->
        SetTaskData(m_pxOperatingDataContainer);
        SetFsmState(READY);
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------

