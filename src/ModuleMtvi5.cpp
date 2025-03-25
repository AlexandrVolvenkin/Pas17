
#include "PasNewConfig.h"
#include "ModuleMtvi5.h"
#include "Events.h"

using namespace std;

void iModuleMtvi5DataHandler(TModuleContext *pxModuleContext)
{
    switch(pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState)
    {
    case MTVI5_FSM_STATE_TRANSFER:
        iModuleMtvi5DataExchange(pxModuleContext);
        break;

    case MTVI5_FSM_STATE_WRITE_REGULATOR_DATABASE:
        // передадим драйверу модуля команду - запись базы данных.
        pxModuleContext ->
        xModuleContextDinamic.
        ucCommandControl = DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_HMI;

        if (iModuleMtvi5RegulatorDataBaseBlockWrite(pxModuleContext) == 0)
        {

            iPlcDataBaseBlockWrite(pxModuleContext ->
                                   xModuleContextDinamic.
                                   ucCommonIndex);
//            vWorkArraysUpdate();

            // сообщим программатору, что ошибок нет, по 14 функции Modbus - запрос о результатах программирования.
            ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
            iPlcDataBaseBlockWrite(CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET +
                                   pxModuleContext ->
                                   xModuleContextStatic.
                                   ucInsideTypeIndex);
        }
        else
        {
            // запишем код ошибки, для передачи программатору, по 14 функции Modbus - запрос о результатах программирования.
            ucDataBaseError = PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_PSP_EEPROM_ERROR;
            // передадим драйверу модуля команду.
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = 0;
            cout << "CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY ERROR" << endl;
            // при записи в модуль произошла ошибка.
            // восстановим базу данных.
            iPlcDataBaseCheck();
        }

        vWorkArraysUpdate();

        pxModuleContext ->
        xModuleContextDinamic.
        ucCommonIndex = 0;

        // автомат переходит в режим - обмен данными с модулем.
        pxModuleContext ->
        xModuleContextDinamic.
        ucFsmState = MTVI5_FSM_STATE_TRANSFER;
        break;

    case MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_REGULATOR:
        // передадим драйверу модуля команду - запись базы данных.
        pxModuleContext ->
        xModuleContextDinamic.
        ucCommandControl = DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_PROGRAMMER;

        if (iModuleMtvi5RegulatorDataBaseBlockWrite(pxModuleContext) == 0)
        {
            // запустим задержку на 1.5 секунды, для записи в EEPROM модуля.
            pxModuleContext ->
            xModuleContextDinamic.
            nucLoopSkipCounter = 0;
            pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState = MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_PSP;
        }
        else
        {
            // запишем код ошибки, для передачи программатору, по 14 функции Modbus - запрос о результатах программирования.
            ucDataBaseError = PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_PSP_EEPROM_ERROR;
            // передадим драйверу модуля команду.
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = 0;
            cout << "MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_REGULATOR ERROR" << endl;
            // при записи в модуль произошла ошибка.
            // восстановим базу данных.
            iPlcDataBaseCheck();
            vWorkArraysUpdate();

            pxModuleContext ->
            xModuleContextDinamic.
            ucCommonIndex = 0;

            // автомат переходит в режим - обмен данными с модулем.
            pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState = MTVI5_FSM_STATE_TRANSFER;
        }
        break;

    case MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_PSP:
        // закончилась задержка на 1.5 секунды, для записи в EEPROM модуля?
        if ((pxModuleContext ->
                xModuleContextDinamic.
                nucLoopSkipCounter) == MTVI5_WRITE_WAIT_QUANTITY)
        {
            if (iModuleMtvi5PspDataBaseBlockWrite(pxModuleContext) == 0)
            {

                iPlcDataBaseBlockWrite(pxModuleContext ->
                                       xModuleContextDinamic.
                                       ucCommonIndex);
                //                vWorkArraysUpdate();

                // сообщим программатору, что ошибок нет, по 14 функции Modbus - запрос о результатах программирования.
                ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
                iPlcDataBaseBlockWrite(CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET +
                                       pxModuleContext ->
                                       xModuleContextStatic.
                                       ucInsideTypeIndex);
                // запустим задержку на 1.5 секунды, для записи в EEPROM модуля.
                pxModuleContext ->
                xModuleContextDinamic.
                nucLoopSkipCounter = 0;
                pxModuleContext ->
                xModuleContextDinamic.
                ucFsmState = MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_PSP_WRITE_WAITING;
            }
            else
            {
                // запишем код ошибки, для передачи программатору, по 14 функции Modbus - запрос о результатах программирования.
                ucDataBaseError = PROGRAMMING_DATA_BASE_READ_WRITE_BLOCK_PSP_EEPROM_ERROR;
                // передадим драйверу модуля команду.
                pxModuleContext ->
                xModuleContextDinamic.
                ucCommandControl = 0;
                // автомат переходит в режим - обмен данными с модулем.
                pxModuleContext ->
                xModuleContextDinamic.
                ucFsmState = MTVI5_FSM_STATE_TRANSFER;
                cout << "MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_PSP ERROR" << endl;
                // при записи в модуль произошла ошибка.
                // восстановим базу данных.
                iPlcDataBaseCheck();
            }

            vWorkArraysUpdate();

            pxModuleContext ->
            xModuleContextDinamic.
            ucCommonIndex = 0;

        }
        else
        {
            (pxModuleContext ->
             xModuleContextDinamic.
             nucLoopSkipCounter)++;
//            iModuleMtvi5DataExchange(pxModuleContext);
        }
        break;

    case MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_PSP_WRITE_WAITING:
        // закончилась задержка на 1.5 секунды, для записи в EEPROM модуля?
        if ((pxModuleContext ->
                xModuleContextDinamic.
                nucLoopSkipCounter) == MTVI5_WRITE_WAIT_QUANTITY)
        {
            // автомат переходит в режим - обмен данными с модулем.
            pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState = MTVI5_FSM_STATE_TRANSFER;
        }
        else
        {
            (pxModuleContext ->
             xModuleContextDinamic.
             nucLoopSkipCounter)++;
//            iModuleMtvi5DataExchange(pxModuleContext);
        }
        break;

    default:
        break;
    };
}

//-----------------------------------------------------------------------------------------------------
// производит обмен данными с модулями аналогового вывода.
int iModuleMtvi5DataExchange(TModuleContext *pxModuleContext)
{
    int i;
    float fData;
    float fResult;
    unsigned short usData;
    float fMinOfScale;
    float fMaxOfScale;
    unsigned char ucRegulatorMode;
    unsigned short usMaxCode;
    unsigned char nucRegulatorCounter;
    unsigned char nucFrameLength;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    TAnalogueInputDescriptionWork *pxAnalogueInputDescriptionWork;
    TAnalogueOutputRegulatorDescriptionWork *pxAnalogueOutputRegulatorDescriptionWork;
    TMtvi5ContAvData *pxMtvi5ContAvDataPointer;
    float *pfCalcValueOriginBufferPointer;
    // массив для преобразования данных регуляторов из формата
    // обработки в формат передачи.
    TMtvi5TransmitData axMtvi5TransmitData[MAX_HANDLED_ANALOGUE_OUTPUT];
    // массив для преобразования состояния регуляторов из формата
    // обработки в формат передачи.
    unsigned char aucMtvi5TransmitState[MAX_HANDLED_ANALOGUE_OUTPUT];
    unsigned char ucRegulatorNumber;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    float fTempResult;

//-----------------------------------------------------------------------------------------------------
//    // модуль аналогового вывода опрашивается раз в - (длительность основного цикла прибора 100mc * MTVI5_LOOP_SKIP_QUANTITY).
//    // прошло - (MTVI5_LOOP_SKIP_QUANTITY) циклов 100mc?
//    if ((pxModuleContext ->
//            xModuleContextDinamic.
//            nucLoopSkipCounter) == MTVI5_LOOP_SKIP_QUANTITY)
//    {
//        // обнулим делитель на - MTVI5_LOOP_SKIP_QUANTITY.
//        pxModuleContext ->
//        xModuleContextDinamic.
//        nucLoopSkipCounter = 0;
    // получим указатель на нормализованный блок базы данных описателей аналоговых входов.
    pxAnalogueInputDescriptionWork = (TAnalogueInputDescriptionWork*)(pxModuleContext ->
                                     xModuleContextStatic.
                                     pucAnalogueInputDescriptionWork);
    // получим указатель на блок базы данных описателей аналоговых выходов в общем формате.
    pxAnalogueOutputRegulatorDescriptionWork = (TAnalogueOutputRegulatorDescriptionWork*)(pxModuleContext ->
            xModuleContextStatic.
            pucDataBaseExtractPointer);
    // получим указатель на буфер с аналоговыми сигналами регуляторов.
    pxMtvi5ContAvDataPointer = (TMtvi5ContAvData*)(pxModuleContext ->
                               xModuleContextStatic.
                               pucContAvBufferPointer);
    nucRegulatorCounter = 0;
    // посчитаем количество запрограммированных регуляторов.
    // от их количества зависит длина пакета данных при обмене с модулем.
    for (i = 0; i < MTVI5_ANALOG_OUTPUT_QUANTITY; i++)
    {
        if (pxAnalogueOutputRegulatorDescriptionWork[i].ucPvAddress)
        {
            nucRegulatorCounter++;
        }
    }

    // количество запрограммированных регуляторов превосходит максимально возможное?
    if (nucRegulatorCounter > MAX_HANDLED_ANALOGUE_OUTPUT)
    {
        // ошибка.
        return -1;
    }

    // заполним пакет для обмена с модулем данными - Флаг изменения SP, OUT 1 байт.
    // есть номер регулятора, праметры которого изменяются?
    if (pxModuleContext ->
            xModuleContextDinamic.
            ucCommonIndex)
    {
        printf("iModuleMtvi5DataHandler ucModuleContextIndex %d\n\r", (pxModuleContext ->
                xModuleContextStatic.
                ucModuleContextIndex));
        printf("iModuleMtvi5DataHandler ucCommonIndex %d\n\r", (pxModuleContext ->
                xModuleContextDinamic.
                ucCommonIndex));
        printf("iModuleMtvi5DataHandler ucCommandControl %d\n\r", (pxModuleContext ->
                xModuleContextDinamic.
                ucCommandControl));
        // получим номер регулятора, праметры которого изменяются.
        ucRegulatorNumber = pxModuleContext ->
                            xModuleContextDinamic.
                            ucCommonIndex;
    }
    else
    {
        // праметры регуляторов не изменяются.
        ucRegulatorNumber = 0x00;
    }

//-----------------------------------------------------------------------------------------------------
    // сформируем данные регуляторов для передачи в модуль.
    for (i = 0; i < nucRegulatorCounter; i++)
    {
//-----------------------------------------------------------------------------------------------------

//            cout << "pucLogSostOriginBufferPointer" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)pxModuleContext ->
//                            xModuleContextStatic.
//                            pucLogSostOriginBufferPointer;
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }

//            cout << "pucCalcValueOriginBufferPointer" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)pxModuleContext ->
//                            xModuleContextStatic.
//                            pucCalcValueOriginBufferPointer;
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }

//-----------------------------------------------------------------------------------------------------
        // определим режим работы регулятора.
        // регулятор находится в автоматическом режиме?
        if ((pxModuleContext ->
                xModuleContextStatic.
                pucContStWriteBufferPointer[i]) &
                (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT))
        {
            // регулятор запрограммирован для работы в режиме - форсированное управление(fc)?
            if (pxAnalogueOutputRegulatorDescriptionWork[i].
                    ucForceControlFcOnAddress)
            {
                // от CodeSys есть команда - форсированное управление(fc)?
                if (pxModuleContext ->
                        xModuleContextStatic.
                        pucLogSostOriginBufferPointer[(pxAnalogueOutputRegulatorDescriptionWork[i].
                                                       ucForceControlFcOnAddress) -
                                                      CONVERT_NATURAL_NUMBER_TO_INTEGER])
                {
                    ucRegulatorMode = REGULATOR_MODE_AUTO_FORCE_CONTROL;
                    // получим номер регулятора, праметры которого изменяются.
                    ucRegulatorNumber = (i % MTVI5_REGULATOR_QUANTITY) +
                                        CONVERT_INTEGER_TO_NATURAL_NUMBER;
                }
                else
                {
                    ucRegulatorMode = REGULATOR_MODE_AUTO_NORMAL;
                }
            }
            // регулятор находится в режиме - прогаммный задатчик?
            else if  ((pxModuleContext ->
                       xModuleContextStatic.
                       pucContStWriteBufferPointer[i]) &
                      (0x01 << MTVI5_STAT_MODE_PROGRAMM_BIT))
            {
                ucRegulatorMode = REGULATOR_MODE_AUTO_PROGRAMM_SET_POINT;
                // получим номер регулятора, праметры которого изменяются.
                ucRegulatorNumber = (i % MTVI5_REGULATOR_QUANTITY) +
                                    CONVERT_INTEGER_TO_NATURAL_NUMBER;
            }
            else
            {
                ucRegulatorMode = REGULATOR_MODE_AUTO_NORMAL;
            }
        }
        else
        {
            ucRegulatorMode = REGULATOR_MODE_MANUAL;
        }

//-----------------------------------------------------------------------------------------------------
        switch(ucRegulatorMode)
        {
//-----------------------------------------------------------------------------------------------------
        case REGULATOR_MODE_MANUAL:
//                cout << "REGULATOR_MODE_MANUAL" << endl;
            // получим значения начала шкалы НШК из нормализованной базы данных описателей аналоговых входов.
            fMinOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                          ucPvAddress) -
                          CONVERT_NATURAL_NUMBER_TO_INTEGER].
                          fMinOfScale;
            // получим значения конца шкалы ВШК из нормализованной базы данных описателей аналоговых входов.
            fMaxOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                          ucPvAddress) -
                          CONVERT_NATURAL_NUMBER_TO_INTEGER].
                          fMaxOfScale;
            // параметр из расчётных значений CoDeSys?
            if ((pxAnalogueOutputRegulatorDescriptionWork[i].ucControl) &
                    (0x01 << REGULATOR_CONTROL_PV_SOURSE_IS_CALC_CDS))
            {
                // получим измеренное значение по адресу (ucPvAddress) из расчётных значений CoDeSys.
                memcpy((unsigned char*)&fData,
                       &(pxModuleContext ->
                         xModuleContextStatic.
                         pucCalcValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                 ucPvAddress) -
                                 CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                       sizeof(float));

                pxMtvi5ContAvDataPointer[i].fPv = fData;
                // преобразуем измеренное значение по адресу входа регулируемого параметра
                // в формат целочисленного кода.
                // преобразуем значение выхода OUT в шкалу 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
            }
            else
            {
                // измеренное значение по адресу входа регулируемого параметра недостоверно?
                if (pxModuleContext ->
                        xModuleContextStatic.
                        pucBadAiOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                                      ucPvAddress) -
                                                     CONVERT_NATURAL_NUMBER_TO_INTEGER)])
                {
                    // измеренное значение по адресу входа регулируемого параметра - недостоверно.
                    // сообщим об ошибке модулю.
                    usData = (unsigned short)MTVI5_PV_ERROR;
                }
                else
                {
                    // получим измеренное значение по адресу входа регулируемого параметра.
                    memcpy((unsigned char*)&fData,
                           &(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                                            ucPvAddress) -
                                                            CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                           sizeof(float));

                    pxMtvi5ContAvDataPointer[i].fPv = fData;
                    // преобразуем измеренное значение по адресу входа регулируемого параметра
                    // в формат целочисленного кода.
                    usData = (unsigned short)(((float)MTVI5_MAX_CODE) * ((fData - fMinOfScale) / (fMaxOfScale - fMinOfScale)));
                }
            }

            // поместим значение в массив для передачи в модуль.
            axMtvi5TransmitData[i].usPv = usData;

//-----------------------------------------------------------------------------------------------------
            // обработаем команду инкремент-декремент уставок - SP, OUT,
            // если поступила от функции vDeviceControlModbusFunction5Handler().
            switch(pxModuleContext ->
                    xModuleContextDinamic.
                    ucCommandControl)
            {
                // поступила команда инкремент уставок - SP, OUT.
            case DEVICE_CONTROL_MTVI5_INCREMENT_SP_OUT:
//                // сбросим команду.
//                pxModuleContext ->
//                xModuleContextDinamic.
//                ucCommandControl = 0;
                // регулятор находится в ручном режиме, инкрементируем значение выхода - OUT.
                fData = pxMtvi5ContAvDataPointer[i].fOut;
                // преобразуем значение выхода OUT в шкалу 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
                // увеличим значение на количество процентов в - usAuxiliaryCounter,
                // поступившее от функции vDeviceControlModbusFunction5Handler().
                usData += (unsigned short)((pxModuleContext ->
                                            xModuleContextDinamic.
                                            usAuxiliaryCounter) * MTVI5_MAX_CODE_ONE_PERCENT);
                // преобразуем значение OUT
                // из формата целочисленного кода, во float IEEE754.
                fData = (float)(((float)PERCENT_100) * ((float)usData / (float)MTVI5_MAX_CODE));
                // отправим в массив аналоговых переменных регуляторов «CONT_AV».
                pxMtvi5ContAvDataPointer[i].fOut = fData;
                break;
                // поступила команда декремент уставок - SP, OUT.
            case DEVICE_CONTROL_MTVI5_DECREMENT_SP_OUT:
//                // сбросим команду.
//                pxModuleContext ->
//                xModuleContextDinamic.
//                ucCommandControl = 0;
                // регулятор находится в ручном режиме, декрементируем значение выхода - OUT.
                fData = pxMtvi5ContAvDataPointer[i].fOut;
                // преобразуем значение выхода OUT в шкалу 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
                // уменьшим значение на количество процентов в - usAuxiliaryCounter,
                // поступившее от функции vDeviceControlModbusFunction5Handler().
                usData -= (unsigned short)((pxModuleContext ->
                                            xModuleContextDinamic.
                                            usAuxiliaryCounter) * MTVI5_MAX_CODE_ONE_PERCENT);
                // преобразуем значение OUT
                // из формата целочисленного кода, во float IEEE754.
                fData = (float)(((float)PERCENT_100) * ((float)usData / (float)MTVI5_MAX_CODE));
                // отправим в массив аналоговых переменных регуляторов «CONT_AV».
                pxMtvi5ContAvDataPointer[i].fOut = fData;
                break;
                // команда инкремент-декремент уставок - SP, OUT - не поступала.
            default:
                // регулятор находится в ручном режиме, отправим модулю значение выхода - OUT.
                fData = pxMtvi5ContAvDataPointer[i].fOut;
                // преобразуем значение выхода OUT в шкалу 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
                break;
            };
            // поместим значение в массив для передачи в модуль.
            axMtvi5TransmitData[i].usSpOut = usData;

//-----------------------------------------------------------------------------------------------------
            aucMtvi5TransmitState[i] = (pxModuleContext ->
                                        xModuleContextStatic.
                                        pucContStWriteBufferPointer[i]);

//                printf("index %d\n", (i)); // MTVI TempRepair.
//                printf("ucPvAddress %d\n", (pxAnalogueOutputRegulatorDescriptionWork[i].
//                                            ucPvAddress));
//                printf("fPv %f\n", (pxMtvi5ContAvDataPointer[i].fPv));
//                printf("fOut %f\n", (pxMtvi5ContAvDataPointer[i].fOut));
//                printf("fSp %f\n", (pxMtvi5ContAvDataPointer[i].fSp));
//                // printf("pxMtvi5ContAvDataPointer %d\n", (pxMtvi5ContAvDataPointer));
////                printf("ucModuleContextIndex %d\n", (pxModuleContext ->
////                                                     xModuleContextStatic.
////                                                     ucModuleContextIndex));
            break;
//-----------------------------------------------------------------------------------------------------
        case REGULATOR_MODE_AUTO_NORMAL:
//                cout << "REGULATOR_MODE_AUTO_NORMAL" << endl;
            // получим значения начала шкалы НШК из нормализованной базы данных описателей аналоговых входов.
            fMinOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                          ucPvAddress) -
                          CONVERT_NATURAL_NUMBER_TO_INTEGER].
                          fMinOfScale;
            // получим значения конца шкалы ВШК из нормализованной базы данных описателей аналоговых входов.
            fMaxOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                          ucPvAddress) -
                          CONVERT_NATURAL_NUMBER_TO_INTEGER].
                          fMaxOfScale;
            // параметр из расчётных значений CoDeSys?
            if ((pxAnalogueOutputRegulatorDescriptionWork[i].ucControl) &
                    (0x01 << REGULATOR_CONTROL_PV_SOURSE_IS_CALC_CDS))
            {
                // получим измеренное значение по адресу (ucPvAddress) из расчётных значений CoDeSys.
                memcpy((unsigned char*)&fData,
                       &(pxModuleContext ->
                         xModuleContextStatic.
                         pucCalcValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                 ucPvAddress) -
                                 CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                       sizeof(float));

                pxMtvi5ContAvDataPointer[i].fPv = fData;
                // преобразуем измеренное значение по адресу входа регулируемого параметра
                // в формат целочисленного кода.
                // преобразуем значение выхода OUT в шкалу 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
            }
            else
            {
                // измеренное значение по адресу входа регулируемого параметра недостоверно?
                if (pxModuleContext ->
                        xModuleContextStatic.
                        pucBadAiOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                                      ucPvAddress) -
                                                     CONVERT_NATURAL_NUMBER_TO_INTEGER)])
                {
                    // измеренное значение по адресу входа регулируемого параметра - недостоверно.
                    // сообщим об ошибке модулю.
                    usData = (unsigned short)MTVI5_PV_ERROR;
                }
                else
                {
                    // получим измеренное значение по адресу входа регулируемого параметра.
                    memcpy((unsigned char*)&fData,
                           &(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                                            ucPvAddress) -
                                                            CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                           sizeof(float));

                    pxMtvi5ContAvDataPointer[i].fPv = fData;
                    // преобразуем измеренное значение по адресу входа регулируемого параметра
                    // в формат целочисленного кода.
                    usData = (unsigned short)(((float)MTVI5_MAX_CODE) * ((fData - fMinOfScale) / (fMaxOfScale - fMinOfScale)));
                }
            }

            // поместим значение в массив для передачи в модуль.
            axMtvi5TransmitData[i].usPv = usData;

