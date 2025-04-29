#ifndef CANALOGUESIGNALS_H
#define CANALOGUESIGNALS_H
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include "Configuration.h"

class Timer;
class Platform;
class CTask;
class CResources;
class CDataStore;
class CLink;
class CLinkInterface;
class CDeviceControl;
class CDataContainerInterface;
class CDataContainerDataBase;
class CInternalModule;
class CInternalModuleMuvr;


//-------------------------------------------------------------------------------
// маска - биты 0 Ц 3 тип шкалы датчика “Ў .
#define ANALOGUE_INPUT_SCALE_TYPE_MASK 0x0F
// маска - биты 4 Ц 7 Ц код размерности измер€емого параметра  –«ћ.
#define ANALOGUE_INPUT_DIMENTION_CODE_MASK 0xF0
// маска - биты 0 Ц 5 Ц код диапазона измер€емой температуры дл€ ћ¬—“5, или код входного сигнала дл€ ћ¬ѕ—5 и ћ¬јќ5 (дл€ ћ¬ј»5 значени€ не имеет)  ƒ»“.
#define ANALOGUE_INPUT_RANGE_CODE_MASK 0x3F
// структура описател€ одного аналогового входа в базе данных.
struct TAnalogueInputDescriptionDataBase
{
    // - биты 0 Ц 3 тип шкалы датчика “Ў .
    // - биты 4 Ц 7 Ц код размерности измер€емого параметра  –«ћ.
    unsigned char ucServiseL;
    // - биты 0 Ц 5 Ц код диапазона измер€емой температуры дл€ ћ¬—“5, или код входного сигнала дл€ ћ¬ѕ—5 и ћ¬јќ5 (дл€ ћ¬ј»5 значени€ не имеет)  ƒ»“.
    // - биты 6,7 Ц резерв.
    unsigned char ucServiseH;
    // ЌЎ  - нижний предел шкалы датчика, ‘ормат Ц FLOAT, 3 байта.
    unsigned char ucMinOfScaleMantissLsb;
    unsigned char ucMinOfScaleMantissMsb;
    unsigned char ucMinOfScaleExponent;
    // ¬Ў  - верхний предел шкалы датчика, ‘ормат Ц FLOAT, 3 байта.
    unsigned char ucMaxOfScaleMantissLsb;
    unsigned char ucMaxOfScaleMantissMsb;
    unsigned char ucMaxOfScaleExponent;
    // уставка LL, ‘ормат Ц FLOAT, 3 байта.
    unsigned char ucSetPointLLMantissLsb;
    unsigned char ucSetPointLLMantissMsb;
    unsigned char ucSetPointLLExponent;
    // уставка L, ‘ормат Ц FLOAT, 3 байта.
    unsigned char ucSetPointLMantissLsb;
    unsigned char ucSetPointLMantissMsb;
    unsigned char ucSetPointLExponent;
    // уставка H, ‘ормат Ц FLOAT, 3 байта.
    unsigned char ucSetPointHMantissLsb;
    unsigned char ucSetPointHMantissMsb;
    unsigned char ucSetPointHExponent;
    // уставка HH, ‘ормат Ц FLOAT, 3 байта.
    unsigned char ucSetPointHHMantissLsb;
    unsigned char ucSetPointHHMantissMsb;
    unsigned char ucSetPointHHExponent;
    // “екстовый реквизит входа Ц 8 символов ASCII.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH];
};

