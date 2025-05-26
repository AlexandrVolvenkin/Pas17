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
#include <fstream>
#include <ctime>

#include "Task.h"
#include "Resources.h"
#include "Platform.h"
#include "DataStore.h"
#include "DataContainer.h"
#include "InternalModule.h"
#include "ConfigurationCreate.h"
#include "AnalogueSignalsArchiveCreate.h"

using namespace std;

//-------------------------------------------------------------------------------
CAnalogueSignalsArchiveCreate::CAnalogueSignalsArchiveCreate()
{
    std::cout << "CAnalogueSignalsArchiveCreate constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[256];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CAnalogueSignalsArchiveCreate::~CAnalogueSignalsArchiveCreate()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
void CAnalogueSignalsArchiveCreate::SetInternalModuleName(std::string sName)
{
    m_sInternalModuleName = sName;
}

//-------------------------------------------------------------------------------
uint8_t CAnalogueSignalsArchiveCreate::Init(void)
{
    std::cout << "CAnalogueSignalsArchiveCreate Init"  << std::endl;
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
    SetCustomerDataContainer(GetExecutorDataContainerPointer());

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

    Allocate();
}

//-------------------------------------------------------------------------------
void CAnalogueSignalsArchiveCreate::Allocate(void)
{
    std::cout << "CAnalogueSignalsArchiveCreate::Allocate 1"  << std::endl;

////    m_uiAddress = xMemoryAllocationContext.uiAddress;
////    m_puiRxBuffer = xMemoryAllocationContext.puiRxBuffer;
////    m_puiTxBuffer = xMemoryAllocationContext.puiTxBuffer;
////    m_puiErrorCode = xMemoryAllocationContext.puiErrorCode;
//
//    // ������� ��������� �� ����� � ������� ���������� ������ ��� �������� ������.
//    m_puiDiscreteInputsState =
//        &(GetResources() ->
//          m_puiDiscreteInputsState[GetResources() ->
//                                                  m_uiUsedDiscreteInputsState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedDiscreteInputsState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ������������� ���������� ������ ��� �������� ������.
//    m_puiDiscreteInputsBadState =
//        &(GetResources() ->
//          m_puiDiscreteInputsBadState[GetResources() ->
//                                                     m_uiUsedDiscreteInputsBadState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedDiscreteInputsBadState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;


    // ������� ��������� �� ����� � ������� ���������� ������ ��� �������� ������.
    m_pfAnalogueInputsValue =
        &(GetResources() ->
          m_pfAnalogueInputsValue[0]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputsValue +=
//        MUVR_ANALOG_INPUT_QUANTITY;


//    // ������� ��������� �� ����� � ������� ��������� ���������� ������ ��� �������� ������.
//    m_puiAnalogueInputsState =
//        &(GetResources() ->
//          m_puiAnalogueInputsState[GetResources() ->
//                                                  m_uiUsedAnalogueInputsState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputsBadState +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ������������� ���������� ������ ��� �������� ������.
//    m_puiAnalogueInputsBadState =
//        &(GetResources() ->
//          m_puiAnalogueInputsBadState[GetResources() ->
//                                                     m_uiUsedAnalogueInputsBadState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputsBadState +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ��������� ���������� �������� ����������� ����������� �������.
//    m_puiAnalogueInputDiscreteInputsState =
//        &(GetResources() ->
//          m_puiAnalogueInputDiscreteInputsState[GetResources() ->
//                                 m_uiUsedAnalogueInputDiscreteInputsState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputDiscreteInputsState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ������ ��������������� ��������� ���������� �������� ����������� ����������� �������.
//    m_puiAnalogueInputDiscreteInputsBadState =
//        &(GetResources() ->
//          m_puiAnalogueInputDiscreteInputsBadState[GetResources() ->
//                                 m_uiUsedAnalogueInputDiscreteInputsBadState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputDiscreteInputsBadState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ���������� ���������� ������ ��� �������� ������.
//    m_puiAnalogueInputsOff =
//        &(GetResources() ->
//          m_puiAnalogueInputsOff[GetResources() ->
//                                                m_uiUsedAnalogueInputsOff]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputsOff +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ������������� �� ���������� ������ ���� ������
//    // ���������� ��������.
//    m_pxAnalogueInputDescriptionWork =
//        &(GetResources() ->
//          m_pxAnalogueInputDescriptionWork[GetResources() ->
//                                                          m_uiUsedAnalogueInputDescriptionWork]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputDescriptionWork +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ���������� ������ ��� �������� ������.
//    m_puiReperPointsAdcBuffer =
//        &(GetResources() ->
//          m_puiReperPointsAdcBuffer[GetResources() ->
//                                                   m_uiUsedReperPointsAdcBuffer]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedReperPointsAdcBuffer +=
//        ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH;


//    // ������� ��������� �� ����� � ������� ������� ���������� �������� ��� �������� ������.
//    m_pxDiscreteSignalsDescriptionWork =
//        &(GetResources() ->
//          m_pxDiscreteSignalsDescriptionWork[GetResources() ->
//                                 m_uiUsedDiscreteSignalsDescriptionWork]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedDiscreteSignalsDescriptionWork +=
//        ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH;
//
//    m_uiBadAnswerCounter = 0;
}

//-------------------------------------------------------------------------------
void CAnalogueSignalsArchiveCreate::CreateArchiveEntry(void)
{
//    std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 1"  << std::endl;

    const std::string inputFile = "input.bin";
    const std::string mtdDev = "/dev/mtd0";

    struct Data
    {
        time_t currentTime; // ���������� ��� �������� �������� �������
        float fAin1;       // ���������� ������� �����
        float fAin2;       // ���������� ������� �����
        float fAin3;       // ���������� �������� �����
        float fAin4;       // ���������� ���������� �����
    };

    Data data;
    Data readData;

    // �������� ������� �����
    time_t now = time(nullptr);
    // �������� ������� ����
    struct tm tstructCurrent = *gmtime(&now);

    // ��������� ���������� ��������� �������
    data.currentTime = now;
    data.fAin1 = (float)(m_pfAnalogueInputsValue[0]); // ������ �������� ��� fAin1
    data.fAin2 = (float)(m_pfAnalogueInputsValue[1]); // ������ �������� ��� fAin2
    data.fAin3 = (float)(m_pfAnalogueInputsValue[2]); // ������ �������� ��� fAin3
    data.fAin4 = (float)(m_pfAnalogueInputsValue[3]); // ������ �������� ��� fAin4


    // ���� ������� ���� ���������� �� ����������,
    // ������, ���� ����������� ������ ����
    if (tstructCurrent.tm_min != m_iLastHour)
//    if (tstructCurrent.tm_hour != m_iLastHour)
    {
        std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 2"  << std::endl;
        // ��������� �������� ��� ��������� ��������
        m_iLastHour = tstructCurrent.tm_min;
//        m_iLastHour = tstructCurrent.tm_hour;

        // ��������� ���� ��� ������
        {
            std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 3"  << std::endl;
//            // �������� ����� ����� �����
            size_t fileSize = m_uiCurrentOffset;

            std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 4 fileSize "  << (float)fileSize << std::endl;
            // ���� ������?
            if (fileSize == 0)
            {
                std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 5"  << std::endl;
                std::cerr << "���� ������." << std::endl;

                // ���������� ������ � ���� /dev/mtd0.
                {
                    std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 6"  << std::endl;
                    std::ofstream output(mtdDev, std::ios::binary | std::ios::out);
                    if (!output.is_open())
                    {
                        std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 7"  << std::endl;
                        std::cerr << "Failed to open /dev/mtd0" << std::endl;
                    }
                    output.seekp(m_uiCurrentOffset, std::ios::beg);
                    output.write(reinterpret_cast<const char*>(&data), sizeof(Data));
                    output.close(); // ��������� ���� ����� ������
                    m_uiCurrentOffset += sizeof(Data);
                }
                return;
            }

            // ��������� ���������� �������� Data � �����
            size_t numDataObjects = (fileSize / sizeof(Data));
            std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 7 numDataObjects "  << (float)numDataObjects << std::endl;

            std::ifstream input(mtdDev, std::ios::binary);
            if (!input.is_open())
            {
                std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 4"  << std::endl;
                std::cerr << "�� ������� ������� ����: " << mtdDev << std::endl;
            }

            // ��������� �������� ���� ��� ���������� ������
            std::ofstream output(inputFile, std::ios::app);

            if (!output.is_open())
            {
                std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 8"  << std::endl;
                std::cerr << "�� ������� ������� ����: " << inputFile << std::endl;
            }

            // ��������� � ����������� ������
            for (size_t i = 0; i < numDataObjects; ++i)
            {

                input.seekg((i * sizeof(Data)), std::ios::beg);
                input.read(reinterpret_cast<char*>(&readData), sizeof(Data));

                if (!input.gcount())
                {
                    std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 9"  << std::endl;
                    std::cerr << "������ ��� ������ �����: �� ��� ����� ���������!" << std::endl;
                    break;
                }

                // �������� ������� ����
                struct tm tstructRead = *gmtime(&readData.currentTime);

                // ����������� ���� � �����
                char dateStr[80];
                strftime(dateStr, sizeof(dateStr), "%d-%m-%Y", &tstructRead);

                char timeStr[80];
                strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &tstructRead);

//    std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 15"  << std::endl;

                // ���������� ������ � ����
                output <<
                       dateStr << ";" <<
                       timeStr << ";" <<
                       readData.fAin1 << ";" <<
                       readData.fAin2 << ";" <<
                       readData.fAin3 << ";" <<
                       readData.fAin4 <<
                       std::endl;
            }

            // ��������� ����
            input.close();
            output.close();
        }

        // ������ ����� ��� �������� ������ � fram �������.
        m_uiCurrentOffset = 0;
        // ���������� ������ � ���� /dev/mtd0.
        {
            std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 10"  << std::endl;
            // �������� ����� � ������ ��������� ������
            std::ofstream output(mtdDev, std::ios::binary | std::ios::out);
            if (!output.is_open())
            {
                std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 11"  << std::endl;
                std::cerr << "Failed to open /dev/mtd0" << std::endl;
                return;
            }
            output.seekp(m_uiCurrentOffset, std::ios::beg);
            output.write(reinterpret_cast<const char*>(&data), sizeof(Data));
            output.close(); // ��������� ���� ����� ������
            m_uiCurrentOffset += sizeof(Data);
        }

        // ���� ������� ��� ���������� �� ����������,
        // ������, ���� ����������� ����� �����
        if (tstructCurrent.tm_mday != m_iLastDay)
        {
            std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 12"  << std::endl;
            // ��������� �������� ��� ��������� ��������
            m_iLastDay = tstructCurrent.tm_mday;
        }
    }
    else
    {
        // ���������� ������ � ���� /dev/mtd0
        {
//            std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 13"  << std::endl;
            // �������� ����� � ������ ��������� ������
            std::ofstream output(mtdDev, std::ios::binary | std::ios::out);
            if (!output.is_open())
            {
                std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 14"  << std::endl;
                std::cerr << "Failed to open /dev/mtd0" << std::endl;
                return;
            }
            output.seekp(m_uiCurrentOffset, std::ios::beg);
            output.write(reinterpret_cast<const char*>(&data), sizeof(Data));
            output.close(); // ��������� ���� ����� ������
            m_uiCurrentOffset += sizeof(Data);
        }
    }











//    // ���������� ������ � ���� /dev/mtd0
//    {
//////    std::ofstream output(mtdDev, std::ios::binary | std::ios_base::app);
//        std::ofstream output(mtdDev, std::ios::binary | std::ios::trunc);
//        if (!output.is_open())
//        {
//            std::cerr << "Failed to open /dev/mtd0" << std::endl;
//            return;
//        }
//        output.write(reinterpret_cast<const char*>(&data), sizeof(Data));
//        output.close(); // ��������� ���� ����� ������
//    }
//
//    // ������ ������ �� /dev/mtd0 � ���������� �� � input.bin
//    {
//        std::ifstream input(mtdDev, std::ios::binary);
//        if (!input.is_open())
//        {
//            std::cerr << "Failed to open /dev/mtd0" << std::endl;
//            return;
//        }
//        input.read(reinterpret_cast<char*>(&readData), sizeof(Data));
//        input.close(); // ��������� ���� ����� ������
//    }
//
//    std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry /dev/mtd0"  << std::endl;
//    // ������� ������ �� /dev/mtd0 �� �����
//    std::cout << "Read Time: " << asctime(localtime(&readData.currentTime)) << std::endl;
//    std::cout << "fAin1: " << readData.fAin1 << std::endl;
//    std::cout << "fAin2: " << readData.fAin2 << std::endl;
//    std::cout << "fAin3: " << readData.fAin3 << std::endl;
//    std::cout << "fAin4: " << readData.fAin4 << std::endl;
//
//
//    // ���������� ������ � ���� input.bin
//    {
//        std::ofstream output(inputFile, std::ios::binary | std::ios::trunc);
//        if (!output.is_open())
//        {
//            std::cerr << "Failed to open input.bin" << std::endl;
//            return;
//        }
//        output.write(reinterpret_cast<const char*>(&readData), sizeof(Data));
//        output.close(); // ��������� ���� ����� ������
//    }
//
//    // ������ ������ �� input.bin
////    Data readData;
//    {
//        std::ifstream input(inputFile, std::ios::binary);
//        if (!input.is_open())
//        {
//            std::cerr << "Failed to open input.bin" << std::endl;
//            return;
//        }
//        input.read(reinterpret_cast<char*>(&readData), sizeof(Data));
//        input.close(); // ��������� ���� ����� ������
//    }
//
//
//    std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry input.bin"  << std::endl;
//    // ������� ������ �� input.bin �� �����
//    std::cout << "Read Time: " << asctime(localtime(&readData.currentTime)) << std::endl;
//    std::cout << "fAin1: " << readData.fAin1 << std::endl;
//    std::cout << "fAin2: " << readData.fAin2 << std::endl;
//    std::cout << "fAin3: " << readData.fAin3 << std::endl;
//    std::cout << "fAin4: " << readData.fAin4 << std::endl;















//    bool bIsFileExist = false;
//
////    // �������� ������� ����
////    time_t now = time(0);
////    struct tm tstruct = *gmtime(&now);
////    struct tm tstruct = *gmtime(&readData.currentTime);
//
////    // ����������� ���� � �����
////    char dateStr[80];
////    strftime(dateStr, sizeof(dateStr), "%d-%m-%Y", &tstruct);
////
////    char timeStr[80];
////    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &tstruct);
//
//    // ������� ��� ����� � �����
//    std::string filename = "AnalogueMeasure_" + std::string(dateStr) + ".csv";
//
//    // ���������, ���������� �� ����
//    if (std::ifstream(filename))
//    {
////        std::cout << "���� ��� ����������: " << filename << std::endl;
//        bIsFileExist = true;
//    }
//
//    // ��������� ���� ��� ���������� ������
//    std::ofstream output(filename, std::ios::app);
//
//    if (!output.is_open())
//    {
//        std::cerr << "�� ������� ������� ����!" << std::endl;
//        return;
//    }
//
////    std::cout << "CreateArchiveEntry filename "  << dateStr << std::endl;
//
//    if (!bIsFileExist)
//    {
//        // ���������� ���������
////        output << "����;�����;AIn1;AIn2;AIn3;AIn4" << std::endl;
////        output << "   ����   " << ";" << "   �����   " << ";" << "   AIn1   " << ";" << "   AIn2   " << ";" << "   AIn3   " << ";" << "   AIn4   " << std::endl;
//        output << "����;�����;AIn1;AIn2;AIn3;AIn4" << std::endl;
//    }
//
////    // ���������� ������ � ����
////    output <<
////         dateStr << ";" <<
////         timeStr << ";" <<
////         (float)(m_pfAnalogueInputsValue[0]) << ";" <<
////         (float)(m_pfAnalogueInputsValue[1]) << ";" <<
////         (float)(m_pfAnalogueInputsValue[2]) << ";" <<
////         (float)(m_pfAnalogueInputsValue[3]) <<
////         std::endl;
//
//    // ���������� ������ � ����
//    output <<
//         dateStr << ";" <<
//         timeStr << ";" <<
//         readData.fAin1 << ";" <<
//         readData.fAin2 << ";" <<
//         readData.fAin3 << ";" <<
//         readData.fAin4 <<
//         std::endl;
//
//    // ��������� ����
//    output.close();
//
////    std::cout << "������ ������� �������� � data.csv" << std::endl;

}

//-------------------------------------------------------------------------------
uint8_t CAnalogueSignalsArchiveCreate::Fsm(void)
{
//    std::cout << "CAnalogueSignalsArchiveCreate::Fsm 1" << endl;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CAnalogueSignalsArchiveCreate::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CAnalogueSignalsArchiveCreate::Fsm START"  << std::endl;
        Init();
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CAnalogueSignalsArchiveCreate::Fsm INIT 1"  << std::endl;
        m_uiInternalModuleId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleName);
        SetFsmState(READY);

        break;

    case READY:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm READY"  << std::endl;
        break;

    case DONE_OK:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START:
        std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else
        {
            std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 3"  << std::endl;
            // ����� �������� ���������� ������� �����������?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
        std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
            std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // ����� �������� ���������� ������� �����������?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
        std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
        }

        break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(GetFsmNextStateDoneWaitingDoneError());
        }
        else
        {
            // ����� �������� ���������� ������� �����������?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateDoneWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case ANALOGUE_SIGNALS_ARCHIVE_CREATE_START:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm ANALOGUE_SIGNALS_ARCHIVE_CREATE_START"  << std::endl;
    {
        CreateArchiveEntry();
        SetFsmState(ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
    }
    break;

    case ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    default:
        break;
    }
}

//-------------------------------------------------------------------------------

