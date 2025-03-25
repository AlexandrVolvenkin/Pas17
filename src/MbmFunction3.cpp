
#include "PasNewConfig.h"
#include "MbmFunction3.h"
#include "Error.h"
#include "Events.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// производит обмен данными с модулями функционального блока «МВМ» функция Modbus - 3.
// производства ЗАО НПП "Центравтоматика".
int iMbmFunction3DataHandler(TModuleContext *pxModuleContext)
{
    unsigned char ucTempData;
    int i;
    float fResult;
    unsigned short usMaxCode;
    float fMinOfScale;
    float fMaxOfScale;
    short sCurrentMeasure;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    TMbmFunction1234 *pxFunction1234Pointer;
    TAnalogueInputDescriptionWork *pxAnalogueInputDescriptionWork;
    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];

    memset(&aucTempArray[0],
           0,
           sizeof(aucTempArray));
    // получим указатель на описатель функционального блока «МВМ» в базе данных.
    pxFunction1234Pointer = (TMbmFunction1234*)((pxModuleContext ->
                            xModuleContextStatic.
                            pucDataBasePointer) +
                            (pxModuleContext ->
                             xModuleContextStatic.
                             ucDataBaseDataOffset));

    // получим указатель на нормализованный блок базы данных описателей аналоговых входов.
    pxAnalogueInputDescriptionWork = (TAnalogueInputDescriptionWork*)(pxModuleContext ->
                                     xModuleContextStatic.
                                     pucAnalogueInputDescriptionWork);
    // установим адрес модуля локальной сети нижнего уровня.
    modbus_set_slave(pxMasterRtuCtx,
                     (pxFunction1234Pointer -> ucModbusSlaveAddress));
    // примем данные от модуля локальной сети нижнего уровня во временный буфер.
    i = modbus_read_registers(pxMasterRtuCtx,
                              (((unsigned short)(pxFunction1234Pointer -> ucAddressH) << 8) |
                               (unsigned short)(pxFunction1234Pointer -> ucAddressL)),
                              (((unsigned short)(pxFunction1234Pointer -> ucQuantityH) << 8) |
                               (unsigned short)(pxFunction1234Pointer -> ucQuantityL)),
                              (unsigned short*)&aucTempArray[0]);

    pthread_mutex_lock(&xMainDataAccessMutex);
    // модуль исправен?
    if (i != -1)
    {
        // модуль исправен.
        pxModuleContext ->
        xModuleContextDinamic.
        nucBadAnswerCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT;
        // сбросим флаг отказа модуля.
        *(pxModuleContext ->
          xModuleContextStatic.
          pucModuleBadStateBufferPointer) = BAD_MODULE_RESPONDED_OK;
        //iBadModuleBuffUpdate();

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
                ucModuleAddress,
                (CEvents::HANDLED_EVENTS_EXTERNAL_MODULES_BAD_TYPE |
                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                0,
                "Норма SL");
        }

        //iBadModuleBuffUpdate();
        // данные модуля достоверны.
        memset(pxModuleContext ->
               xModuleContextStatic.
               pucBadDiBufferPointer,
               0,
               ((pxFunction1234Pointer -> ucIOQuantity) *
                ANALOGUE_INPUT_DI_VALUE_QUANTITY));
        // очистим массив - состояние аналоговых измерительных каналов.
        for (i = 0; i < (pxFunction1234Pointer -> ucIOQuantity); i++)
        {
            pxModuleContext ->
            xModuleContextStatic.
            pucStatAiBufferPointer[i] &=
                ~(ANALOGUE_INPUT_CHANNEL_CALIBRATION |
                  ANALOGUE_INPUT_LINE_BREAK |
                  ANALOGUE_INPUT_CALCULATION_OVERFLOW);
        }
        // какой формат данных, получаемых от модуля локальной сети нижнего уровня?
        switch(((pxFunction1234Pointer ->
                 ucTust) & TUST_DATA_FORMAT_MASK) >> TUST_DATA_FORMAT_OFFSET)
        {
        case SCALABLE_CODE_16_BIT:
            // обработаем запрограммированное количество входов или выходов функционального блока.
            for (i = 0; i < (pxFunction1234Pointer -> ucIOQuantity); i++)
            {
                // получим измеренное значение текущего аналогового входа.
                // big endian to little endian.
                ((unsigned char*)&sCurrentMeasure)[0] = aucTempArray[(i * sizeof(unsigned short))];
                ((unsigned char*)&sCurrentMeasure)[1] = aucTempArray[(i * sizeof(unsigned short)) + 1];

                // получим МАХ код аналогового сигнала.
                usMaxCode = (((unsigned short)(pxFunction1234Pointer ->
                                               ucMaxCodeH) << 8) |
                             (unsigned short)(pxFunction1234Pointer ->
                                              ucMaxCodeL));
                // аналоговый вход выведен из обработки?
                if ((pxModuleContext ->
                        xModuleContextStatic.
                        pucAinOffBufferPointer[i]))
                {
                    // данные входов модуля выведены из обработки, обнулим их.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueBufferPointer[i * sizeof(float)]),
                           0,
                           sizeof(float));
                    // установим флаги недостоверности - входы недостоверны.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 1;
                    // данные входов модуля выведены из обработки, обнулим их.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // установим флаги недостоверности - входы недостоверны.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           1,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // очистим байт - состояние аналоговых измерительных каналов, текущего канала.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] = 0;
                }
                //  или аналоговое измерение меньше порога ошибки (0 - 3 процента MaxCode)?
                //  или аналоговое измерение больше порога ошибки (MaxCode + 3 процента MaxCode)?
                else if ((sCurrentMeasure <
                          ((float)0 - (((float)usMaxCode / PERCENT_100) * MEASURE_ERROR_THRESHOLD))) ||
                         (sCurrentMeasure >
                          ((float)usMaxCode + (((float)usMaxCode / PERCENT_100) * MEASURE_ERROR_THRESHOLD))))
                {
                    // данные входов модуля выведены из обработки, обнулим их.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueBufferPointer[i * sizeof(float)]),
                           0,
                           sizeof(float));
                    // установим флаги недостоверности - входы недостоверны.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 1;
                    // данные входов модуля выведены из обработки, обнулим их.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // установим флаги недостоверности - входы недостоверны.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           1,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // очистим байт - состояние аналоговых измерительных каналов, текущего канала.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] |=
                        ANALOGUE_INPUT_LINE_BREAK;
                }
                else
                {
                    // получим значение НШК - нижний предел шкалы датчика, из описателя
                    // аналогового входа базы данных.
                    fMinOfScale = pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                  ucAiValueArrayBegin) + i].
                                  fMinOfScale;
                    // получим значение ВШК - верхний предел шкалы датчика, из описателя
                    // аналогового входа базы данных.
                    fMaxOfScale = pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                  ucAiValueArrayBegin) + i].
                                  fMaxOfScale;
