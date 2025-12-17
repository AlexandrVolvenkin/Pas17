
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
#include "ModbusMaster.h"
#include "ModbusSlaveLinkLayer.h"
#include "ModbusMasterLinkLayer.h"
#include "ModbusSmMasterLinkLayer.h"

using namespace std;

//-------------------------------------------------------------------------------
CModbusMaster::CModbusMaster()
{
    std::cout << "CModbusMaster constructor"  << std::endl;
    m_pxModbusMasterLinkLayer = 0;
    m_pxDeviceControl = 0;
    // получим имя класса.
    sprintf(GetTaskNamePointer(),
            "%s",
            typeid(*this).name());
    m_puiIntermediateBuff = new uint8_t[MAX_MODBUS_MESSAGE_LENGTH];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CModbusMaster::~CModbusMaster()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
uint8_t CModbusMaster::Init(void)
{
    std::cout << "CModbusMaster Init"  << std::endl;
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));
}

//-------------------------------------------------------------------------------
void CModbusMaster::SetModbusMasterLinkLayerName(std::string sName)
{
    m_sModbusMasterLinkLayerName = sName;
}

//-------------------------------------------------------------------------------
void CModbusMaster::SetModbusMasterLinkLayer(CModbusMasterLinkLayer* pxModbusMasterLinkLayer)
{
    m_pxModbusMasterLinkLayer = pxModbusMasterLinkLayer;
}

//-------------------------------------------------------------------------------
void CModbusMaster::SetDeviceControlName(std::string sName)
{
    m_sDeviceControlName = sName;
}

//-------------------------------------------------------------------------------
void CModbusMaster::SetDeviceControl(CDeviceControl* pxDeviceControl)
{
    m_pxDeviceControl = pxDeviceControl;
}

//-------------------------------------------------------------------------------
CDeviceControl* CModbusMaster::GetDeviceControl(void)
{
    return m_pxDeviceControl;
}

