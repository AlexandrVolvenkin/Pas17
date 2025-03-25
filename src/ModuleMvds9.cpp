
#include "PasNewConfig.h"
#include "ModuleMvds9.h"
#include "Events.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// производит обмен данными с модулем дискретного ввода - MVDS9.
int iModuleMvds9DataHandler(TModuleContext *pxModuleContext)
{
    unsigned char ucData;
    int i;
    int j;
    unsigned char *pucDestination;
    unsigned char *pucDestination2;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // обмен данными по SPI.
    auiSpiTxBuffer[0] = MVDS9_GET_DISCRETE_INPUT_DATA_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
            xModuleContextStatic.
            ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MVDS9_GET_DISCRETE_INPUT_DATA_COMMAND_ANSWER_LENGTH +
                                     ONE_BYTE_CRC_LENGTH,
                                     SPEED_IN_HZ
                                    );
    // модуль исправен?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVDS9_GET_DISCRETE_INPUT_DATA_COMMAND) // if echo answer is ok.
    {
        // данные не повреждены?
        if ((iCrcSummOneByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                    MVDS9_GET_DISCRETE_INPUT_DATA_COMMAND_ANSWER_LENGTH) > 0) &&
                ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                                 MVDS9_GET_DISCRETE_INPUT_DATA_COMMAND_ANSWER_LENGTH]) != 0))
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
            // что ответил модуль?
            switch(auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET])
            {
            case MVDS9_DATA_READY:
                // данные модуля достоверны.
                memset(pxModuleContext ->
                       xModuleContextStatic.
                       pucBadDiBufferPointer,
                       0,
                       MVDS9_DISCRETE_INPUT_QUANTITY);
                // отправим полученные от модуля данные в рабочий массив Modbus прибора.
                pucDestination = pxModuleContext ->
                                 xModuleContextStatic.
                                 pucDiValueBufferPointer;
                pucDestination2 = pxModuleContext ->
                                  xModuleContextStatic.
                                  pucBadDiBufferPointer;

                for (i = 0; i < MVDS9_DISCRETE_INPUT_BYTE_QUANTITY; i++)
                {
                    ucData = (auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + i]);

                    for (j = 0; j < MVDS9_DISCRETE_INPUT_BITS_IN_BYTE_QUANTITY; j++)
                    {
                        // четные биты (D0,D2,D4,D6) = 0 - разомкнуто (OFF), = 1 - замкнуто (ON)
                        // нечетные биты (D1,D3,D5,D7) = 0 - достоверно, = 1 - недостоверно
                        // бит нечётный?
                        if (j & 0x01)
                        {
                            // бит достоверен? 0 - достоверно, 1 - недостоверно.
                            *pucDestination2++ = ((ucData & (1 << j)) ? 1 : 0);
                        }
                        else
                        {
                            //контакты замкнуты?  0 - разомкнуто (OFF), 1 - замкнуто (ON)
                            *pucDestination++ = ((ucData & (1 << j)) ? 1 : 0);
                        }
                    }
                }
                break;

            case MVDS9_DATA_NOT_READY:
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
        // модуль неисправен.
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
               MVDS9_DISCRETE_INPUT_QUANTITY);
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
// сравнивает внутреннюю базу данных модуля с базой в памяти прибора.
// если не совпадает, запишем базу из модуля в прибор.
int iModuleMvds9PlcDataBaseCheck(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // обмен данными по SPI.
    auiSpiTxBuffer[0] = MVDS9_GET_DATA_BASE_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
            xModuleContextStatic.
            ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MVDS9_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH,
                                     LOW_SPEED_IN_HZ);

    // модуль исправен?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVDS9_GET_DATA_BASE_COMMAND) // if echo answer is ok.
    {
        // данные не повреждены?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   MVDS9_DATA_BASE_BLOCK_LENGTH) > 0)
        {
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET];
            // получим указатель на блок в базе данных.
            pucDestination = (unsigned char*)((pxModuleContext ->
                                               xModuleContextStatic.
                                               pucDataBasePointer) +
                                              (pxModuleContext ->
                                               xModuleContextStatic.
                                               ucDataBaseDataOffset));
            // база данных модуля совпадает с базой в памяти прибора?
            if (memcmp((const void*)pucDestination,
                       (const void*)pucSource,
                       MVDS9_DATA_BASE_BLOCK_LENGTH) != 0)
            {
                // база данных модуля не совпадает с базой в памяти прибора.
                // сохраним в памяти прибора базу данных прочитанную из модуля.
                memcpy(pucDestination,
                       pucSource,
                       MVDS9_DATA_BASE_BLOCK_LENGTH);
                cout << "iModuleMvds9PlcDataBaseCheck NO" << endl;
                return -1;
            }
            else
            {
                cout << "iModuleMvds9PlcDataBaseCheck OK" << endl;
                return 0;
            }
        }
    }
    return -1;
}

