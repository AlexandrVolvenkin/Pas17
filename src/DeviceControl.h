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
class CLink;
class CLinkInterface;

//-------------------------------------------------------------------------------
class CDeviceControl : public CTask
{
public:
    enum
    {
        DATA_BASE_BLOCK_READ = NEXT_STEP,
    };

    CDeviceControl();
    virtual ~CDeviceControl();

    // Геттер для m_pxCommandDataLink
    CLinkInterface* GetCommandDataLink() const
    {
        return m_pxCommandDataLink;
    }

    // Сеттер для m_pxCommandDataLink
    void SetCommandDataLink(CLinkInterface* pxValue)
    {
        m_pxCommandDataLink = pxValue;
    }

    // Геттер для m_pxOperatingDataLink
    CLinkInterface* GetOperatingDataLink() const
    {
        return m_pxOperatingDataLink;
    }

    // Сеттер для m_pxOperatingDataLink
    void SetOperatingDataLink(CLinkInterface* pxValue)
    {
        m_pxOperatingDataLink = pxValue;
    }

    uint16_t ConfigurationRead(uint8_t *puiDestination);
    uint16_t DataBaseBlockRead(uint8_t *puiDestination, uint8_t uiBlockIndex);
    uint16_t DataBaseBlockWrite(uint8_t *puiSource, uint8_t uiBlockIndex);
    uint8_t GetFsmOperationStatus(void);

    uint8_t Fsm(void);

protected:
    CLinkInterface* m_pxCommandDataLink;
    CLinkInterface* m_pxOperatingDataLink;
private:
};


//-------------------------------------------------------------------------------
#endif // CDEVICECONTROL_H
