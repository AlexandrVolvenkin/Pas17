#ifndef PASNEWCONFIG_H_INCLUDED
#define PASNEWCONFIG_H_INCLUDED

// документы - источники.
// PAS_05_PO.DOC, PAS_05_NEW_PO.DOC, протокол МВА.txt, запрос 71.txt,
// ПАС-19_адреса Modbus_FULL.txt ПАС-05-8(16)CD_ИЗ_ч2_ДС.pdf, ПАС-05_И3_10 2013.pdf, МЦП-19 06.05.2019.pdf, pas05_iz_1.pdf.

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

// преобразование натурального числа в целое.
#define CONVERT_NATURAL_NUMBER_TO_INTEGER 1
// преобразование целого числа в натуральное.
#define CONVERT_INTEGER_TO_NATURAL_NUMBER 1
#define END_OF_STRING 0
#define END_OF_STRING_LENGTH 1

//-----------------------------------------------------------------------------------------------------
// главные константы проекта. определяют размеры всех массивов.
#define MAX_HANDLED_DISCRETE_INPUT 384 // определяется картой памяти программатора.
#define MAX_HANDLED_DISCRETE_OUTPUT 64

// причина ограничения количества обрабатываемых функциональных блоков:
// длина массива активности сигнализации дискретных сигналов в карте памяти программатора -
// (504 / 2 = 252), 252 = 192 дискретных сигнала + 60 функциональных блока.
// 160-663(0xa0 - 0x297); массив активности сигнализации дискретных сигналов
// Дс1-Дс252, по 2 бита на сигнал (504);
//#define ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH ((MAX_HANDLED_DISCRETE_INPUT + MAX_HANDLED_FUNCTION_BLOCK) * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT)
// (источник - документ ПАС-05-8(16)CD_ИЗ_ч2_ДС. стр. 48, PAS_05_NEW_PO.DOC стр. 16.)

//(источник - PAS_05_NEW_PO.DOC стр. 16.)
//Управление сигнализацией групп дискретных сигналов на HMI панели:
//	1) Принадлежность каждого дискретного сигнала к какой-либо группе сигнализации
//	задается реквизитом №ТБЛ описателя дискретного сигнала
//	2) Стартовая БД создается с формированием групп сигнализации по физическим модулям:
//	- до 16 групп по 12 сигналов для МВДИ,по 24 сигнала для МВАИ, сетевых нет
//	- активная сигнализация - выбранным в HMI способом (цветом, прерывистым свечением)
//	- сквитированная сигнал. - выбранным в HMI способом (цветом, ровным свеченим)
//	3) Программирование групп сигнализации – программатором PRG-05FBL.
//4) Обрабатывается сигнализация, запрограммированная для ФБЛ 1-62,но
//МАХ № Дискр. сигн. = BInp + №ФБЛ МАХ не должен превышать 252.
//№ Дискр. сигн. вых. ФБЛ для сигнализации формируются как BInp + №ФБЛ.
//№ Дискр.сигн. 253 использован для сигнализации от BECK
//5) Общая сигнализация: предупредительная / аварийная мигающим транспарантом желтого / красного цвета по фиксированному адресу ModBus протокола (123,124) со сбросом по КВ и СБ.

// адреса сигнализации функциональных блоков начинаются после
// последнего адреса сигнализации дискретных сигналов.
#define MAX_HANDLED_FUNCTION_BLOCK 120 // определяется картой памяти программатора.

#define ALARM_GROUP_QUANTITY 16 // определяется картой памяти программатора.
#define MAX_HANDLED_ANALOGUE_INPUT 96
#define MAX_HANDLED_ANALOGUE_OUTPUT 12
// HMI панель.
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
#define EXTERNAL_MODULE_FUNCTION_1234_QUANTITY 32  // определяется картой памяти программатора.
#define EXTERNAL_MODULE_FUNCTION_15_QUANTITY 3
// до 32 модулей modbus function 1,2,3,4  и до 3 модулей modbus function 15.
#define EXTERNAL_MODULE_QUANTITY (EXTERNAL_MODULE_FUNCTION_1234_QUANTITY + EXTERNAL_MODULE_FUNCTION_15_QUANTITY)
// типы внешних модулей в описателях функциональных блоков «МВМ».
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

// типы модулей от 0 до 15 заняты внутренними модулями.
// внешние модули тоже имеют типы от 0 до 15.
// чтобы внутренние и внешние модули не пересекались,
// к типу внешних модулей будем прибавлять смещение
// на количество типов внутренних модулей.
#define INTERNAL_MODULE_TYPE_QUANTITY 0x10
// смещение на типы внешних модулей производства ЗАО НПП "Центравтоматика".
#define EXTERNAL_MODULE_TYPE_OFFSET INTERNAL_MODULE_TYPE_QUANTITY
// количество типов внешних модулей производства ЗАО НПП "Центравтоматика".
#define EXTERNAL_MODULE_OWN_PRODUCTION_QUANTITY 0x10
// смещение на типы внешних модулей сторонних производителей.
#define EXTERNAL_MODULE_OTHER_TYPE_OFFSET (EXTERNAL_MODULE_TYPE_OFFSET + EXTERNAL_MODULE_OWN_PRODUCTION_QUANTITY)
// от 0 до 14 идут типы модулей производства ЗАО НПП "Центравтоматика".
// они обрабатываются драйверами определёнными для этих типов.
// типы внешних модулей в системе.
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

// типы модулей производства ЗАО НПП "Центравтоматика".
// они обрабатываются драйверами определёнными для функций Modbus 1,2,3,4,15.
#define OWN_MODULE_TYPE_FUNCTION_1 (0 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)
#define OWN_MODULE_TYPE_FUNCTION_2 (1 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)
#define OWN_MODULE_TYPE_FUNCTION_3 (2 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)
#define OWN_MODULE_TYPE_FUNCTION_4 (3 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)
#define OWN_MODULE_TYPE_FUNCTION_15 (4 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)

// другие типы модулей(сторонних производителей).
// они обрабатываются драйверами определёнными для функций Modbus 1,2,3,4,15.
#define OTHER_MODULE_TYPE_FUNCTION_1 (5 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)
#define OTHER_MODULE_TYPE_FUNCTION_2 (6 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)
#define OTHER_MODULE_TYPE_FUNCTION_3 (7 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)
#define OTHER_MODULE_TYPE_FUNCTION_4 (8 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)
#define OTHER_MODULE_TYPE_FUNCTION_15 (9 + EXTERNAL_MODULE_OTHER_TYPE_OFFSET)

// коды ошибок внешних модулей локальной сети.
#define EXTERNAL_MODULE_ERROR_TIMEOUT 0x01
#define EXTERNAL_MODULE_ERROR_FE_DOR 0x02
#define EXTERNAL_MODULE_ERROR_CRC 0x03
#define EXTERNAL_MODULE_ERROR_ILLEGAL_FUNCTION 0x11
#define EXTERNAL_MODULE_ERROR_ILLEGAL_DATA_ADDRESS 0x12
#define EXTERNAL_MODULE_ERROR_ILLEGAL_DATA_VALUE 0x13
#define EXTERNAL_MODULE_ERROR_SLAVE_OR_SERVER_FAILURE 0x14
// коды ошибок внутренних модулей.
#define INTERNAL_MODULE_ERROR_DATA_BASE 0x15
#define INTERNAL_MODULE_ERROR_REPER_POINTS 0x16


#define MAX_MODULE_QUANTITY (INTERNAL_MODULE_QUANTITY + \
                             EXTERNAL_MODULE_QUANTITY + \
							 DISPLAY_MODULE_QUANTITY) \

#define PLC_DATA_BASE_BLOCK_QUANTITY 100
#define PLC_DATA_BASE_BLOCK_LENGTH 256
#define DATA_BASE_BLOCK_NUMBER_DATA_LENGTH 1
#define PLC_DATA_BASE_ERROR_LENGTH 4
// Сalc_cds:	массив выходных расчетных переменных CoDesys 48*4 = 192 байта
#define CALC_CDS_QUANTITY 48
// Sys_cds:	массив выходных системных переменных CoDesys 10*4 = 40 байт
#define SYS_CDS_QUANTITY 10
// Hand_ust:	массив уставок, вводимых вручную 64*4 = 256 байтов
#define HAND_UST_QUANTITY 64
// Av_res		резерв 64 аналоговых сигнала 64*4 = 256 байтов
#define AV_RES_QUANTITY 64
#define STAT_AI_BIT_QUANTITY 8
#define CURRENT_TIME_BYTE_QUANTITY 7
#define MODIFICATION_CODE_BYTE_QUANTITY 24

// количество дискретных входов в одном модуле.
#define DISCRETE_MODULE_INPUT_QUANTITY 12
#define ANALOGUE_INPUT_DI_VALUE_QUANTITY 4 // количество дискретных сигналов порождаемое одним аналоговым входом.
#define DISCRETE_OUTPUT_MODULE_RELAY_OUTPUT_QUANTITY 64 // количество дискретных выходов управления реле.
// количество аналоговых входов в одном модуле.
#define ANALOG_MODULE_INPUT_QUANTITY 6
// длина строки текстового реквизита аналогового измерения.
#define ANALOGUE_INPUT_NAME_LENGTH 8
// длина строки текстового реквизита размерности измеряемого параметра КРЗМ.
#define DIMENSIONS_PARAMETERS_NAME_LENGTH 6
// длина строки текстового реквизита дискретного сигнала.
#define DISCRETE_SYGNAL_NAME_LENGTH 14
// количество аналоговых выходов в одном модуле.
#define ANALOG_OUTPUT_MODULE_REGULATORS_QUANTITY 6
#define ONLINE_DATA_READ_ONE_INPUT_BYTE_QUANTITY 8 // 7 байт ASCII символы конвертированные из float AiValue, 1 байт StatAi
#define ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY 7 // 7 байт ASCII символы конвертированные из float AiValue
#define ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY 1 // 1 байт StatAi - состояние аналоговых измерительных каналов
#define CALIBRATION_ANALOG_MODULE_NUMBER_MASK 0x00F0 // маска. адрес Modbus function 5 - 0x00X0, X - номер модуля.
#define CALIBRATION_INPUT_NUMBER_MASK 0x000F // маска. адрес Modbus function 5 - 0x000Y, Y - номер калибруемого входа.
// функция Modbus 71 - _FC_ONLINE_DATA_READ modbus_reply() modbus.cpp.
// если в старшем регистре адреса Modbus бит7 = 0, то запрашиваются реперные точки - (бит0 - бит6) - адрес аналогового входа.
#define ANALOGUE_INPUT_ADDRESS_MASK 0x7F
// если в старшем регистре адреса Modbus бит7 = 1, то запрашивается ТХС и (бит0 - бит2) - относительный адрес модуля МВСТ3.
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

// идентифицируют интерфейс - источник входящего сообщения Modbus.
#define MODBUS_MESSAGE_SOURCE_TCP 1
#define MODBUS_MESSAGE_SOURCE_RTU_HIGH_LEVEL 2
#define MODBUS_MESSAGE_SOURCE_RTU_HMI 3
#define MODBUS_MESSAGE_SOURCE_RTU_MASTER 4
#define MODBUS_MESSAGE_SOURCE_TCP_MASTER 5

//-----------------------------------------------------------------------------------------------------
#define MAIN_CYCLE_TIME 100000000 // время главного цикла - 100mc.
//#define MAIN_CYCLE_TIME 100000 // время главного цикла - 100mc.
#define EXTERNAL_CYCLE_STOP_SLEEP_TIME 1000 // длительность "сна", во время ожидания флага синхронизации с главным циклом.

//#define MAIN_DATA_ACCESS_SEMAPHORE_NAME "/MainDataAccessSemaphore"
//#define MAIN_DATA_ACCESS_MUTEX_NAME "/MainDataAccessMutex"

#define ONE_SECOND_TIME 10 // количество периодов главного цикла - MAIN_CYCLE_TIME.
#define TWO_SECOND_TIME 20
#define THREE_SECOND_TIME 30
#define TEN_SECOND_TIME 100

#define MODULE_REQUEST_QUANTITY 5 // количество попыток связаться с модулем.
#define MBM_1234_MODULE_BAD_BIT_ARRAY_LENGTH 1
#define BITS_IN_BYTE_QUANTITY 8
// начальное значение счётчика запросов модуля без ответа.
// если равно нулю - модуль считается неисправным. его данные признаются недостоверными.
// устанавливается флаг сигнализации в рабочем массиве aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET + индекс модуля].
#define BAD_MODULE_CYCLE_COUNT_DEFAULT 10
// начальное значение счётчика запросов модуля без ответа в течении 60 секунд.
#define BAD_MODULE_60_SECOND_CYCLE_COUNT_DEFAULT 600
#define BAD_MODULE_RESPONDED_OK 0
// код ошибки когда модуль не отвечает.
// модуль не отвечает.
#define BAD_MODULE_NOT_RESPONDED 1
// модуль не отвечает при старте.
#define BAD_MODULE_NOT_RESPONDED_ON_START 2
// сигнатура ошибки. устанавливается если измеренное значение недостоверно.
#define ANALOGUE_INPUT_ERROR 0xFF00

// ошибки проверки конфигурации при старте.
//    1. ошибка последовательности - код 01.
#define CONFIGURATION_ERROR_SEQUENCE_ERROR 0x01
//    2. есть пустые слоты - "дырки" - код 02.
#define CONFIGURATION_ERROR_EMPTY_SLOT 0x02
//    4. новая конфигурация и база данных - код 03.
#define CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE 0x03
//    4. новая конфигурация - код 04.
#define CONFIGURATION_ERROR_NEW_CONFIGURATION 0x04
//    3. есть слот с отсутствующим модулем с зарегистрированным отказом - код 05.
#define CONFIGURATION_ERROR_BAD_MODULE_FINDED_AND_REGISTERED 0x05

// контроль системных ошибок.
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
// смещение на контрольную сумму блока базы данных.
#define PLC_DATA_BASE_BLOCK_CRC_OFFSET (PLC_DATA_BASE_BLOCK_LENGTH - TWO_BYTE_CRC_LENGTH)
// 2 - смещение на тип модуля в массиве RX SPI. 0 - команда запроса, 1 - "эхо" команды запроса.
#define MODULE_TYPE_OFFSET 2
#define MODULE_COMMAND_LENGTH 1
// количество байт в ответе на запрос типа модуля.
#define MODULE_COMMAND_ANSWER_LENGTH 1
// количество байт в "хвосте" ответа модуля. 0x7E - OK, 0xE7 - error.
#define TAIL_ANSWER_LENGTH 1
#define DATA_EXCHANGE_OK 0x7E
#define DATA_EXCHANGE_ERROR 0xE7
#define DATA_EXCHANGE_CRC_ERROR 0x03
// MVAI5_REPER_POINTS_ADC_CHANNEL_QUANTITY + MVAI5_ANALOG_INPUT_QUANTITY// 6х7=42.
#define ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH 42

// максимальное количество адресов на шине SPI.
#define SPI_CHIP_SELECT_MAX_ADDRESS 16
// 2 - смещение на данные в массиве RX SPI. 0 - команда запроса, 1 - "эхо" команды запроса.
#define SPI_DATA_BYTE_OFFSET 2
// 1 - смещение на команду в массиве RX SPI.
#define SPI_COMMAND_BYTE_OFFSET 1
#define SPI_COMMAND_BYTE_LENGTH 1

#define SPI_LENGTH_BYTE_OFFSET 2
#define SPI_LENGTH_BYTE_LENGTH 1
// первый байт - команда, второй - ответ, дальше данные.
#define SPI_PREAMBLE_LENGTH 2

#define PERCENT_100 100 // сто процентов.
#define MEASURE_ERROR_THRESHOLD 3 // порог ошибки измеренного значения.

