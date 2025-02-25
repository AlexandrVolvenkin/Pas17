﻿#ifndef CTASK_H
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
        NO_SUBTASK = 0,
        IDDLE = 1,
        STOP,
        START,
        INIT,
        READY,
        DONE_OK,
        DONE_ERROR,

        SUBTASK_START,
        SUBTASK_EXECUTOR_READY_CHECK_START,
        SUBTASK_EXECUTOR_READY_CHECK_WAITING,
        SUBTASK_EXECUTOR_DONE_CHECK_START,
        SUBTASK_EXECUTOR_DONE_CHECK_WAITING,

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

    virtual void SetFsmNextSubTaskState(uint8_t uiData) {};
    virtual uint8_t GetFsmNextSubTaskState(void) {};

    virtual void SetCustomertDataContainer(CDataContainerInterface* pxDataContainer) {};
    virtual CDataContainerInterface* GetCustomertDataContainerPointer(void) {};

    virtual void SetExecutorDataContainer(CDataContainerInterface* pxDataContainer) {};
    virtual CDataContainerInterface* GetExecutorDataContainerPointer(void) {};

    virtual void SetCurrentCustomertDataContainer(CDataContainerInterface* pxDataContainer) {};
    virtual CDataContainerInterface* GetCurrentCustomertDataContainerPointer(void) {};

    virtual void SetCurrentExecutorDataContainer(CDataContainerInterface* pxDataContainer) {};
    virtual CDataContainerInterface* GetCurrentExecutorDataContainerPointer(void) {};

    virtual bool SetTaskData(uint8_t uiTaskId, CDataContainerInterface* pxDataContainer) {};

    virtual size_t GetObjectLength(void) {};
    virtual bool IsTaskReady(void) {};
    virtual bool IsDoneOk(void) {};
    virtual bool IsDoneError(void) {};

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
        TASK_READY_WAITING_TIME = 500,
    };

    CTask();
    CTask(CResources* pxResources);
    virtual ~CTask();

    void SetFsmNextSubTaskState(uint8_t uiData)
    {
        m_uiFsmNextSubTaskState = uiData;
    };

    uint8_t GetFsmNextSubTaskState(void)
    {
        return m_uiFsmNextSubTaskState;
    };

    void SetResources(CResources* pxResources);
    CResources* GetResources(void);

    void SetCustomertDataContainer(CDataContainerInterface* pxDataContainer);
    CDataContainerInterface* GetCustomertDataContainerPointer(void);

    void SetExecutorDataContainer(CDataContainerInterface* pxDataContainer);
    CDataContainerInterface* GetExecutorDataContainerPointer(void);

    void SetCurrentCustomertDataContainer(CDataContainerInterface* pxDataContainer);
    CDataContainerInterface* GetCurrentCustomertDataContainerPointer(void);

    void SetCurrentExecutorDataContainer(CDataContainerInterface* pxDataContainer);
    CDataContainerInterface* GetCurrentExecutorDataContainerPointer(void);

    bool SetTaskData(CDataContainerInterface* pxDataContainer);
    bool SetTaskData(uint8_t uiTaskId, CDataContainerInterface* pxDataContainer);
//    CDataContainerInterface* GetTaskData(void);


    size_t GetObjectLength(void);
    bool IsTaskReady(void);
    bool IsDoneOk(void);
    bool IsDoneError(void);

    char* GetTaskNamePointer(void);

public:
    char m_acTaskName[TASK_NAME_LENGTH] = {0};
//    std::string m_sTaskName;
    CResources* m_pxResources;
    // содержит идентификатор шага автомата, начала выполнения следующей подзадачи.
    uint8_t m_uiFsmNextSubTaskState;

    CDataContainerInterface* m_pxCurrentCustomertDataContainer;
    CDataContainerInterface* m_pxCustomertDataContainer;
    CDataContainerInterface* m_pxCurrentExecutorDataContainer;
    CDataContainerInterface* m_pxExecutorDataContainer;
};

//-------------------------------------------------------------------------------

#endif // CTASK_H
