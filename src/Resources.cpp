//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

#include "Task.h"
#include "DeviceControl.h"
#include "Link.h"
#include "DataContainer.h"
#include "AnalogueSignals.h"
#include "ConfigurationCreate.h"
#include "InternalModuleMuvr.h"
#include "Resources.h"

//-------------------------------------------------------------------------------
CResources::CResources()
{
    std::cout << "CResources constructor"  << std::endl;
    // ������� ��� ������.
    sprintf(GetTaskNamePointer(),
            "%s",
            typeid(*this).name());

    m_lpxCommonTasksList.clear();
    m_mpxCommonTaskMap.clear();
    m_lpxDataContainerList.clear();
    m_uiUsedCommonTaskPointersCounter = 0;
    m_xDeviceConfigSearch = {0};

    m_ppxCommonTaskPointers = new CTaskInterface*[MAX_TASK_NUMBER];
    SetResources(this);
}

//-------------------------------------------------------------------------------
CResources::~CResources()
{
    delete[] m_ppxCommonTaskPointers;
    delete[] m_puiReperPointsAdcBuffer;
    delete[] m_puiAnalogueInputsState;
    delete[] m_puiAnalogueInputsOff;
    delete[] m_pxAnalogueInputDescriptionWork;
    delete[] m_pxDiscreteSignalsDescriptionWork;
}

//-------------------------------------------------------------------------------
void CResources::ModbusWorkingArraysCreate(uint16_t uiCoilsNumber,
        uint16_t uiDiscreteInputsNumber,
        uint16_t uiHoldingRegistersNumber,
        uint16_t uiInputRegistersNumber)
{
    m_puiCoils = new uint8_t(uiCoilsNumber);
//    memset(m_puiCoils, 0, uiCoilsNumber);
    m_puiDiscreteInputs = new uint8_t(uiDiscreteInputsNumber);
//    memset(m_puiDiscreteInputs, 0, uiDiscreteInputsNumber);
    m_puiHoldingRegisters = new uint16_t(uiHoldingRegistersNumber);
//    memset(m_puiHoldingRegisters, 0, (uiHoldingRegistersNumber * sizeof(uint16_t)));
    m_puiInputRegisters = new uint16_t(uiInputRegistersNumber);
//    memset(m_puiInputRegisters, 0, (uiInputRegistersNumber * sizeof(uint16_t)));

    m_uiCoilsNumber = uiCoilsNumber;
    m_uiDiscreteInputsNumber = uiDiscreteInputsNumber;
    m_uiHoldingRegistersNumber = uiHoldingRegistersNumber;
    m_uiInputRegistersNumber = uiInputRegistersNumber;

//    memset(m_puiCoils, 0, uiCoilsNumber);
//    memset(m_puiDiscreteInputs, 0, uiDiscreteInputsNumber);
//    memset(m_puiHoldingRegisters, 0, (uiHoldingRegistersNumber * sizeof(uint16_t)));
//    memset(m_puiInputRegisters, 0, (uiInputRegistersNumber * sizeof(uint16_t)));
}

//-------------------------------------------------------------------------------
void CResources::ModbusWorkingArraysDestroy(void)
{
    delete[] m_puiCoils;
    delete[] m_puiDiscreteInputs;
    delete[] m_puiHoldingRegisters;
    delete[] m_puiInputRegisters;
}

//-------------------------------------------------------------------------------
void CResources::SetCoils(uint8_t* puiPointer)
{
    m_puiCoils = puiPointer;
}

//-------------------------------------------------------------------------------
uint8_t* CResources::GetCoils(void)
{
    return m_puiCoils;
}

//-------------------------------------------------------------------------------
void CResources::SetCoilsNumber(uint16_t uiData)
{
    m_uiCoilsNumber = uiData;
}

//-------------------------------------------------------------------------------
uint16_t CResources::GetCoilsNumber(void)
{
    return m_uiCoilsNumber;
}


//-------------------------------------------------------------------------------
void CResources::SetDiscreteInputs(uint8_t* puiPointer)
{
    m_puiDiscreteInputs = puiPointer;
}

