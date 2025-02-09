#ifndef CANALOGUESIGNALS_H
#define CANALOGUESIGNALS_H
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

class Timer;
class Platform;
class CTask;
class CResources;
class CDataStore;
class CLink;
class CLinkInterface;
class CDataContainerInterface;
class CDataContainerDataBase;


//-------------------------------------------------------------------------------
// маска - биты 0 – 3 тип шкалы датчика ТШК.
#define ANALOGUE_INPUT_SCALE_TYPE_MASK 0x0F
// маска - биты 4 – 7 – код размерности измеряемого параметра КРЗМ.
#define ANALOGUE_INPUT_DIMENTION_CODE_MASK 0xF0
// маска - биты 0 – 5 – код диапазона измеряемой температуры для МВСТ5, или код входного сигнала для МВПС5 и МВАО5 (для МВАИ5 значения не имеет) КДИТ.
#define ANALOGUE_INPUT_RANGE_CODE_MASK 0x3F
// структура описателя одного аналогового входа в базе данных.
struct TAnalogueInputDescriptionDataBase
{
    // - биты 0 – 3 тип шкалы датчика ТШК.
    // - биты 4 – 7 – код размерности измеряемого параметра КРЗМ.
    unsigned char ucServiseL;
    // - биты 0 – 5 – код диапазона измеряемой температуры для МВСТ5, или код входного сигнала для МВПС5 и МВАО5 (для МВАИ5 значения не имеет) КДИТ.
    // - биты 6,7 – резерв.
    unsigned char ucServiseH;
    // НШК - нижний предел шкалы датчика, Формат – FLOAT, 3 байта.
    unsigned char ucMinOfScaleMantissLsb;
    unsigned char ucMinOfScaleMantissMsb;
    unsigned char ucMinOfScaleExponent;
    // ВШК - верхний предел шкалы датчика, Формат – FLOAT, 3 байта.
    unsigned char ucMaxOfScaleMantissLsb;
    unsigned char ucMaxOfScaleMantissMsb;
    unsigned char ucMaxOfScaleExponent;
    // уставка LL, Формат – FLOAT, 3 байта.
    unsigned char ucSetPointLLMantissLsb;
    unsigned char ucSetPointLLMantissMsb;
    unsigned char ucSetPointLLExponent;
    // уставка L, Формат – FLOAT, 3 байта.
    unsigned char ucSetPointLMantissLsb;
    unsigned char ucSetPointLMantissMsb;
    unsigned char ucSetPointLExponent;
    // уставка H, Формат – FLOAT, 3 байта.
    unsigned char ucSetPointHMantissLsb;
    unsigned char ucSetPointHMantissMsb;
    unsigned char ucSetPointHExponent;
    // уставка HH, Формат – FLOAT, 3 байта.
    unsigned char ucSetPointHHMantissLsb;
    unsigned char ucSetPointHHMantissMsb;
    unsigned char ucSetPointHHExponent;
    // Текстовый реквизит входа – 8 символов ASCII.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH];
};

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура описателя одного аналогового входа в базе данных.
struct TAnalogueInputDescriptionDataBasePackOne
{
    // - биты 0 – 3 тип шкалы датчика ТШК.
    // - биты 4 – 7 – код размерности измеряемого параметра КРЗМ.
    unsigned char ucServiseL;
    // - биты 0 – 5 – код диапазона измеряемой температуры для МВСТ5, или код входного сигнала для МВПС5 и МВАО5 (для МВАИ5 значения не имеет) КДИТ.
    // - биты 6,7 – резерв.
    unsigned char ucServiseH;
    // НШК - нижний предел шкалы датчика, Формат – FLOAT, 3 байта.
    unsigned char ucMinOfScaleMantissLsb;
    unsigned char ucMinOfScaleMantissMsb;
    unsigned char ucMinOfScaleExponent;
    // ВШК - верхний предел шкалы датчика, Формат – FLOAT, 3 байта.
    unsigned char ucMaxOfScaleMantissLsb;
    unsigned char ucMaxOfScaleMantissMsb;
    unsigned char ucMaxOfScaleExponent;
    // уставка LL, Формат – FLOAT, 3 байта.
    unsigned char ucSetPointLLMantissLsb;
    unsigned char ucSetPointLLMantissMsb;
    unsigned char ucSetPointLLExponent;
    // уставка L, Формат – FLOAT, 3 байта.
    unsigned char ucSetPointLMantissLsb;
    unsigned char ucSetPointLMantissMsb;
    unsigned char ucSetPointLExponent;
    // уставка H, Формат – FLOAT, 3 байта.
    unsigned char ucSetPointHMantissLsb;
    unsigned char ucSetPointHMantissMsb;
    unsigned char ucSetPointHExponent;
    // уставка HH, Формат – FLOAT, 3 байта.
    unsigned char ucSetPointHHMantissLsb;
    unsigned char ucSetPointHHMantissMsb;
    unsigned char ucSetPointHHExponent;
    // Текстовый реквизит входа – 8 символов ASCII.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH];
};
#pragma pack(pop)