//-----------------------------------------------------------------------------------------------------
            // обработаем команду инкремент-декремент уставок - SP, OUT,
            // если поступила от функции vDeviceControlModbusFunction5Handler().
            switch(pxModuleContext ->
                    xModuleContextDinamic.
                    ucCommandControl)
            {
                // поступила команда инкремент уставок - SP, OUT.
            case DEVICE_CONTROL_MTVI5_INCREMENT_SP_OUT:
//                // сбросим команду.
//                pxModuleContext ->
//                xModuleContextDinamic.
//                ucCommandControl = 0;
                // регулятор находится в автоматическом режиме, инкрементируем уставку - SP.
                fData = pxMtvi5ContAvDataPointer[i].fSp;
                // преобразуем значение уставки SP в формат целочисленного кода.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * ((fData - fMinOfScale) / (fMaxOfScale - fMinOfScale)));
                // увеличим значение на количество процентов в - usAuxiliaryCounter,
                // поступившее от функции vDeviceControlModbusFunction5Handler().
                usData += (unsigned short)((pxModuleContext ->
                                            xModuleContextDinamic.
                                            usAuxiliaryCounter) * MTVI5_MAX_CODE_ONE_PERCENT);
                // преобразуем значение SP
                // из формата целочисленного кода, во float IEEE754.
                fData = (float)((((float)usData * (fMaxOfScale - fMinOfScale)) / (float)MTVI5_MAX_CODE) + fMinOfScale);
                // отправим в массив аналоговых переменных регуляторов «CONT_AV».
                pxMtvi5ContAvDataPointer[i].fSp = fData;
                break;
                // поступила команда декремент уставок - SP, OUT.
            case DEVICE_CONTROL_MTVI5_DECREMENT_SP_OUT:
//                // сбросим команду.
//                pxModuleContext ->
//                xModuleContextDinamic.
//                ucCommandControl = 0;
                // регулятор находится в автоматическом режиме, декрементируем уставку - SP.
                fData = pxMtvi5ContAvDataPointer[i].fSp;
                // преобразуем значение уставки SP в формат целочисленного кода.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * ((fData - fMinOfScale) / (fMaxOfScale - fMinOfScale)));
                // уменьшим значение на количество процентов в - usAuxiliaryCounter,
                // поступившее от функции vDeviceControlModbusFunction5Handler().
                usData -= (unsigned short)((pxModuleContext ->
                                            xModuleContextDinamic.
                                            usAuxiliaryCounter) * MTVI5_MAX_CODE_ONE_PERCENT);
                // преобразуем значение SP
                // из формата целочисленного кода, во float IEEE754.
                fData = (float)((((float)usData * (fMaxOfScale - fMinOfScale)) / (float)MTVI5_MAX_CODE) + fMinOfScale);
                // отправим в массив аналоговых переменных регуляторов «CONT_AV».
                pxMtvi5ContAvDataPointer[i].fSp = fData;
                break;
                // команда инкремент-декремент уставок - SP, OUT - не поступала.
            default:
                // регулятор находится в автоматическом режиме, отправим модулю значение SP.
                fData = pxMtvi5ContAvDataPointer[i].fSp;//34;//
                // преобразуем значение уставки SP в формат целочисленного кода.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * ((fData - fMinOfScale) / (fMaxOfScale - fMinOfScale)));
                break;
            };

            // поместим значение в массив для передачи в модуль.
            axMtvi5TransmitData[i].usSpOut = usData;

//-----------------------------------------------------------------------------------------------------
            aucMtvi5TransmitState[i] = (pxModuleContext ->
                                        xModuleContextStatic.
                                        pucContStWriteBufferPointer[i]);
            break;

//-----------------------------------------------------------------------------------------------------
        case REGULATOR_MODE_AUTO_PROGRAMM_SET_POINT:
//                cout << "REGULATOR_MODE_AUTO_PROGRAMM_SET_POINT" << endl;
            // получим значения начала шкалы НШК из нормализованной базы данных описателей аналоговых входов.
            fMinOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                          ucPvAddress) -
                          CONVERT_NATURAL_NUMBER_TO_INTEGER].
                          fMinOfScale;
            // получим значения конца шкалы ВШК из нормализованной базы данных описателей аналоговых входов.
            fMaxOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                          ucPvAddress) -
                          CONVERT_NATURAL_NUMBER_TO_INTEGER].
                          fMaxOfScale;
            // параметр из расчётных значений CoDeSys?
            if ((pxAnalogueOutputRegulatorDescriptionWork[i].ucControl) &
                    (0x01 << REGULATOR_CONTROL_PV_SOURSE_IS_CALC_CDS))
            {
                // получим измеренное значение по адресу (ucPvAddress) из расчётных значений CoDeSys.
                memcpy((unsigned char*)&fData,
                       &(pxModuleContext ->
                         xModuleContextStatic.
                         pucCalcValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                 ucPvAddress) -
                                 CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                       sizeof(float));

                pxMtvi5ContAvDataPointer[i].fPv = fData;
                // преобразуем измеренное значение по адресу входа регулируемого параметра
                // в формат целочисленного кода.
                // преобразуем значение выхода OUT в шкалу 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
            }
            else
            {
                // измеренное значение по адресу входа регулируемого параметра недостоверно?
                if (pxModuleContext ->
                        xModuleContextStatic.
                        pucBadAiOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                                      ucPvAddress) -
                                                     CONVERT_NATURAL_NUMBER_TO_INTEGER)])
                {
                    // измеренное значение по адресу входа регулируемого параметра - недостоверно.
                    // сообщим об ошибке модулю.
                    usData = (unsigned short)MTVI5_PV_ERROR;
                }
                else
                {
                    // получим измеренное значение по адресу входа регулируемого параметра.
                    memcpy((unsigned char*)&fData,
                           &(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                                            ucPvAddress) -
                                                            CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                           sizeof(float));

                    pxMtvi5ContAvDataPointer[i].fPv = fData;
                    // преобразуем измеренное значение по адресу входа регулируемого параметра
                    // в формат целочисленного кода.
                    usData = (unsigned short)(((float)MTVI5_MAX_CODE) * ((fData - fMinOfScale) / (fMaxOfScale - fMinOfScale)));
                }
            }

            // поместим значение в массив для передачи в модуль.
            axMtvi5TransmitData[i].usPv = usData;

//-----------------------------------------------------------------------------------------------------
            // регулятор находится в режиме - прогаммный задатчик.
            // отправим модулю значение SP полученное от CodeSys.
            // получим значение уставки SP по адресу (ucProgrammSPAddress) из расчётных значений CoDeSys.
            memcpy((unsigned char*)&fData,
                   &(pxModuleContext ->
                     xModuleContextStatic.
                     pucCalcValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                             ucProgrammSPAddress) -
                             CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                   sizeof(float));
            // преобразуем значение уставки SP в формат целочисленного кода.
            // преобразуем значение выхода OUT в шкалу 0-100%.
            usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
            // поместим значение уставки SP в массив для передачи в модуль.
            axMtvi5TransmitData[i].usSpOut = usData;
            // отправим в массив аналоговых переменных регуляторов «CONT_AV».
            pxMtvi5ContAvDataPointer[i].fSp = fData;

//-----------------------------------------------------------------------------------------------------
            aucMtvi5TransmitState[i] = (pxModuleContext ->
                                        xModuleContextStatic.
                                        pucContStWriteBufferPointer[i]);
            break;

//-----------------------------------------------------------------------------------------------------
        case REGULATOR_MODE_AUTO_FORCE_CONTROL:
//                cout << "REGULATOR_MODE_AUTO_FORCE_CONTROL" << endl;
            // получим значения начала шкалы НШК из нормализованной базы данных описателей аналоговых входов.
            fMinOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                          ucPvAddress) -
                          CONVERT_NATURAL_NUMBER_TO_INTEGER].
                          fMinOfScale;
            // получим значения конца шкалы ВШК из нормализованной базы данных описателей аналоговых входов.
            fMaxOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                          ucPvAddress) -
                          CONVERT_NATURAL_NUMBER_TO_INTEGER].
                          fMaxOfScale;
            // параметр из расчётных значений CoDeSys?
            if ((pxAnalogueOutputRegulatorDescriptionWork[i].ucControl) &
                    (0x01 << REGULATOR_CONTROL_PV_SOURSE_IS_CALC_CDS))
            {
                // получим измеренное значение по адресу (ucPvAddress) из расчётных значений CoDeSys.
                memcpy((unsigned char*)&fData,
                       &(pxModuleContext ->
                         xModuleContextStatic.
                         pucCalcValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                 ucPvAddress) -
                                 CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                       sizeof(float));

                pxMtvi5ContAvDataPointer[i].fPv = fData;
                // преобразуем измеренное значение по адресу входа регулируемого параметра
                // в формат целочисленного кода.
                // преобразуем значение выхода OUT в шкалу 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
            }
            else
            {
                // измеренное значение по адресу входа регулируемого параметра недостоверно?
                if (pxModuleContext ->
                        xModuleContextStatic.
                        pucBadAiOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                                      ucPvAddress) -
                                                     CONVERT_NATURAL_NUMBER_TO_INTEGER)])
                {
                    // измеренное значение по адресу входа регулируемого параметра - недостоверно.
                    // сообщим об ошибке модулю.
                    usData = (unsigned short)MTVI5_PV_ERROR;
                }
                else
                {
                    // получим измеренное значение по адресу входа регулируемого параметра.
                    memcpy((unsigned char*)&fData,
                           &(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                                            ucPvAddress) -
                                                            CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                           sizeof(float));

                    pxMtvi5ContAvDataPointer[i].fPv = fData;
                    // преобразуем измеренное значение по адресу входа регулируемого параметра
                    // в формат целочисленного кода.
                    usData = (unsigned short)(((float)MTVI5_MAX_CODE) * ((fData - fMinOfScale) / (fMaxOfScale - fMinOfScale)));
                }
            }

            // поместим значение в массив для передачи в модуль.
            axMtvi5TransmitData[i].usPv = usData;

