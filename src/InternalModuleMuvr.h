#ifndef CINTERNALMODULEMUVR_H
#define CINTERNALMODULEMUVR_H
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
#include <string.h>

// MUVR
//// коды типа модуля.
#define MODULE_TYPE_MVPS3 0x06
#define MODULE_TYPE_MVAI5 0x07
#define MODULE_TYPE_MVST3 0x08
#define MODULE_TYPE_MVAO3 0x09
#define MODULE_TYPE_MUVR 0x0F

enum
{
    MUVR_FSM_STATE_CHANGE_PROTOCOL = 0x00,
// состояние автомата - запись базы данных.
    MUVR_FSM_STATE_WRITE_DATABASE = 0x01,
// состояние автомата - нормальный обмен.
    MUVR_FSM_STATE_TRANSFER = 0x02,
};

#define MAX_MUVR_MODULES_QUANTITY 8
#define MUVR_ANALOG_INPUT_QUANTITY 6
// количество дискретных сигналов порождаемое одним аналоговым входом.
#define MUVR_DI_VALUE_ONE_CHANNEL_LENGTH 4
// количество дискретных сигналов одного модуля.
#define MUVR_DISCRETE_INPUT_QUANTITY (MUVR_ANALOG_INPUT_QUANTITY * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH)
// количество байт занимаемое одним модулем в рабочих массивах Modbus.
#define MUVR_AI_VALUE_BYTE_ARRAY_LENGTH (MUVR_ANALOG_INPUT_QUANTITY * sizeof(float))
#define MUVR_DI_VALUE_BIT_ARRAY_LENGTH MUVR_DISCRETE_INPUT_QUANTITY
#define MUVR_BAD_AI_BIT_ARRAY_LENGTH MUVR_ANALOG_INPUT_QUANTITY
#define MUVR_BAD_DI_BIT_ARRAY_LENGTH (MUVR_ANALOG_INPUT_QUANTITY * MUVR_DI_VALUE_ONE_CHANNEL_LENGTH)
#define MUVR_STAT_AI_BIT_ARRAY_LENGTH (MUVR_ANALOG_INPUT_QUANTITY * STAT_AI_BIT_QUANTITY)
#define MUVR_STAT_AI_BYTE_ARRAY_LENGTH MUVR_ANALOG_INPUT_QUANTITY
#define MUVR_MODULE_BAD_BIT_ARRAY_LENGTH 1
#define MUVR_AIN_OFF_BIT_ARRAY_LENGTH MUVR_ANALOG_INPUT_QUANTITY

// адрес Modbus function 5 - 0x11XY - калибровка начала шкалы - НШК.
// X - номер модуля, Y - номер калибруемого входа.
#define MUVR_COMMAND_CONTROL_SET_BOTTOM_OF_SCALE  0x11
// команда для модуля. бит D6 - калибровка MIN (0-нет, 1-есть)
#define MUVR_SET_BOTTOM_OF_SCALE  0x40
// адрес Modbus function 5 - 0x12XY - калибровка конца шкалы - ВШК.
// X - номер модуля, Y - номер калибруемого входа.
#define MUVR_COMMAND_CONTROL_SET_TOP_OF_SCALE  0x12
// команда для модуля. бит D7 - калибровка MАХ (0-нет, 1-есть).
#define MUVR_SET_TOP_OF_SCALE  0x80
// $43 - запрос ТХС
#define MUVR_GET_TXS_DATA_COMMAND 0x43
#define MUVR_GET_TXS_DATA_COMMAND_ANSWER_LENGTH 5
// номер входа температуры холодного спая ТХС МВСТ3.
#define MUVR_TXS_INPUT_NUMBER  0x07
#define MUVR_TXS_INPUT_QUANTITY  1
// если в старшем регистре адреса Modbus бит7 = 0, то запрашиваются реперные точки - (бит0 - бит6) - адрес аналогового входа.
// если в старшем регистре адреса Modbus бит7 = 1, то запрашивается ТХС и (бит0 - бит2) - относительный адрес модуля МВСТ3.
#define MUVR_TXS_REQUEST_MASK  0x80
// (бит0 - бит2) - относительный адрес модуля МВСТ3.
#define TXS_MVST3_MODULE_NUMBER_MASK  0x07
// на месте приёма от модуля двух байт контрольной суммы.
#define MUVR_TXS_CALIBRATION_DATA_OFFSET 5
// $44 - запрос на основной (циклический) обмен данными
#define MUVR_GET_MEASURE_DATA_COMMAND 0x44
#define MUVR_ANSWER_DATA_NOT_READY 0x24
#define MUVR_ANSWER_DATABASE_ERROR 0x25
#define MUVR_ANSWER_REPER_POINTS_ADC_DATABASE_ERROR 0x26
// $45 - запрос реперных точек и кода АЦП
#define MUVR_GET_REPER_POINTS_ADC_DATA_COMMAND 0x45
// код ошибки - модуль не отвечает. при запросе о результате программированния функцией Modbus 14 от программатора.
#define MUVR_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR 0x01
// код ошибки - ошибка обмена данными. при запросе о результате программированния функцией Modbus 14 от программатора.
#define MUVR_GET_REPER_POINTS_ADC_DATA_COMMUNICATION_ERROR 0x02
#define MUVR_REPER_POINTS_ADC_CHANNEL_QUANTITY 7
#define MUVR_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH 24
// $84 - запрос на прием БД  (от МВА к МЦП)
#define MUVR_GET_DATA_BASE_COMMAND 0x84
// $81 - запрос на передачу БД (от МЦП к МВА)
#define MUVR_SET_DATA_BASE_COMMAND 0x81
// $82 - запрос о результатах программирования
#define MUVR_CHECK_DATA_BASE_WRITE_COMMAND 0x82
#define MUVR_CHECK_DATA_BASE_WRITE_ANSWER_BUSY 0xF0
// определяют время ожидания ответа от модуля, с результатом записи базы данных.
#define MUVR_CHECK_DATA_BASE_WRITE_BUSY_WAITING_TIME 10
#define MUVR_CHECK_DATA_BASE_WRITE_BUSY_DELAY_TIME 500000

