
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
#include "SerialMtMaster.h"
#include "SerialMtMasterLinkLayer.h"

using namespace std;

//-------------------------------------------------------------------------------
CSerialMtMaster::CSerialMtMaster()
{
    std::cout << "CSerialMtMaster constructor"  << std::endl;
    m_pxSerialMtMasterLinkLayer = 0;
    m_pxDeviceControl = 0;
    // получим имя класса.
    sprintf(GetTaskNamePointer(),
            "%s",
            typeid(*this).name());
    m_puiIntermediateBuff = new uint8_t[MAX_SERIAL_MT_MESSAGE_LENGTH];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CSerialMtMaster::~CSerialMtMaster()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
uint8_t CSerialMtMaster::Init(void)
{
    std::cout << "CSerialMtMaster Init"  << std::endl;
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));
}

//-------------------------------------------------------------------------------
void CSerialMtMaster::SetSerialMtMasterLinkLayerName(std::string sName)
{
    m_sSerialMtMasterLinkLayerName = sName;
}

//-------------------------------------------------------------------------------
void CSerialMtMaster::SetSerialMtMasterLinkLayer(CSerialMtMasterLinkLayer* pxSerialMtMasterLinkLayer)
{
    m_pxSerialMtMasterLinkLayer = pxSerialMtMasterLinkLayer;
}

//-------------------------------------------------------------------------------
void CSerialMtMaster::SetDeviceControlName(std::string sName)
{
    m_sDeviceControlName = sName;
}

//-------------------------------------------------------------------------------
void CSerialMtMaster::SetDeviceControl(CDeviceControl* pxDeviceControl)
{
    m_pxDeviceControl = pxDeviceControl;
}

//-------------------------------------------------------------------------------
CDeviceControl* CSerialMtMaster::GetDeviceContro(void)
{
    return m_pxDeviceControl;
}

////-------------------------------------------------------------------------------
//const char *CSerialMtMaster::ModbusStringError(int errnum)
//{
//    switch (errnum)
//    {
//    case EMBXILFUN:
//        return "Illegal function";
//    case EMBXILADD:
//        return "Illegal data address";
//    case EMBXILVAL:
//        return "Illegal data value";
//    case EMBXSFAIL:
//        return "Slave device or server failure";
//    case EMBXACK:
//        return "Acknowledge";
//    case EMBXSBUSY:
//        return "Slave device or server is busy";
//    case EMBXNACK:
//        return "Negative acknowledge";
//    case EMBXMEMPAR:
//        return "Memory parity error";
//    case EMBXGPATH:
//        return "Gateway path unavailable";
//    case EMBXGTAR:
//        return "Target device failed to respond";
//    case EMBBADCRC:
//        return "Invalid CRC";
//    case EMBBADDATA:
//        return "Invalid data";
//    case EMBBADEXC:
//        return "Invalid exception code";
//    case EMBMDATA:
//        return "Too many data";
//    default:
//        return strerror(errnum);
//    }
//}

