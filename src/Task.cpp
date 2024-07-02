//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <iostream>
#include <list>
#include <string.h>

#include "Task.h"
#include "Dfa.h"

//-------------------------------------------------------------------------------
CTask::CTask()
{
    std::cout << "CTask constructor 1"  << std::endl;
//    SetFsmState(IDDLE);
}

//-------------------------------------------------------------------------------
CTask::~CTask()
{
    std::cout << "CTask destructor"  << std::endl;
}

//-------------------------------------------------------------------------------
uint8_t CTask::Init(void)
{
    std::cout << "CTask Init"  << std::endl;
}

//-------------------------------------------------------------------------------
void CTask::AddCurrentlyRunningTask(CTaskInterface* pxTask)
{
    //std::cout << "CTask::AddCurrentlyRunningTask"  << std::endl;

}

//-------------------------------------------------------------------------------
void CTask::AddCommonTask(CTaskInterface* pxTask)
{
    std::cout << "CTask::AddCommonTask 1"  << std::endl;

}

//-------------------------------------------------------------------------------
uint8_t CTask::Fsm(void)
{
//        std::cout << "CTask::Fsm 1"  << std::endl;

    switch (GetFsmState())
    {
    case IDDLE:
        //std::cout << "CTask::Fsm IDDLE"  << std::endl;
        break;

    case START:
//        //std::cout << "CTask::Fsm START"  << std::endl;
        SetFsmState(START);
        break;

    default:
        break;
    }

    return GetFsmState();
}

//-------------------------------------------------------------------------------





//-------------------------------------------------------------------------------
CTaskContainer::CTaskContainer()
{
    std::cout << "CTaskContainer constructor 1"  << std::endl;
//    SetFsmState(IDDLE);
}

//-------------------------------------------------------------------------------
CTaskContainer::~CTaskContainer()
{
    std::cout << "CTaskContainer destructor"  << std::endl;
}

//-------------------------------------------------------------------------------
uint8_t CTaskContainer::Init(void)
{
    std::cout << "CTaskContainer Init"  << std::endl;
}

//-------------------------------------------------------------------------------
uint8_t CTaskContainer::Fsm(void)
{
//        std::cout << "CTaskContainer::Fsm 1"  << std::endl;

    switch (GetFsmState())
    {
    case START:
        //std::cout << "CTaskContainer::Fsm START"  << std::endl;
        break;

    case READY:
        //std::cout << "CTaskContainer::Fsm READY"  << std::endl;
        break;

    case IDDLE:
        //std::cout << "CTaskContainer::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CTaskContainer::Fsm STOP"  << std::endl;
        SetFsmState(START);
        break;

    default:
        break;
    }

    return GetFsmState();
}

//-------------------------------------------------------------------------------