//-----------------------------------------------------------------------------------------------------
            // от CodeSys есть команда - автоматический режим?
            if (pxModuleContext ->
                    xModuleContextStatic.
                    pucLogSostOriginBufferPointer[(pxAnalogueOutputRegulatorDescriptionWork[i].
                                                   ucForceControlHaOnAddress) -
                                                  CONVERT_NATURAL_NUMBER_TO_INTEGER])
            {
                // регулятор находится в автоматическом режиме - форсированного управления – fc.
                // отправим модулю значение SP полученное от CodeSys.
                // получим значение уставки SP по адресу (ucForceControlSpOutAddress) из расчётных значений CoDeSys.
                memcpy((unsigned char*)&fData,
                       &(pxModuleContext ->
                         xModuleContextStatic.
                         pucCalcValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                 ucForceControlSpOutAddress) -
                                 CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                       sizeof(float));
                // преобразуем значение уставки SP в формат целочисленного кода.
                // преобразуем значение выхода OUT в шкалу 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
                // поместим значение уставки SP в массив для передачи в модуль.
                axMtvi5TransmitData[i].usSpOut = usData;
                // отправим в массив аналоговых переменных регуляторов «CONT_AV».
                pxMtvi5ContAvDataPointer[i].fSp = fData;
            }
            else
            {
                // регулятор находится в автоматическом режиме - форсированного управления – fc.
                // отправим модулю значение SP полученное от CodeSys.
                // получим значение уставки SP по адресу (ucForceControlSpOutAddress) из расчётных значений CoDeSys.
                memcpy((unsigned char*)&fData,
                       &(pxModuleContext ->
                         xModuleContextStatic.
                         pucCalcValueOriginBufferPointer[((pxAnalogueOutputRegulatorDescriptionWork[i].
                                 ucForceControlSpOutAddress) -
                                 CONVERT_NATURAL_NUMBER_TO_INTEGER) * sizeof(float)]),
                       sizeof(float));
                // преобразуем значение уставки SP в формат целочисленного кода.
                // преобразуем значение выхода OUT в шкалу 0-100%.
                usData = (unsigned short)(((float)MTVI5_MAX_CODE) * (fData / (float)PERCENT_100));
                // поместим значение уставки SP в массив для передачи в модуль.
                axMtvi5TransmitData[i].usSpOut = usData;
                // отправим в массив аналоговых переменных регуляторов «CONT_AV».
                pxMtvi5ContAvDataPointer[i].fOut = fData;
            }

//-----------------------------------------------------------------------------------------------------
            // от CodeSys есть команда - автоматический режим?
            if (pxModuleContext ->
                    xModuleContextStatic.
                    pucLogSostOriginBufferPointer[(pxAnalogueOutputRegulatorDescriptionWork[i].
                                                   ucForceControlHaOnAddress) -
                                                  CONVERT_NATURAL_NUMBER_TO_INTEGER])
            {
                // установим флаг - автоматический режим.
                aucMtvi5TransmitState[i] = ((pxModuleContext ->
                                             xModuleContextStatic.
                                             pucContStWriteBufferPointer[i]) |
                                            (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT));
            }
            else
            {
                // установим флаг - ручной режим.
                aucMtvi5TransmitState[i] = ((pxModuleContext ->
                                             xModuleContextStatic.
                                             pucContStWriteBufferPointer[i]) &
                                            (~(0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT)));
            }

            break;

//-----------------------------------------------------------------------------------------------------
        default:
            break;
        };
    }

//-----------------------------------------------------------------------------------------------------
    pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET];
    // вычислим длину пакета для обмена данными с модулем.
    nucFrameLength = ((MTVI5_ONE_REGULATOR_FRAME_BYTE_QUANTITY * nucRegulatorCounter) +
                      MTVI5_SP_OUT_SET_BYTE_QUANTITY);
    // сообщим модулю длину пакета для обмена данными.
    *pucDestination++ = nucFrameLength;

    // заполним пакет для обмена данными с модулем значениями Pv
    // в формате целочисленного кода.
    for (i = 0; i < nucRegulatorCounter; i++)
    {
        *pucDestination++ = (unsigned char)axMtvi5TransmitData[i].usPv;
        *pucDestination++ = (unsigned char)((axMtvi5TransmitData[i].usPv) >> 8);
    }
    // заполним пакет для обмена данными с модулем значениями SpOut
    // в формате целочисленного кода.
    for (i = 0; i < nucRegulatorCounter; i++)
    {
        *pucDestination++ = (unsigned char)axMtvi5TransmitData[i].usSpOut;
        *pucDestination++ = (unsigned char)((axMtvi5TransmitData[i].usSpOut) >> 8);
    }

    // заполним пакет для обмена с модулем данными состояния регуляторов.
    for (i = 0; i < nucRegulatorCounter; i++)
    {
        *pucDestination++ = (aucMtvi5TransmitState[i]);
    }

    // заполним пакет для обмена с модулем данными - Флаг изменения SP, OUT 1 байт.
    // получим номер регулятора, праметры которого изменяются.
    *pucDestination++ = ucRegulatorNumber;

    // получим указатель на начало пакета обмена данными с модулем
    // для вычисления CRC(байт с длиной пакета не входит в вычисляемую CRC).
    pucSource = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET + MTVI5_FRAME_LENGTH_BYTE_QUANTITY];
    usData = usCrcSummTwoByteCalculation(pucSource,
                                         nucFrameLength);
    *pucDestination++ = (unsigned char)usData;
    *pucDestination++ = (unsigned char)(usData >> 8);
    *pucDestination++ = DATA_EXCHANGE_OK;

//-----------------------------------------------------------------------------------------------------
    auiSpiTxBuffer[0] = MTVI5_DATA_EXCHANGE_COMMAND;



//        cout << "MTVI5_DATA_EXCHANGE_COMMAND auiSpiTxBuffer[0]" << endl;
////        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)&auiSpiTxBuffer[0];
//        for(int i=0; i<32; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }


//        usleep(100);
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MTVI5_FRAME_LENGTH_BYTE_QUANTITY +
                                     nucFrameLength +
                                     TWO_BYTE_CRC_LENGTH +
                                     TAIL_ANSWER_LENGTH,
                                     MTVI5_EXCHANGE_SPEED_IN_HZ
                                    );


//                printf("iModuleMtvi5DataHandler ucModuleContextIndex %d\n\r", (pxModuleContext ->
//                        xModuleContextStatic.
//                        ucModuleContextIndex));
//
//            cout << "auiSpiTxBuffer[0]" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiTxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
//
//            cout << "auiSpiRxBuffer[0]" << endl;
////            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }

//static uint8_t ui8Counter = 0;
//
//    if (ui8Counter < 30)
//    {
//        ui8Counter++;
//        auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] = MTVI5_DAC_LINK_ERROR;
//    }
//    else
//    {
////        ui8Counter = 0;
//    }
//    auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] = 45;

//                printf("iModuleMtvi5DataExchange usAuxiliaryCounter %d\n\r", (ui8Counter));
//-----------------------------------------------------------------------------------------------------
    // что ответил модуль?
    switch(auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET])
    {
        // нормальный обмен данными.
    case MTVI5_DATA_EXCHANGE_COMMAND:

//            cout << "MTVI5_DATA_EXCHANGE_COMMAND command" << endl;

        // данные не повреждены?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + MTVI5_FRAME_LENGTH_BYTE_QUANTITY],
                                   nucFrameLength) > 0)
        {
//            cout << "auiSpiTxBuffer[0]" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiTxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
//
//            cout << "auiSpiRxBuffer[0]" << endl;
////            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }

            // модуль исправен.
            pxModuleContext ->
            xModuleContextDinamic.
            nucBadAnswerCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT;
            // модуль исправен.
            pxModuleContext ->
            xModuleContextDinamic.
            usAuxiliaryCounter = 0;

            // сбросим флаг отказа модуля.
            *(pxModuleContext ->
              xModuleContextStatic.
              pucModuleBadStateBufferPointer) = BAD_MODULE_RESPONDED_OK;

//                printf("iModuleMtvi5DataExchange MTVI5_DATA_EXCHANGE_COMMAND %d\n\r", (MTVI5_BAD_ANSWER_ERROR_OFFSET));
            // неактивное состояние события ещё не зарегистрировано?
            if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        MTVI5_BAD_ANSWER_ERROR_OFFSET))
            {
                // зарегистрируем неактивное состояние события.
                // норма обмена данными.
                CEvents::EventRegistration(
                    pxModuleContext ->
                    xModuleContextStatic.
                    ucModuleContextIndex,
                    (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                    MTVI5_BAD_ANSWER_ERROR_OFFSET,
                    "Норм. обмена");
            }

            // активное состояние события ещё не зарегистрировано?
            if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        MTVI5_DAC_LINK_ERROR_OFFSET))
            {
                // зарегистрируем активное состояние события.
                // ошибка цепочки ЦАП.
                CEvents::EventRegistration(
                    pxModuleContext ->
                    xModuleContextStatic.
                    ucModuleContextIndex,
                    (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                    MTVI5_DAC_LINK_ERROR_OFFSET,
                    "Норм. ц. ЦАП");
            }

            // данные модуля достоверны.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucBadAoBufferPointer,
                   0,
                   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity);
            //iBadModuleBuffUpdate();
            // получим указатель на начало пакета обмена данными с модулем.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET];

            // пропустим байт с длиной пакета(при приёме данных от модуля не используется).
            pucSource++;

// в драйвере модуля токового вывода, данные уставок - SP, OUT,
// поступают из рабочего массива axAnalogueOutputRegulatorContAv[смещение на номер регулятора],
// после изменения туда же и возвращаются. из этого же массива, по протоколу Modbus,
// устройства верхнего уровня(HMI панели, системы SCADA, программаторы) получают данные для отображения на дисплее.
//  на текущем шаге обмен данными драйвера с модулем по SPI интерфейсу проходит в режиме одновременной записи-чтения.
// после записи новые данные на выходе модуля появятся только на следующем шаге записи-чтения.
// и если на текущем шаге данные с выхода модуля просто вернуть обратно, то никаких изменений не произойдёт.
//  поэтому, после записи в модуль значения уставки, изменённого на величину в переменной usAuxiliaryCounter(выражается в процентах),
// оно же отправляется в массив источник.
// а полученные от модуля данные на текущем шаге игнорируются.
//  так же, это удобно для корректного отображения устройствами верхнего уровня, изменённых данных.
// так как вызов драйвера модуля токового вывода происходит один раз в секунду,
// а функци vDeviceControlModbusMessageHandler(), максимум - время обработки самого медленного модуля,
// то за это время, оператор нажатием кнопки мыши, может инкрементировать-декрементировать значение уставки
// N - раз. значение - N помещается в переменную usAuxiliaryCounter. в конце каждого цикла обмена данными
// драйвера с модулем, переменная usAuxiliaryCounter - обнуляется.

//-----------------------------------------------------------------------------------------------------
            // получим от модуля и обновим значения SP и OUT в массиве аналоговых переменных регуляторов «CONT_AV».
            // есть команда - изменения аналоговых переменных регуляторов «CONT_AV»?
