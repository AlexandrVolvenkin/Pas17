#ifndef PASNEWCONFIG_H_INCLUDED
#define PASNEWCONFIG_H_INCLUDED

// ��������� - ���������.
// PAS_05_PO.DOC, PAS_05_NEW_PO.DOC, �������� ���.txt, ������ 71.txt,
// ���-19_������ Modbus_FULL.txt ���-05-8(16)CD_��_�2_��.pdf, ���-05_�3_10 2013.pdf, ���-19 06.05.2019.pdf, pas05_iz_1.pdf.

using namespace std;

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <assert.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <linux/types.h>
#include <linux/serial.h>
#include <linux/spi/spidev.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <netinet/in.h>
#include <linux/reboot.h>
#include <sys/reboot.h>
#include <sched.h>
#include <linux/kernel.h>
#include <mtd/mtd-user.h>
#include <time.h>
#include <sys/time.h>		/* for setitimer */
#include <unistd.h>		/* for pause */
#include <signal.h>		/* for signal */
#include <sys/types.h>
#include <semaphore.h>
//#include <linux/printk.h>
//#include <linux/module.h>
//#include <linux/init.h>


//#include <linux/init.h>
//#include <linux/module.h>
//#include <linux/kernel.h>
//#include <linux/gpio.h>                 // Required for the GPIO functions
//#include <linux/interrupt.h>            // Required for the IRQ code

#include "../src/Modbus/modbus.h"
//#include "../src/Modbus/modbus-tcp.h"
//#include "../src/Modbus/modbus-rtu.h"

//#include "../src/sqlite-amalgamation-3330000/sqlite3.h"
//#include "../src/sqlite-amalgamation-3330000/sqlite3ext.h"

#include "SerialMT.h"
#include "STEP5_floating_point.h"

#include "ModuleMvdi5.h"
#include "ModuleMvds9.h"
#include "ModuleMvai5.h"
#include "ModuleMrXX.h"
#include "ModuleMtvi5.h"

#include "MbmFunction1.h"
#include "MbmFunction2.h"
#include "MbmFunction3.h"
#include "MbmFunction4.h"
#include "MbmFunction15.h"

#include "Auxiliary.h"
#include "Timer.h"
#include "Crc.h"
#include "DeviceControl.h"
#include "DiscreteInput.h"
#include "FunctionBlocks.h"
//#include "Alarm.h"
#include "PlcErrorTest.h"
#include "Platform.h"

// modbus test
/* Constants defined by configure.ac */
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#ifdef HAVE_STDINT_H
# ifndef _MSC_VER
# include <stdint.h>
# else
# include "stdint.h"
# endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// �������������� ������������ ����� � �����.
#define CONVERT_NATURAL_NUMBER_TO_INTEGER 1
// �������������� ������ ����� � �����������.
#define CONVERT_INTEGER_TO_NATURAL_NUMBER 1
#define END_OF_STRING 0
#define END_OF_STRING_LENGTH 1

//-----------------------------------------------------------------------------------------------------
// ������� ��������� �������. ���������� ������� ���� ��������.
#define MAX_HANDLED_DISCRETE_INPUT 384 // ������������ ������ ������ �������������.
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
#define MAX_HANDLED_ANALOGUE_INPUT 96
#define MAX_HANDLED_ANALOGUE_OUTPUT 12
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
#define INTERNAL_MODULE_QUANTITY 16
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
#define DISCRETE_MODULE_INPUT_QUANTITY 12
#define ANALOGUE_INPUT_DI_VALUE_QUANTITY 4 // ���������� ���������� �������� ����������� ����� ���������� ������.
#define DISCRETE_OUTPUT_MODULE_RELAY_OUTPUT_QUANTITY 64 // ���������� ���������� ������� ���������� ����.
// ���������� ���������� ������ � ����� ������.
#define ANALOG_MODULE_INPUT_QUANTITY 6
// ����� ������ ���������� ��������� ����������� ���������.
#define ANALOGUE_INPUT_NAME_LENGTH 8
// ����� ������ ���������� ��������� ����������� ����������� ��������� ����.
#define DIMENSIONS_PARAMETERS_NAME_LENGTH 6
// ����� ������ ���������� ��������� ����������� �������.
#define DISCRETE_SYGNAL_NAME_LENGTH 14
// ���������� ���������� ������� � ����� ������.
#define ANALOG_OUTPUT_MODULE_REGULATORS_QUANTITY 6
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

//-----------------------------------------------------------------------------------------------------
#define MAIN_CYCLE_TIME 100000000 // ����� �������� ����� - 100mc.
//#define MAIN_CYCLE_TIME 100000 // ����� �������� ����� - 100mc.
#define EXTERNAL_CYCLE_STOP_SLEEP_TIME 1000 // ������������ "���", �� ����� �������� ����� ������������� � ������� ������.

//#define MAIN_DATA_ACCESS_SEMAPHORE_NAME "/MainDataAccessSemaphore"
//#define MAIN_DATA_ACCESS_MUTEX_NAME "/MainDataAccessMutex"

#define ONE_SECOND_TIME 10 // ���������� �������� �������� ����� - MAIN_CYCLE_TIME.
#define TWO_SECOND_TIME 20
#define THREE_SECOND_TIME 30
#define TEN_SECOND_TIME 100

#define MODULE_REQUEST_QUANTITY 5 // ���������� ������� ��������� � �������.
#define MBM_1234_MODULE_BAD_BIT_ARRAY_LENGTH 1
#define BITS_IN_BYTE_QUANTITY 8
// ��������� �������� �������� �������� ������ ��� ������.
// ���� ����� ���� - ������ ��������� �����������. ��� ������ ���������� ��������������.
// ��������������� ���� ������������ � ������� ������� aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET + ������ ������].
#define BAD_MODULE_CYCLE_COUNT_DEFAULT 10
// ��������� �������� �������� �������� ������ ��� ������ � ������� 60 ������.
#define BAD_MODULE_60_SECOND_CYCLE_COUNT_DEFAULT 600
#define BAD_MODULE_RESPONDED_OK 0
// ��� ������ ����� ������ �� ��������.
// ������ �� ��������.
#define BAD_MODULE_NOT_RESPONDED 1
// ������ �� �������� ��� ������.
#define BAD_MODULE_NOT_RESPONDED_ON_START 2
// ��������� ������. ��������������� ���� ���������� �������� ������������.
#define ANALOGUE_INPUT_ERROR 0xFF00

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


#define ONE_BYTE_CRC_LENGTH 1
#define TWO_BYTE_CRC_LENGTH 2
// �������� �� ����������� ����� ����� ���� ������.
#define PLC_DATA_BASE_BLOCK_CRC_OFFSET (PLC_DATA_BASE_BLOCK_LENGTH - TWO_BYTE_CRC_LENGTH)
// 2 - �������� �� ��� ������ � ������� RX SPI. 0 - ������� �������, 1 - "���" ������� �������.
#define MODULE_TYPE_OFFSET 2
#define MODULE_COMMAND_LENGTH 1
// ���������� ���� � ������ �� ������ ���� ������.
#define MODULE_COMMAND_ANSWER_LENGTH 1
// ���������� ���� � "������" ������ ������. 0x7E - OK, 0xE7 - error.
#define TAIL_ANSWER_LENGTH 1
#define DATA_EXCHANGE_OK 0x7E
#define DATA_EXCHANGE_ERROR 0xE7
#define DATA_EXCHANGE_CRC_ERROR 0x03
// MVAI5_REPER_POINTS_ADC_CHANNEL_QUANTITY + MVAI5_ANALOG_INPUT_QUANTITY// 6�7=42.
#define ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH 42

// ������������ ���������� ������� �� ���� SPI.
#define SPI_CHIP_SELECT_MAX_ADDRESS 16
// 2 - �������� �� ������ � ������� RX SPI. 0 - ������� �������, 1 - "���" ������� �������.
#define SPI_DATA_BYTE_OFFSET 2
// 1 - �������� �� ������� � ������� RX SPI.
#define SPI_COMMAND_BYTE_OFFSET 1
#define SPI_COMMAND_BYTE_LENGTH 1

#define SPI_LENGTH_BYTE_OFFSET 2
#define SPI_LENGTH_BYTE_LENGTH 1
// ������ ���� - �������, ������ - �����, ������ ������.
#define SPI_PREAMBLE_LENGTH 2

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

#define GET_MODULE_TYPE_COMMAND 0x91 // ������� - ������ ���� ������.
#define CHANGE_PROTOCOL_COMMAND 0x92 // ������� - ����������� �� ����� ��������.
#define GET_MODULE_TYPE_COMMAND_LENGTH 1
#define GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH 1
// �������� � ������� RX SPI. 0 - ������� �������, 1 - "���" ������� �������.
#define GET_MODULE_TYPE_COMMAND_OFFSET 1

//-----------------------------------------------------------------------------------------------------
// MIND
// ���� ���� ������.
// ��������. //?
#define MODULE_MIND_MODBUS_ADDRESS 0x11
#define MODULE_TYPE_MIND 0x11
#define MODULE_MIND_INDEX (INTERNAL_MODULE_QUANTITY + EXTERNAL_MODULE_QUANTITY)
// $42 - ������ �� �������� (�����������) ����� �������.
#define MIND_DATA_EXCHANGE_COMMAND 0x42
#define MIND_DATA_EXCHANGE_READY 0x42
#define MIND_DATA_EXCHANGE_NOT_READY 0x24
#define MIND_DATA_EXCHANGE_COMMAND_ANSWER_LENGTH 5

#define MIND_INDICATION_MODULE_TYPE_12_CELLS_LCD_4x24 0x88
#define MIND_INDICATION_MODULE_TYPE_24_CELLS_LCD_4x24 0x89
#define MIND_INDICATION_MODULE_TYPE_LCD_320x240 0x8A
#define MIND_INDICATION_MODULE_TYPE_LCD_4x24 0x8B

//-----------------------------------------------------------------------------------------------------
// ����� ������ ���� ������. �������� - �������� PAS_05_PO, ��� 22, ����� 5.3.3. � PAS_05_NEW_PO.DOC ���. 19.
// �������� �� ����� � ���� ������.
// 1-16 - �� ������� ����������� ����� 	28�6=168.
#define ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET 1
// 17   -  �� ������� ����������� ������ 	3�64=192.
#define DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_OFFSET 17
// 18-19  - �� ������� ��� ����� ���. 	13�16=208.
#define MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_OFFSET 18
// 20  - �� ������� ��� ������ �15	72�3=216.
#define MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_OFFSET 20
// 21-42 �� ���������� ������� ��������	11�18=198.
#define DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET 21
// 43-44    �� ��� ����			28�6=168.
#define CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET 43
// 45    �� ������������			6�16=96.
#define DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET 45
// 46-69 �� ��������� ���������� (384 ��, 24 �����).
#define TEXT_TITLES_DATA_BASE_BLOCK_OFFSET 46
// 70-81 �� ���				19�10=190 (120 ���, 12 ������)
#define FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET 70
// 82 � 84 (48 ���, 3 ����� 13*16=208 )
#define MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_OFFSET 82
// 98  - �� ������� ����9-NAMUR 	2�16=32.
#define DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET 98
// 99  - ������� � ����������		1
#define NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET 99

#define ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_QUANTITY 16
#define ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY ANALOG_MODULE_INPUT_QUANTITY
#define DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_QUANTITY 1
#define DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY MAX_HANDLED_DISCRETE_OUTPUT
#define MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_QUANTITY 2
#define MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY 16
#define MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_QUANTITY EXTERNAL_MODULE_FUNCTION_15_QUANTITY
#define DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_QUANTITY 22
#define DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY 18
#define CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_QUANTITY 2
#define CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY ANALOG_OUTPUT_MODULE_REGULATORS_QUANTITY
#define DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY 16
#define DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_QUANTITY 1
#define TEXT_TITLES_DATA_BASE_BLOCK_QUANTITY 24
#define TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY 16
#define TEXT_TITLES_DATA_LENGTH 14
#define FUNCTION_BLOCK_DATA_BASE_BLOCK_QUANTITY 12
#define FUNCTION_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY 10
#define MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_QUANTITY 3
#define MATHEMATICAL_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY 16
#define DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_QUANTITY 1
#define NETWORK_ADDRESS_DATA_BASE_BLOCK_QUANTITY 1
#define FUNCTION_BLOCK_QUANTITY 1

// (sizeof(struct TAnalogueInputDescriptionDataBase) * ANALOG_MODULE_INPUT_QUANTITY)// 28�6=168.
#define ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH 168
// (sizeof(struct TMrXXOneChannelDataBase) * DISCRETE_OUTPUT_MODULE_RELAY_OUTPUT_QUANTITY)// 3�64=192.
#define DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_LENGTH 192
// (sizeof(struct TMbmFunction1234PackOne) * EXTERNAL_MODULE_FUNCTION_1234_QUANTITY) + 1// 13�16=208.
#define MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_LENGTH (208 + 1)
// (sizeof(struct TMbmFunction15Sourse) * EXTERNAL_MODULE_FUNCTION_15_QUANTITY)// 72�3=216.
#define MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_LENGTH 216
// (sizeof(struct TDiscreteInputDescriptionWork) * DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)// 11�18=198.
#define DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH 198
// (sizeof(struct TAnalogueOutputRegulatorDescriptionDataBase) * ANALOG_OUTPUT_MODULE_REGULATORS_QUANTITY)// 28�6=168.
#define CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH 168
#define CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH 168
// DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY * 6// 6�16=96.
#define DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_LENGTH 96
// TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY * TEXT_TITLES_DATA_LENGTH// (384 ��, 24 �����).
#define TEXT_TITLES_DATA_BASE_BLOCK_LENGTH 224
// (sizeof(struct TFunctionBlockDescriptionWork) * FUNCTION_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)// 19�10=190.
#define FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH 190// 19�10=190 (120 ���, 12 ������)
#define MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_LENGTH 208// 82 � 84 (48 ���, 3 ����� 13*16=208 )
#define DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH 32// 2�16=32.
#define NETWORK_ADDRESS_DATA_BASE_BLOCK_LENGTH 1
#define RESERVED_DATA_BASE_BLOCK_LENGTH 1

//-----------------------------------------------------------------------------------------------------
// ����� ������ ������� aucCoilsArray[] modbus, function 1.
// 7.1 ������� 1 � ������ �������� ��������� ���������� �������� ��������:
// ������������ ������ ������ �������������. �������� - �������� ���-19_������ Modbus_FULL � ���-05-8(16)CD_��_�2_��.

// 00-63 (0x00 - 0x3f)  ��������� ������� �� �1 � MP �8: 0-OFF.1-ON.(64 ������);
#define SOST_DO_BIT_ARRAY_LENGTH 64
// 64 - ������� ������ ������������.
#define CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_LENGTH 1
// 65-95 ($41 - $5f) ������ ���������� �����(31);
#define RESERVED_1_BIT_ARRAY_LENGTH 31
// 96-107 ($60 - $6b) ����� ���������� SP/OUT ���-��� ���� (12 ��);
#define INCREMENT_SP_OUT_BIT_ARRAY_LENGTH 12
// 108-119($6c - $77) ����� ���������� SP/OUT ���-��� ���� (12 ��);
#define DECREMENT_SP_OUT_BIT_ARRAY_LENGTH 12
// 120, 121 (0x78, 0x79) ���� ���������� ������������ ��������� ������� ���-
// 05 (�����/������) � ��������� ������������: 00 � �����, 01 � ������������� ���������-
// ���, 10 � �������� ������������;
#define MODULES_ALARM_STATE_BITS_BIT_ARRAY_LENGTH 2
// 122 (0x7a)   ������ ������ ������������ ��� ������� (������);
#define CONFIGURATION_ERROR_SIGNAL_BIT_ARRAY_LENGTH 1
// 123,124(0x7b, 0x7c) ����� ����������� ������ ��������� ��� HMI: 123 -
// �����������������, 124-���������;
#define HMI_COMMON_INTERMITTENT_SIGNAL_BITS_BIT_ARRAY_LENGTH 2
// 125 (0x7d)   ������������� ������ ���������� �0�-��� / �1�-����;
#define LOCK_MODE_SWITCH_BIT_ARRAY_LENGTH 1
// 126,127 (0x7e,0x7f) ������� ������������ � �����;
#define KVIT_COMMAND_BIT_ARRAY_LENGTH 1
#define RESET_COMMAND_BIT_ARRAY_LENGTH 1
// 2 ���� �� ������.
// 00 � �����, 01 � ������������� ������������, 10 � �������� ������������.
// 128-159($80 - $9f) ��1-��16 �� 2 ���� (32);
#define ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_LENGTH 32
// ����� ���� ������������� ��� �������� ������������ ������ ����������� ������� � �����.
#define ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT 2
// 2 ���� �� ������.
// 00 � �����, 01 � ������������� ������������, 10 � �������� ������������.
// 160-927; ������ ���������� ������������ ���������� ��������
// ��1-��384, �� 2 ���� �� ������ (768);
#define ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH (MAX_HANDLED_DISCRETE_INPUT * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT)
// 928-1167; ������ ���������� �������������� ������
// ��1-��120 �� 2 ���� (340);
#define ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_LENGTH (MAX_HANDLED_FUNCTION_BLOCK * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT)
// 1168-1263 AIN1-AIN96 ������ ������ ��������������� ���������� ��������,
// �� 1 ���� �� ������ (96);
#define BAD_AI_BIT_ARRAY_LENGTH MAX_HANDLED_ANALOGUE_INPUT
// 1264-1359 AIN1-AIN96 ������ ������ ���������� ���������� �������� �� ��-
// ������� �� 1 ���� �� ������ (96);
#define AIN_OFF_BIT_ARRAY_LENGTH MAX_HANDLED_ANALOGUE_INPUT
// 1360-1455 ���1-���96 ������ ��������� ����������� �CONT_ST� �� 1 ����� �� ���������, ����� 96 ������.
#define CONT_ST_BIT_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_OUTPUT * 8)
// ���� ������ ��������� ������ ��� 6 �����������, �� ������ ����� �� ��������� - 8 ��� ��� �������.
#define CONT_ST_BYTE_ARRAY_LENGTH MAX_HANDLED_ANALOGUE_OUTPUT
// 1456-1575 Log_Com ������� ������� ���������� ������� � 120.
#define LOG_COM_BIT_ARRAY_LENGTH 120

