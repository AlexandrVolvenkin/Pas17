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
// ����� - ���� 0 � 3 ��� ����� ������� ���.
#define ANALOGUE_INPUT_SCALE_TYPE_MASK 0x0F
// ����� - ���� 4 � 7 � ��� ����������� ����������� ��������� ����.
#define ANALOGUE_INPUT_DIMENTION_CODE_MASK 0xF0
// ����� - ���� 0 � 5 � ��� ��������� ���������� ����������� ��� ����5, ��� ��� �������� ������� ��� ����5 � ����5 (��� ����5 �������� �� �����) ����.
#define ANALOGUE_INPUT_RANGE_CODE_MASK 0x3F
// ��������� ��������� ������ ����������� ����� � ���� ������.
struct TAnalogueInputDescriptionDataBase
{
    // - ���� 0 � 3 ��� ����� ������� ���.
    // - ���� 4 � 7 � ��� ����������� ����������� ��������� ����.
    unsigned char ucServiseL;
    // - ���� 0 � 5 � ��� ��������� ���������� ����������� ��� ����5, ��� ��� �������� ������� ��� ����5 � ����5 (��� ����5 �������� �� �����) ����.
    // - ���� 6,7 � ������.
    unsigned char ucServiseH;
    // ��� - ������ ������ ����� �������, ������ � FLOAT, 3 �����.
    unsigned char ucMinOfScaleMantissLsb;
    unsigned char ucMinOfScaleMantissMsb;
    unsigned char ucMinOfScaleExponent;
    // ��� - ������� ������ ����� �������, ������ � FLOAT, 3 �����.
    unsigned char ucMaxOfScaleMantissLsb;
    unsigned char ucMaxOfScaleMantissMsb;
    unsigned char ucMaxOfScaleExponent;
    // ������� LL, ������ � FLOAT, 3 �����.
    unsigned char ucSetPointLLMantissLsb;
    unsigned char ucSetPointLLMantissMsb;
    unsigned char ucSetPointLLExponent;
    // ������� L, ������ � FLOAT, 3 �����.
    unsigned char ucSetPointLMantissLsb;
    unsigned char ucSetPointLMantissMsb;
    unsigned char ucSetPointLExponent;
    // ������� H, ������ � FLOAT, 3 �����.
    unsigned char ucSetPointHMantissLsb;
    unsigned char ucSetPointHMantissMsb;
    unsigned char ucSetPointHExponent;
    // ������� HH, ������ � FLOAT, 3 �����.
    unsigned char ucSetPointHHMantissLsb;
    unsigned char ucSetPointHHMantissMsb;
    unsigned char ucSetPointHHExponent;
    // ��������� �������� ����� � 8 �������� ASCII.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH];
};

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ��������� ������ ����������� ����� � ���� ������.
struct TAnalogueInputDescriptionDataBasePackOne
{
    // - ���� 0 � 3 ��� ����� ������� ���.
    // - ���� 4 � 7 � ��� ����������� ����������� ��������� ����.
    unsigned char ucServiseL;
    // - ���� 0 � 5 � ��� ��������� ���������� ����������� ��� ����5, ��� ��� �������� ������� ��� ����5 � ����5 (��� ����5 �������� �� �����) ����.
    // - ���� 6,7 � ������.
    unsigned char ucServiseH;
    // ��� - ������ ������ ����� �������, ������ � FLOAT, 3 �����.
    unsigned char ucMinOfScaleMantissLsb;
    unsigned char ucMinOfScaleMantissMsb;
    unsigned char ucMinOfScaleExponent;
    // ��� - ������� ������ ����� �������, ������ � FLOAT, 3 �����.
    unsigned char ucMaxOfScaleMantissLsb;
    unsigned char ucMaxOfScaleMantissMsb;
    unsigned char ucMaxOfScaleExponent;
    // ������� LL, ������ � FLOAT, 3 �����.
    unsigned char ucSetPointLLMantissLsb;
    unsigned char ucSetPointLLMantissMsb;
    unsigned char ucSetPointLLExponent;
    // ������� L, ������ � FLOAT, 3 �����.
    unsigned char ucSetPointLMantissLsb;
    unsigned char ucSetPointLMantissMsb;
    unsigned char ucSetPointLExponent;
    // ������� H, ������ � FLOAT, 3 �����.
    unsigned char ucSetPointHMantissLsb;
    unsigned char ucSetPointHMantissMsb;
    unsigned char ucSetPointHExponent;
    // ������� HH, ������ � FLOAT, 3 �����.
    unsigned char ucSetPointHHMantissLsb;
    unsigned char ucSetPointHHMantissMsb;
    unsigned char ucSetPointHHExponent;
    // ��������� �������� ����� � 8 �������� ASCII.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH];
};
#pragma pack(pop)

