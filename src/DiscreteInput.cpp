
#include "PasNewConfig.h"
#include "DiscreteInput.h"
#include "Alarm.h"
#include "Events.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// обрабатывает сигнализацию запрограммированных активных дискретных сигналов.
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

    // получим указатель на буфер с нормализованной базой данных дискретных сигналов прибора.
    pxDiscreteInputDataBase = axDiscreteInputDescriptionWork;
    pxDiscreteInputWorkingData = axDiscreteInputWorkingData;
    pucDiscreteInputSource = &aucDiscreteInputsArray[DI_VALUE_BIT_ARRAY_OFFSET];
    pucBadDiscreteInputSource = &aucCoilsArray[BAD_DI_BIT_ARRAY_OFFSET];
    pucDoValueDestination = &aucDoValueByteArray[0];
    pucDoValueBlockDestination = &aucDoValueBlockByteArray[0];//&aucCoilsArray[DO_VALUE_BIT_ARRAY_OFFSET];
    // получим указатель на рабочий массив формирования требований отображения сигнализации дискретных сигналов на HMI.
    pxAlarmHmi = &axAlarmHmi[0];
    pucGroupDinAction = &aucCoilsArray[ALARM_ACTIVITY_DISCRETE_SIGNAL_GROUP_BIT_ARRAY_OFFSET];
    pucDinAction = &aucCoilsArray[ALARM_ACTIVITY_DISCRETE_SIGNAL_BIT_ARRAY_OFFSET];


    // обработаем все запрограммированные дискретные сигналы.
    for (int i = 0;
            i < (xPlcConfigService.xPlcConfigServiceData.
                 uiHandledDiscreteInputQuantity);
            i++)
    {
        // данные описателя дискретного сигнала повреждены?
        if (iDiscreteInputDataBaseCrcCheck(pxDiscreteInputDataBase) == -1)
        {
            fbDiscreteInputDataBaseError = 1;
//            cout << "iDiscreteInputDataBaseCrcCheck error" << endl;

            // активное состояние события ещё не зарегистрировано?
            if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                        SYSTEM_ERROR_GROUP_OFFSET,
                        SYSTEM_ERROR_DISCRETE_SYGNAL_DATA_BASE_ERROR_OFFSET))
            {
                // зарегистрируем активное состояние события.
                CEvents::EventRegistration(
                    SYSTEM_ERROR_GROUP_OFFSET,
                    (CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE |
                     CEvents::HANDLED_EVENTS_IS_POPUP |
                     CEvents::HANDLED_EVENTS_IS_SOUND |
                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                    SYSTEM_ERROR_DISCRETE_SYGNAL_DATA_BASE_ERROR_OFFSET,
                    "вх.диск.сигналов");
            }
        }
        else
        {
            // дискретный сигнал достоверен?
            if (*pucBadDiscreteInputSource)
            {
                // недостоверный дискретный сигнал считается нормой.
                ucFlowControl = FLOW_CONTROL_NORMA;
            }
            // дискретный сигнал активен?
            else if (((CONTACT_TYPE_DIS((pxDiscreteInputDataBase))) &&
                      (*pucDiscreteInputSource)) ||
                     (((CONTACT_TYPE_DIS((pxDiscreteInputDataBase))) == 0) &&
                      (*pucDiscreteInputSource) == 0))
            {
                // дискретный сигнал был активен на предыдущем шаге?
                if (pxDiscreteInputWorkingData -> fbDiscreteInputIsActive)
                {
                    // не новое нарушение.
                    ucFlowControl = FLOW_CONTROL_NOT_NEW_VIOLATION;
                }
                else
                {
                    // новое нарушение.
                    ucFlowControl = FLOW_CONTROL_NEW_VIOLATION;
                }
            }
            else
            {
                // норма.
                ucFlowControl = FLOW_CONTROL_NORMA;
            }

            // в каком состоянии дискретный сигнал?
            switch(ucFlowControl)
            {
            case FLOW_CONTROL_NORMA:
//                cout << "FLOW_CONTROL_NORMA" << endl;
                // норма.
                // дискретный сигнал был активен на предыдущем шаге?
                if (pxDiscreteInputWorkingData -> fbDiscreteInputIsActive)
                {
                    // сбросим флаг активности дискретного сигнала.
                    pxDiscreteInputWorkingData -> fbDiscreteInputIsActive = 0;
                    // обнулим счётчик времени задержки УВЗ.
                    pxDiscreteInputWorkingData -> ucDelay = 0;
//                    // есть архивация?
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
                        // неактивное состояние события ещё не зарегистрировано?
                        if(xCAlarmEvent.EventOffIsNotRegistered(
                                    i,
                                    0))
                        {
                            // зарегистрируем неактивное состояние события.
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
                        // неактивное состояние события ещё не зарегистрировано?
                        if(xCAlarmEvent.EventOffIsNotRegistered(
                                    i,
                                    0))
                        {
                            // зарегистрируем неактивное состояние события.
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
                // новое нарушение.
                // установим флаги - новое нарушение, для запрограммированных реле
                // в буфере выходов управления реле - новое нарушение.
                // цикл выбора модуля.
                for (int j = 0;
                        j < (DISCRETE_INPUT_RELAY_OUTPUT_QUANTITY /
                             MRXX_DISCRETE_OUTPUT_QUANTITY);
                        j++)
                {
                    // цикл выбора реле.
                    for (int k = 0;
                            k < (MRXX_DISCRETE_OUTPUT_QUANTITY);
                            k++)
                    {
                        // установим флаг - новое нарушение.
                        pucDoValueDestination[(j * MRXX_DISCRETE_OUTPUT_QUANTITY) + k] |=
                            (((pxDiscreteInputDataBase -> aucRelayOut[j]) >> k) & 0x01);
                    }
                }
                // установим флаг активности дискретного сигнала.
                pxDiscreteInputWorkingData -> fbDiscreteInputIsActive = 1;
                // загрузим в счётчик уставку времени задержки УВЗ.
                pxDiscreteInputWorkingData -> ucDelay =
                    DELAY_TIME_SETPOINT_DIS((pxDiscreteInputDataBase));

                // задержка не запрограммирована?
                if (!(DELAY_TIME_SETPOINT_DIS((pxDiscreteInputDataBase))))
                {
                    // установим флаги - требования включения, для запрограммированных реле,
                    // в буфере выходов требований включения реле - блокировка.
                    // цикл выбора модуля.
                    for (int j = 0;
                            j < (DISCRETE_INPUT_RELAY_OUTPUT_QUANTITY /
                                 MRXX_DISCRETE_OUTPUT_QUANTITY);
                            j++)
                    {
                        // цикл выбора реле.
                        for (int k = 0;
                                k < (MRXX_DISCRETE_OUTPUT_QUANTITY);
                                k++)
                        {
                            // установим флаг - требование включения реле.
                            pucDoValueBlockDestination[(j * MRXX_DISCRETE_OUTPUT_QUANTITY) + k] |=
                                (((pxDiscreteInputDataBase -> aucRelayOut[j]) >> k) & 0x01);
                        }
                    }
                }
//                // есть архивация?
//                if (ARCHIVE_DIS((pxDiscreteInputDataBase)))
//                {
//                    // arch
//                    pxAlarmHmi -> ui8Archive = 1;
//                }
//                else
//                {
//                    pxAlarmHmi -> ui8Archive = 0;
//                }

//                // установим тип общей сигнализации.
//                // нет аварийной сигнализации?
//                if (!((ucAlarmStateCommon >> ALARM_TYPE_ALARM) & 0x01))
//                {
//                    // для текущего дискретного сигнала запрограммирована сигнализация или индикация?
//                    if ((SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase))) ||
//                            (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase))))
//                    {
//                        // для текущего дискретного сигнала запрограммирована сигнализация?
//                        if (((!SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase))) &&
//                                (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))) ||
//                                ((SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase))) &&
//                                 (!SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))))
//                        {
//                            // очистим флаги предыдущего состояния сигнализации.
//                            ucAlarmStateCommon = 0;
//                            // для текущего дискретного сигнала запрограммирована предупредительная сигнализация?
//                            if (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))
//                            {
//                                // установим флаг предупредительной сигнализации.
//                                ucAlarmStateCommon |= (0x01 << ALARM_TYPE_PREVENT);
//                            }
//                            else
//                            {
//                                // сбросим флаг предупредительной сигнализации.
//                                ucAlarmStateCommon &= ~(0x01 << ALARM_TYPE_PREVENT);
//                            }
//                            // для текущего дискретного сигнала запрограммирована аварийная сигнализация?
//                            if (SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase)))
//                            {
//                                // установим флаг аварийной сигнализации.
//                                ucAlarmStateCommon |= (0x01 << ALARM_TYPE_ALARM);
//                            }
//                            else
//                            {
//                                // сбросим флаг аварийной сигнализации.
//                                ucAlarmStateCommon &= ~(0x01 << ALARM_TYPE_ALARM);
//                            }
//                        }
//                    }
//                }
//
//                // установим тип сигнализации для текущего дискретного сигнала.
//                // очистим флаги предыдущего состояния сигнализации.
//                pxAlarmHmi -> ui8AlarmState = 0;
//                // для текущего дискретного сигнала запрограммирована предупредительная сигнализация?
//                if (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))
//                {
//                    // установим флаг предупредительной сигнализации.
//                    pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
//                }
//                else
//                {
//                    // сбросим флаг предупредительной сигнализации.
//                    pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_PREVENT);
//                }
//                // для текущего дискретного сигнала запрограммирована аварийная сигнализация?
//                if (SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase)))
//                {
//                    // установим флаг аварийной сигнализации.
//                    pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
//                }
//                else
//                {
//                    // сбросим флаг аварийной сигнализации.
//                    pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_ALARM);
//                }

                // для текущего дискретного сигнала запрограммирована сигнализация или индикация?
//                if ((SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase))) ||
//                        (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase))))
//                {
//                    // для текущего дискретного сигнала запрограммирована сигнализация?
//                    if (((!SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase))) &&
//                            (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))) ||
//                            ((SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase))) &&
//                             (!SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))))
//                    {
//                        // для текущего дискретного сигнала запрограммирована предупредительная сигнализация?
//                        if (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))
//                        {
//                            // установим флаг предупредительной сигнализации.
//                            pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
//                        }
//                        else
//                        {
//                            // сбросим флаг предупредительной сигнализации.
//                            pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_PREVENT);
//                        }
//                        // для текущего дискретного сигнала запрограммирована аварийная сигнализация?
//                        if (SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase)))
//                        {
//                            // установим флаг аварийной сигнализации.
//                            pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
//                        }
//                        else
//                        {
//                            // сбросим флаг аварийной сигнализации.
//                            pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_ALARM);
//                        }
//                    }
//                }
                break;

            case FLOW_CONTROL_NOT_NEW_VIOLATION:
//                cout << "FLOW_CONTROL_NOT_NEW_VIOLATION" << endl;
                // не новое нарушение.
                // время задержки истекло?
                if (!(pxDiscreteInputWorkingData -> ucDelay))
                {
                    // установим флаги - требования включения, для запрограммированных реле,
                    // в буфере выходов требований включения реле - блокировка.
                    // цикл выбора модуля.
                    for (int j = 0;
                            j < (DISCRETE_INPUT_RELAY_OUTPUT_QUANTITY /
                                 MRXX_DISCRETE_OUTPUT_QUANTITY);
                            j++)
                    {
                        // цикл выбора реле.
                        for (int k = 0;
                                k < (MRXX_DISCRETE_OUTPUT_QUANTITY);
                                k++)
                        {
                            // установим флаг - требование включения реле.
                            pucDoValueBlockDestination[(j * MRXX_DISCRETE_OUTPUT_QUANTITY) + k] |=
                                (((pxDiscreteInputDataBase -> aucRelayOut[j]) >> k) & 0x01);
                        }
                    }
                }
                else
                {
                    // время задержки не истекло.
                    // дискретность времени задержки ДВЗ - 1 секунда?
                    if (!(DELAY_TIME_DISCRETENESS_DIS((pxDiscreteInputDataBase))))
                    {
                        // дискретность времени задержки ДВЗ - 1 секунда.
                        // прошла одна секунда?
                        if (fbOneSecondPassed)
                        {
                            pxDiscreteInputWorkingData -> ucDelay -= 1;
                        }
                    }
                    else
                    {
                        // дискретность времени задержки ДВЗ - 10 секунд.
                        // прошло десять секунд?
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

//            // сигнализация не запрограммирована? или, запрограммирована индикация?
//            if (((SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase))) &&
//                    (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))) ||
//                    ((!(SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase)))) &&
//                     (!(SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase))))))
            // сигнализация не запрограммирована?
            if ((!(SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase)))) &&
                    (!(SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))))
            {
                // сбросим флаги требования отображения сигнализации на HMI - всё в норму.
                *pxAlarmHmi = xAlarmHmiNull;
            }
            else
            {
                // запрограммирована сигнализация.
                // дискретный сигнал активен?
                if (pxDiscreteInputWorkingData -> fbDiscreteInputIsActive)
                {
                    // для текущего дискретного сигнала установлено требование отображения сигнализации на HMI?
                    if (pxAlarmHmi -> ui8AlarmState)
                    {

                    }
                    else
                    {
                        // укажем группу для отображения сигнализации на HMI.
                        pxAlarmHmi -> ui8AlarmState =
                            LIGHT_BOARD_CELL_NUMBER_DIS((pxDiscreteInputDataBase));
                        // установим тип сигнализации для текущего дискретного сигнала.
                        // для текущего дискретного сигнала запрограммирована предупредительная сигнализация?
                        if (SIGNALING_TYPE_1_DIS((pxDiscreteInputDataBase)))
                        {
                            // установим флаг предупредительной сигнализации.
                            pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
                        }
                        else
                        {
                            // сбросим флаг предупредительной сигнализации.
                            pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_PREVENT);
                        }
                        // для текущего дискретного сигнала запрограммирована аварийная сигнализация?
                        if (SIGNALING_TYPE_2_DIS((pxDiscreteInputDataBase)))
                        {
                            // установим флаг аварийной сигнализации.
                            pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
                        }
                        else
                        {
                            // сбросим флаг аварийной сигнализации.
                            pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_ALARM);
                        }

                        if (ARCHIVE_DIS((pxDiscreteInputDataBase)))
                        {
                            // активное состояние события ещё не зарегистрировано?
                            if(xCAlarmEvent.EventOnIsNotRegistered(
                                        i,
                                        0))
                            {
                                // зарегистрируем активное состояние события.
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
                            // активное состояние события ещё не зарегистрировано?
                            if(xCAlarmEvent.EventOnIsNotRegistered(
                                        i,
                                        0))
                            {
                                // зарегистрируем активное состояние события.
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
                    // для текущего дискретного сигнала установлено требование отображения сигнализации на HMI?
                    if (pxAlarmHmi -> ui8AlarmState)
                    {
//            cout << "FLOW_CONTROL_NOT_NEW_VIOLATION 13" << endl;
                        // требование отображения сигнализации на HMI сброшено?
                        if (fbGlobalReset)
                        {
                            // сбросим флаги требования отображения сигнализации на HMI - всё в норму.
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
// создаёт стартовую базу данных обработки дискретных сигналов.
int iDiscreteInputStartDataBaseCreate(void)
{
    int i;
    unsigned char ucCheck;
    unsigned char nucBlockCounter;
    unsigned char nucBlocksInBlockCounter; // один описатель - один блок, в общем блоке.
    unsigned char nucInputCounter;
    unsigned char nucModuleCounter;
    unsigned char *pucSource;
    unsigned char ucFlowControl;
    TDiscreteInputDescriptionWork *pxDiscreteInputDescriptionWork;
    TDiscreteInputDescriptionWorkPackOne *pxDiscreteInputDescriptionWorkPackOne;
    TDiscreteInputDescriptionWorkPackOne xDiscreteInputDescriptionWorkSourse;
    // получим указатель на буфер для нормализованной стартовой базы данных прибора.
    pxDiscreteInputDescriptionWork = axDiscreteInputDescriptionWork;
    // начнём с первого входа.
    nucInputCounter = 0;
    // начнём с первого модуля.
    nucModuleCounter = 1;
    i = 0;

    memset((unsigned char*)axDiscreteInputDescriptionWork,
           0,
           (sizeof(struct TDiscreteInputDescriptionWork)));

    // создадим первую часть стартовой базы данных дискретных сигналов.
    // для сигналов пораждаемых модулями дискретного ввода.
    for (int i = 0;
            i < (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity);
            i++)
    {
        for (int j = 0;
                j < DISCRETE_MODULE_INPUT_QUANTITY;
                j++)
        {
            // заполним номер группы. содержится в счётчике - nucModuleCounter. для каждого модуля свой номер группы.
            pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
            // тип контакта - нормально разомкнутый.
            pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
            // аварийная сигнализация.
            pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
            pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
            // первый выход управления реле, превого модуля реле МР1.
            pxDiscreteInputDescriptionWork[nucInputCounter].aucRelayOut[0] = 1;
            // время задержки - 5 секунд.
            pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay |= (DELAY_TIME_SETPOINT_DISC_MASK & 5);
            // дискретность времени задержки - 1 секунда.
            pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay &= ~DELAY_TIME_DISCRETENESS_DISC_MASK;
            // Не архивировать.
            pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay &= ~ARCHIVE_DISC_MASK;

            // вычислим CRC описателя.
            pucSource = (unsigned char*)&pxDiscreteInputDescriptionWork[nucInputCounter];
            ucCheck = 0xFF;
            for (int k = 0;
                    k < (sizeof(struct TDiscreteInputDescriptionWork) - ONE_BYTE_CRC_LENGTH);
                    k++)
            {
                ucCheck += pucSource[k];
            }
            pxDiscreteInputDescriptionWork[nucInputCounter].ucCrc = ucCheck;

            // следующий вход модуля дискретного ввода.
            // следующий описатель.
            nucInputCounter++;

        }

        // следующий модуль.
        nucModuleCounter++;
    }



//    // создадим первую часть стартовой базы данных дискретных сигналов.
//    // для сигналов пораждаемых модулями дискретного ввода.
//    while (i < (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity *
//                DISCRETE_MODULE_INPUT_QUANTITY))
//    {
//        // заполним номер группы. содержится в счётчике - nucModuleCounter. для каждого модуля свой номер группы.
//        pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
//        // тип контакта - нормально разомкнутый.
//        pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
//        // аварийная сигнализация.
//        pxDiscreteInputDescriptionWork[i].ucTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
//        pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
//        // первый выход управления реле, превого модуля реле МР1.
//        pxDiscreteInputDescriptionWork[i].aucRelayOut[0] = 1;
//        // время задержки - 5 секунд.
//        pxDiscreteInputDescriptionWork[i].ucDelay |= (DELAY_TIME_SETPOINT_DISC_MASK & 5);
//        // дискретность времени задержки - 1 секунда.
//        pxDiscreteInputDescriptionWork[i].ucDelay &= ~DELAY_TIME_DISCRETENESS_DISC_MASK;
//        // Не архивировать.
//        pxDiscreteInputDescriptionWork[i].ucDelay &= ~ARCHIVE_DISC_MASK;
//
//        // вычислим CRC описателя.
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
//        // следующий вход модуля дискретного ввода.
//        nucInputCounter++;
//        // следующий описатель.
//        i++;
//        // обработаны все входы модуля дискретного ввода?
//        if (nucInputCounter == DISCRETE_MODULE_INPUT_QUANTITY)
//        {
//            // начнём с первого входа.
//            nucInputCounter = 0;
//            // следующий модуль.
//            nucModuleCounter++;
//        }
//    }

//    i = nucInputCounter;
//    nucInputCounter = 0;



    // создадим вторую часть стартовой базы данных дискретных сигналов.
    // для сигналов пораждаемых модулями аналогового ввода.
    // один аналоговый вход пораждает четыре дискретных сигнала - уставки: LL, L, H, HH.
    // начнём с создания описателя для уставки LL+L.
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
                // заполним номер группы. содержится в счётчике - nucModuleCounter. для каждого модуля свой номер группы.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // тип контакта - нормально разомкнутый.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // аварийная сигнализация.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
                // нет выхода управления реле, превого модуля реле МР1.
                pxDiscreteInputDescriptionWork[nucInputCounter].aucRelayOut[0] = 0;
                // нет задержки.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay = 0;
                // дискретность времени задержки - 1 секунда.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay = 0;
                // Не архивировать.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay &= ~ARCHIVE_DISC_MASK;
                // перейдём к заполнению описателя следующей уставки.
                ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                break;

            case ANALOGUE_INPUT_SET_POINT_VIOLATION_L:
                // заполним номер группы. содержится в счётчике - nucModuleCounter. для каждого модуля свой номер группы.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // тип контакта - нормально разомкнутый.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // предупредительная сигнализация.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp &= ~SIGNALING_TYPE_2_DISC_MASK;
                // нет выхода управления реле, превого модуля реле МР1.
                pxDiscreteInputDescriptionWork[nucInputCounter].aucRelayOut[0] = 0;
                // нет задержки.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay = 0;
                // дискретность времени задержки - 1 секунда.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay = 0;
                // Не архивировать.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay &= ~ARCHIVE_DISC_MASK;
                // перейдём к заполнению описателя следующей уставки.
                ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                break;

            case ANALOGUE_INPUT_SET_POINT_VIOLATION_H:
                // заполним номер группы. содержится в счётчике - nucModuleCounter. для каждого модуля свой номер группы.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // тип контакта - нормально разомкнутый.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // предупредительная сигнализация.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp &= ~SIGNALING_TYPE_2_DISC_MASK;
                // нет выхода управления реле, превого модуля реле МР1.
                pxDiscreteInputDescriptionWork[nucInputCounter].aucRelayOut[0] = 0;
                // нет задержки.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay = 0;
                // дискретность времени задержки - 1 секунда.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay = 0;
                // Не архивировать.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay &= ~ARCHIVE_DISC_MASK;
                // перейдём к заполнению описателя следующей уставки.
                ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                break;

            case ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H:
                // заполним номер группы. содержится в счётчике - nucModuleCounter. для каждого модуля свой номер группы.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
                // тип контакта - нормально разомкнутый.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
                // аварийная сигнализация.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
                pxDiscreteInputDescriptionWork[nucInputCounter].ucTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
                // нет выхода управления реле, превого модуля реле МР1.
                pxDiscreteInputDescriptionWork[nucInputCounter].aucRelayOut[0] = 0;
                // нет задержки.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay = 0;
                // дискретность времени задержки - 1 секунда.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay = 0;
                // Не архивировать.
                pxDiscreteInputDescriptionWork[nucInputCounter].ucDelay &= ~ARCHIVE_DISC_MASK;
                // перейдём к заполнению описателя следующей уставки.
                ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                break;

            default:
                break;
            };

            // вычислим CRC описателя.
            pucSource = (unsigned char*)&pxDiscreteInputDescriptionWork[nucInputCounter];
            ucCheck = 0xFF;
            for (int k = 0;
                    k < (sizeof(struct TDiscreteInputDescriptionWork) - ONE_BYTE_CRC_LENGTH);
                    k++)
            {
                ucCheck += pucSource[k];
            }
            pxDiscreteInputDescriptionWork[nucInputCounter].ucCrc = ucCheck;

            // следующий вход модуля дискретного ввода.
            // следующий описатель.
            nucInputCounter++;

        }

        // следующий модуль.
        nucModuleCounter++;
    }


//    // создадим вторую часть стартовой базы данных дискретных сигналов.
//    // для сигналов пораждаемых модулями аналогового ввода.
//    // один аналоговый вход пораждает четыре дискретных сигнала - уставки: LL, L, H, HH.
//    // начнём с создания описателя для уставки LL+L.
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
//            // заполним номер группы. содержится в счётчике - nucModuleCounter. для каждого модуля свой номер группы.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
//            // тип контакта - нормально разомкнутый.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
//            // аварийная сигнализация.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
//            // нет выхода управления реле, превого модуля реле МР1.
//            pxDiscreteInputDescriptionWork[i].aucRelayOut[0] = 0;
//            // нет задержки.
//            pxDiscreteInputDescriptionWork[i].ucDelay = 0;
//            // дискретность времени задержки - 1 секунда.
//            pxDiscreteInputDescriptionWork[i].ucDelay = 0;
//            // Не архивировать.
//            pxDiscreteInputDescriptionWork[i].ucDelay &= ~ARCHIVE_DISC_MASK;
//            // перейдём к заполнению описателя следующей уставки.
//            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
//            break;
//
//        case ANALOGUE_INPUT_SET_POINT_VIOLATION_L:
//            // заполним номер группы. содержится в счётчике - nucModuleCounter. для каждого модуля свой номер группы.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
//            // тип контакта - нормально разомкнутый.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
//            // предупредительная сигнализация.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= SIGNALING_TYPE_1_DISC_MASK;
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp &= ~SIGNALING_TYPE_2_DISC_MASK;
//            // нет выхода управления реле, превого модуля реле МР1.
//            pxDiscreteInputDescriptionWork[i].aucRelayOut[0] = 0;
//            // нет задержки.
//            pxDiscreteInputDescriptionWork[i].ucDelay = 0;
//            // дискретность времени задержки - 1 секунда.
//            pxDiscreteInputDescriptionWork[i].ucDelay = 0;
//            // Не архивировать.
//            pxDiscreteInputDescriptionWork[i].ucDelay &= ~ARCHIVE_DISC_MASK;
//            // перейдём к заполнению описателя следующей уставки.
//            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
//            break;
//
//        case ANALOGUE_INPUT_SET_POINT_VIOLATION_H:
//            // заполним номер группы. содержится в счётчике - nucModuleCounter. для каждого модуля свой номер группы.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
//            // тип контакта - нормально разомкнутый.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
//            // предупредительная сигнализация.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= SIGNALING_TYPE_1_DISC_MASK;
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp &= ~SIGNALING_TYPE_2_DISC_MASK;
//            // нет выхода управления реле, превого модуля реле МР1.
//            pxDiscreteInputDescriptionWork[i].aucRelayOut[0] = 0;
//            // нет задержки.
//            pxDiscreteInputDescriptionWork[i].ucDelay = 0;
//            // дискретность времени задержки - 1 секунда.
//            pxDiscreteInputDescriptionWork[i].ucDelay = 0;
//            // Не архивировать.
//            pxDiscreteInputDescriptionWork[i].ucDelay &= ~ARCHIVE_DISC_MASK;
//            // перейдём к заполнению описателя следующей уставки.
//            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
//            break;
//
//        case ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H:
//            // заполним номер группы. содержится в счётчике - nucModuleCounter. для каждого модуля свой номер группы.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= (nucModuleCounter & LIGHT_BOARD_CELL_NUMBER_DISC_MASK);
//            // тип контакта - нормально разомкнутый.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= CONTACT_TYPE_DISC_MASK;
//            // аварийная сигнализация.
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp &= ~SIGNALING_TYPE_1_DISC_MASK;
//            pxDiscreteInputDescriptionWork[i].ucTalTkGrp |= SIGNALING_TYPE_2_DISC_MASK;
//            // нет выхода управления реле, превого модуля реле МР1.
//            pxDiscreteInputDescriptionWork[i].aucRelayOut[0] = 0;
//            // нет задержки.
//            pxDiscreteInputDescriptionWork[i].ucDelay = 0;
//            // дискретность времени задержки - 1 секунда.
//            pxDiscreteInputDescriptionWork[i].ucDelay = 0;
//            // Не архивировать.
//            pxDiscreteInputDescriptionWork[i].ucDelay &= ~ARCHIVE_DISC_MASK;
//            // перейдём к заполнению описателя следующей уставки.
//            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
//            break;
//
//        default:
//            break;
//        };
//
//        // вычислим CRC описателя.
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
//        // следующий вход.
//        nucInputCounter++;
//        // следующий описатель.
//        i++;
//
//        // обработаны все дискретные сигналы пораждённые входами модуля аналогового ввода?
//        if (nucInputCounter == (ANALOG_MODULE_INPUT_QUANTITY *
//                                ANALOGUE_INPUT_DI_VALUE_QUANTITY))
//        {
//            // начнём с первого входа.
//            nucInputCounter = 0;
//            // следующий модуль.
//            nucModuleCounter++;
//        }
//    }

    // получим указатель на буфер с вновь созданной нормализованной стартовой базой данных прибора.
    pxDiscreteInputDescriptionWork = axDiscreteInputDescriptionWork;
    // получим указатель на базу данных прибора в общем формате.
    pxDiscreteInputDescriptionWorkPackOne = (TDiscreteInputDescriptionWorkPackOne*)&(xPlcDataBase.
                                            axPlcDataBaseBlocks[DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET].
                                            aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;

    // преобразуем созданную базу данных в общий формат.
    for (int i = 0;
            i < MAX_HANDLED_DISCRETE_INPUT;
            i++)
    {
        // обработан весь блок базы данных?
        if (nucBlocksInBlockCounter == DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // следующий блок базы данных.
            nucBlockCounter++;
            // получим указатель на следующий блок в общей базе данных прибора.
            pxDiscreteInputDescriptionWorkPackOne = (TDiscreteInputDescriptionWorkPackOne*)&(xPlcDataBase.
                                                    axPlcDataBaseBlocks[DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                                                    aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }

        // скопируем один описатель дискретного сигнала, в буфер общего формата.
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

        // следующий описатель.
        nucBlocksInBlockCounter++;
    }
}

//-----------------------------------------------------------------------------------------------------
// создаёт стартовую базу данных текстовых реквизитов дискретных сигналов.
int iDiscreteSygnalTextDescriptorStartDataBaseCreate(void)
{
    int i;
    unsigned char nucBlockCounter;
    unsigned char nucBlocksInBlockCounter; // один описатель - один блок, в общем блоке.
    unsigned char nucDiscreteInputCounter;
    unsigned char nucModuleInputCounter;
    // количество уставок по одному входу(4 - LL, L, H, HH).
    unsigned char nucInputSetPointCounter;
    unsigned char nucModuleCounter;
    unsigned char ucFlowControl;
    TDiscreteSygnalTextDescriptorPackOne *pxDiscreteSygnalTextDescriptorPackOne;
    TDiscreteSygnalTextDescriptorPackOne xDiscreteSygnalTextDescriptorPackOne;
    TDiscreteSygnalTextDescriptor *pxDiscreteSygnalTextDescriptor;


    // получим указатель на буфер для нормализованной стартовой базы данных прибора.
    pxDiscreteSygnalTextDescriptor = axDiscreteSygnalTextDescriptor;
    // начнём с первого входа.
    nucDiscreteInputCounter = 0;
    nucModuleInputCounter = 0;
    // начнём с первого модуля.
    nucModuleCounter = 1;
    i = 0;

    memset((unsigned char*)pxDiscreteSygnalTextDescriptor,
           0,
           (sizeof(struct TDiscreteSygnalTextDescriptor)));

    // создадим первую часть стартовой базы данных текстовых реквизитов дискретных сигналов.
    // для сигналов пораждаемых модулями дискретного ввода.
    while (i < (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity *
                DISCRETE_MODULE_INPUT_QUANTITY))
    {
        // создадим строку текстового реквизита дискретного сигнала.
        sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                "%s_%02d_%02d  ",
                "Вх. DI",
                nucModuleCounter,
                nucDiscreteInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
        // следующий вход модуля дискретного ввода.
        nucDiscreteInputCounter++;
        // следующий описатель.
        i++;
        // обработаны все входы модуля дискретного ввода?
        if (nucDiscreteInputCounter == DISCRETE_MODULE_INPUT_QUANTITY)
        {
            // начнём с первого входа.
            nucDiscreteInputCounter = 0;
            // следующий модуль.
            nucModuleCounter++;
        }
    }

    // начнём с первого входа.
    nucDiscreteInputCounter = 0;
    nucModuleInputCounter = 0;
    // начнём с первой уставки - LL+L.
    nucInputSetPointCounter = 0;
    // начнём с первого модуля.
    nucModuleCounter = 1;

    // создадим вторую часть стартовой базы данных текстовых реквизитов дискретных сигналов.
    // для сигналов пораждаемых модулями аналогового ввода.
    // один аналоговый вход пораждает четыре дискретных сигнала - уставки: LL, L, H, HH.
    // начнём с создания описателя для уставки LL+L.
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
            // создадим строку текстового реквизита дискретного сигнала.
            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                    "%s_%02d_%01d_LL",
                    "Вх. AI",
                    nucModuleCounter,
                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
            // перейдём к заполнению описателя следующей уставки.
            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
            break;

        case ANALOGUE_INPUT_SET_POINT_VIOLATION_L:
            // создадим строку текстового реквизита дискретного сигнала.
            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                    "%s_%02d_%01d_L ",
                    "Вх. AI",
                    nucModuleCounter,
                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);

            // перейдём к заполнению описателя следующей уставки.
            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
            break;

        case ANALOGUE_INPUT_SET_POINT_VIOLATION_H:
            // создадим строку текстового реквизита дискретного сигнала.
            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                    "%s_%02d_%01d_H ",
                    "Вх. AI",
                    nucModuleCounter,
                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);

            // перейдём к заполнению описателя следующей уставки.
            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
            break;

        case ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H:
            // создадим строку текстового реквизита дискретного сигнала.
            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                    "%s_%02d_%01d_HH",
                    "Вх. AI",
                    nucModuleCounter,
                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);

            // перейдём к заполнению описателя следующей уставки.
            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
            break;

        default:
            break;
        };

        nucDiscreteInputCounter++;
        nucInputSetPointCounter++;
        // количество уставок по одному входу(4 - LL, L, H, HH).
        // прошли по всем уставкам одного входа?
        if (nucInputSetPointCounter >= 4)
        {
            nucInputSetPointCounter = 0;
            // следующий вход.
            nucModuleInputCounter++;
        }
        // следующий описатель.
        i++;

        // обработаны все дискретные сигналы пораждённые входами модуля аналогового ввода?
        if (nucDiscreteInputCounter == (ANALOG_MODULE_INPUT_QUANTITY *
                                        ANALOGUE_INPUT_DI_VALUE_QUANTITY))
        {
            // начнём с первого входа.
            nucDiscreteInputCounter = 0;
            nucModuleInputCounter = 0;
            // начнём с первой уставки - LL+L.
            nucInputSetPointCounter = 0;
            // следующий модуль.
            nucModuleCounter++;
        }
    }

    // получим указатель на буфер с вновь созданной нормализованной стартовой базой данных прибора.
    pxDiscreteSygnalTextDescriptor = axDiscreteSygnalTextDescriptor;
    // получим указатель на базу данных прибора в общем формате.
    pxDiscreteSygnalTextDescriptorPackOne = (TDiscreteSygnalTextDescriptorPackOne*)&(xPlcDataBase.
                                            axPlcDataBaseBlocks[TEXT_TITLES_DATA_BASE_BLOCK_OFFSET].
                                            aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;

    // преобразуем созданную базу данных в общий формат.
    for (int i = 0;
            i < MAX_HANDLED_DISCRETE_INPUT;
            i++)
    {
        // обработан весь блок базы данных?
        if (nucBlocksInBlockCounter == TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // следующий блок базы данных.
            nucBlockCounter++;
            // получим указатель на следующий блок в общей базе данных прибора.
            pxDiscreteSygnalTextDescriptorPackOne = (TDiscreteSygnalTextDescriptorPackOne*)&(xPlcDataBase.
                                                    axPlcDataBaseBlocks[TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                                                    aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }

        // скопируем один описатель текстовых реквизитов дискретного сигнала, в буфер общего формата.
        memcpy((&xDiscreteSygnalTextDescriptorPackOne.
                acTextDescriptor),
               (&pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
               DISCRETE_SYGNAL_NAME_LENGTH);

        memcpy((uint8_t*)&pxDiscreteSygnalTextDescriptorPackOne[nucBlocksInBlockCounter],
               (uint8_t*)&xDiscreteSygnalTextDescriptorPackOne,
               sizeof(struct TDiscreteSygnalTextDescriptorPackOne));

        // следующий описатель.
        nucBlocksInBlockCounter++;
    }
}


//-----------------------------------------------------------------------------------------------------
// преобразовывает из общего формата базы данных, в формат хранения в RAM.
// база данных в приборе - это массив, длиной 100 блоков. каждый блок 256 байт.
// блоки баз данных модулей, дискретных сигналов, функциональных блоков и др., имеют различный рамер.
// базы данных модулей, дискретных сигналов, функциональных блоков и др., могут занимать несколько блоков.
// чтобы иметь возможность "плоской" адресации к описателям объектов, делается преобразование.
void vDiscreteInputDataBaseExstract(void)
{
    unsigned char ucCheck;
    unsigned char nucBlocksInBlockCounter; // один описатель - один блок, в общем блоке.
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

    // получим указатель на буфер с нормализованной базой данных прибора.
    pxDiscreteInputDescriptionWork = axDiscreteInputDescriptionWork;
    // получим указатель на блок в общей базе данных прибора.
    pxDiscreteInputDescriptionWorkPackOne = (TDiscreteInputDescriptionWorkPackOne*)&(xPlcDataBase.
                                            axPlcDataBaseBlocks[DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET].
                                            aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;
    // преобразуем из общего формата базы данных, в формат хранения в RAM.
    for (int i = 0;
            i < MAX_HANDLED_DISCRETE_INPUT;
            i++)
    {
        // обработан весь блок базы данных?
        if (nucBlocksInBlockCounter == DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // следующий блок базы данных.
            nucBlockCounter++;
            // получим указатель на следующий блок в общей базе данных прибора.
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

        // вычислим контрольную сумму описателя.
        pucSource = (unsigned char*)&pxDiscreteInputDescriptionWork[i];
        ucCheck = 0xFF;
        for (int j = 0;
                j < (sizeof(struct TDiscreteInputDescriptionWork) - ONE_BYTE_CRC_LENGTH);
                j++)
        {
            ucCheck += pucSource[j];
        }
        pxDiscreteInputDescriptionWork[i].ucCrc = ucCheck;
        // следующий описатель.

        nucBlocksInBlockCounter++;
    }
}

//-----------------------------------------------------------------------------------------------------
// проверяет совпадение контрольной суммы одного описателя дискретного сигнала.
// получает указатель на один описатель дискретного сигнала в нормализованной базе данных,
// преобразованной функцией vDiscreteInputDataBaseExstract().
int iDiscreteInputDataBaseCrcCheck(
    TDiscreteInputDescriptionWork *pxDiscreteInputDataBase)
{
    unsigned char ucCheck;
    unsigned char *pucSource;
    // получим указатель на один описатель дискретного сигнала в нормализованной базе данных.
    pucSource = (unsigned char*)pxDiscreteInputDataBase;
    // начальное значение CRC. чтобы сумма не была нулевой.
    ucCheck = 0xFF;
    // суммируем байты структуры описателей struct TDiscreteInputDescriptionWork
    // кроме байта CRC.
    for (int i = 0;
            i < ((sizeof(struct TDiscreteInputDescriptionWork)) - ONE_BYTE_CRC_LENGTH);
            i++)
    {
        ucCheck += pucSource[i];
    }
    // CRC совпадает?
    if (pxDiscreteInputDataBase -> ucCrc == ucCheck)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
