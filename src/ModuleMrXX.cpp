

//-----------------------------------------------------------------------------------------------------
#include "PasNewConfig.h"
#include "Platform.h"
#include "ModuleMrXX.h"
#include "Events.h"
#include "SystemComponent.h"

using namespace std;

static unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
static  unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

//void vModuleMrXXDataHandlerNew(TModuleContext *pxModuleContext)
//{
//    switch(pxModuleContext ->
//            xModuleContextDinamic.
//            ucFsmState)
//    {
//    case MRXX_FSM_STATE_CHANGE_PROTOCOL:
//        iModuleMrXXChangeProtocol(pxModuleContext);
//        pxModuleContext ->
//        xModuleContextDinamic.
//        ucFsmState = MRXX_FSM_STATE_WRITE_DATABASE;
//        break;
//
//    case MRXX_FSM_STATE_TRANSFER:
//        iModuleMrXXTransferData(pxModuleContext);
//        break;
//        // при старте база данных записывается из прибора в модуль реле.
//    case MRXX_FSM_STATE_WRITE_DATABASE:
//        iModuleMrXXDataBaseBlockWrite(pxModuleContext);
//        // после, автомат переходит в режим - обмен данными с модулем(MRXX_FSM_STATE_TRANSFER).
//        pxModuleContext ->
//        xModuleContextDinamic.
//        ucFsmState = MRXX_FSM_STATE_TRANSFER;
//        break;
//
//    default:
//        break;
//    };
//}

//-----------------------------------------------------------------------------------------------------
// производит обмен данными с модулем дискретного вывода - MR.
int iModuleMrXXTransferData(TModuleContext *pxModuleContext)
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
        cout << "fbGlobalModbusKvit ok" << endl;
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
        cout << "fbGlobalModbusReset ok" << endl;
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

//    if (ucLocalBou)
//    {
//        cout  << "ucLocalBou" << " " <<  hex << uppercase << setw(2) << setfill('0') << (unsigned int)ucLocalBou << endl;
//    }

    *pucDestination++ = ucLocalBou;
    *pucDestination = HammingDistanceCrc(&auiSpiTxBuffer[0],
                                         (SPI_PREAMBLE_LENGTH +
                                          MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH));

//    cout  << "HammingDistanceCrc" << " " <<  hex << uppercase << setw(2) << setfill('0') << (unsigned int)*pucDestination << endl;

//    *pucDestination++ = (unsigned char)usData;
//    *pucDestination++ = (unsigned char)(usData >> 8);
//    *pucDestination++ = DATA_EXCHANGE_OK;
//
//if (auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET])
//{
//    cout << "SPI_DATA_BYTE_OFFSET" << endl;
//    cout << "auiSpiTxBuffer" << endl;
//    unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)auiSpiTxBuffer;
//    for(int i=0; i<16; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }
//}
//
//if (auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET + 1])
//    {
//        cout << "SPI_DATA_BYTE_OFFSET + 1" << endl;
//        cout << "auiSpiTxBuffer" << endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)auiSpiTxBuffer;
//        for(int i=0; i<32; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//    }


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


//if (auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET])
//    {
//        cout << "SPI_DATA_BYTE_OFFSET" << endl;
//        cout << "auiSpiRxBuffer" << endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
//        for(int i=0; i<16; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//    }
//
//if (auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET + 1])
//{
//        cout << "SPI_DATA_BYTE_OFFSET + 1" << endl;
//        cout << "auiSpiRxBuffer" << endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
//        for(int i=0; i<32; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//}


    // Данные не повреждены?
    if (auiSpiRxBuffer[(SPI_MT_RX_OFFSET +
                        SPI_MT_PRIAMBLE_LENGTH +
                        auiSpiRxBuffer[SPI_MT_RX_OFFSET + SPI_MT_LENGTH_OFFSET])] ==
            HammingDistanceCrc(&auiSpiRxBuffer[SPI_MT_RX_OFFSET],
                               (SPI_MT_PRIAMBLE_LENGTH +
                                auiSpiRxBuffer[SPI_MT_RX_OFFSET + SPI_MT_LENGTH_OFFSET])))
    {
//        cout << "HammingDistanceCrc ok" << endl;
        return 0;
    }
    else
    {
//        cout << "HammingDistanceCrc error" << endl;
        return -1;
    }
}

