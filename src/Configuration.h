//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CONFIGURATION_H_INCLUDED
#define CONFIGURATION_H_INCLUDED

// �������������� ������������ ����� � �����.
#define CONVERT_NATURAL_NUMBER_TO_INTEGER 1
// �������������� ������ ����� � �����������.
#define CONVERT_INTEGER_TO_NATURAL_NUMBER 1
#define END_OF_STRING 0
#define END_OF_STRING_LENGTH 1

#define SOURCE_VERSION "1.0"

//LED-----------------------------------------------------------------------------------------------------
#define PRD_EN_PIN  22 /* (U10) gpmc_ad8.gpio0[22] */
#define PRD_EN_PIN_PORT  GPIO_PORT_0
#define PRD_3_PIN  5  /* (A16) spi0_cs0.gpio0[5] */
#define PRD_3_PIN_PORT  GPIO_PORT_0
#define PRD_2_PIN  20  /* (D13) mcasp0_axr1.gpio3[20] */
#define PRD_2_PIN_PORT  GPIO_PORT_3
#define PRD_1_PIN  21  /* (A14) mcasp0_ahclkx.gpio3[21] */
#define PRD_1_PIN_PORT  GPIO_PORT_3

//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
// ������� ��������� �������. ���������� ������� ���� ��������.
#define MAX_HANDLED_DISCRETE_INPUT 16 // ������������ ������ ������ �������������.
#define MAX_HANDLED_DISCRETE_OUTPUT 64

// ������� ����������� ���������� �������������� �������������� ������:
// ����� ������� ���������� ������������ ���������� �������� � ����� ������ ������������� -
// (504 / 2 = 252), 252 = 192 ���������� ������� + 60 �������������� �����.
// 160-663(0xa0 - 0x297); ������ ���������� ������������ ���������� ��������
// ��1-��252, �� 2 ���� �� ������ (504);
//#define ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH ((MAX_HANDLED_DISCRETE_INPUT + MAX_HANDLED_FUNCTION_BLOCK) * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT)
// (�������� - �������� ���-05-8(16)CD_��_�2_��. ���. 48, PAS_05_NEW_PO.DOC ���. 16.)

//(�������� - PAS_05_NEW_PO.DOC ���. 16.)
//���������� ������������� ����� ���������� �������� �� HMI ������:
//	1) �������������� ������� ����������� ������� � �����-���� ������ ������������
//	�������� ���������� ���� ��������� ����������� �������
//	2) ��������� �� ��������� � ������������� ����� ������������ �� ���������� �������:
//	- �� 16 ����� �� 12 �������� ��� ����,�� 24 ������� ��� ����, ������� ���
//	- �������� ������������ - ��������� � HMI �������� (������, ����������� ���������)
//	- �������������� ������. - ��������� � HMI �������� (������, ������ ��������)
//	3) ���������������� ����� ������������ � �������������� PRG-05FBL.
//4) �������������� ������������, ������������������� ��� ��� 1-62,��
//��� � �����. ����. = BInp + ���� ��� �� ������ ��������� 252.
//� �����. ����. ���. ��� ��� ������������ ����������� ��� BInp + ����.
//� �����.����. 253 ����������� ��� ������������ �� BECK
//5) ����� ������������: ����������������� / ��������� �������� ������������� ������� / �������� ����� �� �������������� ������ ModBus ��������� (123,124) �� ������� �� �� � ��.

// ������ ������������ �������������� ������ ���������� �����
// ���������� ������ ������������ ���������� ��������.
#define MAX_HANDLED_FUNCTION_BLOCK 120 // ������������ ������ ������ �������������.

#define ALARM_GROUP_QUANTITY 16 // ������������ ������ ������ �������������.
#define MAX_HANDLED_ANALOGUE_INPUT 4
#define MAX_HANDLED_ANALOGUE_OUTPUT 2
// HMI ������.
#define DISPLAY_MODULE_TYPE 2
//#define DISPLAY_MODULE_TYPE_12_CELLS_LCD_4x20 0x00
//#define DISPLAY_MODULE_TYPE_24_CELLS_LCD_4x20 0x01
//#define DISPLAY_MODULE_TYPE_LCD_320x240 0x02
//#define DISPLAY_MODULE_TYPE_6_CELLS_LCD_4x20 0x03
//#define DISPLAY_MODULE_TYPE_LCD_4x20 0x04
#define DISPLAY_MODULE_TYPE_HMI_LCD_4x20 0x04
#define DISPLAY_MODULE_QUANTITY 1
#define DISPLAY_MODULE_CODE 1
#define INTERNAL_MODULE_QUANTITY 8
#define EXTERNAL_MODULE_FUNCTION_1234_QUANTITY 32  // ������������ ������ ������ �������������.
#define EXTERNAL_MODULE_FUNCTION_15_QUANTITY 3
// �� 32 ������� modbus function 1,2,3,4  � �� 3 ������� modbus function 15.
#define EXTERNAL_MODULE_QUANTITY (EXTERNAL_MODULE_FUNCTION_1234_QUANTITY + EXTERNAL_MODULE_FUNCTION_15_QUANTITY)
// ���� ������� ������� � ���������� �������������� ������ ���̻.
#define EXTERNAL_MODULE_TYPE_PAS_01 0
#define EXTERNAL_MODULE_TYPE_MPGR 1
#define EXTERNAL_MODULE_TYPE_PIT_TS 2
#define EXTERNAL_MODULE_TYPE_PIT_TP 3
#define EXTERNAL_MODULE_TYPE_MGRI 4
#define EXTERNAL_MODULE_TYPE_PAS_05 5
#define EXTERNAL_MODULE_TYPE_PSS_7 6
#define EXTERNAL_MODULE_TYPE_PEP_01 7
#define EXTERNAL_MODULE_TYPE_PEP_02 8
#define EXTERNAL_MODULE_TYPE_PAS_19 9
#define EXTERNAL_MODULE_TYPE_RESERVED_1 10
#define EXTERNAL_MODULE_TYPE_RESERVED_1 11
#define EXTERNAL_MODULE_TYPE_RESERVED_1 12
#define EXTERNAL_MODULE_TYPE_RESERVED_1 13
#define EXTERNAL_MODULE_TYPE_RESERVED_1 14
#define EXTERNAL_MODULE_TYPE_OTHER 7