// дл€ сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура описател€ одного аналогового входа в базе данных.
struct TAnalogueInputDescriptionDataBasePackOne
{
    // - биты 0 Ц 3 тип шкалы датчика “Ў .
    // - биты 4 Ц 7 Ц код размерности измер€емого параметра  –«ћ.
    unsigned char ucServiseL;
    // - биты 0 Ц 5 Ц код диапазона измер€емой температуры дл€ ћ¬—“5, или код входного сигнала дл€ ћ¬ѕ—5 и ћ¬јќ5 (дл€ ћ¬ј»5 значени€ не имеет)  ƒ»“.
    // - биты 6,7 Ц резерв.
    unsigned char ucServiseH;
    // ЌЎ  - нижний предел шкалы датчика, ‘ормат Ц FLOAT, 3 байта.
    unsigned char ucMinOfScaleMantissLsb;
    unsigned char ucMinOfScaleMantissMsb;
    unsigned char ucMinOfScaleExponent;
    // ¬Ў  - верхний предел шкалы датчика, ‘ормат Ц FLOAT, 3 байта.
    unsigned char ucMaxOfScaleMantissLsb;
    unsigned char ucMaxOfScaleMantissMsb;
    unsigned char ucMaxOfScaleExponent;
    // уставка LL, ‘ормат Ц FLOAT, 3 байта.
    unsigned char ucSetPointLLMantissLsb;
    unsigned char ucSetPointLLMantissMsb;
    unsigned char ucSetPointLLExponent;
    // уставка L, ‘ормат Ц FLOAT, 3 байта.
    unsigned char ucSetPointLMantissLsb;
    unsigned char ucSetPointLMantissMsb;
    unsigned char ucSetPointLExponent;
    // уставка H, ‘ормат Ц FLOAT, 3 байта.
    unsigned char ucSetPointHMantissLsb;
    unsigned char ucSetPointHMantissMsb;
    unsigned char ucSetPointHExponent;
    // уставка HH, ‘ормат Ц FLOAT, 3 байта.
    unsigned char ucSetPointHHMantissLsb;
    unsigned char ucSetPointHHMantissMsb;
    unsigned char ucSetPointHHExponent;
    // “екстовый реквизит входа Ц 8 символов ASCII.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH];
};
#pragma pack(pop)

// структура описател€ одного аналогового входа в буфере RAM, IEEE754.
// после преобразовани€ из общего формата базы данных.
struct TAnalogueInputDescriptionModbus
{
    // ЌЎ  - нижний предел шкалы датчика, ‘ормат Ц FLOAT, IEEE754.
    float fMinOfScale;
    // ¬Ў  - верхний предел шкалы датчика, ‘ормат Ц FLOAT, IEEE754.
    float fMaxOfScale;
    // уставка LL, ‘ормат Ц FLOAT, IEEE754.
    float fSetPointLL;
    // уставка L, ‘ормат Ц FLOAT, IEEE754.
    float fSetPointL;
    // уставка H, ‘ормат Ц FLOAT, IEEE754.
    float fSetPointH;
    // уставка HH, ‘ормат Ц FLOAT, IEEE754.
    float fSetPointHH;
};

// дл€ сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура описател€ одного аналогового входа в буфере RAM, IEEE754.
// после преобразовани€ из общего формата базы данных.
struct TAnalogueInputDescriptionModbusPackOne
{
    // ЌЎ  - нижний предел шкалы датчика, ‘ормат Ц FLOAT, IEEE754.
    float fMinOfScale;
    // ¬Ў  - верхний предел шкалы датчика, ‘ормат Ц FLOAT, IEEE754.
    float fMaxOfScale;
    // уставка LL, ‘ормат Ц FLOAT, IEEE754.
    float fSetPointLL;
    // уставка L, ‘ормат Ц FLOAT, IEEE754.
    float fSetPointL;
    // уставка H, ‘ормат Ц FLOAT, IEEE754.
    float fSetPointH;
    // уставка HH, ‘ормат Ц FLOAT, IEEE754.
    float fSetPointHH;
};
#pragma pack(pop)

// структура описател€ одного аналогового входа в буфере RAM, IEEE754.
// после преобразовани€ из общего формата базы данных.
struct TAnalogueInputDescriptionWork
{
    // тип шкалы датчика “Ў .
    unsigned char ucScaleType;
    // код размерности измер€емого параметра  –«ћ.
    unsigned char ucDimentionCode;
    // код диапазона измер€емой температуры дл€ ћ¬—“5, или код входного сигнала дл€ ћ¬ѕ—5 и ћ¬јќ5 (дл€ ћ¬ј»5 значени€ не имеет)  ƒ»“.
    unsigned char ucRangeCode;
    // ЌЎ  - нижний предел шкалы датчика, ‘ормат Ц FLOAT, IEEE754.
    float fMinOfScale;
    // ¬Ў  - верхний предел шкалы датчика, ‘ормат Ц FLOAT, IEEE754.
    float fMaxOfScale;
    // уставка LL, ‘ормат Ц FLOAT, IEEE754.
    float fSetPointLL;
    // уставка L, ‘ормат Ц FLOAT, IEEE754.
    float fSetPointL;
    // уставка H, ‘ормат Ц FLOAT, IEEE754.
    float fSetPointH;
    // уставка HH, ‘ормат Ц FLOAT, IEEE754.
    float fSetPointHH;
    // “екстовый реквизит входа Ц 8 символов ASCII, плюс нуль - признак конца строки.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
};

