#ifndef FUNCTIONBLOCKS_H_INCLUDED
#define FUNCTIONBLOCKS_H_INCLUDED

#include "PasNewConfig.h"

// количество выходов управлени€ реле.
#define FUNCTION_BLOCK_RELAY_OUTPUT_QUANTITY DO_VALUE_BIT_ARRAY_LENGTH
// типы функциональных блоков.
// функциональный блок - "не".
#define FUNCTION_BLOCK_FLOW_NO 0
// функциональный блок - "и".
#define FUNCTION_BLOCK_FLOW_AND 1
// функциональный блок - "и-не".
#define FUNCTION_BLOCK_FLOW_AND_NOT 2
// функциональный блок - "или".
#define FUNCTION_BLOCK_FLOW_OR 3
// функциональный блок - "или-не".
#define FUNCTION_BLOCK_FLOW_OR_NOT 4
// функциональный блок - "гистерезис".
#define FUNCTION_BLOCK_FLOW_HISTERESIS 5
// функциональный блок - "таймер".
#define FUNCTION_BLOCK_FLOW_TIMER 6
// функциональный блок - "триггер".
#define FUNCTION_BLOCK_FLOW_TRIGGER 7
// функциональный блок - "аналоговый компаратор".
#define FUNCTION_BLOCK_FLOW_ANALOGUE_COMPARATOR 8

// источники входных дискретных сигналов функциональных блоков.
#define FUNCTION_BLOCK_INPUT_SOURCE_DI_VALUE 0
#define FUNCTION_BLOCK_INPUT_SOURCE_FUNCTION_BLOCK_OUTS 1
#define FUNCTION_BLOCK_INPUT_SOURCE_BAD_AI 2
#define FUNCTION_BLOCK_INPUT_SOURCE_LOG_SOST 3
#define FUNCTION_BLOCK_INPUT_SOURCE_BAD_DI 4

// типы таймера дл€ функциональных блоков - "таймер".
// задержка включени€.
#define FUNCTION_BLOCK_TIMER_TYPE_ON_DELAY 0
#define FUNCTION_BLOCK_TIMER_TYPE_RESERVED 1
// задержка отключени€.
#define FUNCTION_BLOCK_TIMER_TYPE_OFF_DELAY 2
// "короткий импульс".
#define FUNCTION_BLOCK_TIMER_TYPE_SHORT_PULSE 3
// "удлинЄнный импульс".
#define FUNCTION_BLOCK_TIMER_TYPE_LONG_PULSE 4
// (*ƒискретность времени 0-200 м—, 1-1—, 2-10—*)
#define FUNCTION_BLOCK_TIMER_DISCRETENESS_100MS 0
#define FUNCTION_BLOCK_TIMER_DISCRETENESS_1S 1
#define FUNCTION_BLOCK_TIMER_DISCRETENESS_10S 2

// типы триггера дл€ функциональных блоков - "триггер".
// приоритет сброса.
#define FUNCTION_BLOCK_TRIGGER_TYPE_R_S 0
// приоритет установки.
#define FUNCTION_BLOCK_TRIGGER_TYPE_S_R 1

// —труктура описателей Ѕƒ функциональных блоков.
// источник: PAS_05_PO.DOC, стр - 3,
// 3. Ѕј«ј ƒјЌЌџ’ ќЅ–јЅќ“ » ƒ»— –≈“Ќџ’ —»√ЌјЋќ¬
struct TFunctionBlockDescriptionWork
{
    unsigned char ucFunctionBlockType;
    // адреса входных управл€ющих сигналов и др.
    // дл€ каждого типа функционального блока свои данные.
    unsigned char aucInputAddress[8];
    // D0-D4 - “ЅЋ (группа): 0,1-24(0-нет вых.на табло).
    // D5 - ј–’ =1 архивировать, 0 Ц не архивировать.
    // D6 - “—√1 Ц тип сигнализации состо€ни€ выхода ‘ЅЋ:
    // D7 - “—√2 Ц тип сигнализации состо€ни€ выхода ‘ЅЋ:
    // 00 Ц нет сигнализации
    // 01 Ц предупредительна€ сигнализаци€
    // 10 Ц аварийна€ сигнализаци€
    // 11 Ц индикаци€ (ровный свет без звукового сигнала)
    unsigned char ucTalArhGrp;
    // Ѕайт 1:
    // ¬ыходы на ћ–1:D0=1 есть вых.на реле 1,D0=0-нет вых на реле 1 и т.д.
    // Ѕайт 2,3,4,5,6,7,8:
    // ¬ыходы на ћ–2,ћ–3,ћ–4,ћ–5,MP6,MP7,MP8:аналог.ћ–1.
    unsigned char aucRelayOut[8];
    // контрольна€ сумма ($ff+сумма байтов 1-10)    *).
    unsigned char ucCrc;
};