// ���� ������� �� 0 �� 15 ������ ����������� ��������.
// ������� ������ ���� ����� ���� �� 0 �� 15.
// ����� ���������� � ������� ������ �� ������������,
// � ���� ������� ������� ����� ���������� ��������
// �� ���������� ����� ���������� �������.
#define INTERNAL_MODULE_TYPE_QUANTITY 0x10
// �������� �� ���� ������� ������� ������������ ��� ��� "���������������".
#define EXTERNAL_MODULE_TYPE_OFFSET INTERNAL_MODULE_TYPE_QUANTITY
// ���������� ����� ������� ������� ������������ ��� ��� "���������������".
#define EXTERNAL_MODULE_OWN_PRODUCTION_QUANTITY 0x10
// �������� �� ���� ������� ������� ��������� ��������������.
#define EXTERNAL_MODULE_OTHER_TYPE_OFFSET (EXTERNAL_MODULE_TYPE_OFFSET + EXTERNAL_MODULE_OWN_PRODUCTION_QUANTITY)
// �� 0 �� 14 ���� ���� ������� ������������ ��� ��� "���������������".
// ��� �������������� ���������� ������������ ��� ���� �����.
// ���� ������� ������� � �������.
#define OWN_MODULE_TYPE_PAS_01 (0 + EXTERNAL_MODULE_TYPE_OFFSET)
#define OWN_MODULE_TYPE_MPGR (1 + EXTERNAL_MODULE_TYPE_OFFSET)
#define OWN_MODULE_TYPE_PIT_TS (2 + EXTERNAL_MODULE_TYPE_OFFSET)
#define OWN_MODULE_TYPE_PIT_TP (3 + EXTERNAL_MODULE_TYPE_OFFSET)
#define OWN_MODULE_TYPE_MGRI (4 + EXTERNAL_MODULE_TYPE_OFFSET)
#define OWN_MODULE_TYPE_PAS_05 (5 + EXTERNAL_MODULE_TYPE_OFFSET)
#define OWN_MODULE_TYPE_PSS (6 + EXTERNAL_MODULE_TYPE_OFFSET)
#define OWN_MODULE_TYPE_RESERVED_1 (7 + EXTERNAL_MODULE_TYPE_OFFSET)
#define OWN_MODULE_TYPE_RESERVED_2 (8 + EXTERNAL_MODULE_TYPE_OFFSET)
#define OWN_MODULE_TYPE_RESERVED_3 (9 + EXTERNAL_MODULE_TYPE_OFFSET)
#define OWN_MODULE_TYPE_RESERVED_4 (10 + EXTERNAL_MODULE_TYPE_OFFSET)
#define OWN_MODULE_TYPE_RESERVED_5 (11 + EXTERNAL_MODULE_TYPE_OFFSET)
#define OWN_MODULE_TYPE_RESERVED_6 (12 + EXTERNAL_MODULE_TYPE_OFFSET)
#define OWN_MODULE_TYPE_RESERVED_7 (13 + EXTERNAL_MODULE_TYPE_OFFSET)
#define OWN_MODULE_TYPE_RESERVED_8 (14 + EXTERNAL_MODULE_TYPE_OFFSET)
#define OWN_MODULE_TYPE_OTHER (15 + EXTERNAL_MODULE_TYPE_OFFSET)

// ���� ������� ������������ ��� ��� "���������������".
// ��� �������������� ���������� ������������ ��� ������� Modbus 1,2,3,4,15.
#define OWN_MODULE_TYPE_FUNCTION_1 (0 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)
#define OWN_MODULE_TYPE_FUNCTION_2 (1 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)
#define OWN_MODULE_TYPE_FUNCTION_3 (2 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)
#define OWN_MODULE_TYPE_FUNCTION_4 (3 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)
#define OWN_MODULE_TYPE_FUNCTION_15 (4 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)