//-------------------------------------------------------------------------------
uint8_t* CResources::GetDiscreteInputs(void)
{
    return m_puiDiscreteInputs;
}

//-------------------------------------------------------------------------------
void CResources::SetDiscreteInputsNumber(uint16_t uiData)
{
    m_uiDiscreteInputsNumber = uiData;
}

//-------------------------------------------------------------------------------
uint16_t CResources::GetDiscreteInputsNumber(void)
{
    return m_uiDiscreteInputsNumber;
}


//-------------------------------------------------------------------------------
void CResources::SetHoldingRegisters(uint16_t* puiPointer)
{
    m_puiHoldingRegisters = puiPointer;
}

//-------------------------------------------------------------------------------
uint16_t* CResources::GetHoldingRegisters(void)
{
    return m_puiHoldingRegisters;
}

//-------------------------------------------------------------------------------
void CResources::SetHoldingRegistersNumber(uint16_t uiData)
{
    m_uiHoldingRegistersNumber = uiData;
}

//-------------------------------------------------------------------------------
uint16_t CResources::GetHoldingRegistersNumber(void)
{
    return m_uiHoldingRegistersNumber;
}


//-------------------------------------------------------------------------------
void CResources::SetInputRegisters(uint16_t* puiPointer)
{
    m_puiInputRegisters = puiPointer;
}

//-------------------------------------------------------------------------------
uint16_t* CResources::GetInputRegisters(void)
{
    return m_puiInputRegisters;
}

//-------------------------------------------------------------------------------
void CResources::SetInputRegistersNumber(uint16_t uiData)
{
    m_uiInputRegistersNumber = uiData;
}

//-------------------------------------------------------------------------------
uint16_t CResources::GetInputRegistersNumber(void)
{
    return m_uiInputRegistersNumber;
}

//-------------------------------------------------------------------------------
CConfigurationCreate::TConfigDataPackOne* CResources::GetDeviceConfigSearchPointer(void)
{
    return &m_xDeviceConfigSearch;
}

//-------------------------------------------------------------------------------
TDiscreteSygnalTextTitle* CResources::GetDiscreteSygnalTextTitlesWorkPointer(void)
{
    return m_pxDiscreteSygnalTextTitlesWork;
}

