#ifndef MODULEMTVI5_H_INCLUDED
#define MODULEMTVI5_H_INCLUDED

#include "PasNewConfig.h"


//============================================================================
// MTVI5
// длительность основного цикла прибора 100mc.
// обмен данными с модулем аналогового вывода производится через -
// (длительность основного цикла прибора 100mc * MTVI5_LOOP_SKIP_QUANTITY).
#define MTVI5_LOOP_SKIP_QUANTITY 5
// задержка на 1.5 секунды, для записи в EEPROM модуля.
// (длительность основного цикла прибора 100mc * MTVI5_WRITE_WAIT_QUANTITY).
#define MTVI5_WRITE_WAIT_QUANTITY 15
// код типа модуля MTVI5.
//#define MODULE_TYPE_MTVI5 0x0E
#define MAX_MTVI5_MODULES_QUANTITY 4
// количество аналоговвых выходов в модуле.
#define MTVI5_ANALOG_OUTPUT_QUANTITY 6
#define MTVI5_STAT_BIT_QUANTITY 8
// количество регуляторов в модуле.
#define MTVI5_REGULATOR_QUANTITY 6
// максимальный целочисленный код.
#define MTVI5_MAX_CODE 16383
// один процент максимального целочисленного кода.
#define MTVI5_MAX_CODE_ONE_PERCENT (MTVI5_MAX_CODE / PERCENT_100)
// размер занимаемый в массиве флагов отказов модулей.
#define MTVI5_MODULE_BAD_BIT_ARRAY_LENGTH 1

// состояние автомата - нормальный обмен.
#define MTVI5_FSM_STATE_TRANSFER 0x00
// состояние автомата - запись базы данных регуляторов.
#define MTVI5_FSM_STATE_WRITE_REGULATOR_DATABASE 0x01
// состояние автомата - запись базы данных регуляторов и задатчиков.
#define MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_REGULATOR 0x02
#define MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_PSP 0x03
#define MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_PSP_WRITE_WAITING 0x04



#define MTVI5_EXCHANGE_SPEED_IN_HZ SPEED_IN_HZ//200000UL//

// биты состояния регуляторов «CONT_ST».
// D0 - «Р/А» – 0 – ручной, 1 – автоматический;
#define MTVI5_STAT_AUTO_OR_MANUAL_BIT 0
// D1 - «АП» – 0 - режим АП не включен, 1 - режим АП включен, при включении режима АП автоматически устанавливается бит 0=1;
#define MTVI5_STAT_NO_AP_OR_AP_BIT 1
// D2 - «ПРОГРАММА» – 0 - режим «ПРОГРАММА» не включен, 1 - режим «ПРОГРАММА» включен;
#define MTVI5_STAT_MODE_PROGRAMM_BIT 2
// D3 - вх.дискр.упр.сигнал 2 (в регуляторе тип 1 – запись, хранение, чтение, действия на алгоритм ПИД не оказывает);
#define MTVI5_STAT_INPUT_DESCRETE_CONTROL_2_BIT 3
// D4 - вх.дискр.упр.сигнал 3 (в регуляторе тип 1 – запись, хранение, чтение, действия на алгоритм ПИД не оказывает);
#define MTVI5_STAT_INPUT_DESCRETE_CONTROL_3_BIT 4
// D5 - вых.дискр.сигнал 1 (в регуляторе тип 1 – записи нет, при чтении всегда - 0);
#define MTVI5_STAT_OUTPUT_DESCRETE_CONTROL_1_BIT 5
// D6 - ошибка ЦАП – 0 – норма, 1 - обрыв линии IOUT, неисправность ЦАП
#define MTVI5_STAT_DAC_ERROR_BIT 6
// D7 - ошибка PV - 0 – норма, 1 – недостоверность;
#define MTVI5_STAT_PV_ERROR_BIT 7
// код запроса на циклический обмен данными.
#define MTVI5_DATA_EXCHANGE_COMMAND 0x44
// код запроса на загрузку предыдущего состояния регуляторов.
#define MTVI5_RESTORE_OUTS_COMMAND 0x45
#define MTVI5_DATA_NOT_READY 0x24
#define MTVI5_DATA_BASE_ERROR 0x25
// был сброс или отключения питания, требуется загрузка предыдущего состояния регуляторов.
#define MTVI5_LOAD_OUTS 0x26
// ошибка обмена данными с ЦАП модуля МТВИ.
#define MTVI5_DAC_LINK_ERROR 0x27
// ошибка обмена данными с ЦАП модуля МТВИ.
#define MTVI5_OUT_DAC_ERROR 0x28
// ошибка обмена данными с ЦАП модуля МТВИ.
#define MTVI5_OUT_DAC_ERROR_OFFSET 0x01
// ошибка обмена данными с ЦАП модуля МТВИ.
#define NO_ERROR_OFFSET 0x00
// ошибка обмена данными модуля МТВИ.
#define MTVI5_BAD_ANSWER_ERROR_OFFSET 0x00
// ошибка цепочки ЦАП модуля МТВИ.
#define MTVI5_DAC_LINK_ERROR_OFFSET 0x0A
// ошибка базы данных модуля МТВИ.
#define MTVI5_DATA_BASE_ERROR_OFFSET 0x0B
// ошибка базы данных модуля МТВИ.
#define MVAI_REPER_POINTS_DATA_BASE_ERROR_OFFSET 0x0C
// код запроса на чтение базы данных регуляторов из модуля.
#define MTVI5_GET_REGULATOR_DATA_BASE_COMMAND 0x84
// код запроса на чтение базы данных программных задатчиков из модуля.
#define MTVI5_GET_PSP_DATA_BASE_COMMAND 0x85
// код запроса на запись базы данных регуляторов в модуль.
#define MTVI5_SET_REGULATOR_DATA_BASE_COMMAND 0x81
// код запроса на запись базы данных программных задатчиков в модуль.
#define MTVI5_SET_PSP_DATA_BASE_COMMAND 0x82
// режимы работы регулятора.
#define REGULATOR_MODE_MANUAL 1
#define REGULATOR_MODE_AUTO_NORMAL 2
#define REGULATOR_MODE_AUTO_PROGRAMM_SET_POINT 3
#define REGULATOR_MODE_AUTO_FORCE_CONTROL 4
// биты ucControl описателя регулятора.
// DO - источник сигнала PV (регулируемого параметра для TYPE 1 или входа повторителя сигнала для TYPE 0) - адрес в байте 0 описателя:
// DO = 0 – аналоговый входной сигнал AIn (в код 0-16383 масшт. по шкале);
// DO = 1 – расчетная величина Сalc_cds (с выхода CoDeSys передается в
// 0-100%, масштабируется в код 0-16383);
// D1 – D7 – резерв.
#define REGULATOR_CONTROL_PV_SOURSE_IS_CALC_CDS 0