// 1576-1695 ������ �������������� ������ �� ������� ����(120
// ���).
#define CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_LENGTH MAX_HANDLED_FUNCTION_BLOCK
// 1696-1735 16 ������� + 24 ���.(�� 1 ����) ����� ������� �������	(40):
#define BAD_MODULES_BIT_ARRAY_LENGTH 40
// 1736-2119 ��1-��384 ������ ������ ��������������� ���������� ��������.
#define BAD_DI_BIT_ARRAY_LENGTH MAX_HANDLED_DISCRETE_INPUT
// 2120-2359 �����. ������ CoDeSyst �� ���.����(240).
#define LOG_SOST_BIT_ARRAY_LENGTH 240



// DO_value	���������� ����������� �������� � 64.
#define DO_VALUE_BIT_ARRAY_LENGTH 64
// Stat_AI		��������� ���������� ������������� ������� � ����� � 384.
#define STAT_AI_BIT_ARRAY_LENGTH MAX_HANDLED_ANALOGUE_INPUT * STAT_AI_BIT_QUANTITY
// Dv_res		������ 160 ������� ����������.
#define DEV_RES_BIT_ARRAY_LENGTH 160
// ��������� ������� ���� �� ��,����� (128)| 2360-2487
// 8 ������� �� 8 ��������, ��� ������� ������ 2 ���� ���������� - 128 ���.
#define STAT_DO_BIT_ARRAY_LENGTH 128
// ������������� ���������� ������� �� (64)| 2488-2551
#define BAD_DO_BIT_ARRAY_LENGTH 64
// ��������� ������� ������� ���� (12) 	 | 2552-2563
#define BAD_AO_BIT_ARRAY_LENGTH 12
// Stat_AI		��������� ���������� ������������� ������� � ������ � 96.
// ���� ������ ����� ������ ��� 6 �������, �� ������ ����� �� ����� - 8 ��� ��� �������. ����� 8 �������.
// 6x8=48
#define STAT_AI_BYTE_ARRAY_LENGTH MAX_HANDLED_ANALOGUE_INPUT


// ������ ������� aucCoilsArray[].
#define COILS_FULL_ARRAY_LENGTH 8192
//#define COILS_WORK_ARRAY_LENGTH (SOST_DO_BIT_ARRAY_LENGTH + \
//                           BAD_AI_BIT_ARRAY_LENGTH + \
//                           BAD_DI_BIT_ARRAY_LENGTH + \
//                           LOG_COM_BIT_ARRAY_LENGTH + \
//                           DO_VALUE_BIT_ARRAY_LENGTH + \
//                           LOG_SOST_BIT_ARRAY_LENGTH + \
//                           DECREMENT_SP_OUT_BIT_ARRAY_LENGTH + \
//                           STAT_AI_BIT_ARRAY_LENGTH + \
//                           BAD_MODULES_BIT_ARRAY_LENGTH + \
//                           AIN_OFF_BIT_ARRAY_LENGTH + \
//                           DEV_RES_BIT_ARRAY_LENGTH)   \

// ������������ �����: ������� - 0x12XX.
#define COILS_WORK_ARRAY_LENGTH 8192
//#define COILS_WORK_ARRAY_LENGTH (SOST_DO_BIT_ARRAY_LENGTH + \
//                                RESERVED_1_BIT_ARRAY_LENGTH + \
//                                INCREMENT_SP_OUT_BIT_ARRAY_LENGTH + \
//                                DECREMENT_SP_OUT_BIT_ARRAY_LENGTH + \
//                                MODULES_ALARM_STATE_BITS_BIT_ARRAY_LENGTH + \
//                                CONFIGURATION_ERROR_SIGNAL_BIT_ARRAY_LENGTH + \
//                                HMI_COMMON_INTERMITTENT_SIGNAL_BITS_BIT_ARRAY_LENGTH + \
//                                LOCK_MODE_SWITCH_BIT_ARRAY_LENGTH + \
//                                KVIT_COMMAND_BIT_ARRAY_LENGTH + \
//                                RESET_COMMAND_BIT_ARRAY_LENGTH + \
//                                ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_LENGTH + \
//                                ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH + \
//                                ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_LENGTH + \
//                                BAD_AI_BIT_ARRAY_LENGTH + \
//                                AIN_OFF_BIT_ARRAY_LENGTH + \
//                                DECREMENT_SP_OUT_BIT_ARRAY_LENGTH + \
//                                HIGH_LEVEL_COMMAND_BIT_ARRAY_LENGTH + \
//                                CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_LENGTH + \
//                                BAD_MODULES_BIT_ARRAY_LENGTH + \
//                                BAD_DI_BIT_ARRAY_LENGTH + \
//                                LOG_SOST_BIT_ARRAY_LENGTH + \
//                                LOG_COM_BIT_ARRAY_LENGTH + \
//                                DO_VALUE_BIT_ARRAY_LENGTH + \
//                                STAT_AI_BIT_ARRAY_OFFSET + \
//                                DEV_RES_BIT_ARRAY_LENGTH + \
//                                BAD_DO_BIT_ARRAY_LENGTH + \
//                                STAT_DO_BIT_ARRAY_LENGTH + \
//                                STAT_AI_BYTE_ARRAY_LENGTH)   \
// �������� � ������� aucCoilsArray[].
// ��������� ����� Modbus.
#define COILS_ARRAY_MODBUS_BEGIN_ADDRESS 0x00
// 00-63 (0x00 - 0x3f)  ��������� ������� �� �1 � MP �8: 0-OFF.1-ON.(64 ������);
#define SOST_DO_BIT_ARRAY_OFFSET 0
// 64 - ������� ������ ������������.
#define CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_OFFSET (SOST_DO_BIT_ARRAY_OFFSET + SOST_DO_BIT_ARRAY_LENGTH)
// 65-95 ($41 - $5f) ������ ���������� �����(31);
#define RESERVED_1_BIT_ARRAY_OFFSET (CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_OFFSET + CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_LENGTH)
// 96-107 ($60 - $6b) ����� ���������� SP/OUT ���-��� ���� (12 ��);
#define INCREMENT_SP_OUT_BIT_ARRAY_OFFSET (RESERVED_1_BIT_ARRAY_OFFSET + RESERVED_1_BIT_ARRAY_LENGTH)
// 108-119($6c - $77) ����� ���������� SP/OUT ���-��� ���� (12 ��);
#define DECREMENT_SP_OUT_BIT_ARRAY_OFFSET (INCREMENT_SP_OUT_BIT_ARRAY_OFFSET + INCREMENT_SP_OUT_BIT_ARRAY_LENGTH)
// 120, 121 (0x78, 0x79) ���� ���������� ������������ ��������� ������� ���-
// 05 (�����/������) � ��������� ������������: 00 � �����, 01 � ������������� ���������-
// ���, 10 � �������� ������������;
#define MODULES_ALARM_STATE_BITS_BIT_ARRAY_OFFSET (DECREMENT_SP_OUT_BIT_ARRAY_OFFSET + DECREMENT_SP_OUT_BIT_ARRAY_LENGTH)
// 122 (0x7a)   ������ ������ ������������ ��� ������� (������);
#define CONFIGURATION_ERROR_SIGNAL_BIT_ARRAY_OFFSET (MODULES_ALARM_STATE_BITS_BIT_ARRAY_OFFSET + MODULES_ALARM_STATE_BITS_BIT_ARRAY_LENGTH)
// 123,124(0x7b, 0x7c) ����� ����������� ������ ��������� ��� HMI: 123 -
// �����������������, 124-���������;
// 00 � �����, 01 � ������������� ������������, 10 � �������� ������������.
#define HMI_COMMON_INTERMITTENT_SIGNAL_BITS_BIT_ARRAY_OFFSET (CONFIGURATION_ERROR_SIGNAL_BIT_ARRAY_OFFSET + CONFIGURATION_ERROR_SIGNAL_BIT_ARRAY_LENGTH)
// 125 (0x7d)   ������������� ������ ���������� �0�-��� / �1�-����;
#define LOCK_MODE_SWITCH_BIT_ARRAY_OFFSET (HMI_COMMON_INTERMITTENT_SIGNAL_BITS_BIT_ARRAY_OFFSET + HMI_COMMON_INTERMITTENT_SIGNAL_BITS_BIT_ARRAY_LENGTH)
// 126,127 (0x7e,0x7f) ������� ������������ � �����;
#define KVIT_COMMAND_BIT_ARRAY_OFFSET (LOCK_MODE_SWITCH_BIT_ARRAY_OFFSET + LOCK_MODE_SWITCH_BIT_ARRAY_LENGTH)
#define RESET_COMMAND_BIT_ARRAY_OFFSET (KVIT_COMMAND_BIT_ARRAY_OFFSET + KVIT_COMMAND_BIT_ARRAY_LENGTH)
// 128-159($80 - $9f) ��1-��16 �� 2 ���� (32);
#define ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_OFFSET (RESET_COMMAND_BIT_ARRAY_OFFSET + RESET_COMMAND_BIT_ARRAY_LENGTH)
// 160-927; ������ ���������� ������������ ���������� ��������
// ��1-��384, �� 2 ���� �� ������ (768);
#define ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_OFFSET (ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_OFFSET + ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_LENGTH)
// 928-1167; ������ ���������� �������������� ������
// ��1-��120 �� 2 ���� (340);
#define ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_OFFSET (ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_OFFSET + ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH)
// 1168-1263 AIN1-AIN96 ������ ������ ��������������� ���������� ��������,
// �� 1 ���� �� ������ (96);
#define BAD_AI_BIT_ARRAY_OFFSET (ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_OFFSET + ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_LENGTH)
// 1264-1359 AIN1-AIN96 ������ ������ ���������� ���������� �������� �� ��-
// ������� �� 1 ���� �� ������ (96);
#define AIN_OFF_BIT_ARRAY_OFFSET (BAD_AI_BIT_ARRAY_OFFSET + BAD_AI_BIT_ARRAY_LENGTH)
// 1360-1455 ���1-���96 ������ ��������� ����������� �CONT_ST� �� 1 ����� �� ���������, ����� 96 ������.
#define CONT_ST_BIT_ARRAY_OFFSET (AIN_OFF_BIT_ARRAY_OFFSET + AIN_OFF_BIT_ARRAY_LENGTH)
// 1456-1575 Log_Com ������� ������� ���������� ������� � 120.
#define LOG_COM_BIT_ARRAY_OFFSET (CONT_ST_BIT_ARRAY_OFFSET + CONT_ST_BIT_ARRAY_LENGTH)

// 1456-1575 ������� ���������� � �������� ������ ��� (120 ���);
#define HIGH_LEVEL_COMMAND_BIT_ARRAY_OFFSET (CONT_ST_BIT_ARRAY_OFFSET + CONT_ST_BIT_ARRAY_LENGTH)

// 1576-1695 ������ �������������� ������ �� ������� ����(120
// ���).
#define CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_OFFSET (LOG_COM_BIT_ARRAY_OFFSET + LOG_COM_BIT_ARRAY_LENGTH)
// 1696-1735 16 ������� + 24 ���.(�� 1 ����) ����� ������� �������	(40):
#define BAD_MODULES_BIT_ARRAY_OFFSET (CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_OFFSET + CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_LENGTH)
// 1736-2119 ��1-��384 ������ ������ ��������������� ���������� ��������.
#define BAD_DI_BIT_ARRAY_OFFSET (BAD_MODULES_BIT_ARRAY_OFFSET + BAD_MODULES_BIT_ARRAY_LENGTH)
// 2120-2359 �����. ������ CoDeSyst �� ���.����(240).
#define LOG_SOST_BIT_ARRAY_OFFSET (BAD_DI_BIT_ARRAY_OFFSET + BAD_DI_BIT_ARRAY_LENGTH)

//// DO_value	���������� ����������� �������� � 64.
//#define DO_VALUE_BIT_ARRAY_OFFSET (LOG_SOST_BIT_ARRAY_OFFSET + LOG_SOST_BIT_ARRAY_LENGTH)
//// Stat_AI		��������� ���������� ������������� ������� � ����� � 384.
//#define STAT_AI_BIT_ARRAY_OFFSET (DO_VALUE_BIT_ARRAY_OFFSET + DO_VALUE_BIT_ARRAY_LENGTH)
//// Dv_res		������ 160 ������� ����������.
//#define DEV_RES_BIT_ARRAY_OFFSET (STAT_AI_BIT_ARRAY_OFFSET + STAT_AI_BIT_ARRAY_LENGTH)
// ��������� ������� ���� �� ��,����� (128)| 2360-2487
#define STAT_DO_BIT_ARRAY_OFFSET (LOG_SOST_BIT_ARRAY_OFFSET + LOG_SOST_BIT_ARRAY_LENGTH)
// ������������� ���������� ������� �� (64)| 2488-2551
#define BAD_DO_BIT_ARRAY_OFFSET (STAT_DO_BIT_ARRAY_OFFSET + STAT_DO_BIT_ARRAY_LENGTH)
// ��������� ������� ������� ���� (12) 	 | 2552-2563
#define BAD_AO_BIT_ARRAY_OFFSET (BAD_DO_BIT_ARRAY_OFFSET + BAD_DO_BIT_ARRAY_LENGTH)

//-----------------------------------------------------------------------------------------------------
// ����� ������ ������� aucDiscreteInputsArray[] modbus, function 2.
// 7.2 ������� 2� ������ �������� ��������� ���������� ������� ��������
// ������������ ������ ������ �������������. �������� - �������� ���-19_������ Modbus_FULL � ���-05-8(16)CD_��_�2_��.

// 00-383 Din1 - DIN384:  ��������� ������� ���������� ��������: 0-OFF, 1-ON,
// ��� ����� ������������ �� 384 ���������� ��������;
#define DI_VALUE_BIT_ARRAY_LENGTH MAX_HANDLED_DISCRETE_INPUT
// ������ ������� aucDiscreteInputsArray[].
#define DISCRETE_INPUTS_ARRAY_LENGTH 8192
//#define DISCRETE_INPUTS_ARRAY_LENGTH DI_VALUE_BIT_ARRAY_LENGTH
// �������� � ������� aucDiscreteInputsArray[].
// ��������� ����� Modbus.
#define DISCRETE_INPUTS_ARRAY_MODBUS_BEGIN_ADDRESS 0x00
// 00-383 Din1 - DIN384:  ��������� ������� ���������� ��������: 0-OFF, 1-ON,
// ��� ����� ������������ �� 384 ���������� ��������;
#define DI_VALUE_BIT_ARRAY_OFFSET 0

//-----------------------------------------------------------------------------------------------------
// ����� ������ ������� aucHoldingRegistersArray[] modbus, function 3.
// 7.3 ������� 3 � ������ ������ � ������� float IEEE 754 (�� 2 2-� ������� ����
// ���� �� 1 ��������).
// ������������ ������ ������ �������������. �������� - �������� ���-19_������ Modbus_FULL � ���-05-8(16)CD_��_�2_��.

// ������� ���� BigEndian, ��� �������� �� ����.

// 7.3.1 ���������� ���������.
// 00-190 Ain1 - �in96 ���������� �������� ������� ���������� ��������,
// ��� ����� ������������ �� 192 2-� ������� ��������� (96 ���������� ��������);
#define AI_VALUE_BIG_ENDIAN_FLOAT_ARRAY_LENGTH MAX_HANDLED_ANALOGUE_INPUT
#define AI_VALUE_BIG_ENDIAN_BYTE_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_INPUT * sizeof(float))
// 7.3.2 1332 - 1342 ���� ������ ���������� ������������� �������
// �� ���������� ������������� ������� ***
//��������� ����������� ����� �1: ���,���,LL,L,H,HH  	| 192 - 202
//��������� ����������� ����� �2: ���,���,LL,L,H,HH  	| 204 - 214
//� �.�.
//��������� ����������� ����� �96: ���,���,LL,L,H,HH 	| 1332 - 1342
#define AI_VALUE_BIG_ENDIAN_DATA_BASE_BYTE_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_INPUT * sizeof(struct TAnalogueInputDescriptionModbusPackOne))
// 7.3.3 1344 - 1414 ���������� ������� ����������� (12*3 = 36)
#define CONT_AV_BIG_ENDIAN_BYTE_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_OUTPUT * sizeof(struct TMtvi5ContAvData))
// 7.3.4 1416 - 1558 ���� ������ ����������� (������������ �16) (���������) Kp,Ti,Td,OL,OH,OSF (12*6)
#define CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_OUTPUT * sizeof(struct TAnalogueOutputRegulatorDescriptionModbusPackOne))