//-----------------------------------------------------------------------------------------------------
int iModuleMrXXTransferDataReceive(TModuleContext *pxModuleContext)
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

    // модуль исправен.
    pxModuleContext ->
    xModuleContextDinamic.
    usAuxiliaryCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT; //MRXX database error Debug

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

    // сбросим флаги недостоверности состояния выходов реле.
    memset(pxModuleContext ->
           xModuleContextStatic.
           pucBadDoBufferPointer,
           0,
           MRXX_BAD_DO_BIT_ARRAY_LENGTH);
    //iBadModuleBuffUpdate();
    // получим указатель на буфер - состояние выходов модулей реле МР №1 – MP №8.
    pucDestination = pxModuleContext ->
                     xModuleContextStatic.
                     pucSostDoBufferPointer;
    ucData = (auiSpiRxBuffer[(SPI_MT_RX_OFFSET + MRXX_RELAY_STATE_OFFSET)]);
    // отправим данные - состояние выходов модулей реле МР №1 – MP №8.
    for (int i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
    {
        if (ucData & (1 << i))
        {
            *pucDestination++ = 1;
            // Архивировать событие?
            if (pxMrXXOneChannelDataBase[i].ucControl & ARCHIVE_MRXX_MASK)
            {
                // активное состояние события ещё не зарегистрировано?
                if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                            pxModuleContext ->
                            xModuleContextStatic.
                            ucModuleContextIndex,
                            MTVI5_OUT_DAC_ERROR_OFFSET + i,
                            MRXX_ERROR_CODE_RELAY_ON_OFF))
                {
                    // зарегистрируем активное состояние события.
                    // включение реле - i.
                    CEvents::EventRegistration(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        (CEvents::HANDLED_EVENTS_MODULES_EVENTS_TYPE |
                         CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                        MTVI5_OUT_DAC_ERROR_OFFSET + i,
                        "Вкл. реле");
                }
            }
        }
        else
        {
            *pucDestination++ = 0;
            // Архивировать событие?
            if (pxMrXXOneChannelDataBase[i].ucControl & ARCHIVE_MRXX_MASK)
            {
                // неактивное состояние события ещё не зарегистрировано?
                if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
                            pxModuleContext ->
                            xModuleContextStatic.
                            ucModuleContextIndex,
                            MTVI5_OUT_DAC_ERROR_OFFSET + i,
                            MRXX_ERROR_CODE_RELAY_ON_OFF))
                {
                    // зарегистрируем неактивное состояние события.
                    // выключение реле - i.
                    CEvents::EventRegistration(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        (CEvents::HANDLED_EVENTS_MODULES_EVENTS_TYPE |
                         CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                        MTVI5_OUT_DAC_ERROR_OFFSET + i,
                        "Откл. реле");
                }
            }
        }
    }

    // некоторые типы модулей имеют дополнительную возможность контроля:
    // достоверность состояния выходов реле - MR51;
    // обрыв обмотки реле и короткое замыкание обмотки реле - MR53.
    switch(pxModuleContext ->
            xModuleContextStatic.
            ucModuleType)
    {
    case MODULE_TYPE_MR51:
        // Только для МР-51:
        // Байт сост. ВЫХ по ОС rel_OS: D0 - обр. связь реле №1 ВКЛ., D7 - обр. связь реле №8 ВКЛ.
        // Должен совпадать с байтом Упр. ВЫХ r_out, иначе ошибка, несовпадающий бит-неисправное реле
        for (int i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
        {
            // биты состояния выходов по обратной связи и выходов упраления реле совпадают?
            if (((auiSpiRxBuffer[(SPI_MT_RX_OFFSET + MRXX_RELAY_STATE_OFFSET)]) >> i) ==
                    ((auiSpiRxBuffer[(SPI_MT_RX_OFFSET + MRXX_RELAY_STATE_OFFSET)]) >> i))
            {
                // биты состояния выходов по обратной связи и выходов упраления реле совпадают.
                // сбросим флаг недостоверности выхода.
                pxModuleContext ->
                xModuleContextStatic.
                pucBadDoBufferPointer[i] = 0;
            }
            else
            {
                // биты состояния выходов по обратной связи и выходов упраления реле не совпадают.
                // установим флаг недостоверности выхода.
                pxModuleContext ->
                xModuleContextStatic.
                pucBadDoBufferPointer[i] = 1;
            }
        }
//                return 0;
        break;
    case MODULE_TYPE_MR53:
        // байты состояния обмоток 8-ми реле модуля. только МР53.
        // байт - 2: состояния обмоток реле, если бит = 1 - короткое замыкание.
        // #define MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET 0
        // байт - 3: состояния обмоток реле, если бит = 1 - обрыв.
        // #define MRXX_STAT_DO_BREACK_BIT_OFFSET 1
        for (int i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
        {
            // есть обрыв или короткое замыкание обмоток реле?
            if (((((auiSpiRxBuffer[(SPI_MT_RX_OFFSET + MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET)]) >> i) & 0x01) != 0) ||
                    (((auiSpiRxBuffer[(SPI_MT_RX_OFFSET + MRXX_STAT_DO_BREACK_BIT_OFFSET)]) >> i) & 0x01) != 0)
            {
                // есть обрыв или короткое замыкание обмоток реле.
                // установим флаг недостоверности выхода.
                pxModuleContext ->
                xModuleContextStatic.
                pucBadDoBufferPointer[i] = 1;
            }
            else
            {
                // нет обрыва или короткого замыкания обмоток реле.
                // сбросим флаг недостоверности выхода.
                pxModuleContext ->
                xModuleContextStatic.
                pucBadDoBufferPointer[i] = 0;
            }
        }

        for (int i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
        {
            // есть короткое замыкание обмотки реле?
            if (((auiSpiRxBuffer[(SPI_MT_RX_OFFSET + MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET)]) >> i) & 0x01)
            {
                // есть короткое замыкание обмотки реле.
                // установим флаг короткого замыкания обмотки реле.
                (pxModuleContext ->
                 xModuleContextStatic.
                 pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
                                        MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET]) = 1;

                // активное состояние события ещё не зарегистрировано?
                if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                            pxModuleContext ->
                            xModuleContextStatic.
                            ucModuleContextIndex,
                            MTVI5_OUT_DAC_ERROR_OFFSET + i,
                            MRXX_ERROR_CODE_RELAY_SHORT_CIRCUIT))
                {
                    // зарегистрируем активное состояние события.
                    // включение реле - i.
                    CEvents::EventRegistration(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                         CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                        MTVI5_OUT_DAC_ERROR_OFFSET + i,
                        "Кз. обмотки");
                }
            }
            else
            {
                // нет короткого замыкания обмотки реле.
                // сбросим флаг короткого замыкания обмотки реле.
                (pxModuleContext ->
                 xModuleContextStatic.
                 pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
                                        MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET]) = 0;

                // неактивное состояние события ещё не зарегистрировано?
                if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
                            pxModuleContext ->
                            xModuleContextStatic.
                            ucModuleContextIndex,
                            MTVI5_OUT_DAC_ERROR_OFFSET + i,
                            MRXX_ERROR_CODE_RELAY_SHORT_CIRCUIT))
                {
                    // зарегистрируем неактивное состояние события.
                    // выключение реле - i.
                    CEvents::EventRegistration(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                         CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                        MTVI5_OUT_DAC_ERROR_OFFSET + i,
                        "Норм. кз. об");
                }
            }

            // есть обрыв обмотки реле?
            if (((auiSpiRxBuffer[(SPI_MT_RX_OFFSET + MRXX_STAT_DO_BREACK_BIT_OFFSET)]) >> i) & 0x01)
            {
                // есть обрыв обмотки реле.
                // установим флаг обрыва обмотки реле.
                (pxModuleContext ->
                 xModuleContextStatic.
                 pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
                                        MRXX_STAT_DO_BREACK_BIT_OFFSET]) = 1;

                // активное состояние события ещё не зарегистрировано?
                if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
                            pxModuleContext ->
                            xModuleContextStatic.
                            ucModuleContextIndex,
                            MTVI5_OUT_DAC_ERROR_OFFSET + i,
                            MRXX_ERROR_CODE_RELAY_BREACK))
                {
                    // зарегистрируем активное состояние события.
                    // включение реле - i.
                    CEvents::EventRegistration(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                         CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                        MTVI5_OUT_DAC_ERROR_OFFSET + i,
                        "Обр. обмотки");
                }
            }
            else
            {
                // нет обрыва обмотки реле.
                // сбросим флаг обрыва обмотки реле.
                (pxModuleContext ->
                 xModuleContextStatic.
                 pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
                                        MRXX_STAT_DO_BREACK_BIT_OFFSET]) = 0;

                // неактивное состояние события ещё не зарегистрировано?
                if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
                            pxModuleContext ->
                            xModuleContextStatic.
                            ucModuleContextIndex,
                            MTVI5_OUT_DAC_ERROR_OFFSET + i,
                            MRXX_ERROR_CODE_RELAY_BREACK))
                {
                    // зарегистрируем неактивное состояние события.
                    // выключение реле - i.
                    CEvents::EventRegistration(
                        pxModuleContext ->
                        xModuleContextStatic.
                        ucModuleContextIndex,
                        (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
                         CEvents::HANDLED_EVENTS_IS_ARCHIVE),
                        MTVI5_OUT_DAC_ERROR_OFFSET + i,
                        "Норм обр. об");
                }
            }
        }

        return 0;
        break;

    default:
        break;
    };

    return 0;
}

