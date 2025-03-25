//-----------------------------------------------------------------------------------------------------
// SerialMT - serial multi trans.
//-----------------------------------------------------------------------------------------------------
#ifndef SERIALMT_H_INCLUDED
#define SERIALMT_H_INCLUDED

//-------------------------------------------------------------------------------------------------
#include <stdint.h>
#include "PasNewConfig.h"

//-------------------------------------------------------------------------------------------------
class CSerialMT
{
public:
    // ������ ���������� ��������� SerialMT: ���������, ������.
    // ��������� ���������: ����(����� �������-������) - 1 ����, ���������� ���� ������(��� ���������) - 1 ����.
    // ������: ������ ���������.

    struct TSerialMTData
    {
        // ��������� �� ��������� ���������.
        uint8_t *pui8DataPointer;
        // ����� ���������� ���������.
        int16_t i16Length;
        // ����� �������-������.
        uint8_t ui8Address;
        // ����� �������-������(�������-������ ����� ��������� � ���� ������� ��� �������).
        uint8_t ui8Mode;
    };

    typedef enum
    {
        // ����� ��������� ���������� ���������.
        SERIAL_MT_HEADER_LENGTH = 2,
        // ������������ ����� ������ ���������.
        SERIAL_MT_MESSAGE_BAG_MAX_LENGTH = 252
    };

//-------------------------------------------------------------------------------------------------
// SpiMT
    // ������ ����� SpiMT: ������� - 1 ����, ���������� ���� � ����� - 1 ����,
    // ������, ����������� �����(����� ����������� ���� ������(�������, ����������, ������)) - 2 �����.
    typedef enum
    {
        // ������������ ����� ���������.
        MESSAGE_BAG_MAX_LENGTH = 256,
        // ��� ������� - ����� �������.
        DATA_EXCHANGE_COMMAND = 0x42,
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
        // ���� ����� ������������� �������� ����� ������ ����� ������������ �����, ��
        // ����������� ���� ����� �������.
        // ������� ����� ������������� �������� ����� ������ ���� ������ ������������ �����.
        // ������������ ����� ������������ ����� �������� �� ������ ��������������.
        // ������������ ����� ������������ �����.
        MAX_RECEIVED_FRAME_LENGTH = 24
    };

    // �������� �� ����� �������-������ � ��������� ���������� ���������.
    static const uint8_t SERIAL_MT_PORT_OFFSET = 0;
    // �������� �� ����� ���������� ��������� � ��������� ���������� ���������.
    static const uint8_t SERIAL_MT_MESSAGE_LENGTH_OFFSET = 1;
    // �������� �� ������ �� ��������� ���������.
    static const uint8_t SERIAL_MT_DATA_OFFSET = 2;
    // ��� ������������� �����.
    static const uint8_t SERIAL_MT_HANDSHAKING_BIT = 7;
    static const uint8_t SERIAL_MT_WRITE = 0x80;
    static const uint8_t SERIAL_MT_READ = 0;

    uint8_t* ucGetBuffMessageBag(void);
    uint8_t GetLengthMessageBag(void);
    uint8_t IsWrite(void);
    uint8_t* MessagePushRequest(uint8_t , uint8_t );
    int8_t MessagePop(void);

    int8_t DataExchangeIsOccurred(void);
    void DataExchangeFlagSet(void);
    void DataExchangeFlagReset(void);
    virtual uint8_t* GetReceiveMessageBuff(void) = 0;
    virtual uint8_t* GetTransmitMessageBuff(void) = 0;
    virtual int8_t DataReceive(void) = 0;
    virtual int8_t DataTransmit(void) = 0;
    virtual int8_t DataExchange(void) = 0;
    virtual void MessagePushInitialization(void);
    virtual void MessagePopInitialization(void);
    virtual uint8_t MessagePopCheck(void);

//-------------------------------------------------------------------------------------------------
    // ����� ������ ���������.
    uint8_t ui8MessageBagLength;
    // ������ � ������ ������ ���������.
    // �������� �������� �� ����� ��� ���������� ���������� ���������.
    uint8_t ui8MessageBagIndex;
    // ��������� �� ����� ������ ���������.
    uint8_t *pui8MessageBag;
    // ��������� �� ��������� ���������.
    uint8_t *pui8DataPointer;
    // ����� ���������� ���������.
    uint8_t ui8Length;
    // ����� �������-������.
    uint8_t ui8Address;

    static uint8_t aui8ReceiveMessageBuff[MESSAGE_BAG_MAX_LENGTH];
    static uint8_t aui8TransmitMessageBuff[MESSAGE_BAG_MAX_LENGTH];
    static uint8_t nui8BuffByteCounter;
    // ������� ������ ��� ���������� ������������ ������ ��� ������ ������� �� SPI.
    static uint8_t ui8SpiReceivedByteCounter;
    static uint8_t ui8TempSPDR;
    uint8_t fui8DataExchangeIsOccurred;
    uint8_t fui8BuffOverflow;

//-------------------------------------------------------------------------------------------------
};



