#ifndef MBMFUNCTION15_H_INCLUDED
#define MBMFUNCTION15_H_INCLUDED

#include "PasNewConfig.h"

// длина массива с Адресами дискретных сигналов в битовом массиве DI_value (0-512) при TUST D3 = 0
// или Адресами логических ячеек в битовом массиве Sost_DO (0-1536) при TUST D3 = 1.
#define MBM_FUNCTION_15_SOURCE_BIT_ADDRESS_ARRAY_LENGTH 32
// адрес бита команды - "квитирование".
#define MBM_FUNCTION_15_KVIT_BIT_FUNCTION_5_ADDRESS 0x00
// адрес бита команды - "сброс".
#define MBM_FUNCTION_15_RESET_BIT_FUNCTION_5_ADDRESS 0x01
// маска бита выбора источника входного сигнала в байте ucTust.
// D3 = 0 – входные сигналы из массива DI_value
// D3 = 1 – логические ячейки из массивов, начиная с Sost_DO
#define TUST_DI_VALUE_OR_SOST_DO_SOURCE_BIT_MASK 0x08
#define TUST_DI_VALUE_OR_SOST_DO_SOURCE_OFFSET 1000
// маска бита - разрешение квитирования в байте ucTust.
// D4 – КВ (Е/Н).
#define TUST_KVIT_PC_ENABLE_BIT_MASK 0x10
// маска бита - разрешение сброса в байте ucTust.
// D5 – СБ (Е/Н).
#define TUST_RESET_PC_ENABLE_BIT_MASK 0x20

//-----------------------------------------------------------------------------------------------------
// код модуля PSS7.
#define MODULE_TYPE_PSS7 0x06

// Структура описателей функциональных блоков «МВМ», функции 15, в базе данных.
struct TMbmFunction15
{
    unsigned char ucModbusFunction; // Код функции протокола MODBUS (1, 2, 3, 4).
    unsigned char ucModbusSlaveAddress; // Адрес SLAVE в запросе MS.
    unsigned char ucAddressH; // Начальный адрес данных в запросе – старший байт.
    unsigned char ucAddressL; // Начальный адрес данных в запросе – младший байт.
    unsigned char ucQuantityH; // Количество  данных в запросе – старший байт.
    unsigned char ucQuantityL; // Количество  данных в запросе – младший байт.
    // Код типа устройства (D0 – D3 п. 6.2.1)
    // D5 – КВ (Е/Н), D6 – СБ (Е/Н) - только для ПСС-07
    unsigned char ucTust;
    unsigned char ucIOQuantity; // Количество входов или выходов функционального блока.
    // Начальный адрес в битовом массиве Di_value (ф1, 2, 3, 4, 15). Количество адресов определяется MBM_IO
    uint16_t ui16DiValueArrayBegin;
    // Адрес дискретного сигнала в битовом массиве:
    // - 1-384 сигнал из массива DI_value (0-383);
    // - 1001 – 1240 сигнал из массива Log_Sost (0-239); // 2120-2359 логич. выходы CoDeSyst на тек.шаге(240).
    // - 0 – бит не запрограммирован (в ф.15 всегда передается 0).
    unsigned char aucBitSourceAddress[MBM_FUNCTION_15_SOURCE_BIT_ADDRESS_ARRAY_LENGTH];
};


#endif // MBMFUNCTION15_H_INCLUDED