// ��������� ��������� ������ ����������� ����� � ������ RAM, IEEE754.
// ����� �������������� �� ������ ������� ���� ������.
struct TAnalogueInputDescriptionModbus
{
    // ��� - ������ ������ ����� �������, ������ � FLOAT, IEEE754.
    float fMinOfScale;
    // ��� - ������� ������ ����� �������, ������ � FLOAT, IEEE754.
    float fMaxOfScale;
    // ������� LL, ������ � FLOAT, IEEE754.
    float fSetPointLL;
    // ������� L, ������ � FLOAT, IEEE754.
    float fSetPointL;
    // ������� H, ������ � FLOAT, IEEE754.
    float fSetPointH;
    // ������� HH, ������ � FLOAT, IEEE754.
    float fSetPointHH;
};

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ��������� ������ ����������� ����� � ������ RAM, IEEE754.
// ����� �������������� �� ������ ������� ���� ������.
struct TAnalogueInputDescriptionModbusPackOne
{
    // ��� - ������ ������ ����� �������, ������ � FLOAT, IEEE754.
    float fMinOfScale;
    // ��� - ������� ������ ����� �������, ������ � FLOAT, IEEE754.
    float fMaxOfScale;
    // ������� LL, ������ � FLOAT, IEEE754.
    float fSetPointLL;
    // ������� L, ������ � FLOAT, IEEE754.
    float fSetPointL;
    // ������� H, ������ � FLOAT, IEEE754.
    float fSetPointH;
    // ������� HH, ������ � FLOAT, IEEE754.
    float fSetPointHH;
};
#pragma pack(pop)

// ��������� ��������� ������ ����������� ����� � ������ RAM, IEEE754.
// ����� �������������� �� ������ ������� ���� ������.
struct TAnalogueInputDescriptionWork
{
    // ��� ����� ������� ���.
    unsigned char ucScaleType;
    // ��� ����������� ����������� ��������� ����.
    unsigned char ucDimentionCode;
    // ��� ��������� ���������� ����������� ��� ����5, ��� ��� �������� ������� ��� ����5 � ����5 (��� ����5 �������� �� �����) ����.
    unsigned char ucRangeCode;
    // ��� - ������ ������ ����� �������, ������ � FLOAT, IEEE754.
    float fMinOfScale;
    // ��� - ������� ������ ����� �������, ������ � FLOAT, IEEE754.
    float fMaxOfScale;
    // ������� LL, ������ � FLOAT, IEEE754.
    float fSetPointLL;
    // ������� L, ������ � FLOAT, IEEE754.
    float fSetPointL;
    // ������� H, ������ � FLOAT, IEEE754.
    float fSetPointH;
    // ������� HH, ������ � FLOAT, IEEE754.
    float fSetPointHH;
    // ��������� �������� ����� � 8 �������� ASCII, ���� ���� - ������� ����� ������.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
};

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ��������� ������ ����������� ����� � ������ RAM, IEEE754.
// ����� �������������� �� ������ ������� ���� ������.
struct TAnalogueInputDescriptionWorkPackOne
{
    // ��� ����� ������� ���.
    unsigned char ucScaleType;
    // ��� ����������� ����������� ��������� ����.
    unsigned char ucDimentionCode;
    // ��� ��������� ���������� ����������� ��� ����5, ��� ��� �������� ������� ��� ����5 � ����5 (��� ����5 �������� �� �����) ����.
    unsigned char ucRangeCode;
    // ��� - ������ ������ ����� �������, ������ � FLOAT, IEEE754.
    float fMinOfScale;
    // ��� - ������� ������ ����� �������, ������ � FLOAT, IEEE754.
    float fMaxOfScale;
    // ������� LL, ������ � FLOAT, IEEE754.
    float fSetPointLL;
    // ������� L, ������ � FLOAT, IEEE754.
    float fSetPointL;
    // ������� H, ������ � FLOAT, IEEE754.
    float fSetPointH;
    // ������� HH, ������ � FLOAT, IEEE754.
    float fSetPointHH;
    // ��������� �������� ����� � 8 �������� ASCII, ���� ���� - ������� ����� ������.
    char acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH + END_OF_STRING_LENGTH];
};
#pragma pack(pop)

// ��������� ��� �������� � �������� ����������� ��������� ���� ���.
struct TTxsData
{
    float fTxsValue; // �������� ���.
    unsigned char ucTxsStat; // ��������� ������ ���.
};

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ��� �������� � �������� ����������� ��������� ���� ���.
struct TTxsDataPackOne
{
    float fTxsValue; // �������� ���.
    unsigned char ucTxsStat; // ��������� ������ ���.
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
