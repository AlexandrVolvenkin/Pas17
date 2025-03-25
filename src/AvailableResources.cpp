
//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#include <iostream>

#include "PasNewConfig.h"
#include "AvailableResources.h"

//-----------------------------------------------------------------------------------------------------
CAvailableResources::CAvailableResources()
{

}
//-----------------------------------------------------------------------------------------------------
CAvailableResources::~CAvailableResources()
{

}
//-----------------------------------------------------------------------------------------------------
void CAvailableResources::Initialize(void)
{
	InitializeAnalogInputValuesPointer(afAnalogueInputData);
	InitializeCalculatedValuesPointer(afCalcValue);
}
