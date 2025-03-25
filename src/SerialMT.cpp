//-----------------------------------------------------------------------------------------------------
// SerialMT - serial multi trans.
//-----------------------------------------------------------------------------------------------------
#include "SerialMT.h"
#include "Crc.h"


CSpiSerialMT xCSpiSerialMT;
CModbusSerialMT xCModbusSerialMT;

//-----------------------------------------------------------------------------------------------------
uint8_t CSerialMT::aui8ReceiveMessageBuff[];
uint8_t CSerialMT::aui8TransmitMessageBuff[];
uint8_t CSerialMT::nui8BuffByteCounter;
// счётчик байтов для вычисления длительности пакета при обмене данными по SPI.
uint8_t CSerialMT::ui8SpiReceivedByteCounter;
uint8_t CSerialMT::ui8TempSPDR;

//-----------------------------------------------------------------------------------------------------
// возвращает тип запроса. прочитать, если ноль. или записать, если единица.
uint8_t CSerialMT::IsWrite(void)
{
    return ((ui8Address & SERIAL_MT_WRITE) != 0);
}

//-----------------------------------------------------------------------------------------------------
// готовит буфер общего сообщения к извлечению вложенных сообщений.
void CSerialMT::MessagePushInitialization(void)
{
    // получим указатель на буфер общего сообщения.
    pui8MessageBag = GetTransmitMessageBuff();
    // перейдём к началу общего сообщения.
    ui8MessageBagLength = 0;
    // перейдём к первому вложенному сообщению.
    ui8MessageBagIndex = 0;
}

//-----------------------------------------------------------------------------------------------------
// формирует заголовок сообщения.
// возвращает указатель на свободное место в буфере общего сообщения.
uint8_t* CSerialMT::MessagePushRequest(uint8_t ui8Length, uint8_t ui8Address)
{
    // для вложенного сообщения нет места?
    if ((ui8MessageBagLength +
            (ui8Length) +
            SERIAL_MT_HEADER_LENGTH) >
            SERIAL_MT_MESSAGE_BAG_MAX_LENGTH)
    {
        // для вложенного сообщения нет места.
        return 0;
    }
    else
    {
        uint8_t* pui8MessageBagTemp;
        // получим указатель на буфер общего сообщения.
        pui8MessageBagTemp = &pui8MessageBag[ui8MessageBagLength];
        // сформируем заголовок вложенного сообщения - поместим адрес функции-задачи исполнителя.
        *pui8MessageBagTemp++ = ui8Address;
        // сформируем заголовок вложенного сообщения - поместим длину вложенного сообщения.
        *pui8MessageBagTemp++ = ui8Length;
        // изменим длину общего сообщения на длину вложенного плюс заголовок.
        ui8MessageBagLength += (ui8Length + SERIAL_MT_HEADER_LENGTH);
        // вернём указатель на свободное место в буфере общего сообщения.
        return pui8MessageBagTemp;
    }
}

//-----------------------------------------------------------------------------------------------------
// готовит буфер общего сообщения к извлечению вложенных сообщений.
void CSerialMT::MessagePopInitialization(void)
{
    // получим указатель на буфер принятого общего сообщения.
    pui8MessageBag = pui8DataPointer;
    // получим длину принятого общего сообщения.
    ui8MessageBagLength = ui8Length;
    // перейдём к первому вложенному сообщению.
    ui8MessageBagIndex = 0;
}

