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
// ������� ������ ��� ���������� ������������ ������ ��� ������ ������� �� SPI.
uint8_t CSerialMT::ui8SpiReceivedByteCounter;
uint8_t CSerialMT::ui8TempSPDR;

//-----------------------------------------------------------------------------------------------------
// ���������� ��� �������. ���������, ���� ����. ��� ��������, ���� �������.
uint8_t CSerialMT::IsWrite(void)
{
    return ((ui8Address & SERIAL_MT_WRITE) != 0);
}

//-----------------------------------------------------------------------------------------------------
// ������� ����� ������ ��������� � ���������� ��������� ���������.
void CSerialMT::MessagePushInitialization(void)
{
    // ������� ��������� �� ����� ������ ���������.
    pui8MessageBag = GetTransmitMessageBuff();
    // ������� � ������ ������ ���������.
    ui8MessageBagLength = 0;
    // ������� � ������� ���������� ���������.
    ui8MessageBagIndex = 0;
}

//-----------------------------------------------------------------------------------------------------
// ��������� ��������� ���������.
// ���������� ��������� �� ��������� ����� � ������ ������ ���������.
uint8_t* CSerialMT::MessagePushRequest(uint8_t ui8Length, uint8_t ui8Address)
{
    // ��� ���������� ��������� ��� �����?
    if ((ui8MessageBagLength +
            (ui8Length) +
            SERIAL_MT_HEADER_LENGTH) >
            SERIAL_MT_MESSAGE_BAG_MAX_LENGTH)
    {
        // ��� ���������� ��������� ��� �����.
        return 0;
    }
    else
    {
        uint8_t* pui8MessageBagTemp;
        // ������� ��������� �� ����� ������ ���������.
        pui8MessageBagTemp = &pui8MessageBag[ui8MessageBagLength];
        // ���������� ��������� ���������� ��������� - �������� ����� �������-������ �����������.
        *pui8MessageBagTemp++ = ui8Address;
        // ���������� ��������� ���������� ��������� - �������� ����� ���������� ���������.
        *pui8MessageBagTemp++ = ui8Length;
        // ������� ����� ������ ��������� �� ����� ���������� ���� ���������.
        ui8MessageBagLength += (ui8Length + SERIAL_MT_HEADER_LENGTH);
        // ����� ��������� �� ��������� ����� � ������ ������ ���������.
        return pui8MessageBagTemp;
    }
}

//-----------------------------------------------------------------------------------------------------
// ������� ����� ������ ��������� � ���������� ��������� ���������.
void CSerialMT::MessagePopInitialization(void)
{
    // ������� ��������� �� ����� ��������� ������ ���������.
    pui8MessageBag = pui8DataPointer;
    // ������� ����� ��������� ������ ���������.
    ui8MessageBagLength = ui8Length;
    // ������� � ������� ���������� ���������.
    ui8MessageBagIndex = 0;
}