//  1560 - 1654 �alc_cds:	��������� �������� ������ CoDeSys (48)
#define CALC_CDS_FLOAT_ARRAY_LENGTH CALC_CDS_QUANTITY
#define CALC_CDS_BYTE_ARRAY_LENGTH (CALC_CDS_FLOAT_ARRAY_LENGTH * sizeof(float))
// Sys_cds:	������ �������� ��������� ���������� CoDesys 10*4 = 40 ����
#define SYS_CDS_FLOAT_ARRAY_LENGTH SYS_CDS_QUANTITY
#define SYS_CDS_BYTE_ARRAY_LENGTH (SYS_CDS_FLOAT_ARRAY_LENGTH * sizeof(float))
// Hand_ust ������ ���� ������� (64)			| 1656 - 1782
#define HAND_UST_FLOAT_ARRAY_LENGTH HAND_UST_QUANTITY
#define HAND_UST_BYTE_ARRAY_LENGTH (HAND_UST_FLOAT_ARRAY_LENGTH * sizeof(float))
//// Av_res		������ 64 ���������� ������� 64*4 = 256 ������
//#define AV_RES_BYTE_ARRAY_LENGTH (AV_RES_QUANTITY * sizeof(float))
// ����� + ����	(1 �������� - 1 ����������� �������)	| 1784 - 1790 (���,���,���,��,���,���,��)
// ������� ����� � ����: 7 2-� ������� ���������, ����� �����:
// ���, ���, ���, ���� ���, ��� ,���, ���� ������ (��� ������:1 - �����������),
// ������������ ��� ������ �16.
#define CURRENT_TIME_HOLDING_BYTE_ARRAY_LENGTH (CURRENT_TIME_BYTE_QUANTITY * sizeof(short))

// ������� ���� LittleEndian, ��� ���������� ������.

// 7.3.1 ���������� ���������.
// Ain1 - �in96 ���������� �������� ������� ���������� �������� 1 � 96,
// ��� ����� ������������ �� 192 2-� ������� ��������� (96 ���������� ��������);
#define AI_VALUE_FLOAT_ARRAY_LENGTH MAX_HANDLED_ANALOGUE_INPUT
#define AI_VALUE_BYTE_ARRAY_LENGTH (AI_VALUE_FLOAT_ARRAY_LENGTH * sizeof(float))
// 7.3.2 ���� ������ ���������� ������������� �������
#define AI_VALUE_DATA_BASE_FLOAT_ARRAY_LENGTH ((MAX_HANDLED_ANALOGUE_INPUT * sizeof(struct TAnalogueInputDescriptionModbusPackOne)) / sizeof(float))//288
#define AI_VALUE_DATA_BASE_BYTE_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_INPUT * sizeof(struct TAnalogueInputDescriptionModbusPackOne))
// 7.3.3 ���������� ������� �����������
#define CONT_AV_FLOAT_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_OUTPUT * (sizeof(struct TMtvi5ContAvData) / sizeof(float)))//18
#define CONT_AV_BYTE_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_OUTPUT * sizeof(struct TMtvi5ContAvData))
// 7.3.4 ���� ������ ����������� (������������ �16)
#define CURRENT_OUTPUT_DATA_BASE_FLOAT_ARRAY_LENGTH ((MAX_HANDLED_ANALOGUE_OUTPUT * sizeof(struct TAnalogueOutputRegulatorDescriptionModbusPackOne)) / sizeof(float))//36
#define CURRENT_OUTPUT_DATA_BASE_BYTE_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_OUTPUT * sizeof(struct TAnalogueOutputRegulatorDescriptionModbusPackOne))
#define DATA_BASE_BLOCK_DATA_BYTE_ARRAY_LENGTH PLC_DATA_BASE_BLOCK_LENGTH
// ������ ������� aucHoldingRegistersArray[].
#define HOLDING_REGISTERS_ARRAY_LENGTH 8192//16384//
//#define HOLDING_REGISTERS_ARRAY_LENGTH  AI_VALUE_BIG_ENDIAN_BYTE_ARRAY_LENGTH + \
//                                        AI_VALUE_BIG_ENDIAN_DATA_BASE_BYTE_ARRAY_LENGTH + \
//                                        CONT_AV_BIG_ENDIAN_BYTE_ARRAY_LENGTH + \
//                                        CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_LENGTH + \
//                                        CALC_CDS_BYTE_ARRAY_LENGTH + \
//                                        HAND_UST_BYTE_ARRAY_LENGTH + \
//                                        CURRENT_TIME_HOLDING_BYTE_ARRAY_LENGTH + \
//                                        AI_VALUE_BYTE_ARRAY_LENGTH + \
//                                        AI_VALUE_DATA_BASE_BYTE_ARRAY_LENGTH + \
//                                        CONT_AV_BYTE_ARRAY_LENGTH + \
//                                        CURRENT_OUTPUT_DATA_BASE_BYTE_ARRAY_LENGTH + \
//                                        DATA_BASE_BLOCK_DATA_BYTE_ARRAY_LENGTH \

//#define HOLDING_REGISTERS_MODBUS_ARRAY_LENGTH  AI_VALUE_BIG_ENDIAN_BYTE_ARRAY_LENGTH + \
//                                        AI_VALUE_BIG_ENDIAN_DATA_BASE_BYTE_ARRAY_LENGTH + \
//                                        CONT_AV_BIG_ENDIAN_BYTE_ARRAY_LENGTH + \
//                                        CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_LENGTH + \
//                                        CALC_CDS_BYTE_ARRAY_LENGTH + \
//                                        HAND_UST_BYTE_ARRAY_LENGTH \
// �������� � ������� aucHoldingRegistersArray[].
// ��������� ����� Modbus.
#define HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS 0x00

// ������� ���� BigEndian, ��� �������� �� ����.

// 7.3.1 ���������� ���������.
// 00-190 Ain1 - �in96 ���������� �������� ������� ���������� ��������,
// ��� ����� ������������ �� 192 2-� ������� ��������� (96 ���������� ��������);
#define AI_VALUE_BIG_ENDIAN_BYTE_ARRAY_OFFSET 0
// 7.3.2 192 - 1342 ���� ������ ���������� ������������� �������
// ��������� �� ����������� ����� �1: ���, ���, LL, L, H, HH;
// ��������� �� ����������� ����� �2: ���, ���, LL, L, H, HH;
// � �.�.
// ��������� �� ����������� ����� �96: ���, ���, LL, L, H, HH;
#define AI_VALUE_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET (AI_VALUE_BIG_ENDIAN_BYTE_ARRAY_OFFSET + AI_VALUE_BIG_ENDIAN_BYTE_ARRAY_LENGTH)
// 7.3.3  1344 - 1414 (12*3 = 36)  ���������� ������� �����������
//   ���������� ������� ����������� � 18 ���������� ��������:
// SP ���������� �1 (������������ �16);
// OUT ���������� �1 (������������ �16);
// PV ���������� �1;
#define CONT_AV_BIG_ENDIAN_BYTE_ARRAY_OFFSET (AI_VALUE_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET + AI_VALUE_BIG_ENDIAN_DATA_BASE_BYTE_ARRAY_LENGTH)
// 7.3.4 1416 - 1558 ���� ������ ����������� (������������ �16) (���������) Kp,Ti,Td,OL,OH,OSF (12*6)
// ��������� �� ���������� �1: Kp, Ti, Td, OL, OH, dSP
// ��������� �� ���������� �2: Kp, Ti, Td, OL, OH, dSP
// � �.�.
// �������� �� ���������� �6: Kp, Ti, Td, OL, OH, dSP
#define CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET (CONT_AV_BIG_ENDIAN_BYTE_ARRAY_OFFSET + CONT_AV_BIG_ENDIAN_BYTE_ARRAY_LENGTH)

// ������� ���� LittleEndian, ��� ���������� ������.


//  1560 - 1654 �alc_cds:	��������� �������� ������ CoDeSys (48)
#define CALC_CDS_BYTE_ARRAY_OFFSET (CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET + CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_LENGTH)
// Hand_ust ������ ���� ������� (64)			| 1656 - 1782
#define HAND_UST_BYTE_ARRAY_OFFSET (CALC_CDS_BYTE_ARRAY_OFFSET + CALC_CDS_BYTE_ARRAY_LENGTH)
//// 7.3.7 ����� + ����	(1 �������� - 1 ����������� �������)	| 1784 - 1790 (���,���,���,��,���,���,��)
//// ������� ����� � ����: 7 2-� ������� ���������, ����� �����:
//// ���, ���, ���, ���� ���, ��� ,���, ���� ������ (��� ������:1 - �����������),
//// ������������ ��� ������ �16.
#define CURRENT_TIME_HOLDING_BYTE_ARRAY_OFFSET (HAND_UST_BYTE_ARRAY_OFFSET + HAND_UST_BYTE_ARRAY_LENGTH)
//// Av_res		������ 64 ���������� ������� 64*4 = 256 ������
//#define AV_RES_BYTE_ARRAY_OFFSET (HAND_UST_BYTE_ARRAY_OFFSET + HAND_UST_BYTE_ARRAY_LENGTH)

// ���������� �������� ������� ���������� �������� 1 � 96,
// ��� ����� ������������ �� 192 2-� ������� ��������� (96 ���������� ��������);
//#define AI_VALUE_BYTE_ARRAY_OFFSET (CURRENT_TIME_HOLDING_BYTE_ARRAY_OFFSET + CURRENT_TIME_HOLDING_BYTE_ARRAY_LENGTH)
//#define AI_VALUE_BYTE_ARRAY_OFFSET (CURRENT_TIME_HOLDING_BYTE_ARRAY_OFFSET + 16)
// 7.3.2 ���� ������ ���������� ������������� �������
//#define AI_VALUE_DATA_BASE_BYTE_ARRAY_OFFSET (AI_VALUE_BYTE_ARRAY_OFFSET + AI_VALUE_BYTE_ARRAY_LENGTH)
// 7.3.3 ���������� ������� �����������
//#define CONT_AV_BYTE_ARRAY_OFFSET (AI_VALUE_DATA_BASE_BYTE_ARRAY_OFFSET + AI_VALUE_DATA_BASE_BYTE_ARRAY_LENGTH)
// 7.3.4 ���� ������ ����������� (������������ �16)
//#define CURRENT_OUTPUT_DATA_BASE_BYTE_ARRAY_OFFSET (CONT_AV_BYTE_ARRAY_OFFSET + CONT_AV_BYTE_ARRAY_LENGTH)

//-----------------------------------------------------------------------------------------------------
// ����� ������ ������� aucInputRegistersArray[] modbus, function 4.
// 7.4 ������� 4 � ������ ��������� ���������� ���������� �������� � �������
// �������������� ����������������� ���� 0-16383 � ������ ������ (�� 1 2-� ��������
// �������� �� 1 ��������).
// ������������ ������ ������ �������������. �������� - �������� ���-19_������ Modbus_FULL � ���-05-8(16)CD_��_�2_��.

// 7.4.1 ���������� ���������
#define AI_VALUE_SHORT_CODE_SHORT_ARRAY_LENGTH MAX_HANDLED_ANALOGUE_INPUT
#define AI_VALUE_SHORT_CODE_BYTE_ARRAY_LENGTH (AI_VALUE_SHORT_CODE_SHORT_ARRAY_LENGTH * sizeof(short))
//// 7.4.2 ���� ������ ���������� ������������� �������
//#define AI_VALUE_DATA_BASE_SHORT_CODE_SHORT_ARRAY_LENGTH ((MAX_HANDLED_ANALOGUE_INPUT * sizeof(struct TAnalogueInputShortDescriptionData)) / sizeof(short))
//#define AI_VALUE_DATA_BASE_SHORT_CODE_BYTE_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_INPUT * sizeof(struct TAnalogueInputShortDescriptionData))
//
//#define EMPTY_ADDRESS_BYTE_ARRAY_LENGTH ((1000 * sizeof(short)) - (AI_VALUE_SHORT_CODE_BYTE_ARRAY_LENGTH + AI_VALUE_DATA_BASE_SHORT_CODE_BYTE_ARRAY_LENGTH))
//
//#define EMPTY_ADDRESS_BYTE_ARRAY_OFFSET
// 7.4.3 ����� �������� �������
#define CURRENT_TIME_BYTE_ARRAY_LENGTH (CURRENT_TIME_BYTE_QUANTITY * sizeof(short))
// 7.4.4 ���� ����������� �������
#define MODIFICATION_CODE_BYTE_ARRAY_LENGTH (MODIFICATION_CODE_BYTE_QUANTITY * sizeof(short))
#define NET_ADDRESS_BYTE_ARRAY_LENGTH (1 * sizeof(short))
// - ������� 1031 � ������� ����� ������� (1-32) � ���� ModBus, ���������
// RS-485.
// ������ ������� aucInputRegistersArray[].
#define INPUT_REGISTERS_ARRAY_LENGTH 8192
//#define INPUT_REGISTERS_ARRAY_LENGTH AI_VALUE_SHORT_CODE_BYTE_ARRAY_LENGTH + \
//                                     AI_VALUE_DATA_BASE_SHORT_CODE_BYTE_ARRAY_LENGTH + \
//                                     EMPTY_ADDRESS_BYTE_ARRAY_LENGTH + \
//                                     CURRENT_TIME_BYTE_ARRAY_LENGTH + \
//                                     MODIFICATION_CODE_BYTE_ARRAY_LENGTH + \
//                                     NET_ADDRESS_BYTE_ARRAY_LENGTH \

// �������� � ������� aucInputRegistersArray[].
// ��������� ����� Modbus.
#define INPUT_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS 0x00
// 7.4.1 ���������� ���������
// 00-95 ($00 - $5F) ���������� �������� ������� ���������� �������� 1 � 96,
// ��� ����� ������������ �� 96 ��������� (96 ���������� ��������);
#define AI_VALUE_SHORT_CODE_BYTE_ARRAY_OFFSET 0
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
// 7.4.4 ���� ����������� �������
// 1007-1030 - ������ ����� ����������� ������� (24 ��������)
// ���������� ��������� � ������:
// - ������� 1007 - ���������� ������� �����-������ (1 � 16);
// - �������� 1008 / 1023 - ���� ������� ����� (������) �� ������� (�������
// 1008 � ��� ������ ����� (������) � ������� �0�, ������� 1023 � ��� ������ ���-
// �� (������) � ������� �15�);
// - ������� 1024 - ����� ���������� ������ ����������� ����� +1;
// (BIaddr_max+1), ��� 0, ���� ��� ������� ����� ���������� ��������;
// - ������� 1025 - ����� ���������� ������ ����������� ����� +1
// (�Iaddr_max+1), ��� 0 ���� ��� ������� ����� ���������� ��������;
// - ������� 1026 - ���������� ���������� ������� (BOut);
// - ������� 1027 - ���������� �������� ������� �������� (BInp);
// - ������� 1028 - ���������� ���������� ������� �������� (�Inp);
// - ������� 1029� ��� ������ ��������� : 0 � 12 ����� �������� ������������
// + ��� (4�20), 1 � 24 ������ �������� ������������ +��� (4�20), 2 - ��� �����-
// ������,  3 � 6 ����� �������� ������������ +��� (4�20), 4 � ��� ����������� +
// ��� (4�20);
#define MODIFICATION_CODE_BYTE_ARRAY_OFFSET (CURRENT_TIME_BYTE_ARRAY_OFFSET + CURRENT_TIME_BYTE_ARRAY_LENGTH)
// - ������� 1031 � ������� ����� ������� (1-32) � ���� ModBus, ���������
// RS-485.
#define NET_ADDRESS_BYTE_ARRAY_OFFSET (MODIFICATION_CODE_BYTE_ARRAY_OFFSET + MODIFICATION_CODE_BYTE_ARRAY_LENGTH)

//-----------------------------------------------------------------------------------------------------
// TUST - � ��������� ���������� �������������� ������ ���̻, � ��������� - ucTust.
// D4, D5 � ������ ������, ���������� �� SLAVE (������ ��� �3,�.4 ��������� Modbus):
#define SCALABLE_CODE_16_BIT 0 // 0 � ������ � ���� ����������������� ���� 16 ��������;
// ���. ����� � ������� IEEE754 � 2 2-� ������� ��������;
#define IEEE754_BYTE_ORDER_MSB_LSB 1 // 1 � ������� ������ ������(���-�, ����, ����, ����);
#define IEEE754_BYTE_ORDER_LSB_MSB 2 // 2 � ������� ������ ������(����, ����, ����, ���-�);
#define IEEE754_BYTE_ORDER_LSR_MSR 3 // 3 � ������� ��������� ������(����, ����, ���-�, ����).
// ����� - D0 � D3 - ��� ���� ����������, ����� ucTust;
#define TUST_MODULE_TYPE_MASK 0x0F
// ����� - D4, D5 � ������ ������, ����� ucTust;
#define TUST_DATA_FORMAT_MASK 0x30
#define TUST_DATA_FORMAT_OFFSET 4