//-----------------------------------------------------------------------------------------------------
// сравнивает внутреннюю базу данных модуля с базой в памяти прибора.
// если не совпадает, запишем базу из прибора в модуль.
int iModuleMvds9ModuleDataBaseCheck(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    unsigned short usCrc;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // обмен данными по SPI.
    auiSpiTxBuffer[0] = MVDS9_GET_DATA_BASE_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
            xModuleContextStatic.
            ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MVDS9_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH,
                                     LOW_SPEED_IN_HZ);

    // модуль исправен?
    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MVDS9_GET_DATA_BASE_COMMAND) // if echo answer is ok.
    {
        // данные не повреждены?
        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
                                   MVDS9_DATA_BASE_BLOCK_LENGTH) > 0)
        {
            pucSource = &auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET];
            // получим указатель на блок в базе данных.
            pucDestination = (unsigned char*)((pxModuleContext ->
                                               xModuleContextStatic.
                                               pucDataBasePointer) +
                                              (pxModuleContext ->
                                               xModuleContextStatic.
                                               ucDataBaseDataOffset));
            // база данных модуля совпадает с базой в памяти прибора?
            if (memcmp((const void*)pucDestination,
                       (const void*)pucSource,
                       MVDS9_DATA_BASE_BLOCK_LENGTH) != 0)
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
                    // база данных модуля не совпадает с базой в памяти прибора.
                    // сохраним в памяти модуля базу данных прочитанную из прибора.
                    iModuleMvds9DataBaseBlockWrite(pxModuleContext);
                    cout << "New data base saved in module" << endl;
                }
                cout << "iModuleMvds9ModuleDataBaseCheck NO" << endl;
                return -1;
            }
            else
            {
                cout << "iModuleMvds9ModuleDataBaseCheck OK" << endl;
                return 0;
            }
        }
    }
    return -1;
}

//-----------------------------------------------------------------------------------------------------
// записывает базу данных из RAM прибора в EEPROM модуля.
int iModuleMvds9DataBaseBlockWrite(TModuleContext *pxModuleContext)
{
    unsigned short usData;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    // получим указатель на блок в базе данных.
    pucSource =
        ((pxModuleContext ->
          xModuleContextStatic.
          pucDataBasePointer) +
         (pxModuleContext ->
          xModuleContextStatic.
          ucDataBaseDataOffset));

    pucDestination = &auiSpiTxBuffer[0];
    *pucDestination++ = MVDS9_SET_DATA_BASE_COMMAND;
    *pucDestination++ = 0;
    *pucDestination++ = *pucSource++;
    *pucDestination++ = *pucSource;
    usData = usCrcSummTwoByteCalculation(&auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET],
                                         MVDS9_DATA_BASE_BLOCK_LENGTH);
    *pucDestination++ = (unsigned char)usData;
    *pucDestination = (unsigned char)(usData >> 8);

    // обмен данными по SPI.
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
            xModuleContextStatic.
            ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MVDS9_DATA_BASE_BLOCK_LENGTH +
                                     TWO_BYTE_CRC_LENGTH +
                                     TAIL_ANSWER_LENGTH,
                                     LOW_SPEED_IN_HZ);
    // база данных записана в память модуля?
    if ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
                        MVDS9_DATA_BASE_BLOCK_LENGTH +
                        TWO_BYTE_CRC_LENGTH]) ==
            MVDS9_CHECK_DATA_BASE_WRITE_ANSWER_OK)
    {
        return 0;
    }
    return -1;
}