#define ANALOGUE_INPUT_SET_POINT_VIOLATION_L 0x03 // 3 – нарушение уставки L.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L 0x04 // 4 – нарушение уставок LL+L.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_H 0x05 // 5 – нарушение уставки H.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H 0x06 // 6 – нарушение уставок Н+НН.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK 0x07
#define ANALOGUE_INPUT_CHANNEL_CALIBRATION 0x20 // канал находится в режиме калибровки.
#define ANALOGUE_INPUT_LINE_BREAK 0x40 // обрыв линии канала.
#define ANALOGUE_INPUT_CALCULATION_OVERFLOW 0x80 // переполнение при расчетах.
// смещения на флаги нарушения уставок в массиве DI_value для одного аналогового входа.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L_OFFSET 0 // смещение уставок LL+L.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET 1 // смещение уставки L.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET 2 // смещение уставки H.
#define ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H_OFFSET 3 // смещение уставок Н+НН.

// коды состояния аналогового измерения для передачи и интерпретации в МИНД.
#define HMI_ANALOGUE_INPUT_LINE_BREAK 1 // обрыв линии канала.
#define HMI_ANALOGUE_INPUT_CALCULATION_OVERFLOW 2 // переполнение при расчетах.
#define HMI_ANALOGUE_INPUT_CHANNEL_CALIBRATION 3 // канал находится в режиме калибровки.
#define HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_L 4 // 3 – нарушение уставки L.
#define HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L 5 // 4 – нарушение уставок LL+L.
#define HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_H 6 // 5 – нарушение уставки H.
#define HMI_ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H 7 // 6 – нарушение уставок Н+НН.

////при загрузке из eclipse.
//#define CONFIG_FILE_NAME "PasConfig.dat" // имя файла конфигурации.
//#define RESERV_CONFIG_FILE_NAME "ReservPasConfig.dat" // имя резервного файла конфигурации.
//#define DATA_BASE_FILE_NAME "PasDataBase.dat" // имя файла базы данных.
//#define RESERV_DATA_BASE_FILE_NAME "ReservPasDataBase.dat" // имя резервного файла базы данных.
// при автозагрузке linux.
#define CONFIG_FILE_NAME "/home/debian/PasConfig.dat" // имя файла конфигурации.
#define RESERV_CONFIG_FILE_NAME "/home/debian/ReservPasConfig.dat" // имя резервного файла конфигурации.
#define DATA_BASE_FILE_NAME "/home/debian/PasDataBase.dat" // имя файла базы данных.
#define RESERV_DATA_BASE_FILE_NAME "/home/debian/ReservPasDataBase.dat" // имя резервного файла базы данных.

// обработка ошибок. источник - документ pas05_iz_1, стр. 109.
// состояние.
#define PREVIOUS_REQUES_WAS_NOT_A_BLOCK_TRANSFER 0x00
#define PROGRAMMING_DATA_BASE_WRITE_OK 0x01
// коды ошибок.
// предыдущий запрос не был передачей блока БД (при запросе о результатах программирования функцией 14).
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
// не корректная база данных при программировании МР.
// если не запрограммирован сброс для управляющего выхода с блокировкой.
// если не запрограммировано квитирование для сигнализирующего выхода "звук".
// если не запрограммировано квитирование и сброс для сигнализирующего выхода "свет".
#define PROGRAMMING_MR_DATA_BASE_NOT_CORRECT 0x0D
#define PROGRAMMING_MR_DATA_BASE_ERROR 0x0E

#define DATA_BASE_FLOW_CONTROL_OK 1
#define DATA_BASE_FLOW_CONTROL_ERROR 2

// константа - признак ошибки конфигурации.
#define CONFIGURATION_ERROR 0xE0

#define CONFIGURATION_FLOW_CONTROL_EXIT 0
#define CONFIGURATION_FLOW_CONTROL_BUS_CHECK 1
#define CONFIGURATION_FLOW_CONTROL_SEQUENCE_CHECK 2
#define CONFIGURATION_FLOW_CONTROL_SERVICE_DATA_CALCULATE 3
#define CONFIGURATION_FLOW_CONTROL_CONFIGURATION_CHECK 4
#define CONFIGURATION_FLOW_CONTROL_NORMAL_FLOW 5
#define CONFIGURATION_FLOW_CONTROL_ERROR_FLOW_STOP 6

// для определения корректности последовательности модулей на SPI и вычмсления ucInsideTypeIndex -
// индекса среди однотипных модулей -
// дискретного ввода, аналогово ввода,
// дискретного вывода, аналогово вывода и т.д.
// ucInsideTypeIndex будет необходим для вычисления индекса модуля, для доступа к нему.
#define DISCRETE_INPUT_MODULE_GROUP 1
#define ANOLOGUE_INPUT_MODULE_GROUP 2
#define DISCRETE_OUTPUT_MODULE_GROUP 3
#define CURRENT_OUTPUT_MODULE_GROUP 4
#define EXTERNAL_1234_MODULE_GROUP 5
#define EXTERNAL_15_MODULE_GROUP 6

#define GET_MODULE_TYPE_COMMAND 0x91 // команда - чтение типа модуля.
#define CHANGE_PROTOCOL_COMMAND 0x92 // команда - переключить на новый протокол.
#define GET_MODULE_TYPE_COMMAND_LENGTH 1
#define GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH 1
// смещение в массиве RX SPI. 0 - команда запроса, 1 - "эхо" команды запроса.
#define GET_MODULE_TYPE_COMMAND_OFFSET 1

//-----------------------------------------------------------------------------------------------------
// MIND
// коды типа модуля.
// временно. //?
#define MODULE_MIND_MODBUS_ADDRESS 0x11
#define MODULE_TYPE_MIND 0x11
#define MODULE_MIND_INDEX (INTERNAL_MODULE_QUANTITY + EXTERNAL_MODULE_QUANTITY)
// $42 - запрос на основной (циклический) обмен данными.
#define MIND_DATA_EXCHANGE_COMMAND 0x42
#define MIND_DATA_EXCHANGE_READY 0x42
#define MIND_DATA_EXCHANGE_NOT_READY 0x24
#define MIND_DATA_EXCHANGE_COMMAND_ANSWER_LENGTH 5

#define MIND_INDICATION_MODULE_TYPE_12_CELLS_LCD_4x24 0x88
#define MIND_INDICATION_MODULE_TYPE_24_CELLS_LCD_4x24 0x89
#define MIND_INDICATION_MODULE_TYPE_LCD_320x240 0x8A
#define MIND_INDICATION_MODULE_TYPE_LCD_4x24 0x8B

//-----------------------------------------------------------------------------------------------------
// карта памяти базы данных. источник - документ PAS_05_PO, стр 22, пункт 5.3.3. и PAS_05_NEW_PO.DOC стр. 19.
// смещение на блоки в базе данных.
// 1-16 - БД модулей аналогового ввода 	28х6=168.
#define ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET 1
// 17   -  БД модулей дискретного вывода 	3х64=192.
#define DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_OFFSET 17
// 18-19  - БД модулей МБМ ввода инф. 	13х16=208.
#define MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_OFFSET 18
// 20  - БД модулей МБМ вывода ф15	72х3=216.
#define MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_OFFSET 20
// 21-42 БД дискретных входных сигналов	11х18=198.
#define DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET 21
// 43-44    БД ПИД МТВИ			28х6=168.
#define CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET 43
// 45    БД размерностей			6х16=96.
#define DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET 45
// 46-69 БД текстовых реквизитов (384 ТР, 24 блока).
#define TEXT_TITLES_DATA_BASE_BLOCK_OFFSET 46
// 70-81 БД ФБЛ				19х10=190 (120 ФБЛ, 12 блоков)
#define FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET 70
// 82 – 84 (48 ФБМ, 3 блока 13*16=208 )
#define MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_OFFSET 82
// 98  - БД модулей МВДС9-NAMUR 	2х16=32.
#define DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET 98
// 99  - Сетевой № устройства		1
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

// (sizeof(struct TAnalogueInputDescriptionDataBase) * ANALOG_MODULE_INPUT_QUANTITY)// 28х6=168.
#define ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH 168
// (sizeof(struct TMrXXOneChannelDataBase) * DISCRETE_OUTPUT_MODULE_RELAY_OUTPUT_QUANTITY)// 3х64=192.
#define DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_LENGTH 192
// (sizeof(struct TMbmFunction1234PackOne) * EXTERNAL_MODULE_FUNCTION_1234_QUANTITY) + 1// 13х16=208.
#define MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_LENGTH (208 + 1)
// (sizeof(struct TMbmFunction15Sourse) * EXTERNAL_MODULE_FUNCTION_15_QUANTITY)// 72х3=216.
#define MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_LENGTH 216
// (sizeof(struct TDiscreteInputDescriptionWork) * DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)// 11х18=198.
#define DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH 198
// (sizeof(struct TAnalogueOutputRegulatorDescriptionDataBase) * ANALOG_OUTPUT_MODULE_REGULATORS_QUANTITY)// 28х6=168.
#define CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH 168
#define CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH 168
// DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY * 6// 6х16=96.
#define DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_LENGTH 96
// TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY * TEXT_TITLES_DATA_LENGTH// (384 ТР, 24 блока).
#define TEXT_TITLES_DATA_BASE_BLOCK_LENGTH 224
// (sizeof(struct TFunctionBlockDescriptionWork) * FUNCTION_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)// 19х10=190.
#define FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH 190// 19х10=190 (120 ФБЛ, 12 блоков)
#define MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_LENGTH 208// 82 – 84 (48 ФБМ, 3 блока 13*16=208 )
#define DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH 32// 2х16=32.
#define NETWORK_ADDRESS_DATA_BASE_BLOCK_LENGTH 1
#define RESERVED_DATA_BASE_BLOCK_LENGTH 1

//-----------------------------------------------------------------------------------------------------
// карта памяти массива aucCoilsArray[] modbus, function 1.
// 7.1 Функция 1 – чтение текущего состояния дискретных выходных сигналов:
// определяется картой памяти программатора. источник - документ ПАС-19_адреса Modbus_FULL и ПАС-05-8(16)CD_ИЗ_ч2_ДС.

// 00-63 (0x00 - 0x3f)  состояние выходов МР №1 – MP №8: 0-OFF.1-ON.(64 выхода);
#define SOST_DO_BIT_ARRAY_LENGTH 64
// 64 - команда запись конфигурации.
#define CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_LENGTH 1
// 65-95 ($41 - $5f) резерв логических ячеек(31);
#define RESERVED_1_BIT_ARRAY_LENGTH 31
// 96-107 ($60 - $6b) флаги инкремента SP/OUT рег-ров МТВИ (12 шт);
#define INCREMENT_SP_OUT_BIT_ARRAY_LENGTH 12
// 108-119($6c - $77) флаги декремента SP/OUT рег-ров МТВИ (12 шт);
#define DECREMENT_SP_OUT_BIT_ARRAY_LENGTH 12
// 120, 121 (0x78, 0x79) биты активности сигнализации состояния модулей ПАС-
// 05 (норма/авария) – групповая сигнализация: 00 – норма, 01 – квитированная сигнализа-
// ция, 10 – активная сигнализация;
#define MODULES_ALARM_STATE_BITS_BIT_ARRAY_LENGTH 2
// 122 (0x7a)   сигнал ошибки конфигурации при запуске (резерв);
#define CONFIGURATION_ERROR_SIGNAL_BIT_ARRAY_LENGTH 1
// 123,124(0x7b, 0x7c) общий прерывистый сигнал нарушений для HMI: 123 -
// предупредительный, 124-аварийный;
#define HMI_COMMON_INTERMITTENT_SIGNAL_BITS_BIT_ARRAY_LENGTH 2
// 125 (0x7d)   переключатель режима блокировки «0»-ВКЛ / «1»-ОТКЛ;
#define LOCK_MODE_SWITCH_BIT_ARRAY_LENGTH 1
// 126,127 (0x7e,0x7f) команды КВИТИРОВАНИЕ и СБРОС;
#define KVIT_COMMAND_BIT_ARRAY_LENGTH 1
#define RESET_COMMAND_BIT_ARRAY_LENGTH 1
// 2 бита на сигнал.
// 00 – норма, 01 – квитированная сигнализация, 10 – активная сигнализация.
// 128-159($80 - $9f) Гр1-Гр16 по 2 бита (32);
#define ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_LENGTH 32
// длина поля определяющего тип активной сигнализации одного дискретного сигнала в битах.
#define ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT 2
// 2 бита на сигнал.
// 00 – норма, 01 – квитированная сигнализация, 10 – активная сигнализация.
// 160-927; массив активности сигнализации дискретных сигналов
// Дс1-Дс384, по 2 бита на сигнал (768);
#define ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH (MAX_HANDLED_DISCRETE_INPUT * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT)
// 928-1167; массив активности функциональных блоков
// ФБ1-ФБ120 по 2 бита (340);
#define ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_LENGTH (MAX_HANDLED_FUNCTION_BLOCK * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT)
// 1168-1263 AIN1-AIN96 массив флагов недостоверности аналоговых сигналов,
// по 1 биту на сигнал (96);
#define BAD_AI_BIT_ARRAY_LENGTH MAX_HANDLED_ANALOGUE_INPUT
// 1264-1359 AIN1-AIN96 массив флагов исключения аналоговых сигналов из об-
// работки по 1 биту на сигнал (96);
#define AIN_OFF_BIT_ARRAY_LENGTH MAX_HANDLED_ANALOGUE_INPUT
// 1360-1455 ДсР1-ДсР96 массив состояния регуляторов «CONT_ST» по 1 байту на регулятор, итого 96 байтов.
#define CONT_ST_BIT_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_OUTPUT * 8)
// один модуль принимает данные для 6 регуляторов, по одному байту на регулятор - 8 бит для каждого.
#define CONT_ST_BYTE_ARRAY_LENGTH MAX_HANDLED_ANALOGUE_OUTPUT
// 1456-1575 Log_Com внешние команды управления логикой – 120.
#define LOG_COM_BIT_ARRAY_LENGTH 120

// 1576-1695 выходы функциональных блоков на текущем шаге(120
// ФБЛ).
#define CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_LENGTH MAX_HANDLED_FUNCTION_BLOCK
// 1696-1735 16 модулей + 24 рез.(по 1 биту) флаги отказов модулей	(40):
#define BAD_MODULES_BIT_ARRAY_LENGTH 40
// 1736-2119 Дс1-Дс384 массив флагов недостоверности дискретных сигналов.
#define BAD_DI_BIT_ARRAY_LENGTH MAX_HANDLED_DISCRETE_INPUT
// 2120-2359 логич. выходы CoDeSyst на тек.шаге(240).
#define LOG_SOST_BIT_ARRAY_LENGTH 240



// DO_value	управление дискретными выходами – 64.
#define DO_VALUE_BIT_ARRAY_LENGTH 64
// Stat_AI		состояние аналоговых измерительных каналов в битах – 384.
#define STAT_AI_BIT_ARRAY_LENGTH MAX_HANDLED_ANALOGUE_INPUT * STAT_AI_BIT_QUANTITY
// Dv_res		резерв 160 битовых переменных.
#define DEV_RES_BIT_ARRAY_LENGTH 160
// состояние обмоток реле МР КЗ,ОБРЫВ (128)| 2360-2487
// 8 модулей по 8 выыходов, для каждого выхода 2 бита статистики - 128 бит.
#define STAT_DO_BIT_ARRAY_LENGTH 128
// неисправность дискретных выходов МР (64)| 2488-2551
#define BAD_DO_BIT_ARRAY_LENGTH 64
// состояние токовых выходов МТВИ (12) 	 | 2552-2563
#define BAD_AO_BIT_ARRAY_LENGTH 12
// Stat_AI		состояние аналоговых измерительных каналов в байтах – 96.
// один модуль выдаёт данные для 6 каналов, по одному байту на канал - 8 бит для каждого. всего 8 модулей.
// 6x8=48
#define STAT_AI_BYTE_ARRAY_LENGTH MAX_HANDLED_ANALOGUE_INPUT