// ��������� ���������� �������������� ������ ���̻, ������� 1,2,3,4, � ���� ������.
// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
struct TMbmFunction1234PackOne
{
    // ��� ������� ��������� MODBUS (1, 2, 3, 4). ���� ����� 0, �� ������ �� �����������������.
    unsigned char ucModbusFunction;
    unsigned char ucModbusSlaveAddress; // ����� SLAVE � ������� MS.
    unsigned char ucAddressH; // ��������� ����� ������ � ������� � ������� ����.
    unsigned char ucAddressL; // ��������� ����� ������ � ������� � ������� ����.
    unsigned char ucQuantityH; // ����������  ������ � ������� � ������� ����.
    unsigned char ucQuantityL; // ����������  ������ � ������� � ������� ����.
    // D0 � D3 - ��� ���� ����������.
    // D4, D5 � ������ ������ � ����������� ���������, ���������� �� SLAVE (������ ��� �3,�.4):
    // 0 � ������ � ���� ����������������� ���� 16 ��������;
    // 1 � 3 � ������������ ����� � ������� IEEE754 � 2 2-� ������� ��������.
    // ������� ������ ������������� �����:
    // 1 � ������� ������ ������ (���-�, ����, ����, ����);
    // 2 � ������� ������ ������ (����, ����, ����, ���-�);
    // 3 � ������� ��������� ������ (����, ����, ���-�, ����).
    unsigned char ucTust;
    // ���������� ������ ��� ������� ��������������� �����.
    unsigned char ucIOQuantity;
    // ��������� ����� � ������� ������� Di_value (�1, 2, 3, 4). ���������� ������� ������������ MBM_IO
    uint16_t ui16DiValueArrayBegin;
    // ��������� ����� � ������� ���������� ��������� Ai_value:0 � 95 (�3, 4). ���������� ������� �����. MBM_IO
    unsigned char ucAiValueArrayBegin;
    // ��� ��� ����������� ������� ��. ���� (���������. �3,4)
    unsigned char ucMaxCodeL;
    // ��� ��� ����������� ������� ��. ���� (���������. �3,4)
    unsigned char ucMaxCodeH;
};
#pragma pack(pop)

// ��������� ���������� �������������� ������ ���̻, ������� 1,2,3,4, � ���� ������.
struct TMbmFunction1234
{
    // ��� ������� ��������� MODBUS (1, 2, 3, 4). ���� ����� 0, �� ������ �� �����������������.
    unsigned char ucModbusFunction;
    unsigned char ucModbusSlaveAddress; // ����� SLAVE � ������� MS.
    unsigned char ucAddressH; // ��������� ����� ������ � ������� � ������� ����.
    unsigned char ucAddressL; // ��������� ����� ������ � ������� � ������� ����.
    unsigned char ucQuantityH; // ����������  ������ � ������� � ������� ����.
    unsigned char ucQuantityL; // ����������  ������ � ������� � ������� ����.
    // D0 � D3 - ��� ���� ����������.
    // D4, D5 � ������ ������ � ����������� ���������, ���������� �� SLAVE (������ ��� �3,�.4):
    // 0 � ������ � ���� ����������������� ���� 16 ��������;
    // 1 � 3 � ������������ ����� � ������� IEEE754 � 2 2-� ������� ��������.
    // ������� ������ ������������� �����:
    // 1 � ������� ������ ������ (���-�, ����, ����, ����);
    // 2 � ������� ������ ������ (����, ����, ����, ���-�);
    // 3 � ������� ��������� ������ (����, ����, ���-�, ����).
    unsigned char ucTust;
    // ���������� ������ ��� ������� ��������������� �����.
    unsigned char ucIOQuantity;
    // ��������� ����� � ������� ������� Di_value (�1, 2, 3, 4). ���������� ������� ������������ MBM_IO
    uint16_t ui16DiValueArrayBegin;
    // ��������� ����� � ������� ���������� ��������� Ai_value:0 � 95 (�3, 4). ���������� ������� �����. MBM_IO
    unsigned char ucAiValueArrayBegin;
    // ��� ��� ����������� ������� ��. ���� (���������. �3,4)
    unsigned char ucMaxCodeL;
    // ��� ��� ����������� ������� ��. ���� (���������. �3,4)
    unsigned char ucMaxCodeH;
};

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ���������� �������������� ������ ���̻, ������� 5, � ���� ������.
struct TMbmFunction5PackOne
{
    unsigned char ucModbusFunction; // ��� ������� ��������� MODBUS (1, 2, 3, 4).
    unsigned char ucModbusSlaveAddress; // ����� SLAVE � ������� MS.
    unsigned char ucAddressH; // ��������� ����� ������ � ������� � ������� ����.
    unsigned char ucAddressL; // ��������� ����� ������ � ������� � ������� ����.
    unsigned char ucIndicatorH; // ��������� � ������� ���� ($FF � ON, 00 � OFF).
    unsigned char ucIndicatorL; // ��������� � ������� ���� (������ 0).

    unsigned char ucTust; // D0 � D2 - ��� ���� ����������, D3 � D7 � ������.
    unsigned short usIndicatorSourceAddress; // ����� ��������� ������������ ���������� (������� ��������).
    unsigned char ucReserved1; // ������.
    unsigned char ucReserved2; // ������.
    unsigned char ucReserved3; // ������.
};
#pragma pack(pop)

// ��������� ���������� �������������� ������ ���̻, ������� 5, � ���� ������.
struct TMbmFunction5
{
    unsigned char ucModbusFunction; // ��� ������� ��������� MODBUS (1, 2, 3, 4).
    unsigned char ucModbusSlaveAddress; // ����� SLAVE � ������� MS.
    unsigned char ucAddressH; // ��������� ����� ������ � ������� � ������� ����.
    unsigned char ucAddressL; // ��������� ����� ������ � ������� � ������� ����.
    unsigned char ucIndicatorH; // ��������� � ������� ���� ($FF � ON, 00 � OFF).
    unsigned char ucIndicatorL; // ��������� � ������� ���� (������ 0).

    unsigned char ucTust; // D0 � D2 - ��� ���� ����������, D3 � D7 � ������.
    unsigned short usIndicatorSourceAddress; // ����� ��������� ������������ ���������� (������� ��������).
    unsigned char ucReserved1; // ������.
    unsigned char ucReserved2; // ������.
    unsigned char ucReserved3; // ������.
};

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
struct TConfigDataPackOne // ������ ��� ������� ���� ����������, ������� modbus - 17.
{
    //ucModbusSlaveAddress
    // ����������� ���������� �������.
    unsigned char ucModulesQuantity;
    unsigned char aucModuleType[INTERNAL_MODULE_QUANTITY]; // ���� ������� ����� (������) �� ������� SPI.
    unsigned char ucLastDiscreteInputModuleAddressPlusOne; // ����� ���������� ������ ����������� ����� +1
    unsigned char ucLastAnalogueInputModuleAddressPlusOne; // ����� ���������� ������ ����������� ����� +1
    unsigned char ucDiscreteOutputQuantity;
    uint16_t ui16DiscreteInputQuantity;
    unsigned char ucAnalogueInputQuantity;
    unsigned char ucDisplayType;
    unsigned char ucErrorCode;
};
#pragma pack(pop)

struct TConfigData // ������ ��� ������� ���� ����������, ������� modbus - 17.
{
    // ����������� ���������� �������.
    unsigned char ucModulesQuantity;
    unsigned char aucModuleType[INTERNAL_MODULE_QUANTITY]; // ���� ������� ����� (������) �� ������� SPI.
    unsigned char ucLastDiscreteInputModuleAddressPlusOne; // ����� ���������� ������ ����������� ����� +1
    unsigned char ucLastAnalogueInputModuleAddressPlusOne; // ����� ���������� ������ ����������� ����� +1
    unsigned char ucDiscreteOutputQuantity;
    uint16_t ui16DiscreteInputQuantity;
    unsigned char ucAnalogueInputQuantity;
    unsigned char ucDisplayType;
    unsigned char ucErrorCode;
};

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
struct TConfigDataModbusRepresentPackOne // ������ ��� ������� ���� ����������, ������� modbus - 17.
{
    //ucModbusSlaveAddress
    // ����������� ���������� �������.
    uint16_t ui16ModulesQuantity;
    uint16_t aui16ModuleType[INTERNAL_MODULE_QUANTITY]; // ���� ������� ����� (������) �� ������� SPI.
    uint16_t ui16LastDiscreteInputModuleAddressPlusOne; // ����� ���������� ������ ����������� ����� +1
    uint16_t ui16LastAnalogueInputModuleAddressPlusOne; // ����� ���������� ������ ����������� ����� +1
    uint16_t ui16DiscreteOutputQuantity;
    uint16_t ui16DiscreteInputQuantity;
    uint16_t ui16AnalogueInputQuantity;
    uint16_t ui16DisplayType;
    uint16_t ui16ErrorCode;
};
#pragma pack(pop)

struct TPlcConfigData
{
    TConfigData xConfigData;
    unsigned short usCrc;
};

struct TConfigSearch
{
    uint8_t ui8Address;
    uint8_t ui8Type;
};

// ��������� ��� �������������� ���������� ������� ��� ������ �� SPI.
struct TPlcConfigSearch
{
    uint8_t ui8ModulesQuantity;
    TConfigSearch axConfigSearch[INTERNAL_MODULE_QUANTITY];
};


struct TPlcConfigServiceData
{
    // ��������� ����������. ����������� ��� ������. �������� �� ���������� ����� ������� ������.
    // ������������ ��� ���������� �������, �������� � �.�.
    // ����������� ������ ����������� ����� ������ ������� ������ 100mc.
    unsigned char ucInternalModulesQuantity;
    unsigned char ucExternalModulesQuantity;

    unsigned int uiHandledDiscreteInputQuantity;
    unsigned int uiHandledFunctionBlockQuantity;

    unsigned char ucLastDiscreteInputModuleIndex; // ������� ������� ��������� �� ������� � ����� ������.
    unsigned char ucLastAnalogueInputModuleIndex;
    unsigned char ucLastDiscreteOutputModuleIndex;
    unsigned char ucLastAnalogueOutputModuleIndex;

    unsigned char ucServiceDiscreteInputModuleQuantity; // ���������� �������.
    unsigned char ucServiceAnalogueInputModuleQuantity;
    unsigned char ucServiceDiscreteOutputModuleQuantity;
    unsigned char ucServiceAnalogueOutputModuleQuantity;

    unsigned char ucServiceDiscreteInputQuantity; // ���������� ������-�������.
    unsigned char ucServiceAnalogueInputQuantity;
    unsigned char ucServiceDiscreteOutputQuantity;
    unsigned char ucServiceAnalogueOutputQuantity;
};

struct TPlcConfigService
{
    TPlcConfigServiceData xPlcConfigServiceData;
    unsigned short usCrc;
};

struct TPlcDataBaseBlock // ��������� ������ ����� ���� ������.
{
    unsigned char aucPlcDataBaseBlockData[PLC_DATA_BASE_BLOCK_LENGTH];
};

struct TPlcDataBase // ��������� ���� ���� ������.
{
    TPlcDataBaseBlock axPlcDataBaseBlocks[PLC_DATA_BASE_BLOCK_QUANTITY];
    unsigned short usCrc;
};

// ��� ������ ���� ������ �� Modbus - ������� 69, �� ���� ��� ��������� ���� ���� ���� �
// ��� �����. ����� ������ ��� ������ ������ �� ������������ � ������� ��� � �������,
// � ������� vMainTaskMemoryMapFormation(void)
// ��� ������� ����� ������� ������������ usBlockLength, ucModuleType, ucModuleIndex ������ ����������.
// ����������� ������ ����������� ����� ������ ������� ������ 100mc.
struct TPlcDataBaseBlockControl // ��������� ���������� ����� ������ ���� ������.
{
    unsigned short usBlockLength; // ������ � ������ ����� ���� ������, ������� ������������� � ������.
    unsigned char ucModuleType; // ��� ������ � �������� ������������� ���� ���� ������.
    unsigned char ucModuleIndex; // ������ ������ � ������� ��������� � �������� ������������� ���� ���� ������.
};

struct TPlcDataBaseControl // ��������� ���������� ���� ����� ������.
{
    TPlcDataBaseBlockControl axPlcDataBaseBlockControl[PLC_DATA_BASE_BLOCK_QUANTITY];
    unsigned short usCrc;
};

#define AIN_OFF_PROCESS_OWNER_IS_NONE 0
#define AIN_OFF_PROCESS_OWNER_IS_PROGRAMMER 1
#define AIN_OFF_PROCESS_OWNER_IS_HMI 2
struct TMainFlagRegister
{
    // ������������ ��� ������������� ��������� ����
    // ������� ������ � �������� ������.
    bool fbExternalModulesDataAccessIsAllowed;
//    // ������������� � ���, ��� ������� ����� ���������� �
//    // ��������� ��������� �� ��������� ���������� ����� � HMI ������.
//    bool fbCalibrationInProgress;
    // ������ �������� ���������� �� ��������� ���������� �����.
    uint8_t ui8AinOffProcessOwnerIndex;
};

// ���������� �������� ������ (HMI ������, ������� SCADA, �������������), ��������������� � ��������
// �� ��������� "Modbus". � ������� ������� ������-������ "Modbus", ������� ���������� ��������
// ������ � ������, ������������� � �������� "Modbus" - aucCoilsArray[], aucDiscreteInputsArray[],
// aucHoldingRegistersArray[], aucInputRegistersArray[].
//  � ������������ � �������, ������� ������, ���������� ������������ � �������� "Modbus".
// � ������������ - vMainTaskMemoryMapFormation(void) �������� �������� �������� ������� ������,
// ������� ���������� � ������ ���� - struct TAllModulesContext. ����������� ��������
// ��������� - struct TModuleContextStatic.
//  �� ����� ����������� �������� ����� 100mc, �� �������, ��� ������� ������
// �� ��� ����(������� ucModuleType, ��������� - struct TModuleContextStatic,
// ������� ���� - struct TAllModulesContext) - ���������� ������� � ��������� ��������� ��
// ��� ��������.
// � ��������� ���������� ���������, �������, ����� � ������ ��������������� ������.

// ��������� ��������� ������ ������ �� ���������� � ��������.
// ����������� ������ ����������� ����� ������ ������� ������ 100mc.
struct TModuleContextStatic
{
    // ������ � ������� � ��������� ��������� ���� �������.
    unsigned char ucModuleContextIndex;
    // ����� ������ �� SPI.
    unsigned char ucModuleAddress;
    // ��� ������.
    unsigned char ucModuleType;
    // ������ ����� ���������� ������� -
    // ����������� �����, ��������� �����,
    // ����������� ������, ��������� ������ � �.�.
    unsigned char ucInsideTypeIndex;
    // ��������� �� ������ � ������ - ������������ �� ����������� ����� ������������.
    unsigned char *pucPlcConfigDataTempPointer;

    // ��������� �� ������������ � �������� "Modbus".
    // ��������� �� ������ � ������� ��������.
    // ���������� �������� ������� ���������� ��������.
    unsigned char *pucAiValueOriginBufferPointer;
    // ��������� �������� CoDeSys.
    unsigned char *pucCalcValueOriginBufferPointer;
    // ����� CoDeSys.
    unsigned char *pucLogSostOriginBufferPointer;
    // ��������� ������� ���������� ��������.
    unsigned char *pucDiValueOriginBufferPointer;
    // ��������� ������� ������� ���� �� �1 � MP �8.
    unsigned char *pucSostDoOriginBufferPointer;
    // ������ ������ ��������������� ���������� ��������.
    unsigned char *pucBadAiOriginBufferPointer;
    // ������ �������� �������.
    unsigned char *pucCurrentTimeBufferPointer;
    // ������ ������� ������� "������".
    unsigned char *pucAlarmEventsLog;
    // ������ ������� �������� �������.
    unsigned char *pucArchiveEventsLog;
    // ������ ������� ������.
    unsigned char *pucErrorLog;
    // ������ � ��������������� ����� ������ ��������� ���������� ��������.
    unsigned char *pucAnalogueInputDescriptionWork;
//    // ������ � ��������������� ����� ������ ��������� ���������� �������.
//    unsigned char *pucAnalogueOutputRegulatorDescriptionDataBase;
    // ������ � ��������������� ����� ������ ������������.
    unsigned char *pucDimentionsParametersDataBase;

    // ��������� �� ������������ � �������� "Modbus".
    // ��������� �� ������������ � ������� ��������,
    // ������� ����������� ��� ������� ������.
    // ���������� �������� ������� ���������� ��������.
    unsigned char *pucAiValueBufferPointer;
    // ���������� ������� �����������.
    unsigned char *pucContAvBufferPointer;

