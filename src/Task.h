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
#include <memory>

#include "Timer.h"
#include "Dfa.h"
//class CTimer;
//class CDfaLite;
class CDfa;
class CResourcesInterface;
class CResources;
class CDataContainerInterface;

//-------------------------------------------------------------------------------
class CTaskInterface : public CDfa
{
public:
    enum
    {
        IDDLE = 1,
        STOP,
        START,
        INIT,
        READY,
        NEXT_STEP,
    };

    struct TArgumentData
    {
        uint8_t* m_puiDataPointer;
        uint16_t m_uiDataOffset;
        uint16_t m_uiDataLength;
    };

//    CTaskInterface();
//    virtual ~CTaskInterface();
//    virtual char* GetTaskNamePointer(void);
//    virtual std::shared_ptr<TArgumentData> GetArgumentDataPointer(void);
//    virtual void SetResources(CResources* pxResources);
//    virtual CResources* GetResources(void);
//    virtual void SetTaskCustomerName(std::string sName);
    virtual char* GetTaskNamePointer(void) {};
    virtual std::shared_ptr<TArgumentData> GetArgumentDataPointer(void) {};
    virtual std::shared_ptr<CDataContainerInterface> GetDataContainerPointer(void) {};
    virtual void SetResources(CResources* pxResources) {};
    virtual CResources* GetResources(void) {};
    virtual void SetTaskCustomerName(std::string sName) {};
//    virtual uint8_t Init(void) {};
//    virtual uint8_t Fsm(void) {};
//    virtual uint8_t GetFsmState(void) {};
//    virtual void SetFsmState(uint8_t uiData) {};
//    virtual CTimer* GetTimerPointer(void) {};

//   virtual uint8_t GetFsmState(void)
//    {
//        return m_uiFsmState;
//    };
//
////protected:
//    virtual void SetFsmState(uint8_t uiData)
//    {
//        m_uiFsmState = uiData;
//    };
//    virtual CTimer* GetTimerPointer(void)
//    {
//        return &m_xTimer;
//    };
//    CTimer m_xTimer;
//
//    uint8_t m_uiFsmState;
//    uint8_t m_uiFsmCommandState;
//    uint8_t m_uiFsmAnswerState;
};

//-------------------------------------------------------------------------------






//-------------------------------------------------------------------------------
class CTask : public CTaskInterface//, public CDfa
{
public:
    enum
    {
        TASK_NAME_LENGTH = 32,
        TASK_READY_WAITING_TIME = 1000,
    };

    CTask();
    CTask(CResources* pxResources);
    virtual ~CTask();

    void SetResources(CResources* pxResources);
    CResources* GetResources(void);
    void SetTaskCustomerName(std::string sName);
//    uint8_t Init(void);
    uint8_t Fsm(void);
//    void AddCurrentlyRunningTask(CTaskInterface* pxTask);
//    void AddCommonTask(CTaskInterface* pxTask);
    uint8_t GetFsmState(void);
    void SetFsmState(uint8_t uiData);
    CTimer* GetTimerPointer(void);

    char* GetTaskNamePointer(void);
    std::shared_ptr<TArgumentData> GetArgumentDataPointer(void);
    std::shared_ptr<CDataContainerInterface> GetDataContainerPointer(void);

public:
    char m_acTaskName[TASK_NAME_LENGTH] = {0};
//    std::string m_sTaskName;
//    CTimer m_xTimer;
    CResources* m_pxResources;
    std::string m_sTaskCustomerName;
    CTaskInterface* m_pxTaskCustomer;
//    TArgumentData* m_pxArgumentData;
    std::shared_ptr<TArgumentData> m_pxArgumentData;
    std::shared_ptr<CDataContainerInterface> m_pxDataContainer;
};

//-------------------------------------------------------------------------------

#endif // CTASK_H