//-------------------------------------------------------------------------------
void CResources::Allocate(void)
{
    std::cout << "CResources::Allocate 1"  << std::endl;

    // ���������� � �������� ������� ������ �������� ������� ������������ ���������� ���������.

//    memset(m_puiCoils, 0, COILS_WORK_ARRAY_LENGTH);
//    memset(m_puiDiscreteInputs, 0, DISCRETE_INPUTS_ARRAY_LENGTH);
//    memset(m_puiHoldingRegisters, 0, (HOLDING_REGISTERS_ARRAY_LENGTH * sizeof(uint16_t)));
//    memset(m_puiInputRegisters, 0, (INPUT_REGISTERS_ARRAY_LENGTH * sizeof(uint16_t)));

//    xMemoryAllocationContext.puiRxBuffer = m_auiSpiRxBuffer;
//    xMemoryAllocationContext.puiTxBuffer = m_auiSpiTxBuffer;
//    xMemoryAllocationContext.puiErrorCode = &m_uiErrorCode;

    // ������� ����� ����� ���������� ������.
    m_uiUsedDiscreteInputsState = 0;
    // ��������� ����� ��� �������� ��������� ���������� ������.
    m_puiDiscreteInputsState =
        &m_puiDiscreteInputs[DISCRETE_INPUTS_STATE_OFFSET];

    // ������� ����� ����� ���������� ������.
    m_uiUsedDiscreteInputsBadState = 0;
    // ��������� ����� ��� �������� ��������� ������������� ���������� ������.
    m_puiDiscreteInputsBadState =
        &m_puiCoils[DISCRETE_INPUTS_BAD_STATE_OFFSET];

//    // ������� ����� ����� ���������� ������.
//    m_uiUsedDiscreteOutputState = 0;
//    // ��������� ����� ��� �������� ��������� ���������� �������.
//    m_puiDiscreteOutputState =
//        &m_puiDiscreteInputs[DISCRETE_OUTPUT_BIT_ARRAY_OFFSET];
//
//    // ������� ����� ����� ���������� ������.
//    m_uiUsedDiscreteOutputControl = 0;
//    // ��������� ����� ��� ���������� ����������� ��������.
//    m_puiDiscreteOutputControl =
//        &m_puiDiscreteInputs[DISCRETE_OUTPUT_BIT_ARRAY_OFFSET];

    // ������� ����� ����� ���������� ������.
    m_uiUsedAnalogueInputsValue = 0;
    // ��������� ����� ��� �������� ���������� �������� ���������� ������.
    m_pfAnalogueInputsValue =
        (float*)&m_puiHoldingRegisters[ANALOGUE_INPUTS_VALUE_OFFSET];

    // ������� ����� ����� ���������� ������.
    m_uiUsedAnalogueInputsState = 0;
    // ��������� ����� ��� �������� ��������� ���������� ������.
    m_puiAnalogueInputsState =
        new uint8_t[MAX_HANDLED_ANALOGUE_INPUT];

    // ������� ����� ����� ���������� ������.
    m_uiUsedAnalogueInputsBadState = 0;
    // ��������� ����� ��� �������� ��������� ������������� ���������� ������.
    m_puiAnalogueInputsBadState =
        &m_puiCoils[ANALOGUE_INPUTS_BAD_STATE_OFFSET];

    // ������� ����� ����� ���������� ������.
    m_uiUsedAnalogueInputDiscreteInputsState = 0;
    // ��������� ����� ��� �������� ��������� ���������� �������� ����������� ����������� �������.
    m_puiAnalogueInputDiscreteInputsState =
        &m_puiDiscreteInputs[DISCRETE_INPUTS_STATE_OFFSET + MUVR_DISCRETE_INPUT_QUANTITY];

    // ������� ����� ����� ���������� ������.
    m_uiUsedAnalogueInputDiscreteInputsBadState = 0;
    // ��������� ����� ��� �������� ������ ��������������� ��������� ���������� �������� ����������� ����������� �������.
    m_puiAnalogueInputDiscreteInputsBadState =
        &m_puiCoils[ANALOGUE_INPUTS_DISCRETE_SIGNALS_BAD_STATE_OFFSET];

    // ������� ����� ����� ���������� ������.
    m_uiUsedAnalogueInputsOff = 0;
    // ��������� ����� ��� �������� ����������� ���������� ������.
    m_puiAnalogueInputsOff =
        new uint8_t[MAX_HANDLED_ANALOGUE_INPUT];
    // ��������� ������ ������
    memset(m_puiAnalogueInputsOff, 0, MAX_HANDLED_ANALOGUE_INPUT);

    // ������� ����� ����� ���������� ������.
    m_uiUsedReperPointsAdcBuffer = 0;
    // ��������� ����� ��� �������� ��������� ������������� ���������� ������.
    m_puiReperPointsAdcBuffer =
        new uint8_t[ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH];

    // ������� ����� ����� ���������� ������.
    m_uiUsedAnalogueInputDescriptionWork = 0;
    m_pxAnalogueInputDescriptionWork = new TAnalogueInputDescriptionWork[6];

    // ������� ����� ����� ���������� ������.
    m_uiUsedDiscreteSignalsDescriptionWork = 0;
    m_pxDiscreteSignalsDescriptionWork = new TDiscreteSignalsDescriptionWork[MAX_HANDLED_DISCRETE_INPUT];
}

//-------------------------------------------------------------------------------
void CResources::AddCommonListTask(CTaskInterface* pxTask)
{
    std::cout << "CResources::AddCommonListTask 1"  << std::endl;

//    std::cout << "CResources::AddCommonListTask this name" << " " << (this -> GetTaskNamePointer()) << std::endl;
//    std::cout << "CResources::AddCommonListTask pxTask name" << " " << (pxTask -> GetTaskNamePointer()) << std::endl;

    m_lpxCommonTasksList.push_back(pxTask);
}

