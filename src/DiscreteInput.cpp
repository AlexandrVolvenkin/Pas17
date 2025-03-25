
#include "PasNewConfig.h"
#include "DiscreteInput.h"
#include "Alarm.h"
#include "Events.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// ������������ ������������ ������������������� �������� ���������� ��������.
void vDiscreteInputHandler(void)
{
    unsigned char *pucSource;
    unsigned char *pucDiscreteInputSource;
    unsigned char *pucBadDiscreteInputSource;
    unsigned char *pucDoValueDestination;
    unsigned char *pucDoValueBlockDestination;
    unsigned char *pucGroupDinAction;
    unsigned char *pucDinAction;
    unsigned char ucFlowControl;
    TDiscreteInputDescriptionWork *pxDiscreteInputDataBase;
    TDiscreteInputWorkingData *pxDiscreteInputWorkingData;
    TAlarmHmi *pxAlarmHmi;

    // ������� ��������� �� ����� � ��������������� ����� ������ ���������� �������� �������.
    pxDiscreteInputDataBase = axDiscreteInputDescriptionWork;
    pxDiscreteInputWorkingData = axDiscreteInputWorkingData;
    pucDiscreteInputSource = &aucDiscreteInputsArray[DI_VALUE_BIT_ARRAY_OFFSET];
    pucBadDiscreteInputSource = &aucCoilsArray[BAD_DI_BIT_ARRAY_OFFSET];
    pucDoValueDestination = &aucDoValueByteArray[0];
    pucDoValueBlockDestination = &aucDoValueBlockByteArray[0];//&aucCoilsArray[DO_VALUE_BIT_ARRAY_OFFSET];
    // ������� ��������� �� ������� ������ ������������ ���������� ����������� ������������ ���������� �������� �� HMI.
    pxAlarmHmi = &axAlarmHmi[0];
    pucGroupDinAction = &aucCoilsArray[ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_OFFSET];
    pucDinAction = &aucCoilsArray[ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_OFFSET];


    // ���������� ��� ������������������� ���������� �������.
    for (int i = 0;
            i < (xPlcConfigService.xPlcConfigServiceData.
                 uiHandledDiscreteInputQuantity);
            i++)
    {
        // ������ ��������� ����������� ������� ����������?
        if (iDiscreteInputDataBaseCrcCheck(pxDiscreteInputDataBase) == -1)
        {
            fbDiscreteInputDataBaseError = 1;
//            cout << "iDiscreteInputDataBaseCrcCheck error" << endl;

            // �������� ��������� ������� ��� �� ����������������?
            if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                        SYSTEM_ERROR_GROUP_OFFSET,
                        SYSTEM_ERROR_DISCRETE_SYGNAL_DATA_BASE_ERROR_OFFSET))
            {
                // �������������� �������� ��������� �������.
                CEvents::EventRegistration(
                    SYSTEM_ERROR_GROUP_OFFSET,
                    (CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE |
                     CEvents::HANDLED_EVENTS_IS_POPUP |
                     CEvents::HANDLED_EVENTS_IS_SOUND |
                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                    SYSTEM_ERROR_DISCRETE_SYGNAL_DATA_BASE_ERROR_OFFSET,
                    "��.����.��������");
            }
        }
        else
        {
            // ���������� ������ ����������?
            if (*pucBadDiscreteInputSource)
            {
                // ������������� ���������� ������ ��������� ������.
                ucFlowControl = FLOW_CONTROL_NORMA;
            }
            // ���������� ������ �������?
            else if (((CONTACT_TYPE_DIS((pxDiscreteInputDataBase))) &&
                      (*pucDiscreteInputSource)) ||
                     (((CONTACT_TYPE_DIS((pxDiscreteInputDataBase))) == 0) &&
                      (*pucDiscreteInputSource) == 0))
            {
                // ���������� ������ ��� ������� �� ���������� ����?
                if (pxDiscreteInputWorkingData -> fbDiscreteInputIsActive)
                {
                    // �� ����� ���������.
                    ucFlowControl = FLOW_CONTROL_NOT_NEW_VIOLATION;
                }
                else
                {
                    // ����� ���������.
                    ucFlowControl = FLOW_CONTROL_NEW_VIOLATION;
                }
            }
            else
            {
                // �����.
                ucFlowControl = FLOW_CONTROL_NORMA;
            }

            // � ����� ��������� ���������� ������?
            switch(ucFlowControl)
            {
            case FLOW_CONTROL_NORMA:
//                cout << "FLOW_CONTROL_NORMA" << endl;
                // �����.
                // ���������� ������ ��� ������� �� ���������� ����?
                if (pxDiscreteInputWorkingData -> fbDiscreteInputIsActive)
                {
                    // ������� ���� ���������� ����������� �������.
                    pxDiscreteInputWorkingData -> fbDiscreteInputIsActive = 0;
                    // ������� ������� ������� �������� ���.
                    pxDiscreteInputWorkingData -> ucDelay = 0;
//                    // ���� ���������?
//                    if (ARCHIVE_DIS((pxDiscreteInputDataBase)))
//                    {
//                        // arch
//                        pxAlarmHmi -> ui8Archive = 1;
//                    }
//                    else
//                    {
//                        pxAlarmHmi -> ui8Archive = 0;
//                    }


                    if (ARCHIVE_DIS((pxDiscreteInputDataBase)))
                    {
                        // ���������� ��������� ������� ��� �� ����������������?
                        if(xCAlarmEvent.EventOffIsNotRegistered(
                                    i,
                                    0))
                        {
                            // �������������� ���������� ��������� �������.
                            CEvents::EventRegistration(
                                i,
                                (CEvents::HANDLED_EVENTS_DISCRETE_INPUTS_TYPE |
                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                                0,
                                0);
                        }
                    }
                    else
                    {
                        // ���������� ��������� ������� ��� �� ����������������?
                        if(xCAlarmEvent.EventOffIsNotRegistered(
                                    i,
                                    0))
                        {
                            // �������������� ���������� ��������� �������.
                            CEvents::EventRegistration(
                                i,
                                CEvents::HANDLED_EVENTS_DISCRETE_INPUTS_TYPE,
                                0,
                                0);
                        }
                    }
                }
                break;

            case FLOW_CONTROL_NEW_VIOLATION:
//                cout << "FLOW_CONTROL_NEW_VIOLATION" << endl;
                // ����� ���������.
                // ��������� ����� - ����� ���������, ��� ������������������� ����
                // � ������ ������� ���������� ���� - ����� ���������.
                // ���� ������ ������.
                for (int j = 0;
                        j < (DISCRETE_INPUT_RELAY_OUTPUT_QUANTITY /
                             MRXX_DISCRETE_OUTPUT_QUANTITY);
                        j++)
                {
                    // ���� ������ ����.
                    for (int k = 0;
                            k < (MRXX_DISCRETE_OUTPUT_QUANTITY);
                            k++)
                    {
                        // ��������� ���� - ����� ���������.
                        pucDoValueDestination[(j * MRXX_DISCRETE_OUTPUT_QUANTITY) + k] |=
                            (((pxDiscreteInputDataBase -> aucRelayOut[j]) >> k) & 0x01);
                    }
                }
                // ��������� ���� ���������� ����������� �������.
                pxDiscreteInputWorkingData -> fbDiscreteInputIsActive = 1;
                // �������� � ������� ������� ������� �������� ���.
                pxDiscreteInputWorkingData -> ucDelay =
                    DELAY_TIME_SETPOINT_DIS((pxDiscreteInputDataBase));

                // �������� �� �����������������?
                if (!(DELAY_TIME_SETPOINT_DIS((pxDiscreteInputDataBase))))
                {
                    // ��������� ����� - ���������� ���������, ��� ������������������� ����,
                    // � ������ ������� ���������� ��������� ���� - ����������.
                    // ���� ������ ������.
                    for (int j = 0;
                            j < (DISCRETE_INPUT_RELAY_OUTPUT_QUANTITY /
                                 MRXX_DISCRETE_OUTPUT_QUANTITY);
                            j++)
                    {
                        // ���� ������ ����.
                        for (int k = 0;
                                k < (MRXX_DISCRETE_OUTPUT_QUANTITY);
                                k++)
                        {
                            // ��������� ���� - ���������� ��������� ����.
                            pucDoValueBlockDestination[(j * MRXX_DISCRETE_OUTPUT_QUANTITY) + k] |=
                                (((pxDiscreteInputDataBase -> aucRelayOut[j]) >> k) & 0x01);
                        }
                    }
                }
//                // ���� ���������?
//                if (ARCHIVE_DIS((pxDiscreteInputDataBase)))
//                {
//                    // arch
//                    pxAlarmHmi -> ui8Archive = 1;
//                }
//                else
//                {
//                    pxAlarmHmi -> ui8Archive = 0;
//                }

//                // ��������� ��� ����� ������������.
//                // ��� ��������� ������������?
//                if (!((ucAlarmStateCommon >> ALARM_TYPE_ALARM) & 0x01))
//                {
//                    // ��� �������� ����������� ������� ����������������� ������������ ��� ���������?
//                    if ((SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase))) ||
//                            (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase))))
//                    {
//                        // ��� �������� ����������� ������� ����������������� ������������?
//                        if (((!SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase))) &&
//                                (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))) ||
//                                ((SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase))) &&
//                                 (!SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))))
//                        {
//                            // ������� ����� ����������� ��������� ������������.
//                            ucAlarmStateCommon = 0;
//                            // ��� �������� ����������� ������� ����������������� ����������������� ������������?
//                            if (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))
//                            {
//                                // ��������� ���� ����������������� ������������.
//                                ucAlarmStateCommon |= (0x01 << ALARM_TYPE_PREVENT);
//                            }
//                            else
//                            {
//                                // ������� ���� ����������������� ������������.
//                                ucAlarmStateCommon &= ~(0x01 << ALARM_TYPE_PREVENT);
//                            }
//                            // ��� �������� ����������� ������� ����������������� ��������� ������������?
//                            if (SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase)))
//                            {
//                                // ��������� ���� ��������� ������������.
//                                ucAlarmStateCommon |= (0x01 << ALARM_TYPE_ALARM);
//                            }
//                            else
//                            {
//                                // ������� ���� ��������� ������������.
//                                ucAlarmStateCommon &= ~(0x01 << ALARM_TYPE_ALARM);
//                            }
//                        }
//                    }
//                }
//
//                // ��������� ��� ������������ ��� �������� ����������� �������.
//                // ������� ����� ����������� ��������� ������������.
//                pxAlarmHmi -> ui8AlarmState = 0;
//                // ��� �������� ����������� ������� ����������������� ����������������� ������������?
//                if (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))
//                {
//                    // ��������� ���� ����������������� ������������.
//                    pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
//                }
//                else
//                {
//                    // ������� ���� ����������������� ������������.
//                    pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_PREVENT);
//                }
//                // ��� �������� ����������� ������� ����������������� ��������� ������������?
//                if (SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase)))
//                {
//                    // ��������� ���� ��������� ������������.
//                    pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
//                }
//                else
//                {
//                    // ������� ���� ��������� ������������.
//                    pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_ALARM);
//                }

                // ��� �������� ����������� ������� ����������������� ������������ ��� ���������?
//                if ((SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase))) ||
//                        (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase))))
//                {
//                    // ��� �������� ����������� ������� ����������������� ������������?
//                    if (((!SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase))) &&
//                            (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))) ||
//                            ((SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase))) &&
//                             (!SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))))
//                    {
//                        // ��� �������� ����������� ������� ����������������� ����������������� ������������?
//                        if (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))
//                        {
//                            // ��������� ���� ����������������� ������������.
//                            pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
//                        }
//                        else
//                        {
//                            // ������� ���� ����������������� ������������.
//                            pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_PREVENT);
//                        }
//                        // ��� �������� ����������� ������� ����������������� ��������� ������������?
//                        if (SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase)))
//                        {
//                            // ��������� ���� ��������� ������������.
//                            pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
//                        }
//                        else
//                        {
//                            // ������� ���� ��������� ������������.
//                            pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_ALARM);
//                        }
//                    }
//                }
                break;

            case FLOW_CONTROL_NOT_NEW_VIOLATION:
//                cout << "FLOW_CONTROL_NOT_NEW_VIOLATION" << endl;
                // �� ����� ���������.
                // ����� �������� �������?
                if (!(pxDiscreteInputWorkingData -> ucDelay))
                {
                    // ��������� ����� - ���������� ���������, ��� ������������������� ����,
                    // � ������ ������� ���������� ��������� ���� - ����������.
                    // ���� ������ ������.
                    for (int j = 0;
                            j < (DISCRETE_INPUT_RELAY_OUTPUT_QUANTITY /
                                 MRXX_DISCRETE_OUTPUT_QUANTITY);
                            j++)
                    {
                        // ���� ������ ����.
                        for (int k = 0;
                                k < (MRXX_DISCRETE_OUTPUT_QUANTITY);
                                k++)
                        {
                            // ��������� ���� - ���������� ��������� ����.
                            pucDoValueBlockDestination[(j * MRXX_DISCRETE_OUTPUT_QUANTITY) + k] |=
                                (((pxDiscreteInputDataBase -> aucRelayOut[j]) >> k) & 0x01);
                        }
                    }
                }
                else
                {
                    // ����� �������� �� �������.
                    // ������������ ������� �������� ��� - 1 �������?
                    if (!(DELAY_TIME_DISCRETENESS_DIS((pxDiscreteInputDataBase))))
                    {
                        // ������������ ������� �������� ��� - 1 �������.
                        // ������ ���� �������?
                        if (fbOneSecondPassed)
                        {
                            pxDiscreteInputWorkingData -> ucDelay -= 1;
                        }
                    }
                    else
                    {
                        // ������������ ������� �������� ��� - 10 ������.
                        // ������ ������ ������?
                        if (fbTenSecondsPassed)
                        {
                            pxDiscreteInputWorkingData -> ucDelay -= 1;
                        }
                    }
                }
                break;

            default:
                break;
            };

//            // ������������ �� �����������������? ���, ����������������� ���������?
//            if (((SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase))) &&
//                    (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))) ||
//                    ((!(SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase)))) &&
//                     (!(SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase))))))
            // ������������ �� �����������������?
            if ((!(SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase)))) &&
                    (!(SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))))
            {
                // ������� ����� ���������� ����������� ������������ �� HMI - �� � �����.
                *pxAlarmHmi = xAlarmHmiNull;
            }
            else
            {
                // ����������������� ������������.
                // ���������� ������ �������?
                if (pxDiscreteInputWorkingData -> fbDiscreteInputIsActive)
                {
                    // ��� �������� ����������� ������� ����������� ���������� ����������� ������������ �� HMI?
                    if (pxAlarmHmi -> ui8AlarmState)
                    {

                    }
                    else
                    {
                        // ������ ������ ��� ����������� ������������ �� HMI.
                        pxAlarmHmi -> ui8AlarmState =
                            LIGHT_BOARD_CELL_NUMBER_DIS((pxDiscreteInputDataBase));
                        // ��������� ��� ������������ ��� �������� ����������� �������.
                        // ��� �������� ����������� ������� ����������������� ����������������� ������������?
                        if (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))
                        {
                            // ��������� ���� ����������������� ������������.
                            pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
                        }
                        else
                        {
                            // ������� ���� ����������������� ������������.
                            pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_PREVENT);
                        }
                        // ��� �������� ����������� ������� ����������������� ��������� ������������?
                        if (SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase)))
                        {
                            // ��������� ���� ��������� ������������.
                            pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
                        }
                        else
                        {
                            // ������� ���� ��������� ������������.
                            pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_ALARM);
                        }

                        if (ARCHIVE_DIS((pxDiscreteInputDataBase)))
                        {
                            // �������� ��������� ������� ��� �� ����������������?
                            if(xCAlarmEvent.EventOnIsNotRegistered(
                                        i,
                                        0))
                            {
                                // �������������� �������� ��������� �������.
                                CEvents::EventRegistration(
                                    i,
                                    (CEvents::HANDLED_EVENTS_DISCRETE_INPUTS_TYPE |
                                     CEvents::HANDLED_EVENTS_IS_POPUP |
                                     CEvents::HANDLED_EVENTS_IS_SOUND |
                                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                                    (pxAlarmHmi -> ui8AlarmState),
                                    0);
                            }
                        }
                        else
                        {
                            // �������� ��������� ������� ��� �� ����������������?
                            if(xCAlarmEvent.EventOnIsNotRegistered(
                                        i,
                                        0))
                            {
                                // �������������� �������� ��������� �������.
                                CEvents::EventRegistration(
                                    i,
                                    (CEvents::HANDLED_EVENTS_DISCRETE_INPUTS_TYPE |
                                     CEvents::HANDLED_EVENTS_IS_POPUP |
                                     CEvents::HANDLED_EVENTS_IS_SOUND),
                                    (pxAlarmHmi -> ui8AlarmState),
                                    0);
                            }
                        }
                    }
                }
                else
                {
//            cout << "FLOW_CONTROL_NOT_NEW_VIOLATION 12" << endl;
                    // ��� �������� ����������� ������� ����������� ���������� ����������� ������������ �� HMI?
                    if (pxAlarmHmi -> ui8AlarmState)
                    {
//            cout << "FLOW_CONTROL_NOT_NEW_VIOLATION 13" << endl;
                        // ���������� ����������� ������������ �� HMI ��������?
                        if (fbGlobalReset)
                        {
                            // ������� ����� ���������� ����������� ������������ �� HMI - �� � �����.
                            *pxAlarmHmi = xAlarmHmiNull;
                        }
                        else
                        {

                        }
                    }
                    else
                    {

                    }
                }
            }
        }
        pucDiscreteInputSource++;
        pxDiscreteInputDataBase++;
        pxDiscreteInputWorkingData++;
        pucBadDiscreteInputSource++;
        pxAlarmHmi++;
    }
}

