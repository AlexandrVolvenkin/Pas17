
#include "PasNewConfig.h"
#include "FunctionBlocks.h"
#include "Alarm.h"
#include "Events.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// преобразовывает из общего формата базы данных, в формат хранения в RAM.
// база данных в приборе - это массив, длиной 100 блоков. каждый блок 256 байт.
// блоки баз данных модулей, дискретных сигналов, функциональных блоков и др., имеют различный рамер.
// базы данных модулей, дискретных сигналов, функциональных блоков и др., могут занимать несколько блоков.
// чтобы иметь возможность "плоской" адресации к описателям объектов, делается преобразование.

void vFunctionBlockDataBaseExstract(void)
{
    unsigned char ucCheck;
    unsigned char nucBlocksInBlockCounter; // один описатель - один блок, в общем блоке.
    unsigned char nucBlockCounter;
    unsigned char nucRelayOutCounter;
    unsigned char nucIndexNumber;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char ucFlowControl;
    TFunctionBlockDescriptionWork *pxFunctionBlockDataBaseSource;
    TFunctionBlockDescriptionWorkPackOne *pxFunctionBlockDescriptionWork;
    TFunctionBlockDescriptionWorkPackOne xFunctionBlockDescriptionWork;

//    nucBlocksInBlockCounter = 0;
//    nucBlockCounter = 0;
//    nucIndexNumber = 0;
//    pxFunctionBlockDescriptionWork = (TFunctionBlockDescriptionWorkPackOne*)&(xPlcDataBase.
//                                    axPlcDataBaseBlocks[FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET].
//                                    aucPlcDataBaseBlockData[0]);
//    // посчитаем количество запрограммированных, обрабатываемых функциональных блоков.
//    for (int i = 0;
//            i < MAX_HANDLED_FUNCTION_BLOCK;
//            i++)
//    {
//        // обработан весь блок базы данных?
//        if (nucBlocksInBlockCounter == FUNCTION_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
//        {
//            // следующий блок базы данных.
//            nucBlockCounter++;
//            // получим указатель на блок в общей базе данных прибора.
//            pxFunctionBlockDescriptionWork = (TFunctionBlockDescriptionWorkPackOne*)&(xPlcDataBase.
//                                            axPlcDataBaseBlocks[FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
//                                            aucPlcDataBaseBlockData[0]);
//            nucIndexNumber = 0;
//        }
//
//        pucSource = (unsigned char*)&pxFunctionBlockDescriptionWork[nucIndexNumber];
//        ucCheck = 0;
//        // суммируем данные описателя, если, не равно нулю, то функциональный блок запрограммирован.
//        for (int j = 0;
//                j < (sizeof(struct TFunctionBlockDescriptionWorkPackOne) - ONE_BYTE_CRC_LENGTH);
//                j++)
//        {
//            ucCheck += pucSource[j];
//        }
//
//        if (ucCheck)
//        {
//            // ещё один функциональный блок запрограммирован.
//            nucBlocksInBlockCounter++;
//            nucIndexNumber++;
//        }
//        else
//        {
//            break;
//        }
//    }
//
//    xPlcConfigService.xPlcConfigServiceData.
//    uiHandledFunctionBlockQuantity =
//        nucBlocksInBlockCounter;


    // получим указатель на буфер с нормализованной базой данных прибора.
    pxFunctionBlockDataBaseSource = axFunctionBlockDescriptionWork;
    // получим указатель на блок в общей базе данных прибора.
    pxFunctionBlockDescriptionWork = (TFunctionBlockDescriptionWorkPackOne*)&(xPlcDataBase.
                                     axPlcDataBaseBlocks[FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET].
                                     aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;
    // преобразуем из общего формата базы данных, в формат хранения в RAM.
    for (int i = 0;
            i < MAX_HANDLED_FUNCTION_BLOCK;
            i++)
    {
        // обработан весь блок базы данных?
        if (nucBlocksInBlockCounter == FUNCTION_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // следующий блок базы данных.
            nucBlockCounter++;
            pxFunctionBlockDescriptionWork = (TFunctionBlockDescriptionWorkPackOne*)&(xPlcDataBase.
                                             axPlcDataBaseBlocks[FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                                             aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }
        // скопируем один описатель функционального блока.

        memcpy((uint8_t*)&xFunctionBlockDescriptionWork,
               (uint8_t*)&pxFunctionBlockDescriptionWork[nucBlocksInBlockCounter],
               sizeof(struct TFunctionBlockDescriptionWorkPackOne));

        pxFunctionBlockDataBaseSource[i].ucFunctionBlockType =
            xFunctionBlockDescriptionWork.ucFunctionBlockType;

        memcpy(pxFunctionBlockDataBaseSource[i].aucInputAddress,
               xFunctionBlockDescriptionWork.aucInputAddress,
               8);

        pxFunctionBlockDataBaseSource[i].ucTalArhGrp =
            xFunctionBlockDescriptionWork.ucTalArhGrp;

        memcpy(pxFunctionBlockDataBaseSource[i].aucRelayOut,
               xFunctionBlockDescriptionWork.aucRelayOut,
               8);

        // вычислим контрольную сумму описателя.
        pucSource = (unsigned char*)&pxFunctionBlockDataBaseSource[i];
        ucCheck = 0xFF;
        for (int j = 0;
                j < (sizeof(struct TFunctionBlockDescriptionWork) - ONE_BYTE_CRC_LENGTH);
                j++)
        {
            ucCheck += pucSource[j];
        }
        pxFunctionBlockDataBaseSource[i].ucCrc = ucCheck;
        // следующий описатель.
        nucBlocksInBlockCounter++;
    }
}

//-----------------------------------------------------------------------------------------------------
// проверяет совпадение контрольной суммы одного описателя функционального блока.
// получает указатель на один описатель функционального блока в нормализованной базе данных,
// преобразованной функцией vFunctionBlockDataBaseExstract().
int iFunctionBlockDataBaseCrcCheck(
    TFunctionBlockDescriptionWork *pxFunctionBlockDescriptionWork)
{
    unsigned char ucCheck;
    unsigned char *pucSource;
    // получим указатель на описатель в нормализованной базе данных.
    pucSource = (unsigned char*)pxFunctionBlockDescriptionWork;
    // начальное значение CRC. чтобы сумма не была нулевой.
    ucCheck = 0xFF;
    // суммируем байты структуры описателей struct TFunctionBlockDescriptionWork
    // кроме байта CRC.
    for (int i = 0;
            i < (sizeof(struct TFunctionBlockDescriptionWork) - ONE_BYTE_CRC_LENGTH);
            i++)
    {
        ucCheck += pucSource[i];
    }

    // CRC совпадает?
    if (pxFunctionBlockDescriptionWork -> ucCrc == ucCheck)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

//-----------------------------------------------------------------------------------------------------
// получает состояние дискретного сигнала по адресу - АДРЕС, из источника - ИВхС.
// если сигнал недостоверен, то возвращает ошибку(-1).
int iFunctionBlockInputCheck(unsigned char ucInputSource, unsigned short usSourceAddress)
{
    // из какого источника дискретный сигнал?
    switch(ucInputSource)
    {
        // измеренные значения входных дискретных сигналов.
    case FUNCTION_BLOCK_INPUT_SOURCE_DI_VALUE:
        // источник дискретного сигнала запрограммирован? и
        // дискретный сигнал достоверен?
        if ((usSourceAddress) && (!(aucCoilsArray[BAD_DI_BIT_ARRAY_OFFSET +
                                    (usSourceAddress - 1)])))
        {
            return aucDiscreteInputsArray[DI_VALUE_BIT_ARRAY_OFFSET +
                                          (usSourceAddress - 1)];
        }
        else
        {
            // дискретный сигнал недостоверен или не запрограммирован.
            return -1;
        }
        break;
        // выходы функциональных блоков или внешние команды управления логикой.
        // адреса: 1 - MAX_HANDLED_FUNCTION_BLOCK, - выход ФБ (OutFbl).
        // адреса: (MAX_HANDLED_FUNCTION_BLOCK + 1) - 184, - команда КВУ(LogCom 64 команды).
    case FUNCTION_BLOCK_INPUT_SOURCE_FUNCTION_BLOCK_OUTS:
        // источник дискретного сигнала запрограммирован?
        if (usSourceAddress)
        {
            // источник дискретного сигнала - выход функционального блока?
            if (usSourceAddress <= MAX_HANDLED_FUNCTION_BLOCK)
            {
                // источник дискретного сигнала - выход функционального блока.
                return aucCoilsArray[CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_OFFSET +
                                     (usSourceAddress - 1)];
            }
            // источник дискретного сигнала - внешние команды управления логикой?
            else if (usSourceAddress <= (MAX_HANDLED_FUNCTION_BLOCK + LOG_COM_BIT_ARRAY_LENGTH))
            {
                // источник дискретного сигнала - внешние команды управления логикой.
                return aucCoilsArray[LOG_COM_BIT_ARRAY_OFFSET +
                                     (usSourceAddress - 1) -
                                     MAX_HANDLED_FUNCTION_BLOCK];
            }
            // источник дискретного сигнала - входные сигналы "квитирование"?
            else if (usSourceAddress == (MAX_HANDLED_FUNCTION_BLOCK + LOG_COM_BIT_ARRAY_LENGTH +
                                         KVIT_COMMAND_BIT_ARRAY_LENGTH))
            {
                return fbGlobalKvit;//debag//
            }
            // источник дискретного сигнала - входные сигналы "сброс"?
            else if (usSourceAddress == (MAX_HANDLED_FUNCTION_BLOCK + LOG_COM_BIT_ARRAY_LENGTH +
                                         KVIT_COMMAND_BIT_ARRAY_LENGTH + RESET_COMMAND_BIT_ARRAY_LENGTH))
            {
                return fbGlobalReset;//debag//
            }

//            // источник дискретного сигнала - внешние команды управления логикой?
//            if (usSourceAddress > MAX_HANDLED_FUNCTION_BLOCK)
//            {
//                // источник дискретного сигнала - внешние команды управления логикой.
//                return aucCoilsArray[LOG_COM_BIT_ARRAY_OFFSET +
//                                     (usSourceAddress - 1) -
//                                     MAX_HANDLED_FUNCTION_BLOCK];
//            }
//            // источник дискретного сигнала - внешние команды управления логикой?
//            else if (usSourceAddress > MAX_HANDLED_FUNCTION_BLOCK)
//            {
//                // источник дискретного сигнала - выход функционального блока.
//                return aucCoilsArray[CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_OFFSET +
//                                     (usSourceAddress - 1)];
//            }
        }
        else
        {
            // дискретный сигнал не запрограммирован.
            return -1;
        }
        break;
        // биты достоверности аналоговых входных сигналов.
    case FUNCTION_BLOCK_INPUT_SOURCE_BAD_AI:
        // источник дискретного сигнала запрограммирован?
        if (usSourceAddress)
        {
            return aucCoilsArray[BAD_AI_BIT_ARRAY_OFFSET +
                                 (usSourceAddress - 1)];
        }
        else
        {
            // дискретный сигнал не запрограммирован.
            return -1;
        }
        break;
        // состояние выходов функциональной логики CoDeSys.
    case FUNCTION_BLOCK_INPUT_SOURCE_LOG_SOST:
        // источник дискретного сигнала запрограммирован?
        if (usSourceAddress)
        {
            return aucCoilsArray[LOG_SOST_BIT_ARRAY_OFFSET +
                                 (usSourceAddress - 1)];
        }
        else
        {
            // дискретный сигнал не запрограммирован.
            return -1;
        }
        break;
        // биты достоверности дискретных входных сигналов.
    case FUNCTION_BLOCK_INPUT_SOURCE_BAD_DI:
        // источник дискретного сигнала запрограммирован?
        if (usSourceAddress)
        {
            return aucCoilsArray[BAD_DI_BIT_ARRAY_OFFSET +
                                 (usSourceAddress - 1)];
        }
        else
        {
            // дискретный сигнал не запрограммирован.
            return -1;
        }
        break;

    default:
        // неизвестный источник дискретного сигнала.
        return -1;
        break;
    };
}

//-----------------------------------------------------------------------------------------------------
void vFunctionBlockHandler(void)
{
    unsigned char *pucSource;
    float *pfAnalogueInputSource;
    float *pfCalculatedValueSource;
    float fAnalogueData;
    float fSetPoint;
    float fMinOfScale;
    float fMaxOfScale;
    float fDifference;
    float fHysteresisDifference;
    float fHysteresis;
    float fHysteresisScale;
    unsigned char *pucFunctionBlockOuts;
    unsigned char *pucDoValueDestination;
    unsigned char *pucDoValueBlockDestination;
    unsigned char ucFlowControl;
    unsigned char ucFunctionBlockPreviousState;
    unsigned char ucFunctionBlockCurrentState;
    unsigned char ucFunctionBlockIntermediateState;
    int iDiscreteInputState;
    unsigned char ucInputSet;
    unsigned char ucInputReset;
    bool fbInputIsReliable;
    bool fbInputSetIsReliable;
    bool fbInputResetIsReliable;
    TFunctionBlockDescriptionWork *pxFunctionBlockDescriptionWork;
    TFunctionBlockWorkingData *pxFunctionBlockWorkingData;
    TAnalogueInputDescriptionWork *pxAnalogueInputDescriptionWork;
    TAlarmHmi *pxAlarmHmi;

    // получим указатель на буфер с нормализованной базой данных функциональных блоков прибора.
    pxFunctionBlockDescriptionWork =
        axFunctionBlockDescriptionWork;
    pxFunctionBlockWorkingData = &axFunctionBlockWorkingData[0];
    // получим указатель на буфер с нормализованной базой данных аналоговых входов.
    pxAnalogueInputDescriptionWork =
        axAnalogueInputDescriptionWork;
    pucFunctionBlockOuts =
        &aucCoilsArray[CURRENT_STEP_FUNCTION_BLOCK_OUTS_BIT_ARRAY_OFFSET];
    pfAnalogueInputSource =
        afAnalogueInputData;
    pfCalculatedValueSource =
        (float*)&aucHoldingRegistersArray[CALC_CDS_BYTE_ARRAY_LENGTH];
    pucDoValueDestination =
        &aucDoValueByteArray[0];
    pucDoValueBlockDestination =
        &aucDoValueBlockByteArray[0];//&aucCoilsArray[DO_VALUE_BIT_ARRAY_OFFSET];
    // получим указатель на рабочий массив формирования требований отображения сигнализации дискретных сигналов на HMI.
    // плучим указатель на место в массиве после данных дискретных сигналов.
    pxAlarmHmi =
        &axAlarmHmi[(xPlcConfigService.xPlcConfigServiceData.
                     uiHandledDiscreteInputQuantity)];

//cout << "uiHandledFunctionBlockQuantity " << (int)(xPlcConfigService.xPlcConfigServiceData.
//                 uiHandledFunctionBlockQuantity) << endl;

    // обработаем все запрограммированные функциональные блоки.
    for (int i = 0;
            i < (xPlcConfigService.xPlcConfigServiceData.
                 uiHandledFunctionBlockQuantity);
            i++)
    {
        // описатель функционального блока в базе данных повреждён?
        if (iFunctionBlockDataBaseCrcCheck(pxFunctionBlockDescriptionWork) == -1)
        {
            fbFunctionBlockDataBaseError = 1;
//            cout << "iFunctionBlockDataBaseCrcCheck error" << endl;
            // error

            // активное состояние события ещё не зарегистрировано?
            if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                        SYSTEM_ERROR_GROUP_OFFSET,
                        SYSTEM_ERROR_FBL_DATA_BASE_ERROR_OFFSET))
            {
                // зарегистрируем активное состояние события.
                CEvents::EventRegistration(
                    SYSTEM_ERROR_GROUP_OFFSET,
                    (CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE |
                     CEvents::HANDLED_EVENTS_IS_POPUP |
                     CEvents::HANDLED_EVENTS_IS_SOUND |
                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                    SYSTEM_ERROR_FBL_DATA_BASE_ERROR_OFFSET,
                    "Ошибка б.д. ФБЛ");
            }
        }
        else
        {
            // получим состояние функционального блока на предыдущем шаге.
            ucFunctionBlockPreviousState = *pucFunctionBlockOuts;
            // очистим флаг текущего состояния функционального блока.
            ucFunctionBlockCurrentState = 0;
            // какой тип  функционального блока?
            switch(FUNCTION_BLOCK_TYPE_FBL((pxFunctionBlockDescriptionWork)))
            {
//-----------------------------------------------------------------------------------------------------
                // функциональный блок "И".
            case FUNCTION_BLOCK_FLOW_AND:
                ucFunctionBlockCurrentState =
                    ucFourInputElementAnd(pxFunctionBlockDescriptionWork);
                // сохраним состояние функционального блока на текущем шаге.
                *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                break;

//-----------------------------------------------------------------------------------------------------
                // функциональный блок "И-НЕ".
            case FUNCTION_BLOCK_FLOW_AND_NOT:
                ucFunctionBlockCurrentState =
                    ucFourInputElementAnd(pxFunctionBlockDescriptionWork);
                // изменим состояние выхода в соответствии с логикой "И-НЕ".
                if (ucFunctionBlockCurrentState)
                {
                    ucFunctionBlockCurrentState = 0;
                }
                else
                {
                    ucFunctionBlockCurrentState = 1;
                }
                // сохраним состояние функционального блока на текущем шаге.
                *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                break;

//-----------------------------------------------------------------------------------------------------
                // функциональный блок "ИЛИ".
            case FUNCTION_BLOCK_FLOW_OR:
                ucFunctionBlockCurrentState =
                    ucFourInputElementOr(pxFunctionBlockDescriptionWork);
                // сохраним состояние функционального блока на текущем шаге.
                *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                break;

//-----------------------------------------------------------------------------------------------------
                // функциональный блок "ИЛИ-НЕ".
            case FUNCTION_BLOCK_FLOW_OR_NOT:
                ucFunctionBlockCurrentState =
                    ucFourInputElementOr(pxFunctionBlockDescriptionWork);
                // изменим состояние выхода в соответствии с логикой "ИЛИ-НЕ".
                if (ucFunctionBlockCurrentState)
                {
                    ucFunctionBlockCurrentState = 0;
                }
                else
                {
                    ucFunctionBlockCurrentState = 1;
                }
                // сохраним состояние функционального блока на текущем шаге.
                *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                break;

//-----------------------------------------------------------------------------------------------------
                // функциональный блок "Гистерезис".
            case FUNCTION_BLOCK_FLOW_HISTERESIS:
                // на предыдущем шаге функциональный блок был активен?
                if (ucFunctionBlockPreviousState)
                {
                    // на предыдущем шаге функциональный блок был активен.
                    // обратный ход - действует как "ИЛИ".
                    ucFunctionBlockCurrentState =
                        ucFourInputElementOr(pxFunctionBlockDescriptionWork);
                    // сохраним состояние функционального блока на текущем шаге.
                    *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                }
                else
                {
                    // на предыдущем шаге функциональный блок не был активен.
                    // прямой ход - действует как "И".
                    ucFunctionBlockCurrentState =
                        ucFourInputElementAnd(pxFunctionBlockDescriptionWork);
                    // сохраним состояние функционального блока на текущем шаге.
                    *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                }
                break;

//-----------------------------------------------------------------------------------------------------
// функциональный блок "Таймер".
// *** тип таймера:
// *** - 0 Задержка включения
// *** - 1 резерв
// *** - 2 Задержка отключения
// *** - 3 Короткий импульс
// *** - 4 Удлиненный импульс
// *** Пусковой момент ***
// При пуске ПАС-05 в П/П "vos_pred_8" флаги запуска таймеров уст. в "1"
// Это обеспечивает следующий алгоритм работы таймера в момент пуска:
// 1) Стартовое состояние выхода - "0"
// 2) Если  на входе "SET" нет активного сигнала, то выход остается "0"
//	и флаг запуска сбрасывается или нет в соответствии с типом таймера
// 3) Если  на входе "SET" есть активный сигнал, то выход также остается "0"
//	что соответствует состоянию обработанного активного сигнала
//	Флаг запуска остается до переключения входа "SET" в "0"
// Это позволяет избежать самозапуска таймеров с инверсными входами,
// но создает некотрые проблемы пуска схемы с обратной связью *)
            case FUNCTION_BLOCK_FLOW_TIMER:
                iDiscreteInputState =
                    iFunctionBlockInputCheck(INPUT_SOURCE_FBL_SET((pxFunctionBlockDescriptionWork)),
                                             ((((unsigned short)(ADDRESS_HIGH_FBL_SET((pxFunctionBlockDescriptionWork)))) << 8) |
                                              ((unsigned short)(ADDRESS_LOW_FBL_SET((pxFunctionBlockDescriptionWork))))));
                // дискретный сигнал "SET" - запрограммирован и достоверен?
                if (iDiscreteInputState != -1)
                {
                    // дискретный сигнал "SET" - "активен"?
                    if (((INPUT_TYPE_FBL_SET((pxFunctionBlockDescriptionWork))) &&
                            (iDiscreteInputState)) ||
                            (((INPUT_TYPE_FBL_SET((pxFunctionBlockDescriptionWork))) == 0) &&
                             (iDiscreteInputState == 0)))
                    {
                        ucInputSet = 1;
                    }
                    else
                    {
                        ucInputSet = 0;
                    }
                }
                else
                {
                    // сбросим таймер.
                    pxFunctionBlockWorkingData -> ucDelay = 0;
                    ucFunctionBlockCurrentState = 0;
                    break;
                }

//-----------------------------------------------------------------------------------------------------
                iDiscreteInputState =
                    iFunctionBlockInputCheck(INPUT_SOURCE_FBL_RESET((pxFunctionBlockDescriptionWork)),
                                             ((((unsigned short)(ADDRESS_HIGH_FBL_RESET((pxFunctionBlockDescriptionWork)))) << 8) |
                                              ((unsigned short)(ADDRESS_LOW_FBL_RESET((pxFunctionBlockDescriptionWork))))));
                // дискретный сигнал "RESET" - запрограммирован и достоверен?
                if (iDiscreteInputState != -1)
                {
                    // дискретный сигнал "RESET" - "активен"?
                    if (((INPUT_TYPE_FBL_RESET((pxFunctionBlockDescriptionWork))) &&
                            (iDiscreteInputState)) ||
                            (((INPUT_TYPE_FBL_RESET((pxFunctionBlockDescriptionWork))) == 0) &&
                             (iDiscreteInputState == 0)))
                    {
                        ucInputReset = 1;
                    }
                    else
                    {
                        ucInputReset = 0;
                    }
                }
                else
                {
                    ucInputReset = 0;
                }

//-----------------------------------------------------------------------------------------------------
                // какой тип таймера?
                switch(TIMER_TYPE_FBL((pxFunctionBlockDescriptionWork)))
                {
// (*** ТИП 0 - Задержка включения ***
// 1) SET
// Выход устанавливается в 1 после истечения времени задержки, запускаемой
// 1 на входе SET и сбрасывается 0 на входе SET.
// Если 1 на входе SET короче Tзад, то выход в 1 не устанавливается
// 2) RESET
// -сбрасывает отсчет времени, если он идет
// -сбрасывает выход в 0, если время истекло и он установлен в 1
// -если RESET=1 и на вход SET приходит 1, запуск не происходит*)
                case FUNCTION_BLOCK_TIMER_TYPE_ON_DELAY:
                    // дискретный сигнал "RESET" - активен?
                    if (ucInputReset)
                    {
                        // сбросим таймер.
                        pxFunctionBlockWorkingData -> ucDelay = 0;
                        ucFunctionBlockCurrentState = 0;
                        break;
                    }
                    // дискретный сигнал "SET" - активен?
                    if (ucInputSet)
                    {
                        // таймер запущен?
                        if (pxFunctionBlockWorkingData -> fbFunctionBlockIsActive)
                        {
                            // на предыдущем шаге выход функционального блока - "Таймер" был активен?
                            if (ucFunctionBlockPreviousState)
                            {
                                ucFunctionBlockCurrentState = 1;
                            }
                            else
                            {
                                // время задержки запрограммировано?
                                if (DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork)))
                                {
                                    // время задержки не истекло?
                                    if (pxFunctionBlockWorkingData -> ucDelay)
                                    {
                                        // какая дискретность времени задержки?
                                        switch(DELAY_TIME_DISCRETENESS_FBL((pxFunctionBlockDescriptionWork)))
                                        {
                                        case FUNCTION_BLOCK_TIMER_DISCRETENESS_100MS:
                                            pxFunctionBlockWorkingData -> ucDelay -= 1;
                                            break;

                                        case FUNCTION_BLOCK_TIMER_DISCRETENESS_1S:
                                            if (fbOneSecondPassed)
                                            {
                                                pxFunctionBlockWorkingData -> ucDelay -= 1;
                                            }
                                            break;

                                        case FUNCTION_BLOCK_TIMER_DISCRETENESS_10S:
                                            if (fbTenSecondsPassed)
                                            {
                                                pxFunctionBlockWorkingData -> ucDelay -= 1;
                                            }
                                            break;

                                        default:
                                            break;
                                        };
                                    }
                                    else
                                    {
                                        ucFunctionBlockCurrentState = 1;
                                    }
                                }
                                else
                                {
                                    ucFunctionBlockCurrentState = 1;
                                }
                            }
                        }
                        else
                        {
                            // установим время задержки таймера.
                            pxFunctionBlockWorkingData -> ucDelay =
                                DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork));
                            // запустим таймер.
                            pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 1;
                            ucFunctionBlockCurrentState = 0;
                        }
                    }
                    else
                    {
                        // сбросим таймер.
                        pxFunctionBlockWorkingData -> ucDelay = 0;
                        // остановим таймер.
                        pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 0;
                        ucFunctionBlockCurrentState = 0;
                    }


                    break;

//-----------------------------------------------------------------------------------------------------
                case FUNCTION_BLOCK_TIMER_TYPE_RESERVED:
                    break;

//-----------------------------------------------------------------------------------------------------
// (*** ТИП 2 - Задержка отключения ***
// 1) SET
// Выход устанавливается в 1 сразу при поступлении 1 на вход SET
// и переходит в 0 после истечения времени задержки, запускаемой переходом SET в 0
// Если вход SET перешел в 0 и через время короче Tзад вернулся в 1, то
// отсчет сбрасывается и выход остается в 1
// 2) RESET
// -сбрасывает отсчет времени, если он идет и сбрасывает при этом выход в 0
// -если отсчет времени не запущен, действия не оказывает*)
                case FUNCTION_BLOCK_TIMER_TYPE_OFF_DELAY:
                    // дискретный сигнал "RESET" - активен?
                    if (ucInputReset)
                    {
                        // таймер запущен?
                        if (pxFunctionBlockWorkingData -> fbFunctionBlockIsActive)
                        {
                            // сбросим таймер.
                            pxFunctionBlockWorkingData -> ucDelay = 0;
                            // остановим таймер.
                            pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 0;
                            ucFunctionBlockCurrentState = 0;
                            break;
                        }
                    }
                    // дискретный сигнал "SET" - активен?
                    if (ucInputSet)
                    {
                        // сбросим таймер.
                        pxFunctionBlockWorkingData -> ucDelay = 0;
                        // остановим таймер.
                        pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 0;
                        ucFunctionBlockCurrentState = 1;
                    }
                    else
                    {
                        // на предыдущем шаге выход функционального блока - "Таймер" был активен?
                        if (ucFunctionBlockPreviousState)
                        {
                            ucFunctionBlockCurrentState = 1;
                            // таймер запущен?
                            if (pxFunctionBlockWorkingData -> fbFunctionBlockIsActive)
                            {
                                // время задержки запрограммировано?
                                if (DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork)))
                                {
                                    // время задержки не истекло?
                                    if (pxFunctionBlockWorkingData -> ucDelay)
                                    {
                                        // какая дискретность времени задержки?
                                        switch(DELAY_TIME_DISCRETENESS_FBL((pxFunctionBlockDescriptionWork)))
                                        {
                                        case FUNCTION_BLOCK_TIMER_DISCRETENESS_100MS:
                                            pxFunctionBlockWorkingData -> ucDelay -= 1;
                                            break;

                                        case FUNCTION_BLOCK_TIMER_DISCRETENESS_1S:
                                            if (fbOneSecondPassed)
                                            {
                                                pxFunctionBlockWorkingData -> ucDelay -= 1;
                                            }
                                            break;

                                        case FUNCTION_BLOCK_TIMER_DISCRETENESS_10S:
                                            if (fbTenSecondsPassed)
                                            {
                                                pxFunctionBlockWorkingData -> ucDelay -= 1;
                                            }
                                            break;

                                        default:
                                            break;
                                        };
                                    }
                                    else
                                    {
                                        ucFunctionBlockCurrentState = 0;
                                    }
                                }
                                else
                                {
                                    ucFunctionBlockCurrentState = 0;
                                }
                            }
                            else
                            {
                                // установим время задержки таймера.
                                pxFunctionBlockWorkingData -> ucDelay =
                                    DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork));
                                // запустим таймер.
                                pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 1;
                            }
                        }
                        else
                        {
                            // сбросим таймер.
                            pxFunctionBlockWorkingData -> ucDelay = 0;
                            // остановим таймер.
                            pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 0;
                            ucFunctionBlockCurrentState = 0;
                        }
                    }
                    break;

