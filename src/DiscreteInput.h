#ifndef DISCRETEINPUT_H_INCLUDED
#define DISCRETEINPUT_H_INCLUDED

#include "PasNewConfig.h"

#define DISCRETE_INPUT_RELAY_OUTPUT_QUANTITY DO_VALUE_BIT_ARRAY_LENGTH
// норма.
#define FLOW_CONTROL_NORMA 1
// новое нарушение.
#define FLOW_CONTROL_NEW_VIOLATION 2
// не новое нарушение.
#define FLOW_CONTROL_NOT_NEW_VIOLATION 3


// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// Структура описателей БД дискретных входных сигналов.
// источник: PAS_05_PO.DOC, стр - 3,
// 3. БАЗА ДАННЫХ ОБРАБОТКИ ДИСКРЕТНЫХ СИГНАЛОВ
struct TDiscreteInputDescriptionWorkPackOne
{
    // Байт 1:
    // D0-D4 - ТБЛ (группа): 0,1-24(0-нет вых.на табло), D5-ТК (тип контакта): 0-НЗ,1-НР.
    // D6 - ТСГ1 – тип сигнализации состояния дискретного сигнала.
    // D7 - ТСГ2 – тип сигнализации состояния дискретного сигнала.
    unsigned char 	ucTalTkGrp;
    // Байт 2:
    // Выходы на МР1:D0=1 есть вых.на реле 1,D0=0-нет вых на реле 1 и т.д.
    // Байт 3,4,5,6,7,8,9:
    // Выходы на МР2,МР3,МР4,МР5,MP6,MP7,MP8:аналог.МР1.
    unsigned char aucRelayOut[8];
    // Байт 10:
    // D7 - резерв, D6 - ДВЗ:0-задерж.в сек,1-в дес.сек.
    // D0-D5 - УВЗ:уставка времени задержки выхода в ед.ДВЗ(0-60).
    unsigned char ucDelay;
    // Байт 11 - контрольная сумма ($ff+сумма байтов 1-10)    *).
    unsigned char ucCrc;
};
#pragma pack(pop)

// Структура описателей БД дискретных входных сигналов.
// источник: PAS_05_PO.DOC, стр - 3,
// 3. БАЗА ДАННЫХ ОБРАБОТКИ ДИСКРЕТНЫХ СИГНАЛОВ
struct TDiscreteInputDescriptionWork
{
    // Байт 1:
    // D0-D4 - ТБЛ (группа): 0,1-24(0-нет вых.на табло), D5-ТК (тип контакта): 0-НЗ,1-НР.
    // D6 - ТСГ1 – тип сигнализации состояния дискретного сигнала.
    // D7 - ТСГ2 – тип сигнализации состояния дискретного сигнала.
    unsigned char 	ucTalTkGrp;
    // Байт 2:
    // Выходы на МР1:D0=1 есть вых.на реле 1,D0=0-нет вых на реле 1 и т.д.
    // Байт 3,4,5,6,7,8,9:
    // Выходы на МР2,МР3,МР4,МР5,MP6,MP7,MP8:аналог.МР1.
    unsigned char aucRelayOut[8];
    // Байт 10:
    // D7 - резерв, D6 - ДВЗ:0-задерж.в сек,1-в дес.сек.
    // D0-D5 - УВЗ:уставка времени задержки выхода в ед.ДВЗ(0-60).
    unsigned char ucDelay;
    // Байт 11 - контрольная сумма ($ff+сумма байтов 1-10)    *).
    unsigned char ucCrc;
};

// маски злементов структуры описателей БД дискретных входных сигналов.
// struct TDiscreteInputDescriptionWork
// источник: PAS_05_PO.DOC, стр - 3,
// 3. БАЗА ДАННЫХ ОБРАБОТКИ ДИСКРЕТНЫХ СИГНАЛОВ
// Байт 1: ucTalTkGrp.
// D0-D4 - ТБЛ (группа), номер ячейки на световом табло.
#define LIGHT_BOARD_CELL_NUMBER_DISC_MASK 0x1F
// D5-ТК (тип контакта): 0-НЗ,1-НР
#define CONTACT_TYPE_DISC_MASK 0x20
// D6,D7 - ТСГ тип сигнализации: 0-нет сигн.,1-пред.,2-авар.,3-индик.(ровн.свет)
// ТСГ1 – тип сигнализации состояния дискретного сигнала
#define SIGNALING_TYPE_1_DISC_MASK 0x40
// ТСГ2 – тип сигнализации состояния дискретного сигнала
#define SIGNALING_TYPE_2_DISC_MASK 0x80
// Байт 10: ucDelay.
// D0-D5 - УВЗ: уставка времени задержки выхода в ед.ДВЗ(0-60)
#define DELAY_TIME_SETPOINT_DISC_MASK 0x3F
// D6 - ДВЗ дискретность времени задержки: 0-задерж.в сек,1-в дес.сек.
#define DELAY_TIME_DISCRETENESS_DISC_MASK 0x40
// D7 - АРХ =1 архивировать, 0 – не архивировать.
#define ARCHIVE_DISC_MASK 0x80

// ТБЛ (группа), номер ячейки на световом табло.
#define LIGHT_BOARD_CELL_NUMBER_DIS(pointer) (((pointer) -> ucTalTkGrp) & LIGHT_BOARD_CELL_NUMBER_DISC_MASK)
// ТК (тип контакта): 0-НЗ,1-НР
#define CONTACT_TYPE_DIS(pointer) (((pointer) -> ucTalTkGrp) & CONTACT_TYPE_DISC_MASK)
// ТСГ тип сигнализации: 0-нет сигн.,1-пред.,2-авар.,3-индик.(ровн.свет)
// ТСГ1 – тип сигнализации состояния дискретного сигнала
#define SIGNALING_TYPE_1_DIS(pointer) (((pointer) -> ucTalTkGrp) & SIGNALING_TYPE_1_DISC_MASK)
// ТСГ2 – тип сигнализации состояния дискретного сигнала
#define SIGNALING_TYPE_2_DIS(pointer) (((pointer) -> ucTalTkGrp) & SIGNALING_TYPE_2_DISC_MASK)
// УВЗ: уставка времени задержки выхода в ед.ДВЗ(0-60)
#define DELAY_TIME_SETPOINT_DIS(pointer) (((pointer) -> ucDelay) & DELAY_TIME_SETPOINT_DISC_MASK)
// ДВЗ дискретность времени задержки: 0-задерж.в сек,1-в дес.сек.
#define DELAY_TIME_DISCRETENESS_DIS(pointer) (((pointer) -> ucDelay) & DELAY_TIME_DISCRETENESS_DISC_MASK)
// архив.
#define ARCHIVE_DIS(pointer) ((pointer) -> ucDelay) & ARCHIVE_DISC_MASK

// контрольная сумма.
#define CRC_DIS(pointer) ((pointer) -> ucCrc)

// структура вспомогательных данных формирования требований отображения дискретных сигналов на HMI.
struct TDiscreteInputWorkingData
{
    // флаг активности дискретного входа.
    bool fbDiscreteInputIsActive;
    // счётчик дискретного времени задержки ДВЗ.
    unsigned char ucDelay;
};

#endif // DISCRETEINPUT_H_INCLUDED
