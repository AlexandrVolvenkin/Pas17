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

    SetResources(this);
//    m_pxResources -> AddCommonListTask(this);

    ModbusWorkingArraysCreate(COILS_WORK_ARRAY_LENGTH,
                              DISCRETE_INPUTS_ARRAY_LENGTH,
                              HOLDING_REGISTERS_ARRAY_LENGTH,
                              INPUT_REGISTERS_ARRAY_LENGTH);
}

//-------------------------------------------------------------------------------
CResources::~CResources()
{
    // Освобождаем память
    for (auto& pair : m_mpxCommonTaskMap)
    {
        delete pair.second; // Удаляем указатели на объекты
    }
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
void CResources::AddCommonListTask(CTaskInterface* pxTask)
{
    std::cout << "CResources::AddCommonListTask 1"  << std::endl;

    std::cout << "CResources::AddCommonListTask this name" << " " << (this -> GetTaskNamePointer()) << std::endl;
    std::cout << "CResources::AddCommonListTask pxTask name" << " " << (pxTask -> GetTaskNamePointer()) << std::endl;

    m_lpxCommonTasksList.push_back(pxTask);
}

//-------------------------------------------------------------------------------
void CResources::AddCurrentlyRunningTasksList(CTaskInterface* pxTask)
{
    std::cout << "CResources::AddCurrentlyRunningTasksList 1"  << std::endl;

    std::cout << "CResources::AddCurrentlyRunningTasksList this name" << " " << (this -> GetTaskNamePointer()) << std::endl;
    std::cout << "CResources::AddCurrentlyRunningTasksList pxTask name" << " " << (pxTask -> GetTaskNamePointer()) << std::endl;

    m_lpxCurrentlyRunningTasksList.push_back(pxTask);
}

//-------------------------------------------------------------------------------
CTaskInterface* CResources::GetCommonListTaskPointer(char* pcTaskName)
{
    std::cout << "CResources::GetCommonListTaskPointer 1"  << std::endl;

    std::cout << "CResources::GetCommonListTaskPointer this name" << " " << (this -> GetTaskNamePointer()) << std::endl;
    std::cout << "CResources::GetCommonListTaskPointer pcTaskName" << " " << pcTaskName << std::endl;

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
void CResources::AddCommonTaskToMap(std::string sTaskName, CTaskInterface* pxTask)
{
    std::cout << "CResources::AddCommonTaskToMap 1"  << std::endl;

    std::cout << "CResources::AddCommonTaskToMap this name" << " " << (this -> GetTaskNamePointer()) << std::endl;
    std::cout << "CResources::AddCommonTaskToMap sTaskName" << " " << (sTaskName) << std::endl;

//    m_lpxCommonTasksList.push_back(pxTask);
//    m_mpxCommonTaskMap[sTaskName].push_back(pxTask);
    m_mpxCommonTaskMap[sTaskName] = pxTask;
}

//-------------------------------------------------------------------------------
CTaskInterface* CResources::GetCommonTaskFromMapPointer(std::string sTaskName)
{
    std::cout << "CResources::GetCommonTaskFromMapPointer 1"  << std::endl;

    std::cout << "CResources::GetCommonTaskFromMapPointer this name" << " " << (this -> GetTaskNamePointer()) << std::endl;
    std::cout << "CResources::GetCommonTaskFromMapPointer sTaskName" << " " << (sTaskName) << std::endl;

    // Пытаемся получить значение по ключу 1
    std::map<std::string, CTaskInterface*>::iterator it = m_mpxCommonTaskMap.find(sTaskName);

    if (it != m_mpxCommonTaskMap.end())
    {
        // ключ найден
        std::cout << "CResources::GetCommonTaskFromMapPointer 2"  << std::endl;
        std::cout << "CResources::GetCommonTaskFromMapPointer this name" << " " << ((it -> second) -> GetTaskNamePointer()) << std::endl;
        return it -> second;
    }
    else
    {
        std::cout << "CResources::GetCommonTaskFromMapPointer 3"  << std::endl;
        return 0;
    }

//    if (strstr(((*xListIterator) -> GetTaskNamePointer()), pcTaskName) != 0)
//    {
//        std::cout << "CResources::GetCommonTaskFromMapPointer 2"  << std::endl;
//        return (*xListIterator);
//    }
////        else
////        {
////            std::cout << "CResources::GetCommonTaskFromMapPointer 3"  << std::endl;
////            return 0;
////        }
//
//    std::list<CTaskInterface*>::iterator xListIterator;
//
//    for(xListIterator = m_lpxCommonTasksList.begin();
//            xListIterator != m_lpxCommonTasksList.end();
//            xListIterator++)
//    {
//        std::cout << "CResources::GetCommonTaskFromMapPointer xListIterator name" << " " << ((*xListIterator) -> GetTaskNamePointer()) << std::endl;
//
//        if (strstr(((*xListIterator) -> GetTaskNamePointer()), pcTaskName) != 0)
//        {
//            std::cout << "CResources::GetCommonTaskFromMapPointer 2"  << std::endl;
//            return (*xListIterator);
//        }
////        else
////        {
////            std::cout << "CResources::GetCommonTaskFromMapPointer 3"  << std::endl;
////            return 0;
////        }
//    }
//
//    std::cout << "CResources::GetCommonTaskFromMapPointer 4"  << std::endl;
//    return 0;
}

//-------------------------------------------------------------------------------


//#include <iostream>
//#include <map>
//#include <string>
//
//class Object
//{
//public:
//    Object(const std::string& name) : name(name) {}
//    void display() const
//    {
//        std::cout << "Object name: " << name << std::endl;
//    }
//private:
//    std::string name;
//};
//
//int main()
//{
//    // Создаем std::map, где ключ - строка, значение - указатель на объект
//    std::map<std::string, Object*> objectMap;
//
//    // Создаем несколько объектов и добавляем их в std::map
//    Object* obj1 = new Object("Object1");
//    Object* obj2 = new Object("Object2");
//    Object* obj3 = new Object("Object3");
//
//    // Добавляем объекты в map
//    objectMap["Object1"] = obj1;
//    objectMap["Object2"] = obj2;
//    objectMap["Object3"] = obj3;
//
//    // Получаем объект по имени и вызываем метод
//    objectMap["Object1"]->display();
//    objectMap["Object2"]->display();
//    objectMap["Object3"]->display();
//
//    // Освобождаем память
//    for (auto& pair : objectMap)
//    {
//        delete pair.second; // Удаляем указатели на объекты
//    }
//
//    return 0;
//}
//
////Объяснение кода:
////
////Определён класс Object, который имеет конструктор для инициализации имени объекта и метод
////для отображения имени.
////В main() создаётся std::map, где ключом является std::string, а значением — указатель на
////Object.
////Создаются объекты типа Object с различными именами, и их адреса хранятся в std::map, где имя
////объекта является ключом.
////Затем, по ключу из map, вызывается метод display() для отображения имени объекта.
////В конце освободите выделенную память, удаляя указатели на объекты.
////
////Этот пример демонстрирует, как использовать std::map для хранения объектов и их имен.
