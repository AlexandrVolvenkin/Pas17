//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CONFIGURATION_H_INCLUDED
#define CONFIGURATION_H_INCLUDED

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


enum
{
    COILS_WORK_ARRAY_LENGTH = 128,
    DISCRETE_INPUTS_ARRAY_LENGTH = 128,
    HOLDING_REGISTERS_ARRAY_LENGTH = 128,
    INPUT_REGISTERS_ARRAY_LENGTH = 128,
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
    SPI_MAX_BUS_ADDRESS = 16,
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
};

//-------------------------------------------------------------------------------
#endif // CONFIGURATION_H_INCLUDED