//-----------------------------------------------------------------------------------------------------
// готовит буфер общего сообщения к извлечению вложенных сообщений.
uint8_t CSerialMT::MessagePopCheck(void)
{
    // длина общего сообщения меньше минимальной длины?
    if (ui8MessageBagLength < SERIAL_MT_HEADER_LENGTH)
    {
        // сообщим о том, что нет сообщения.
        ui8Length = 0;
        return 0;
    }

    uint8_t *pui8MessageBagTemp;
    pui8MessageBagTemp = pui8MessageBag;
    // вычислим сумму длин всех вложенных сообщений с их заголовками.
    uint8_t i = 0;
    while (i < ui8MessageBagLength)
    {
        // заголовок вложенного сообщения.
        ui8Address = *pui8MessageBagTemp++;
        ui8Length = *pui8MessageBagTemp++;
        // перейдём к заголовку следующего вложенного сообщения.
        i += (ui8Length + SERIAL_MT_HEADER_LENGTH);
        pui8MessageBagTemp += ui8Length;
    }
    // сумма длин всех вложенных сообщений и длин их заголовков равна длине общего сообщения?
    if (ui8MessageBagLength == i)
    {
        return 1;
    }
    else
    {
        // ошибка: сумма длин всех вложенных сообщений и длин их заголовков не равна длине общего сообщения.
        // сообщим о том, что нет сообщения.
        ui8Length = 0;
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// извлекает вложенное сообщение из общего.
// возвращает -1, если:
// длина общего сообщения меньше минимальной длины,
// извлечены все вложенные сообщения,
// сумма длин всех вложенных сообщений и длин их заголовков не равна длине общего сообщения.
// возвращает адрес функции-задачи, указатель на вложенное сообщение, размер сообщения.
int8_t CSerialMT::MessagePop(void)
{
    // извлечены все вложенные сообщения?
    if (ui8MessageBagLength <= ui8MessageBagIndex)
    {
        // извлечены все вложенные сообщения.
        // сообщим о том, что нет сообщения.
        ui8Length = 0;
        return 0;
    }

    uint8_t *pui8MessageBagTemp;

    pui8MessageBagTemp = (pui8MessageBag + ui8MessageBagIndex);
    // вернём адрес функции-задачи, для которой поступило вложенное сообщение.
    ui8Address = *pui8MessageBagTemp++;
    // вернём длину вложенного сообщения.
    ui8Length = *pui8MessageBagTemp++;
    // вернём указатель на вложенное сообщение.
    pui8DataPointer = pui8MessageBagTemp;
    // перейдём к следующему вложенному сообщению.
    ui8MessageBagIndex += (ui8Length + SERIAL_MT_HEADER_LENGTH);
    return 1;
}

//-------------------------------------------------------------------------------------------------
int8_t CSerialMT::DataExchangeIsOccurred(void)
{
    // произошел обмен данными по Modbus?
    return fui8DataExchangeIsOccurred;
}

//-------------------------------------------------------------------------------------------------
void CSerialMT::DataExchangeFlagSet(void)
{
    // установим флаг - произошел обмен данными по Modbus.
    fui8DataExchangeIsOccurred = 1;
}

//-------------------------------------------------------------------------------------------------
void CSerialMT::DataExchangeFlagReset(void)
{
    // сбросим флаг - произошел обмен данными по Modbus.
    fui8DataExchangeIsOccurred = 0;
}

//-------------------------------------------------------------------------------------------------
int8_t CSerialMT::DataReceive(void)
{
    // длина сообщения слишком мала?
    if (ui8Length <=
            (HEADER_LENGTH))
    {
        return -1;
    }
    // получим указатель на принятое общее сообщение в кадре ModbusMT.
    pui8DataPointer = GetReceiveMessageBuff();
    return 0;
}

//-------------------------------------------------------------------------------------------------
int8_t CSerialMT::DataTransmit(void)
{
    // длина сообщения слишком мала?
    if (ui8MessageBagLength <=
            HEADER_LENGTH)
    {
        return -1;
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------
int8_t CSerialMT::DataExchange(void)
{

    return 0;
}

//-------------------------------------------------------------------------------------------------
uint8_t* CSerialMT::GetReceiveMessageBuff(void)
{
    return &aui8ReceiveMessageBuff[DATA_BYTE_OFFSET];
}

//-------------------------------------------------------------------------------------------------
uint8_t* CSerialMT::GetTransmitMessageBuff(void)
{
    return &aui8TransmitMessageBuff[DATA_BYTE_OFFSET];
}

//-----------------------------------------------------------------------------------------------------
// Spi

//-------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
// готовит буфер общего сообщения к извлечению вложенных сообщений.
void CSpiSerialMT::MessagePushInitialization(void)
{
    // получим указатель на буфер общего сообщения.
    pui8MessageBag = (GetTransmitMessageBuff() + HEADER_LENGTH);
    // перейдём к началу общего сообщения.
    ui8MessageBagLength = 0;
    // перейдём к первому вложенному сообщению.
    ui8MessageBagIndex = 0;
}

//-----------------------------------------------------------------------------------------------------
// готовит буфер общего сообщения к извлечению вложенных сообщений.
void CSpiSerialMT::MessagePopInitialization(void)
{
    // получим указатель на буфер принятого общего сообщения.
    pui8MessageBag = (GetReceiveMessageBuff() + 3);
    // получим длину принятого общего сообщения.
    ui8MessageBagLength = ui8Length;
    // перейдём к первому вложенному сообщению.
    ui8MessageBagIndex = 0;
}

//-------------------------------------------------------------------------------------------------
uint8_t* CSpiSerialMT::GetReceiveMessageBuff(void)
{
    return aui8ReceiveMessageBuff;
}

//-------------------------------------------------------------------------------------------------
uint8_t* CSpiSerialMT::GetTransmitMessageBuff(void)
{
    return aui8TransmitMessageBuff;
}

//-------------------------------------------------------------------------------------------------
int8_t CSpiSerialMT::DataExchange(void)
{
//-----------------------------------------------------------------------------------------------------

////if ((xCSpiSerialMT.ui8Address == 14) || (xCSpiSerialMT.ui8Address == 132))
////{
//     cout << "CSerialMT::GetTxBuff()" << endl;
//    unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)GetTransmitMessageBuff();
//    for(int i=0; i<32; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }
////
////}

    memset(GetReceiveMessageBuff(), 0, 32);

    // обмен данными по SPI.
    CGpio::ClearPin(SPI_CHIP_MIND_ENABLE_PIN_PORT, SPI_CHIP_MIND_ENABLE_PIN);
    usleep(100);
    xSpiCommunicationDevice.Exchange(GetTransmitMessageBuff(),
                                     GetReceiveMessageBuff(),
                                     HEADER_LENGTH +
                                     ui8MessageBagLength +
                                     CRC_LENGTH,
                                     SPEED_IN_HZ
                                    );
    CGpio::SetPin(SPI_CHIP_MIND_ENABLE_PIN_PORT, SPI_CHIP_MIND_ENABLE_PIN);
    // обнулим длину отправляемого сообщения.
    // чтобы не использовалось повторно в случае ошибки.
    ui8MessageBagLength = 0;
//-----------------------------------------------------------------------------------------------------


//    uint8_t ui8DataIsFound;
//    ui8DataIsFound = 0;
////    unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)GetReceiveMessageBuff();
//    for (uint8_t i = 0; i < 64; i++)
//    {
//        if ((*pucSourceTemp == 0x0e) || (*pucSourceTemp == 0x8e))
//        {
//            ui8DataIsFound = 1;
//        }
//        pucSourceTemp++;
//    }
//
////if ((xCSpiSerialMT.ui8Address == 14) || (xCSpiSerialMT.ui8Address == 132))
//    if (ui8DataIsFound)
//    {
//        ui8DataIsFound = 0;
//        //    printf("DataExchange gap %d\n\r", (ui8MessageBagLength));
//
//
//        cout << "CSerialMT::GetRxBuff()" << endl;
////        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)GetReceiveMessageBuff();
//        for(int i=0; i<256; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//    }

//    printf("DataExchange gap %d\n\r", (ui8MessageBagLength));
//
//    cout << "CSerialMT::GetRxBuff()" << endl;
////        unsigned char *pucSourceTemp;
//    pucSourceTemp = (unsigned char*)GetReceiveMessageBuff();
//    for(int i=0; i<64; )
//    {
//        for(int j=0; j<8; j++)
//        {
//            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//        }
//        cout << endl;
//        i += 8;
//    }

    return 1;
}

//-------------------------------------------------------------------------------------------------
int8_t CSpiSerialMT::DataReceive(void)
{
    uint8_t ui8LengthLocal;
    uint8_t *pui8ReceiveMessageBuff;
    pui8ReceiveMessageBuff = (GetReceiveMessageBuff() + 1);
    // принято сообщение с командой не SPI_DATA_EXCHANGE_COMMAND?
    if (*pui8ReceiveMessageBuff++ !=
            DATA_EXCHANGE_COMMAND)
    {
//        printf("DataReceive DATA_EXCHANGE_COMMAND err %d\n\r", (ui8LengthLocal));
//
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)GetReceiveMessageBuff();
//        for(int i=0; i<64; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
        return -1;
    }
    // произошло переполнение буфера?
    if (fui8BuffOverflow)
    {
//        printf("DataReceive fui8BuffOverflow err %d\n\r", (ui8LengthLocal));
        return -1;
    }
    // получим длину кадра SpiMT при обмене данными по SPI.
    ui8LengthLocal = *pui8ReceiveMessageBuff++;

    // длина сообщения слишком мала?
    if (ui8LengthLocal <=
            (HEADER_LENGTH))
    {
//        printf("DataReceive HEADER_LENGTH %d\n\r", (ui8LengthLocal));
        return -1;
    }
    // сообщение повреждено?
    if(iCrcSummTwoByteCompare(((GetReceiveMessageBuff() + 1)),
                              (ui8LengthLocal)) == -1)
    {
//        printf("DataReceive iCrcSummTwoByteCompare err %d\n\r", (ui8LengthLocal));
//
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)GetReceiveMessageBuff();
//        for(int i=0; i<32; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
        return -1;
    }
    else
    {
//printf("DataReceive iCrcSummTwoByteCompare ok %d\n\r", (ui8LengthLocal));
////printf("Crc ok %d\n\r", (ui8LengthLocal));
////
//        unsigned char *pucSourceTemp;
//        pucSourceTemp = (unsigned char*)GetReceiveMessageBuff();
//        for(int i=0; i<32; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
        // получим указатель на принятое общее сообщение в кадре SpiMT.
        pui8DataPointer = pui8ReceiveMessageBuff;
        // получим длину принятого общего сообщения в кадре SpiMT.
        ui8Length = (int16_t)(ui8LengthLocal -
                              HEADER_LENGTH);
        return 0;
    }
}

//-------------------------------------------------------------------------------------------------
int8_t CSpiSerialMT::DataTransmit(void)
{
    uint16_t ui16Data;
    uint8_t ui8LengthLocal;
    uint8_t *pui8TransmitMessageBuff;

    // перейдём на место команды в кадре SpiMT.
    pui8TransmitMessageBuff = GetTransmitMessageBuff();
    // получим размер общего сообщения.
    ui8LengthLocal = ui8MessageBagLength;
    // длина сообщения слишком мала?
    if (ui8LengthLocal <=
            HEADER_LENGTH)
    {
        // поместим команду кадра SpiMT.
        *pui8TransmitMessageBuff++ = 0;
        // поместим длину кадра SpiMT.
        *pui8TransmitMessageBuff++ = 0;
        return -1;
    }

    // поместим команду кадра SpiMT.
    *pui8TransmitMessageBuff++ = DATA_EXCHANGE_COMMAND;
    // поместим длину кадра SpiMT.
    *pui8TransmitMessageBuff++ =
        (ui8LengthLocal +
         HEADER_LENGTH);

    // перейдём на место CRC в кадре SpiMT.
    pui8TransmitMessageBuff += ui8LengthLocal;
    // вычислим CRC кадра SpiMT.
    ui16Data = usCrcSummTwoByteCalculation((unsigned char*)aui8TransmitMessageBuff,
                                           (ui8LengthLocal +
                                            HEADER_LENGTH));
    // поместим младший байт CRC кадра SpiMT.
    *pui8TransmitMessageBuff++ = (unsigned char)ui16Data;
    // поместим старший байт CRC кадра SpiMT.
    *pui8TransmitMessageBuff = (unsigned char)(ui16Data >> 8);

    return 0;
}


//-----------------------------------------------------------------------------------------------------
void CSpiSerialMT::DataExchangeEnable(void)
{
    // сбросим флаг - произошел обмен данными по SPI.
    fui8DataExchangeIsOccurred = 0;
//    vSpiDataExchangeEnable();
}

//-----------------------------------------------------------------------------------------------------
void CSpiSerialMT::DataExchangeDisable(void)
{
//    vSpiDataExchangeDisable();
}

//-------------------------------------------------------------------------------------------------
void CSpiSerialMT::PrepareDataExchange(void)
{
    nui8BuffByteCounter = 0;
    fui8BuffOverflow = 0;
}

//-----------------------------------------------------------------------------------------------------
void CSpiSerialMT::BuffOverflowFlagSet(void)
{
    // установим флаг - произошел обмен данными по SPI.
    fui8BuffOverflow = 1;
}

//-----------------------------------------------------------------------------------------------------
void CSpiSerialMT::BuffOverflowFlagReset(void)
{
    // сбросим флаг - произошел обмен данными по SPI.
    fui8BuffOverflow = 0;
}

//-----------------------------------------------------------------------------------------------------
void CSpiSerialMT::BuffByteCounterIncrement(void)
{
    // инкрементируем счётчик принятых байтов при обмене данными по SPI.
    nui8BuffByteCounter++;
}

//-----------------------------------------------------------------------------------------------------
int16_t CSpiSerialMT::BuffByteCounterGet(void)
{
    // получим счётчик принятых байтов при обмене данными по SPI.
    return nui8BuffByteCounter;
}

//-----------------------------------------------------------------------------------------------------
void CSpiSerialMT::BuffByteCounterClear(void)
{
    // обнулим счётчик байтов при обмене данными по SPI.
    nui8BuffByteCounter = 0;
}


//-----------------------------------------------------------------------------------------------------
// ModbusMT


//-----------------------------------------------------------------------------------------------------
// готовит буфер общего сообщения к извлечению вложенных сообщений.
void CModbusSerialMT::MessagePushInitialization(void)
{
    // получим указатель на буфер общего сообщения.
    pui8MessageBag = GetTransmitMessageBuff();
    // перейдём к началу общего сообщения.
    ui8MessageBagLength = 0;
    // перейдём к первому вложенному сообщению.
    ui8MessageBagIndex = 0;
}

//-----------------------------------------------------------------------------------------------------
// готовит буфер общего сообщения к извлечению вложенных сообщений.
void CModbusSerialMT::MessagePopInitialization(void)
{
    // получим указатель на буфер принятого общего сообщения.
    pui8MessageBag = pui8DataPointer;
    // получим длину принятого общего сообщения.
    ui8MessageBagLength = ui8Length;
    // перейдём к первому вложенному сообщению.
    ui8MessageBagIndex = 0;
}

//-------------------------------------------------------------------------------------------------
int8_t CModbusSerialMT::DataExchange(void)
{
////    cout << "CSerialMT::GetTxBuff()" << endl;
////    unsigned char *pucSourceTemp;
////    pucSourceTemp = (unsigned char*)GetTransmitMessageBuff();
////    for(int i=0; i<32; )
////    {
////        for(int j=0; j<8; j++)
////        {
////            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////        }
////        cout << endl;
////        i += 8;
////    }
//
//    // обмен данными по Modbus.
//    // установим адрес модуля локальной сети нижнего уровня.
//    modbus_set_slave(pxMasterTcpCtx,
//                     MODULE_MIND_MODBUS_ADDRESS);
//    int8_t i;
//    // обменяемся данными с модулем локальной сети нижнего уровня во временный буфер.
//    i = modbus_data_exchange(pxMasterTcpCtx,
//                             ui8MessageBagLength,
//                             GetTransmitMessageBuff(),
//                             GetReceiveMessageBuff());
////    printf("DataExchange gap %d\n\r", (ui8MessageBagLength));
////
////    cout << "CSerialMT::GetRxBuff()" << endl;
//////    unsigned char *pucSourceTemp;
////    pucSourceTemp = (unsigned char*)GetReceiveMessageBuff();
////    for(int i=0; i<32; )
////    {
////        for(int j=0; j<8; j++)
////        {
////            cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////        }
////        cout << endl;
////        i += 8;
////    }
//
//    if (i != -1)
//    {
//        ui8Length = i;
////        // установим флаг - произошел обмен данными по Modbus.
////        DataExchangeFlagSet();
//    }
//    return i;
}

//-------------------------------------------------------------------------------------------------
int8_t CModbusSerialMT::DataReceive(void)
{
    // длина сообщения слишком мала?
    if (ui8Length <=
            (HEADER_LENGTH))
    {
        return -1;
    }
    // получим указатель на принятое общее сообщение в кадре ModbusMT.
    pui8DataPointer = GetReceiveMessageBuff();
    return 0;
}

//-------------------------------------------------------------------------------------------------
int8_t CModbusSerialMT::DataTransmit(void)
{
    // длина сообщения слишком мала?
    if (ui8MessageBagLength <=
            HEADER_LENGTH)
    {
        return -1;
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------
uint8_t* CModbusSerialMT::GetReceiveMessageBuff(void)
{
    return aui8ReceiveMessageBuff;
}

//-------------------------------------------------------------------------------------------------
uint8_t* CModbusSerialMT::GetTransmitMessageBuff(void)
{
    // вернём указатель на буфер передачи.
    return aui8TransmitMessageBuff;
}