//-----------------------------------------------------------------------------------------------------
int iModuleMrXXTransferDataReceiveError(TModuleContext *pxModuleContext)
{
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

        // установим флаги недостоверности состояния выходов реле.
        memset(pxModuleContext ->
               xModuleContextStatic.
               pucBadDoBufferPointer,
               1,
               MRXX_BAD_DO_BIT_ARRAY_LENGTH);
        //iBadModuleBuffUpdate();
        return -1;
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


////-----------------------------------------------------------------------------------------------------
//// производит обмен данными с модулем дискретного вывода - MR.
//int iModuleMrXXTransferData(TModuleContext *pxModuleContext)
//{
//    unsigned char ucData;
//    unsigned short usData;
//    unsigned char ucLocalKvit;
//    unsigned char ucLocalReset;
//    // ucLocalBou байт управления модулем реле.
//    // Байт упр-я f_bou: флаги управления МР:  D0-"квит.от РС", D1-"сброс от РС"
//    // D2-кнопка"квит.Ц", D3-кнопка"сброс Ц", D4-флаг "блок.ВКЛ/ОТКЛ".
//    // биты управления МР байта f_bou.
//    // квитирование с верхнего уровня.
//    // #define GLOBAL_KVIT_PC_BIT 0
//    // сброс с верхнего уровня.
//    // #define GLOBAL_RESET_PC_BIT 1
//    // квитирование с цетральной панели МИНД.
//    // #define GLOBAL_KVIT_BUTTON_BIT 2
//    // сброс с цетральной панели МИНД.
//    // #define GLOBAL_RESET_BUTTON_BIT 3
//    // режим блокировки.
//    // #define GLOBAL_BLOCK_BIT 4
//    unsigned char ucLocalBou;
//    int i;
//    unsigned char *pucSource;
//    unsigned char *pucDestination;
//    TMrXXOneChannelDataBase *pxMrXXOneChannelDataBase;
//
//    // получим указатель на блок в базе данных.
//    pxMrXXOneChannelDataBase =
//        (TMrXXOneChannelDataBase*)((pxModuleContext ->
//                                    xModuleContextStatic.
//                                    pucDataBasePointer) +
//                                   (pxModuleContext ->
//                                    xModuleContextStatic.
//                                    ucDataBaseDataOffset));
//
//    ucLocalKvit = 0;
//    // цикл проверяет наличие внешнего квитирования.
//    for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//    {
//        // для текущего входа внешнее квитирование запрограммировано?
//        if (pxMrXXOneChannelDataBase[i].ucExternalKvitAddress)
//        {
//            // для текущего входа есть внешнее квитирование?
//            if (*((pxModuleContext ->
//                    xModuleContextStatic.
//                    pucDiValueOriginBufferPointer) +
//                    (pxMrXXOneChannelDataBase[i].
//                     ucExternalKvitAddress) - 1))
//            {
//                // установим квитирование для текущего входа.
//                ucLocalKvit |= (1 << i);
//            }
//        }
//    }
//
//    ucLocalReset = 0;
//    // цикл проверяет наличие внешнего сброса.
//    for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//    {
//        // для текущего входа внешний сброс запрограммирован?
//        if (pxMrXXOneChannelDataBase[i].ucExternalResetAddress)
//        {
//            // для текущего входа есть внешний сброс?
//            if (*((pxModuleContext ->
//                    xModuleContextStatic.
//                    pucDiValueOriginBufferPointer) +
//                    (pxMrXXOneChannelDataBase[i].
//                     ucExternalResetAddress) - 1))
//            {
//                // установим сброс для текущего входа.
//                ucLocalReset |= (1 << i);
//            }
//        }
//    }
//
//    ucLocalBou = 0;
//    // есть квитирование Modbus?
//    if (pxModuleContext ->
//            xModuleContextDinamic.
//            fbGlobalModbusKvit)
//    {
//        cout << "fbGlobalModbusKvit" << endl;
//        // сбросим флаг поступившей команды.
//        pxModuleContext ->
//        xModuleContextDinamic.
//        fbGlobalModbusKvit = 0;
//        // установим флаг управления - квитирование Modbus.
//        ucLocalBou |= (1 << GLOBAL_KVIT_PC_BIT);
//    }
//    // есть сброс Modbus?
//    if (pxModuleContext ->
//            xModuleContextDinamic.
//            fbGlobalModbusReset)
//    {
//        cout << "fbGlobalModbusReset" << endl;
//        // сбросим флаг поступившей команды.
//        pxModuleContext ->
//        xModuleContextDinamic.
//        fbGlobalModbusReset = 0;
//        // установим флаг управления - сброс Modbus.
//        ucLocalBou |= (1 << GLOBAL_RESET_PC_BIT);
//    }
//    // есть квитирование с кнопки центральной панели?
//    if (pxModuleContext ->
//            xModuleContextDinamic.
//            fbGlobalButtonKvit)
//    {
//        // сбросим флаг поступившей команды.
//        pxModuleContext ->
//        xModuleContextDinamic.
//        fbGlobalButtonKvit = 0;
//        // установим флаг управления - квитирование с центральной кнопки.
//        ucLocalBou |= (1 << GLOBAL_KVIT_BUTTON_BIT);
//    }
//    // есть сброс с кнопки центральной панели?
//    if (pxModuleContext ->
//            xModuleContextDinamic.
//            fbGlobalButtonReset)
//    {
//        // сбросим флаг поступившей команды.
//        pxModuleContext ->
//        xModuleContextDinamic.
//        fbGlobalButtonReset = 0;
//        // установим флаг управления - сброс с центральной кнопки.
//        ucLocalBou |= (1 << GLOBAL_RESET_BUTTON_BIT);
//    }
//    // включен режим блокировки?
////    if (pxModuleContext ->
////            xModuleContextDinamic.
////            fbGlobalModbusBlock)
//    if (fbGlobalBlock)
//    {
////        // сбросим флаг поступившей команды.
////        pxModuleContext ->
////        xModuleContextDinamic.
////        fbGlobalModbusBlock = 0;
//        // установим флаг управления - блокировка Modbus.
//        ucLocalBou |= (1 << GLOBAL_BLOCK_BIT);
//    }
//    // нет повреждений баз данных?
//    if ((!fbModuleMrOutOff) || (!fbPlcErrorReset))
//    {
//        pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET];
//        pucSource = (pxModuleContext ->
//                     xModuleContextStatic.
//                     pucDoValueBlockBufferPointer);
//        ucData = 0;
//        // заполним требования включения для каждого реле.
//        for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//        {
//            if (pucSource[i])
//            {
//                ucData |= (1 << i);
//            }
//            else
//            {
//
//            }
//        }
//        *pucDestination++ = ucData;
////        cout << "iModuleMrXXTransferData ucData 1 " << std::showbase << std::dec << (int)ucData << std::endl;
//
//        pucSource = (pxModuleContext ->
//                     xModuleContextStatic.
//                     pucDoValueBufferPointer);
//        ucData = 0;
//        // заполним новые нарушения для каждого реле.
//        for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//        {
//            if (pucSource[i])
//            {
//                ucData |= (1 << i);
//            }
//            else
//            {
//
//            }
//        }
//        *pucDestination++ = ucData;
////        cout << "iModuleMrXXTransferData ucData 2 " << std::showbase << std::dec << (int)ucData << std::endl;
//    }
//    else
//    {
//        // есть повреждения баз данных.
//        *pucDestination++ = 0;
//        *pucDestination++ = 0;
//    }
//
//
//    *pucDestination++ = ucLocalKvit;
//    *pucDestination++ = ucLocalReset;
//    *pucDestination++ = ucLocalBou;
//    usData = usCrcSummTwoByteCalculation(&auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET],
//                                         MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH);
//    *pucDestination++ = (unsigned char)usData;
////    *pucDestination++ = (unsigned char)(usData >> 8);
////    *pucDestination++ = DATA_EXCHANGE_OK;
//
//    auiSpiTxBuffer[0] = MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND;
//    auiSpiTxBuffer[1] = 7;
////
////if (auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET])
////{
////        cout << "SPI_DATA_BYTE_OFFSET" << endl;
////        cout << "auiSpiTxBuffer" << endl;
////        unsigned char *pucSourceTemp;
////        pucSourceTemp = (unsigned char*)auiSpiTxBuffer;
////        for(int i=0; i<32; )
////        {
////            for(int j=0; j<8; j++)
////            {
////                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////            }
////            cout << endl;
////            i += 8;
////        }
////}
////
////if (auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET + 1])
////{
////        cout << "SPI_DATA_BYTE_OFFSET + 1" << endl;
////        cout << "auiSpiTxBuffer" << endl;
////        unsigned char *pucSourceTemp;
////        pucSourceTemp = (unsigned char*)auiSpiTxBuffer;
////        for(int i=0; i<32; )
////        {
////            for(int j=0; j<8; j++)
////            {
////                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////            }
////            cout << endl;
////            i += 8;
////        }
////}
//
//
//    xSpiCommunicationDevice.Exchange(pxModuleContext ->
//xModuleContextStatic.
//ucModuleAddress,
//                 auiSpiTxBuffer,
//                 auiSpiRxBuffer,
//                 SPI_PREAMBLE_LENGTH +
//                 MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH +
//                 ONE_BYTE_CRC_LENGTH,
//                 SPEED_IN_HZ
//                );
//
//
////if (auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET])
////{
////        cout << "SPI_DATA_BYTE_OFFSET" << endl;
////        cout << "auiSpiRxBuffer" << endl;
////        unsigned char *pucSourceTemp;
////        pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
////        for(int i=0; i<32; )
////        {
////            for(int j=0; j<8; j++)
////            {
////                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////            }
////            cout << endl;
////            i += 8;
////        }
////}
////
////if (auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET + 1])
////{
////        cout << "SPI_DATA_BYTE_OFFSET + 1" << endl;
////        cout << "auiSpiRxBuffer" << endl;
////        unsigned char *pucSourceTemp;
////        pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
////        for(int i=0; i<32; )
////        {
////            for(int j=0; j<8; j++)
////            {
////                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////            }
////            cout << endl;
////            i += 8;
////        }
////}
//
//
//    // что ответил модуль?
//    switch(auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET])
//    {
//    case MRXX_DATA_READY:
//        // данные не повреждены?
//        if (iCrcSummTwoByteCompare(&auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET],
//                                   MRXX_GET_DISCRETE_OUTPUT_DATA_COMMAND_ANSWER_LENGTH) > 0)
//        {
//            // модуль исправен.
//            pxModuleContext ->
//            xModuleContextDinamic.
//            usAuxiliaryCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT; //MRXX database error Debug
//
//            // модуль исправен.
//            pxModuleContext ->
//            xModuleContextDinamic.
//            nucBadAnswerCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT;
//            // сбросим флаг отказа модуля.
//            *(pxModuleContext ->
//              xModuleContextStatic.
//              pucModuleBadStateBufferPointer) = BAD_MODULE_RESPONDED_OK;
//
//            // неактивное состояние события ещё не зарегистрировано?
//            if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        ucModuleContextIndex,
//                        MTVI5_BAD_ANSWER_ERROR_OFFSET))
//            {
//                // зарегистрируем неактивное состояние события.
//                // норма обмена данными.
//                CEvents::EventRegistration(
//                    pxModuleContext ->
//                    xModuleContextStatic.
//                    ucModuleContextIndex,
//                    (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                    MTVI5_BAD_ANSWER_ERROR_OFFSET,
//                    "Норм. обмена");
//            }
//
//            // сбросим флаги недостоверности состояния выходов реле.
//            memset(pxModuleContext ->
//                   xModuleContextStatic.
//                   pucBadDoBufferPointer,
//                   0,
//                   MRXX_BAD_DO_BIT_ARRAY_LENGTH);
//            //iBadModuleBuffUpdate();
//            // получим указатель на буфер - состояние выходов модулей реле МР №1 – MP №8.
//            pucDestination = pxModuleContext ->
//                             xModuleContextStatic.
//                             pucSostDoBufferPointer;
//            ucData = (auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]);
//            // отправим данные - состояние выходов модулей реле МР №1 – MP №8.
//            for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//            {
//                if (ucData & (1 << i))
//                {
//                    *pucDestination++ = 1;
//                    // Архивировать событие?
//                    if (pxMrXXOneChannelDataBase[i].ucControl & ARCHIVE_MRXX_MASK)
//                    {
//                        // активное состояние события ещё не зарегистрировано?
//                        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                                    pxModuleContext ->
//                                    xModuleContextStatic.
//                                    ucModuleContextIndex,
//                                    MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                    MRXX_ERROR_CODE_RELAY_ON_OFF))
//                        {
//                            // зарегистрируем активное состояние события.
//                            // включение реле - i.
//                            CEvents::EventRegistration(
//                                pxModuleContext ->
//                                xModuleContextStatic.
//                                ucModuleContextIndex,
//                                (CEvents::HANDLED_EVENTS_MODULES_EVENTS_TYPE |
//                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                                MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                "Вкл. реле");
//                        }
//                    }
//                }
//                else
//                {
//                    *pucDestination++ = 0;
//                    // Архивировать событие?
//                    if (pxMrXXOneChannelDataBase[i].ucControl & ARCHIVE_MRXX_MASK)
//                    {
//                        // неактивное состояние события ещё не зарегистрировано?
//                        if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
//                                    pxModuleContext ->
//                                    xModuleContextStatic.
//                                    ucModuleContextIndex,
//                                    MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                    MRXX_ERROR_CODE_RELAY_ON_OFF))
//                        {
//                            // зарегистрируем неактивное состояние события.
//                            // выключение реле - i.
//                            CEvents::EventRegistration(
//                                pxModuleContext ->
//                                xModuleContextStatic.
//                                ucModuleContextIndex,
//                                (CEvents::HANDLED_EVENTS_MODULES_EVENTS_TYPE |
//                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                                MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                "Откл. реле");
//                        }
//                    }
//                }
//            }
//            // некоторые типы модулей имеют дополнительную возможность контроля:
//            // достоверность состояния выходов реле - MR51;
//            // обрыв обмотки реле и короткое замыкание обмотки реле - MR53.
//            switch(pxModuleContext ->
//                    xModuleContextStatic.
//                    ucModuleType)
//            {
//            case MODULE_TYPE_MR51:
//                // Только для МР-51:
//                // Байт сост. ВЫХ по ОС rel_OS: D0 - обр. связь реле №1 ВКЛ., D7 - обр. связь реле №8 ВКЛ.
//                // Должен совпадать с байтом Упр. ВЫХ r_out, иначе ошибка, несовпадающий бит-неисправное реле
//                for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//                {
//                    // биты состояния выходов по обратной связи и выходов упраления реле совпадают?
//                    if (((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET]) >> i) ==
//                            ((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 1]) >> i))
//                    {
//                        // биты состояния выходов по обратной связи и выходов упраления реле совпадают.
//                        // сбросим флаг недостоверности выхода.
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        pucBadDoBufferPointer[i] = 0;
//                    }
//                    else
//                    {
//                        // биты состояния выходов по обратной связи и выходов упраления реле не совпадают.
//                        // установим флаг недостоверности выхода.
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        pucBadDoBufferPointer[i] = 1;
//                    }
//
//                    if (i == 3)
//                    {
//                        // биты состояния выходов по обратной связи и выходов упраления реле не совпадают.
//                        // установим флаг недостоверности выхода.
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        pucBadDoBufferPointer[i] = 1;
//                    }
//                }
////                return 0;
//                break;
//            case MODULE_TYPE_MR53:
//                // байты состояния обмоток 8-ми реле модуля. только МР53.
//                // байт - 2: состояния обмоток реле, если бит = 1 - короткое замыкание.
//                // #define MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET 0
//                // байт - 3: состояния обмоток реле, если бит = 1 - обрыв.
//                // #define MRXX_STAT_DO_BREACK_BIT_OFFSET 1
////                (auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 1]) = 4;
////                (auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 2]) = 4;
//                for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//                {
//                    // есть обрыв или короткое замыкание обмоток реле?
//                    if (((((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 1]) >> i) & 0x01) != 0) ||
//                            (((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 2]) >> i) & 0x01) != 0)
//                    {
//                        // есть обрыв или короткое замыкание обмоток реле.
//                        // установим флаг недостоверности выхода.
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        pucBadDoBufferPointer[i] = 1;
//                    }
//                    else
//                    {
//                        // нет обрыва или короткого замыкания обмоток реле.
//                        // сбросим флаг недостоверности выхода.
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        pucBadDoBufferPointer[i] = 0;
//                    }
//
////                    if (i == 3)
////                    {
////                        // биты состояния выходов по обратной связи и выходов упраления реле не совпадают.
////                        // установим флаг недостоверности выхода.
////                        pxModuleContext ->
////                        xModuleContextStatic.
////                        pucBadDoBufferPointer[i] = 1;
////                    }
//                }
//
//                for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//                {
//                    // есть короткое замыкание обмотки реле?
//                    if (((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 1]) >> i) & 0x01)
//                    {
//                        // есть короткое замыкание обмотки реле.
//                        // установим флаг короткого замыкания обмотки реле.
//                        (pxModuleContext ->
//                         xModuleContextStatic.
//                         pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
//                                                MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET]) = 1;
//
//                        // активное состояние события ещё не зарегистрировано?
//                        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                                    pxModuleContext ->
//                                    xModuleContextStatic.
//                                    ucModuleContextIndex,
//                                    MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                    MRXX_ERROR_CODE_RELAY_SHORT_CIRCUIT))
//                        {
//                            // зарегистрируем активное состояние события.
//                            // включение реле - i.
//                            CEvents::EventRegistration(
//                                pxModuleContext ->
//                                xModuleContextStatic.
//                                ucModuleContextIndex,
//                                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                                MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                "Кз. обмотки");
//                        }
//                    }
//                    else
//                    {
//                        // нет короткого замыкания обмотки реле.
//                        // сбросим флаг короткого замыкания обмотки реле.
//                        (pxModuleContext ->
//                         xModuleContextStatic.
//                         pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
//                                                MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET]) = 0;
//
//                        // неактивное состояние события ещё не зарегистрировано?
//                        if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
//                                    pxModuleContext ->
//                                    xModuleContextStatic.
//                                    ucModuleContextIndex,
//                                    MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                    MRXX_ERROR_CODE_RELAY_SHORT_CIRCUIT))
//                        {
//                            // зарегистрируем неактивное состояние события.
//                            // выключение реле - i.
//                            CEvents::EventRegistration(
//                                pxModuleContext ->
//                                xModuleContextStatic.
//                                ucModuleContextIndex,
//                                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                                MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                "Норм. кз. об");
//                        }
//                    }
//
//
////                    if (i == 3)
////                    {
////                        // есть короткое замыкание обмотки реле.
////                        // установим флаг короткого замыкания обмотки реле.
////                        (pxModuleContext ->
////                         xModuleContextStatic.
////                         pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
////                                                MRXX_STAT_DO_SHORT_CIRCUIT_BIT_OFFSET]) = 1;
////                    }
//
//                    // есть обрыв обмотки реле?
////                    if ((ui8TempCommand) && (ui8TempCommand - 1) == i)
//                    if (((auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET + 2]) >> i) & 0x01)
//                    {
//                        // есть обрыв обмотки реле.
//                        // установим флаг обрыва обмотки реле.
//                        (pxModuleContext ->
//                         xModuleContextStatic.
//                         pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
//                                                MRXX_STAT_DO_BREACK_BIT_OFFSET]) = 1;
//
//                        // активное состояние события ещё не зарегистрировано?
//                        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                                    pxModuleContext ->
//                                    xModuleContextStatic.
//                                    ucModuleContextIndex,
//                                    MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                    MRXX_ERROR_CODE_RELAY_BREACK))
//                        {
//                            // зарегистрируем активное состояние события.
//                            // включение реле - i.
//                            CEvents::EventRegistration(
//                                pxModuleContext ->
//                                xModuleContextStatic.
//                                ucModuleContextIndex,
//                                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                                MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                "Обр. обмотки");
//                        }
//                    }
//                    else
//                    {
//                        // нет обрыва обмотки реле.
//                        // сбросим флаг обрыва обмотки реле.
//                        (pxModuleContext ->
//                         xModuleContextStatic.
//                         pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
//                                                MRXX_STAT_DO_BREACK_BIT_OFFSET]) = 0;
//
//                        // неактивное состояние события ещё не зарегистрировано?
//                        if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
//                                    pxModuleContext ->
//                                    xModuleContextStatic.
//                                    ucModuleContextIndex,
//                                    MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                    MRXX_ERROR_CODE_RELAY_BREACK))
//                        {
//                            // зарегистрируем неактивное состояние события.
//                            // выключение реле - i.
//                            CEvents::EventRegistration(
//                                pxModuleContext ->
//                                xModuleContextStatic.
//                                ucModuleContextIndex,
//                                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                                MTVI5_OUT_DAC_ERROR_OFFSET + i,
//                                "Норм обр. об");
//                        }
//                    }
//
////                    if (i == 3)
////                    {
////                        // есть обрыв обмотки реле.
////                        // установим флаг обрыва обмотки реле.
////                        (pxModuleContext ->
////                         xModuleContextStatic.
////                         pucStatDoBufferPointer[(i * MRXX_STAT_DO_ONE_OUTPUT_LENGTH) +
////                                                MRXX_STAT_DO_BREACK_BIT_OFFSET]) = 1;
////                    }
//
//                }
//
//                return 0;
//                break;
//
//            default:
//                break;
//            };
//            break;
//        }
//
//    case MRXX_DATA_BASE_ERROR:
//        // База данных повреждена?
//        if ((pxModuleContext ->
//                xModuleContextDinamic.
//                usAuxiliaryCounter) == 0) //MRXX database error Debug
//        {
//            cout << "MRXX_DATA_BASE_ERROR" << endl;
//            fbDiscreteOutputModuleDataBaseError = 1;
//            // получим код ошибки;
//            (pxModuleContext ->
//             xModuleContextDinamic.
//             ucErrorCode) = INTERNAL_MODULE_ERROR_DATA_BASE;
//
//            // активное состояние события ещё не зарегистрировано?
//            if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                        pxModuleContext ->
//                        xModuleContextStatic.
//                        ucModuleContextIndex,
//                        MTVI5_DATA_BASE_ERROR_OFFSET))
//            {
//                // зарегистрируем активное состояние события.
//                CEvents::EventRegistration(
//                    pxModuleContext ->
//                    xModuleContextStatic.
//                    ucModuleContextIndex,
//                    (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                     CEvents::HANDLED_EVENTS_IS_POPUP |
//                     CEvents::HANDLED_EVENTS_IS_SOUND |
//                     CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                    MTVI5_DATA_BASE_ERROR_OFFSET,
//                    "Ошибка б. МР");
//            }
//        }
//        else
//        {
//            // уменьшим значение счётчика - "количество сеансов связи с модулем без ответа".
//            (pxModuleContext ->
//             xModuleContextDinamic.
//             usAuxiliaryCounter)--;
//
//            cout << "MRXX_DATA_BASE_ERROR MRXX_FSM_STATE_WRITE_DATABASE" << endl;
//            // База данных повреждена. Попытаемся записать её повторно.
//            pxModuleContext ->
//            xModuleContextDinamic.
//            ucFsmState = MRXX_FSM_STATE_WRITE_DATABASE;
//        }
//
//        break;
//
//    default:
////        // модуль исправен.
////        pxModuleContext ->
////        xModuleContextDinamic.
////        usAuxiliaryCounter = BAD_MODULE_CYCLE_COUNT_DEFAULT; //MRXX database error Debug
//        break;
//    };
//
//    // модуль признан неисправным?
//    if ((pxModuleContext ->
//            xModuleContextDinamic.
//            nucBadAnswerCounter) == 0)
//    {
//        // модуль признан неисправным.
//        *(pxModuleContext ->
//          xModuleContextStatic.
//          pucModuleBadStateBufferPointer) = BAD_MODULE_NOT_RESPONDED;
//
//        // активное состояние события ещё не зарегистрировано?
//        if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
//                    pxModuleContext ->
//                    xModuleContextStatic.
//                    ucModuleContextIndex,
//                    MTVI5_BAD_ANSWER_ERROR_OFFSET))
//        {
//            // зарегистрируем активное состояние события.
//            // ошибка обмена данными.
//            CEvents::EventRegistration(
//                pxModuleContext ->
//                xModuleContextStatic.
//                ucModuleContextIndex,
//                (CEvents::HANDLED_EVENTS_INTERNAL_MODULES_BAD_TYPE |
//                 CEvents::HANDLED_EVENTS_IS_POPUP |
//                 CEvents::HANDLED_EVENTS_IS_SOUND |
//                 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//                MTVI5_BAD_ANSWER_ERROR_OFFSET,
//                "Ошиб. обмена");
//        }
//
//        // установим флаги недостоверности состояния выходов реле.
//        memset(pxModuleContext ->
//               xModuleContextStatic.
//               pucBadDoBufferPointer,
//               1,
//               MRXX_BAD_DO_BIT_ARRAY_LENGTH);
//        //iBadModuleBuffUpdate();
//        return -1;
//    }
//    else
//    {
//        // уменьшим значение счётчика - "количество сеансов связи с модулем без ответа".
//        (pxModuleContext ->
//         xModuleContextDinamic.
//         nucBadAnswerCounter)--;
//    }
//    return 0;
//}

////-----------------------------------------------------------------------------------------------------
//// записывает базу данных из RAM прибора в EEPROM модуля.
//int iModuleMrXXDataBaseBlockWrite(TModuleContext *pxModuleContext)
//{
//    unsigned char ucData;
//    unsigned short usData;
//    int i;
//    unsigned char *pucSource;
//    unsigned char *pucDestination;
//    unsigned char *pucTempArray;
//    TMrXXOneChannelDataBase *pxMrXXOneChannelDataBaseSource;
//
//    pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET];
//    // получим указатель на блок в базе данных.
//    pxMrXXOneChannelDataBaseSource =
//        (TMrXXOneChannelDataBase*)((pxModuleContext ->
//                                    xModuleContextStatic.
//                                    pucDataBasePointer) +
//                                   (pxModuleContext ->
//                                    xModuleContextStatic.
//                                    ucDataBaseDataOffset));
//
//    // При загрузке БД в МР передается только байт 0 описателя каждого выхода.
//    for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
//    {
//
//        pucDestination[i] =
//            pxMrXXOneChannelDataBaseSource[i].ucControl;
//    }
//
//    pucSource = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET];
//    usData = usCrcSummTwoByteCalculation(pucSource,
//                                         MRXX_DISCRETE_OUTPUT_QUANTITY);
//    pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET +
//                                     MRXX_DISCRETE_OUTPUT_QUANTITY];
//    *pucDestination++ = (unsigned char)usData;
//    *pucDestination = (unsigned char)(usData >> 8);
//
//    auiSpiTxBuffer[0] = MRXX_SET_DATA_BASE_COMMAND;
//    xSpiCommunicationDevice.Exchange(pxModuleContext ->
//            xModuleContextStatic.
//            ucModuleAddress,
//                 auiSpiTxBuffer,
//                 auiSpiRxBuffer,
//                 SPI_PREAMBLE_LENGTH +
//                 MRXX_SET_DATA_BASE_COMMAND_ANSWER_LENGTH +
//                 TWO_BYTE_CRC_LENGTH +
//                 TAIL_ANSWER_LENGTH,
//                 LOW_SPEED_IN_HZ
//                );
//    // модуль отвечает?
//    if (auiSpiRxBuffer[SPI_COMMAND_BYTE_OFFSET] == MRXX_SET_DATA_BASE_COMMAND) // if echo answer is ok.
//    {
//        // есть подтверждение записи базы данных в EEPROM модуля?
//        if (auiSpiRxBuffer[SPI_DATA_BYTE_OFFSET +
//                           MRXX_DISCRETE_OUTPUT_QUANTITY +
//                           TWO_BYTE_CRC_LENGTH] == DATA_EXCHANGE_OK)
//        {
//            // есть подтверждение записи базы данных в EEPROM модуля.
//            cout << "iModuleMrXXDataBaseBlockWrite ok" << endl;
//            return 0;
//        }
//    }
//    cout << "iModuleMrXXDataBaseBlockWrite error" << endl;
//
//    return -1;
//}

//-----------------------------------------------------------------------------------------------------
// записывает базу данных из RAM прибора в EEPROM модуля.
int iModuleMrXXDataBaseBlockWrite(TModuleContext *pxModuleContext)
{
    unsigned char ucData;
    unsigned short usData;
    int i;
    unsigned char *pucSource;
    unsigned char *pucDestination;
    unsigned char *pucTempArray;
    TMrXXOneChannelDataBase *pxMrXXOneChannelDataBaseSource;
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET];
    // получим указатель на блок в базе данных.
    pxMrXXOneChannelDataBaseSource =
        (TMrXXOneChannelDataBase*)((pxModuleContext ->
                                    xModuleContextStatic.
                                    pucDataBasePointer) +
                                   (pxModuleContext ->
                                    xModuleContextStatic.
                                    ucDataBaseDataOffset));


    auiSpiTxBuffer[0] = MRXX_SET_DATA_BASE_COMMAND;
    auiSpiTxBuffer[1] = 8;
    // При загрузке БД в МР передается только байт 0 описателя каждого выхода.
    for (i = 0; i < MRXX_DISCRETE_OUTPUT_QUANTITY; i++)
    {

        pucDestination[i] =
            pxMrXXOneChannelDataBaseSource[i].ucControl;
    }

    pucSource = &auiSpiTxBuffer[0];
    usData = HammingDistanceCrc(pucSource,
                                MRXX_DISCRETE_OUTPUT_QUANTITY + 2);
    pucDestination = &auiSpiTxBuffer[SPI_DATA_BYTE_OFFSET +
                                     MRXX_DISCRETE_OUTPUT_QUANTITY];
    *pucDestination = (unsigned char)usData;

//    {
//        cout << "MRXX_SET_DATA_BASE_COMMAND " << endl;
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)auiSpiTxBuffer;
//        for(int i=0; i<16; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//    }

    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     MRXX_SET_DATA_BASE_COMMAND_ANSWER_LENGTH +
                                     ONE_BYTE_CRC_LENGTH,
                                     LOW_SPEED_IN_HZ
                                    );

    return 0;
}

