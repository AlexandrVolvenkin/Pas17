
#include "PasNewConfig.h"
#include "PlcErrorTest.h"
#include "Alarm.h"
#include "Error.h"
#include "Task.h"
#include "nkh-lab/logger.hpp"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// сравнивает текущее состояние буфера отказов модулей с предыдущим.
// если появилась разница - установлен флаг отказа модуля, сохраняет текущий буфер в FRAM.
int iBadModuleBuffUpdate(void)
{
	// флаг отказа модуля не установлен?
	if (memcmp(&aucBadModuleCompareBuff[0],
			   &aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET],
			   BAD_MODULES_BIT_ARRAY_LENGTH) == 0)
	{
		return 0;
	}
	cout << "iBadModuleBuffUpdate" << endl;
	// зафиксируем флаг отказа модуля в буфере предыдущего шага.
	memcpy(&aucBadModuleCompareBuff[0],
		   &aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET],
		   BAD_MODULES_BIT_ARRAY_LENGTH);
	// сохраним текущий буфер отказа модулей в FRAM.
	iFramWrite(FRAM_BAD_MODULES_BIT_ARRAY_OFFSET,
			   &aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET],
			   BAD_MODULES_BIT_ARRAY_LENGTH);
	return 0;
}

//-----------------------------------------------------------------------------------------------------
// получает состояние буфера отказов модулей из FRAM.
int iBadModuleBuffRead(void)
{
	iFramRead(&aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET],
			  FRAM_BAD_MODULES_BIT_ARRAY_OFFSET,
			  BAD_MODULES_BIT_ARRAY_LENGTH);
}

//-----------------------------------------------------------------------------------------------------
// сбрасывает все флаги отказов модулей в RAM и FRAM.
int iBadModuleBuffClear(void)
{
	memset(&aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET],
		   0,
		   BAD_MODULES_BIT_ARRAY_LENGTH);
	iFramWrite(FRAM_BAD_MODULES_BIT_ARRAY_OFFSET,
			   &aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET],
			   BAD_MODULES_BIT_ARRAY_LENGTH);
}

//-----------------------------------------------------------------------------------------------------
// проверка ошибок прибора.
void vBadModulesTest(void)
{
	// рабочие массивы повреждены?
	xCCrc.i8Error = xCCrc.CheckAll();
	if (xCCrc.i8Error != 0)
	{
//        // ошибка.
//        // сигнализация.
//        // установим флаг звуковой сигнализации.
//        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
		// перезагрузка.
		// установим флаг "перезагрузка программы".
//        fbPlcErrorReset = 1;
		printf("xCCrc.CheckAll() error %d\n\r", (xCCrc.i8Error));
		LOG_DBG << "xCCrc.CheckAll() Error!" << (xCCrc.i8Error);

		// активное состояние события ещё не зарегистрировано?
		if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
					SYSTEM_ERROR_GROUP_OFFSET,
					SYSTEM_ERROR_CRC_ERROR_OFFSET))
		{
			// зарегистрируем активное состояние события.
			CEvents::EventRegistration(
				SYSTEM_ERROR_GROUP_OFFSET,
				(CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
				 CEvents::HANDLED_EVENTS_IS_POPUP |
				 CEvents::HANDLED_EVENTS_IS_SOUND |
				 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
				(xCCrc.i8Error),
				"Ошибка CRC");
		}
	}
	else
	{
		// неактивное состояние события ещё не зарегистрировано?
		if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
					SYSTEM_ERROR_GROUP_OFFSET,
					SYSTEM_ERROR_CRC_ERROR_OFFSET))
		{
			// зарегистрируем неактивное состояние события.
			CEvents::EventRegistration(
				SYSTEM_ERROR_GROUP_OFFSET,
				(CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
				 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
				(xCCrc.i8Error),
				"Норма CRC");
		}
	}


