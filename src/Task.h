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
class CDataContainerDataBase;
class CMessageBoxInterface;
class CMessageBoxGeneral;

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
//        READY_WAITING,
        READY,
        DONE_OK,
        DONE_ERROR,
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
    virtual char* GetTaskNamePointer(void) {};

    virtual void SetCustomertDataContainer(CDataContainerInterface* pxDataContainer) {};
    virtual CDataContainerInterface* GetCustomertDataContainerPointer(void) {};

    virtual void SetExecutorDataContainer(CDataContainerInterface* pxDataContainer) {};
    virtual CDataContainerInterface* GetExecutorDataContainerPointer(void) {};

    virtual bool SetTaskData(uint8_t uiTaskId, CDataContainerInterface* pxDataContainer) {};

    virtual size_t GetObjectLength(void) {};
    virtual bool IsTaskReady(void) {};

    virtual void SetResources(CResources* pxResources) {};
    virtual CResources* GetResources(void) {};
};

//-------------------------------------------------------------------------------






//-------------------------------------------------------------------------------
class CTask : public CTaskInterface
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

    void SetCustomertDataContainer(CDataContainerInterface* pxDataContainer);
    CDataContainerInterface* GetCustomertDataContainerPointer(void);

    void SetExecutorDataContainer(CDataContainerInterface* pxDataContainer);
    CDataContainerInterface* GetExecutorDataContainerPointer(void);

//    bool SetTaskData(CDataContainerInterface* pxDataContainer);
//    CDataContainerInterface* GetTaskData(void);

    bool SetTaskData(uint8_t uiTaskId, CDataContainerInterface* pxDataContainer);

    size_t GetObjectLength(void);
    bool IsTaskReady(void);

    char* GetTaskNamePointer(void);

public:
    char m_acTaskName[TASK_NAME_LENGTH] = {0};
//    std::string m_sTaskName;
    CResources* m_pxResources;

    CDataContainerInterface* m_pxCustomertDataContainer;
    CDataContainerInterface* m_pxExecutorDataContainer;
};

//-------------------------------------------------------------------------------

#endif // CTASK_H
