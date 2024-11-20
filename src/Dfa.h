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

#include "Timer.h"
//class CTimer;

//-------------------------------------------------------------------------------
class CDfa
{
public:
    enum EOperationStatus
    {
        IN_PROGRESS = 1,
        DONE_SUCCESSFULLY,
        ERROR_OCCURED,
    };

    enum EFsmStates
    {
        IDDLE = 1,
        STOP,
        START,
        INIT,
        READY,
        DONE_OK,
        DONE_ERROR,
        NEXT_STEP,
    };

    CDfa();
    virtual ~CDfa();
    virtual uint8_t Fsm(void);
    virtual uint8_t GetFsmState(void);
    virtual void SetFsmState(uint8_t uiData);
    virtual void SetFsmCommandState(uint8_t uiData);
    virtual uint8_t GetFsmCommandState(void);
    virtual void SetFsmOperationStatus(uint8_t uiData);
    virtual uint8_t GetFsmOperationStatus(void);
    virtual CTimer* GetTimerPointer(void);
    virtual bool IsDoneOk(void);
    virtual bool IsDoneError(void);

protected:
    uint8_t m_uiFsmState;
    uint8_t m_uiFsmCommandState;
    uint8_t m_uiFsmOperationStatus;
    CTimer m_xTimer;
};

//-------------------------------------------------------------------------------
#endif // CDFA_H
