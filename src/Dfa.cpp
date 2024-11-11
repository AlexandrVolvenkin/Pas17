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
void CDfa::SetFsmState(uint8_t uiData)
{
    m_uiFsmState = uiData;
}

//-------------------------------------------------------------------------------
uint8_t CDfa::GetFsmState(void)
{
    return m_uiFsmState;
}

//-------------------------------------------------------------------------------
void CDfa::SetFsmCommandState(uint8_t uiData)
{
    m_uiFsmCommandState = uiData;
}

//-------------------------------------------------------------------------------
uint8_t CDfa::GetFsmCommandState(void)
{
    return m_uiFsmCommandState;
}

//-------------------------------------------------------------------------------
void CDfa::SetFsmAnswerState(uint8_t uiData)
{
    m_uiFsmAnswerState = uiData;
}

//-------------------------------------------------------------------------------
uint8_t CDfa::GetFsmAnswerState(void)
{
    return m_uiFsmAnswerState;
}

//-------------------------------------------------------------------------------
CTimer* CDfa::GetTimerPointer(void)
{
    return &m_xTimer;
}

//-------------------------------------------------------------------------------
bool CDfa::IsDoneOk(void)
{
    if (m_uiFsmAnswerState == DONE_OK)
    {
        SetFsmAnswerState(0);
        return true;
    }
    else
    {
        return false;
    }
}

//-------------------------------------------------------------------------------
bool CDfa::IsDoneError(void)
{
    if (m_uiFsmAnswerState == DONE_ERROR)
    {
        SetFsmAnswerState(0);
        return true;
    }
    else
    {
        return false;
    }
}