// размер массива aucCoilsArray[].
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

// максимальный адрес: команды - 0x12XX.
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
// смещения в массиве aucCoilsArray[].
// начальный адрес Modbus.
#define COILS_ARRAY_MODBUS_BEGIN_ADDRESS 0x00
// 00-63 (0x00 - 0x3f)  состояние выходов МР №1 – MP №8: 0-OFF.1-ON.(64 выхода);
#define SOST_DO_BIT_ARRAY_OFFSET 0
// 64 - команда запись конфигурации.
#define CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_OFFSET (SOST_DO_BIT_ARRAY_OFFSET + SOST_DO_BIT_ARRAY_LENGTH)
// 65-95 ($41 - $5f) резерв логических ячеек(31);
#define RESERVED_1_BIT_ARRAY_OFFSET (CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_OFFSET + CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_LENGTH)
// 96-107 ($60 - $6b) флаги инкремента SP/OUT рег-ров МТВИ (12 шт);
#define INCREMENT_SP_OUT_BIT_ARRAY_OFFSET (RESERVED_1_BIT_ARRAY_OFFSET + RESERVED_1_BIT_ARRAY_LENGTH)
// 108-119($6c - $77) флаги декремента SP/OUT рег-ров МТВИ (12 шт);
#define DECREMENT_SP_OUT_BIT_ARRAY_OFFSET (INCREMENT_SP_OUT_BIT_ARRAY_OFFSET + INCREMENT_SP_OUT_BIT_ARRAY_LENGTH)
// 120, 121 (0x78, 0x79) биты активности сигнализации состояния модулей ПАС-
// 05 (норма/авария) – групповая сигнализация: 00 – норма, 01 – квитированная сигнализа-
// ция, 10 – активная сигнализация;
#define MODULES_ALARM_STATE_BITS_BIT_ARRAY_OFFSET (DECREMENT_SP_OUT_BIT_ARRAY_OFFSET + DECREMENT_SP_OUT_BIT_ARRAY_LENGTH)
// 122 (0x7a)   сигнал ошибки конфигурации при запуске (резерв);
#define CONFIGURATION_ERROR_SIGNAL_BIT_ARRAY_OFFSET (MODULES_ALARM_STATE_BITS_BIT_ARRAY_OFFSET + MODULES_ALARM_STATE_BITS_BIT_ARRAY_LENGTH)
// 123,124(0x7b, 0x7c) общий прерывистый сигнал нарушений для HMI: 123 -
// предупредительный, 124-аварийный;
// 00 – норма, 01 – квитированная сигнализация, 10 – активная сигнализация.
#define HMI_COMMON_INTERMITTENT_SIGNAL_BITS_BIT_ARRAY_OFFSET (CONFIGURATION_ERROR_SIGNAL_BIT_ARRAY_OFFSET + CONFIGURATION_ERROR_SIGNAL_BIT_ARRAY_LENGTH)
// 125 (0x7d)   переключатель режима блокировки «0»-ВКЛ / «1»-ОТКЛ;
#define LOCK_MODE_SWITCH_BIT_ARRAY_OFFSET (HMI_COMMON_INTERMITTENT_SIGNAL_BITS_BIT_ARRAY_OFFSET + HMI_COMMON_INTERMITTENT_SIGNAL_BITS_BIT_ARRAY_LENGTH)
// 126,127 (0x7e,0x7f) команды КВИТИРОВАНИЕ и СБРОС;
#define KVIT_COMMAND_BIT_ARRAY_OFFSET (LOCK_MODE_SWITCH_BIT_ARRAY_OFFSET + LOCK_MODE_SWITCH_BIT_ARRAY_LENGTH)
#define RESET_COMMAND_BIT_ARRAY_OFFSET (KVIT_COMMAND_BIT_ARRAY_OFFSET + KVIT_COMMAND_BIT_ARRAY_LENGTH)
// 128-159($80 - $9f) Гр1-Гр16 по 2 бита (32);
#define ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_OFFSET (RESET_COMMAND_BIT_ARRAY_OFFSET + RESET_COMMAND_BIT_ARRAY_LENGTH)
// 160-927; массив активности сигнализации дискретных сигналов
// Дс1-Дс384, по 2 бита на сигнал (768);
#define ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_OFFSET (ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_OFFSET + ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_LENGTH)
// 928-1167; массив активности функциональных блоков
// ФБ1-ФБ120 по 2 бита (340);
#define ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_OFFSET (ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_OFFSET + ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH)
// 1168-1263 AIN1-AIN96 массив флагов недостоверности аналоговых сигналов,
// по 1 биту на сигнал (96);
#define BAD_AI_BIT_ARRAY_OFFSET (ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_OFFSET + ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_LENGTH)
// 1264-1359 AIN1-AIN96 массив флагов исключения аналоговых сигналов из об-
// работки по 1 биту на сигнал (96);
#define AIN_OFF_BIT_ARRAY_OFFSET (BAD_AI_BIT_ARRAY_OFFSET + BAD_AI_BIT_ARRAY_LENGTH)
// 1360-1455 ДсР1-ДсР96 массив состояния регуляторов «CONT_ST» по 1 байту на регулятор, итого 96 байтов.
#define CONT_ST_BIT_ARRAY_OFFSET (AIN_OFF_BIT_ARRAY_OFFSET + AIN_OFF_BIT_ARRAY_LENGTH)
// 1456-1575 Log_Com внешние команды управления логикой – 120.
#define LOG_COM_BIT_ARRAY_OFFSET (CONT_ST_BIT_ARRAY_OFFSET + CONT_ST_BIT_ARRAY_LENGTH)

// 1456-1575 команды управления с верхнего уровня КВУ (120 КВУ);
#define HIGH_LEVEL_COMMAND_BIT_ARRAY_OFFSET (CONT_ST_BIT_ARRAY_OFFSET + CONT_ST_BIT_ARRAY_LENGTH)

// 1576-1695 выходы функциональных блоков на текущем шаге(120
// ФБЛ).
#define CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_OFFSET (LOG_COM_BIT_ARRAY_OFFSET + LOG_COM_BIT_ARRAY_LENGTH)
// 1696-1735 16 модулей + 24 рез.(по 1 биту) флаги отказов модулей	(40):
#define BAD_MODULES_BIT_ARRAY_OFFSET (CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_OFFSET + CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_LENGTH)
// 1736-2119 Дс1-Дс384 массив флагов недостоверности дискретных сигналов.
#define BAD_DI_BIT_ARRAY_OFFSET (BAD_MODULES_BIT_ARRAY_OFFSET + BAD_MODULES_BIT_ARRAY_LENGTH)
// 2120-2359 логич. выходы CoDeSyst на тек.шаге(240).
#define LOG_SOST_BIT_ARRAY_OFFSET (BAD_DI_BIT_ARRAY_OFFSET + BAD_DI_BIT_ARRAY_LENGTH)

//// DO_value	управление дискретными выходами – 64.
//#define DO_VALUE_BIT_ARRAY_OFFSET (LOG_SOST_BIT_ARRAY_OFFSET + LOG_SOST_BIT_ARRAY_LENGTH)
//// Stat_AI		состояние аналоговых измерительных каналов в битах – 384.
//#define STAT_AI_BIT_ARRAY_OFFSET (DO_VALUE_BIT_ARRAY_OFFSET + DO_VALUE_BIT_ARRAY_LENGTH)
//// Dv_res		резерв 160 битовых переменных.
//#define DEV_RES_BIT_ARRAY_OFFSET (STAT_AI_BIT_ARRAY_OFFSET + STAT_AI_BIT_ARRAY_LENGTH)
// состояние обмоток реле МР КЗ,ОБРЫВ (128)| 2360-2487
#define STAT_DO_BIT_ARRAY_OFFSET (LOG_SOST_BIT_ARRAY_OFFSET + LOG_SOST_BIT_ARRAY_LENGTH)
// неисправность дискретных выходов МР (64)| 2488-2551
#define BAD_DO_BIT_ARRAY_OFFSET (STAT_DO_BIT_ARRAY_OFFSET + STAT_DO_BIT_ARRAY_LENGTH)
// состояние токовых выходов МТВИ (12) 	 | 2552-2563
#define BAD_AO_BIT_ARRAY_OFFSET (BAD_DO_BIT_ARRAY_OFFSET + BAD_DO_BIT_ARRAY_LENGTH)

//-----------------------------------------------------------------------------------------------------
// карта памяти массива aucDiscreteInputsArray[] modbus, function 2.
// 7.2 Функция 2– чтение текущего состояния дискретных входных сигналов
// определяется картой памяти программатора. источник - документ ПАС-19_адреса Modbus_FULL и ПАС-05-8(16)CD_ИЗ_ч2_ДС.

// 00-383 Din1 - DIN384:  состояние входных дискретных сигналов: 0-OFF, 1-ON,
// МАХ может передаваться до 384 дискретных сигналов;
#define DI_VALUE_BIT_ARRAY_LENGTH MAX_HANDLED_DISCRETE_INPUT
// размер массива aucDiscreteInputsArray[].
#define DISCRETE_INPUTS_ARRAY_LENGTH 8192
//#define DISCRETE_INPUTS_ARRAY_LENGTH DI_VALUE_BIT_ARRAY_LENGTH
// смещения в массиве aucDiscreteInputsArray[].
// начальный адрес Modbus.
#define DISCRETE_INPUTS_ARRAY_MODBUS_BEGIN_ADDRESS 0x00
// 00-383 Din1 - DIN384:  состояние входных дискретных сигналов: 0-OFF, 1-ON,
// МАХ может передаваться до 384 дискретных сигналов;
#define DI_VALUE_BIT_ARRAY_OFFSET 0

//-----------------------------------------------------------------------------------------------------
// карта памяти массива aucHoldingRegistersArray[] modbus, function 3.
// 7.3 Функция 3 – чтение данных в формате float IEEE 754 (по 2 2-х байтных реги
// стра на 1 значение).
// определяется картой памяти программатора. источник - документ ПАС-19_адреса Modbus_FULL и ПАС-05-8(16)CD_ИЗ_ч2_ДС.

// порядок байт BigEndian, для передачи по сети.

// 7.3.1 Аналоговые измерения.
// 00-190 Ain1 - Аin96 измеренные значения входных аналоговых сигналов,
// МАХ может передаваться до 192 2-х байтных регистров (96 аналоговых сигналов);
#define AI_VALUE_BIG_ENDIAN_FLOAT_ARRAY_LENGTH MAX_HANDLED_ANALOGUE_INPUT
#define AI_VALUE_BIG_ENDIAN_BYTE_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_INPUT * sizeof(float))
// 7.3.2 1332 - 1342 База данных аналоговых измерительных каналов
// БД аналоговых измерительных каналов ***
//Реквизиты аналогового входа №1: НШК,ВШК,LL,L,H,HH  	| 192 - 202
//Реквизиты аналогового входа №2: НШК,ВШК,LL,L,H,HH  	| 204 - 214
//и т.д.
//Реквизиты аналогового входа №96: НШК,ВШК,LL,L,H,HH 	| 1332 - 1342
#define AI_VALUE_BIG_ENDIAN_DATA_BASE_BYTE_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_INPUT * sizeof(struct TAnalogueInputDescriptionModbusPackOne))
// 7.3.3 1344 - 1414 Аналоговые сигналы регуляторов (12*3 = 36)
#define CONT_AV_BIG_ENDIAN_BYTE_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_OUTPUT * sizeof(struct TMtvi5ContAvData))
// 7.3.4 1416 - 1558 База данных регуляторов (используется ф16) (настройки) Kp,Ti,Td,OL,OH,OSF (12*6)
#define CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_OUTPUT * sizeof(struct TAnalogueOutputRegulatorDescriptionModbusPackOne))

//  1560 - 1654 Сalc_cds:	Расчетные выходные данные CoDeSys (48)
#define CALC_CDS_FLOAT_ARRAY_LENGTH CALC_CDS_QUANTITY
#define CALC_CDS_BYTE_ARRAY_LENGTH (CALC_CDS_FLOAT_ARRAY_LENGTH * sizeof(float))
// Sys_cds:	массив выходных системных переменных CoDesys 10*4 = 40 байт
#define SYS_CDS_FLOAT_ARRAY_LENGTH SYS_CDS_QUANTITY
#define SYS_CDS_BYTE_ARRAY_LENGTH (SYS_CDS_FLOAT_ARRAY_LENGTH * sizeof(float))
// Hand_ust Ручной ввод уставок (64)			| 1656 - 1782
#define HAND_UST_FLOAT_ARRAY_LENGTH HAND_UST_QUANTITY
#define HAND_UST_BYTE_ARRAY_LENGTH (HAND_UST_FLOAT_ARRAY_LENGTH * sizeof(float))
//// Av_res		резерв 64 аналоговых сигнала 64*4 = 256 байтов
//#define AV_RES_BYTE_ARRAY_LENGTH (AV_RES_QUANTITY * sizeof(float))
// Время + дата	(1 реквизит - 1 двухбайтный регистр)	| 1784 - 1790 (сек,мин,час,дм,мес,год,дн)
// текущее время и дата: 7 2-х байтных регистров, целые числа:
// сек, мин, час, день мес, мес ,год, день недели (при чтении:1 - понедельник),
// используется для записи Ф16.
#define CURRENT_TIME_HOLDING_BYTE_ARRAY_LENGTH (CURRENT_TIME_BYTE_QUANTITY * sizeof(short))

// порядок байт LittleEndian, для внутренней работы.

// 7.3.1 Аналоговые измерения.
// Ain1 - Аin96 измеренные значения входных аналоговых сигналов 1 – 96,
// МАХ может передаваться до 192 2-х байтных регистров (96 аналоговых сигналов);
#define AI_VALUE_FLOAT_ARRAY_LENGTH MAX_HANDLED_ANALOGUE_INPUT
#define AI_VALUE_BYTE_ARRAY_LENGTH (AI_VALUE_FLOAT_ARRAY_LENGTH * sizeof(float))
// 7.3.2 База данных аналоговых измерительных каналов
#define AI_VALUE_DATA_BASE_FLOAT_ARRAY_LENGTH ((MAX_HANDLED_ANALOGUE_INPUT * sizeof(struct TAnalogueInputDescriptionModbusPackOne)) / sizeof(float))//288
#define AI_VALUE_DATA_BASE_BYTE_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_INPUT * sizeof(struct TAnalogueInputDescriptionModbusPackOne))
// 7.3.3 Аналоговые сигналы регуляторов
#define CONT_AV_FLOAT_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_OUTPUT * (sizeof(struct TMtvi5ContAvData) / sizeof(float)))//18
#define CONT_AV_BYTE_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_OUTPUT * sizeof(struct TMtvi5ContAvData))
// 7.3.4 База данных регуляторов (используется ф16)
#define CURRENT_OUTPUT_DATA_BASE_FLOAT_ARRAY_LENGTH ((MAX_HANDLED_ANALOGUE_OUTPUT * sizeof(struct TAnalogueOutputRegulatorDescriptionModbusPackOne)) / sizeof(float))//36
#define CURRENT_OUTPUT_DATA_BASE_BYTE_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_OUTPUT * sizeof(struct TAnalogueOutputRegulatorDescriptionModbusPackOne))
#define DATA_BASE_BLOCK_DATA_BYTE_ARRAY_LENGTH PLC_DATA_BASE_BLOCK_LENGTH
// размер массива aucHoldingRegistersArray[].
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
// смещения в массиве aucHoldingRegistersArray[].
// начальный адрес Modbus.
#define HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS 0x00

// порядок байт BigEndian, для передачи по сети.

