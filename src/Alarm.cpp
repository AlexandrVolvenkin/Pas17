

#include "PasNewConfig.h"
#include "FunctionBlocks.h"
#include "Alarm.h"
#include "Events.h"
#include "EventsDB.h"
#include "SystemComponent.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
uint8_t CAlarm::ui8AlarmEventLogBriefIsUpdated;
const  CAlarm::TAlarmEvent CAlarm::xAlarmEventNull =
{
	0,
	0,
	0,
	0
};
uint8_t CAlarm::ui8AlarmEventLogLength;
uint8_t CAlarm::ui8AlarmEventLogBriefLength;
CAlarm::TAlarmEventBrief CAlarm::axAlarmEventsLogBrief[];
CAlarm::TAlarmEventLogQueryListPackOne CAlarm::xAlarmEventsQueryList;
int16_t CAlarm::i16AlarmEventQueryListQuantity;
CAlarm::TAlarmEvent CAlarm::axAlarmEventsLog[];
CAlarm::TAlarmEventsLogControl CAlarm::xAlarmEventsLogControl;

//-----------------------------------------------------------------------------------------------------
// ������ �������: ����� ��������� � ����������.
void CAlarm::PlcOnOffEvetnsCreate(void)
{
	CEvents::TEventDataCommon *pxEventData;

	// ������� ��������� �� ����� � ��������� ������ ������� �������.
	pxEventData = xCAlarmEvent.EventDataPush();
// �������� ������ �������.
	// ��������� id �������.
	(pxEventData -> ui16ID) =
		1;
	// ��������� ��� �������.
	(pxEventData -> ui8Type) =
		CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE;
	// ��������� ����� ��������� �������.
	(pxEventData -> ui16Address) = 0;
	// ��������� ��� �������.
	(pxEventData -> ui8State) =
		0;

	// ������� ��������� ���������� ����� � FRAM.
	iFramRead((uint8_t*)&xCurrentTime,
			  FRAM_LAST_SAVED_TIME_OFFSET,
			  sizeof(xCurrentTime));

	// ��������� ����� �������.
	(pxEventData -> xCurrentTime) =
		xCurrentTime;
	// �������� ������ ���������� ���������.
	sprintf((char*)(pxEventData -> acTextDescriptor),
			"%s",
			"��������� ������");
	// �������� ������ ������ � ���� �������������� ��������� ������ ������� ������ � ���� ������.
	sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
			"%s",
			" ");
	xCArchiveEventsDB.DataBaseDataPush(pxEventData);

	// ������ ��������� ���� ��������������� ����������� ������ ������� �� ���������� ������.
	// ����� ���� �������� ������ ������� � �������� - ��� �������, �� �������� ��������
	// �������. �� ������� ���������� ���������� � ���, ��� ������� ������ ���.
	// ����� �������� ������ ��������� ���� � ���, ��� ������� ������ ��� - �������� ��
	// ���������� ������ ����� ������ �������. ������� ��� �������� - ��� �������(ui16ID = 0).
	// ��������� ������ (ui16EventDataCommonPopIndex) �� ����� ���������� � ��������� ������ ������ ������� �������.
	xCAlarmEvent.EventDataPopIndexSet(0);
	// ������� ��������� �� ����� ���������� � ��������� ������ ������ ������� �������.
	pxEventData = xCAlarmEvent.EventDataPop();
	// ������� ������� �������� - ��� �������.
	pxEventData -> ui16ID = 0;


	time_t xLocalLastTime;
	uint32_t ui32Seconds;

	// ����������� � ������������� ������ ��������� ���������� ����� � FRAM.
	xLocalLastTime = mktime(&xCurrentTime);
	// ������� ������� ����� � ���������� xCurrentTime.
	vCurrentTimeUpdate();
	// ������� ������� � �������� ����� �������� ����������
	// � �������� ���������.
	ui32Seconds = (uint32_t)difftime(mktime(&xCurrentTime), xLocalLastTime);

	// ���������� ���������� ���� ������ ��� 1 ������ �����?
	if (ui32Seconds > 60)
	{
		// ����� ������ ����5, ����� ��������� �� ������� - ������������ ������ ��� ����� ���� ������.
		// ��������� OSF � ������� � ����� ������ɻ.
		// ������ �� ���� ��������� � ������������ �������.
		for (int i = 0; i < (xPlcConfigService.
							 xPlcConfigServiceData.
							 ucInternalModulesQuantity); i++)
		{
			// ������ ������ ����5?
			if (xAllModulesContext.axAllModulesContext[i].
					xModuleContextStatic.ucModuleType == MODULE_TYPE_MTVI5)
			{
				// �������� ������� ������ ����5 - ������������ ������ ��� ����� ���� ������.
				// ��������� OSF � ������� � ����� ������ɻ.
				xAllModulesContext.axAllModulesContext[i].
				xModuleContextDinamic.
				ucCommandControl = DEVICE_CONTROL_MTVI5_LOAD_OUTS_OSF;
			}
		}
	}

	// ������� ��������� �� ����� � ��������� ������ ������� �������.
	pxEventData = xCAlarmEvent.EventDataPush();

	// �������� ������ �������.
	// ��������� id �������.
	(pxEventData -> ui16ID) =
		1;
	// ��������� ��� �������.
	(pxEventData -> ui8Type) =
		CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE;
	// ��������� ����� ��������� �������.
	(pxEventData -> ui16Address) = 0;
	// ��������� ��� �������.
	(pxEventData -> ui8State) =
		0;

	// ��������� ����� �������.
	(pxEventData -> xCurrentTime) =
		xCurrentTime;
	// �������� ������ ���������� ���������.
	sprintf((char*)(pxEventData -> acTextDescriptor),
			"%s",
			"������ ������");
	// �������� ������ ������ � ���� �������������� ��������� ������ ������� ������ � ���� ������.
	sprintf((char*)(pxEventData -> acTextDescriptorAdditional),
			"%s",
			" ");
	xCArchiveEventsDB.DataBaseDataPush(pxEventData);

	// ������ ��������� ���� ��������������� ����������� ������ ������� �� ���������� ������.
	// ����� ���� �������� ������ ������� � �������� - ��� �������, �� �������� ��������
	// �������. �� ������� ���������� ���������� � ���, ��� ������� ������ ���.
	// ����� �������� ������ ��������� ���� � ���, ��� ������� ������ ��� - �������� ��
	// ���������� ������ ����� ������ �������. ������� ��� �������� - ��� �������(ui16ID = 0).
	// ��������� ������ (ui16EventDataCommonPopIndex) �� ����� ���������� � ��������� ������ ������ ������� �������.
	xCAlarmEvent.EventDataPopIndexSet(0);
	// ������� ��������� �� ����� ���������� � ��������� ������ ������ ������� �������.
	pxEventData = xCAlarmEvent.EventDataPop();
	// ������� ������� �������� - ��� �������.
	pxEventData -> ui16ID = 0;
}


