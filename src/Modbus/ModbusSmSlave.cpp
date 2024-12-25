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
//    m_pxDeviceControlLink = 0;
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
}

//-------------------------------------------------------------------------------
uint8_t CModbusSmSlave::Init(void)
{
    std::cout << "CModbusSmSlave Init"  << std::endl;
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));
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
CDeviceControl* CModbusSmSlave::GetDeviceContro(void)
{
    return m_pxDeviceControl;
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
uint16_t CModbusSmSlave::ReadExceptionStatus(void)
{
//    errno = ENOPROTOOPT;
    return -1;
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
    uiLength += m_pxModbusSlaveLinkLayer ->
                ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

    std::cout << "CModbusSmSlave::ReportSlaveID 7" << std::endl;
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

    case _FC_READ_EXCEPTION_STATUS:
        uiLength = ReadExceptionStatus();
        break;

    case _FC_REPORT_SLAVE_ID:
        uiLength = ReportSlaveID();
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

//-------------------------------------------------------------------------------
uint16_t CModbusSmSlave::ReadExceptionStatusAnswer(void)
{
//    errno = ENOPROTOOPT;
    return -1;
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
    uiLength += m_pxModbusSlaveLinkLayer ->
                ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

    std::cout << "CModbusSmSlave::ReportSlaveIDAnswer 7" << std::endl;
    return uiLength;
}

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

    case _FC_READ_EXCEPTION_STATUS:
        uiLength = ReadExceptionStatusAnswer();
        break;

    case _FC_REPORT_SLAVE_ID:
        uiLength = ReportSlaveIDAnswer();
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
        Init();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
//        std::cout << "CModbusSmSlave::Fsm INIT 1"  << std::endl;
    {
        CTaskInterface* pxTask =
            GetResources() ->
            GetTaskPointerByNameFromMap(m_sModbusSlaveLinkLayerName);

        if (pxTask != 0)
        {
//                std::cout << "CModbusSmSlave::Fsm INIT 2"  << std::endl;
            if (pxTask -> GetFsmState() >= READY)
            {
//                    std::cout << "CModbusSmSlave::Fsm INIT 3"  << std::endl;
                SetModbusSlaveLinkLayer((CModbusSmSlaveLinkLayer*)pxTask);
                uiReadyTaskCounter += 1;
//                    SetFsmState(READY);
            }
        }
        else
        {
//                std::cout << "CModbusSmSlave::Fsm INIT 4"  << std::endl;
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CModbusSmSlave::Fsm INIT 5"  << std::endl;
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
//                std::cout << "CModbusSmSlave::Fsm INIT 2"  << std::endl;
            if (pxTask -> GetFsmState() >= READY)
            {
//                    std::cout << "CModbusSmSlave::Fsm INIT 3"  << std::endl;
                SetDeviceControl((CDeviceControl*)pxTask);
                uiReadyTaskCounter += 1;
//                    SetFsmState(READY);
            }
        }
        else
        {
            //                std::cout << "CModbusSmSlave::Fsm INIT 4"  << std::endl;
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CModbusSmSlave::Fsm INIT 5"  << std::endl;
                SetFsmState(STOP);
            }
        }
    }

    if (uiReadyTaskCounter > 1)
    {
        std::cout << "CModbusSmSlave::Fsm INIT 9"  << std::endl;
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
//        SetFsmState(MESSAGE_RECEIVE_WAITING);
        SetFsmState(ANSWER_PROCESSING);

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
            if (m_uiLength)
            {
                std::cout << "CModbusSmSlave::Fsm REQUEST_PROCESSING 2"  << std::endl;
                GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
                SetFsmState(ANSWER_PROCESSING_WAITING);
            }
            else
            {
                std::cout << "CModbusSmSlave::Fsm REQUEST_PROCESSING 3"  << std::endl;
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
//        std::cout << "CModbusSmSlave::Fsm ANSWER_PROCESSING_WAITING"  << std::endl;
    {
        m_pxDeviceControl ->
        GetTaskData(m_pxOperatingDataContainer);

        uint8_t uiFsmState = m_pxOperatingDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CModbusSmSlave::Fsm ANSWER_PROCESSING_WAITING 2"  << std::endl;
            GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
            SetFsmState(ANSWER_PROCESSING);
        }
        else if (uiFsmState == DONE_ERROR)
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
//                m_pxDeviceControl ->
//                SetFsmCommandState(0);
                SetFsmState(MESSAGE_RECEIVE_WAITING);
            }
        }
    }
    break;

    case ANSWER_PROCESSING:
        std::cout << "CModbusSmSlave::Fsm ANSWER_PROCESSING"  << std::endl;
