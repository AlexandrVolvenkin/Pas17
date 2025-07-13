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
#include "DiscreteSignals.h"
#include "Resources.h"

using namespace std;

//-------------------------------------------------------------------------------
CResources::CResources()
{
    std::cout << "CResources constructor"  << std::endl;
    // получим имя класса.
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
    delete[] m_pxDiscreteOutputControl;

}

//-------------------------------------------------------------------------------
void CResources::ModbusWorkingArraysCreate(uint16_t uiCoilsNumber,
        uint16_t uiDiscreteInputsNumber,
        uint16_t uiHoldingRegistersNumber,
        uint16_t uiInputRegistersNumber)
{
    std::cout << "CResources::ModbusWorkingArraysCreate 1"  << std::endl;
    m_puiCoils = new uint8_t[uiCoilsNumber];
    if (m_puiCoils == nullptr)
    {
        // Обработка ошибки при выделении памяти
        std::cerr << "Failed to allocate memory m_puiCoils!" << std::endl;
    }
    memset(m_puiCoils, 0, uiCoilsNumber);
    m_puiDiscreteInputs = new uint8_t[uiDiscreteInputsNumber];

    if (m_puiDiscreteInputs == nullptr)
    {
        // Обработка ошибки при выделении памяти
        std::cerr << "Failed to allocate memory m_puiDiscreteInputs!" << std::endl;
    }
    memset(m_puiDiscreteInputs, 0, uiDiscreteInputsNumber);

    m_puiHoldingRegisters = new uint16_t[uiHoldingRegistersNumber];

    if (m_puiHoldingRegisters == nullptr)
    {
        // Обработка ошибки при выделении памяти
        std::cerr << "Failed to allocate memory m_puiHoldingRegisters!" << std::endl;
    }
    memset(m_puiHoldingRegisters, 0, (uiHoldingRegistersNumber * sizeof(uint16_t)));

    m_puiInputRegisters = new uint16_t[uiInputRegistersNumber];
    if (m_puiInputRegisters == nullptr)
    {
        // Обработка ошибки при выделении памяти
        std::cerr << "Failed to allocate memory m_puiInputRegisters!" << std::endl;
    }
    memset(m_puiInputRegisters, 0, (uiInputRegistersNumber * sizeof(uint16_t)));


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
    delete[] m_puiSerialAndId;
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

    // Подготовим к отправке текущие адреса объектов системы используемых драйверами устройств.

//    memset(m_puiCoils, 0, COILS_WORK_ARRAY_LENGTH);
//    memset(m_puiDiscreteInputs, 0, DISCRETE_INPUTS_ARRAY_LENGTH);
//    memset(m_puiHoldingRegisters, 0, (HOLDING_REGISTERS_ARRAY_LENGTH * sizeof(uint16_t)));
//    memset(m_puiInputRegisters, 0, (INPUT_REGISTERS_ARRAY_LENGTH * sizeof(uint16_t)));

//    xMemoryAllocationContext.puiRxBuffer = m_auiSpiRxBuffer;
//    xMemoryAllocationContext.puiTxBuffer = m_auiSpiTxBuffer;
//    xMemoryAllocationContext.puiErrorCode = &m_uiErrorCode;

    // Обнулим общий объём выделенной памяти.
    m_uiUsedDiscreteInputsState = 0;
    // Подключим буфер для хранения состояний дискретных входов.
    m_puiDiscreteInputsState =
        &m_puiDiscreteInputs[DISCRETE_INPUTS_STATE_OFFSET + MUVR_DISCRETE_INPUT_QUANTITY];

    // Обнулим общий объём выделенной памяти.
    m_uiUsedDiscreteInputsBadState = 0;
    // Подключим буфер для хранения состояний достоверности дискретных входов.
    m_puiDiscreteInputsBadState =
        &m_puiCoils[DISCRETE_INPUTS_BAD_STATE_OFFSET];

//    // Обнулим общий объём выделенной памяти.
//    m_uiUsedDiscreteOutputState = 0;
//    // Подключим буфер для хранения состояний дискретных выходов.
//    m_puiDiscreteOutputState =
//        &m_puiDiscreteInputs[DISCRETE_OUTPUT_BIT_ARRAY_OFFSET];
//
    // Обнулим общий объём выделенной памяти.
    m_uiUsedDiscreteOutputControl = 0;
    // Подключим буфер для управления дискретными выходами.
    m_pxDiscreteOutputControl =
        new TDiscreteOutputControl[DISCRETE_OUTPUT_NUMBER];
    memset(m_pxDiscreteOutputControl,
           0,
           (sizeof(struct TDiscreteOutputControl) * DISCRETE_OUTPUT_NUMBER));


    // Обнулим общий объём выделенной памяти.
    m_uiUsedAnalogueInputsValue = 0;
    // Подключим буфер для хранения измеренных значений аналоговых входов.
    m_pfAnalogueInputsValue =
        (float*)&m_puiHoldingRegisters[ANALOGUE_INPUTS_VALUE_OFFSET];

    // Обнулим общий объём выделенной памяти.
    m_uiUsedAnalogueInputsState = 0;
    // Подключим буфер для хранения состояний аналоговых входов.
    m_puiAnalogueInputsState =
        new uint8_t[MAX_HANDLED_ANALOGUE_INPUT];

    // Обнулим общий объём выделенной памяти.
    m_uiUsedAnalogueInputsBadState = 0;
    // Подключим буфер для хранения состояний достоверности аналоговых входов.
    m_puiAnalogueInputsBadState =
        &m_puiCoils[ANALOGUE_INPUTS_BAD_STATE_OFFSET];

    // Обнулим общий объём выделенной памяти.
    m_uiUsedAnalogueInputDiscreteInputsState = 0;
    // Подключим буфер для хранения состояний дискретных сигналов порождаемых аналоговыми входами.
    m_puiAnalogueInputDiscreteInputsState =
        &m_puiDiscreteInputs[DISCRETE_INPUTS_STATE_OFFSET + MUVR_DISCRETE_INPUT_QUANTITY];

    // Обнулим общий объём выделенной памяти.
    m_uiUsedAnalogueInputDiscreteInputsBadState = 0;
    // Подключим буфер для хранения флагов недостоверности состояний дискретных сигналов порождаемых аналоговыми входами.
    m_puiAnalogueInputDiscreteInputsBadState =
        &m_puiCoils[ANALOGUE_INPUTS_DISCRETE_SIGNALS_BAD_STATE_OFFSET];

    // Обнулим общий объём выделенной памяти.
    m_uiUsedAnalogueInputsOff = 0;
    // Подключим буфер для хранения отключенных аналоговых входов.
    m_puiAnalogueInputsOff =
        new uint8_t[MAX_HANDLED_ANALOGUE_INPUT];
    // Заполняем массив нулями
    memset(m_puiAnalogueInputsOff, 0, MAX_HANDLED_ANALOGUE_INPUT);

    // Обнулим общий объём выделенной памяти.
    m_uiUsedReperPointsAdcBuffer = 0;
    // Подключим буфер для хранения состояний достоверности аналоговых входов.
    m_puiReperPointsAdcBuffer =
        new uint8_t[ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH];

    // Обнулим общий объём выделенной памяти.
    m_uiUsedAnalogueInputDescriptionWork = 0;
    m_pxAnalogueInputDescriptionWork = new TAnalogueInputDescriptionWork[6];

    // Обнулим общий объём выделенной памяти.
    m_uiUsedDiscreteSignalsDescriptionWork = 0;
    m_pxDiscreteSignalsDescriptionWork = new TDiscreteSignalsDescriptionWork[MAX_HANDLED_DISCRETE_INPUT];


    // Обнулим общий объём выделенной памяти.
    m_uiUsedSerialAndId = 0;
    // Подключим буфер с серийным номером и идентификатором прибора.
    m_puiSerialAndId =
        new uint8_t[SERIAL_AND_ID_DATA_BASE_BLOCK_LENGTH];
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

        // Проверяем, не является ли указатель нулем или равен 0
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
        // ключ найден
//        std::cout << "CResources::GetTaskPointerByNameFromMap 2" << std::endl;
//            std::cout << "CResources::GetTaskPointerByNameFromMap this name" << " "
//                      << ((it -> second) -> GetTaskNamePointer()) << std::endl;
        return it -> second.get(); // Преобразование shared_ptr в raw указатель
    }
    else
    {
//        std::cout << "CResources::GetTaskPointerByNameFromMap 3" << std::endl;
        return nullptr; // Вернем nullptr, если ключ не найден
    }
}

//-------------------------------------------------------------------------------
uint8_t CResources::GetTaskIdByNameFromMap(std::string sTaskName)
{
//    std::cout << "CResources::GetTaskIdByNameFromMap 1"  << std::endl;
    CTaskInterface* pxTask = GetTaskPointerByNameFromMap(sTaskName);

    if (pxTask != nullptr)
    {
        // ключ найден
//        std::cout << "CResources::GetTaskIdByNameFromMap 2"  << std::endl;
        if (m_uiUsedCommonTaskPointersCounter < MAX_TASK_NUMBER)
        {
//            std::cout << "CResources::GetTaskIdByNameFromMap 3"  << std::endl;
            m_ppxCommonTaskPointers[m_uiUsedCommonTaskPointersCounter] = pxTask;
            m_uiUsedCommonTaskPointersCounter++;
            // Вернем индекс на указатель в массиве плюс 1. это будет id задачи в системе.
            // id задач начинаются с единицы. ноль - задачи не существует.
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
        // ключ не найден
        return 0; // Вернем 0, если ключ не найден
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

