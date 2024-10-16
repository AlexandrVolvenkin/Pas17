//------------------------------------------------------------------------------
// Hardware abstract layer for FTDI EVE2 (only FT81x chips)
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "EVE_HAL.h"
#include "EVE_Resources.h"
#include "USPI.h"
//#include "TASK.h"


//#include "Hardware.h"

#include "Platform.h"

extern CSpi xSpiCommunicationDevice;

using namespace std;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//***************************** ПРЕРЫВАНИЯ *************************************
//------------------------------------------------------------------------------
//// Внешнее прерывание от видеопроцессора по входу INT1
//__interrupt void EVE_HAL::__INT1Interrupt(void)
//{
//    if((PIND & BIT(HARD::EVE_INT)) == 0)
//    {
//        TASK::SetInInterrupt(TASK::EVE_EVENT);
//
//        HARD::DisableSound();
//    };
//
//    /*
//     extern volatile unsigned char FTASK;
//
//     if((PINE & BIT(HARD::EVE_INT)) == 0)
//     {
//      FTASK |= BIT(HARD::EVE_EVENT);
//     };*/
//};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//***************************** ФУНКЦИИ ****************************************
//------------------------------------------------------------------------------
// Отправка Host-команды
void EVE_HAL::Command(unsigned char Cmd,unsigned char Parameters)
{
    Enable();

//    USPI::Write((Cmd & MEM_MASK) | HOST_CMD);       // <01cc_cccc> - Host-command
//    USPI::Write(Parameters);
//    USPI::Write(0x00);


    std::cout << "EVE_HAL::Command 1"  << std::endl;
    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    usleep(100);
    auiSpiTxBuffer[0] = ((Cmd & MEM_MASK) | HOST_CMD);
    auiSpiTxBuffer[1] = Parameters;
    auiSpiTxBuffer[2] = 0x00;


    {
        cout << "EVE_HAL::Command auiSpiTxBuffer" << endl;
        unsigned char *pucSourceTemp;
        pucSourceTemp = (unsigned char*)auiSpiTxBuffer;
        for(int i=0; i<16; )
        {
            for(int j=0; j<8; j++)
            {
                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
            }
            cout << endl;
            i += 8;
        }
    }

    xSpiCommunicationDevice.Exchange(0,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     3,
                                     LOW_SPEED_IN_HZ);
    std::cout << "EVE_HAL::Command 2"  << std::endl;

    {
        cout << "EVE_HAL::Command auiSpiRxBuffer" << endl;
        unsigned char *pucSourceTemp;
        pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
        for(int i=0; i<16; )
        {
            for(int j=0; j<8; j++)
            {
                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
            }
            cout << endl;
            i += 8;
        }
    }


    Disable();
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Активация видепроцессора
void EVE_HAL::Activation(void)
{
    Enable();

//    USPI::Write(0x00);                              // Фиктивное чтение из 0x000000
//    USPI::Write(0x00);
//    USPI::Write(0x00);


    std::cout << "EVE_HAL::Activation 1"  << std::endl;
    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    usleep(100);
    auiSpiTxBuffer[0] = 0x00;
    auiSpiTxBuffer[1] = 0x00;
    auiSpiTxBuffer[2] = 0x00;


    {
        cout << "EVE_HAL::Activation auiSpiTxBuffer" << endl;
        unsigned char *pucSourceTemp;
        pucSourceTemp = (unsigned char*)auiSpiTxBuffer;
        for(int i=0; i<16; )
        {
            for(int j=0; j<8; j++)
            {
                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
            }
            cout << endl;
            i += 8;
        }
    }

    xSpiCommunicationDevice.Exchange(0,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     3,
                                     LOW_SPEED_IN_HZ);
    std::cout << "EVE_HAL::Activation 2"  << std::endl;

    {
        cout << "EVE_HAL::Activation auiSpiRxBuffer" << endl;
        unsigned char *pucSourceTemp;
        pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
        for(int i=0; i<16; )
        {
            for(int j=0; j<8; j++)
            {
                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
            }
            cout << endl;
            i += 8;
        }
    }



    Disable();
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Чтение байта данных по заданному адресу
unsigned char EVE_HAL::Read8(unsigned long Address)
{
    Enable();

//    USPI::Write((Address >> 16L) & MEM_MASK);       // <00aa_aaaa> - команда чтения
//    USPI::Write( Address >>  8L);
//    USPI::Write( Address >>  0L);
//
//    USPI::Write(0x00);				// Пустой синхро-байт

//    unsigned char Readed = USPI::Read();

    std::cout << "EVE_HAL::Read8 1"  << std::endl;
    uint8_t auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    uint8_t auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    usleep(100);
    auiSpiTxBuffer[0] = ((Address >> 16L) & MEM_MASK);
    auiSpiTxBuffer[1] = ( Address >>  8L);
    auiSpiTxBuffer[2] = ( Address >>  0L);
    auiSpiTxBuffer[3] = 0;


    {
        cout << "EVE_HAL::Read8 auiSpiTxBuffer" << endl;
        unsigned char *pucSourceTemp;
        pucSourceTemp = (unsigned char*)auiSpiTxBuffer;
        for(int i=0; i<16; )
        {
            for(int j=0; j<8; j++)
            {
                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
            }
            cout << endl;
            i += 8;
        }
    }

    xSpiCommunicationDevice.Exchange(0,
                                     auiSpiTxBuffer,
                                     auiSpiRxBuffer,
                                     8,
                                     LOW_SPEED_IN_HZ);
    std::cout << "EVE_HAL::Read8 2"  << std::endl;

    {
        cout << "EVE_HAL::Read8 auiSpiRxBuffer" << endl;
        unsigned char *pucSourceTemp;
        pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
        for(int i=0; i<16; )
        {
            for(int j=0; j<8; j++)
            {
                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
            }
            cout << endl;
            i += 8;
        }
    }

    unsigned char Readed = auiSpiRxBuffer[3];

    Disable();

    return Readed;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Чтение 2-байтового значения по заданному адресу
unsigned short EVE_HAL::Read16(unsigned long Address)
{
    Enable();

    USPI::Write((Address >> 16L) & MEM_MASK);       // <00aa_aaaa> - команда чтения
    USPI::Write(HIGH(Address));
    USPI::Write( LOW(Address));

    USPI::Write(0x00);				// Пустой синхро-байт

    union
    {
        unsigned short Readed;
        unsigned char  Byte[2];
    };

    Byte[0] = USPI::Read();
    Byte[1] = USPI::Read();

    Disable();

    return Readed;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Чтение 4-байтового значения по заданному адресу
unsigned long EVE_HAL::Read32(unsigned long Address)
{
    Enable();

    USPI::Write((Address >> 16L) & MEM_MASK);	// <00aa_aaaa> - команда чтения
    USPI::Write(HIGH(Address));
    USPI::Write( LOW(Address));

    USPI::Write(0x00);				// Пустой синхро-байт

    union
    {
        unsigned long Readed;
        unsigned char Byte[4];
    };

    Byte[0] = USPI::Read();
    Byte[1] = USPI::Read();
    Byte[2] = USPI::Read();
    Byte[3] = USPI::Read();

    Disable();

    return Readed;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Запись байта данных по заданному адресу
void EVE_HAL::Write8(unsigned long Address,unsigned char Value)
{
    Enable();

    USPI::Write(((Address >> 16L) & MEM_MASK) | MEM_WRITE);	// <10aa_aaaa> - команда записи
    USPI::Write(HIGH(Address));
    USPI::Write( LOW(Address));

    USPI::Write(Value);

    Disable();
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Запись 2-байтового значения по заданному адресу
void EVE_HAL::Write16(unsigned long Address,unsigned short Value)
{
    Enable();

    USPI::Write(((Address >> 16L) & MEM_MASK) | MEM_WRITE);	// <10aa_aaaa> - команда записи
    USPI::Write(HIGH(Address));
    USPI::Write( LOW(Address));

    USPI::Write( LOW(Value));
    USPI::Write(HIGH(Value));

    Disable();
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Запись 4-байтового значения по заданному адресу
void EVE_HAL::Write32(unsigned long Address,unsigned long Value)
{
    Enable();

    USPI::Write(((Address >> 16L) & MEM_MASK) | MEM_WRITE);	// <10aa_aaaa> - команда записи
    USPI::Write(HIGH(Address));
    USPI::Write( LOW(Address));

    USPI::Write( LOW(Value));
    USPI::Write(HIGH(Value));
    USPI::Write(Value >> 16L);
    USPI::Write(Value >> 24L);

    Disable();
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Чтение блока данных
void EVE_HAL::Read(unsigned long Address,void *Data,unsigned char Lenght)
{
    Enable();

    USPI::Write((Address >> 16L) & MEM_MASK);	// <00aa_aaaa> - команда чтения
    USPI::Write(HIGH(Address));
    USPI::Write( LOW(Address));

    USPI::Write(0x00);				// Пустой синхро-байт

    unsigned char *Ptr = ((unsigned char *)Data);

    for(unsigned char i = 0; i < Lenght; i++)      // NOT OPTIMAL!!!
    {
        *Ptr++ = USPI::Read();
    };

    Disable();
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Чтение блока данных
void EVE_HAL::Read(void *Data, unsigned long Address, unsigned char Lenght)
{
    Enable();

    USPI::Write((Address >> 16L) & MEM_MASK);	// <00aa_aaaa> - команда чтения
    USPI::Write(HIGH(Address));
    USPI::Write( LOW(Address));

    USPI::Write(0x00);				// Пустой синхро-байт

    unsigned char *Ptr = ((unsigned char *)Data);

    for(unsigned char i = 0; i < Lenght; i++)      // NOT OPTIMAL!!!
    {
        *Ptr++ = USPI::Read();
    };

    Disable();
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Запись блока данных
void EVE_HAL::Write(unsigned long Address,void *Data,unsigned short Lenght)
{
    Enable();

    USPI::Write(((Address >> 16L) & MEM_MASK) | MEM_WRITE);	// <10aa_aaaa> - команда записи
    USPI::Write(HIGH(Address));
    USPI::Write( LOW(Address));

    unsigned char *Ptr = ((unsigned char *)Data);

    for(unsigned short i = 0; i < Lenght; i++)     // NOT OPTIMAL!!!
    {
        USPI::Write(*Ptr++);
    };

    Disable();
};
////------------------------------------------------------------------------------
////------------------------------------------------------------------------------
//// Запись блока данных из Flash
// void EVE_HAL::Write(unsigned long Address,void __farflash *Data,unsigned short Lenght)
//{
//    Enable();
//
//    USPI::Write(((Address >> 16L) & MEM_MASK) | MEM_WRITE);	// <10aa_aaaa> - команда записи
//    USPI::Write(HIGH(Address));
//    USPI::Write( LOW(Address));
//
//    unsigned char __farflash *Ptr = ((unsigned char __farflash *)Data);
//
//    for(unsigned short i = 0; i < Lenght; i++)
//    {
//        USPI::Write(*Ptr++);
//    };
//
//    Disable();
//};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Инициализация видеопроцессора (требует доработки)
void EVE_HAL::Initialize(void)
{
//********************************************
//********************************************

    Activation();				// Будим видеопроцессор
    //__delay_cycles(25000L * XTAL);		// Задержка 25 ms

    Command(RST_PULSE);			// Сброс ядра
    Command(CLK_EXT);		        // Переключаем на внешний осциллятор
    while(Read8(REG_ID) != 0x7C);;;

//--------------------------------------------
//********************************************
    Write8(REG_PCLK,0);		// Отключаем тактировку
    Write8(REG_PWM_DUTY,0);	// Подсветка на 0

//********************************************
    /*
     Write16(REG_HCYCLE,548);	// 525 - number if horizontal cycles for display
     Write16(REG_HOFFSET,43);	// horizontal offset from starting signal

     Write16(REG_HSIZE,480);	// width resolution
     Write16(REG_VSIZE,272);	// height resolution

     Write16(REG_HSYNC0,  0);	// hsync falls
     Write16(REG_HSYNC1, 41);	// hsync rise
     Write16(REG_VCYCLE,292);	// number of vertical cycles for display
     Write16(REG_VOFFSET,12);	// vertical offset from start signal

     Write16(REG_VSYNC0, 0);	// vsync falls
     Write16(REG_VSYNC1,10);	// vsync rise
    */

    static unsigned long  DSettings[10] =
    {
        548,43,480,0,41,292,12,272,0,10
    };

    Write(REG_HCYCLE,DSettings,sizeof(DSettings));


//********************************************
//********************************************

    Write8(REG_PCLK_POL,1);	// clock polarity: 0 - rising edge, 1 - falling edge
    Write8(REG_CSPREAD, 1);	// output clock spread enable
    Write8(REG_DITHER,0);        // [##]

    Write32(REG_OUTBITS,0);

    Write8(REG_VOL_SOUND,0);
//********************************************
//--------------------------------------------

    static unsigned long  DList[3] =       // Пустой начальный Display-лист
    {
        CLEAR_COLOR_RGB(14,65,92), //CLEAR_COLOR_RGB(0,0,0),
        CLEAR(1,1,1),
        DISPLAY()
    };

    Write(RAM_DL,DList,sizeof(DList));

    Write8(REG_DLSWAP,DLSWAP_FRAME);               // Запуск DL на исполнение

    /*
     Enable();
        Write32(RAM_DL + 0,CLEAR_COLOR_RGB(0,25,0));        // Пустой DL
        Write32(RAM_DL + 4,CLEAR(1,1,1));
        Write32(RAM_DL + 8,DISPLAY());
     Disable();

     Write8(REG_DLSWAP,DLSWAP_FRAME);                       // Запуск DL на исполнение
    */
//--------------------------------------------
//--------------------------------------------

    Write8(REG_GPIO_DIR,(Read8(REG_GPIO_DIR) | 0x82));	// DE, GPIO1 - на выход,
    Write8(REG_GPIO,(Read8(REG_GPIO) | 0x82));		// устанавливаем в 1

//Write8(REG_TOUCH_MODE,TOUCHMODE_OFF);		// Без тачскрина

    Write8(REG_ROTATE,2);		// Rotate in portrait (view CMD_SETROTATE!!!)

    Write8(REG_TOUCH_MODE,TOUCHMODE_FRAME);	        // Опрос сенсора с частотой кадров
//Write16(REG_TOUCH_RZTHRESH,1200);                    // Уровень чувствительности
// Write16(REG_TOUCH_CONFIG,0x9381);                      // RTP, защита от КЗ, sample clock

    Write16(REG_TOUCH_CONFIG,0x0381);                      // RTP, защита от КЗ, sample clock


//Write8(REG_INT_MASK,0x04);                             // TAG_CHANGE interrupt       - пока не используется
//Write8(REG_INT_EN,1);                                  // Enable interrupt


    Write8(REG_PCLK,4);		// 60/5 = 12 MHz - after this display is visible on the LCD

    Write16(REG_PWM_HZ,1000);	// PWM Bright (for Newhawen Display)
    Write8(REG_PWM_DUTY,40);

//--------------------------------------------
//EICRB |= BIT(ISC41);           // Внешнее прерывание INT4 по спаду
//EIMSK |= BIT(INT4);
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Инициализация видеопроцессора
void EVE_HAL::InitializeNEW(bool HardReset)
{
//********************************************
//    if(HardReset)                          // Аппаратный сброс по линии PD
//    {
        SetLine_PD(false);
        //__delay_cycles(6000L * XTAL);         // Задержка 5 ms
        usleep(5000);
        SetLine_PD(true);
        //__delay_cycles(25000L * XTAL);        // Задержка 25 ms
        usleep(25000);
//    };

//********************************************

    Activation();				// Будим видеопроцессор
    //__delay_cycles(25000L * XTAL);		// Задержка 25 ms
    usleep(25000);


//if(HardReset == false)
    {
        Command(RST_PULSE);			// Сброс ядра
    };

//    while(Read8(REG_ID) != 0x7C);;;        // Ожидание готовности
    usleep(25000);
    Read8(REG_ID);
    usleep(25000);
    Read8(REG_ID);
    usleep(25000);
    Read8(REG_ID);

//    Command(CLK_EXT);		        // Переключаем на внешний осциллятор

//    while(Read8(REG_ID) != 0x7C);;;        // Ожидание готовности
//
////********************************************
//    Write8(REG_PCLK,0);		// Отключаем тактировку
//    Write8(REG_PWM_DUTY,0);	// Подсветка на 0
//
////********************************************
//
//    Write16(REG_HCYCLE,548);	// 525 - number if horizontal cycles for display        525 / 548 / 500 <- ErrImage
//    Write16(REG_HOFFSET,43);	// horizontal offset from starting signal
//
//    Write16(REG_HSIZE,480);	// width resolution     (Fixed!!!)
//    Write16(REG_VSIZE,272);	// height resolution    (Fixed!!!)
//
//    Write16(REG_HSYNC0,  0);	// hsync falls
//    Write16(REG_HSYNC1, 41);	// hsync rise 41
//    Write16(REG_VCYCLE,292);	// number of vertical cycles for display
//    Write16(REG_VOFFSET,12);	// vertical offset from start signal
//
//    Write16(REG_VSYNC0, 0);	// vsync falls
//    Write16(REG_VSYNC1,10);	// vsync rise
//    /*
//    ft_int16_t FT_DispWidth = 480;
//    ft_int16_t FT_DispHeight = 272;
//    ft_int16_t FT_DispHCycle =  548;
//    ft_int16_t FT_DispHOffset = 43;
//    ft_int16_t FT_DispHSync0 = 0;
//    ft_int16_t FT_DispHSync1 = 41;
//    ft_int16_t FT_DispVCycle = 292;
//    ft_int16_t FT_DispVOffset = 12;
//    ft_int16_t FT_DispVSync0 = 0;
//    ft_int16_t FT_DispVSync1 = 10;
//    */
//
//    /*
//     static unsigned long  DSettings[10] =
//     {
//      548,43,480,0,41,292,12,272,0,10
//     };
//
//     Write(REG_HCYCLE,DSettings,sizeof(DSettings));
//    */
//
////********************************************
//
//    Write8(REG_PCLK_POL,1);	// clock polarity: 0 - rising edge, 1 - falling edge
//    Write8(REG_CSPREAD, 1);	// output clock spread enable
//    Write8(REG_DITHER,1);          // [##]
//
////Write32(REG_OUTBITS,0);
//
////********************************************
//
//    static unsigned long  DList[3] =       // Пустой начальный Display-лист
//    {
//        CLEAR_COLOR_RGB(14,65,92),
//        CLEAR(1,1,1),
//        DISPLAY()
//    };
//
//    Write(RAM_DL,DList,sizeof(DList));
//
//    Write8(REG_DLSWAP,DLSWAP_FRAME);               // Запуск DL на исполнение
//
////********************************************
//
//    Write8(REG_ROTATE,2);                                  // Поворот в портретный режим
//
//    Write8(REG_GPIO_DIR,(Read8(REG_GPIO_DIR) | 0x82));	// DE, GPIO1 - на выход,
//    Write8(REG_GPIO,(Read8(REG_GPIO) | 0x82));		// устанавливаем в 1
//
////********************************************
//
//    unsigned char ChipID = (Read32(REG_CHIPID) >> 8) & 0xFF;       // 0x10,0x11,0x12,0x13 - FT810,FT811,FT812,FT813
//
//    Write8(REG_TOUCH_MODE,TOUCHMODE_FRAME);	        // Опрос сенсора с частотой кадров
//
//    if(ChipID & 0x01)
//    {
//        Write16(REG_TOUCH_CONFIG,0x0381);                      // CTP сенсор
//    }
//    else
//    {
////Write16(REG_TOUCH_RZTHRESH,1200);                      // Уровень чувствительности
//        Write16(REG_TOUCH_CONFIG,0x9381);                      // RTP сенсор, защита от КЗ, sample clock
//    };
//
////********************************************
//    Write8(REG_INT_MASK,1 << 3);                           // Sound effect ended
//    Write8(REG_INT_EN,1);                                  // Enable interrupt
////********************************************
//
//    if(ChipID == 0x10)                                     // FT810 OR FT812/813 in Newhawen Display
//    {
//        Write16(REG_PWM_HZ,5000);	                        // PWM Bright
//        Write8(REG_PWM_DUTY,50);
//        Write8(REG_PCLK,5);	                          	// 60/5 = 12 MHz - after this display is visible on the LCD
//    }
//    else
//    {
//        Write16(REG_PWM_HZ,1000);	                        // PWM Bright (for Newhawen Display)
//        Write8(REG_PWM_DUTY,110);
//        Write8(REG_PCLK,7);	                           	// 60/5 = 12 MHz - after this display is visible on the LCD
//    };
//
//
//    Write8(REG_VOL_SOUND,0);       // Громкость на 0
//    Write16(REG_SOUND,0x60);       // Mute
//    Write8(REG_PLAY,1);
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_HAL::InitializeFAST(bool HardReset)
{

    if(HardReset)                          // Аппаратный сброс по линии PD
    {
        SetLine_PD(false);
        //__delay_cycles(6000L * XTAL);         // Задержка 6 ms
        SetLine_PD(true);
        //__delay_cycles(25000L * XTAL);        // Задержка 25 ms
    };

//********************************************
    Activation();				// Будим видеопроцессор
    //__delay_cycles(25000L * XTAL);		// Задержка 25 ms


    Command(RST_PULSE);			// Сброс ядра
    while(Read8(REG_ID) != 0x7C);;;     // Ожидание готовности

    Command(CLK_EXT);		        // Переключаем на внешний осциллятор
    while(Read8(REG_ID) != 0x7C);;;     // Ожидание готовности

//********************************************
//Write8(REG_PCLK,0);		// Отключаем тактировку
//Write8(REG_PWM_DUTY,0);	// Подсветка на 0

//********************************************

    Write16(REG_HCYCLE,548);	// 525 - number if horizontal cycles for display        525 / 548 / 500 <- ErrImage
    Write16(REG_HOFFSET,43);	// horizontal offset from starting signal

    Write16(REG_HSIZE,480);	// width resolution     (Fixed!!!)
    Write16(REG_VSIZE,272);	// height resolution    (Fixed!!!)

    Write16(REG_HSYNC0,  0);	// hsync falls
    Write16(REG_HSYNC1, 41);	// hsync rise 41
    Write16(REG_VCYCLE,292);	// number of vertical cycles for display
    Write16(REG_VOFFSET,12);	// vertical offset from start signal

    Write16(REG_VSYNC0, 0);	// vsync falls
    Write16(REG_VSYNC1,10);	// vsync rise

//********************************************
    Write8(REG_PCLK_POL,1);	// clock polarity: 0 - rising edge, 1 - falling edge
    Write8(REG_CSPREAD, 1);	// output clock spread enable
    Write8(REG_DITHER,1);          // [##]

//********************************************

    static unsigned long  DList[3] =       // Пустой начальный Display-лист
    {
        CLEAR_COLOR_RGB(14,65,92),    //CLEAR_COLOR_RGB(0,0,0),
        CLEAR(1,1,1),
        DISPLAY()
    };

    Write(RAM_DL,DList,sizeof(DList));

    Write8(REG_DLSWAP,DLSWAP_FRAME);               // Запуск DL на исполнение

//********************************************

    Write8(REG_ROTATE,2);                                  // Поворот в портретный режим

    Write8(REG_GPIO_DIR,(Read8(REG_GPIO_DIR) | 0x82));	// DE, GPIO1 - на выход,
    Write8(REG_GPIO,(Read8(REG_GPIO) | 0x82));		// устанавливаем в 1

//********************************************

    unsigned char ChipID = (Read32(REG_CHIPID) >> 8) & 0xFF;       // 0x10,0x11,0x12,0x13 - FT810,FT811,FT812,FT813

    Write8(REG_TOUCH_MODE,TOUCHMODE_FRAME);	        // Опрос сенсора с частотой кадров

    if(ChipID & 0x01)
    {
        Write16(REG_TOUCH_CONFIG,0x0381);                      // CTP сенсор
    }
    else
    {
//Write16(REG_TOUCH_RZTHRESH,1200);                      // Уровень чувствительности
        Write16(REG_TOUCH_CONFIG,0x9381);                      // RTP сенсор, защита от КЗ, sample clock
    };

//********************************************
    Write8(REG_INT_MASK,1 << 3);                           // Sound effect ended
    Write8(REG_INT_EN,1);                                  // Enable interrupt
//********************************************

    if(ChipID == 0x10)                                     // FT810 OR FT812/813 in Newhawen Display
    {
        Write16(REG_PWM_HZ,5000);	                        // PWM Bright
        Write8(REG_PWM_DUTY,50);
        Write8(REG_PCLK,5);	                          	// 60/5 = 12 MHz - after this display is visible on the LCD
    }
    else
    {
        Write16(REG_PWM_HZ,1000);	                        // PWM Bright (for Newhawen Display)
        Write8(REG_PWM_DUTY,110);
        Write8(REG_PCLK,7);	                           	// 60/5 = 12 MHz - after this display is visible on the LCD
    };


    Write8(REG_VOL_SOUND,0);       // Громкость на 0
    Write16(REG_SOUND,0x60);       // Mute
    Write8(REG_PLAY,1);
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static const  unsigned long ControlPointAddr[10] =
{
    1000, 25000, 27000, 32000, 45000, 57000, 65000, 70000, 132000, 400000
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Запись контрольных точек в RAM_G видеопроцессора
void EVE_HAL::WriteControlPoints(void)
{
    for(unsigned char i = 0; i < 10; i++)
    {
        unsigned long Addr = ControlPointAddr[i] - sizeof(unsigned long);

        EVE_HAL::Write32(Addr, 0xABCD517E);
    };
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Проверка контрольных точек в RAM_G видеопроцессора
unsigned short EVE_HAL::CheckControlPoints(void)
{
    unsigned short ErrorFlags = 0x0000;
    unsigned short Mask       = 0x0001;

    for(unsigned char i = 0; i < 10; i++)
    {
        unsigned long Address = ControlPointAddr[i] - sizeof(unsigned long);
        unsigned long Value   = EVE_HAL::Read32(Address);

        if(Value != 0xABCD517E)
        {
            //__delay_cycles(XTAL * 25L);

            Value = EVE_HAL::Read32(Address);

            if(Value != 0xABCD517E)
            {
                ErrorFlags |= Mask;
            };
        };

        Mask <<= 1;
    };

    return ErrorFlags;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Проверка контрольных точек в RAM_G видеопроцессора (SEP 2023)
unsigned char EVE_HAL::CheckControlPoints2(void)
{
    unsigned char Counter = 0;

    for(unsigned char i = 0; i < 10; i++)
    {
        unsigned long Address = ControlPointAddr[i] - sizeof(unsigned long);

        if(EVE_HAL::Read32(Address) != 0xABCD517E)
        {
            //__delay_cycles(XTAL * 25L);

            if(EVE_HAL::Read32(Address) != 0xABCD517E)
            {
                Counter++;
            };
        };
    };

    return Counter;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Установка уровня яркости подсветки экрана
void EVE_HAL::SetLighting(unsigned char Value)
{
    if(Value <  10) Value =  10;
    if(Value > 128) Value = 128;

    EVE_HAL::Write8(REG_PWM_DUTY, Value);
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
