#ifndef CDEVICECONTROL_H
#define CDEVICECONTROL_H
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

class Timer;
class Platform;
class CTask;
class CResources;

//-------------------------------------------------------------------------------
class CDeviceControl : public CTask//, public CDfa
{
public:
    CDeviceControl();
    virtual ~CDeviceControl();

//    void SetResources(CResources* pxResources);
//    CResources* GetResources(void);
    uint16_t ConfigurationRead(uint8_t *puiDestination);
    uint16_t DataBaseBlockRead(uint8_t *puiDestination, uint8_t uiBlockIndex);
    uint16_t DataBaseBlockWrite(uint8_t *puiSource, uint8_t uiBlockIndex);

protected:

private:
//    CResources* m_pxResources;
};


//-------------------------------------------------------------------------------
#endif // CDEVICECONTROL_H
