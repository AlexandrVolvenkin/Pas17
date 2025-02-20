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
#include "Resources.h"

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

    m_ppxCommonTaskPointers = new CTaskInterface*[256];
    SetResources(this);
}

//-------------------------------------------------------------------------------
CResources::~CResources()
{
    delete[] m_ppxCommonTaskPointers;
}

//-------------------------------------------------------------------------------
void CResources::ModbusWorkingArraysCreate(uint16_t uiCoilsNumber,
        uint16_t uiDiscreteInputsNumber,
        uint16_t uiHoldingRegistersNumber,
        uint16_t uiInputRegistersNumber)
{
    m_puiCoils = new uint8_t(uiCoilsNumber);
    m_puiDiscreteInputs = new uint8_t(uiDiscreteInputsNumber);
    m_puiHoldingRegisters = new uint16_t(uiHoldingRegistersNumber);
    m_puiInputRegisters = new uint16_t(uiInputRegistersNumber);

    m_uiCoilsNumber = uiCoilsNumber;
    m_uiDiscreteInputsNumber = uiDiscreteInputsNumber;
    m_uiHoldingRegistersNumber = uiHoldingRegistersNumber;
    m_uiInputRegistersNumber = uiInputRegistersNumber;
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

//-----------------------------------------------------------------------------------------------------
void CResources::Allocate(void)
{
    std::cout << "CResources::Allocate 1"  << std::endl;

    // Подготовим к отправке текущие адреса объектов системы используемых драйверами устройств.

//    xMemoryAllocationContext.puiRxBuffer = m_auiSpiRxBuffer;
//    xMemoryAllocationContext.puiTxBuffer = m_auiSpiTxBuffer;
//    xMemoryAllocationContext.puiErrorCode = &m_uiErrorCode;

    // Обнулим общий объём выделенной памяти.
    m_uiUsedDiscreteInputsState = 0;
    // Подключим буфер для хранения состояний дискретных входов.
    m_puiDiscreteInputsState =
        &m_puiDiscreteInputs[DISCRETE_INPUTS_STATE_OFFSET];

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
//    // Обнулим общий объём выделенной памяти.
//    m_uiUsedDiscreteOutputControl = 0;
//    // Подключим буфер для управления дискретными выходами.
//    m_puiDiscreteOutputControl =
//        &m_puiDiscreteInputs[DISCRETE_OUTPUT_BIT_ARRAY_OFFSET];

    // Обнулим общий объём выделенной памяти.
    m_uiUsedAnalogueInputsValue = 0;
    // Подключим буфер для хранения измеренных значений аналоговых входов.
    m_pfAnalogueInputsValue =
        (float*)&m_puiHoldingRegisters[ANALOGUE_INPUTS_VALUE_OFFSET];

//    // Обнулим общий объём выделенной памяти.
//    m_uiUsedAnalogueInputsState = 0;
//    // Подключим буфер для хранения состояний аналоговых входов.
//    m_puiAnalogueInputsState =
//        &m_puiCoils[ANALOGUE_INPUTS_BAD_STATE_OFFSET];

    // Обнулим общий объём выделенной памяти.
    m_uiUsedAnalogueInputsBadState = 0;
    // Подключим буфер для хранения состояний достоверности аналоговых входов.
    m_puiAnalogueInputsBadState =
        &m_puiCoils[ANALOGUE_INPUTS_BAD_STATE_OFFSET];

    // Обнулим общий объём выделенной памяти.
    m_uiUsedAnalogueInputsOff = 0;
    // Подключим буфер для хранения отключенных аналоговых входов.
    m_puiAnalogueInputsOff =
        &m_puiCoils[ANALOGUE_INPUTS_BAD_STATE_OFFSET];

    // Обнулим общий объём выделенной памяти.
    m_uiUsedAnalogueInputDescriptionWork = 0;

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
        std::cout << "CResources::GetTaskIdByNameFromMap 2"  << std::endl;
        // ключ найден
        m_ppxCommonTaskPointers[m_uiUsedCommonTaskPointersCounter] = pxTask;
        m_uiUsedCommonTaskPointersCounter++;
        // Вернем индекс на указатель в массиве плюс 1. это это будет id задачи в системе.
        // id задач начинаются с единицы. ноль - задачи не существует.
        return m_uiUsedCommonTaskPointersCounter;
    }
    else
    {
        std::cout << "CResources::GetTaskIdByNameFromMap 3"  << std::endl;
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
        std::cout << "CResources::GetTaskPointerById 2"  << std::endl;
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

CTaskInterface** m_ppxCommonTaskPointers;
uint8_t m_uiUsedCommonTaskPointersCounter;