// дл€ сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// —труктура описателей Ѕƒ функциональных блоков.
// источник: PAS_05_PO.DOC, стр - 3,
// 3. Ѕј«ј ƒјЌЌџ’ ќЅ–јЅќ“ » ƒ»— –≈“Ќџ’ —»√ЌјЋќ¬
struct TFunctionBlockDescriptionWorkPackOne
{
    unsigned char ucFunctionBlockType;
    // адреса входных управл€ющих сигналов и др.
    // дл€ каждого типа функционального блока свои данные.
    unsigned char aucInputAddress[8];
    // D0-D4 - “ЅЋ (группа): 0,1-24(0-нет вых.на табло).
    // D5 - ј–’ =1 архивировать, 0 Ц не архивировать.
    // D6 - “—√1 Ц тип сигнализации состо€ни€ выхода ‘ЅЋ:
    // D7 - “—√2 Ц тип сигнализации состо€ни€ выхода ‘ЅЋ:
    // 00 Ц нет сигнализации
    // 01 Ц предупредительна€ сигнализаци€
    // 10 Ц аварийна€ сигнализаци€
    // 11 Ц индикаци€ (ровный свет без звукового сигнала)
    unsigned char ucTalArhGrp;
    // Ѕайт 1:
    // ¬ыходы на ћ–1:D0=1 есть вых.на реле 1,D0=0-нет вых на реле 1 и т.д.
    // Ѕайт 2,3,4,5,6,7,8:
    // ¬ыходы на ћ–2,ћ–3,ћ–4,ћ–5,MP6,MP7,MP8:аналог.ћ–1.
    unsigned char aucRelayOut[8];
    // контрольна€ сумма ($ff+сумма байтов 1-10)    *).
    unsigned char ucCrc;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------------------------------
// данные описател€ функциональных блоков - обща€ часть всех блоков.
// маски злементов структуры описателей Ѕƒ функциональных блоков.
// struct TFunctionBlockDescriptionWork
// источник: PAS_05_PO.DOC, стр - 3,
// 3. Ѕј«ј ƒјЌЌџ’ ќЅ–јЅќ“ » ƒ»— –≈“Ќџ’ —»√ЌјЋќ¬
// Ѕайт: ucTalArhGrp.
// “‘ЅЋ Ц тип функционального блока:
#define FUNCTION_BLOCK_TYPE_FBL_MASK 0x0F
// D0-D4 - є “ЅЋ Ц номер €чейки светового табло: (1-24), 0 Ц нет выхода на световое табло.
#define LIGHT_BOARD_CELL_NUMBER_FBL_MASK 0x1F
// D5 - ј–’ =1 архивировать, 0 Ц не архивировать.
#define ARCHIVE_FBL_MASK 0x20
// D6-D7 - “—√2, “—√1 Ц тип сигнализации состо€ни€ выхода ‘ЅЋ:
// 00 Ц нет сигнализации;
// 01 Ц предупредительна€ сигнализаци€;
// 10 Ц аварийна€ сигнализаци€;
// 11 Ц индикаци€ (ровный свет без звукового сигнала).
// D6 Ц “—√1.
#define SIGNALING_TYPE_1_FBL_MASK 0x40
// D7 Ц “—√2.
#define SIGNALING_TYPE_2_FBL_MASK 0x80

// данные элементов в байте - ucFunctionBlockType, структуры - struct TFunctionBlockDescriptionWork.
// “‘ЅЋ Ц тип функционального блока:
#define FUNCTION_BLOCK_TYPE_FBL(pointer) (((pointer) -> ucFunctionBlockType) & FUNCTION_BLOCK_TYPE_FBL_MASK)

// данные элементов в байте - ucTalArhGrp, структуры - struct TFunctionBlockDescriptionWork.
// “ЅЋ Ц номер €чейки светового табло.
#define LIGHT_BOARD_CELL_NUMBER_FBL(pointer) (((pointer) -> ucTalArhGrp) & LIGHT_BOARD_CELL_NUMBER_FBL_MASK)
// ј–’ =1 архивировать, 0 Ц не архивировать.
#define ARCHIVE_FBL(pointer) (((pointer) -> ucTalArhGrp) & ARCHIVE_FBL_MASK)
// “—√1 Ц тип сигнализации состо€ни€ выхода ‘ЅЋ
#define SIGNALING_TYPE_1_FBL(pointer) (((pointer) -> ucTalArhGrp) & SIGNALING_TYPE_1_FBL_MASK)
// “—√2 Ц тип сигнализации состо€ни€ выхода ‘ЅЋ
#define SIGNALING_TYPE_2_FBL(pointer) (((pointer) -> ucTalArhGrp) & SIGNALING_TYPE_2_FBL_MASK)
// контрольна€ сумма описател€.
#define CRC_FBL(pointer) ((pointer) -> ucCrc)

//-----------------------------------------------------------------------------------------------------
// данные описател€ функциональных блоков - Ђ»ї, Ђ»-Ќ≈ї, Ђ»Ћ»ї, Ђ»Ћ»-Ќ≈ї, Ђ√—“ї гистерезис.
//TFunctionBlockDescriptionWork

// индексы в массиве - aucInputAddress[8], структуры - struct TFunctionBlockDescriptionWork.
// »¬х—1 Ц источник входного дискретного сигнала - 1.
#define INPUT_SOURCE_FBL_1_INDEX 0
// »¬х—2 Ц источник входного дискретного сигнала - 2.
#define INPUT_SOURCE_FBL_2_INDEX 2
// »¬х—3 Ц источник входного дискретного сигнала - 3.
#define INPUT_SOURCE_FBL_3_INDEX 4
// »¬х—4 Ц источник входного дискретного сигнала - 4.
#define INPUT_SOURCE_FBL_4_INDEX 6

// “¬х1Ц тип входа 1.
#define INPUT_TYPE_FBL_1_INDEX 0
// “¬х2Ц тип входа 2.
#define INPUT_TYPE_FBL_2_INDEX 2
// “¬х3Ц тип входа 3.
#define INPUT_TYPE_FBL_3_INDEX 4
// “¬х4Ц тип входа 4.
#define INPUT_TYPE_FBL_4_INDEX 6

// јƒ–≈—1 Ц в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_1_INDEX 0
// јƒ–≈—2 Ц в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_2_INDEX 2
// јƒ–≈—3 Ц в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_3_INDEX 4
// јƒ–≈—4 Ц в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_4_INDEX 6

// јƒ–≈—1 Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_1_INDEX 1
// јƒ–≈—2 Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_2_INDEX 3
// јƒ–≈—3 Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_3_INDEX 5
// јƒ–≈—4 Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_4_INDEX 7

// маски элементов в массиве - aucInputAddress[8], структуры - struct TFunctionBlockDescriptionWork.
// »¬х—1 Ц »¬х—4 Ц источник входного дискретного сигнала:
// 0 Ц входной сигнал ”—ќ (битовый массив TSOST, адрес 1-192);
// 1 Ц выход ‘ЅЋ (адрес 1-120) или команда управлени€ с верхнего уровн€  ¬” (адрес 121-184);
// 2 Ц флаг недостоверности входного аналогового сигнала (адрес 1-96);
// 3 Ц дискретный выход алгоритма CoDeSys (адрес 1-112 только дл€ дл€ ѕј—05-16 CDS)
// 4 Ц флаг недостоверности входного дискретного сигнала (адрес 1-192);
#define INPUT_SOURCE_FBL_MASK 0x07
// “¬х1 Ц “¬х4 Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define INPUT_TYPE_FBL_MASK 0x08
// јƒ–≈—1 Ц јƒ–≈—4 Ц адрес в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_MASK 0xC0
// јƒ–≈—1 Ц јƒ–≈—4 Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_MASK 0xFF

// данные элементов в массиве - aucInputAddress[8], структуры - struct TFunctionBlockDescriptionWork.
// »¬х—1 Ц источник входного дискретного сигнала - 1.
#define INPUT_SOURCE_FBL_1(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_1_INDEX]) & INPUT_SOURCE_FBL_MASK)
// »¬х—2 Ц источник входного дискретного сигнала - 2.
#define INPUT_SOURCE_FBL_2(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_2_INDEX]) & INPUT_SOURCE_FBL_MASK)
// »¬х—3 Ц источник входного дискретного сигнала - 3.
#define INPUT_SOURCE_FBL_3(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_3_INDEX]) & INPUT_SOURCE_FBL_MASK)
// »¬х—4 Ц источник входного дискретного сигнала - 4.
#define INPUT_SOURCE_FBL_4(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_4_INDEX]) & INPUT_SOURCE_FBL_MASK)

// “¬х1Ц тип входа 1.
#define INPUT_TYPE_FBL_1(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_1_INDEX]) & INPUT_TYPE_FBL_MASK)
// “¬х2Ц тип входа 2.
#define INPUT_TYPE_FBL_2(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_2_INDEX]) & INPUT_TYPE_FBL_MASK)
// “¬х3Ц тип входа 3.
#define INPUT_TYPE_FBL_3(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_3_INDEX]) & INPUT_TYPE_FBL_MASK)
// “¬х4Ц тип входа 4.
#define INPUT_TYPE_FBL_4(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_4_INDEX]) & INPUT_TYPE_FBL_MASK)

// јƒ–≈—1 Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_HIGH_FBL_1(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_1_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)
// јƒ–≈—2 Ц адрес в массиве источника входного дискретного сигнала - младший байт;
#define ADDRESS_HIGH_FBL_2(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_2_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)
// јƒ–≈—3 Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_HIGH_FBL_3(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_3_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)
// јƒ–≈—4 Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_HIGH_FBL_4(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_4_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)

// јƒ–≈—1 Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_1(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_1_INDEX])
// јƒ–≈—2 Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_2(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_2_INDEX])
// јƒ–≈—3 Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_3(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_3_INDEX])
// јƒ–≈—4 Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_4(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_4_INDEX])

//-----------------------------------------------------------------------------------------------------
// данные описател€ функционального блока - Ђ“аймерї.
// индексы в массиве - aucInputAddress[8], структуры - struct TFunctionBlockDescriptionWork.
// »¬х—(SET) Ц источник входного дискретного сигнала.
#define INPUT_SOURCE_FBL_SET_INDEX 0
// »¬х—(RESET) Ц источник входного дискретного сигнала.
#define INPUT_SOURCE_FBL_RESET_INDEX 2

// “¬х(SET) Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define INPUT_TYPE_FBL_SET_INDEX 0
// “¬х(RESET) Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define INPUT_TYPE_FBL_RESET_INDEX 2

// јƒ–≈—1(SET) Ц адрес в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_SET_INDEX 0
// јƒ–≈—2(SET) Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_SET_INDEX 1

// јƒ–≈—1(RESET) Ц адрес в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_RESET_INDEX 2
// јƒ–≈—2(RESET) Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_RESET_INDEX 3

// ƒ¬« Ц дискретность времени задержки: 0 -100 мсек,1 Ц ед. сек.,2 Ц дес. сек.
#define DELAY_TIME_DISCRETENESS_FBL_INDEX 4
// ”¬« Ц уставка времени задержки.
#define DELAY_TIME_SETPOINT_FBL_INDEX 5
// јƒ–≈— ”¬« Ц адрес уставки времени задержки.
#define DELAY_TIME_SETPOINT_FBL_ADDRESS_INDEX 6
// “”—“ Ц тип уставки времени задержки.
#define DELAY_TIME_SETPOINT_FBL_TYPE_INDEX 6

// маски элементов в массиве - aucInputAddress[8], структуры - struct TFunctionBlockDescriptionWork.
// “»ѕ “ј…ћ≈–ј Ц код типа таймера 0 - 4
// 0 Ц Ђзадержка включени€ї;
// 1 Ц Ђзадержка включени€ с запоминаниемї;
// 2 Ц Ђзадержка отключени€ї;
// 3 Ц Ђкороткий импульсї;
// 4 Ц Ђудлиненный импульсї;
#define TIMER_TYPE_FBL_MASK 0xF0
// »¬х—1 Ц »¬х—4 Ц источник входного дискретного сигнала:
// 0 Ц входной сигнал ”—ќ (битовый массив TSOST, адрес 1-192);
// 1 Ц выход ‘ЅЋ (адрес 1-120) или команда управлени€ с верхнего уровн€  ¬” (адрес 121-184);
// 2 Ц флаг недостоверности входного аналогового сигнала (адрес 1-96);
// 3 Ц дискретный выход алгоритма CoDeSys (адрес 1-112 только дл€ дл€ ѕј—05-16 CDS)
// 4 Ц флаг недостоверности входного дискретного сигнала (адрес 1-192);
#define INPUT_SOURCE_FBL_MASK 0x07
// “¬х Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define INPUT_TYPE_FBL_MASK 0x08
// јƒ–≈—1(SET) Ц јƒ–≈—2(RESET) Ц адрес в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_MASK 0xC0
// јƒ–≈—1(SET) Ц јƒ–≈—2(RESET) Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_MASK 0xFF
// ƒ¬« Ц дискретность времени задержки: 0 -100 мсек,1 Ц ед. сек.,2 Ц дес. сек.
#define DELAY_TIME_DISCRETENESS_FBL_MASK 0x03
// ”¬« Ц значение зависит от параметра “”—“:
// - при “”—“ = 0: ”¬« Ц уставка времени задержки, выраженна€ в единицах ƒ¬« (0-120)
// 0,1х120=12 —, 1х120=120 —(2,0 мин), 10х120=1200 — (20,0 мин)
// - при “”—“ = 1: - не имеет значени€.
#define DELAY_TIME_SETPOINT_FBL_MASK 0xFF
// јƒ–≈— ”¬« Ц значение зависит от параметра “”—“:
// - при “”—“ = 1: адрес уставки времени задержки, расположенной в области хранени€ измен€емых уставок (1 Ц 10). ‘ормат уставки тот же, что и ”¬«. ¬сего может быть 10 измен€емых уставок, к которым может адресоватьс€ любой таймер.
// - при “”—“ = 0: - не имеет значени€.
#define DELAY_TIME_SETPOINT_FBL_ADDRESS_MASK 0x0F
// “”—“ Ц тип уставки времени задержки
// 0 Ц фиксированное значение уставки
// 1 Ц измен€емое значение уставки
#define DELAY_TIME_SETPOINT_FBL_TYPE_MASK 0x10

// данные элементов в массиве - aucInputAddress[8], структуры - struct TFunctionBlockDescriptionWork.
#define TIMER_TYPE_FBL(pointer) (((((pointer) -> ucFunctionBlockType) & TIMER_TYPE_FBL_MASK)) >> 4)

// »¬х—(SET) Ц источник входного дискретного сигнала.
#define INPUT_SOURCE_FBL_SET(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_SET_INDEX]) & INPUT_SOURCE_FBL_MASK)
// »¬х—(RESET) Ц источник входного дискретного сигнала.
#define INPUT_SOURCE_FBL_RESET(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_RESET_INDEX]) & INPUT_SOURCE_FBL_MASK)

// “¬х(SET) Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define INPUT_TYPE_FBL_SET(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_SET_INDEX]) & INPUT_TYPE_FBL_MASK)
// “¬х(RESET) Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define INPUT_TYPE_FBL_RESET(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_RESET_INDEX]) & INPUT_TYPE_FBL_MASK)

// јƒ–≈—1(SET) Ц в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_SET(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_SET_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)
// јƒ–≈—2(RESET) Ц в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_RESET(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_RESET_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)

// јƒ–≈—1(SET) Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_SET(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_SET_INDEX])
// јƒ–≈—2(RESET) Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_RESET(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_RESET_INDEX])

// ƒ¬« Ц дискретность времени задержки: 0 -100 мсек,1 Ц ед. сек.,2 Ц дес. сек.
#define DELAY_TIME_DISCRETENESS_FBL(pointer) ((pointer -> aucInputAddress[DELAY_TIME_DISCRETENESS_FBL_INDEX]) & DELAY_TIME_DISCRETENESS_FBL_MASK)
// ”¬« Ц уставка времени задержки.
#define DELAY_TIME_SETPOINT_FBL(pointer) ((pointer) -> aucInputAddress[DELAY_TIME_SETPOINT_FBL_INDEX])
// јƒ–≈— ”¬« Ц адрес уставки времени задержки.
#define DELAY_TIME_SETPOINT_FBL_ADDRESS(pointer) (((pointer) -> aucInputAddress[DELAY_TIME_SETPOINT_FBL_ADDRESS_INDEX]) & DELAY_TIME_SETPOINT_FBL_ADDRESS_MASK)
// “”—“ Ц тип уставки времени задержки.
#define DELAY_TIME_SETPOINT_FBL_TYPE_INDEX 6#define DELAY_TIME_SETPOINT_FBL_TYPE(pointer) (((pointer) -> aucInputAddress[DELAY_TIME_SETPOINT_FBL_TYPE_INDEX]) & DELAY_TIME_SETPOINT_FBL_TYPE_MASK)

//-----------------------------------------------------------------------------------------------------
// данные описател€ функционального блока - Ђ“риггерї.
// индексы в массиве - aucInputAddress[8], структуры - struct TFunctionBlockDescriptionWork.
// »¬х—(SET1) Ц источник входного дискретного сигнала.
#define INPUT_SOURCE_FBL_SET_1_INDEX 0
// »¬х—(SET2) Ц источник входного дискретного сигнала.
#define INPUT_SOURCE_FBL_SET_2_INDEX 2
// »¬х—(RESET1) Ц источник входного дискретного сигнала.
#define INPUT_SOURCE_FBL_RESET_1_INDEX 4
// »¬х—(RESET2) Ц источник входного дискретного сигнала.
#define INPUT_SOURCE_FBL_RESET_2_INDEX 6

// “¬х(SET1) Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define INPUT_TYPE_FBL_SET_1_INDEX 0
// “¬х(SET2) Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define INPUT_TYPE_FBL_SET_2_INDEX 2
// “¬х(RESET1) Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define INPUT_TYPE_FBL_RESET_1_INDEX 4
// “¬х(RESET2) Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define INPUT_TYPE_FBL_RESET_2_INDEX 6

// јƒ–≈—1(SET1) Ц адрес в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_SET_1_INDEX 0
// јƒ–≈—2(SET1) Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_SET_1_INDEX 1
// јƒ–≈—1(SET2) Ц адрес в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_SET_2_INDEX 2
// јƒ–≈—2(SET2) Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_SET_2_INDEX 3

// јƒ–≈—1(RESET1) Ц адрес в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_RESET_1_INDEX 4
// јƒ–≈—2(RESET1) Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_RESET_1_INDEX 5
// јƒ–≈—1(RESET2) Ц адрес в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_RESET_2_INDEX 6
// јƒ–≈—2(RESET2) Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_RESET_2_INDEX 7

// маски элементов в массиве - aucInputAddress[8], структуры - struct TFunctionBlockDescriptionWork.
#define TRIGGER_TYPE_FBL_MASK 0xF0
// »¬х—1 Ц »¬х—4 Ц источник входного дискретного сигнала:
// 0 Ц входной сигнал ”—ќ (битовый массив TSOST, адрес 1-192);
// 1 Ц выход ‘ЅЋ (адрес 1-120) или команда управлени€ с верхнего уровн€  ¬” (адрес 121-184);
// 2 Ц флаг недостоверности входного аналогового сигнала (адрес 1-96);
// 3 Ц дискретный выход алгоритма CoDeSys (адрес 1-112 только дл€ дл€ ѕј—05-16 CDS)
// 4 Ц флаг недостоверности входного дискретного сигнала (адрес 1-192);
#define INPUT_SOURCE_FBL_MASK 0x07
// “¬х Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define INPUT_TYPE_FBL_MASK 0x08
// јƒ–≈—1(SET1), јƒ–≈—2(SET2), јƒ–≈—3(RESET1), јƒ–≈—4(RESET2) Ц адрес в массиве - источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_MASK 0xC0
// јƒ–≈—1(SET1), јƒ–≈—2(SET2), јƒ–≈—3(RESET1), јƒ–≈—4(RESET2) Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_MASK 0xFF

// данные элементов в массиве - aucInputAddress[8], структуры - struct TFunctionBlockDescriptionWork.
#define TRIGGER_TYPE_FBL(pointer) (((((pointer) -> ucFunctionBlockType) & TRIGGER_TYPE_FBL_MASK)) >> 4)

// »¬х—(SET1) Ц источник входного дискретного сигнала.
#define INPUT_SOURCE_FBL_SET_1(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_SET_1_INDEX]) & INPUT_SOURCE_FBL_MASK)
// »¬х—(SET2) Ц источник входного дискретного сигнала.
#define INPUT_SOURCE_FBL_SET_2(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_SET_2_INDEX]) & INPUT_SOURCE_FBL_MASK)
// »¬х—(RESET1) Ц источник входного дискретного сигнала.
#define INPUT_SOURCE_FBL_RESET_1(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_RESET_1_INDEX]) & INPUT_SOURCE_FBL_MASK)
// »¬х—(RESET2) Ц источник входного дискретного сигнала.
#define INPUT_SOURCE_FBL_RESET_2(pointer) (((pointer) -> aucInputAddress[INPUT_SOURCE_FBL_RESET_2_INDEX]) & INPUT_SOURCE_FBL_MASK)

// “¬х(SET1) Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define INPUT_TYPE_FBL_SET_1(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_SET_1_INDEX]) & INPUT_TYPE_FBL_MASK)
// “¬х(SET2) Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define INPUT_TYPE_FBL_SET_2(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_SET_2_INDEX]) & INPUT_TYPE_FBL_MASK)
// “¬х(RESET1) Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define INPUT_TYPE_FBL_RESET_1(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_RESET_1_INDEX]) & INPUT_TYPE_FBL_MASK)
// “¬х(RESET2) Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define INPUT_TYPE_FBL_RESET_2(pointer) (((pointer) -> aucInputAddress[INPUT_TYPE_FBL_RESET_2_INDEX]) & INPUT_TYPE_FBL_MASK)

// јƒ–≈—1(SET1) Ц адрес в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_SET_1(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_SET_1_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)
// јƒ–≈—2(SET2) Ц адрес в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_SET_2(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_SET_2_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)
// јƒ–≈—3(RESET1) Ц адрес в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_RESET_1(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_RESET_1_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)
// јƒ–≈—4(RESET2) Ц адрес в массиве-источнике входного дискретного сигнала - старший байт;
#define ADDRESS_HIGH_FBL_RESET_2(pointer) (((((pointer) -> aucInputAddress[ADDRESS_HIGH_FBL_RESET_2_INDEX]) & ADDRESS_HIGH_FBL_MASK)) >> 6)

// јƒ–≈—1(SET1) Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_SET_1(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_SET_1_INDEX])
// јƒ–≈—2(SET2) Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_SET_2(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_SET_2_INDEX])
// јƒ–≈—3(RESET1) Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_RESET_1(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_RESET_1_INDEX])
// јƒ–≈—4(RESET2) Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define ADDRESS_LOW_FBL_RESET_2(pointer) ((pointer) -> aucInputAddress[ADDRESS_LOW_FBL_RESET_2_INDEX])

//-----------------------------------------------------------------------------------------------------
// данные описател€ функционального блока - Ђјналоговый компараторї.
// индексы в массиве - aucInputAddress[8], структуры - struct TFunctionBlockDescriptionWork.
// »¬х—ј - источник входного аналогового сигнала.
#define ANALOGUE_INPUT_FBL_ADDRESS_INDEX 0
// тип входного аналогового сигнала:
#define ANALOGUE_INPUT_FBL_TYPE_INDEX 0

// ”ставка FLOAT Ц ‘иксированное значение уставки при јƒ–_”—“ = 0, при јƒ–_”—“ = 1 Ц 10 не имеет значени€;
// ”ставка FLOAT (мантисса младший байт) при јƒ–_”—“ = 0.
#define SETPOINT_FBL_BYTE_1_INDEX 1
// ”ставка FLOAT (мантисса средний байт) при јƒ–_”—“ = 0.
#define SETPOINT_FBL_BYTE_2_INDEX 2
// ”ставка FLOAT (мантисса старший байт) при јƒ–_”—“ = 0.
#define SETPOINT_FBL_BYTE_3_INDEX 3
// ”ставка FLOAT (пор€док) при јƒ–_”—“ = 0.
#define SETPOINT_FBL_BYTE_4_INDEX 4

// √—“ Ц гистерезис 0 Ц 31 % диапазона измерени€ (шкалы) аналогового сигнала.
#define HYSTERESIS_ANALOGUE_SCALE_RANGE_FBL_INDEX 5
// cond Ц условие формировани€ выхода.
#define COMPARISON_CONDITION_FBL_INDEX 5

// дискретный сигнал "ENABLE".
// »¬х— Ц источник входного дискретного сигнала:
#define DISCRETE_INPUT_ENABLE_SOURCE_FBL_INDEX 6
// “¬х Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define DISCRETE_INPUT_ENABLE_TYPE_FBL_INDEX 6
// јƒ–≈— Ц адрес в массиве-источнике входного дискретного сигнала - старший байт;
#define DISCRETE_INPUT_ENABLE_ADDRESS_HIGH_FBL_INDEX 6
// јƒ–≈— Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define DISCRETE_INPUT_ENABLE_ADDRESS_LOW_FBL_INDEX 7

// маски элементов в массиве - aucInputAddress[8], структуры - struct TFunctionBlockDescriptionWork.
// јƒ–_”—“ Ц адрес уставки сравнени€ аналогового сигнала (0, 1-10):
// - 0 Ц фиксированное значение уставки (уставка находитс€ в байтах Ђ”ставка FLOATї)
// - 1 - 10 Ц измен€емое значение уставки (уставка находитс€ в области хранени€ уставок компараторов по адресу 1-10);
#define SETPOINT_ADDRESS_FBL_MASK 0xF0
// »¬х—ј - источник входного аналогового сигнала.
#define ANALOGUE_INPUT_FBL_ADDRESS_MASK 0x7F
// тип входного аналогового сигнала:
// 0 Ц входной аналоговый сигнал Ц измер€ема€ величина (1-96);
// 1 Ц расчетна€ величина (1-24).
#define ANALOGUE_INPUT_FBL_TYPE_FBL_MASK 0x80

// ”ставка FLOAT Ц ‘иксированное значение уставки при јƒ–_”—“ = 0, при јƒ–_”—“ = 1 Ц 10 не имеет значени€;
// ”ставка FLOAT (мантисса LSB) при јƒ–_”—“ = 0.
#define SETPOINT_FBL_BYTE_1_MASK 0xFF
// ”ставка FLOAT (мантисса MSB) при јƒ–_”—“ = 0.
#define SETPOINT_FBL_BYTE_2_MASK 0xFF
// ”ставка FLOAT (пор€док) при јƒ–_”—“ = 0.
#define SETPOINT_FBL_BYTE_3_MASK 0xFF

// √—“ Ц гистерезис 0 Ц 31 % диапазона измерени€ (шкалы) аналогового сигнала
// (имеет значение только дл€ »¬х—ј, бит 7 = 0, если бит 7 = 1 то √—“ = 0)
#define HYSTERESIS_ANALOGUE_SCALE_RANGE_FBL_MASK 0x7C
// cond Ц условие формировани€ выхода:
// ѕри cond = 0 выход = 1, если аналоговый сигнал ћ≈Ќ№Ў≈ ”—“ј¬ »
// ѕри cond = 1 выход = 1, если аналоговый сигнал ЅќЋ№Ў≈ ”—“ј¬ »
#define COMPARISON_CONDITION_FBL_MASK 0x80
// дискретный сигнал "ENABLE".
// »¬х— Ц источник входного дискретного сигнала:
// 0 Ц входной сигнал ”—ќ (битовый массив TSOST, адрес 1-192);
// 1 Ц выход ‘ЅЋ (адрес 1-120) или команда управлени€ с верхнего уровн€  ¬” (адрес 121-184);
// 2 Ц флаг недостоверности входного аналогового сигнала (адрес 1-96);
// 3 Ц дискретный выход алгоритма CoDeSys (адрес 1-112 только дл€ дл€ ѕј—05-16 CDS)
// 4 Ц флаг недостоверности входного дискретного сигнала (адрес 1-192);
#define DISCRETE_INPUT_ENABLE_SOURCE_FBL_MASK 0x07
// “¬х Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define DISCRETE_INPUT_ENABLE_TYPE_FBL_MASK 0x08
// јƒ–≈— Ц адрес в массиве-источнике входного дискретного сигнала - старший байт;
#define DISCRETE_INPUT_ENABLE_ADDRESS_HIGH_FBL_MASK 0xC0
// јƒ–≈— Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define DISCRETE_INPUT_ENABLE_ADDRESS_LOW_FBL_MASK 0xFF

// данные элементов в массиве - aucInputAddress[8], структуры - struct TFunctionBlockDescriptionWork.
// »¬х—ј - источник входного аналогового сигнала.
#define ANALOGUE_INPUT_FBL_ADDRESS(pointer) (((pointer) -> aucInputAddress[ANALOGUE_INPUT_FBL_ADDRESS_INDEX]) & ANALOGUE_INPUT_FBL_ADDRESS_MASK)
// тип входного аналогового сигнала:
#define ANALOGUE_INPUT_FBL_TYPE_FBL(pointer) (((pointer) -> aucInputAddress[ANALOGUE_INPUT_FBL_TYPE_INDEX]) & ANALOGUE_INPUT_FBL_TYPE_FBL_MASK)

// ”ставка FLOAT (мантисса младший байт) при јƒ–_”—“ = 0.
#define SETPOINT_FBL_BYTE_1(pointer) ((pointer) -> aucInputAddress[SETPOINT_FBL_BYTE_1_INDEX])
// ”ставка FLOAT (мантисса средний байт) при јƒ–_”—“ = 0.
#define SETPOINT_FBL_BYTE_2(pointer) ((pointer) -> aucInputAddress[SETPOINT_FBL_BYTE_2_INDEX])
// ”ставка FLOAT (мантисса старший байт) при јƒ–_”—“ = 0.
#define SETPOINT_FBL_BYTE_3(pointer) ((pointer) -> aucInputAddress[SETPOINT_FBL_BYTE_3_INDEX])
// ”ставка FLOAT (пор€док) при јƒ–_”—“ = 0.
#define SETPOINT_FBL_BYTE_4(pointer) ((pointer) -> aucInputAddress[SETPOINT_FBL_BYTE_4_INDEX])
// ”ставка FLOAT Ц ‘иксированное значение уставки при јƒ–_”—“ = 0, при јƒ–_”—“ = 1 Ц 10 не имеет значени€;
#define SETPOINT_FBL(pointer) ((pointer) -> aucInputAddress[SETPOINT_FBL_BYTE_1_INDEX])

// √—“ Ц гистерезис 0 Ц 31 % диапазона измерени€ (шкалы) аналогового сигнала.
#define HYSTERESIS_ANALOGUE_SCALE_RANGE_FBL(pointer) (((((pointer) -> aucInputAddress[HYSTERESIS_ANALOGUE_SCALE_RANGE_FBL_INDEX]) & HYSTERESIS_ANALOGUE_SCALE_RANGE_FBL_MASK)) >> 2)
// cond Ц условие формировани€ выхода.
#define COMPARISON_CONDITION_FBL(pointer) (((pointer) -> aucInputAddress[COMPARISON_CONDITION_FBL_INDEX]) & COMPARISON_CONDITION_FBL_MASK)

// дискретный сигнал "ENABLE".
// »¬х— Ц источник входного дискретного сигнала:
#define DISCRETE_INPUT_ENABLE_SOURCE_FBL(pointer) (((pointer) -> aucInputAddress[DISCRETE_INPUT_ENABLE_SOURCE_FBL_INDEX]) & DISCRETE_INPUT_ENABLE_SOURCE_FBL_MASK)
// “¬х Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
#define DISCRETE_INPUT_ENABLE_TYPE_FBL(pointer) (((pointer) -> aucInputAddress[DISCRETE_INPUT_ENABLE_TYPE_FBL_INDEX]) & DISCRETE_INPUT_ENABLE_TYPE_FBL_MASK)
// јƒ–≈— Ц адрес в массиве-источнике входного дискретного сигнала - старший байт;
#define DISCRETE_INPUT_ENABLE_ADDRESS_HIGH_FBL(pointer) (((((pointer) -> aucInputAddress[DISCRETE_INPUT_ENABLE_ADDRESS_HIGH_FBL_INDEX]) & DISCRETE_INPUT_ENABLE_ADDRESS_HIGH_FBL_MASK)) >> 6)
// јƒ–≈— Ц адрес в массиве-источнике входного дискретного сигнала - младший байт;
#define DISCRETE_INPUT_ENABLE_ADDRESS_LOW_FBL(pointer) ((pointer) -> aucInputAddress[DISCRETE_INPUT_ENABLE_ADDRESS_LOW_FBL_INDEX])

// структура вспомогательных данных формировани€ требований отображени€ дискретных сигналов на HMI.
struct TFunctionBlockWorkingData
{
    // флаг активности функциональногоблока.
    bool fbFunctionBlockIsActive;
    // счЄтчик дискретного времени задержки ƒ¬«.
    unsigned char ucDelay;
};


#endif // FUNCTIONBLOCKS_H_INCLUDED
