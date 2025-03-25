#ifndef MODULEMVDS9_H_INCLUDED
#define MODULEMVDS9_H_INCLUDED

//============================================================================
// MVDS9
//#define MODULE_TYPE_MVDS9 0x0A

#define MVDS9_DISCRETE_INPUT_QUANTITY 12
// состояние 12 дискретных входов из модуля поступают в трёх байтах.
// байт 1:D0,D1-сост.вх.1, D2,D3-сост.вх.2,D4,D5-сост.вх.3,D6,D7-сост.вх.4
// байт 2:D0,D1-сост.вх.5, D2,D3-сост.вх.6,D4,D5-сост.вх.7,D6,D7-сост.вх.8
// байт 3:D0,D1-сост.вх.9, D2,D3-сост.вх.10,D4,D5-сост.вх.11,D6,D7-сост.вх.12
// четные биты (D0,D2,D4,D6) =0 - разомкнуто (OFF), =1 - замкнуто (ON)
// нечетные биты (D1,D3,D5,D7) =0 - достоверно, =1 - недостоверно
#define MVDS9_DISCRETE_INPUT_BYTE_QUANTITY 3
#define MVDS9_DISCRETE_INPUT_BITS_IN_BYTE_QUANTITY 8
// блок состоит из 8 элементов по 2 байта.
#define MVDS9_DATA_BASE_BLOCK_LENGTH 2
// количество байт занимаемое модулем в рабочем массиве - состояние входных дискретных сигналов.
#define MVDS9_DI_VALUE_BIT_ARRAY_LENGTH MVDS9_DISCRETE_INPUT_QUANTITY
// количество байт занимаемое модулем в рабочем массиве - массив флагов недостоверности дискретных сигналов.
#define MVDS9_BAD_DI_BIT_ARRAY_LENGTH MVDS9_DISCRETE_INPUT_QUANTITY
// количество байт занимаемое модулем в рабочем массиве - массив флагов отказов модулей ПАС-05.
#define MVDS9_MODULE_BAD_BIT_ARRAY_LENGTH 1
// команда - Запрос на циклический обмен.
#define MVDS9_GET_DISCRETE_INPUT_DATA_COMMAND 0x42
#define MVDS9_DATA_READY 0x42
#define MVDS9_DATA_NOT_READY 0x24
#define MVDS9_GET_DISCRETE_INPUT_DATA_COMMAND_ANSWER_LENGTH 5
// Запрос на прием БД: $82 .
#define MVDS9_GET_DATA_BASE_COMMAND 0x82
#define MVDS9_GET_DATA_BASE_COMMAND_ANSWER_LENGTH 6
// Запрос на передачу БД: $81.
#define MVDS9_SET_DATA_BASE_COMMAND 0x81
#define MVDS9_SET_DATA_BASE_COMMAND_ANSWER_LENGTH 7

#define MVDS9_CHECK_DATA_BASE_WRITE_ANSWER_OK 0x7E
#define MVDS9_CHECK_DATA_BASE_WRITE_ANSWER_ERROR 0xE7

// структура базы данных одного модуля дискретных входов.
struct TMvds9ModuleDataBase
{
    unsigned char ucMvds9ModuleDataBaseByte1;
    unsigned char ucMvds9ModuleDataBaseByte2;
};


#endif // MODULEMVDS9_H_INCLUDED