// ������ ���� �������(��������� ��������������).
// ��� �������������� ���������� ������������ ��� ������� Modbus 1,2,3,4,15.
#define OTHER_MODULE_TYPE_FUNCTION_1 (5 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)
#define OTHER_MODULE_TYPE_FUNCTION_2 (6 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)
#define OTHER_MODULE_TYPE_FUNCTION_3 (7 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)
#define OTHER_MODULE_TYPE_FUNCTION_4 (8 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)
#define OTHER_MODULE_TYPE_FUNCTION_15 (9 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)

// ���� ������ ������� ������� ��������� ����.
#define EXTERNAL_MODULE_ERROR_TIMEOUT 0x01
#define EXTERNAL_MODULE_ERROR_FE_DOR 0x02
#define EXTERNAL_MODULE_ERROR_CRC 0x03
#define EXTERNAL_MODULE_ERROR_ILLEGAL_FUNCTION 0x11
#define EXTERNAL_MODULE_ERROR_ILLEGAL_DATA_ADDRESS 0x12
#define EXTERNAL_MODULE_ERROR_ILLEGAL_DATA_VALUE 0x13
#define EXTERNAL_MODULE_ERROR_SLAVE_OR_SERVER_FAILURE 0x14
// ���� ������ ���������� �������.
#define INTERNAL_MODULE_ERROR_DATA_BASE 0x15
#define INTERNAL_MODULE_ERROR_REPER_POINTS 0x16


#define MAX_MODULE_QUANTITY (INTERNAL_MODULE_QUANTITY + \
                             EXTERNAL_MODULE_QUANTITY + \
							 DISPLAY_MODULE_QUANTITY) \

#define PLC_DATA_BASE_BLOCK_QUANTITY 100
#define PLC_DATA_BASE_BLOCK_LENGTH 256
#define DATA_BASE_BLOCK_NUMBER_DATA_LENGTH 1
#define PLC_DATA_BASE_ERROR_LENGTH 4
// �alc_cds:	������ �������� ��������� ���������� CoDesys 48*4 = 192 �����
#define CALC_CDS_QUANTITY 48
// Sys_cds:	������ �������� ��������� ���������� CoDesys 10*4 = 40 ����
#define SYS_CDS_QUANTITY 10
// Hand_ust:	������ �������, �������� ������� 64*4 = 256 ������
#define HAND_UST_QUANTITY 64
// Av_res		������ 64 ���������� ������� 64*4 = 256 ������
#define AV_RES_QUANTITY 64
#define STAT_AI_BIT_QUANTITY 8
#define CURRENT_TIME_BYTE_QUANTITY 7
#define MODIFICATION_CODE_BYTE_QUANTITY 24

// ���������� ���������� ������ � ����� ������.
#define DISCRETE_MODULE_INPUT_QUANTITY 6
#define ANALOGUE_INPUT_DI_VALUE_QUANTITY 4 // ���������� ���������� �������� ����������� ����� ���������� ������.
#define DISCRETE_OUTPUT_MODULE_MAX_NUMBER 5 // ���������� ������� ���������� ������� ���������� ����.
#define MUVR_MR_DISCRETE_OUTPUT_NUMBER 4 // ���������� ���������� ������� ���������� ����.
#define DISCRETE_OUTPUT_NUMBER (MUVR_MR_DISCRETE_OUTPUT_NUMBER * DISCRETE_OUTPUT_MODULE_MAX_NUMBER) // ���������� ���������� ������� ���������� ����.
// ���������� ���������� ������ � ����� ������.
#define ANALOG_MODULE_INPUT_QUANTITY 4
// ����� ������ ���������� ��������� ����������� ���������.
#define ANALOGUE_INPUT_NAME_LENGTH 8
// ����� ������ ���������� ��������� ����������� ����������� ��������� ����.
#define DIMENSIONS_PARAMETERS_NAME_LENGTH 6
// ����� ������ ���������� ��������� ����������� �������.
#define DISCRETE_SYGNAL_NAME_LENGTH 14
// ����� ������ ���������� ��������� ����������� �������.
#define ANALOGUE_SYGNAL_NAME_LENGTH 35
// ���������� ���������� ������� � ����� ������.
#define ANALOG_OUTPUT_MODULE_REGULATORS_QUANTITY 2
#define ONLINE_DATA_READ_ONE_INPUT_BYTE_QUANTITY 8 // 7 ���� ASCII ������� ���������������� �� float AiValue, 1 ���� StatAi
#define ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY 7 // 7 ���� ASCII ������� ���������������� �� float AiValue
#define ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY 1 // 1 ���� StatAi - ��������� ���������� ������������� �������
#define CALIBRATION_ANALOG_MODULE_NUMBER_MASK 0x00F0 // �����. ����� Modbus function 5 - 0x00X0, X - ����� ������.
#define CALIBRATION_INPUT_NUMBER_MASK 0x000F // �����. ����� Modbus function 5 - 0x000Y, Y - ����� ������������ �����.
// ������� Modbus 71 - _FC_ONLINE_DATA_READ modbus_reply() modbus.cpp.
// ���� � ������� �������� ������ Modbus ���7 = 0, �� ������������� �������� ����� - (���0 - ���6) - ����� ����������� �����.
#define ANALOGUE_INPUT_ADDRESS_MASK 0x7F
// ���� � ������� �������� ������ Modbus ���7 = 1, �� ������������� ��� � (���0 - ���2) - ������������� ����� ������ ����3.
#define ANALOGUE_MODULE_ADDRESS_MASK 0x07