//-----------------------------------------------------------------------------------------------------
// (*** ТИП 3 - Короткий импульс ***
// 1) SET
// Выход устанавливается в 1 сразу при поступлении 1 на вход SET
// и задержка сразу запускается.
// Выход переходит в 0 после истечения времени задержки.
// Если 1 на входе SET короче Tзад, то выход переходит в 0
// с переходом SET в 0
// 2) RESET
// -сбрасывает отсчет времени и выход в 0, если он идет
// -если RESET=1 и на вход SET приходит 1, запуск не происходит*)
                case FUNCTION_BLOCK_TIMER_TYPE_SHORT_PULSE:
                    // дискретный сигнал "RESET" - активен?
                    if (ucInputReset)
                    {
                        // сбросим таймер.
                        pxFunctionBlockWorkingData -> ucDelay = 0;
                        ucFunctionBlockCurrentState = 0;
                        break;
                    }
                    // дискретный сигнал "SET" - активен?
                    if (ucInputSet)
                    {
                        ucFunctionBlockCurrentState = 1;
                        // таймер запущен?
                        if (pxFunctionBlockWorkingData -> fbFunctionBlockIsActive)
                        {
                            // время задержки запрограммировано?
                            if (DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork)))
                            {
                                // время задержки не истекло?
                                if (pxFunctionBlockWorkingData -> ucDelay)
                                {
                                    // какая дискретность времени задержки?
                                    switch(DELAY_TIME_DISCRETENESS_FBL((pxFunctionBlockDescriptionWork)))
                                    {
                                    case FUNCTION_BLOCK_TIMER_DISCRETENESS_100MS:
                                        pxFunctionBlockWorkingData -> ucDelay -= 1;
                                        break;

                                    case FUNCTION_BLOCK_TIMER_DISCRETENESS_1S:
                                        if (fbOneSecondPassed)
                                        {
                                            pxFunctionBlockWorkingData -> ucDelay -= 1;
                                        }
                                        break;

                                    case FUNCTION_BLOCK_TIMER_DISCRETENESS_10S:
                                        if (fbTenSecondsPassed)
                                        {
                                            pxFunctionBlockWorkingData -> ucDelay -= 1;
                                        }
                                        break;

                                    default:
                                        break;
                                    };
                                }
                                else
                                {
                                    ucFunctionBlockCurrentState = 0;
                                }
                            }
                            else
                            {
                                ucFunctionBlockCurrentState = 0;
                            }
                        }
                        else
                        {
                            // установим время задержки таймера.
                            pxFunctionBlockWorkingData -> ucDelay =
                                DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork));
                            // запустим таймер.
                            pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 1;
                            ucFunctionBlockCurrentState = 1;
                        }
                    }
                    else
                    {
                        // сбросим таймер.
                        pxFunctionBlockWorkingData -> ucDelay = 0;
                        // остановим таймер.
                        pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 0;
                        ucFunctionBlockCurrentState = 0;
                    }
                    break;

