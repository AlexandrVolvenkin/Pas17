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
        NO_SUBTASK = 0,
        IDDLE = 1,
        STOP,
        START,
        INIT,
        READY,
        DONE_OK,
        DONE_ERROR,

        SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START,
        SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING,

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
    virtual void SetTaskName(std::string& sTaskName) {};
    virtual std::string& GetTaskName(void) {};
    virtual char* GetTaskNamePointer(void) {};

    virtual void SetFsmNextSubTaskState(uint8_t uiData) {};
    virtual uint8_t GetFsmNextSubTaskState(void) {};
    virtual void SetFsmNextStateDoneOk(uint8_t uiData) {};
    virtual uint8_t GetFsmNextStateDoneOk(void) {};
    virtual void SetFsmNextStateDoneError(uint8_t uiData) {};
    virtual uint8_t GetFsmNextStateDoneError(void) {};
    virtual void SetFsmNextStateReadyWaitingError(uint8_t uiData) {};
    virtual uint8_t GetFsmNextStateReadyWaitingError(void) {};
    virtual void SetFsmNextStateDoneWaitingError(uint8_t uiData) {};
    virtual uint8_t GetFsmNextStateDoneWaitingError(void) {};
    virtual void SetFsmNextStateDoneWaitingDoneError(uint8_t uiData) {};
    virtual uint8_t GetFsmNextStateDoneWaitingDoneError(void) {};
    virtual void SetFsmNextStateDoneWaitingDoneOk(uint8_t uiData) {};
    virtual uint8_t GetFsmNextStateDoneWaitingDoneOk(void) {};

    virtual void SetCustomerDataContainer(CDataContainerInterface* pxDataContainer) {};
    virtual CDataContainerInterface* GetCustomerDataContainerPointer(void) {};

    virtual void SetExecutorDataContainer(CDataContainerInterface* pxDataContainer) {};
    virtual CDataContainerInterface* GetExecutorDataContainerPointer(void) {};

    virtual void SetCurrentCustomerDataContainer(CDataContainerInterface* pxDataContainer) {};
    virtual CDataContainerInterface* GetCurrentCustomerDataContainerPointer(void) {};

    virtual void SetCurrentExecutorDataContainer(CDataContainerInterface* pxDataContainer) {};
    virtual CDataContainerInterface* GetCurrentExecutorDataContainerPointer(void) {};

    virtual bool SetTaskData(uint8_t uiTaskId, CDataContainerInterface* pxDataContainer) {};

    virtual size_t GetObjectLength(void) {};
    virtual bool IsTaskReady(void) {};
    virtual bool IsDoneOk(void) {};
    virtual bool IsDoneError(void) {};

    virtual std::string GetExecutorTaskName() {};
    virtual void SetExecutorTaskName(std::string& sName) {};
    virtual void SetExecutorTaskId(uint8_t uiData) {};
    virtual uint8_t GetExecutorTaskId(void) {};

    virtual std::string GetNextTaskDoneOkName() {};
    virtual void SetNextTaskDoneOkName(std::string& sName) {};
    virtual void SetNextTaskDoneOkId(uint8_t uiData) {};
    virtual uint8_t GetNextTaskDoneOkId(void) {};

    virtual std::string GetNextTaskDoneErrorName() {};
    virtual void SetNextTaskDoneErrorName(std::string& sName) {};
    virtual void SetNextTaskDoneErrorId(uint8_t uiData) {};
    virtual uint8_t GetNextTaskDoneErrorId(void) {};

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
        TASK_READY_WAITING_TIME_LONG = 10000,
    };

    CTask();
    CTask(CResources* pxResources);
    virtual ~CTask();

    void SetTaskName(std::string& sTaskName)
    {
        m_sTaskName = sTaskName;
    };

    std::string& GetTaskName(void)
    {
        return m_sTaskName;
    };

    void SetFsmNextSubTaskState(uint8_t uiData)
    {
        m_uiFsmNextSubTaskState = uiData;
    };

    uint8_t GetFsmNextSubTaskState(void)
    {
        return m_uiFsmNextSubTaskState;
    };

    void SetFsmNextStateDoneOk(uint8_t uiData)
    {
        m_uiFsmNextStateDoneOk = uiData;
    };

    uint8_t GetFsmNextStateDoneOk(void)
    {
        return m_uiFsmNextStateDoneOk;
    };

    void SetFsmNextStateDoneError(uint8_t uiData)
    {
        m_uiFsmNextStateDoneError = uiData;
    };

    uint8_t GetFsmNextStateDoneError(void)
    {
        return m_uiFsmNextStateDoneError;
    };

    void SetFsmNextStateReadyWaitingError(uint8_t uiData)
    {
        m_uiFsmNextStateReadyWaitingError = uiData;
    };

    uint8_t GetFsmNextStateReadyWaitingError(void)
    {
        return m_uiFsmNextStateReadyWaitingError;
    };

    void SetFsmNextStateDoneWaitingError(uint8_t uiData)
    {
        m_uiFsmNextStateDoneWaitingError = uiData;
    };

    uint8_t GetFsmNextStateDoneWaitingError(void)
    {
        return m_uiFsmNextStateDoneWaitingError;
    };

    void SetFsmNextStateDoneWaitingDoneError(uint8_t uiData)
    {
        m_uiFsmNextStateDoneWaitingDoneError = uiData;
    };

    uint8_t GetFsmNextStateDoneWaitingDoneError(void)
    {
        return m_uiFsmNextStateDoneWaitingDoneError;
    };

    void SetFsmNextStateDoneWaitingDoneOk(uint8_t uiData)
    {
        m_uiFsmNextStateDoneWaitingDoneOk = uiData;
    };

    uint8_t GetFsmNextStateDoneWaitingDoneOk(void)
    {
        return m_uiFsmNextStateDoneWaitingDoneOk;
    };

    void SetResources(CResources* pxResources);
    CResources* GetResources(void);

    void SetCustomerDataContainer(CDataContainerInterface* pxDataContainer);
    CDataContainerInterface* GetCustomerDataContainerPointer(void);

    void SetExecutorDataContainer(CDataContainerInterface* pxDataContainer);
    CDataContainerInterface* GetExecutorDataContainerPointer(void);

    void SetCurrentCustomerDataContainer(CDataContainerInterface* pxDataContainer);
    CDataContainerInterface* GetCurrentCustomerDataContainerPointer(void);

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
    std::string m_sTaskName;
    CResources* m_pxResources;
    // содержит идентификатор шага автомата, начала выполнения следующей подзадачи.
    uint8_t m_uiFsmNextSubTaskState;
    // содержит идентификатор шага автомата, при успешном завершении подзадачи.
    uint8_t m_uiFsmNextStateDoneOk;
    // содержит идентификатор шага автомата, при не успешном завершении подзадачи.
    uint8_t m_uiFsmNextStateDoneError;
    // содержит идентификатор шага автомата, при не успешном завершении ожидания готовности подзадачи.
    uint8_t m_uiFsmNextStateReadyWaitingError;
    // содержит идентификатор шага автомата, при не успешном завершении ожидания результата подзадачи.
    uint8_t m_uiFsmNextStateDoneWaitingError;
    // содержит идентификатор шага автомата, при не успешном завершении подзадачи.
    uint8_t m_uiFsmNextStateDoneWaitingDoneError;
    // содержит идентификатор шага автомата, при успешном завершении подзадачи.
    uint8_t m_uiFsmNextStateDoneWaitingDoneOk;

    CDataContainerInterface* m_pxCurrentCustomerDataContainer;
    CDataContainerInterface* m_pxCustomerDataContainer;
    CDataContainerInterface* m_pxCurrentExecutorDataContainer;
    CDataContainerInterface* m_pxExecutorDataContainer;
};






