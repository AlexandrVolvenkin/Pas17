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

#include "Dfa.h"
//class CDfa;

//class CDfa;
class CResources;

//-------------------------------------------------------------------------------
class CTaskInterface : public CDfa
{
public:

    virtual char* GetTaskNamePointer(void) {};
    virtual void SetResources(CResources* pxResources) {};
    virtual CResources* GetResources(void) {};
    virtual uint8_t Init(void) {};
//    virtual uint8_t Fsm(void) {};
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

    void SetResources(CResources* pxResources);
    CResources* GetResources(void);
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
//    std::string m_sTaskName;
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