//-----------------------------------------------------------------------------------------------------
// (*** ТИП 4 - Удлиненный импульс ***
// 1) SET
// Выход устанавливается в 1 сразу при поступлении 1 на вход SET
// и задержка сразу запускается. Выход переходит в 0 после истечения
// времени задержки независимо от длительности импульса запуска.
// Если SET перешел в 0 и через время короче Tзад вернулся в 1, то выход
// остается 1 и отсчет начинается сначала.
// 2) RESET
// -сбрасывает отсчет времени и выход в 0, если он идет
// -если RESET=1 и на вход SET приходит 1, запуск не происходит*)
                case FUNCTION_BLOCK_TIMER_TYPE_LONG_PULSE:
                    // дискретный сигнал "RESET" - активен?
                    if (ucInputReset)
                    {
                        // сбросим таймер.
                        pxFunctionBlockWorkingData -> ucDelay = 0;
                        ucFunctionBlockCurrentState = 0;
                        break;
                    }
                    // дискретный сигнал "SET" - активен?
                    if (ucInputSet)
                    {
                        ucFunctionBlockCurrentState = 1;
                        // таймер запущен?
                        if (pxFunctionBlockWorkingData -> fbFunctionBlockIsActive)
                        {
                            // время задержки запрограммировано?
                            if (DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork)))
                            {
                                // время задержки не истекло?
                                if (pxFunctionBlockWorkingData -> ucDelay)
                                {
                                    // какая дискретность времени задержки?
                                    switch(DELAY_TIME_DISCRETENESS_FBL((pxFunctionBlockDescriptionWork)))
                                    {
                                    case FUNCTION_BLOCK_TIMER_DISCRETENESS_100MS:
                                        pxFunctionBlockWorkingData -> ucDelay -= 1;
                                        break;

                                    case FUNCTION_BLOCK_TIMER_DISCRETENESS_1S:
                                        if (fbOneSecondPassed)
                                        {
                                            pxFunctionBlockWorkingData -> ucDelay -= 1;
                                        }
                                        break;

                                    case FUNCTION_BLOCK_TIMER_DISCRETENESS_10S:
                                        if (fbTenSecondsPassed)
                                        {
                                            pxFunctionBlockWorkingData -> ucDelay -= 1;
                                        }
                                        break;

                                    default:
                                        break;
                                    };
                                }
                                else
                                {
                                    ucFunctionBlockCurrentState = 0;
                                }
                            }
                            else
                            {
                                ucFunctionBlockCurrentState = 0;
                            }
                        }
                        else
                        {
                            // установим время задержки таймера.
                            pxFunctionBlockWorkingData -> ucDelay =
                                DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork));
                            // запустим таймер.
                            pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 1;
                            ucFunctionBlockCurrentState = 1;
                        }
                    }
                    else
                    {
                        ucFunctionBlockCurrentState = 1;
                        // остановим таймер.
                        pxFunctionBlockWorkingData -> fbFunctionBlockIsActive = 0;
                        // время задержки запрограммировано?
                        if (DELAY_TIME_SETPOINT_FBL((pxFunctionBlockDescriptionWork)))
                        {
                            // время задержки не истекло?
                            if (pxFunctionBlockWorkingData -> ucDelay)
                            {
                                // какая дискретность времени задержки?
                                switch(DELAY_TIME_DISCRETENESS_FBL((pxFunctionBlockDescriptionWork)))
                                {
                                case FUNCTION_BLOCK_TIMER_DISCRETENESS_100MS:
                                    pxFunctionBlockWorkingData -> ucDelay -= 1;
                                    break;

                                case FUNCTION_BLOCK_TIMER_DISCRETENESS_1S:
                                    if (fbOneSecondPassed)
                                    {
                                        pxFunctionBlockWorkingData -> ucDelay -= 1;
                                    }
                                    break;

                                case FUNCTION_BLOCK_TIMER_DISCRETENESS_10S:
                                    if (fbTenSecondsPassed)
                                    {
                                        pxFunctionBlockWorkingData -> ucDelay -= 1;
                                    }
                                    break;

                                default:
                                    break;
                                };
                            }
                            else
                            {
                                ucFunctionBlockCurrentState = 0;
                            }
                        }
                        else
                        {
                            ucFunctionBlockCurrentState = 0;
                        }
                    }
                    break;

                default:
                    break;
                };
                // сохраним состояние функционального блока на текущем шаге.
                *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                break;

