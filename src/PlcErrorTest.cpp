
#include "PasNewConfig.h"
#include "PlcErrorTest.h"
#include "Alarm.h"
#include "Error.h"
#include "Task.h"
#include "nkh-lab/logger.hpp"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// ���������� ������� ��������� ������ ������� ������� � ����������.
// ���� ��������� ������� - ���������� ���� ������ ������, ��������� ������� ����� � FRAM.
int iBadModuleBuffUpdate(void)
{
	// ���� ������ ������ �� ����������?
	if (memcmp(&aucBadModuleCompareBuff[0],
			   &aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET],
			   BAD_MODULES_BIT_ARRAY_LENGTH) == 0)
	{
		return 0;
	}
	cout << "iBadModuleBuffUpdate" << endl;
	// ����������� ���� ������ ������ � ������ ����������� ����.
	memcpy(&aucBadModuleCompareBuff[0],
		   &aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET],
		   BAD_MODULES_BIT_ARRAY_LENGTH);
	// �������� ������� ����� ������ ������� � FRAM.
	iFramWrite(FRAM_BAD_MODULES_BIT_ARRAY_OFFSET,
			   &aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET],
			   BAD_MODULES_BIT_ARRAY_LENGTH);
	return 0;
}

//-----------------------------------------------------------------------------------------------------
// �������� ��������� ������ ������� ������� �� FRAM.
int iBadModuleBuffRead(void)
{
	iFramRead(&aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET],
			  FRAM_BAD_MODULES_BIT_ARRAY_OFFSET,
			  BAD_MODULES_BIT_ARRAY_LENGTH);
}

//-----------------------------------------------------------------------------------------------------
// ���������� ��� ����� ������� ������� � RAM � FRAM.
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
// �������� ������ �������.
void vBadModulesTest(void)
{
	// ������� ������� ����������?
	xCCrc.i8Error = xCCrc.CheckAll();
	if (xCCrc.i8Error != 0)
	{
//        // ������.
//        // ������������.
//        // ��������� ���� �������� ������������.
//        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
		// ������������.
		// ��������� ���� "������������ ���������".
//        fbPlcErrorReset = 1;
		printf("xCCrc.CheckAll() error %d\n\r", (xCCrc.i8Error));
		LOG_DBG << "xCCrc.CheckAll() Error!" << (xCCrc.i8Error);

		// �������� ��������� ������� ��� �� ����������������?
		if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
					SYSTEM_ERROR_GROUP_OFFSET,
					SYSTEM_ERROR_CRC_ERROR_OFFSET))
		{
			// �������������� �������� ��������� �������.
			CEvents::EventRegistration(
				SYSTEM_ERROR_GROUP_OFFSET,
				(CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
				 CEvents::HANDLED_EVENTS_IS_POPUP |
				 CEvents::HANDLED_EVENTS_IS_SOUND |
				 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
				(xCCrc.i8Error),
				"������ CRC");
		}
	}
	else
	{
		// ���������� ��������� ������� ��� �� ����������������?
		if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
					SYSTEM_ERROR_GROUP_OFFSET,
					SYSTEM_ERROR_CRC_ERROR_OFFSET))
		{
			// �������������� ���������� ��������� �������.
			CEvents::EventRegistration(
				SYSTEM_ERROR_GROUP_OFFSET,
				(CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
				 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
				(xCCrc.i8Error),
				"����� CRC");
		}
	}


//    // ��������� ���������� ����������?
//    if (xPlcConfigService.usCrc !=
//            usCrcSummTwoByteCalculation((unsigned char*)&xPlcConfigService.xPlcConfigServiceData,
//                                        sizeof(xPlcConfigService.xPlcConfigServiceData)))
//    {
//        // ������.
//        // ������������.
//        // ��������� ���� �������� ������������.
//        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//        // ������������.
//        // ��������� ���� "������������ ���������".
//        fbPlcErrorReset = 1;
//        cout << "xPlcConfigServiceData error" << endl;
//    }