#define DEFAULT_SERVER_ID 1
#define SERVER_ID 17
#define HMI_SLAVE_ADDRESS 1
#define INVALID_SERVER_ID 18
#define MODBUS_BAUD_RATE 9600//19200//
#define HMI_MODBUS_BAUD_RATE 19200//9600//
#define MODBUS_RTU_ENABLE
#define MODBUS_TCP_ENABLE
#define MODBUS_HMI_ENABLE

// �������������� ��������� - �������� ��������� ��������� Modbus.
#define MODBUS_MESSAGE_SOURCE_TCP 1
#define MODBUS_MESSAGE_SOURCE_RTU_HIGH_LEVEL 2
#define MODBUS_MESSAGE_SOURCE_RTU_HMI 3
#define MODBUS_MESSAGE_SOURCE_RTU_MASTER 4
#define MODBUS_MESSAGE_SOURCE_TCP_MASTER 5

//-------------------------------------------------------------------------------
#define MAIN_CYCLE_TIME 100000000 // ����� �������� ����� - 100mc.
//#define MAIN_CYCLE_TIME 100000 // ����� �������� ����� - 100mc.
#define EXTERNAL_CYCLE_STOP_SLEEP_TIME 1000 // ������������ "���", �� ����� �������� ����� ������������� � ������� ������.

//#define MAIN_DATA_ACCESS_SEMAPHORE_NAME "/MainDataAccessSemaphore"
//#define MAIN_DATA_ACCESS_MUTEX_NAME "/MainDataAccessMutex"

#define ONE_SECOND_TIME 10 // ���������� �������� �������� ����� - MAIN_CYCLE_TIME.
#define TWO_SECOND_TIME 20
#define THREE_SECOND_TIME 30
#define TEN_SECOND_TIME 100

//#define MODULE_REQUEST_QUANTITY 5 // ���������� ������� ��������� � �������.
//#define MBM_1234_MODULE_BAD_BIT_ARRAY_LENGTH 1
//#define BITS_IN_BYTE_QUANTITY 8
//// ��������� �������� �������� �������� ������ ��� ������.
//// ���� ����� ���� - ������ ��������� �����������. ��� ������ ���������� ��������������.
//// ��������������� ���� ������������ � ������� ������� aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET + ������ ������].
//#define BAD_MODULE_CYCLE_COUNT_DEFAULT 10
//// ��������� �������� �������� �������� ������ ��� ������ � ������� 60 ������.
//#define BAD_MODULE_60_SECOND_CYCLE_COUNT_DEFAULT 600
//#define BAD_MODULE_RESPONDED_OK 0
//// ��� ������ ����� ������ �� ��������.
//// ������ �� ��������.
//#define BAD_MODULE_NOT_RESPONDED 1
//// ������ �� �������� ��� ������.
//#define BAD_MODULE_NOT_RESPONDED_ON_START 2
// ��������� ������. ��������������� ���� ���������� �������� ������������.
//#define ANALOGUE_INPUT_ERROR 0xFF00

// ������ �������� ������������ ��� ������.
//    1. ������ ������������������ - ��� 01.
#define CONFIGURATION_ERROR_SEQUENCE_ERROR 0x01
//    2. ���� ������ ����� - "�����" - ��� 02.
#define CONFIGURATION_ERROR_EMPTY_SLOT 0x02
//    4. ����� ������������ � ���� ������ - ��� 03.
#define CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE 0x03
//    4. ����� ������������ - ��� 04.
#define CONFIGURATION_ERROR_NEW_CONFIGURATION 0x04
//    3. ���� ���� � ������������� ������� � ������������������ ������� - ��� 05.
#define CONFIGURATION_ERROR_BAD_MODULE_FINDED_AND_REGISTERED 0x05

// �������� ��������� ������.
#define SYSTEM_ERROR_GROUP_OFFSET (MAX_MODULE_QUANTITY - 1)
#define SYSTEM_ERROR_BLOCK_MODE_OFFSET 0
#define SYSTEM_ERROR_CODE_SYS_NOT_RESPONDED_OFFSET 1
#define SYSTEM_ERROR_MIND_NOT_RESPONDED_OFFSET 2
#define SYSTEM_ERROR_CRC_ERROR_OFFSET 3
#define SYSTEM_ERROR_COMMON_DATA_BASE_ERROR_OFFSET 4
#define SYSTEM_ERROR_DISCRETE_SYGNAL_DATA_BASE_ERROR_OFFSET 5
#define SYSTEM_ERROR_FBL_DATA_BASE_ERROR_OFFSET 6
#define SYSTEM_ERROR_CONFIGURATION_ERROR_OFFSET 7