//-----------------------------------------------------------------------------------------------------
// функциональный блок "Триггер".
// ( *  "ТРИГГЕР"
// 		тип триггера  D4=0 - R-S приоритет сброса
// 					  D4=1 - S-R приоритет установки*)
// ( *Обработка входов SET/RESET для ФБЛ "ТРИГГЕР"
// Входы 1,2 - SET по схеме "ИЛИ", входы 3,4 - RESET по схеме "ИЛИ"
// Если оба входа SET или оба входа RESET недостоверны или
// не запрограммированы, то на выходе всегда RESET*)
            case FUNCTION_BLOCK_FLOW_TRIGGER:
                // Начальная установка - вход неактивен.
                ucInputSet = 0;
                fbInputSetIsReliable = 0;

                iDiscreteInputState =
                    iFunctionBlockInputCheck(INPUT_SOURCE_FBL_SET_1((pxFunctionBlockDescriptionWork)),
                                             ((((unsigned short)(ADDRESS_HIGH_FBL_SET_1((pxFunctionBlockDescriptionWork)))) << 8) |
                                              ((unsigned short)(ADDRESS_LOW_FBL_SET_1((pxFunctionBlockDescriptionWork))))));
                // дискретный сигнал "SET1" - запрограммирован и достоверен?
                if (iDiscreteInputState != -1)
                {
                    fbInputSetIsReliable = 1;
                    // дискретный сигнал "SET1" - "активен"?
                    if (((INPUT_TYPE_FBL_SET_1((pxFunctionBlockDescriptionWork))) &&
                            (iDiscreteInputState)) ||
                            (((INPUT_TYPE_FBL_SET_1((pxFunctionBlockDescriptionWork))) == 0) &&
                             (iDiscreteInputState == 0)))
                    {
                        ucInputSet = 1;
                    }
                    else
                    {

                    }
                }
                else
                {

                }

