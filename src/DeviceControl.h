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
class CDataStore;
class CLink;
class CLinkInterface;

//-------------------------------------------------------------------------------
class CDeviceControl : public CTask
{
public:
    enum
    {
        DATA_BASE_BLOCK_READ = NEXT_STEP,
        DATA_BASE_BLOCK_START_WRITE,
        DATA_BASE_BLOCK_WRITE_END_WAITING,
    };

    CDeviceControl();
    virtual ~CDeviceControl();

    // Геттер для m_sDataStoreLinkName
    std::string GetDataStoreLinkName() const
    {
        return m_sDataStoreLinkName;
    }

    // Сеттер для m_sDataStoreLinkName
    void SetDataStoreLinkName(const std::string& sName)
    {
        m_sDataStoreLinkName = sName;
    }

    // Геттер для m_pxDataStoreLink
    CLinkInterface* GetDataStoreLink() const
    {
        return m_pxDataStoreLink;
    }

    // Сеттер для m_pxDataStoreLink
    void SetDataStoreLink(CLinkInterface* pxLink)
    {
        m_pxDataStoreLink = pxLink;
    }

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
    std::string m_sDataStoreLinkName;
    CLinkInterface* m_pxDataStoreLink;

    CLinkInterface* m_pxCommandDataLink;
    CLinkInterface* m_pxOperatingDataLink;
private:
};


//-------------------------------------------------------------------------------
#endif // CDEVICECONTROL_H
