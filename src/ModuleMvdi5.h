#ifndef MODULEMVDI5_H_INCLUDED
#define MODULEMVDI5_H_INCLUDED

#include "PasNewConfig.h"


//============================================================================
// MVDI5
//#define MODULE_TYPE_MVDI 0x01

#define MVDI5_DISCRETE_INPUT_QUANTITY 12
// состо€ние 12 дискретных входов из модул€ поступают в двух байтах. один байт - 6 входов.
#define MVDI5_DISCRETE_INPUT_BYTE_QUANTITY 2
// один байт - 6 дискретных входов.
#define MVDI5_DISCRETE_INPUT_BITS_IN_BYTE_QUANTITY (MVDI5_DISCRETE_INPUT_QUANTITY / 2)
// количество байт занимаемое модулем в рабочем массиве - состо€ние входных дискретных сигналов.
#define MVDI5_DI_VALUE_BIT_ARRAY_LENGTH MVDI5_DISCRETE_INPUT_QUANTITY
// количество байт занимаемое модулем в рабочем массиве - массив флагов недостоверности дискретных сигналов.
#define MVDI5_BAD_DI_BIT_ARRAY_LENGTH MVDI5_DISCRETE_INPUT_QUANTITY
// количество байт занимаемое модулем в рабочем массиве - массив флагов отказов модулей ѕј—-05.
#define MVDI5_MODULE_BAD_BIT_ARRAY_LENGTH 1
// команда - «апрос на циклический обмен.
#define MVDI5_GET_DISCRETE_INPUT_DATA_COMMAND 0x42
#define MVDI5_DATA_READY 0x42
#define MVDI5_DATA_NOT_READY 0x24
// длина ответного сообщени€ не включающа€ SPI_PREAMBLE_LENGTH + ONE_BYTE_CRC_LENGTH.
#define MVDI5_GET_DISCRETE_INPUT_DATA_COMMAND_ANSWER_LENGTH 3



#endif // MODULEMVDI5_H_INCLUDED
