
#include "PasNewConfig.h"
#include "DeviceControl.h"
#include "Task.h"
#include "SystemComponent.h"
#include "MainProductionCycle.h"

using namespace std;

extern CMainProductionCycle xMainProductionCycle;

//-----------------------------------------------------------------------------------------------------
// ������������ �������� ��������� �� Modbus �����������,
// ��������� ���������� � ������� modbus_reply() modbus.cpp.

void vDeviceControlModbusMessageHandler(modbus_mapping_t *pxModbusMapping)
{
	// ��������� ���������?
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
		// ��������� ����������, ����� ���������.
		pxModbusMapping -> message_ready = 0;
	}
}

//-----------------------------------------------------------------------------------------------------
// ������������ �������� ��������� �� Modbus �����������, �� 5 �������.
void vDeviceControlModbusFunction5Handler(modbus_mapping_t *pxModbusMapping)
{
	unsigned char nucIndexNumber;
	int i;

//-----------------------------------------------------------------------------------------------------
// Function 5
	// �������, �� ������ ������ ������������ ���.
	switch(pxModbusMapping->current_message_address_bits -
			COILS_ARRAY_MODBUS_BEGIN_ADDRESS)
	{
		// ������������ � �������� ������.
	case DEVICE_CONTROL_PC_KVIT:
		// ��� ���������� ��� �������?
		if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
		{
			// ���� ���������� - �������.
			pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
			// ��������� ���� ������������ ���� �������.
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

		// ����� � �������� ������.
	case DEVICE_CONTROL_PC_RESET:
		// ��� ���������� ��� �������?
		if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
		{
			// ���� ���������� - �������.
			pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
			// ��������� ���� ������ ���� �������.
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

		// ���������� � �������� ������.
	case DEVICE_CONTROL_BLOCK:
		// ��� ���������� ��� �������?
		if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
		{
			// ���� ���������� - �������.
			pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//            // ��������� ���� ���������� ���� �������.
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
//            // ������� ���� ���������� ���� �������.
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
	// �������, �� ������ ������ ������������ ���.
	switch(((pxModbusMapping->current_message_address_bits -
			 COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & 0xFF00))
	{
		// ���������-���������� ������ ����������.
	case DEVICE_CONTROL_CALIBRATION_ON_OFF:
		// ����� ���� ��� ���� ������ �� ����� �� ��������� ���������� ����?
		if ((xMainFlagRegister.ui8AinOffProcessOwnerIndex == AIN_OFF_PROCESS_OWNER_IS_NONE) ||
				(xMainFlagRegister.ui8AinOffProcessOwnerIndex == AIN_OFF_PROCESS_OWNER_IS_PROGRAMMER))
		{
//        // �������� ������ ������ � ������� ���������, � �������� ��������� ������� - ����������.
//        nucIndexNumber = (((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
//                           xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
//                          ((((pxModbusMapping->current_message_address_bits -
//                              COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) - 1));
			// �������� ������ ������ � ������� ���������, � �������� ��������� ������� - ����������.
			nucIndexNumber = (((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
							   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
							  (((pxModbusMapping->current_message_address_bits -
								 COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4));
			// �� ������� - nucIndexNumber ���� ������?
			if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex))
			{
				// ��� ���������� ��� �������?
				if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
				{
					// ��������� ������ ����������.
					cout << "DEVICE_CONTROL_CALIBRATION_ON" << endl;
//                cout << "DEVICE_CONTROL_CALIBRATION_ON nucIndexNumber " << (int)nucIndexNumber << endl;
//                cout << "DEVICE_CONTROL_CALIBRATION_ON current_message_address_bits " << (int)(pxModbusMapping->current_message_address_bits -
//                        COILS_ARRAY_MODBUS_BEGIN_ADDRESS) << endl;
					// ���� ���������� - �������.
					pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
					// ��������� �������� ������ ����� ������������ �����.
					// � DO-D2 - � ����� � ������ (0-��� ������ ����������).
					xAllModulesContext.axAllModulesContext[nucIndexNumber].
					xModuleContextDinamic.
					ucCommonIndex = (((pxModbusMapping->current_message_address_bits -
									   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_INPUT_NUMBER_MASK));
					// ����� � ������� ��������� ��� ���������� �� ��������� ���������� �����.
					for (i = 0; i < MAX_HANDLED_ANALOGUE_INPUT; i++)
					{
						aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET + i] = 0;
					}
//            // ������� �� ��������� ���������� ���� - Y, ������ - X.
//            // � ������ Modbus 0x00XY, X - ����� ������, Y - ����� ������������ �����.
//            aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET +
//                          ((((((pxModbusMapping->current_message_address_bits - // ��������� ����� ������
//                                COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
//                               CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) - 1) *
//                            ANALOG_MODULE_INPUT_QUANTITY) +
//
//                           (((pxModbusMapping->current_message_address_bits - // ��������� ����� �����
//                              COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
//                             CALIBRATION_INPUT_NUMBER_MASK) - 1))] = 1;

					// ������� �� ��������� ���������� ���� - Y, ������ - X.
					// � ������ Modbus 0x00XY, X - ����� ������, Y - ����� ������������ �����.
					aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET +
								  (((((pxModbusMapping->current_message_address_bits - // ��������� ����� ������
									   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
									  CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) *
									ANALOG_MODULE_INPUT_QUANTITY) +

								   (((pxModbusMapping->current_message_address_bits - // ��������� ����� �����
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
									 CALIBRATION_INPUT_NUMBER_MASK) - 1))] = 1;

					xMainFlagRegister.ui8AinOffProcessOwnerIndex = AIN_OFF_PROCESS_OWNER_IS_PROGRAMMER;
				}
				else
				{
					// ���������� ������ ����������.
					cout << "DEVICE_CONTROL_CALIBRATION_OFF" << endl;
//                cout << "DEVICE_CONTROL_CALIBRATION_ON nucIndexNumber " << (int)nucIndexNumber << endl;
//                cout << "DEVICE_CONTROL_CALIBRATION_ON current_message_address_bits " << (int)(pxModbusMapping->current_message_address_bits -
//                        COILS_ARRAY_MODBUS_BEGIN_ADDRESS) << endl;
					// ��������� �������� ������ ����� ������������ �����. ���� 0, ���������� ���������.
					// � DO-D2 - � ����� � ������ (0-��� ������ ����������).
					xAllModulesContext.axAllModulesContext[nucIndexNumber].
					xModuleContextDinamic.
					ucCommonIndex = 0;
					// ����� � ������� ��������� ��� ���������� �� ��������� ���������� �����.
					for (i = 0; i < MAX_HANDLED_ANALOGUE_INPUT; i++)
					{
						aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET + i] = 0;
					}
// ����� � ������� ��������� ���������� ���� - Y, ������ - X.
//            aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET +
//                          ((((((pxModbusMapping->current_message_address_bits - // ��������� ����� ������
//                                COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
//                               CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) - 1) *
//                            ANALOG_MODULE_INPUT_QUANTITY) +
//
//                           (((pxModbusMapping->current_message_address_bits - // ��������� ����� �����
//                              COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
//                             CALIBRATION_INPUT_NUMBER_MASK) - 1))] = 0;


// ����� � ������� ��������� ���������� ���� - Y, ������ - X.
//            aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET +
//                          (((((pxModbusMapping->current_message_address_bits - // ��������� ����� ������
//                                COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
//                               CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) *
//                            ANALOG_MODULE_INPUT_QUANTITY) +
//
//                           (((pxModbusMapping->current_message_address_bits - // ��������� ����� �����
//                              COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
//                             CALIBRATION_INPUT_NUMBER_MASK) - 1))] = 0;

					xMainFlagRegister.ui8AinOffProcessOwnerIndex = AIN_OFF_PROCESS_OWNER_IS_NONE;
				}
			}
		}
		break;

		// ���������� ������ ����� - ���.
	case DEVICE_CONTROL_SET_BOTTOM_OF_SCALE:
//        // �������� ������ ������ � ������� ���������, � �������� ��������� �������.
//        nucIndexNumber = (((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
//                           xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
//                          ((((pxModbusMapping->current_message_address_bits -
//                              COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) - 1));
		// �������� ������ ������ � ������� ���������, � �������� ��������� �������.
		nucIndexNumber = (((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
						   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
						  (((pxModbusMapping->current_message_address_bits -
							 COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4));
		// �� ������� - nucIndexNumber ���� ������?
		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex))
		{
			// ��� ���������� ��� �������?
			if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
										  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
			{
				// ���� ���������� - �������.
				pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
										  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
				// ��������� �������� ������ ������� - ���������� ������ ����� - ���.
				xAllModulesContext.axAllModulesContext[nucIndexNumber].
				xModuleContextDinamic.
				ucCommandControl = MVAI5_COMMAND_CONTROL_SET_BOTTOM_OF_SCALE;
			}
			else
			{

			}
		}
		break;

		// ���������� ����� ����� - ���.
	case DEVICE_CONTROL_SET_TOP_OF_SCALE:
//        // �������� ������ ������ � ������� ���������, � �������� ��������� �������.
//        nucIndexNumber = (((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
//                           xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
//                          ((((pxModbusMapping->current_message_address_bits -
//                              COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) - 1));
		// �������� ������ ������ � ������� ���������, � �������� ��������� �������.
		nucIndexNumber = (((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
						   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
						  (((pxModbusMapping->current_message_address_bits -
							 COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4));
		// �� ������� - nucIndexNumber ���� ������?
		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex))
		{
			// ��� ���������� ��� �������?
			if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
										  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
			{
				// ���� ���������� - �������.
				pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
										  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
				// ��������� �������� ������ ������� - ���������� ����� ����� - ���.
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
// ��������� ������� - SP, OUT, ����������� ������� �������� ������ - MTVI5.
	// ����� ������������� ���� ��������� � ��������� ���������� ������� - SP, OUT, ����������� ������� �������� ������?
	if (((pxModbusMapping -> current_message_address_bits -
			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= INCREMENT_SP_OUT_BIT_ARRAY_OFFSET) &&
			((pxModbusMapping -> current_message_address_bits -
			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (INCREMENT_SP_OUT_BIT_ARRAY_OFFSET +
					  INCREMENT_SP_OUT_BIT_ARRAY_LENGTH)))
	{
		// ��� ���������� ��� �������?
		if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
		{
			// ���� ���������� - �������.
			pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
			cout << "INCREMENT_SP_OUT_1__BIT_ARRAY_OFFSET 1" << endl;
			// ��������� ���� ����������-���������� ������� - SP, OUT, �����������.
			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1; //?
		}
		else
		{
			cout << "INCREMENT_SP_OUT_1__BIT_ARRAY_OFFSET 0" << endl;
			// ������� ���� ����������-���������� ������� - SP, OUT, �����������.
			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0; //?
		}

		// �������� ������ ������ � ������� ���������, � �������� ��������� �������.
		nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
							((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
							 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
						   ((unsigned char)(((pxModbusMapping->current_message_address_bits -
											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
											 INCREMENT_SP_OUT_BIT_ARRAY_OFFSET) /
											(MTVI5_ANALOG_OUTPUT_QUANTITY))));

		// �� ������� - nucIndexNumber ���� ������?
		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
		{
//        if (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex)
//        {
			// �������� ����� ���������� ������ �������� ������, � ������� ��������������-�������������� �������.
			xAllModulesContext.axAllModulesContext[nucIndexNumber].
			xModuleContextDinamic.
			ucCommonIndex = (unsigned char)((((pxModbusMapping->current_message_address_bits -
											   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
											  INCREMENT_SP_OUT_BIT_ARRAY_OFFSET) %
											 MTVI5_ANALOG_OUTPUT_QUANTITY) +
											CONVERT_INTEGER_TO_NATURAL_NUMBER);

// � �������� ������ �������� ������, ������ ������� - SP, OUT,
// ��������� �� �������� ������� axAnalogueOutputRegulatorContAv[�������� �� ����� ����������],
// ����� ��������� ���� �� � ������������. �� ����� �� �������, �� ��������� Modbus,
// ���������� �������� ������(HMI ������, ������� SCADA, �������������) �������� ������ ��� ����������� �� �������.
//  �� ������� ���� ����� ������� �������� � ������� �� SPI ���������� �������� � ������ ������������� ������-������.
// ����� ������ ����� ������ �� ������ ������ �������� ������ �� ��������� ���� ������-������.
// � ���� �� ������� ���� ������ � ������ ������ ������ ������� �������, �� ������� ��������� �� ���������.
//  �������, ����� ������ � ������ �������� �������, ���������� �� �������� � ���������� usAuxiliaryCounter(���������� � ���������),
// ��� �� ������������ � ������ ��������.
//  ��� ��, ��� ������ ��� ����������� ����������� ������������ �������� ������, ��������� ������.
// ��� ��� ����� �������� ������ �������� ������ ���������� ���� ��� � �������,
// � ������ vDeviceControlModbusMessageHandler(), �������� - ����� ��������� ������ ���������� ������,
// �� �� ��� �����, �������� �������� ������ ����, ����� ����������������-���������������� �������� �������
// N - ���. �������� - N ���������� � ���������� usAuxiliaryCounter. � ����� ������� ����� ������ �������
// �������� � �������, ���������� usAuxiliaryCounter - ����������.
			// ������� �������� ����������, �� ������� � �������� ������ ��������� �������� �������.
			xAllModulesContext.axAllModulesContext[nucIndexNumber].
			xModuleContextDinamic.
			usAuxiliaryCounter += 1;

			// ��������� �������� ������ ������� - ����������������.
			xAllModulesContext.axAllModulesContext[nucIndexNumber].
			xModuleContextDinamic.
			ucCommandControl = DEVICE_CONTROL_MTVI5_INCREMENT_SP_OUT;
		}
	}

//-----------------------------------------------------------------------------------------------------
// ��������� ������� - SP, OUT, ����������� ������� �������� ������ - MTVI5.
// ����� ������������� ���� ��������� � ��������� ���������� ������� - SP, OUT, ����������� ������� �������� ������?
	if (((pxModbusMapping -> current_message_address_bits -
			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= DECREMENT_SP_OUT_BIT_ARRAY_OFFSET) &&
			((pxModbusMapping -> current_message_address_bits -
			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (DECREMENT_SP_OUT_BIT_ARRAY_OFFSET +
					  DECREMENT_SP_OUT_BIT_ARRAY_LENGTH)))
	{
		// ��� ���������� ��� �������?
		if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
		{
			// ���� ���������� - �������.
			pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
			cout << "DECREMENT_SP_OUT_BIT_ARRAY_OFFSET 1" << endl;
			// ��������� ���� ����������-���������� ������� - SP, OUT, �����������.
			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1; //?
		}
		else
		{
			cout << "DECREMENT_SP_OUT_BIT_ARRAY_OFFSET 0" << endl;
			// ������� ���� ����������- ���������� ������� - SP, OUT, �����������.
			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0; //?
		}

		// �������� ������ ������ � ������� ���������, � �������� ��������� �������.
		nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
							((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
							 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
						   ((unsigned char)(((pxModbusMapping->current_message_address_bits -
											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
											 DECREMENT_SP_OUT_BIT_ARRAY_OFFSET) /
											(MTVI5_ANALOG_OUTPUT_QUANTITY))));
		// �� ������� - nucIndexNumber ���� ������?
		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
		{
			// �������� ����� ���������� ������ �������� ������, � ������� ��������������-�������������� �������.
			xAllModulesContext.axAllModulesContext[nucIndexNumber].
			xModuleContextDinamic.
			ucCommonIndex = (unsigned char)((((pxModbusMapping->current_message_address_bits -
											   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
											  DECREMENT_SP_OUT_BIT_ARRAY_OFFSET) %
											 MTVI5_ANALOG_OUTPUT_QUANTITY) +
											CONVERT_INTEGER_TO_NATURAL_NUMBER);

// � �������� ������ �������� ������, ������ ������� - SP, OUT,
// ��������� �� �������� ������� axAnalogueOutputRegulatorContAv[�������� �� ����� ����������],
// ����� ��������� ���� �� � ������������. �� ����� �� �������, �� ��������� Modbus,
// ���������� �������� ������(HMI ������, ������� SCADA, �������������) �������� ������ ��� ����������� �� �������.
//  �� ������� ���� ����� ������� �������� � ������� �� SPI ���������� �������� � ������ ������������� ������-������.
// ����� ������ ����� ������ �� ������ ������ �������� ������ �� ��������� ���� ������-������.
// � ���� �� ������� ���� ������ � ������ ������ ������ ������� �������, �� ������� ��������� �� ���������.
//  �������, ����� ������ � ������ �������� �������, ���������� �� �������� � ���������� usAuxiliaryCounter(���������� � ���������),
// ��� �� ������������ � ������ ��������.
//  ��� ��, ��� ������ ��� ����������� ����������� ������������ �������� ������, ��������� ������.
// ��� ��� ����� �������� ������ �������� ������ ���������� ���� ��� � �������,
// � ������ vDeviceControlModbusMessageHandler() - �������� ����� ��������� ������ ���������� ������,
// �� �� ��� �����, �������� �������� ������ ����, ����� ����������������-���������������� �������� �������
// N - ���. �������� - N ���������� � ���������� usAuxiliaryCounter. � ����� ������� ����� ������ �������
// �������� � �������, ���������� usAuxiliaryCounter - ����������.
			// ������� �������� ����������, �� ������� � �������� ������ ��������� �������� �������.
			xAllModulesContext.axAllModulesContext[nucIndexNumber].
			xModuleContextDinamic.
			usAuxiliaryCounter += 1;

			// ��������� �������� ������ ������� - ����������������.
			xAllModulesContext.axAllModulesContext[nucIndexNumber].
			xModuleContextDinamic.
			ucCommandControl = DEVICE_CONTROL_MTVI5_DECREMENT_SP_OUT;
		}
	}

//-----------------------------------------------------------------------------------------------------
// ������ ���������� �������� �����������(CONT_ST), ������� �������� ������ - MTVI5.
	// ����� ������������� ���� ��������� � ��������� ������� ���������� �������� �����������(CONT_ST) ������� �������� ������?
	if (((pxModbusMapping -> current_message_address_bits -
			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= CONT_ST_BIT_ARRAY_OFFSET) &&
			((pxModbusMapping -> current_message_address_bits -
			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (CONT_ST_BIT_ARRAY_OFFSET +
					  CONT_ST_BIT_ARRAY_LENGTH)))
	{
		// ��� ���������� ��� �������?
		if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
		{
			// ���� ���������� - �������.
			pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
			cout << "CONT_ST_BIT_ARRAY 1" << endl;
			// ��������� ��� ���������� �������� �����������.
//            aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//                           COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1; //?
			aucContStWriteBitArray[(pxModbusMapping -> current_message_address_bits -
									COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
								   CONT_ST_BIT_ARRAY_OFFSET] = 1;
		}
		else
		{
			cout << "CONT_ST_BIT_ARRAY 0" << endl;
			// ������� ��� ���������� �������� �����������.
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

		// �������� ������ ������ � ������� ���������, � �������� ��������� �������.
		nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
							((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
							 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
						   ((unsigned char)(((pxModbusMapping->current_message_address_bits -
											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
											 CONT_ST_BIT_ARRAY_OFFSET) /
											(MTVI5_STAT_BIT_QUANTITY * MTVI5_ANALOG_OUTPUT_QUANTITY))));
		// �� ������� - nucIndexNumber ���� ������?
		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
		{
//            // ��������� �������� ������ ������� - ������ ���������� �������� �����������.
//            xAllModulesContext.axAllModulesContext[nucIndexNumber].
//            xModuleContextDinamic.
//            ucCommandControl = DEVICE_CONTROL_MTVI5_CONT_ST_WRITE;
		}
	}

//-----------------------------------------------------------------------------------------------------
// ������ ������ ���������� �������� ������.
	// ����� ������������� ���� ��������� � ��������� ������� ������ ���������� �������� ������?
	if (((pxModbusMapping -> current_message_address_bits -
			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= LOG_COM_BIT_ARRAY_OFFSET) &&
			((pxModbusMapping -> current_message_address_bits -
			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (LOG_COM_BIT_ARRAY_OFFSET +
					  LOG_COM_BIT_ARRAY_LENGTH)))
	{
		// ��� ���������� ��� �������?
		if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
		{
			// ���� ���������� - �������.
			pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
			cout << "LOG_COM_BIT_ARRAY_OFFSET 1" << endl;
			// ��������� ��� ������ ���������� �������� ������.
			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1;
		}
		else
		{
			cout << "LOG_COM_BIT_ARRAY_OFFSET 0" << endl;
			// ������� ��� ������ ���������� �������� ������.
			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0;
		}
	}

//-----------------------------------------------------------------------------------------------------
// ������� �� ��������� ���������� ����.
//    // ��������� ������ � ���������� ModbusRTU HMI?
//    if ((pxModbusMapping -> message_sourse) ==
//            MODBUS_MESSAGE_SOURCE_RTU_HMI)
	//    {
	// ����� ������������� ���� ��������� � ��������� ������� ������ �� ��������� ���������� ������?
	if (((pxModbusMapping -> current_message_address_bits -
			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= AIN_OFF_BIT_ARRAY_OFFSET) &&
			((pxModbusMapping -> current_message_address_bits -
			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (AIN_OFF_BIT_ARRAY_OFFSET +
					  AIN_OFF_BIT_ARRAY_LENGTH)))
	{
		// ����� ���� ��� ���� ������ �� ����� �� ��������� ���������� ����?
		if ((xMainFlagRegister.ui8AinOffProcessOwnerIndex == AIN_OFF_PROCESS_OWNER_IS_NONE) ||
				(xMainFlagRegister.ui8AinOffProcessOwnerIndex == AIN_OFF_PROCESS_OWNER_IS_HMI))
		{
			// �������� ������ ������ � ������� ���������, � �������� ��������� �������.
			nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex) + 1) -
								(xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)) +
							   ((unsigned char)(((pxModbusMapping->current_message_address_bits -
												  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
												 AIN_OFF_BIT_ARRAY_OFFSET) /
												(MVAI5_ANALOG_INPUT_QUANTITY))));
			// �� ������� - nucIndexNumber ���� ������?
			if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex))
			{
				// ����� � ������� ��������� ���������� �� ��������� ���������� ����.
				for (i = 0; i < MAX_HANDLED_ANALOGUE_INPUT; i++)
				{
					// ���������� ���� - i ������� �� ���������?
					if (aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET + i])
					{
						// ����� � ������� ��������� ���������� �� ��������� ���������� ���� - i.
						aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET + i] = 0;
//                        // �������� ������ ������ � ������� ���������, ������� � ������� ������ ��������� �������.
//                        nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex) + 1) -
//                                            (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)) +
//                                           ((unsigned char)((i) /
//                                                            (MVAI5_ANALOG_INPUT_QUANTITY))));
						// ��������� �������� ������ ����� ������������ �����. ���� 0, ���������� ���������.
						// �������� ���������� ���� ������ ������;
						xAllModulesContext.axAllModulesContext[
							((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex) + 1) -
							  (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)) +
							 ((unsigned char)((i) /
											  (MVAI5_ANALOG_INPUT_QUANTITY))))].
						xModuleContextDinamic.
						ucCommonIndex = 0;
					}
				}

				// ��� ���������� ��� �������?
				if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
				{
					// ��������� ������ ����������.
					cout << "AIN_OFF_BIT_ARRAY_OFFSET 1" << endl;
					// ���� ���������� - �������.
					pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
					// �������� � ��������� �������� ������ ����� ������������ �����.
					xAllModulesContext.axAllModulesContext[nucIndexNumber].
					xModuleContextDinamic.
					ucCommonIndex = (unsigned char)((((pxModbusMapping->current_message_address_bits -
													   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
													  AIN_OFF_BIT_ARRAY_OFFSET) %
													 MVAI5_ANALOG_INPUT_QUANTITY) +
													CONVERT_INTEGER_TO_NATURAL_NUMBER);
					// ������� ���������� ���� �� ���������.
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
					// ���������� ������ ����������.
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
// ��������� ������� - ������ ������������.
	if (((pxModbusMapping -> current_message_address_bits -
			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) ==
			CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_OFFSET))
	{
		// ��� ���������� ��� �������?
		if (pxModbusMapping->tab_bits[pxModbusMapping->current_message_address_bits -
									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
		{
			// ���� ���������� - �������.
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
				// ���������� ������������ � ���� ������.
				iPlcConfigurationSave();
				iPlcDataBaseSave();
//    vWorkArraysUpdate();
				iBadModuleBuffClear();
				// ����� ������ ������ ������������ � ���� ������,
				// ��� �������� �� ���������� ����������� ������.
				fbPlcConfigurationError = 0;
				fbPlcConfigurationFileError = 0;
				fbPlcDataBaseFileError = 0;
				// ��������� ���� "������������ ���������".
				fbPlcErrorReset = 1;
				xPlcConfigDataTransmit.xConfigData.ucErrorCode = 0;

				// �������������� �������.
				CEvents::EventRegistration(
					0,
					(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
					 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
					0,
					"����� ����. � ��.");

				cout << "CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE 2" << endl;
				break;
			case CONFIGURATION_ERROR_NEW_CONFIGURATION:
//            iPlcConfigurationCreate();
//            vMainTaskMemoryMapFormation();
				// ���������� ������������ � ���� ������.
				iPlcConfigurationSave();
				//                iPlcDataBaseSave();
//    vWorkArraysUpdate();
				iBadModuleBuffClear();
				// ����� ������ ������ ������������ � ���� ������,
				// ��� �������� �� ���������� ����������� ������.
				fbPlcConfigurationError = 0;
				fbPlcConfigurationFileError = 0;
				fbPlcDataBaseFileError = 0;
				// ��������� ���� "������������ ���������".
				fbPlcErrorReset = 1;
				xPlcConfigDataTransmit.xConfigData.ucErrorCode = 0;

				// �������������� �������.
				CEvents::EventRegistration(
					0,
					(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
					 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
					0,
					"����� ������������.");

				cout << "CONFIGURATION_ERROR_NEW_CONFIGURATION 3" << endl;
				break;
			case CONFIGURATION_ERROR_BAD_MODULE_FINDED_AND_REGISTERED:
				// ����� ������ ������ ������������ � ���� ������,
				// ��� �������� �� ���������� ����������� ������.
				fbPlcConfigurationError = 0;
				fbPlcConfigurationFileError = 0;
				fbPlcDataBaseFileError = 0;
				// ��������� ���� "������������ ���������".
				fbPlcErrorReset = 1;
				break;

			default:
				break;
			};

		}
		else
		{
			cout << "CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_OFFSET 0" << endl;
			// ������� ��� ������ - ������������.
		}
	}
}

//-----------------------------------------------------------------------------------------------------
// ������������ �������� ��������� �� Modbus ����������� �� 15 �������.
// ������ N - ������ ���������� �������� ������.
void vDeviceControlModbusFunction15Handler(modbus_mapping_t *pxModbusMapping)
{
//-----------------------------------------------------------------------------------------------------
// Function 15
//-----------------------------------------------------------------------------------------------------
	// ����� ������������ ��� ��������� � ��������� ������� ������ ���������� �������� ������?
	if (((pxModbusMapping -> current_message_address_bits -
			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= LOG_COM_BIT_ARRAY_OFFSET) &&
			((pxModbusMapping -> current_message_address_bits -
			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (LOG_COM_BIT_ARRAY_OFFSET +
					  LOG_COM_BIT_ARRAY_LENGTH)))
	{
		// ���������� ������������ ��� �� ������� �� ������� ������� ������?
		if (((pxModbusMapping -> current_message_address_bits -
				COILS_ARRAY_MODBUS_BEGIN_ADDRESS) +
				(pxModbusMapping -> current_message_nb_bits)) <
				(LOG_COM_BIT_ARRAY_OFFSET +
				 LOG_COM_BIT_ARRAY_LENGTH))
		{
			// ������� N - ������ ���������� �������� ������.
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
// ������������ �������� ��������� �� Modbus ����������� �� 16 �������.
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
// ���������� ���������� ���������� ����������� ������� �������� ������.
	// ����� ������������ ��������� ��������� � ��������� ������� ���������� ���������� ����������� ������� �������� ������ - MTVI5?
	if (((((pxModbusMapping -> current_message_address_registers) * 2) -
			HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) >= CONT_AV_BIG_ENDIAN_BYTE_ARRAY_OFFSET) &&
			((((pxModbusMapping -> current_message_address_registers) * 2) -
			  HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) < (CONT_AV_BIG_ENDIAN_BYTE_ARRAY_OFFSET +
					  CONT_AV_BIG_ENDIAN_BYTE_ARRAY_LENGTH)))
	{
		// ���������� ������������ ��������� �� ������� �� ������� ������� ������?
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

			// �������� ������ ������ � ������� ���������, � �������� ��������� �������.
			nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
								((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
								 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
							   ((unsigned char)((((((pxModbusMapping -> current_message_address_registers) * 2) -
												   HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) -
												  CONT_AV_BIG_ENDIAN_BYTE_ARRAY_OFFSET) /
												 (sizeof(struct TMtvi5ContAvDataPackOne) *
												  MTVI5_ANALOG_OUTPUT_QUANTITY)))));

			// �� ������� - nucIndexNumber ���� ������?
			if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
			{
				// �������� ����� ���������� ������ �������� ������.
				xAllModulesContext.axAllModulesContext[nucIndexNumber].
				xModuleContextDinamic.
				ucCommonIndex = (unsigned char)(((((((pxModbusMapping -> current_message_address_registers) * 2) -
													HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) -
												   CONT_AV_BIG_ENDIAN_BYTE_ARRAY_OFFSET) /
												  sizeof(struct TMtvi5ContAvDataPackOne)) %
												 MTVI5_ANALOG_OUTPUT_QUANTITY) +
												CONVERT_INTEGER_TO_NATURAL_NUMBER);

				// ��������� �������� ������ ������� - ������ ���������� ���������� ����������� �CONT_AV�.
				xAllModulesContext.axAllModulesContext[nucIndexNumber].
				xModuleContextDinamic.
				ucCommandControl = DEVICE_CONTROL_MTVI5_CONT_AV_WRITE;

				cout << "CONT_AV_BIG_ENDIAN_BYTE_ARRAY OK" << endl;
			}
		}
	}

//-----------------------------------------------------------------------------------------------------
// ���������� ���� ������ ����������� ������� �������� ������.
// ����� ������������ ��������� ��������� � ��������� ������� ���� ������ ����������� ������� �������� ������ - MTVI5?
	if (((((pxModbusMapping -> current_message_address_registers) * 2) -
			HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) >= CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET) &&
			((((pxModbusMapping -> current_message_address_registers) * 2) -
			  HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) < (CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET +
					  CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_LENGTH)))
	{
		// ���������� ������������ ��������� �� ������� �� ������� ������� ������?
		if (((((pxModbusMapping -> current_message_address_registers) * 2) -
				HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) +
				((pxModbusMapping->current_message_nb_registers) * 2)) <=
				(CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET +
				 CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_LENGTH))
		{
			// ��������� ���� ���� ������ ��������� �� Modbus � ����� ���� ������ RAM.
			memcpy(&aucHoldingRegistersArray[(((pxModbusMapping -> current_message_address_registers) * 2) -
											  HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS)],
				   (unsigned char*)&(pxModbusMapping->tab_registers[(pxModbusMapping -> current_message_address_registers) -
									 HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS]),
				   (unsigned int)((pxModbusMapping->current_message_nb_registers) * 2));
			vAnalogueOutputDataBasePack();
//            // ����� ������� ������ ���. � ������ ���������� �� ������ ���� ������.
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







//    // ������� ��������� �� ���� ���� ������.
//    pucDestination = &(xPlcDataBase.
//                       axPlcDataBaseBlocks[nucDataBaseBlockIndex].
//                       aucPlcDataBaseBlockData[0]);
//    // ��������� ���� ���� ������ ��������� �� Modbus � ����� ���� ������ RAM.
//    // ����� ������� ������ ���. � ������ ���������� �� ������ ���� ������.
//    memset(pucDestination,
//           0,
//           PLC_DATA_BASE_BLOCK_LENGTH);
//    memcpy(pucDestination,
//           pucSource,
//           (xPlcDataBaseControl.
//            axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
//            usBlockLength));
//    vDataBaseOneBlockCrcCreate((TPlcDataBaseBlock*)pucDestination);





			// �������� ������ ������ � ������� ���������, � �������� ��������� �������.
			nucIndexNumber =  (xPlcDataBaseControl.axPlcDataBaseBlockControl[
								   CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET +
								   ((((pxModbusMapping -> current_message_address_registers) * 2) -
									 CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET) /
									(CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_LENGTH / 2))].
							   ucModuleIndex);

			// �� ������� - nucIndexNumber ���� ������?
			if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
			{
				printf("CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET nucIndexNumber %d\n\r", (nucIndexNumber));
//            // ��������� �������� ������ ������� - ������ ���� ������.
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
//                // ��������� �������� ������ ������� - ������ ���� ������.
//                xAllModulesContext.axAllModulesContext[nucIndexNumber].
//                xModuleContextDinamic.
//                ucCommandControl = 0;
//                cout << "CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY ERROR" << endl;
//                // ��� ������ � ������ ��������� ������.
//                // ����������� ���� ������.
//                iPlcDataBaseCheck();
//            }

//            // ����� �������� ��������� � ���� ������, ��� ������� ������� � ������ ������ ����� ��������.
//            vAnalogueInputDescriptionWorkExstract();
//            vAnalogueInputDescriptionWorkToModbus();
//            vDimentionsParametersDataBaseExstract();
//            vModbusMasterModuleDataBaseExstract();
//            vAnalogueOutputDescriptionWorkExstract();
//            vAnalogueOutputDescriptionWorkToModbus();
//            vDiscreteInputDataBaseExstract();
//            vDiscreteSygnalTextDescriptorDataBaseExstract();
//            vFunctionBlockDataBaseExstract();
//            // �������� ����������� �����. ����� ����� ��������� ����������� ����� ������ ������� ������.
////            xPlcConfigService.usCrc =
////                usCrcSummTwoByteCalculation((unsigned char*)&xPlcConfigService.xPlcConfigServiceData,
////                                            sizeof(xPlcConfigService.xPlcConfigServiceData));
//            xCCrc.CreateAll();

				vWorkArraysUpdate();



				// �������� ������� ������ � ����� ������ ���� ������ ����������� � ����������.
				xAllModulesContext.axAllModulesContext[nucIndexNumber].
				xModuleContextDinamic.
				ucFsmState = MTVI5_FSM_STATE_WRITE_REGULATOR_DATABASE;
			}
		}
	}

//-----------------------------------------------------------------------------------------------------
// ���������� ������ �������.
// ����� ������������ ��������� ��������� � ��������� ������� ���������� ���������� ����������� ������� �������� ������ - MTVI5?
	if (((((pxModbusMapping -> current_message_address_registers) * 2) -
			HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) >= HAND_UST_BYTE_ARRAY_OFFSET) &&
			((((pxModbusMapping -> current_message_address_registers) * 2) -
			  HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) < (HAND_UST_BYTE_ARRAY_OFFSET +
					  HAND_UST_BYTE_ARRAY_LENGTH)))
	{
		// ���������� ������������ ��������� �� ������� �� ������� ������� ������?
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
// ���������� ����� � ����.
// ����� ������������ ��������� ��������� � ��������� ������� ���������� ���������� ����������� ������� �������� ������ - MTVI5?
	if (((((pxModbusMapping -> current_message_address_registers) * 2) -
			HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) >= CURRENT_TIME_HOLDING_BYTE_ARRAY_OFFSET) &&
			((((pxModbusMapping -> current_message_address_registers) * 2) -
			  HOLDING_REGISTERS_ARRAY_MODBUS_BEGIN_ADDRESS) < (CURRENT_TIME_HOLDING_BYTE_ARRAY_OFFSET +
					  CURRENT_TIME_HOLDING_BYTE_ARRAY_LENGTH)))
	{
		// ���������� ������������ ��������� �� ������� �� ������� ������� ������?
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
// ������������ �������� ��������� �� Modbus ����������� �� 17 ������� - ������ ������������.
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
// ������������ �������� ��������� �� Modbus ����������� �� 68 ������� - ������ ����� ���� ������.
void vDeviceControlDataBaseBlockReadHandler(modbus_mapping_t *pxModbusMapping)
{
	unsigned char *pucSource;
	unsigned char *pucDestination;
	unsigned char nucDataBaseBlockIndex;
//-----------------------------------------------------------------------------------------------------
// DataBaseBlockRead
//-----------------------------------------------------------------------------------------------------
	// ����� ����� ���� ������.
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
// ������������ �������� ��������� �� Modbus ����������� �� 69 ������� - ������ ����� ���� ������.
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
	// ������� ����� ����� ���� ������ ��������� �� modbus.
	nucDataBaseBlockIndex = (unsigned char)pxModbusMapping -> current_message_address_registers;
	// ������� ��������� �� ������ ��������� �� modbus.
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


	// ������� ��������� �� ���� ���� ������.
	pucDestination = &(xPlcDataBase.
					   axPlcDataBaseBlocks[nucDataBaseBlockIndex].
					   aucPlcDataBaseBlockData[0]);
	// ��������� ���� ���� ������ ��������� �� Modbus � ����� ���� ������ RAM.
	// ����� ������� ������ ���. � ������ ���������� �� ������ ���� ������.
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

	// ����� ��������� ��� ������ ����� ���� ������.
	switch(nucDataBaseBlockIndex)
	{
	case DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET:
		cout << "DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET" << endl;
		nucIndexNumber = 0;
		// ������ �� ��������� ���� �������.
		for (i = 0;
				i < (xPlcConfigService.
					 xPlcConfigServiceData.
					 ucInternalModulesQuantity);
				i++)
		{
			// ������ ���� ������?
			switch(xAllModulesContext.axAllModulesContext[nucIndexNumber].
					xModuleContextStatic.ucModuleType)
			{
				// � ������� ������������ ������ MVDS9.
			case MODULE_TYPE_MVDS9:
				iResult = iModuleMvds9DataBaseBlockWrite(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
				if (iModuleMvds9DataBaseBlockWrite(&xAllModulesContext.axAllModulesContext[nucIndexNumber]) == -1)
				{
					iResult = -1;
					// ������� ��� ������, ��� �������� �������������, �� 14 ������� Modbus - ������ � ����������� ����������������.
					ucDataBaseError = PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_MVDS9_EEPROM_ERROR;
					cout << "DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_MVDS9_EEPROM_ERROR" << endl;
				}
				else
				{
					iResult = 0;
					iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
					// ������� �������������, ��� ������ ���, �� 14 ������� Modbus - ������ � ����������� ����������������.
					ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
					cout << "DISCRETE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET PROGRAMMING_DATA_BASE_WRITE_OK" << endl;
				}
				break;

			default:
				// ������� �������������, ��� ������ ���, �� 14 ������� Modbus - ������ � ����������� ����������������.
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
		// ������ ���� ������?
		switch(xPlcDataBaseControl.
				axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
				ucModuleType)
		{
			// � ������� ������������ ������ ����������� �����.
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
//                // ������� ��� ������, ��� �������� �������������, �� 14 ������� Modbus - ������ � ����������� ����������������.
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
			// ������� �������������, ��� ������ ���, �� 14 ������� Modbus - ������ � ����������� ����������������.
			ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
			break;
		};
		break;

	case DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_OFFSET:
		cout << "DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_OFFSET" << endl;
		// ������� ��������� �� ���� ���� ������, �������� �� Modbus �� ��������� �����.
		pxMrXXOneChannelDataBaseSource =
			(TMrXXOneChannelDataBase*)pucSource;
		// ������ �� ��������� ������� ����������� ������.
		for (int i = 0;
				i < (xPlcConfigService.
					 xPlcConfigServiceData.
					 ucServiceDiscreteOutputQuantity);
				i++)
		{
			// ���� �� ���������������� ����� ��� ������������ ������ � ����������� - ���� ������ �� ���������.
			if ((((pxMrXXOneChannelDataBaseSource[i].ucControl) & OUTPUT_TYPE_MASK) &&
					((pxMrXXOneChannelDataBaseSource[i].ucControl) & BLOCK_ALARM_TYPE_MASK) &&
					(!((pxMrXXOneChannelDataBaseSource[i].ucControl) & PC_RESET_MASK)) &&
					(!((pxMrXXOneChannelDataBaseSource[i].ucControl) & BUTTON_RESET_MASK))) &&
					(!(pxMrXXOneChannelDataBaseSource[i].ucExternalResetAddress)))
			{
				cout << "PROGRAMMING_MR_DATA_BASE_NOT_CORRECT 1" << endl;
				// ������� ��� ������, ��� �������� �������������, �� 14 ������� Modbus - ������ � ����������� ����������������.
				ucDataBaseError = PROGRAMMING_MR_DATA_BASE_NOT_CORRECT;
				iResult = -1;
				break;
			}
			else
			{
				iResult = 0;
			}
			// ���� �� ����������������� ������������ ��� ���������������� ������ "����" - ���� ������ �� ���������.
			if (((!((pxMrXXOneChannelDataBaseSource[i].ucControl) & OUTPUT_TYPE_MASK)) &&
					((pxMrXXOneChannelDataBaseSource[i].ucControl) & BLOCK_ALARM_TYPE_MASK) &&
					(!((pxMrXXOneChannelDataBaseSource[i].ucControl) & PC_KVIT_MASK)) &&
					(!((pxMrXXOneChannelDataBaseSource[i].ucControl) & BUTTON_KVIT_MASK))) &&
					(!(pxMrXXOneChannelDataBaseSource[i].ucExternalKvitAddress)))
			{
				cout << "PROGRAMMING_MR_DATA_BASE_NOT_CORRECT 2" << endl;
				// ������� ��� ������, ��� �������� �������������, �� 14 ������� Modbus - ������ � ����������� ����������������.
				ucDataBaseError = PROGRAMMING_MR_DATA_BASE_NOT_CORRECT;
				iResult = -1;
				break;
			}
			else
			{
				iResult = 0;
			}
			// ���� �� ����������������� ������������ � ����� ��� ���������������� ������ "����" - ���� ������ �� ���������.
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
				// ������� ��� ������, ��� �������� �������������, �� 14 ������� Modbus - ������ � ����������� ����������������.
				ucDataBaseError = PROGRAMMING_MR_DATA_BASE_NOT_CORRECT;
				iResult = -1;
				break;
			}
			else
			{
				iResult = 0;
			}
		}
		// ���� ���� ������ �� ��������� - �� ����� ���������� � � ������.
		if (iResult == -1)
		{
			break;
		}

		nucIndexNumber = 0;
		// ������ �� ��������� ���� �������.
		for (i = 0;
				i < (xPlcConfigService.
					 xPlcConfigServiceData.
					 ucInternalModulesQuantity);
				i++)
		{
			// ������ ���� ������?
			switch(xAllModulesContext.axAllModulesContext[nucIndexNumber].
					xModuleContextStatic.ucModuleType)
			{
				// � ������� ������������ ������ ����������� ������.
			case MODULE_TYPE_MR53:
			case MODULE_TYPE_MR54:
			case MODULE_TYPE_MR55:
			case MODULE_TYPE_MR51:
//                iResult = iModuleMrXXDataBaseBlockWrite(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
				// ��������� ��������� ��������, �������� ������ - ���������� �����.
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
		// ������ ���� ������?
		switch(xPlcDataBaseControl.
				axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
				ucModuleType)
		{
			// � ������� ������������ ������ ����������� ������.
		case MODULE_TYPE_MTVI5:
			nucIndexNumber = xPlcDataBaseControl.
							 axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
							 ucModuleIndex;
			printf("vDeviceControlDataBaseBlockWriteHandler nucIndexNumber %d\n\r", (nucIndexNumber));
//            // ��������� �������� ������ ������� - ������ ���� ������.
//            xAllModulesContext.axAllModulesContext[nucIndexNumber].
//            xModuleContextDinamic.
//            ucCommandControl = DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_PROGRAMMER;
//            iResult =
//                iModuleMtvi5RegulatorDataBaseBlockWrite(
//                    &xAllModulesContext.axAllModulesContext[nucIndexNumber]);
//            if (iResult == -1)
//            {
//                // ������� ��� ������, ��� �������� �������������, �� 14 ������� Modbus - ������ � ����������� ����������������.
//                ucDataBaseError = PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_PSP_EEPROM_ERROR;
//                // ��������� �������� ������ ������� - ������ ���� ������.
//                xAllModulesContext.axAllModulesContext[nucIndexNumber].
//                xModuleContextDinamic.
//                ucCommandControl = 0;
//            }



			xAllModulesContext.
			axAllModulesContext[nucIndexNumber].
			xModuleContextDinamic.
			ucCommonIndex = nucDataBaseBlockIndex;
			// �������� ������� ������ � ����� ������ ���� ������ ����������� � ����������.
			xAllModulesContext.axAllModulesContext[nucIndexNumber].
			xModuleContextDinamic.
			ucFsmState = MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_REGULATOR;
//            return;//debag//
			break;
		default:
			// ������� ��� ������, ��� �������� �������������, �� 14 ������� Modbus - ������ � ����������� ����������������.
			ucDataBaseError = PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_PSP_EEPROM_ERROR;
			iResult = -1;//debag//
			break;
		};

		iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
		break;

//    case CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_OFFSET:
//        cout << "CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_OFFSET" << endl;
//        // ������ ���� ������?
//        switch(xPlcDataBaseControl.
//                axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
//                ucModuleType)
//        {
//            // � ������� ������������ ������ ����������� ������.
//        case MODULE_TYPE_MTVI5:
//
//            iResult = iModuleMtvi5PspDataBaseBlockWrite(&xAllModulesContext.axAllModulesContext[xPlcDataBaseControl.
//                      axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
//                      ucModuleIndex]);
//            if (iResult == -1)
//            {
//                // ������� ��� ������, ��� �������� �������������, �� 14 ������� Modbus - ������ � ����������� ����������������.
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
		// ������� �������������, ��� ������ ���, �� 14 ������� Modbus - ������ � ����������� ����������������.
		ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
		break;

	case MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_OFFSET:
		cout << "MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_OFFSET" << endl;
//        iResult = 0;
		iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
		// ������� �������������, ��� ������ ���, �� 14 ������� Modbus - ������ � ����������� ����������������.
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
		// ������� �������������, ��� ������ ���, �� 14 ������� Modbus - ������ � ����������� ����������������.
		ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
		break;

	case DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET:
		cout << "DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET" << endl;
//        iResult = 0;
		iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
		// ������� �������������, ��� ������ ���, �� 14 ������� Modbus - ������ � ����������� ����������������.
		ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
//        if (iResult == -1)
//        {
//            // ������� ��� ������, ��� �������� �������������, �� 14 ������� Modbus - ������ � ����������� ����������������.
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
		// ������� �������������, ��� ������ ���, �� 14 ������� Modbus - ������ � ����������� ����������������.
		ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
//        if (iResult == -1)
//        {
//            // ������� ��� ������, ��� �������� �������������, �� 14 ������� Modbus - ������ � ����������� ����������������.
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
		// ������� �������������, ��� ������ ���, �� 14 ������� Modbus - ������ � ����������� ����������������.
		ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
		break;

	case MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_OFFSET:
	case MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_OFFSET + 1:
	case MATHEMATICAL_BLOCK_DATA_BASE_BLOCK_OFFSET + 2:
		cout << "MATHEMATICAL_FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET" << endl;
//        iResult = 0;
		iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
		// ������� �������������, ��� ������ ���, �� 14 ������� Modbus - ������ � ����������� ����������������.
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
		// ������� �������������, ��� ������ ���, �� 14 ������� Modbus - ������ � ����������� ����������������.
		ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
		break;

	default:
		cout << "default" << endl;
		iResult = -1;
		break;
	};

	cout << "iResult" << endl;
	cout << "0x" << hex << uppercase << setw(2) << setfill('0') << (unsigned int)iResult << " " << endl;
	// ���� ������ ��� - ������� ���� ���� ������ � EEPROM �������.
	if (iResult == 0)
	{
		// �������� ������� �������� �����:
		// void CMainProductionCycle::Fsm(void)
		// � ����� ������ ����������������
		xMainProductionCycle.SetFsmState(CMainProductionCycle::PROGRAMMING_CYCLE_START);

//	cout << "xMainProductionCycle.SetFsmState begin" << endl;


//		cout << "iResult if (iResult == 0)" << endl;
//
//		// ���������������� �������� ������?
//		if (xProgrammTimeMeasure.LastMoreThan(10))
//		{
//			cout << "if (xProgrammTimeMeasure.LastMoreThan(10))" << endl;
//			// �������������� �������.
//			CEvents::EventRegistration(
//				0,
//				(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
//				 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//				0,
//				"����������������");
//		}
//
////        iPlcDataBaseBlockWrite(nucDataBaseBlockIndex,
////                               pucSource,
////                               (xPlcDataBaseControl.
////                                axPlcDataBaseBlockControl[nucDataBaseBlockIndex].
////                                usBlockLength));
////        iPlcDataBaseBlockWrite(nucDataBaseBlockIndex);
//		// ����� ��������� ��� ������ ����� ���� ������.
//		switch(nucDataBaseBlockIndex)
//		{
//			// ���� ������ ���� ���� ������ ��� ������� ��������� ���� ������� ������,
//			// �� ��� ������, ��� ���������� ���������� �������, ������. ��������� ��������� ������������.
//		case MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_OFFSET:
//		case MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_OFFSET + 1:
//		case MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_OFFSET:
//			// �������� ����� ������������
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
//			// ������� ��������� ���������� ModbusRtu, ����� � ��������� ����� ������� ������, RS-485.
//			iModbusRtuMasterReset();
//			break;
//
//		default:
//			break;
//		};
//
////        // ������� �������������, ��� ������ ���, �� 14 ������� Modbus - ������ � ����������� ����������������.
////        ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
	}
	else
	{
		cout << "iResult if (iResult != 0)" << endl;
		// ��� ������ � ������ ��������� ������.
		// ����������� ���� ������.
		iPlcDataBaseCheck();
	}
////    // ����� �������� ��������� � ���� ������, ��� ������� ������� � ������ ������ ����� ��������.
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
////    // �������� ����������� �����. ����� ����� ��������� ����������� ����� ������ ������� ������.
//////    xPlcConfigService.usCrc =
//////        usCrcSummTwoByteCalculation((unsigned char*)&xPlcConfigService.xPlcConfigServiceData,
//////                                    sizeof(xPlcConfigService.xPlcConfigServiceData));
////    xCCrc.CreateAll();
//
//	vWorkArraysUpdate();
}

//-----------------------------------------------------------------------------------------------------
// ������������ �������� ��������� �� Modbus ����������� �� 71 ������� - ������ ������ ������, ������� ����������� �����.
// ������� ���������� �������� ���������� ������, �������� ����� ���, �������� ���.
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
// ���� � ������� �������� ������ Modbus ���7 = 0, �� ������������� �������� ����� - (���0 - ���6) - ����� ����������� �����.
// ���� � ������� �������� ������ Modbus ���7 = 1, �� ������������� ��� � (���0 - ���2) - ������������� ����� ������ ����3.
// ucAddressLow - ��������� ���������� ���������� ������.

	ucAddressLow = (unsigned char)(pxModbusMapping ->
								   current_message_address_common);
	ucAddressHigh = (unsigned char)((pxModbusMapping ->
									 current_message_address_common) >> 8);
	pucDestination = pxModbusMapping -> buffer_pointer;

	// ������ ��� ��� �������� �����?
	if (ucAddressHigh & MVAI5_TXS_REQUEST_MASK)
	{
// ������ ���.
//        cout << "TXS" << endl;
		// �������� ������ ������ � ������� ���������.
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
// ������ �������� �����.

		// ����� ����������� ����� ����������� ����������� ��� �������� ������?
		if ((xPlcConfigService.
				xPlcConfigServiceData.
				ucServiceAnalogueInputModuleQuantity) >=
				(((ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK) /
				  MVAI5_ANALOG_INPUT_QUANTITY) + 1))
		{
			// ����� ����������� ����� ����������� ����������� ������.
//            cout << "GENERAL" << endl;
			// �������� ������ ������ � ������� ���������.
			nucIndexNumber =
				(((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex + 1) -
				  xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)  +
				 ((ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK) /
				  MVAI5_ANALOG_INPUT_QUANTITY));

			iModuleMvai5ReperPointsAdcRead(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
			// ������� ����� �������� ����������� �����.
//            pfSource = (float*)&aucHoldingRegistersArray[AI_VALUE_BYTE_ARRAY_OFFSET +
//                       ((ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK) *
//                        sizeof(float))];
			pfSource = &afAnalogueInputData[(ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK)];
			// ������� ����� �������� ��������� ������ ����������� �����.
			pucSource2 = &aucStatAiByteArray[ucAddressHigh];
//            cout << "_FC_ONLINE_DATA_READ" << endl;
			for (i = 0; i < (ucAddressLow); i++)
			{
				// ����������� ������ 7 ���� float ��������, � ASCII �������, ��� ����������� � ��������� ���� � �������������.
				sprintf((char*)pucDestination,
						"%7.4f",
						pfSource[i]);
//                cout << "_FC_ONLINE_DATA_READ 1" << endl;
				pucDestination += ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY;
				// ������� �������� ��������� ������ ����������� �����.
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
			// ����� ����������� ����� ����������� �������� ������.
//            cout << "LSNU" << endl;
			// ������� ����� �������� ����������� �����.
//            pfSource = (float*)&aucHoldingRegistersArray[AI_VALUE_BYTE_ARRAY_OFFSET +
//                       ((ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK) *
//                        sizeof(float))];
			pfSource = &afAnalogueInputData[(ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK)];
			// ������� ����� �������� ��������� ������ ����������� �����.
			pucSource2 = &aucStatAiByteArray[ucAddressHigh];

			for (i = 0; i < (ucAddressLow); i++)
			{
				// ����������� ������ 7 ���� float ��������, � ASCII �������, ��� ����������� � ��������� ���� � �������������.
				sprintf((char*)pucDestination,
						"%7.4f",
						pfSource[i]);
				pucDestination += ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY;
				// ������� �������� ��������� ������ ����������� �����.
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
// ������������ �������� ��������� �� Modbus ����������� �� 70 ������� - ��������� �������� ������� �� ��� ������� ���� .
// �������� - ���-05_�3_10 2013.pdf.
// 4.3.11 ������� 70: ��������� �������� ������� �� ��� ������� ����
// ������
// ����������������� ������ (�����=0), ��������� ���� SL ����. ���������� 7
// ������.
// ���� 1 - ������� 0 � 59;
// ���� 2 - ������ 0 �59;
// ���� 3 - ��� 0 � 23;
// ���� 4 - ���� 1 � 31;
// ���� 5 - ����� 1 � 12.
// ���� 6 - ��� 0 � 99.
// ���� 7 � ���� ������ 1 � 7.
// �����
// �� ������ ������ SL �� ��������.
void vCurrentTimeSet(modbus_mapping_t *pxModbusMapping)
{
//    time_t rawtime;
//    struct tm *timeinfo;
//    struct timeval systime;
//    unsigned char *pucSource;
//    unsigned short *pusDestination;
//    // ������� ��������� �� ����� � ������� ������� ���������� �� Modbus.
//    pucSource = (unsigned char*)(pxModbusMapping -> buffer_pointer);
////    // ������� ��������� �� ����� � ������� �������� � ������� ������� �������.
////    pusDestination = (unsigned short*)&aucInputRegistersArray[CURRENT_TIME_BYTE_ARRAY_OFFSET];
////    // ��������� ������ ������� ��������� �� Modbus � ������� ������ �������.
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