// структура описателя одного аналогового входа в буфере RAM, IEEE754.
// после преобразования из общего формата базы данных.
struct TAnalogueInputDescriptionModbus
{
    // НШК - нижний предел шкалы датчика, Формат – FLOAT, IEEE754.
    float fMinOfScale;
    // ВШК - верхний предел шкалы датчика, Формат – FLOAT, IEEE754.
    float fMaxOfScale;
    // уставка LL, Формат – FLOAT, IEEE754.
    float fSetPointLL;
    // уставка L, Формат – FLOAT, IEEE754.
    float fSetPointL;
    // уставка H, Формат – FLOAT, IEEE754.
    float fSetPointH;
    // уставка HH, Формат – FLOAT, IEEE754.
    float fSetPointHH;
};

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура описателя одного аналогового входа в буфере RAM, IEEE754.
// после преобразования из общего формата базы данных.
struct TAnalogueInputDescriptionModbusPackOne
{
    // НШК - нижний предел шкалы датчика, Формат – FLOAT, IEEE754.
    float fMinOfScale;
    // ВШК - верхний предел шкалы датчика, Формат – FLOAT, IEEE754.
    float fMaxOfScale;
    // уставка LL, Формат – FLOAT, IEEE754.
    float fSetPointLL;
    // уставка L, Формат – FLOAT, IEEE754.
    float fSetPointL;
    // уставка H, Формат – FLOAT, IEEE754.
    float fSetPointH;
    // уставка HH, Формат – FLOAT, IEEE754.
    float fSetPointHH;
};
#pragma pack(pop)

// структура описателя одного аналогового входа в буфере RAM, IEEE754.
// после преобразования из общего формата базы данных.
struct TAnalogueInputDescriptionWork
{
    // тип шкалы датчика ТШК.
    unsigned char ucScaleType;
    // код размерности измеряемого параметра КРЗМ.
    unsigned char ucDimentionCode;
    // код диапазона измеряемой температуры для МВСТ5, или код входного сигнала для МВПС5 и МВАО5 (для МВАИ5 значения не имеет) КДИТ.
    unsigned char ucRangeCode;
    // НШК - нижний предел шкалы датчика, Формат – FLOAT, IEEE754.
    float fMinOfScale;
    // ВШК - верхний предел шкалы датчика, Формат – FLOAT, IEEE754.
    float fMaxOfScale;
    // уставка LL, Формат – FLOAT, IEEE754.
    float fSetPointLL;
    // уставка L, Формат – FLOAT, IEEE754.
    float fSetPointL;
    // уставка H, Формат – FLOAT, IEEE754.
    float fSetPointH;
    // уставка HH, Формат – FLOAT, IEEE754.
    float fSetPointHH;
    // Текстовый реквизит входа – 8 символов ASCII, плюс нуль - признак конца строки.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
};

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура описателя одного аналогового входа в буфере RAM, IEEE754.
// после преобразования из общего формата базы данных.
struct TAnalogueInputDescriptionWorkPackOne
{
    // тип шкалы датчика ТШК.
    unsigned char ucScaleType;
    // код размерности измеряемого параметра КРЗМ.
    unsigned char ucDimentionCode;
    // код диапазона измеряемой температуры для МВСТ5, или код входного сигнала для МВПС5 и МВАО5 (для МВАИ5 значения не имеет) КДИТ.
    unsigned char ucRangeCode;
    // НШК - нижний предел шкалы датчика, Формат – FLOAT, IEEE754.
    float fMinOfScale;
    // ВШК - верхний предел шкалы датчика, Формат – FLOAT, IEEE754.
    float fMaxOfScale;
    // уставка LL, Формат – FLOAT, IEEE754.
    float fSetPointLL;
    // уставка L, Формат – FLOAT, IEEE754.
    float fSetPointL;
    // уставка H, Формат – FLOAT, IEEE754.
    float fSetPointH;
    // уставка HH, Формат – FLOAT, IEEE754.
    float fSetPointHH;
    // Текстовый реквизит входа – 8 символов ASCII, плюс нуль - признак конца строки.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
};
#pragma pack(pop)

// структура для хранения и передачи температуры холодного спая ТХС.
struct TTxsData
{
    float fTxsValue; // значение ТХС.
    unsigned char ucTxsStat; // состояние канала ТХС.
};

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура для хранения и передачи температуры холодного спая ТХС.
struct TTxsDataPackOne
{
    float fTxsValue; // значение ТХС.
    unsigned char ucTxsStat; // состояние канала ТХС.
};

#pragma pack(pop)

//-------------------------------------------------------------------------------
class CAnalogueSignals : public CTask
{
public:
    enum
    {
        DATA_BASE_BLOCK_START_READ = NEXT_STEP,
        DATA_BASE_BLOCK_READ_END_WAITING,
        DATA_BASE_BLOCK_START_WRITE,
        DATA_BASE_BLOCK_WRITE_END_WAITING,
    };

    CAnalogueSignals();
    virtual ~CAnalogueSignals();

    uint8_t Init(void);
    bool SetTaskData(CDataContainerDataBase* pxDataContainer);
    bool GetTaskData(CDataContainerDataBase* pxDataContainer);
    void Allocate(void);
    void Exstract(void);
    uint8_t Fsm(void);

protected:

private:
    uint8_t* m_puiIntermediateBuff;
    CDataContainerDataBase* m_pxOperatingDataContainer;
};



//-------------------------------------------------------------------------------
#endif // CANALOGUESIGNALS_H
