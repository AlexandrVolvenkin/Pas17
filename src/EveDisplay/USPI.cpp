//------------------------------------------------------------------------------
// Реализация USART_1 в режиме SPI Master
// SPI mode 0 (CPOL & CPHA = 0), MSB First
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "USPI.h"

#include "Platform.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//***************************** ФУНКЦИИ ****************************************
//------------------------------------------------------------------------------
// Начальная инициализация
void USPI::Initialize(void)
{
//    UBRR1  = 0;                            // Обнуляем регистр скорости обмена
//
//    UCSR1C = BIT(UMSEL11)|BIT(UMSEL10);    // Master SPI, MSB first, UCPOL1 & UCPHA1 = 0 (Mode 0)
//    UCSR1B = BIT(RXEN1)|BIT(TXEN1);        // Разрешаем приемник / передатчик
//
////UBRR1 = 1;                            // 3686400 bod                          [## XTAL DIV = 14.7456]
//    UBRR1  = 0;                            // 3686400 bod                          [## XTAL DIV = 7.37280]  0
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Запись блока данных
 void USPI::Write(void *Data,unsigned short Lenght)
{
//    unsigned char *Ptr = ((unsigned char *)Data);
//
//    UCSR1A |= BIT(TXC1);                   // Сбрасываем флаг передачи
//
//    /*
//     while(Lenght > 0)
//     {
//      while (!(UCSR1A & BIT(UDRE1)));       // Wait for empty transmit buffer
//
//      UDR1 = *Ptr++;                        // Put data into buffer, sends the data
//      Lenght--;
//     };
//
//     while (!(UCSR1A & BIT(TXC1)));         // Wait until transmission is complete
//    */
//
//
//
//    for(unsigned short i = 0; i < Lenght; i++)     // NOT OPTIMAL!!!
//    {
//        while (!(UCSR1A & BIT(UDRE1)));               // Wait for empty transmit buffer
//
//        UDR1 = *Ptr++;                                // Put data into buffer, sends the data
//    };
//
//
//    while (!(UCSR1A & BIT(TXC1)));         // Wait until transmission is complete
};
////------------------------------------------------------------------------------
////------------------------------------------------------------------------------
//// Запись блока данных
// void USPI::Write(void  *Data,unsigned short Lenght)
//{
////    unsigned char  *Ptr = ((unsigned char  *)Data);
////
////    UCSR1A |= BIT(TXC1);                   // Сбрасываем флаг передачи
////
////    /*
////     while(Lenght > 0)
////     {
////      while (!(UCSR1A & BIT(UDRE1)));       // Wait for empty transmit buffer
////
////      UDR1 = *Ptr++;                        // Put data into buffer, sends the data
////      Lenght--;
////     };
////
////     while (!(UCSR1A & BIT(TXC1)));         // Wait until transmission is complete
////
////    */
////
////
////    for(unsigned short i = 0; i < Lenght; i++)     // NOT OPTIMAL!!!
////    {
////        while (!(UCSR1A & BIT(UDRE1)));               // Wait for empty transmit buffer
////
////        UDR1 = *Ptr++;                                // Put data into buffer, sends the data
////    };
////
////
////    while (!(UCSR1A & BIT(TXC1)));         // Wait until transmission is complete
////
////
////
//
//
//};
////------------------------------------------------------------------------------
////------------------------------------------------------------------------------
////------------------------------------------------------------------------------
////------------------------------------------------------------------------------