//-----------------------------------------------------------------------------------------------------
int iModuleMrXXDataBaseBlockWriteError(TModuleContext *pxModuleContext)
{
    // База данных повреждена?
    if ((pxModuleContext ->
            xModuleContextDinamic.
            usAuxiliaryCounter) == 0) //MRXX database error Debug
    {
//        cout << "MRXX_DATA_BASE_ERROR" << endl;
        fbDiscreteOutputModuleDataBaseError = 1;
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
                "Ошибка б. МР");
        }
    }
    else
    {
        // уменьшим значение счётчика - "количество сеансов связи с модулем без ответа".
        (pxModuleContext ->
         xModuleContextDinamic.
         usAuxiliaryCounter)--;
    }
}

//-----------------------------------------------------------------------------------------------------
// записывает базу данных из RAM прибора в EEPROM модуля.
int iModuleMrXXChangeProtocol(TModuleContext *pxModuleContext)
{
//    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
//    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    auiSpiTxBuffer[0] = CHANGE_PROTOCOL_COMMAND;
    xSpiCommunicationDevice.Exchange(pxModuleContext ->
                                     xModuleContextStatic.
                                     ucModuleAddress,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     SPI_PREAMBLE_LENGTH +
                                     GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH +
                                     ONE_BYTE_CRC_LENGTH,
                                     LOW_SPEED_IN_HZ);


//    cout << "CHANGE_PROTOCOL_COMMAND " << endl;
//    unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
//    for(int i=0; i<16; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }

    // модуль отвечает?
    if (auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET] ==
            CHANGE_PROTOCOL_COMMAND) // if echo answer is ok.
    {
        // обмен данными прошёл без ошибок?
        if ((iCrcSummOneByteCompare(&auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET],
                                    GET_MODULE_TYPE_COMMAND_LENGTH +
                                    GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH) > 0))
        {
            // есть подтверждение записи базы данных в EEPROM модуля.
            cout << "iModuleMrXXChangeProtocol ok" << endl;
            return 0;
        }
    }

    cout << "iModuleMrXXChangeProtocol error" << endl;

    return -1;
}

