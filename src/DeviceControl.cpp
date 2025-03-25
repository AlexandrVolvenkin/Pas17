
#include "PasNewConfig.h"
#include "DeviceControl.h"
#include "Task.h"
#include "SystemComponent.h"
#include "MainProductionCycle.h"

using namespace std;

extern CMainProductionCycle xMainProductionCycle;

//-----------------------------------------------------------------------------------------------------
// обрабатывает входящие сообщения от Modbus интерфейсов,
// параметры передаются в функции modbus_reply() modbus.cpp.

void vDeviceControlModbusMessageHandler(modbus_mapping_t *pxModbusMapping)
{
	// поступило сообщение?
	if (pxModbusMapping -> message_ready)
	{
		switch(pxModbusMapping->function_code)
		{
		case _FC_WRITE_SINGLE_COIL:
			cout << "_FC_WRITE_SINGLE_COIL" << endl;
			vDeviceControlModbusFunction5Handler(pxModbusMapping);
			break;
		case _FC_WRITE_SINGLE_REGISTER:

			break;
		case _FC_WRITE_MULTIPLE_COILS:
			cout << "_FC_WRITE_MULTIPLE_COILS" << endl;
			vDeviceControlModbusFunction15Handler(pxModbusMapping);
			break;
		case _FC_WRITE_MULTIPLE_REGISTERS:
			cout << "_FC_WRITE_MULTIPLE_REGISTERS" << endl;
			vDeviceControlModbusFunction16Handler(pxModbusMapping);
			break;
		case _FC_WRITE_AND_READ_REGISTERS:
			break;
		case _FC_CONFIGURATION_REQUEST:
			cout << "_FC_CONFIGURATION_REQUEST" << endl;
			vDeviceControlConfigurationRequesHandler(pxModbusMapping);
			break;
		case _FC_DATA_BASE_READ:
//            cout << "_FC_DATA_BASE_READ" << endl;
			vDeviceControlDataBaseBlockReadHandler(pxModbusMapping);
			break;
		case _FC_DATA_BASE_WRITE:
			cout << "_FC_DATA_BASE_WRITE" << endl;
			vDeviceControlDataBaseBlockWriteHandler(pxModbusMapping);
			break;
		case _FC_ONLINE_DATA_READ:
//            cout << "_FC_ONLINE_DATA_READ" << endl;
			vDeviceControlOnlineDataReadHandler(pxModbusMapping);
			break;
		case _FC_TIME_SET:
//            cout << "_FC_TIME_SET" << endl;
			vCurrentTimeSet(pxModbusMapping);
			break;
		case _FC_DATA_EXCHANGE:
			cout << "_FC_DATA_EXCHANGE" << endl;
			break;
		default:
			break;
		};
		pxModbusMapping -> function_code = 0;
		// сообщение обработано, буфер освобождён.
		pxModbusMapping -> message_ready = 0;
	}
}