// 7.3.1 Аналоговые измерения.
// 00-190 Ain1 - Аin96 измеренные значения входных аналоговых сигналов,
// МАХ может передаваться до 192 2-х байтных регистров (96 аналоговых сигналов);
#define AI_VALUE_BIG_ENDIAN_BYTE_ARRAY_OFFSET 0
// 7.3.2 192 - 1342 База данных аналоговых измерительных каналов
// реквизиты БД аналогового входа №1: НШК, ВШК, LL, L, H, HH;
// реквизиты БД аналогового входа №2: НШК, ВШК, LL, L, H, HH;
// и т.д.
// реквизиты БД аналогового входа №96: НШК, ВШК, LL, L, H, HH;
#define AI_VALUE_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET (AI_VALUE_BIG_ENDIAN_BYTE_ARRAY_OFFSET + AI_VALUE_BIG_ENDIAN_BYTE_ARRAY_LENGTH)
// 7.3.3  1344 - 1414 (12*3 = 36)  Аналоговые сигналы регуляторов
//   аналоговые сигналы регуляторов – 18 аналоговых сигналов:
// SP регулятора №1 (используется ф16);
// OUT регулятора №1 (используется ф16);
// PV регулятора №1;
#define CONT_AV_BIG_ENDIAN_BYTE_ARRAY_OFFSET (AI_VALUE_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET + AI_VALUE_BIG_ENDIAN_DATA_BASE_BYTE_ARRAY_LENGTH)
// 7.3.4 1416 - 1558 База данных регуляторов (используется ф16) (настройки) Kp,Ti,Td,OL,OH,OSF (12*6)
// реквизиты БД регулятора №1: Kp, Ti, Td, OL, OH, dSP
// реквизиты БД регулятора №2: Kp, Ti, Td, OL, OH, dSP
// и т.д.
// еквизиты БД регулятора №6: Kp, Ti, Td, OL, OH, dSP
#define CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET (CONT_AV_BIG_ENDIAN_BYTE_ARRAY_OFFSET + CONT_AV_BIG_ENDIAN_BYTE_ARRAY_LENGTH)

// порядок байт LittleEndian, для внутренней работы.


//  1560 - 1654 Сalc_cds:	Расчетные выходные данные CoDeSys (48)
#define CALC_CDS_BYTE_ARRAY_OFFSET (CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET + CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_LENGTH)
// Hand_ust Ручной ввод уставок (64)			| 1656 - 1782
#define HAND_UST_BYTE_ARRAY_OFFSET (CALC_CDS_BYTE_ARRAY_OFFSET + CALC_CDS_BYTE_ARRAY_LENGTH)
//// 7.3.7 Время + дата	(1 реквизит - 1 двухбайтный регистр)	| 1784 - 1790 (сек,мин,час,дм,мес,год,дн)
//// текущее время и дата: 7 2-х байтных регистров, целые числа:
//// сек, мин, час, день мес, мес ,год, день недели (при чтении:1 - понедельник),
//// используется для записи Ф16.
#define CURRENT_TIME_HOLDING_BYTE_ARRAY_OFFSET (HAND_UST_BYTE_ARRAY_OFFSET + HAND_UST_BYTE_ARRAY_LENGTH)
//// Av_res		резерв 64 аналоговых сигнала 64*4 = 256 байтов
//#define AV_RES_BYTE_ARRAY_OFFSET (HAND_UST_BYTE_ARRAY_OFFSET + HAND_UST_BYTE_ARRAY_LENGTH)

// измеренные значения входных аналоговых сигналов 1 – 96,
// МАХ может передаваться до 192 2-х байтных регистров (96 аналоговых сигналов);
//#define AI_VALUE_BYTE_ARRAY_OFFSET (CURRENT_TIME_HOLDING_BYTE_ARRAY_OFFSET + CURRENT_TIME_HOLDING_BYTE_ARRAY_LENGTH)
//#define AI_VALUE_BYTE_ARRAY_OFFSET (CURRENT_TIME_HOLDING_BYTE_ARRAY_OFFSET + 16)
// 7.3.2 База данных аналоговых измерительных каналов
//#define AI_VALUE_DATA_BASE_BYTE_ARRAY_OFFSET (AI_VALUE_BYTE_ARRAY_OFFSET + AI_VALUE_BYTE_ARRAY_LENGTH)
// 7.3.3 Аналоговые сигналы регуляторов
//#define CONT_AV_BYTE_ARRAY_OFFSET (AI_VALUE_DATA_BASE_BYTE_ARRAY_OFFSET + AI_VALUE_DATA_BASE_BYTE_ARRAY_LENGTH)
// 7.3.4 База данных регуляторов (используется ф16)
//#define CURRENT_OUTPUT_DATA_BASE_BYTE_ARRAY_OFFSET (CONT_AV_BYTE_ARRAY_OFFSET + CONT_AV_BYTE_ARRAY_LENGTH)

//-----------------------------------------------------------------------------------------------------
// карта памяти массива aucInputRegistersArray[] modbus, function 4.
// 7.4 Функция 4 – чтение регистров аналоговых измеренных значений в формате
// целочисленного масштабированного кода 0-16383 и других данных (по 1 2-х байтному
// регистру на 1 значение).
// определяется картой памяти программатора. источник - документ ПАС-19_адреса Modbus_FULL и ПАС-05-8(16)CD_ИЗ_ч2_ДС.

// 7.4.1 Аналоговые измерения
#define AI_VALUE_SHORT_CODE_SHORT_ARRAY_LENGTH MAX_HANDLED_ANALOGUE_INPUT
#define AI_VALUE_SHORT_CODE_BYTE_ARRAY_LENGTH (AI_VALUE_SHORT_CODE_SHORT_ARRAY_LENGTH * sizeof(short))
//// 7.4.2 База данных аналоговых измерительных каналов
//#define AI_VALUE_DATA_BASE_SHORT_CODE_SHORT_ARRAY_LENGTH ((MAX_HANDLED_ANALOGUE_INPUT * sizeof(struct TAnalogueInputShortDescriptionData)) / sizeof(short))
//#define AI_VALUE_DATA_BASE_SHORT_CODE_BYTE_ARRAY_LENGTH (MAX_HANDLED_ANALOGUE_INPUT * sizeof(struct TAnalogueInputShortDescriptionData))
//
//#define EMPTY_ADDRESS_BYTE_ARRAY_LENGTH ((1000 * sizeof(short)) - (AI_VALUE_SHORT_CODE_BYTE_ARRAY_LENGTH + AI_VALUE_DATA_BASE_SHORT_CODE_BYTE_ARRAY_LENGTH))
//
//#define EMPTY_ADDRESS_BYTE_ARRAY_OFFSET
// 7.4.3 Буфер текущего времени
#define CURRENT_TIME_BYTE_ARRAY_LENGTH (CURRENT_TIME_BYTE_QUANTITY * sizeof(short))
// 7.4.4 Коды модификации прибора
#define MODIFICATION_CODE_BYTE_ARRAY_LENGTH (MODIFICATION_CODE_BYTE_QUANTITY * sizeof(short))
#define NET_ADDRESS_BYTE_ARRAY_LENGTH (1 * sizeof(short))
// - регистр 1031 – сетевой адрес прибора (1-32) в сети ModBus, интерфейс
// RS-485.
// размер массива aucInputRegistersArray[].
#define INPUT_REGISTERS_ARRAY_LENGTH 8192
//#define INPUT_REGISTERS_ARRAY_LENGTH AI_VALUE_SHORT_CODE_BYTE_ARRAY_LENGTH + \
//                                     AI_VALUE_DATA_BASE_SHORT_CODE_BYTE_ARRAY_LENGTH + \
//                                     EMPTY_ADDRESS_BYTE_ARRAY_LENGTH + \
//                                     CURRENT_TIME_BYTE_ARRAY_LENGTH + \
//                                     MODIFICATION_CODE_BYTE_ARRAY_LENGTH + \
//                                     NET_ADDRESS_BYTE_ARRAY_LENGTH \

// смещения в массиве aucInputRegistersArray[].
// начальный адрес Modbus.
#define INPUT_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS 0x00
// 7.4.1 Аналоговые измерения
// 00-95 ($00 - $5F) измеренные значения входных аналоговых сигналов 1 – 96,
// МАХ может передаваться до 96 регистров (96 аналоговых сигналов);
#define AI_VALUE_SHORT_CODE_BYTE_ARRAY_OFFSET 0
// 7.4.3 Буфер текущего времени
// 1000-1006  7 регистров текущего времени: секунда, минута, час, день ме-
// сяца, месяц, год (0-99), день недели (1 – 7);
#define CURRENT_TIME_BYTE_ARRAY_OFFSET (1000 * sizeof(short))
// офсеты для массивов типа - short.
#define CURRENT_TIME_SECOND_OFFSET 0
#define CURRENT_TIME_MINUTE_OFFSET 1
#define CURRENT_TIME_HOUR_OFFSET 2
#define CURRENT_TIME_MONTH_DAY_OFFSET 3
#define CURRENT_TIME_MONTH_OFFSET 4
#define CURRENT_TIME_YEAR_OFFSET 5
#define CURRENT_TIME_WEEK_DAY_OFFSET 6
// 7.4.4 Коды модификации прибора
// 1007-1030 - массив кодов модификации прибора (24 регистра)
// Содержание регистров в ответе:
// - регистр 1007 - количество модулей ввода-вывода (1 – 16);
// - регистры 1008 / 1023 - коды модулей ввода (вывода) по адресам (регистр
// 1008 – код модуля ввода (вывода) с адресом «0», регистр 1023 – код модуля вво-
// да (вывода) с адресом «15»);
// - регистр 1024 - адрес последнего модуля дискретного ввода +1;
// (BIaddr_max+1), или 0, если нет модулей ввода дискретных сигналов;
// - регистр 1025 - адрес последнего модуля аналогового ввода +1
// (АIaddr_max+1), или 0 если нет модулей ввода аналоговых сигналов;
// - регистр 1026 - количество дискретных выходов (BOut);
// - регистр 1027 - количество бинарных входных сигналов (BInp);
// - регистр 1028 - количество аналоговых входных сигналов (АInp);
// - регистр 1029– тип модуля индикации : 0 – 12 ячеек световой сигнализации
// + ЖКИ (4х20), 1 – 24 ячейки световой сигнализации +ЖКИ (4х20), 2 - ЖКД графи-
// ческий,  3 – 6 ячеек световой сигнализации +ЖКИ (4х20), 4 – ЖКД графический +
// ЖКИ (4х20);
#define MODIFICATION_CODE_BYTE_ARRAY_OFFSET (CURRENT_TIME_BYTE_ARRAY_OFFSET + CURRENT_TIME_BYTE_ARRAY_LENGTH)
// - регистр 1031 – сетевой адрес прибора (1-32) в сети ModBus, интерфейс
// RS-485.
#define NET_ADDRESS_BYTE_ARRAY_OFFSET (MODIFICATION_CODE_BYTE_ARRAY_OFFSET + MODIFICATION_CODE_BYTE_ARRAY_LENGTH)

//-----------------------------------------------------------------------------------------------------
// TUST - в структуре описателей функциональных блоков «МВМ», в программе - ucTust.
// D4, D5 – формат данных, получаемых от SLAVE (только для ф3,ф.4 протокола Modbus):
#define SCALABLE_CODE_16_BIT 0 // 0 – данные в виде масштабированного кода 16 разрядов;
// вещ. числа в формате IEEE754 – 2 2-х байтных регистра;
#define IEEE754_BYTE_ORDER_MSB_LSB 1 // 1 – старшим байтом вперед(пор-к, стбм, србм, млбм);
#define IEEE754_BYTE_ORDER_LSB_MSB 2 // 2 – младшим байтом вперед(млбм, србм, стбм, пор-к);
#define IEEE754_BYTE_ORDER_LSR_MSR 3 // 3 – младшим регистром вперед(србм, млбм, пор-к, стбм).
// маска - D0 – D3 - Код типа устройства, байта ucTust;
#define TUST_MODULE_TYPE_MASK 0x0F
// маска - D4, D5 – формат данных, байта ucTust;
#define TUST_DATA_FORMAT_MASK 0x30
#define TUST_DATA_FORMAT_OFFSET 4

// Структура описателей функциональных блоков «МВМ», функции 1,2,3,4, в базе данных.
// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
struct TMbmFunction1234PackOne
{
    // Код функции протокола MODBUS (1, 2, 3, 4). если равно 0, то ничего не запрограммировано.
    unsigned char ucModbusFunction;
    unsigned char ucModbusSlaveAddress; // Адрес SLAVE в запросе MS.
    unsigned char ucAddressH; // Начальный адрес данных в запросе – старший байт.
    unsigned char ucAddressL; // Начальный адрес данных в запросе – младший байт.
    unsigned char ucQuantityH; // Количество  данных в запросе – старший байт.
    unsigned char ucQuantityL; // Количество  данных в запросе – младший байт.
    // D0 – D3 - Код типа устройства.
    // D4, D5 – формат данных в двухбайтных регистрах, получаемых от SLAVE (только для ф3,ф.4):
    // 0 – данные в виде масштабированного кода 16 разрядов;
    // 1 – 3 – вещественные числа в формате IEEE754 – 2 2-х байтных регистра.
    // Порядок байтов вещественного числа:
    // 1 – старшим байтом вперед (пор-к, стбм, србм, млбм);
    // 2 – младшим байтом вперед (млбм, србм, стбм, пор-к);
    // 3 – младшим регистром вперед (србм, млбм, пор-к, стбм).
    unsigned char ucTust;
    // Количество входов или выходов функционального блока.
    unsigned char ucIOQuantity;
    // Начальный адрес в битовом массиве Di_value (ф1, 2, 3, 4). Количество адресов определяется MBM_IO
    uint16_t ui16DiValueArrayBegin;
    // Начальный адрес в массиве аналоговых измерений Ai_value:0 – 95 (ф3, 4). Количество адресов опред. MBM_IO
    unsigned char ucAiValueArrayBegin;
    // МАХ код аналогового сигнала мл. байт (масштабир. ф3,4)
    unsigned char ucMaxCodeL;
    // МАХ код аналогового сигнала ст. байт (масштабир. ф3,4)
    unsigned char ucMaxCodeH;
};
#pragma pack(pop)

// Структура описателей функциональных блоков «МВМ», функции 1,2,3,4, в базе данных.
struct TMbmFunction1234
{
    // Код функции протокола MODBUS (1, 2, 3, 4). если равно 0, то ничего не запрограммировано.
    unsigned char ucModbusFunction;
    unsigned char ucModbusSlaveAddress; // Адрес SLAVE в запросе MS.
    unsigned char ucAddressH; // Начальный адрес данных в запросе – старший байт.
    unsigned char ucAddressL; // Начальный адрес данных в запросе – младший байт.
    unsigned char ucQuantityH; // Количество  данных в запросе – старший байт.
    unsigned char ucQuantityL; // Количество  данных в запросе – младший байт.
    // D0 – D3 - Код типа устройства.
    // D4, D5 – формат данных в двухбайтных регистрах, получаемых от SLAVE (только для ф3,ф.4):
    // 0 – данные в виде масштабированного кода 16 разрядов;
    // 1 – 3 – вещественные числа в формате IEEE754 – 2 2-х байтных регистра.
    // Порядок байтов вещественного числа:
    // 1 – старшим байтом вперед (пор-к, стбм, србм, млбм);
    // 2 – младшим байтом вперед (млбм, србм, стбм, пор-к);
    // 3 – младшим регистром вперед (србм, млбм, пор-к, стбм).
    unsigned char ucTust;
    // Количество входов или выходов функционального блока.
    unsigned char ucIOQuantity;
    // Начальный адрес в битовом массиве Di_value (ф1, 2, 3, 4). Количество адресов определяется MBM_IO
    uint16_t ui16DiValueArrayBegin;
    // Начальный адрес в массиве аналоговых измерений Ai_value:0 – 95 (ф3, 4). Количество адресов опред. MBM_IO
    unsigned char ucAiValueArrayBegin;
    // МАХ код аналогового сигнала мл. байт (масштабир. ф3,4)
    unsigned char ucMaxCodeL;
    // МАХ код аналогового сигнала ст. байт (масштабир. ф3,4)
    unsigned char ucMaxCodeH;
};

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// Структура описателей функциональных блоков «МВМ», функции 5, в базе данных.
struct TMbmFunction5PackOne
{
    unsigned char ucModbusFunction; // Код функции протокола MODBUS (1, 2, 3, 4).
    unsigned char ucModbusSlaveAddress; // Адрес SLAVE в запросе MS.
    unsigned char ucAddressH; // Начальный адрес данных в запросе – старший байт.
    unsigned char ucAddressL; // Начальный адрес данных в запросе – младший байт.
    unsigned char ucIndicatorH; // индикатор – старший байт ($FF – ON, 00 – OFF).
    unsigned char ucIndicatorL; // индикатор – младший байт (всегда 0).