//-----------------------------------------------------------------------------------------------------
// создаёт стартовую базу данных модулей дискретного вывода.
int iModuleMrXXStartDataBaseCreate(void)
{
    int i;
    unsigned char ucCheck;
    unsigned char nucBlockCounter;
    unsigned char nucBlocksInBlockCounter; // один описатель - один блок, в общем блоке.
    unsigned char nucInputCounter;
    unsigned char nucModuleCounter;
    unsigned char *pucSource;
    unsigned char ucFlowControl;
    unsigned char aucTempArray[PLC_DATA_BASE_BLOCK_LENGTH];
    TMrXXOneChannelDataBase *pxMrXXOneChannelDataBaseSource;
    TMrXXOneChannelDataBase *pxMrXXOneChannelDataBaseDestination;

    memset(&aucTempArray[0],
           0,
           sizeof(aucTempArray));
    pxMrXXOneChannelDataBaseDestination = (TMrXXOneChannelDataBase*)&aucTempArray[0];
    // создадим стартовую базу данных дискретных выходов.
    for (int i = 0;
            i < (xPlcConfigService.
                 xPlcConfigServiceData.
                 ucServiceDiscreteOutputModuleQuantity *
                 MRXX_DISCRETE_OUTPUT_QUANTITY);
            i++)
    {
        // ТИП – тип выхода 1 – управляющий.
        pxMrXXOneChannelDataBaseDestination[i].ucControl |= OUTPUT_TYPE_MASK;
        // БЛ/ТК: для управляющего выхода: 0 – без блокировки.
        pxMrXXOneChannelDataBaseDestination[i].ucControl &= ~BLOCK_ALARM_TYPE_MASK;
        // ВУС – сброс с верхнего уровня: 0 – нет.
        pxMrXXOneChannelDataBaseDestination[i].ucControl &= ~PC_RESET_MASK;
        // ВУК – квитирование с верхнего уровня: 0 – нет.
        pxMrXXOneChannelDataBaseDestination[i].ucControl &= ~PC_KVIT_MASK;
        // СБЦ – сброс с центральной панели МИНД: 0 – нет.
        pxMrXXOneChannelDataBaseDestination[i].ucControl &= ~BUTTON_RESET_MASK;
        // КВЦ – квитирование с центральной панели МИНД: 0 – нет.
        pxMrXXOneChannelDataBaseDestination[i].ucControl &= ~BUTTON_KVIT_MASK;
        // D7 - АРХ =1 архивировать, 0 – не архивировать.
        pxMrXXOneChannelDataBaseDestination[i].ucControl &= ~ARCHIVE_MRXX_MASK;
        // КВ_АДР - адрес входного сигнала внешнего квитирования. нет.
        pxMrXXOneChannelDataBaseDestination[i].ucExternalKvitAddress = 0;
        // СБ_АДР - адрес входного сигнала внешнего сброса. нет.
        pxMrXXOneChannelDataBaseDestination[i].ucExternalResetAddress = 0;
    }

    // получим указатель на буфер с вновь созданной стартовой базой данных прибора.
    pxMrXXOneChannelDataBaseSource = (TMrXXOneChannelDataBase*)&aucTempArray[0];
    // получим указатель на базу данных прибора в общем формате.
    pxMrXXOneChannelDataBaseDestination = (TMrXXOneChannelDataBase*)&(xPlcDataBase.
                                          axPlcDataBaseBlocks[DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_OFFSET].
                                          aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;
    // преобразуем созданную базу данных в общий формат.
    for (int i = 0;
            i < MAX_HANDLED_DISCRETE_OUTPUT ;
            i++)
    {
        // обработан весь блок базы данных?
        if (nucBlocksInBlockCounter == DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // следующий блок базы данных.
            nucBlockCounter++;
            // получим указатель на следующий блок в общей базе данных прибора.
            pxMrXXOneChannelDataBaseDestination = (TMrXXOneChannelDataBase*)&(xPlcDataBase.
                                                  axPlcDataBaseBlocks[DISCRETE_OUTPUT_MODULE_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                                                  aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }
        // скопируем один описатель дискретного выхода, в буфер общей базы данных прибора.
        pxMrXXOneChannelDataBaseDestination[i] =
            pxMrXXOneChannelDataBaseSource[i];
        // следующий описатель.
        nucBlocksInBlockCounter++;
    }

    return 0;
}

//-----------------------------------------------------------------------------------------------------
// Главный автомат.
void vModuleMrXXDataHandler(TModuleContext *pxModuleContext)
{
    switch (pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState)
    {
    case MRXX_IDDLE:
        break;

    case MRXX_CHANGE_PROTOCOL:
        cout << "MRXX_CHANGE_PROTOCOL" << endl;
        if (iModuleMrXXChangeProtocol(pxModuleContext) != -1)
        {
            pxModuleContext ->
            xModuleContextDinamic.
            ucFsmState = MRXX_WRITE_DATA_BASE;
        }
        break;

    case MRXX_WRITE_DATA_BASE:
        cout << "MRXX_WRITE_DATA_BASE" << endl;
        iModuleMrXXDataBaseBlockWrite(pxModuleContext);
        pxModuleContext ->
        xModuleContextDinamic.
        ucFsmState = MRXX_WRITE_DATA_BASE_CHECK;
        break;

    case MRXX_WRITE_DATA_BASE_CHECK:
        cout << "MRXX_WRITE_DATA_BASE_CHECK" << endl;
        if (iModuleMrXXTransferData(pxModuleContext) != -1)
        {
            // Проверим ответ модуля на предыдущую команду - запись базы данных.
            switch (auiSpiRxBuffer[(SPI_MT_RX_OFFSET + SPI_MT_FUNCTION_CODE_OFFSET)])
            {
                // База данных записана успешно?
            case MRXX_SET_DATA_BASE_COMMAND:
                cout << "MRXX_SET_DATA_BASE_COMMAND" << endl;
//                if (ucDataBaseError != )
//                {
                    // сообщим программатору, что ошибок нет, по 14 функции Modbus - запрос о результатах программирования.
                    ucDataBaseError = PROGRAMMING_DATA_BASE_WRITE_OK;
//                }
                pxModuleContext ->
                xModuleContextDinamic.
                ucFsmState = MRXX_DATA_EXCHANGE;
                break;
                // Во время записи базы данных произощла ошибка?
            case MRXX_SET_DATA_BASE_COMMAND_ERROR:
                cout << "MRXX_SET_DATA_BASE_COMMAND_ERROR" << endl;
//                ucDataBaseError = PROGRAMMING_MR_DATA_BASE_ERROR;
                iModuleMrXXDataBaseBlockWriteError(pxModuleContext);
                pxModuleContext ->
                xModuleContextDinamic.
                ucFsmState = MRXX_WRITE_DATA_BASE;
                break;

                // Произошла ошибка обмена?
            default:
                cout << "MRXX_SET_DATA_BASE_COMMAND_ERROR default" << endl;
                iModuleMrXXTransferDataReceiveError(pxModuleContext);
                pxModuleContext ->
                xModuleContextDinamic.
                ucFsmState = MRXX_WRITE_DATA_BASE;
                break;
            }
        }
        else
        {
            cout << "WRITE_DATA_BASE_CHECK  error" << endl;
            iModuleMrXXTransferDataReceiveError(pxModuleContext);
        }

        break;

    case MRXX_DATA_EXCHANGE:
//        cout << "DATA_EXCHANGE" << endl;
        if (iModuleMrXXTransferData(pxModuleContext) != -1)
        {
            // Проверим ответ модуля на предыдущую команду - запись базы данных.
            switch (auiSpiRxBuffer[(SPI_MT_RX_OFFSET + SPI_MT_FUNCTION_CODE_OFFSET)])
            {
                // Обмен данными прошёл без ошибок?
            case MRXX_DATA_READY:
//                cout << "MRXX_DATA_READY" << endl;
                iModuleMrXXTransferDataReceive(pxModuleContext);
                break;
                // Требуется передача базы данных?
            case MRXX_DATA_BASE_ERROR:
                cout << "MRXX_DATA_BASE_ERROR" << endl;
                pxModuleContext ->
                xModuleContextDinamic.
                ucFsmState = MRXX_WRITE_DATA_BASE;
                break;
                // Произошла ошибка обмена?
            default:
                cout << "MRXX_SET_DATA_BASE_COMMAND_ERROR default" << endl;
                iModuleMrXXTransferDataReceiveError(pxModuleContext);
                break;
            }
        }
        else
        {
            cout << "MRXX_DATA_EXCHANGE  error" << endl;
            iModuleMrXXTransferDataReceiveError(pxModuleContext);
        }
        break;

    default:
        break;
    }
}

