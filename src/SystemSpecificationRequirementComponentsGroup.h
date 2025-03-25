//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#ifndef CSYSTEMSPECIFICATIONGROUP_H
#define CSYSTEMSPECIFICATIONGROUP_H

#include <stdint.h>

#include "PasNewConfig.h"
#include "Platform.h"
#include "SystemSpecificationRequirementComponent.h"

//-----------------------------------------------------------------------------------------------------
class CSystemSpecificationRequirementComponentsGroupInterface
{
public:

};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CSystemSpecificationRequirementComponentsGroup : public CSystemSpecificationRequirementComponentsGroupInterface
{
public:
    CSystemSpecificationRequirementComponentsGroup();
    virtual ~CSystemSpecificationRequirementComponentsGroup();

protected:
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CSystemSpecificationRequirementComponentsGroupInternalModules : public CSystemSpecificationRequirementComponentsGroup
{
public:
    CSystemSpecificationRequirementComponentsGroupInternalModules();
    virtual ~CSystemSpecificationRequirementComponentsGroupInternalModules();

private:
    TSystemSpecificationRequirementComponent axSystemSpecificationRequirementComponents[INTERNAL_MODULE_QUANTITY];
};
//-----------------------------------------------------------------------------------------------------
#endif // CSYSTEMSPECIFICATIONGROUP_H