    // ��������� ������� ���������� ��������.
    unsigned char *pucDiValueBufferPointer;
    // ��������� ������� ������� ���� �� �1 � MP �8.
    unsigned char *pucSostDoBufferPointer;
    // ������ ������ ��������������� ���������� ��������.
    unsigned char *pucBadAiBufferPointer;
    // ������ ������ ��������������� ���������� ��������.
    unsigned char *pucBadDiBufferPointer;
    // ���������� ����������� �������� - ����� ���������.
    unsigned char *pucDoValueBufferPointer;
    // ���������� ����������� �������� - ���������� ��������� ����.
    unsigned char *pucDoValueBlockBufferPointer;
    // ������ ���������� �������� ����������� ��� ������.
    unsigned char *pucContStBufferPointer;
    // ������ ���������� �������� ����������� ��� ������.
    unsigned char *pucContStWriteBufferPointer;
    // ��������� ���������� ������������� �������.
    unsigned char *pucStatAiBufferPointer;
    // ������ ������ ������� �������.
    unsigned char *pucModuleBadStateBufferPointer;
    // ������ ������ ���������� ���������� �������� �� ���������.
    unsigned char *pucAinOffBufferPointer;
    // ������ ������ ��������������� ���������� ������� ������� ����.
    unsigned char *pucBadDoBufferPointer;
    // ������ ������ ������ ���������� ������� ������� ����.
    unsigned char *pucStatDoBufferPointer;
    // ��������� ���������� �������.
    unsigned char *pucBadAoBufferPointer;
    // ����� ������ ����������� ��������� ���� ���, ������ ����3.
    unsigned char *pucTxsBufferPointer;
    // ����� ������ �������� ����� � ���� ���, ������ ������������ �����.
    unsigned char *pucReperPointsAdcBufferPointer;

    unsigned char *pucDataBasePointer; // ��������� �� ���� � ���� ������.
    unsigned char ucDataBaseDataOffset; // �������� �� ���� ������, � ����� ���� ������.
    unsigned char *pucDataBaseExtractPointer; // ��������� �� ���� � ��������������� ���� ������.
};

// ��������� ��������� ������ ������ ���������� � ��������.
struct TModuleContextDinamic
{
    // ����� ���� ��� �������� ������. ������������ ��� �������� ������ ������������ �����,
    // ������ ���������� � ��.
    unsigned char ucCommonIndex;
    // ������� �������� ������ ��� ������.
    // ���� ����� ���� - ������ ��������� �����������. ��� ������ ���������� ��������������.
    unsigned char nucBadAnswerCounter;
    // ��� ������.
    unsigned char ucErrorCode;
    // ������� - ��������, ��� ��������� ������� ������� � MTVI5.
    unsigned char nucLoopSkipCounter;
    unsigned char ucDataFormat;
    unsigned char ucDataByteOrder;
    unsigned char ucCommandControl;
    unsigned char ucFsmState; // ��������� ��������.
    unsigned short usAuxiliaryCounter;
    unsigned short usAuxiliaryData;

    bool fbGlobalModbusKvit; // ���� - ������������ � �������� ������ �� Modbus.
    bool fbGlobalModbusReset; // ���� - ����� � �������� ������ �� Modbus.
    bool fbGlobalModbusBlock; // ���� - ����� ���������� � �������� ������ �� Modbus.
    bool fbGlobalButtonKvit; // ���� - ������������ � ����������� ������ ����.
    bool fbGlobalButtonReset; // ���� - ����� � ����������� ������ ����.
    bool fbGlobalButtonBlock; // ���� - ����� ���������� � ����������� ������ ����.
};

struct TModuleContext // ��������� ��������� ������ ������.
{
    // ������ � ����������� �������.
    TModuleContextDinamic xModuleContextDinamic;
    // ������ � ������������� �������.
    TModuleContextStatic xModuleContextStatic;
};

struct TAllModulesContext // ��������� ��������� ���� �������.
{
    // ������ � ��������� ��������� ���� �������.
    TModuleContext axAllModulesContext[MAX_MODULE_QUANTITY];
    unsigned short usCrc;
};

// ����� - ���� 0 � 3 ��� ����� ������� ���.
#define ANALOGUE_INPUT_SCALE_TYPE_MASK 0x0F
// ����� - ���� 4 � 7 � ��� ����������� ����������� ��������� ����.
#define ANALOGUE_INPUT_DIMENTION_CODE_MASK 0xF0
// ����� - ���� 0 � 5 � ��� ��������� ���������� ����������� ��� ����5, ��� ��� �������� ������� ��� ����5 � ����5 (��� ����5 �������� �� �����) ����.
#define ANALOGUE_INPUT_RANGE_CODE_MASK 0x3F
// ��������� ��������� ������ ����������� ����� � ���� ������.
struct TAnalogueInputDescriptionDataBase
{
    // - ���� 0 � 3 ��� ����� ������� ���.
    // - ���� 4 � 7 � ��� ����������� ����������� ��������� ����.
    unsigned char ucServiseL;
    // - ���� 0 � 5 � ��� ��������� ���������� ����������� ��� ����5, ��� ��� �������� ������� ��� ����5 � ����5 (��� ����5 �������� �� �����) ����.
    // - ���� 6,7 � ������.
    unsigned char ucServiseH;
    // ��� - ������ ������ ����� �������, ������ � FLOAT, 3 �����.
    unsigned char ucMinOfScaleMantissLsb;
    unsigned char ucMinOfScaleMantissMsb;
    unsigned char ucMinOfScaleExponent;
    // ��� - ������� ������ ����� �������, ������ � FLOAT, 3 �����.
    unsigned char ucMaxOfScaleMantissLsb;
    unsigned char ucMaxOfScaleMantissMsb;
    unsigned char ucMaxOfScaleExponent;
    // ������� LL, ������ � FLOAT, 3 �����.
    unsigned char ucSetPointLLMantissLsb;
    unsigned char ucSetPointLLMantissMsb;
    unsigned char ucSetPointLLExponent;
    // ������� L, ������ � FLOAT, 3 �����.
    unsigned char ucSetPointLMantissLsb;
    unsigned char ucSetPointLMantissMsb;
    unsigned char ucSetPointLExponent;
    // ������� H, ������ � FLOAT, 3 �����.
    unsigned char ucSetPointHMantissLsb;
    unsigned char ucSetPointHMantissMsb;
    unsigned char ucSetPointHExponent;
    // ������� HH, ������ � FLOAT, 3 �����.
    unsigned char ucSetPointHHMantissLsb;
    unsigned char ucSetPointHHMantissMsb;
    unsigned char ucSetPointHHExponent;
    // ��������� �������� ����� � 8 �������� ASCII.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH];
};

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ��������� ������ ����������� ����� � ���� ������.
struct TAnalogueInputDescriptionDataBasePackOne
{
    // - ���� 0 � 3 ��� ����� ������� ���.
    // - ���� 4 � 7 � ��� ����������� ����������� ��������� ����.
    unsigned char ucServiseL;
    // - ���� 0 � 5 � ��� ��������� ���������� ����������� ��� ����5, ��� ��� �������� ������� ��� ����5 � ����5 (��� ����5 �������� �� �����) ����.
    // - ���� 6,7 � ������.
    unsigned char ucServiseH;
    // ��� - ������ ������ ����� �������, ������ � FLOAT, 3 �����.
    unsigned char ucMinOfScaleMantissLsb;
    unsigned char ucMinOfScaleMantissMsb;
    unsigned char ucMinOfScaleExponent;
    // ��� - ������� ������ ����� �������, ������ � FLOAT, 3 �����.
    unsigned char ucMaxOfScaleMantissLsb;
    unsigned char ucMaxOfScaleMantissMsb;
    unsigned char ucMaxOfScaleExponent;
    // ������� LL, ������ � FLOAT, 3 �����.
    unsigned char ucSetPointLLMantissLsb;
    unsigned char ucSetPointLLMantissMsb;
    unsigned char ucSetPointLLExponent;
    // ������� L, ������ � FLOAT, 3 �����.
    unsigned char ucSetPointLMantissLsb;
    unsigned char ucSetPointLMantissMsb;
    unsigned char ucSetPointLExponent;
    // ������� H, ������ � FLOAT, 3 �����.
    unsigned char ucSetPointHMantissLsb;
    unsigned char ucSetPointHMantissMsb;
    unsigned char ucSetPointHExponent;
    // ������� HH, ������ � FLOAT, 3 �����.
    unsigned char ucSetPointHHMantissLsb;
    unsigned char ucSetPointHHMantissMsb;
    unsigned char ucSetPointHHExponent;
    // ��������� �������� ����� � 8 �������� ASCII.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH];
};
#pragma pack(pop)

// ��������� ��������� ������ ����������� ����� � ������ RAM, IEEE754.
// ����� �������������� �� ������ ������� ���� ������.
struct TAnalogueInputDescriptionModbus
{
    // ��� - ������ ������ ����� �������, ������ � FLOAT, IEEE754.
    float fMinOfScale;
    // ��� - ������� ������ ����� �������, ������ � FLOAT, IEEE754.
    float fMaxOfScale;
    // ������� LL, ������ � FLOAT, IEEE754.
    float fSetPointLL;
    // ������� L, ������ � FLOAT, IEEE754.
    float fSetPointL;
    // ������� H, ������ � FLOAT, IEEE754.
    float fSetPointH;
    // ������� HH, ������ � FLOAT, IEEE754.
    float fSetPointHH;
};

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ��������� ������ ����������� ����� � ������ RAM, IEEE754.
// ����� �������������� �� ������ ������� ���� ������.
struct TAnalogueInputDescriptionModbusPackOne
{
    // ��� - ������ ������ ����� �������, ������ � FLOAT, IEEE754.
    float fMinOfScale;
    // ��� - ������� ������ ����� �������, ������ � FLOAT, IEEE754.
    float fMaxOfScale;
    // ������� LL, ������ � FLOAT, IEEE754.
    float fSetPointLL;
    // ������� L, ������ � FLOAT, IEEE754.
    float fSetPointL;
    // ������� H, ������ � FLOAT, IEEE754.
    float fSetPointH;
    // ������� HH, ������ � FLOAT, IEEE754.
    float fSetPointHH;
};
#pragma pack(pop)

//// ��������� ��������� ������ ����������� ����� � ������ RAM, IEEE754.
//// ����� �������������� �� ������ ������� ���� ������.
//// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
//#pragma pack(push)
//#pragma pack(1)
//struct TAnalogueInputDescriptionWorkPackOne
//{
//    // ��� ����� ������� ���.
//    unsigned char ucScaleType;
//    // ��� ����������� ����������� ��������� ����.
//    unsigned char ucDimentionCode;
//    // ��� ��������� ���������� ����������� ��� ����5, ��� ��� �������� ������� ��� ����5 � ����5 (��� ����5 �������� �� �����) ����.
//    unsigned char ucRangeCode;
////    // ��� - ������ ������ ����� �������, ������ � FLOAT, IEEE754.
////    float fMinOfScale;
////    // ��� - ������� ������ ����� �������, ������ � FLOAT, IEEE754.
////    float fMaxOfScale;
////    // ������� LL, ������ � FLOAT, IEEE754.
////    float fSetPointLL;
////    // ������� L, ������ � FLOAT, IEEE754.
////    float fSetPointL;
////    // ������� H, ������ � FLOAT, IEEE754.
////    float fSetPointH;
////    // ������� HH, ������ � FLOAT, IEEE754.
////    float fSetPointHH;
//    // ��������� �������� ����� � 8 �������� ASCII, ���� ���� - ������� ����� ������.
//    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
//};
//#pragma pack(pop)

// ��������� ��������� ������ ����������� ����� � ������ RAM, IEEE754.
// ����� �������������� �� ������ ������� ���� ������.
struct TAnalogueInputDescriptionWork
{
    // ��� ����� ������� ���.
    unsigned char ucScaleType;
    // ��� ����������� ����������� ��������� ����.
    unsigned char ucDimentionCode;
    // ��� ��������� ���������� ����������� ��� ����5, ��� ��� �������� ������� ��� ����5 � ����5 (��� ����5 �������� �� �����) ����.
    unsigned char ucRangeCode;
    // ��� - ������ ������ ����� �������, ������ � FLOAT, IEEE754.
    float fMinOfScale;
    // ��� - ������� ������ ����� �������, ������ � FLOAT, IEEE754.
    float fMaxOfScale;
    // ������� LL, ������ � FLOAT, IEEE754.
    float fSetPointLL;
    // ������� L, ������ � FLOAT, IEEE754.
    float fSetPointL;
    // ������� H, ������ � FLOAT, IEEE754.
    float fSetPointH;
    // ������� HH, ������ � FLOAT, IEEE754.
    float fSetPointHH;
    // ��������� �������� ����� � 8 �������� ASCII, ���� ���� - ������� ����� ������.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
};

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ��������� ������ ����������� ����� � ������ RAM, IEEE754.
// ����� �������������� �� ������ ������� ���� ������.
struct TAnalogueInputDescriptionWorkPackOne
{
    // ��� ����� ������� ���.
    unsigned char ucScaleType;
    // ��� ����������� ����������� ��������� ����.
    unsigned char ucDimentionCode;
    // ��� ��������� ���������� ����������� ��� ����5, ��� ��� �������� ������� ��� ����5 � ����5 (��� ����5 �������� �� �����) ����.
    unsigned char ucRangeCode;
    // ��� - ������ ������ ����� �������, ������ � FLOAT, IEEE754.
    float fMinOfScale;
    // ��� - ������� ������ ����� �������, ������ � FLOAT, IEEE754.
    float fMaxOfScale;
    // ������� LL, ������ � FLOAT, IEEE754.
    float fSetPointLL;
    // ������� L, ������ � FLOAT, IEEE754.
    float fSetPointL;
    // ������� H, ������ � FLOAT, IEEE754.
    float fSetPointH;
    // ������� HH, ������ � FLOAT, IEEE754.
    float fSetPointHH;
    // ��������� �������� ����� � 8 �������� ASCII, ���� ���� - ������� ����� ������.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
};
#pragma pack(pop)

//// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
//#pragma pack(push)
//#pragma pack(1)
//// ��������� ������ ���������� ���������.
//struct TAnalogueMeasurePackOne
//{
//    // ����� ������� �������������� ����������� ���������.
//    // ����� ������� (� �����, ����� �
//    // (���������� �������� ������� ���������� �������� - AI_VALUE_BYTE_ARRAY_OFFSET)
//    uint8_t ui8Address;
//    // ��������� ������ ����������� ���������.
//    uint8_t ui8Status;
//    // �������� ����������� ���������.
//    float fData;
//    // ��������� �������� ����� � 8 �������� ASCII, ���� ���� - ������� ����� ������.
//    char acTextDescriptor[CMind::ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
//    // ��������� �������� ����������� ����������� ��������� ����.
//    char aucDimentionCode[CMind::DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
//};
//#pragma pack(pop)
//
//// ��������� ������ ���������� ���������.
//struct TAnalogueMeasure
//{
//    // ����� ������� �������������� ����������� ���������.
//    // ����� ������� (� �����, ����� �
//    // (���������� �������� ������� ���������� �������� - AI_VALUE_BYTE_ARRAY_OFFSET)
//    uint8_t ui8Address;
//    // ��������� ������ ����������� ���������.
//    uint8_t ui8Status;
//    // �������� ����������� ���������.
//    float fData;
//    // ��������� �������� ����� � 8 �������� ASCII, ���� ���� - ������� ����� ������.
//    char acTextDescriptor[CMind::ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
//    // ��������� �������� ����������� ����������� ��������� ����.
//    char aucDimentionCode[CMind::DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
//};

//// ��������� ��������� ������ ����������� ����� � ������ RAM, ������������� ��� 16 ���.
//// ����� �������������� �� ������ ������� ���� ������.
//struct TAnalogueInputShortDescriptionData
//{
//    // ������� LL, ������ � SHORT, ������������� ��� 16 ���.
//    unsigned short usSetPointLL;
//    // ������� L, ������ � SHORT, ������������� ��� 16 ���.
//    unsigned short usSetPointL;
//    // ������� L, ������ � SHORT, ������������� ��� 16 ���.
//    unsigned short usSetPointH;
//    // ������� HH, ������ � SHORT, ������������� ��� 16 ���.
//    unsigned short usSetPointHH;
//};

// ��������� ��������� ����� �����������.
// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ��������� ����� �����������.
// ����� �������������� �� ������ ������� ���� ������.
struct TDimentionParameterPackOne
{
    // ��������� �������� ����������� � 6 �������� ASCII.
    char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH];
};
#pragma pack(pop)

// ��������� ��������� ����� �����������.
struct TDimentionParameter
{
    // ��������� �������� ����������� � 6 �������� ASCII, ���� ���� - ������� ����� ������.
    char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
};

// ��������� ��� �������� � �������� ����������� ��������� ���� ���.
struct TTxsData
{
    float fTxsValue; // �������� ���.
    unsigned char ucTxsStat; // ��������� ������ ���.
};

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ��� �������� � �������� ����������� ��������� ���� ���.
struct TTxsDataPackOne
{
    float fTxsValue; // �������� ���.
    unsigned char ucTxsStat; // ��������� ������ ���.
};

#pragma pack(pop)

