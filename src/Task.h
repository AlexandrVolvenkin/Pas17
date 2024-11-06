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
//class CTimer;
//class CDfaLite;
class CDfa;
class CResourcesInterface;
class CResources;
class CDataContainerInterface;

struct TArgumentData
{
    uint8_t* m_uiDataPointer;
    uint16_t m_uiDataOffset;
    uint16_t m_uiDataLength;
};
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

//#pragma pack(push)
//#pragma pack(1)
//// структура данных задачи.
//    struct TTaskDataPackOne
//    {
//        uint8_t m_uiFsmCommandState;
//
//        union
//        {
//            // числовые данные события.
//            struct
//            {
//                // адрес источника события.
//                uint16_t ui16Address;
//                // состояние события.
//                uint8_t ui8State;
//                // время события.
//                uint8_t ui8Second;
//                uint8_t ui8Minute;
//                uint8_t ui8Hour;
//                uint8_t ui8MonthDay;
//                uint8_t ui8Month;
//                uint8_t ui8Year;
//            };
//
//            struct
//            {
//                uint32_t uiArgumentData1;
//                uint32_t uiArgumentData2;
//                uint32_t uiArgumentData3;
//                uint32_t uiArgumentData4;
//            };
//        };
//    };
//#pragma pack(pop)
//
//    struct TArgumentData
//    {
////        union
////        {
////            // числовые данные события.
////            struct
////            {
//                uint8_t* m_uiDataPointer;
//                uint16_t m_uiDataOffset;
//                uint16_t m_uiDataLength;
////            };
//
////            struct
////            {
////                uint32_t uiArgumentData1;
////                uint32_t uiArgumentData2;
//////                uint32_t uiArgumentData3;
//////                uint32_t uiArgumentData4;
////            };
////        };
//
//////        TArgumentData(uint8_t* uiDataPointer,
//////                      uint16_t uiDataOffset,
//////                      uint16_t uiDataLength) :
////        TArgumentData(void) :
////            m_uiDataPointer(0),
////            m_uiDataOffset(0),
////            m_uiDataLength(0)
////        {
////            std::cout << "TArgumentData created: "
////                      << m_uiDataPointer
////                      << ", "
////                      << m_uiDataOffset
////                      << ", "
////                      << m_uiDataLength
////                      << ", "
////                      << std::endl;
////        }
////
////        ~TArgumentData()
////        {
////            std::cout << "TArgumentData destroyed" << std::endl;
////        }
//    };

//    virtual char* GetTaskNamePointer(void) {};
//    virtual std::shared_ptr<TArgumentData> GetArgumentDataPointer(void) {};
//    virtual void SetResources(CResources* pxResources) {};
//    virtual CResources* GetResources(void) {};
//    virtual void SetTaskCustomerName(std::string sName) {};
////    virtual uint8_t Init(void) {};
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

public:
    char m_acTaskName[TASK_NAME_LENGTH] = {0};
//    std::string m_sTaskName;
//    CTimer m_xTimer;
    CResources* m_pxResources;
    std::string m_sTaskCustomerName;
    CTaskInterface* m_pxTaskCustomer;
//    TArgumentData* m_pxArgumentData;
    std::shared_ptr<TArgumentData> m_pxArgumentData;
//    std::shared_ptr<CDataContainerInterface> m_pxDataContainer;

    CTimer m_xTimer;

    uint8_t m_uiFsmState;
    uint8_t m_uiFsmCommandState;
    uint8_t m_uiFsmAnswerState;

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
