//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CONFIGURATION_H_INCLUDED
#define CONFIGURATION_H_INCLUDED

// преобразование натурального числа в целое.
#define CONVERT_NATURAL_NUMBER_TO_INTEGER 1
// преобразование целого числа в натуральное.
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
// главные константы проекта. определяют размеры всех массивов.
#define MAX_HANDLED_DISCRETE_INPUT 16 // определяется картой памяти программатора.
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
#define MAX_HANDLED_ANALOGUE_INPUT 4
#define MAX_HANDLED_ANALOGUE_OUTPUT 2
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
#define INTERNAL_MODULE_QUANTITY 8
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
#define DISCRETE_MODULE_INPUT_QUANTITY 6
#define ANALOGUE_INPUT_DI_VALUE_QUANTITY 4 // количество дискретных сигналов порождаемое одним аналоговым входом.
#define DISCRETE_OUTPUT_MODULE_MAX_NUMBER 5 // количество модулей дискретных выходов управления реле.
#define MUVR_MR_DISCRETE_OUTPUT_NUMBER 4 // количество дискретных выходов управления реле.
#define DISCRETE_OUTPUT_NUMBER (MUVR_MR_DISCRETE_OUTPUT_NUMBER * DISCRETE_OUTPUT_MODULE_MAX_NUMBER) // количество дискретных выходов управления реле.
// количество аналоговых входов в одном модуле.
#define ANALOG_MODULE_INPUT_QUANTITY 4
// длина строки текстового реквизита аналогового измерения.
#define ANALOGUE_INPUT_NAME_LENGTH 8
// длина строки текстового реквизита размерности измеряемого параметра КРЗМ.
#define DIMENSIONS_PARAMETERS_NAME_LENGTH 6
// длина строки текстового реквизита дискретного сигнала.
#define DISCRETE_SYGNAL_NAME_LENGTH 14
// длина строки текстового реквизита аналогового сигнала.
#define ANALOGUE_SYGNAL_NAME_LENGTH 35
// количество аналоговых выходов в одном модуле.
#define ANALOG_OUTPUT_MODULE_REGULATORS_QUANTITY 2
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

//-------------------------------------------------------------------------------
#define MAIN_CYCLE_TIME 100000000 // время главного цикла - 100mc.
//#define MAIN_CYCLE_TIME 100000 // время главного цикла - 100mc.
#define EXTERNAL_CYCLE_STOP_SLEEP_TIME 1000 // длительность "сна", во время ожидания флага синхронизации с главным циклом.

//#define MAIN_DATA_ACCESS_SEMAPHORE_NAME "/MainDataAccessSemaphore"
//#define MAIN_DATA_ACCESS_MUTEX_NAME "/MainDataAccessMutex"

#define ONE_SECOND_TIME 10 // количество периодов главного цикла - MAIN_CYCLE_TIME.
#define TWO_SECOND_TIME 20
#define THREE_SECOND_TIME 30
#define TEN_SECOND_TIME 100

//#define MODULE_REQUEST_QUANTITY 5 // количество попыток связаться с модулем.
//#define MBM_1234_MODULE_BAD_BIT_ARRAY_LENGTH 1
//#define BITS_IN_BYTE_QUANTITY 8
//// начальное значение счётчика запросов модуля без ответа.
//// если равно нулю - модуль считается неисправным. его данные признаются недостоверными.
//// устанавливается флаг сигнализации в рабочем массиве aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET + индекс модуля].
//#define BAD_MODULE_CYCLE_COUNT_DEFAULT 10
//// начальное значение счётчика запросов модуля без ответа в течении 60 секунд.
//#define BAD_MODULE_60_SECOND_CYCLE_COUNT_DEFAULT 600
//#define BAD_MODULE_RESPONDED_OK 0
//// код ошибки когда модуль не отвечает.
//// модуль не отвечает.
//#define BAD_MODULE_NOT_RESPONDED 1
//// модуль не отвечает при старте.
//#define BAD_MODULE_NOT_RESPONDED_ON_START 2
// сигнатура ошибки. устанавливается если измеренное значение недостоверно.
//#define ANALOGUE_INPUT_ERROR 0xFF00

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


//#define ONE_BYTE_CRC_LENGTH 1
//#define TWO_BYTE_CRC_LENGTH 2
// смещение на контрольную сумму блока базы данных.
#define PLC_DATA_BASE_BLOCK_CRC_OFFSET (PLC_DATA_BASE_BLOCK_LENGTH - TWO_BYTE_CRC_LENGTH)
//// 2 - смещение на тип модуля в массиве RX SPI. 0 - команда запроса, 1 - "эхо" команды запроса.
//#define MODULE_TYPE_OFFSET 2
//#define MODULE_COMMAND_LENGTH 1
//// количество байт в ответе на запрос типа модуля.
//#define MODULE_COMMAND_ANSWER_LENGTH 1
//// количество байт в "хвосте" ответа модуля. 0x7E - OK, 0xE7 - error.
//#define TAIL_ANSWER_LENGTH 1
//#define DATA_EXCHANGE_OK 0x7E
//#define DATA_EXCHANGE_ERROR 0xE7
//#define DATA_EXCHANGE_CRC_ERROR 0x03
// MVAI5_REPER_POINTS_ADC_CHANNEL_QUANTITY + MVAI5_ANALOG_INPUT_QUANTITY// 6х7=42.
#define ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH 42