//#define ONE_BYTE_CRC_LENGTH 1
//#define TWO_BYTE_CRC_LENGTH 2
// �������� �� ����������� ����� ����� ���� ������.
#define PLC_DATA_BASE_BLOCK_CRC_OFFSET (PLC_DATA_BASE_BLOCK_LENGTH - TWO_BYTE_CRC_LENGTH)
//// 2 - �������� �� ��� ������ � ������� RX SPI. 0 - ������� �������, 1 - "���" ������� �������.
//#define MODULE_TYPE_OFFSET 2
//#define MODULE_COMMAND_LENGTH 1
//// ���������� ���� � ������ �� ������ ���� ������.
//#define MODULE_COMMAND_ANSWER_LENGTH 1
//// ���������� ���� � "������" ������ ������. 0x7E - OK, 0xE7 - error.
//#define TAIL_ANSWER_LENGTH 1
//#define DATA_EXCHANGE_OK 0x7E
//#define DATA_EXCHANGE_ERROR 0xE7
//#define DATA_EXCHANGE_CRC_ERROR 0x03
// MVAI5_REPER_POINTS_ADC_CHANNEL_QUANTITY + MVAI5_ANALOG_INPUT_QUANTITY// 6�7=42.
#define ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH 42

#define FRAM_LAST_SAVED_TIME_OFFSET 0
#define FRAM_LAST_SAVED_TIME_ARRAY_LENGTH sizeof(struct tm)//sizeof(struct TAnalogueSignalsArchiveHourData)
#define FRAM_ANALOGUE_MEASURE_ARCHIVE_ARRAY_OFFSET (FRAM_LAST_SAVED_TIME_OFFSET + FRAM_LAST_SAVED_TIME_ARRAY_LENGTH)
#define FRAM_ANALOGUE_MEASURE_ARCHIVE_ARRAY_LENGTH 0


//// ������������ ���������� ������� �� ���� SPI.
//#define SPI_CHIP_SELECT_MAX_ADDRESS 16
//// 2 - �������� �� ������ � ������� RX SPI. 0 - ������� �������, 1 - "���" ������� �������.
//#define SPI_DATA_BYTE_OFFSET 2
//// 1 - �������� �� ������� � ������� RX SPI.
//#define SPI_COMMAND_BYTE_OFFSET 1
//#define SPI_COMMAND_BYTE_LENGTH 1
//
//#define SPI_LENGTH_BYTE_OFFSET 2
//#define SPI_LENGTH_BYTE_LENGTH 1
//// ������ ���� - �������, ������ - �����, ������ ������.
//#define SPI_PREAMBLE_LENGTH 2

#define PERCENT_100 100 // ��� ���������.
#define MEASURE_ERROR_THRESHOLD 3 // ����� ������ ����������� ��������.

#define ANALOGUE_INPUT_SET_POINT_VIOLATION_L 0x03 // 3 � ��������� ������� L.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L 0x04 // 4 � ��������� ������� LL+L.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_H 0x05 // 5 � ��������� ������� H.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H 0x06 // 6 � ��������� ������� �+��.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK 0x07
#define ANALOGUE_INPUT_CHANNEL_CALIBRATION 0x20 // ����� ��������� � ������ ����������.
#define ANALOGUE_INPUT_LINE_BREAK 0x40 // ����� ����� ������.
#define ANALOGUE_INPUT_CALCULATION_OVERFLOW 0x80 // ������������ ��� ��������.
// �������� �� ����� ��������� ������� � ������� DI_value ��� ������ ����������� �����.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L_OFFSET 0 // �������� ������� LL+L.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET 1 // �������� ������� L.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET 2 // �������� ������� H.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H_OFFSET 3 // �������� ������� �+��.

// ���� ��������� ����������� ��������� ��� �������� � ������������� � ����.
#define HMI_ANALOGUE_INPUT_LINE_BREAK 1 // ����� ����� ������.
#define HMI_ANALOGUE_INPUT_CALCULATION_OVERFLOW 2 // ������������ ��� ��������.
#define HMI_ANALOGUE_INPUT_CHANNEL_CALIBRATION 3 // ����� ��������� � ������ ����������.
#define HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_L 4 // 3 � ��������� ������� L.
#define HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L 5 // 4 � ��������� ������� LL+L.
#define HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_H 6 // 5 � ��������� ������� H.
#define HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H 7 // 6 � ��������� ������� �+��.

////��� �������� �� eclipse.
//#define CONFIG_FILE_NAME "PasConfig.dat" // ��� ����� ������������.
//#define RESERV_CONFIG_FILE_NAME "ReservPasConfig.dat" // ��� ���������� ����� ������������.
//#define DATA_BASE_FILE_NAME "PasDataBase.dat" // ��� ����� ���� ������.
//#define RESERV_DATA_BASE_FILE_NAME "ReservPasDataBase.dat" // ��� ���������� ����� ���� ������.
// ��� ������������ linux.
#define CONFIG_FILE_NAME "/home/debian/PasConfig.dat" // ��� ����� ������������.
#define RESERV_CONFIG_FILE_NAME "/home/debian/ReservPasConfig.dat" // ��� ���������� ����� ������������.
#define DATA_BASE_FILE_NAME "/home/debian/PasDataBase.dat" // ��� ����� ���� ������.
#define RESERV_DATA_BASE_FILE_NAME "/home/debian/ReservPasDataBase.dat" // ��� ���������� ����� ���� ������.

