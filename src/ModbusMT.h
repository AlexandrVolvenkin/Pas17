//-------------------------------------------------------------------------------------------------
// ModbusMT - modbus multi trans.
//-------------------------------------------------------------------------------------------------
#ifndef MODBUSMT_H_INCLUDED
#define MODBUSMT_H_INCLUDED

//#include "AM335xPlatform.h"
#include "PasNewConfig.h"
#include <stdint.h>
#include "SerialMT.h"
#include "SpiMT.h"

//-------------------------------------------------------------------------------------------------
class CModbusMT
{
public:
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
        CRC_LENGTH = 2
    };

//-------------------------------------------------------------------------------------------------
public:
    static uint8_t fui8DataExchangeIsOccurred;
//    static uint8_t aui8ReceiveMessageBuff[CModbusMT::MESSAGE_BAG_MAX_LENGTH];
//    static uint8_t aui8TransmitMessageBuff[CModbusMT::MESSAGE_BAG_MAX_LENGTH];
//    static uint16_t nui16BuffByteCounter;
    static uint16_t nui16RxFrameLength;
    static uint16_t nui16TxFrameLength;

    static int16_t DataReceive(CSerialMT::TSerialMTData *);
    static int16_t DataTransmit(CSerialMT::TSerialMTData *);
    static int16_t DataExchangeSlave(modbus_mapping_t *);
    static int16_t DataExchangeMaster(modbus_t *, uint8_t);

    static uint8_t* GetModbusRxBuff(void)
    {
        // ����� ��������� �� ����� �����.
        return CSpiMT::aui8ReceiveMessageBuff;
    }
    static uint8_t* GetModbusTxBuff(void)
    {
        // ����� ��������� �� ����� ��������.
        return CSpiMT::aui8TransmitMessageBuff;
    }
    static void DataExchangeFlagSet(void)
    {
        // ��������� ���� - ��������� ����� ������� �� Modbus.
        CModbusMT::fui8DataExchangeIsOccurred = 1;
    }
    static void DataExchangeFlagReset(void)
    {
        // ������� ���� - ��������� ����� ������� �� Modbus.
        CModbusMT::fui8DataExchangeIsOccurred = 0;
    }
    static int8_t DataExchangeIsOccurred(void)
    {
        // ��������� ����� ������� �� Modbus?
        return CModbusMT::fui8DataExchangeIsOccurred;
    }

private:
};


#endif // MODBUSMT_H_INCLUDED
