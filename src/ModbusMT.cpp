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
    // длина сообщения слишком мала?
    if (CModbusMT::nui16RxFrameLength <=
            (CModbusMT::HEADER_LENGTH))
    {
        return -1;
    }
    // получим указатель на принятое общее сообщение в кадре ModbusMT.
    pxData -> pui8DataPointer = CModbusMT::GetModbusRxBuff();
    // получим длину принятого общего сообщения в кадре ModbusMT.
    pxData -> i16Length = CModbusMT::nui16RxFrameLength;
    return 0;
}

//-------------------------------------------------------------------------------------------------
int16_t CModbusMT::DataTransmit(CSerialMT::TSerialMTData *pxData)
{
    // длина сообщения слишком мала?
    if ((pxData -> i16Length) <=
            CModbusMT::HEADER_LENGTH)
    {
        return -1;
    }
    // передадим размер созданного сообщение SerialMT в функию отправки данных по Modbus.
    CModbusMT::nui16TxFrameLength = pxData -> i16Length;

    return 0;
}

//-------------------------------------------------------------------------------------------------
int16_t CModbusMT::DataExchangeSlave(modbus_mapping_t *pxModbusMapping)
{
    // получим длину принятого общего сообщения в кадре ModbusMT.
    CModbusMT::nui16RxFrameLength = pxModbusMapping -> current_message_nb_common;
    // получим указатель на созданное общее сообщение SerialMT.
    (pxModbusMapping -> buffer_pointer) = CModbusMT::GetModbusTxBuff();
    // передадим размер созданного сообщение SerialMT в функию отправки данных по Modbus.
    pxModbusMapping -> current_message_nb_common = CModbusMT::nui16TxFrameLength;

    // установим флаг - произошел обмен данными по Modbus.
    CModbusMT::DataExchangeFlagSet();
    nui8FrameCounter++; // modbus mind.
}

//-------------------------------------------------------------------------------------------------
int16_t CModbusMT::DataExchangeMaster(modbus_t *pxCtx, uint8_t ui8Address)
{
    // обмен данными по Modbus.
    // установим адрес модуля локальной сети нижнего уровня.
    modbus_set_slave(pxCtx,
                     ui8Address);
    // обменяемся данными с модулем локальной сети нижнего уровня во временный буфер.
    int16_t i = modbus_data_exchange(pxCtx,
                                     CModbusMT::nui16TxFrameLength,
                                     CModbusMT::GetModbusTxBuff(),
                                     CModbusMT::GetModbusRxBuff());

    CModbusMT::nui16RxFrameLength = i;
    if (i != -1)
    {
        // установим флаг - произошел обмен данными по Modbus.
        CModbusMT::DataExchangeFlagSet();
    }
    nui8FrameCounter++; // modbus mind.
    return i;
}

//-----------------------------------------------------------------------------------------------------
