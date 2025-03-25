//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#ifndef CSYSTEMSPECIFICATION_H
#define CSYSTEMSPECIFICATION_H

#include <stdint.h>

//-----------------------------------------------------------------------------------------------------
#pragma pack(push)
#pragma pack(1)
struct  TSystemSpecificationRequirementComponent
{
    uint8_t uiType;
    uint8_t uiStatus;
    uint8_t uiAddress;
    uint8_t uiReservedAddress;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------------------------------









//-----------------------------------------------------------------------------------------------------
class CSystemSpecificationRequirementComponentInterface
{

};

//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CSystemSpecificationRequirementComponent : public CSystemSpecificationRequirementComponentInterface
{
public:
    CSystemSpecificationRequirementComponent();
    virtual ~CSystemSpecificationRequirementComponent();
protected:
private:
};

//-----------------------------------------------------------------------------------------------------
#endif // CSYSTEMSPECIFICATION_H