// ;____________________________________________________________________________________
// ;	Передача МЦП -> МТВИ				Прием МЦП <- МТВИ
// ;0)	N-количество байтов в сеансе обмена	|	нет
// ;1)	PV - по 2 байта (код 0-16383)		|OUT - по 2 байта (код 0-16383)
// ; 	на регулятор (6*2=12 байтов)		|на регулятор (6*2=12 байтов)
// ;2) SP-(реж.АВТО) или OUT (реж.РУЧН) 		|SP-всегда
// ;	по 2 байта на рег-р (2*6=12 байта МАХ)	|по 2 байта на рег-р (2*6=12 байта МАХ)
// ;3) STAT-1 байт (битовые величины)		|STAT-1 байт (битовые величины)
// ;	по 1 байту на рег-р (6*1=6 байтов МАХ)	|по 1 байту на рег-р (6*1=6 байтов МАХ)
// ;4)	Флаг изменения SP, OUT-1 байт		|	нет
// ;5) КС переданных данных 1,2,3,4 - 2 байта	|	КС переданных данных 1,2,3,4 - 2 байта
// ;6) код завершения $7e - ОК			|	код завершения $7e - ОК
// ;____________________________________________________________________________________
// ;Количество байтов в сеансе обмена определяется количеством запрограммированных
// ;регуляторов от 1 до 6 по 5 байтов на регулятор - 30+1 байт МАХ

// размер данных с длиной пакета.
#define MTVI5_FRAME_LENGTH_BYTE_QUANTITY 1
// размер данных одного регулятора в байтах.
#define MTVI5_ONE_REGULATOR_FRAME_BYTE_QUANTITY 5
// размер данных флага изменения SP, OUT-1 байт.
#define MTVI5_SP_OUT_SET_BYTE_QUANTITY 1
// измеренное значение аналогового входа по адресу входа регулируемого параметра (PV=1-96) - недостоверно.
#define MTVI5_PV_ERROR 0xFE00
#define MTVI5_RESTORE_OUTS_BUFF_LENGTH (MTVI5_ANALOG_OUTPUT_QUANTITY * sizeof(short))
#define MTVI5_RESTORE_OUTS_FLAG_BYTE_QUANTITY 1

//// структура данных аналоговых переменных регуляторов «CONT_AV»
//struct TMtvi5ContAvData
//{
//    float fSp;
//    float fOut;
//    float fPv;
//};

// структура данных для преобразования данных отправляемых в модуль.
struct TMtvi5TransmitData
{
    unsigned short usPv;
    unsigned short usSpOut;
};
#define MTVI5_CONT_AV_BYTE_ARRAY_LENGTH MTVI5_ANALOG_OUTPUT_QUANTITY * sizeof(struct TMtvi5ContAvData)
#define MTVI5_CONT_ST_BYTE_ARRAY_LENGTH MTVI5_ANALOG_OUTPUT_QUANTITY * 1

#endif // MODULEMTVI5_H_INCLUDED
