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

    bool bIsFileExist = false;

    // �������� ������� ����
    time_t now = time(0);
    struct tm tstruct = *gmtime(&now);

    // ����������� ���� � �����
    char dateStr[80];
    strftime(dateStr, sizeof(dateStr), "%d-%m-%Y", &tstruct);

    char timeStr[80];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &tstruct);

    // ������� ��� ����� � �����
    std::string filename = "AnalogueMeasure_" + std::string(dateStr) + ".csv";

    // ���������, ���������� �� ����
    if (std::ifstream(filename))
    {
//        std::cout << "���� ��� ����������: " << filename << std::endl;
        bIsFileExist = true;
    }

    // ��������� ���� ��� ���������� ������
    std::ofstream file(filename, std::ios::app);

    if (!file.is_open())
    {
        std::cerr << "�� ������� ������� ����!" << std::endl;
        return;
    }

//    std::cout << "CreateArchiveEntry filename "  << dateStr << std::endl;

    if (!bIsFileExist)
    {
        // ���������� ���������
        file << "����;�����;AIn1;AIn2;AIn3;AIn4" << std::endl;
    }

    // ���������� ������ � ����
    file <<
         dateStr << ";" <<
         timeStr << ";" <<
         (float)(m_pfAnalogueInputsValue[0]) << ";" <<
         (float)(m_pfAnalogueInputsValue[1]) << ";" <<
         (float)(m_pfAnalogueInputsValue[2]) << ";" <<
         (float)(m_pfAnalogueInputsValue[3]) <<
         std::endl;

    // ��������� ����
    file.close();

//    std::cout << "������ ������� �������� � data.csv" << std::endl;

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