//-----------------------------------------------------------------------------------------------------
// ������� ����� ������ ��������� � ���������� ��������� ���������.
uint8_t CSerialMT::MessagePopCheck(void)
{
    // ����� ������ ��������� ������ ����������� �����?
    if (ui8MessageBagLength < SERIAL_MT_HEADER_LENGTH)
    {
        // ������� � ���, ��� ��� ���������.
        ui8Length = 0;
        return 0;
    }

    uint8_t *pui8MessageBagTemp;
    pui8MessageBagTemp = pui8MessageBag;
    // �������� ����� ���� ���� ��������� ��������� � �� �����������.
    uint8_t i = 0;
    while (i < ui8MessageBagLength)
    {
        // ��������� ���������� ���������.
        ui8Address = *pui8MessageBagTemp++;
        ui8Length = *pui8MessageBagTemp++;
        // ������� � ��������� ���������� ���������� ���������.
        i += (ui8Length + SERIAL_MT_HEADER_LENGTH);
        pui8MessageBagTemp += ui8Length;
    }
    // ����� ���� ���� ��������� ��������� � ���� �� ���������� ����� ����� ������ ���������?
    if (ui8MessageBagLength == i)
    {
        return 1;
    }
    else
    {
        // ������: ����� ���� ���� ��������� ��������� � ���� �� ���������� �� ����� ����� ������ ���������.
        // ������� � ���, ��� ��� ���������.
        ui8Length = 0;
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������� ��������� ��������� �� ������.
// ���������� -1, ����:
// ����� ������ ��������� ������ ����������� �����,
// ��������� ��� ��������� ���������,
// ����� ���� ���� ��������� ��������� � ���� �� ���������� �� ����� ����� ������ ���������.
// ���������� ����� �������-������, ��������� �� ��������� ���������, ������ ���������.
int8_t CSerialMT::MessagePop(void)
{
    // ��������� ��� ��������� ���������?
    if (ui8MessageBagLength <= ui8MessageBagIndex)
    {
        // ��������� ��� ��������� ���������.
        // ������� � ���, ��� ��� ���������.
        ui8Length = 0;
        return 0;
    }

    uint8_t *pui8MessageBagTemp;

    pui8MessageBagTemp = (pui8MessageBag + ui8MessageBagIndex);
    // ����� ����� �������-������, ��� ������� ��������� ��������� ���������.
    ui8Address = *pui8MessageBagTemp++;
    // ����� ����� ���������� ���������.
    ui8Length = *pui8MessageBagTemp++;
    // ����� ��������� �� ��������� ���������.
    pui8DataPointer = pui8MessageBagTemp;
    // ������� � ���������� ���������� ���������.
    ui8MessageBagIndex += (ui8Length + SERIAL_MT_HEADER_LENGTH);
    return 1;
}

//-------------------------------------------------------------------------------------------------
int8_t CSerialMT::DataExchangeIsOccurred(void)
{
    // ��������� ����� ������� �� Modbus?
    return fui8DataExchangeIsOccurred;
}

//-------------------------------------------------------------------------------------------------
void CSerialMT::DataExchangeFlagSet(void)
{
    // ��������� ���� - ��������� ����� ������� �� Modbus.
    fui8DataExchangeIsOccurred = 1;
}

//-------------------------------------------------------------------------------------------------
void CSerialMT::DataExchangeFlagReset(void)
{
    // ������� ���� - ��������� ����� ������� �� Modbus.
    fui8DataExchangeIsOccurred = 0;
}

//-------------------------------------------------------------------------------------------------
int8_t CSerialMT::DataReceive(void)
{
    // ����� ��������� ������� ����?
    if (ui8Length <=
            (HEADER_LENGTH))
    {
        return -1;
    }
    // ������� ��������� �� �������� ����� ��������� � ����� ModbusMT.
    pui8DataPointer = GetReceiveMessageBuff();
    return 0;
}

//-------------------------------------------------------------------------------------------------
int8_t CSerialMT::DataTransmit(void)
{
    // ����� ��������� ������� ����?
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
// ������� ����� ������ ��������� � ���������� ��������� ���������.
void CSpiSerialMT::MessagePushInitialization(void)
{
    // ������� ��������� �� ����� ������ ���������.
    pui8MessageBag = (GetTransmitMessageBuff() + HEADER_LENGTH);
    // ������� � ������ ������ ���������.
    ui8MessageBagLength = 0;
    // ������� � ������� ���������� ���������.
    ui8MessageBagIndex = 0;
}

//-----------------------------------------------------------------------------------------------------
// ������� ����� ������ ��������� � ���������� ��������� ���������.
void CSpiSerialMT::MessagePopInitialization(void)
{
    // ������� ��������� �� ����� ��������� ������ ���������.
    pui8MessageBag = (GetReceiveMessageBuff() + 3);
    // ������� ����� ��������� ������ ���������.
    ui8MessageBagLength = ui8Length;
    // ������� � ������� ���������� ���������.
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

    // ����� ������� �� SPI.
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
    // ������� ����� ������������� ���������.
    // ����� �� �������������� �������� � ������ ������.
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
    // ������� ��������� � �������� �� SPI_DATA_EXCHANGE_COMMAND?
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
    // ��������� ������������ ������?
    if (fui8BuffOverflow)
    {
//        printf("DataReceive fui8BuffOverflow err %d\n\r", (ui8LengthLocal));
        return -1;
    }
    // ������� ����� ����� SpiMT ��� ������ ������� �� SPI.
    ui8LengthLocal = *pui8ReceiveMessageBuff++;

    // ����� ��������� ������� ����?
    if (ui8LengthLocal <=
            (HEADER_LENGTH))
    {
//        printf("DataReceive HEADER_LENGTH %d\n\r", (ui8LengthLocal));
        return -1;
    }
    // ��������� ����������?
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
        // ������� ��������� �� �������� ����� ��������� � ����� SpiMT.
        pui8DataPointer = pui8ReceiveMessageBuff;
        // ������� ����� ��������� ������ ��������� � ����� SpiMT.
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

    // ������� �� ����� ������� � ����� SpiMT.
    pui8TransmitMessageBuff = GetTransmitMessageBuff();
    // ������� ������ ������ ���������.
    ui8LengthLocal = ui8MessageBagLength;
    // ����� ��������� ������� ����?
    if (ui8LengthLocal <=
            HEADER_LENGTH)
    {
        // �������� ������� ����� SpiMT.
        *pui8TransmitMessageBuff++ = 0;
        // �������� ����� ����� SpiMT.
        *pui8TransmitMessageBuff++ = 0;
        return -1;
    }

    // �������� ������� ����� SpiMT.
    *pui8TransmitMessageBuff++ = DATA_EXCHANGE_COMMAND;
    // �������� ����� ����� SpiMT.
    *pui8TransmitMessageBuff++ =
        (ui8LengthLocal +
         HEADER_LENGTH);

    // ������� �� ����� CRC � ����� SpiMT.
    pui8TransmitMessageBuff += ui8LengthLocal;
    // �������� CRC ����� SpiMT.
    ui16Data = usCrcSummTwoByteCalculation((unsigned char*)aui8TransmitMessageBuff,
                                           (ui8LengthLocal +
                                            HEADER_LENGTH));
    // �������� ������� ���� CRC ����� SpiMT.
    *pui8TransmitMessageBuff++ = (unsigned char)ui16Data;
    // �������� ������� ���� CRC ����� SpiMT.
    *pui8TransmitMessageBuff = (unsigned char)(ui16Data >> 8);

    return 0;
}


//-----------------------------------------------------------------------------------------------------
void CSpiSerialMT::DataExchangeEnable(void)
{
    // ������� ���� - ��������� ����� ������� �� SPI.
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
    // ��������� ���� - ��������� ����� ������� �� SPI.
    fui8BuffOverflow = 1;
}

//-----------------------------------------------------------------------------------------------------
void CSpiSerialMT::BuffOverflowFlagReset(void)
{
    // ������� ���� - ��������� ����� ������� �� SPI.
    fui8BuffOverflow = 0;
}

//-----------------------------------------------------------------------------------------------------
void CSpiSerialMT::BuffByteCounterIncrement(void)
{
    // �������������� ������� �������� ������ ��� ������ ������� �� SPI.
    nui8BuffByteCounter++;
}

//-----------------------------------------------------------------------------------------------------
int16_t CSpiSerialMT::BuffByteCounterGet(void)
{
    // ������� ������� �������� ������ ��� ������ ������� �� SPI.
    return nui8BuffByteCounter;
}

//-----------------------------------------------------------------------------------------------------
void CSpiSerialMT::BuffByteCounterClear(void)
{
    // ������� ������� ������ ��� ������ ������� �� SPI.
    nui8BuffByteCounter = 0;
}


//-----------------------------------------------------------------------------------------------------
// ModbusMT


//-----------------------------------------------------------------------------------------------------
// ������� ����� ������ ��������� � ���������� ��������� ���������.
void CModbusSerialMT::MessagePushInitialization(void)
{
    // ������� ��������� �� ����� ������ ���������.
    pui8MessageBag = GetTransmitMessageBuff();
    // ������� � ������ ������ ���������.
    ui8MessageBagLength = 0;
    // ������� � ������� ���������� ���������.
    ui8MessageBagIndex = 0;
}

//-----------------------------------------------------------------------------------------------------
// ������� ����� ������ ��������� � ���������� ��������� ���������.
void CModbusSerialMT::MessagePopInitialization(void)
{
    // ������� ��������� �� ����� ��������� ������ ���������.
    pui8MessageBag = pui8DataPointer;
    // ������� ����� ��������� ������ ���������.
    ui8MessageBagLength = ui8Length;
    // ������� � ������� ���������� ���������.
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
//    // ����� ������� �� Modbus.
//    // ��������� ����� ������ ��������� ���� ������� ������.
//    modbus_set_slave(pxMasterTcpCtx,
//                     MODULE_MIND_MODBUS_ADDRESS);
//    int8_t i;
//    // ���������� ������� � ������� ��������� ���� ������� ������ �� ��������� �����.
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
////        // ��������� ���� - ��������� ����� ������� �� Modbus.
////        DataExchangeFlagSet();
//    }
//    return i;
}

//-------------------------------------------------------------------------------------------------
int8_t CModbusSerialMT::DataReceive(void)
{
    // ����� ��������� ������� ����?
    if (ui8Length <=
            (HEADER_LENGTH))
    {
        return -1;
    }
    // ������� ��������� �� �������� ����� ��������� � ����� ModbusMT.
    pui8DataPointer = GetReceiveMessageBuff();
    return 0;
}

//-------------------------------------------------------------------------------------------------
int8_t CModbusSerialMT::DataTransmit(void)
{
    // ����� ��������� ������� ����?
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
    // ����� ��������� �� ����� ��������.
    return aui8TransmitMessageBuff;
}