// ��������� ������. �������� - �������� pas05_iz_1, ���. 109.
// ���������.
#define PREVIOUS_REQUES_WAS_NOT_A_BLOCK_TRANSFER 0x00
#define PROGRAMMING_DATA_BASE_WRITE_OK 0x01
// ���� ������.
// ���������� ������ �� ��� ��������� ����� �� (��� ������� � ����������� ���������������� �������� 14).
#define PREVIOUS_REQUES_WAS_NOT_A_BLOCK_TRANSFER_ERROR 0x01
#define PROGRAMMING_DATA_BASE_REPER_POINTS_ADC_MODULE_NOT_RESPOND 0x02
#define PROGRAMMING_DATA_BASE_BLOCK_CRC_ERROR 0x03
#define PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_EEPROM_ERROR 0x05
#define PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_NOT_CONFIRM 0x0C
#define PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_LOAD_ERROR 0x0B
#define PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_TXS_READ_ERROR 0x05
#define PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_MVDS9_EEPROM_ERROR 0x05
#define PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_PSP_EEPROM_ERROR 0x05
#define PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_DIMENSIONS_TEXT_TITLES_EEPROM_ERROR 0x05
#define PROGRAMMING_DATA_BASE_WRITE_BLOCK_BUSY 0x06
#define PROGRAMMING_LOCAL_PROGRAMMER_IS_CONNECTED 0x07
// �� ���������� ���� ������ ��� ���������������� ��.
// ���� �� ���������������� ����� ��� ������������ ������ � �����������.
// ���� �� ����������������� ������������ ��� ���������������� ������ "����".
// ���� �� ����������������� ������������ � ����� ��� ���������������� ������ "����".
#define PROGRAMMING_MR_DATA_BASE_NOT_CORRECT 0x0D
#define PROGRAMMING_MR_DATA_BASE_ERROR 0x0E

#define DATA_BASE_FLOW_CONTROL_OK 1
#define DATA_BASE_FLOW_CONTROL_ERROR 2

// ��������� - ������� ������ ������������.
#define CONFIGURATION_ERROR 0xE0

#define CONFIGURATION_FLOW_CONTROL_EXIT 0
#define CONFIGURATION_FLOW_CONTROL_BUS_CHECK 1
#define CONFIGURATION_FLOW_CONTROL_SEQUENCE_CHECK 2
#define CONFIGURATION_FLOW_CONTROL_SERVICE_DATA_CALCULATE 3
#define CONFIGURATION_FLOW_CONTROL_CONFIGURATION_CHECK 4
#define CONFIGURATION_FLOW_CONTROL_NORMAL_FLOW 5
#define CONFIGURATION_FLOW_CONTROL_ERROR_FLOW_STOP 6

// ��� ����������� ������������ ������������������ ������� �� SPI � ���������� ucInsideTypeIndex -
// ������� ����� ���������� ������� -
// ����������� �����, ��������� �����,
// ����������� ������, ��������� ������ � �.�.
// ucInsideTypeIndex ����� ��������� ��� ���������� ������� ������, ��� ������� � ����.
#define DISCRETE_INPUT_MODULE_GROUP 1
#define ANOLOGUE_INPUT_MODULE_GROUP 2
#define DISCRETE_OUTPUT_MODULE_GROUP 3
#define CURRENT_OUTPUT_MODULE_GROUP 4
#define EXTERNAL_1234_MODULE_GROUP 5
#define EXTERNAL_15_MODULE_GROUP 6

//#define GET_MODULE_TYPE_COMMAND 0x91 // ������� - ������ ���� ������.
//#define CHANGE_PROTOCOL_COMMAND 0x92 // ������� - ����������� �� ����� ��������.
//#define GET_MODULE_TYPE_COMMAND_LENGTH 1
//#define GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH 1
//// �������� � ������� RX SPI. 0 - ������� �������, 1 - "���" ������� �������.
//#define GET_MODULE_TYPE_COMMAND_OFFSET 1

#define AIN_OFF_PROCESS_OWNER_IS_NONE 0
#define AIN_OFF_PROCESS_OWNER_IS_PROGRAMMER 1
#define AIN_OFF_PROCESS_OWNER_IS_HMI 2


