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
class CAnalogueSignals;
class CDataContainerInterface;
class CDataContainerDataBase;
class CConfigurationCreate;

//-------------------------------------------------------------------------------
class CDeviceControl : public CTask
{
public:
    enum
    {
        CONFIGURATION_REQUEST_START = NEXT_STEP,
//        CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_START,
//        CONFIGURATION_REQUEST_EXECUTOR_READY_CHECK_WAITING,
//        CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_START,
//        CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING,
        CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING,

        DATA_BASE_BLOCK_READ,
        DATA_BASE_BLOCK_START_WRITE,
        DATA_BASE_BLOCK_WRITE_END_WAITING,
    };

    CDeviceControl();
    virtual ~CDeviceControl();

//    size_t GetObjectLength(void);

    // Геттер для m_sDataStoreName
    std::string GetDataStoreName() const
    {
        return m_sDataStoreName;
    }

    // Сеттер для m_sDataStoreName
    void SetDataStoreName(const std::string& sName)
    {
        m_sDataStoreName = sName;
    }

    // Геттер для m_pxDataStore
    CDataStore* GetDataStore() const
    {
        return m_pxDataStore;
    }

    // Сеттер для m_pxDataStore
    void SetDataStore(CDataStore* pxDataStore)
    {
        m_pxDataStore = pxDataStore;
    }

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

    void SetConfigurationCreateName(const std::string& sName)
    {
        m_sConfigurationCreateName = sName;
    }

    uint8_t Init(void);
//    bool SetTaskData(CDataContainerDataBase* pxDataContainer);
//    bool GetTaskData(CDataContainerDataBase* pxDataContainer);
//    CDataContainerDataBase* GetTaskData(void);

    uint16_t ConfigurationRead(uint8_t *puiDestination);
    uint16_t DataBaseBlockRead(uint8_t *puiDestination, uint8_t uiBlockIndex);
    uint16_t DataBaseBlockReadAnswer(void);
    uint16_t DataBaseBlockWrite(void);
    uint8_t GetFsmOperationStatus(void);

    uint8_t Fsm(void);

protected:
    std::string m_sDataStoreName;
    CDataStore* m_pxDataStore;

    std::string m_sConfigurationCreateName;
    uint8_t m_uiConfigurationCreateId;

    std::string m_sDataStoreLinkName;
    CLinkInterface* m_pxDataStoreLink;

    CLinkInterface* m_pxCommandDataLink;
    CLinkInterface* m_pxOperatingDataLink;

    uint8_t* m_puiIntermediateBuff;

    CDataContainerDataBase* m_pxCommandDataContainer;
    CDataContainerDataBase* m_pxOperatingDataContainer;
private:
};


//-------------------------------------------------------------------------------
#endif // CDEVICECONTROL_H
