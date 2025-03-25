#ifndef MVAI5MODULE_H_INCLUDED
#define MVAI5MODULE_H_INCLUDED

#include "PasNewConfig.h"


//============================================================================
// MVAI5
//// коды типа модуля.
//#define MODULE_TYPE_MVPS3 0x06
//#define MODULE_TYPE_MVAI5 0x07
//#define MODULE_TYPE_MVST3 0x08
//#define MODULE_TYPE_MVAO3 0x09

enum
{
    MVAI5_FSM_STATE_CHANGE_PROTOCOL = 0x00,
// состояние автомата - запись базы данных.
    MVAI5_FSM_STATE_WRITE_DATABASE = 0x01,
// состояние автомата - нормальный обмен.
    MVAI5_FSM_STATE_TRANSFER = 0x02,
};

enum
{
    MVAI5_IDDLE = 0,
    MVAI5_CHANGE_PROTOCOL,
    MVAI5_WRITE_DATA_BASE,
    MVAI5_WRITE_DATA_BASE_CHECK,
    MVAI5_DATA_EXCHANGE,
};

#define MAX_MVAI5_MODULES_QUANTITY 8
#define MVAI5_ANALOG_INPUT_QUANTITY 6
// количество дискретных сигналов порождаемое одним аналоговым входом.
#define MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH 4
// количество дискретных сигналов одного модуля.
#define MVAI5_DISCRETE_INPUT_QUANTITY (MVAI5_ANALOG_INPUT_QUANTITY * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH)
// количество байт занимаемое одним модулем в рабочих массивах Modbus.
#define MVAI5_AI_VALUE_BYTE_ARRAY_LENGTH (MVAI5_ANALOG_INPUT_QUANTITY * sizeof(float))
#define MVAI5_DI_VALUE_BIT_ARRAY_LENGTH MVAI5_DISCRETE_INPUT_QUANTITY
#define MVAI5_BAD_AI_BIT_ARRAY_LENGTH MVAI5_ANALOG_INPUT_QUANTITY
#define MVAI5_BAD_DI_BIT_ARRAY_LENGTH (MVAI5_ANALOG_INPUT_QUANTITY * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH)
#define MVAI5_STAT_AI_BIT_ARRAY_LENGTH (MVAI5_ANALOG_INPUT_QUANTITY * STAT_AI_BIT_QUANTITY)
#define MVAI5_STAT_AI_BYTE_ARRAY_LENGTH MVAI5_ANALOG_INPUT_QUANTITY
#define MVAI5_MODULE_BAD_BIT_ARRAY_LENGTH 1
#define MVAI5_AIN_OFF_BIT_ARRAY_LENGTH MVAI5_ANALOG_INPUT_QUANTITY

