#ifndef CRESOURCES_H
#define CRESOURCES_H
//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <stdint.h>
#include <list>
#include <map>
#include <memory>
#include <string>

#include "DataTypes.h"
//#include "Dfa.h"
#include "Task.h"
#include "ConfigurationCreate.h"
#include "DataBaseCreate.h"
//#include "DiscreteSignals.h"

class CDfa;
//class CTaskInterface;
//class CTask;
class CDeviceControl;
class CDataStore;
class CConfiguration;
class CServiceMarket;
class CLinkInterface;
class CAnalogueSignals;
//class CConfigurationCreate;
struct TAnalogueInputDescriptionWork;
//struct TConfigDataPackOne;
class CDiscreteSignals;

//-------------------------------------------------------------------------------
class CResourcesInterface : public CTask
{
public:
    virtual void Allocate(void) {};
    virtual void AddCommonListTask(CTaskInterface* pxTask) {};
    virtual CTaskInterface* GetCommonListTaskPointer(char* pcTaskName) {};
    virtual CTaskInterface* AddCommonTaskToMap(std::string sTaskName, std::shared_ptr<CTaskInterface>) {};
    virtual bool CheckCommonTaskMap(void) {};
    virtual CTaskInterface* GetTaskPointerByNameFromMap(std::string sTaskName) {};
    virtual std::list<CTaskInterface*>* GetCommonTasksListPointer(void) {};
    virtual uint8_t* CreateObjectBySize(size_t uiLength) {};
    virtual CDataContainerInterface* AddDataContainer(std::shared_ptr<CDataContainerInterface> pxDataContainer) {};
    virtual CConfigurationCreate::TConfigDataPackOne* GetDeviceConfigSearchPointer(void) {};
    virtual TDiscreteSygnalTextTitle* GetDiscreteSygnalTextTitlesWorkPointer(void) {};

};

//-------------------------------------------------------------------------------










//-------------------------------------------------------------------------------
class CResources : public CResourcesInterface
{
public:
    enum
    {
        MAX_TASK_NUMBER = 1024,
    };

    CResources();
    virtual ~CResources();

    void AddCommonListTask(CTaskInterface* pxTask);
    void AddCurrentlyRunningTasksList(CTaskInterface* pxTask);
    CTaskInterface* GetCommonListTaskPointer(char* pcTaskName);
    CTaskInterface* AddCommonTaskToMap(std::string sTaskName, std::shared_ptr<CTaskInterface> pxTask);
    CTaskInterface* CreateTaskAndLinkToOthers(std::shared_ptr<CTaskInterface> pxTask,
            std::string sTaskName,
            std::string sExecutorTaskName,
            std::string sNextTaskDoneOkName,
            std::string sNextTaskDoneErrorName);
    bool CheckCommonTaskMap(void);
    CTaskInterface* GetTaskPointerByNameFromMap(std::string sTaskName);
    uint8_t GetTaskIdByNameFromMap(std::string sTaskName);
    CTaskInterface* GetTaskPointerById(uint8_t uiTaskId);
    uint8_t* CreateObjectBySize(size_t uiLength);
    CDataContainerInterface* AddDataContainer(std::shared_ptr<CDataContainerInterface> pxDataContainer);

    void SendRequest(void);
    void ReceiveAnswer(void);

    CConfigurationCreate::TConfigDataPackOne* GetDeviceConfigSearchPointer(void);
    TDiscreteSygnalTextTitle* GetDiscreteSygnalTextTitlesWorkPointer(void);


    void ModbusWorkingArraysCreate(uint16_t uiCoilsNumber,
                                   uint16_t uiDiscreteInputsNumber,
                                   uint16_t uiHoldingRegistersNumber,
                                   uint16_t uiInputRegistersNumber);
    void ModbusWorkingArraysDestroy(void);
    void SetCoils(uint8_t* puiPointer);
    uint8_t* GetCoils(void);
    void SetCoilsNumber(uint16_t uiData);
    uint16_t GetCoilsNumber(void);

    void SetDiscreteInputs(uint8_t* puiPointer);
    uint8_t* GetDiscreteInputs(void);
    void SetDiscreteInputsNumber(uint16_t uiData);
    uint16_t GetDiscreteInputsNumber(void);

    void SetHoldingRegisters(uint16_t* puiPointer);
    uint16_t* GetHoldingRegisters(void);
    void SetHoldingRegistersNumber(uint16_t uiData);
    uint16_t GetHoldingRegistersNumber(void);