//    // ������ ���������� ����� ������ ����������?
//    if (xPlcDataBaseControl.usCrc !=
//            usCrcSummTwoByteCalculation((unsigned char*)xPlcDataBaseControl.axPlcDataBaseBlockControl,
//                                        (sizeof(xPlcDataBaseControl.axPlcDataBaseBlockControl))))
//    {
//        // ������.
//        // ������������.
//        // ��������� ���� �������� ������������.
//        fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ERROR;
//        // ������������.
//        // ��������� ���� "������������ ���������".
//        fbPlcErrorReset = 1;
//        cout << "axPlcDataBaseBlockControl error" << endl;
//    }

	// ������ ������������� ��������� ������� ����������?
	if (iModulesContextStaticCrcCheck(&xAllModulesContext) == -1)
	{
		// ������.
		// ������������.
		// ������������.
		// ��������� ���� "������������ ���������".
		fbPlcErrorReset = 1;
		xCCrc.i8Error = (sizeof(xCCrc.axTCrcContextInit) + CONVERT_INTEGER_TO_NATURAL_NUMBER);
		cout << "xModuleContextStatic error" << endl;
		LOG_DBG << "xModuleContextStatic Error!" << (xCCrc.i8Error);
	}

	// �������� ����� ������� ���������� �������.
	memcpy(&aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET],
		   aucBadModuleStateBuff,
		   INTERNAL_MODULE_QUANTITY);

	// ���� ������ ������������?
	if (fbPlcConfigurationError)
	{
		// ��������� ���� - ������ ������������.
		aucCoilsArray[CONFIGURATION_ERROR_SIGNAL_BIT_ARRAY_OFFSET] = 1;
	}