//-----------------------------------------------------------------------------------------------------
                iDiscreteInputState =
                    iFunctionBlockInputCheck(INPUT_SOURCE_FBL_SET_2((pxFunctionBlockDescriptionWork)),
                                             ((((unsigned short)(ADDRESS_HIGH_FBL_SET_2((pxFunctionBlockDescriptionWork)))) << 8) |
                                              ((unsigned short)(ADDRESS_LOW_FBL_SET_2((pxFunctionBlockDescriptionWork))))));
                // дискретный сигнал "SET2" - запрограммирован и достоверен?
                if (iDiscreteInputState != -1)
                {
                    fbInputSetIsReliable = 1;
                    // дискретный сигнал "SET2" - "активен"?
                    if (((INPUT_TYPE_FBL_SET_2((pxFunctionBlockDescriptionWork))) &&
                            (iDiscreteInputState)) ||
                            (((INPUT_TYPE_FBL_SET_2((pxFunctionBlockDescriptionWork))) == 0) &&
                             (iDiscreteInputState == 0)))
                    {
                        ucInputSet = 1;
                    }
                    else
                    {

                    }
                }
                else
                {

                }

//-----------------------------------------------------------------------------------------------------
                // Начальная установка - вход неактивен.
                ucInputReset = 0;
                fbInputResetIsReliable = 0;

                iDiscreteInputState =
                    iFunctionBlockInputCheck(INPUT_SOURCE_FBL_RESET_1((pxFunctionBlockDescriptionWork)),
                                             ((((unsigned short)(ADDRESS_HIGH_FBL_RESET_1((pxFunctionBlockDescriptionWork)))) << 8) |
                                              ((unsigned short)(ADDRESS_LOW_FBL_RESET_1((pxFunctionBlockDescriptionWork))))));
                // дискретный сигнал "RESET1" - запрограммирован и достоверен?
                if (iDiscreteInputState != -1)
                {
                    fbInputResetIsReliable = 1;
                    // дискретный сигнал "RESET1" - "активен"?
                    if (((INPUT_TYPE_FBL_RESET_1((pxFunctionBlockDescriptionWork))) &&
                            (iDiscreteInputState)) ||
                            (((INPUT_TYPE_FBL_RESET_1((pxFunctionBlockDescriptionWork))) == 0) &&
                             (iDiscreteInputState == 0)))
                    {
                        ucInputReset = 1;
                    }
                    else
                    {

                    }
                }
                else
                {

                }

//-----------------------------------------------------------------------------------------------------
                iDiscreteInputState =
                    iFunctionBlockInputCheck(INPUT_SOURCE_FBL_RESET_2((pxFunctionBlockDescriptionWork)),
                                             ((((unsigned short)(ADDRESS_HIGH_FBL_RESET_2((pxFunctionBlockDescriptionWork)))) << 8) |
                                              ((unsigned short)(ADDRESS_LOW_FBL_RESET_2((pxFunctionBlockDescriptionWork))))));
                // дискретный сигнал "RESET2" - запрограммирован и достоверен?
                if (iDiscreteInputState != -1)
                {
                    fbInputResetIsReliable = 1;
                    // дискретный сигнал "RESET2" - "активен"?
                    if (((INPUT_TYPE_FBL_RESET_2((pxFunctionBlockDescriptionWork))) &&
                            (iDiscreteInputState)) ||
                            (((INPUT_TYPE_FBL_RESET_2((pxFunctionBlockDescriptionWork))) == 0) &&
                             (iDiscreteInputState == 0)))
                    {
                        ucInputReset = 1;
                    }
                    else
                    {

                    }
                }
                else
                {

                }

//-----------------------------------------------------------------------------------------------------
                // Оба входа SET или оба входа RESET недостоверны или не запрограммированы?
                if ((fbInputSetIsReliable == 0) ||
                        (fbInputResetIsReliable == 0))
                {
                    // RESET.
                    ucInputSet = 0;
                    ucInputReset = 1;
                }
                // Оба входа неактивны?
                if ((ucInputSet == 0) &&
                        (ucInputReset == 0))
                {
                    // оставляем предыдущее состояние.
                    // на предыдущем шаге выход функционального блока был активен?
                    if (ucFunctionBlockPreviousState)
                    {
                        ucFunctionBlockCurrentState = 1;
                    }
                    else
                    {
                        ucFunctionBlockCurrentState = 0;
                    }
                    break;
                }
                // какой тип триггера?
                switch(TRIGGER_TYPE_FBL((pxFunctionBlockDescriptionWork)))
                {
                case FUNCTION_BLOCK_TRIGGER_TYPE_R_S:
                    if (ucInputReset)
                    {
                        ucFunctionBlockCurrentState = 0;
                    }
                    else
                    {
                        ucFunctionBlockCurrentState = 1;
                    }
                    break;

                case FUNCTION_BLOCK_TRIGGER_TYPE_S_R:
                    if (ucInputSet)
                    {
                        ucFunctionBlockCurrentState = 1;
                    }
                    else
                    {
                        ucFunctionBlockCurrentState = 0;
                    }
                    break;

                default:
                    break;
                };
                // сохраним состояние функционального блока на текущем шаге.
                *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                break;