    unsigned char ucTust; // D0 – D2 - Код типа устройства, D3 – D7 – резерв.
    unsigned short usIndicatorSourceAddress; // Адрес источника формирования индикатора (битовая величина).
    unsigned char ucReserved1; // Резерв.
    unsigned char ucReserved2; // Резерв.
    unsigned char ucReserved3; // Резерв.
};
#pragma pack(pop)

// Структура описателей функциональных блоков «МВМ», функции 5, в базе данных.
struct TMbmFunction5
{
    unsigned char ucModbusFunction; // Код функции протокола MODBUS (1, 2, 3, 4).
    unsigned char ucModbusSlaveAddress; // Адрес SLAVE в запросе MS.
    unsigned char ucAddressH; // Начальный адрес данных в запросе – старший байт.
    unsigned char ucAddressL; // Начальный адрес данных в запросе – младший байт.
    unsigned char ucIndicatorH; // индикатор – старший байт ($FF – ON, 00 – OFF).
    unsigned char ucIndicatorL; // индикатор – младший байт (всегда 0).

    unsigned char ucTust; // D0 – D2 - Код типа устройства, D3 – D7 – резерв.
    unsigned short usIndicatorSourceAddress; // Адрес источника формирования индикатора (битовая величина).
    unsigned char ucReserved1; // Резерв.
    unsigned char ucReserved2; // Резерв.
    unsigned char ucReserved3; // Резерв.
};

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
struct TConfigDataPackOne // данные для запроса типа устройства, функция modbus - 17.
{
    //ucModbusSlaveAddress
    // фактическое количество модулей.
    unsigned char ucModulesQuantity;
    unsigned char aucModuleType[INTERNAL_MODULE_QUANTITY]; // коды модулей ввода (вывода) по адресам SPI.
    unsigned char ucLastDiscreteInputModuleAddressPlusOne; // адрес последнего модуля дискретного ввода +1
    unsigned char ucLastAnalogueInputModuleAddressPlusOne; // адрес последнего модуля аналогового ввода +1
    unsigned char ucDiscreteOutputQuantity;
    uint16_t ui16DiscreteInputQuantity;
    unsigned char ucAnalogueInputQuantity;
    unsigned char ucDisplayType;
    unsigned char ucErrorCode;
};
#pragma pack(pop)

struct TConfigData // данные для запроса типа устройства, функция modbus - 17.
{
    // фактическое количество модулей.
    unsigned char ucModulesQuantity;
    unsigned char aucModuleType[INTERNAL_MODULE_QUANTITY]; // коды модулей ввода (вывода) по адресам SPI.
    unsigned char ucLastDiscreteInputModuleAddressPlusOne; // адрес последнего модуля дискретного ввода +1
    unsigned char ucLastAnalogueInputModuleAddressPlusOne; // адрес последнего модуля аналогового ввода +1
    unsigned char ucDiscreteOutputQuantity;
    uint16_t ui16DiscreteInputQuantity;
    unsigned char ucAnalogueInputQuantity;
    unsigned char ucDisplayType;
    unsigned char ucErrorCode;
};

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
struct TConfigDataModbusRepresentPackOne // данные для запроса типа устройства, функция modbus - 17.
{
    //ucModbusSlaveAddress
    // фактическое количество модулей.
    uint16_t ui16ModulesQuantity;
    uint16_t aui16ModuleType[INTERNAL_MODULE_QUANTITY]; // коды модулей ввода (вывода) по адресам SPI.
    uint16_t ui16LastDiscreteInputModuleAddressPlusOne; // адрес последнего модуля дискретного ввода +1
    uint16_t ui16LastAnalogueInputModuleAddressPlusOne; // адрес последнего модуля аналогового ввода +1
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

// структура для упорядочивания следования модулей при поиске на SPI.
struct TPlcConfigSearch
{
    uint8_t ui8ModulesQuantity;
    TConfigSearch axConfigSearch[INTERNAL_MODULE_QUANTITY];
};


struct TPlcConfigServiceData
{
    // служебные переменные. вычисляются при старте. хранятся на протяжении всего времени работы.
    // используются для вычисления адресов, смещений и т.д.
    // целостность данных проверяется перед каждым рабочим циклом 100mc.
    unsigned char ucInternalModulesQuantity;
    unsigned char ucExternalModulesQuantity;

    unsigned int uiHandledDiscreteInputQuantity;
    unsigned int uiHandledFunctionBlockQuantity;

    unsigned char ucLastDiscreteInputModuleIndex; // индексы модулей последних по порядку в своей группе.
    unsigned char ucLastAnalogueInputModuleIndex;
    unsigned char ucLastDiscreteOutputModuleIndex;
    unsigned char ucLastAnalogueOutputModuleIndex;

    unsigned char ucServiceDiscreteInputModuleQuantity; // количество модулей.
    unsigned char ucServiceAnalogueInputModuleQuantity;
    unsigned char ucServiceDiscreteOutputModuleQuantity;
    unsigned char ucServiceAnalogueOutputModuleQuantity;

    unsigned char ucServiceDiscreteInputQuantity; // количество входов-выходов.
    unsigned char ucServiceAnalogueInputQuantity;
    unsigned char ucServiceDiscreteOutputQuantity;
    unsigned char ucServiceAnalogueOutputQuantity;
};

struct TPlcConfigService
{
    TPlcConfigServiceData xPlcConfigServiceData;
    unsigned short usCrc;
};

struct TPlcDataBaseBlock // структура одного блока базы данных.
{
    unsigned char aucPlcDataBaseBlockData[PLC_DATA_BASE_BLOCK_LENGTH];
};

struct TPlcDataBase // структура всей базы данных.
{
    TPlcDataBaseBlock axPlcDataBaseBlocks[PLC_DATA_BASE_BLOCK_QUANTITY];
    unsigned short usCrc;
};

// при записи базы данных по Modbus - функция 69, за один раз передаётся один блок базы и
// его номер. чтобы узнать для какого модуля он предназначен и связать его с модулем,
// в функции vMainTaskMemoryMapFormation(void)
// для каждого блока заранее записываются usBlockLength, ucModuleType, ucModuleIndex модуля назначения.
// целостность данных проверяется перед каждым рабочим циклом 100mc.
struct TPlcDataBaseBlockControl // структура управления одним блоком базы данных.
{
    unsigned short usBlockLength; // размер в байтах блока базы данных, который привязывается к модулю.
    unsigned char ucModuleType; // тип модуля к которому привязывается блок базы данных.
    unsigned char ucModuleIndex; // индекс модуля в массиве контекста к которому привязывается блок базы данных.
};

struct TPlcDataBaseControl // структура управления всей базой данных.
{
    TPlcDataBaseBlockControl axPlcDataBaseBlockControl[PLC_DATA_BASE_BLOCK_QUANTITY];
    unsigned short usCrc;
};

#define AIN_OFF_PROCESS_OWNER_IS_NONE 0
#define AIN_OFF_PROCESS_OWNER_IS_PROGRAMMER 1
#define AIN_OFF_PROCESS_OWNER_IS_HMI 2
struct TMainFlagRegister
{
    // используется для синхронизации локальной сети
    // нижнего уровня с основным циклом.
    bool fbExternalModulesDataAccessIsAllowed;
//    // сигнализирует о том, что включен режим калибровки и
//    // запрещает выключать из обработки аналоговые входы с HMI панели.
//    bool fbCalibrationInProgress;
    // индекс процесса выводящего из обработки аналоговые входы.
    uint8_t ui8AinOffProcessOwnerIndex;
};

// устройства верхнего уровня (HMI панели, системы SCADA, программаторы), взаимодействуют с прибором
// по протоколу "Modbus". с помощью функций чтения-записи "Modbus", внешние устройства получают
// доступ к данным, расположенным в массивах "Modbus" - aucCoilsArray[], aucDiscreteInputsArray[],
// aucHoldingRegistersArray[], aucInputRegistersArray[].
//  в соответствии с задачей, каждому модулю, выделяется пространство в массивах "Modbus".
// в подпрограмме - vMainTaskMemoryMapFormation(void) создаётся контекст драйвера каждого модуля,
// который помещается в массив типа - struct TAllModulesContext. заполняются элементы
// структуры - struct TModuleContextStatic.
//  во время прохождения главного цикла 100mc, по очереди, для каждого модуля
// по его типу(элемент ucModuleType, структуры - struct TModuleContextStatic,
// массива типа - struct TAllModulesContext) - вызывается драйвер и передаётся указатель на
// его контекст.
// в контексте передаются указатели, команды, флаги и другие вспомогательные данные.

// структура контекста одного модуля не изменяемая в процессе.
// целостность данных проверяется перед каждым рабочим циклом 100mc.
struct TModuleContextStatic
{
    // индекс в массиве с объектами контекста всех модулей.
    unsigned char ucModuleContextIndex;
    // адрес модуля на SPI.
    unsigned char ucModuleAddress;
    // тип модуля.
    unsigned char ucModuleType;
    // индекс среди однотипных модулей -
    // дискретного ввода, аналогово ввода,
    // дискретного вывода, аналогово вывода и т.д.
    unsigned char ucInsideTypeIndex;
    // указатель на объект в памяти - передаваемая по интерфейсам связи конфигурация.
    unsigned char *pucPlcConfigDataTempPointer;

    // указатели на пространство в массивах "Modbus".
    // указатели на начало в рабочих массивах.
    // измеренные значения входных аналоговых сигналов.
    unsigned char *pucAiValueOriginBufferPointer;
    // расчетные значения CoDeSys.
    unsigned char *pucCalcValueOriginBufferPointer;
    // флаги CoDeSys.
    unsigned char *pucLogSostOriginBufferPointer;
    // состояние входных дискретных сигналов.
    unsigned char *pucDiValueOriginBufferPointer;
    // состояние выходов модулей реле МР №1 – MP №8.
    unsigned char *pucSostDoOriginBufferPointer;
    // массив флагов недостоверности аналоговых сигналов.
    unsigned char *pucBadAiOriginBufferPointer;
    // массив текущего времени.
    unsigned char *pucCurrentTimeBufferPointer;
    // массив журнала событий "кратко".
    unsigned char *pucAlarmEventsLog;
    // массив журнала архивных событий.
    unsigned char *pucArchiveEventsLog;
    // массив журнала ошибок.
    unsigned char *pucErrorLog;
    // массив с нормализованной базой данных обработки аналоговых сигналов.
    unsigned char *pucAnalogueInputDescriptionWork;
//    // массив с нормализованной базой данных обработки аналоговых выходов.
//    unsigned char *pucAnalogueOutputRegulatorDescriptionDataBase;
    // массив с нормализованной базой данных размерностей.
    unsigned char *pucDimentionsParametersDataBase;

    // указатели на пространство в массивах "Modbus".
    // указатели на пространство в рабочих массивах,
    // заранее определённое для каждого модуля.
    // измеренные значения входных аналоговых сигналов.
    unsigned char *pucAiValueBufferPointer;
    // Аналоговые сигналы регуляторов.
    unsigned char *pucContAvBufferPointer;

    // состояние входных дискретных сигналов.
    unsigned char *pucDiValueBufferPointer;
    // состояние выходов модулей реле МР №1 – MP №8.
    unsigned char *pucSostDoBufferPointer;
    // массив флагов недостоверности аналоговых сигналов.
    unsigned char *pucBadAiBufferPointer;
    // массив флагов недостоверности дискретных сигналов.
    unsigned char *pucBadDiBufferPointer;
    // управление дискретными выходами - новое нарушение.
    unsigned char *pucDoValueBufferPointer;
    // управление дискретными выходами - требование включения реле.
    unsigned char *pucDoValueBlockBufferPointer;
    // массив дискретных сигналов регуляторов для чтения.
    unsigned char *pucContStBufferPointer;
    // массив дискретных сигналов регуляторов для записи.
    unsigned char *pucContStWriteBufferPointer;
    // состояние аналоговых измерительных каналов.
    unsigned char *pucStatAiBufferPointer;
    // массив флагов отказов модулей.
    unsigned char *pucModuleBadStateBufferPointer;
    // массив флагов исключения аналоговых сигналов из обработки.
    unsigned char *pucAinOffBufferPointer;
    // массив флагов недостоверности дискретных выходов модулей реле.
    unsigned char *pucBadDoBufferPointer;
    // массив флагов ошибок дискретных выходов модулей реле.
    unsigned char *pucStatDoBufferPointer;
    // состояние аналоговых выходов.
    unsigned char *pucBadAoBufferPointer;
    // буфер данных температуры холодного спая ТХС, модуля МВСТ3.
    unsigned char *pucTxsBufferPointer;
    // буфер данных реперных точек и кода АЦП, модуля аналоговоого ввода.
    unsigned char *pucReperPointsAdcBufferPointer;

    unsigned char *pucDataBasePointer; // указатель на блок в базе данных.
    unsigned char ucDataBaseDataOffset; // смещение на блок модуля, в блоке базе данных.
    unsigned char *pucDataBaseExtractPointer; // указатель на блок в преобразованной базе данных.
};

// структура контекста одного модуля изменяемая в процессе.
struct TModuleContextDinamic
{
    // общий байт для передачи индека. используется для передачи номера калибруемого входа,
    // номера регулятора и др.
    unsigned char ucCommonIndex;
    // счётчик запросов модуля без ответа.
    // если равно нулю - модуль считается неисправным. его данные признаются недостоверными.
    unsigned char nucBadAnswerCounter;
    // код ошибки.
    unsigned char ucErrorCode;
    // счётчик - делитель, для изменения времени доступа к MTVI5.
    unsigned char nucLoopSkipCounter;
    unsigned char ucDataFormat;
    unsigned char ucDataByteOrder;
    unsigned char ucCommandControl;
    unsigned char ucFsmState; // состояние автомата.
    unsigned short usAuxiliaryCounter;
    unsigned short usAuxiliaryData;