//-------------------------------------------------------------------------------
void CResources::AddCurrentlyRunningTasksList(CTaskInterface* pxTask)
{
    std::cout << "CResources::AddCurrentlyRunningTasksList 1"  << std::endl;

//    std::cout << "CResources::AddCurrentlyRunningTasksList this name" << " " << (this -> GetTaskNamePointer()) << std::endl;
//    std::cout << "CResources::AddCurrentlyRunningTasksList pxTask name" << " " << (pxTask -> GetTaskNamePointer()) << std::endl;

    m_lpxCurrentlyRunningTasksList.push_back(pxTask);
}

//-------------------------------------------------------------------------------
CTaskInterface* CResources::GetCommonListTaskPointer(char* pcTaskName)
{
    std::cout << "CResources::GetCommonListTaskPointer 1"  << std::endl;

//    std::cout << "CResources::GetCommonListTaskPointer this name" << " " << (this -> GetTaskNamePointer()) << std::endl;
//    std::cout << "CResources::GetCommonListTaskPointer pcTaskName" << " " << pcTaskName << std::endl;

    //    std::list<std::shared_ptr<CTaskInterface>>::iterator xListIterator;
    std::list<CTaskInterface*>::iterator xListIterator;

    for(xListIterator = m_lpxCommonTasksList.begin();
            xListIterator != m_lpxCommonTasksList.end();
            xListIterator++)
    {
        std::cout << "CResources::GetCommonListTaskPointer xListIterator name" << " " << ((*xListIterator) -> GetTaskNamePointer()) << std::endl;

        if (strstr(((*xListIterator) -> GetTaskNamePointer()), pcTaskName) != 0)
        {
            std::cout << "CResources::GetCommonListTaskPointer 2"  << std::endl;
            return (*xListIterator);
        }
//        else
//        {
//            std::cout << "CResources::GetCommonListTaskPointer 3"  << std::endl;
//            return 0;
//        }
    }

    std::cout << "CResources::GetCommonListTaskPointer 4"  << std::endl;
    return 0;
}

//-------------------------------------------------------------------------------
CTaskInterface* CResources::AddCommonTaskToMap(std::string sTaskName, std::shared_ptr<CTaskInterface> pxTask)
{
    std::cout << "CResources::AddCommonTaskToMap 1"  << std::endl;

//    std::cout << "CResources::AddCommonTaskToMap this name" << " " << (this -> GetTaskNamePointer()) << std::endl;
//    std::cout << "CResources::AddCommonTaskToMap sTaskName" << " " << (sTaskName) << std::endl;
    pxTask -> SetTaskName(sTaskName);
    m_mpxCommonTaskMap[sTaskName] = pxTask;

    return pxTask.get();
}

//-------------------------------------------------------------------------------
CTaskInterface* CResources::CreateTaskAndLinkToOthers(std::shared_ptr<CTaskInterface> pxTask,
        std::string sTaskName,
        std::string sExecutorTaskName,
        std::string sNextTaskDoneOkName,
        std::string sNextTaskDoneErrorName)
{
    std::cout << "CResources::AddCommonTaskToMap 1"  << std::endl;

//    std::cout << "CResources::AddCommonTaskToMap this name" << " " << (this -> GetTaskNamePointer()) << std::endl;
//    std::cout << "CResources::AddCommonTaskToMap sTaskName" << " " << (sTaskName) << std::endl;
    pxTask -> SetTaskName(sTaskName);
    pxTask -> SetExecutorTaskName(sExecutorTaskName);
    pxTask -> SetNextTaskDoneOkName(sNextTaskDoneOkName);
    pxTask -> SetNextTaskDoneErrorName(sNextTaskDoneErrorName);
    m_mpxCommonTaskMap[sTaskName] = pxTask;

    return pxTask.get();
}

//-------------------------------------------------------------------------------
bool CResources::CheckCommonTaskMap(void)
{
    std::cout << "CResources::CheckCommonTaskMap 1"  << std::endl;

    for (const auto& pair : m_mpxCommonTaskMap)
    {
        std::string key = pair.first;
        CTaskInterface* taskPtr = pair.second.get();

        // ���������, �� �������� �� ��������� ����� ��� ����� 0
        if (taskPtr == 0)
        {
            std::cout << "Task '" << key << "' has a NULL pointer or is set to zero." << std::endl;
            return false;
        }
        else
        {
            std::cout << "Task '" << key << "' is valid and has a pointer to task." << std::endl;
            std::cout << "CResources::CheckCommonTaskMap GetObjectLength() "  << (int)(taskPtr -> GetObjectLength()) << std::endl;
        }
    }

    return true;
}