//    // служебные переменные повреждены?
//    if (xPlcConfigService.usCrc !=
//            usCrcSummTwoByteCalculation((unsigned char*)&xPlcConfigService.xPlcConfigServiceData,
//                                        sizeof(xPlcConfigService.xPlcConfigServiceData)))
//    {
//        // ошибка.
//        // сигнализация.
//        // установим флаг звуковой сигнализации.
//        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//        // перезагрузка.
//        // установим флаг "перезагрузка программы".
//        fbPlcErrorReset = 1;
//        cout << "xPlcConfigServiceData error" << endl;
//    }

//    // данные управления базой данных повреждены?
//    if (xPlcDataBaseControl.usCrc !=
//            usCrcSummTwoByteCalculation((unsigned char*)xPlcDataBaseControl.axPlcDataBaseBlockControl,
//                                        (sizeof(xPlcDataBaseControl.axPlcDataBaseBlockControl))))
//    {
//        // ошибка.
//        // сигнализация.
//        // установим флаг звуковой сигнализации.
//        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//        // перезагрузка.
//        // установим флаг "перезагрузка программы".
//        fbPlcErrorReset = 1;
//        cout << "axPlcDataBaseBlockControl error" << endl;
//    }

	// данные неизменяемого контекста модулей повреждены?
	if (iModulesContextStaticCrcCheck(&xAllModulesContext) == -1)
	{
		// ошибка.
		// сигнализация.
		// перезагрузка.
		// установим флаг "перезагрузка программы".
		fbPlcErrorReset = 1;
		xCCrc.i8Error = (sizeof(xCCrc.axTCrcContextInit) + CONVERT_INTEGER_TO_NATURAL_NUMBER);
		cout << "xModuleContextStatic error" << endl;
		LOG_DBG << "xModuleContextStatic Error!" << (xCCrc.i8Error);
	}

	// копируем флаги отказов внутренних модулей.
	memcpy(&aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET],
		   aucBadModuleStateBuff,
		   INTERNAL_MODULE_QUANTITY);

	// есть ошибка конфигурации?
	if (fbPlcConfigurationError)
	{
		// установим флаг - ошибка конфигурации.
		aucCoilsArray[CONFIGURATION_ERROR_SIGNAL_BIT_ARRAY_OFFSET] = 1;
	}