//-------------------------------------------------------------------------------
void CSerialMtMaster::WorkingArraysInit(void)
{
    std::cout << "CSerialMtMaster WorkingArraysInit 1"  << std::endl;
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
uint16_t CSerialMtMaster::ByteToBitPack(uint16_t uiAddress,
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

//-----------------------------------------------------------------------------------------------------
int8_t CSerialMtMaster::SendMessage(uint8_t uiSlaveAddress,
                                    uint8_t *puiDataBuffer,
                                    uint16_t uiLength)
{
    if (uiLength > MAX_SERIAL_MT_MESSAGE_LENGTH)
    {
        return 0;
    }
    else
    {
        if (IsTaskReady())
        {
            uint8_t * puiResponse = m_pxSerialMtMasterLinkLayer -> GetTxBuffer();

            m_uiSlaveAddress = uiSlaveAddress;
            m_uiFunctionCode = _FC_SEND_MESSAGE;

            memcpy(puiResponse,
                   puiDataBuffer,
                   uiLength);
            m_pxSerialMtMasterLinkLayer ->
            SetFrameLength(uiLength);

            SetFsmState(REQUEST_START);
            return 1;
        }
        else
        {
            return 0;
        }
    }
}








//-------------------------------------------------------------------------------
////AnswerProcessing



//-------------------------------------------------------------------------------
uint16_t CSerialMtMaster::SendMessageAnswer(void)
{
    std::cout << "CSerialMtMaster::SendMessageAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtMasterLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtMasterLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtMasterLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtMasterLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    std::cout << "CSerialMtMaster::SendMessageAnswer uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CSerialMtMaster::SendMessageAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    std::cout << "CSerialMtMaster::SendMessageAnswer 4" << std::endl;

    std::cout << "CSerialMtMaster::SendMessageAnswer 7" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CSerialMtMaster::AnswerProcessing(void)
{
    std::cout << "CSerialMtMaster::AnswerProcessing 1" << std::endl;

    uint16_t uiPduOffset = m_pxSerialMtMasterLinkLayer -> GetPduOffset();
    uint8_t * puiRequest = m_pxSerialMtMasterLinkLayer -> GetRxBuffer();
    uint8_t * puiResponse = m_pxSerialMtMasterLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxSerialMtMasterLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    std::cout << "CSerialMtMaster::AnswerProcessing uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CSerialMtMaster::AnswerProcessing uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    /* Filter on the Modbus unit identifier (slave) in RTU mode */
    if (uiSlave != m_uiOwnAddress && uiSlave != MODBUS_BROADCAST_ADDRESS)
    {
        std::cout << "CSerialMtMaster::AnswerProcessing 2" << std::endl;
        return 0;
    }

    // проверяем сохранённый локально текущий код функции.
    switch (m_uiFunctionCode)
    {
        std::cout << "CSerialMtMaster::AnswerProcessing 3" << std::endl;

    case _FC_SEND_MESSAGE:
        uiLength = SendMessageAnswer();
        break;

    default:
        std::cout << "CSerialMtMaster::AnswerProcessing 4" << std::endl;
        uiLength = m_pxSerialMtMasterLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_FUNCTION,
                                     puiResponse);
        break;
    }

    std::cout << "CSerialMtMaster::AnswerProcessing 5" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
void CSerialMtMaster::SetByteFromBits(uint8_t *dest, uint16_t index, const uint8_t value)
{
    uint16_t i;

    for (i=0; i<8; i++)
    {
        dest[index+i] = (value & (1 << i)) ? 1 : 0;
    }
}

//-------------------------------------------------------------------------------
void CSerialMtMaster::SetBytesFromBits(uint8_t *dest, uint16_t index, uint16_t nb_bits,
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
uint8_t CSerialMtMaster::GetByteFromBits(const uint8_t *src, uint16_t index,
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
float CSerialMtMaster::GetFloat(const uint16_t *src)
{
    float f = 0.0f;
    uint32_t i;

    i = (((uint32_t)src[1]) << 16) + src[0];
    memcpy(&f, &i, sizeof(float));

    return f;
}

//-------------------------------------------------------------------------------
void CSerialMtMaster::SetFloat(float f, uint16_t *dest)
{
    uint32_t i = 0;

    memcpy(&i, &f, sizeof(uint32_t));
    dest[0] = (uint16_t)i;
    dest[1] = (uint16_t)(i >> 16);
}

//-------------------------------------------------------------------------------
uint8_t CSerialMtMaster::Fsm(void)
{
//    std::cout << "CSerialMtMaster::Fsm 1" << endl;
    uint8_t uiReadyTaskCounter = 0;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CSerialMtMaster::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CSerialMtMaster::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CSerialMtMaster::Fsm START"  << std::endl;
        std::cout << "CSerialMtMaster::Fsm m_sSerialMtMasterLinkLayerName" << " " << (m_sSerialMtMasterLinkLayerName) << std::endl;
        Init();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
//        std::cout << "CSerialMtMaster::Fsm INIT 1"  << std::endl;
    {
        CTaskInterface* pxTask =
            GetResources() ->
            GetTaskPointerByNameFromMap(m_sSerialMtMasterLinkLayerName);

        if (pxTask != 0)
        {
//                std::cout << "CSerialMtMaster::Fsm INIT 2"  << std::endl;
            if (pxTask -> GetFsmState() >= READY)
            {
//                    std::cout << "CSerialMtMaster::Fsm INIT 3"  << std::endl;
                SetSerialMtMasterLinkLayer((CSerialMtMasterLinkLayer*)pxTask);
                uiReadyTaskCounter += 1;
            }
        }
        else
        {
//                std::cout << "CSerialMtMaster::Fsm INIT 4"  << std::endl;
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CSerialMtMaster::Fsm INIT 5"  << std::endl;
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
//                std::cout << "CSerialMtMaster::Fsm INIT 2"  << std::endl;
            if (pxTask -> GetFsmState() >= READY)
            {
//                    std::cout << "CSerialMtMaster::Fsm INIT 3"  << std::endl;
                SetDeviceControl((CDeviceControl*)pxTask);
                uiReadyTaskCounter += 1;
            }
        }
        else
        {
            //                std::cout << "CSerialMtMaster::Fsm INIT 4"  << std::endl;
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CSerialMtMaster::Fsm INIT 5"  << std::endl;
                SetFsmState(STOP);
            }
        }
    }

    if (uiReadyTaskCounter > 1)
    {
        std::cout << "CSerialMtMaster::Fsm INIT 9"  << std::endl;
        SetFsmState(COMMUNICATION_START);
    }
    break;

    case READY:
//        std::cout << "CSerialMtMaster::Fsm READY"  << std::endl;
        break;

    case DONE_OK:
//        std::cout << "CSerialMtMaster::Fsm DONE_OK"  << std::endl;
        SetFsmOperationStatus(DONE_OK);
        break;

    case DONE_ERROR:
//        std::cout << "CSerialMtMaster::Fsm DONE_ERROR"  << std::endl;
        SetFsmOperationStatus(DONE_ERROR);
        break;

    case COMMUNICATION_START:
        std::cout << "CSerialMtMaster::Fsm COMMUNICATION_START"  << std::endl;
        m_pxOperatingDataContainer -> m_uiFsmCommandState =
            CSerialMtMasterLinkLayer::COMMUNICATION_START;
        m_pxSerialMtMasterLinkLayer ->
        SetTaskData(m_pxOperatingDataContainer);
        SetFsmState(READY);
        break;

    case COMMUNICATION_RECEIVE_START:
        std::cout << "CSerialMtMaster::Fsm COMMUNICATION_RECEIVE_START"  << std::endl;
        m_pxOperatingDataContainer -> m_uiFsmCommandState =
            CSerialMtMasterLinkLayer::COMMUNICATION_RECEIVE_START;
        m_pxSerialMtMasterLinkLayer ->
        SetTaskData(m_pxOperatingDataContainer);
        SetFsmState(MESSAGE_RECEIVE_WAITING);
        break;

    case MESSAGE_RECEIVE_WAITING:
//        std::cout << "CSerialMtMaster::Fsm MESSAGE_RECEIVE_WAITING"  << std::endl;
    {
        m_pxSerialMtMasterLinkLayer ->
        GetTaskData(m_pxOperatingDataContainer);

        uint8_t uiFsmState = m_pxOperatingDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CSerialMtMaster::Fsm MESSAGE_RECEIVE_WAITING 2"  << std::endl;
            SetFsmState(ANSWER_PROCESSING);
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CSerialMtMaster::Fsm MESSAGE_RECEIVE_WAITING 3"  << std::endl;
            SetFsmState(DONE_ERROR);
        }
    }
    break;

    case ANSWER_PROCESSING:
        std::cout << "CSerialMtMaster::Fsm ANSWER_PROCESSING"  << std::endl;
        if (AnswerProcessing())
        {
            SetFsmState(DONE_OK);
        }
        else
        {
            SetFsmState(DONE_ERROR);
        }

        break;

    case REQUEST_START:
        std::cout << "CSerialMtMaster::Fsm REQUEST_START"  << std::endl;
        GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
        SetFsmState(BEFORE_REQUEST_WAITING);
        break;

    case BEFORE_REQUEST_WAITING:
//        std::cout << "CSerialMtMaster::Fsm BEFORE_REQUEST_WAITING"  << std::endl;
        // Закончилось время паузы между приёмом и передачей(5 милисекунд)?
        if (GetTimerPointer() -> IsOverflow())
        {
            std::cout << "CSerialMtMaster::Fsm BEFORE_REQUEST_WAITING 2"  << std::endl;
            GetTimerPointer() -> Set(m_uiConfirmationTimeout);
            m_pxOperatingDataContainer -> m_uiFsmCommandState =
                CSerialMtMasterLinkLayer::COMMUNICATION_TRANSMIT_START;
            m_pxSerialMtMasterLinkLayer ->
            SetTaskData(m_pxOperatingDataContainer);
            SetFsmState(AFTER_REQUEST_WAITING);
        }
        break;

    case AFTER_REQUEST_WAITING:
//        std::cout << "CSerialMtMaster::Fsm AFTER_REQUEST_WAITING"  << std::endl;
    {
        m_pxSerialMtMasterLinkLayer ->
        GetTaskData(m_pxOperatingDataContainer);

        uint8_t uiFsmState = m_pxOperatingDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CSerialMtMaster::Fsm AFTER_REQUEST_WAITING 2"  << std::endl;
            SetFsmState(COMMUNICATION_RECEIVE_START);
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CSerialMtMaster::Fsm AFTER_REQUEST_WAITING 3"  << std::endl;
            SetFsmState(DONE_ERROR);
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CSerialMtMaster::Fsm AFTER_REQUEST_WAITING 4"  << std::endl;
                SetFsmState(DONE_ERROR);
            }
        }
    }
    break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------

