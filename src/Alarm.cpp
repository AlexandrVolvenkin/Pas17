
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <typeinfo>
#include <stdint.h>
#include <iostream>
#include <stdio.h>

//#include "PasNewConfig.h"
//#include "FunctionBlocks.h"
#include "Alarm.h"
#include "Events.h"
#include "EventsDB.h"
//#include "SystemComponent.h"

using namespace std;

////-----------------------------------------------------------------------------------------------------
//uint8_t CAlarm::ui8AlarmEventLogBriefIsUpdated;
//const  CAlarm::TAlarmEvent CAlarm::xAlarmEventNull =
//{
//	0,
//	0,
//	0,
//	0
//};
//uint8_t CAlarm::ui8AlarmEventLogLength;
//uint8_t CAlarm::ui8AlarmEventLogBriefLength;
//CAlarm::TAlarmEventBrief CAlarm::axAlarmEventsLogBrief[];
//CAlarm::TAlarmEventLogQueryListPackOne CAlarm::xAlarmEventsQueryList;
//int16_t CAlarm::i16AlarmEventQueryListQuantity;
//CAlarm::TAlarmEvent CAlarm::axAlarmEventsLog[];
//CAlarm::TAlarmEventsLogControl CAlarm::xAlarmEventsLogControl;
//
////-----------------------------------------------------------------------------------------------------
//// создаёт события: время включения и выключения.
//void CAlarm::PlcOnOffEvetnsCreate(void)
//{
//	CEvents::TEventDataCommon *pxEventData;
//
//	// получим указатель на место в кольцевом буфере журнала событий.
//	pxEventData = xCAlarmEvent.EventDataPush();
//// сохраним данные события.
//	// установим id события.
//	(pxEventData -> ui16ID) =
//		1;
//	// установим тип события.
//	(pxEventData -> ui8Type) =
//		CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE;
//	// установим адрес источника события.
//	(pxEventData -> ui16Address) = 0;
//	// установим код события.
//	(pxEventData -> ui8State) =
//		0;
//
//	// получим последнее сохранённое время в FRAM.
//	iFramRead((uint8_t*)&xCurrentTime,
//			  FRAM_LAST_SAVED_TIME_OFFSET,
//			  sizeof(xCurrentTime));
//
//	// установим время события.
//	(pxEventData -> xCurrentTime) =
//		xCurrentTime;
//	// создадим строку текстового реквизита.
//	sprintf((char*)(pxEventData -> acTextDescriptor),
//			"%s",
//			"Окончание работы");
//	// создадим пустую строку в поле дополнительных текстовых данных таблицы архива в базе данных.
//	sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
//			"%s",
//			" ");
//	xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//
//	// модуль индикации МИНД последовательно запрашивает данные событий из кольцевого буфера.
//	// когда МИНД получает данные события с маркером - нет события, он перестаёт посылать
//	// запросы. на дисплее появляется информация о том, что событий больше нет.
//	// чтобы сообщить модулю индикации МИНД о том, что событий больше нет - вытесним из
//	// кольцевого буфера самое давнее событие. пометим его маркером - нет события(ui16ID = 0).
//	// установим индекс (ui16EventDataCommonPopIndex) на место следующего в кольцевом буфере самого давнего события.
//	xCAlarmEvent.EventDataPopIndexSet(0);
//	// получим указатель на место следующего в кольцевом буфере самого давнего события.
//	pxEventData = xCAlarmEvent.EventDataPop();
//	// пометим событие маркером - нет события.
//	pxEventData -> ui16ID = 0;
//
//
//	time_t xLocalLastTime;
//	uint32_t ui32Seconds;
//
//	// преобразуем в целочисленный формат последнее сохранённое время в FRAM.
//	xLocalLastTime = mktime(&xCurrentTime);
//	// обновим текущее время в переменную xCurrentTime.
//	vCurrentTimeUpdate();
//	// получим разницу в секундах между временем выключения
//	// и временем включения.
//	ui32Seconds = (uint32_t)difftime(mktime(&xCurrentTime), xLocalLastTime);
//
//	// предыдущее выключение было больше чем 1 минуту назад?
//	if (ui32Seconds > 60)
//	{
//		// найдём модули МТВИ5, чтобы отправить им команду - перезагрузка больше чем через одну минуту.
//		// выставить OSF и перейти в режим «РУЧНОЙ».
//		// пройдём по всем имеющимся в конфигурации модулям.
//		for (int i = 0; i < (xPlcConfigService.
//							 xPlcConfigServiceData.
//							 ucInternalModulesQuantity); i++)
//		{
//			// найден модуль МТВИ5?
//			if (xAllModulesContext.axAllModulesContext[i].
//					xModuleContextStatic.ucModuleType == MODULE_TYPE_MTVI5)
//			{
//				// отправим команду модулю МТВИ5 - перезагрузка больше чем через одну минуту.
//				// выставить OSF и перейти в режим «РУЧНОЙ».
//				xAllModulesContext.axAllModulesContext[i].
//				xModuleContextDinamic.
//				ucCommandControl = DEVICE_CONTROL_MTVI5_LOAD_OUTS_OSF;
//			}
//		}
//	}
//
//	// получим указатель на место в кольцевом буфере журнала событий.
//	pxEventData = xCAlarmEvent.EventDataPush();
//
//	// сохраним данные события.
//	// установим id события.
//	(pxEventData -> ui16ID) =
//		1;
//	// установим тип события.
//	(pxEventData -> ui8Type) =
//		CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE;
//	// установим адрес источника события.
//	(pxEventData -> ui16Address) = 0;
//	// установим код события.
//	(pxEventData -> ui8State) =
//		0;
//
//	// установим время события.
//	(pxEventData -> xCurrentTime) =
//		xCurrentTime;
//	// создадим строку текстового реквизита.
//	sprintf((char*)(pxEventData -> acTextDescriptor),
//			"%s",
//			"Начало работы");
//	// создадим пустую строку в поле дополнительных текстовых данных таблицы архива в базе данных.
//	sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
//			"%s",
//			" ");
//	xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//
//	// модуль индикации МИНД последовательно запрашивает данные событий из кольцевого буфера.
//	// когда МИНД получает данные события с маркером - нет события, он перестаёт посылать
//	// запросы. на дисплее появляется информация о том, что событий больше нет.
//	// чтобы сообщить модулю индикации МИНД о том, что событий больше нет - вытесним из
//	// кольцевого буфера самое давнее событие. пометим его маркером - нет события(ui16ID = 0).
//	// установим индекс (ui16EventDataCommonPopIndex) на место следующего в кольцевом буфере самого давнего события.
//	xCAlarmEvent.EventDataPopIndexSet(0);
//	// получим указатель на место следующего в кольцевом буфере самого давнего события.
//	pxEventData = xCAlarmEvent.EventDataPop();
//	// пометим событие маркером - нет события.
//	pxEventData -> ui16ID = 0;
//}
//
//
////-----------------------------------------------------------------------------------------------------
//// функция событий сигнализации дискретных сигналов и функциональных блоков.
//// если есть сигнализация, то создаёт событие и помещает запись в журнал событий "кратко".
//void CAlarm::AlarmEvetnsLogCreate(void)
//{
//	TAlarmHmi *pxAlarmHmi;
//	uint8_t ui8EventIndex;
//	TDiscreteSygnalTextDescriptor *pxDiscreteSygnalTextDescriptor;
//
////    // журнал событий "кратко" не обновлён?
////    if (!(CAlarm::ui8AlarmEventLogBriefIsUpdated))
////    {
//	// получим указатель на буфер с нормализованной базой данных прибора.
//	pxDiscreteSygnalTextDescriptor = axDiscreteSygnalTextDescriptor;
//	// получим указатель на рабочий массив формирования требований отображения сигнализации дискретных сигналов на HMI.
//	pxAlarmHmi = &axAlarmHmi[0];
//
////    // создадим события сработавших сигнализаций, если запрограммированы.
////    for (int i = 0;
////            i < ((xPlcConfigService.xPlcConfigServiceData.
////                  uiHandledDiscreteInputQuantity) +
////                 (xPlcConfigService.xPlcConfigServiceData.
////                  uiHandledFunctionBlockQuantity));
////            i++, pxAlarmHmi++)
////    {
////        // сигнализация по текущему обрабатываеммому дискретному сигналу
////        // или функциональному блоку активна?
////        if (pxAlarmHmi -> ui8AlarmState)
////        {
////            CEvents::TEventDataCommon *pxEventData;
////            // событие новое?
////            pxEventData = xCAlarmEvent.EventDataPushRequest(i);
////            // есть возможность зафиксировать событие?
////            if (pxEventData)
////            {
////                // сигнализация аварийная?
////                if ((pxAlarmHmi -> ui8AlarmState) & (0x01 << ALARM_TYPE_ALARM))
////                {
////                    fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ALARM;
////                }
////                // сигнализация предупредительная?
////                else if ((pxAlarmHmi -> ui8AlarmState) & (0x01 << ALARM_TYPE_PREVENT))
////                    // предупредительная сигнализация.
////                {
////                    fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_WARNING;
////                }
////                else
////                {
////                    fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ALARM;
////                }
////
////                printf("AlarmEvetnsLogCreate: i %d\n\r", (i));
////                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
////                // если есть возможность зафиксировать событие,
////                // то по указателю(pxEventData) на место в журнале,
////                // сохраним данные события.
////                // установим id события.
////                (pxEventData -> ui16ID) =
////                    1;
////                // установим тип события.
////                (pxEventData -> ui8Type) =
////                    CEvents::HANDLED_EVENTS_ALARM_TYPE;
////                // установим адрес источника события.
////                (pxEventData -> ui16Address) = (i + CONVERT_INTEGER_TO_NATURAL_NUMBER);
////                // установим код события.
////                (pxEventData -> ui8State) =
////                    (pxAlarmHmi -> ui8AlarmState);
////                // установим время события.
////                (pxEventData -> xCurrentTime) =
////                    xCurrentTime;
////                // источник события - дискретный вход?
////                if (i < (xPlcConfigService.xPlcConfigServiceData.
////                         uiHandledDiscreteInputQuantity))
////                {
////                    // источник события - дискретный вход.
////                    // скопируем строку текстового реквизита.
////                    memcpy(&(pxEventData -> acTextDescriptor),
////                           (&(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor)),
////                           (CAlarm::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
////                }
////                else
////                {
////                    // источник события - функциональный блок.
////                    // создадим строку текстового реквизита.
////                    sprintf((char*)(pxEventData -> acTextDescriptor),
////                            "%s%d",
////                            "ФБЛ #",
////                            ((i - ((xPlcConfigService.xPlcConfigServiceData.
////                                    uiHandledDiscreteInputQuantity) -
////                                   CONVERT_NATURAL_NUMBER_TO_INTEGER)) +
////                             CONVERT_INTEGER_TO_NATURAL_NUMBER));
////                }
////
////                // архивировать?
////                if ((pxAlarmHmi -> ui8Archive))
////                {
////                    xCArchiveEventsDB.DataBaseDataPush(pxEventData);
////                }
////            }
////        }
////        else
////        {
////            // очистим признаки того, что событие было зафиксировано ранее.
////            xCAlarmEvent.EventReset(i);
////        }
////    }
//}
//
////-----------------------------------------------------------------------------------------------------
//// функция обработки сигнализации дискретных сигналов и функциональных блоков.
//void vAlarmHandler(void)
//{
//	TAlarmHmi *pxAlarmHmi;
//	unsigned char *pucGroupDinAction;
//	unsigned char *pucDinAction;
//	unsigned char *pucDoValueDestination;
//	unsigned char *pucDoValueBlockDestination;
//	unsigned char *pucBlincHmi;
//
//	pxAlarmHmi = &axAlarmHmi[0];
//	pucGroupDinAction = &aucCoilsArray[ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_OFFSET];
//	pucDinAction = &aucCoilsArray[ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_OFFSET];
//	pucDoValueDestination = &aucDoValueByteArray[0];
//	pucDoValueBlockDestination = &aucDoValueBlockByteArray[0];//&aucCoilsArray[DO_VALUE_BIT_ARRAY_OFFSET];
//	pucBlincHmi = &aucCoilsArray[HMI_COMMON_INTERMITTENT_SIGNAL_BITS_BIT_ARRAY_OFFSET];
//
//	vGeneralTimeCounter();
//
////-----------------------------------------------------------------------------------------------------
//	// управление мигающим транспарантом на HMI.
//	// фаза отсутствия свечения закончилась?
//	if ((!fbBlinkIsOn) && (!nucBlinkCounter))
//	{
//		nucBlinkCounter = THREE_SECOND_TIME;
//		// фаза свечения.
//		fbBlinkIsOn = 1;
//	}
//	else
//	{
//		// фаза свечения закончилась?
//		if ((fbBlinkIsOn) && (!nucBlinkCounter))
//		{
//			nucBlinkCounter = TWO_SECOND_TIME;
//			// фаза отсутствия свечения.
//			fbBlinkIsOn = 0;
//		}
//		else
//		{
//			nucBlinkCounter--;
//		}
//	}
//
////-----------------------------------------------------------------------------------------------------
//	// Приведение буферов выходов на реле в исходное состояние.
//	for (int i = 0;
//			i < (DISCRETE_INPUT_RELAY_OUTPUT_QUANTITY);
//			i++)
//	{
//		pucDoValueDestination[i] = 0;
//		pucDoValueBlockDestination[i] = 0;
//	}
//
////-----------------------------------------------------------------------------------------------------
//	vDiscreteInputHandler();
//	vFunctionBlockHandler();
//
////-----------------------------------------------------------------------------------------------------
////    CAlarm::AlarmEvetnsLogCreate();
//
////-----------------------------------------------------------------------------------------------------
//
//	pxAlarmHmi = &axAlarmHmi[0];
//	// установим тип общей сигнализации.
//	// пройдём по всем запрограммированным дискретным сигналам и функциональным блокам.
//	for (int i = 0;
//			i < (ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH +
//				 ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_LENGTH);
//			i++, pxAlarmHmi++)
//	{
//		// нет аварийной сигнализации?
//		if (!((ucAlarmStateCommon >> ALARM_TYPE_ALARM) & 0x01))
//		{
//			// для текущего дискретного сигнала установлено требование отображения сигнализации?
//			if (((!((pxAlarmHmi -> ui8AlarmState) & (0x01 << ALARM_TYPE_PREVENT))) &&
//					(((pxAlarmHmi -> ui8AlarmState) & (0x01 << ALARM_TYPE_ALARM)))) ||
//					((((pxAlarmHmi -> ui8AlarmState) & (0x01 << ALARM_TYPE_PREVENT))) &&
//					 (!((pxAlarmHmi -> ui8AlarmState) & (0x01 << ALARM_TYPE_ALARM)))))
//			{
//				// очистим флаги предыдущего состояния сигнализации.
//				ucAlarmStateCommon = 0;
//				// для текущего дискретного сигнала запрограммирована предупредительная сигнализация?
//				if ((pxAlarmHmi -> ui8AlarmState) & (0x01 << ALARM_TYPE_PREVENT))
//				{
//					// установим флаг предупредительной сигнализации.
//					ucAlarmStateCommon |= (0x01 << ALARM_TYPE_PREVENT);
//				}
//				else
//				{
//					// сбросим флаг предупредительной сигнализации.
//					ucAlarmStateCommon &= ~(0x01 << ALARM_TYPE_PREVENT);
//				}
//				// для текущего дискретного сигнала запрограммирована аварийная сигнализация?
//				if ((pxAlarmHmi -> ui8AlarmState) & (0x01 << ALARM_TYPE_ALARM))
//				{
//					// установим флаг аварийной сигнализации.
//					ucAlarmStateCommon |= (0x01 << ALARM_TYPE_ALARM);
//				}
//				else
//				{
//					// сбросим флаг аварийной сигнализации.
//					ucAlarmStateCommon &= ~(0x01 << ALARM_TYPE_ALARM);
//				}
//			}
//		}
//		else
//		{
//			break;
//		}
//	}
//
//	// очистим буфер - общий прерывистый сигнал нарушений для HMI. уберём сигнализацию.
//	pucBlincHmi[ALARM_TYPE_ALARM_OFFSET] = 0;
//	pucBlincHmi[ALARM_TYPE_PREVENT_OFFSET] = 0;
//
//	// есть общая сигнализация?
//	if (ucAlarmStateCommon)
//	{
//		// сигнализация квитирована?
//		if ((ucAlarmStateCommon & (0x01 << ALARM_TYPE_ALARM)) &&
//				(ucAlarmStateCommon & (0x01 << ALARM_TYPE_PREVENT)))
//		{
//			// устанновим бит аварийной сигнализации.
//			pucBlincHmi[ALARM_TYPE_ALARM_OFFSET] = 1; // аварийная сигнализация
//			// устанновим бит предупредительной сигнализации.
//			pucBlincHmi[ALARM_TYPE_PREVENT_OFFSET] = 1; // предупредительная сигнализация
//		}
//		else if (ucAlarmStateCommon & (0x01 << ALARM_TYPE_ALARM))
//		{
//			// фаза свечения?
//			if (fbBlinkIsOn)
//			{
//				// устанновим бит аварийной сигнализации.
//				pucBlincHmi[ALARM_TYPE_ALARM_OFFSET] = 1; // аварийная сигнализация
//			}
//		}
//		else if (ucAlarmStateCommon & (0x01 << ALARM_TYPE_PREVENT))
//			// предупредительная сигнализация.
//		{
//			// фаза свечения?
//			if (fbBlinkIsOn)
//			{
//				// устанновим бит предупредительной сигнализации.
//				pucBlincHmi[ALARM_TYPE_PREVENT_OFFSET] = 1; // предупредительная сигнализация
//			}
//		}
//	}
//
////-----------------------------------------------------------------------------------------------------
//	// есть общее квитирование или сброс?
//	if (fbGlobalKvit || fbGlobalReset)
//	{
//
//		pxAlarmHmi = &axAlarmHmi[0];
//		// очистим признаки общей сигнализации.
//		ucAlarmStateCommon = 0;
//		// сбросим флаг звуковой сигнализации.
//		fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_OFF;
//		// есть общее квитирование?
//		if (fbGlobalKvit && !fbGlobalReset)
//		{
//			// установим признак квитирования для всех обрабатываемых дискретных входов и функциональных блоков.
//			// квитируем все активные сигнализации.
//			for (int i = 0;
//					i < (ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH +
//						 ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_LENGTH);
//					i++, pxAlarmHmi++)
//			{
//				// сигнализация по текущему обрабатываеммому дискретному входу
//				// или функциональному блоку активна?
//				if (pxAlarmHmi -> ui8AlarmState)
//				{
//					// установим признак квитированой сигнализации текущего сигнала.
//					// установим флаги ровный свет:
//					// установим флаги предупредительной сигнализации.
//					pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
//					// установим флаг аварийной сигнализации.
//					pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
////                    axAlarmModules[i].ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
////                    // установим флаг аварийной сигнализации.
////                    axAlarmModules[i].ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
//				}
//			}
//		}
//		else
//		{
//			// установим признак нормы для всех обрабатываемых дискретных входов и функциональных блоков.
//			// сбросим все активные сигнализации.
//			for (int i = 0;
//					i < (ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH +
//						 ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_LENGTH);
//					i++, pxAlarmHmi++)
//			{
//				// сигнализация по текущему обрабатываеммому дискретному входу
//				// или функциональному блоку активна?
//				if (pxAlarmHmi -> ui8AlarmState)
//				{
//					// сбросим все активные сигнализации.
//					*pxAlarmHmi = xAlarmHmiNull;
//				}
//			}
//		}
//	}
//
////-----------------------------------------------------------------------------------------------------
//	// очистим все текущие сигнализации групп дискретных сигналов.
//	memset(pucGroupDinAction,
//		   0,
//		   (ALARM_GROUP_QUANTITY * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT));
//	// очистим все текущие сигнализации дискретных сигналов.
//	memset(pucDinAction,
//		   0,
//		   (ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH +
//			ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_LENGTH));
//
//	// установим сработавшие сигнализации в буферах сигнализаций HMI, если запрограммированы.
//	pxAlarmHmi = &axAlarmHmi[0];
//	for (int i = 0;
//			i < (ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH +
//				 ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_LENGTH);
//			i++, pxAlarmHmi++)
//	{
//		// сигнализация по текущему обрабатываеммому дискретному входу
//		// или функциональному блоку активна?
//		if (pxAlarmHmi -> ui8AlarmState)
//		{
//			// сигнализация квитирована?
//			if ((pxAlarmHmi -> ui8AlarmState & ALARM_TYPE_PREVENT) &&
//					(pxAlarmHmi -> ui8AlarmState & ALARM_TYPE_ALARM))
//
//			{
//				// установим признак квитированой сигнализации текущего сигнала.
//				pucDinAction[(i * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT)] = 1;
//				pucDinAction[(i * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT) + 1] = 1;
//
//				// текущий сигнал входит в группу?
//				if (((pxAlarmHmi -> ui8AlarmState & LIGHT_BOARD_CELL_NUMBER_DISC_MASK) > 0) &&
//						((pxAlarmHmi -> ui8AlarmState & LIGHT_BOARD_CELL_NUMBER_DISC_MASK) <= ALARM_GROUP_QUANTITY))
//				{
//					// установим признак квитированой сигнализации для групы в которую входит текущий сигнал.
//					pucGroupDinAction[(((pxAlarmHmi -> ui8AlarmState & LIGHT_BOARD_CELL_NUMBER_DISC_MASK) - 1) *
//									   ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT)] = 1;
//
//					pucGroupDinAction[(((pxAlarmHmi -> ui8AlarmState & LIGHT_BOARD_CELL_NUMBER_DISC_MASK) - 1) *
//									   ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT) + 1] = 1;
//				}
//			}
//			else
//			{
//				// установим признак активной сигнализации текущего сигнала.
//				pucDinAction[(i * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT) + 1] = 1;
//				// текущий сигнал входит в группу?
//				if (((pxAlarmHmi -> ui8AlarmState & LIGHT_BOARD_CELL_NUMBER_DISC_MASK) > 0) &&
//						((pxAlarmHmi -> ui8AlarmState & LIGHT_BOARD_CELL_NUMBER_DISC_MASK) <= ALARM_GROUP_QUANTITY))
//				{
//					// установим признак активной сигнализации для групы в которую входит текущий сигнал.
//					pucGroupDinAction[(((pxAlarmHmi -> ui8AlarmState & LIGHT_BOARD_CELL_NUMBER_DISC_MASK) - 1) *
//									   ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT) + 1] = 1;
//				}
//			}
//		}
//	}
//}