//-----------------------------------------------------------------------------------------------------
// обрабатывает входящие сообщения от Modbus интерфейсов, по 5 функции.
void vDeviceControlModbusFunction5Handler(modbus_mapping_t *pxModbusMapping)
{
	unsigned char nucIndexNumber;
	int i;

//-----------------------------------------------------------------------------------------------------
// Function 5
	// смотрим, по какому адресу записывается бит.
	switch(pxModbusMapping->current_message_address_bits -
			COILS_ARRAY_MODBUS_BEGIN_ADDRESS)
	{
		// квитирование с верхнего уровня.
	case DEVICE_CONTROL_PC_KVIT:
		// бит установлен или сброшен?
		if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
		{
			// если установлен - сбросим.
			pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
			// установим флаг квитирования всем модулям.
			for (i = 0; i < MAX_MODULE_QUANTITY; i++)
			{
				xAllModulesContext.axAllModulesContext[i].
				xModuleContextDinamic.
				fbGlobalModbusKvit = 1;
			}
//            fbHighLevelKvit = 1;
			fbGlobalKvit = 1;

		}
		else
		{

		}
		break;

		// сброс с верхнего уровня.
	case DEVICE_CONTROL_PC_RESET:
		// бит установлен или сброшен?
		if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
		{
			// если установлен - сбросим.
			pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
			// установим флаг сброса всем модулям.
			for (i = 0; i < MAX_MODULE_QUANTITY; i++)
			{
				xAllModulesContext.axAllModulesContext[i].
				xModuleContextDinamic.
				fbGlobalModbusReset = 1;
			}
//            fbHighLevelReset = 1;
			fbGlobalReset = 1;
		}
		else
		{

		}
		break;

		// блокировка с верхнего уровня.
	case DEVICE_CONTROL_BLOCK:
		// бит установлен или сброшен?
		if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
		{
			// если установлен - сбросим.
			pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//            // установим флаг блокировки всем модулям.
//            for (i = 0; i < MAX_MODULE_QUANTITY; i++)
//            {
//                xAllModulesContext.axAllModulesContext[i].
//                xModuleContextDinamic.
//                fbGlobalModbusBlock = 1;
//            }
//            fbHighLevelBlock = 1;
			fbGlobalBlock = 1;
		}
		else
		{
//            // сбросим флаг блокировки всем модулям.
//            for (i = 0; i < MAX_MODULE_QUANTITY; i++)
//            {
//                xAllModulesContext.axAllModulesContext[i].
//                xModuleContextDinamic.
//                fbGlobalModbusBlock = 0;
//            }
//            fbHighLevelBlock = 0;
			fbGlobalBlock = 0;
		}
		break;

	default:
		break;
	};

//-----------------------------------------------------------------------------------------------------
	// смотрим, по какому адресу записывается бит.
	switch(((pxModbusMapping->current_message_address_bits -
			 COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & 0xFF00))
	{
		// включение-выключение режима калибровки.
	case DEVICE_CONTROL_CALIBRATION_ON_OFF:
		// перед этим кто либо другой не вывел из обработки аналоговый вход?
		if ((xMainFlagRegister.ui8AinOffProcessOwnerIndex == AIN_OFF_PROCESS_OWNER_IS_NONE) ||
				(xMainFlagRegister.ui8AinOffProcessOwnerIndex == AIN_OFF_PROCESS_OWNER_IS_PROGRAMMER))
		{
//        // вычислим индекс модуля в массиве контекста, к которому поступила команда - калибровка.
//        nucIndexNumber = (((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
//                           xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
//                          ((((pxModbusMapping->current_message_address_bits -
//                              COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) - 1));
			// вычислим индекс модуля в массиве контекста, к которому поступила команда - калибровка.
			nucIndexNumber = (((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
							   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
							  (((pxModbusMapping->current_message_address_bits -
								 COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4));
			// по индексу - nucIndexNumber есть модуль?
			if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex))
			{
				// бит установлен или сброшен?
				if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
				{
					// включение режима калибровки.
					cout << "DEVICE_CONTROL_CALIBRATION_ON" << endl;
//                cout << "DEVICE_CONTROL_CALIBRATION_ON nucIndexNumber " << (int)nucIndexNumber << endl;
//                cout << "DEVICE_CONTROL_CALIBRATION_ON current_message_address_bits " << (int)(pxModbusMapping->current_message_address_bits -
//                        COILS_ARRAY_MODBUS_BEGIN_ADDRESS) << endl;
					// если установлен - сбросим.
					pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
					// передадим драйверу модуля номер калибруемого входа.
					// в DO-D2 - № входа в модуле (0-нет режима калибровки).
					xAllModulesContext.axAllModulesContext[nucIndexNumber].
					xModuleContextDinamic.
					ucCommonIndex = (((pxModbusMapping->current_message_address_bits -
									   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_INPUT_NUMBER_MASK));
					// вернём в рабочее состояние все выведенные из обработки аналоговые входы.
					for (i = 0; i < MAX_HANDLED_ANALOGUE_INPUT; i++)
					{
						aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET + i] = 0;
					}
//            // выведем из обработки аналоговый вход - Y, модуля - X.
//            // в адресе Modbus 0x00XY, X - номер модуля, Y - номер калибруемого входа.
//            aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET +
//                          ((((((pxModbusMapping->current_message_address_bits - // вычисляем номер модуля
//                                COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
//                               CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) - 1) *
//                            ANALOG_MODULE_INPUT_QUANTITY) +
//
//                           (((pxModbusMapping->current_message_address_bits - // вычисляем номер входа
//                              COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
//                             CALIBRATION_INPUT_NUMBER_MASK) - 1))] = 1;

					// выведем из обработки аналоговый вход - Y, модуля - X.
					// в адресе Modbus 0x00XY, X - номер модуля, Y - номер калибруемого входа.
					aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET +
								  (((((pxModbusMapping->current_message_address_bits - // вычисляем номер модуля
									   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
									  CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) *
									ANALOG_MODULE_INPUT_QUANTITY) +

								   (((pxModbusMapping->current_message_address_bits - // вычисляем номер входа
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
									 CALIBRATION_INPUT_NUMBER_MASK) - 1))] = 1;

					xMainFlagRegister.ui8AinOffProcessOwnerIndex = AIN_OFF_PROCESS_OWNER_IS_PROGRAMMER;
				}
				else
				{
					// выключение режима калибровки.
					cout << "DEVICE_CONTROL_CALIBRATION_OFF" << endl;
//                cout << "DEVICE_CONTROL_CALIBRATION_ON nucIndexNumber " << (int)nucIndexNumber << endl;
//                cout << "DEVICE_CONTROL_CALIBRATION_ON current_message_address_bits " << (int)(pxModbusMapping->current_message_address_bits -
//                        COILS_ARRAY_MODBUS_BEGIN_ADDRESS) << endl;
					// передадим драйверу модуля номер калибруемого входа. если 0, калибровка выключена.
					// в DO-D2 - № входа в модуле (0-нет режима калибровки).
					xAllModulesContext.axAllModulesContext[nucIndexNumber].
					xModuleContextDinamic.
					ucCommonIndex = 0;
					// вернём в рабочее состояние все выведенные из обработки аналоговые входы.
					for (i = 0; i < MAX_HANDLED_ANALOGUE_INPUT; i++)
					{
						aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET + i] = 0;
					}
// вернём в рабочее состояние аналоговый вход - Y, модуля - X.
//            aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET +
//                          ((((((pxModbusMapping->current_message_address_bits - // вычисляем номер модуля
//                                COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
//                               CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) - 1) *
//                            ANALOG_MODULE_INPUT_QUANTITY) +
//
//                           (((pxModbusMapping->current_message_address_bits - // вычисляем номер входа
//                              COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
//                             CALIBRATION_INPUT_NUMBER_MASK) - 1))] = 0;


// вернём в рабочее состояние аналоговый вход - Y, модуля - X.
//            aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET +
//                          (((((pxModbusMapping->current_message_address_bits - // вычисляем номер модуля
//                                COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
//                               CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) *
//                            ANALOG_MODULE_INPUT_QUANTITY) +
//
//                           (((pxModbusMapping->current_message_address_bits - // вычисляем номер входа
//                              COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
//                             CALIBRATION_INPUT_NUMBER_MASK) - 1))] = 0;

					xMainFlagRegister.ui8AinOffProcessOwnerIndex = AIN_OFF_PROCESS_OWNER_IS_NONE;
				}
			}
		}
		break;

		// калибровка начала шкалы - НШК.
	case DEVICE_CONTROL_SET_BOTTOM_OF_SCALE:
//        // вычислим индекс модуля в массиве контекста, к которому поступила команда.
//        nucIndexNumber = (((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
//                           xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
//                          ((((pxModbusMapping->current_message_address_bits -
//                              COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) - 1));
		// вычислим индекс модуля в массиве контекста, к которому поступила команда.
		nucIndexNumber = (((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
						   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
						  (((pxModbusMapping->current_message_address_bits -
							 COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4));
		// по индексу - nucIndexNumber есть модуль?
		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex))
		{
			// бит установлен или сброшен?
			if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
										  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
			{
				// если установлен - сбросим.
				pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
										  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
				// передадим драйверу модуля команду - калибровка начала шкалы - НШК.
				xAllModulesContext.axAllModulesContext[nucIndexNumber].
				xModuleContextDinamic.
				ucCommandControl = MVAI5_COMMAND_CONTROL_SET_BOTTOM_OF_SCALE;
			}
			else
			{

			}
		}
		break;

		// калибровка конца шкалы - ВШК.
	case DEVICE_CONTROL_SET_TOP_OF_SCALE:
//        // вычислим индекс модуля в массиве контекста, к которому поступила команда.
//        nucIndexNumber = (((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
//                           xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
//                          ((((pxModbusMapping->current_message_address_bits -
//                              COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) - 1));
		// вычислим индекс модуля в массиве контекста, к которому поступила команда.
		nucIndexNumber = (((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
						   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
						  (((pxModbusMapping->current_message_address_bits -
							 COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4));
		// по индексу - nucIndexNumber есть модуль?
		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex))
		{
			// бит установлен или сброшен?
			if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
										  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
			{
				// если установлен - сбросим.
				pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
										  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
				// передадим драйверу модуля команду - калибровка конца шкалы - ВШК.
				xAllModulesContext.axAllModulesContext[nucIndexNumber].
				xModuleContextDinamic.
				ucCommandControl = MVAI5_COMMAND_CONTROL_SET_TOP_OF_SCALE;
			}
			else
			{

			}
		}

		break;
	default:
		break;
	};

//-----------------------------------------------------------------------------------------------------
// инкремент уставок - SP, OUT, регуляторов модулей токового вывода - MTVI5.
	// адрес записываемого бита находится в диапазоне инкремента уставок - SP, OUT, регуляторов модулей токового вывода?
	if (((pxModbusMapping -> current_message_address_bits -
			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= INCREMENT_SP_OUT_BIT_ARRAY_OFFSET) &&
			((pxModbusMapping -> current_message_address_bits -
			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (INCREMENT_SP_OUT_BIT_ARRAY_OFFSET +
					  INCREMENT_SP_OUT_BIT_ARRAY_LENGTH)))
	{
		// бит установлен или сброшен?
		if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
		{
			// если установлен - сбросим.
			pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
			cout << "INCREMENT_SP_OUT_1__BIT_ARRAY_OFFSET 1" << endl;
			// установим флаг инкремента-декремента уставок - SP, OUT, регуляторов.
			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1; //?
		}
		else
		{
			cout << "INCREMENT_SP_OUT_1__BIT_ARRAY_OFFSET 0" << endl;
			// сбросим флаг инкремента-декремента уставок - SP, OUT, регуляторов.
			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0; //?
		}

		// вычислим индекс модуля в массиве контекста, к которому поступила команда.
		nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
							((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
							 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
						   ((unsigned char)(((pxModbusMapping->current_message_address_bits -
											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
											 INCREMENT_SP_OUT_BIT_ARRAY_OFFSET) /
											(MTVI5_ANALOG_OUTPUT_QUANTITY))));

		// по индексу - nucIndexNumber есть модуль?
		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
		{
//        if (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex)
//        {
			// вычислим номер регулятора модуля токового вывода, в котором инкрементируем-декрементируем уставку.
			xAllModulesContext.axAllModulesContext[nucIndexNumber].
			xModuleContextDinamic.
			ucCommonIndex = (unsigned char)((((pxModbusMapping->current_message_address_bits -
											   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
											  INCREMENT_SP_OUT_BIT_ARRAY_OFFSET) %
											 MTVI5_ANALOG_OUTPUT_QUANTITY) +
											CONVERT_INTEGER_TO_NATURAL_NUMBER);

// в драйвере модуля токового вывода, данные уставок - SP, OUT,
// поступают из рабочего массива axAnalogueOutputRegulatorContAv[смещение на номер регулятора],
// после изменения туда же и возвращаются. из этого же массива, по протоколу Modbus,
// устройства верхнего уровня(HMI панели, системы SCADA, программаторы) получают данные для отображения на дисплее.
//  на текущем шаге обмен данными драйвера с модулем по SPI интерфейсу проходит в режиме одновременной записи-чтения.
// после записи новые данные на выходе модуля появятся только на следующем шаге записи-чтения.
// и если на текущем шаге данные с выхода модуля просто вернуть обратно, то никаких изменений не произойдёт.
//  поэтому, после записи в модуль значения уставки, изменённого на величину в переменной usAuxiliaryCounter(выражается в процентах),
// оно же отправляется в массив источник.
//  так же, это удобно для корректного отображения устройствами верхнего уровня, изменённых данных.
// так как вызов драйвера модуля токового вывода происходит один раз в секунду,
// а функци vDeviceControlModbusMessageHandler(), максимум - время обработки самого медленного модуля,
// то за это время, оператор нажатием кнопки мыши, может инкрементировать-декрементировать значение уставки
// N - раз. значение - N помещается в переменную usAuxiliaryCounter. в конце каждого цикла обмена данными
// драйвера с модулем, переменная usAuxiliaryCounter - обнуляется.
			// изменим значение переменной, на которое в драйвере модуля изменится значение уставки.
			xAllModulesContext.axAllModulesContext[nucIndexNumber].
			xModuleContextDinamic.
			usAuxiliaryCounter += 1;

			// передадим драйверу модуля команду - инкрементировать.
			xAllModulesContext.axAllModulesContext[nucIndexNumber].
			xModuleContextDinamic.
			ucCommandControl = DEVICE_CONTROL_MTVI5_INCREMENT_SP_OUT;
		}
	}

//-----------------------------------------------------------------------------------------------------
// декремент уставок - SP, OUT, регуляторов модулей токового вывода - MTVI5.
// адрес записываемого бита находится в диапазоне декремента уставок - SP, OUT, регуляторов модулей токового вывода?
	if (((pxModbusMapping -> current_message_address_bits -
			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= DECREMENT_SP_OUT_BIT_ARRAY_OFFSET) &&
			((pxModbusMapping -> current_message_address_bits -
			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (DECREMENT_SP_OUT_BIT_ARRAY_OFFSET +
					  DECREMENT_SP_OUT_BIT_ARRAY_LENGTH)))
	{
		// бит установлен или сброшен?
		if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
		{
			// если установлен - сбросим.
			pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
			cout << "DECREMENT_SP_OUT_BIT_ARRAY_OFFSET 1" << endl;
			// установим флаг инкремента-декремента уставок - SP, OUT, регуляторов.
			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1; //?
		}
		else
		{
			cout << "DECREMENT_SP_OUT_BIT_ARRAY_OFFSET 0" << endl;
			// сбросим флаг инкремента- декремента уставок - SP, OUT, регуляторов.
			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0; //?
		}

		// вычислим индекс модуля в массиве контекста, к которому поступила команда.
		nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
							((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
							 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
						   ((unsigned char)(((pxModbusMapping->current_message_address_bits -
											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
											 DECREMENT_SP_OUT_BIT_ARRAY_OFFSET) /
											(MTVI5_ANALOG_OUTPUT_QUANTITY))));
		// по индексу - nucIndexNumber есть модуль?
		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
		{
			// вычислим номер регулятора модуля токового вывода, в котором инкрементируем-декрементируем уставку.
			xAllModulesContext.axAllModulesContext[nucIndexNumber].
			xModuleContextDinamic.
			ucCommonIndex = (unsigned char)((((pxModbusMapping->current_message_address_bits -
											   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
											  DECREMENT_SP_OUT_BIT_ARRAY_OFFSET) %
											 MTVI5_ANALOG_OUTPUT_QUANTITY) +
											CONVERT_INTEGER_TO_NATURAL_NUMBER);

// в драйвере модуля токового вывода, данные уставок - SP, OUT,
// поступают из рабочего массива axAnalogueOutputRegulatorContAv[смещение на номер регулятора],
// после изменения туда же и возвращаются. из этого же массива, по протоколу Modbus,
// устройства верхнего уровня(HMI панели, системы SCADA, программаторы) получают данные для отображения на дисплее.
//  на текущем шаге обмен данными драйвера с модулем по SPI интерфейсу проходит в режиме одновременной записи-чтения.
// после записи новые данные на выходе модуля появятся только на следующем шаге записи-чтения.
// и если на текущем шаге данные с выхода модуля просто вернуть обратно, то никаких изменений не произойдёт.
//  поэтому, после записи в модуль значения уставки, изменённого на величину в переменной usAuxiliaryCounter(выражается в процентах),
// оно же отправляется в массив источник.
//  так же, это удобно для корректного отображения устройствами верхнего уровня, изменённых данных.
// так как вызов драйвера модуля токового вывода происходит один раз в секунду,
// а функци vDeviceControlModbusMessageHandler() - максимум время обработки самого медленного модуля,
// то за это время, оператор нажатием кнопки мыши, может инкрементировать-декрементировать значение уставки
// N - раз. значение - N помещается в переменную usAuxiliaryCounter. в конце каждого цикла обмена данными
// драйвера с модулем, переменная usAuxiliaryCounter - обнуляется.
			// изменим значение переменной, на которое в драйвере модуля изменится значение уставки.
			xAllModulesContext.axAllModulesContext[nucIndexNumber].
			xModuleContextDinamic.
			usAuxiliaryCounter += 1;

			// передадим драйверу модуля команду - декрементировать.
			xAllModulesContext.axAllModulesContext[nucIndexNumber].
			xModuleContextDinamic.
			ucCommandControl = DEVICE_CONTROL_MTVI5_DECREMENT_SP_OUT;
		}
	}

//-----------------------------------------------------------------------------------------------------
// запись дискретных сигналов регуляторов(CONT_ST), модулей токового вывода - MTVI5.
	// адрес записываемого бита находится в диапазоне массива дискретных сигналов регуляторов(CONT_ST) модулей токового вывода?
	if (((pxModbusMapping -> current_message_address_bits -
			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= CONT_ST_BIT_ARRAY_OFFSET) &&
			((pxModbusMapping -> current_message_address_bits -
			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (CONT_ST_BIT_ARRAY_OFFSET +
					  CONT_ST_BIT_ARRAY_LENGTH)))
	{
		// бит установлен или сброшен?
		if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
		{
			// если установлен - сбросим.
			pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
			cout << "CONT_ST_BIT_ARRAY 1" << endl;
			// установим бит дискретных сигналов регуляторов.
//            aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//                           COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1; //?
			aucContStWriteBitArray[(pxModbusMapping -> current_message_address_bits -
									COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
								   CONT_ST_BIT_ARRAY_OFFSET] = 1;
		}
		else
		{
			cout << "CONT_ST_BIT_ARRAY 0" << endl;
			// сбросим бит дискретных сигналов регуляторов.
//            aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//                           COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0; //?
			aucContStWriteBitArray[(pxModbusMapping -> current_message_address_bits -
									COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
								   CONT_ST_BIT_ARRAY_OFFSET] = 0;
		}

//        vBitToByteArray(&aucContStWriteByteArray[0],
//                        &aucCoilsArray[CONT_ST_BIT_ARRAY_OFFSET],
//                        CONT_ST_BYTE_ARRAY_LENGTH);
		vBitToByteArray(aucContStWriteByteArray,
						aucContStWriteBitArray,
						CONT_ST_BYTE_ARRAY_LENGTH);

		// вычислим индекс модуля в массиве контекста, к которому поступила команда.
		nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
							((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
							 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
						   ((unsigned char)(((pxModbusMapping->current_message_address_bits -
											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
											 CONT_ST_BIT_ARRAY_OFFSET) /
											(MTVI5_STAT_BIT_QUANTITY * MTVI5_ANALOG_OUTPUT_QUANTITY))));
		// по индексу - nucIndexNumber есть модуль?
		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
		{
//            // передадим драйверу модуля команду - запись дискретных сигналов регуляторов.
//            xAllModulesContext.axAllModulesContext[nucIndexNumber].
//            xModuleContextDinamic.
//            ucCommandControl = DEVICE_CONTROL_MTVI5_CONT_ST_WRITE;
		}
	}

//-----------------------------------------------------------------------------------------------------
// запись команд управления верхнего уровня.
	// адрес записываемого бита находится в диапазоне массива команд управления верхнего уровня?
	if (((pxModbusMapping -> current_message_address_bits -
			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= LOG_COM_BIT_ARRAY_OFFSET) &&
			((pxModbusMapping -> current_message_address_bits -
			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (LOG_COM_BIT_ARRAY_OFFSET +
					  LOG_COM_BIT_ARRAY_LENGTH)))
	{
		// бит установлен или сброшен?
		if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
		{
			// если установлен - сбросим.
			pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
			cout << "LOG_COM_BIT_ARRAY_OFFSET 1" << endl;
			// установим бит команд управления верхнего уровня.
			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1;
		}
		else
		{
			cout << "LOG_COM_BIT_ARRAY_OFFSET 0" << endl;
			// сбросим бит команд управления верхнего уровня.
			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0;
		}
	}

//-----------------------------------------------------------------------------------------------------
// выводит из обработки аналоговый вход.
//    // сообщение пришло с интерфейса ModbusRTU HMI?
//    if ((pxModbusMapping -> message_sourse) ==
//            MODBUS_MESSAGE_SOURCE_RTU_HMI)
	//    {
	// адрес записываемого бита находится в диапазоне массива вывода из обработки аналоговых входов?
	if (((pxModbusMapping -> current_message_address_bits -
			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= AIN_OFF_BIT_ARRAY_OFFSET) &&
			((pxModbusMapping -> current_message_address_bits -
			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (AIN_OFF_BIT_ARRAY_OFFSET +
					  AIN_OFF_BIT_ARRAY_LENGTH)))
	{
		// перед этим кто либо другой не вывел из обработки аналоговый вход?
		if ((xMainFlagRegister.ui8AinOffProcessOwnerIndex == AIN_OFF_PROCESS_OWNER_IS_NONE) ||
				(xMainFlagRegister.ui8AinOffProcessOwnerIndex == AIN_OFF_PROCESS_OWNER_IS_HMI))
		{
			// вычислим индекс модуля в массиве контекста, к которому поступила команда.
			nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex) + 1) -
								(xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)) +
							   ((unsigned char)(((pxModbusMapping->current_message_address_bits -
												  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
												 AIN_OFF_BIT_ARRAY_OFFSET) /
												(MVAI5_ANALOG_INPUT_QUANTITY))));
			// по индексу - nucIndexNumber есть модуль?
			if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex))
			{
				// вернём в рабочее состояние выведенный из обработки аналоговый вход.
				for (i = 0; i < MAX_HANDLED_ANALOGUE_INPUT; i++)
				{
					// аналоговый вход - i выведен из обработки?
					if (aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET + i])
					{
						// вернём в рабочее состояние выведенный из обработки аналоговый вход - i.
						aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET + i] = 0;
//                        // вычислим индекс модуля в массиве контекста, который в текущий момент выполняет команду.
//                        nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex) + 1) -
//                                            (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)) +
//                                           ((unsigned char)((i) /
//                                                            (MVAI5_ANALOG_INPUT_QUANTITY))));
						// передадим драйверу модуля номер калибруемого входа. если 0, калибровка выключена.
						// выключим калибровку всех входов модуля;
						xAllModulesContext.axAllModulesContext[
							((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex) + 1) -
							  (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)) +
							 ((unsigned char)((i) /
											  (MVAI5_ANALOG_INPUT_QUANTITY))))].
						xModuleContextDinamic.
						ucCommonIndex = 0;
					}
				}

				// бит установлен или сброшен?
				if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
				{
					// включение режима калибровки.
					cout << "AIN_OFF_BIT_ARRAY_OFFSET 1" << endl;
					// если установлен - сбросим.
					pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
					// вычислим и передадим драйверу модуля номер калибруемого входа.
					xAllModulesContext.axAllModulesContext[nucIndexNumber].
					xModuleContextDinamic.
					ucCommonIndex = (unsigned char)((((pxModbusMapping->current_message_address_bits -
													   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
													  AIN_OFF_BIT_ARRAY_OFFSET) %
													 MVAI5_ANALOG_INPUT_QUANTITY) +
													CONVERT_INTEGER_TO_NATURAL_NUMBER);
					// выведем аналоговый вход из обработки.
					aucCoilsArray[(pxModbusMapping -> current_message_address_bits) -
								  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 1;
					xMainFlagRegister.ui8AinOffProcessOwnerIndex = AIN_OFF_PROCESS_OWNER_IS_HMI;

//                    aucDoValueBlockByteArray[(pxModbusMapping -> current_message_address_bits) -
//                                             COILS_ARRAY_MODBUS_BEGIN_ADDRESS - AIN_OFF_BIT_ARRAY_OFFSET] = 1;
//                    ui8TempCommand = xAllModulesContext.axAllModulesContext[nucIndexNumber].
//                                     xModuleContextDinamic.
//                                     ucCommonIndex;
				}
				else
				{
					// выключение режима калибровки.
					cout << "AIN_OFF_BIT_ARRAY_OFFSET 0" << endl;
					xMainFlagRegister.ui8AinOffProcessOwnerIndex = AIN_OFF_PROCESS_OWNER_IS_NONE;

//                    aucDoValueBlockByteArray[(pxModbusMapping -> current_message_address_bits) -
//                                             COILS_ARRAY_MODBUS_BEGIN_ADDRESS - AIN_OFF_BIT_ARRAY_OFFSET] = 0;
//                    ui8TempCommand = 0;
				}
			}


		}
	}

//-----------------------------------------------------------------------------------------------------
// принимает команду - запись конфигурации.
	if (((pxModbusMapping -> current_message_address_bits -
			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) ==
			CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_OFFSET))
	{
		// бит установлен или сброшен?
		if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
		{
			// если установлен - сбросим.
			pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
			cout << "CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_OFFSET 1" << endl;

			switch(xPlcConfigDataTransmit.xConfigData.ucErrorCode)
			{
			case CONFIGURATION_ERROR_SEQUENCE_ERROR:
			case CONFIGURATION_ERROR_EMPTY_SLOT:
				break;

			case CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE:
//            iPlcConfigurationCreate();
//            vMainTaskMemoryMapFormation();
				// сохранение конфигурации и базы данных.
				iPlcConfigurationSave();
				iPlcDataBaseSave();
//    vWorkArraysUpdate();
				iBadModuleBuffClear();
				// сброс флагов ошибок конфигурации и базы данных,
				// для перехода на нормальную циклическую работу.
				fbPlcConfigurationError = 0;
				fbPlcConfigurationFileError = 0;
				fbPlcDataBaseFileError = 0;
				// установим флаг "перезагрузка программы".
				fbPlcErrorReset = 1;
				xPlcConfigDataTransmit.xConfigData.ucErrorCode = 0;

				// зарегистрируем событие.
				CEvents::EventRegistration(
					0,
					(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
					 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
					0,
					"Новая конф. и бд.");

				cout << "CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE 2" << endl;
				break;
			case CONFIGURATION_ERROR_NEW_CONFIGURATION:
//            iPlcConfigurationCreate();
//            vMainTaskMemoryMapFormation();
				// сохранение конфигурации и базы данных.
				iPlcConfigurationSave();
				//                iPlcDataBaseSave();
//    vWorkArraysUpdate();
				iBadModuleBuffClear();
				// сброс флагов ошибок конфигурации и базы данных,
				// для перехода на нормальную циклическую работу.
				fbPlcConfigurationError = 0;
				fbPlcConfigurationFileError = 0;
				fbPlcDataBaseFileError = 0;
				// установим флаг "перезагрузка программы".
				fbPlcErrorReset = 1;
				xPlcConfigDataTransmit.xConfigData.ucErrorCode = 0;

				// зарегистрируем событие.
				CEvents::EventRegistration(
					0,
					(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
					 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
					0,
					"Новая конфигурация.");

				cout << "CONFIGURATION_ERROR_NEW_CONFIGURATION 3" << endl;
				break;
			case CONFIGURATION_ERROR_BAD_MODULE_FINDED_AND_REGISTERED:
				// сброс флагов ошибок конфигурации и базы данных,
				// для перехода на нормальную циклическую работу.
				fbPlcConfigurationError = 0;
				fbPlcConfigurationFileError = 0;
				fbPlcDataBaseFileError = 0;
				// установим флаг "перезагрузка программы".
				fbPlcErrorReset = 1;
				break;

			default:
				break;
			};

		}
		else
		{
			cout << "CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_OFFSET 0" << endl;
			// сбросим бит запись - конфигурации.
		}
	}
}

//-----------------------------------------------------------------------------------------------------
// обрабатывает входящие сообщения от Modbus интерфейсов по 15 функции.
// запись N - команд управления верхнего уровня.
void vDeviceControlModbusFunction15Handler(modbus_mapping_t *pxModbusMapping)
{
//-----------------------------------------------------------------------------------------------------
// Function 15
//-----------------------------------------------------------------------------------------------------
	// адрес записываемых бит находится в диапазоне массива команд управления верхнего уровня?
	if (((pxModbusMapping -> current_message_address_bits -
			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= LOG_COM_BIT_ARRAY_OFFSET) &&
			((pxModbusMapping -> current_message_address_bits -
			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (LOG_COM_BIT_ARRAY_OFFSET +
					  LOG_COM_BIT_ARRAY_LENGTH)))
	{
		// количество записываемых бит не выходит за пределы размера буфера?
		if (((pxModbusMapping -> current_message_address_bits -
				COILS_ARRAY_MODBUS_BEGIN_ADDRESS) +
				(pxModbusMapping -> current_message_nb_bits)) <
				(LOG_COM_BIT_ARRAY_OFFSET +
				 LOG_COM_BIT_ARRAY_LENGTH))
		{
			// запишем N - команд управления верхнего уровня.
			memcpy(&(aucCoilsArray[pxModbusMapping->current_message_address_bits -
								   COILS_ARRAY_MODBUS_BEGIN_ADDRESS]),
				   &(pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
											   COILS_ARRAY_MODBUS_BEGIN_ADDRESS]),
				   (pxModbusMapping -> current_message_nb_bits));
			cout << "HIGH_LEVEL_COMMAND_BIT_ARRAY_DATA_STORE" << endl;
		}
	}
}

//-----------------------------------------------------------------------------------------------------
// обрабатывает входящие сообщения от Modbus интерфейсов по 16 функции.
void vDeviceControlModbusFunction16Handler(modbus_mapping_t *pxModbusMapping)
{
	unsigned char *pucSource;
	unsigned char *pucDestination;
	unsigned short usData;
	unsigned char nucIndexNumber;
	int i;
	unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];

//-----------------------------------------------------------------------------------------------------
// Function 16
// записывает аналоговые переменные регуляторов модулей токового вывода.
	// адрес записываемых регистров находится в диапазоне массива аналоговых переменных регуляторов модулей токового вывода - MTVI5?
	if (((((pxModbusMapping -> current_message_address_registers) * 2) -
			HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) >= CONT_AV_BIG_ENDIAN_BYTE_ARRAY_OFFSET) &&
			((((pxModbusMapping -> current_message_address_registers) * 2) -
			  HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) < (CONT_AV_BIG_ENDIAN_BYTE_ARRAY_OFFSET +
					  CONT_AV_BIG_ENDIAN_BYTE_ARRAY_LENGTH)))
	{
		// количество записываемых регистров не выходит за пределы размера буфера?
		if (((((pxModbusMapping -> current_message_address_registers) * 2) -
				HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) +
				((pxModbusMapping->current_message_nb_registers) * 2)) <=
				(CONT_AV_BIG_ENDIAN_BYTE_ARRAY_OFFSET +
				 CONT_AV_BIG_ENDIAN_BYTE_ARRAY_LENGTH))
		{

			memcpy(&aucHoldingRegistersArray[(((pxModbusMapping -> current_message_address_registers) * 2) -
											  HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS)],
				   (unsigned char*)&(pxModbusMapping->tab_registers[(pxModbusMapping -> current_message_address_registers) -
									 HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS]),
				   (unsigned int)((pxModbusMapping->current_message_nb_registers) * 2));

			vAnalogueOutputContAvModbusToWork();

			// вычислим индекс модуля в массиве контекста, к которому поступила команда.
			nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
								((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
								 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
							   ((unsigned char)((((((pxModbusMapping -> current_message_address_registers) * 2) -
												   HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) -
												  CONT_AV_BIG_ENDIAN_BYTE_ARRAY_OFFSET) /
												 (sizeof(struct TMtvi5ContAvDataPackOne) *
												  MTVI5_ANALOG_OUTPUT_QUANTITY)))));

			// по индексу - nucIndexNumber есть модуль?
			if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
			{
				// вычислим номер регулятора модуля токового вывода.
				xAllModulesContext.axAllModulesContext[nucIndexNumber].
				xModuleContextDinamic.
				ucCommonIndex = (unsigned char)(((((((pxModbusMapping -> current_message_address_registers) * 2) -
													HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) -
												   CONT_AV_BIG_ENDIAN_BYTE_ARRAY_OFFSET) /
												  sizeof(struct TMtvi5ContAvDataPackOne)) %
												 MTVI5_ANALOG_OUTPUT_QUANTITY) +
												CONVERT_INTEGER_TO_NATURAL_NUMBER);

				// передадим драйверу модуля команду - запись аналоговых переменных регуляторов «CONT_AV».
				xAllModulesContext.axAllModulesContext[nucIndexNumber].
				xModuleContextDinamic.
				ucCommandControl = DEVICE_CONTROL_MTVI5_CONT_AV_WRITE;

				cout << "CONT_AV_BIG_ENDIAN_BYTE_ARRAY OK" << endl;
			}
		}
	}

//-----------------------------------------------------------------------------------------------------
// записывает базу данных регуляторов модулей токового вывода.
// адрес записываемых регистров находится в диапазоне массива базы данных регуляторов модулей токового вывода - MTVI5?
	if (((((pxModbusMapping -> current_message_address_registers) * 2) -
			HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) >= CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET) &&
			((((pxModbusMapping -> current_message_address_registers) * 2) -
			  HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) < (CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET +
					  CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_LENGTH)))
	{
		// количество записываемых регистров не выходит за пределы размера буфера?
		if (((((pxModbusMapping -> current_message_address_registers) * 2) -
				HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) +
				((pxModbusMapping->current_message_nb_registers) * 2)) <=
				(CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET +
				 CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_LENGTH))
		{
			// скопируем блок базы данных пришедший по Modbus в буфер базы данных RAM.
			memcpy(&aucHoldingRegistersArray[(((pxModbusMapping -> current_message_address_registers) * 2) -
											  HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS)],
				   (unsigned char*)&(pxModbusMapping->tab_registers[(pxModbusMapping -> current_message_address_registers) -
									 HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS]),
				   (unsigned int)((pxModbusMapping->current_message_nb_registers) * 2));
			vAnalogueOutputDataBasePack();
//            // здесь запишем первый раз. в модули записываем из буфера базы данных.
//            memset(aucTempArray,
//                   0,
//                   PLC_DATA_BASE_BLOCK_LENGTH);
//            memcpy(aucTempArray,
//                   (&xPlcDataBase.
//                    axPlcDataBaseBlocks[CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET].
//                    aucPlcDataBaseBlockData[0]),
//                   CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH);
			vDataBaseOneBlockCrcCreate((TPlcDataBaseBlock*)&xPlcDataBase.
									   axPlcDataBaseBlocks[CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET].
									   aucPlcDataBaseBlockData[0]);
			vDataBaseOneBlockCrcCreate((TPlcDataBaseBlock*)&xPlcDataBase.
									   axPlcDataBaseBlocks[CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET + 1].
									   aucPlcDataBaseBlockData[0]);







//    // получим указатель на блок базы данных.
//    pucDestination = &(xPlcDataBase.
//                       axPlcDataBaseBlocks[nucDataBaseBlockIndex].
//                       aucPlcDataBaseBlockData[0]);
//    // скопируем блок базы данных пришедший по Modbus в буфер базы данных RAM.
//    // здесь запишем первый раз. в модули записываем из буфера базы данных.
//    memset(pucDestination,
//           0,
//           PLC_DATA_BASE_BLOCK_LENGTH);
//    memcpy(pucDestination,
//           pucSource,
//           (xPlcDataBaseControl.
//            axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
//            usBlockLength));
//    vDataBaseOneBlockCrcCreate((TPlcDataBaseBlock*)pucDestination);





			// вычислим индекс модуля в массиве контекста, к которому поступила команда.
			nucIndexNumber =  (xPlcDataBaseControl.axPlcDataBaseBlockControl[
								   CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET +
								   ((((pxModbusMapping -> current_message_address_registers) * 2) -
									 CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET) /
									(CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_LENGTH / 2))].
							   ucModuleIndex);

			// по индексу - nucIndexNumber есть модуль?
			if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
			{
				printf("CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET nucIndexNumber %d\n\r", (nucIndexNumber));
//            // передадим драйверу модуля команду - запись базы данных.
//            xAllModulesContext.axAllModulesContext[nucIndexNumber].
//            xModuleContextDinamic.
//            ucCommandControl = DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_HMI;
//
//            if ((iModuleMtvi5RegulatorDataBaseBlockWrite(&xAllModulesContext.axAllModulesContext[nucIndexNumber])) != -1)
//            {
//                //                iPlcDataBaseBlockWrite(CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET,
////                                       aucTempArray,
////                                       CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH);
//                iPlcDataBaseBlockWrite(CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET);
//                cout << "CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY OK" << endl;
//            }
//            else
//            {
//                // передадим драйверу модуля команду - запись базы данных.
//                xAllModulesContext.axAllModulesContext[nucIndexNumber].
//                xModuleContextDinamic.
//                ucCommandControl = 0;
//                cout << "CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY ERROR" << endl;
//                // при записи в модуль произошла ошибка.
//                // восстановим базу данных.
//                iPlcDataBaseCheck();
//            }

//            // после внесения изменений в базу данных, все рабочие массивы с базами данных нужно обновить.
//            vAnalogueInputDescriptionWorkExstract();
//            vAnalogueInputDescriptionWorkToModbus();
//            vDimentionsParametersDataBaseExstract();
//            vModbusMasterModuleDataBaseExstract();
//            vAnalogueOutputDescriptionWorkExstract();
//            vAnalogueOutputDescriptionWorkToModbus();
//            vDiscreteInputDataBaseExstract();
//            vDiscreteSygnalTextDescriptorDataBaseExstract();
//            vFunctionBlockDataBaseExstract();
//            // вычислим контрольные суммы. потом будем проверять целостность перед каждым главным циклом.
////            xPlcConfigService.usCrc =
////                usCrcSummTwoByteCalculation((unsigned char*)&xPlcConfigService.xPlcConfigServiceData,
////                                            sizeof(xPlcConfigService.xPlcConfigServiceData));
//            xCCrc.CreateAll();

				vWorkArraysUpdate();



				// переведём драйвер модуля в режим записи базы данных регуляторов и задатчиков.
				xAllModulesContext.axAllModulesContext[nucIndexNumber].
				xModuleContextDinamic.
				ucFsmState = MTVI5_FSM_STATE_WRITE_REGULATOR_DATABASE;
			}
		}
	}

//-----------------------------------------------------------------------------------------------------
// записывает ручные уставки.
// адрес записываемых регистров находится в диапазоне массива аналоговых переменных регуляторов модулей токового вывода - MTVI5?
	if (((((pxModbusMapping -> current_message_address_registers) * 2) -
			HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) >= HAND_UST_BYTE_ARRAY_OFFSET) &&
			((((pxModbusMapping -> current_message_address_registers) * 2) -
			  HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) < (HAND_UST_BYTE_ARRAY_OFFSET +
					  HAND_UST_BYTE_ARRAY_LENGTH)))
	{
		// количество записываемых регистров не выходит за пределы размера буфера?
		if (((((pxModbusMapping -> current_message_address_registers) * 2) -
				HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) +
				((pxModbusMapping->current_message_nb_registers) * 2)) <=
				(HAND_UST_BYTE_ARRAY_OFFSET +
				 HAND_UST_BYTE_ARRAY_LENGTH))
		{

			memcpy(&aucHoldingRegistersArray[(((pxModbusMapping -> current_message_address_registers) * 2) -
											  HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS)],
				   (unsigned char*)&(pxModbusMapping->tab_registers[(pxModbusMapping -> current_message_address_registers) -
									 HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS]),
				   (unsigned int)((pxModbusMapping->current_message_nb_registers) * 2));

			cout << "HAND_UST_BYTE_ARRAY_OFFSET OK" << endl;
		}
	}

//-----------------------------------------------------------------------------------------------------
// записывает время и дату.
// адрес записываемых регистров находится в диапазоне массива аналоговых переменных регуляторов модулей токового вывода - MTVI5?
	if (((((pxModbusMapping -> current_message_address_registers) * 2) -
			HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) >= CURRENT_TIME_HOLDING_BYTE_ARRAY_OFFSET) &&
			((((pxModbusMapping -> current_message_address_registers) * 2) -
			  HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) < (CURRENT_TIME_HOLDING_BYTE_ARRAY_OFFSET +
					  CURRENT_TIME_HOLDING_BYTE_ARRAY_LENGTH)))
	{
		// количество записываемых регистров не выходит за пределы размера буфера?
		if (((((pxModbusMapping -> current_message_address_registers) * 2) -
				HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) +
				((pxModbusMapping->current_message_nb_registers) * 2)) <=
				(CURRENT_TIME_HOLDING_BYTE_ARRAY_OFFSET +
				 CURRENT_TIME_HOLDING_BYTE_ARRAY_LENGTH))
		{

			memcpy(&aucHoldingRegistersArray[(((pxModbusMapping -> current_message_address_registers) * 2) -
											  HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS)],
				   (unsigned char*)&(pxModbusMapping->tab_registers[(pxModbusMapping -> current_message_address_registers) -
									 HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS]),
				   (unsigned int)((pxModbusMapping->current_message_nb_registers) * 2));

			uint16_t* pui16Sourse;
			uint8_t aui8Destination[CURRENT_TIME_BYTE_QUANTITY];

			pui16Sourse = (uint16_t*)&aucHoldingRegistersArray[CURRENT_TIME_HOLDING_BYTE_ARRAY_OFFSET];
			for (int i = 0;
					(i < CURRENT_TIME_BYTE_QUANTITY);
					i++)
			{
				aui8Destination[i] = (uint8_t)*pui16Sourse++;
			}

			vLinuxCurrentTimeSet(aui8Destination);


			cout << "CURRENT_TIME_HOLDING_BYTE_ARRAY_OFFSET OK" << endl;
		}
	}
}

//-----------------------------------------------------------------------------------------------------
// обрабатывает входящие сообщения от Modbus интерфейсов по 17 функции - запрос конфигурации.
void vDeviceControlConfigurationRequesHandler(modbus_mapping_t *pxModbusMapping)
{
	unsigned char *pucSource;
	unsigned char *pucDestination;

	pucSource = (unsigned char*)&xPlcConfigDataTransmit.xConfigData;
	pucDestination = pxModbusMapping -> buffer_pointer;

	vConfigurationDataPack((TConfigDataPackOne*)(pxModbusMapping -> buffer_pointer),
						   (TConfigData*)&xPlcConfigDataTransmit.xConfigData);

	pxModbusMapping -> current_message_nb_common =
		sizeof(struct TConfigDataPackOne);

//                    printf("sizeof(struct TConfigDataPackOne) %d\n\r", (sizeof(struct TConfigDataPackOne)));
//
//        cout << "send_msg msg pucSource" << endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)pucSource;
//        for(int i=0; i<32; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }

}

//-----------------------------------------------------------------------------------------------------
// обрабатывает входящие сообщения от Modbus интерфейсов по 68 функции - чтение блока базы данных.
void vDeviceControlDataBaseBlockReadHandler(modbus_mapping_t *pxModbusMapping)
{
	unsigned char *pucSource;
	unsigned char *pucDestination;
	unsigned char nucDataBaseBlockIndex;
//-----------------------------------------------------------------------------------------------------
// DataBaseBlockRead
//-----------------------------------------------------------------------------------------------------
	// номер блока базы данных.
	nucDataBaseBlockIndex = (unsigned char)pxModbusMapping -> current_message_address_common;
	pucSource = (unsigned char*)&(xPlcDataBase.
								  axPlcDataBaseBlocks[nucDataBaseBlockIndex].
								  aucPlcDataBaseBlockData[0]);

//    if (nucDataBaseBlockIndex ==
//            ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + 1)
//    {
//        cout << "DeviceControlDataBaseBlockRead pucSource xPlcDataBase" << endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)pucSource;
//        for(int i=0; i<170; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//    }

	pucDestination = pxModbusMapping -> buffer_pointer;

	pxModbusMapping -> current_message_address_common = 0;

//    if (nucDataBaseBlockIndex ==
//            ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_OFFSET)
//    {
//        if (ucModuleError ==
//                MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR)
//        {
//            pxModbusMapping -> current_message_address_common =
//                MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR;
//            cout << "MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR" << endl;
//            ucModuleError = 0;
//        }
//        else if (ucModuleError ==
//                 MVAI5_GET_REPER_POINTS_ADC_DATA_COMMUNICATION_ERROR)
//        {
//            pxModbusMapping -> current_message_address_common =
//                MVAI5_GET_REPER_POINTS_ADC_DATA_COMMUNICATION_ERROR;
//            cout << "MVAI5_GET_REPER_POINTS_ADC_DATA_COMMUNICATION_ERROR" << endl;
//            ucModuleError = 0;
//        }
//    }

	memcpy(pucDestination,
		   pucSource,
		   xPlcDataBaseControl.
		   axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
		   usBlockLength);
	pxModbusMapping -> current_message_nb_common =
		(xPlcDataBaseControl.
		 axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
		 usBlockLength);
}

//-----------------------------------------------------------------------------------------------------
// обрабатывает входящие сообщения от Modbus интерфейсов по 69 функции - запись блока базы данных.
void vDeviceControlDataBaseBlockWriteHandler(modbus_mapping_t *pxModbusMapping)
{
	unsigned char *pucSource;
	unsigned char *pucDestination;
	unsigned short usData;
	unsigned char nucIndexNumber;
	unsigned char nucDataBaseBlockIndex;
	int i;
	int iResult;
	FILE *pxDataBaseFile;
	FILE *pxReservDataBaseFile;
	char *pxFileName;
	char *pxReservFileName;
	TMrXXOneChannelDataBase *pxMrXXOneChannelDataBaseSource;
	TMrXXOneChannelDataBase *pxMrXXOneChannelDataBaseDestination;

//-----------------------------------------------------------------------------------------------------
// DataBaseBlockWrite
//-----------------------------------------------------------------------------------------------------
	ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_BUSY;
//    iResult = -1;
	iResult = 0;
	// получим номер блока базы данных пришедший по modbus.
	nucDataBaseBlockIndex = (unsigned char)pxModbusMapping -> current_message_address_registers;
	// получим указатель на данные пришедшие по modbus.
	pucSource = (unsigned char*)(pxModbusMapping -> buffer_pointer);

//    cout << "DeviceControlDataBaseBlockWrite nucDataBaseBlockIndex " << (int)nucDataBaseBlockIndex << endl;
//    cout << "DeviceControlDataBaseBlockWrite tab_registers pucSource" << endl;
//    unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)pucSource;
//    for(int i=0; i<256; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }


	// получим указатель на блок базы данных.
	pucDestination = &(xPlcDataBase.
					   axPlcDataBaseBlocks[nucDataBaseBlockIndex].
					   aucPlcDataBaseBlockData[0]);
	// скопируем блок базы данных пришедший по Modbus в буфер базы данных RAM.
	// здесь запишем первый раз. в модули записываем из буфера базы данных.
	memset(pucDestination,
		   0,
		   PLC_DATA_BASE_BLOCK_LENGTH);
	memcpy(pucDestination,
		   pucSource,
		   (xPlcDataBaseControl.
			axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
			usBlockLength));
	vDataBaseOneBlockCrcCreate((TPlcDataBaseBlock*)pucDestination);

//    cout << "DeviceControlDataBaseBlockWrite nucDataBaseBlockIndex length " << (int)(xPlcDataBaseControl.
//			axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
//			usBlockLength) << endl;

//    cout << "DeviceControlDataBaseBlockWrite xPlcDataBase pucDestination" << endl;
//    //unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)pucDestination;
//    for(int i=0; i<170; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }

	// номер принятого для записи блока базы данных.
	switch(nucDataBaseBlockIndex)
	{
	case DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET:
		cout << "DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET" << endl;
		nucIndexNumber = 0;
		// пройдём по контексту всех модулей.
		for (i = 0;
				i < (xPlcConfigService.
					 xPlcConfigServiceData.
					 ucInternalModulesQuantity);
				i++)
		{
			// какого типа модуль?
			switch(xAllModulesContext.axAllModulesContext[nucIndexNumber].
					xModuleContextStatic.ucModuleType)
			{
				// в приборе присутствует модуль MVDS9.
			case MODULE_TYPE_MVDS9:
				iResult = iModuleMvds9DataBaseBlockWrite(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
				if (iModuleMvds9DataBaseBlockWrite(&xAllModulesContext.axAllModulesContext[nucIndexNumber]) == -1)
				{
					iResult = -1;
					// запишем код ошибки, для передачи программатору, по 14 функции Modbus - запрос о результатах программирования.
					ucDataBaseError = PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_MVDS9_EEPROM_ERROR;
					cout << "DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_MVDS9_EEPROM_ERROR" << endl;
				}
				else
				{
					iResult = 0;
					iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
					// сообщим программатору, что ошибок нет, по 14 функции Modbus - запрос о результатах программирования.
					ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
					cout << "DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET PROGRAMMING_DATA_BASE_WRITE_OK" << endl;
				}
				break;

			default:
				// сообщим программатору, что ошибок нет, по 14 функции Modbus - запрос о результатах программирования.
				ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
				cout << "DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET no module MODULE_TYPE_MVDS9" << endl;
				break;
			};
			nucIndexNumber++;
		}
		break;

	case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET:
	case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + 1:
	case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + 2:
	case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + 3:
	case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + 4:
	case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + 5:
	case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + 6:
	case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + 7:
	case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + 8:
	case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + 9:
	case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + 10:
	case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + 11:
	case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + 12:
	case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + 13:
	case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + 14:
	case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + 15:
		cout << "ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET" << endl;
		// какого типа модуль?
		switch(xPlcDataBaseControl.
				axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
				ucModuleType)
		{
			// в приборе присутствует модуль аналогового ввода.
		case MODULE_TYPE_MVPS3:
		case MODULE_TYPE_MVAI5:
		case MODULE_TYPE_MVST3:
		case MODULE_TYPE_MVAO3:
////            iModuleMvai5DataBaseBlockWrite(&xAllModulesContext.axAllModulesContext[xPlcDataBaseControl.
////                                           axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
////                                           ucModuleIndex]);
//            iResult = iModuleMvai5DataBaseBlockWrite(&xAllModulesContext.axAllModulesContext[xPlcDataBaseControl.
//                      axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
//                      ucModuleIndex]);
//            if (iResult == -1)
//            {
//                // запишем код ошибки, для передачи программатору, по 14 функции Modbus - запрос о результатах программирования.
//                ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_EEPROM_ERROR;
//            }


			xAllModulesContext.
			axAllModulesContext[xPlcDataBaseControl.
								axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
								ucModuleIndex].
			xModuleContextDinamic.
			ucCommonIndex = nucDataBaseBlockIndex;
			xAllModulesContext.
			axAllModulesContext[xPlcDataBaseControl.
								axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
								ucModuleIndex].
			xModuleContextDinamic.
			ucFsmState = MVAI5_WRITE_DATA_BASE;
			break;
		default:
			iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
			// сообщим программатору, что ошибок нет, по 14 функции Modbus - запрос о результатах программирования.
			ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
			break;
		};
		break;

	case DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_OFFSET:
		cout << "DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_OFFSET" << endl;
		// получим указатель на блок базы данных, принятый по Modbus во временный буфер.
		pxMrXXOneChannelDataBaseSource =
			(TMrXXOneChannelDataBase*)pucSource;
		// пройдём по контексту модулей дискретного вывода.
		for (int i = 0;
				i < (xPlcConfigService.
					 xPlcConfigServiceData.
					 ucServiceDiscreteOutputQuantity);
				i++)
		{
			// если не запрограммирован сброс для управляющего выхода с блокировкой - база данных не корректна.
			if ((((pxMrXXOneChannelDataBaseSource[i].ucControl) & OUTPUT_TYPE_MASK) &&
					((pxMrXXOneChannelDataBaseSource[i].ucControl) & BLOCK_ALARM_TYPE_MASK) &&
					(!((pxMrXXOneChannelDataBaseSource[i].ucControl) & PC_RESET_MASK)) &&
					(!((pxMrXXOneChannelDataBaseSource[i].ucControl) & BUTTON_RESET_MASK))) &&
					(!(pxMrXXOneChannelDataBaseSource[i].ucExternalResetAddress)))
			{
				cout << "PROGRAMMING_MR_DATA_BASE_NOT_CORRECT 1" << endl;
				// запишем код ошибки, для передачи программатору, по 14 функции Modbus - запрос о результатах программирования.
				ucDataBaseError = PROGRAMMING_MR_DATA_BASE_NOT_CORRECT;
				iResult = -1;
				break;
			}
			else
			{
				iResult = 0;
			}
			// если не запрограммировано квитирование для сигнализирующего выхода "звук" - база данных не корректна.
			if (((!((pxMrXXOneChannelDataBaseSource[i].ucControl) & OUTPUT_TYPE_MASK)) &&
					((pxMrXXOneChannelDataBaseSource[i].ucControl) & BLOCK_ALARM_TYPE_MASK) &&
					(!((pxMrXXOneChannelDataBaseSource[i].ucControl) & PC_KVIT_MASK)) &&
					(!((pxMrXXOneChannelDataBaseSource[i].ucControl) & BUTTON_KVIT_MASK))) &&
					(!(pxMrXXOneChannelDataBaseSource[i].ucExternalKvitAddress)))
			{
				cout << "PROGRAMMING_MR_DATA_BASE_NOT_CORRECT 2" << endl;
				// запишем код ошибки, для передачи программатору, по 14 функции Modbus - запрос о результатах программирования.
				ucDataBaseError = PROGRAMMING_MR_DATA_BASE_NOT_CORRECT;
				iResult = -1;
				break;
			}
			else
			{
				iResult = 0;
			}
			// если не запрограммировано квитирование и сброс для сигнализирующего выхода "свет" - база данных не корректна.
			if ((((!((pxMrXXOneChannelDataBaseSource[i].ucControl) & OUTPUT_TYPE_MASK)) &&
					(!((pxMrXXOneChannelDataBaseSource[i].ucControl) & BLOCK_ALARM_TYPE_MASK)) &&
					(!((pxMrXXOneChannelDataBaseSource[i].ucControl) & PC_KVIT_MASK)) &&
					(!((pxMrXXOneChannelDataBaseSource[i].ucControl) & PC_RESET_MASK)) &&
					(!((pxMrXXOneChannelDataBaseSource[i].ucControl) & BUTTON_KVIT_MASK))) &&
					(!((pxMrXXOneChannelDataBaseSource[i].ucControl) & BUTTON_RESET_MASK))) &&
					((!(pxMrXXOneChannelDataBaseSource[i].ucExternalKvitAddress))) &&
					((!(pxMrXXOneChannelDataBaseSource[i].ucExternalResetAddress))))
			{
				cout << "PROGRAMMING_MR_DATA_BASE_NOT_CORRECT 3" << endl;
				// запишем код ошибки, для передачи программатору, по 14 функции Modbus - запрос о результатах программирования.
				ucDataBaseError = PROGRAMMING_MR_DATA_BASE_NOT_CORRECT;
				iResult = -1;
				break;
			}
			else
			{
				iResult = 0;
			}
		}
		// если база данных не корректна - не будем записывать её в модуль.
		if (iResult == -1)
		{
			break;
		}

		nucIndexNumber = 0;
		// пройдём по контексту всех модулей.
		for (i = 0;
				i < (xPlcConfigService.
					 xPlcConfigServiceData.
					 ucInternalModulesQuantity);
				i++)
		{
			// какого типа модуль?
			switch(xAllModulesContext.axAllModulesContext[nucIndexNumber].
					xModuleContextStatic.ucModuleType)
			{
				// в приборе присутствует модуль дискретного вывода.
			case MODULE_TYPE_MR53:
			case MODULE_TYPE_MR54:
			case MODULE_TYPE_MR55:
			case MODULE_TYPE_MR51:
//                iResult = iModuleMrXXDataBaseBlockWrite(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
				// установим состояние автомата, драйвера модуля - нормальный обмен.
				xAllModulesContext.axAllModulesContext[nucIndexNumber].
				xModuleContextDinamic.
				ucFsmState = MRXX_WRITE_DATA_BASE;
				break;
			default:
				break;
			};
			nucIndexNumber++;
		}

		iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
		break;

	case CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET:
	case CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET + 1:
		cout << "CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET" << endl;
		// какого типа модуль?
		switch(xPlcDataBaseControl.
				axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
				ucModuleType)
		{
			// в приборе присутствует модуль аналогового вывода.
		case MODULE_TYPE_MTVI5:
			nucIndexNumber = xPlcDataBaseControl.
							 axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
							 ucModuleIndex;
			printf("vDeviceControlDataBaseBlockWriteHandler nucIndexNumber %d\n\r", (nucIndexNumber));
//            // передадим драйверу модуля команду - запись базы данных.
//            xAllModulesContext.axAllModulesContext[nucIndexNumber].
//            xModuleContextDinamic.
//            ucCommandControl = DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_PROGRAMMER;
//            iResult =
//                iModuleMtvi5RegulatorDataBaseBlockWrite(
//                    &xAllModulesContext.axAllModulesContext[nucIndexNumber]);
//            if (iResult == -1)
//            {
//                // запишем код ошибки, для передачи программатору, по 14 функции Modbus - запрос о результатах программирования.
//                ucDataBaseError = PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_PSP_EEPROM_ERROR;
//                // передадим драйверу модуля команду - запись базы данных.
//                xAllModulesContext.axAllModulesContext[nucIndexNumber].
//                xModuleContextDinamic.
//                ucCommandControl = 0;
//            }



			xAllModulesContext.
			axAllModulesContext[nucIndexNumber].
			xModuleContextDinamic.
			ucCommonIndex = nucDataBaseBlockIndex;
			// переведём драйвер модуля в режим записи базы данных регуляторов и задатчиков.
			xAllModulesContext.axAllModulesContext[nucIndexNumber].
			xModuleContextDinamic.
			ucFsmState = MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_REGULATOR;
//            return;//debag//
			break;
		default:
			// запишем код ошибки, для передачи программатору, по 14 функции Modbus - запрос о результатах программирования.
			ucDataBaseError = PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_PSP_EEPROM_ERROR;
			iResult = -1;//debag//
			break;
		};

		iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
		break;

//    case CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_OFFSET:
//        cout << "CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_OFFSET" << endl;
//        // какого типа модуль?
//        switch(xPlcDataBaseControl.
//                axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
//                ucModuleType)
//        {
//            // в приборе присутствует модуль аналогового вывода.
//        case MODULE_TYPE_MTVI5:
//
//            iResult = iModuleMtvi5PspDataBaseBlockWrite(&xAllModulesContext.axAllModulesContext[xPlcDataBaseControl.
//                      axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
//                      ucModuleIndex]);
//            if (iResult == -1)
//            {
//                // запишем код ошибки, для передачи программатору, по 14 функции Modbus - запрос о результатах программирования.
//                ucDataBaseError = PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_PSP_EEPROM_ERROR;
//            }
//            break;
//        default:
//            break;
//        };
//        break;

	case MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_OFFSET:
	case MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_OFFSET + 1:
		cout << "MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_OFFSET" << endl;
//        iResult = 0;
		iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
		// сообщим программатору, что ошибок нет, по 14 функции Modbus - запрос о результатах программирования.
		ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
		break;

	case MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_OFFSET:
		cout << "MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_OFFSET" << endl;
//        iResult = 0;
		iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
		// сообщим программатору, что ошибок нет, по 14 функции Modbus - запрос о результатах программирования.
		ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
		break;

	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 1:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 2:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 3:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 4:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 5:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 6:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 7:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 8:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 9:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 10:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 11:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 12:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 13:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 14:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 15:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 16:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 17:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 18:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 19:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 20:
	case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + 21:
		cout << "DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET" << endl;
//        iResult = 0;
		iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
		// сообщим программатору, что ошибок нет, по 14 функции Modbus - запрос о результатах программирования.
		ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
		break;

	case DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET:
		cout << "DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET" << endl;
//        iResult = 0;
		iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
		// сообщим программатору, что ошибок нет, по 14 функции Modbus - запрос о результатах программирования.
		ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
//        if (iResult == -1)
//        {
//            // запишем код ошибки, для передачи программатору, по 14 функции Modbus - запрос о результатах программирования.
//            ucDataBaseError = PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_DIMENSIONS_TEXT_TITLES_EEPROM_ERROR;
//        }
		break;

	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 1:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 2:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 3:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 4:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 5:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 6:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 7:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 8:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 9:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 10:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 11:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 12:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 13:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 14:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 15:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 16:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 17:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 18:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 19:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 20:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 21:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 22:
	case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 23:
		cout << "TEXT_TITLES_DATA_BASE_BLOCK_OFFSET" << endl;
//        iResult = 0;
		iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
		// сообщим программатору, что ошибок нет, по 14 функции Modbus - запрос о результатах программирования.
		ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
//        if (iResult == -1)
//        {
//            // запишем код ошибки, для передачи программатору, по 14 функции Modbus - запрос о результатах программирования.
//            ucDataBaseError = PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_DIMENSIONS_TEXT_TITLES_EEPROM_ERROR;
//        }
		break;

	case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET:
	case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET + 1:
	case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET + 2:
	case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET + 3:
	case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET + 4:
	case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET + 5:
	case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET + 6:
	case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET + 7:
	case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET + 8:
	case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET + 9:
	case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET + 10:
	case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET + 11:
		cout << "FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET" << endl;
//        iResult = 0;
		iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
		// сообщим программатору, что ошибок нет, по 14 функции Modbus - запрос о результатах программирования.
		ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
		break;

	case MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_OFFSET:
	case MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_OFFSET + 1:
	case MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_OFFSET + 2:
		cout << "MATHEMATICAL_FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET" << endl;
//        iResult = 0;
		iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
		// сообщим программатору, что ошибок нет, по 14 функции Modbus - запрос о результатах программирования.
		ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
		break;

	case NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET:
		cout << "NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET" << endl;
		ucModbusSlaveAddress = (xPlcDataBase.
								axPlcDataBaseBlocks[nucDataBaseBlockIndex].
								aucPlcDataBaseBlockData[0]) + 1;
		modbus_set_slave(pxSlaveTcpCtx, ucModbusSlaveAddress);
		modbus_set_slave(pxSlaveRtuCtx, ucModbusSlaveAddress);
//        iResult = 0;
		iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
		// сообщим программатору, что ошибок нет, по 14 функции Modbus - запрос о результатах программирования.
		ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
		break;

	default:
		cout << "default" << endl;
		iResult = -1;
		break;
	};

	cout << "iResult" << endl;
	cout << "0x" << hex << uppercase << setw(2) << setfill('0') << (unsigned int)iResult << " " << endl;
	// если ошибок нет - запишем блок базы данных в EEPROM прибора.
	if (iResult == 0)
	{
		// переведём автомат главного цикла:
		// void CMainProductionCycle::Fsm(void)
		// в режим начала программирования
		xMainProductionCycle.SetFsmState(CMainProductionCycle::PROGRAMMING_CYCLE_START);

//	cout << "xMainProductionCycle.SetFsmState begin" << endl;


//		cout << "iResult if (iResult == 0)" << endl;
//
//		// программирование началось сейчас?
//		if (xProgrammTimeMeasure.LastMoreThan(10))
//		{
//			cout << "if (xProgrammTimeMeasure.LastMoreThan(10))" << endl;
//			// зарегистрируем событие.
//			CEvents::EventRegistration(
//				0,
//				(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
//				 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//				0,
//				"Программирование");
//		}
//
////        iPlcDataBaseBlockWrite(nucDataBaseBlockIndex,
////                               pucSource,
////                               (xPlcDataBaseControl.
////                                axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
////                                usBlockLength));
////        iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
//		// номер принятого для записи блока базы данных.
//		switch(nucDataBaseBlockIndex)
//		{
//			// если принят блок базы данных для модулей локальной сети нижнего уровня,
//			// то это значит, что изменилось количество модулей, входов. требуется изменение конфигурации.
//		case MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_OFFSET:
//		case MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_OFFSET + 1:
//		case MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_OFFSET:
//			// создадим новую конфигурацию
////            iPlcDataBaseCheck();
////            iPlcConfigurationCheck();
////            vModbusMasterModuleDataBaseExstract();
//
//
//
//
//
////            iPlcModulesSearch();
//			iPlcConfigurationCreate();
//			vMainTaskMemoryMapFormation();
//			iPlcConfigurationSave();
//			CSettingsRW::PlcSettingsLoad();
//			// обновим настройки интерфейса ModbusRtu, связь с локальной сетью нижнего уровня, RS-485.
//			iModbusRtuMasterReset();
//			break;
//
//		default:
//			break;
//		};
//
////        // сообщим программатору, что ошибок нет, по 14 функции Modbus - запрос о результатах программирования.
////        ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
	}
	else
	{
		cout << "iResult if (iResult != 0)" << endl;
		// при записи в модуль произошла ошибка.
		// восстановим базу данных.
		iPlcDataBaseCheck();
	}
////    // после внесения изменений в базу данных, все рабочие массивы с базами данных нужно обновить.
////    CSettingsRW::PlcSettingsLoad();
////    vAnalogueInputDescriptionWorkExstract();
////    vAnalogueInputDescriptionWorkToModbus();
////    vDimentionsParametersDataBaseExstract();
////    vModbusMasterModuleDataBaseExstract();
////    vAnalogueOutputDescriptionWorkExstract();
////    vAnalogueOutputDescriptionWorkToModbus();
////    vDiscreteInputDataBaseExstract();
////    vDiscreteSygnalTextDescriptorDataBaseExstract();
////    vFunctionBlockDataBaseExstract();
////
////    // вычислим контрольные суммы. потом будем проверять целостность перед каждым главным циклом.
//////    xPlcConfigService.usCrc =
//////        usCrcSummTwoByteCalculation((unsigned char*)&xPlcConfigService.xPlcConfigServiceData,
//////                                    sizeof(xPlcConfigService.xPlcConfigServiceData));
////    xCCrc.CreateAll();
//
//	vWorkArraysUpdate();
}

//-----------------------------------------------------------------------------------------------------
// обрабатывает входящие сообщения от Modbus интерфейсов по 71 функции - чтение данных онлайн, модулей аналогового ввода.
// передаёт измеренные значения аналоговых входов, реперные точки АЦП, значения ТХС.
void vDeviceControlOnlineDataReadHandler(modbus_mapping_t *pxModbusMapping)
{
	float *pfSource;
	unsigned char *pucSource;
	unsigned char *pucSource2;
	unsigned char *pucDestination;
	unsigned char *pucTempArray;
	unsigned int nuiBusyTimeCounter;
	unsigned char nucIndexNumber;
	unsigned char ucTempData;
	unsigned short usCrc;
	unsigned char ucFlowControl;
	unsigned char ucAddressLow;
	unsigned char ucAddressHigh;
	int i;
	int j;
	int nb;

// ucAddressHigh -
// если в старшем регистре адреса Modbus бит7 = 0, то запрашиваются реперные точки - (бит0 - бит6) - адрес аналогового входа.
// если в старшем регистре адреса Modbus бит7 = 1, то запрашивается ТХС и (бит0 - бит2) - относительный адрес модуля МВСТ3.
// ucAddressLow - требуемое количество аналоговых входов.

	ucAddressLow = (unsigned char)(pxModbusMapping ->
								   current_message_address_common);
	ucAddressHigh = (unsigned char)((pxModbusMapping ->
									 current_message_address_common) >> 8);
	pucDestination = pxModbusMapping -> buffer_pointer;

	// запрос ТХС или реперных точек?
	if (ucAddressHigh & MVAI5_TXS_REQUEST_MASK)
	{
// запрос ТХС.
//        cout << "TXS" << endl;
		// вычислим индекс модуля в массиве контекста.
		nucIndexNumber =
			(((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
			  xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
			 (ucAddressHigh & ANALOGUE_MODULE_ADDRESS_MASK));

//            printf("vDeviceControlOnlineDataReadHandler ucAddressLow %d\n\r", (ucAddressLow));
//            printf("vDeviceControlOnlineDataReadHandler ucAddressHigh %d\n\r", (ucAddressHigh & ANALOGUE_MODULE_ADDRESS_MASK));
//            printf("vDeviceControlOnlineDataReadHandler ucLastAnalogueInputModuleIndex %d\n\r", (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex));
//            printf("vDeviceControlOnlineDataReadHandler ucServiceAnalogueInputModuleQuantity %d\n\r", (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity));
//            printf("vDeviceControlOnlineDataReadHandler nucIndexNumber %d\n\r", (nucIndexNumber));
		iModuleMvai5TxsRead(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);

		sprintf((char*)pucDestination,
				"%7.4f",
				((TTxsData*)(xAllModulesContext.axAllModulesContext[nucIndexNumber].
							 xModuleContextStatic.
							 pucTxsBufferPointer)) -> fTxsValue);

		pucDestination += ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY;
		*pucDestination = ((TTxsData*)(xAllModulesContext.axAllModulesContext[nucIndexNumber].
									   xModuleContextStatic.
									   pucTxsBufferPointer)) -> ucTxsStat;

////           cout << "vDeviceControlOnlineDataReadHandler aucStatAiByteArray" << endl;
//           cout << "vDeviceControlOnlineDataReadHandler ucAddressHigh" << (int)ucAddressHigh << endl;
//           cout << "vDeviceControlOnlineDataReadHandler nucIndexNumber" << (int)nucIndexNumber << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)pucDestination;
//            for(int i=0; i<8; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }

		pxModbusMapping -> current_message_nb_common =
			(ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY +
			 ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY);
	}
	else
	{
// запрос реперных точек.

		// адрес аналогового входа принадлежит внутреннему или внешнему модулю?
		if ((xPlcConfigService.
				xPlcConfigServiceData.
				ucServiceAnalogueInputModuleQuantity) >=
				(((ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK) /
				  MVAI5_ANALOG_INPUT_QUANTITY) + 1))
		{
			// адрес аналогового входа принадлежит внутреннему модулю.
//            cout << "GENERAL" << endl;
			// вычислим индекс модуля в массиве контекста.
			nucIndexNumber =
				(((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
				  xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
				 ((ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK) /
				  MVAI5_ANALOG_INPUT_QUANTITY));

			iModuleMvai5ReperPointsAdcRead(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
			// получим адрес значения аналогового входа.
//            pfSource = (float*)&aucHoldingRegistersArray[AI_VALUE_BYTE_ARRAY_OFFSET +
//                       ((ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK) *
//                        sizeof(float))];
			pfSource = &afAnalogueInputData[(ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK)];
			// получим адрес значения состояние канала аналогового входа.
			pucSource2 = &aucStatAiByteArray[ucAddressHigh];
//            cout << "_FC_ONLINE_DATA_READ" << endl;
			for (i = 0; i < (ucAddressLow); i++)
			{
				// преобразуем первые 7 цифр float значения, в ASCII символы, для отображения в строковом виде в программаторе.
				sprintf((char*)pucDestination,
						"%7.4f",
						pfSource[i]);
//                cout << "_FC_ONLINE_DATA_READ 1" << endl;
				pucDestination += ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY;
				// получим значения состояния канала аналогового входа.
				*pucDestination = *pucSource2++;
//                cout << "_FC_ONLINE_DATA_READ 2" << endl;
				pucDestination += ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY;
//                cout << "_FC_ONLINE_DATA_READ 3" << endl;
			}

//if (aucStatAiByteArray[ucAddressHigh + 1] == 0)
//{
////           cout << "vDeviceControlOnlineDataReadHandler aucStatAiByteArray" << endl;
//           cout << "vDeviceControlOnlineDataReadHandler ucAddressLow" << (int)ucAddressLow << endl;
//           cout << "vDeviceControlOnlineDataReadHandler ucAddressHigh" << (int)ucAddressHigh << endl;
//           cout << "vDeviceControlOnlineDataReadHandler nucIndexNumber" << (int)nucIndexNumber << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&aucStatAiByteArray[ucAddressHigh];//pxModbusMapping -> buffer_pointer;//
//            for(int i=0; i<8; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
//}

			memcpy(pucDestination,
				   xAllModulesContext.axAllModulesContext[nucIndexNumber].
				   xModuleContextStatic.
				   pucReperPointsAdcBufferPointer,
				   ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH);

			pxModbusMapping -> current_message_nb_common =
				((ucAddressLow *
				  (ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY +
				   ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY)) +
				 ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH);
		}
		else
		{
			// адрес аналогового входа принадлежит внешнему модулю.
//            cout << "LSNU" << endl;
			// получим адрес значения аналогового входа.
//            pfSource = (float*)&aucHoldingRegistersArray[AI_VALUE_BYTE_ARRAY_OFFSET +
//                       ((ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK) *
//                        sizeof(float))];
			pfSource = &afAnalogueInputData[(ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK)];
			// получим адрес значения состояние канала аналогового входа.
			pucSource2 = &aucStatAiByteArray[ucAddressHigh];

			for (i = 0; i < (ucAddressLow); i++)
			{
				// преобразуем первые 7 цифр float значения, в ASCII символы, для отображения в строковом виде в программаторе.
				sprintf((char*)pucDestination,
						"%7.4f",
						pfSource[i]);
				pucDestination += ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY;
				// получим значения состояния канала аналогового входа.
				*pucDestination = *pucSource2++;
				pucDestination += ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY;
			}
			pxModbusMapping -> current_message_nb_common =
				(ucAddressLow *
				 (ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY +
				  ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY));
		}
	}

	pxModbusMapping -> current_message_address_common = 0;

	if (ucModuleError ==
			PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_TXS_READ_ERROR)
	{
		pxModbusMapping -> current_message_address_common =
			PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_TXS_READ_ERROR;
		cout << "PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_TXS_READ_ERROR" << endl;
		ucModuleError = 0;
	}

	if (ucModuleError ==
			MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR)
	{
		pxModbusMapping -> current_message_address_common =
			MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR;
		cout << "MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR" << endl;
		ucModuleError = 0;
	}

}

//-----------------------------------------------------------------------------------------------------
// обрабатывает входящие сообщения от Modbus интерфейсов по 70 функции - УСТАНОВКА ТЕКУЩЕГО ВРЕМЕНИ ВО ВСЕ ПРИБОРЫ СЕТИ .
// источник - ПАС-05_И3_10 2013.pdf.
// 4.3.11 ФУНКЦИЯ 70: УСТАНОВКА ТЕКУЩЕГО ВРЕМЕНИ ВО ВСЕ ПРИБОРЫ СЕТИ
// Запрос
// Широковещательный запрос (адрес=0), адресован всем SL сети. Передается 7
// байтов.
// Байт 1 - секунда 0 – 59;
// Байт 2 - минута 0 –59;
// Байт 3 - час 0 – 23;
// Байт 4 - день 1 – 31;
// Байт 5 - месяц 1 – 12.
// Байт 6 - год 0 – 99.
// Байт 7 – день недели 1 – 7.
// Ответ
// На данный запрос SL не отвечает.
void vCurrentTimeSet(modbus_mapping_t *pxModbusMapping)
{
//    time_t rawtime;
//    struct tm *timeinfo;
//    struct timeval systime;
//    unsigned char *pucSource;
//    unsigned short *pusDestination;
//    // получим указатель на буфер с данными времени пришедшими по Modbus.
//    pucSource = (unsigned char*)(pxModbusMapping -> buffer_pointer);
////    // получим указатель на буфер с текущим временем в рабочем массиве прибора.
////    pusDestination = (unsigned short*)&aucInputRegistersArray[CURRENT_TIME_BYTE_ARRAY_OFFSET];
////    // скопируем данные времени пришедшие по Modbus в рабочий массив прибора.
////    for(int i = 0; i < CURRENT_TIME_BYTE_QUANTITY; i++)
////    {
////        pusDestination[i] = pucSource[i];
////    }
//
//    rawtime = time(NULL);
//    time ( &rawtime );
//    timeinfo = localtime ( &rawtime );
//
//    timeinfo -> tm_sec = pucSource[CURRENT_TIME_SECOND_OFFSET];
//    timeinfo -> tm_min = pucSource[CURRENT_TIME_MINUTE_OFFSET];
//    timeinfo -> tm_hour = pucSource[CURRENT_TIME_HOUR_OFFSET];
//    timeinfo -> tm_mday = pucSource[CURRENT_TIME_MONTH_DAY_OFFSET];
//    (timeinfo -> tm_mon) = pucSource[CURRENT_TIME_MONTH_OFFSET] - 1;
//    (timeinfo -> tm_year) = pucSource[CURRENT_TIME_YEAR_OFFSET] - 1900 + 2000;
//    timeinfo -> tm_wday = pucSource[CURRENT_TIME_WEEK_DAY_OFFSET];
//
//    systime =
//    {
//        mktime(timeinfo),
//        0
//    };
//    settimeofday(&systime, NULL);

	vLinuxCurrentTimeSet((unsigned char*)(pxModbusMapping -> buffer_pointer));

	pxModbusMapping -> current_message_nb_common =
		CURRENT_TIME_BYTE_QUANTITY;
}
