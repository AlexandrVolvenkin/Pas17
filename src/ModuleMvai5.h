#ifndef MVAI5MODULE_H_INCLUDED
#define MVAI5MODULE_H_INCLUDED

#include "PasNewConfig.h"


//============================================================================
// MVAI5
//// ���� ���� ������.
//#define MODULE_TYPE_MVPS3 0x06
//#define MODULE_TYPE_MVAI5 0x07
//#define MODULE_TYPE_MVST3 0x08
//#define MODULE_TYPE_MVAO3 0x09

enum
{
    MVAI5_FSM_STATE_CHANGE_PROTOCOL = 0x00,
// ��������� �������� - ������ ���� ������.
    MVAI5_FSM_STATE_WRITE_DATABASE = 0x01,
// ��������� �������� - ���������� �����.
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
// ���������� ���������� �������� ����������� ����� ���������� ������.
#define MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH 4
// ���������� ���������� �������� ������ ������.
#define MVAI5_DISCRETE_INPUT_QUANTITY (MVAI5_ANALOG_INPUT_QUANTITY * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH)
// ���������� ���� ���������� ����� ������� � ������� �������� Modbus.
#define MVAI5_AI_VALUE_BYTE_ARRAY_LENGTH (MVAI5_ANALOG_INPUT_QUANTITY * sizeof(float))
#define MVAI5_DI_VALUE_BIT_ARRAY_LENGTH MVAI5_DISCRETE_INPUT_QUANTITY
#define MVAI5_BAD_AI_BIT_ARRAY_LENGTH MVAI5_ANALOG_INPUT_QUANTITY
#define MVAI5_BAD_DI_BIT_ARRAY_LENGTH (MVAI5_ANALOG_INPUT_QUANTITY * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH)
#define MVAI5_STAT_AI_BIT_ARRAY_LENGTH (MVAI5_ANALOG_INPUT_QUANTITY * STAT_AI_BIT_QUANTITY)
#define MVAI5_STAT_AI_BYTE_ARRAY_LENGTH MVAI5_ANALOG_INPUT_QUANTITY
#define MVAI5_MODULE_BAD_BIT_ARRAY_LENGTH 1
#define MVAI5_AIN_OFF_BIT_ARRAY_LENGTH MVAI5_ANALOG_INPUT_QUANTITY

// ����� Modbus function 5 - 0x11XY - ���������� ������ ����� - ���.
// X - ����� ������, Y - ����� ������������ �����.
#define MVAI5_COMMAND_CONTROL_SET_BOTTOM_OF_SCALE  0x11
// ������� ��� ������. ��� D6 - ���������� MIN (0-���, 1-����)
#define MVAI5_SET_BOTTOM_OF_SCALE  0x40
// ����� Modbus function 5 - 0x12XY - ���������� ����� ����� - ���.
// X - ����� ������, Y - ����� ������������ �����.
#define MVAI5_COMMAND_CONTROL_SET_TOP_OF_SCALE  0x12
// ������� ��� ������. ��� D7 - ���������� M�� (0-���, 1-����).
#define MVAI5_SET_TOP_OF_SCALE  0x80
// $43 - ������ ���
#define MVAI5_GET_TXS_DATA_COMMAND 0x43
#define MVAI5_GET_TXS_DATA_COMMAND_ANSWER_LENGTH 5
// ����� ����� ����������� ��������� ���� ��� ����3.
#define MVAI5_TXS_INPUT_NUMBER  0x07
#define MVAI5_TXS_INPUT_QUANTITY  1
// ���� � ������� �������� ������ Modbus ���7 = 0, �� ������������� �������� ����� - (���0 - ���6) - ����� ����������� �����.
// ���� � ������� �������� ������ Modbus ���7 = 1, �� ������������� ��� � (���0 - ���2) - ������������� ����� ������ ����3.
#define MVAI5_TXS_REQUEST_MASK  0x80
// (���0 - ���2) - ������������� ����� ������ ����3.
#define TXS_MVST3_MODULE_NUMBER_MASK  0x07
// �� ����� ����� �� ������ ���� ���� ����������� �����.
#define MVAI5_TXS_CALIBRATION_DATA_OFFSET 5
// $44 - ������ �� �������� (�����������) ����� �������
#define MVAI5_GET_MEASURE_DATA_COMMAND 0x44
#define MVAI5_ANSWER_DATA_NOT_READY 0x24
#define MVAI5_ANSWER_DATABASE_ERROR 0x25
#define MVAI5_ANSWER_REPER_POINTS_ADC_DATABASE_ERROR 0x26
// $45 - ������ �������� ����� � ���� ���
#define MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND 0x45
// ��� ������ - ������ �� ��������. ��� ������� � ���������� ����������������� �������� Modbus 14 �� �������������.
#define MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR 0x01
// ��� ������ - ������ ������ �������. ��� ������� � ���������� ����������������� �������� Modbus 14 �� �������������.
#define MVAI5_GET_REPER_POINTS_ADC_DATA_COMMUNICATION_ERROR 0x02
#define MVAI5_REPER_POINTS_ADC_CHANNEL_QUANTITY 7
#define MVAI5_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH 24
// $84 - ������ �� ����� ��  (�� ��� � ���)
#define MVAI5_GET_DATA_BASE_COMMAND 0x84
// $81 - ������ �� �������� �� (�� ��� � ���)
#define MVAI5_SET_DATA_BASE_COMMAND 0x81
// $82 - ������ � ����������� ����������������
#define MVAI5_CHECK_DATA_BASE_WRITE_COMMAND 0x82
#define MVAI5_CHECK_DATA_BASE_WRITE_ANSWER_BUSY 0xF0
// ���������� ����� �������� ������ �� ������, � ����������� ������ ���� ������.
#define MVAI5_CHECK_DATA_BASE_WRITE_BUSY_WAITING_TIME 10
#define MVAI5_CHECK_DATA_BASE_WRITE_BUSY_DELAY_TIME 500000

// �������� �� ������ � ������ auiSpiRxBuffer[], ��� ������ �� SPI.
#define MVAI5_COMMAND_BYTE_OFFSET 1
#define MVAI5_COMMAND_BYTE_LENGTH 1
// �������� �� ������ ���������� �������� ���������� ��������.
#define MVAI5_MEASURE_DATA_OFFSET (MVAI5_COMMAND_BYTE_OFFSET + 1)
#define MVAI5_DATA_OFFSET (MVAI5_COMMAND_BYTE_OFFSET + 1)
// ���������� ���� ������ ��������� ���������� ������.
#define MVAI5_ANALOG_INPUT_STATE_BYTE_QUANTITY 6
// ���������� ���� ������ ������ ����������� �������� ����������� �������(������ float STEP5).
#define MVAI5_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY 3
// �������� �� ������ ��������� ������� ���������� ������.
#define MVAI5_STATE_DATA_OFFSET (MVAI5_MEASURE_DATA_OFFSET + (MVAI5_ANALOG_INPUT_QUANTITY * MVAI5_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY))
// �������� �� ������ ���������� ����������. ���������� � ������ ����� �������,
// �� ����� ����� �� ������ ���� ���� ����������� �����.
#define MVAI5_CALIBRATION_DATA_OFFSET (MVAI5_STATE_DATA_OFFSET + MVAI5_ANALOG_INPUT_STATE_BYTE_QUANTITY)


#endif // MVAI5MODULE_H_INCLUDED
