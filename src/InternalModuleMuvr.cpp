//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include "Timer.h"
#include "Task.h"
#include "Platform.h"
#include "Resources.h"
#include "DataStore.h"
#include "Crc.h"
#include "DataContainer.h"
#include "CommunicationDevice.h"
#include "SpiCommunicationDevice.h"
#include "STEP5_floating_point.h"
#include "AnalogueSignals.h"
#include "InternalModule.h"
#include "InternalModuleMuvr.h"

using namespace std;

//-------------------------------------------------------------------------------
CInternalModuleMuvr::CInternalModuleMuvr()
{
    //std::cout << "CInternalModuleMuvr constructor"  << std::endl;
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CInternalModuleMuvr::~CInternalModuleMuvr()
{
    //dtor
}

//-------------------------------------------------------------------------------
void CInternalModuleMuvr::SetCommunicationDeviceName(std::string sName)
{
    m_sCommunicationDeviceName = sName;
}

//-------------------------------------------------------------------------------
void CInternalModuleMuvr::SetCommunicationDevice(CCommunicationDeviceInterface* pxCommunicationDevice)
{
    m_pxCommunicationDevice = pxCommunicationDevice;
}

//-------------------------------------------------------------------------------
uint8_t CInternalModuleMuvr::Init(void)
{
    //std::cout << "CInternalModuleMuvr Init"  << std::endl;
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
    SetCustomerDataContainer(GetExecutorDataContainerPointer());

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

    Allocate();
}

////-------------------------------------------------------------------------------
//bool CInternalModuleMuvr::SetTaskData(CDataContainerDataBase* pxDataContainer)
//{
////    //std::cout << "CInternalModuleMuvr::SetTaskData 1" << std::endl;
//
//    if (IsTaskReady())
//    {
////        //std::cout << "CInternalModuleMuvr::SetTaskData 2" << std::endl;
//        *m_pxOperatingDataContainer = *pxDataContainer;
//        SetFsmState(m_pxOperatingDataContainer -> m_uiFsmCommandState);
//        return true;
//    }
//    else
//    {
//        //std::cout << "CInternalModuleMuvr::SetTaskData 3" << std::endl;
//        return false;
//    }
//}

////-------------------------------------------------------------------------------
//bool CInternalModuleMuvr::GetTaskData(CDataContainerDataBase* pxDataContainer)
//{
//    //std::cout << "CInternalModuleMuvr::GetTaskData 1" << std::endl;
//
//    m_pxOperatingDataContainer -> m_uiFsmCommandState = GetFsmState();
//    *pxDataContainer = *m_pxOperatingDataContainer;
//
//    return true;
//}

////-------------------------------------------------------------------------------
//uint8_t CInternalModuleMuvr::GetType(void)
//{
//    return MISSING_COMPONENT;
//}
//
////-------------------------------------------------------------------------------
//uint8_t CInternalModuleMuvr::GetWorkingStatus(void)
//{
//    return MISSING_COMPONENT;
//}

////-------------------------------------------------------------------------------
//void CInternalModuleMuvr::SetAddress(uint8_t uiAddress)
//{
//    m_uiAddress = uiAddress;
//}
//
////-------------------------------------------------------------------------------
//uint8_t CInternalModuleMuvr::GetAddress(void)
//{
//    return m_uiAddress;
//}
//
////-------------------------------------------------------------------------------
//bool CInternalModuleMuvr::IsAbleToReplace(uint8_t uiType)
//{
//    return false;
//}
//
////-------------------------------------------------------------------------------
//bool CInternalModuleMuvr::IsReadyToStartWork(void)
//{
//    return false;
//}

//-------------------------------------------------------------------------------
void CInternalModuleMuvr::Allocate(void)
{
    std::cout << "CInternalModuleMuvr::Allocate 1"  << std::endl;

//    m_uiAddress = xMemoryAllocationContext.uiAddress;
//    m_puiRxBuffer = xMemoryAllocationContext.puiRxBuffer;
//    m_puiTxBuffer = xMemoryAllocationContext.puiTxBuffer;
//    m_puiErrorCode = xMemoryAllocationContext.puiErrorCode;

    // ������� ��������� �� ����� � ������� ���������� ������ ��� �������� ������.
    m_puiDiscreteInputsState =
        &(GetResources() ->
          m_puiDiscreteInputsState[GetResources() ->
                                                  m_uiUsedDiscreteInputsState]);
    // �������� ����� ����� ���������� ������.
    GetResources() ->
    m_uiUsedDiscreteInputsState +=
        MUVR_DISCRETE_SIGNALS_QUANTITY;


    // ������� ��������� �� ����� � ������� ������������� ���������� ������ ��� �������� ������.
    m_puiDiscreteInputsBadState =
        &(GetResources() ->
          m_puiDiscreteInputsBadState[GetResources() ->
                                                     m_uiUsedDiscreteInputsBadState]);
    // �������� ����� ����� ���������� ������.
    GetResources() ->
    m_uiUsedDiscreteInputsBadState +=
        MUVR_DISCRETE_SIGNALS_QUANTITY;


    // ��������� ����� ��� ���������� ����������� ��������.
    m_pxDiscreteOutputControl =
        &(GetResources() ->
          m_pxDiscreteOutputControl[GetResources() ->
                                                   m_uiUsedDiscreteOutputControl]);
    // �������� ����� ����� ���������� ������.
    GetResources() ->
    m_uiUsedDiscreteOutputControl +=
        MUVR_MR_DISCRETE_OUTPUT_NUMBER;


    // ������� ��������� �� ����� � ������� ���������� ������ ��� �������� ������.
    m_pfAnalogueInputsValue =
        &(GetResources() ->
          m_pfAnalogueInputsValue[GetResources() ->
                                                 m_uiUsedAnalogueInputsValue]);
    // �������� ����� ����� ���������� ������.
    GetResources() ->
    m_uiUsedAnalogueInputsValue +=
        MUVR_ANALOG_INPUT_QUANTITY;


    // ������� ��������� �� ����� � ������� ��������� ���������� ������ ��� �������� ������.
    m_puiAnalogueInputsState =
        &(GetResources() ->
          m_puiAnalogueInputsState[GetResources() ->
                                                  m_uiUsedAnalogueInputsState]);
    // �������� ����� ����� ���������� ������.
    GetResources() ->
    m_uiUsedAnalogueInputsBadState +=
        MUVR_ANALOG_INPUT_QUANTITY;


    // ������� ��������� �� ����� � ������� ������������� ���������� ������ ��� �������� ������.
    m_puiAnalogueInputsBadState =
        &(GetResources() ->
          m_puiAnalogueInputsBadState[GetResources() ->
                                                     m_uiUsedAnalogueInputsBadState]);
    // �������� ����� ����� ���������� ������.
    GetResources() ->
    m_uiUsedAnalogueInputsBadState +=
        MUVR_ANALOG_INPUT_QUANTITY;


    // ������� ��������� �� ����� � ������� ��������� ���������� �������� ����������� ����������� �������.
    m_puiAnalogueInputDiscreteInputsState =
        &(GetResources() ->
          m_puiAnalogueInputDiscreteInputsState[GetResources() ->
                                 m_uiUsedAnalogueInputDiscreteInputsState]);
    // �������� ����� ����� ���������� ������.
    GetResources() ->
    m_uiUsedAnalogueInputDiscreteInputsState +=
        MUVR_DISCRETE_SIGNALS_QUANTITY;


    // ������� ��������� �� ����� � ������� ������ ��������������� ��������� ���������� �������� ����������� ����������� �������.
    m_puiAnalogueInputDiscreteInputsBadState =
        &(GetResources() ->
          m_puiAnalogueInputDiscreteInputsBadState[GetResources() ->
                                 m_uiUsedAnalogueInputDiscreteInputsBadState]);
    // �������� ����� ����� ���������� ������.
    GetResources() ->
    m_uiUsedAnalogueInputDiscreteInputsBadState +=
        MUVR_DISCRETE_SIGNALS_QUANTITY;


    // ������� ��������� �� ����� � ������� ���������� ���������� ������ ��� �������� ������.
    m_puiAnalogueInputsOff =
        &(GetResources() ->
          m_puiAnalogueInputsOff[GetResources() ->
                                                m_uiUsedAnalogueInputsOff]);
    // �������� ����� ����� ���������� ������.
    GetResources() ->
    m_uiUsedAnalogueInputsOff +=
        MUVR_ANALOG_INPUT_QUANTITY;


    // ������� ��������� �� ����� � ������� ������������� �� ���������� ������ ���� ������
    // ���������� ��������.
    m_pxAnalogueInputDescriptionWork =
        &(GetResources() ->
          m_pxAnalogueInputDescriptionWork[GetResources() ->
                                                          m_uiUsedAnalogueInputDescriptionWork]);
    // �������� ����� ����� ���������� ������.
    GetResources() ->
    m_uiUsedAnalogueInputDescriptionWork +=
        MUVR_ANALOG_INPUT_QUANTITY;


    // ������� ��������� �� ����� � ������� ���������� ������ ��� �������� ������.
    m_puiReperPointsAdcBuffer =
        &(GetResources() ->
          m_puiReperPointsAdcBuffer[GetResources() ->
                                                   m_uiUsedReperPointsAdcBuffer]);
    // �������� ����� ����� ���������� ������.
    GetResources() ->
    m_uiUsedReperPointsAdcBuffer +=
        ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH;


    m_uiBadAnswerCounter = 0;
}

////-------------------------------------------------------------------------------
//uint8_t CInternalModuleMuvr::GetModuleType(uint8_t uiAddress)
//{
//    //std::cout << "CInternalModuleMuvr::GetModuleType 1"  << std::endl;
//    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];
//
//    usleep(10000);
//    auiSpiTxBuffer[0] = GET_MODULE_TYPE_COMMAND;
//    m_pxCommunicationDevice -> Exchange(uiAddress,
//                                        auiSpiTxBuffer,
//                                        auiSpiRxBuffer,
//                                        SPI_PREAMBLE_LENGTH +
//                                        GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH +
//                                        ONE_BYTE_CRC_LENGTH,
//                                        LOW_SPEED_IN_HZ);
//    //std::cout << "CInternalModuleMuvr::GetModuleType 2"  << std::endl;
//
//    {
//        cout << "GET_MODULE_TYPE_COMMAND auiSpiRxBuffer" << endl;
//        uint8_t *pucSourceTemp;
//        pucSourceTemp = (uint8_t*)auiSpiRxBuffer;
//        for(int i=0; i<16; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//    }
//
//    // ������ ��������?
//    if (auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET] ==
//            GET_MODULE_TYPE_COMMAND) // if echo answer is ok.
//    {
//        //std::cout << "CInternalModuleMuvr::GetModuleType 3"  << std::endl;
////                //std::cout << "CInternalModuleMuvr::GetModuleType ucSlaveAddress 1 "  << (int)ucSlaveAddress << std::endl;
//        // ����� ������� ������ ��� ������?
//        if ((iCrcSummOneByteCompare(&auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET],
//                                    GET_MODULE_TYPE_COMMAND_LENGTH +
//                                    GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH) > 0))
//        {
//            //std::cout << "CInternalModuleMuvr::GetModuleType 4"  << std::endl;
////            // ������ ������������. �������� �������� ���������� -
////            // ����������� ���������� ������� � �������.
////            (xPlcConfigSearchTemp.ui8ModulesQuantity)++;
////            // �������� ��� ������ � ������� ��� �������������� ���������� ������� ��� ������ �� SPI.
////            xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Type =
////                auiSpiRxBuffer[MODULE_TYPE_OFFSET];
////            // �������� ����� ������ � ������� ��� �������������� ���������� ������� ��� ������ �� SPI.
////            xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address =
////                ucSlaveAddress;
////
////            AddSystemComponent(auiSpiRxBuffer[MODULE_TYPE_OFFSET],
////                               ucSlaveAddress);
////            //std::cout << "CInternalModuleMuvr::GetModuleType ucSlaveAddress "  << (int)xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address << std::endl;
////            //std::cout << "CInternalModuleMuvr::GetModuleType ui8Type "  << (int)xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Type << std::endl;
//        }
//        else
//        {
//            //std::cout << "CInternalModuleMuvr::GetModuleType 5"  << std::endl;
////            // �������� ����� ������ � ������� ��� �������������� ���������� ������� ��� ������ �� SPI.
////            xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address =
////                ucSlaveAddress;
////
////            AddSystemComponent(0,
////                               ucSlaveAddress);
//        }
//    }
//    //std::cout << "CInternalModuleMuvr::GetModuleType 6"  << std::endl;
//}

//-------------------------------------------------------------------------------
uint8_t CInternalModuleMuvr::DataExchange(void)
{
//    std::cout << "CInternalModuleMuvr::DataExchange 1"  << std::endl;
    unsigned short usData;
    float fData;
    uint8_t ucCalibrPlus;
    uint8_t ucCalibrMinus;
    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    memset(auiSpiTxBuffer, 0, 250);
    ucCalibrPlus = 0;
    ucCalibrMinus = 0;
    uint8_t uiCommonIndex = GetCommonIndex();

    // ��������� ����?(���� ucCommonIndex �� ����� 0, �� �������� ����� ������������ �����)
    if ((uiCommonIndex > 0) && (uiCommonIndex < (MUVR_TXS_INPUT_NUMBER + 1)))
    {
//        std::cout << "CInternalModuleMuvr::DataExchange 12"  << std::endl;
        // ������� ����� ������������ �����.
        ucCalibrPlus = uiCommonIndex;
        // ��������� ������ ����� ���?
        if (GetCommandControl() == MUVR_COMMAND_CONTROL_SET_BOTTOM_OF_SCALE)
        {
            std::cout << "CInternalModuleMuvr::DataExchange 13"  << std::endl;
            SetCommandControl(0);
            // ������� ������� ������.
            ucCalibrPlus |= MUVR_SET_BOTTOM_OF_SCALE;
        }
        // ��������� ����� ����� ���?
        else if (GetCommandControl() == MUVR_COMMAND_CONTROL_SET_TOP_OF_SCALE)
        {
            std::cout << "CInternalModuleMuvr::DataExchange 14"  << std::endl;
            SetCommandControl(0);
            // ������� ������� ������.
            ucCalibrPlus |= MUVR_SET_TOP_OF_SCALE;
        }
        // ������ ���������� ���������� ������, � ���������� ���� � �����(ucCalibrPlus) �
        // � ���������������(!ucCalibrPlus + 1) � �����(ucCalibrMinus).
        // ��� ��������, ����� (ucCalibrPlus + ucCalibrMinus) ������ ���� 0.
        ucCalibrMinus = 0x00 - ucCalibrPlus;
    }
    auiSpiTxBuffer[(SPI_PREAMBLE_LENGTH +
                    MUVR_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH)] = ucCalibrPlus;
    auiSpiTxBuffer[(SPI_PREAMBLE_LENGTH +
                    MUVR_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH + 1)] = ucCalibrMinus;
    auiSpiTxBuffer[(SPI_PREAMBLE_LENGTH +
                    MUVR_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH + 2)] = 0x7e;

    auiSpiTxBuffer[0] = MUVR_GET_MEASURE_DATA_COMMAND;
    // ������ ������� 0-16383 (4�)
    auiSpiTxBuffer[1] = 0;
    auiSpiTxBuffer[2] = 0;
    auiSpiTxBuffer[3] = 0;
    auiSpiTxBuffer[4] = 0;

    uint8_t uiData = 0;
    // �������� ���������� ��������� ��� ������� ����.
    for (uint8_t i = 0; i < MUVR_MR_DISCRETE_OUTPUT_NUMBER; i++)
    {
        if (m_pxDiscreteOutputControl[i].uiRelayActivationRequest)
        {
            uiData |= (1 << i);
        }
    }
    // ���������� ���� ��� ���� (1�)
    auiSpiTxBuffer[5] = uiData;

    usData = usCrcSummTwoByteCalculation(&auiSpiTxBuffer[1],
                                         5);
    auiSpiTxBuffer[6] = (uint8_t)usData;
    auiSpiTxBuffer[7] = (uint8_t)(usData >> 8);

//    {
//        std::cout << "CInternalModuleMuvr::DataExchange auiSpiTxBuffer"  << std::endl;
//        uint8_t *pucSourceTemp;
//        pucSourceTemp = (uint8_t*)&auiSpiTxBuffer[0];
//        for(int i=0; i<32 ; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//    }

//    usleep(10000);
    m_pxCommunicationDevice -> Exchange(GetAddress(),
                                        auiSpiTxBuffer,
                                        auiSpiRxBuffer,
                                        SPI_PREAMBLE_LENGTH +
                                        MUVR_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH +
                                        TWO_BYTE_CRC_LENGTH +
                                        TAIL_ANSWER_LENGTH,
                                        LOW_SPEED_IN_HZ);

//    std::cout << "CInternalModuleMuvr::DataExchange auiSpiRxBuffer"  << std::endl;
//    uint8_t *pucSourceTemp;
//    pucSourceTemp = (uint8_t*)&auiSpiRxBuffer[0];
//    for(int i=0; i<32 ; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }

    // ��� ������� ������?
    switch(auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET])
    {
    case MUVR_ANSWER_REPER_POINTS_ADC_DATABASE_ERROR:
    //std::cout << "CInternalModuleMuvr::DataExchange 15"  << std::endl;
    // ������ �� �������� �����, �� ����� ����������� ������.
    case MUVR_GET_MEASURE_DATA_COMMAND:
        //std::cout << "CInternalModuleMuvr::DataExchange 16"  << std::endl;
        // ������ �� ����������?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   MUVR_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH) > 0)
        {
            //std::cout << "CInternalModuleMuvr::DataExchange 17"  << std::endl;
            // ������ ��������.
            SetBadAnswerCounter(BAD_MODULE_CYCLE_COUNT_DEFAULT);
//            // ������� ���� ������ ������.
//            *(m_puiModuleBadStateBuffer) = BAD_MODULE_RESPONDED_OK;

            // ������� ������ ��������� ������� ���������� ������.
            memcpy(m_puiAnalogueInputsState,
                   &auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET],
                   MUVR_ANALOG_INPUT_QUANTITY);

            // ������� ���������� �������� ���� ���������� ������ ������.
            for (uint8_t i = 0; i < MUVR_ANALOG_INPUT_QUANTITY; i++)
            {
                //std::cout << "CInternalModuleMuvr::DataExchange 2"  << std::endl;
                // ���� ����� ����� ��� ������������ ��� �������� �������� �����?
                if ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_LINE_BREAK) ||
                        (auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_CALCULATION_OVERFLOW))
                {
                    //std::cout << "CInternalModuleMuvr::DataExchange 3"  << std::endl;
                    // ������ ����� ������������, ������� ��.
                    memset(&(m_pfAnalogueInputsValue[i]),
                           0,
                           sizeof(float));
                    //std::cout << "CInternalModuleMuvr::DataExchange 31"  << std::endl;
                    // ��������� ���� ��������������� - ���� ������������.
                    m_puiAnalogueInputsBadState[i] = 1;
                    //std::cout << "CInternalModuleMuvr::DataExchange 32"  << std::endl;
//                    // ���������� ������ ����� ������������, ������� ��.
//                    memset(&(m_puiDiscreteInputsState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
//                           0,
//                           MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                    // ���������� ������ ����� ������������, ������� ��.
                    memset(&(m_puiAnalogueInputDiscreteInputsState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
                           0,
                           MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                    //std::cout << "CInternalModuleMuvr::DataExchange 33"  << std::endl;
                    // ��������� ����� ��������������� ������� LL, L, H, HH - ������������.
//                    memset(&(m_puiDiscreteInputsBadState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
//                           1,
//                           MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                    memset(&(m_puiAnalogueInputDiscreteInputsBadState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
                           1,
                           MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                    //std::cout << "CInternalModuleMuvr::DataExchange 34"  << std::endl;
                }
                // ������� ����� ���������� �������� �����?
                else if(
                    (((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_LINE_BREAK) == 0) &&
                     ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_CALCULATION_OVERFLOW) == 0)) &&
                    ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_CHANNEL_CALIBRATION)))
                {
                    //std::cout << "CInternalModuleMuvr::DataExchange 4"  << std::endl;
                    // ������� ���������� �������� � �����������.
                    fData = fStep5ToFloat(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                                                                               (i * MUVR_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY)]);
                    // �������� ��� � ������� ������.
                    memcpy(&(m_pfAnalogueInputsValue[i]),
                           (uint8_t*)&fData,
                           sizeof(float));
                    // ��������� ���� ��������������� - ���� ������������.
                    m_puiAnalogueInputsBadState[i] = 1;
