#ifndef DISCRETESIGNALS_H
#define DISCRETESIGNALS_H
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


class Timer;
class Platform;
class CTask;
class CResources;
class CCommunicationDevice;
class CCommunicationDeviceInterface;
class CSpiCommunicationDevice;
class CDataContainerInterface;
class CDataContainerDataBase;
class CInternalModule;
class CConfigurationCreate;
class CDeviceControl;
class CDataBaseCreate;
class CDiscreteSignals;

#define DISCRETE_INPUT_RELAY_OUTPUT_QUANTITY 4//DO_VALUE_BIT_ARRAY_LENGTH
// норма.
#define FLOW_CONTROL_NORMA 1
// новое нарушение.
#define FLOW_CONTROL_NEW_VIOLATION 2
// не новое нарушение.
#define FLOW_CONTROL_NOT_NEW_VIOLATION 3


// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// Структура описателей БД дискретных входных сигналов.
// источник: PAS_05_PO.DOC, стр - 3,
// 3. БАЗА ДАННЫХ ОБРАБОТКИ ДИСКРЕТНЫХ СИГНАЛОВ
struct TDiscreteSignalsDescriptionWorkPackOne
{
    // Байт 1:
    // D0-D4 - ТБЛ (группа): 0,1-24(0-нет вых.на табло), D5-ТК (тип контакта): 0-НЗ,1-НР.
    // D6 - ТСГ1 – тип сигнализации состояния дискретного сигнала.
    // D7 - ТСГ2 – тип сигнализации состояния дискретного сигнала.
    uint8_t 	uiTalTkGrp;
    // Байт 2:
    // Выходы на МР1:D0=1 есть вых.на реле 1,D0=0-нет вых на реле 1 и т.д.
    // Байт 3,4,5,6,7,8,9:
    // Выходы на МР2,МР3,МР4,МР5,MP6,MP7,MP8:аналог.МР1.
    uint8_t auiRelayOut[DISCRETE_OUTPUT_MODULE_MAX_NUMBER];
    // Байт 10:
    // D7 - резерв, D6 - ДВЗ:0-задерж.в сек,1-в дес.сек.
    // D0-D5 - УВЗ:уставка времени задержки выхода в ед.ДВЗ(0-60).
    uint8_t uiDelay;
    // Байт 11 - контрольная сумма ($ff+сумма байтов 1-10)    *).
    uint8_t uiCrc;
};
#pragma pack(pop)

// Структура описателей БД дискретных входных сигналов.
// источник: PAS_05_PO.DOC, стр - 3,
// 3. БАЗА ДАННЫХ ОБРАБОТКИ ДИСКРЕТНЫХ СИГНАЛОВ
struct TDiscreteSignalsDescriptionWork
{
    // Байт 1:
    // D0-D4 - ТБЛ (группа): 0,1-24(0-нет вых.на табло), D5-ТК (тип контакта): 0-НЗ,1-НР.
    // D6 - ТСГ1 – тип сигнализации состояния дискретного сигнала.
    // D7 - ТСГ2 – тип сигнализации состояния дискретного сигнала.
    uint8_t 	uiTalTkGrp;
    // Байт 2:
    // Выходы на МР1:D0=1 есть вых.на реле 1,D0=0-нет вых на реле 1 и т.д.
    // Байт 3,4,5,6,7,8,9:
    // Выходы на МР2,МР3,МР4,МР5,MP6,MP7,MP8:аналог.МР1.
    uint8_t auiRelayOut[DISCRETE_OUTPUT_MODULE_MAX_NUMBER];
    // Байт 10:
    // D7 - резерв, D6 - ДВЗ:0-задерж.в сек,1-в дес.сек.
    // D0-D5 - УВЗ:уставка времени задержки выхода в ед.ДВЗ(0-60).
    uint8_t uiDelay;
    // Байт 11 - контрольная сумма ($ff+сумма байтов 1-10)    *).
    uint8_t uiCrc;
};

// маски злементов структуры описателей БД дискретных входных сигналов.
// struct TDiscreteSignalsDescriptionWork
// источник: PAS_05_PO.DOC, стр - 3,
// 3. БАЗА ДАННЫХ ОБРАБОТКИ ДИСКРЕТНЫХ СИГНАЛОВ
// Байт 1: uiTalTkGrp.
// D0-D4 - ТБЛ (группа), номер ячейки на световом табло.
#define LIGHT_BOARD_CELL_NUMBER_DISC_MASK 0x1F
// D5-ТК (тип контакта): 0-НЗ,1-НР
#define CONTACT_TYPE_DISC_MASK 0x20
// D6,D7 - ТСГ тип сигнализации: 0-нет сигн.,1-пред.,2-авар.,3-индик.(ровн.свет)
// ТСГ1 – тип сигнализации состояния дискретного сигнала
#define SIGNALING_TYPE_1_DISC_MASK 0x40
// ТСГ2 – тип сигнализации состояния дискретного сигнала
#define SIGNALING_TYPE_2_DISC_MASK 0x80
// Байт 10: uiDelay.
// D0-D5 - УВЗ: уставка времени задержки выхода в ед.ДВЗ(0-60)
#define DELAY_TIME_SETPOINT_DISC_MASK 0x3F
// D6 - ДВЗ дискретность времени задержки: 0-задерж.в сек,1-в дес.сек.
#define DELAY_TIME_DISCRETENESS_DISC_MASK 0x40
// D7 - АРХ =1 архивировать, 0 – не архивировать.
#define ARCHIVE_DISC_MASK 0x80

