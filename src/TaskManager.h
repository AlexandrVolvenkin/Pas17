#ifndef CTASKMANAGER_H
#define CTASKMANAGER_H
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

class CTask;
class CTaskInterface;
class CResources;

//-------------------------------------------------------------------------------
class CTaskManager : public CTask//, public CDfa
{
public:
    CTaskManager();
    CTaskManager(CResources* pxResources);
    virtual ~CTaskManager();
//    using CTask::CTask;  // Наследование конструктора CTask

protected:

private:
};

#endif // CTASKMANAGER_H
