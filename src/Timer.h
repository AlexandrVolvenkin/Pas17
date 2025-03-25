#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED


#include <stdint.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <signal.h>		/* for signal */
//#include "PasNewConfig.h"

using namespace std;

#define CLOCKID CLOCK_MONOTONIC//CLOCK_REALTIME
#define SIG (SIGRTMIN + 1)//SIGRTMAX//
#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

//-----------------------------------------------------------------------------------------------------
class CTimer
{
public:
//-----------------------------------------------------------------------------------------------------
    CTimer();
    CTimer(uint16_t uiTime);
//    CTimer(const CTimer& xTimer);
    virtual ~CTimer();

    void Set(uint16_t uiTime);
    void Reset(void);
    bool IsOverflow(void);

//-----------------------------------------------------------------------------------------------------
private:
    uint16_t m_uiTime;
    uint16_t m_uiLastSystemTick;
};

//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CTimeMeasure
{
public:
    typedef enum
    {
        STORED_MEASURE_NUMBER = 36000,
        TIME_MEASURE_COUNTER_NUMBER = 100
    };

    void Begin(void);
    void End(void);
    void Store(void);
    uint8_t LastMoreThan(int32_t );


    ofstream outdata; // outdata is like cin
    const char* pccOutDataFileName;
    struct timeval  xTimeCur;
    struct timeval xTimeLast;
    struct timeval  xTimeMax;
    struct timeval  xTimeDelta;
    int iTimeMeasureCounter;
    int iMeasureCounter;
    int aiTimeMeasureAverage[CTimeMeasure::TIME_MEASURE_COUNTER_NUMBER];

public:
    CTimeMeasure()
    {
        pccOutDataFileName = "PasTimeMeasure.csv";
        iMeasureCounter = 0;
    }

    ~CTimeMeasure()
    {

    }

};

extern CTimeMeasure xCTimeMeasure;
extern CTimeMeasure xProgrammTimeMeasure;

//-----------------------------------------------------------------------------------------------------
extern timer_t timerid;
extern timer_t timerid2;
extern struct sigevent sev;
extern struct itimerspec its;
extern sigset_t mask;
extern struct sigaction sa;
extern pthread_attr_t attr;

int iHandlerCreate(int signum,
                   struct sigaction *act,
                   void (*handler)(int, siginfo_t *, void *));
int iHandlerBlock(int signum,
                  sigset_t *mask);
int iHandlerUnlock(sigset_t *mask);
int iTimerCreate(timer_t *timerid,
                 struct sigevent *sevp);
int iTimerSetTime(timer_t *timerid,
                  struct itimerspec *new_value,
                  unsigned long ulTime);
int iTimerRestart(timer_t *timerid,
                  struct itimerspec *new_value);
//void vHandler100Ms(int sig, siginfo_t *si, void *uc);
void vHandler100Ms(union sigval val);
#endif // TIMER_H_INCLUDED
