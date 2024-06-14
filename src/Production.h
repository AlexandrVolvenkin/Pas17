#ifndef CPRODUCTION_H
#define CPRODUCTION_H
//-------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------------------

#include <stdint.h>
#include <thread>
#include <chrono>
#include <list>

#include "Task.h"
//#include "MainProductionCycle.h"

class CMainProductionCycleInterface;
class CLedBlinker;

//-------------------------------------------------------------------------------------------
class CProductionInterface : public CTask
{
public:
    virtual void AddTask(CTaskInterface* pxTask) {};
    virtual void DeleteComletedTasks(void) {};
    virtual void Place(CTaskInterface* pxTask) {};
    virtual uint8_t Init(void) {};
    virtual uint8_t MakeDeal(void) {};
    virtual uint8_t Fsm(void) {};

private:
    static void Process(CTaskInterface* pxTask) {};
};

//-------------------------------------------------------------------------------------------








//-------------------------------------------------------------------------------------------
class CProduction : public CProductionInterface//, public CTask
{
public:
    enum EProduction
    {
        IDDLE  = 0,
        START,

        MAKE_DEAL_START,
        MAKE_DEAL,
        MAKE_DEAL_END,
        END,
    };

    CProduction();
    virtual ~CProduction();
    CProduction(CTaskInterface* pxRootTask);

    void AddTask(CTaskInterface* pxTask);
    void DeleteComletedTasks(void);
    static void Process(CTaskInterface* pxTask);
    void Place(CTaskInterface* pxTask);
    uint8_t Init(void);
    uint8_t Fsm(void);

    void SetThread(std::thread* pxThread)
    {
        m_pxThread = pxThread;
    };
    std::thread* GetThread(void)
    {
        return m_pxThread;
    };

    std::list<CTaskInterface*>* GetExecutingTasksListPointer(void)
    {
        return &m_lpxExecutingTasksList;
    };

protected:

private:
    std::thread* m_pxThread;
    std::list<CTaskInterface*> m_lpxExecutingTasksList;
};

//-------------------------------------------------------------------------------------------








//-------------------------------------------------------------------------------------------
class CMainThreadProduction : public CProduction
{
public:
    CMainThreadProduction();
    virtual ~CMainThreadProduction();
    CMainThreadProduction(CTaskInterface* pxRootTask);

    uint8_t Init(void);
    void Place(CTaskInterface* pxTask);
    static void Process(CTaskInterface* pxTask);
    uint8_t MakeDeal(void);
    uint8_t Fsm(void);

protected:

private:
};

//-------------------------------------------------------------------------------------------








//-------------------------------------------------------------------------------------------
class CRtspThreadProduction : public CProduction
{
public:
    CRtspThreadProduction();
    virtual ~CRtspThreadProduction();

    static void Process(CTaskInterface* pxTask);

protected:

private:
};

//-------------------------------------------------------------------------------------------








//-------------------------------------------------------------------------------------------
class CFileDescriptorEventsWaitingProduction : public CProduction
{
public:

    enum
    {
        START = 0,
        READY,
        IDDLE,
        STOP,
        LED_ON,
        LED_ON_PERIOD_END_WAITING,
        LED_OFF,
        LED_OFF_PERIOD_END_WAITING,
        LED_BLINK_ON,
        LED_BLINK_OFF,

    };

    CFileDescriptorEventsWaitingProduction();
    virtual ~CFileDescriptorEventsWaitingProduction();

    static void Process(CTaskInterface* pxTask);
    void Place(CTaskInterface* pxTask);
    uint8_t Fsm(void);

    void SetThread(std::thread* pxThread)
    {
        m_pxThread = pxThread;
    };
    std::thread* GetThread(void)
    {
        return m_pxThread;
    };

protected:
    std::thread* m_pxThread;
//    CLedBlinker* m_pxLedBlinker;

private:
};

//-------------------------------------------------------------------------------------------








//-------------------------------------------------------------------------------------------
class CSecondThreadProduction : public CProduction
{
public:
    enum ESecondThreadProductionStates
    {
        IDDLE  = 0,
        START,

        MAKE_DEAL_START,
        MAKE_DEAL,
        MAKE_DEAL_END,
        END,
    };

    CSecondThreadProduction();
    virtual ~CSecondThreadProduction();
    CSecondThreadProduction(CTaskInterface* pxRootTask);

    uint8_t Init(void);
    void Place(CTaskInterface* pxTask);
    uint8_t Fsm(void);

private:
    static void Process(CTaskInterface* pxTask);

private:
    std::thread* m_pxThread;
};

//-------------------------------------------------------------------------------------------
#endif // CPRODUCTION_H
