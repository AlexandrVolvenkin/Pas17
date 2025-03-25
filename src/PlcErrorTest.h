#ifndef PLCERRORTEST_H_INCLUDED
#define PLCERRORTEST_H_INCLUDED

#include "PasNewConfig.h"

// BAD_MODULES_BIT_ARRAY_OFFSET(*Битовый массив неисправности модулей для сигнализации на HMI
// адреса ModBus. 1016-1055:
// биты 0 - 15 - отказы 16 модулей УСО, адреса 1016-1031,
// биты 17-19 - резерв, адреса 1033-1035
// остальное -  резерв*)
// бит 16 - отказ МAL - адрес 1032,
#define MAL_ERROR_OFFSET 16
// биты 17-19 - резерв, адреса 1033-1035
#define RESERVE_1_ERROR_OFFSET 17
// биты 17-19 - резерв, адреса 1033-1035
#define RESERVE_2_ERROR_OFFSET 18
// биты 17-19 - резерв, адреса 1033-1035
#define RESERVE_3_ERROR_OFFSET 19
// бит 20 - ошибка базы данных дискретных сигналов, адрес 1036,
#define DISCRETE_SYGNALS_DATA_BASE_ERROR_OFFSET 20
// бит 21 - ошибка базы данных модуей аналогового ввода МВА, адр 1037,
#define ANALOGUE_INPUT_MODULE_DATA_BASE_ERROR_OFFSET 21
// бит 22 - ошибка базы данных модуей дискретного  вывода МР, адр 1038,
#define DISCRETE_OUTPUT_MODULE_DATA_BASE_ERROR_OFFSET 22
// бит 23 - ошибка базы данных локальной сети нижнего уровня ЛСНУ, адр 1039
#define LOCAL_NET_MODULE_DATA_BASE_ERROR_OFFSET 23


#endif // PLCERRORTEST_H_INCLUDED