//                    // получим МАХ код аналогового сигнала.
//                    usMaxCode = (((unsigned short)(pxFunction1234Pointer ->
//                                                   ucMaxCodeH) << 8) |
//                                 (unsigned short)(pxFunction1234Pointer ->
//                                                  ucMaxCodeL));

                    // данные от модуля передается в виде целого числа Х в диапазоне от 0 до
                    // 8191 (0x1FFF), представляющего собой линеаризованный код, пропорциональный  вы-
                    // ходному токовому сигналу (4-20 мА). Для получения измеренного значения в физиче-
                    // ских единицах измеряемого параметра на стороне MASTER должно быть проведено
                    // масштабирование по формуле:
                    // Y = (X / 8191) (MAX – MIN) + MIN,
                    // где  МАХ – максимум шкалы, MIN – минимум шкалы датчика в физических величинах
                    // измеряемого параметра.
                    // В ПИТ-ТС МЕ, ПИТ-ТП МЕ возможны заходы сигнала за минимум или за макси-
                    // мум шкалы.
                    // произведём преобразование, получим измеренное значение в физиче-
                    // ских единицах измеряемого параметра.
                    fResult = ((((float)sCurrentMeasure /
                                 (float)usMaxCode) *
                                (fMaxOfScale - fMinOfScale)) +
                               fMinOfScale);

                    // поместим его в рабочий массив - измеренные значения входных аналоговых сигналов.
                    memcpy(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueBufferPointer[i * sizeof(float)]),
                           (unsigned char*)&fResult,
                           sizeof(float));
                    // сбросим флаг недостоверности - вход достоверен.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 0;
                    // сбросим флаги уставок LL, L, H, HH.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i *
                                                     ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // сбросим флаг недостоверности - дискретные сигналы уставок LL, L, H, HH достоверны.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // очистим байт - состояние аналоговых измерительных каналов, текущего канала.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] &=
                        ~(ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_L |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_H |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H);
                    // нарушена уставка LL + L?
                    if (fResult < (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointLL))
                    {
                        // установим флаг нарушения уставки LL + L.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                        // установим флаг нарушения уставки LL.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L_OFFSET]) = 1;
                        // установим флаг нарушения уставки L.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                    }
                    // нарушена уставка L?
                    if (fResult < (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointL))
                    {
                        // установим флаг нарушения уставки L.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                        // установим флаг нарушения уставки L.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                    }
                    // нарушена уставка H?
                    if (fResult > (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointH))
                    {
                        // установим флаг нарушения уставки H.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                        // установим флаг нарушения уставки H.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                    }
                    // нарушена уставка HH + H?
                    if (fResult > (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointHH))
                    {
                        // установим флаг нарушения уставки HH + H.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                        // установим флаг нарушения уставки H.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                        // установим флаг нарушения уставки HH.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H_OFFSET]) = 1;
                    }
                }
            }
            break;

        case IEEE754_BYTE_ORDER_MSB_LSB:
            // обработаем запрограммированное количество входов или выходов функционального блока.
            for (i = 0; i < (pxFunction1234Pointer -> ucIOQuantity); i++)
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
                    // установим флаги недостоверности - входы недостоверны.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 1;
                    // данные входов модуля выведены из обработки, обнулим их.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // установим флаги недостоверности - входы недостоверны.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           1,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // очистим байт - состояние аналоговых измерительных каналов, текущего канала.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] = 0;
                }
                else
                {
                    fResult = 0;
                    // получим измеренное значение текущего аналогового входа.
                    // big endian to little endian.
                    ((unsigned char*)&fResult)[0] = aucTempArray[(i * sizeof(float)) + 2];
                    ((unsigned char*)&fResult)[1] = aucTempArray[(i * sizeof(float)) + 3];
                    ((unsigned char*)&fResult)[2] = aucTempArray[(i * sizeof(float)) + 0];
                    ((unsigned char*)&fResult)[3] = aucTempArray[(i * sizeof(float)) + 1];
                    // поместим его в рабочий массив - измеренные значения входных аналоговых сигналов.
                    memcpy(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueBufferPointer[i * sizeof(float)]),
                           (unsigned char*)&fResult,
                           sizeof(float));
                    // сбросим флаг недостоверности - вход достоверен.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 0;
                    // сбросим флаги уставок LL, L, H, HH.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i *
                                                     ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // сбросим флаг недостоверности - дискретные сигналы уставок LL, L, H, HH достоверны.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // очистим байт - состояние аналоговых измерительных каналов, текущего канала.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] &=
                        ~(ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_L |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_H |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H);

                    // нарушена уставка LL + L?
                    if (fResult < (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointLL))
                    {
                        // установим флаг нарушения уставки LL + L.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                        // установим флаг нарушения уставки LL.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L_OFFSET]) = 1;
                        // установим флаг нарушения уставки L.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                    }
                    // нарушена уставка L?
                    if (fResult < (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointL))
                    {
                        // установим флаг нарушения уставки L.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                        // установим флаг нарушения уставки L.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                    }
                    // нарушена уставка H?
                    if (fResult > (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointH))
                    {
                        // установим флаг нарушения уставки H.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                        // установим флаг нарушения уставки H.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                    }
                    // нарушена уставка HH + H?
                    if (fResult > (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointHH))
                    {
                        // установим флаг нарушения уставки HH + H.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                        // установим флаг нарушения уставки H.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                        // установим флаг нарушения уставки HH.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H_OFFSET]) = 1;
                    }
                }
            }
            break;

        case IEEE754_BYTE_ORDER_LSB_MSB:
            // обработаем запрограммированное количество входов или выходов функционального блока.
            for (i = 0; i < (pxFunction1234Pointer -> ucIOQuantity); i++)
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
                    // установим флаги недостоверности - входы недостоверны.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 1;
                    // данные входов модуля выведены из обработки, обнулим их.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // установим флаги недостоверности - входы недостоверны.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           1,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // очистим байт - состояние аналоговых измерительных каналов, текущего канала.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] = 0;
                }
                else
                {
                    fResult = 0;
                    // получим измеренное значение текущего аналогового входа.
                    ((unsigned char*)&fResult)[0] = aucTempArray[(i * sizeof(float)) + 2];
                    ((unsigned char*)&fResult)[1] = aucTempArray[(i * sizeof(float)) + 3];
                    ((unsigned char*)&fResult)[2] = aucTempArray[(i * sizeof(float)) + 0];
                    ((unsigned char*)&fResult)[3] = aucTempArray[(i * sizeof(float)) + 1];
                    // поместим его в рабочий массив - измеренные значения входных аналоговых сигналов.
                    memcpy(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueBufferPointer[i * sizeof(float)]),
                           (unsigned char*)&fResult,
                           sizeof(float));
                    // сбросим флаг недостоверности - вход достоверен.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 0;
                    // сбросим флаги уставок LL, L, H, HH.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i *
                                                     ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // очистим байт - состояние аналоговых измерительных каналов, текущего канала.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] &=
                        ~(ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_L |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_H |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H);

                    // нарушена уставка LL + L?
                    if (fResult < (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointLL))
                    {
                        // установим флаг нарушения уставки LL + L.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                        // установим флаг нарушения уставки LL.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L_OFFSET]) = 1;
                        // установим флаг нарушения уставки L.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                    }
                    // нарушена уставка L?
                    if (fResult < (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointL))
                    {
                        // установим флаг нарушения уставки L.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                        // установим флаг нарушения уставки L.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                    }
                    // нарушена уставка H?
                    if (fResult > (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointH))
                    {
                        // установим флаг нарушения уставки H.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                        // установим флаг нарушения уставки H.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                    }
                    // нарушена уставка HH + H?
                    if (fResult > (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointHH))
                    {
                        // установим флаг нарушения уставки HH + H.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                        // установим флаг нарушения уставки H.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                        // установим флаг нарушения уставки HH.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H_OFFSET]) = 1;
                    }
                }
            }
            break;

        case IEEE754_BYTE_ORDER_LSR_MSR:
            // обработаем запрограммированное количество входов или выходов функционального блока.
            for (i = 0; i < (pxFunction1234Pointer -> ucIOQuantity); i++)
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
                    // установим флаги недостоверности - входы недостоверны.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 1;
                    // данные входов модуля выведены из обработки, обнулим их.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // установим флаги недостоверности - входы недостоверны.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucBadDiBufferPointer[i * ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           1,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // очистим байт - состояние аналоговых измерительных каналов, текущего канала.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] = 0;
                }
                else
                {
                    fResult = 0;
                    // получим измеренное значение текущего аналогового входа.
                    ((unsigned char*)&fResult)[0] = aucTempArray[(i * sizeof(float)) + 0];
                    ((unsigned char*)&fResult)[1] = aucTempArray[(i * sizeof(float)) + 1];
                    ((unsigned char*)&fResult)[2] = aucTempArray[(i * sizeof(float)) + 2];
                    ((unsigned char*)&fResult)[3] = aucTempArray[(i * sizeof(float)) + 3];
                    // поместим его в рабочий массив - измеренные значения входных аналоговых сигналов.
                    memcpy(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucAiValueBufferPointer[i * sizeof(float)]),
                           (unsigned char*)&fResult,
                           sizeof(float));
                    // сбросим флаг недостоверности - вход достоверен.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucBadAiBufferPointer[i] = 0;
                    // сбросим флаги уставок LL, L, H, HH.
                    memset(&(pxModuleContext ->
                             xModuleContextStatic.
                             pucDiValueBufferPointer[i *
                                                     ANALOGUE_INPUT_DI_VALUE_QUANTITY]),
                           0,
                           ANALOGUE_INPUT_DI_VALUE_QUANTITY);
                    // очистим байт - состояние аналоговых измерительных каналов, текущего канала.
                    pxModuleContext ->
                    xModuleContextStatic.
                    pucStatAiBufferPointer[i] &=
                        ~(ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_L |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_H |
                          ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H);

                    // нарушена уставка LL + L?
                    if (fResult < (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointLL))
                    {
                        // установим флаг нарушения уставки LL + L.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                        // установим флаг нарушения уставки LL.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L_OFFSET]) = 1;
                        // установим флаг нарушения уставки L.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                    }
                    // нарушена уставка L?
                    if (fResult < (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointL))
                    {
                        // установим флаг нарушения уставки L.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                        // установим флаг нарушения уставки L.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_L_OFFSET]) = 1;
                    }
                    // нарушена уставка H?
                    if (fResult > (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointH))
                    {
                        // установим флаг нарушения уставки H.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                        // установим флаг нарушения уставки H.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                    }
                    // нарушена уставка HH + H?
                    if (fResult > (pxAnalogueInputDescriptionWork[(pxFunction1234Pointer ->
                                   ucAiValueArrayBegin) + i].
                                   fSetPointHH))
                    {
                        // установим флаг нарушения уставки HH + H.
                        pxModuleContext ->
                        xModuleContextStatic.
                        pucStatAiBufferPointer[i] |=
                            ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                        // установим флаг нарушения уставки H.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_H_OFFSET]) = 1;
                        // установим флаг нарушения уставки HH.
                        (pxModuleContext ->
                         xModuleContextStatic.
                         pucDiValueBufferPointer[(i *
                                                  ANALOGUE_INPUT_DI_VALUE_QUANTITY) +
                                                 ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H_OFFSET]) = 1;
                    }
                }
            }
            break;

        default:
            break;
        };
    }
    else
    {
        // модуль признан неисправным?
        if ((pxModuleContext ->
                xModuleContextDinamic.
                nucBadAnswerCounter) == 0)
        {
            // получим код ошибки;
            (pxModuleContext ->
             xModuleContextDinamic.
             ucErrorCode) = CError::ModbusErrorToPlcError(errno);

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
                    ucModuleAddress,
                    (CEvents::HANDLED_EVENTS_EXTERNAL_MODULES_BAD_TYPE |
//                     CEvents::HANDLED_EVENTS_IS_POPUP |
//                     CEvents::HANDLED_EVENTS_IS_SOUND |
                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                    CError::ModbusErrorToPlcError(errno),
                    "Ошибка SL");
            }

            // установим флаги недостоверности - входы недостоверны.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucBadAiBufferPointer,
                   1,
                   (pxFunction1234Pointer -> ucIOQuantity));
            // установим флаги недостоверности всех дискретных сигналов модуля.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucBadDiBufferPointer,
                   1,
                   ((pxFunction1234Pointer -> ucIOQuantity) *
                    ANALOGUE_INPUT_DI_VALUE_QUANTITY));
            // данные входов модуля недостоверны, обнулим их.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucAiValueBufferPointer,
                   0,
                   (pxFunction1234Pointer -> ucIOQuantity) * sizeof(float));
            // дискретные данные входов модуля недостоверны, обнулим их.
            memset(pxModuleContext ->
                   xModuleContextStatic.
                   pucDiValueBufferPointer,
                   0,
                   (pxFunction1234Pointer -> ucIOQuantity) *
                   ANALOGUE_INPUT_DI_VALUE_QUANTITY);
            // отправим в буфер - состояние аналоговых измерительных каналов - обрыв линии канала.
            for (i = 0; i < (pxFunction1234Pointer -> ucIOQuantity); i++)
            {
                pxModuleContext ->
                xModuleContextStatic.
                pucStatAiBufferPointer[i] |=
                    ANALOGUE_INPUT_LINE_BREAK;
            }
            //iBadModuleBuffUpdate();
        }
        else
        {
            // уменьшим значение счётчика - "количество сеансов связи с модулем без ответа".
            (pxModuleContext ->
             xModuleContextDinamic.
             nucBadAnswerCounter)--;
        }
    }

    pthread_mutex_unlock(&xMainDataAccessMutex);
    return 0;
}