// адрес Modbus function 5 - 0x11XY - калибровка начала шкалы - НШК.
// X - номер модуля, Y - номер калибруемого входа.
#define MVAI5_COMMAND_CONTROL_SET_BOTTOM_OF_SCALE  0x11
// команда для модуля. бит D6 - калибровка MIN (0-нет, 1-есть)
#define MVAI5_SET_BOTTOM_OF_SCALE  0x40
// адрес Modbus function 5 - 0x12XY - калибровка конца шкалы - ВШК.
// X - номер модуля, Y - номер калибруемого входа.
#define MVAI5_COMMAND_CONTROL_SET_TOP_OF_SCALE  0x12
// команда для модуля. бит D7 - калибровка MАХ (0-нет, 1-есть).
#define MVAI5_SET_TOP_OF_SCALE  0x80
// $43 - запрос ТХС
#define MVAI5_GET_TXS_DATA_COMMAND 0x43
#define MVAI5_GET_TXS_DATA_COMMAND_ANSWER_LENGTH 5
// номер входа температуры холодного спая ТХС МВСТ3.
#define MVAI5_TXS_INPUT_NUMBER  0x07
#define MVAI5_TXS_INPUT_QUANTITY  1
// если в старшем регистре адреса Modbus бит7 = 0, то запрашиваются реперные точки - (бит0 - бит6) - адрес аналогового входа.
// если в старшем регистре адреса Modbus бит7 = 1, то запрашивается ТХС и (бит0 - бит2) - относительный адрес модуля МВСТ3.
#define MVAI5_TXS_REQUEST_MASK  0x80
// (бит0 - бит2) - относительный адрес модуля МВСТ3.
#define TXS_MVST3_MODULE_NUMBER_MASK  0x07
// на месте приёма от модуля двух байт контрольной суммы.
#define MVAI5_TXS_CALIBRATION_DATA_OFFSET 5
// $44 - запрос на основной (циклический) обмен данными
#define MVAI5_GET_MEASURE_DATA_COMMAND 0x44
#define MVAI5_ANSWER_DATA_NOT_READY 0x24
#define MVAI5_ANSWER_DATABASE_ERROR 0x25
#define MVAI5_ANSWER_REPER_POINTS_ADC_DATABASE_ERROR 0x26
// $45 - запрос реперных точек и кода АЦП
#define MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND 0x45
// код ошибки - модуль не отвечает. при запросе о результате программированния функцией Modbus 14 от программатора.
#define MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR 0x01
// код ошибки - ошибка обмена данными. при запросе о результате программированния функцией Modbus 14 от программатора.
#define MVAI5_GET_REPER_POINTS_ADC_DATA_COMMUNICATION_ERROR 0x02
#define MVAI5_REPER_POINTS_ADC_CHANNEL_QUANTITY 7
#define MVAI5_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH 24
// $84 - запрос на прием БД  (от МВА к МЦП)
#define MVAI5_GET_DATA_BASE_COMMAND 0x84
// $81 - запрос на передачу БД (от МЦП к МВА)
#define MVAI5_SET_DATA_BASE_COMMAND 0x81
// $82 - запрос о результатах программирования
#define MVAI5_CHECK_DATA_BASE_WRITE_COMMAND 0x82
#define MVAI5_CHECK_DATA_BASE_WRITE_ANSWER_BUSY 0xF0
// определяют время ожидания ответа от модуля, с результатом записи базы данных.
#define MVAI5_CHECK_DATA_BASE_WRITE_BUSY_WAITING_TIME 10
#define MVAI5_CHECK_DATA_BASE_WRITE_BUSY_DELAY_TIME 500000

// смещения на данные в буфере auiSpiRxBuffer[], при обмене по SPI.
#define MVAI5_COMMAND_BYTE_OFFSET 1
#define MVAI5_COMMAND_BYTE_LENGTH 1
// смещение на данные измеренных значений аналоговых сигналов.
#define MVAI5_MEASURE_DATA_OFFSET (MVAI5_COMMAND_BYTE_OFFSET + 1)
#define MVAI5_DATA_OFFSET (MVAI5_COMMAND_BYTE_OFFSET + 1)
// количество байт данных состояния аналоговых входов.
#define MVAI5_ANALOG_INPUT_STATE_BYTE_QUANTITY 6
// количество байт данных одного измеренного значения аналогового сигнала(формат float STEP5).
#define MVAI5_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY 3
// смещение на данные состояния каналов аналоговых входов.
#define MVAI5_STATE_DATA_OFFSET (MVAI5_MEASURE_DATA_OFFSET + (MVAI5_ANALOG_INPUT_QUANTITY * MVAI5_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY))
// смещение на данные требования калибровки. передаются в модуль двумя байтами,
// на месте приёма от модуля двух байт контрольной суммы.
#define MVAI5_CALIBRATION_DATA_OFFSET (MVAI5_STATE_DATA_OFFSET + MVAI5_ANALOG_INPUT_STATE_BYTE_QUANTITY)


#endif // MVAI5MODULE_H_INCLUDED