    bool fbGlobalModbusKvit; // флаг - квитирование с верхнего уровня по Modbus.
    bool fbGlobalModbusReset; // флаг - сброс с верхнего уровня по Modbus.
    bool fbGlobalModbusBlock; // флаг - режим блокировки с верхнего уровня по Modbus.
    bool fbGlobalButtonKvit; // флаг - квитирование с центральной панели МИНД.
    bool fbGlobalButtonReset; // флаг - сброс с центральной панели МИНД.
    bool fbGlobalButtonBlock; // флаг - режим блокировки с центральной панели МИНД.
};

struct TModuleContext // структура контекста одного модуля.
{
    // объект с изменяемыми данными.
    TModuleContextDinamic xModuleContextDinamic;
    // объект с неизменяемыми данными.
    TModuleContextStatic xModuleContextStatic;
};

struct TAllModulesContext // структура контекста всех модулей.
{
    // массив с объектами контекста всех модулей.
    TModuleContext axAllModulesContext[MAX_MODULE_QUANTITY];
    unsigned short usCrc;
};

// маска - биты 0 – 3 тип шкалы датчика ТШК.
#define ANALOGUE_INPUT_SCALE_TYPE_MASK 0x0F
// маска - биты 4 – 7 – код размерности измеряемого параметра КРЗМ.
#define ANALOGUE_INPUT_DIMENTION_CODE_MASK 0xF0
// маска - биты 0 – 5 – код диапазона измеряемой температуры для МВСТ5, или код входного сигнала для МВПС5 и МВАО5 (для МВАИ5 значения не имеет) КДИТ.
#define ANALOGUE_INPUT_RANGE_CODE_MASK 0x3F
// структура описателя одного аналогового входа в базе данных.
struct TAnalogueInputDescriptionDataBase
{
    // - биты 0 – 3 тип шкалы датчика ТШК.
    // - биты 4 – 7 – код размерности измеряемого параметра КРЗМ.
    unsigned char ucServiseL;
    // - биты 0 – 5 – код диапазона измеряемой температуры для МВСТ5, или код входного сигнала для МВПС5 и МВАО5 (для МВАИ5 значения не имеет) КДИТ.
    // - биты 6,7 – резерв.
    unsigned char ucServiseH;
    // НШК - нижний предел шкалы датчика, Формат – FLOAT, 3 байта.
    unsigned char ucMinOfScaleMantissLsb;
    unsigned char ucMinOfScaleMantissMsb;
    unsigned char ucMinOfScaleExponent;
    // ВШК - верхний предел шкалы датчика, Формат – FLOAT, 3 байта.
    unsigned char ucMaxOfScaleMantissLsb;
    unsigned char ucMaxOfScaleMantissMsb;
    unsigned char ucMaxOfScaleExponent;
    // уставка LL, Формат – FLOAT, 3 байта.
    unsigned char ucSetPointLLMantissLsb;
    unsigned char ucSetPointLLMantissMsb;
    unsigned char ucSetPointLLExponent;
    // уставка L, Формат – FLOAT, 3 байта.
    unsigned char ucSetPointLMantissLsb;
    unsigned char ucSetPointLMantissMsb;
    unsigned char ucSetPointLExponent;
    // уставка H, Формат – FLOAT, 3 байта.
    unsigned char ucSetPointHMantissLsb;
    unsigned char ucSetPointHMantissMsb;
    unsigned char ucSetPointHExponent;
    // уставка HH, Формат – FLOAT, 3 байта.
    unsigned char ucSetPointHHMantissLsb;
    unsigned char ucSetPointHHMantissMsb;
    unsigned char ucSetPointHHExponent;
    // Текстовый реквизит входа – 8 символов ASCII.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH];
};

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура описателя одного аналогового входа в базе данных.
struct TAnalogueInputDescriptionDataBasePackOne
{
    // - биты 0 – 3 тип шкалы датчика ТШК.
    // - биты 4 – 7 – код размерности измеряемого параметра КРЗМ.
    unsigned char ucServiseL;
    // - биты 0 – 5 – код диапазона измеряемой температуры для МВСТ5, или код входного сигнала для МВПС5 и МВАО5 (для МВАИ5 значения не имеет) КДИТ.
    // - биты 6,7 – резерв.
    unsigned char ucServiseH;
    // НШК - нижний предел шкалы датчика, Формат – FLOAT, 3 байта.
    unsigned char ucMinOfScaleMantissLsb;
    unsigned char ucMinOfScaleMantissMsb;
    unsigned char ucMinOfScaleExponent;
    // ВШК - верхний предел шкалы датчика, Формат – FLOAT, 3 байта.
    unsigned char ucMaxOfScaleMantissLsb;
    unsigned char ucMaxOfScaleMantissMsb;
    unsigned char ucMaxOfScaleExponent;
    // уставка LL, Формат – FLOAT, 3 байта.
    unsigned char ucSetPointLLMantissLsb;
    unsigned char ucSetPointLLMantissMsb;
    unsigned char ucSetPointLLExponent;
    // уставка L, Формат – FLOAT, 3 байта.
    unsigned char ucSetPointLMantissLsb;
    unsigned char ucSetPointLMantissMsb;
    unsigned char ucSetPointLExponent;
    // уставка H, Формат – FLOAT, 3 байта.
    unsigned char ucSetPointHMantissLsb;
    unsigned char ucSetPointHMantissMsb;
    unsigned char ucSetPointHExponent;
    // уставка HH, Формат – FLOAT, 3 байта.
    unsigned char ucSetPointHHMantissLsb;
    unsigned char ucSetPointHHMantissMsb;
    unsigned char ucSetPointHHExponent;
    // Текстовый реквизит входа – 8 символов ASCII.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH];
};
#pragma pack(pop)

// структура описателя одного аналогового входа в буфере RAM, IEEE754.
// после преобразования из общего формата базы данных.
struct TAnalogueInputDescriptionModbus
{
    // НШК - нижний предел шкалы датчика, Формат – FLOAT, IEEE754.
    float fMinOfScale;
    // ВШК - верхний предел шкалы датчика, Формат – FLOAT, IEEE754.
    float fMaxOfScale;
    // уставка LL, Формат – FLOAT, IEEE754.
    float fSetPointLL;
    // уставка L, Формат – FLOAT, IEEE754.
    float fSetPointL;
    // уставка H, Формат – FLOAT, IEEE754.
    float fSetPointH;
    // уставка HH, Формат – FLOAT, IEEE754.
    float fSetPointHH;
};

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура описателя одного аналогового входа в буфере RAM, IEEE754.
// после преобразования из общего формата базы данных.
struct TAnalogueInputDescriptionModbusPackOne
{
    // НШК - нижний предел шкалы датчика, Формат – FLOAT, IEEE754.
    float fMinOfScale;
    // ВШК - верхний предел шкалы датчика, Формат – FLOAT, IEEE754.
    float fMaxOfScale;
    // уставка LL, Формат – FLOAT, IEEE754.
    float fSetPointLL;
    // уставка L, Формат – FLOAT, IEEE754.
    float fSetPointL;
    // уставка H, Формат – FLOAT, IEEE754.
    float fSetPointH;
    // уставка HH, Формат – FLOAT, IEEE754.
    float fSetPointHH;
};
#pragma pack(pop)

//// структура описателя одного аналогового входа в буфере RAM, IEEE754.
//// после преобразования из общего формата базы данных.
//// для сериализации, разбора данных полученных по интерфейсам передачи данных.
//#pragma pack(push)
//#pragma pack(1)
//struct TAnalogueInputDescriptionWorkPackOne
//{
//    // тип шкалы датчика ТШК.
//    unsigned char ucScaleType;
//    // код размерности измеряемого параметра КРЗМ.
//    unsigned char ucDimentionCode;
//    // код диапазона измеряемой температуры для МВСТ5, или код входного сигнала для МВПС5 и МВАО5 (для МВАИ5 значения не имеет) КДИТ.
//    unsigned char ucRangeCode;
////    // НШК - нижний предел шкалы датчика, Формат – FLOAT, IEEE754.
////    float fMinOfScale;
////    // ВШК - верхний предел шкалы датчика, Формат – FLOAT, IEEE754.
////    float fMaxOfScale;
////    // уставка LL, Формат – FLOAT, IEEE754.
////    float fSetPointLL;
////    // уставка L, Формат – FLOAT, IEEE754.
////    float fSetPointL;
////    // уставка H, Формат – FLOAT, IEEE754.
////    float fSetPointH;
////    // уставка HH, Формат – FLOAT, IEEE754.
////    float fSetPointHH;
//    // Текстовый реквизит входа – 8 символов ASCII, плюс нуль - признак конца строки.
//    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
//};
//#pragma pack(pop)

// структура описателя одного аналогового входа в буфере RAM, IEEE754.
// после преобразования из общего формата базы данных.
struct TAnalogueInputDescriptionWork
{
    // тип шкалы датчика ТШК.
    unsigned char ucScaleType;
    // код размерности измеряемого параметра КРЗМ.
    unsigned char ucDimentionCode;
    // код диапазона измеряемой температуры для МВСТ5, или код входного сигнала для МВПС5 и МВАО5 (для МВАИ5 значения не имеет) КДИТ.
    unsigned char ucRangeCode;
    // НШК - нижний предел шкалы датчика, Формат – FLOAT, IEEE754.
    float fMinOfScale;
    // ВШК - верхний предел шкалы датчика, Формат – FLOAT, IEEE754.
    float fMaxOfScale;
    // уставка LL, Формат – FLOAT, IEEE754.
    float fSetPointLL;
    // уставка L, Формат – FLOAT, IEEE754.
    float fSetPointL;
    // уставка H, Формат – FLOAT, IEEE754.
    float fSetPointH;
    // уставка HH, Формат – FLOAT, IEEE754.
    float fSetPointHH;
    // Текстовый реквизит входа – 8 символов ASCII, плюс нуль - признак конца строки.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
};

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура описателя одного аналогового входа в буфере RAM, IEEE754.
// после преобразования из общего формата базы данных.
struct TAnalogueInputDescriptionWorkPackOne
{
    // тип шкалы датчика ТШК.
    unsigned char ucScaleType;
    // код размерности измеряемого параметра КРЗМ.
    unsigned char ucDimentionCode;
    // код диапазона измеряемой температуры для МВСТ5, или код входного сигнала для МВПС5 и МВАО5 (для МВАИ5 значения не имеет) КДИТ.
    unsigned char ucRangeCode;
    // НШК - нижний предел шкалы датчика, Формат – FLOAT, IEEE754.
    float fMinOfScale;
    // ВШК - верхний предел шкалы датчика, Формат – FLOAT, IEEE754.
    float fMaxOfScale;
    // уставка LL, Формат – FLOAT, IEEE754.
    float fSetPointLL;
    // уставка L, Формат – FLOAT, IEEE754.
    float fSetPointL;
    // уставка H, Формат – FLOAT, IEEE754.
    float fSetPointH;
    // уставка HH, Формат – FLOAT, IEEE754.
    float fSetPointHH;
    // Текстовый реквизит входа – 8 символов ASCII, плюс нуль - признак конца строки.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
};
#pragma pack(pop)

//// для сериализации, разбора данных полученных по интерфейсам передачи данных.
//#pragma pack(push)
//#pragma pack(1)
//// структура данных аналоговых измерений.
//struct TAnalogueMeasurePackOne
//{
//    // адрес первого запрашиваемого аналогового измерения.
//    // адрес сигнала (№ входа, адрес в
//    // (измеренные значения входных аналоговых сигналов - AI_VALUE_BYTE_ARRAY_OFFSET)
//    uint8_t ui8Address;
//    // состояние канала аналогового измерения.
//    uint8_t ui8Status;
//    // значение аналогового измерения.
//    float fData;
//    // Текстовый реквизит входа – 8 символов ASCII, плюс нуль - признак конца строки.
//    char acTextDescriptor[CMind::ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
//    // Текстовый реквизит размерности измеряемого параметра КРЗМ.
//    char aucDimentionCode[CMind::DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
//};
//#pragma pack(pop)
//
//// структура данных аналоговых измерений.
//struct TAnalogueMeasure
//{
//    // адрес первого запрашиваемого аналогового измерения.
//    // адрес сигнала (№ входа, адрес в
//    // (измеренные значения входных аналоговых сигналов - AI_VALUE_BYTE_ARRAY_OFFSET)
//    uint8_t ui8Address;
//    // состояние канала аналогового измерения.
//    uint8_t ui8Status;
//    // значение аналогового измерения.
//    float fData;
//    // Текстовый реквизит входа – 8 символов ASCII, плюс нуль - признак конца строки.
//    char acTextDescriptor[CMind::ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
//    // Текстовый реквизит размерности измеряемого параметра КРЗМ.
//    char aucDimentionCode[CMind::DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
//};

//// структура описателя одного аналогового входа в буфере RAM, целочисленный код 16 бит.
//// после преобразования из общего формата базы данных.
//struct TAnalogueInputShortDescriptionData
//{
//    // уставка LL, Формат – SHORT, целочисленный код 16 бит.
//    unsigned short usSetPointLL;
//    // уставка L, Формат – SHORT, целочисленный код 16 бит.
//    unsigned short usSetPointL;
//    // уставка L, Формат – SHORT, целочисленный код 16 бит.
//    unsigned short usSetPointH;
//    // уставка HH, Формат – SHORT, целочисленный код 16 бит.
//    unsigned short usSetPointHH;
//};

// структура описателя одной размерности.
// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура описателя одной размерности.
// после преобразования из общего формата базы данных.
struct TDimentionParameterPackOne
{
    // Текстовый реквизит размерности – 6 символов ASCII.
    char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH];
};
#pragma pack(pop)

// структура описателя одной размерности.
struct TDimentionParameter
{
    // Текстовый реквизит размерности – 6 символов ASCII, плюс нуль - признак конца строки.
    char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
};

// структура для хранения и передачи температуры холодного спая ТХС.
struct TTxsData
{
    float fTxsValue; // значение ТХС.
    unsigned char ucTxsStat; // состояние канала ТХС.
};

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура для хранения и передачи температуры холодного спая ТХС.
struct TTxsDataPackOne
{
    float fTxsValue; // значение ТХС.
    unsigned char ucTxsStat; // состояние канала ТХС.
};

#pragma pack(pop)

