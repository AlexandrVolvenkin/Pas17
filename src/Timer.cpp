
#include "PasNewConfig.h"
#include "Timer.h"
#include "RingBuffer.h"

#include "Platform.h"

using namespace std;


//-----------------------------------------------------------------------------------------------------
CTimer::CTimer()
{

}

//-----------------------------------------------------------------------------------------------------
CTimer::CTimer(uint16_t uiTime) :
    m_uiTime(uiTime)
{
    m_uiLastSystemTick = CPlatform::GetCurrentTime();
}

////-----------------------------------------------------------------------------------------------------
//CTimer::CTimer(const CTimer& xTimer)
//{
//    m_uiTime = xTimer.m_uiTime;
//    m_uiLastSystemTick = xTimer.m_uiLastSystemTick;
//}

//-----------------------------------------------------------------------------------------------------
CTimer::~CTimer()
{

}

//-----------------------------------------------------------------------------------------------------
void CTimer::Set(uint16_t uiTime)
{
    m_uiLastSystemTick = CPlatform::GetCurrentTime();
    m_uiTime = uiTime;
}

//-----------------------------------------------------------------------------------------------------
void CTimer::Reset(void)
{
    m_uiLastSystemTick = CPlatform::GetCurrentTime();
}

//-----------------------------------------------------------------------------------------------------
bool CTimer::IsOverflow(void)
{
    return ((CPlatform::GetCurrentTime() - m_uiLastSystemTick)  >=
            m_uiTime);
}

//-----------------------------------------------------------------------------------------------------







//-----------------------------------------------------------------------------------------------------
timer_t timerid;
timer_t timerid2;
struct sigevent sev;
struct itimerspec its;
sigset_t mask;
struct sigaction sa;


CTimeMeasure xCTimeMeasure;
CTimeMeasure xProgrammTimeMeasure;

////-----------------------------------------------------------------------------------------------------
//CTimeMeasure::CTimeMeasure()
//{
//    pccOutDataFileName = "PasTimeMeasure.csv";
//    iMeasureCounter = 0;
//    CRingBuffer xCMainCycleMeasureRingBuffer;
//}

//-----------------------------------------------------------------------------------------------------
void CTimeMeasure::Begin(void)
{
    gettimeofday( &xTimeLast, NULL );
    if( gettimeofday( &xTimeLast, NULL ) != 0 )
    {
        /* gettimeofday failed - retry next time. */
        xTimeLast.tv_usec = 0;
    }
}

//-----------------------------------------------------------------------------------------------------
void CTimeMeasure::End(void)
{
    if( gettimeofday( &xTimeCur, NULL ) != 0 )
    {
        /* gettimeofday failed - retry next time. */
        xTimeCur.tv_usec = 0;
    }
    else
    {
        Store();
    }
}

//-----------------------------------------------------------------------------------------------------
uint8_t CTimeMeasure::LastMoreThan(int32_t ui32Seconds)
{
    gettimeofday( &xTimeCur, NULL );
    if ((xTimeCur.tv_sec - xTimeLast.tv_sec) > ui32Seconds)
    {
        gettimeofday( &xTimeLast, NULL );
        return 1;
    }
    else

    {
        gettimeofday( &xTimeLast, NULL );
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
void CTimeMeasure::Store(void)
{
    if (xTimeCur.tv_usec &&
            xTimeLast.tv_usec &&
            (xTimeCur.tv_usec > xTimeLast.tv_usec))
    {
        xCMainCycleMeasureRingBuffer.Push((int32_t)(xTimeCur.tv_usec - xTimeLast.tv_usec));

//        if (iMeasureCounter < STORED_MEASURE_NUMBER)
//        {
//            iMeasureCounter++;
//            outdata.open(pccOutDataFileName, std::ios::app); // opens the file
//            if (!outdata)   // file couldn't be opened
//            {
//                cerr << "Error: file could not be opened" << endl;
//                exit(1);
//            }
//
//            outdata << (xTimeCur.tv_usec - xTimeLast.tv_usec) << endl;//std::setw(2) <<
//
//            outdata.close();
//        }
    }
}

//-----------------------------------------------------------------------------------------------------
int iTimerCreate(timer_t *timerid,
                 struct sigevent *sevp)
{
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    sevp -> sigev_notify = SIGEV_THREAD;//SIGEV_SIGNAL;//SIGEV_NONE;//
    //sevp -> sigev_signo = SIG;
    sevp -> sigev_notify_function = vHandler100Ms;
    sevp -> sigev_value.sival_ptr = timerid;
    //A value that's to be passed to the notification function.
    //sevp -> sigev_value.sival_int =20;
    sevp -> sigev_notify_attributes = &attr;
    if (timer_create(CLOCKID, sevp, timerid) == -1)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
int iTimerSetTime(timer_t *timerid,
                  struct itimerspec *new_value,
                  unsigned long ulTime)
{
    new_value -> it_value.tv_sec = ulTime / 1000000000;
    new_value -> it_value.tv_nsec = ulTime % 1000000000;
    new_value -> it_interval.tv_sec = 0;
    new_value -> it_interval.tv_nsec = 0;

    if (timer_settime(*timerid, 0, new_value, NULL) == -1)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
int iTimerRestart(timer_t *timerid,
                  struct itimerspec *new_value)
{
    if (timer_settime(*timerid, 0, new_value, NULL) == -1)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
int iHandlerCreate(int signum,
                   struct sigaction *act,
                   void (*handler)(int, siginfo_t *, void *))
{
    memset(act, 0, sizeof(struct sigaction));
    act -> sa_flags = SA_SIGINFO;
    act -> sa_sigaction = handler;
    sigemptyset(&(act -> sa_mask));
    if (sigaction(signum, act, NULL) == -1)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
int iHandlerBlock(int signum,
                  sigset_t *mask)
{
    sigemptyset(mask);
    sigaddset(mask, signum);
    if (sigprocmask(SIG_BLOCK, mask, NULL) == -1)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
int iHandlerUnlock(sigset_t *mask)
{
    /* Разблокируем сигнал таймера, чтобы доставлялись
       уведомления таймера */
    if (sigprocmask(SIG_UNBLOCK, mask, NULL) == -1)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
void vHandler100Ms(union sigval val)
{
    ucMainCycleTimeStart = 1;
}
