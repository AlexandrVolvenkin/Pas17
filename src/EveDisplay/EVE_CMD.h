//------------------------------------------------------------------------------
// Command coprocessor layer for FTDI EVE2 (only FT81x chips)
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifndef __EVE_CMD_H
#define __EVE_CMD_H
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "EVE_HAL.h"
#include "EVE_Resources.h"

////#include "Unsigned_int24_t.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//****************************** МАКРОСЫ ***************************************
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class EVE_CMD: public EVE_HAL
{
protected:
public:

    static unsigned short P;                       // Счетчик команд
    static unsigned short CMD_Read;                // Состояние регистра REG_CMD_READ

    static unsigned short CMD_Write;               // Состояние регистра REG_CMD_WRITE  [##]

    static void WriteCMD(unsigned char);           // Ускоренные версии потоковой записи
    static void Write32F(unsigned long);
    static void Write08F(unsigned char);

    static void Alignment(unsigned char);          // Выравнивание блока данных на границу 4 байт

public:

    static void WriteTransaction(unsigned long);
    static void EndTransaction(void);
    static void Write32(unsigned long);

    static void BeginDL(void);
    static void EndDL(void);
    static void Execute(unsigned short = 0);


//    static  void WriteStream(unsigned long  *,unsigned char);            // [##]

    static void SetRotate(unsigned char);
    static void SetBase(unsigned char);
    static void RomFont(unsigned char,unsigned char);
    static void SetFont(unsigned long,unsigned char,unsigned char);


    static void SetBGColor(unsigned char,unsigned char,unsigned char);
    static void SetBGColor(unsigned long);
    static void SetFGColor(unsigned char,unsigned char,unsigned char);

    static  void String(char *);
//    static  void String(char __farflash *);
    static  void StringCYR(char *);
//    static  void StringCYR(char __farflash *);

    static void Text(unsigned short,unsigned short,unsigned char,unsigned short = 0);
    static void Number(unsigned short,unsigned short,unsigned char,unsigned short,long);
    static void Keys(unsigned short,unsigned short,unsigned char,unsigned char,unsigned char,unsigned short);
    static void Button(unsigned short,unsigned short,unsigned char,unsigned char,unsigned char,unsigned short = 0);

    static void Clock(unsigned short,unsigned short,unsigned char,unsigned short = 0);

    static void Track(unsigned short,unsigned short,unsigned short,unsigned short,unsigned char);
    static void Append(unsigned long,unsigned short);


//****************************************************
//****************************************************
    static inline void DL(unsigned long);

    static inline void Tag(unsigned char);
    static inline void SetColor(unsigned char,unsigned char,unsigned char);
    static inline void SetColor(unsigned long);
    static inline void SetGrayColor(unsigned char);
    static inline void SetAlphaChannel(unsigned char);

//static inline void SetColor(uint24_t);

    static inline void Text(unsigned short,unsigned short,unsigned char,unsigned short,char *);
    static inline void Text(unsigned short,unsigned short,unsigned char,char *);
//    static inline void Text(unsigned short,unsigned short,unsigned char,unsigned short,char __farflash *);
//    static inline void Text(unsigned short,unsigned short,unsigned char,char __farflash *);

    static inline void Button(unsigned short,unsigned short,unsigned char,unsigned char,unsigned char,unsigned short,char *);
//    static inline void Button(unsigned short,unsigned short,unsigned char,unsigned char,unsigned char,unsigned short,char __farflash *);
//    static inline void Button(unsigned short,unsigned short,unsigned char,unsigned char,unsigned char,char __farflash *);

    static inline void Number(unsigned short X,unsigned short Y,unsigned char Font,long Value);

    static inline void Clock(unsigned short,unsigned short,unsigned char,unsigned short,unsigned char,unsigned char,unsigned char = 0xFF);

    static inline unsigned long GetChipID(void);

    static inline void LineWidth(unsigned short);

//****************************************************

    static inline void SaveContext(void);
    static inline void RestoreContext(void);

//****************************************************
//****************************************************
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*
class EVE2: public EVE_CMD
{
};
*/
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//************************** INLINE - ФУНКЦИИ **********************************
//------------------------------------------------------------------------------
void EVE_CMD::DL(unsigned long Value)
{
    Write32(Value);
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::Tag(unsigned char Value)
{
    Write32(TAG(Value));
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::SetColor(unsigned char Red,unsigned char Green,unsigned char Blue)
{
    Write32(COLOR_RGB(Red, Green, Blue));
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::SetColor(unsigned long Color)
{
    Write32(COLOR_RGB_NEW(Color));
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::SetGrayColor(unsigned char Level)
{
    Write32(COLOR_RGB(Level, Level, Level));
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::SetAlphaChannel(unsigned char Alpha)
{
    Write32(COLOR_A(Alpha));
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::Text(unsigned short X,unsigned short Y,unsigned char Font,unsigned short Options,char *Str)
{
    Text(X,Y,Font,Options & (~OPT_CYR));

    if(Options & OPT_CYR)
    {
        StringCYR(Str);
    }
    else
    {
        String(Str);
    };
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::Text(unsigned short X,unsigned short Y,unsigned char Font,char *Str)
{
    Text(X,Y,Font);
    String(Str);
};
////------------------------------------------------------------------------------
////------------------------------------------------------------------------------
//void EVE_CMD::Text(unsigned short X,unsigned short Y,unsigned char Font,unsigned short Options,char __farflash *Str)
//{
//    Text(X,Y,Font,Options & (~OPT_CYR));
//
//    if(Options & OPT_CYR)
//    {
//        StringCYR(Str);
//    }
//    else
//    {
//        String(Str);
//    };
//};
////------------------------------------------------------------------------------
////------------------------------------------------------------------------------
//void EVE_CMD::Text(unsigned short X,unsigned short Y,unsigned char Font,char __farflash *Str)
//{
//    Text(X,Y,Font);
//    String(Str);
//};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::Button(unsigned short X,unsigned short Y,unsigned char W,unsigned char H,unsigned char Font,unsigned short Options,char *Str)
{
//Button(X,Y,W,H,Font,Options);
//String(Str);

    Button(X,Y,W,H,Font,Options & (~OPT_CYR));

    if(Options & OPT_CYR)
    {
        StringCYR(Str);
    }
    else
    {
        String(Str);
    };
};
////------------------------------------------------------------------------------
////------------------------------------------------------------------------------
//void EVE_CMD::Button(unsigned short X,unsigned short Y,unsigned char W,unsigned char H,unsigned char Font,unsigned short Options,char __farflash *Str)
//{
////Button(X,Y,W,H,Font,Options);
////String(Str);
//
//    Button(X,Y,W,H,Font,Options & (~OPT_CYR));
//
//    if(Options & OPT_CYR)
//    {
//        StringCYR(Str);
//    }
//    else
//    {
//        String(Str);
//    };
//};
////------------------------------------------------------------------------------
////------------------------------------------------------------------------------
//void EVE_CMD::Button(unsigned short X,unsigned short Y,unsigned char W,unsigned char H,unsigned char Font,char __farflash *Str)
//{
//    Button(X,Y,W,H,Font);
//    String(Str);
//};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::Clock(unsigned short X,unsigned short Y,unsigned char Radius,unsigned short Options,
                    unsigned char Hour,unsigned char Minute,unsigned char Second)
{
    if(Second == 0xFF) Options |= OPT_NOSECS;

    Clock(X,Y,Radius,Options);
    Write32(PACK(Minute,Hour));
    Write32(PACK(0,Second));
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::Number(unsigned short X,unsigned short Y,unsigned char Font,long Value)
{
    Number(X,Y,Font,0,Value);
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::LineWidth(unsigned short Width)
{
    EVE_CMD::DL(LINE_WIDTH(Width));
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
unsigned long EVE_CMD::GetChipID(void)
{
    return EVE_HAL::Read32(REG_CHIPID);
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::SaveContext(void)
{
    EVE_CMD::DL(SAVE_CONTEXT());
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::RestoreContext(void)
{
    EVE_CMD::DL(RESTORE_CONTEXT());
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#endif //__EVE_CMD_H
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