// структура описателя одного регулятора модуля аналогового вывода в базе данных.
struct TAnalogueOutputRegulatorDescriptionDataBase
{
    // Адрес регулируемого параметра (PV=1-96), 0 – пустой описатель.
    unsigned char ucPvAddress;
    // Тип регулятора (TYPE = 0, 1, 8 – 13):
    // TYPE 0 – повторитель сигнала:
    // - вход «PV» копируется на выход «OUT»;
    // - выход «OUT» поступает на ЦАП.
    // TYPE 1 – одноконтурный регулятор (байт 2 = 0-5) или регулятор внешнего контура каскадной схемы (байт 2 = 8-13):
    // - задание берется от входа «SP», на который поступает с ВУ;
    // - выход «OUT» поступает на ЦАП (байт 2 = 0-5) или на вход SP регулятора внутреннего контура (байт 2 = 8-13).
    // TYPE 8 – 13 – регулятор внутреннего контура каскадной схемы:
    // - в режиме «А» на вход «SP» поступает сигнал с ВУ;
    // - в режиме «АП» на вход «SP» поступает выход «OUT» регулятора внешнего контура: 8 – регулятор №1, 13 – регулятор №6;
    // - выход «OUT» поступает на ЦАП.
    unsigned char ucRegulatorType;
    // Адрес назначения выходного сигнала «OUT»:
    // 0 – 5 – адрес аналогового выхода МТВИ (ЦАП) для:
    // - TYPE 0 – повторитель сигнала;
    //  - TYPE 1 – одноконтурный регулятор;
    //  - TYPE 8 – 13 – внутренний контур каскадной схемы);
    // 8 – 13 – назначение выхода «OUT» для TYPE 1 – регулятор внешнего контура каскадной схемы
    // - адрес регулятора внутреннего контура, для которого «OUT» является заданием в каскадной схеме
    // (8 – регулятор №1, 13 – регулятор №6).
    unsigned char ucOutAddress;
    // D0 – условие формирования выхода: 0 – прямой, 1 - обратный (остальные биты – резерв).
    unsigned char ucOutInverse;
    // D0 - источник сигнала PV (регулируемого параметра для TYPE 1 или входа повторителя сигнала для TYPE 0) - адрес в байте 0 описателя:
    // D0 = 0 – аналоговый входной сигнал AIn (в код 0-16383 масшт. по шкале);
    // D0 = 1 – расчетная величина Сalc_cds (с выхода CoDeSys передается в
    // 0-100%, масштабируется в код 0-16383);
    // D1 – D7 – резерв.
    unsigned char ucControl;
    // Адрес источника программного задания, поступающего на вход SP – расчетная величина Сalc_cds (с выхода CoDeSys передается в единицах шкалы PV, масштабируется в код 0-16383 перед подачей на вход SP);
    // 0 – нет программного задания
    unsigned char ucProgrammSPAddress;
    // Настройки – Kp - float, 3 байта.
    unsigned char ucKpMantissLsb;
    unsigned char ucKpMantissMsb;
    unsigned char ucKpExponent;
    // Настройки – Ti - float, 3 байта.
    unsigned char ucTiMantissLsb;
    unsigned char ucTiMantissMsb;
    unsigned char ucTiExponent;
    // Настройки – Td - float, 3 байта.
    unsigned char ucTdMantissLsb;
    unsigned char ucTdMantissMsb;
    unsigned char ucTdExponent;
    // Ограничение выхода снизу (OL 0-100%) – 1 число float, 3 байта.
    unsigned char ucOutRestrictionLowMantissLsb;
    unsigned char ucOutRestrictionLowMantissMsb;
    unsigned char ucOutRestrictionLowExponent;
    // Ограничение выхода сверху (OН 0-100%) - 1 число float, 3 байта.
    unsigned char ucOutRestrictionHighMantissLsb;
    unsigned char ucOutRestrictionHighMantissMsb;
    unsigned char ucOutRestrictionHighExponent;
    // Предпочтительное значение выхода OUT при включении (OSF 0-100%) - 1 число float, 3 байта.
    unsigned char ucPreferredOutputValueMantissLsb;
    unsigned char ucPreferredOutputValueMantissMsb;
    unsigned char ucPreferredOutputValueExponent;
    // Адрес выхода LogSost [fc] CoDeSys (1-240)
    // – сигнализация включения режима форсированного управления – fc,
    // 0 – режим fc не предусмотрен.
    unsigned char ucForceControlFcOnAddress;
    // Адрес выхода LogSost [ha] CoDeSys (1-240) для управления режимом Р/А в условиях форсированного управления - fc
    unsigned char ucForceControlHaOnAddress;
    // Адрес расчетной величины Сalc_cds [spout] CoDeSys (1-48) для управления SP/OUT в условиях форсированного управления - fc
    unsigned char ucForceControlSpOutAddress;
    // Резерв.
    unsigned char ucReserved;
};

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура описателя одного регулятора модуля аналогового вывода в базе данных.
struct TAnalogueOutputRegulatorDescriptionDataBasePackOne
{
    // Адрес регулируемого параметра (PV=1-96), 0 – пустой описатель.
    unsigned char ucPvAddress;
    // Тип регулятора (TYPE = 0, 1, 8 – 13):
    // TYPE 0 – повторитель сигнала:
    // - вход «PV» копируется на выход «OUT»;
    // - выход «OUT» поступает на ЦАП.
    // TYPE 1 – одноконтурный регулятор (байт 2 = 0-5) или регулятор внешнего контура каскадной схемы (байт 2 = 8-13):
    // - задание берется от входа «SP», на который поступает с ВУ;
    // - выход «OUT» поступает на ЦАП (байт 2 = 0-5) или на вход SP регулятора внутреннего контура (байт 2 = 8-13).
    // TYPE 8 – 13 – регулятор внутреннего контура каскадной схемы:
    // - в режиме «А» на вход «SP» поступает сигнал с ВУ;
    // - в режиме «АП» на вход «SP» поступает выход «OUT» регулятора внешнего контура: 8 – регулятор №1, 13 – регулятор №6;
    // - выход «OUT» поступает на ЦАП.
    unsigned char ucRegulatorType;
    // Адрес назначения выходного сигнала «OUT»:
    // 0 – 5 – адрес аналогового выхода МТВИ (ЦАП) для:
    // - TYPE 0 – повторитель сигнала;
    //  - TYPE 1 – одноконтурный регулятор;
    //  - TYPE 8 – 13 – внутренний контур каскадной схемы);
    // 8 – 13 – назначение выхода «OUT» для TYPE 1 – регулятор внешнего контура каскадной схемы
    // - адрес регулятора внутреннего контура, для которого «OUT» является заданием в каскадной схеме
    // (8 – регулятор №1, 13 – регулятор №6).
    unsigned char ucOutAddress;
    // D0 – условие формирования выхода: 0 – прямой, 1 - обратный (остальные биты – резерв).
    unsigned char ucOutInverse;
    // D0 - источник сигнала PV (регулируемого параметра для TYPE 1 или входа повторителя сигнала для TYPE 0) - адрес в байте 0 описателя:
    // D0 = 0 – аналоговый входной сигнал AIn (в код 0-16383 масшт. по шкале);
    // D0 = 1 – расчетная величина Сalc_cds (с выхода CoDeSys передается в
    // 0-100%, масштабируется в код 0-16383);
    // D1 – D7 – резерв.
    unsigned char ucControl;
    // Адрес источника программного задания, поступающего на вход SP – расчетная величина Сalc_cds (с выхода CoDeSys передается в единицах шкалы PV, масштабируется в код 0-16383 перед подачей на вход SP);
    // 0 – нет программного задания
    unsigned char ucProgrammSPAddress;
    // Настройки – Kp - float, 3 байта.
    unsigned char ucKpMantissLsb;
    unsigned char ucKpMantissMsb;
    unsigned char ucKpExponent;
    // Настройки – Ti - float, 3 байта.
    unsigned char ucTiMantissLsb;
    unsigned char ucTiMantissMsb;
    unsigned char ucTiExponent;
    // Настройки – Td - float, 3 байта.
    unsigned char ucTdMantissLsb;
    unsigned char ucTdMantissMsb;
    unsigned char ucTdExponent;
    // Ограничение выхода снизу (OL 0-100%) – 1 число float, 3 байта.
    unsigned char ucOutRestrictionLowMantissLsb;
    unsigned char ucOutRestrictionLowMantissMsb;
    unsigned char ucOutRestrictionLowExponent;
    // Ограничение выхода сверху (OН 0-100%) - 1 число float, 3 байта.
    unsigned char ucOutRestrictionHighMantissLsb;
    unsigned char ucOutRestrictionHighMantissMsb;
    unsigned char ucOutRestrictionHighExponent;
    // Предпочтительное значение выхода OUT при включении (OSF 0-100%) - 1 число float, 3 байта.
    unsigned char ucPreferredOutputValueMantissLsb;
    unsigned char ucPreferredOutputValueMantissMsb;
    unsigned char ucPreferredOutputValueExponent;
    // Адрес выхода LogSost [fc] CoDeSys (1-240)
    // – сигнализация включения режима форсированного управления – fc,
    // 0 – режим fc не предусмотрен.
    unsigned char ucForceControlFcOnAddress;
    // Адрес выхода LogSost [ha] CoDeSys (1-240) для управления режимом Р/А в условиях форсированного управления - fc
    unsigned char ucForceControlHaOnAddress;
    // Адрес расчетной величины Сalc_cds [spout] CoDeSys (1-48) для управления SP/OUT в условиях форсированного управления - fc
    unsigned char ucForceControlSpOutAddress;
    // Резерв.
    unsigned char ucReserved;
};
#pragma pack(pop)

// структура описателя одного аналогового вывода в буфере RAM, IEEE754.
// после преобразования из общего формата базы данных.
struct TAnalogueOutputRegulatorDescriptionModbus
{
    // Настройки – Kp - float,  IEEE754.
    float fKp;
    // Настройки – Ti - float,  IEEE754.
    float fTi;
    // Настройки – Td - float,  IEEE754.
    float fTd;
    // Ограничение выхода снизу (OL 0-100%) – 1 число float, IEEE754.
    float fOutRestrictionLow;
    // Ограничение выхода сверху (OН 0-100%) - 1 число float, IEEE754.
    float fOutRestrictionHigh;
    // Предпочтительное значение выхода OUT при включении (OSF 0-100%) float, IEEE754.
    float fPreferredOutputValue;
};

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура описателя одного аналогового вывода в буфере RAM, IEEE754.
// после преобразования из общего формата базы данных.
struct TAnalogueOutputRegulatorDescriptionModbusPackOne
{
    // Настройки – Kp - float,  IEEE754.
    float fKp;
    // Настройки – Ti - float,  IEEE754.
    float fTi;
    // Настройки – Td - float,  IEEE754.
    float fTd;
    // Ограничение выхода снизу (OL 0-100%) – 1 число float, IEEE754.
    float fOutRestrictionLow;
    // Ограничение выхода сверху (OН 0-100%) - 1 число float, IEEE754.
    float fOutRestrictionHigh;
    // Предпочтительное значение выхода OUT при включении (OSF 0-100%) float, IEEE754.
    float fPreferredOutputValue;
};
#pragma pack(pop)

// структура описателя одного аналогового вывода в буфере RAM, IEEE754.
// после преобразования из общего формата базы данных.
struct TAnalogueOutputRegulatorDescriptionWork
{
    // Адрес регулируемого параметра (PV=1-96), 0 – пустой описатель.
    unsigned char ucPvAddress;
    // Тип регулятора (TYPE = 0, 1, 8 – 13):
    // TYPE 0 – повторитель сигнала:
    // - вход «PV» копируется на выход «OUT»;
    // - выход «OUT» поступает на ЦАП.
    // TYPE 1 – одноконтурный регулятор (байт 2 = 0-5) или регулятор внешнего контура каскадной схемы (байт 2 = 8-13):
    // - задание берется от входа «SP», на который поступает с ВУ;
    // - выход «OUT» поступает на ЦАП (байт 2 = 0-5) или на вход SP регулятора внутреннего контура (байт 2 = 8-13).
    // TYPE 8 – 13 – регулятор внутреннего контура каскадной схемы:
    // - в режиме «А» на вход «SP» поступает сигнал с ВУ;
    // - в режиме «АП» на вход «SP» поступает выход «OUT» регулятора внешнего контура: 8 – регулятор №1, 13 – регулятор №6;
    // - выход «OUT» поступает на ЦАП.
    unsigned char ucRegulatorType;
    // Адрес назначения выходного сигнала «OUT»:
    // 0 – 5 – адрес аналогового выхода МТВИ (ЦАП) для:
    // - TYPE 0 – повторитель сигнала;
    //  - TYPE 1 – одноконтурный регулятор;
    //  - TYPE 8 – 13 – внутренний контур каскадной схемы);
    // 8 – 13 – назначение выхода «OUT» для TYPE 1 – регулятор внешнего контура каскадной схемы
    // - адрес регулятора внутреннего контура, для которого «OUT» является заданием в каскадной схеме
    // (8 – регулятор №1, 13 – регулятор №6).
    unsigned char ucOutAddress;
    // D0 – условие формирования выхода: 0 – прямой, 1 - обратный (остальные биты – резерв).
    unsigned char ucOutInverse;
    // D0 - источник сигнала PV (регулируемого параметра для TYPE 1 или входа повторителя сигнала для TYPE 0) - адрес в байте 0 описателя:
    // D0 = 0 – аналоговый входной сигнал AIn (в код 0-16383 масшт. по шкале);
    // D0 = 1 – расчетная величина Сalc_cds (с выхода CoDeSys передается в
    // 0-100%, масштабируется в код 0-16383);
    // D1 – D7 – резерв.
    unsigned char ucControl;
    // Адрес источника программного задания, поступающего на вход SP – расчетная величина Сalc_cds (с выхода CoDeSys передается в единицах шкалы PV, масштабируется в код 0-16383 перед подачей на вход SP);
    // 0 – нет программного задания
    unsigned char ucProgrammSPAddress;
    // Настройки – Kp - float,  IEEE754.
    float fKp;
    // Настройки – Ti - float,  IEEE754.
    float fTi;
    // Настройки – Td - float,  IEEE754.
    float fTd;
    // Ограничение выхода снизу (OL 0-100%) – 1 число float, IEEE754.
    float fOutRestrictionLow;
    // Ограничение выхода сверху (OН 0-100%) - 1 число float, IEEE754.
    float fOutRestrictionHigh;
    // Предпочтительное значение выхода OUT при включении (OSF 0-100%) float, IEEE754.
    float fPreferredOutputValue;
    // Адрес выхода LogSost [fc] CoDeSys (1-240)
    // – сигнализация включения режима форсированного управления – fc,
    // 0 – режим fc не предусмотрен.
    unsigned char ucForceControlFcOnAddress;
    // Адрес выхода LogSost [ha] CoDeSys (1-240) для управления режимом Р/А в условиях форсированного управления - fc
    unsigned char ucForceControlHaOnAddress;
    // Адрес расчетной величины Сalc_cds [spout] CoDeSys (1-48) для управления SP/OUT в условиях форсированного управления - fc
    unsigned char ucForceControlSpOutAddress;
};

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура описателя одного аналогового вывода в буфере RAM, IEEE754.
// после преобразования из общего формата базы данных.
struct TAnalogueOutputRegulatorDescriptionWorkPackOne
{
    // Адрес регулируемого параметра (PV=1-96), 0 – пустой описатель.
    unsigned char ucPvAddress;
    // Тип регулятора (TYPE = 0, 1, 8 – 13):
    // TYPE 0 – повторитель сигнала:
    // - вход «PV» копируется на выход «OUT»;
    // - выход «OUT» поступает на ЦАП.
    // TYPE 1 – одноконтурный регулятор (байт 2 = 0-5) или регулятор внешнего контура каскадной схемы (байт 2 = 8-13):
    // - задание берется от входа «SP», на который поступает с ВУ;
    // - выход «OUT» поступает на ЦАП (байт 2 = 0-5) или на вход SP регулятора внутреннего контура (байт 2 = 8-13).
    // TYPE 8 – 13 – регулятор внутреннего контура каскадной схемы:
    // - в режиме «А» на вход «SP» поступает сигнал с ВУ;
    // - в режиме «АП» на вход «SP» поступает выход «OUT» регулятора внешнего контура: 8 – регулятор №1, 13 – регулятор №6;
    // - выход «OUT» поступает на ЦАП.
    unsigned char ucRegulatorType;
    // Адрес назначения выходного сигнала «OUT»:
    // 0 – 5 – адрес аналогового выхода МТВИ (ЦАП) для:
    // - TYPE 0 – повторитель сигнала;
    //  - TYPE 1 – одноконтурный регулятор;
    //  - TYPE 8 – 13 – внутренний контур каскадной схемы);
    // 8 – 13 – назначение выхода «OUT» для TYPE 1 – регулятор внешнего контура каскадной схемы
    // - адрес регулятора внутреннего контура, для которого «OUT» является заданием в каскадной схеме
    // (8 – регулятор №1, 13 – регулятор №6).
    unsigned char ucOutAddress;
    // D0 – условие формирования выхода: 0 – прямой, 1 - обратный (остальные биты – резерв).
    unsigned char ucOutInverse;
    // D0 - источник сигнала PV (регулируемого параметра для TYPE 1 или входа повторителя сигнала для TYPE 0) - адрес в байте 0 описателя:
    // D0 = 0 – аналоговый входной сигнал AIn (в код 0-16383 масшт. по шкале);
    // D0 = 1 – расчетная величина Сalc_cds (с выхода CoDeSys передается в
    // 0-100%, масштабируется в код 0-16383);
    // D1 – D7 – резерв.
    unsigned char ucControl;
    // Адрес источника программного задания, поступающего на вход SP – расчетная величина Сalc_cds (с выхода CoDeSys передается в единицах шкалы PV, масштабируется в код 0-16383 перед подачей на вход SP);
    // 0 – нет программного задания
    unsigned char ucProgrammSPAddress;
    // Настройки – Kp - float,  IEEE754.
    float fKp;
    // Настройки – Ti - float,  IEEE754.
    float fTi;
    // Настройки – Td - float,  IEEE754.
    float fTd;
    // Ограничение выхода снизу (OL 0-100%) – 1 число float, IEEE754.
    float fOutRestrictionLow;
    // Ограничение выхода сверху (OН 0-100%) - 1 число float, IEEE754.
    float fOutRestrictionHigh;
    // Предпочтительное значение выхода OUT при включении (OSF 0-100%) float, IEEE754.
    float fPreferredOutputValue;
    // Адрес выхода LogSost [fc] CoDeSys (1-240)
    // – сигнализация включения режима форсированного управления – fc,
    // 0 – режим fc не предусмотрен.
    unsigned char ucForceControlFcOnAddress;
    // Адрес выхода LogSost [ha] CoDeSys (1-240) для управления режимом Р/А в условиях форсированного управления - fc
    unsigned char ucForceControlHaOnAddress;
    // Адрес расчетной величины Сalc_cds [spout] CoDeSys (1-48) для управления SP/OUT в условиях форсированного управления - fc
    unsigned char ucForceControlSpOutAddress;
};
#pragma pack(pop)

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура данных аналоговых переменных регуляторов «CONT_AV»
struct TMtvi5ContAvDataPackOne
{
    float fSp;
    float fOut;
    float fPv;
};
#pragma pack(pop)

