//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef TIMERS_H_INCLUDED
#define TIMERS_H_INCLUDED

#include <stdint.h>

#include "Platform.h"

//class CTimeMeasure
//-------------------------------------------------------------------------------
class CTimer// : public CTimeMeasure
{
public:
//-------------------------------------------------------------------------------
    CTimer();
    CTimer(uint16_t uiTime);
    virtual ~CTimer();
    void Set(uint16_t uiTime);
    void Reset(void);
    bool IsOverflow(void);

//-------------------------------------------------------------------------------
private:
    uint16_t m_uiTime;
    uint16_t m_uiLastSystemTick;
};
//-------------------------------------------------------------------------------








//-------------------------------------------------------------------------------
class CTimeMeasure
{
public:
    typedef enum
    {
        STORED_MEASURE_NUMBER = 36000,
        TIME_MEASURE_COUNTER_NUMBER = 100
    };

    CTimeMeasure();
    ~CTimeMeasure();

    uint8_t Begin(void);
    uint32_t End(void);
    void Store(void);
//    uint8_t LastMoreThan(int32_t );



private:
//    ofstream outdata; // outdata is like cin
//    const char* pccOutDataFileName;
    struct timeval  xTimeCur;
    struct timeval xTimeLast;
    struct timeval  xTimeMax;
    struct timeval  xTimeDelta;
//    int iTimeMeasureCounter;
//    int iMeasureCounter;
//    int aiTimeMeasureAverage[CTimeMeasure::TIME_MEASURE_COUNTER_NUMBER];


};

//-------------------------------------------------------------------------------
#endif // TIMERS_H_INCLUDED
