#ifndef CCONFIGURATIONCREATE_H
#define CCONFIGURATIONCREATE_H
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


class CPlatform;
class CTimer;
class CTask;
class CResources;

//-------------------------------------------------------------------------------
class CConfigurationCreate : public CTask//, public CDfa
{
    public:
        CConfigurationCreate();
        virtual ~CConfigurationCreate();

//    void SetResources(CResources* pxResources);
//    CResources* GetResources(void);

private:
//    CResources* m_pxResources;
};

#endif // CCONFIGURATIONCREATE_H
//-------------------------------------------------------------------------------
