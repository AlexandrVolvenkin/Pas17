//-------------------------------------------------------------------------------------------------
// SpiMT - spi multi trans.
//-------------------------------------------------------------------------------------------------
#include <stdint.h>
#include "SpiMT.h"
#include "SerialMT.h"
#include "Crc.h"
#include "PasNewConfig.h"
#include "Error.h"

//-------------------------------------------------------------------------------------------------
uint8_t CSpiMT::aui8ReceiveMessageBuff[CSpiMT::MESSAGE_BAG_MAX_LENGTH];
uint8_t CSpiMT::aui8TransmitMessageBuff[CSpiMT::MESSAGE_BAG_MAX_LENGTH];
uint16_t CSpiMT::nui16BuffByteCounter;
uint16_t CSpiMT::nui16RxFrameLength;
uint16_t CSpiMT::nui16TxFrameLength;
uint8_t CSpiMT::fui8DataExchangeIsOccurred;
uint8_t CSpiMT::fui8BuffOverflow;

//-------------------------------------------------------------------------------------------------
int16_t CSpiMT::DataReceive(CSerialMT::TSerialMTData *pxData)
{
    // ������� ����� ����� SpiMT ��� ������ ������� �� SPI.
    CSpiMT::nui16RxFrameLength = (CSpiMT::aui8ReceiveMessageBuff[CSpiMT::RX_BUFF_BEGIN_OFFSET + CSpiMT::DATA_LENGTH_BYTE_OFFSET]);

    // ������� ��������� � �������� �� DATA_EXCHANGE_COMMAND?
    if (CSpiMT::aui8ReceiveMessageBuff[CSpiMT::RX_BUFF_BEGIN_OFFSET + CSpiMT::COMMAND_BYTE_OFFSET] !=
            CSpiMT::DATA_EXCHANGE_COMMAND)
    {
        return -1;
    }
    // ��������� ������������ ������?
    if (CSpiMT::fui8BuffOverflow)
    {
        return -1;
    }
    // ����� ��������� ������� ����?
    if (CSpiMT::nui16RxFrameLength <=
            (CSpiMT::HEADER_LENGTH))
    {
        return -1;
    }
    // ��������� ����������?
    if(iCrcSummTwoByteCompare(&CSpiMT::aui8ReceiveMessageBuff[CSpiMT::RX_BUFF_BEGIN_OFFSET],
                              (CSpiMT::aui8ReceiveMessageBuff[CSpiMT::RX_BUFF_BEGIN_OFFSET + CSpiMT::DATA_LENGTH_BYTE_OFFSET])) == -1)
    {
        return -1;
    }
    else
    {
        // ������� ��������� �� �������� ����� ��������� � ����� SpiMT.
        pxData -> pui8DataPointer = &CSpiMT::aui8ReceiveMessageBuff[CSpiMT::RX_BUFF_BEGIN_OFFSET + CSpiMT::DATA_BYTE_OFFSET];
        // ������� ����� ��������� ������ ��������� � ����� SpiMT.
        pxData -> i16Length = ((int16_t)(CSpiMT::nui16RxFrameLength -
                                         CSpiMT::HEADER_LENGTH));
        return 0;
    }
}

//-------------------------------------------------------------------------------------------------
int16_t CSpiMT::DataTransmit(CSerialMT::TSerialMTData *pxData)
{
    uint16_t ui16Data;
    uint16_t ui16Index;

    // ����� ��������� ������� ����?
    if ((pxData -> i16Length) <=
            CSpiMT::HEADER_LENGTH)
    {
        return -1;
    }

    // ��������� ������ ���������� ��������� SerialMT � ������ �������� ������ �� Modbus.
    CSpiMT::nui16TxFrameLength = pxData -> i16Length;
    // ������� �� ����� ������� � ����� SpiMT.
    ui16Index = CSpiMT::COMMAND_BYTE_OFFSET;
    // �������� ������� ����� SpiMT.
    CSpiMT::aui8TransmitMessageBuff[ui16Index++] = CSpiMT::DATA_EXCHANGE_COMMAND;
    // �������� ����� ����� SpiMT.
    CSpiMT::aui8TransmitMessageBuff[ui16Index++] =
        ((pxData -> i16Length) +
         CSpiMT::HEADER_LENGTH);
    // ������� �� ����� CRC � ����� SpiMT.
    ui16Index += (pxData -> i16Length);
    // �������� CRC ����� SpiMT.
    ui16Data = usCrcSummTwoByteCalculation((unsigned char*)CSpiMT::aui8TransmitMessageBuff,
                                           ((pxData -> i16Length) +
                                            CSpiMT::HEADER_LENGTH));
    // �������� ������� ���� CRC ����� SpiMT.
    CSpiMT::aui8TransmitMessageBuff[ui16Index++] = (unsigned char)ui16Data;
    // �������� ������� ���� CRC ����� SpiMT.
    CSpiMT::aui8TransmitMessageBuff[ui16Index] = (unsigned char)(ui16Data >> 8);

    return 0;
}

//-----------------------------------------------------------------------------------------------------
void CSpiMT::DataExchangeEnable(void)
{
    // ������� ���� - ��������� ����� ������� �� SPI.
    CSpiMT::fui8DataExchangeIsOccurred = 0;
//    vSpiDataExchangeEnable();
}

//-----------------------------------------------------------------------------------------------------
void CSpiMT::DataExchangeDisable(void)
{
//    vSpiDataExchangeDisable();
}

//-------------------------------------------------------------------------------------------------
void CSpiMT::PrepareDataExchange(void)
{
    CSpiMT::nui16BuffByteCounter = 0;
    CSpiMT::fui8BuffOverflow = 0;
}

//-----------------------------------------------------------------------------------------------------
int8_t CSpiMT::DataExchangeIsOccurred(void)
{
    // ��������� ����� ������� �� SPI?
    return CSpiMT::fui8DataExchangeIsOccurred;
}