// ��������� ��������� ������ ���������� ������ ����������� ������ � ���� ������.
struct TAnalogueOutputRegulatorDescriptionDataBase
{
    // ����� ������������� ��������� (PV=1-96), 0 � ������ ���������.
    unsigned char ucPvAddress;
    // ��� ���������� (TYPE = 0, 1, 8 � 13):
    // TYPE 0 � ����������� �������:
    // - ���� �PV� ���������� �� ����� �OUT�;
    // - ����� �OUT� ��������� �� ���.
    // TYPE 1 � ������������� ��������� (���� 2 = 0-5) ��� ��������� �������� ������� ��������� ����� (���� 2 = 8-13):
    // - ������� ������� �� ����� �SP�, �� ������� ��������� � ��;
    // - ����� �OUT� ��������� �� ��� (���� 2 = 0-5) ��� �� ���� SP ���������� ����������� ������� (���� 2 = 8-13).
    // TYPE 8 � 13 � ��������� ����������� ������� ��������� �����:
    // - � ������ ��� �� ���� �SP� ��������� ������ � ��;
    // - � ������ ��ϻ �� ���� �SP� ��������� ����� �OUT� ���������� �������� �������: 8 � ��������� �1, 13 � ��������� �6;
    // - ����� �OUT� ��������� �� ���.
    unsigned char ucRegulatorType;
    // ����� ���������� ��������� ������� �OUT�:
    // 0 � 5 � ����� ����������� ������ ���� (���) ���:
    // - TYPE 0 � ����������� �������;
    //  - TYPE 1 � ������������� ���������;
    //  - TYPE 8 � 13 � ���������� ������ ��������� �����);
    // 8 � 13 � ���������� ������ �OUT� ��� TYPE 1 � ��������� �������� ������� ��������� �����
    // - ����� ���������� ����������� �������, ��� �������� �OUT� �������� �������� � ��������� �����
    // (8 � ��������� �1, 13 � ��������� �6).
    unsigned char ucOutAddress;
    // D0 � ������� ������������ ������: 0 � ������, 1 - �������� (��������� ���� � ������).
    unsigned char ucOutInverse;
    // D0 - �������� ������� PV (������������� ��������� ��� TYPE 1 ��� ����� ����������� ������� ��� TYPE 0) - ����� � ����� 0 ���������:
    // D0 = 0 � ���������� ������� ������ AIn (� ��� 0-16383 �����. �� �����);
    // D0 = 1 � ��������� �������� �alc_cds (� ������ CoDeSys ���������� �
    // 0-100%, �������������� � ��� 0-16383);
    // D1 � D7 � ������.
    unsigned char ucControl;
    // ����� ��������� ������������ �������, ������������ �� ���� SP � ��������� �������� �alc_cds (� ������ CoDeSys ���������� � �������� ����� PV, �������������� � ��� 0-16383 ����� ������� �� ���� SP);
    // 0 � ��� ������������ �������
    unsigned char ucProgrammSPAddress;
    // ��������� � Kp - float, 3 �����.
    unsigned char ucKpMantissLsb;
    unsigned char ucKpMantissMsb;
    unsigned char ucKpExponent;
    // ��������� � Ti - float, 3 �����.
    unsigned char ucTiMantissLsb;
    unsigned char ucTiMantissMsb;
    unsigned char ucTiExponent;
    // ��������� � Td - float, 3 �����.
    unsigned char ucTdMantissLsb;
    unsigned char ucTdMantissMsb;
    unsigned char ucTdExponent;
    // ����������� ������ ����� (OL 0-100%) � 1 ����� float, 3 �����.
    unsigned char ucOutRestrictionLowMantissLsb;
    unsigned char ucOutRestrictionLowMantissMsb;
    unsigned char ucOutRestrictionLowExponent;
    // ����������� ������ ������ (O� 0-100%) - 1 ����� float, 3 �����.
    unsigned char ucOutRestrictionHighMantissLsb;
    unsigned char ucOutRestrictionHighMantissMsb;
    unsigned char ucOutRestrictionHighExponent;
    // ���������������� �������� ������ OUT ��� ��������� (OSF 0-100%) - 1 ����� float, 3 �����.
    unsigned char ucPreferredOutputValueMantissLsb;
    unsigned char ucPreferredOutputValueMantissMsb;
    unsigned char ucPreferredOutputValueExponent;
    // ����� ������ LogSost [fc] CoDeSys (1-240)
    // � ������������ ��������� ������ �������������� ���������� � fc,
    // 0 � ����� fc �� ������������.
    unsigned char ucForceControlFcOnAddress;
    // ����� ������ LogSost [ha] CoDeSys (1-240) ��� ���������� ������� �/� � �������� �������������� ���������� - fc
    unsigned char ucForceControlHaOnAddress;
    // ����� ��������� �������� �alc_cds [spout] CoDeSys (1-48) ��� ���������� SP/OUT � �������� �������������� ���������� - fc
    unsigned char ucForceControlSpOutAddress;
    // ������.
    unsigned char ucReserved;
};

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ��������� ������ ���������� ������ ����������� ������ � ���� ������.
struct TAnalogueOutputRegulatorDescriptionDataBasePackOne
{
    // ����� ������������� ��������� (PV=1-96), 0 � ������ ���������.
    unsigned char ucPvAddress;
    // ��� ���������� (TYPE = 0, 1, 8 � 13):
    // TYPE 0 � ����������� �������:
    // - ���� �PV� ���������� �� ����� �OUT�;
    // - ����� �OUT� ��������� �� ���.
    // TYPE 1 � ������������� ��������� (���� 2 = 0-5) ��� ��������� �������� ������� ��������� ����� (���� 2 = 8-13):
    // - ������� ������� �� ����� �SP�, �� ������� ��������� � ��;
    // - ����� �OUT� ��������� �� ��� (���� 2 = 0-5) ��� �� ���� SP ���������� ����������� ������� (���� 2 = 8-13).
    // TYPE 8 � 13 � ��������� ����������� ������� ��������� �����:
    // - � ������ ��� �� ���� �SP� ��������� ������ � ��;
    // - � ������ ��ϻ �� ���� �SP� ��������� ����� �OUT� ���������� �������� �������: 8 � ��������� �1, 13 � ��������� �6;
    // - ����� �OUT� ��������� �� ���.
    unsigned char ucRegulatorType;
    // ����� ���������� ��������� ������� �OUT�:
    // 0 � 5 � ����� ����������� ������ ���� (���) ���:
    // - TYPE 0 � ����������� �������;
    //  - TYPE 1 � ������������� ���������;
    //  - TYPE 8 � 13 � ���������� ������ ��������� �����);
    // 8 � 13 � ���������� ������ �OUT� ��� TYPE 1 � ��������� �������� ������� ��������� �����
    // - ����� ���������� ����������� �������, ��� �������� �OUT� �������� �������� � ��������� �����
    // (8 � ��������� �1, 13 � ��������� �6).
    unsigned char ucOutAddress;
    // D0 � ������� ������������ ������: 0 � ������, 1 - �������� (��������� ���� � ������).
    unsigned char ucOutInverse;
    // D0 - �������� ������� PV (������������� ��������� ��� TYPE 1 ��� ����� ����������� ������� ��� TYPE 0) - ����� � ����� 0 ���������:
    // D0 = 0 � ���������� ������� ������ AIn (� ��� 0-16383 �����. �� �����);
    // D0 = 1 � ��������� �������� �alc_cds (� ������ CoDeSys ���������� �
    // 0-100%, �������������� � ��� 0-16383);
    // D1 � D7 � ������.
    unsigned char ucControl;
    // ����� ��������� ������������ �������, ������������ �� ���� SP � ��������� �������� �alc_cds (� ������ CoDeSys ���������� � �������� ����� PV, �������������� � ��� 0-16383 ����� ������� �� ���� SP);
    // 0 � ��� ������������ �������
    unsigned char ucProgrammSPAddress;
    // ��������� � Kp - float, 3 �����.
    unsigned char ucKpMantissLsb;
    unsigned char ucKpMantissMsb;
    unsigned char ucKpExponent;
    // ��������� � Ti - float, 3 �����.
    unsigned char ucTiMantissLsb;
    unsigned char ucTiMantissMsb;
    unsigned char ucTiExponent;
    // ��������� � Td - float, 3 �����.
    unsigned char ucTdMantissLsb;
    unsigned char ucTdMantissMsb;
    unsigned char ucTdExponent;
    // ����������� ������ ����� (OL 0-100%) � 1 ����� float, 3 �����.
    unsigned char ucOutRestrictionLowMantissLsb;
    unsigned char ucOutRestrictionLowMantissMsb;
    unsigned char ucOutRestrictionLowExponent;
    // ����������� ������ ������ (O� 0-100%) - 1 ����� float, 3 �����.
    unsigned char ucOutRestrictionHighMantissLsb;
    unsigned char ucOutRestrictionHighMantissMsb;
    unsigned char ucOutRestrictionHighExponent;
    // ���������������� �������� ������ OUT ��� ��������� (OSF 0-100%) - 1 ����� float, 3 �����.
    unsigned char ucPreferredOutputValueMantissLsb;
    unsigned char ucPreferredOutputValueMantissMsb;
    unsigned char ucPreferredOutputValueExponent;
    // ����� ������ LogSost [fc] CoDeSys (1-240)
    // � ������������ ��������� ������ �������������� ���������� � fc,
    // 0 � ����� fc �� ������������.
    unsigned char ucForceControlFcOnAddress;
    // ����� ������ LogSost [ha] CoDeSys (1-240) ��� ���������� ������� �/� � �������� �������������� ���������� - fc
    unsigned char ucForceControlHaOnAddress;
    // ����� ��������� �������� �alc_cds [spout] CoDeSys (1-48) ��� ���������� SP/OUT � �������� �������������� ���������� - fc
    unsigned char ucForceControlSpOutAddress;
    // ������.
    unsigned char ucReserved;
};
#pragma pack(pop)

// ��������� ��������� ������ ����������� ������ � ������ RAM, IEEE754.
// ����� �������������� �� ������ ������� ���� ������.
struct TAnalogueOutputRegulatorDescriptionModbus
{
    // ��������� � Kp - float,  IEEE754.
    float fKp;
    // ��������� � Ti - float,  IEEE754.
    float fTi;
    // ��������� � Td - float,  IEEE754.
    float fTd;
    // ����������� ������ ����� (OL 0-100%) � 1 ����� float, IEEE754.
    float fOutRestrictionLow;
    // ����������� ������ ������ (O� 0-100%) - 1 ����� float, IEEE754.
    float fOutRestrictionHigh;
    // ���������������� �������� ������ OUT ��� ��������� (OSF 0-100%) float, IEEE754.
    float fPreferredOutputValue;
};

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ��������� ������ ����������� ������ � ������ RAM, IEEE754.
// ����� �������������� �� ������ ������� ���� ������.
struct TAnalogueOutputRegulatorDescriptionModbusPackOne
{
    // ��������� � Kp - float,  IEEE754.
    float fKp;
    // ��������� � Ti - float,  IEEE754.
    float fTi;
    // ��������� � Td - float,  IEEE754.
    float fTd;
    // ����������� ������ ����� (OL 0-100%) � 1 ����� float, IEEE754.
    float fOutRestrictionLow;
    // ����������� ������ ������ (O� 0-100%) - 1 ����� float, IEEE754.
    float fOutRestrictionHigh;
    // ���������������� �������� ������ OUT ��� ��������� (OSF 0-100%) float, IEEE754.
    float fPreferredOutputValue;
};
#pragma pack(pop)

// ��������� ��������� ������ ����������� ������ � ������ RAM, IEEE754.
// ����� �������������� �� ������ ������� ���� ������.
struct TAnalogueOutputRegulatorDescriptionWork
{
    // ����� ������������� ��������� (PV=1-96), 0 � ������ ���������.
    unsigned char ucPvAddress;
    // ��� ���������� (TYPE = 0, 1, 8 � 13):
    // TYPE 0 � ����������� �������:
    // - ���� �PV� ���������� �� ����� �OUT�;
    // - ����� �OUT� ��������� �� ���.
    // TYPE 1 � ������������� ��������� (���� 2 = 0-5) ��� ��������� �������� ������� ��������� ����� (���� 2 = 8-13):
    // - ������� ������� �� ����� �SP�, �� ������� ��������� � ��;
    // - ����� �OUT� ��������� �� ��� (���� 2 = 0-5) ��� �� ���� SP ���������� ����������� ������� (���� 2 = 8-13).
    // TYPE 8 � 13 � ��������� ����������� ������� ��������� �����:
    // - � ������ ��� �� ���� �SP� ��������� ������ � ��;
    // - � ������ ��ϻ �� ���� �SP� ��������� ����� �OUT� ���������� �������� �������: 8 � ��������� �1, 13 � ��������� �6;
    // - ����� �OUT� ��������� �� ���.
    unsigned char ucRegulatorType;
    // ����� ���������� ��������� ������� �OUT�:
    // 0 � 5 � ����� ����������� ������ ���� (���) ���:
    // - TYPE 0 � ����������� �������;
    //  - TYPE 1 � ������������� ���������;
    //  - TYPE 8 � 13 � ���������� ������ ��������� �����);
    // 8 � 13 � ���������� ������ �OUT� ��� TYPE 1 � ��������� �������� ������� ��������� �����
    // - ����� ���������� ����������� �������, ��� �������� �OUT� �������� �������� � ��������� �����
    // (8 � ��������� �1, 13 � ��������� �6).
    unsigned char ucOutAddress;
    // D0 � ������� ������������ ������: 0 � ������, 1 - �������� (��������� ���� � ������).
    unsigned char ucOutInverse;
    // D0 - �������� ������� PV (������������� ��������� ��� TYPE 1 ��� ����� ����������� ������� ��� TYPE 0) - ����� � ����� 0 ���������:
    // D0 = 0 � ���������� ������� ������ AIn (� ��� 0-16383 �����. �� �����);
    // D0 = 1 � ��������� �������� �alc_cds (� ������ CoDeSys ���������� �
    // 0-100%, �������������� � ��� 0-16383);
    // D1 � D7 � ������.
    unsigned char ucControl;
    // ����� ��������� ������������ �������, ������������ �� ���� SP � ��������� �������� �alc_cds (� ������ CoDeSys ���������� � �������� ����� PV, �������������� � ��� 0-16383 ����� ������� �� ���� SP);
    // 0 � ��� ������������ �������
    unsigned char ucProgrammSPAddress;
    // ��������� � Kp - float,  IEEE754.
    float fKp;
    // ��������� � Ti - float,  IEEE754.
    float fTi;
    // ��������� � Td - float,  IEEE754.
    float fTd;
    // ����������� ������ ����� (OL 0-100%) � 1 ����� float, IEEE754.
    float fOutRestrictionLow;
    // ����������� ������ ������ (O� 0-100%) - 1 ����� float, IEEE754.
    float fOutRestrictionHigh;
    // ���������������� �������� ������ OUT ��� ��������� (OSF 0-100%) float, IEEE754.
    float fPreferredOutputValue;
    // ����� ������ LogSost [fc] CoDeSys (1-240)
    // � ������������ ��������� ������ �������������� ���������� � fc,
    // 0 � ����� fc �� ������������.
    unsigned char ucForceControlFcOnAddress;
    // ����� ������ LogSost [ha] CoDeSys (1-240) ��� ���������� ������� �/� � �������� �������������� ���������� - fc
    unsigned char ucForceControlHaOnAddress;
    // ����� ��������� �������� �alc_cds [spout] CoDeSys (1-48) ��� ���������� SP/OUT � �������� �������������� ���������� - fc
    unsigned char ucForceControlSpOutAddress;
};

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ��������� ������ ����������� ������ � ������ RAM, IEEE754.
// ����� �������������� �� ������ ������� ���� ������.
struct TAnalogueOutputRegulatorDescriptionWorkPackOne
{
    // ����� ������������� ��������� (PV=1-96), 0 � ������ ���������.
    unsigned char ucPvAddress;
    // ��� ���������� (TYPE = 0, 1, 8 � 13):
    // TYPE 0 � ����������� �������:
    // - ���� �PV� ���������� �� ����� �OUT�;
    // - ����� �OUT� ��������� �� ���.
    // TYPE 1 � ������������� ��������� (���� 2 = 0-5) ��� ��������� �������� ������� ��������� ����� (���� 2 = 8-13):
    // - ������� ������� �� ����� �SP�, �� ������� ��������� � ��;
    // - ����� �OUT� ��������� �� ��� (���� 2 = 0-5) ��� �� ���� SP ���������� ����������� ������� (���� 2 = 8-13).
    // TYPE 8 � 13 � ��������� ����������� ������� ��������� �����:
    // - � ������ ��� �� ���� �SP� ��������� ������ � ��;
    // - � ������ ��ϻ �� ���� �SP� ��������� ����� �OUT� ���������� �������� �������: 8 � ��������� �1, 13 � ��������� �6;
    // - ����� �OUT� ��������� �� ���.
    unsigned char ucRegulatorType;
    // ����� ���������� ��������� ������� �OUT�:
    // 0 � 5 � ����� ����������� ������ ���� (���) ���:
    // - TYPE 0 � ����������� �������;
    //  - TYPE 1 � ������������� ���������;
    //  - TYPE 8 � 13 � ���������� ������ ��������� �����);
    // 8 � 13 � ���������� ������ �OUT� ��� TYPE 1 � ��������� �������� ������� ��������� �����
    // - ����� ���������� ����������� �������, ��� �������� �OUT� �������� �������� � ��������� �����
    // (8 � ��������� �1, 13 � ��������� �6).
    unsigned char ucOutAddress;
    // D0 � ������� ������������ ������: 0 � ������, 1 - �������� (��������� ���� � ������).
    unsigned char ucOutInverse;
    // D0 - �������� ������� PV (������������� ��������� ��� TYPE 1 ��� ����� ����������� ������� ��� TYPE 0) - ����� � ����� 0 ���������:
    // D0 = 0 � ���������� ������� ������ AIn (� ��� 0-16383 �����. �� �����);
    // D0 = 1 � ��������� �������� �alc_cds (� ������ CoDeSys ���������� �
    // 0-100%, �������������� � ��� 0-16383);
    // D1 � D7 � ������.
    unsigned char ucControl;
    // ����� ��������� ������������ �������, ������������ �� ���� SP � ��������� �������� �alc_cds (� ������ CoDeSys ���������� � �������� ����� PV, �������������� � ��� 0-16383 ����� ������� �� ���� SP);
    // 0 � ��� ������������ �������
    unsigned char ucProgrammSPAddress;
    // ��������� � Kp - float,  IEEE754.
    float fKp;
    // ��������� � Ti - float,  IEEE754.
    float fTi;
    // ��������� � Td - float,  IEEE754.
    float fTd;
    // ����������� ������ ����� (OL 0-100%) � 1 ����� float, IEEE754.
    float fOutRestrictionLow;
    // ����������� ������ ������ (O� 0-100%) - 1 ����� float, IEEE754.
    float fOutRestrictionHigh;
    // ���������������� �������� ������ OUT ��� ��������� (OSF 0-100%) float, IEEE754.
    float fPreferredOutputValue;
    // ����� ������ LogSost [fc] CoDeSys (1-240)
    // � ������������ ��������� ������ �������������� ���������� � fc,
    // 0 � ����� fc �� ������������.
    unsigned char ucForceControlFcOnAddress;
    // ����� ������ LogSost [ha] CoDeSys (1-240) ��� ���������� ������� �/� � �������� �������������� ���������� - fc
    unsigned char ucForceControlHaOnAddress;
    // ����� ��������� �������� �alc_cds [spout] CoDeSys (1-48) ��� ���������� SP/OUT � �������� �������������� ���������� - fc
    unsigned char ucForceControlSpOutAddress;
};
#pragma pack(pop)

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ������ ���������� ���������� ����������� �CONT_AV�
struct TMtvi5ContAvDataPackOne
{
    float fSp;
    float fOut;
    float fPv;
};
#pragma pack(pop)