//            if ((pxModuleContext ->
//                    xModuleContextDinamic.
//                    ucCommandControl) ==
//                    (DEVICE_CONTROL_MTVI5_CONT_AV_WRITE |
//                     DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_HMI |
//                     DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_PROGRAMMER))
            if ((pxModuleContext ->
                    xModuleContextDinamic.
                    ucCommandControl) != 0)
            {
//                // сбросим команду.
//                pxModuleContext ->
//                xModuleContextDinamic.
//                ucCommandControl = 0;
                // праметры регуляторов OUT в массиве аналоговых переменных регуляторов «CONT_AV» не изменяются.
                for (i = 0; i < nucRegulatorCounter; i++)
                {
                    // перейдём к следующим двухбайтным данным.
                    pucSource += 2;
                }
                // праметры регуляторов SP в массиве аналоговых переменных регуляторов «CONT_AV» не изменяются.
                for (i = 0; i < nucRegulatorCounter; i++)
                {
                    // перейдём к следующим двухбайтным данным.
                    pucSource += 2;
                }
            }
            else
            {
                // получим от модуля и обновим значения OUT в массиве аналоговых переменных регуляторов «CONT_AV».
                for (i = 0; i < nucRegulatorCounter; i++)
                {
                    // получим значение OUT от модуля.
                    usData = (unsigned short)(*pucSource++);
                    usData |= ((unsigned short)(*pucSource++) << 8);
                    // преобразуем значение OUT
                    // из формата целочисленного кода, во float IEEE754.
                    fData = (float)(((float)PERCENT_100) * ((float)usData / (float)MTVI5_MAX_CODE));
                    // отправим в массив аналоговых переменных регуляторов «CONT_AV».
                    pxMtvi5ContAvDataPointer[i].fOut = fData;
                }
                // получим от модуля и обновим значения SP в массиве аналоговых переменных регуляторов «CONT_AV».
                for (i = 0; i < nucRegulatorCounter; i++)
                {
                    // получим значения начала шкалы НШК из нормализованной базы данных описателей аналоговых входов.
                    fMinOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                                  ucPvAddress) -
                                  CONVERT_NATURAL_NUMBER_TO_INTEGER].
                                  fMinOfScale;
                    // получим значения конца шкалы ВШК из нормализованной базы данных описателей аналоговых входов.
                    fMaxOfScale = pxAnalogueInputDescriptionWork[(pxAnalogueOutputRegulatorDescriptionWork[i].
                                  ucPvAddress) -
                                  CONVERT_NATURAL_NUMBER_TO_INTEGER].
                                  fMaxOfScale;
                    // получим значение SP от модуля.
                    usData = (unsigned short)(*pucSource++);
                    usData |= ((unsigned short)(*pucSource++) << 8);
                    // преобразуем значение SP
                    // из формата целочисленного кода, во float IEEE754.
                    fData = (float)((((float)usData * (fMaxOfScale - fMinOfScale)) / (float)MTVI5_MAX_CODE) + fMinOfScale);
                    // отправим в массив аналоговых переменных регуляторов «CONT_AV».
                    pxMtvi5ContAvDataPointer[i].fSp = fData;
                }
            }

            // получим от модуля байты состояния регуляторов - STAT:
            // есть команда - изменения аналоговых переменных регуляторов «CONT_AV»?
//            if ((pxModuleContext ->
//                    xModuleContextDinamic.
//                    ucCommandControl) ==
//                    (DEVICE_CONTROL_MTVI5_CONT_AV_WRITE |
//                     DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_HMI |
//                     DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_PROGRAMMER))
            if ((pxModuleContext ->
                    xModuleContextDinamic.
                    ucCommandControl) != 0)
            {
//                // сбросим команду.
//                pxModuleContext ->
//                xModuleContextDinamic.
//                ucCommandControl = 0;
                // получим от модуля байты состояния регуляторов - STAT:
                for (i = 0; i < nucRegulatorCounter; i++)
                {
                    uint8_t ui8Data;
                    ui8Data = *pucSource++;

//                    // проверим режим работы.
//                    // режим ручной-авто изменился?
//                    if ((ui8Data & (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT)) !=
//                            (pxModuleContext ->
//                             xModuleContextStatic.
//                             pucContStWriteBufferPointer[i]) |=
//                                (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT))
//                    {
//                        // проверим режим работы.
//                        // от модуля есть команда - автоматический режим?
//                        if (ui8Data & (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT))
//                        {
//                            // установим флаг - автоматический режим.
//                            (pxModuleContext ->
//                             xModuleContextStatic.
//                             pucContStWriteBufferPointer[i]) |=
//                                 (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT);
//                        }
//                        else
//                        {
//                            // установим флаг - ручной режим.
//                            (pxModuleContext ->
//                             xModuleContextStatic.
//                             pucContStWriteBufferPointer[i]) &=
//                                 (~(0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT));
//                        }
//                    }

                    // проверим режим работы.
                    // от модуля есть команда - автоматический режим?
                    if (ui8Data & (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT))
                    {
                        // установим флаг - автоматический режим.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucContStWriteBufferPointer[i]) |=
                             (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT);
                    }
                    else
                    {
                        // установим флаг - ручной режим.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucContStWriteBufferPointer[i]) &=
                             (~(0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT));
                    }
                }
            }
            else
            {
                // получим от модуля байты состояния регуляторов - STAT:
                for (i = 0; i < nucRegulatorCounter; i++)
                {
                    uint8_t ui8Data;
                    ui8Data = *pucSource++;
                    //
                    // получим от модуля байт состояния регуляторов - STAT:
                    // D0-Р/А; D1-АП/не АП; D2,3-сост.2-дискр.; D4-6-резерв; D7-АВАР/НОРМ,
                    // и отправим его в массив состояния регуляторов «CONT_ST».
                    (pxModuleContext ->
                     xModuleContextStatic.
                     pucContStBufferPointer[i]) = ui8Data;

                    // проверим режим работы.
                    // режим ручной-авто изменился?
                    if ((ui8Data & (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT)) !=
                            ((pxModuleContext ->
                              xModuleContextStatic.
                              pucContStWriteBufferPointer[i]) &
                             (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT)))
                    {
//                        // отправим команду - изменение состояния ручной-автомат.
//                        pxModuleContext ->
//                        xModuleContextDinamic.
//                        ucCommandControl = DEVICE_CONTROL_MTVI5_CONT_ST_CHANGE;
//    printf("iModuleMtvi5DataExchange ui8Data %d\n\r", (ui8Data & (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT)));
//    printf("iModuleMtvi5DataExchange pucContStWriteBufferPointer %d\n\r", ((pxModuleContext ->
//                             xModuleContextStatic.
//                             pucContStWriteBufferPointer[i]) &
//                            (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT)));
//                        // проверим режим работы.
//                        // от модуля есть команда - автоматический режим?
//                        if (ui8Data & (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT))
//                        {
//                            // установим флаг - автоматический режим.
//                            (pxModuleContext ->
//                             xModuleContextStatic.
//                             pucContStWriteBufferPointer[i]) |=
//                                 (0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT);
//                        }
//                        else
//                        {
//                            // установим флаг - ручной режим.
//                            (pxModuleContext ->
//                             xModuleContextStatic.
//                             pucContStWriteBufferPointer[i]) &=
//                                 (~(0x01 << MTVI5_STAT_AUTO_OR_MANUAL_BIT));
//                        }
                    }

                    // есть неисправность ЦАП текущего выхода?
                    if (ui8Data & (0x01 << MTVI5_STAT_DAC_ERROR_BIT))
                    {
//                        // модуль признан неисправным.
//                        *(pxModuleContext ->
//                          xModuleContextStatic.
//                          pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;
//                        // получим код ошибки;
//                        (pxModuleContext ->
//                         xModuleContextDinamic.
//                         ucErrorCode) = MTVI5_OUT_DAC_ERROR;
                        // установим флаг недостоверности - выход недостоверен.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucBadAoBufferPointer[i] = 1;
                        // активное состояние события ещё не зарегистрировано?
                        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                                    pxModuleContext ->
                                    xModuleContextStatic.
                                    ucModuleContextIndex,
                                    MTVI5_OUT_DAC_ERROR_OFFSET + i))
                        {
                            // зарегистрируем активное состояние события.
                            // ошибка ЦАП выхода по идексу - i.
                            CEvents::EventRegistration(
                                pxModuleContext ->
                                xModuleContextStatic.
                                ucModuleContextIndex,
                                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                                 CEvents::HANDLED_EVENTS_IS_POPUP |
                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                                MTVI5_OUT_DAC_ERROR_OFFSET + i,
                                "Ошибка ЦАП");
                        }
                    }
                    else
                    {
                        // сбросим флаг недостоверности - выход достоверен.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucBadAoBufferPointer[i] = 0;
                        // неактивное состояние события ещё не зарегистрировано?
                        if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
                                    pxModuleContext ->
                                    xModuleContextStatic.
                                    ucModuleContextIndex,
                                    MTVI5_OUT_DAC_ERROR_OFFSET + i))
                        {
                            // зарегистрируем неактивное состояние события.
                            // норма ЦАП выхода по идексу - i.
                            CEvents::EventRegistration(
                                pxModuleContext ->
                                xModuleContextStatic.
                                ucModuleContextIndex,
                                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                                MTVI5_OUT_DAC_ERROR_OFFSET + i,
                                "Норма ЦАП");
                        }
                    }
                }
            }

            // сбросим номер регулятора.
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommonIndex = 0;
            // сбросим команду.
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = 0;
            // сбросим счётчик кликов мыши.
            // на значение в переменной usAuxiliaryCounter инкрементируется-декрементируется
            // значение уставок SP, OUT.
            pxModuleContext ->
            xModuleContextDinamic.
            usAuxiliaryCounter = 0;

////            cout << "pucContStBufferPointer" << endl;
////            unsigned char *pucSourceTemp;
////            pucSourceTemp = (unsigned char*)pxModuleContext ->
////                            xModuleContextStatic.
////                            pucContStBufferPointer;
//            cout << "pucContStBufferPointer" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)aucContStByteArray;
//            for(int i=0; i<12; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
        }
        else
        {
            cout << "MTVI5_DATA_EXCHANGE_COMMAND crc error" << endl;
            printf("iModuleMtvi5DataHandler ucModuleContextIndex %d\n\r", (pxModuleContext ->
                    xModuleContextStatic.
                    ucModuleContextIndex));



//            cout << "auiSpiTxBuffer[0]" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiTxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
//
//            cout << "auiSpiRxBuffer[0]" << endl;
////            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }


            // модуль признан неисправным?
            if ((pxModuleContext ->
                    xModuleContextDinamic.
                    nucBadAnswerCounter) == 0)
            {
                // модуль признан неисправным.
                *(pxModuleContext ->
                  xModuleContextStatic.
                  pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;

                // модуль не отвечает в течении 60 секунд?
                if ((pxModuleContext ->
                        xModuleContextDinamic.
                        usAuxiliaryCounter) >= BAD_MODULE_60_SECOND_CYCLE_COUNT_DEFAULT)
                {
                    // отправим команду модулю МТВИ5 - перезагрузка больше чем через одну минуту.
                    // выставить OSF и перейти в режим «РУЧНОЙ».
                    pxModuleContext ->
                    xModuleContextDinamic.
                    ucCommandControl = DEVICE_CONTROL_MTVI5_LOAD_OUTS_OSF;
                }
                else
                {
                    // уменьшим значение счётчика - "количество сеансов связи с модулем без ответа".
                    (pxModuleContext ->
                     xModuleContextDinamic.
                     usAuxiliaryCounter)++;
                }

                // получим код ошибки;
                (pxModuleContext ->
                 xModuleContextDinamic.
                 ucErrorCode) = DATA_EXCHANGE_CRC_ERROR;

                // активное состояние события ещё не зарегистрировано?
                if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                            pxModuleContext ->
                            xModuleContextStatic.
                            ucModuleContextIndex,
                            MTVI5_BAD_ANSWER_ERROR_OFFSET))
                {
                    // зарегистрируем активное состояние события.
                    // ошибка обмена данными.
                    CEvents::EventRegistration(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                         CEvents::HANDLED_EVENTS_IS_POPUP |
                         CEvents::HANDLED_EVENTS_IS_SOUND |
                         CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                        MTVI5_BAD_ANSWER_ERROR_OFFSET,
                        "Ошиб. обмена");
                }

                // установим флаги недостоверности - выходы недостоверны.
                memset(pxModuleContext ->
                       xModuleContextStatic.
                       pucBadAoBufferPointer,
                       1,
                       xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity);
                //iBadModuleBuffUpdate();
            }
            else
            {
                (pxModuleContext ->
                 xModuleContextDinamic.
                 nucBadAnswerCounter)--;
            }
        }

