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

    SetResources(this);
//    m_pxResources -> AddCommonListTask(this);

//    ModbusWorkingArraysCreate(COILS_WORK_ARRAY_LENGTH,
//                              DISCRETE_INPUTS_ARRAY_LENGTH,
//                              HOLDING_REGISTERS_ARRAY_LENGTH,
//                              INPUT_REGISTERS_ARRAY_LENGTH);
}

//-------------------------------------------------------------------------------
CResources::~CResources()
{
//    // Освобождаем память
//    for (auto& pair : m_mpxCommonTaskMap)
//    {
//        delete pair.second; // Удаляем указатели на объекты
//    }
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

////-------------------------------------------------------------------------------
//// Геттер для получения списка shared_ptr<CDataContainerInterface>
//std::shared_ptr<std::list<std::shared_ptr<CDataContainerInterface>>> CResources::GetDataContainerList() const
//{
//    return m_lpxDataContainerList;
//}
//
//
////-------------------------------------------------------------------------------
//// Сеттер для установки списка shared_ptr<CDataContainerInterface>
//void CResources::SetDataContainerList(std::shared_ptr<std::list<std::shared_ptr<CDataContainerInterface>>> pDataContainerList)
//{
//    m_lpxDataContainerList = pDataContainerList;
//}

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

//    for (auto ptr : m_lpxCommonTasksList)
//    {
//        delete ptr;
//    }

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

////-------------------------------------------------------------------------------
//void CResources::AddCommonTaskToMap(std::string sTaskName, CTaskInterface* pxTask)
//{
//    std::cout << "CResources::AddCommonTaskToMap 1"  << std::endl;
//
////    std::cout << "CResources::AddCommonTaskToMap this name" << " " << (this -> GetTaskNamePointer()) << std::endl;
////    std::cout << "CResources::AddCommonTaskToMap sTaskName" << " " << (sTaskName) << std::endl;
//
////    m_lpxCommonTasksList.push_back(pxTask);
////    m_mpxCommonTaskMap[sTaskName].push_back(pxTask);
//    m_mpxCommonTaskMap[sTaskName] = pxTask;
//}

//-------------------------------------------------------------------------------
CTaskInterface* CResources::AddCommonTaskToMap(std::string sTaskName, std::shared_ptr<CTaskInterface> pxTask)
{
    std::cout << "CResources::AddCommonTaskToMap 1"  << std::endl;

//    std::cout << "CResources::AddCommonTaskToMap this name" << " " << (this -> GetTaskNamePointer()) << std::endl;
//    std::cout << "CResources::AddCommonTaskToMap sTaskName" << " " << (sTaskName) << std::endl;

//    m_lpxCommonTasksList.push_back(pxTask);
//    m_mpxCommonTaskMap[sTaskName].push_back(pxTask);
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
//        if (taskPtr == nullptr || *taskPtr == nullptr)
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

////-------------------------------------------------------------------------------
//CTaskInterface* CResources::GetTaskPointerByNameFromMap(std::string sTaskName)
//{
//    std::cout << "CResources::GetTaskPointerByNameFromMap 1"  << std::endl;
//
////    std::cout << "CResources::GetTaskPointerByNameFromMap this name" << " " << (this -> GetTaskNamePointer()) << std::endl;
////    std::cout << "CResources::GetTaskPointerByNameFromMap sTaskName" << " " << (sTaskName) << std::endl;
//
//    // Пытаемся получить значение по ключу 1
//    std::map<std::string, CTaskInterface*>::iterator it = m_mpxCommonTaskMap.find(sTaskName);
//
//    if (it != m_mpxCommonTaskMap.end())
//    {
//        // ключ найден
//        std::cout << "CResources::GetTaskPointerByNameFromMap 2"  << std::endl;
////        std::cout << "CResources::GetTaskPointerByNameFromMap this name" << " " << ((it -> second) -> GetTaskNamePointer()) << std::endl;
//        return it -> second;
//    }
//    else
//    {
//        std::cout << "CResources::GetTaskPointerByNameFromMap 3"  << std::endl;
//        return 0;
//    }
//
//
////    for (const auto& pair : m_mpxCommonTaskMap)
////    {
////        // Выводим ключи и объекты
////        std::cout << "Key: " << pair.first << ", Object address: " << pair.second.get() << std::endl;
////    }
//}

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
CLinkInterface* CResources::CreateLinkByPerformerName(std::string sTaskName)
{
    std::cout << "CResources::CreateLinkByPerformerName 1"  << std::endl;

//    std::cout << "CResources::CreateLinkByPerformerName this name" << " " << (this -> GetTaskNamePointer()) << std::endl;
//    std::cout << "CResources::CreateLinkByPerformerName sTaskName" << " " << (sTaskName) << std::endl;


    CTaskInterface* pxTask =
        GetTaskPointerByNameFromMap(sTaskName);

    if (pxTask != 0)
    {
//        std::cout << "CResources::CreateLinkByPerformerName 2"  << std::endl;
//        CLinkInterface* pxLink;
////        CLinkInterface* pxLink = new CLink();
////        AddCommonTaskToMap(sTaskName + "Link",
////                           pxLink);
//        pxLink =
//            static_cast<CLink*>(AddCommonTaskToMap(sTaskName + "Link",
//                                std::make_shared<CLink>()));
//        std::cout << "CResources::CreateLinkByPerformerName 22 " << sTaskName + "Link" << std::endl;
//        pxLink ->
//        SetResources(this);
//        pxLink ->
//        SetTaskPerformerName("sTaskName");
//        pxLink ->
//        SetTaskPerformer(pxTask);
//        AddCurrentlyRunningTasksList(pxLink);

//        return pxLink;
    }
    else
    {
        std::cout << "CResources::CreateLinkByPerformerName 3"  << std::endl;
        return 0;
    }
}

//-------------------------------------------------------------------------------
uint8_t* CResources::CreateObjectBySize(size_t uiLength)
{
    std::cout << "CResources::CreateObjectBySize 1"  << std::endl;

    return new uint8_t[uiLength];
}

////-------------------------------------------------------------------------------
//std::list<CTaskInterface*>* CResources::GetCommonTasksListPointer(void)
//{
//    return &m_lpxCommonTasksList;
//}

////-------------------------------------------------------------------------------
//std::list<CTaskInterface*>* CResources::GetCurrentlyRunningTasksListPointer(void)
//{
//    return &m_lpxCurrentlyRunningTasksList;
//}

//-------------------------------------------------------------------------------
CDataContainerInterface* CResources::AddDataContainer(std::shared_ptr<CDataContainerInterface> pxDataContainer)
{
    std::cout << "CResources::AddDataContainer 1"  << std::endl;

    m_lpxDataContainerList.push_back(pxDataContainer);
    return pxDataContainer.get();
}

//-------------------------------------------------------------------------------
