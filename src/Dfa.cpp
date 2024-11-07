//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include "Timer.h"
#include "Dfa.h"

//-------------------------------------------------------------------------------
CDfa::CDfa()
{
    m_uiFsmState = 0;
//    m_uiFsmNextState = 0;
}

//-------------------------------------------------------------------------------
CDfa::~CDfa()
{
    //dtor
}

//-------------------------------------------------------------------------------
uint8_t CDfa::Fsm(void)
{

}

//-------------------------------------------------------------------------------
uint8_t CDfa::GetFsmState(void)
{
    return m_uiFsmState;
}

//-------------------------------------------------------------------------------
void CDfa::SetFsmState(uint8_t uiData)
{
    m_uiFsmState = uiData;
}

//-------------------------------------------------------------------------------
void CDfa::SetFsmCommandState(uint8_t uiData)
{
    m_uiFsmCommandState = uiData;
}

//-------------------------------------------------------------------------------
CTimer* CDfa::GetTimerPointer(void)
{
    return &m_xTimer;
}