//    // база данных локальной сети нижнего уровня повреждена?
//    if ((iCrc16Check((unsigned char*)&(xPlcDataBase.
//                                       axPlcDataBaseBlocks[MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_OFFSET].
//                                       aucPlcDataBaseBlockData[0]),
//                     (PLC_DATA_BASE_BLOCK_LENGTH -
//                      TWO_BYTE_CRC_LENGTH))) == -1)
//    {
//        // установим флаг - база данных локальной сети нижнего уровня повреждена.
//        fbLocalNetworkModuleDataBaseError = 1;
//        aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET +
//                      LOCAL_NET_MODULE_DATA_BASE_ERROR_OFFSET] = 1;
//    }
//    // база данных локальной сети нижнего уровня повреждена?
//    if ((iCrc16Check((unsigned char*)&(xPlcDataBase.
//                                       axPlcDataBaseBlocks[MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_OFFSET].
//                                       aucPlcDataBaseBlockData[0]),
//                     (PLC_DATA_BASE_BLOCK_LENGTH -
//                      TWO_BYTE_CRC_LENGTH))) == -1)
//    {
//        // база данных локальной сети нижнего уровня повреждена.
//        fbLocalNetworkModuleDataBaseError = 1;
//        aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET +
//                      LOCAL_NET_MODULE_DATA_BASE_ERROR_OFFSET] = 1;
//    }
	// база данных дискретных сигналов повреждена?
	if (fbDiscreteInputDataBaseError)
	{
//        fbDiscreteInputDataBaseError = 0;
		// установим флаг - база данных дискретных сигналов повреждена.
		aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET +
												   DISCRETE_SYGNALS_DATA_BASE_ERROR_OFFSET] = 1;
		fbModuleMrOutOff = 1;
	}
	// база данных функциональных блоков повреждена?
	if (fbFunctionBlockDataBaseError)
	{
//        // установим флаг - база данных функциональных блоков повреждена.
//        fbFunctionBlockDataBaseError = 0;
		fbModuleMrOutOff = 1;
	}
	// база данных модулей аналогового ввода повреждена?
	if (fbAnalogueInputModuleDataBaseError)
	{
		fbModuleMrOutOff = 1;
//        fbAnalogueInputModuleDataBaseError = 0;
		// установим флаг - база данных модулей аналогового ввода повреждена.
		aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET +
												   ANALOGUE_INPUT_MODULE_DATA_BASE_ERROR_OFFSET] = 1;
	}
	// база данных реперных точек модулей аналогового ввода повреждена?
	if (fbAnalogueInputModuleReperPointsAdcDataBaseError)
	{
		fbModuleMrOutOff = 1;
	}
	// база данных модулей дискретного вывода повреждена?
	if (fbDiscreteOutputModuleDataBaseError)
	{
		fbModuleMrOutOff = 1;
//        fbDiscreteOutputModuleDataBaseError = 0;
		// установим флаг - база данных модулей дискретного вывода повреждена.
		aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET +
												   DISCRETE_OUTPUT_MODULE_DATA_BASE_ERROR_OFFSET] = 1;
	}
	// получен ответ?
	if (xCPingTaskWEEW.ui8Status ==
			CTaskSerialMT::TASK_STATE_MESSAGE_RESPONSE_OK)
	{
		// получен ответ. следующий запрос.
		xCPingTaskWEEW.ui8DataExchangeFsmState = CTaskSerialMT::TASK_MODE_MESSAGE_SEND_READ;
		xCPingTaskWEEW.ui8Status = CTaskSerialMT::TASK_STATE_MESSAGE_ANSWER_WAITING;

		// неактивное состояние события ещё не зарегистрировано?
		if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
					SYSTEM_ERROR_GROUP_OFFSET,
					SYSTEM_ERROR_MIND_NOT_RESPONDED_OFFSET))
		{
			// зарегистрируем неактивное состояние события.
			CEvents::EventRegistration(
				SYSTEM_ERROR_GROUP_OFFSET,
				(CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE |
				 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
				SYSTEM_ERROR_MIND_NOT_RESPONDED_OFFSET,
				"Норма МИНД");
		}
	}
	// не получен ответ?
	else if (xCPingTaskWEEW.ui8Status ==
			 CTaskSerialMT::TASK_STATE_MESSAGE_RECEIVE_TIMEOUT)
	{
		// не получен ответ. следующий запрос.
		xCPingTaskWEEW.ui8DataExchangeFsmState = CTaskSerialMT::TASK_MODE_MESSAGE_SEND_READ;
		xCPingTaskWEEW.ui8Status = CTaskSerialMT::TASK_STATE_MESSAGE_ANSWER_WAITING;
		printf("CPingTaskWEEW vBadModulesTest ui8Status  %d\n\r", (xCPingTaskWEEW.ui8Status));
		xCPingTaskWEEW.ui8Status = CTaskSerialMT::TASK_STATE_MESSAGE_NOT_SEND;
		fbMindDataExchangeError = 1;
		// установим флаг - нет связи с МИНД.
		aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET +
												   MAL_ERROR_OFFSET] = 1;

		// активное состояние события ещё не зарегистрировано?
		if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
					SYSTEM_ERROR_GROUP_OFFSET,
					SYSTEM_ERROR_MIND_NOT_RESPONDED_OFFSET))
		{
			// зарегистрируем активное состояние события.
			CEvents::EventRegistration(
				SYSTEM_ERROR_GROUP_OFFSET,
				(CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE |
				 CEvents::HANDLED_EVENTS_IS_POPUP |
				 CEvents::HANDLED_EVENTS_IS_SOUND |
				 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
				SYSTEM_ERROR_MIND_NOT_RESPONDED_OFFSET,
				"Нет связи с МИНД");
		}
	}