// смещения на данные в буфере auiSpiRxBuffer[], при обмене по SPI.
#define MUVR_COMMAND_BYTE_OFFSET 1
#define MUVR_COMMAND_BYTE_LENGTH 1
// смещение на данные измеренных значений аналоговых сигналов.
#define MUVR_MEASURE_DATA_OFFSET (MUVR_COMMAND_BYTE_OFFSET + 1)
#define MUVR_DATA_OFFSET (MUVR_COMMAND_BYTE_OFFSET + 1)
// количество байт данных состояния аналоговых входов.
#define MUVR_ANALOG_INPUT_STATE_BYTE_QUANTITY 6
// количество байт данных одного измеренного значения аналогового сигнала(формат float STEP5).
#define MUVR_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY 3
// смещение на данные состояния каналов аналоговых входов.
#define MUVR_STATE_DATA_OFFSET (MUVR_MEASURE_DATA_OFFSET + (MUVR_ANALOG_INPUT_QUANTITY * MUVR_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY))
// смещение на данные требования калибровки. передаются в модуль двумя байтами,
// на месте приёма от модуля двух байт контрольной суммы.
#define MUVR_CALIBRATION_DATA_OFFSET (MUVR_STATE_DATA_OFFSET + MUVR_ANALOG_INPUT_STATE_BYTE_QUANTITY)

// (sizeof(struct TAnalogueInputDescriptionDataBase) * ANALOG_MODULE_INPUT_QUANTITY)// 28х6=168.
#define ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH 168

class CTimer;
class CPlatform;
class CTask;
class CResources;
class CCommunicationDeviceInterface;
class CInternalModule;

//-------------------------------------------------------------------------------
class CInternalModuleMuvr : public CInternalModule
{
public:
    enum
    {
        MUVR_IDDLE = NEXT_STEP,
        MUVR_CHANGE_PROTOCOL,
        MUVR_WRITE_DATA_BASE,
        MUVR_WRITE_DATA_BASE_CHECK,
        MUVR_DATA_EXCHANGE,
    };

    CInternalModuleMuvr();
    CInternalModuleMuvr(uint8_t muiAddress);
    virtual ~CInternalModuleMuvr();

//    void SetResources(CResources* pxResources);
//    CResources* GetResources(void);

    uint8_t GetType(void);

    uint8_t GetWorkingStatus(void);

    void SetAddress(uint8_t uiAddress);
    uint8_t GetAddress(void);

    bool IsReadyToStartWork(void);
    bool IsAbleToReplace(uint8_t uiType);
    void SetCommunicationDevice(CCommunicationDeviceInterface* pxCommunicationDevice);
    uint8_t GetModuleType(uint8_t uiAddress);
    uint8_t DataBaseRead(uint8_t uiAddress);

private:
    uint8_t m_uiAddress;
    CCommunicationDeviceInterface* m_pxCommunicationDevice;
//    CResources* m_pxResources;
};

#endif // CINTERNALMODULEMUVR_H
