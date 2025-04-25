//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include "Platform.h"
#include "Timer.h"

CTimeMeasure xTimeMeasure;
//-------------------------------------------------------------------------------
CTimer::CTimer()
{

}

//-------------------------------------------------------------------------------
CTimer::CTimer(uint16_t uiTime)
{
    m_uiLastSystemTick = GetCurrentTime();
    m_uiTime = uiTime;
}

//-------------------------------------------------------------------------------
CTimer::~CTimer()
{

}

//-------------------------------------------------------------------------------
uint16_t CTimer::GetCurrentTime(void)
{
    struct timeval xCurrentTime;

    gettimeofday( &xCurrentTime, NULL );

    return static_cast<uint16_t>(((xCurrentTime.tv_sec * 1000) +
                                  (xCurrentTime.tv_usec / 1000)));
}

//-------------------------------------------------------------------------------
void CTimer::Set(uint16_t uiTime)
{
    m_uiLastSystemTick = GetCurrentTime();
    m_uiTime = uiTime;
}

//-------------------------------------------------------------------------------
void CTimer::Reset(void)
{
    m_uiLastSystemTick = GetCurrentTime();
}

//-------------------------------------------------------------------------------
bool CTimer::IsOverflow(void)
{
    return ((uint16_t)(GetCurrentTime() - (uint16_t)m_uiLastSystemTick)  >=
            m_uiTime);
}

//-------------------------------------------------------------------------------







//-------------------------------------------------------------------------------
//timer_t timerid;
//timer_t timerid2;
//struct sigevent sev;
//struct itimerspec its;
//sigset_t mask;
//struct sigaction sa;
//
//
//CTimeMeasure xCTimeMeasure;
//CTimeMeasure xProgrammTimeMeasure;

//-------------------------------------------------------------------------------
CTimeMeasure::CTimeMeasure()
{
//    pccOutDataFileName = "PasTimeMeasure.csv";
//    iMeasureCounter = 0;
//    CRingBuffer xCMainCycleMeasureRingBuffer;
}

//-------------------------------------------------------------------------------
CTimeMeasure::~CTimeMeasure()
{

}

//-------------------------------------------------------------------------------
uint8_t CTimeMeasure::Begin(void)
{
    gettimeofday( &xTimeLast, NULL );
    if( gettimeofday( &xTimeLast, NULL ) != 0 )
    {
        /* gettimeofday failed - retry next time. */
        xTimeLast.tv_usec = 0;
        return 0;
    }
    else
    {
        return 1;
    }
}

//-------------------------------------------------------------------------------
uint32_t CTimeMeasure::End(void)
{
    if( gettimeofday( &xTimeCur, NULL ) != 0 )
    {
        /* gettimeofday failed - retry next time. */
        xTimeCur.tv_usec = 0;
        return 0;
    }
    else
    {
//        Store();
        if (xTimeCur.tv_usec &&
                xTimeLast.tv_usec &&
                ((uint32_t)xTimeCur.tv_usec - (uint32_t)xTimeLast.tv_usec))
        {
            uint32_t uiTime = ((uint32_t)((uint32_t)xTimeCur.tv_usec - (uint32_t)xTimeLast.tv_usec));
            if (uiTime > 1000000000)
            {
                return 0;
            }
            else
            {
                return uiTime;
            }
//            return ((uint32_t)((uint32_t)xTimeCur.tv_usec - (uint32_t)xTimeLast.tv_usec));
        }
        else
        {
//        std::cout << "CTimeMeasure::End xTimeCur.tv_usec"  << (uint32_t)xTimeCur.tv_usec  << std::endl;
//        std::cout << "CTimeMeasure::End xTimeLast.tv_usec"  << (uint32_t)xTimeLast.tv_usec  << std::endl;
//        std::cout << "CTimeMeasure::End xTimeCur.tv_usec"  << (uint32_t)xTimeCur.tv_usec  << std::endl;
            return 0;
        }
    }
}
//(uint16_t)(CPlatform::GetCurrentTime() - (uint16_t)m_uiLastSystemTick)
////-------------------------------------------------------------------------------
//uint8_t CTimeMeasure::LastMoreThan(int32_t ui32Seconds)
//{
//    gettimeofday( &xTimeCur, NULL );
//    if ((xTimeCur.tv_sec - xTimeLast.tv_sec) > ui32Seconds)
//    {
//        gettimeofday( &xTimeLast, NULL );
//        return 1;
//    }
//    else
//
//    {
//        gettimeofday( &xTimeLast, NULL );
//        return 0;
//    }
//}

//-------------------------------------------------------------------------------
void CTimeMeasure::Store(void)
{
    if (xTimeCur.tv_usec &&
            xTimeLast.tv_usec &&
            (xTimeCur.tv_usec > xTimeLast.tv_usec))
    {
//        xCMainCycleMeasureRingBuffer.Push((int32_t)(xTimeCur.tv_usec - xTimeLast.tv_usec));

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

//-------------------------------------------------------------------------------