//-------------------------------------------------------------------------------
CTaskInterface* CResources::GetTaskPointerByNameFromMap(std::string sTaskName)
{
//    std::cout << "CResources::GetTaskPointerByNameFromMap 1"  << std::endl;
    auto it = m_mpxCommonTaskMap.find(sTaskName);

    if (it != m_mpxCommonTaskMap.end())
    {
        // ���� ������
//        std::cout << "CResources::GetTaskPointerByNameFromMap 2" << std::endl;
//            std::cout << "CResources::GetTaskPointerByNameFromMap this name" << " "
//                      << ((it -> second) -> GetTaskNamePointer()) << std::endl;
        return it -> second.get(); // �������������� shared_ptr � raw ���������
    }
    else
    {
//        std::cout << "CResources::GetTaskPointerByNameFromMap 3" << std::endl;
        return nullptr; // ������ nullptr, ���� ���� �� ������
    }
}

//-------------------------------------------------------------------------------
uint8_t CResources::GetTaskIdByNameFromMap(std::string sTaskName)
{
    std::cout << "CResources::GetTaskIdByNameFromMap 1"  << std::endl;
    CTaskInterface* pxTask = GetTaskPointerByNameFromMap(sTaskName);

    if (pxTask != nullptr)
    {
        // ���� ������
//        std::cout << "CResources::GetTaskIdByNameFromMap 2"  << std::endl;
        if (m_uiUsedCommonTaskPointersCounter < MAX_TASK_NUMBER)
        {
            std::cout << "CResources::GetTaskIdByNameFromMap 3"  << std::endl;
            m_ppxCommonTaskPointers[m_uiUsedCommonTaskPointersCounter] = pxTask;
            m_uiUsedCommonTaskPointersCounter++;
            // ������ ������ �� ��������� � ������� ���� 1. ��� ����� id ������ � �������.
            // id ����� ���������� � �������. ���� - ������ �� ����������.
            return m_uiUsedCommonTaskPointersCounter;
        }
        else
        {
            std::cout << "CResources::GetTaskIdByNameFromMap 4"  << std::endl;
            return 0;
        }
    }
    else
    {
        std::cout << "CResources::GetTaskIdByNameFromMap 5"  << std::endl;
        // ���� �� ������
        return 0; // ������ 0, ���� ���� �� ������
    }
}

//-------------------------------------------------------------------------------
CTaskInterface* CResources::GetTaskPointerById(uint8_t uiTaskId)
{
//    std::cout << "CResources::GetTaskPointerById 1"  << std::endl;

    if (uiTaskId > 0)
    {
//        std::cout << "CResources::GetTaskPointerById 2"  << std::endl;
        return m_ppxCommonTaskPointers[(uiTaskId - 1)];
    }
    else
    {
        std::cout << "CResources::GetTaskPointerById 3"  << std::endl;
        return nullptr;
    }
}

//-------------------------------------------------------------------------------
uint8_t* CResources::CreateObjectBySize(size_t uiLength)
{
    std::cout << "CResources::CreateObjectBySize 1"  << std::endl;

    return new uint8_t[uiLength];
}

//-------------------------------------------------------------------------------
CDataContainerInterface* CResources::AddDataContainer(std::shared_ptr<CDataContainerInterface> pxDataContainer)
{
    std::cout << "CResources::AddDataContainer 1"  << std::endl;

    m_lpxDataContainerList.push_back(pxDataContainer);
    return pxDataContainer.get();
}

//-------------------------------------------------------------------------------
void CResources::SendRequest(void)
{
    std::cout << "CResources::SendRequest 1"  << std::endl;

}

//-------------------------------------------------------------------------------
void CResources::ReceiveAnswer(void)
{
    std::cout << "CResources::ReceiveAnswer 1"  << std::endl;

}

//-------------------------------------------------------------------------------