// (sizeof(struct TAnalogueInputDescriptionDataBase) * ANALOG_MODULE_INPUT_QUANTITY)// 28�6=168.
#define ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH 168
// (sizeof(struct TMrXXOneChannelDataBase) * DISCRETE_OUTPUT_NUMBER)// 3�64=192.
#define DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_LENGTH 192
// (sizeof(struct TMbmFunction1234PackOne) * EXTERNAL_MODULE_FUNCTION_1234_QUANTITY) + 1// 13�16=208.
#define MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_LENGTH (208 + 1)
// (sizeof(struct TMbmFunction15Sourse) * EXTERNAL_MODULE_FUNCTION_15_QUANTITY)// 72�3=216.
#define MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_LENGTH 216
// (sizeof(struct TDiscreteInputDescriptionWork) * DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)// 11�18=198.
#define DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH 192
// (sizeof(struct TAnalogueOutputRegulatorDescriptionDataBase) * ANALOG_OUTPUT_MODULE_REGULATORS_QUANTITY)// 28�6=168.
#define CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH 56
#define CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH 56
// DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY * 6// 6�16=96.
#define DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_LENGTH 96
// TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY * TEXT_TITLES_DATA_LENGTH// (384 ��, 24 �����).
#define TEXT_TITLES_DATA_BASE_BLOCK_LENGTH 224
// (sizeof(struct TFunctionBlockDescriptionWork) * FUNCTION_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)// 19�10=190.
#define FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH 160// 19�10=190 (120 ���, 12 ������)
#define MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_LENGTH 208// 82 � 84 (48 ���, 3 ����� 13*16=208 )
#define DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH 32// 2�16=32.
#define REFERENCE_POINTS_ADC_CODES_DATA_BASE_BLOCK_LENGTH 42
#define SERIAL_AND_ID_DATA_BASE_BLOCK_LENGTH 16
#define NETWORK_ADDRESS_DATA_BASE_BLOCK_LENGTH 1
#define CONFIGURATION_DATA_BASE_BLOCK_LENGTH 32
#define RESERVED_DATA_BASE_BLOCK_LENGTH 16


#define ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_QUANTITY 1
#define ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY 6
//#define DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_QUANTITY 1
//#define DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY MAX_HANDLED_DISCRETE_OUTPUT
//#define MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_QUANTITY 2
//#define MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY 16
//#define MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_QUANTITY EXTERNAL_MODULE_FUNCTION_15_QUANTITY
#define DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_QUANTITY 1
#define DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY 24
//#define CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_QUANTITY 2
//#define CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY ANALOG_OUTPUT_MODULE_REGULATORS_QUANTITY
#define DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY 16
#define DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_QUANTITY 1
#define TEXT_TITLES_DATA_BASE_BLOCK_QUANTITY 2
#define TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY 16
//#define TEXT_TITLES_DATA_LENGTH 14
//#define FUNCTION_BLOCK_DATA_BASE_BLOCK_QUANTITY 12
//#define FUNCTION_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY 10
//#define MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_QUANTITY 3
//#define MATHEMATICAL_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY 16
//#define DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_QUANTITY 1
//#define NETWORK_ADDRESS_DATA_BASE_BLOCK_QUANTITY 1
//#define FUNCTION_BLOCK_QUANTITY 1

// �������� � ������� aucCoilsArray[].
// ��������� ����� Modbus.
#define COILS_ARRAY_MODBUS_BEGIN_ADDRESS 0x00
// �������� � ������� aucDiscreteInputsArray[].
// ��������� ����� Modbus.
#define DISCRETE_INPUTS_ARRAY_MODBUS_BEGIN_ADDRESS 0x00
// �������� � ������� aucHoldingRegistersArray[].
// ��������� ����� Modbus.
#define HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS 0x00
// �������� � ������� aucInputRegistersArray[].
// ��������� ����� Modbus.
#define INPUT_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS 0x00


// 7.4.3 ����� �������� �������
// 1000-1006  7 ��������� �������� �������: �������, ������, ���, ���� ��-
// ����, �����, ��� (0-99), ���� ������ (1 � 7);
#define CURRENT_TIME_BYTE_ARRAY_OFFSET (1000 * sizeof(short))
// ������ ��� �������� ���� - short.
#define CURRENT_TIME_SECOND_OFFSET 0
#define CURRENT_TIME_MINUTE_OFFSET 1
#define CURRENT_TIME_HOUR_OFFSET 2
#define CURRENT_TIME_MONTH_DAY_OFFSET 3
#define CURRENT_TIME_MONTH_OFFSET 4
#define CURRENT_TIME_YEAR_OFFSET 5
#define CURRENT_TIME_WEEK_DAY_OFFSET 6
// � linux ����������� - 0.
#define LINUX_WEEK_DAY_SUNDAY 0
#define WEEK_DAY_SUNDAY 7

enum EDataBaseBlockOffsets
{
    ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET = 1,
    DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET = 12,
    FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET = 20,//20-31
    CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET = 32,
    CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_OFFSET = 33,
    DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET = 35,
    TEXT_TITLES_DATA_BASE_BLOCK_OFFSET = 40,//40,41
    REFERENCE_POINTS_ADC_CODES_DATA_BASE_BLOCK_OFFSET = 60,
    SERIAL_AND_ID_DATA_BASE_BLOCK_OFFSET = 97,
    NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET = 99,
    CONFIGURATION_DATA_BASE_BLOCK_OFFSET = 100,
};

//-------------------------------------------------------------------------------
enum
{
    COILS_WORK_ARRAY_LENGTH = 8000,
    DISCRETE_INPUTS_ARRAY_LENGTH = 8000,
    HOLDING_REGISTERS_ARRAY_LENGTH = 2000,
    INPUT_REGISTERS_ARRAY_LENGTH = 2000,
};