#define FRAM_LAST_SAVED_TIME_OFFSET 0
#define FRAM_LAST_SAVED_TIME_ARRAY_LENGTH sizeof(struct tm)//sizeof(struct TAnalogueSignalsArchiveHourData)
#define FRAM_ANALOGUE_MEASURE_ARCHIVE_ARRAY_OFFSET (FRAM_LAST_SAVED_TIME_OFFSET + FRAM_LAST_SAVED_TIME_ARRAY_LENGTH)
#define FRAM_ANALOGUE_MEASURE_ARCHIVE_ARRAY_LENGTH 0


//// максимальное количество адресов на шине SPI.
//#define SPI_CHIP_SELECT_MAX_ADDRESS 16
//// 2 - смещение на данные в массиве RX SPI. 0 - команда запроса, 1 - "эхо" команды запроса.
//#define SPI_DATA_BYTE_OFFSET 2
//// 1 - смещение на команду в массиве RX SPI.
//#define SPI_COMMAND_BYTE_OFFSET 1
//#define SPI_COMMAND_BYTE_LENGTH 1
//
//#define SPI_LENGTH_BYTE_OFFSET 2
//#define SPI_LENGTH_BYTE_LENGTH 1
//// первый байт - команда, второй - ответ, дальше данные.
//#define SPI_PREAMBLE_LENGTH 2

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

//#define GET_MODULE_TYPE_COMMAND 0x91 // команда - чтение типа модуля.
//#define CHANGE_PROTOCOL_COMMAND 0x92 // команда - переключить на новый протокол.
//#define GET_MODULE_TYPE_COMMAND_LENGTH 1
//#define GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH 1
//// смещение в массиве RX SPI. 0 - команда запроса, 1 - "эхо" команды запроса.
//#define GET_MODULE_TYPE_COMMAND_OFFSET 1

#define AIN_OFF_PROCESS_OWNER_IS_NONE 0
#define AIN_OFF_PROCESS_OWNER_IS_PROGRAMMER 1
#define AIN_OFF_PROCESS_OWNER_IS_HMI 2


// (sizeof(struct TAnalogueInputDescriptionDataBase) * ANALOG_MODULE_INPUT_QUANTITY)// 28х6=168.
#define ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH 168
// (sizeof(struct TMrXXOneChannelDataBase) * DISCRETE_OUTPUT_NUMBER)// 3х64=192.
#define DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_LENGTH 192
// (sizeof(struct TMbmFunction1234PackOne) * EXTERNAL_MODULE_FUNCTION_1234_QUANTITY) + 1// 13х16=208.
#define MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_LENGTH (208 + 1)
// (sizeof(struct TMbmFunction15Sourse) * EXTERNAL_MODULE_FUNCTION_15_QUANTITY)// 72х3=216.
#define MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_LENGTH 216
// (sizeof(struct TDiscreteInputDescriptionWork) * DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)// 11х18=198.
#define DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_LENGTH 192
// (sizeof(struct TAnalogueOutputRegulatorDescriptionDataBase) * ANALOG_OUTPUT_MODULE_REGULATORS_QUANTITY)// 28х6=168.
#define CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH 56
#define CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH 56
// DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY * 6// 6х16=96.
#define DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_LENGTH 96
// TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY * TEXT_TITLES_DATA_LENGTH// (384 ТР, 24 блока).
#define TEXT_TITLES_DATA_BASE_BLOCK_LENGTH 224
// (sizeof(struct TFunctionBlockDescriptionWork) * FUNCTION_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)// 19х10=190.
#define FUNCTION_BLOCK_DATA_BASE_BLOCK_LENGTH 160// 19х10=190 (120 ФБЛ, 12 блоков)
#define MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_LENGTH 208// 82 – 84 (48 ФБМ, 3 блока 13*16=208 )
#define DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH 32// 2х16=32.
#define REFERENCE_POINTS_ADC_CODES_DATA_BASE_BLOCK_LENGTH 42
#define SERIAL_AND_ID_DATA_BASE_BLOCK_LENGTH 16
#define NETWORK_ADDRESS_DATA_BASE_BLOCK_LENGTH 1
#define CONFIGURATION_DATA_BASE_BLOCK_LENGTH 32
#define RESERVED_DATA_BASE_BLOCK_LENGTH 16
#define SETTINGS_DATA_BASE_BLOCK_LENGTH sizeof(struct TPlcSettingsPackOne)


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
#define SETTINGS_DATA_BASE_BLOCK_BLOCK_QUANTITY 1

