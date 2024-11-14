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
#include <string>

//#include "Dfa.h"
#include "Task.h"

class CDfa;
//class CTaskInterface;
//class CTask;
class CDeviceControl;
class CDataStore;
class CConfigurationCreate;
class CServiceMarket;

//-------------------------------------------------------------------------------
class CResourcesInterface : public CTask
{
public:
    virtual void AddCommonListTask(CTaskInterface* pxTask) {};
    virtual CTaskInterface* GetCommonListTaskPointer(char* pcTaskName) {};
    virtual void AddCommonTaskToMap(std::string sTaskName, CTaskInterface* pxTask) {};
    virtual CTaskInterface* GetCommonTaskFromMapPointer(std::string sTaskName) {};
    virtual std::list<CTaskInterface*>* GetCommonTasksListPointer(void) {};

};

//-------------------------------------------------------------------------------










//-------------------------------------------------------------------------------
class CResources : public CResourcesInterface
{
public:
    CResources();
    virtual ~CResources();

    void AddCommonListTask(CTaskInterface* pxTask);
    void AddCurrentlyRunningTasksList(CTaskInterface* pxTask);
    CTaskInterface* GetCommonListTaskPointer(char* pcTaskName);
    void AddCommonTaskToMap(std::string sTaskName, CTaskInterface* pxTask);
    CTaskInterface* GetCommonTaskFromMapPointer(std::string sTaskName);

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

//    std::list<CTaskInterface*>* GetCommonTasksListPointer(void);
//    std::list<CTaskInterface*>* GetCurrentlyRunningTasksListPointer(void);

//protected:
//
//private:
    std::list<CTaskInterface*> m_lpxCommonTasksList;
    std::list<CTaskInterface*>::iterator m_xCommonTasksListIterator;
    std::list<CTaskInterface*> m_lpxCurrentlyRunningTasksList;
    std::list<CTaskInterface*>::iterator m_xCurrentlyRunningTasksListIterator;

    // Создаем std::map, где ключ - строка, значение - указатель на объект
    std::map<std::string, CTaskInterface*> m_mpxCommonTaskMap;
    std::string m_sTaskName;

    uint8_t m_uiAddress;
    uint8_t *m_puiRxBuffer;
    uint8_t *m_puiTxBuffer;
    uint8_t *m_puiErrorCode;
//    uint8_t *m_puiDiscreteOutputState;
//    uint8_t m_uiUsedDiscreteOutputState;
//    TDiscreteOutputControl *m_pxDiscreteOutputControl;
//    uint8_t m_uiUsedDiscreteOutputs;
//    TOutputData *m_pxDiscreteOutputDataBase;
////    uint8_t *m_puiExternalReceiptAddress;
////    uint8_t *m_puiExternalResetAddress;
//    uint8_t m_uiUsedDiscreteOutputDataBase;
//    uint8_t* m_puiErrorAlarmDataArray;
//    uint8_t m_uiUsedErrorAlarmDataArray;

    uint8_t *m_puiCoils;

    uint8_t *m_puiDiscreteInputs;
    uint8_t m_uiUsedDiscreteInputs;

    uint16_t *m_puiHoldingRegisters;
    uint16_t *m_puiInputRegisters;
    uint16_t m_uiCoilsNumber;
    uint16_t m_uiDiscreteInputsNumber;
    uint16_t m_uiHoldingRegistersNumber;
    uint16_t m_uiInputRegistersNumber;

    CDeviceControl* m_pxDeviceControl;
    CDataStore* m_pxDataStore;
    CConfigurationCreate* m_pxConfigurationCreate;
    CServiceMarket* m_pxServiceMarket;
};

//-------------------------------------------------------------------------------
#endif // CRESOURCES_H