enum
{
    MODBUS_RTU_BAUD_RATE = 9600UL,
    MODBUS_RTU_MAX_ADU_LENGTH = 256,
    MAIN_CYCLE_PERIOD_TIME = 100,
    PROGRAMMING_TIME = 1000,
    PERIPHERY_SCAN_TIME = 10,

//    SPI_MT_PRIAMBLE_LENGTH = 2,
//    SPI_MT_FUNCTION_CODE_OFFSET = 0,
//    SPI_MT_LENGTH_OFFSET = 1,
//    SPI_MT_DATA_OFFSET = 2,
//    SPI_MT_RX_OFFSET = 1,
//
//    SPI_PREAMBLE_LENGTH = 2,
//    SPI_BUFFER_LENGTH = 64,
    SPI_MAX_BUS_ADDRESS = 1,
//// 2 - �������� �� ������ � ������� RX SPI. 0 - ������� �������, 1 - "���" ������� �������.
//    SPI_DATA_BYTE_OFFSET = 2,
//// 1 - �������� �� ������� � ������� RX SPI.
//    SPI_COMMAND_BYTE_OFFSET = 1,
//    SPI_COMMAND_BYTE_LENGTH = 1,
//    ONE_BYTE_CRC_LENGTH = 1,
//    TWO_BYTE_CRC_LENGTH = 2,
//    GET_MODULE_TYPE_COMMAND = 0x91,
//    GET_MODULE_TYPE_COMMAND_LENGTH = 1,
//    GET_MODULE_TYPE_REQUEST_LENGTH = 4,
//    GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH = 1,
//    MODULE_REQUEST_QUANTITY = 5,
//    MODULE_TYPE_OFFSET = SPI_DATA_BYTE_OFFSET,
//// ��������� �������� �������� �������� ������ ��� ������.
//// ���� ����� ���� - ������ ��������� �����������. ��� ������ ���������� ��������������.
//// ��������������� ���� ������������ � ������� ������� aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET + ������ ������].
//    BAD_MODULE_CYCLE_COUNT_DEFAULT = 10,
// �������� � ������������� ������ �� ��������� ����-���������� ��������� � 5 ������� ������.
    BIT_STATE_OFFSET = 0,
// �������� � ������������� ������ �� ������-�����, �������� ����������� �����.
    COMMON_INDEX_OFFSET = 0,
// �������� � ������������� ������ �� �������, �������� ��������� ���� ����������� �����.  .
    COMMON_COMMAND_CONTROL_OFFSET = 1,
};

enum EModbusFunctionCoilsDataMap
{
    ANALOGUE_INPUTS_BAD_STATE_OFFSET = 544,
    DISCRETE_INPUTS_BAD_STATE_OFFSET = 1056,
    // � ���� ����������� ���������� ����� ������� ������ ��������������� ���������� ������
    // � ������ ��������������� ���������� �������� ����������� ����������� ������� ���������
    ANALOGUE_INPUTS_DISCRETE_SIGNALS_BAD_STATE_OFFSET = 1056,
};

enum EModbusFunctionDiscreteInputsDataMap
{
    DISCRETE_INPUTS_STATE_OFFSET = 0,
};

enum EModbusFunctionHoldingRegistersDataMap
{
    ANALOGUE_INPUTS_VALUE_OFFSET = 0,
    CURRENT_TIME_OFFSET_HOLDING_REGISTERS = 1000,
};

enum EModbusFunctionInputRegistersDataMap
{
//    DISCRETE_INPUTS_STATE_OFFSET = 0,
    CURRENT_TIME_OFFSET_INPUT_REGISTERS = 1000,
};

enum TYPE_INDICATION
{
    NORMAL		 = 0,			// ��� �����������
    PREVENTIVE	 = 1,			// ����������������� ������������
    EMERGENCY	 = 2,			// ��������� ������������
    INDICATION	 = 3,			// ���������

    IND_PREVENTIVE = 4,			// ��������� + ����������������� ������������
    IND_EMERGENCY  = 5,			// ��������� + ��������� ������������
    NAMUR_INDICATION = 6,			// ��������� ��������������� ������� namur.
    ERROR  = 7,			// ���� ������� ������.
    BEEP_SIGNAL  = 8,			// ���� ������� ������.
};

enum TYPE_ALARM_STATE
{
    ALARM_STATE_IS_NORMAL		 = 0,			// �����
    ALARM_STATE_IS_RECEIPTED	 = 1,			// ������������ �����������
    ALARM_STATE_IS_ACTIVE	 = 2,			// ������������ �������
    ALARM_STATE_IS_INDICATION	 = 3,			// ���������
};

enum Errors
{
    NO_ERROR	= 0,
    IMD_ERROR	= 1,		// ������ ����� � �������� �����
    OMD_ERROR	= 2,		// ������ ����� � �������� ������
    CFG_ERROR	= 3,		// ������ ������������
    DB_ERROR	= 4,		// ������ ��
    MBS_ERROR	= 5,		// ������ ������ �����
};

enum INPUT_STATE_CODE
{
    OFF		= 0x00,			// ���������� �������
    ON		= 0x01,			// ������������ �������
    WIRE_BREAK	= 0x02,			// ����� �����
    SHORT_CIRCUIT	= 0x03,			// �������� ���������
    INPUT_IS_INVALID	= 0x04,			// ���� ������������
};


//-------------------------------------------------------------------------------
#endif // CONFIGURATION_H_INCLUDED