//            cout << "iModuleMtvi5DataHandler pucContStBufferPointer" << endl;
////        unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)pxModuleContext ->
//                            xModuleContextStatic.
//                            pucContStBufferPointer;
//            for(int i=0; i<16; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
        break;
        // данные не готовы.
    case MTVI5_DATA_NOT_READY:
        cout << "MTVI5_DATA_NOT_READY" << endl;
//        // модуль признан неисправным?
//        if ((pxModuleContext ->
//                xModuleContextDinamic.
//                nucBadAnswerCounter) == 0)
//        {
//            // модуль признан неисправным.
//            *(pxModuleContext ->
//              xModuleContextStatic.
//              pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;
//
//            // получим код ошибки;
//            (pxModuleContext ->
//             xModuleContextDinamic.
//             ucErrorCode) = MTVI5_DATA_NOT_READY;
//
//            // установим флаги недостоверности - выходы недостоверны.
//            memset(pxModuleContext ->
//                   xModuleContextStatic.
//                   pucBadAoBufferPointer,
//                   1,
//                   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity);
//            //iBadModuleBuffUpdate();
//        }
//        else
//        {
//            (pxModuleContext ->
//             xModuleContextDinamic.
//             nucBadAnswerCounter)--;
//        }
        break;
        // ошибка базы данных.
    case MTVI5_DATA_BASE_ERROR:
        cout << "MTVI5_DATA_BASE_ERROR" << endl;
        // переведём драйвер модуля в режим записи базы данных регуляторов и задатчиков.
        pxModuleContext ->
        xModuleContextDinamic.
        ucFsmState = MTVI5_FSM_STATE_WRITE_REGULATOR_AND_PSP_DATABASE_REGULATOR;

        // активное состояние события ещё не зарегистрировано?
        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                    pxModuleContext ->
                    xModuleContextStatic.
                    ucModuleContextIndex,
                    MTVI5_DATA_BASE_ERROR_OFFSET))
        {
            // зарегистрируем активное состояние события.
            CEvents::EventRegistration(
                pxModuleContext ->
                xModuleContextStatic.
                ucModuleContextIndex,
                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                MTVI5_DATA_BASE_ERROR_OFFSET,
                "Ошиб. баз. д.");
        }

//        // модуль признан неисправным?
//        if ((pxModuleContext ->
//                xModuleContextDinamic.
//                nucBadAnswerCounter) == 0)
//        {
//            // модуль признан неисправным.
//            *(pxModuleContext ->
//              xModuleContextStatic.
//              pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;
//
//            // получим код ошибки;
//            (pxModuleContext ->
//             xModuleContextDinamic.
//             ucErrorCode) = MTVI5_DATA_BASE_ERROR;
//
//            // установим флаги недостоверности - выходы недостоверны.
//            memset(pxModuleContext ->
//                   xModuleContextStatic.
//                   pucBadAoBufferPointer,
//                   1,
//                   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity);
//            //iBadModuleBuffUpdate();
//        }
//        else
//        {
//            (pxModuleContext ->
//             xModuleContextDinamic.
//             nucBadAnswerCounter)--;
//        }
        break;
        // загрузить сохранёные значения выходов.
    case MTVI5_LOAD_OUTS:
        cout << "MTVI5_LOAD_OUTS" << endl;

        printf("iModuleMtvi5DataHandler ucModuleContextIndex %d\n\r", (pxModuleContext ->
                xModuleContextStatic.
                ucModuleContextIndex));


//            cout << "auiSpiRxBuffer[0]" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }

        usleep(1000);
        // (SPI_DATA_BYTE_OFFSET - 1) - после команды "загрузить входы" не ожидается подтверждение в виде эха,
        // а сразу идут данные.
        pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET - 1];
        // загрузиv значения выходов - 50%.
        usData = (unsigned short)8192;

        *pucDestination++ = (unsigned char)usData;
        *pucDestination++ = (unsigned char)(usData >> 8);
        usData = (unsigned short)8192;
        *pucDestination++ = (unsigned char)usData;
        *pucDestination++ = (unsigned char)(usData >> 8);
        usData = (unsigned short)8192;
        *pucDestination++ = (unsigned char)usData;
        *pucDestination++ = (unsigned char)(usData >> 8);
        usData = (unsigned short)8192;
        *pucDestination++ = (unsigned char)usData;
        *pucDestination++ = (unsigned char)(usData >> 8);
        usData = (unsigned short)8192;
        *pucDestination++ = (unsigned char)usData;
        *pucDestination++ = (unsigned char)(usData >> 8);
        usData = (unsigned short)8192;
        *pucDestination++ = (unsigned char)usData;
        *pucDestination++ = (unsigned char)(usData >> 8);

        // есть команда - перезагрузка больше чем через одну минуту?
        if ((pxModuleContext ->
                xModuleContextDinamic.
                ucCommandControl) == DEVICE_CONTROL_MTVI5_LOAD_OUTS_OSF)
        {
            // сбросим команду.
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = 0;
            // выставить OSF и перейти в режим «РУЧНОЙ».
            *pucDestination++ = 0x01;
        }
        else
        {
            // загрузить последнее значение OUT.
            *pucDestination++ = 0x00;
        }

        pucSource = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET - 1];
        usData = usCrcSummTwoByteCalculation(pucSource,
                                             (MTVI5_RESTORE_OUTS_BUFF_LENGTH +
                                              MTVI5_RESTORE_OUTS_FLAG_BYTE_QUANTITY));
        *pucDestination++ = (unsigned char)usData;
        *pucDestination++ = (unsigned char)(usData >> 8);

        auiSpiTxBuffer[0] = MTVI5_RESTORE_OUTS_COMMAND;
        xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                         xModuleContextStatic.
                                         ucModuleAddress,
                                         auiSpiTxBuffer,
                                         auiSpiRxBuffer,
                                         // (SPI_PREAMBLE_LENGTH - 1) - после команды "загрузить входы" не ожидается подтверждение в виде эха.
                                         // а сразу идут данные.
                                         (SPI_PREAMBLE_LENGTH - 1) +
                                         MTVI5_RESTORE_OUTS_BUFF_LENGTH +
                                         MTVI5_RESTORE_OUTS_FLAG_BYTE_QUANTITY +
                                         TWO_BYTE_CRC_LENGTH +
                                         TAIL_ANSWER_LENGTH,
                                         MTVI5_EXCHANGE_SPEED_IN_HZ
                                        );

        // отправим команду - изменение состояния ручной-автомат.
        pxModuleContext ->
        xModuleContextDinamic.
        ucCommandControl = DEVICE_CONTROL_MTVI5_CONT_ST_CHANGE;

//            cout << "auiSpiTxBuffer[0]" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiTxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }

//
//            cout << "auiSpiRxBuffer[0]" << endl;
////            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[0];
//            for(int i=0; i<64; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }


        break;
        // данные не готовы.
    case MTVI5_DAC_LINK_ERROR:
        cout << "MTVI5_DAC_LINK_ERROR" << endl;
//        printf("iModuleMtvi5DataHandler ucModuleContextIndex %d\n\r", (pxModuleContext ->
//                xModuleContextStatic.
//                ucModuleContextIndex));

        // модуль признан неисправным?
        if ((pxModuleContext ->
                xModuleContextDinamic.
                nucBadAnswerCounter) == 0)
        {
            // модуль признан неисправным.
            *(pxModuleContext ->
              xModuleContextStatic.
              pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;

            // отправим команду - изменение состояния ручной-автомат.
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = DEVICE_CONTROL_MTVI5_CONT_ST_CHANGE;

//            // получим код ошибки;
//            (pxModuleContext ->
//             xModuleContextDinamic.
//             ucErrorCode) = MTVI5_DAC_LINK_ERROR;

            // активное состояние события ещё не зарегистрировано?
            if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        MTVI5_DAC_LINK_ERROR_OFFSET))
            {
//        printf("iModuleMtvi5DataHandler nucBadAnswerCounter %d\n\r", (pxModuleContext ->
//                xModuleContextDinamic.
//                nucBadAnswerCounter));
                // зарегистрируем активное состояние события.
                // ошибка цепочки ЦАП.
                CEvents::EventRegistration(
                    pxModuleContext ->
                    xModuleContextStatic.
                    ucModuleContextIndex,
                    (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                     CEvents::HANDLED_EVENTS_IS_POPUP |
                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                    MTVI5_DAC_LINK_ERROR_OFFSET,
                    "Ошиб. ц. ЦАП");
            }

            // установим флаги недостоверности - выходы недостоверны.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucBadAoBufferPointer,
                   1,
                   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity);

            //iBadModuleBuffUpdate();
        }
        else
        {
            (pxModuleContext ->
             xModuleContextDinamic.
             nucBadAnswerCounter)--;
        }
        break;
        // ненормальный ответ - ошибка.
    default:
//        cout << "MTVI5 ERROR" << endl;
//
//        printf("iModuleMtvi5DataHandler ucModuleContextIndex %d\n\r", (pxModuleContext ->
//                xModuleContextStatic.
//                ucModuleContextIndex));
////
////            cout << "auiSpiRxBuffer[0]" << endl;
////            unsigned char *pucSourceTemp;
////            pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[0];
////            for(int i=0; i<64; )
////            {
////                for(int j=0; j<8; j++)
////                {
////                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////                }
////                cout << endl;
////                i += 8;
////            }

        // модуль признан неисправным?
        if ((pxModuleContext ->
                xModuleContextDinamic.
                nucBadAnswerCounter) == 0)
        {
            // модуль признан неисправным.
            *(pxModuleContext ->
              xModuleContextStatic.
              pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;

            // отправим команду - изменение состояния ручной-автомат.
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = DEVICE_CONTROL_MTVI5_CONT_ST_CHANGE;

            // модуль не отвечает в течении 60 секунд?
            if ((pxModuleContext ->
                    xModuleContextDinamic.
                    usAuxiliaryCounter) >= BAD_MODULE_60_SECOND_CYCLE_COUNT_DEFAULT)
            {
                // отправим команду модулю МТВИ5 - перезагрузка больше чем через одну минуту.
                // выставить OSF и перейти в режим «РУЧНОЙ».
                pxModuleContext ->
                xModuleContextDinamic.
                ucCommandControl = DEVICE_CONTROL_MTVI5_LOAD_OUTS_OSF;
            }
            else
            {
                // уменьшим значение счётчика - "количество сеансов связи с модулем без ответа".
                (pxModuleContext ->
                 xModuleContextDinamic.
                 usAuxiliaryCounter)++;
            }

//            // получим код ошибки;
//            (pxModuleContext ->
//             xModuleContextDinamic.
//             ucErrorCode) = BAD_MODULE_NOT_RESPONDED;

            // активное состояние события ещё не зарегистрировано?
            if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        MTVI5_BAD_ANSWER_ERROR_OFFSET))
            {
                // зарегистрируем активное состояние события.
                // ошибка обмена данными.
                CEvents::EventRegistration(
                    pxModuleContext ->
                    xModuleContextStatic.
                    ucModuleContextIndex,
                    (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                     CEvents::HANDLED_EVENTS_IS_POPUP |
                     CEvents::HANDLED_EVENTS_IS_SOUND |
                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                    MTVI5_BAD_ANSWER_ERROR_OFFSET,
                    "Ошиб. обмена");
            }


            // установим флаги недостоверности - выходы недостоверны.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucBadAoBufferPointer,
                   1,
                   xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity);
            //iBadModuleBuffUpdate();
        }
        else
        {
            // уменьшим значение счётчика - "количество сеансов связи с модулем без ответа".
            (pxModuleContext ->
             xModuleContextDinamic.
             nucBadAnswerCounter)--;
        }
        break;
    };
//    // сбросим номер регулятора.
//    pxModuleContext ->
//    xModuleContextDinamic.
//    ucCommonIndex = 0;
//    // сбросим команду.
//    pxModuleContext ->
//    xModuleContextDinamic.
//    ucCommandControl = 0;
//    // сбросим счётчик кликов мыши.
//    // на значение в переменной usAuxiliaryCounter инкрементируется-декрементируется
//    // значение уставок SP, OUT.
//    pxModuleContext ->
//    xModuleContextDinamic.
//    usAuxiliaryCounter = 0;

//    }
//    // инкрементируем делитель времени обращения к модулю.
//    (pxModuleContext ->
//     xModuleContextDinamic.
//     nucLoopSkipCounter)++;
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// проверим, внутренняя база данных модуля совпадает с базой в памяти прибора?
// если не совпадает, запишем базу из модуля в прибор.
int iModuleMtvi5RegulatorPlcDataBaseCheck(TModuleContext *pxModuleContext)
{
//    unsigned short usData;
//    int i;
//    unsigned char *pucSource;
//    unsigned char *pucDestination;
//    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
//
//    memset(&aucTempArray[0],
//           0,
//           sizeof(aucTempArray));
//
//    xSpiCommunicationDevice.ChipSelectAddressSet(pxModuleContext ->
//                             xModuleContextStatic.
//                             ucModuleAddress);
//
//    auiSpiTxBuffer[0] = MTVI5_GET_REGULATOR_DATA_BASE_COMMAND;
//    // отправляет команду модулю.
//    xSpiCommunicationDevice.Exchange(iSpiDeviceHandle,
//                 auiSpiTxBuffer,
//                 auiSpiRxBuffer,
//                 1,
//                 LOW_SPEED_IN_HZ);
//    usleep(100);
//    auiSpiTxBuffer[0] = 0;
//    // отправляет команду модулю.
//    xSpiCommunicationDevice.Exchange(iSpiDeviceHandle,
//                 auiSpiTxBuffer,
//                 auiSpiRxBuffer,
//                 1,
//                 LOW_SPEED_IN_HZ);
//    usleep(100);
//
//    //  (SPI_COMMAND_BYTE_OFFSET - 1) - на один байт меньше, потому, что перед этим команду передали отдельной транзакцией.
//    if (auiSpiRxBuffer[(0)] != MTVI5_GET_REGULATOR_DATA_BASE_COMMAND) // if echo answer is ok.
//    {
//        cout << "MTVI5_GET_REGULATOR_DATA_BASE_COMMAND no" << endl;
//        return -1;
//    }
//
//    // чтение-запись данных.
//    xSpiCommunicationDevice.Exchange(iSpiDeviceHandle,
//                 auiSpiTxBuffer,
//                 auiSpiRxBuffer,
//// количество байт при обмене данными с модулем по SPI: (один байт - команда) + (второй байт - "эхо"(ответ модуля)) +
//// (длина передаваемого-принимаемого сообщения(например длина блока базы данных)) + (один или два байта контрольной суммы).
//// SPI_PREAMBLE_LENGTH = 2, первый байт - команда, второй - ответ.
////  в функции обмена данными по SPI xSpiCommunicationDevice.Exchange() в одном пакете сразу передаются - команда и данные.
//// после принятия команды модуль затрачивает некоторое время на формирование ответа,
//// чтобы проинформировать прибор о своём состоянии. если время на формирование ответа относительно велико,
//// то модуль пропустит следующий байт - принятые данные будут некорректны.
//// чтобы этого не произошло, при обмене данными с "медленными модулями", прередача команды и передача-приём данных
//// происходит за две транзакции.
////  (SPI_PREAMBLE_LENGTH - 1) - на один байт меньше, потому, что перед этим команду передали отдельной транзакцией.
////                 (SPI_PREAMBLE_LENGTH - 1) +
//                 CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH +
//                 TWO_BYTE_CRC_LENGTH,
//                 LOW_SPEED_IN_HZ);
//    CGpio::ClearPin(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
////    //  (SPI_COMMAND_BYTE_OFFSET - 1) - на один байт меньше, потому, что перед этим команду передали отдельной транзакцией.
////    if (auiSpiRxBuffer[(SPI_COMMAND_BYTE_OFFSET - 1)] == MTVI5_GET_REGULATOR_DATA_BASE_COMMAND) // if echo answer is ok.
////    {
//    //  (SPI_DATA_BYTE_OFFSET - 1) - на один байт меньше, потому, что перед этим команду передали отдельной транзакцией.
//    if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET - 2)],
//                               CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH) > 0)
//    {
//        pucSource = &auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET - 2)];
//        // получим указатель на блок в базе данных.
//        pucDestination = (unsigned char*)(pxModuleContext ->
//                                          xModuleContextStatic.
//                                          pucDataBasePointer);
//        // база данных модуля и прибора не совпадает?
//        if (memcmp((const void*)pucDestination,
//                   (const void*)pucSource,
//                   CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH) != 0)
//        {
//            cout << "iModuleMtvi5RegulatorPlcDataBaseCheck pucSource" << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)pucSource;
//            for(int i=0; i<256; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
//            cout << "iModuleMtvi5RegulatorPlcDataBaseCheck pucDestination" << endl;
////        unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)pucDestination;
//            for(int i=0; i<256; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
//            // база данных модуля и прибора не совпадает.
//            // скопируем базу данных модуля в прибор.
//            memcpy(pucDestination,
//                   pucSource,
//                   CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH);
//
////             cout << "iModuleMtvi5RegulatorPlcDataBaseCheck pucDestination 2" << endl;
//////        unsigned char *pucSourceTemp;
////        pucSourceTemp = (unsigned char*)pucDestination;
////        for(int i=0; i<256; )
////        {
////            for(int j=0; j<8; j++)
////            {
////                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////            }
////            cout << endl;
////            i += 8;
////        }
//            cout << "iModuleMtvi5RegulatorPlcDataBaseCheck NO" << endl;
//            return -1;
//        }
//        else
//        {
//            cout << "iModuleMtvi5RegulatorPlcDataBaseCheck OK" << endl;
//            return 0;
//        }
//    }
//    cout << "iModuleMtvi5RegulatorPlcDataBaseCheck iCrcSummTwoByteCompare error" << endl;
//
//            cout << "iModuleMtvi5RegulatorPlcDataBaseCheck pucDestination" << endl;
////        unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)pucDestination;
//            for(int i=0; i<256; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
////    }
//    return -1;



    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

//    usleep(1200);

    auiSpiTxBuffer[0] = MTVI5_GET_REGULATOR_DATA_BASE_COMMAND;

    // чтение-запись данных.
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     (SPI_PREAMBLE_LENGTH +
                                      CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH +
                                      TWO_BYTE_CRC_LENGTH),
                                     MTVI5_EXCHANGE_SPEED_IN_HZ);//LOW_SPEED_IN_HZ

    pucSource = &auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET)];
    // получим указатель на блок в базе данных.
    pucDestination = (unsigned char*)(pxModuleContext ->
                                      xModuleContextStatic.
                                      pucDataBasePointer);

//    cout << "iModuleMtvi5RegulatorPlcDataBaseCheck pucSource" << endl;
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
//    cout << "iModuleMtvi5RegulatorPlcDataBaseCheck pucDestination" << endl;
////        unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)pucDestination;
//    for(int i=0; i<256; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }

    if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET)],
                               CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH) > 0)
    {
//        pucSource = &auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET)];
//        // получим указатель на блок в базе данных.
//        pucDestination = (unsigned char*)(pxModuleContext ->
//                                          xModuleContextStatic.
//                                          pucDataBasePointer);
        // база данных модуля и прибора не совпадает?
        if (memcmp((const void*)pucDestination,
                   (const void*)pucSource,
                   CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH) != 0)
        {
            // база данных модуля и прибора не совпадает.
            // скопируем базу данных модуля в прибор.
            memcpy(pucDestination,
                   pucSource,
                   CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH);

//             cout << "iModuleMtvi5RegulatorPlcDataBaseCheck pucDestination 2" << endl;
////        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)pucDestination;
//        for(int i=0; i<256; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
            cout << "iModuleMtvi5RegulatorPlcDataBaseCheck NO" << endl;
            return -1;
        }
        else
        {
            cout << "iModuleMtvi5RegulatorPlcDataBaseCheck OK" << endl;
            return 0;
        }
    }
    cout << "iModuleMtvi5RegulatorPlcDataBaseCheck iCrcSummTwoByteCompare error" << endl;

//    cout << "iModuleMtvi5RegulatorPlcDataBaseCheck ucSpiRxBuffer[(0)]" << endl;
////        unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[(0)];
//    for(int i=0; i<256; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }
    return -1;
}