//-----------------------------------------------------------------------------------------------------
// функциональный блок "Аналоговый компаратор".
// (*  "АНАЛОГОВЫЙ КОМПАРАТОР"
//		Inp_Addr [1] - D6-D0-адрес аналогового сигнала , D7=0  сигнал из области аналоговых измерений
//						D7 = 1 сигнал из области расчетных величин
//		Inp_Addr [6] - D7 = 0 сравнение на МЕНЬШЕ, D7 = 1 сравнение на БОЛЬШЕ, D6-D2  гистерезис 0-31%
//		Inp_Addr [7], Inp_Addr [8]  - адрес входа дискретного сигнала разрешения
//		Inp_Addr [2] - Inp_Addr [5] - уставка в формате IEEE 754 -  это не соотв. сущ. структуре !!!
//		Остальное как и у всех ФБЛ *)
// (*Определение состояния дискретного сигнала разрешения
// Принцип действия сигнала: если не запрещено, то разрешено, запрещает "0"
// Если входной сигнал в норме и активен, или не запрограммирован, или недостоверен - ФБЛ работает
// Если входной сигнал в норме и неактивен - ФБЛ не работает*)
            case FUNCTION_BLOCK_FLOW_ANALOGUE_COMPARATOR:
                iDiscreteInputState =
                    iFunctionBlockInputCheck(DISCRETE_INPUT_ENABLE_SOURCE_FBL((pxFunctionBlockDescriptionWork)),
                                             ((((unsigned short)(DISCRETE_INPUT_ENABLE_ADDRESS_HIGH_FBL((pxFunctionBlockDescriptionWork)))) << 8) |
                                              ((unsigned short)(DISCRETE_INPUT_ENABLE_ADDRESS_LOW_FBL((pxFunctionBlockDescriptionWork))))));
                // дискретный сигнал "ENABLE" - запрограммирован и достоверен?
                if (iDiscreteInputState != -1)
                {
                    fbInputIsReliable = 1;
                    // дискретный сигнал "ENABLE" - "активен"?
                    if (((DISCRETE_INPUT_ENABLE_TYPE_FBL((pxFunctionBlockDescriptionWork))) &&
                            (iDiscreteInputState)) ||
                            (((DISCRETE_INPUT_ENABLE_TYPE_FBL((pxFunctionBlockDescriptionWork))) == 0) &&
                             (iDiscreteInputState == 0)))
                    {
                        // дискретный сигнал "ENABLE" - "активен" - функциональный блок работает.
                    }
                    else
                    {
                        // дискретный сигнал "ENABLE" - "неактивен" - функциональный блок не работает.
                        ucFunctionBlockCurrentState = 0;
                        break;
                    }
                }
                else
                {
                    fbInputIsReliable = 0;
                }
                // входной аналоговый сигнал не запрограммирован?
                if (!(ANALOGUE_INPUT_FBL_ADDRESS((pxFunctionBlockDescriptionWork))))
                {
                    ucFunctionBlockCurrentState = 0;
                    break;
                }
                // Входной сигнал из области расчетных величин?
                if (ANALOGUE_INPUT_FBL_TYPE_FBL((pxFunctionBlockDescriptionWork)))
                {
                    // Входной сигнал из области расчетных величин.
                    // получим значение аналоговой величины FLOAT.
                    fAnalogueData =
                        pfCalculatedValueSource[ANALOGUE_INPUT_FBL_ADDRESS((pxFunctionBlockDescriptionWork))];
                }
                else
                {
                    // Входной сигнал из области аналоговых измерений.
                    // получим значение аналоговой величины FLOAT.
                    fAnalogueData =
                        pfAnalogueInputSource[ANALOGUE_INPUT_FBL_ADDRESS((pxFunctionBlockDescriptionWork))];
                }
                // получим значение уставки FLOAT.
                fSetPoint =
                    SETPOINT_FBL((pxFunctionBlockDescriptionWork));
                // получим разность между значением аналоговой величины и значением уставки.
                fDifference = fAnalogueData - fSetPoint;
                // уставка нарушена?
                // (значение аналоговой величины меньше уставки при условии сравнения
                // (При cond = 0 выход = 1, если аналоговый сигнал МЕНЬШЕ УСТАВКИ)? или
                // значение аналоговой величины больше уставки при условии сравнения
                // (При cond = 1 выход = 1, если аналоговый сигнал БОЛЬШЕ УСТАВКИ)?)
                if (((fDifference < 0) &&
                        ((COMPARISON_CONDITION_FBL((pxFunctionBlockDescriptionWork))) == 0)) ||
                        ((fDifference >= 0) &&
                         (COMPARISON_CONDITION_FBL((pxFunctionBlockDescriptionWork)))))
                {
                    ucFunctionBlockCurrentState = 1;
                }
                else
                {
                    // Входной сигнал из области расчетных величин? или
                    // на предыдущем шаге нарушения не было? или
                    // гистерезис не запрограммирован?
                    if ((ANALOGUE_INPUT_FBL_TYPE_FBL((pxFunctionBlockDescriptionWork))) ||
                            (!(ucFunctionBlockPreviousState)) ||
                            (HYSTERESIS_ANALOGUE_SCALE_RANGE_FBL((pxFunctionBlockDescriptionWork)) == 0))
                    {
                        ucFunctionBlockCurrentState = 0;
                        break;
                    }
                    else
                    {
                        // Требуется проверка гистерезиса - вычисление ширины зоны гистерезиса в единицах шкалы.
                        // получим значение НШК - нижний предел шкалы датчика, из описателя
                        // аналогового входа базы данных.
                        fMinOfScale =
                            pxAnalogueInputDescriptionWork[ANALOGUE_INPUT_FBL_ADDRESS((pxFunctionBlockDescriptionWork))].
                            fMinOfScale;
                        // получим значение ВШК - верхний предел шкалы датчика, из описателя
                        // аналогового входа базы данных.
                        fMaxOfScale =
                            pxAnalogueInputDescriptionWork[ANALOGUE_INPUT_FBL_ADDRESS((pxFunctionBlockDescriptionWork))].
                            fMaxOfScale;
                        // преобразуем значение гистерезиса из (*Зона гистерезиса в % шкалы 0-31% FLOAT*) в
                        // (*Зона гистерезиса в единицах шкалы*).
                        fHysteresisScale =
                            (float)(((fMaxOfScale - fMinOfScale) / 100) *
                                    HYSTERESIS_ANALOGUE_SCALE_RANGE_FBL((pxFunctionBlockDescriptionWork)));
                        // значение аналоговой величины меньше уставки?
                        if (fDifference < 0)
                        {
                            // значение аналоговой величины меньше уставки.
                            // проверим нижнюю зону гистерезиса.
                            // получим разность между значением гистерезиса и разностью
                            // между аналоговой величиной и уставкой
                            // (гистерезис + (значение аналоговой величины - уставка)).
                            // если разность положительная, значит аналоговая величина в зоне гистерезиса.
                            fHysteresisDifference = fHysteresisScale + fDifference;
                        }
                        else
                        {
                            // значение аналоговой величины больше уставки.
                            // проверим верхнюю зону гистерезиса.
                            // получим разность между значением гистерезиса и разностью
                            // между аналоговой величиной и уставкой
                            // (гистерезис - (значение аналоговой величины - уставка)).
                            // если разность положительная, значит аналоговая величина в зоне гистерезиса.
                            fHysteresisDifference = fHysteresisScale - fDifference;
                        }
                        // Еще в зоне гистерезиса?
                        if (fHysteresisDifference >= 0)
                        {
                            // Еще в зоне гистерезиса, нарушение сохраняется.
                            ucFunctionBlockCurrentState = 1;
                            break;
                        }
                        else
                        {
                            // Вышли из зоны гистерезиса, нарушения нет.
                            ucFunctionBlockCurrentState = 0;
                            break;
                        }
                    }
                }
                // сохраним состояние функционального блока на текущем шаге.
                *pucFunctionBlockOuts = ucFunctionBlockCurrentState;
                break;

            default:
                break;
            };
        };