//    // ���� ������ ��������� ���� ������� ������ ����������?
//    if ((iCrc16Check((unsigned char*)&(xPlcDataBase.
//                                       axPlcDataBaseBlocks[MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_OFFSET].
//                                       aucPlcDataBaseBlockData[0]),
//                     (PLC_DATA_BASE_BLOCK_LENGTH -
//                      TWO_BYTE_CRC_LENGTH))) == -1)
//    {
//        // ��������� ���� - ���� ������ ��������� ���� ������� ������ ����������.
//        fbLocalNetworkModuleDataBaseError = 1;
//        aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET +
//                      LOCAL_NET_MODULE_DATA_BASE_ERROR_OFFSET] = 1;
//    }
//    // ���� ������ ��������� ���� ������� ������ ����������?
//    if ((iCrc16Check((unsigned char*)&(xPlcDataBase.
//                                       axPlcDataBaseBlocks[MBM_FUNCTION_15_BLOCK_DATA_BASE_BLOCK_OFFSET].
//                                       aucPlcDataBaseBlockData[0]),
//                     (PLC_DATA_BASE_BLOCK_LENGTH -
//                      TWO_BYTE_CRC_LENGTH))) == -1)
//    {
//        // ���� ������ ��������� ���� ������� ������ ����������.
//        fbLocalNetworkModuleDataBaseError = 1;
//        aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET +
//                      LOCAL_NET_MODULE_DATA_BASE_ERROR_OFFSET] = 1;
//    }
	// ���� ������ ���������� �������� ����������?
	if (fbDiscreteInputDataBaseError)
	{
//        fbDiscreteInputDataBaseError = 0;
		// ��������� ���� - ���� ������ ���������� �������� ����������.
		aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET +
												   DISCRETE_SYGNALS_DATA_BASE_ERROR_OFFSET] = 1;
		fbModuleMrOutOff = 1;
	}
	// ���� ������ �������������� ������ ����������?
	if (fbFunctionBlockDataBaseError)
	{
//        // ��������� ���� - ���� ������ �������������� ������ ����������.
//        fbFunctionBlockDataBaseError = 0;
		fbModuleMrOutOff = 1;
	}
	// ���� ������ ������� ����������� ����� ����������?
	if (fbAnalogueInputModuleDataBaseError)
	{
		fbModuleMrOutOff = 1;
//        fbAnalogueInputModuleDataBaseError = 0;
		// ��������� ���� - ���� ������ ������� ����������� ����� ����������.
		aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET +
												   ANALOGUE_INPUT_MODULE_DATA_BASE_ERROR_OFFSET] = 1;
	}
	// ���� ������ �������� ����� ������� ����������� ����� ����������?
	if (fbAnalogueInputModuleReperPointsAdcDataBaseError)
	{
		fbModuleMrOutOff = 1;
	}
	// ���� ������ ������� ����������� ������ ����������?
	if (fbDiscreteOutputModuleDataBaseError)
	{
		fbModuleMrOutOff = 1;
//        fbDiscreteOutputModuleDataBaseError = 0;
		// ��������� ���� - ���� ������ ������� ����������� ������ ����������.
		aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET +
												   DISCRETE_OUTPUT_MODULE_DATA_BASE_ERROR_OFFSET] = 1;
	}
	// ������� �����?
	if (xCPingTaskWEEW.ui8Status ==
			CTaskSerialMT::TASK_STATE_MESSAGE_RESPONSE_OK)
	{
		// ������� �����. ��������� ������.
		xCPingTaskWEEW.ui8DataExchangeFsmState = CTaskSerialMT::TASK_MODE_MESSAGE_SEND_READ;
		xCPingTaskWEEW.ui8Status = CTaskSerialMT::TASK_STATE_MESSAGE_ANSWER_WAITING;

		// ���������� ��������� ������� ��� �� ����������������?
		if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
					SYSTEM_ERROR_GROUP_OFFSET,
					SYSTEM_ERROR_MIND_NOT_RESPONDED_OFFSET))
		{
			// �������������� ���������� ��������� �������.
			CEvents::EventRegistration(
				SYSTEM_ERROR_GROUP_OFFSET,
				(CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE |
				 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
				SYSTEM_ERROR_MIND_NOT_RESPONDED_OFFSET,
				"����� ����");
		}
	}
	// �� ������� �����?
	else if (xCPingTaskWEEW.ui8Status ==
			 CTaskSerialMT::TASK_STATE_MESSAGE_RECEIVE_TIMEOUT)
	{
		// �� ������� �����. ��������� ������.
		xCPingTaskWEEW.ui8DataExchangeFsmState = CTaskSerialMT::TASK_MODE_MESSAGE_SEND_READ;
		xCPingTaskWEEW.ui8Status = CTaskSerialMT::TASK_STATE_MESSAGE_ANSWER_WAITING;
		printf("CPingTaskWEEW vBadModulesTest ui8Status  %d\n\r", (xCPingTaskWEEW.ui8Status));
		xCPingTaskWEEW.ui8Status = CTaskSerialMT::TASK_STATE_MESSAGE_NOT_SEND;
		fbMindDataExchangeError = 1;
		// ��������� ���� - ��� ����� � ����.
		aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET +
												   MAL_ERROR_OFFSET] = 1;

		// �������� ��������� ������� ��� �� ����������������?
		if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
					SYSTEM_ERROR_GROUP_OFFSET,
					SYSTEM_ERROR_MIND_NOT_RESPONDED_OFFSET))
		{
			// �������������� �������� ��������� �������.
			CEvents::EventRegistration(
				SYSTEM_ERROR_GROUP_OFFSET,
				(CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE |
				 CEvents::HANDLED_EVENTS_IS_POPUP |
				 CEvents::HANDLED_EVENTS_IS_SOUND |
				 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
				SYSTEM_ERROR_MIND_NOT_RESPONDED_OFFSET,
				"��� ����� � ����");
		}
	}