// структура данных аналоговых переменных регуляторов «CONT_AV»
struct TMtvi5ContAvData
{
    float fSp;
    float fOut;
    float fPv;
};

// структура данных, для взаимодействия с "CodeSys", через разделяемую память.
// от Linux к CodeSys.
struct TDataExchangeLinuxToCodeSysBuff
{
    // измеренные значения входных аналоговых сигналов.
    float afAiValue[AI_VALUE_FLOAT_ARRAY_LENGTH];
    // массив уставок, вводимых вручную (64 уставки FLOAT)
    float afHandUst[HAND_UST_FLOAT_ARRAY_LENGTH];
    // аналоговые сигналы регуляторов
    float afContAv[CONT_AV_FLOAT_ARRAY_LENGTH];
    // состояние входных дискретных сигналов.
    unsigned char aucDiValue[DI_VALUE_BIT_ARRAY_LENGTH];
    // состояние выходов МР.
    unsigned char aucSostDo[SOST_DO_BIT_ARRAY_LENGTH];
    // массив флагов недостоверности аналоговых сигналов.
    unsigned char aucBadAi[BAD_AI_BIT_ARRAY_LENGTH];
    // массив флагов недостоверности дискретных сигналов.
    unsigned char aucBadDi[BAD_DI_BIT_ARRAY_LENGTH];
    // внешние команды управления логикой.
    unsigned char aucLogCom[LOG_COM_BIT_ARRAY_LENGTH];
    // выходы функциональных блоков на текущем шаге.
    unsigned char aucFbOut[CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_LENGTH];
    // дискретные сигналы регуляторов
    unsigned char aucContSt[CONT_ST_BIT_ARRAY_LENGTH];

    unsigned char fucGlobalKvit; // флаг - квитирование.
    unsigned char fucGlobalReset; // флаг - сброс.
};

// структура данных, для взаимодействия с "CodeSys", через разделяемую память.
// от CodeSys к Linux.
struct TDataExchangeCodeSysToLinuxBuff
{
    // массив выходных расчетных переменных CoDesys.
    float afCalcValue[CALC_CDS_FLOAT_ARRAY_LENGTH];
    // массив выходных системых переменных CoDesys.
    float afSysValue[SYS_CDS_FLOAT_ARRAY_LENGTH];
    // управление дискретными выходами.
    unsigned char aucDoValue[DO_VALUE_BIT_ARRAY_LENGTH];
    // состояние выходов функциональной логики CoDeSys.
    unsigned char aucLogSost[LOG_SOST_BIT_ARRAY_LENGTH];

    unsigned char Sound; // флаг - звук.
};

//#pragma pack(push)
//#pragma pack(4)
// структура данных, для взаимодействия с "CodeSys", через разделяемую память.
struct TDataExchangeControlBuff
{
    // конфигурация прибора: содержимое объекта xPlcConfigDataTemp.xConfigData, типа - TConfigData.
//    unsigned char aucConfigData[sizeof(struct TConfigData)];
    TConfigData xConfigData;
    // текущее время.
    // Байт 1 - секунда 0 – 59;
    // Байт 2 - минута 0 –59;
    // Байт 3 - час 0 – 23;
    // Байт 4 - день 1 – 31;
    // Байт 5 - месяц 1 – 12.
    // Байт 6 - год 0 – 99.
    // Байт 7 – день недели 1 – 7.
    unsigned char aucCurrentTime[CURRENT_TIME_BYTE_QUANTITY];
    // флаг - данные для обмена готовы.
    bool fbDataExchangeReady;
};
//#pragma pack(pop)

// структура формирования требований отображения сигнализации на HMI.
struct TAlarmHmi
{
    // состояние сигнализации.
    // D0-D4 - ТБЛ (группа): 0,1-24(0-нет вых.на табло).
    // D6 - ТСГ1 – тип сигнализации состояния дискретного сигнала.
    // D7 - ТСГ2 – тип сигнализации состояния дискретного сигнала.
    // 00 – нет сигнализации
    // 01 – предупредительная сигнализация
    // 10 – аварийная сигнализация
    // 11 – индикация (ровный свет без звукового сигнала)
    uint8_t ui8AlarmState;
    // если ui8Archive = 1, архивировать.
    uint8_t ui8Archive;
};

// Структура описателей - "Текстовый реквизит дискретного сигнала", в базе данных.
// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
struct TDiscreteSygnalTextDescriptorPackOne
{
    // Текстовый реквизит дискретного сигнала – 14 символов ASCII.
    char acTextDescriptor[DISCRETE_SYGNAL_NAME_LENGTH];
};
#pragma pack(pop)

// Структура описателей - "Текстовый реквизит дискретного сигнала", в базе данных.
struct TDiscreteSygnalTextDescriptor
{
// Текстовый реквизит дискретного сигнала – 14 символов ASCII, плюс нуль - признак конца строки.
    char acTextDescriptor[DISCRETE_SYGNAL_NAME_LENGTH + END_OF_STRING_LENGTH];
};

//extern uint8_t ui8TempCommand;

//extern CCommunicationDeviceInterface xSpiCommunicationDevice;
extern CSpi xSpiCommunicationDevice;
//extern CSpiInterface xSpiCommunicationDevice;
extern CSerialPort xSerialPortCommunicationDevice;

extern TPlcDataBaseControl xPlcDataBaseControl; // объект в памяти - управление базой данных.
extern TAllModulesContext xAllModulesContext; // объект в памяти - контекст модулей.
// массив с объектами контекста всех модулей.
//extern TModuleContext xAllModulesContext.axAllModulesContext[];
// объект в памяти - для упорядочивания следования модулей при поиске на SPI.
extern TPlcConfigSearch xPlcConfigSearch;
extern TPlcConfigSearch xPlcConfigSearchTemp;
extern TPlcConfigData xPlcConfigData; // объект в памяти - конфигурация.
extern TPlcConfigData xPlcConfigDataTemp; // объект в памяти - вновь созданная конфигурация.
extern TPlcConfigData xPlcConfigDataTransmit; // объект в памяти - передаваемая программатору конфигурация.
extern TPlcConfigData *pxPlcConfigData;
extern TPlcConfigService xPlcConfigService; // объект в памяти - служебные переменные.
extern TPlcConfigService *pxPlcConfigService;
// объект в памяти - база данных.
// считывается из файла и хранится без изменений на протяжении всей работы.
extern TPlcDataBase xPlcDataBase;
extern TPlcDataBase *pxPlcDataBase;
extern struct TMainFlagRegister xMainFlagRegister;
// объект в памяти - Рабочий массив формирования требований отображения дискретных сигналов на HMI.
extern const struct TAlarmHmi xAlarmHmiNull;
// текущее время.
extern struct tm xCurrentTime;
extern uint8_t ui8CurrentTimeUpdateDelayCounter;
extern uint8_t ui8CurrentTimeSaveDelayCounter;

extern TDataExchangeLinuxToCodeSysBuff *pxDataExchangeLinuxToCodeSysBuff; // указатель на объект в разделяемой памяти.
extern TDataExchangeCodeSysToLinuxBuff *pxDataExchangeCodeSysToLinuxBuff; // указатель на объект в разделяемой памяти.
extern TDataExchangeControlBuff *pxDataExchangeControlBuff; // указатель на объект в разделяемой памяти.

extern pthread_mutex_t xMainDataAccessMutex;
extern unsigned char ucMainCycleTimeStart; // флаг управления временем главного цикла.
extern unsigned char ucCodeSysExchangeErrorCycleCounter;
extern unsigned char ucDataBaseError; // коды ошибок для запроса о результатах программирования.
extern unsigned char ucModuleError; // коды ошибок модулей для запроса о результатах чтения.
extern unsigned char nucDataBaseBlockIndex; // получает номер блока базы данных от Modbus.
extern unsigned char ucModulesQuantity;  // промежуточная переменная количества модулей, получает данные от аргумента (argv[1]).
//// массив сопоставления номера посадочного слота модуля с адресом на шине SPI.
//extern const uint8_t aui8ModuleSlotNumberToSpiAddressMatching[];
extern const unsigned char aucDataBaseBlockLength[]; // массив с константами размеров блоков базы данных.
extern unsigned char ucModbusSlaveAddress;

extern struct sigaction MainAction;
//extern struct sigaction ArchiveSave;
extern uint8_t ui8MainRestart;

extern struct timeval  xGlobalTimeCur; // объект в памяти - переменная для замера времени - текущее чтение значения времени.
extern struct timeval xGlobalTimeLast; // объект в памяти - переменная для замера времени - предыдущее чтение значения времени.

extern bool fbGlobalKvit; // флаг - общее квитирование.
extern bool fbGlobalReset; // флаг - общий сброс.
extern bool fbGlobalBlock; // флаг - общая блокировка.
//extern bool fbHighLevelKvit; // флаг - квитирование от МИНД.
//extern bool fbHighLevelReset; // флаг - сброс от МИНД.
//extern bool fbHighLevelBlock; // флаг - блокировка от МИНД.
//extern bool fbMindKvit; // флаг - квитирование от МИНД.
//extern bool fbMindReset; // флаг - сброс от МИНД.
//extern bool fbMindBlock; // флаг - блокировка от МИНД.
extern bool fbGlobalMmcCopy; // флаг - копирование диска mmc0 на mmc1.
extern bool fbGlobalMmcCopyEnd; // флаг - закончено копирование диска mmc0 на mmc1.

extern bool fbOneSecondPassed;
extern bool fbTenSecondsPassed;
extern unsigned char nucOneSecondCounter;
extern unsigned char nucTenSecondsCounter;
// (*Флаг общей сигн. для HMI D6-предупр. D7-аварийная (прерывистые сигналы - мигающий транспарант)*).
extern unsigned char ucAlarmStateCommon;
// (*Флаг звука для модуля сигнализации MAL*).
extern unsigned char fucZvkMal;
// (*Обшие флаги сигнализации, квитирования и сброса. Адреса ModBus протокола существующие*)
// (*адреса:120,121($78,$79)-групповая сигнализация состояния модулей ПАС-05:
// 00 - норма, 01 квитированная сигнализация, 10 активная сигнализация
// адрес: 122(7а) - сигнал ошибки конфигурации при запуске
// адреса:123,124($7b,$7с) - общий прерыв.сигнал на HMI 123 - предупр. 124 - аварийн.
// адреса:125($7d) - пер.реж.блок, 126,127 ($7e,$7f) - ком.КВИТ.и СБРОС*)
extern unsigned char fucKvSbBef;
extern unsigned char nucBlinkCounter; // (*Счетчик мигающего сигнала*)
extern bool fbBlinkIsOn; // (*Флаг СВЕЧЕНИЕ - TRUE, ПАУЗА - FALSE*)
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

//// текущее время.
//extern uint8_t aui8CurrentTime[];
// байтовый массив - состояние аналоговых измерительных каналов для взаимодействия с модулем.
// для взаимодействия с Modbus преобразовывается в битовый массив - состояние аналоговых измерительных каналов.
extern unsigned char aucStatAiByteArray[];
// байтовый массив - бинарные сигналы регуляторов для взаимодействия с модулем.
// для взаимодействия с Modbus преобразовывается в битовый массив - бинарные сигналы регуляторов.
extern unsigned char aucContStByteArray[];
extern unsigned char aucContStWriteBitArray[];
extern unsigned char aucContStWriteByteArray[];
extern unsigned char aucDoValueByteArray[]; // Буфер выходов управления реле - новое нарушение, для модулей реле МР.
extern unsigned char aucDoValueBlockByteArray[]; // Буфер выходов требование включения реле - блокировка, для модулей реле МР.
extern struct TTxsData axAllModulesTxsData[]; // массив с данными ТХС.
// массив с измеренными значениями аналоговых входов.
extern float afAnalogueInputData[];
// массив с расчетными значениями CoDeSys.
extern float afCalcValue[];
// массив выходных системых переменных CoDesys.
extern float afSysValue[];
extern unsigned char aucReperPointsAdcBufferPointer[];
// массив с нормализованной базой данных обработки аналоговых сигналов.
extern struct TAnalogueInputDescriptionWork axAnalogueInputDescriptionWork[MAX_HANDLED_ANALOGUE_INPUT];
// массив с нормализованной базой данных регуляторов модулей аналогового вывода.
extern struct TAnalogueOutputRegulatorDescriptionWork axAnalogueOutputRegulatorDescriptionWork[MAX_HANDLED_ANALOGUE_OUTPUT];
// массив с аналоговыми сигналами регуляторов.
extern struct TMtvi5ContAvData axAnalogueOutputRegulatorContAv[];
// массив с нормализованной базой данных размерностей.
extern struct TDimentionParameter axDimentionsParametersDescriptionWork[DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY];
extern struct TDiscreteInputDescriptionWork axDiscreteInputDescriptionWork[]; // массив с нормализованной базой данных обработки дискретных сигналов.
extern TDiscreteSygnalTextDescriptor axDiscreteSygnalTextDescriptor[MAX_HANDLED_DISCRETE_INPUT]; // массив с нормализованной базой данных текстовых реквизитов дискретных сигналов.
// массив с нормализованной базой данных модулей Modbus-master.
extern struct TMbmFunction1234 axModbusMasterFunction1234DescriptionWork[EXTERNAL_MODULE_FUNCTION_1234_QUANTITY];
// массив со вспомогательными данными для обработки дискретных сигналов.
extern struct TDiscreteInputWorkingData axDiscreteInputWorkingData[];
extern struct TAlarmHmi axAlarmHmi[MAX_HANDLED_DISCRETE_INPUT + MAX_HANDLED_FUNCTION_BLOCK]; // Рабочий массив формирования требований отображения сигнализации дискретных сигналов на HMI.
extern struct TAlarmHmi axAlarmModules[]; // Рабочий массив формирования требований отображения сигнализации неисправностей прибора на HMI.
extern unsigned char aucGroupDinAction[]; // массив активности сигнализации групп дискретных сигналов.
extern unsigned char aucDinAction[]; // массив активности сигнализации дискретных сигналов.
extern struct TFunctionBlockDescriptionWork axFunctionBlockDescriptionWork[]; // массив с нормализованной базой данных обработки функциональных блоков.
// массив со вспомогательными данными для обработки функциональных блоков.
extern struct TFunctionBlockWorkingData axFunctionBlockWorkingData[];
// массив с данными ошибок модулей на предыдущем шаге, для сравнения с текущими.
extern unsigned char aucBadModuleCompareBuff[];
// массив с данными ошибок отказов модулей.
extern unsigned char aucBadModuleStateBuff[];

extern unsigned char aucCoilsArray[]; // рабочий массив modbus состояния логических ячеек,
extern unsigned char aucDiscreteInputsArray[]; // рабочий массив modbus дискретных входов,
extern unsigned char aucHoldingRegistersArray[]; // рабочий массив modbus регистров хранения аналоговых сигналов,
extern unsigned char aucInputRegistersArray[]; // рабочий массив modbus регистров аналоговых входов,

//-----------------------------------------------------------------------------------------------------
// контекст modbus.
// рабочие массивы, служебные данные.
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
