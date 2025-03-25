//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#ifndef CSYSTEMCOMPONENTSGROUP_H
#define CSYSTEMCOMPONENTSGROUP_H

#include <stdint.h>
#include <iostream>
#include <vector>

#include "Platform.h"
#include "PasNewConfig.h"
#include "SystemComponent.h"
#include "SystemSpecificationRequirementComponent.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
class CSystemComponentsGroupInterface
{
public:
    virtual void SetCommunicationDevice(CCommunicationDeviceInterface* pxCommunicationDevice) {};
    virtual void AddSystemComponent(CSystemComponentInterface* pxSystemComponent) {};
    virtual void AddSystemComponent(uint8_t uiType, uint8_t uiAddress) {};
    virtual void Gathering(void) {};
    virtual void Muster(void) {};
    virtual bool IsSequenceSlotOk(void) {};
    virtual bool IsConfigurationMatch(void) {};
    virtual void NewConfigurationAndDataBaseCreate(void) {};
    virtual void MrXXDataBaseCreate(void) {};
    virtual bool IsDiscreteAndAnalogueModuleQuantityMatch(void) {};
    virtual bool IsReadyToStartWork(void) {};
    virtual bool IsSpecificationCorrect(void) {};
};

//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CSystemComponentsGroupInternalModules : public CSystemComponentsGroupInterface
{
public:
    CSystemComponentsGroupInternalModules();
//        CSystemComponentsGroupInternalModules(const CSystemComponentsGroupInternalModules& other);
    virtual ~CSystemComponentsGroupInternalModules();

    void SetCommunicationDevice(CCommunicationDeviceInterface* pxCommunicationDevice)
    {
        m_pxCommunicationDevice = pxCommunicationDevice;
    }
    void AddSystemComponent(CSystemComponentInterface* pxSystemComponent);
    void AddSystemComponent(uint8_t uiType, uint8_t uiAddress);
    void Gathering(void);
    void Muster(void);
    bool IsSequenceSlotOk(void);
    bool IsConfigurationMatch(void);
    void NewConfigurationAndDataBaseCreate(void);
    void MrXXDataBaseCreate(void);
    bool IsDiscreteAndAnalogueModuleQuantityMatch(void);
    bool IsReadyToStartWork(void);
    bool IsSpecificationCorrect(void);

private:
    CCommunicationDeviceInterface* m_pxCommunicationDevice;
    uint8_t m_uiComponentsNumber;
//    vector<CSystemComponentInterface*> m_vpxSystemComponents;
    CSystemComponentInterface* m_apxSystemComponents[INTERNAL_MODULE_QUANTITY];
};

//-----------------------------------------------------------------------------------------------------
#endif // CSYSTEMCOMPONENTSGROUP_H
