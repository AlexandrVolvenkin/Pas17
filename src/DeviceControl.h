#ifndef CDEVICECONTROL_H
#define CDEVICECONTROL_H
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <thread>

#include "DataTypes.h"

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
class CSettingsSet;


//-------------------------------------------------------------------------------
// команды для драйверов модулей передаваемые через контекст.
// инкремент уставок - SP, OUT.
#define DEVICE_CONTROL_MTVI5_INCREMENT_SP_OUT  0x01
// декремент уставок - SP, OUT.
#define DEVICE_CONTROL_MTVI5_DECREMENT_SP_OUT  0x02
// записать байт-команду в массив состояния регуляторов «CONT_ST».
#define DEVICE_CONTROL_MTVI5_CONT_ST_WRITE  0x04
// записать параметры в массив аналоговых переменных регуляторов «CONT_AV».
#define DEVICE_CONTROL_MTVI5_CONT_AV_WRITE  0x05
#define DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_PROGRAMMER  0x06
#define DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_HMI  0x07
#define DEVICE_CONTROL_MTVI5_LOAD_OUTS_OSF  0x08
#define DEVICE_CONTROL_MTVI5_CONT_ST_CHANGE  0x09

// адреса-команды modbus функции 5.
// включение-выключение режима калибровки.
#define DEVICE_CONTROL_CALIBRATION_ON_OFF 0x1000
// калибровка начала шкалы.
#define DEVICE_CONTROL_SET_BOTTOM_OF_SCALE  0x1100
// калибровка конца шкалы.
#define DEVICE_CONTROL_SET_TOP_OF_SCALE  0x1200
// включение-выключение режима блоуировки.
#define DEVICE_CONTROL_BLOCK 0x007D
// квитирование с верхнего уровня.
#define DEVICE_CONTROL_PC_KVIT  0x007E
// сброс с верхнего уровня.
#define DEVICE_CONTROL_PC_RESET 0x007F
// количество байт в сообщении установки времени.
#define CURRENT_TIME_BYTE_QUANTITY 7

//-------------------------------------------------------------------------------
class CDeviceControl : public CTask
{
public:
    // коды опций домена управления устройством, запись
    enum
    {
        DEVICE_CONTROL_DOMAIN_DATA_WRITE_ANALOGUE_MEASURE_ARCHIVE_WRITE = 1,
        DEVICE_CONTROL_DOMAIN_DATA_WRITE_REGULATORS_DAC_DATA_WRITE,
        DEVICE_CONTROL_DOMAIN_DATA_WRITE_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE,
    };

    // коды опций домена управления устройством, чтение
    enum
    {
        DEVICE_CONTROL_DOMAIN_DATA_READ_STATE_DATA_READ = 1,
        DEVICE_CONTROL_DOMAIN_DATA_READ_ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST,
        DEVICE_CONTROL_DOMAIN_DATA_READ_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ,
    };

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

        DEVICE_CONTROL_DOMAIN_DATA_WRITE_START,
        DEVICE_CONTROL_DOMAIN_DATA_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DEVICE_CONTROL_DOMAIN_DATA_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        DEVICE_CONTROL_DOMAIN_DATA_READ_START,
        DEVICE_CONTROL_DOMAIN_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DEVICE_CONTROL_DOMAIN_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        DEVICE_CONTROL_DOMAIN_DATA_READ_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_GET,


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

        SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_START,
        SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,
        SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_START,
        SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_START,
        SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

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

    // Геттер для m_sDataStoreName
    std::string GetDataStoreName() const
    {
        return m_sDataStoreName;
    };
    // Сеттер для m_sDataStoreName
    void SetDataStoreName(const std::string& sName)
    {
        m_sDataStoreName = sName;
    };

    // Геттер для m_pxDataStore
    CDataStore* GetDataStore() const
    {
        return m_pxDataStore;
    };
    // Сеттер для m_pxDataStore
    void SetDataStore(CDataStore* pxDataStore)
    {
        m_pxDataStore = pxDataStore;
    };

    // Геттер для m_sDataStoreLinkName
    std::string GetDataStoreLinkName() const
    {
        return m_sDataStoreLinkName;
    };
    // Сеттер для m_sDataStoreLinkName
    void SetDataStoreLinkName(const std::string& sName)
    {
        m_sDataStoreLinkName = sName;
    };

    // Геттер для m_pxDataStoreLink
    CLinkInterface* GetDataStoreLink() const
    {
        return m_pxDataStoreLink;
    };
    // Сеттер для m_pxDataStoreLink
    void SetDataStoreLink(CLinkInterface* pxLink)
    {
        m_pxDataStoreLink = pxLink;
    };

    // Геттер для m_pxCommandDataLink
    CLinkInterface* GetCommandDataLink() const
    {
        return m_pxCommandDataLink;
    };
    // Сеттер для m_pxCommandDataLink
    void SetCommandDataLink(CLinkInterface* pxValue)
    {
        m_pxCommandDataLink = pxValue;
    };

    // Геттер для m_pxOperatingDataLink
    CLinkInterface* GetOperatingDataLink() const
    {
        return m_pxOperatingDataLink;
    };
    // Сеттер для m_pxOperatingDataLink
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
    void AnalogueMeasureArchiveWrite(void);
    void OnlineDataRead(void);
    uint8_t ModbusFunction5Handler(void);
    uint16_t DataBaseBlockReadBlockRelatedAction(void);
    uint16_t DataBaseBlockWriteBlockRelatedAction(void);

//void *thread_ArchiveFileSave(void *value);

    uint8_t Fsm(void);

protected:
    struct tm* pxCurrentTime;

    std::string m_sDataStoreName;
    CDataStore* m_pxDataStore;
    uint16_t m_uiDataStoreId;

    std::string m_sConfigurationCreateName;
    uint16_t m_uiConfigurationCreateId;

    std::string m_sInternalModuleMuvrName;
    uint16_t m_uiInternalModuleMuvrId;

    std::string m_sDataStoreLinkName;
    CLinkInterface* m_pxDataStoreLink;

    CLinkInterface* m_pxCommandDataLink;
    CLinkInterface* m_pxOperatingDataLink;

    uint16_t* m_puiHoldingRegisters;
    uint16_t* m_puiInputRegisters;

    uint8_t* m_puiSerialAndId;

    std::shared_ptr<std::thread> m_pxAnalogueMeasureArchiveWriteThread;

    uint8_t* m_puiIntermediateBuff;
    bool m_fbIsConfigurationConfirmed = false;
    uint8_t uiArchiveFileIsSaveState;
    TFileSaveStateDataPackOne xFileSaveStateDataPackOne;

    CDataContainerDataBase* m_pxCommandDataContainer;
    CDataContainerDataBase* m_pxOperatingDataContainer;
private:
};


//-------------------------------------------------------------------------------
#endif // CDEVICECONTROL_H