// дл€ сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура описател€ одного аналогового входа в буфере RAM, IEEE754.
// после преобразовани€ из общего формата базы данных.
struct TAnalogueInputDescriptionWorkPackOne
{
    // тип шкалы датчика “Ў .
    unsigned char ucScaleType;
    // код размерности измер€емого параметра  –«ћ.
    unsigned char ucDimentionCode;
    // код диапазона измер€емой температуры дл€ ћ¬—“5, или код входного сигнала дл€ ћ¬ѕ—5 и ћ¬јќ5 (дл€ ћ¬ј»5 значени€ не имеет)  ƒ»“.
    unsigned char ucRangeCode;
    // ЌЎ  - нижний предел шкалы датчика, ‘ормат Ц FLOAT, IEEE754.
    float fMinOfScale;
    // ¬Ў  - верхний предел шкалы датчика, ‘ормат Ц FLOAT, IEEE754.
    float fMaxOfScale;
    // уставка LL, ‘ормат Ц FLOAT, IEEE754.
    float fSetPointLL;
    // уставка L, ‘ормат Ц FLOAT, IEEE754.
    float fSetPointL;
    // уставка H, ‘ормат Ц FLOAT, IEEE754.
    float fSetPointH;
    // уставка HH, ‘ормат Ц FLOAT, IEEE754.
    float fSetPointHH;
    // “екстовый реквизит входа Ц 8 символов ASCII, плюс нуль - признак конца строки.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
};
#pragma pack(pop)

// структура дл€ хранени€ и передачи температуры холодного спа€ “’—.
struct TTxsData
{
    float fTxsValue; // значение “’—.
    unsigned char ucTxsStat; // состо€ние канала “’—.
};

// дл€ сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура дл€ хранени€ и передачи температуры холодного спа€ “’—.
struct TTxsDataPackOne
{
    float fTxsValue; // значение “’—.
    unsigned char ucTxsStat; // состо€ние канала “’—.
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
        DATA_BASE_BLOCK_WRITE_START,
        DATA_BASE_BLOCK_WRITE_END_WAITING,
        DATA_BASE_BLOCK_STORAGE_START_WRITE,
        DATA_BASE_BLOCK_STORAGE_WRITE_END_WAITING,
        DATA_BASE_BLOCK_MODULE_START_WRITE,
        DATA_BASE_BLOCK_MODULE_WRITE_END_WAITING,

        DATA_BASE_BLOCK_CHECK_START,
        DATA_BASE_BLOCK_CHECK_MODULE_BLOCK_READ,
        DATA_BASE_BLOCK_CHECK_MODULE_BLOCK_READ_END_WAITING,
        DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_READ,
        DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_READ_END_WAITING,
        DATA_BASE_BLOCK_CHECK_MODULE_AND_STORAGE_BLOCK_COMPARE,
        DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_START_WRITE,
        DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_WRITE_END_WAITING,
    };

    CAnalogueSignals();
    virtual ~CAnalogueSignals();

    void SetDeviceControlName(const std::string& sName)
    {
        m_sDeviceControlName = sName;
    }

    uint8_t Init(void);
    bool SetTaskData(CDataContainerDataBase* pxDataContainer);
    bool GetTaskData(CDataContainerDataBase* pxDataContainer);
    void Allocate(void);
    void Exstract(void);
    uint8_t Fsm(void);

protected:

private:
    std::string m_sDeviceControlName;
    uint8_t m_uiDeviceControlId;

    uint8_t* m_puiIntermediateBuff;
    CDataContainerDataBase* m_pxOperatingDataContainer;
};



//-------------------------------------------------------------------------------
#endif // CANALOGUESIGNALS_H
