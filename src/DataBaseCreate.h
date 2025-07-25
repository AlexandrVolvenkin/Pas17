#ifndef DATABASECREATE_H
#define DATABASECREATE_H
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <time.h>


class Timer;
class Platform;
class CTask;
class CResources;
class CCommunicationDevice;
class CCommunicationDeviceInterface;
class CSpiCommunicationDevice;
class CDataContainerInterface;
class CDataContainerDataBase;
class CInternalModule;
class CConfigurationCreate;
class CDeviceControl;
class CDiscreteSignals;


//-------------------------------------------------------------------------------
// ��������� ��������� ����� �����������.
// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ��������� ����� �����������.
// ����� �������������� �� ������ ������� ���� ������.
struct TDimentionParameterPackOne
{
    // ��������� �������� ����������� � 6 �������� ASCII.
    char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH];
};
#pragma pack(pop)

// ��������� ��������� ����� �����������.
struct TDimentionParameter
{
    // ��������� �������� ����������� � 6 �������� ASCII, ���� ���� - ������� ����� ������.
    char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
};

//-------------------------------------------------------------------------------
// ��������� ���������� - "��������� �������� ����������� �������", � ���� ������.
// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
struct TDiscreteSygnalTextTitlePackOne
{
    // ��������� �������� ����������� ������� � 14 �������� ASCII.
    char acTextDescriptor[DISCRETE_SYGNAL_NAME_LENGTH];
};
#pragma pack(pop)

// ��������� ���������� - "��������� �������� ����������� �������", � ���� ������.
struct TDiscreteSygnalTextTitle
{
// ��������� �������� ����������� ������� � 14 �������� ASCII, ���� ���� - ������� ����� ������.
    char acTextDescriptor[DISCRETE_SYGNAL_NAME_LENGTH + END_OF_STRING_LENGTH];
};

//-------------------------------------------------------------------------------
// ��������� ���������� - "��������� �������� ����������� �������", � ���� ������.
// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
struct TAnalogoueSignalsTextTitlePackOne
{
    // ��������� �������� ����������� ������� � 35 �������� ASCII.
    char acTextDescriptor[ANALOGUE_SYGNAL_NAME_LENGTH];
};
#pragma pack(pop)

// ��������� ���������� - "��������� �������� ����������� �������", � ���� ������.
struct TAnalogoueSignalsTextTitle
{
// ��������� �������� ����������� ������� � 35 �������� ASCII, ���� ���� - ������� ����� ������.
    char acTextDescriptor[ANALOGUE_SYGNAL_NAME_LENGTH + END_OF_STRING_LENGTH];
};

//-------------------------------------------------------------------------------
class CDataBaseCreate : public CTask
{
public:
    enum
    {
        DATA_BASE_CREATE_START = NEXT_STEP,

        DATA_BASE_CREATE_DEFAULT_ALL_DATA_BASE_BLOCKS_WRITE_START,
        DATA_BASE_CREATE_DEFAULT_ALL_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING,

        DATA_BASE_CREATE_DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_START,
        DATA_BASE_CREATE_DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING,

        DATA_BASE_CREATE_DIMENTIONS_PARAMETERS_CREATE_START,
        DATA_BASE_CREATE_DIMENTIONS_PARAMETERS_CREATE_EXECUTOR_ANSWER_PROCESSING,

        DATA_BASE_CREATE_TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START,
        DATA_BASE_CREATE_TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING,

        DATA_BASE_CREATE_TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START,
        DATA_BASE_CREATE_TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING,

        DATA_BASE_CREATE_MUVR_CURRENT_OUTPUTS_DATA_BASE_BLOCK_WRITE_START,
        DATA_BASE_CREATE_MUVR_CURRENT_OUTPUTS_DATA_BASE_BLOCK_WRITE_EXECUTOR_ANSWER_PROCESSING,

        DATA_BASE_CREATE_CONFIGURATION_DATA_BASE_BLOCKS_WRITE_START,
        DATA_BASE_CREATE_CONFIGURATION_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING,

        DATA_BASE_CREATE_SERIAL_AND_ID_DATA_BASE_BLOCKS_WRITE_START,
        DATA_BASE_CREATE_SERIAL_AND_ID_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING,

        DATA_BASE_CREATE_NETWORK_ADDRESS_DATA_BASE_BLOCKS_WRITE_START,
        DATA_BASE_CREATE_NETWORK_ADDRESS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING,

        DATA_BASE_CREATE_MODULE_MUVR_DATA_BASE_READ_START,
        DATA_BASE_CREATE_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_ANSWER_PROCESSING,
        DATA_BASE_CREATE_MODULE_MUVR_DATA_BASE_BLOCKS_WRITE_START,
        DATA_BASE_CREATE_MODULE_MUVR_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING,
    };

    CDataBaseCreate();
    virtual ~CDataBaseCreate();

    // ������ ��� m_sDataStoreName
    std::string GetDataStoreName() const
    {
        return m_sDataStoreName;
    };

    // ������ ��� m_sDataStoreName
    void SetDataStoreName(const std::string& sName)
    {
        m_sDataStoreName = sName;
    };

    void SetInternalModuleName(std::string sName);
    void SetInternalModuleMuvrName(std::string sName);
    void SetDeviceControlName(std::string sName);

    uint8_t Init(void);
    uint8_t Fsm(void);

private:
    void DimentionsParametersDataBaseCreate(uint8_t* puiBlockDataPointer);
    void AnalogoueInputModuleDiscreteSignalsTextTitlesCreate(uint8_t* puiBlockDataPointer);
    void AnalogoueInputModuleAnalogoueSignalsTextTitlesCreate(uint8_t* puiBlockDataPointer);
    void MuvrCurrentOutputsDataBaseCreate(uint8_t* puiBlockDataPointer);

private:
    std::string m_sDataStoreName;
    uint8_t m_uiDataStoreId;

    std::string m_sInternalModuleName;
    uint8_t m_uiInternalModuleId;

    std::string m_sInternalModuleMuvrName;
    uint8_t m_uiInternalModuleMuvrId;

    std::string m_sDeviceControlName;
    uint8_t m_uiDeviceControlId;

    uint8_t* m_puiIntermediateBuff;
    uint8_t m_uiBlocksCounter;

};

#endif // DATABASECREATE_H
//-------------------------------------------------------------------------------