//-----------------------------------------------------------------------------------------------------
//    // ������� ������ ������.
//    CError::ErrorLogCreate();
	// ���������� �������.
	CEvents::EventsHandler();
	// ������� ����� �������������� ������� � FRAM.
	iBadModuleBuffUpdate();

//-----------------------------------------------------------------------------------------------------
	// �������� ���������� ����������� ������������ �������������� ������� �� HMI.
	for (int i = 0; i < BAD_MODULES_BIT_ARRAY_LENGTH; i++) //?
	{
		// ���� ������������� �������?
		if (aucCoilsArray[BAD_MODULES_BIT_ARRAY_OFFSET + i])
		{
			// ���� ������������� �������.
			// ���� ������������ ��� ������� ������������� �������?
			if (axAlarmModules[i].ui8AlarmState)
			{


			}
			else
			{
				// ��������� ������������ ��� ������� ������������� �������.
				// ������ ������ ��� ����������� ������������ �� HMI(������ ������).
				axAlarmModules[i].ui8AlarmState = 1;
				// ��������� ��� ������������ ��� �������� ����������� �������.
				// ��������� ���� ��������� ������������.
				axAlarmModules[i].ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
			}
		}
		else
		{
			// ��� �������������.
			// ���� ������������ ��� ������� ������������� �������?
			if (axAlarmModules[i].ui8AlarmState)
			{
				// ���� ������� �����?
				if (fbGlobalReset)
				{
					// ������� ������������ ��� ������� ������������� �������.
					axAlarmModules[i] = xAlarmHmiNull;
				}
			}
			else
			{

			}
		}
	}

//-----------------------------------------------------------------------------------------------------
	// ���� ������� ������������ ��� �����?
	if (fbGlobalKvit || fbGlobalReset)
	{
		// ������� ���� �������� ������������.
		fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_OFF;
		// ������� ������������ ������������� �������.
		for (int i = 0; i < BAD_MODULES_BIT_ARRAY_LENGTH; i++)
		{
			// ���� ������������ ��� ������� ������������� �������?
			if (axAlarmModules[i].ui8AlarmState)
			{
				// ������� ������������ ��� ������� ������������� �������.
				// ����� ������� ������������ ��� �����, ���� � ������������ � �����.
				// ��������� ����� ������ ����:
				// ��������� ����� ����������������� ������������.
				axAlarmModules[i].ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
				// ��������� ���� ��������� ������������.
				axAlarmModules[i].ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
			}
		}
	}

//-----------------------------------------------------------------------------------------------------
	// ��������� ��� ������������ � ������ ���������� ������������ ��������� ������� ���.
	for (int i = 0; i < BAD_MODULES_BIT_ARRAY_LENGTH; i++)
	{
		// ���� ������������ ��� ������� ������������� �������?
		if (axAlarmModules[i].ui8AlarmState)
		{
			// ������������ ��� ������� ������������� ������� �����������?
			if ((axAlarmModules[i].ui8AlarmState & ALARM_TYPE_PREVENT) &&
					(axAlarmModules[i].ui8AlarmState & ALARM_TYPE_ALARM))
			{
				// ��������� ��� ������������ - ������������ ������������.
				aucCoilsArray[MODULES_ALARM_STATE_BITS_BIT_ARRAY_OFFSET] = 1;
			}
			else
			{
				// ��������� ��� ������������ - ��������� ������������.
				aucCoilsArray[MODULES_ALARM_STATE_BITS_BIT_ARRAY_OFFSET + 1] = 1;
			}
		}
		else
		{
			// ��������� ��� ������������ - �����, ��� �������������.
			aucCoilsArray[MODULES_ALARM_STATE_BITS_BIT_ARRAY_OFFSET] = 0;
			aucCoilsArray[MODULES_ALARM_STATE_BITS_BIT_ARRAY_OFFSET + 1] = 0;
		}
	}

//    sync();
//    reboot(LINUX_REBOOT_CMD_RESTART);

}