//-------------------------------------------------------------------------------
const char *CModbusMaster::ModbusStringError(int errnum)
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
void CModbusMaster::WorkingArraysInit(void)
{
    std::cout << "CModbusMaster WorkingArraysInit 1"  << std::endl;
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
uint16_t CModbusMaster::ByteToBitPack(uint16_t uiAddress,
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
int8_t CModbusMaster::ReadDiscreteInputsRequest(uint8_t uiSlaveAddress,
        uint16_t uiAddress,
        uint16_t uiNumberB)
{
    if (uiNumberB < 1 || MODBUS_MAX_READ_BITS < uiNumberB)
    {
        return EMBMDATA;
    }
    else if ((uiAddress + uiNumberB) > m_uiDiscreteInputsNumber)
    {
        return EMBBADEXC;
    }
    else
    {
        if (IsTaskReady())
        {
            uint8_t* puiResponse = m_pxModbusMasterLinkLayer -> GetTxBuffer();

            m_uiFunctionCode = _FC_READ_DISCRETE_INPUTS;
            m_uiAddress = uiAddress;
            m_uiQuantity = uiNumberB;
            uint16_t  uiLength = m_pxModbusMasterLinkLayer ->
                                 RequestBasis(uiSlaveAddress,
                                              m_uiFunctionCode,
                                              uiAddress,
                                              uiNumberB,
                                              puiResponse);

            uiLength = m_pxModbusMasterLinkLayer ->
                       Tail(puiResponse, uiLength);
            m_pxModbusMasterLinkLayer ->
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
uint16_t CModbusMaster::ReadDiscreteInputsAnswer(void)
{
    std::cout << "CModbusMaster::ReadDiscreteInputsAnswer 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusMasterLinkLayer -> GetPduOffset();
    uint8_t* puiRequest = m_pxModbusMasterLinkLayer -> GetRxBuffer();


    {
        cout << "CModbusMaster::ReadDiscreteInputsAnswer puiRequest" << endl;
        uint8_t *pucSourceTemp;
        pucSourceTemp = (uint8_t*)puiRequest;
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
    SetBytesFromBits(m_puiDiscreteInputs,
                     m_uiAddress,
                     m_uiQuantity,
                     &puiRequest[uiPduOffset + 2]);

    {
        cout << "CModbusMaster::ReadDiscreteInputsAnswer m_puiDiscreteInputs" << endl;
        uint8_t *pucSourceTemp;
        pucSourceTemp = (uint8_t*)m_puiDiscreteInputs;
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

    return m_uiQuantity;
}

//-------------------------------------------------------------------------------
uint16_t CModbusMaster::ReadExceptionStatus(void)
{
//    errno = ENOPROTOOPT;
    return -1;
}

//-------------------------------------------------------------------------------
uint16_t CModbusMaster::ReportSlaveID(void)
{
    std::cout << "CModbusMaster::ReportSlaveID 1" << std::endl;

//    uint16_t uiPduOffset = m_pxModbusMasterLinkLayer -> GetPduOffset();
//    uint8_t* puiRequest = m_pxModbusMasterLinkLayer -> GetRxBuffer();
//    uint8_t* puiResponse = m_pxModbusMasterLinkLayer -> GetTxBuffer();
//    uint16_t  uiLength = m_pxModbusMasterLinkLayer -> GetFrameLength();
//
//    int8_t uiSlave = puiRequest[uiPduOffset - 1];
//    int8_t uiFunctionCode = puiRequest[uiPduOffset];
//
//    std::cout << "CModbusMaster::ReportSlaveID uiSlave "  << (int)uiSlave << std::endl;
//    std::cout << "CModbusMaster::ReportSlaveID uiFunctionCode "  << (int)uiFunctionCode << std::endl;
//
//    std::cout << "CModbusMaster::ReportSlaveID 4" << std::endl;
//
//    CDeviceControl* pxDeviceControl =
//        (CDeviceControl*)GetResources() ->
//        GetTaskPointerByNameFromMap("DeviceControlRtuUpperLevel");
//
//    uiLength = pxDeviceControl ->
//               ConfigurationRead(&puiResponse[uiPduOffset + 2]);
////    uiLength = m_pxResources ->
////               m_pxDeviceControl ->
////               ConfigurationRead(&puiResponse[uiPduOffset + 2]);
//
////    uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
////    memcpy(&puiResponse[uiPduOffset + 2], auiTempData, sizeof(auiTempData));
////    uiLength += sizeof(auiTempData);
//
//    // количество байт в прикладном сообщении массиве конфигурации, не включая остальные.
//    puiResponse[uiPduOffset + 1] = uiLength;//sizeof(auiTempData);// + 1;
//    uiLength ++;
//    uiLength += m_pxModbusMasterLinkLayer ->
//                ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
//
//    std::cout << "CModbusMaster::ReportSlaveID 7" << std::endl;
//    return uiLength;
//
//
//
////        {
////            uint16_t uiPduOffset = m_pxModbusMasterLinkLayer -> GetPduOffset();
////            uint8_t* puiRequest = m_pxModbusMasterLinkLayer -> GetRxBuffer();
////            uint8_t* puiResponse = (m_pxModbusMasterLinkLayer -> GetTxBuffer());
////            uint16_t  uiLength = m_pxModbusMasterLinkLayer -> GetFrameLength();
////
////            int8_t uiSlave = 1;//puiRequest[uiPduOffset - 1];
////            int8_t uiFunctionCode = 23;//puiRequest[uiPduOffset];
////
////            uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
////            memcpy(&puiResponse[uiPduOffset + 2], auiTempData, sizeof(auiTempData));
////            uiLength += sizeof(auiTempData);
////
////            // количество байт в прикладном сообщении массиве конфигурации, не включая остальные.
////            puiResponse[uiPduOffset + 1] = uiLength;//sizeof(auiTempData);// + 1;
////            uiLength ++;
////            uiLength += m_pxModbusMasterLinkLayer ->
////                        ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
////
////            uiLength = m_pxModbusMasterLinkLayer -> Tail(puiResponse, uiLength);
////            m_pxModbusMasterLinkLayer -> SetFrameLength(uiLength);
////
////            GetTimerPointer() -> Set(1000);
////            SetFsmState(BEFORE_REQUEST_WAITING);
////        }
}









//-------------------------------------------------------------------------------
////AnswerProcessing

//-------------------------------------------------------------------------------
uint16_t CModbusMaster::ReadExceptionStatusAnswer(void)
{
//    errno = ENOPROTOOPT;
    return -1;
}

//-------------------------------------------------------------------------------
uint16_t CModbusMaster::ReportSlaveIDAnswer(void)
{
    std::cout << "CModbusMaster::ReportSlaveIDAnswer 1" << std::endl;

//    uint16_t uiPduOffset = m_pxModbusMasterLinkLayer -> GetPduOffset();
//    uint8_t* puiRequest = m_pxModbusMasterLinkLayer -> GetRxBuffer();
//    uint8_t* puiResponse = m_pxModbusMasterLinkLayer -> GetTxBuffer();
//    uint16_t  uiLength = m_pxModbusMasterLinkLayer -> GetFrameLength();
//
//    int8_t uiSlave = puiRequest[uiPduOffset - 1];
//    int8_t uiFunctionCode = puiRequest[uiPduOffset];
//
//    std::cout << "CModbusMaster::ReportSlaveIDAnswer uiSlave "  << (int)uiSlave << std::endl;
//    std::cout << "CModbusMaster::ReportSlaveIDAnswer uiFunctionCode "  << (int)uiFunctionCode << std::endl;
//
//    std::cout << "CModbusMaster::ReportSlaveIDAnswer 4" << std::endl;
//
//    CDeviceControl* pxDeviceControl =
//        (CDeviceControl*)GetResources() ->
//        GetTaskPointerByNameFromMap("DeviceControlRtuUpperLevel");
//
//    uiLength = pxDeviceControl ->
//               ConfigurationRead(&puiResponse[uiPduOffset + 2]);
////    uiLength = m_pxResources ->
////               m_pxDeviceControl ->
////               ConfigurationRead(&puiResponse[uiPduOffset + 2]);
//
////    uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
////    memcpy(&puiResponse[uiPduOffset + 2], auiTempData, sizeof(auiTempData));
////    uiLength += sizeof(auiTempData);
//
//    // количество байт в прикладном сообщении массиве конфигурации, не включая остальные.
//    puiResponse[uiPduOffset + 1] = uiLength;//sizeof(auiTempData);// + 1;
//    uiLength ++;
//    uiLength += m_pxModbusMasterLinkLayer ->
//                ResponseBasis(uiSlave, uiFunctionCode, puiResponse);
//
//    std::cout << "CModbusMaster::ReportSlaveIDAnswer 7" << std::endl;
//    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CModbusMaster::AnswerProcessing(void)
{
    std::cout << "CModbusMaster::AnswerProcessing 1" << std::endl;

    uint16_t uiPduOffset = m_pxModbusMasterLinkLayer -> GetPduOffset();
    uint8_t* puiRequest = m_pxModbusMasterLinkLayer -> GetRxBuffer();
    uint8_t* puiResponse = m_pxModbusMasterLinkLayer -> GetTxBuffer();
    uint16_t  uiLength = m_pxModbusMasterLinkLayer -> GetFrameLength();

    int8_t uiSlave = puiRequest[uiPduOffset - 1];
    int8_t uiFunctionCode = puiRequest[uiPduOffset];

    std::cout << "CModbusMaster::AnswerProcessing uiSlave "  << (int)uiSlave << std::endl;
    std::cout << "CModbusMaster::AnswerProcessing uiFunctionCode "  << (int)uiFunctionCode << std::endl;

    /* Filter on the Modbus unit identifier (slave) in RTU mode */
    if (uiSlave != m_uiOwnAddress && uiSlave != MODBUS_BROADCAST_ADDRESS)
    {
        std::cout << "CModbusMaster::AnswerProcessing 2" << std::endl;
        return 0;
    }

    // проверяем сохранённый локально текущий код функции.
    switch (m_uiFunctionCode)
    {
        std::cout << "CModbusMaster::AnswerProcessing 3" << std::endl;

    case _FC_READ_DISCRETE_INPUTS:
        uiLength = ReadDiscreteInputsAnswer();
        break;

    case _FC_READ_EXCEPTION_STATUS:
        uiLength = ReadExceptionStatusAnswer();
        break;

    case _FC_REPORT_SLAVE_ID:
        uiLength = ReportSlaveIDAnswer();
        break;

    default:
        std::cout << "CModbusMaster::AnswerProcessing 4" << std::endl;
        uiLength = m_pxModbusMasterLinkLayer ->
                   ResponseException(uiSlave,
                                     uiFunctionCode,
                                     MODBUS_EXCEPTION_ILLEGAL_FUNCTION,
                                     puiResponse);
        break;
    }

    std::cout << "CModbusMaster::AnswerProcessing 5" << std::endl;
    return uiLength;
}

//-------------------------------------------------------------------------------
void CModbusMaster::SetByteFromBits(uint8_t *dest, uint16_t index, const uint8_t value)
{
    uint16_t i;

    for (i=0; i<8; i++)
    {
        dest[index+i] = (value & (1 << i)) ? 1 : 0;
    }
}

//-------------------------------------------------------------------------------
void CModbusMaster::SetBytesFromBits(uint8_t *dest, uint16_t index, uint16_t nb_bits,
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
uint8_t CModbusMaster::GetByteFromBits(const uint8_t *src, uint16_t index,
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
float CModbusMaster::GetFloat(const uint16_t *src)
{
    float f = 0.0f;
    uint32_t i;

    i = (((uint32_t)src[1]) << 16) + src[0];
    memcpy(&f, &i, sizeof(float));

    return f;
}

//-------------------------------------------------------------------------------
void CModbusMaster::SetFloat(float f, uint16_t *dest)
{
    uint32_t i = 0;

    memcpy(&i, &f, sizeof(uint32_t));
    dest[0] = (uint16_t)i;
    dest[1] = (uint16_t)(i >> 16);
}

//-------------------------------------------------------------------------------
uint8_t CModbusMaster::Fsm(void)
{
//    std::cout << "CModbusMaster::Fsm 1" << endl;
    uint8_t uiReadyTaskCounter = 0;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CModbusMaster::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CModbusMaster::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CModbusMaster::Fsm START"  << std::endl;
        std::cout << "CModbusMaster::Fsm m_sModbusMasterLinkLayerName" << " " << (m_sModbusMasterLinkLayerName) << std::endl;
        Init();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
//        std::cout << "CModbusMaster::Fsm INIT 1"  << std::endl;
    {
        CTaskInterface* pxTask =
            GetResources() ->
            GetTaskPointerByNameFromMap(m_sModbusMasterLinkLayerName);

        if (pxTask != 0)
        {
//                std::cout << "CModbusMaster::Fsm INIT 2"  << std::endl;
            if (pxTask -> GetFsmState() >= READY)
            {
//                    std::cout << "CModbusMaster::Fsm INIT 3"  << std::endl;
                SetModbusMasterLinkLayer((CModbusMasterLinkLayer*)pxTask);
                uiReadyTaskCounter += 1;
            }
        }
        else
        {
//                std::cout << "CModbusMaster::Fsm INIT 4"  << std::endl;
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CModbusMaster::Fsm INIT 5"  << std::endl;
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
//                std::cout << "CModbusMaster::Fsm INIT 2"  << std::endl;
            if (pxTask -> GetFsmState() >= READY)
            {
//                    std::cout << "CModbusMaster::Fsm INIT 3"  << std::endl;
                SetDeviceControl((CDeviceControl*)pxTask);
                uiReadyTaskCounter += 1;
            }
        }
        else
        {
            //                std::cout << "CModbusMaster::Fsm INIT 4"  << std::endl;
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CModbusMaster::Fsm INIT 5"  << std::endl;
                SetFsmState(STOP);
            }
        }
    }

    if (uiReadyTaskCounter > 1)
    {
        std::cout << "CModbusMaster::Fsm INIT 9"  << std::endl;
        SetFsmState(COMMUNICATION_START);
    }
    break;

    case READY:
//        std::cout << "CModbusMaster::Fsm READY"  << std::endl;
        break;

    case DONE_OK:
//        std::cout << "CModbusMaster::Fsm DONE_OK"  << std::endl;
        SetFsmOperationStatus(DONE_OK);
        break;

    case DONE_ERROR:
//        std::cout << "CModbusMaster::Fsm DONE_ERROR"  << std::endl;
        SetFsmOperationStatus(DONE_ERROR);
        break;

    case COMMUNICATION_START:
        std::cout << "CModbusMaster::Fsm COMMUNICATION_START"  << std::endl;
        m_pxOperatingDataContainer -> m_uiFsmCommandState =
            CModbusSmMasterLinkLayer::COMMUNICATION_START;
        m_pxModbusMasterLinkLayer ->
        SetTaskData(m_pxOperatingDataContainer);
        SetFsmState(READY);
        break;

    case COMMUNICATION_RECEIVE_START:
        std::cout << "CModbusMaster::Fsm COMMUNICATION_RECEIVE_START"  << std::endl;
        m_pxOperatingDataContainer -> m_uiFsmCommandState =
            CModbusSmMasterLinkLayer::COMMUNICATION_RECEIVE_START;
        m_pxModbusMasterLinkLayer ->
        SetTaskData(m_pxOperatingDataContainer);
        SetFsmState(MESSAGE_RECEIVE_WAITING);
        break;

    case MESSAGE_RECEIVE_WAITING:
//        std::cout << "CModbusMaster::Fsm MESSAGE_RECEIVE_WAITING"  << std::endl;
    {
        m_pxModbusMasterLinkLayer ->
        GetTaskData(m_pxOperatingDataContainer);

        uint8_t uiFsmState = m_pxOperatingDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CModbusMaster::Fsm MESSAGE_RECEIVE_WAITING 2"  << std::endl;
            SetFsmState(EXECUTOR_ANSWER_PROCESSING);
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CModbusMaster::Fsm MESSAGE_RECEIVE_WAITING 3"  << std::endl;
            SetFsmState(DONE_ERROR);
        }
    }
    break;

    case EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CModbusMaster::Fsm EXECUTOR_ANSWER_PROCESSING"  << std::endl;
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
        std::cout << "CModbusMaster::Fsm REQUEST_START"  << std::endl;
        GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
        SetFsmState(BEFORE_REQUEST_WAITING);
        break;

    case BEFORE_REQUEST_WAITING:
//        std::cout << "CModbusMaster::Fsm BEFORE_REQUEST_WAITING"  << std::endl;
        // Закончилось время паузы между приёмом и передачей(5 милисекунд)?
        if (GetTimerPointer() -> IsOverflow())
        {
            std::cout << "CModbusMaster::Fsm BEFORE_REQUEST_WAITING 2"  << std::endl;
            GetTimerPointer() -> Set(m_uiConfirmationTimeout);
            m_pxOperatingDataContainer -> m_uiFsmCommandState =
                CModbusSmMasterLinkLayer::COMMUNICATION_TRANSMIT_START;
            m_pxModbusMasterLinkLayer ->
            SetTaskData(m_pxOperatingDataContainer);
            SetFsmState(AFTER_REQUEST_WAITING);
        }
        break;

    case AFTER_REQUEST_WAITING:
//        std::cout << "CModbusMaster::Fsm AFTER_REQUEST_WAITING"  << std::endl;
    {
        m_pxModbusMasterLinkLayer ->
        GetTaskData(m_pxOperatingDataContainer);

        uint8_t uiFsmState = m_pxOperatingDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CModbusMaster::Fsm AFTER_REQUEST_WAITING 2"  << std::endl;
            SetFsmState(COMMUNICATION_RECEIVE_START);
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CModbusMaster::Fsm AFTER_REQUEST_WAITING 3"  << std::endl;
            SetFsmState(DONE_ERROR);
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CModbusMaster::Fsm AFTER_REQUEST_WAITING 4"  << std::endl;
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