#define SPI_ONE_BYTE_RECEIVE_TIME ((1000000 / 150000) * 9)
// ���������� ���� �������� �� ���� ����������� ��� ������ ������� �� SPI.
#define SPI_ONE_MS_RECEIVED_BYTE_QUANTITY (1000 / SPI_ONE_BYTE_RECEIVE_TIME)

//-------------------------------------------------------------------------------------------------
class CSpiSerialMT : public CSerialMT
{
public:
// SpiMT
    // ������ ����� SpiMT: ������� - 1 ����, ���������� ���� � ����� - 1 ����(�������, ����������, ������),
    // ������, ����������� �����(����� ����������� ���� ������(�������, ����������, ������)) - 2 �����.
    typedef enum
    {
        // ������������ ����� ���������.
        MESSAGE_BAG_MAX_LENGTH = 256,
        // ��� ������� - ����� �������.
        DATA_EXCHANGE_COMMAND = 0x42,
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
        MAX_RECEIVED_FRAME_LENGTH = 24
    };

//-------------------------------------------------------------------------------------------------
    uint8_t* GetReceiveMessageBuff(void);
    uint8_t* GetTransmitMessageBuff(void);
    void MessagePushInitialization(void);
    void MessagePopInitialization(void);
    int8_t DataReceive(void);
    int8_t DataTransmit(void);
    int8_t DataExchange(void);
    void DataExchangeEnable(void);
    void DataExchangeDisable(void);
    void PrepareDataExchange(void);

    void BuffOverflowFlagSet(void);
    void BuffOverflowFlagReset(void);
    void BuffByteCounterIncrement(void);
    int16_t BuffByteCounterGet(void);
    void BuffByteCounterClear(void);

////-----------------------------------------------------------------------------------------------------------------
//// ������������ ��������� ����� �� ����� ������ ������� �� SPI.
//// �� ����� ����� SPI ������ ���������� ������� ��������� � ���������� CTimers::ui16SystemTimer
//// �� ����������������. ����� �������� ������������ �� ������������ ����������
//// �������, ������� � �������� �����. ���� ������ ����� �� �������� 100000��� ��������
//// SPI_ONE_BYTE_RECEIVE_TIME ((1000000 / 100000) * 9).
//    void SpiFrameTimeCalculate(void)
//    {
//        // ����� ����� ������ ��� ������ ������� �� SPI ��������� 1��?
//        if (ui8SpiReceivedByteCounter >= SPI_ONE_MS_RECEIVED_BYTE_QUANTITY)
//        {
//            // ������� ������� ������ ��� ���������� ������������ ������ ��� ������ ������� �� SPI.
//            ui8SpiReceivedByteCounter = 0;
//            // �������������� ��������� ������.
//            CTimers::ui16SystemTimer++;
//        }
//    }


//-------------------------------------------------------------------------------------------------
};

//-------------------------------------------------------------------------------------------------
class CModbusSerialMT : public CSerialMT
{
public:

//-------------------------------------------------------------------------------------------------
// ModbusMT
    typedef enum
    {
        // ������������ ����� ���������.
        MESSAGE_BAG_MAX_LENGTH = 256,
        // ��� ������� - ����� �������.
        DATA_EXCHANGE_COMMAND = 0x42,
        // 0 - �������� �� ������� � ������� RX SPI_MT.
        COMMAND_BYTE_OFFSET = 0,
        // 1 - �������� �� ������ ��������� � ������� RX SPI_MT.
        SPI_DATA_LENGTH_BYTE_OFFSET = 1,
        // 2 - �������� �� ������ � ������� RX SPI_MT.
        DATA_BYTE_OFFSET = 2,
        // ����� ��������� ���������.
        HEADER_LENGTH = 2,
        // ����� ����������� �����.
        CRC_LENGTH = 2
    };

//-------------------------------------------------------------------------------------------------
public:

//-------------------------------------------------------------------------------------------------
    uint8_t* GetReceiveMessageBuff(void);
    uint8_t* GetTransmitMessageBuff(void);
    void MessagePushInitialization(void);
    void MessagePopInitialization(void);
    int8_t DataReceive(void);
    int8_t DataTransmit(void);
    int8_t DataExchange(void);

//-------------------------------------------------------------------------------------------------
};

extern CSpiSerialMT xCSpiSerialMT;
extern CModbusSerialMT xCModbusSerialMT;


//-----------------------------------------------------------------------------------------------------

#endif // SERIALMT_H_INCLUDED

