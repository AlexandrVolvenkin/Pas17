//-----------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------
#ifndef CDFA_H
#define CDFA_H

#include <stdint.h>
#include "Timer.h"

//-----------------------------------------------------------------------------------------
class CDfa
{
public:
    CDfa();
    virtual ~CDfa();
    virtual uint8_t Fsm(void);
    uint8_t GetFsmState(void)
    {
        return m_uiFsmState;
    };

//protected:
    void SetFsmState(uint8_t uiData)
    {
        m_uiFsmState = uiData;
    };

    uint8_t m_uiFsmState;
};


//-----------------------------------------------------------------------------------------
#endif // CDFA_H
