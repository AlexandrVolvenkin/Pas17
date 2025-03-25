
#include "PasNewConfig.h"
#include "ModuleMvai5.h"
#include "Events.h"

using namespace std;

static unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
static  unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];



//-----------------------------------------------------------------------------------------------------
// производит обмен данными с модулями аналогового ввода.
int iModuleMvai5DataHandler(TModuleContext *pxModuleContext)
{
    int i;
    int j;
    float fData;
    unsigned char ucCalibrPlus;
    unsigned char ucCalibrMinus;
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    ucCalibrPlus = 0;
    ucCalibrMinus = 0;
    uint8_t ui8CommonIndex = (pxModuleContext ->
                              xModuleContextDinamic.
                              ucCommonIndex);
    // калибруем вход?(если ucCommonIndex не равен 0, то содержит номер калибруемого входа)
    if ((ui8CommonIndex > 0) && (ui8CommonIndex < (MVAI5_TXS_INPUT_NUMBER + 1)))
    {
        // получим номер калибруемого входа.
        ucCalibrPlus = ui8CommonIndex;
        // установим начало шкалы НШК?
        if (pxModuleContext ->
                xModuleContextDinamic.
                ucCommandControl == MVAI5_COMMAND_CONTROL_SET_BOTTOM_OF_SCALE)
        {
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = 0;
            // добавим команду модулю.
            ucCalibrPlus |= MVAI5_SET_BOTTOM_OF_SCALE;
        }
        // установим конец шкалы ВШК?
        else if (pxModuleContext ->
                 xModuleContextDinamic.
                 ucCommandControl == MVAI5_COMMAND_CONTROL_SET_TOP_OF_SCALE)
        {
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = 0;
            // добавим команду модулю.
            ucCalibrPlus |= MVAI5_SET_TOP_OF_SCALE;
        }
        // данные калибровки передаются дважды, в нормальном виде в байте(ucCalibrPlus) и
        // в инвертированном(!ucCalibrPlus + 1) в байте(ucCalibrMinus).
        // при контроле, сумма (ucCalibrPlus + ucCalibrMinus) должна быть 0.
        ucCalibrMinus = 0x00 - ucCalibrPlus;
    }

    auiSpiTxBuffer[MVAI5_CALIBRATION_DATA_OFFSET] = ucCalibrPlus;
    auiSpiTxBuffer[MVAI5_CALIBRATION_DATA_OFFSET + 1] = ucCalibrMinus;
    auiSpiTxBuffer[0] = MVAI5_GET_MEASURE_DATA_COMMAND;

    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MVAI5_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH +
                                     TWO_BYTE_CRC_LENGTH +
                                     TAIL_ANSWER_LENGTH,
                                     SPEED_IN_HZ
                                    );

    // что ответил модуль?
    switch(auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET])
    {
    case MVAI5_ANSWER_REPER_POINTS_ADC_DATABASE_ERROR:
        // ошибка БД реперных точек, но будет продолжение обмена.
        fbAnalogueInputModuleReperPointsAdcDataBaseError = 1;
        // получим код ошибки;
        (pxModuleContext ->
         xModuleContextDinamic.
         ucErrorCode) = INTERNAL_MODULE_ERROR_REPER_POINTS;

        // активное состояние события ещё не зарегистрировано?
        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                    pxModuleContext ->
                    xModuleContextStatic.
                    ucModuleContextIndex,
                    MVAI_REPER_POINTS_DATA_BASE_ERROR_OFFSET))
        {
            // зарегистрируем активное состояние события.
            CEvents::EventRegistration(
                pxModuleContext ->
                xModuleContextStatic.
                ucModuleContextIndex,
                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                 CEvents::HANDLED_EVENTS_IS_POPUP |
                 CEvents::HANDLED_EVENTS_IS_SOUND |
                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                MVAI_REPER_POINTS_DATA_BASE_ERROR_OFFSET,
                "Реперные т.");
        }


    case MVAI5_GET_MEASURE_DATA_COMMAND:
        // данные не повреждены?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   MVAI5_GET_MEASURE_DATA_COMMAND_ANSWER_LENGTH) > 0)
        {
            // модуль исправен.
            pxModuleContext ->
            xModuleContextDinamic.
            nucBadAnswerCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT;
            // сбросим флаг отказа модуля.
            *(pxModuleContext ->
              xModuleContextStatic.
              pucModuleBadStateBufferPointer) = BAD_MODULE_RESPONDED_OK;

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

            //iBadModuleBuffUpdate();
            // получим данные состояния каналов аналоговых входов.
            memcpy(pxModuleContext ->
                   xModuleContextStatic.
                   pucStatAiBufferPointer,
                   &auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET],
                   MVAI5_ANALOG_INPUT_QUANTITY);
