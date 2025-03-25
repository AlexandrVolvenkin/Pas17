
//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#ifndef CMAINPRODUCTIONCYCLE_H
#define CMAINPRODUCTIONCYCLE_H

#include <iostream>
#include <string.h>
#include <list>
#include <vector>

#include "Dfa.h"
#include "DataStore.h"
#include "SystemComponent.h"
#include "SystemComponentsGroup.h"
#include "SystemSpecificationRequirementComponent.h"
#include "AvailableResources.h"
#include "FunctionalBlocks/MathematicalFunctionalBlocks/MathematicalFunctionalBlocks.h"

//-----------------------------------------------------------------------------------------------------
class CMainProductionCycle : public CDfa
{
public:
    enum
    {
        IDDLE = 0,
        DATA_STORE_CHECK,
        SYSTEM_INITIALIZATION,
        SYSTEM_RE_INITIALIZATION,
        MAIN_TASK_INITIALIZATION,
        MAIN_TASK_COMPONENTS_INFORMATION_COLLECTION,
        SYSTEM_SPECIFICATION_CHECK,
        MAIN_TASK_INITIALIZATION_END,
        MAIN_TASK_DISPLAY_INITIALIZATION_WAIT,

        MAIN_CYCLE_BOOT_SOURCE_CHECK,
        MAIN_CYCLE_DISK_COPY_DISPLAY_READY_WAITING,
        MAIN_CYCLE_DISK_COPY_END_WAITING,
        MAIN_CYCLE_DISK_COPY_END_REBOOT_WAITING,

        MAIN_CYCLE_START_WAITING,
        MAIN_CYCLE_MODULES_INTERACTION,
        MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING,
        MAIN_CYCLE_END,

        EMERGENCY_CYCLE,

        PROGRAMMING_CYCLE_START,
        PROGRAMMING_CYCLE,
        PROGRAMMING_CYCLE_MODULES_INTERACTION,
        PROGRAMMING_CYCLE_EVENT_WRITE_END_WAIT,
        PROGRAMMING_CYCLE_END,
    };

    CMainProductionCycle();
    virtual ~CMainProductionCycle();

    bool IsDataStoreOk(void);
    void GlobalVariablesInitInitialization(void);
    void PlatformComponentsInitialization(void);
    void MainTaskInitialization(void);
    void PlatformComponentsGathering(void);
    void MainTaskComponentsInformationCollection(void);
    bool IsSpecificationCorrect(void);
    bool IsDataBaseCorrect(void);
    bool IsConfigurationCorrect(void);
    void MainTaskInitializationEnd(void);
    void Fsm(void);
    void Run(void);

protected:
private:
    CDataStore* pxDataStoreFileSystem;
    CSystemComponentsGroupInterface* m_pxInternalModulesGroup;
    vector<CSystemSpecificationRequirementComponentInterface> m_vxInternalModulesSpecificationIn;
//    vector<CSystemComponentInterface> m_vxInternalModules;

	CAvailableResources* m_pxAvailableResources;
	CMathematicalFunctionalBlockManager* m_pxMathematicalFunctionalBlockManager;

    CTimer m_xTimer;
};

//-----------------------------------------------------------------------------------------------------
#endif // CMAINPRODUCTIONCYCLE_H

//-----------------------------------------------------------------------------------------------------