//-----------------------------------------------------------------------------------------------------
// сравнивает внутреннюю базу данных модуля с базой в памяти прибора.
// если не совпадает, запишем базу из прибора в модуль.
int iModuleMtvi5RegulatorModuleDataBaseCheck(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
//    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

//    memset(&aucTempArray[0],
//           0,
//           sizeof(aucTempArray));

//    usleep(600);

    auiSpiTxBuffer[0] = MTVI5_GET_REGULATOR_DATA_BASE_COMMAND;
    // отправляет команду модулю.
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     1,
                                     MTVI5_EXCHANGE_SPEED_IN_HZ);//LOW_SPEED_IN_HZ
    usleep(100);
    auiSpiTxBuffer[0] = 0;
    // отправляет команду модулю.
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     1,
                                     MTVI5_EXCHANGE_SPEED_IN_HZ);//LOW_SPEED_IN_HZ
    usleep(100);

    //  (SPI_COMMAND_BYTE_OFFSET - 1) - на один байт меньше, потому, что перед этим команду передали отдельной транзакцией.
    if (auiSpiRxBuffer[(0)] != MTVI5_GET_REGULATOR_DATA_BASE_COMMAND) // if echo answer is ok.
    {
        cout << "MTVI5_GET_REGULATOR_DATA_BASE_COMMAND no" << endl;
        return -1;
    }

    auiSpiTxBuffer[0] = 0;
    // чтение-запись данных.
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
// количество байт при обмене данными с модулем по SPI: (один байт - команда) + (второй байт - "эхо"(ответ модуля)) +
// (длина передаваемого-принимаемого сообщения(например длина блока базы данных)) + (один или два байта контрольной суммы).
// SPI_PREAMBLE_LENGTH = 2, первый байт - команда, второй - ответ.
//  в функции обмена данными по SPI xSpiCommunicationDevice.Exchange() в одном пакете сразу передаются - команда и данные.
// после принятия команды модуль затрачивает некоторое время на формирование ответа,
// чтобы проинформировать прибор о своём состоянии. если время на формирование ответа относительно велико,
// то модуль пропустит следующий байт - принятые данные будут некорректны.
// чтобы этого не произошло, при обмене данными с "медленными модулями", прередача команды и передача-приём данных
// происходит за две транзакции.
//  (SPI_PREAMBLE_LENGTH - 1) - на один байт меньше, потому, что перед этим команду передали отдельной транзакцией.
                                     //(SPI_PREAMBLE_LENGTH - 2) +
                                     CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH,
                                     MTVI5_EXCHANGE_SPEED_IN_HZ);//LOW_SPEED_IN_HZ
//    //  (SPI_COMMAND_BYTE_OFFSET - 1) - на один байт меньше, потому, что перед этим команду передали отдельной транзакцией.
//    if (auiSpiRxBuffer[(SPI_COMMAND_BYTE_OFFSET - 1)] == MTVI5_GET_REGULATOR_DATA_BASE_COMMAND) // if echo answer is ok.
//    {
    //  (SPI_DATA_BYTE_OFFSET - 1) - на один байт меньше, потому, что перед этим команду передали отдельной транзакцией.
    if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET - 2)],
                               CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH) > 0)
    {
        pucSource = &auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET - 2)];
        // получим указатель на блок в базе данных.
        pucDestination = (unsigned char*)(pxModuleContext ->
                                          xModuleContextStatic.
                                          pucDataBasePointer);
        // база данных модуля и прибора не совпадает?
        if (memcmp((const void*)pucDestination,
                   (const void*)pucSource,
                   CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH) != 0)
        {
            // база данных пустая? или контрольная сумма не совпадает?
            if ((iBufferIsZero(pucDestination,
                               (PLC_DATA_BASE_BLOCK_LENGTH -
                                TWO_BYTE_CRC_LENGTH))) ||
                    (iCrc16Check((pxModuleContext ->
                                  xModuleContextStatic.
                                  pucDataBasePointer),
                                 (PLC_DATA_BASE_BLOCK_LENGTH -
                                  TWO_BYTE_CRC_LENGTH))) == -1)
            {
                // база данных прибора пустая или контрольная сумма не совпадает.
                // не будем копировать базу данных из прибора в модуль.
                cout << "Data base is empty or damaged." << endl;
            }
            else
            {
                // база данных модуля и прибора не совпадает.
                // сохраним в памяти модуля базу данных прочитанную из прибора.
                iModuleMtvi5RegulatorDataBaseBlockWrite(pxModuleContext);
                cout << "New data base saved in module" << endl;
            }
            cout << "iModuleMtvi5RegulatorModuleDataBaseCheck NO" << endl;
            return -1;
        }
        else
        {
            cout << "iModuleMtvi5RegulatorModuleDataBaseCheck OK" << endl;
            return 0;
        }
    }
    cout << "iModuleMtvi5RegulatorModuleDataBaseCheck crc no" << endl;
    return -1;
//    }
//    cout << "MTVI5_GET_REGULATOR_DATA_BASE_COMMAND no" << endl;
//    return -1;




//    unsigned short usData;
//    int i;
//    unsigned char *pucSource;
//    unsigned char *pucDestination;
//
//    xSpiCommunicationDevice.ChipSelectAddressSet(pxModuleContext ->
//                             xModuleContextStatic.
//                             ucModuleAddress);
//
//    auiSpiTxBuffer[0] = MTVI5_GET_REGULATOR_DATA_BASE_COMMAND;
//    // чтение-запись данных.
//    xSpiCommunicationDevice.Exchange(iSpiDeviceHandle,
//                 auiSpiTxBuffer,
//                 auiSpiRxBuffer,
//                 (SPI_PREAMBLE_LENGTH +
//                  CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH +
//                  TWO_BYTE_CRC_LENGTH),
//                 LOW_SPEED_IN_HZ);
//    CGpio::ClearPin(SPI_CHIP_ENABLE_PIN_PORT, SPI_CHIP_ENABLE_PIN);
//
//    if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET)],
//                               CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH) > 0)
//    {
//        pucSource = &auiSpiRxBuffer[(SPI_DATA_BYTE_OFFSET)];
//        // получим указатель на блок в базе данных.
//        pucDestination = (unsigned char*)(pxModuleContext ->
//                                          xModuleContextStatic.
//                                          pucDataBasePointer);
//        // база данных модуля и прибора не совпадает?
//        if (memcmp((const void*)pucDestination,
//                   (const void*)pucSource,
//                   CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH) != 0)
//        {
//            // база данных пустая? или контрольная сумма не совпадает?
//            if ((iBufferIsZero(pucDestination,
//                               (PLC_DATA_BASE_BLOCK_LENGTH -
//                                TWO_BYTE_CRC_LENGTH))) ||
//                    (iCrc16Check((pxModuleContext ->
//                                  xModuleContextStatic.
//                                  pucDataBasePointer),
//                                 (PLC_DATA_BASE_BLOCK_LENGTH -
//                                  TWO_BYTE_CRC_LENGTH))) == -1)
//            {
//                // база данных прибора пустая или контрольная сумма не совпадает.
//                // не будем копировать базу данных из прибора в модуль.
//                cout << "Data base is empty or damaged." << endl;
//            }
//            else
//            {
//                // база данных модуля и прибора не совпадает.
//                // сохраним в памяти модуля базу данных прочитанную из прибора.
//                iModuleMtvi5RegulatorDataBaseBlockWrite(pxModuleContext);
//                cout << "New data base saved in module" << endl;
//            }
//            cout << "iModuleMtvi5RegulatorModuleDataBaseCheck NO" << endl;
//            return -1;
//        }
//        else
//        {
//            cout << "iModuleMtvi5RegulatorModuleDataBaseCheck OK" << endl;
//            return 0;
//        }
//    }
//    cout << "iModuleMtvi5RegulatorModuleDataBaseCheck crc no" << endl;
//    return -1;
}

//-----------------------------------------------------------------------------------------------------
// записывает базу данных из RAM прибора в EEPROM модуля.
int iModuleMtvi5RegulatorDataBaseBlockWrite(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // получим указатель на блок базы данных, принятый по Modbus во временный буфер.
    pucSource =
        ((pxModuleContext ->
          xModuleContextStatic.
          pucDataBasePointer) +
         (pxModuleContext ->
          xModuleContextStatic.
          ucDataBaseDataOffset));

    // скопируем в буфер TX SPI блок базы данных.
    memcpy(&auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET],
           pucSource,
           CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH);
    usData = usCrcSummTwoByteCalculation(pucSource,
                                         CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH);
    // сместимся в буфере на место CRC.
    pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET + CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH];
    *pucDestination++ = (unsigned char)usData;
    *pucDestination = (unsigned char)(usData >> 8);

    auiSpiTxBuffer[0] = MTVI5_SET_REGULATOR_DATA_BASE_COMMAND;
//    usleep(600);
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH +
                                     TAIL_ANSWER_LENGTH,
                                     MTVI5_EXCHANGE_SPEED_IN_HZ);//LOW_SPEED_IN_HZ

    // есть подтверждение записи базы данных в EEPROM модуля?
    if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                        CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_LENGTH +
                        TWO_BYTE_CRC_LENGTH]) ==
            DATA_EXCHANGE_OK)
    {
//        usleep(1500000);
        // есть подтверждение записи базы данных в EEPROM модуля.
        cout << "iModuleMtvi5RegulatorDataBaseBlockWrite OK" << endl;
//        // изменение базы данных с программатора?
//        if (pxModuleContext ->
//                xModuleContextDinamic.
//                ucCommandControl == DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_PROGRAMMER)
//        {
//            // есть подтверждение записи базы данных в EEPROM модуля?
//            if ((iModuleMtvi5PspDataBaseBlockWrite(pxModuleContext)) != -1)
//            {
////                usleep(1500000);
//                return 0;
//            }
//            else
//            {
//                return -1;
//            }
//        }
        return 0;
    }
    else
    {
        cout << "iModuleMtvi5RegulatorDataBaseBlockWrite NO" << endl;
        return -1;
    }
}

//-----------------------------------------------------------------------------------------------------
int iModuleMtvi5PspDataBaseBlockWrite(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // получим указатель на блок базы данных, принятый по Modbus во временный буфер.
//    pucSource =
//        ((pxModuleContext ->
//          xModuleContextStatic.
//          pucDataBasePointer) +
//         (pxModuleContext ->
//          xModuleContextStatic.
//          ucDataBaseDataOffset));

//    memcpy(&auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET],
//           pucSource,
//           CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH);

    pucSource = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET];
    memset(pucSource,
           0,
           CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH);
    usData = usCrcSummTwoByteCalculation(pucSource,
                                         CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH);
    pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET + CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH];
    *pucDestination++ = (unsigned char)usData;
    *pucDestination = (unsigned char)(usData >> 8);

    auiSpiTxBuffer[0] = MTVI5_SET_PSP_DATA_BASE_COMMAND;
//    usleep(600);
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH +
                                     TAIL_ANSWER_LENGTH,
                                     MTVI5_EXCHANGE_SPEED_IN_HZ);//LOW_SPEED_IN_HZ

    if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                        CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_LENGTH +
                        TWO_BYTE_CRC_LENGTH]) ==
            DATA_EXCHANGE_OK)
    {
        // есть подтверждение записи базы данных в EEPROM модуля.
        cout << "iModuleMtvi5PspDataBaseBlockWrite OK" << endl;
        return 0;
    }
    else
    {
        cout << "iModuleMtvi5PspDataBaseBlockWrite NO" << endl;
        return -1;
    }
}