//            cout << "MVAI5_GET_MEASURE_DATA_COMMAND ucCalibrMinus" << (int)ucCalibrMinus << endl;
//            unsigned char *pucSourceTemp;
//            pucSourceTemp = (unsigned char*)&auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET];//pxModbusMapping -> buffer_pointer;//
//            for(int i=0; i<8; )
//            {
//                for(int j=0; j<8; j++)
//                {
//                    cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                }
//                cout << endl;
//                i += 8;
//            }
            // получим измеренные значения всех аналоговых входов модуля.
            for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
            {
                // есть обрыв линии или переполнение при расчётах текущего входа?
                if ((auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_LINE_BREAK) ||
                        (auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_CALCULATION_OVERFLOW))
                {
                    // данные входа недостоверны, обнулим их.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueBufferPointer[i * sizeof(float)]),
                           0,
                           sizeof(float));
                    // установим флаг недостоверности - вход недостоверен.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 1;
                    // дискретные данные входа недостоверны, обнулим их.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH]),
                           0,
                           MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
                    // установим флаги недостоверности уставок LL, L, H, HH - недостоверны.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH]),
                           1,
                           MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
                }
                // включен режим калибровки текущего входа?
                else if(
                    (((auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_LINE_BREAK) == 0) &&
                     ((auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_CALCULATION_OVERFLOW) == 0)) &&
                    ((auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_CHANNEL_CALIBRATION)))
                {
                    // получим измеренное значение и преобразуем.
                    fData = fStep5ToFloat(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                                                          (i * MVAI5_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY)]);
                    // поместим его в рабочий массив.
                    memcpy(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueBufferPointer[i * sizeof(float)]),
                           (unsigned char*)&fData,
                           sizeof(float));
                    // установим флаг недостоверности - вход недостоверен.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 1;
                    // дискретные данные входа недостоверны, обнулим их.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH]),
                           0,
                           MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
                    // установим флаги недостоверности уставок LL, L, H, HH - недостоверны.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH]),
                           1,
                           MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
                }
                else
                {
                    // аналоговый вход выведен из обработки?
                    if (pxModuleContext ->
                            xModuleContextStatic.
                            pucAinOffBufferPointer[i])
                    {
                        // данные входов модуля выведены из обработки, обнулим их.
                        memset(&(pxModuleContext ->
                                 xModuleContextStatic.
                                 pucAiValueBufferPointer[i * sizeof(float)]),
                               0,
                               sizeof(float));
                        // установим флаг недостоверности - вход недостоверен.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucBadAiBufferPointer[i] = 1;
                        // дискретные данные входа недостоверны, обнулим их.
                        memset(&(pxModuleContext ->
                                 xModuleContextStatic.
                                 pucDiValueBufferPointer[i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH]),
                               0,
                               MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
                        // установим флаги недостоверности уставок LL, L, H, HH - недостоверны.
                        memset(&(pxModuleContext ->
                                 xModuleContextStatic.
                                 pucBadDiBufferPointer[i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH]),
                               1,
                               MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
                    }
                    else
                    {
                        // получим измеренное значение и преобразуем.
                        fData = fStep5ToFloat(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                                                              (i * MVAI5_ONE_ANALOG_INPUT_DATA_BYTE_QUANTITY)]);
                        // поместим его в рабочий массив.
                        memcpy(&(pxModuleContext ->
                                 xModuleContextStatic.
                                 pucAiValueBufferPointer[i * sizeof(float)]),
                               (unsigned char*)&fData,
                               sizeof(float));
                        // сбросим флаг недостоверности - вход достоверен.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucBadAiBufferPointer[i] = 0;
                        // сбросим флаги уставок LL, L, H, HH.
                        memset(&(pxModuleContext ->
                                 xModuleContextStatic.
                                 pucDiValueBufferPointer[i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH]),
                               0,
                               MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
                        // сбросим флаги недостоверности уставок LL, L, H, HH - достоверны.
                        memset(&(pxModuleContext ->
                                 xModuleContextStatic.
                                 pucBadDiBufferPointer[i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH]),
                               0,
                               MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
                        // нарушена уставка LL + L?
                        if ((auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                                ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L)
                        {
                            // установим флаг нарушения уставки LL.
                            (pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[(i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                     ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L_OFFSET]) = 1;

                            // установим флаг нарушения уставки L.
                            (pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[(i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                     ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                        }
                        // нарушена уставка L?
                        if ((auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                                ANALOGUE_INPUT_SET_POINT_VIOLATION_L)
                        {
                            // установим флаг нарушения уставки L.
                            (pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[(i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                     ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                        }
                        // нарушена уставка H?
                        if ((auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                                ANALOGUE_INPUT_SET_POINT_VIOLATION_H)
                        {
                            // установим флаг нарушения уставки H.
                            (pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[(i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                     ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                        }
                        // нарушена уставка HH + H?
                        if ((auiSpiRxBuffer[MVAI5_STATE_DATA_OFFSET + i] & ANALOGUE_INPUT_SET_POINT_VIOLATION_MASK) ==
                                ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H)
                        {
                            // установим флаг нарушения уставки HH.
                            (pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[(i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                     ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H_OFFSET]) = 1;

                            // установим флаг нарушения уставки H.
                            (pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[(i * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH) +
                                                     ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                        }
                    }
                }
            }
            return 0;
        }
        break;

    case MVAI5_ANSWER_DATA_NOT_READY:
        // данные не готовы.
        return 0;
        break;

    case MVAI5_ANSWER_DATABASE_ERROR:
        // ошибка БД обработки, конец связи.
        fbAnalogueInputModuleDataBaseError = 1;
        // получим код ошибки;
        (pxModuleContext ->
         xModuleContextDinamic.
         ucErrorCode) = INTERNAL_MODULE_ERROR_DATA_BASE;

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
                 CEvents::HANDLED_EVENTS_IS_POPUP |
                 CEvents::HANDLED_EVENTS_IS_SOUND |
                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                MTVI5_DATA_BASE_ERROR_OFFSET,
                "Ошиб. б. МВА");
        }

        return 0;
        break;

    default:
        break;
    };


    // модуль признан неисправным?
    if ((pxModuleContext ->
            xModuleContextDinamic.
            nucBadAnswerCounter) == 0)
    {
        // модуль признан неисправным.
        *(pxModuleContext ->
          xModuleContextStatic.
          pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;

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

        // данные входов модуля недостоверны, обнулим их.
        memset(pxModuleContext ->
               xModuleContextStatic.
               pucAiValueBufferPointer,
               0,
               MVAI5_ANALOG_INPUT_QUANTITY * sizeof(float));
        // установим флаги недостоверности - входы недостоверны.
        memset(pxModuleContext ->
               xModuleContextStatic.
               pucBadAiBufferPointer,
               1,
               MVAI5_ANALOG_INPUT_QUANTITY);
        // дискретные данные входов модуля недостоверны, обнулим их.
        memset(pxModuleContext ->
               xModuleContextStatic.
               pucDiValueBufferPointer,
               0,
               MVAI5_ANALOG_INPUT_QUANTITY * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
        // установим флаги недостоверности всех дискретных сигналов модуля.
        memset(pxModuleContext ->
               xModuleContextStatic.
               pucBadDiBufferPointer,
               1,
               MVAI5_ANALOG_INPUT_QUANTITY * MVAI5_DI_VALUE_ONE_CHANNEL_LENGTH);
        //iBadModuleBuffUpdate();
    }
    else
    {
        // уменьшим значение счётчика - "количество сеансов связи с модулем без ответа".
        (pxModuleContext ->
         xModuleContextDinamic.
         nucBadAnswerCounter)--;
    }
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// чтение из модуля зачений реперных точек и кода АЦП.
int iModuleMvai5ReperPointsAdcRead(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
//    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

//    memset(&aucTempArray[0],
//           0,
//           sizeof(aucTempArray));

    auiSpiTxBuffer[0] = MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH,
                                     SPEED_IN_HZ);
    // модуль отвечает?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND) // if echo answer is ok.
    {
        // данные не повреждены?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH) > 0)
        {
            // получим указатель на данные реперных точек и кодов АЦП принятых от модуля.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET];
//            // полученные от модуля зачения реперных точек и кода АЦП хранятся в блоках базы данных начиная с
//            // номера(ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_OFFSET) включительно.
//            // в количестве(MAX_MVAI5_MODULES_QUANTITY).
//            // в указателе pucDataBasePointer содержится адрес блока в базе данных, модулей аналогового ввода.
//            // количество блоков баз данных модулей аналогового ввода равно
//            // количеству блоков с данными реперных точек МВА + коды АЦП.
//            // прибавим разницу между смещениями на блок реперных точек МВА + коды АЦП и блок модулей аналогового ввода.
//            // и получим адрес блока реперных точек МВА + коды АЦП в базе данных.
//            pucDestination = (unsigned char*)(pxModuleContext ->
//                                              xModuleContextStatic.
//                                              pucDataBasePointer +
//                                              ((ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_OFFSET -
//                                                ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET)  * PLC_DATA_BASE_BLOCK_LENGTH));
//
            pucDestination = pxModuleContext ->
                             xModuleContextStatic.
                             pucReperPointsAdcBufferPointer;

            // скопируем данные реперных точек и кодов АЦП из временного буфера в рабочий буфер прибора.
            memcpy(pucDestination,
                   pucSource,
                   ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH);
            // всё OK.
            //ucModuleError = 0;
        }
        else
        {
            // ошибка обмена данными.
            cout << "MVAI5_GET_REPER_POINTS_ADC_CRC error" << endl;
            ucModuleError = MVAI5_GET_REPER_POINTS_ADC_DATA_COMMUNICATION_ERROR;
        }
    }
    else
    {
        // модуль не отвечает.
        cout << "MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND error" << endl;
        ucModuleError = MVAI5_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR;
    }
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// чтение из модуля, зачения температуры холодного спая ТХС (только для МВСТ3, тип 8)
int iModuleMvai5TxsRead(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    float fData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char ucCalibrPlus;
    unsigned char ucCalibrMinus;
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

//    cout << "iModuleMvai5TxsRead ucCommonIndex" << (int)(pxModuleContext ->
//            xModuleContextDinamic.
//            ucCommonIndex) << endl;
    ucCalibrPlus = 0;
    ucCalibrMinus = 0;
    uint8_t ui8CommonIndex = (pxModuleContext ->
                              xModuleContextDinamic.
                              ucCommonIndex);
    // калибруем вход?(если ucCommonIndex не равен 0, то содержит номер калибруемого входа)
    if ((ui8CommonIndex > 0) && (ui8CommonIndex < (MVAI5_TXS_INPUT_NUMBER + 1)))
//    // канал ТХС калибруется?
//    if (pxModuleContext ->
//            xModuleContextDinamic.
//            ucCommonIndex == MVAI5_TXS_INPUT_NUMBER)
    {
        // получим номер калибруемого входа.
        ucCalibrPlus = ui8CommonIndex;
        // установим начало шкалы НШК?
        if (pxModuleContext ->
                xModuleContextDinamic.
                ucCommandControl == MVAI5_COMMAND_CONTROL_SET_BOTTOM_OF_SCALE)
        {
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = 0;
            // добавим команду модулю.
            ucCalibrPlus |= MVAI5_SET_BOTTOM_OF_SCALE;
        }
        // установим конец шкалы ВШК?
        else if (pxModuleContext ->
                 xModuleContextDinamic.
                 ucCommandControl == MVAI5_COMMAND_CONTROL_SET_TOP_OF_SCALE)
        {
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommandControl = 0;
            // добавим команду модулю.
            ucCalibrPlus |= MVAI5_SET_TOP_OF_SCALE;
        }
        // данные калибровки передаются дважды, в нормальном виде в байте(ucCalibrPlus) и
        // в инвертированном(!ucCalibrPlus + 1) в байте(ucCalibrMinus).
        // при контроле, сумма (ucCalibrPlus + ucCalibrMinus) должна быть 0.
        ucCalibrMinus = 0x00 - ucCalibrPlus;
    }
//    cout << "iModuleMvai5TxsRead ucCalibrPlus" << (int)ucCalibrPlus << endl;
//    cout << "iModuleMvai5TxsRead ucCalibrMinus" << (int)ucCalibrMinus << endl;
    // требования калибровки передаются в модуль двумя байтами,
    // на месте приёма от модуля двух байт контрольной суммы.
    auiSpiTxBuffer[SPI_PREAMBLE_LENGTH + MVAI5_TXS_CALIBRATION_DATA_OFFSET] = ucCalibrPlus;
    auiSpiTxBuffer[SPI_PREAMBLE_LENGTH + MVAI5_TXS_CALIBRATION_DATA_OFFSET + 1] = ucCalibrMinus;
    auiSpiTxBuffer[0] = MVAI5_GET_TXS_DATA_COMMAND;

    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MVAI5_GET_TXS_DATA_COMMAND_ANSWER_LENGTH +
                                     TWO_BYTE_CRC_LENGTH +
                                     TAIL_ANSWER_LENGTH,
                                     SPEED_IN_HZ);
    // модуль отвечает?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVAI5_GET_TXS_DATA_COMMAND) // if echo answer is ok.
    {
        // данные не повреждены?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   MVAI5_GET_TXS_DATA_COMMAND_ANSWER_LENGTH) > 0)
        {
            // получим, преобразуем и поместим в рабочий массив, измеренное значение ТХС.
            ((TTxsData*)(pxModuleContext ->
                         xModuleContextStatic.
                         pucTxsBufferPointer)) -> fTxsValue =
                             fStep5ToFloat(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 1]);
            // получим и поместим в рабочий массив, состояние канала ТХС.
            ((TTxsData*)(pxModuleContext ->
                         xModuleContextStatic.
                         pucTxsBufferPointer)) -> ucTxsStat =
                             auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 4];
        }
        else
        {
            // ошибка обмена данными.
            cout << "MVAI5_GET_TXS_CRC error" << endl;
            ucModuleError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_TXS_READ_ERROR;
        }
    }
    else
    {
        // модуль не отвечает.
        cout << "MVAI5_GET_TXS_DATA_COMMAND error" << endl;
        ucModuleError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_TXS_READ_ERROR;
    }
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// сравнивает внутреннюю базу данных модуля с базой в памяти прибора.
// если не совпадает, запишем базу из модуля в прибор.
int iModuleMvai5PlcDataBaseCheck(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    memset(&aucTempArray[0],
           0,
           sizeof(aucTempArray));

    auiSpiTxBuffer[0] = MVAI5_GET_DATA_BASE_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH,
                                     LOW_SPEED_IN_HZ);
    // модуль отвечает?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVAI5_GET_DATA_BASE_COMMAND) // if echo answer is ok.
    {
        // данные не повреждены?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH) > 0)
        {
// в модулях аналогового ввода и в памяти прибора, базы данных хранятся в разных форматах.
//  в модулях аналогового ввода сначала идут данные программирования входов(6 входов, по 20 байт) 120 байт, потом
// текстовые реквизиты входов(6 входов, по 8 байт) 48 байт. 6 входов(120 + 48 = 168) байт.
//  в памяти прибора данные программирования и текстовые реквизиты идут отдельными блоками для каждого входа
// (один вход: 20 байт - данные программирования и 8 байт - текстовые реквизиты). один блок - 28 байт. 6 входов(6 * 28 = 168) байт.
// здесь производится преобразование формата базы данных, от модуля к прибору.
            // получим указатель на данные программирования первого входа, принятые из модуля.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET];
            pucDestination = &aucTempArray[0];
            for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
            {
                // скопируем во временный буфер часть блока - данные программирования одного входа(20 байт).
                memcpy(pucDestination, pucSource, 20);
                // перейдём к данным программирования следующего входа в модуле(+20 байт).
                pucSource += 20;
                // перейдём к следующему блоку данных одного входа в приборе(+28 байт).
                pucDestination += 28;
            }
            // получим указатель на данные текстового реквизита первого входа, принятые из модуля.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + (20 * 6)];
            pucDestination = &aucTempArray[20];
            for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
            {
                // скопируем во временный буфер часть блока - текстовые реквизиты одного входа(8 байт).
                memcpy(pucDestination, pucSource, 8);
                // перейдём к данным текстового реквизита следующего входа в модуле(+8 байт).
                pucSource += 8;
                // перейдём к следующему блоку данных одного входа в приборе(+28 байт).
                pucDestination += 28;
            }

            pucSource = &aucTempArray[0];
            // получим указатель на блок в базе данных прибора.
            pucDestination = (unsigned char*)(pxModuleContext ->
                                              xModuleContextStatic.
                                              pucDataBasePointer);
            // база данных не совпадает?
            if (memcmp((const void*)pucDestination,
                       (const void*)pucSource,
                       ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH) != 0)
            {

                // скопируем базу данных из модуля в прибор.
                memcpy(pucDestination,
                       pucSource,
                       ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH);
                cout << "iModuleMvai5PlcDataBaseCheck NO" << endl;
                return -1;
            }
            else
            {
                // база данных совпадает.
                cout << "iModuleMvai5PlcDataBaseCheck OK" << endl;
                return 0;
            }
        }
        else
        {
            // ошибка обмена данными.
            cout << "iModuleMvai5PlcDataBaseCheck CRC error" << endl;
            //cout << "MVAI5_GET_DATA_BASE_CRC error" << endl;
        }
    }
    else
    {
        // модуль не отвечает.
        cout << "iModuleMvai5PlcDataBaseCheck COMMAND error" << endl;
    }
    return -1;
}

//-----------------------------------------------------------------------------------------------------
// сравнивает внутреннюю базу данных модуля с базой в памяти прибора.
// если не совпадает, запишем базу из прибора в модуль.
int iModuleMvai5ModuleDataBaseCheck(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    memset(&aucTempArray[0],
           0,
           sizeof(aucTempArray));

    auiSpiTxBuffer[0] = MVAI5_GET_DATA_BASE_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH,
                                     LOW_SPEED_IN_HZ);
    // модуль отвечает?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVAI5_GET_DATA_BASE_COMMAND) // if echo answer is ok.
    {
        // данные не повреждены?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH) > 0)
        {
// в модулях аналогового ввода и в памяти прибора, базы данных хранятся в разных форматах.
//  в модулях аналогового ввода сначала идут данные программирования входов(6 входов, по 20 байт) 120 байт, потом
// текстовые реквизиты входов(6 входов, по 8 байт) 48 байт. 6 входов(120 + 48 = 168) байт.
//  в памяти прибора данные программирования и текстовые реквизиты идут отдельными блоками для каждого входа
// (один вход: 20 байт - данные программирования и 8 байт - текстовые реквизиты). один блок - 28 байт. 6 входов(6 * 28 = 168) байт.
// здесь производится преобразование формата базы данных, от модуля к прибору.
            // получим указатель на данные программирования первого входа, принятые из модуля.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET];

            pucDestination = &aucTempArray[0];
            for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
            {
                // скопируем во временный буфер часть блока - данные программирования одного входа(20 байт).
                memcpy(pucDestination, pucSource, 20);
                // перейдём к данным программирования следующего входа в модуле(+20 байт).
                pucSource += 20;
                // перейдём к следующему блоку данных одного входа в приборе(+28 байт).
                pucDestination += 28;
            }
            // получим указатель на данные текстового реквизита первого входа, принятые из модуля.
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + (20 * 6)];
            pucDestination = &aucTempArray[20];
            for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
            {
                // скопируем во временный буфер часть блока - текстовые реквизиты одного входа(8 байт).
                memcpy(pucDestination, pucSource, 8);
                // перейдём к данным текстового реквизита следующего входа в модуле(+8 байт).
                pucSource += 8;
                // перейдём к следующему блоку данных одного входа в приборе(+28 байт).
                pucDestination += 28;
            }

            pucSource = &aucTempArray[0];
            // получим указатель на блок в базе данных прибора.
            pucDestination = (unsigned char*)(pxModuleContext ->
                                              xModuleContextStatic.
                                              pucDataBasePointer);
            // база данных не совпадает?
            if (memcmp((const void*)pucDestination,
                       (const void*)pucSource,
                       ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH) != 0)
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
                    iModuleMvai5DataBaseBlockWrite(pxModuleContext);
                    cout << "New data base saved in module" << endl;
                }
                cout << "iModuleMvai5ModuleDataBaseCheck NO" << endl;
                return -1;
            }
            else
            {
                // база данных совпадает.
                cout << "iModuleMvai5ModuleDataBaseCheck OK" << endl;
                return 0;
            }
        }
        else
        {
            // ошибка обмена данными.
            cout << "iModuleMvai5ModuleDataBaseCheck CRC error" << endl;
        }
    }
    else
    {
        // модуль не отвечает.
        cout << "iModuleMvai5ModuleDataBaseCheck COMMAND error" << endl;
    }
    return -1;
}

////-----------------------------------------------------------------------------------------------------
//// записывает базу данных из RAM прибора в EEPROM модуля.
//int iModuleMvai5DataBaseBlockWrite(TModuleContext *pxModuleContext)
//{
//    unsigned short usData;
//    int i;
//    unsigned char *pucSource;
//    unsigned char *pucDestination;
//    unsigned int nuiBusyTimeCounter;
//    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
////    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
////    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];
//
//// в модулях аналогового ввода и в памяти прибора, базы данных хранятся в разных форматах.
////  в модулях аналогового ввода сначала идут данные программирования входов(6 входов, по 20 байт) 120 байт, потом
//// текстовые реквизиты входов(6 входов, по 8 байт) 48 байт. 6 входов(120 + 48 = 168) байт.
////  в памяти прибора данные программирования и текстовые реквизиты идут отдельными блоками для каждого входа
//// (один вход: 20 байт - данные программирования и 8 байт - текстовые реквизиты). один блок - 28 байт. 6 входов(6 * 28 = 168) байт.
//// здесь производится преобразование формата базы данных, от прибора к модулю.
//    memset(&aucTempArray[0],
//           0,
//           sizeof(aucTempArray));
//    // получим указатель на блок базы данных, принятый по Modbus во временный буфер.
//
//    pucSource =
//        ((pxModuleContext ->
//          xModuleContextStatic.
//          pucDataBasePointer) +
//         (pxModuleContext ->
//          xModuleContextStatic.
//          ucDataBaseDataOffset));
//
//    // получим указатель на данные программирования первого входа, для записи в модуль.
//    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET];
//    for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
//    {
//        // скопируем во временный буфер часть блока - данные программирования одного входа(20 байт).
//        memcpy(pucDestination, pucSource, 20);
//        // перейдём к данным программирования следующего входа в модуле(+20 байт).
//        pucDestination += 20;
//        // перейдём к следующему блоку данных одного входа в приборе(+28 байт).
//        pucSource += 28;
//    }
//    // получим указатель на блок базы данных, принятый по Modbus во временный буфер.
//
//    pucSource =
//        ((pxModuleContext ->
//          xModuleContextStatic.
//          pucDataBasePointer) +
//         (pxModuleContext ->
//          xModuleContextStatic.
//          ucDataBaseDataOffset));
//
//    // перейдём к данным текстового реквизита первого входа(+20 байт).
//    pucSource += 20;
//    // получим указатель на данные текстового реквизита первого входа, для записи в модуль.
//    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET + (20 * 6)];
//    for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
//    {
//        // скопируем во временный буфер часть блока - текстовые реквизиты одного входа(8 байт).
//        memcpy(pucDestination, pucSource, 8);
//        // перейдём к данным текстового реквизита следующего входа в модуле(+8 байт).
//        pucDestination += 8;
//        // перейдём к следующему блоку данных одного входа в приборе(+28 байт).
//        pucSource += 28;
//    }
//    // получим указатель на преобразованную базу данных от прибора к модулю.
//    pucSource = &aucTempArray[SPI_DATA_BYTE_OFFSET];
//    usData = usCrcSummTwoByteCalculation(pucSource,
//                                         ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH);
//    // получим указатель на место CRC в пакете.
//    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET +
//                                   ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH];
//    *pucDestination++ = (unsigned char)usData;
//    *pucDestination = (unsigned char)(usData >> 8);
//    // скопируем преобразованную базу данных из временного в буфер TX SPI.
//    memcpy(&auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET],
//           pucSource,
//           ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
//           TWO_BYTE_CRC_LENGTH);
//    // отправим данные в модуль.
//    auiSpiTxBuffer[0] = MVAI5_SET_DATA_BASE_COMMAND;
//    xSpiCommunicationDevice.Exchange(pxModuleContext ->
//                                     xModuleContextStatic.
//                                     ucModuleAddress,
//                                     auiSpiTxBuffer,
//                                     auiSpiRxBuffer,
//                                     SPI_PREAMBLE_LENGTH +
//                                     ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
//                                     TWO_BYTE_CRC_LENGTH +
//                                     TAIL_ANSWER_LENGTH,
//                                     LOW_SPEED_IN_HZ);
//    // модуль отвечает?
//    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVAI5_SET_DATA_BASE_COMMAND) // if echo answer is ok.
//    {
//        // данные приняты без ошибок?
//        if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
//                            ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
//                            TWO_BYTE_CRC_LENGTH]) ==
//                DATA_EXCHANGE_OK)
//        {
//            // есть подтверждение записи базы данных в EEPROM модуля.
//            cout << "iModuleMvai5DataBaseBlockWrite OK" << endl;
//            i = 0;
////            nuiBusyTimeCounter = MVAI5_CHECK_DATA_BASE_WRITE_BUSY_WAITING_TIME;
////            // ждём подтверждения записи базы данных в EEPROM модуля.
////            // если таймаут - ошибка.
////            while (1)
////            {
////                // ждём, пока модуль не запишет базу данных в EEPROM.
////                usleep(MVAI5_CHECK_DATA_BASE_WRITE_BUSY_DELAY_TIME);
////                // примем данные с результатом программирования.
////                auiSpiTxBuffer[0] = MVAI5_CHECK_DATA_BASE_WRITE_COMMAND;
////                xSpiCommunicationDevice.Exchange(pxModuleContext ->
////                                                 xModuleContextStatic.
////                                                 ucModuleAddress,
////                                                 auiSpiTxBuffer,
////                                                 auiSpiRxBuffer,
////                                                 SPI_PREAMBLE_LENGTH +
////                                                 TAIL_ANSWER_LENGTH,
////                                                 LOW_SPEED_IN_HZ);
////                // есть подтверждение записи базы данных в EEPROM модуля?
////                if((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == DATA_EXCHANGE_OK)
////                {
////                    // есть подтверждение записи базы данных в EEPROM модуля.
////                    cout << "iModuleMvai5DataBaseBlockWrite OK" << endl;
////                    i = 0;
////                    break;
////                }
////                // модуль занят записью данных в EEPROM?
////                else if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == MVAI5_CHECK_DATA_BASE_WRITE_ANSWER_BUSY)
////                {
////                    // модуль занят записью данных в EEPROM.
////                }
////                // произошла ошибка при записи базы данных в EEPROM модуля?
////                else if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == DATA_EXCHANGE_ERROR)
////                {
////                    // произошла ошибка при записи базы данных в EEPROM модуля.
////                    ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_EEPROM_ERROR;
////                    cout << "iModuleMvai5DataBaseBlockWrite NO" << endl;
////                    i = -1;
////                    break;
////                }
////                else
////                {
////
////                }
////                // модуль не отвечает на запрос о результате программирования базы данных?
////                if (!nuiBusyTimeCounter--)
////                {
////                    // нет подтверждения записи базы данных в EEPROM модуля.
////                    ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_NOT_CONFIRM;
////                    cout << "iModuleMvai5DataBaseBlockWrite NO" << endl;
////                    i = -1;
////                    break;
////                }
////            }
//        }
//        else
//        {
//            // ошибка загрузки базы данных в модуль.
//            ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_LOAD_ERROR;
//            cout << "iModuleMvai5DataBaseBlockWrite NO" << endl;
//            i = -1;
//        }
//    }
//    else
//    {
//        // модуль не отвечает.
//        ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_LOAD_ERROR;
//        cout << "iModuleMvai5DataBaseBlockWrite NO" << endl;
//        i = -1;//debag//
//    }
//
//    return i;
//}




















////-----------------------------------------------------------------------------------------------------
//// записывает базу данных из RAM прибора в EEPROM модуля.
//int iModuleMvai5ChangeProtocol(TModuleContext *pxModuleContext)
//{
////    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
////    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];
//
//    auiSpiTxBuffer[0] = CHANGE_PROTOCOL_COMMAND;
//    xSpiCommunicationDevice.Exchange(pxModuleContext ->
//                                     xModuleContextStatic.
//                                     ucModuleAddress,
//                                     auiSpiTxBuffer,
//                                     auiSpiRxBuffer,
//                                     SPI_PREAMBLE_LENGTH +
//                                     GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH +
//                                     ONE_BYTE_CRC_LENGTH,
//                                     LOW_SPEED_IN_HZ);
//
//
////    cout << "CHANGE_PROTOCOL_COMMAND " << endl;
////    unsigned char *pucSourceTemp;
////    pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
////    for(int i=0; i<16; )
////    {
////        for(int j=0; j<8; j++)
////        {
////            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////        }
////        cout << endl;
////        i += 8;
////    }
//
//    // модуль отвечает?
//    if (auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET] ==
//            CHANGE_PROTOCOL_COMMAND) // if echo answer is ok.
//    {
//        // обмен данными прошёл без ошибок?
//        if ((iCrcSummOneByteCompare(&auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET],
//                                    GET_MODULE_TYPE_COMMAND_LENGTH +
//                                    GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH) > 0))
//        {
//            // есть подтверждение записи базы данных в EEPROM модуля.
//            cout << "iModuleMvai5ChangeProtocol ok" << endl;
//            return 0;
//        }
//    }
//
//    cout << "iModuleMvai5ChangeProtocol error" << endl;
//
//    return -1;
//}

//-----------------------------------------------------------------------------------------------------
// производит обмен данными с модулем дискретного вывода - MR.
int iModuleMvai5TransferData(TModuleContext *pxModuleContext)
{
    unsigned char ucData;
    unsigned short usData;
    unsigned char ucLocalKvit;
    unsigned char ucLocalReset;
    // ucLocalBou байт управления модулем реле.
    // Байт упр-я f_bou: флаги управления МР:  D0-"квит.от РС", D1-"сброс от РС"
    // D2-кнопка"квит.Ц", D3-кнопка"сброс Ц", D4-флаг "блок.ВКЛ/ОТКЛ".
    // биты управления МР байта f_bou.
    // квитирование с верхнего уровня.
    // #define GLOBAL_KVIT_PC_BIT 0
    // сброс с верхнего уровня.
    // #define GLOBAL_RESET_PC_BIT 1
    // квитирование с цетральной панели МИНД.
    // #define GLOBAL_KVIT_BUTTON_BIT 2
    // сброс с цетральной панели МИНД.
    // #define GLOBAL_RESET_BUTTON_BIT 3
    // режим блокировки.
    // #define GLOBAL_BLOCK_BIT 4
    unsigned char ucLocalBou;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    TMrXXOneChannelDataBase *pxMrXXOneChannelDataBase;
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // получим указатель на блок в базе данных.
    pxMrXXOneChannelDataBase =
        (TMrXXOneChannelDataBase*)((pxModuleContext ->
                                    xModuleContextStatic.
                                    pucDataBasePointer) +
                                   (pxModuleContext ->
                                    xModuleContextStatic.
                                    ucDataBaseDataOffset));

    ucLocalKvit = 0;
    // цикл проверяет наличие внешнего квитирования.
    for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
    {
        // для текущего входа внешнее квитирование запрограммировано?
        if (pxMrXXOneChannelDataBase[i].ucExternalKvitAddress)
        {
            // для текущего входа есть внешнее квитирование?
            if (*((pxModuleContext ->
                    xModuleContextStatic.
                    pucDiValueOriginBufferPointer) +
                    (pxMrXXOneChannelDataBase[i].
                     ucExternalKvitAddress) - 1))
            {
                // установим квитирование для текущего входа.
                ucLocalKvit |= (1 << i);
            }
        }
    }

    ucLocalReset = 0;
    // цикл проверяет наличие внешнего сброса.
    for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
    {
        // для текущего входа внешний сброс запрограммирован?
        if (pxMrXXOneChannelDataBase[i].ucExternalResetAddress)
        {
            // для текущего входа есть внешний сброс?
            if (*((pxModuleContext ->
                    xModuleContextStatic.
                    pucDiValueOriginBufferPointer) +
                    (pxMrXXOneChannelDataBase[i].
                     ucExternalResetAddress) - 1))
            {
                // установим сброс для текущего входа.
                ucLocalReset |= (1 << i);
            }
        }
    }

    ucLocalBou = 0;
    // есть квитирование Modbus?
    if (pxModuleContext ->
            xModuleContextDinamic.
            fbGlobalModbusKvit)
    {
        cout << "fbGlobalModbusKvit" << endl;
        // сбросим флаг поступившей команды.
        pxModuleContext ->
        xModuleContextDinamic.
        fbGlobalModbusKvit = 0;
        // установим флаг управления - квитирование Modbus.
        ucLocalBou |= (1 << GLOBAL_KVIT_PC_BIT);
    }
    // есть сброс Modbus?
    if (pxModuleContext ->
            xModuleContextDinamic.
            fbGlobalModbusReset)
    {
        cout << "fbGlobalModbusReset" << endl;
        // сбросим флаг поступившей команды.
        pxModuleContext ->
        xModuleContextDinamic.
        fbGlobalModbusReset = 0;
        // установим флаг управления - сброс Modbus.
        ucLocalBou |= (1 << GLOBAL_RESET_PC_BIT);
    }
    // есть квитирование с кнопки центральной панели?
    if (pxModuleContext ->
            xModuleContextDinamic.
            fbGlobalButtonKvit)
    {
        // сбросим флаг поступившей команды.
        pxModuleContext ->
        xModuleContextDinamic.
        fbGlobalButtonKvit = 0;
        // установим флаг управления - квитирование с центральной кнопки.
        ucLocalBou |= (1 << GLOBAL_KVIT_BUTTON_BIT);
    }
    // есть сброс с кнопки центральной панели?
    if (pxModuleContext ->
            xModuleContextDinamic.
            fbGlobalButtonReset)
    {
        // сбросим флаг поступившей команды.
        pxModuleContext ->
        xModuleContextDinamic.
        fbGlobalButtonReset = 0;
        // установим флаг управления - сброс с центральной кнопки.
        ucLocalBou |= (1 << GLOBAL_RESET_BUTTON_BIT);
    }
    // включен режим блокировки?
//    if (pxModuleContext ->
//            xModuleContextDinamic.
//            fbGlobalModbusBlock)
    if (fbGlobalBlock)
    {
//        // сбросим флаг поступившей команды.
//        pxModuleContext ->
//        xModuleContextDinamic.
//        fbGlobalModbusBlock = 0;
        // установим флаг управления - блокировка Modbus.
        ucLocalBou |= (1 << GLOBAL_BLOCK_BIT);
    }
    // нет повреждений баз данных?
    if ((!fbModuleMrOutOff) || (!fbPlcErrorReset))
    {
        pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET];
        pucSource = (pxModuleContext ->
                     xModuleContextStatic.
                     pucDoValueBlockBufferPointer);
        ucData = 0;
        // заполним требования включения для каждого реле.
        for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
        {
            if (pucSource[i])
            {
                ucData |= (1 << i);
            }
            else
            {

            }
        }

//        if (ucData)
//        {
//            cout  << "ucData" << " " <<  hex << uppercase << setw(2) << setfill('0') << (unsigned int)ucData << endl;
//        }
        *pucDestination++ = ucData;
//        cout << "iModuleMrXXTransferData ucData 1 " << std::showbase << std::dec << (int)ucData << std::endl;

        pucSource = (pxModuleContext ->
                     xModuleContextStatic.
                     pucDoValueBufferPointer);
        ucData = 0;
        // заполним новые нарушения для каждого реле.
        for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
        {
            if (pucSource[i])
            {
                ucData |= (1 << i);
            }
            else
            {

            }
        }

//        if (ucData)
//        {
//            cout  << "ucData2" << " " <<  hex << uppercase << setw(2) << setfill('0') << (unsigned int)ucData << endl;
//        }
        *pucDestination++ = ucData;
//        cout << "iModuleMrXXTransferData ucData 2 " << std::showbase << std::dec << (int)ucData << std::endl;
    }
    else
    {
        // есть повреждения баз данных.
        *pucDestination++ = 0;
        *pucDestination++ = 0;
    }



    auiSpiTxBuffer[0] = MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND;
    auiSpiTxBuffer[1] = MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH;

    *pucDestination++ = ucLocalKvit;
    *pucDestination++ = ucLocalReset;

    *pucDestination++ = ucLocalBou;
    *pucDestination = HammingDistanceCrc(&auiSpiTxBuffer[0],
                                         (SPI_PREAMBLE_LENGTH +
                                          MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH));


    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH +
                                     ONE_BYTE_CRC_LENGTH,
                                     SPEED_IN_HZ
                                    );


    // Данные не повреждены?
    if (auiSpiRxBuffer[(SPI_MT_RX_OFFSET +
                        SPI_MT_PRIAMBLE_LENGTH +
                        auiSpiRxBuffer[SPI_MT_RX_OFFSET + SPI_MT_LENGTH_OFFSET])] ==
            HammingDistanceCrc(&auiSpiRxBuffer[SPI_MT_RX_OFFSET],
                               (SPI_MT_PRIAMBLE_LENGTH +
                                auiSpiRxBuffer[SPI_MT_RX_OFFSET + SPI_MT_LENGTH_OFFSET])))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

//-----------------------------------------------------------------------------------------------------
// записывает базу данных из RAM прибора в EEPROM модуля.
int iModuleMvai5DataBaseBlockWrite(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned int nuiBusyTimeCounter;
    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

// в модулях аналогового ввода и в памяти прибора, базы данных хранятся в разных форматах.
//  в модулях аналогового ввода сначала идут данные программирования входов(6 входов, по 20 байт) 120 байт, потом
// текстовые реквизиты входов(6 входов, по 8 байт) 48 байт. 6 входов(120 + 48 = 168) байт.
//  в памяти прибора данные программирования и текстовые реквизиты идут отдельными блоками для каждого входа
// (один вход: 20 байт - данные программирования и 8 байт - текстовые реквизиты). один блок - 28 байт. 6 входов(6 * 28 = 168) байт.
// здесь производится преобразование формата базы данных, от прибора к модулю.
    memset(&aucTempArray[0],
           0,
           sizeof(aucTempArray));
    // получим указатель на блок базы данных, принятый по Modbus во временный буфер.

    pucSource =
        ((pxModuleContext ->
          xModuleContextStatic.
          pucDataBasePointer) +
         (pxModuleContext ->
          xModuleContextStatic.
          ucDataBaseDataOffset));

    // получим указатель на данные программирования первого входа, для записи в модуль.
    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET];
    for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
    {
        // скопируем во временный буфер часть блока - данные программирования одного входа(20 байт).
        memcpy(pucDestination, pucSource, 20);
        // перейдём к данным программирования следующего входа в модуле(+20 байт).
        pucDestination += 20;
        // перейдём к следующему блоку данных одного входа в приборе(+28 байт).
        pucSource += 28;
    }
    // получим указатель на блок базы данных, принятый по Modbus во временный буфер.

    pucSource =
        ((pxModuleContext ->
          xModuleContextStatic.
          pucDataBasePointer) +
         (pxModuleContext ->
          xModuleContextStatic.
          ucDataBaseDataOffset));

    // перейдём к данным текстового реквизита первого входа(+20 байт).
    pucSource += 20;
    // получим указатель на данные текстового реквизита первого входа, для записи в модуль.
    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET + (20 * 6)];
    for (i = 0; i < MVAI5_ANALOG_INPUT_QUANTITY; i++)
    {
        // скопируем во временный буфер часть блока - текстовые реквизиты одного входа(8 байт).
        memcpy(pucDestination, pucSource, 8);
        // перейдём к данным текстового реквизита следующего входа в модуле(+8 байт).
        pucDestination += 8;
        // перейдём к следующему блоку данных одного входа в приборе(+28 байт).
        pucSource += 28;
    }
    // получим указатель на преобразованную базу данных от прибора к модулю.
    pucSource = &aucTempArray[SPI_DATA_BYTE_OFFSET];
    usData = usCrcSummTwoByteCalculation(pucSource,
                                         ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH);
    // получим указатель на место CRC в пакете.
    pucDestination = &aucTempArray[SPI_DATA_BYTE_OFFSET +
                                   ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH];
    *pucDestination++ = (unsigned char)usData;
    *pucDestination = (unsigned char)(usData >> 8);
    // скопируем преобразованную базу данных из временного в буфер TX SPI.
    memcpy(&auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET],
           pucSource,
           ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
           TWO_BYTE_CRC_LENGTH);
    // отправим данные в модуль.
    auiSpiTxBuffer[0] = MVAI5_SET_DATA_BASE_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH +
                                     TAIL_ANSWER_LENGTH,
                                     LOW_SPEED_IN_HZ);

    pxModuleContext ->
    xModuleContextDinamic.
    usAuxiliaryCounter = 0;

    return 0;
}

//-----------------------------------------------------------------------------------------------------
int iModuleMvai5DataBaseBlockWriteError(TModuleContext *pxModuleContext)
{
    // База данных не записана?
    if ((pxModuleContext ->
            xModuleContextDinamic.
            usAuxiliaryCounter) >= 20)
    {
        // ошибка БД обработки, конец связи.
        fbAnalogueInputModuleDataBaseError = 1;
        // получим код ошибки;
        (pxModuleContext ->
         xModuleContextDinamic.
         ucErrorCode) = INTERNAL_MODULE_ERROR_DATA_BASE;

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
                 CEvents::HANDLED_EVENTS_IS_POPUP |
                 CEvents::HANDLED_EVENTS_IS_SOUND |
                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                MTVI5_DATA_BASE_ERROR_OFFSET,
                "Ошиб. б. МВА");
        }

        return -1;
    }
    else
    {
        // Увеличим значение счётчика - "количество сеансов связи с модулем без ответа".
        (pxModuleContext ->
         xModuleContextDinamic.
         usAuxiliaryCounter)++;

        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// записывает базу данных из RAM прибора в EEPROM модуля.
int iModuleMvai5DataBaseBlockWriteCheck(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned int nuiBusyTimeCounter;
    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // примем данные с результатом программирования.
    auiSpiTxBuffer[0] = MVAI5_CHECK_DATA_BASE_WRITE_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     TAIL_ANSWER_LENGTH,
                                     LOW_SPEED_IN_HZ);

    return 0;

//    // есть подтверждение записи базы данных в EEPROM модуля?
//    if((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == DATA_EXCHANGE_OK)
//    {
//        // есть подтверждение записи базы данных в EEPROM модуля.
////        ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
//        cout << "iModuleMvai5DataBaseBlockWrite OK" << endl;
//        return 0;
//    }
//    // модуль занят записью данных в EEPROM?
//    else if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == MVAI5_CHECK_DATA_BASE_WRITE_ANSWER_BUSY)
//    {
//        // модуль занят записью данных в EEPROM.
////        ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_BUSY;
//        cout << "iModuleMvai5DataBaseBlockWrite BUSY" << endl;
//        return -1;
//    }
//    // произошла ошибка при записи базы данных в EEPROM модуля?
//    else if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) == DATA_EXCHANGE_ERROR)
//    {
//        // произошла ошибка при записи базы данных в EEPROM модуля.
//        ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_EEPROM_ERROR;
//        cout << "iModuleMvai5DataBaseBlockWrite ERROR" << endl;
//        return -1;
//    }
//    else
//    {
//        // произошла ошибка при записи базы данных в EEPROM модуля.
//        ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_EEPROM_ERROR;
//        cout << "iModuleMvai5DataBaseBlockWrite ERROR" << endl;
//        return -1;
//    }
}

//-----------------------------------------------------------------------------------------------------
// Главный автомат.
void vModuleMvai5DataHandlerNew(TModuleContext *pxModuleContext)
{
    switch (pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState)
    {
    case MVAI5_IDDLE:
        break;

//    case MVAI5_CHANGE_PROTOCOL:
//        cout << "MVAI5_CHANGE_PROTOCOL" << endl;
//        if (iModuleMvai5ChangeProtocol(pxModuleContext) != -1)
//        {
//            pxModuleContext ->
//            xModuleContextDinamic.
//            ucFsmState = MVAI5_DATA_EXCHANGE;
//        }
//        break;

    case MVAI5_WRITE_DATA_BASE:
        cout << "MVAI5_WRITE_DATA_BASE" << endl;
        iModuleMvai5DataBaseBlockWrite(pxModuleContext);
        pxModuleContext ->
        xModuleContextDinamic.
        ucFsmState = MVAI5_WRITE_DATA_BASE_CHECK;
        break;

    case MVAI5_WRITE_DATA_BASE_CHECK:
        cout << "MVAI5_WRITE_DATA_BASE_CHECK" << endl;
        iModuleMvai5DataBaseBlockWriteCheck(pxModuleContext);

        // Проверим ответ модуля на предыдущую команду - запись базы данных.
        switch (auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET])
        {
            // База данных записана успешно?
        case DATA_EXCHANGE_OK:
            cout << "DATA_EXCHANGE_OK" << endl;

            iPlcDataBaseBlockWrite(pxModuleContext ->
                                   xModuleContextDinamic.
                                   ucCommonIndex);

            vWorkArraysUpdate();

            ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
            cout << "iModuleMvai5DataBaseBlockWrite OK" << endl
                 ;
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommonIndex = 0;

            pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState = MVAI5_DATA_EXCHANGE;
            break;
            // Во время записи базы данных произощла ошибка?
        case MVAI5_CHECK_DATA_BASE_WRITE_ANSWER_BUSY:
            cout << "MVAI5_CHECK_DATA_BASE_WRITE_ANSWER_BUSY" << endl;
            // модуль занят записью данных в EEPROM.
            ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_BUSY;
            cout << "iModuleMvai5DataBaseBlockWrite BUSY" << endl;
            if (iModuleMvai5DataBaseBlockWriteError(pxModuleContext) == -1)
            {

                // при записи в модуль произошла ошибка.
                // восстановим базу данных.
                iPlcDataBaseCheck();
                vWorkArraysUpdate();

                // произошла ошибка при записи базы данных в EEPROM модуля.
                ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_EEPROM_ERROR;
                cout << "iModuleMvai5DataBaseBlockWrite ERROR" << endl;
                ;
                pxModuleContext ->
                xModuleContextDinamic.
                ucCommonIndex = 0;

                pxModuleContext ->
                xModuleContextDinamic.
                ucFsmState = MVAI5_DATA_EXCHANGE;
            }
            break;

            // Во время записи базы данных произощла ошибка?
        case DATA_EXCHANGE_ERROR:
            cout << "DATA_EXCHANGE_ERROR" << endl;

            // при записи в модуль произошла ошибка.
            // восстановим базу данных.
            iPlcDataBaseCheck();
            vWorkArraysUpdate();

            // произошла ошибка при записи базы данных в EEPROM модуля.
            ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_EEPROM_ERROR;
            cout << "iModuleMvai5DataBaseBlockWrite ERROR" << endl;
            ;
            pxModuleContext ->
            xModuleContextDinamic.
            ucCommonIndex = 0;

            pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState = MVAI5_DATA_EXCHANGE;
            break;

            // Произошла ошибка обмена?
        default:
            cout << "DATA_EXCHANGE_ERROR default" << endl;

            if (iModuleMvai5DataBaseBlockWriteError(pxModuleContext) == -1)
            {
                // при записи в модуль произошла ошибка.
                // восстановим базу данных.
                iPlcDataBaseCheck();
                vWorkArraysUpdate();

                // произошла ошибка при записи базы данных в EEPROM модуля.
                ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_EEPROM_ERROR;
                cout << "iModuleMvai5DataBaseBlockWrite ERROR" << endl;
                ;
                pxModuleContext ->
                xModuleContextDinamic.
                ucCommonIndex = 0;

                pxModuleContext ->
                xModuleContextDinamic.
                ucFsmState = MVAI5_DATA_EXCHANGE;
            }

            break;
        }

        break;

    case MVAI5_DATA_EXCHANGE:
////        cout << "DATA_EXCHANGE" << endl;
        iModuleMvai5DataHandler(pxModuleContext);

//        if (iModuleMvai5TransferData(pxModuleContext) != -1)
//        {
//            // Проверим ответ модуля на предыдущую команду - запись базы данных.
//            switch (auiSpiRxBuffer[(SPI_MT_RX_OFFSET + SPI_MT_FUNCTION_CODE_OFFSET)])
//            {
//                // Обмен данными прошёл без ошибок?
//            case MVAI5_DATA_READY:
////                cout << "MVAI5_DATA_READY" << endl;
////                iModuleMvai5TransferDataReceive(pxModuleContext);
//                break;
//                // Требуется передача базы данных?
//            case MVAI5_DATA_BASE_ERROR:
//                cout << "MVAI5_DATA_BASE_ERROR" << endl;
//                pxModuleContext ->
//                xModuleContextDinamic.
//                ucFsmState = MVAI5_WRITE_DATA_BASE;
//                break;
//                // Произошла ошибка обмена?
//            default:
//                cout << "MVAI5_SET_DATA_BASE_COMMAND_ERROR default" << endl;
////                iModuleMvai5TransferDataReceiveError(pxModuleContext);
//                break;
//            }
//        }
//        else
//        {
//            cout << "MVAI5_DATA_EXCHANGE  error" << endl;
////            iModuleMvai5TransferDataReceiveError(pxModuleContext);
//        }
        break;

    default:
        break;
    }
}

