//-------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------------------
#include "Resources.h"
#include "DeviceControl.h"

//-------------------------------------------------------------------------------------------
CResources::CResources()
{
    //ctor
}

//-------------------------------------------------------------------------------------------
CResources::~CResources()
{
    //dtor
}

//-------------------------------------------------------------------------------------------
void CResources::SetCoils(uint8_t* puiPointer)
{
    m_puiCoils = puiPointer;
}

//-------------------------------------------------------------------------------------------
uint8_t* CResources::GetCoils(void)
{
    return m_puiCoils;
}

//-------------------------------------------------------------------------------------------
void CResources::SetCoilsNumber(uint16_t uiData)
{
    m_uiCoilsNumber = uiData;
}

//-------------------------------------------------------------------------------------------
uint16_t CResources::GetCoilsNumber(void)
{
    return m_uiCoilsNumber;
}


//-------------------------------------------------------------------------------------------
void CResources::SetDiscreteInputs(uint8_t* puiPointer)
{
    m_puiDiscreteInputs = puiPointer;
}

//-------------------------------------------------------------------------------------------
uint8_t* CResources::GetDiscreteInputs(void)
{
    return m_puiDiscreteInputs;
}

//-------------------------------------------------------------------------------------------
void CResources::SetDiscreteInputsNumber(uint16_t uiData)
{
    m_uiDiscreteInputsNumber = uiData;
}

//-------------------------------------------------------------------------------------------
uint16_t CResources::GetDiscreteInputsNumber(void)
{
    return m_uiDiscreteInputsNumber;
}


//-------------------------------------------------------------------------------------------
void CResources::SetHoldingRegisters(uint16_t* puiPointer)
{
    m_puiHoldingRegisters = puiPointer;
}

//-------------------------------------------------------------------------------------------
uint16_t* CResources::GetHoldingRegisters(void)
{
    return m_puiHoldingRegisters;
}

//-------------------------------------------------------------------------------------------
void CResources::SetHoldingRegistersNumber(uint16_t uiData)
{
    m_uiHoldingRegistersNumber = uiData;
}

//-------------------------------------------------------------------------------------------
uint16_t CResources::GetHoldingRegistersNumber(void)
{
    return m_uiHoldingRegistersNumber;
}


//-------------------------------------------------------------------------------------------
void CResources::SetInputRegisters(uint16_t* puiPointer)
{
    m_puiInputRegisters = puiPointer;
}

//-------------------------------------------------------------------------------------------
uint16_t* CResources::GetInputRegisters(void)
{
    return m_puiInputRegisters;
}

//-------------------------------------------------------------------------------------------
void CResources::SetInputRegistersNumber(uint16_t uiData)
{
    m_uiInputRegistersNumber = uiData;
}

//-------------------------------------------------------------------------------------------
uint16_t CResources::GetInputRegistersNumber(void)
{
    return m_uiInputRegistersNumber;
}

//-------------------------------------------------------------------------------------------
