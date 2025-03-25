//-------------------------------------------------------------------------------------------------
// SpiMT - spi multi trans.
//-------------------------------------------------------------------------------------------------
#ifndef SPIMT_H_INCLUDED
#define SPIMT_H_INCLUDED

#include <stdint.h>
#include "SerialMT.h"

//-------------------------------------------------------------------------------------------------
class CSpiMT
{
public:
    // ������ ����� SpiMT: ������� - 1 ����, ���������� ���� � ����� - 1 ����,
    // ������, ����������� �����(����� ����������� ���� ������(�������, ����������, ������)) - 2 �����.
    typedef enum
    {
        // ������������ ����� ���������.
        MESSAGE_BAG_MAX_LENGTH = 256,
        // ��� ������� - ����� �������.
        DATA_EXCHANGE_COMMAND = 0x42,
        // ���������� slave ���������� ���� ����� ���������.
        // �������� �� ������ � ������� RX SPI_MT.
        RX_BUFF_BEGIN_OFFSET = 1,
        // 0 - �������� �� ������� � ������� RX SPI_MT.
        COMMAND_BYTE_OFFSET = 0,
        // 1 - �������� �� ������ ��������� � ������� RX SPI_MT.
        DATA_LENGTH_BYTE_OFFSET = 1,
        // 2 - �������� �� ������ � ������� RX SPI_MT.
        DATA_BYTE_OFFSET = 2,
        // ����� ��������� ���������.
        HEADER_LENGTH = 2,
        // ����� ����������� �����.
        CRC_LENGTH = 2,
        // �������� � ���� �� ���� SPI ���������� � ���������� ������.
        // ���� ����� ������������� ����� ������ ����� ������������ �����, ��
        // ����������� ���� ����� �������.
        // ������� ����� ������������� ����� ������ ���� ������ ������������ �����.
        // ������������ ����� ������������ ����� �������� �� ������ ��������������.
        // ������������ ����� ������������ �����.
        MAX_RECEIVED_FRAME_LENGTH = 32
    };

//-------------------------------------------------------------------------------------------------
public:
    static uint8_t fui8DataExchangeIsOccurred;
    static uint8_t fui8BuffOverflow;

    static uint8_t aui8ReceiveMessageBuff[CSpiMT::MESSAGE_BAG_MAX_LENGTH];
    static uint8_t aui8TransmitMessageBuff[CSpiMT::MESSAGE_BAG_MAX_LENGTH];
    static uint16_t nui16BuffByteCounter;
    static uint16_t nui16RxFrameLength;
    static uint16_t nui16TxFrameLength;

//-------------------------------------------------------------------------------------------------
    static int16_t DataReceive(CSerialMT::TSerialMTData *pxData);
    static int16_t DataTransmit(CSerialMT::TSerialMTData *pxData);
    static void DataExchangeEnable(void);
    static void DataExchangeDisable(void);
    static void PrepareDataExchange(void);
    static int8_t DataExchangeIsOccurred(void);

    static uint8_t* GetSpiRxBuff(void)
    {
        // ����� ��������� �� ����� �����.
        return CSpiMT::aui8ReceiveMessageBuff;
    }
    static uint8_t* GetSpiTxBuff(void)
    {
        // ����� ��������� �� ����� ��������.
        return CSpiMT::aui8TransmitMessageBuff;
    }
    static uint8_t* GetReceiveMessageBuff(void)
    {
        return &CSpiMT::aui8ReceiveMessageBuff[CSpiMT::DATA_BYTE_OFFSET];
    }
    static uint8_t* GetTransmitMessageBuff(void)
    {
        return &CSpiMT::aui8TransmitMessageBuff[CSpiMT::DATA_BYTE_OFFSET];
    }
    static int16_t GetMessageLength(void)
    {
        return (int16_t)(CSpiMT::nui16RxFrameLength -
                         CSpiMT::HEADER_LENGTH);
    }
    static void DataExchangeFlagSet(void)
    {
        // ��������� ���� - ��������� ����� ������� �� SPI.
        CSpiMT::fui8DataExchangeIsOccurred = 1;
    }
    static void DataExchangeFlagReset(void)
    {
        // ������� ���� - ��������� ����� ������� �� SPI.
        CSpiMT::fui8DataExchangeIsOccurred = 0;
    }
    static void BuffOverflowFlagSet(void)
    {
        // ��������� ���� - ��������� ����� ������� �� SPI.
        CSpiMT::fui8BuffOverflow = 1;
    }
    static void BuffOverflowFlagReset(void)
    {
        // ������� ���� - ��������� ����� ������� �� SPI.
        CSpiMT::fui8BuffOverflow = 0;
    }
    static void BuffByteCounterIncrement(void)
    {
        // �������������� ������� �������� ������ ��� ������ ������� �� SPI.
        CSpiMT::nui16BuffByteCounter++;
    }
    static int16_t BuffByteCounterGet(void)
    {
        // ������� ������� �������� ������ ��� ������ ������� �� SPI.
        return CSpiMT::nui16BuffByteCounter;
    }
    static void BuffByteCounterClear(void)
    {
        // ������� ������� ������ ��� ������ ������� �� SPI.
        CSpiMT::nui16BuffByteCounter = 0;
    }
    static void FrameLengthGet(void)
    {
        // ������� ����� ����� SpiMT ��� ������ ������� �� SPI.
        CSpiMT::nui16RxFrameLength = CSpiMT::nui16BuffByteCounter;
    }

private:
};

#endif // SPIMT_H_INCLUDED
