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
// ������� ��� ��������� ������� ������������ ����� ��������.
// ��������� ������� - SP, OUT.
#define DEVICE_CONTROL_MTVI5_INCREMENT_SP_OUT  0x01
// ��������� ������� - SP, OUT.
#define DEVICE_CONTROL_MTVI5_DECREMENT_SP_OUT  0x02
// �������� ����-������� � ������ ��������� ����������� �CONT_ST�.
#define DEVICE_CONTROL_MTVI5_CONT_ST_WRITE  0x04
// �������� ��������� � ������ ���������� ���������� ����������� �CONT_AV�.
#define DEVICE_CONTROL_MTVI5_CONT_AV_WRITE  0x05
#define DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_PROGRAMMER  0x06
#define DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_HMI  0x07
#define DEVICE_CONTROL_MTVI5_LOAD_OUTS_OSF  0x08
#define DEVICE_CONTROL_MTVI5_CONT_ST_CHANGE  0x09

// ������-������� modbus ������� 5.
// ���������-���������� ������ ����������.
#define DEVICE_CONTROL_CALIBRATION_ON_OFF 0x1000
// ���������� ������ �����.
#define DEVICE_CONTROL_SET_BOTTOM_OF_SCALE  0x1100
// ���������� ����� �����.
#define DEVICE_CONTROL_SET_TOP_OF_SCALE  0x1200
// ���������-���������� ������ ����������.
#define DEVICE_CONTROL_BLOCK 0x007D
// ������������ � �������� ������.
#define DEVICE_CONTROL_PC_KVIT  0x007E
// ����� � �������� ������.
#define DEVICE_CONTROL_PC_RESET 0x007F
// ���������� ���� � ��������� ��������� �������.
#define CURRENT_TIME_BYTE_QUANTITY 7

//-------------------------------------------------------------------------------
class CDeviceControl : public CTask
{
public:
    enum
    {
        CONFIGURATION_REQUEST_START = NEXT_STEP,
        CONFIGURATION_REQUEST_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        CONFIGURATION_REQUEST_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        STATE_DATA_READ_START,
        STATE_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        STATE_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        ANALOGUE_MEASURE_ARCHIVE_WRITE_START,
        ANALOGUE_MEASURE_ARCHIVE_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        ANALOGUE_MEASURE_ARCHIVE_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST_START,
        ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,


        MODBUS_FUNCTION_5_HANDLER_START,
        MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_START,
        MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_START,
        MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        ONLINE_DATA_READ_START,
        ONLINE_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        ONLINE_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        TIME_SET_START,
        TIME_SET_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        TIME_SET_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        TIME_UPDATE_START,
        TIME_UPDATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        TIME_UPDATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

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

        CONFIGURATION_CONFIRMATION_CHECK_START,
        CONFIGURATION_CONFIRMATION_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        CONFIGURATION_CONFIRMATION_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        DATA_BASE_BLOCK_MODULE_MUVR_WRITE_START,
        DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,
        DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_START,
        DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        DATA_BASE_BLOCK_SERIAL_AND_ID_WRITE_START,
        DATA_BASE_BLOCK_SERIAL_AND_ID_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DATA_BASE_BLOCK_SERIAL_AND_ID_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_START,
        DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,
    };

    CDeviceControl();
    virtual ~CDeviceControl();

//    size_t GetObjectLength(void);

    // ������ ��� m_sDataStoreName
    std::string GetDataStoreName() const
    {
        return m_sDataStoreName;
    };
    // ������ ��� m_sDataStoreName
    void SetDataStoreName(const std::string& sName)
    {
        m_sDataStoreName = sName;
    };

    // ������ ��� m_pxDataStore
    CDataStore* GetDataStore() const
    {
        return m_pxDataStore;
    };
    // ������ ��� m_pxDataStore
    void SetDataStore(CDataStore* pxDataStore)
    {
        m_pxDataStore = pxDataStore;
    };

    // ������ ��� m_sDataStoreLinkName
    std::string GetDataStoreLinkName() const
    {
        return m_sDataStoreLinkName;
    };
    // ������ ��� m_sDataStoreLinkName
    void SetDataStoreLinkName(const std::string& sName)
    {
        m_sDataStoreLinkName = sName;
    };

    // ������ ��� m_pxDataStoreLink
    CLinkInterface* GetDataStoreLink() const
    {
        return m_pxDataStoreLink;
    };
    // ������ ��� m_pxDataStoreLink
    void SetDataStoreLink(CLinkInterface* pxLink)
    {
        m_pxDataStoreLink = pxLink;
    };

    // ������ ��� m_pxCommandDataLink
    CLinkInterface* GetCommandDataLink() const
    {
        return m_pxCommandDataLink;
    };
    // ������ ��� m_pxCommandDataLink
    void SetCommandDataLink(CLinkInterface* pxValue)
    {
        m_pxCommandDataLink = pxValue;
    };

    // ������ ��� m_pxOperatingDataLink
    CLinkInterface* GetOperatingDataLink() const
    {
        return m_pxOperatingDataLink;
    };
    // ������ ��� m_pxOperatingDataLink
    void SetOperatingDataLink(CLinkInterface* pxValue)
    {
        m_pxOperatingDataLink = pxValue;
    };

    void SetConfigurationCreateName(const std::string& sName)
    {
        m_sConfigurationCreateName = sName;
    };
    void SetInternalModuleMuvrName(std::string sName);

    uint8_t Init(void);
    void Allocate(void);
    void LinuxCurrentTimeSet(unsigned char *pucSource);
    void CurrentTimeSet(void);
    void CurrentTimeUpdate(void);
    void OnlineDataRead(void);
    uint8_t ModbusFunction5Handler(void);
    uint16_t DataBaseBlockReadBlockRelatedAction(void);
    uint16_t DataBaseBlockWriteBlockRelatedAction(void);

    uint8_t Fsm(void);

protected:
    struct tm* pxCurrentTime;

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

    uint16_t* m_puiHoldingRegisters;
    uint16_t* m_puiInputRegisters;

    uint8_t* m_puiIntermediateBuff;
    bool m_fbIsConfigurationConfirmed = false;

    CDataContainerDataBase* m_pxCommandDataContainer;
    CDataContainerDataBase* m_pxOperatingDataContainer;
private:
};


//-------------------------------------------------------------------------------
#endif // CDEVICECONTROL_H
