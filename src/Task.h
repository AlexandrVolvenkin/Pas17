#ifndef CTASK_H
#define CTASK_H
//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <stdint.h>
#include <list>
//#include "Dfa.h"
//class CDfa;

class CResources;

//-------------------------------------------------------------------------------
class CTaskInterface
{
public:

    virtual uint8_t Init(void) {};
    virtual uint8_t Fsm(void) {};

//    virtual void AddCurrentlyRunningTask(CTaskInterface* pxTask) {};
//    virtual void AddCommonTask(CTaskInterface* pxTask) {};
//    virtual std::list<CTaskInterface*>* GetCommonTasksListPointer(void) {};
//    virtual std::list<CTaskInterface*>* GetCurrentlyRunningTasksListPointer(void) {};
};

//-------------------------------------------------------------------------------






//-------------------------------------------------------------------------------
class CTask : public CTaskInterface//, public CDfa
{
public:
    enum
    {
        START = 0,
        READY,
        IDDLE,
        STOP,
    };

    enum
    {
        TASK_NAME_LENGTH = 32,
    };

    CTask();
    CTask(CResources* pxResources);
    virtual ~CTask();

//    void SetResources(CResources* pxResources);
//    CResources* GetResources(void);
    uint8_t Init(void);
//    uint8_t Fsm(void);
//    void AddCurrentlyRunningTask(CTaskInterface* pxTask);
//    void AddCommonTask(CTaskInterface* pxTask);

    char* GetTaskNamePointer(void)
    {
        return m_acTaskName;
    };

public:
    char m_acTaskName[TASK_NAME_LENGTH] = {0};
//    CTimer m_xTimer;
    CResources* m_pxResources;

protected:
};

//-------------------------------------------------------------------------------






////-------------------------------------------------------------------------------
//class CTaskContainer : public CTaskInterface//, public CDfa
//{
//public:
//    enum
//    {
//        START = 0,
//        READY,
//        IDDLE,
//        STOP,
//    };
//
//    enum
//    {
//        TASK_NAME_LENGTH = 32,
//    };
//
//    CTaskContainer();
//    virtual ~CTaskContainer();
//    uint8_t Init(void);
////    uint8_t Fsm(void);
////    void AddCurrentlyRunningTask(CTaskInterface* pxTask);
////    void AddCommonTask(CTaskInterface* pxTask);
//
//    char* GetTaskNamePointer(void)
//    {
//        return m_acTaskName;
//    };
//
//public:
//    char m_acTaskName[TASK_NAME_LENGTH] = {0};
//
//protected:
//};

//-------------------------------------------------------------------------------

#endif // CTASK_H