// ��������� ������ ���������� ���������� ����������� �CONT_AV�
struct TMtvi5ContAvData
{
    float fSp;
    float fOut;
    float fPv;
};

// ��������� ������, ��� �������������� � "CodeSys", ����� ����������� ������.
// �� Linux � CodeSys.
struct TDataExchangeLinuxToCodeSysBuff
{
    // ���������� �������� ������� ���������� ��������.
    float afAiValue[AI_VALUE_FLOAT_ARRAY_LENGTH];
    // ������ �������, �������� ������� (64 ������� FLOAT)
    float afHandUst[HAND_UST_FLOAT_ARRAY_LENGTH];
    // ���������� ������� �����������
    float afContAv[CONT_AV_FLOAT_ARRAY_LENGTH];
    // ��������� ������� ���������� ��������.
    unsigned char aucDiValue[DI_VALUE_BIT_ARRAY_LENGTH];
    // ��������� ������� ��.
    unsigned char aucSostDo[SOST_DO_BIT_ARRAY_LENGTH];
    // ������ ������ ��������������� ���������� ��������.
    unsigned char aucBadAi[BAD_AI_BIT_ARRAY_LENGTH];
    // ������ ������ ��������������� ���������� ��������.
    unsigned char aucBadDi[BAD_DI_BIT_ARRAY_LENGTH];
    // ������� ������� ���������� �������.
    unsigned char aucLogCom[LOG_COM_BIT_ARRAY_LENGTH];
    // ������ �������������� ������ �� ������� ����.
    unsigned char aucFbOut[CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_LENGTH];
    // ���������� ������� �����������
    unsigned char aucContSt[CONT_ST_BIT_ARRAY_LENGTH];

    unsigned char fucGlobalKvit; // ���� - ������������.
    unsigned char fucGlobalReset; // ���� - �����.
};

// ��������� ������, ��� �������������� � "CodeSys", ����� ����������� ������.
// �� CodeSys � Linux.
struct TDataExchangeCodeSysToLinuxBuff
{
    // ������ �������� ��������� ���������� CoDesys.
    float afCalcValue[CALC_CDS_FLOAT_ARRAY_LENGTH];
    // ������ �������� �������� ���������� CoDesys.
    float afSysValue[SYS_CDS_FLOAT_ARRAY_LENGTH];
    // ���������� ����������� ��������.
    unsigned char aucDoValue[DO_VALUE_BIT_ARRAY_LENGTH];
    // ��������� ������� �������������� ������ CoDeSys.
    unsigned char aucLogSost[LOG_SOST_BIT_ARRAY_LENGTH];

    unsigned char Sound; // ���� - ����.
};

//#pragma pack(push)
//#pragma pack(4)
// ��������� ������, ��� �������������� � "CodeSys", ����� ����������� ������.
struct TDataExchangeControlBuff
{
    // ������������ �������: ���������� ������� xPlcConfigDataTemp.xConfigData, ���� - TConfigData.
//    unsigned char aucConfigData[sizeof(struct TConfigData)];
    TConfigData xConfigData;
    // ������� �����.
    // ���� 1 - ������� 0 � 59;
    // ���� 2 - ������ 0 �59;
    // ���� 3 - ��� 0 � 23;
    // ���� 4 - ���� 1 � 31;
    // ���� 5 - ����� 1 � 12.
    // ���� 6 - ��� 0 � 99.
    // ���� 7 � ���� ������ 1 � 7.
    unsigned char aucCurrentTime[CURRENT_TIME_BYTE_QUANTITY];
    // ���� - ������ ��� ������ ������.
    bool fbDataExchangeReady;
};
//#pragma pack(pop)

// ��������� ������������ ���������� ����������� ������������ �� HMI.
struct TAlarmHmi
{
    // ��������� ������������.
    // D0-D4 - ��� (������): 0,1-24(0-��� ���.�� �����).
    // D6 - ���1 � ��� ������������ ��������� ����������� �������.
    // D7 - ���2 � ��� ������������ ��������� ����������� �������.
    // 00 � ��� ������������
    // 01 � ����������������� ������������
    // 10 � ��������� ������������
    // 11 � ��������� (������ ���� ��� ��������� �������)
    uint8_t ui8AlarmState;
    // ���� ui8Archive = 1, ������������.
    uint8_t ui8Archive;
};

// ��������� ���������� - "��������� �������� ����������� �������", � ���� ������.
// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
struct TDiscreteSygnalTextDescriptorPackOne
{
    // ��������� �������� ����������� ������� � 14 �������� ASCII.
    char acTextDescriptor[DISCRETE_SYGNAL_NAME_LENGTH];
};
#pragma pack(pop)

// ��������� ���������� - "��������� �������� ����������� �������", � ���� ������.
struct TDiscreteSygnalTextDescriptor
{
// ��������� �������� ����������� ������� � 14 �������� ASCII, ���� ���� - ������� ����� ������.
    char acTextDescriptor[DISCRETE_SYGNAL_NAME_LENGTH + END_OF_STRING_LENGTH];
};

//extern uint8_t ui8TempCommand;

//extern CCommunicationDeviceInterface xSpiCommunicationDevice;
extern CSpi xSpiCommunicationDevice;
//extern CSpiInterface xSpiCommunicationDevice;
extern CSerialPort xSerialPortCommunicationDevice;

extern TPlcDataBaseControl xPlcDataBaseControl; // ������ � ������ - ���������� ����� ������.
extern TAllModulesContext xAllModulesContext; // ������ � ������ - �������� �������.
// ������ � ��������� ��������� ���� �������.
//extern TModuleContext xAllModulesContext.axAllModulesContext[];
// ������ � ������ - ��� �������������� ���������� ������� ��� ������ �� SPI.
extern TPlcConfigSearch xPlcConfigSearch;
extern TPlcConfigSearch xPlcConfigSearchTemp;
extern TPlcConfigData xPlcConfigData; // ������ � ������ - ������������.
extern TPlcConfigData xPlcConfigDataTemp; // ������ � ������ - ����� ��������� ������������.
extern TPlcConfigData xPlcConfigDataTransmit; // ������ � ������ - ������������ ������������� ������������.
extern TPlcConfigData *pxPlcConfigData;
extern TPlcConfigService xPlcConfigService; // ������ � ������ - ��������� ����������.
extern TPlcConfigService *pxPlcConfigService;
// ������ � ������ - ���� ������.
// ����������� �� ����� � �������� ��� ��������� �� ���������� ���� ������.
extern TPlcDataBase xPlcDataBase;
extern TPlcDataBase *pxPlcDataBase;
extern struct TMainFlagRegister xMainFlagRegister;
// ������ � ������ - ������� ������ ������������ ���������� ����������� ���������� �������� �� HMI.
extern const struct TAlarmHmi xAlarmHmiNull;
// ������� �����.
extern struct tm xCurrentTime;
extern uint8_t ui8CurrentTimeUpdateDelayCounter;
extern uint8_t ui8CurrentTimeSaveDelayCounter;

extern TDataExchangeLinuxToCodeSysBuff *pxDataExchangeLinuxToCodeSysBuff; // ��������� �� ������ � ����������� ������.
extern TDataExchangeCodeSysToLinuxBuff *pxDataExchangeCodeSysToLinuxBuff; // ��������� �� ������ � ����������� ������.
extern TDataExchangeControlBuff *pxDataExchangeControlBuff; // ��������� �� ������ � ����������� ������.

extern pthread_mutex_t xMainDataAccessMutex;
extern unsigned char ucMainCycleTimeStart; // ���� ���������� �������� �������� �����.
extern unsigned char ucCodeSysExchangeErrorCycleCounter;
extern unsigned char ucDataBaseError; // ���� ������ ��� ������� � ����������� ����������������.
extern unsigned char ucModuleError; // ���� ������ ������� ��� ������� � ����������� ������.
extern unsigned char nucDataBaseBlockIndex; // �������� ����� ����� ���� ������ �� Modbus.
extern unsigned char ucModulesQuantity;  // ������������� ���������� ���������� �������, �������� ������ �� ��������� (argv[1]).
//// ������ ������������� ������ ����������� ����� ������ � ������� �� ���� SPI.
//extern const uint8_t aui8ModuleSlotNumberToSpiAddressMatching[];
extern const unsigned char aucDataBaseBlockLength[]; // ������ � ����������� �������� ������ ���� ������.
extern unsigned char ucModbusSlaveAddress;

extern struct sigaction MainAction;
//extern struct sigaction ArchiveSave;
extern uint8_t ui8MainRestart;

extern struct timeval  xGlobalTimeCur; // ������ � ������ - ���������� ��� ������ ������� - ������� ������ �������� �������.
extern struct timeval xGlobalTimeLast; // ������ � ������ - ���������� ��� ������ ������� - ���������� ������ �������� �������.

extern bool fbGlobalKvit; // ���� - ����� ������������.
extern bool fbGlobalReset; // ���� - ����� �����.
extern bool fbGlobalBlock; // ���� - ����� ����������.
//extern bool fbHighLevelKvit; // ���� - ������������ �� ����.
//extern bool fbHighLevelReset; // ���� - ����� �� ����.
//extern bool fbHighLevelBlock; // ���� - ���������� �� ����.
//extern bool fbMindKvit; // ���� - ������������ �� ����.
//extern bool fbMindReset; // ���� - ����� �� ����.
//extern bool fbMindBlock; // ���� - ���������� �� ����.
extern bool fbGlobalMmcCopy; // ���� - ����������� ����� mmc0 �� mmc1.
extern bool fbGlobalMmcCopyEnd; // ���� - ��������� ����������� ����� mmc0 �� mmc1.

extern bool fbOneSecondPassed;
extern bool fbTenSecondsPassed;
extern unsigned char nucOneSecondCounter;
extern unsigned char nucTenSecondsCounter;
// (*���� ����� ����. ��� HMI D6-�������. D7-��������� (����������� ������� - �������� �����������)*).
extern unsigned char ucAlarmStateCommon;
// (*���� ����� ��� ������ ������������ MAL*).
extern unsigned char fucZvkMal;
// (*����� ����� ������������, ������������ � ������. ������ ModBus ��������� ������������*)
// (*������:120,121($78,$79)-��������� ������������ ��������� ������� ���-05:
// 00 - �����, 01 ������������� ������������, 10 �������� ������������
// �����: 122(7�) - ������ ������ ������������ ��� �������
// ������:123,124($7b,$7�) - ����� ������.������ �� HMI 123 - �������. 124 - �������.
// ������:125($7d) - ���.���.����, 126,127 ($7e,$7f) - ���.����.� �����*)
extern unsigned char fucKvSbBef;
extern unsigned char nucBlinkCounter; // (*������� ��������� �������*)
extern bool fbBlinkIsOn; // (*���� �������� - TRUE, ����� - FALSE*)
extern bool fbDiscreteInputDataBaseError;
extern bool fbFunctionBlockDataBaseError;
extern bool fbPlcErrorReset;
extern bool fbPlcDataBaseFileError;
extern bool fbPlcConfigurationFileError;
extern bool fbPlcConfigurationError;
extern bool fbAnalogueInputModuleDataBaseError;
extern bool fbDiscreteOutputModuleDataBaseError;
extern bool fbLocalNetworkModuleDataBaseError;
extern bool fbAnalogueInputModuleReperPointsAdcDataBaseError;
extern bool fbMindDataExchangeError;
extern bool fbCodeSysDataExchangeError;
extern bool fbModuleMrOutOff;

//// ������� �����.
//extern uint8_t aui8CurrentTime[];
// �������� ������ - ��������� ���������� ������������� ������� ��� �������������� � �������.
// ��� �������������� � Modbus ����������������� � ������� ������ - ��������� ���������� ������������� �������.
extern unsigned char aucStatAiByteArray[];
// �������� ������ - �������� ������� ����������� ��� �������������� � �������.
// ��� �������������� � Modbus ����������������� � ������� ������ - �������� ������� �����������.
extern unsigned char aucContStByteArray[];
extern unsigned char aucContStWriteBitArray[];
extern unsigned char aucContStWriteByteArray[];
extern unsigned char aucDoValueByteArray[]; // ����� ������� ���������� ���� - ����� ���������, ��� ������� ���� ��.
extern unsigned char aucDoValueBlockByteArray[]; // ����� ������� ���������� ��������� ���� - ����������, ��� ������� ���� ��.
extern struct TTxsData axAllModulesTxsData[]; // ������ � ������� ���.
// ������ � ����������� ���������� ���������� ������.
extern float afAnalogueInputData[];
// ������ � ���������� ���������� CoDeSys.
extern float afCalcValue[];
// ������ �������� �������� ���������� CoDesys.
extern float afSysValue[];
extern unsigned char aucReperPointsAdcBufferPointer[];
// ������ � ��������������� ����� ������ ��������� ���������� ��������.
extern struct TAnalogueInputDescriptionWork axAnalogueInputDescriptionWork[MAX_HANDLED_ANALOGUE_INPUT];
// ������ � ��������������� ����� ������ ����������� ������� ����������� ������.
extern struct TAnalogueOutputRegulatorDescriptionWork axAnalogueOutputRegulatorDescriptionWork[MAX_HANDLED_ANALOGUE_OUTPUT];
// ������ � ����������� ��������� �����������.
extern struct TMtvi5ContAvData axAnalogueOutputRegulatorContAv[];
// ������ � ��������������� ����� ������ ������������.
extern struct TDimentionParameter axDimentionsParametersDescriptionWork[DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY];
extern struct TDiscreteInputDescriptionWork axDiscreteInputDescriptionWork[]; // ������ � ��������������� ����� ������ ��������� ���������� ��������.
extern TDiscreteSygnalTextDescriptor axDiscreteSygnalTextDescriptor[MAX_HANDLED_DISCRETE_INPUT]; // ������ � ��������������� ����� ������ ��������� ���������� ���������� ��������.
// ������ � ��������������� ����� ������ ������� Modbus-master.
extern struct TMbmFunction1234 axModbusMasterFunction1234DescriptionWork[EXTERNAL_MODULE_FUNCTION_1234_QUANTITY];
// ������ �� ���������������� ������� ��� ��������� ���������� ��������.
extern struct TDiscreteInputWorkingData axDiscreteInputWorkingData[];
extern struct TAlarmHmi axAlarmHmi[MAX_HANDLED_DISCRETE_INPUT + MAX_HANDLED_FUNCTION_BLOCK]; // ������� ������ ������������ ���������� ����������� ������������ ���������� �������� �� HMI.
extern struct TAlarmHmi axAlarmModules[]; // ������� ������ ������������ ���������� ����������� ������������ �������������� ������� �� HMI.
extern unsigned char aucGroupDinAction[]; // ������ ���������� ������������ ����� ���������� ��������.
extern unsigned char aucDinAction[]; // ������ ���������� ������������ ���������� ��������.
extern struct TFunctionBlockDescriptionWork axFunctionBlockDescriptionWork[]; // ������ � ��������������� ����� ������ ��������� �������������� ������.
// ������ �� ���������������� ������� ��� ��������� �������������� ������.
extern struct TFunctionBlockWorkingData axFunctionBlockWorkingData[];
// ������ � ������� ������ ������� �� ���������� ����, ��� ��������� � ��������.
extern unsigned char aucBadModuleCompareBuff[];
// ������ � ������� ������ ������� �������.
extern unsigned char aucBadModuleStateBuff[];

