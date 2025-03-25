//-------------------------------------------------------------------------------------------------
// ModbusMT - modbus multi trans.
//-------------------------------------------------------------------------------------------------
//#include "AM335xPlatform.h"
#include "PasNewConfig.h"
#include <stdint.h>
#include "ModbusMT.h"
#include "SerialMT.h"
#include "Crc.h"
#include "Error.h"

//-------------------------------------------------------------------------------------------------
//uint8_t CModbusMT::aui8ReceiveMessageBuff[CModbusMT::MESSAGE_BAG_MAX_LENGTH];
//uint8_t CModbusMT::aui8TransmitMessageBuff[CModbusMT::MESSAGE_BAG_MAX_LENGTH];
//uint16_t CModbusMT::nui16BuffByteCounter;
uint16_t CModbusMT::nui16RxFrameLength;
uint16_t CModbusMT::nui16TxFrameLength;
uint8_t CModbusMT::fui8DataExchangeIsOccurred;

//-------------------------------------------------------------------------------------------------
int16_t CModbusMT::DataReceive(CSerialMT::TSerialMTData *pxData)
{
    // ����� ��������� ������� ����?
    if (CModbusMT::nui16RxFrameLength <=
            (CModbusMT::HEADER_LENGTH))
    {
        return -1;
    }
    // ������� ��������� �� �������� ����� ��������� � ����� ModbusMT.
    pxData -> pui8DataPointer = CModbusMT::GetModbusRxBuff();
    // ������� ����� ��������� ������ ��������� � ����� ModbusMT.
    pxData -> i16Length = CModbusMT::nui16RxFrameLength;
    return 0;
}

//-------------------------------------------------------------------------------------------------
int16_t CModbusMT::DataTransmit(CSerialMT::TSerialMTData *pxData)
{
    // ����� ��������� ������� ����?
    if ((pxData -> i16Length) <=
            CModbusMT::HEADER_LENGTH)
    {
        return -1;
    }
    // ��������� ������ ���������� ��������� SerialMT � ������ �������� ������ �� Modbus.
    CModbusMT::nui16TxFrameLength = pxData -> i16Length;

    return 0;
}

//-------------------------------------------------------------------------------------------------
int16_t CModbusMT::DataExchangeSlave(modbus_mapping_t *pxModbusMapping)
{
    // ������� ����� ��������� ������ ��������� � ����� ModbusMT.
    CModbusMT::nui16RxFrameLength = pxModbusMapping -> current_message_nb_common;
    // ������� ��������� �� ��������� ����� ��������� SerialMT.
    (pxModbusMapping -> buffer_pointer) = CModbusMT::GetModbusTxBuff();
    // ��������� ������ ���������� ��������� SerialMT � ������ �������� ������ �� Modbus.
    pxModbusMapping -> current_message_nb_common = CModbusMT::nui16TxFrameLength;

    // ��������� ���� - ��������� ����� ������� �� Modbus.
    CModbusMT::DataExchangeFlagSet();
    nui8FrameCounter++; // modbus mind.
}

//-------------------------------------------------------------------------------------------------
int16_t CModbusMT::DataExchangeMaster(modbus_t *pxCtx, uint8_t ui8Address)
{
    // ����� ������� �� Modbus.
    // ��������� ����� ������ ��������� ���� ������� ������.
    modbus_set_slave(pxCtx,
                     ui8Address);
    // ���������� ������� � ������� ��������� ���� ������� ������ �� ��������� �����.
    int16_t i = modbus_data_exchange(pxCtx,
                                     CModbusMT::nui16TxFrameLength,
                                     CModbusMT::GetModbusTxBuff(),
                                     CModbusMT::GetModbusRxBuff());

    CModbusMT::nui16RxFrameLength = i;
    if (i != -1)
    {
        // ��������� ���� - ��������� ����� ������� �� Modbus.
        CModbusMT::DataExchangeFlagSet();
    }
    nui8FrameCounter++; // modbus mind.
    return i;
}

//-----------------------------------------------------------------------------------------------------