//        if (AnswerProcessing())
//        {
//            GetTimerPointer() -> Set(m_uiTransmitDelayTimeout);
//            SetFsmState(BEFORE_ANSWERING_WAITING);
//        }
//        else
//        {
//            SetFsmState(MESSAGE_RECEIVE_WAITING);
//        }



        {
            uint16_t uiPduOffset = m_pxModbusSlaveLinkLayer -> GetPduOffset();
            uint8_t * puiRequest = m_pxModbusSlaveLinkLayer -> GetRxBuffer();
            uint8_t * puiResponse = (m_pxModbusSlaveLinkLayer -> GetTxBuffer());
            uint16_t  uiLength = m_pxModbusSlaveLinkLayer -> GetFrameLength();

            int8_t uiSlave = 1;//puiRequest[uiPduOffset - 1];
            int8_t uiFunctionCode = 23;//puiRequest[uiPduOffset];

            uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
            memcpy(&puiResponse[uiPduOffset + 2], auiTempData, sizeof(auiTempData));
            uiLength += sizeof(auiTempData);

            // количество байт в прикладном сообщении массиве конфигурации, не включая остальные.
            puiResponse[uiPduOffset + 1] = uiLength;//sizeof(auiTempData);// + 1;
            uiLength ++;
            uiLength += m_pxModbusSlaveLinkLayer ->
                        ResponseBasis(uiSlave, uiFunctionCode, puiResponse);

            uiLength = m_pxModbusSlaveLinkLayer -> Tail(puiResponse, uiLength);
            m_pxModbusSlaveLinkLayer -> SetFrameLength(uiLength);

            GetTimerPointer() -> Set(1000);
            SetFsmState(BEFORE_ANSWERING_WAITING);
        }
        break;

    case BEFORE_ANSWERING_WAITING:
//        std::cout << "CModbusSmSlave::Fsm BEFORE_ANSWERING_WAITING"  << std::endl;
        // Закончилось время паузы между приёмом и передачей(5 милисекунд)?
        if (GetTimerPointer() -> IsOverflow())
        {
            std::cout << "CModbusSmSlave::Fsm BEFORE_ANSWERING_WAITING 2"  << std::endl;
            GetTimerPointer() -> Set(m_uiConfirmationTimeout);
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
//        std::cout << "CModbusSmSlave::Fsm AFTER_ANSWERING_WAITING"  << std::endl;
        if (m_pxModbusSlaveLinkLayer -> IsDoneOk())
        {
            std::cout << "CModbusSmSlave::Fsm AFTER_ANSWERING_WAITING 2"  << std::endl;
//            m_pxModbusSlaveLinkLayer ->
//            CommunicationReceiveStart();
//            SetFsmState(MESSAGE_RECEIVE_WAITING);
            SetFsmState(ANSWER_PROCESSING);
        }
        else if (m_pxModbusSlaveLinkLayer -> IsDoneError())
        {
            std::cout << "CModbusSmSlave::Fsm AFTER_ANSWERING_WAITING 3"  << std::endl;
//            SetFsmState(COMMUNICATION_START);
            SetFsmState(ANSWER_PROCESSING);
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CModbusSmSlave::Fsm AFTER_ANSWERING_WAITING 4"  << std::endl;
//                m_pxDeviceControl ->
//                SetFsmCommandState(0);
//                SetFsmState(MESSAGE_RECEIVE_WAITING);
                SetFsmState(ANSWER_PROCESSING);
            }
        }
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------

