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


//-------------------------------------------------------------------------------
// структура описател€ одной размерности.
// дл€ сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// структура описател€ одной размерности.
// после преобразовани€ из общего формата базы данных.
struct TDimentionParameterPackOne
{
    // “екстовый реквизит размерности Ц 6 символов ASCII.
    char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH];
};
#pragma pack(pop)

// структура описател€ одной размерности.
struct TDimentionParameter
{
    // “екстовый реквизит размерности Ц 6 символов ASCII, плюс нуль - признак конца строки.
    char acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH + END_OF_STRING_LENGTH];
};

//-------------------------------------------------------------------------------
// —труктура описателей - "“екстовый реквизит дискретного сигнала", в базе данных.
// дл€ сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
struct TDiscreteSygnalTextTitlePackOne
{
    // “екстовый реквизит дискретного сигнала Ц 14 символов ASCII.
    char acTextDescriptor[DISCRETE_SYGNAL_NAME_LENGTH];
};
#pragma pack(pop)

// —труктура описателей - "“екстовый реквизит дискретного сигнала", в базе данных.
struct TDiscreteSygnalTextTitle
{
// “екстовый реквизит дискретного сигнала Ц 14 символов ASCII, плюс нуль - признак конца строки.
    char acTextDescriptor[DISCRETE_SYGNAL_NAME_LENGTH + END_OF_STRING_LENGTH];
};

//-------------------------------------------------------------------------------
// —труктура описателей - "“екстовый реквизит аналогового сигнала", в базе данных.
// дл€ сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
struct TAnalogoueSignalsTextTitlePackOne
{
    // “екстовый реквизит аналогового сигнала Ц 35 символов ASCII.
    char acTextDescriptor[ANALOGUE_SYGNAL_NAME_LENGTH];
};
#pragma pack(pop)

// —труктура описателей - "“екстовый реквизит аналогового сигнала", в базе данных.
struct TAnalogoueSignalsTextTitle
{
// “екстовый реквизит аналогового сигнала Ц 35 символов ASCII, плюс нуль - признак конца строки.
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

        DATA_BASE_CREATE_DIMENTIONS_PARAMETERS_CREATE_START,
        DATA_BASE_CREATE_DIMENTIONS_PARAMETERS_CREATE_EXECUTOR_ANSWER_PROCESSING,

        DATA_BASE_CREATE_TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START,
        DATA_BASE_CREATE_TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING,
        DATA_BASE_CREATE_TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START,
        DATA_BASE_CREATE_TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING,
    };

    CDataBaseCreate();
    virtual ~CDataBaseCreate();

    void SetInternalModuleName(std::string sName);
    void SetDeviceControlName(std::string sName);

    uint8_t Init(void);
    uint8_t Fsm(void);

private:
    void DimentionsParametersDataBaseCreate(uint8_t* puiBlockDataPointer);
    void AnalogoueInputModuleDiscreteSignalsTextTitlesCreate(uint8_t* puiBlockDataPointer);
    void AnalogoueInputModuleAnalogoueSignalsTextTitlesCreate(uint8_t* puiBlockDataPointer);

private:
    std::string m_sInternalModuleName;
    uint8_t m_uiInternalModuleId;

    std::string m_sDeviceControlName;
    uint8_t m_uiDeviceControlId;

    uint8_t* m_puiIntermediateBuff;
    uint8_t m_uiBlocksCounter;

};

#endif // DATABASECREATE_H
//-------------------------------------------------------------------------------