// смещения в массиве aucCoilsArray[].
// начальный адрес Modbus.
#define COILS_ARRAY_MODBUS_BEGIN_ADDRESS 0x00
// смещения в массиве aucDiscreteInputsArray[].
// начальный адрес Modbus.
#define DISCRETE_INPUTS_ARRAY_MODBUS_BEGIN_ADDRESS 0x00
// смещения в массиве aucHoldingRegistersArray[].
// начальный адрес Modbus.
#define HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS 0x00
// смещения в массиве aucInputRegistersArray[].
// начальный адрес Modbus.
#define INPUT_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS 0x00


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
// в linux воскресенье - 0.
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
    SETTINGS_DATA_BASE_BLOCK_OFFSET = 101,
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
//// 2 - смещение на данные в массиве RX SPI. 0 - команда запроса, 1 - "эхо" команды запроса.
//    SPI_DATA_BYTE_OFFSET = 2,
//// 1 - смещение на команду в массиве RX SPI.
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
//// начальное значение счётчика запросов модуля без ответа.
//// если равно нулю - модуль считается неисправным. его данные признаются недостоверными.
//// устанавливается флаг сигнализации в рабочем массиве aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET + индекс модуля].
//    BAD_MODULE_CYCLE_COUNT_DEFAULT = 10,
// смещение в промежуточном буфере на состояние бита-индикатора принятого в 5 функции модбас.
    BIT_STATE_OFFSET = 0,
// смещение в промежуточном буфере на индекс-номер, например аналогового входа.
    COMMON_INDEX_OFFSET = 0,
// смещение в промежуточном буфере на команду, например установка типа калибруемой шкалы.  .
    COMMON_COMMAND_CONTROL_OFFSET = 1,
    // смещение на количество байт в pdu
    PDU_LENGTH_OFFSET = 0,
    PDU_LENGTH_LENGTH = 1,
// смещение в промежуточном буфере на код опции.
    OPTION_CODE_OFFSET = 1,
    OPTION_CODE_LENGTH = 1,
// смещение в промежуточном буфере на данные.
    DATA_OFFSET = 2,
    DATA_LENGTH = 1,
    // размер заголовка
    PREAMBLE_LENGTH = 2,
};

enum EModbusFunctionCoilsDataMap
{
    ANALOGUE_INPUTS_BAD_STATE_OFFSET = 544,
    DISCRETE_INPUTS_BAD_STATE_OFFSET = 1056,
    // в мувр отсутствуют дискретные входы поэтому адреса недостоверности дискретных входов
    // и адреса недостоверности дискретных сигналов порождаемых аналоговыми входами совпадают
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
    NORMAL		 = 0,			// Нет отображения
    PREVENTIVE	 = 1,			// Предупредительная сигнализация
    EMERGENCY	 = 2,			// Аварийная сигнализация
    INDICATION	 = 3,			// Индикация

    IND_PREVENTIVE = 4,			// Индикация + предупредительная сигнализация
    IND_EMERGENCY  = 5,			// Индикация + аварийная сигнализация
    NAMUR_INDICATION = 6,			// Индикация недостоверности сигнала namur.
    ERROR  = 7,			// Звук нажатия кнопок.
    BEEP_SIGNAL  = 8,			// Звук нажатия кнопок.
};

enum TYPE_ALARM_STATE
{
    ALARM_STATE_IS_NORMAL		 = 0,			// норма
    ALARM_STATE_IS_RECEIPTED	 = 1,			// сигнализация квитирована
    ALARM_STATE_IS_ACTIVE	 = 2,			// сигнализация активна
    ALARM_STATE_IS_INDICATION	 = 3,			// Индикация
};

enum Errors
{
    NO_ERROR	= 0,
    IMD_ERROR	= 1,		// Ошибка связи с модулями ввода
    OMD_ERROR	= 2,		// Ошибка связи с модулями вывода
    CFG_ERROR	= 3,		// Ошибка конфигурации
    DB_ERROR	= 4,		// Ошибка БД
    MBS_ERROR	= 5,		// Ошибка канала связи
};

enum INPUT_STATE_CODE
{
    OFF		= 0x00,			// Дективация датчика
    ON		= 0x01,			// Срабатывание датчика
    WIRE_BREAK	= 0x02,			// Обрыв линии
    SHORT_CIRCUIT	= 0x03,			// Короткое замыкание
    INPUT_IS_INVALID	= 0x04,			// вход недостоверен
};


//-------------------------------------------------------------------------------
#endif // CONFIGURATION_H_INCLUDED