extern unsigned char aucCoilsArray[]; // ������� ������ modbus ��������� ���������� �����,
extern unsigned char aucDiscreteInputsArray[]; // ������� ������ modbus ���������� ������,
extern unsigned char aucHoldingRegistersArray[]; // ������� ������ modbus ��������� �������� ���������� ��������,
extern unsigned char aucInputRegistersArray[]; // ������� ������ modbus ��������� ���������� ������,

//-----------------------------------------------------------------------------------------------------
// �������� modbus.
// ������� �������, ��������� ������.
extern struct modbus_mapping_t *pxCommonModbusMapping;

extern struct modbus_t xMasterRtuCtx;
extern struct modbus_t *pxMasterRtuCtx;
extern struct modbus_mapping_t xMasterRtuModbusMapping;
extern struct modbus_mapping_t *pxMasterRtuModbusMapping;
//extern unsigned char aucMasterRtuCoilsArray[COILS_WORK_ARRAY_LENGTH];
//extern unsigned char aucMasterRtuDiscreteInputsArray[DISCRETE_INPUTS_ARRAY_LENGTH];
//extern unsigned char aucMasterRtuHoldingRegistersArray[HOLDING_REGISTERS_ARRAY_LENGTH];
//extern unsigned char aucMasterRtuInputRegistersArray[INPUT_REGISTERS_ARRAY_LENGTH];
extern unsigned char aucMasterRtuAuxiliaryArray[];

extern struct modbus_t xMasterTcpCtx;
extern struct modbus_t *pxMasterTcpCtx;
extern struct modbus_mapping_t xMasterTcpModbusMapping;
extern struct modbus_mapping_t *pxMasterTcpModbusMapping;
//extern unsigned char aucMasterTcpCoilsArray[COILS_WORK_ARRAY_LENGTH];
//extern unsigned char aucMasterTcpDiscreteInputsArray[DISCRETE_INPUTS_ARRAY_LENGTH];
//extern unsigned char aucMasterTcpHoldingRegistersArray[HOLDING_REGISTERS_ARRAY_LENGTH];
//extern unsigned char aucMasterTcpInputRegistersArray[INPUT_REGISTERS_ARRAY_LENGTH];
extern unsigned char aucMasterTcpAuxiliaryArray[];

#ifdef MODBUS_TCP_ENABLE
extern uint8_t ui8ModbusTcpRestart;
extern pthread_t xModbusTcp;
extern struct sigaction ModbusTcpAction;
extern struct modbus_t xSlaveTcpCtx;
extern struct modbus_t *pxSlaveTcpCtx;
extern struct modbus_mapping_t xTcpModbusMapping;
extern struct modbus_mapping_t *pxTcpModbusMapping;
extern unsigned char aucTcpCoilsArray[COILS_WORK_ARRAY_LENGTH];
extern unsigned char aucTcpDiscreteInputsArray[DISCRETE_INPUTS_ARRAY_LENGTH];
extern unsigned char aucTcpHoldingRegistersArray[HOLDING_REGISTERS_ARRAY_LENGTH];
extern unsigned char aucTcpInputRegistersArray[INPUT_REGISTERS_ARRAY_LENGTH];
extern unsigned char aucTcpAuxiliaryArray[];
#endif // MODBUS_TCP_ENABLE

#ifdef MODBUS_RTU_ENABLE
extern uint8_t ui8ModbusRtuRestart;
extern pthread_t xModbusRtu;
extern struct sigaction ModbusRtuAction;
extern struct modbus_t xSlaveRtuCtx;
extern struct modbus_t *pxSlaveRtuCtx;
extern struct modbus_mapping_t xRtuModbusMapping;
extern struct modbus_mapping_t *pxRtuModbusMapping;
extern unsigned char aucRtuCoilsArray[COILS_WORK_ARRAY_LENGTH];
extern unsigned char aucRtuDiscreteInputsArray[DISCRETE_INPUTS_ARRAY_LENGTH];
extern unsigned char aucRtuHoldingRegistersArray[HOLDING_REGISTERS_ARRAY_LENGTH];
extern unsigned char aucRtuInputRegistersArray[INPUT_REGISTERS_ARRAY_LENGTH];
extern unsigned char aucRtuAuxiliaryArray[];
#endif // MODBUS_RTU_ENABLE

#ifdef MODBUS_HMI_ENABLE
extern uint8_t ui8ModbusHmiRestart;
extern pthread_t xHmiModbusRtu;
extern struct sigaction ModbusHmiAction;
extern struct modbus_t xSlaveHmiRtuCtx;
extern struct modbus_t *pxSlaveHmiRtuCtx;
extern struct modbus_mapping_t xHmiRtuModbusMapping;
extern struct modbus_mapping_t *pxHmiRtuModbusMapping;
extern unsigned char aucHmiRtuCoilsArray[COILS_WORK_ARRAY_LENGTH];
extern unsigned char aucHmiRtuDiscreteInputsArray[DISCRETE_INPUTS_ARRAY_LENGTH];
extern unsigned char aucHmiRtuHoldingRegistersArray[HOLDING_REGISTERS_ARRAY_LENGTH];
extern unsigned char aucHmiRtuInputRegistersArray[INPUT_REGISTERS_ARRAY_LENGTH];
extern unsigned char aucHmiAuxiliaryArray[];
#endif // MODBUS_HMI_ENABLE

extern pthread_t xExternalModuleInteractionCycle;      // this is our handle to the pthread

////-----------------------------------------------------------------------------------------------------
//extern struct modbus_mapping_t *pxCommonModbusMapping;
//#ifdef MODBUS_TCP_ENABLE
//extern struct modbus_t xSlaveTcpCtx;
//extern struct modbus_t *pxSlaveTcpCtx;
//extern struct modbus_mapping_t xTcpModbusMapping;
//extern struct modbus_mapping_t *pxTcpModbusMapping;
//extern unsigned char aucTcpCoilsArray[COILS_WORK_ARRAY_LENGTH];
//extern unsigned char aucTcpDiscreteInputsArray[DISCRETE_INPUTS_ARRAY_LENGTH];
//extern unsigned char aucTcpHoldingRegistersArray[HOLDING_REGISTERS_ARRAY_LENGTH];
//extern unsigned char aucTcpInputRegistersArray[INPUT_REGISTERS_ARRAY_LENGTH];
//#endif // MODBUS_TCP_ENABLE
//
//#ifdef MODBUS_RTU_ENABLE
//
//#endif // MODBUS_RTU_ENABLE
//
//#ifdef MODBUS_HMI_ENABLE
//
//#endif // MODBUS_HMI_ENABLE

//SPI MRAM-----------------------------------------------------------------------------------------------------
#define SPI_MRAM_DEV "/dev/mtd0"

#define FRAM_BAD_MODULES_BIT_ARRAY_OFFSET 0
#define FRAM_LAST_SAVED_TIME_OFFSET FRAM_BAD_MODULES_BIT_ARRAY_OFFSET + BAD_MODULES_BIT_ARRAY_LENGTH

int iFramRead(unsigned char* pucDestination, unsigned long ulSource, unsigned int nuiLength);
int iFramWrite(unsigned long ulDestination, unsigned char* pucSource, unsigned int nuiLength);


float fFloatLsbToMsbByteOrderConverter(float fData);
float fFloatMsbToLsbByteOrderConverter(float fData);
void vLittleToBigEndianFloatConverter(unsigned char* pucDestination, unsigned char* pucSource, unsigned int nuiLength);
void vBigToLittleEndianFloatConverter(unsigned char* pucDestination, unsigned char* pucSource, unsigned int nuiLength);
void vAnalogueInputToScalableCodeConverter(void);
//void vAnalogueInputDataBaseToScalableCodeConverter(void);
//void vAnalogueInputDescriptionModbusExstract(void);
void vAnalogueInputDescriptionWorkExstract(void);
void vAnalogueInputDescriptionWorkToModbus(void);
int iDimentionsParametersDataBaseCreate(void);
void vDimentionsParametersDataBaseExstract(void);
void vModbusMasterModuleDataBaseExstract(void);
void vDiscreteSygnalTextDescriptorDataBaseExstract(void);
void vAnalogueOutputDescriptionWorkExstract(void);
//void vAnalogueOutputDescriptionModbusExstract(void);
void vAnalogueOutputDescriptionModbusToWork(void);
void vAnalogueOutputDescriptionWorkToModbus(void);
void vAnalogueOutputDataBasePack(void);
void vAnalogueOutputContAvModbusToWork(void);
void vAnalogueOutputContAvWorkToModbus(void);
void vConfigurationDataPack(TConfigDataPackOne *pxDestination, TConfigData *pxSourse);
void vConfigurationModbusRepresentDataPack(TConfigDataModbusRepresentPackOne *pxDestination, TConfigData *pxSourse);
//void vAnalogueInputDescriptionWorkBasePack(TAnalogueInputDescriptionWorkPackOne *pxDestination, TAnalogueInputDescriptionWork *pxSourse);
void vByteToBitArray(unsigned char *pucDestination, unsigned char *pucSource, int iLength);
void vBitToByteArray(unsigned char *pucDestination, unsigned char *pucSource, int iLength);
void vByteToShortArray(unsigned short *pusDestination, unsigned char *pucSource, int iLength);
int iBufferIsZero(unsigned char *pucSource, int iLength);
void vDataBaseBlockCrcCreate(void);
void vDataBaseOneBlockCrcCreate(TPlcDataBaseBlock *pxSource);
void vModulesContextStaticCrcCreate(TAllModulesContext *pxSource);
int iModulesContextStaticCrcCheck(TAllModulesContext *pxSource);
void vWorkArraysUpdate(void);
void vCurrentTimeUpdate(void);

void vDeviceControlModbusMessageHandler(modbus_mapping_t *pxModbusMapping);
void vDeviceControlModbusFunction5Handler(modbus_mapping_t *pxModbusMapping);
void vDeviceControlModbusFunction15Handler(modbus_mapping_t *pxModbusMapping);
void vDeviceControlModbusFunction16Handler(modbus_mapping_t *pxModbusMapping);
void vDeviceControlConfigurationRequesHandler(modbus_mapping_t *pxModbusMapping);
void vDeviceControlDataBaseBlockReadHandler(modbus_mapping_t *pxModbusMapping);
void vDeviceControlDataBaseBlockWriteHandler(modbus_mapping_t *pxModbusMapping);
void vDeviceControlOnlineDataReadHandler(modbus_mapping_t *pxModbusMapping);
void vLinuxCurrentTimeSet(unsigned char *pucSource);
void vCurrentTimeSet(modbus_mapping_t *pxModbusMapping);

void vMain100MsCycle(void);
int iModbusTcpMasterSet(void);
int iModbusRtuMasterSet(void);
int iModbusRtuMasterReset(void);
int iPlcDataBaseCheck(void);
int iPlcModulesSearch(void);
uint8_t ui8DiscreteAndAnalogueModuleQuantityCompare(void);
int iPlcConfigurationCheck(void);
int iPlcConfigurationCreate(void);
int iPlcConfigurationCompare(void);
int iPlcModuleReplacementCheck(void);
int iPlcConfigurationSave(void);
int iPlcDataBaseSave(void);
int iPlcGetModulesDataBase(void);
//int iPlcDataBaseBlockWrite(unsigned char nucDataBaseBlockIndex, unsigned char *pucSource, unsigned short nusBlockLength);
int iPlcDataBaseBlockWrite(unsigned char nucDataBaseBlockIndex);
void vThreadsCreate(void);
void SharedMemoryCreate(void);
void CodeSysStart(void);
void vMainTaskMemoryMapFormation(void);
void vMainTaskMemoryMapModuleContextInitialisation(TModuleContext *pxModuleContext);
void vMainTaskMemoryMapToNextModule(TModuleContext *pxModuleContext);
void vLinuxToCodeSysDataExchange(void);
void vInternalToModbusBuffDataExchange(void);
void vGeneralTimeCounter(void);
void vAlarmHandler(void);
void vBadModulesTest(void);
int iBadModuleBuffUpdate(void);
int iBadModuleBuffRead(void);
int iBadModuleBuffClear(void);

int iMbmFunction1DataHandler(TModuleContext *pxModuleContext);
int iMbmFunction2DataHandler(TModuleContext *pxModuleContext);
int iMbmFunction3DataHandler(TModuleContext *pxModuleContext);
int iMbmFunction3OwnProductionDataHandler(TModuleContext *pxModuleContext);
int iMbmFunction4DataHandler(TModuleContext *pxModuleContext);
int iMbmFunction4OwnProductionDataHandler(TModuleContext *pxModuleContext);
void vMbmFunction15DataHandler(TModuleContext *pxModuleContext);

int iModuleMvdiDataHandler(TModuleContext *pxModuleContext);

int iModuleMvds9DataHandler(TModuleContext *pxModuleContext);
int iModuleMvds9DataRead(TModuleContext *pxModuleContext);
int iModuleMvds9ModuleDataBaseCheck(TModuleContext *pxModuleContext);
int iModuleMvds9PlcDataBaseCheck(TModuleContext *pxModuleContext);
int iModuleMvds9DataBaseBlockWrite(TModuleContext *pxModuleContext);

//void vModuleMvai5DataHandler(TModuleContext *pxModuleContext);
int iModuleMvai5ChangeProtocol(TModuleContext *pxModuleContext);

int iModuleMvai5DataHandler(TModuleContext *pxModuleContext);
void vModuleMvai5DataHandlerNew(TModuleContext *pxModuleContext);
int iModuleMvai5ReperPointsAdcRead(TModuleContext *pxModuleContext);
int iModuleMvai5TxsRead(TModuleContext *pxModuleContext);
int iModuleMvai5PlcDataBaseCheck(TModuleContext *pxModuleContext);
int iModuleMvai5ModuleDataBaseCheck(TModuleContext *pxModuleContext);
int iModuleMvai5DataBaseBlockWrite(TModuleContext *pxModuleContext);
int iModuleMvai5DataBaseBlockWriteCheck(TModuleContext *pxModuleContext);

int iModuleMrXXStartDataBaseCreate(void);
void vModuleMrXXDataHandler(TModuleContext *pxModuleContext);
void vModuleMrXXDataHandlerNew(TModuleContext *pxModuleContext);
int iModuleMrXXTransferData(TModuleContext *pxModuleContext);
int iModuleMrXXTransferDataReceive(TModuleContext *pxModuleContext);
int iModuleMrXXTransferDataReceiveError(TModuleContext *pxModuleContext);
int iModuleMrXXDataBaseBlockWrite(TModuleContext *pxModuleContext);
int iModuleMrXXDataBaseBlockWriteError(TModuleContext *pxModuleContext);
int iModuleMrXXChangeProtocol(TModuleContext *pxModuleContext);

void iModuleMtvi5DataHandler(TModuleContext *pxModuleContext);
int iModuleMtvi5DataExchange(TModuleContext *pxModuleContext);
int iModuleMtvi5ReperPointsAdcRead(TModuleContext *pxModuleContext);
int iModuleMtvi5RegulatorPlcDataBaseCheck(TModuleContext *pxModuleContext);
int iModuleMtvi5RegulatorModuleDataBaseCheck(TModuleContext *pxModuleContext);
//int iModuleMtvi5PspPlcDataBaseCheck(TModuleContext *pxModuleContext);
//int iModuleMtvi5PspModuleDataBaseCheck(TModuleContext *pxModuleContext);
int iModuleMtvi5RegulatorDataBaseBlockWrite(TModuleContext *pxModuleContext);
int iModuleMtvi5PspDataBaseBlockWrite(TModuleContext *pxModuleContext);

int iModuleMindDataExchange(void);
int iModuleMindConfigurationLoad(void);
int iModuleMindDataHandler(TModuleContext *pxModuleContext);
int16_t iMindDoingNothingFunction(uint8_t *&pucSource, int16_t &ui16Length);

int iDiscreteInputStartDataBaseCreate(void);
int iDiscreteSygnalTextDescriptorStartDataBaseCreate(void);
void vDiscreteInputDataBaseExstract(void);
int iDiscreteInputDataBaseCrcCheck(struct TDiscreteInputDescriptionWork *pxDiscreteInputDataBase);
void vDiscreteInputHandler(void);

void vFunctionBlockDataBaseExstract(void);
int iFunctionBlockDataBaseCrcCheck(struct TFunctionBlockDescriptionWork *pxFunctionBlockDataBase);
int iFunctionBlockInputCheck(unsigned char ucInputSource, unsigned short usSourceAddress);
float fGetSetPointFbl(struct TFunctionBlockDescriptionWork *pxFunctionBlockDescriptionWork);
void vFunctionBlockHandler(void);
unsigned char ucFourInputElementAnd(TFunctionBlockDescriptionWork *pxSource);
unsigned char ucFourInputElementOr(TFunctionBlockDescriptionWork *pxSource);
unsigned char ucTimer(TFunctionBlockDescriptionWork *pxSource);

int iConfigurationErrorCheck(unsigned char ucDataForCompare);

#ifdef __cplusplus
}
#endif

#endif // PASNEWCONFIG_H_INCLUDED