//-----------------------------------------------------------------------------------------------------
        // функциональный блок активен?
        if (ucFunctionBlockCurrentState)
        {
            // функциональный блок был активен на предыдущем шаге?
            if (ucFunctionBlockPreviousState)
            {
                // не новое нарушение.
                // установим флаги - требования включения, для запрограммированных реле,
                // в буфере выходов требований включения реле - блокировка.
                // цикл выбора модуля.
                for (int j = 0;
                        j < (FUNCTION_BLOCK_RELAY_OUTPUT_QUANTITY /
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
                            (((pxFunctionBlockDescriptionWork -> aucRelayOut[j]) >> k) & 0x01);
                    }
                }
            }
            else
            {
                // новое нарушение.
                // установим флаги - новое нарушение, для запрограммированных реле
                // в буфере выходов управления реле - новое нарушение.
                // цикл выбора модуля.
                for (int j = 0;
                        j < (FUNCTION_BLOCK_RELAY_OUTPUT_QUANTITY /
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
                            (((pxFunctionBlockDescriptionWork -> aucRelayOut[j]) >> k) & 0x01);
                    }
                }
                // установим флаги - требования включения, для запрограммированных реле,
                // в буфере выходов требований включения реле - блокировка.
                // цикл выбора модуля.
                for (int j = 0;
                        j < (FUNCTION_BLOCK_RELAY_OUTPUT_QUANTITY /
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
                            (((pxFunctionBlockDescriptionWork -> aucRelayOut[j]) >> k) & 0x01);
                    }
                }
            }

//            // архивировать?
//            if (ARCHIVE_FBL((pxFunctionBlockDescriptionWork)))
//            {
//                // arch
//                pxAlarmHmi -> ui8Archive = 1;
//            }
//            else
//            {
//                pxAlarmHmi -> ui8Archive = 0;
//            }

//            // установим тип общей сигнализации.
//            // нет аварийной сигнализации?
//            if (!((ucAlarmStateCommon >> ALARM_TYPE_ALARM) & 0x01))
//            {
//                // для текущего функционального блока запрограммирована сигнализация или индикация?
//                if ((SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork))) ||
//                        (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork))))
//                {
//                    // для текущего функционального блока запрограммирована сигнализация?
//                    if (((!SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork))) &&
//                            (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))) ||
//                            ((SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork))) &&
//                             (!SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))))
//                    {
//                        // очистим флаги предыдущего состояния сигнализации.
//                        ucAlarmStateCommon = 0;
//                        // для текущего функционального блока запрограммирована предупредительная сигнализация?
//                        if (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))
//                        {
//                            // установим флаг предупредительной сигнализации.
//                            ucAlarmStateCommon |= (0x01 << ALARM_TYPE_PREVENT);
//                        }
//                        else
//                        {
//                            // сбросим флаг предупредительной сигнализации.
//                            ucAlarmStateCommon &= ~(0x01 << ALARM_TYPE_PREVENT);
//                        }
//                        // для текущего функционального блока запрограммирована аварийная сигнализация?
//                        if (SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork)))
//                        {
//                            // установим флаг аварийной сигнализации.
//                            ucAlarmStateCommon |= (0x01 << ALARM_TYPE_ALARM);
//                        }
//                        else
//                        {
//                            // сбросим флаг аварийной сигнализации.
//                            ucAlarmStateCommon &= ~(0x01 << ALARM_TYPE_ALARM);
//                        }
//                    }
//                }
//            }
//
//            // очистим флаги предыдущего состояния сигнализации.
//            pxAlarmHmi -> ui8AlarmState = 0;
//            // установим тип сигнализации для текущего функционального блока.
//            // для текущего функционального блока запрограммирована предупредительная сигнализация?
//            if (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))
//            {
//                // установим флаг предупредительной сигнализации.
//                pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
//            }
//            else
//            {
//                // сбросим флаг предупредительной сигнализации.
//                pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_PREVENT);
//            }
//            // для текущего функционального блока запрограммирована аварийная сигнализация?
//            if (SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork)))
//            {
//                // установим флаг аварийной сигнализации.
//                pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
//            }
//            else
//            {
//                // сбросим флаг аварийной сигнализации.
//                pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_ALARM);
//            }

//
//            // для текущего функционального блока запрограммирована сигнализация или индикация?
//            if ((SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork))) ||
//                    (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork))))
//            {
//                // для текущего функционального блока запрограммирована сигнализация?
//                if (((!SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork))) &&
//                        (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))) ||
//                        ((SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork))) &&
//                         (!SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))))
//                {
//                    // для текущего функционального блока запрограммирована предупредительная сигнализация?
//                    if (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))
//                    {
//                        // установим флаг предупредительной сигнализации.
//                        pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
//                    }
//                    else
//                    {
//                        // сбросим флаг предупредительной сигнализации.
//                        pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_PREVENT);
//                    }
//                    // для текущего функционального блока запрограммирована аварийная сигнализация?
//                    if (SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork)))
//                    {
//                        // установим флаг аварийной сигнализации.
//                        pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
//                    }
//                    else
//                    {
//                        // сбросим флаг аварийной сигнализации.
//                        pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_ALARM);
//                    }
//                }
//            }
        }
        else
        {
            // функциональный блок был активен на предыдущем шаге?
            if (ucFunctionBlockPreviousState)
            {
//                // архивировать?
//                if (ARCHIVE_FBL((pxFunctionBlockDescriptionWork)))
//                {
//                    // arch
//                    pxAlarmHmi -> ui8Archive = 1;
//                }
//                else
//                {
//                    pxAlarmHmi -> ui8Archive = 0;
//                }

                if (ARCHIVE_FBL((pxFunctionBlockDescriptionWork)))
                {
                    // неактивное состояние события ещё не зарегистрировано?
                    if(xCAlarmEvent.EventOffIsNotRegistered(
                        i,
                        0))
                    {
                        // зарегистрируем неактивное состояние события.
                        CEvents::EventRegistration(
                            i,
                            (CEvents::HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE |
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
                            (CEvents::HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE),
                            0,
                            0);
                    }
                }
            }
        }

//-----------------------------------------------------------------------------------------------------
//        // сигнализация не запрограммирована? или, запрограммирована индикация?
//        if (((SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork))) &&
//                (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))) ||
//                ((!(SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork)))) &&
//                 (!(SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork))))))
        // сигнализация не запрограммирована?
        if ((!(SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork)))) &&
                (!(SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))))
        {
            // сбросим флаги требования отображения сигнализации на HMI - всё в норму.
            *pxAlarmHmi = xAlarmHmiNull;
        }
        else
        {
            // запрограммирована сигнализация.
            // функциональный блок активен?
            if (ucFunctionBlockCurrentState)
            {
                // для текущего дискретного сигнала установлено требование отображения сигнализации на HMI?
                if (pxAlarmHmi -> ui8AlarmState)
                {

                }
                else
                {
                    // укажем группу для отображения сигнализации на HMI.
                    pxAlarmHmi -> ui8AlarmState =
                        LIGHT_BOARD_CELL_NUMBER_FBL((pxFunctionBlockDescriptionWork));
                    // установим тип сигнализации для текущего функционального блока.
                    // для текущего функционального блока запрограммирована предупредительная сигнализация?
                    if (SIGNALING_TYPE_1_FBL((pxFunctionBlockDescriptionWork)))
                    {
                        // установим флаг предупредительной сигнализации.
                        pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_PREVENT);
                    }
                    else
                    {
                        // сбросим флаг предупредительной сигнализации.
                        pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_PREVENT);
                    }
                    // для текущего функционального блока запрограммирована аварийная сигнализация?
                    if (SIGNALING_TYPE_2_FBL((pxFunctionBlockDescriptionWork)))
                    {
                        // установим флаг аварийной сигнализации.
                        pxAlarmHmi -> ui8AlarmState |= (0x01 << ALARM_TYPE_ALARM);
                    }
                    else
                    {
                        // сбросим флаг аварийной сигнализации.
                        pxAlarmHmi -> ui8AlarmState &= ~(0x01 << ALARM_TYPE_ALARM);
                    }

                    if (ARCHIVE_FBL((pxFunctionBlockDescriptionWork)))
                    {
                        // активное состояние события ещё не зарегистрировано?
                        if(xCAlarmEvent.EventOnIsNotRegistered(
                                    i,
                                    0))
                        {
                            // зарегистрируем активное состояние события.
                            CEvents::EventRegistration(
                                i,
                                (CEvents::HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE |
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
                                (CEvents::HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE |
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
                // для текущего дискретного сигнала установлено требование отображения сигнализации на HMI?
                if (pxAlarmHmi -> ui8AlarmState)
                {
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
        pucFunctionBlockOuts++;
        pxFunctionBlockDescriptionWork++;
        pxFunctionBlockWorkingData++;
        pxAnalogueInputDescriptionWork++;
        pxAlarmHmi++;

//-----------------------------------------------------------------------------------------------------
    }
}

//-----------------------------------------------------------------------------------------------------
// четырёхвходовый логический элемент "И".
// получает указатель на описатель функционального блока в нормализованной базе данных.
// возвращает состояние выхода логического элемента.
unsigned char ucFourInputElementAnd(TFunctionBlockDescriptionWork *pxSource)
{
    unsigned char ucCurrentState;
    int iDiscreteInputState;
    bool fbInputIsReliable;
    // начальное состояние выхода четырёхвходового элемента "И" - 1.
    ucCurrentState = 1;
    fbInputIsReliable = 0;

    iDiscreteInputState =
        iFunctionBlockInputCheck(INPUT_SOURCE_FBL_1(pxSource),
                                 ((((unsigned short)(ADDRESS_HIGH_FBL_1(pxSource))) << 8) |
                                  ((unsigned short)(ADDRESS_LOW_FBL_1(pxSource)))));
    // дискретный сигнал - запрограммирован и достоверен?
    if (iDiscreteInputState != -1)
    {
        fbInputIsReliable = 1;
        // дискретный сигнал - "активен"?
        if (((INPUT_TYPE_FBL_1(pxSource)) &&
                (iDiscreteInputState)) ||
                (((INPUT_TYPE_FBL_1(pxSource)) == 0) &&
                 (iDiscreteInputState == 0)))
        {

        }
        else
        {
            // дискретный сигнал - "неактивен".
            // состояние выхода четырёхвходового элемента "И" - 0.
            ucCurrentState = 0;
        }
    }

//-----------------------------------------------------------------------------------------------------
    iDiscreteInputState =
        iFunctionBlockInputCheck(INPUT_SOURCE_FBL_2(pxSource),
                                 ((((unsigned short)(ADDRESS_HIGH_FBL_2(pxSource))) << 8) |
                                  ((unsigned short)(ADDRESS_LOW_FBL_2(pxSource)))));
    // дискретный сигнал - запрограммирован и достоверен?
    if (iDiscreteInputState != -1)
    {
        fbInputIsReliable = 1;
        // дискретный сигнал - "активен"?
        if (((INPUT_TYPE_FBL_2(pxSource)) &&
                (iDiscreteInputState)) ||
                (((INPUT_TYPE_FBL_2(pxSource)) == 0) &&
                 (iDiscreteInputState == 0)))
        {

        }
        else
        {
            // дискретный сигнал - "неактивен".
            // состояние выхода четырёхвходового элемента "И" - 0.
            ucCurrentState = 0;
        }
    }

//-----------------------------------------------------------------------------------------------------
    iDiscreteInputState =
        iFunctionBlockInputCheck(INPUT_SOURCE_FBL_3(pxSource),
                                 ((((unsigned short)(ADDRESS_HIGH_FBL_3(pxSource))) << 8) |
                                  ((unsigned short)(ADDRESS_LOW_FBL_3(pxSource)))));
    // дискретный сигнал - запрограммирован и достоверен?
    if (iDiscreteInputState != -1)
    {
        fbInputIsReliable = 1;
        // дискретный сигнал - "активен"?
        if (((INPUT_TYPE_FBL_3(pxSource)) &&
                (iDiscreteInputState)) ||
                (((INPUT_TYPE_FBL_3(pxSource)) == 0) &&
                 (iDiscreteInputState == 0)))
        {

        }
        else
        {
            // дискретный сигнал - "неактивен".
            // состояние выхода четырёхвходового элемента "И" - 0.
            ucCurrentState = 0;
        }
    }

//-----------------------------------------------------------------------------------------------------
    iDiscreteInputState =
        iFunctionBlockInputCheck(INPUT_SOURCE_FBL_4(pxSource),
                                 ((((unsigned short)(ADDRESS_HIGH_FBL_4(pxSource))) << 8) |
                                  ((unsigned short)(ADDRESS_LOW_FBL_4(pxSource)))));
    // дискретный сигнал - запрограммирован и достоверен?
    if (iDiscreteInputState != -1)
    {
        fbInputIsReliable = 1;
        // дискретный сигнал - "активен"?
        if (((INPUT_TYPE_FBL_4(pxSource)) &&
                (iDiscreteInputState)) ||
                (((INPUT_TYPE_FBL_4(pxSource)) == 0) &&
                 (iDiscreteInputState == 0)))
        {

        }
        else
        {
            // дискретный сигнал - "неактивен".
            // состояние выхода четырёхвходового элемента "И" - 0.
            ucCurrentState = 0;
        }
    }

    // дискретный сигнал - запрограммирован и достоверен?
    if (!(fbInputIsReliable))
    {
        // дискретный сигнал - незапрограммирован или недостоверен.
        // состояние выхода четырёхвходового элемента "И" - 0.
        ucCurrentState = 0;
    }
    return ucCurrentState;
}

//-----------------------------------------------------------------------------------------------------
// четырёхвходовый логический элемент "ИЛИ".
// получает указатель на описатель функционального блока в нормализованной базе данных.
// возвращает состояние выхода логического элемента.
unsigned char ucFourInputElementOr(TFunctionBlockDescriptionWork *pxSource)
{
    unsigned char ucCurrentState;
    int iDiscreteInputState;
    bool fbInputIsReliable;
    // начальное состояние выхода четырёхвходового элемента "ИЛИ" - 0.
    ucCurrentState = 0;
    fbInputIsReliable = 0;

    iDiscreteInputState =
        iFunctionBlockInputCheck(INPUT_SOURCE_FBL_1(pxSource),
                                 ((((unsigned short)(ADDRESS_HIGH_FBL_1(pxSource))) << 8) |
                                  ((unsigned short)(ADDRESS_LOW_FBL_1(pxSource)))));
    // дискретный сигнал - запрограммирован и достоверен?
    if (iDiscreteInputState != -1)
    {
        fbInputIsReliable = 1;
        // дискретный сигнал - "активен"?
        if (((INPUT_TYPE_FBL_1(pxSource)) &&
                (iDiscreteInputState)) ||
                (((INPUT_TYPE_FBL_1(pxSource)) == 0) &&
                 (iDiscreteInputState == 0)))
        {
            // дискретный сигнал - "активен".
            // состояние выхода четырёхвходового элемента "ИЛИ" - 1.
            ucCurrentState = 1;
        }
        else
        {

        }
    }

//-----------------------------------------------------------------------------------------------------
    iDiscreteInputState =
        iFunctionBlockInputCheck(INPUT_SOURCE_FBL_2(pxSource),
                                 ((((unsigned short)(ADDRESS_HIGH_FBL_2(pxSource))) << 8) |
                                  ((unsigned short)(ADDRESS_LOW_FBL_2(pxSource)))));
    // дискретный сигнал - запрограммирован и достоверен?
    if (iDiscreteInputState != -1)
    {
        fbInputIsReliable = 1;
        // дискретный сигнал - "активен"?
        if (((INPUT_TYPE_FBL_2(pxSource)) &&
                (iDiscreteInputState)) ||
                (((INPUT_TYPE_FBL_2(pxSource)) == 0) &&
                 (iDiscreteInputState == 0)))
        {
            // дискретный сигнал - "активен".
            // состояние выхода четырёхвходового элемента "ИЛИ" - 1.
            ucCurrentState = 1;
        }
        else
        {

        }
    }

//-----------------------------------------------------------------------------------------------------
    iDiscreteInputState =
        iFunctionBlockInputCheck(INPUT_SOURCE_FBL_3(pxSource),
                                 ((((unsigned short)(ADDRESS_HIGH_FBL_3(pxSource))) << 8) |
                                  ((unsigned short)(ADDRESS_LOW_FBL_3(pxSource)))));
    // дискретный сигнал - запрограммирован и достоверен?
    if (iDiscreteInputState != -1)
    {
        fbInputIsReliable = 1;
        // дискретный сигнал - "активен"?
        if (((INPUT_TYPE_FBL_3(pxSource)) &&
                (iDiscreteInputState)) ||
                (((INPUT_TYPE_FBL_3(pxSource)) == 0) &&
                 (iDiscreteInputState == 0)))
        {
            // дискретный сигнал - "активен".
            // состояние выхода четырёхвходового элемента "ИЛИ" - 1.
            ucCurrentState = 1;
        }
        else
        {

        }
    }

//-----------------------------------------------------------------------------------------------------
    iDiscreteInputState =
        iFunctionBlockInputCheck(INPUT_SOURCE_FBL_4(pxSource),
                                 ((((unsigned short)(ADDRESS_HIGH_FBL_4(pxSource))) << 8) |
                                  ((unsigned short)(ADDRESS_LOW_FBL_4(pxSource)))));
    // дискретный сигнал - запрограммирован и достоверен?
    if (iDiscreteInputState != -1)
    {
        fbInputIsReliable = 1;
        // дискретный сигнал - "активен"?
        if (((INPUT_TYPE_FBL_4(pxSource)) &&
                (iDiscreteInputState)) ||
                (((INPUT_TYPE_FBL_4(pxSource)) == 0) &&
                 (iDiscreteInputState == 0)))
        {
            // дискретный сигнал - "активен".
            // состояние выхода четырёхвходового элемента "ИЛИ" - 1.
            ucCurrentState = 1;
        }
        else
        {

        }
    }
    return ucCurrentState;
}