//-----------------------------------------------------------------------------------------------------
// ������� ������� ������������ ���������� �������� � �������������� ������.
// ���� ���� ������������, �� ������ ������� � �������� ������ � ������ ������� "������".
void CAlarm::AlarmEvetnsLogCreate(void)
{
	TAlarmHmi *pxAlarmHmi;
	uint8_t ui8EventIndex;
	TDiscreteSygnalTextDescriptor *pxDiscreteSygnalTextDescriptor;

//    // ������ ������� "������" �� �������?
//    if (!(CAlarm::ui8AlarmEventLogBriefIsUpdated))
//    {
	// ������� ��������� �� ����� � ��������������� ����� ������ �������.
	pxDiscreteSygnalTextDescriptor = axDiscreteSygnalTextDescriptor;
	// ������� ��������� �� ������� ������ ������������ ���������� ����������� ������������ ���������� �������� �� HMI.
	pxAlarmHmi = &axAlarmHmi[0];

//    // �������� ������� ����������� ������������, ���� �����������������.
//    for (int i = 0;
//            i < ((xPlcConfigService.xPlcConfigServiceData.
//                  uiHandledDiscreteInputQuantity) +
//                 (xPlcConfigService.xPlcConfigServiceData.
//                  uiHandledFunctionBlockQuantity));
//            i++, pxAlarmHmi++)
//    {
//        // ������������ �� �������� ���������������� ����������� �������
//        // ��� ��������������� ����� �������?
//        if (pxAlarmHmi -> ui8AlarmState)
//        {
//            CEvents::TEventDataCommon *pxEventData;
//            // ������� �����?
//            pxEventData = xCAlarmEvent.EventDataPushRequest(i);
//            // ���� ����������� ������������� �������?
//            if (pxEventData)
//            {
//                // ������������ ���������?
//                if ((pxAlarmHmi -> ui8AlarmState) & (0x01 << ALARM_TYPE_ALARM))
//                {
//                    fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ALARM;
//                }
//                // ������������ �����������������?
//                else if ((pxAlarmHmi -> ui8AlarmState) & (0x01 << ALARM_TYPE_PREVENT))
//                    // ����������������� ������������.
//                {
//                    fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_WARNING;
//                }
//                else
//                {
//                    fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_ALARM;
//                }
//
//                printf("AlarmEvetnsLogCreate: i %d\n\r", (i));
//                xCPlcArchiveEvent.EventDataPointerPush(pxEventData);
//                // ���� ���� ����������� ������������� �������,
//                // �� �� ���������(pxEventData) �� ����� � �������,
//                // �������� ������ �������.
//                // ��������� id �������.
//                (pxEventData -> ui16ID) =
//                    1;
//                // ��������� ��� �������.
//                (pxEventData -> ui8Type) =
//                    CEvents::HANDLED_EVENTS_ALARM_TYPE;
//                // ��������� ����� ��������� �������.
//                (pxEventData -> ui16Address) = (i + CONVERT_INTEGER_TO_NATURAL_NUMBER);
//                // ��������� ��� �������.
//                (pxEventData -> ui8State) =
//                    (pxAlarmHmi -> ui8AlarmState);
//                // ��������� ����� �������.
//                (pxEventData -> xCurrentTime) =
//                    xCurrentTime;
//                // �������� ������� - ���������� ����?
//                if (i < (xPlcConfigService.xPlcConfigServiceData.
//                         uiHandledDiscreteInputQuantity))
//                {
//                    // �������� ������� - ���������� ����.
//                    // ��������� ������ ���������� ���������.
//                    memcpy(&(pxEventData -> acTextDescriptor),
//                           (&(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor)),
//                           (CAlarm::EVENT_NAME_LENGTH + END_OF_STRING_LENGTH));
//                }
//                else
//                {
//                    // �������� ������� - �������������� ����.
//                    // �������� ������ ���������� ���������.
//                    sprintf((char*)(pxEventData -> acTextDescriptor),
//                            "%s%d",
//                            "��� #",
//                            ((i - ((xPlcConfigService.xPlcConfigServiceData.
//                                    uiHandledDiscreteInputQuantity) -
//                                   CONVERT_NATURAL_NUMBER_TO_INTEGER)) +
//                             CONVERT_INTEGER_TO_NATURAL_NUMBER));
//                }
//
//                // ������������?
//                if ((pxAlarmHmi -> ui8Archive))
//                {
//                    xCArchiveEventsDB.DataBaseDataPush(pxEventData);
//                }
//            }
//        }
//        else
//        {
//            // ������� �������� ����, ��� ������� ���� ������������� �����.
//            xCAlarmEvent.EventReset(i);
//        }
//    }
}

