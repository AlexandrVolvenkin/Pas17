//------------------------------------------------------------------------------
// Hardware abstract layer for FTDI EVE2 (only FT81x chips)
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifndef __EVE_HAL_H
#define __EVE_HAL_H
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "Hardware.h"

//class CSpi;
//class CGpio;

#include "Platform.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//****************************** МАКРОСЫ ***************************************
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class EVE_HAL
{
protected:

#pragma vector = INT1_vect
//    static __interrupt void __INT1Interrupt(void); // Прерывание по INT1

public:

    static inline void SetLine_CS(unsigned char);  // Установка состояния линии CS
    static inline void SetLine_PD(unsigned char);  // Установка состояния линии PD

    static inline void Enable(void);		// Выборка видеопроцессора
    static inline void Disable(void);		// Деактивация видеопроцессора

    enum OPERATIONS
    {
        MEM_MASK  = 0x3F,                             // <0011_1111> - маска команд R/W
        MEM_READ  = 0x00,                             // <00aa_aaaa> - команда чтения
        MEM_WRITE = 0x80,                             // <10aa_aaaa> - команда записи
        HOST_CMD  = 0x40                              // <01cc_cccc> - Host - команда
    };

    enum MEMORY_MAP
    {
        RAM_G		= 0x000000,			// 1024 KB
        RAM_G_END	= 0x0FFFFF,

        ROM_FONT	= 0x1E0000,			// 1152 KB
        ROM_FONT_END	= 0x2FFFFB,
        ROM_FONT_ADDR	= 0x2FFFFC,			// 4 Byte (unsigned long)

        RAM_DL	= 0x300000,			// 8 KB
        RAM_DL_END	= 0x301FFF,

        RAM_REG	= 0x302000,			// 4 KB
        RAM_REG_END	= 0x302FFF,

        RAM_CMD	= 0x308000,			// 4 KB
        RAM_CMD_END	= 0x308FFF
    };

    enum HOST_COMMANDS
    {
        ACTIVE	= 0x00,				// Отдельная команда
        STANDBY	= 0x41,
        SLEEP		= 0x42,
        PWR_DOWN	= 0x43,				// 0x50

        CLK_EXT	= 0x44,
        CLK_INT	= 0x48,

        RST_PULSE	= 0x68
    };

public:

    static void Command(unsigned char,unsigned char = 0x00);       // Host - команда
    static void Activation(void);                                  // Активация видепроцессора

    static unsigned char   Read8(unsigned long);                   // Чтение 1/2/4 байт данных
    static unsigned short Read16(unsigned long);
    static unsigned long  Read32(unsigned long);

    static void  Write8(unsigned long,unsigned char);              // Запись 1/2/4 байт данных
    static void Write16(unsigned long,unsigned short);
    static void Write32(unsigned long, unsigned long);

    static  void Read(unsigned long,void *,unsigned char);              // Чтение блока данных
    static  void Read(void *,unsigned long,unsigned char);              // Чтение блока данных

    static  void Write(unsigned long,void *,unsigned short);            // Запись блока данных
//    static  void Write(unsigned long,void __farflash*,unsigned short);  // Запись блока данных (Flash)

    static void Initialize(void);				        // Инициализация интерфейса
    static void InitializeNEW(bool = false);
    static void InitializeFAST(bool = false);


//*******************************************
    static void WriteControlPoints(void);
    static unsigned short CheckControlPoints(void);
    static unsigned char  CheckControlPoints2(void);

    static void SetLighting(unsigned char);

};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//************************** INLINE - ФУНКЦИИ **********************************
//------------------------------------------------------------------------------
void EVE_HAL::SetLine_CS(unsigned char Value)
{
//    if(Value) PORTD |= BIT(HARD::EVE_CS);
//    else PORTD &= ~BIT(HARD::EVE_CS);

    if (Value)
    {
        CGpio::SetPin(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    }
    else
    {
        CGpio::ClearPin(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
    }
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_HAL::SetLine_PD(unsigned char Value)
{
//    if(Value) PORTD |= BIT(HARD::EVE_PD);
//    else PORTD &= ~BIT(HARD::EVE_PD);

    if (Value)
    {
        CGpio::SetPin(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    }
    else
    {
        CGpio::ClearPin(SPI_CHIP_SELECT_PIN_1_PORT, SPI_CHIP_SELECT_PIN_1);
    }
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_HAL::Enable(void)
{
//    PORTD &= ~BIT(HARD::EVE_CS);
    CGpio::SetPin(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_HAL::Disable(void)
{
//    PORTD |= BIT(HARD::EVE_CS);
    CGpio::ClearPin(SPI_CHIP_SELECT_PIN_0_PORT, SPI_CHIP_SELECT_PIN_0);
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#endif //__EVE_HAL_H
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
