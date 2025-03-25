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
    // формат кадра SpiMT: команда - 1 байт, количество байт в кадре - 1 байт,
    // данные, контрольная сумма(сумма содержимого всех байтов(команда, количество, данные)) - 2 байта.
    typedef enum
    {
        // максимальная длина сообщения.
        MESSAGE_BAG_MAX_LENGTH = 256,
        // код команды - обмен данными.
        DATA_EXCHANGE_COMMAND = 0x42,
        // устройство slave отправляет байт после получения.
        // смещение на начало в массиве RX SPI_MT.
        RX_BUFF_BEGIN_OFFSET = 1,
        // 0 - смещение на команду в массиве RX SPI_MT.
        COMMAND_BYTE_OFFSET = 0,
        // 1 - смещение на размер сообщения в массиве RX SPI_MT.
        DATA_LENGTH_BYTE_OFFSET = 1,
        // 2 - смещение на данные в массиве RX SPI_MT.
        DATA_BYTE_OFFSET = 2,
        // длина заголовка сообщения.
        HEADER_LENGTH = 2,
        // длина контрольной суммы.
        CRC_LENGTH = 2,
        // передача и приём на шине SPI происходит в дуплексном режиме.
        // если длина передаваемого кадра меньше длины принимаемого кадра, то
        // принимаемый кадр будет потерян.
        // поэтому длина передаваемого кадра должна быть больше принимаемого кадра.
        // максимальная длина принимаемого кадра известна на стадии проектирования.
        // максимальная длина принимаемого кадра.
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
        // вернём указатель на буфер приёма.
        return CSpiMT::aui8ReceiveMessageBuff;
    }
    static uint8_t* GetSpiTxBuff(void)
    {
        // вернём указатель на буфер передачи.
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
        // установим флаг - произошел обмен данными по SPI.
        CSpiMT::fui8DataExchangeIsOccurred = 1;
    }
    static void DataExchangeFlagReset(void)
    {
        // сбросим флаг - произошел обмен данными по SPI.
        CSpiMT::fui8DataExchangeIsOccurred = 0;
    }
    static void BuffOverflowFlagSet(void)
    {
        // установим флаг - произошел обмен данными по SPI.
        CSpiMT::fui8BuffOverflow = 1;
    }
    static void BuffOverflowFlagReset(void)
    {
        // сбросим флаг - произошел обмен данными по SPI.
        CSpiMT::fui8BuffOverflow = 0;
    }
    static void BuffByteCounterIncrement(void)
    {
        // инкрементируем счётчик принятых байтов при обмене данными по SPI.
        CSpiMT::nui16BuffByteCounter++;
    }
    static int16_t BuffByteCounterGet(void)
    {
        // получим счётчик принятых байтов при обмене данными по SPI.
        return CSpiMT::nui16BuffByteCounter;
    }
    static void BuffByteCounterClear(void)
    {
        // обнулим счётчик байтов при обмене данными по SPI.
        CSpiMT::nui16BuffByteCounter = 0;
    }
    static void FrameLengthGet(void)
    {
        // получим длину кадра SpiMT при обмене данными по SPI.
        CSpiMT::nui16RxFrameLength = CSpiMT::nui16BuffByteCounter;
    }

private:
};

#endif // SPIMT_H_INCLUDED
