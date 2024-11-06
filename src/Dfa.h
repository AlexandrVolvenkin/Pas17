//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CDFA_H
#define CDFA_H

#include <stdint.h>

//#include "Timer.h"
//class CTimer;

//-------------------------------------------------------------------------------
class CDfa
{
public:
    CDfa();
    virtual ~CDfa();
    uint8_t Fsm(void) {};
    uint8_t GetFsmState(void) {};
    void SetFsmState(uint8_t uiData) {};
//    virtual CTimer* GetTimerPointer(void);

    uint8_t m_uiFsmState;
//    uint8_t m_uiFsmNextState;
    uint8_t m_uiFsmCommandState;
    uint8_t m_uiFsmAnswerState;
//    CTimer m_xTimer;
};

//-------------------------------------------------------------------------------





////-------------------------------------------------------------------------------
//class CDfa : public CDfaLite
//{
//public:
//    CDfa();
//    virtual ~CDfa();
////    uint8_t GetSavedFsmState(void)
////    {
////        return m_uiSavedFsmState;
////    };
////
////    uint8_t GetFsmEvent(void)
////    {
////        return m_uiFsmEvent;
////    };
////
//////protected:
////    void SetSavedFsmState(uint8_t uiData)
////    {
////        m_uiSavedFsmState = uiData;
////    };
////    void SetFsmEvent(uint8_t uiData)
////    {
////        m_uiFsmEvent = uiData;
////    };
//    virtual CTimer* GetTimerPointer(void);
//
////    uint8_t m_uiSavedFsmState;
////    uint8_t m_uiFsmEvent;
//    CTimer m_xTimer;
//};


//-------------------------------------------------------------------------------
#endif // CDFA_H