// ТБЛ (группа), номер ячейки на световом табло.
#define LIGHT_BOARD_CELL_NUMBER_DIS(pointer) (((pointer) -> uiTalTkGrp) & LIGHT_BOARD_CELL_NUMBER_DISC_MASK)
// ТК (тип контакта): 0-НЗ,1-НР
#define CONTACT_TYPE_DIS(pointer) (((pointer) -> uiTalTkGrp) & CONTACT_TYPE_DISC_MASK)
// ТСГ тип сигнализации: 0-нет сигн.,1-пред.,2-авар.,3-индик.(ровн.свет)
// ТСГ1 – тип сигнализации состояния дискретного сигнала
#define SIGNALING_TYPE_1_DIS(pointer) (((pointer) -> uiTalTkGrp) & SIGNALING_TYPE_1_DISC_MASK)
// ТСГ2 – тип сигнализации состояния дискретного сигнала
#define SIGNALING_TYPE_2_DIS(pointer) (((pointer) -> uiTalTkGrp) & SIGNALING_TYPE_2_DISC_MASK)
// УВЗ: уставка времени задержки выхода в ед.ДВЗ(0-60)
#define DELAY_TIME_SETPOINT_DIS(pointer) (((pointer) -> uiDelay) & DELAY_TIME_SETPOINT_DISC_MASK)
// ДВЗ дискретность времени задержки: 0-задерж.в сек,1-в дес.сек.
#define DELAY_TIME_DISCRETENESS_DIS(pointer) (((pointer) -> uiDelay) & DELAY_TIME_DISCRETENESS_DISC_MASK)
// архив.
#define ARCHIVE_DIS(pointer) ((pointer) -> uiDelay) & ARCHIVE_DISC_MASK

// контрольная сумма.
#define CRC_DIS(pointer) ((pointer) -> uiCrc)

// структура вспомогательных данных формирования требований отображения дискретных сигналов на HMI.
struct TDiscreteSignalsWorkingData
{
    // флаг активности дискретного входа.
    bool fbDiscreteSignalIsActive;
    // счётчик дискретного времени задержки ДВЗ.
    uint8_t uiDelay;
};

//-------------------------------------------------------------------------------
class CDiscreteSignals : public CTask
{
public:
    enum
    {
        DISCRETE_SIGNALS_CREATE_DATA_BASE_START = NEXT_STEP,
        DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_START,
        DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        DISCRETE_SIGNALS_CREATE_SERVICE_DATA_CREATE_START,
        DISCRETE_SIGNALS_CREATE_SERVICE_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DISCRETE_SIGNALS_CREATE_SERVICE_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,

        DISCRETE_SIGNALS_CREATE_ALARM_HANDLERS_START,
        DISCRETE_SIGNALS_CREATE_ALARM_HANDLERS_EXECUTOR_DONE_OK_ANSWER_PROCESSING,
        DISCRETE_SIGNALS_CREATE_ALARM_HANDLERS_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING,
    };

    CDiscreteSignals();
    virtual ~CDiscreteSignals();

    void SetInternalModuleName(std::string sName);
    void SetDeviceControlName(std::string sName);
    // РЎРµС‚С‚РµСЂ РґР»СЏ m_sDataStoreName
    void SetDataStoreName(const std::string& sName)
    {
        m_sDataStoreName = sName;
    };

    void DiscreteSignalsStartDataBaseCreate(void);
    void DiscreteSignalsStartDataBlockWorkToCommonFormat(uint8_t* puiBlockDataPointer,
            TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDescriptionWork);
    void DiscreteSignalsDataBlockCommonFormatToWork(void);
    uint8_t DiscreteSignalsDataBaseCrcCheck(TDiscreteSignalsDescriptionWork *pxDiscreteSignalsDataBase);
    void ProgrammedDiscreteSignalsNumberCount(void);
    void ServiceDataCreate(void);
    void CreateAlarmHandlers(void);
    void AlarmHandlersProcessing(void);

    uint8_t Init(void);
    uint8_t Fsm(void);
    void Allocate(void);

private:

private:
    std::string m_sInternalModuleName;
    uint8_t m_uiInternalModuleId;

    std::string m_sDeviceControlName;
    uint8_t m_uiDeviceControlId;

    std::string m_sDataStoreName;
    uint8_t m_uiDataStoreId;

    std::vector<CTaskInterface*> m_vpxAlarmHandlers;
    std::vector<CTaskInterface*>::iterator m_xAlarmHandlersIterator;

    uint8_t* m_puiIntermediateBuff;
    TDiscreteSignalsDescriptionWork *m_pxDiscreteSignalsDescriptionWork;
    uint8_t m_uiBlocksCounter;
};

#endif // DISCRETESIGNALS_H
//-------------------------------------------------------------------------------
