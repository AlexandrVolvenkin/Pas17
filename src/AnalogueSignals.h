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
    uint8_t ucServiseL;
    // - ���� 0 � 5 � ��� ��������� ���������� ����������� ��� ����5, ��� ��� �������� ������� ��� ����5 � ����5 (��� ����5 �������� �� �����) ����.
    // - ���� 6,7 � ������.
    uint8_t ucServiseH;
    // ��� - ������ ������ ����� �������, ������ � FLOAT, 3 �����.
    uint8_t ucMinOfScaleMantissLsb;
    uint8_t ucMinOfScaleMantissMsb;
    uint8_t ucMinOfScaleExponent;
    // ��� - ������� ������ ����� �������, ������ � FLOAT, 3 �����.
    uint8_t ucMaxOfScaleMantissLsb;
    uint8_t ucMaxOfScaleMantissMsb;
    uint8_t ucMaxOfScaleExponent;
    // ������� LL, ������ � FLOAT, 3 �����.
    uint8_t ucSetPointLLMantissLsb;
    uint8_t ucSetPointLLMantissMsb;
    uint8_t ucSetPointLLExponent;
    // ������� L, ������ � FLOAT, 3 �����.
    uint8_t ucSetPointLMantissLsb;
    uint8_t ucSetPointLMantissMsb;
    uint8_t ucSetPointLExponent;
    // ������� H, ������ � FLOAT, 3 �����.
    uint8_t ucSetPointHMantissLsb;
    uint8_t ucSetPointHMantissMsb;
    uint8_t ucSetPointHExponent;
    // ������� HH, ������ � FLOAT, 3 �����.
    uint8_t ucSetPointHHMantissLsb;
    uint8_t ucSetPointHHMantissMsb;
    uint8_t ucSetPointHHExponent;
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
    uint8_t ucServiseL;
    // - ���� 0 � 5 � ��� ��������� ���������� ����������� ��� ����5, ��� ��� �������� ������� ��� ����5 � ����5 (��� ����5 �������� �� �����) ����.
    // - ���� 6,7 � ������.
    uint8_t ucServiseH;
    // ��� - ������ ������ ����� �������, ������ � FLOAT, 3 �����.
    uint8_t ucMinOfScaleMantissLsb;
    uint8_t ucMinOfScaleMantissMsb;
    uint8_t ucMinOfScaleExponent;
    // ��� - ������� ������ ����� �������, ������ � FLOAT, 3 �����.
    uint8_t ucMaxOfScaleMantissLsb;
    uint8_t ucMaxOfScaleMantissMsb;
    uint8_t ucMaxOfScaleExponent;
    // ������� LL, ������ � FLOAT, 3 �����.
    uint8_t ucSetPointLLMantissLsb;
    uint8_t ucSetPointLLMantissMsb;
    uint8_t ucSetPointLLExponent;
    // ������� L, ������ � FLOAT, 3 �����.
    uint8_t ucSetPointLMantissLsb;
    uint8_t ucSetPointLMantissMsb;
    uint8_t ucSetPointLExponent;
    // ������� H, ������ � FLOAT, 3 �����.
    uint8_t ucSetPointHMantissLsb;
    uint8_t ucSetPointHMantissMsb;
    uint8_t ucSetPointHExponent;
    // ������� HH, ������ � FLOAT, 3 �����.
    uint8_t ucSetPointHHMantissLsb;
    uint8_t ucSetPointHHMantissMsb;
    uint8_t ucSetPointHHExponent;
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
    uint8_t ucScaleType;
    // ��� ����������� ����������� ��������� ����.
    uint8_t ucDimentionCode;
    // ��� ��������� ���������� ����������� ��� ����5, ��� ��� �������� ������� ��� ����5 � ����5 (��� ����5 �������� �� �����) ����.
    uint8_t ucRangeCode;
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
    uint8_t ucScaleType;
    // ��� ����������� ����������� ��������� ����.
    uint8_t ucDimentionCode;
    // ��� ��������� ���������� ����������� ��� ����5, ��� ��� �������� ������� ��� ����5 � ����5 (��� ����5 �������� �� �����) ����.
    uint8_t ucRangeCode;
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
    uint8_t ucTxsStat; // ��������� ������ ���.
};

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ��� �������� � �������� ����������� ��������� ���� ���.
struct TTxsDataPackOne
{
    float fTxsValue; // �������� ���.
    uint8_t ucTxsStat; // ��������� ������ ���.
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