//-------------------------------------------------------------------------------
class CNewTask : public CTaskInterface
{
public:
    enum
    {
        TASK_NAME_LENGTH = 32,
        TASK_READY_WAITING_TIME = 500,
        TASK_READY_WAITING_TIME_LONG = 10000,
    };

    enum
    {
        NO_SUBTASK = 0,
        INIT,
        STOP,
        START,
        DONE_OK,
        DONE_ERROR,

        NEXT_STEP,
    };

//-------------------------------------------------------------------------------
    CNewTask();
    virtual ~CNewTask();

//-------------------------------------------------------------------------------
    virtual void SetTaskName(std::string& sTaskName)
    {
        m_sTaskName = sTaskName;
    };
//-------------------------------------------------------------------------------
    virtual std::string& GetTaskName(void)
    {
        return m_sTaskName;
    };

//-------------------------------------------------------------------------------
    virtual std::string GetExecutorTaskName()
    {
        return m_sExecutorTaskName;
    };
    virtual void SetExecutorTaskName(std::string& sName)
    {
        m_sExecutorTaskName = sName;
    };

    virtual void SetExecutorTaskId(uint8_t uiData)
    {
        m_uiExecutorTaskId = uiData;
    };
    virtual uint8_t GetExecutorTaskId(void)
    {
        return m_uiExecutorTaskId;
    };

//-------------------------------------------------------------------------------
    virtual std::string GetNextTaskDoneOkName()
    {
        return m_sNextTaskDoneOkName;
    };
    virtual void SetNextTaskDoneOkName(std::string& sName)
    {
        m_sNextTaskDoneOkName = sName;
    };

    virtual void SetNextTaskDoneOkId(uint8_t uiData)
    {
        m_uiNextTaskDoneOkId = uiData;
    };
    virtual uint8_t GetNextTaskDoneOkId(void)
    {
        return m_uiNextTaskDoneOkId;
    };

//-------------------------------------------------------------------------------
    virtual std::string GetNextTaskDoneErrorName()
    {
        return m_sNextTaskDoneErrorName;
    };
    virtual void SetNextTaskDoneErrorName(std::string& sName)
    {
        m_sNextTaskDoneErrorName = sName;
    };

    virtual void SetNextTaskDoneErrorId(uint8_t uiData)
    {
        m_uiNextTaskDoneErrorId = uiData;
    };
    virtual uint8_t GetNextTaskDoneErrorId(void)
    {
        return m_uiNextTaskDoneErrorId;
    };

//-------------------------------------------------------------------------------
    virtual void SetResources(CResources* pxResources);
    virtual CResources* GetResources(void);

    virtual uint8_t Init(void);
    uint8_t Fsm(void);

//-------------------------------------------------------------------------------
public:
    // содержит имя задачи исполнителя.
    std::string m_sTaskName;
    CResources* m_pxResources;

    // содержит имя задачи.
    std::string m_sExecutorTaskName;
    // содержит идентификатор задачи исполнителя.
    uint8_t m_uiExecutorTaskId;
    // содержит имя следующей задачи, при успешном завершении задачи исполнителя.
    std::string m_sNextTaskDoneOkName;
    // содержит идентификатор следующей задачи, при успешном завершении задачи исполнителя.
    uint8_t m_uiNextTaskDoneOkId;
    // содержит имя следующей задачи, при не успешном завершении задачи исполнителя.
    std::string m_sNextTaskDoneErrorName;
    // содержит идентификатор следующей задачи, при не успешном завершении задачи исполнителя.
    uint8_t m_uiNextTaskDoneErrorId;
};


//-------------------------------------------------------------------------------

#endif // CTASK_H

