#ifndef ASYNCHRONOUSTASK_H
#define ASYNCHRONOUSTASK_H
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <time.h>


class Timer;
class Platform;
class CTask;
class CResources;
class CCommunicationDevice;
class CCommunicationDeviceInterface;
class CSpiCommunicationDevice;
class CDataContainerInterface;
class CDataContainerDataBase;
class CInternalModule;
class CConfigurationCreate;
class CDeviceControl;
class CDataBaseCreate;
class CConfigurationCheck;
class CNewTask;


//-------------------------------------------------------------------------------
class CAsynchronousTask : public CNewTask
{
public:
    enum
    {
        ASYNCHRONOUS_TASK_EXECUTOR_READY_CHECK_START = NEXT_STEP,
        ASYNCHRONOUS_TASK_EXECUTOR_READY_CHECK_WAITING,
        ASYNCHRONOUS_TASK_EXECUTOR_DONE_CHECK_START,
        ASYNCHRONOUS_TASK_EXECUTOR_DONE_CHECK_WAITING,
    };

    CAsynchronousTask();
    virtual ~CAsynchronousTask();

    uint8_t Init(void);
    uint8_t Fsm(void);

private:

private:
    uint8_t* m_puiIntermediateBuff;

};

#endif // ASYNCHRONOUSTASK_H
//-------------------------------------------------------------------------------