//-----------------------------------------------------------------------------------------------------
// ������� ��������� ������������ ���������� �������� � �������������� ������.
void vAlarmHandler(void)
{
	TAlarmHmi *pxAlarmHmi;
	unsigned char *pucGroupDinAction;
	unsigned char *pucDinAction;
	unsigned char *pucDoValueDestination;
	unsigned char *pucDoValueBlockDestination;
	unsigned char *pucBlincHmi;

	pxAlarmHmi = &axAlarmHmi[0];
	pucGroupDinAction = &aucCoilsArray[ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_OFFSET];
	pucDinAction = &aucCoilsArray[ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_OFFSET];
	pucDoValueDestination = &aucDoValueByteArray[0];
	pucDoValueBlockDestination = &aucDoValueBlockByteArray[0];//&aucCoilsArray[DO_VALUE_BIT_ARRAY_OFFSET];
	pucBlincHmi = &aucCoilsArray[HMI_COMMON_INTERMITTENT_SIGNAL_BITS_BIT_ARRAY_OFFSET];

	vGeneralTimeCounter();

//-----------------------------------------------------------------------------------------------------
	// ���������� �������� ������������� �� HMI.
	// ���� ���������� �������� �����������?
	if ((!fbBlinkIsOn) && (!nucBlinkCounter))
	{
		nucBlinkCounter = THREE_SECOND_TIME;
		// ���� ��������.
		fbBlinkIsOn = 1;
	}
	else
	{
		// ���� �������� �����������?
		if ((fbBlinkIsOn) && (!nucBlinkCounter))
		{
			nucBlinkCounter = TWO_SECOND_TIME;
			// ���� ���������� ��������.
			fbBlinkIsOn = 0;
		}
		else
		{
			nucBlinkCounter--;
		}
	}

//-----------------------------------------------------------------------------------------------------
	// ���������� ������� ������� �� ���� � �������� ���������.
	for (int i = 0;
			i < (DISCRETE_INPUT_RELAY_OUTPUT_QUANTITY);
			i++)
	{
		pucDoValueDestination[i] = 0;
		pucDoValueBlockDestination[i] = 0;
	}

//-----------------------------------------------------------------------------------------------------
	vDiscreteInputHandler();
	vFunctionBlockHandler();

//-----------------------------------------------------------------------------------------------------
//    CAlarm::AlarmEvetnsLogCreate();

//-----------------------------------------------------------------------------------------------------

	pxAlarmHmi = &axAlarmHmi[0];
	// ��������� ��� ����� ������������.
	// ������ �� ���� ������������������� ���������� �������� � �������������� ������.
	for (int i = 0;
			i < (ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH +
				 ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_LENGTH);
			i++, pxAlarmHmi++)
	{
		// ��� ��������� ������������?
		if (!((ucAlarmStateCommon >> ALARM_TYPE_ALARM) & 0x01))
		{
			// ��� �������� ����������� ������� ����������� ���������� ����������� ������������?
			if (((!((pxAlarmHmi -> ui8AlarmState) & (0x01 << ALARM_TYPE_PREVENT))) &&
					(((pxAlarmHmi -> ui8AlarmState) & (0x01 << ALARM_TYPE_ALARM)))) ||
					((((pxAlarmHmi -> ui8AlarmState) & (0x01 << ALARM_TYPE_PREVENT))) &&
					 (!((pxAlarmHmi -> ui8AlarmState) & (0x01 << ALARM_TYPE_ALARM)))))
			{
				// ������� ����� ����������� ��������� ������������.
				ucAlarmStateCommon = 0;
				// ��� �������� ����������� ������� ����������������� ����������������� ������������?
				if ((pxAlarmHmi -> ui8AlarmState) & (0x01 << ALARM_TYPE_PREVENT))
				{
					// ��������� ���� ����������������� ������������.
					ucAlarmStateCommon |= (0x01 << ALARM_TYPE_PREVENT);
				}
				else
				{
					// ������� ���� ����������������� ������������.
					ucAlarmStateCommon &= ~(0x01 << ALARM_TYPE_PREVENT);
				}
				// ��� �������� ����������� ������� ����������������� ��������� ������������?
				if ((pxAlarmHmi -> ui8AlarmState) & (0x01 << ALARM_TYPE_ALARM))
				{
					// ��������� ���� ��������� ������������.
					ucAlarmStateCommon |= (0x01 << ALARM_TYPE_ALARM);
				}
				else
				{
					// ������� ���� ��������� ������������.
					ucAlarmStateCommon &= ~(0x01 << ALARM_TYPE_ALARM);
				}
			}
		}
		else
		{
			break;
		}
	}

	// ������� ����� - ����� ����������� ������ ��������� ��� HMI. ����� ������������.
	pucBlincHmi[ALARM_TYPE_ALARM_OFFSET] = 0;
	pucBlincHmi[ALARM_TYPE_PREVENT_OFFSET] = 0;

	// ���� ����� ������������?
	if (ucAlarmStateCommon)
	{
		// ������������ �����������?
		if ((ucAlarmStateCommon & (0x01 << ALARM_TYPE_ALARM)) &&
				(ucAlarmStateCommon & (0x01 << ALARM_TYPE_PREVENT)))
		{
			// ���������� ��� ��������� ������������.
			pucBlincHmi[ALARM_TYPE_ALARM_OFFSET] = 1; // ��������� ������������
			// ���������� ��� ����������������� ������������.
			pucBlincHmi[ALARM_TYPE_PREVENT_OFFSET] = 1; // ����������������� ������������
		}
		else if (ucAlarmStateCommon & (0x01 << ALARM_TYPE_ALARM))
		{
			// ���� ��������?
			if (fbBlinkIsOn)
			{
				// ���������� ��� ��������� ������������.
				pucBlincHmi[ALARM_TYPE_ALARM_OFFSET] = 1; // ��������� ������������
			}
		}
		else if (ucAlarmStateCommon & (0x01 << ALARM_TYPE_PREVENT))
			// ����������������� ������������.
		{
			// ���� ��������?
			if (fbBlinkIsOn)
			{
				// ���������� ��� ����������������� ������������.
				pucBlincHmi[ALARM_TYPE_PREVENT_OFFSET] = 1; // ����������������� ������������
			}
		}
	}

//-----------------------------------------------------------------------------------------------------
	// ���� ����� ������������ ��� �����?
	if (fbGlobalKvit || fbGlobalReset)
	{

		pxAlarmHmi = &axAlarmHmi[0];
		// ������� �������� ����� ������������.
		ucAlarmStateCommon = 0;
		// ������� ���� �������� ������������.
		fucZvkMal = COMMAND_SOUND_SIGNAL_TYPE_OFF;
		// ���� ����� ������������?
		if (fbGlobalKvit && !fbGlobalReset)
		{
			// ��������� ������� ������������ ��� ���� �������������� ���������� ������ � �������������� ������.
			// ��������� ��� �������� ������������.
			for (int i = 0;
					i < (ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH +
						 ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_LENGTH);
					i++, pxAlarmHmi++)
			{
				// ������������ �� �������� ���������������� ����������� �����
				// ��� ��������������� ����� �������?
				if (pxAlarmHmi -> ui8AlarmState)
				{
					// ��������� ������� ������������ ������������ �������� �������.
					// ��������� ����� ������ ����:
					// ��������� ����� ����������������� ������������.
					pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
					// ��������� ���� ��������� ������������.
					pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
//                    axAlarmModules[i].ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
//                    // ��������� ���� ��������� ������������.
//                    axAlarmModules[i].ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
				}
			}
		}
		else
		{
			// ��������� ������� ����� ��� ���� �������������� ���������� ������ � �������������� ������.
			// ������� ��� �������� ������������.
			for (int i = 0;
					i < (ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH +
						 ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_LENGTH);
					i++, pxAlarmHmi++)
			{
				// ������������ �� �������� ���������������� ����������� �����
				// ��� ��������������� ����� �������?
				if (pxAlarmHmi -> ui8AlarmState)
				{
					// ������� ��� �������� ������������.
					*pxAlarmHmi = xAlarmHmiNull;
				}
			}
		}
	}

//-----------------------------------------------------------------------------------------------------
	// ������� ��� ������� ������������ ����� ���������� ��������.
	memset(pucGroupDinAction,
		   0,
		   (ALARM_GROUP_QUANTITY * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT));
	// ������� ��� ������� ������������ ���������� ��������.
	memset(pucDinAction,
		   0,
		   (ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH +
			ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_LENGTH));

	// ��������� ����������� ������������ � ������� ������������ HMI, ���� �����������������.
	pxAlarmHmi = &axAlarmHmi[0];
	for (int i = 0;
			i < (ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_LENGTH +
				 ALARM_ACTIVITY_FUNCTION_BLOCK_BIT_ARRAY_LENGTH);
			i++, pxAlarmHmi++)
	{
		// ������������ �� �������� ���������������� ����������� �����
		// ��� ��������������� ����� �������?
		if (pxAlarmHmi -> ui8AlarmState)
		{
			// ������������ �����������?
			if ((pxAlarmHmi -> ui8AlarmState & ALARM_TYPE_PREVENT) &&
					(pxAlarmHmi -> ui8AlarmState & ALARM_TYPE_ALARM))

			{
				// ��������� ������� ������������ ������������ �������� �������.
				pucDinAction[(i * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT)] = 1;
				pucDinAction[(i * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT) + 1] = 1;

				// ������� ������ ������ � ������?
				if (((pxAlarmHmi -> ui8AlarmState & LIGHT_BOARD_CELL_NUMBER_DISC_MASK) > 0) &&
						((pxAlarmHmi -> ui8AlarmState & LIGHT_BOARD_CELL_NUMBER_DISC_MASK) <= ALARM_GROUP_QUANTITY))
				{
					// ��������� ������� ������������ ������������ ��� ����� � ������� ������ ������� ������.
					pucGroupDinAction[(((pxAlarmHmi -> ui8AlarmState & LIGHT_BOARD_CELL_NUMBER_DISC_MASK) - 1) *
									   ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT)] = 1;

					pucGroupDinAction[(((pxAlarmHmi -> ui8AlarmState & LIGHT_BOARD_CELL_NUMBER_DISC_MASK) - 1) *
									   ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT) + 1] = 1;
				}
			}
			else
			{
				// ��������� ������� �������� ������������ �������� �������.
				pucDinAction[(i * ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT) + 1] = 1;
				// ������� ������ ������ � ������?
				if (((pxAlarmHmi -> ui8AlarmState & LIGHT_BOARD_CELL_NUMBER_DISC_MASK) > 0) &&
						((pxAlarmHmi -> ui8AlarmState & LIGHT_BOARD_CELL_NUMBER_DISC_MASK) <= ALARM_GROUP_QUANTITY))
				{
					// ��������� ������� �������� ������������ ��� ����� � ������� ������ ������� ������.
					pucGroupDinAction[(((pxAlarmHmi -> ui8AlarmState & LIGHT_BOARD_CELL_NUMBER_DISC_MASK) - 1) *
									   ALARM_TYPE_QUANTITY_OF_SIGNIFICANT_BIT) + 1] = 1;
				}
			}
		}
	}
}

