
#include "PasNewConfig.h"
#include "ModuleMvdi5.h"
#include "Events.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// производит обмен данными с модулем дискретного ввода - MVDI5.
int iModuleMvdiDataHandler(TModuleContext *pxModuleContext)
{
    unsigned char ucData;
    int i;
    int j;
    unsigned char *pucDestination;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // обмен данными по SPI.
    auiSpiTxBuffer[0] = MVDI5_GET_DISCRETE_INPUT_DATA_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MVDI5_GET_DISCRETE_INPUT_DATA_COMMAND_ANSWER_LENGTH +
                                     ONE_BYTE_CRC_LENGTH,
                                     SPEED_IN_HZ
                                    );

    // модуль исправен?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVDI5_GET_DISCRETE_INPUT_DATA_COMMAND) // if echo answer is ok.
    {
        // данные не повреждены?
        if ((iCrcSummOneByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                    MVDI5_GET_DISCRETE_INPUT_DATA_COMMAND_ANSWER_LENGTH) > 0) &&
                ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                                 MVDI5_GET_DISCRETE_INPUT_DATA_COMMAND_ANSWER_LENGTH]) != 0))
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
                    ucModuleContextIndex,
                    (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                    MTVI5_BAD_ANSWER_ERROR_OFFSET,
                    "Норм. обмена");
            }

            // что ответил модуль?
            switch(auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET])
            {
            case MVDI5_DATA_READY:
                // данные модуля достоверны.
                memset(pxModuleContext ->
                       xModuleContextStatic.
                       pucBadDiBufferPointer,
                       0,
                       MVDI5_DISCRETE_INPUT_QUANTITY);
                // отправим полученные от модуля данные в рабочий массив Modbus прибора.
                pucDestination = pxModuleContext ->
                                 xModuleContextStatic.
                                 pucDiValueBufferPointer;
                for (i = 0; i < MVDI5_DISCRETE_INPUT_BYTE_QUANTITY; i++)
                {
                    ucData = (auiSpiRxBuffer[i + SPI_DATA_BYTE_OFFSET]);

                    for (j = 0; j < MVDI5_DISCRETE_INPUT_BITS_IN_BYTE_QUANTITY; j++)
                    {
                        // инвертируем значение. в модуле нормально разомкнутый контакт - 1, нормально замкнутый - 0.
                        *pucDestination++ = !((ucData & (1 << j)) ? 1 : 0);
                    }
                }
                break;

            case MVDI5_DATA_NOT_READY:
                break;

            default:
                break;
            };
            return 0;
        }
    }

    // модуль признан неисправным?
    if ((pxModuleContext ->
            xModuleContextDinamic.
            nucBadAnswerCounter) == 0)
    {
        // модуль не исправен.
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

        // данные модуля недостоверны.
        memset(pxModuleContext ->
               xModuleContextStatic.
               pucBadDiBufferPointer,
               1,
               MVDI5_DISCRETE_INPUT_QUANTITY);
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