//                    // ���������� ������ ����� ������������, ������� ��.
//                    memset(&(m_puiDiscreteInputsState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
//                           0,
//                           MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                    // ���������� ������ ����� ������������, ������� ��.
                    memset(&(m_puiAnalogueInputDiscreteInputsState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
                           0,
                           MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                    // ��������� ����� ��������������� ������� LL, L, H, HH - ������������.
//                    memset(&(m_puiDiscreteInputsBadState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
//                           1,
//                           MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                    memset(&(m_puiAnalogueInputDiscreteInputsBadState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
                           1,
                           MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                }
                else
                {
                    //std::cout << "CInternalModuleMuvr::DataExchange 5"  << std::endl;
                    // ���������� ���� ������� �� ���������?
                    if (m_puiAnalogueInputsOff[i])
                    {
                        //std::cout << "CInternalModuleMuvr::DataExchange 51"  << std::endl;
                        // ������ ������ ������ �������� �� ���������, ������� ��.
                        memset(&(m_pfAnalogueInputsValue[i]),
                               0,
                               sizeof(float));
                        // ��������� ���� ��������������� - ���� ������������.
                        m_puiAnalogueInputsBadState[i] = 1;
//                        // ���������� ������ ����� ������������, ������� ��.
//                        memset(&(m_puiDiscreteInputsState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
//                               0,
//                               MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                        // ���������� ������ ����� ������������, ������� ��.
                        memset(&(m_puiAnalogueInputDiscreteInputsState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
                               0,
                               MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                        // ��������� ����� ��������������� ������� LL, L, H, HH - ������������.
//                        memset(&(m_puiDiscreteInputsBadState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
//                               1,
//                               MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                        memset(&(m_puiAnalogueInputDiscreteInputsBadState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
                               1,
                               MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                    }
                    else
                    {
                        //std::cout << "CInternalModuleMuvr::DataExchange 6"  << std::endl;
                        // ������� ���������� �������� � �����������.
                        fData = fStep5ToFloat(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                                                                                   (i * MUVR_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY)]);
                        // �������� ��� � ������� ������.
                        memcpy(&(m_pfAnalogueInputsValue[i]),
                               (uint8_t*)&fData,
                               sizeof(float));
                        // ������� ���� ��������������� - ���� ����������.
                        m_puiAnalogueInputsBadState[i] = 0;
                        //                        // ������� ����� ������� LL, L, H, HH.
//                        memset(&(m_puiDiscreteInputsState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
//                               0,
//                               MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                        // ������� ����� ������� LL, L, H, HH.
                        memset(&(m_puiAnalogueInputDiscreteInputsState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
                               0,
                               MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                        // ������� ����� ��������������� ������� LL, L, H, HH - ����������.
                        //                        memset(&(m_puiDiscreteInputsBadState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
//                               0,
//                               MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                        memset(&(m_puiAnalogueInputDiscreteInputsBadState[i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH]),
                               0,
                               MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
                        // �������� ������� LL + L?
                        if ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                                ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L)
                        {
                            // ��������� ���� ��������� ������� LL.
                            (m_puiAnalogueInputDiscreteInputsState[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                                                                          ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L_OFFSET]) = 1;

                            // ��������� ���� ��������� ������� L.
                            (m_puiAnalogueInputDiscreteInputsState[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                                                                          ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                        }
                        // �������� ������� L?
                        if ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                                ANALOGUE_INPUT_SET_POINT_VIOLATION_L)
                        {
                            // ��������� ���� ��������� ������� L.
                            (m_puiAnalogueInputDiscreteInputsState[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                                                                          ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                        }
                        // �������� ������� H?
                        if ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                                ANALOGUE_INPUT_SET_POINT_VIOLATION_H)
                        {
                            // ��������� ���� ��������� ������� H.
                            (m_puiAnalogueInputDiscreteInputsState[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                                                                          ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                        }
                        // �������� ������� HH + H?
                        if ((auiSpiRxBuffer[MUVR_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                                ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H)
                        {
                            // ��������� ���� ��������� ������� HH.
                            (m_puiAnalogueInputDiscreteInputsState[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                                                                          ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H_OFFSET]) = 1;

                            // ��������� ���� ��������� ������� H.
                            (m_puiAnalogueInputDiscreteInputsState[(i * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                                                                          ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                        }
                    }
                }
            }
            return 0;
        }
        break;

    case MUVR_ANSWER_DATA_NOT_READY:
        //std::cout << "CInternalModuleMuvr::DataExchange 7"  << std::endl;
        // ������ �� ������.
        return 0;
        break;

    case MUVR_ANSWER_DATABASE_ERROR:
        //std::cout << "CInternalModuleMuvr::DataExchange 8"  << std::endl;
//        // ������ �� ���������, ����� �����.
//        // ������� ��� ������;
//        SetErrorCode(INTERNAL_MODULE_ERROR_DATA_BASE);
        return 0;
        break;

    default:
        //std::cout << "CInternalModuleMuvr::DataExchange 9"  << std::endl;
        break;
    };


    // ������ ������� �����������?
    if (GetBadAnswerCounter() == 0)
    {
        //std::cout << "CInternalModuleMuvr::DataExchange 10"  << std::endl;
//        // ������ ������� �����������.
//        *(m_puiModuleBadStateBuffer) = BAD_MODULE_NOT_RESPONDED;

        // ������ ������ ������ ������������, ������� ��.
        memset(m_pfAnalogueInputsValue,
               0,
               MUVR_ANALOG_INPUT_QUANTITY * sizeof(float));
        // ��������� ����� ��������������� - ����� ������������.
        memset(m_puiAnalogueInputsBadState,
               1,
               MUVR_ANALOG_INPUT_QUANTITY);
//        // ���������� ������ ������ ������ ������������, ������� ��.
//        memset(m_puiDiscreteInputsState,
//               0,
//               MUVR_ANALOG_INPUT_QUANTITY * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH);
        // ��������� ����� ��������������� ���� ���������� �������� ������.
//        memset(m_puiDiscreteInputsBadState,
//               1,
//               (MUVR_ANALOG_INPUT_QUANTITY * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH));
        memset(m_puiAnalogueInputDiscreteInputsBadState,
               1,
               (MUVR_ANALOG_INPUT_QUANTITY * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH));
        // ���������� ������ ����������� ����������� ������� ������������, ������� ��.
        memset(m_puiAnalogueInputDiscreteInputsState,
               0,
               (MUVR_ANALOG_INPUT_QUANTITY * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH));
        //iBadModuleBuffUpdate();
    }
    else
    {
        //std::cout << "CInternalModuleMuvr::DataExchange 11"  << std::endl;
        // �������� �������� �������� - "���������� ������� ����� � ������� ��� ������".
        SetBadAnswerCounter(GetBadAnswerCounter() - 1);
    }
    return 0;
}

//-------------------------------------------------------------------------------
// ������ �� ������ ������� �������� ����� � ���� ���.
uint8_t CInternalModuleMuvr::ReperPointsAdcRead(void)
{
    //std::cout << "CInternalModuleMuvr::ReperPointsAdcRead 1"  << std::endl;
    unsigned short usData;
    int i;
    uint8_t *pucSource;
    uint8_t *pucDestination;
    uint8_t aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];

//    memset(&aucTempArray[0],
//           0,
//           sizeof(aucTempArray));

    auiSpiTxBuffer[0] = MUVR_GET_REPER_POINTS_ADC_DATA_COMMAND;
    m_pxCommunicationDevice -> Exchange(GetAddress(),
                                        auiSpiTxBuffer,
                                        auiSpiRxBuffer,
                                        SPI_PREAMBLE_LENGTH +
                                        ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH +
                                        TWO_BYTE_CRC_LENGTH,
                                        SPEED_IN_HZ);
    // ������ ��������?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MUVR_GET_REPER_POINTS_ADC_DATA_COMMAND) // if echo answer is ok.
    {
        std::cout << "CInternalModuleMuvr::ReperPointsAdcRead 2"  << std::endl;
        // ������ �� ����������?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH) > 0)
        {
            std::cout << "CInternalModuleMuvr::ReperPointsAdcRead 3"  << std::endl;
            // ������� ��������� �� ������ �������� ����� � ����� ��� �������� �� ������.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET];
//            // ���������� �� ������ ������� �������� ����� � ���� ��� �������� � ������ ���� ������ ������� �
//            // ������(ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_OFFSET) ������������.
//            // � ����������(MAX_MUVR_MODULES_QUANTITY).
//            // � ��������� pucDataBasePointer ���������� ����� ����� � ���� ������, ������� ����������� �����.
//            // ���������� ������ ��� ������ ������� ����������� ����� �����
//            // ���������� ������ � ������� �������� ����� ��� + ���� ���.
//            // �������� ������� ����� ���������� �� ���� �������� ����� ��� + ���� ��� � ���� ������� ����������� �����.
//            // � ������� ����� ����� �������� ����� ��� + ���� ��� � ���� ������.
//            pucDestination = (uint8_t*)(pxModuleContext ->
//                                              xModuleContextStatic.
//                                              pucDataBasePointer +
//                                              ((ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_OFFSET -
//                                                ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET)  * PLC_DATA_BASE_BLOCK_LENGTH));
//

//            pucDestination = m_puiReperPointsAdcBuffer;
            pucDestination =
                (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataLength =
                ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH;
            // ��������� ������ �������� ����� � ����� ��� �� ���������� ������ � ������� ����� �������.
            memcpy(pucDestination,
                   pucSource,
                   ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH);

            {
                std::cout << "CInternalModuleMuvr::ReperPointsAdcRead pucDestination"  << std::endl;
                uint8_t *pucSourceTemp;
                pucSourceTemp = (uint8_t*)pucDestination;//pxCustomerDataContainer -> m_puiDataPointer;
                for(int i=0; i<64; )
                {
                    for(int j=0; j<8; j++)
                    {
                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
                    }
                    cout << endl;
                    i += 8;
                }
            }
            // �� OK.
            //ucModuleError = 0;
            return 1;
        }
        else
        {
            // ������ ������ �������.
            std::cout << "CInternalModuleMuvr::ReperPointsAdcRead 4"  << std::endl;
//            ucModuleError = MUVR_GET_REPER_POINTS_ADC_DATA_COMMUNICATION_ERROR;
            return 0;
        }
        return 0;
    }
    else
    {
        // ������ �� ��������.
        std::cout << "CInternalModuleMuvr::ReperPointsAdcRead 5"  << std::endl;
//        ucModuleError = MUVR_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR;
        return 0;
    }
}

//-------------------------------------------------------------------------------
uint8_t CInternalModuleMuvr::DataBaseRead(void)
{
    //std::cout << "CInternalModuleMuvr::DataBaseRead 1"  << std::endl;
    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    uint8_t *pucSource;
    uint8_t *pucDestination;
    uint8_t aucTempArray[256];

    memset(&aucTempArray[0],
           0,
           sizeof(aucTempArray));
//
//    usleep(10000);
    auiSpiTxBuffer[0] = MUVR_GET_DATA_BASE_COMMAND;
    m_pxCommunicationDevice -> Exchange(GetAddress(),
                                        auiSpiTxBuffer,
                                        auiSpiRxBuffer,
                                        SPI_PREAMBLE_LENGTH +
                                        ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
                                        TWO_BYTE_CRC_LENGTH,
                                        LOW_SPEED_IN_HZ);

//    {
//        std::cout << "CInternalModuleMuvr::DataBaseRead auiSpiRxBuffer"  << std::endl;
//        uint8_t *pucSourceTemp;
//        pucSourceTemp = (uint8_t*)auiSpiRxBuffer;
//        for(int i=0; i<256; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//    }

    // ������ ��������?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MUVR_GET_DATA_BASE_COMMAND) // if echo answer is ok.
    {
        //std::cout << "CInternalModuleMuvr::DataBaseRead 2"  << std::endl;
        // ������ �� ����������?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH) > 0)
        {
            //std::cout << "CInternalModuleMuvr::DataBaseRead 3"  << std::endl;
// � ������� ����������� ����� � � ������ �������, ���� ������ �������� � ������ ��������.
//  � ������� ����������� ����� ������� ���� ������ ���������������� ������(6 ������, �� 20 ����) 120 ����, �����
// ��������� ��������� ������(6 ������, �� 8 ����) 48 ����. 6 ������(120 + 48 = 168) ����.
//  � ������ ������� ������ ���������������� � ��������� ��������� ���� ���������� ������� ��� ������� �����
// (���� ����: 20 ���� - ������ ���������������� � 8 ���� - ��������� ���������). ���� ���� - 28 ����. 6 ������(6 * 28 = 168) ����.
// ����� ������������ �������������� ������� ���� ������, �� ������ � �������.
            // ������� ��������� �� ������ ���������������� ������� �����, �������� �� ������.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET];
            pucDestination = &aucTempArray[0];
// � ������������ � ����������: ������ � ���� ���-17.txt
//����� ������������������� ��������� �87-�89 � ������ �� RAM B119 - B121, �.�.
//������������������� �� ������ �0-�79 + 32 = 112 ������
            pucDestination[119] = pucSource[87];
            pucDestination[120] = pucSource[88];
            pucDestination[121] = pucSource[89];
//            //std::cout << "CInternalModuleMuvr::DataBaseRead pucSource[87] "  << (int)(pucSource[87]) << std::endl;
//            //std::cout << "CInternalModuleMuvr::DataBaseRead pucSource[88] "  << (int)(pucSource[88]) << std::endl;
//            //std::cout << "CInternalModuleMuvr::DataBaseRead pucSource[89] "  << (int)(pucSource[89]) << std::endl;

            for (uint8_t i = 0; i < MUVR_ANALOG_INPUT_QUANTITY; i++)
            {
                // ��������� �� ��������� ����� ����� ����� - ������ ���������������� ������ �����(20 ����).
                memcpy(pucDestination, pucSource, 20);
                // ������� � ������ ���������������� ���������� ����� � ������(+20 ����).
                pucSource += 20;
                // ������� � ���������� ����� ������ ������ ����� � �������(+28 ����).
                pucDestination += 28;
            }
            // ������� ��������� �� ������ ���������� ��������� ������� �����, �������� �� ������.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                                                             ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_TEXT_DESCRIPTOR_OFFSET];
            pucDestination = &aucTempArray[20];
            for (uint8_t i = 0; i < MUVR_ANALOG_INPUT_QUANTITY; i++)
            {
                // ��������� �� ��������� ����� ����� ����� - ��������� ��������� ������ �����(8 ����).
                memcpy(pucDestination, pucSource, 8);
                // ������� � ������ ���������� ��������� ���������� ����� � ������(+8 ����).
                pucSource += 8;
                // ������� � ���������� ����� ������ ������ ����� � �������(+28 ����).
                pucDestination += 28;
            }

            CDataContainerDataBase* pxExecutorDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

//            pucSource = &aucTempArray[0];
//            memcpy(m_pxOperatingDataContainer -> m_puiDataPointer,
//                   pucSource,
//                   ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH);
//            m_pxOperatingDataContainer -> m_uiDataLength = MUVR_ANALOG_INPUT_QUANTITY;

            pucDestination = &aucTempArray[0];

            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
                   pucDestination,
                   ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH);
            pxCustomerDataContainer -> m_uiDataLength = ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH;

//            {
//                std::cout << "CInternalModuleMuvr::DataBaseRead m_puiDataPointer"  << std::endl;
//                uint8_t *pucSourceTemp;
//                pucSourceTemp = (uint8_t*)pucDestination;//pxCustomerDataContainer -> m_puiDataPointer;
//                for(int i=0; i<256; )
//                {
//                    for(int j=0; j<8; j++)
//                    {
//                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                    }
//                    cout << endl;
//                    i += 8;
//                }
//            }

            cout << "CInternalModuleMuvr::DataBaseRead 4" << endl;
            return 1;
        }
        else
        {
            // ������ ������ �������.
            cout << "CInternalModuleMuvr::DataBaseRead 5" << endl;
            return 0;
        }
    }
    else
    {
        // ������ �� ��������.
        cout << "iCInternalModuleMuvr::DataBaseRead 6" << endl;
        return 0;
    }
}

//-------------------------------------------------------------------------------
// ���������� ���� ������ �� RAM ������� � EEPROM ������.
uint8_t CInternalModuleMuvr::DataBaseBlockWrite(void)
{
    unsigned short usData;
    uint8_t *pucSource;
    uint8_t *pucDestination;
    unsigned int nuiBusyTimeCounter;
    uint8_t aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];

// � ������� ����������� ����� � � ������ �������, ���� ������ �������� � ������ ��������.
//  � ������� ����������� ����� ������� ���� ������ ���������������� ������(6 ������, �� 20 ����) 120 ����, �����
// ��������� ��������� ������(6 ������, �� 8 ����) 48 ����. 6 ������(120 + 48 = 168) ����.
//  � ������ ������� ������ ���������������� � ��������� ��������� ���� ���������� ������� ��� ������� �����
// (���� ����: 20 ���� - ������ ���������������� � 8 ���� - ��������� ���������). ���� ���� - 28 ����. 6 ������(6 * 28 = 168) ����.
// ����� ������������ �������������� ������� ���� ������, �� ������� � ������.
    memset(&aucTempArray[0],
           0,
           sizeof(aucTempArray));
    // ������� ��������� �� ���� ���� ������, �������� �� Modbus �� ��������� �����.

    pucSource = (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);

    // ������� ��������� �� ������ ���������������� ������� �����, ��� ������ � ������.
    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET];

// � ������������ � ����������: ������ � ���� ���-17.txt
//����� ������������������� ��������� �87-�89 � ������ �� RAM B119 - B121, �.�.
//������������������� �� ������ �0-�79 + 32 = 112 ������
    pucDestination[87] = pucSource[119];
    pucDestination[88] = pucSource[120];
    pucDestination[89] = pucSource[121];

    for (uint8_t i = 0; i < MUVR_ANALOG_INPUT_QUANTITY; i++)
    {
        // ��������� �� ��������� ����� ����� ����� - ������ ���������������� ������ �����(20 ����).
        memcpy(pucDestination, pucSource, 20);
        // ������� � ������ ���������������� ���������� ����� � ������(+20 ����).
        pucDestination += 20;
        // ������� � ���������� ����� ������ ������ ����� � �������(+28 ����).
        pucSource += 28;
    }
    // ������� ��������� �� ���� ���� ������, �������� �� Modbus �� ��������� �����.

    pucSource = (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);

    // ������� � ������ ���������� ��������� ������� �����(+20 ����).
    pucSource += 20;
    // ������� ��������� �� ������ ���������� ��������� ������� �����, ��� ������ � ������.
    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET + (120)];
    for (uint8_t i = 0; i < MUVR_ANALOG_INPUT_QUANTITY; i++)
    {
        // ��������� �� ��������� ����� ����� ����� - ��������� ��������� ������ �����(8 ����).
        memcpy(pucDestination, pucSource, 8);
        // ������� � ������ ���������� ��������� ���������� ����� � ������(+8 ����).
        pucDestination += 8;
        // ������� � ���������� ����� ������ ������ ����� � �������(+28 ����).
        pucSource += 28;
    }

    // ������� ��������� �� ��������������� ���� ������ �� ������� � ������.
    pucSource = &aucTempArray[SPI_DATA_BYTE_OFFSET];
    usData = usCrcSummTwoByteCalculation(pucSource,
                                         ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH);
    // ������� ��������� �� ����� CRC � ������.
    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET +
                                                        ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH];
    *pucDestination++ = (uint8_t)usData;
    *pucDestination = (uint8_t)(usData >> 8);
    // ��������� ��������������� ���� ������ �� ���������� � ����� TX SPI.
    memcpy(&auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET],
           pucSource,
           ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
           TWO_BYTE_CRC_LENGTH);
//    // �������� ������ � ������.
//    usleep(10000);
    auiSpiTxBuffer[0] = MUVR_SET_DATA_BASE_COMMAND;
    m_pxCommunicationDevice -> Exchange(GetAddress(),
                                        auiSpiTxBuffer,
                                        auiSpiRxBuffer,
                                        SPI_PREAMBLE_LENGTH +
                                        ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
                                        TWO_BYTE_CRC_LENGTH,
                                        LOW_SPEED_IN_HZ);

//    pxModuleContext ->
//    xModuleContextDinamic.
//    usAuxiliaryCounter = 0;

    return 1;
}

////-------------------------------------------------------------------------------
//int CInternalModuleMuvr::DataBaseBlockWriteError(TModuleContext *pxModuleContext)
//{
//    // ���� ������ �� ��������?
//    if ((pxModuleContext ->
//            xModuleContextDinamic.
//            usAuxiliaryCounter) >= 20)
//    {
//        // ������ �� ���������, ����� �����.
//        fbAnalogueInputModuleDataBaseError = 1;
//        // ������� ��� ������;
//        (pxModuleContext ->
//         xModuleContextDinamic.
//         ucErrorCode) = INTERNAL_MODULE_ERROR_DATA_BASE;
//
//        // �������� ��������� ������� ��� �� ����������������?
//        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                    pxModuleContext ->
//                    xModuleContextStatic.
//                    ucModuleContextIndex,
//                    MTVI5_DATA_BASE_ERROR_OFFSET))
//        {
//            // �������������� �������� ��������� �������.
//            CEvents::EventRegistration(
//                pxModuleContext ->
//                xModuleContextStatic.
//                ucModuleContextIndex,
//                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                 CEvents::HANDLED_EVENTS_IS_POPUP |
//                 CEvents::HANDLED_EVENTS_IS_SOUND |
//                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                MTVI5_DATA_BASE_ERROR_OFFSET,
//                "����. �. ���");
//        }
//
//        return -1;
//    }
//    else
//    {
//        // �������� �������� �������� - "���������� ������� ����� � ������� ��� ������".
//        (pxModuleContext ->
//         xModuleContextDinamic.
//         usAuxiliaryCounter)++;
//
//        return 0;
//    }
//}

//-------------------------------------------------------------------------------
// ���������� ���� ������ �� RAM ������� � EEPROM ������.
uint8_t CInternalModuleMuvr::DataBaseBlockWriteCheck(void)
{
    unsigned short usData;
    uint8_t *pucSource;
    uint8_t *pucDestination;
    unsigned int nuiBusyTimeCounter;
    uint8_t aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // ������ ������ � ����������� ����������������.
    auiSpiTxBuffer[0] = MUVR_CHECK_DATA_BASE_WRITE_COMMAND;
    m_pxCommunicationDevice -> Exchange(GetAddress(),
                                        auiSpiTxBuffer,
                                        auiSpiRxBuffer,
                                        SPI_PREAMBLE_LENGTH +
                                        TAIL_ANSWER_LENGTH,
                                        LOW_SPEED_IN_HZ);

    return DATA_EXCHANGE_OK;

    // ���� ������������� ������ ���� ������ � EEPROM ������?
    if((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == DATA_EXCHANGE_OK)
    {
        // ���� ������������� ������ ���� ������ � EEPROM ������.
        cout << "CInternalModuleMuvr::DataBaseBlockWriteCheck OK" << endl;
        return DATA_EXCHANGE_OK;
    }
    // ������ ����� ������� ������ � EEPROM?
    else if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == MUVR_CHECK_DATA_BASE_WRITE_ANSWER_BUSY)
    {
        // ������ ����� ������� ������ � EEPROM.
        cout << "CInternalModuleMuvr::DataBaseBlockWriteCheck BUSY" << endl;
        return MUVR_CHECK_DATA_BASE_WRITE_ANSWER_BUSY;
    }
    // ��������� ������ ��� ������ ���� ������ � EEPROM ������?
    else if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == DATA_EXCHANGE_ERROR)
    {
        // ��������� ������ ��� ������ ���� ������ � EEPROM ������.
        cout << "CInternalModuleMuvr::DataBaseBlockWriteCheck ERROR" << endl;
        return DATA_EXCHANGE_ERROR;
    }
    else
    {
        // ��������� ������ ��� ������ ���� ������ � EEPROM ������.
        cout << "CInternalModuleMuvr::DataBaseBlockWriteCheck ERROR" << endl;
        return MUVR_CHECK_DATA_BASE_WRITE_ANSWER_BUSY;
    }
}

//-------------------------------------------------------------------------------
uint8_t CInternalModuleMuvr::Fsm(void)
{
//    //std::cout << "CInternalModuleMuvr::Fsm 1" << endl;
    uint8_t uiReadyTaskCounter = 0;
    switch (GetFsmState())
    {
    case IDDLE:
//        //std::cout << "CInternalModuleMuvr::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CInternalModuleMuvr::Fsm STOP"  << std::endl;
        break;

    case START:
        //std::cout << "CInternalModuleMuvr::Fsm START"  << std::endl;
        Init();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
//        //std::cout << "CInternalModuleMuvr::Fsm INIT 1"  << std::endl;
    {
        CTaskInterface* pxTask =
            GetResources() ->
            GetTaskPointerByNameFromMap(m_sCommunicationDeviceName);

        if (pxTask != 0)
        {
//                //std::cout << "CInternalModuleMuvr::Fsm INIT 2"  << std::endl;
            if (pxTask -> GetFsmState() >= READY)
            {
//                    //std::cout << "CInternalModuleMuvr::Fsm INIT 3"  << std::endl;
                SetCommunicationDevice((CSpiCommunicationDevice*)pxTask);
                uiReadyTaskCounter += 1;
//                    SetFsmState(READY);
            }
        }
        else
        {
//                //std::cout << "CInternalModuleMuvr::Fsm INIT 4"  << std::endl;
            if (GetTimerPointer() -> IsOverflow())
            {
                //std::cout << "CInternalModuleMuvr::Fsm INIT 5"  << std::endl;
                SetFsmState(STOP);
            }
        }
    }

    if (uiReadyTaskCounter > 0)
    {
        SetFsmState(READY);
    }

    break;

    case READY:
//        //std::cout << "CInternalModuleMuvr::Fsm READY"  << std::endl;
//    {
//
//        if (GetCommandDataLink() != 0)
//        {
//            //std::cout << "CInternalModuleMuvr::Fsm READY 2"  << std::endl;
//            SetOperatingDataLink(GetCommandDataLink());
//            SetFsmState(GetCommandDataLink() ->
//                        GetDataContainerPointer() ->
//                        GetFsmCommandState());
//            GetCommandDataLink() ->
//            GetDataContainerPointer() ->
//            SetFsmCommandState(0);
//            SetCommandDataLink(0);
//        }
//    }
        break;

    case DONE_OK:
//        //std::cout << "CInternalModuleMuvr::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        //std::cout << "CInternalModuleMuvr::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

    case MUVR_GET_MODULE_TYPE:
        //std::cout << "CInternalModuleMuvr::Fsm MUVR_GET_MODULE_TYPE"  << std::endl;
    {
        GetModuleType(GetAddress());
        SetFsmState(DONE_OK);
    }
    break;

    case MUVR_REPER_POINTS_ADC_READ:
        std::cout << "CInternalModuleMuvr::Fsm MUVR_REPER_POINTS_ADC_READ"  << std::endl;
        {
            ReperPointsAdcRead();
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case MUVR_DATA_BASE_READ:
        std::cout << "CInternalModuleMuvr::Fsm MUVR_DATA_BASE_READ"  << std::endl;
        {
            DataBaseRead();
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case MUVR_WRITE_DATA_BASE:
        std::cout << "CInternalModuleMuvr::Fsm MUVR_WRITE_DATA_BASE"  << std::endl;
        {
            DataBaseBlockWrite();
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            // ��������� ����� ������� ������ ������ ��� ������� ���������� ����������������.
            m_xWriteCompleteWaitTimer.Set(500);
            //        SetFsmState(MUVR_WRITE_DATA_BASE_CHECK);
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        }
        SetFsmState(DONE_OK);
        break;

    case MUVR_WRITE_DATA_BASE_CHECK:
        //std::cout << "CInternalModuleMuvr::Fsm MUVR_WRITE_DATA_BASE_CHECK 1"  << std::endl;
    {
        // ����� ������� ������ ������ ��� ������� ���������� ���������������� �����������?
        if (m_xWriteCompleteWaitTimer.IsOverflow())
        {
            std::cout << "CInternalModuleMuvr::Fsm MUVR_WRITE_DATA_BASE_CHECK 5"  << std::endl;
            uint8_t uiFsmState = DataBaseBlockWriteCheck();

            if (uiFsmState == DATA_EXCHANGE_OK)
            {
                std::cout << "CInternalModuleMuvr::Fsm MUVR_WRITE_DATA_BASE_CHECK 2"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
                SetFsmState(DONE_OK);
            }
            else if (uiFsmState == DATA_EXCHANGE_ERROR)
            {
                std::cout << "CInternalModuleMuvr::Fsm MUVR_WRITE_DATA_BASE_CHECK 3"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(DONE_ERROR);
            }
            else
            {
                // ����� �������� ���������� ������� �����������?
                if (GetTimerPointer() -> IsOverflow())
                {
                    std::cout << "CInternalModuleMuvr::Fsm MUVR_WRITE_DATA_BASE_CHECK 4"  << std::endl;
                    ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                    SetFsmState(DONE_ERROR);
                }
            }
        }
    }

//        switch (DataBaseBlockWriteCheck())
//        {
//        case DATA_EXCHANGE_OK:
//            SetFsmState(DONE_OK);
//            break;
//
//        case MUVR_CHECK_DATA_BASE_WRITE_ANSWER_BUSY:
//            break;
//
//        case DATA_EXCHANGE_ERROR:
//            SetFsmState(DONE_ERROR);
//            break;
//
//        default:
//            SetFsmState(DONE_ERROR);
//            break;
//        }

    break;

    case MUVR_DATA_EXCHANGE:
        //std::cout << "CInternalModuleMuvr::Fsm MUVR_DATA_EXCHANGE"  << std::endl;
    {
        DataExchange();
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case MUVR_CALIBRATION_ON_OFF:
        std::cout << "CInternalModuleMuvr::Fsm MUVR_CALIBRATION_ON_OFF"  << std::endl;
        {
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
            // ��������� ����� ������������ �����.
            SetCommonIndex((pxDataContainer -> m_puiDataPointer[0]));
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case MUVR_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE:
        std::cout << "CInternalModuleMuvr::Fsm MUVR_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE"  << std::endl;
        {
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
            // ��������� ������ ��� ����� �����.
            SetCommandControl((pxDataContainer -> m_puiDataPointer[1]));
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------
