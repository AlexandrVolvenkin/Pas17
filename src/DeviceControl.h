#ifndef CDEVICECONTROL_H
#define CDEVICECONTROL_H
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

#include "Platform.h"
//#include "Resources.h"
#include "Dfa.h"
#include "Timer.h"

class CResources;

//-------------------------------------------------------------------------------
class CDeviceControl : public CDfa
{
public:
    CDeviceControl();
    virtual ~CDeviceControl();

    void SetResources(CResources* pxResources);
    CResources* GetResources(void);
    uint16_t ConfigurationRead(uint8_t *puiDestination);
    uint16_t DataBaseBlockRead(uint8_t *puiDestination, uint8_t uiBlockIndex);
    uint16_t DataBaseBlockWrite(uint8_t *puiSource, uint8_t uiBlockIndex);

protected:

private:
    CResources* m_pxResources;
};


//-------------------------------------------------------------------------------
#endif // CDEVICECONTROL_H
