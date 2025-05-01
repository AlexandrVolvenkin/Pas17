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
        CONFIGURATION_REQUEST_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        CONFIGURATION_REQUEST_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        ONLINE_DATA_READ_START,
        ONLINE_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        ONLINE_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        DATA_BASE_BLOCK_READ,
        DATA_BASE_BLOCK_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DATA_BASE_BLOCK_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_START,
        DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_START,
        DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,



        DATA_BASE_BLOCK_WRITE_START,
        DATA_BASE_BLOCK_WRITE_END_WAITING,
        DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,
        DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_START,
        DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        DATA_BASE_BLOCK_MODULE_MUVR_WRITE_START,
        DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,
        DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_START,
        DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,
    };

    CDeviceControl();
    virtual ~CDeviceControl();

//    size_t GetObjectLength(void);

    // ������ ��� m_sDataStoreName
    std::string GetDataStoreName() const
    {
        return m_sDataStoreName;
    }

    // ������ ��� m_sDataStoreName
    void SetDataStoreName(const std::string& sName)
    {
        m_sDataStoreName = sName;
    }

    // ������ ��� m_pxDataStore
    CDataStore* GetDataStore() const
    {
        return m_pxDataStore;
    }

    // ������ ��� m_pxDataStore
    void SetDataStore(CDataStore* pxDataStore)
    {
        m_pxDataStore = pxDataStore;
    }

    // ������ ��� m_sDataStoreLinkName
    std::string GetDataStoreLinkName() const
    {
        return m_sDataStoreLinkName;
    }

    // ������ ��� m_sDataStoreLinkName
    void SetDataStoreLinkName(const std::string& sName)
    {
        m_sDataStoreLinkName = sName;
    }

    // ������ ��� m_pxDataStoreLink
    CLinkInterface* GetDataStoreLink() const
    {
        return m_pxDataStoreLink;
    }

    // ������ ��� m_pxDataStoreLink
    void SetDataStoreLink(CLinkInterface* pxLink)
    {
        m_pxDataStoreLink = pxLink;
    }

    // ������ ��� m_pxCommandDataLink
    CLinkInterface* GetCommandDataLink() const
    {
        return m_pxCommandDataLink;
    }

    // ������ ��� m_pxCommandDataLink
    void SetCommandDataLink(CLinkInterface* pxValue)
    {
        m_pxCommandDataLink = pxValue;
    }

    // ������ ��� m_pxOperatingDataLink
    CLinkInterface* GetOperatingDataLink() const
    {
        return m_pxOperatingDataLink;
    }

    // ������ ��� m_pxOperatingDataLink
    void SetOperatingDataLink(CLinkInterface* pxValue)
    {
        m_pxOperatingDataLink = pxValue;
    }

    void SetConfigurationCreateName(const std::string& sName)
    {
        m_sConfigurationCreateName = sName;
    }
    void SetInternalModuleMuvrName(std::string sName);

    uint8_t Init(void);
    void OnlineDataRead(void);
    uint16_t DataBaseBlockReadBlockRelatedAction(void);
    uint16_t DataBaseBlockWriteBlockRelatedAction(void);

    uint8_t Fsm(void);

protected:
    std::string m_sDataStoreName;
    CDataStore* m_pxDataStore;
    uint8_t m_uiDataStoreId;

    std::string m_sConfigurationCreateName;
    uint8_t m_uiConfigurationCreateId;

    std::string m_sInternalModuleMuvrName;
    uint8_t m_uiInternalModuleMuvrId;

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