//-----------------------------------------------------------------------------------------------------
//    // обновим журнал ошибок.
//    CError::ErrorLogCreate();
	// обработаем события.
	CEvents::EventsHandler();
	// обновим флаги неисправностей прибора в FRAM.
	iBadModuleBuffUpdate();

//-----------------------------------------------------------------------------------------------------
	// проверим требования отображения сигнализации неисправностей прибора на HMI.
	for (int i = 0; i < BAD_MODULES_BIT_ARRAY_LENGTH; i++) //?
	{
		// есть неисправность прибора?
		if (aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET + i])
		{
			// есть неисправность прибора.
			// есть сигнализация для текущей неисправности прибора?
			if (axAlarmModules[i].ui8AlarmState)
			{


			}
			else
			{
				// установим сигнализацию для текущей неисправности прибора.
				// укажем группу для отображения сигнализации на HMI(первая группа).
				axAlarmModules[i].ui8AlarmState = 1;
				// установим тип сигнализации для текущего дискретного сигнала.
				// установим флаг аварийной сигнализации.
				axAlarmModules[i].ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
			}
		}
		else
		{
			// нет неисправности.
			// есть сигнализация для текущей неисправности прибора?
			if (axAlarmModules[i].ui8AlarmState)
			{
				// есть команда сброс?
				if (fbGlobalReset)
				{
					// сбросим сигнализацию для текущей неисправности прибора.
					axAlarmModules[i] = xAlarmHmiNull;
				}
			}
			else
			{

			}
		}
	}

//-----------------------------------------------------------------------------------------------------
	// есть команда квитирование или сброс?
	if (fbGlobalKvit || fbGlobalReset)
	{
		// сбросим флаг звуковой сигнализации.
		fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_OFF;
		// сбросим сигнализацию неисправности прибора.
		for (int i = 0; i < BAD_MODULES_BIT_ARRAY_LENGTH; i++)
		{
			// есть сигнализация для текущей неисправности прибора?
			if (axAlarmModules[i].ui8AlarmState)
			{
				// сбросим сигнализацию для текущей неисправности прибора.
				// любая команда квитирование или сброс, дают и квитирование и сброс.
				// установим флаги ровный свет:
				// установим флаги предупредительной сигнализации.
				axAlarmModules[i].ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
				// установим флаг аварийной сигнализации.
				axAlarmModules[i].ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
			}
		}
	}

//-----------------------------------------------------------------------------------------------------
	// установим тип сигнализации в буфере активности сигнализации состояния модулей ПАС.
	for (int i = 0; i < BAD_MODULES_BIT_ARRAY_LENGTH; i++)
	{
		// есть сигнализация для текущей неисправности прибора?
		if (axAlarmModules[i].ui8AlarmState)
		{
			// сигнализация для текущей неисправности прибора квитирована?
			if ((axAlarmModules[i].ui8AlarmState & ALARM_TYPE_PREVENT) &&
					(axAlarmModules[i].ui8AlarmState & ALARM_TYPE_ALARM))
			{
				// установим тип сигнализации - квитированая сигнализация.
				aucCoilsArray[MODULES_ALARM_STATE_BITS_BIT_ARRAY_OFFSET] = 1;
			}
			else
			{
				// установим тип сигнализации - аварийная сигнализация.
				aucCoilsArray[MODULES_ALARM_STATE_BITS_BIT_ARRAY_OFFSET + 1] = 1;
			}
		}
		else
		{
			// установим тип сигнализации - норма, нет сигнализациии.
			aucCoilsArray[MODULES_ALARM_STATE_BITS_BIT_ARRAY_OFFSET] = 0;
			aucCoilsArray[MODULES_ALARM_STATE_BITS_BIT_ARRAY_OFFSET + 1] = 0;
		}
	}

//    sync();
//    reboot(LINUX_REBOOT_CMD_RESTART);

}