    void SetInputRegisters(uint16_t* puiPointer);
    uint16_t* GetInputRegisters(void);
    void SetInputRegistersNumber(uint16_t uiData);
    uint16_t GetInputRegistersNumber(void);

    void Allocate(void);

//protected:
//
//private:
    struct tm xCurrentTime;

    std::list<CTaskInterface*> m_lpxCommonTasksList;
    std::list<CTaskInterface*>::iterator m_xCommonTasksListIterator;
    std::list<CTaskInterface*> m_lpxCurrentlyRunningTasksList;
    std::list<CTaskInterface*>::iterator m_xCurrentlyRunningTasksListIterator;
    std::list<std::shared_ptr<CDataContainerInterface>> m_lpxDataContainerList;

    TAnalogueInputDescriptionWork* m_pxAnalogueInputDescriptionWork;
//    uint8_t* m_puiAnalogueInputDescriptionWork;
    uint32_t m_uiUsedAnalogueInputDescriptionWork;

    struct TDiscreteSignalsDescriptionWork *m_pxDiscreteSignalsDescriptionWork;
    uint32_t m_uiUsedDiscreteSignalsDescriptionWork;

    TDiscreteSygnalTextTitle *m_pxDiscreteSygnalTextTitlesWork;
    uint32_t m_uiUsedDiscreteSygnalTextTitlesWork;

    // Массив управления модулями дискретных выходов(реле).
    TDiscreteOutputControl* m_pxDiscreteOutputControl;


    // Создаем std::map, где ключ - строка, значение - указатель на объект
    std::map<std::string, std::shared_ptr<CTaskInterface>> m_mpxCommonTaskMap;
    std::string m_sTaskName;
    CTaskInterface** m_ppxCommonTaskPointers;
    uint16_t m_uiUsedCommonTaskPointersCounter;

    // объект с конфигурацией во внутреннем формате.
    CConfigurationCreate::TConfigDataPackOne m_xDeviceConfigSearch;

    uint8_t* m_puiCoils;
    uint8_t* m_puiDiscreteInputs;
    uint16_t* m_puiHoldingRegisters;
    uint16_t* m_puiInputRegisters;
    uint16_t m_uiCoilsNumber;
    uint16_t m_uiDiscreteInputsNumber;
    uint16_t m_uiHoldingRegistersNumber;
    uint16_t m_uiInputRegistersNumber;


    uint8_t m_uiAddress;
    uint8_t* m_puiRxBuffer;
    uint8_t* m_puiTxBuffer;
    uint8_t* m_puiErrorCode;

    uint8_t* m_puiDiscreteInputsState;
    uint32_t m_uiUsedDiscreteInputsState;
    uint8_t* m_puiDiscreteInputsBadState;
    uint8_t m_uiUsedDiscreteInputsBadState;
    uint8_t* m_puiDiscreteOutputState;
    uint8_t m_uiUsedDiscreteOutputState;
//    uint8_t* m_puiDiscreteOutputControl;
    uint8_t m_uiUsedDiscreteOutputControl;

    float* m_pfAnalogueInputsValue;
    uint32_t m_uiUsedAnalogueInputsValue;
    uint8_t* m_puiAnalogueInputsState;
    uint32_t m_uiUsedAnalogueInputsState;
    uint8_t* m_puiAnalogueInputsOff;
    uint32_t m_uiUsedAnalogueInputsOff;
    uint8_t* m_puiAnalogueInputsBadState;
    uint32_t m_uiUsedAnalogueInputsBadState;
    uint8_t* m_puiReperPointsAdcBuffer;
    uint32_t m_uiUsedReperPointsAdcBuffer;
    uint8_t* m_puiAnalogueInputDiscreteInputsState;
    uint32_t m_uiUsedAnalogueInputDiscreteInputsState;
    uint8_t* m_puiAnalogueInputDiscreteInputsBadState;
    uint8_t m_uiUsedAnalogueInputDiscreteInputsBadState;

    uint8_t* m_puiSerialAndId;
    uint8_t m_uiUsedSerialAndId;

    CDeviceControl* m_pxDeviceControl;
    CDataStore* m_pxDataStore;
    CConfiguration* m_pxConfigurationCreate;
    CServiceMarket* m_pxServiceMarket;

    uint8_t m_uiModbusReceipt;
    uint8_t m_uiModbusReset;
    uint8_t m_uiModbusBlockeded;
    uint8_t m_uiAinOffProcessOwnerIndex = 0;

};

//-------------------------------------------------------------------------------
#endif // CRESOURCES_H