//-----------------------------------------------------------------------------------------------------
// ������ ��������� ���� ������ ��������� ���������� ��������.
int iDiscreteInputStartDataBaseCreate(void)
{
    int i;
    unsigned char ucCheck;
    unsigned char nucBlockCounter;
    unsigned char nucBlocksInBlockCounter; // ���� ��������� - ���� ����, � ����� �����.
    unsigned char nucInputCounter;
    unsigned char nucModuleCounter;
    unsigned char *pucSource;
    unsigned char ucFlowControl;
    TDiscreteInputDescriptionWork *pxDiscreteInputDescriptionWork;
    TDiscreteInputDescriptionWorkPackOne *pxDiscreteInputDescriptionWorkPackOne;
    TDiscreteInputDescriptionWorkPackOne xDiscreteInputDescriptionWorkSourse;
    // ������� ��������� �� ����� ��� ��������������� ��������� ���� ������ �������.
    pxDiscreteInputDescriptionWork = axDiscreteInputDescriptionWork;
    // ����� � ������� �����.
    nucInputCounter = 0;
    // ����� � ������� ������.
    nucModuleCounter = 1;
    i = 0;

    memset((unsigned char*)axDiscreteInputDescriptionWork,
           0,
           (sizeof(struct TDiscreteInputDescriptionWork)));

    // �������� ������ ����� ��������� ���� ������ ���������� ��������.
    // ��� �������� ����������� �������� ����������� �����.
    for (int i = 0;
            i < (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity);
            i++)
    {
        for (int j = 0;
                j < DISCRETE_MODULE_INPUT_QUANTITY;
                j++)
        {
            // �������� ����� ������. ���������� � �������� - nucModuleCounter. ��� ������� ������ ���� ����� ������.
            pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
            // ��� �������� - ��������� �����������.
            pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
            // ��������� ������������.
            pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
            pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
            // ������ ����� ���������� ����, ������� ������ ���� ��1.
            pxDiscreteInputDescriptionWork[nucInputCounter].aucRelayOut[0] = 1;
            // ����� �������� - 5 ������.
            pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay |= (DELAY_TIME_SETPOINT_DISC_MASK & 5);
            // ������������ ������� �������� - 1 �������.
            pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay &= ~DELAY_TIME_DISCRETENESS_DISC_MASK;
            // �� ������������.
            pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay &= ~ARCHIVE_DISC_MASK;

            // �������� CRC ���������.
            pucSource = (unsigned char*)&pxDiscreteInputDescriptionWork[nucInputCounter];
            ucCheck = 0xFF;
            for (int k = 0;
                    k < (sizeof(struct TDiscreteInputDescriptionWork) - ONE_BYTE_CRC_LENGTH);
                    k++)
            {
                ucCheck += pucSource[k];
            }
            pxDiscreteInputDescriptionWork[nucInputCounter].ucCrc = ucCheck;

            // ��������� ���� ������ ����������� �����.
            // ��������� ���������.
            nucInputCounter++;

        }

        // ��������� ������.
        nucModuleCounter++;
    }



//    // �������� ������ ����� ��������� ���� ������ ���������� ��������.
//    // ��� �������� ����������� �������� ����������� �����.
//    while (i < (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity *
//                DISCRETE_MODULE_INPUT_QUANTITY))
//    {
//        // �������� ����� ������. ���������� � �������� - nucModuleCounter. ��� ������� ������ ���� ����� ������.
//        pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
//        // ��� �������� - ��������� �����������.
//        pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
//        // ��������� ������������.
//        pxDiscreteInputDescriptionWork[i].ucTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
//        pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
//        // ������ ����� ���������� ����, ������� ������ ���� ��1.
//        pxDiscreteInputDescriptionWork[i].aucRelayOut[0] = 1;
//        // ����� �������� - 5 ������.
//        pxDiscreteInputDescriptionWork[i].ucDelay |= (DELAY_TIME_SETPOINT_DISC_MASK & 5);
//        // ������������ ������� �������� - 1 �������.
//        pxDiscreteInputDescriptionWork[i].ucDelay &= ~DELAY_TIME_DISCRETENESS_DISC_MASK;
//        // �� ������������.
//        pxDiscreteInputDescriptionWork[i].ucDelay &= ~ARCHIVE_DISC_MASK;
//
//        // �������� CRC ���������.
//        pucSource = (unsigned char*)&pxDiscreteInputDescriptionWork[i];
//        ucCheck = 0xFF;
//        for (int j = 0;
//                j < (sizeof(struct TDiscreteInputDescriptionWork) - ONE_BYTE_CRC_LENGTH);
//                j++)
//        {
//            ucCheck += pucSource[j];
//        }
//        pxDiscreteInputDescriptionWork[i].ucCrc = ucCheck;
//
//        // ��������� ���� ������ ����������� �����.
//        nucInputCounter++;
//        // ��������� ���������.
//        i++;
//        // ���������� ��� ����� ������ ����������� �����?
//        if (nucInputCounter == DISCRETE_MODULE_INPUT_QUANTITY)
//        {
//            // ����� � ������� �����.
//            nucInputCounter = 0;
//            // ��������� ������.
//            nucModuleCounter++;
//        }
//    }

//    i = nucInputCounter;
//    nucInputCounter = 0;



    // �������� ������ ����� ��������� ���� ������ ���������� ��������.
    // ��� �������� ����������� �������� ����������� �����.
    // ���� ���������� ���� ��������� ������ ���������� ������� - �������: LL, L, H, HH.
    // ����� � �������� ��������� ��� ������� LL+L.
    ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;

    for (int i = 0;
            i < (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity);
            i++)
    {
        for (int j = 0;
                j < (ANALOG_MODULE_INPUT_QUANTITY *
                     ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                j++)
        {
            switch(ucFlowControl)
            {
            case ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L:
                // �������� ����� ������. ���������� � �������� - nucModuleCounter. ��� ������� ������ ���� ����� ������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // ��� �������� - ��������� �����������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // ��������� ������������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
                // ��� ������ ���������� ����, ������� ������ ���� ��1.
                pxDiscreteInputDescriptionWork[nucInputCounter].aucRelayOut[0] = 0;
                // ��� ��������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay = 0;
                // ������������ ������� �������� - 1 �������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay = 0;
                // �� ������������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay &= ~ARCHIVE_DISC_MASK;
                // ������� � ���������� ��������� ��������� �������.
                ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                break;

            case ANALOGUE_INPUT_SET_POINT_VIOLATION_L:
                // �������� ����� ������. ���������� � �������� - nucModuleCounter. ��� ������� ������ ���� ����� ������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // ��� �������� - ��������� �����������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // ����������������� ������������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp &= ~SIGNALING_TYPE_2_DISC_MASK;
                // ��� ������ ���������� ����, ������� ������ ���� ��1.
                pxDiscreteInputDescriptionWork[nucInputCounter].aucRelayOut[0] = 0;
                // ��� ��������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay = 0;
                // ������������ ������� �������� - 1 �������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay = 0;
                // �� ������������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay &= ~ARCHIVE_DISC_MASK;
                // ������� � ���������� ��������� ��������� �������.
                ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                break;

            case ANALOGUE_INPUT_SET_POINT_VIOLATION_H:
                // �������� ����� ������. ���������� � �������� - nucModuleCounter. ��� ������� ������ ���� ����� ������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // ��� �������� - ��������� �����������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // ����������������� ������������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp &= ~SIGNALING_TYPE_2_DISC_MASK;
                // ��� ������ ���������� ����, ������� ������ ���� ��1.
                pxDiscreteInputDescriptionWork[nucInputCounter].aucRelayOut[0] = 0;
                // ��� ��������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay = 0;
                // ������������ ������� �������� - 1 �������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay = 0;
                // �� ������������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay &= ~ARCHIVE_DISC_MASK;
                // ������� � ���������� ��������� ��������� �������.
                ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                break;

            case ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H:
                // �������� ����� ������. ���������� � �������� - nucModuleCounter. ��� ������� ������ ���� ����� ������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // ��� �������� - ��������� �����������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // ��������� ������������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
                // ��� ������ ���������� ����, ������� ������ ���� ��1.
                pxDiscreteInputDescriptionWork[nucInputCounter].aucRelayOut[0] = 0;
                // ��� ��������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay = 0;
                // ������������ ������� �������� - 1 �������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay = 0;
                // �� ������������.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay &= ~ARCHIVE_DISC_MASK;
                // ������� � ���������� ��������� ��������� �������.
                ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                break;

            default:
                break;
            };

            // �������� CRC ���������.
            pucSource = (unsigned char*)&pxDiscreteInputDescriptionWork[nucInputCounter];
            ucCheck = 0xFF;
            for (int k = 0;
                    k < (sizeof(struct TDiscreteInputDescriptionWork) - ONE_BYTE_CRC_LENGTH);
                    k++)
            {
                ucCheck += pucSource[k];
            }
            pxDiscreteInputDescriptionWork[nucInputCounter].ucCrc = ucCheck;

            // ��������� ���� ������ ����������� �����.
            // ��������� ���������.
            nucInputCounter++;

        }

        // ��������� ������.
        nucModuleCounter++;
    }


//    // �������� ������ ����� ��������� ���� ������ ���������� ��������.
//    // ��� �������� ����������� �������� ����������� �����.
//    // ���� ���������� ���� ��������� ������ ���������� ������� - �������: LL, L, H, HH.
//    // ����� � �������� ��������� ��� ������� LL+L.
//    ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
//    while (i < ((xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity *
//                 DISCRETE_MODULE_INPUT_QUANTITY) +
//                (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity *
//                 ANALOG_MODULE_INPUT_QUANTITY *
//                 ANALOGUE_INPUT_DI_VALUE_QUANTITY)))
//    {
//        switch(ucFlowControl)
//        {
//        case ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L:
//            // �������� ����� ������. ���������� � �������� - nucModuleCounter. ��� ������� ������ ���� ����� ������.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
//            // ��� �������� - ��������� �����������.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
//            // ��������� ������������.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
//            // ��� ������ ���������� ����, ������� ������ ���� ��1.
//            pxDiscreteInputDescriptionWork[i].aucRelayOut[0] = 0;
//            // ��� ��������.
//            pxDiscreteInputDescriptionWork[i].ucDelay = 0;
//            // ������������ ������� �������� - 1 �������.
//            pxDiscreteInputDescriptionWork[i].ucDelay = 0;
//            // �� ������������.
//            pxDiscreteInputDescriptionWork[i].ucDelay &= ~ARCHIVE_DISC_MASK;
//            // ������� � ���������� ��������� ��������� �������.
//            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
//            break;
//
//        case ANALOGUE_INPUT_SET_POINT_VIOLATION_L:
//            // �������� ����� ������. ���������� � �������� - nucModuleCounter. ��� ������� ������ ���� ����� ������.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
//            // ��� �������� - ��������� �����������.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
//            // ����������������� ������������.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= SIGNALING_TYPE_1_DISC_MASK;
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp &= ~SIGNALING_TYPE_2_DISC_MASK;
//            // ��� ������ ���������� ����, ������� ������ ���� ��1.
//            pxDiscreteInputDescriptionWork[i].aucRelayOut[0] = 0;
//            // ��� ��������.
//            pxDiscreteInputDescriptionWork[i].ucDelay = 0;
//            // ������������ ������� �������� - 1 �������.
//            pxDiscreteInputDescriptionWork[i].ucDelay = 0;
//            // �� ������������.
//            pxDiscreteInputDescriptionWork[i].ucDelay &= ~ARCHIVE_DISC_MASK;
//            // ������� � ���������� ��������� ��������� �������.
//            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
//            break;
//
//        case ANALOGUE_INPUT_SET_POINT_VIOLATION_H:
//            // �������� ����� ������. ���������� � �������� - nucModuleCounter. ��� ������� ������ ���� ����� ������.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
//            // ��� �������� - ��������� �����������.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
//            // ����������������� ������������.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= SIGNALING_TYPE_1_DISC_MASK;
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp &= ~SIGNALING_TYPE_2_DISC_MASK;
//            // ��� ������ ���������� ����, ������� ������ ���� ��1.
//            pxDiscreteInputDescriptionWork[i].aucRelayOut[0] = 0;
//            // ��� ��������.
//            pxDiscreteInputDescriptionWork[i].ucDelay = 0;
//            // ������������ ������� �������� - 1 �������.
//            pxDiscreteInputDescriptionWork[i].ucDelay = 0;
//            // �� ������������.
//            pxDiscreteInputDescriptionWork[i].ucDelay &= ~ARCHIVE_DISC_MASK;
//            // ������� � ���������� ��������� ��������� �������.
//            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
//            break;
//
//        case ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H:
//            // �������� ����� ������. ���������� � �������� - nucModuleCounter. ��� ������� ������ ���� ����� ������.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
//            // ��� �������� - ��������� �����������.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
//            // ��������� ������������.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
//            // ��� ������ ���������� ����, ������� ������ ���� ��1.
//            pxDiscreteInputDescriptionWork[i].aucRelayOut[0] = 0;
//            // ��� ��������.
//            pxDiscreteInputDescriptionWork[i].ucDelay = 0;
//            // ������������ ������� �������� - 1 �������.
//            pxDiscreteInputDescriptionWork[i].ucDelay = 0;
//            // �� ������������.
//            pxDiscreteInputDescriptionWork[i].ucDelay &= ~ARCHIVE_DISC_MASK;
//            // ������� � ���������� ��������� ��������� �������.
//            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
//            break;
//
//        default:
//            break;
//        };
//
//        // �������� CRC ���������.
//        pucSource = (unsigned char*)&pxDiscreteInputDescriptionWork[i];
//        ucCheck = 0xFF;
//        for (int j = 0;
//                j < (sizeof(struct TDiscreteInputDescriptionWork) - ONE_BYTE_CRC_LENGTH);
//                j++)
//        {
//            ucCheck += pucSource[j];
//        }
//        pxDiscreteInputDescriptionWork[i].ucCrc = ucCheck;
//
//        // ��������� ����.
//        nucInputCounter++;
//        // ��������� ���������.
//        i++;
//
//        // ���������� ��� ���������� ������� ���������� ������� ������ ����������� �����?
//        if (nucInputCounter == (ANALOG_MODULE_INPUT_QUANTITY *
//                                ANALOGUE_INPUT_DI_VALUE_QUANTITY))
//        {
//            // ����� � ������� �����.
//            nucInputCounter = 0;
//            // ��������� ������.
//            nucModuleCounter++;
//        }
//    }

    // ������� ��������� �� ����� � ����� ��������� ��������������� ��������� ����� ������ �������.
    pxDiscreteInputDescriptionWork = axDiscreteInputDescriptionWork;
    // ������� ��������� �� ���� ������ ������� � ����� �������.
    pxDiscreteInputDescriptionWorkPackOne = (TDiscreteInputDescriptionWorkPackOne*)&(xPlcDataBase.
                                            axPlcDataBaseBlocks[DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET].
                                            aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;

    // ����������� ��������� ���� ������ � ����� ������.
    for (int i = 0;
            i < MAX_HANDLED_DISCRETE_INPUT;
            i++)
    {
        // ��������� ���� ���� ���� ������?
        if (nucBlocksInBlockCounter == DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // ��������� ���� ���� ������.
            nucBlockCounter++;
            // ������� ��������� �� ��������� ���� � ����� ���� ������ �������.
            pxDiscreteInputDescriptionWorkPackOne = (TDiscreteInputDescriptionWorkPackOne*)&(xPlcDataBase.
                                                    axPlcDataBaseBlocks[DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                                                    aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }

        // ��������� ���� ��������� ����������� �������, � ����� ������ �������.
        xDiscreteInputDescriptionWorkSourse.ucTalTkGrp =
            pxDiscreteInputDescriptionWork[i].ucTalTkGrp;

        memcpy(xDiscreteInputDescriptionWorkSourse.aucRelayOut,
               pxDiscreteInputDescriptionWork[i].aucRelayOut,
               8);

        xDiscreteInputDescriptionWorkSourse.ucDelay =
            pxDiscreteInputDescriptionWork[i].ucDelay;
        xDiscreteInputDescriptionWorkSourse.ucCrc =
            pxDiscreteInputDescriptionWork[i].ucCrc;

        memcpy((uint8_t*)&pxDiscreteInputDescriptionWorkPackOne[nucBlocksInBlockCounter],
               (uint8_t*)&xDiscreteInputDescriptionWorkSourse,
               sizeof(struct TDiscreteInputDescriptionWorkPackOne));

        // ��������� ���������.
        nucBlocksInBlockCounter++;
    }
}

//-----------------------------------------------------------------------------------------------------
// ������ ��������� ���� ������ ��������� ���������� ���������� ��������.
int iDiscreteSygnalTextDescriptorStartDataBaseCreate(void)
{
    int i;
    unsigned char nucBlockCounter;
    unsigned char nucBlocksInBlockCounter; // ���� ��������� - ���� ����, � ����� �����.
    unsigned char nucDiscreteInputCounter;
    unsigned char nucModuleInputCounter;
    // ���������� ������� �� ������ �����(4 - LL, L, H, HH).
    unsigned char nucInputSetPointCounter;
    unsigned char nucModuleCounter;
    unsigned char ucFlowControl;
    TDiscreteSygnalTextDescriptorPackOne *pxDiscreteSygnalTextDescriptorPackOne;
    TDiscreteSygnalTextDescriptorPackOne xDiscreteSygnalTextDescriptorPackOne;
    TDiscreteSygnalTextDescriptor *pxDiscreteSygnalTextDescriptor;


    // ������� ��������� �� ����� ��� ��������������� ��������� ���� ������ �������.
    pxDiscreteSygnalTextDescriptor = axDiscreteSygnalTextDescriptor;
    // ����� � ������� �����.
    nucDiscreteInputCounter = 0;
    nucModuleInputCounter = 0;
    // ����� � ������� ������.
    nucModuleCounter = 1;
    i = 0;

    memset((unsigned char*)pxDiscreteSygnalTextDescriptor,
           0,
           (sizeof(struct TDiscreteSygnalTextDescriptor)));

    // �������� ������ ����� ��������� ���� ������ ��������� ���������� ���������� ��������.
    // ��� �������� ����������� �������� ����������� �����.
    while (i < (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity *
                DISCRETE_MODULE_INPUT_QUANTITY))
    {
        // �������� ������ ���������� ��������� ����������� �������.
        sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                "%s_%02d_%02d  ",
                "��. DI",
                nucModuleCounter,
                nucDiscreteInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
        // ��������� ���� ������ ����������� �����.
        nucDiscreteInputCounter++;
        // ��������� ���������.
        i++;
        // ���������� ��� ����� ������ ����������� �����?
        if (nucDiscreteInputCounter == DISCRETE_MODULE_INPUT_QUANTITY)
        {
            // ����� � ������� �����.
            nucDiscreteInputCounter = 0;
            // ��������� ������.
            nucModuleCounter++;
        }
    }

    // ����� � ������� �����.
    nucDiscreteInputCounter = 0;
    nucModuleInputCounter = 0;
    // ����� � ������ ������� - LL+L.
    nucInputSetPointCounter = 0;
    // ����� � ������� ������.
    nucModuleCounter = 1;

    // �������� ������ ����� ��������� ���� ������ ��������� ���������� ���������� ��������.
    // ��� �������� ����������� �������� ����������� �����.
    // ���� ���������� ���� ��������� ������ ���������� ������� - �������: LL, L, H, HH.
    // ����� � �������� ��������� ��� ������� LL+L.
    ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
    while (i < ((xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity *
                 DISCRETE_MODULE_INPUT_QUANTITY) +
                (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity *
                 ANALOG_MODULE_INPUT_QUANTITY *
                 ANALOGUE_INPUT_DI_VALUE_QUANTITY)))
    {
        switch(ucFlowControl)
        {
        case ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L:
            // �������� ������ ���������� ��������� ����������� �������.
            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                    "%s_%02d_%01d_LL",
                    "��. AI",
                    nucModuleCounter,
                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
            // ������� � ���������� ��������� ��������� �������.
            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
            break;

        case ANALOGUE_INPUT_SET_POINT_VIOLATION_L:
            // �������� ������ ���������� ��������� ����������� �������.
            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                    "%s_%02d_%01d_L ",
                    "��. AI",
                    nucModuleCounter,
                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);

            // ������� � ���������� ��������� ��������� �������.
            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
            break;

        case ANALOGUE_INPUT_SET_POINT_VIOLATION_H:
            // �������� ������ ���������� ��������� ����������� �������.
            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                    "%s_%02d_%01d_H ",
                    "��. AI",
                    nucModuleCounter,
                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);

            // ������� � ���������� ��������� ��������� �������.
            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
            break;

        case ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H:
            // �������� ������ ���������� ��������� ����������� �������.
            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                    "%s_%02d_%01d_HH",
                    "��. AI",
                    nucModuleCounter,
                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);

            // ������� � ���������� ��������� ��������� �������.
            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
            break;

        default:
            break;
        };

        nucDiscreteInputCounter++;
        nucInputSetPointCounter++;
        // ���������� ������� �� ������ �����(4 - LL, L, H, HH).
        // ������ �� ���� �������� ������ �����?
        if (nucInputSetPointCounter >= 4)
        {
            nucInputSetPointCounter = 0;
            // ��������� ����.
            nucModuleInputCounter++;
        }
        // ��������� ���������.
        i++;

        // ���������� ��� ���������� ������� ���������� ������� ������ ����������� �����?
        if (nucDiscreteInputCounter == (ANALOG_MODULE_INPUT_QUANTITY *
                                        ANALOGUE_INPUT_DI_VALUE_QUANTITY))
        {
            // ����� � ������� �����.
            nucDiscreteInputCounter = 0;
            nucModuleInputCounter = 0;
            // ����� � ������ ������� - LL+L.
            nucInputSetPointCounter = 0;
            // ��������� ������.
            nucModuleCounter++;
        }
    }

    // ������� ��������� �� ����� � ����� ��������� ��������������� ��������� ����� ������ �������.
    pxDiscreteSygnalTextDescriptor = axDiscreteSygnalTextDescriptor;
    // ������� ��������� �� ���� ������ ������� � ����� �������.
    pxDiscreteSygnalTextDescriptorPackOne = (TDiscreteSygnalTextDescriptorPackOne*)&(xPlcDataBase.
                                            axPlcDataBaseBlocks[TEXT_TITLES_DATA_BASE_BLOCK_OFFSET].
                                            aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;

    // ����������� ��������� ���� ������ � ����� ������.
    for (int i = 0;
            i < MAX_HANDLED_DISCRETE_INPUT;
            i++)
    {
        // ��������� ���� ���� ���� ������?
        if (nucBlocksInBlockCounter == TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // ��������� ���� ���� ������.
            nucBlockCounter++;
            // ������� ��������� �� ��������� ���� � ����� ���� ������ �������.
            pxDiscreteSygnalTextDescriptorPackOne = (TDiscreteSygnalTextDescriptorPackOne*)&(xPlcDataBase.
                                                    axPlcDataBaseBlocks[TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                                                    aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }

        // ��������� ���� ��������� ��������� ���������� ����������� �������, � ����� ������ �������.
        memcpy((&xDiscreteSygnalTextDescriptorPackOne.
                acTextDescriptor),
               (&pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
               DISCRETE_SYGNAL_NAME_LENGTH);

        memcpy((uint8_t*)&pxDiscreteSygnalTextDescriptorPackOne[nucBlocksInBlockCounter],
               (uint8_t*)&xDiscreteSygnalTextDescriptorPackOne,
               sizeof(struct TDiscreteSygnalTextDescriptorPackOne));

        // ��������� ���������.
        nucBlocksInBlockCounter++;
    }
}


//-----------------------------------------------------------------------------------------------------
// ��������������� �� ������ ������� ���� ������, � ������ �������� � RAM.
// ���� ������ � ������� - ��� ������, ������ 100 ������. ������ ���� 256 ����.
// ����� ��� ������ �������, ���������� ��������, �������������� ������ � ��., ����� ��������� �����.
// ���� ������ �������, ���������� ��������, �������������� ������ � ��., ����� �������� ��������� ������.
// ����� ����� ����������� "�������" ��������� � ���������� ��������, �������� ��������������.
void vDiscreteInputDataBaseExstract(void)
{
    unsigned char ucCheck;
    unsigned char nucBlocksInBlockCounter; // ���� ��������� - ���� ����, � ����� �����.
    unsigned char nucBlockCounter;
    unsigned char nucRelayOutCounter;
    unsigned char nucIndexNumber;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char ucFlowControl;
    TDiscreteInputDescriptionWork *pxDiscreteInputDescriptionWork;
    TDiscreteInputDescriptionWork xDiscreteInputDescriptionWork;
    TDiscreteInputDescriptionWorkPackOne *pxDiscreteInputDescriptionWorkPackOne;
    TDiscreteInputDescriptionWorkPackOne xDiscreteInputDescriptionWorkSourse;

    // ������� ��������� �� ����� � ��������������� ����� ������ �������.
    pxDiscreteInputDescriptionWork = axDiscreteInputDescriptionWork;
    // ������� ��������� �� ���� � ����� ���� ������ �������.
    pxDiscreteInputDescriptionWorkPackOne = (TDiscreteInputDescriptionWorkPackOne*)&(xPlcDataBase.
                                            axPlcDataBaseBlocks[DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET].
                                            aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;
    // ����������� �� ������ ������� ���� ������, � ������ �������� � RAM.
    for (int i = 0;
            i < MAX_HANDLED_DISCRETE_INPUT;
            i++)
    {
        // ��������� ���� ���� ���� ������?
        if (nucBlocksInBlockCounter == DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // ��������� ���� ���� ������.
            nucBlockCounter++;
            // ������� ��������� �� ��������� ���� � ����� ���� ������ �������.
            pxDiscreteInputDescriptionWorkPackOne = (TDiscreteInputDescriptionWorkPackOne*)&(xPlcDataBase.
                                                    axPlcDataBaseBlocks[DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                                                    aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }
        memcpy((uint8_t*)&xDiscreteInputDescriptionWorkSourse,
               (uint8_t*)&pxDiscreteInputDescriptionWorkPackOne[nucBlocksInBlockCounter],
               sizeof(struct TDiscreteInputDescriptionWorkPackOne));

        pxDiscreteInputDescriptionWork[i].ucTalTkGrp =
            xDiscreteInputDescriptionWorkSourse.ucTalTkGrp;

        memcpy(pxDiscreteInputDescriptionWork[i].aucRelayOut,
               xDiscreteInputDescriptionWorkSourse.aucRelayOut,
               8);

        pxDiscreteInputDescriptionWork[i].ucDelay =
            xDiscreteInputDescriptionWorkSourse.ucDelay;

        // �������� ����������� ����� ���������.
        pucSource = (unsigned char*)&pxDiscreteInputDescriptionWork[i];
        ucCheck = 0xFF;
        for (int j = 0;
                j < (sizeof(struct TDiscreteInputDescriptionWork) - ONE_BYTE_CRC_LENGTH);
                j++)
        {
            ucCheck += pucSource[j];
        }
        pxDiscreteInputDescriptionWork[i].ucCrc = ucCheck;
        // ��������� ���������.

        nucBlocksInBlockCounter++;
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������� ���������� ����������� ����� ������ ��������� ����������� �������.
// �������� ��������� �� ���� ��������� ����������� ������� � ��������������� ���� ������,
// ��������������� �������� vDiscreteInputDataBaseExstract().
int iDiscreteInputDataBaseCrcCheck(
    TDiscreteInputDescriptionWork *pxDiscreteInputDataBase)
{
    unsigned char ucCheck;
    unsigned char *pucSource;
    // ������� ��������� �� ���� ��������� ����������� ������� � ��������������� ���� ������.
    pucSource = (unsigned char*)pxDiscreteInputDataBase;
    // ��������� �������� CRC. ����� ����� �� ���� �������.
    ucCheck = 0xFF;
    // ��������� ����� ��������� ���������� struct TDiscreteInputDescriptionWork
    // ����� ����� CRC.
    for (int i = 0;
            i < ((sizeof(struct TDiscreteInputDescriptionWork)) - ONE_BYTE_CRC_LENGTH);
            i++)
    {
        ucCheck += pucSource[i];
    }
    // CRC ���������?
    if (pxDiscreteInputDataBase -> ucCrc == ucCheck)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
