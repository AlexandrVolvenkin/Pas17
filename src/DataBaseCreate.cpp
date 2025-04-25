//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <iostream>
#include <string.h>
#include <typeinfo>

#include "Task.h"
#include "Resources.h"
#include "Platform.h"
#include "DataStore.h"
#include "DeviceControl.h"
#include "DataContainer.h"
#include "InternalModule.h"
#include "ConfigurationCreate.h"
#include "DataBaseCreate.h"

using namespace std;

//-------------------------------------------------------------------------------
CDataBaseCreate::CDataBaseCreate()
{
    std::cout << "CDataBaseCreate constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[256];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CDataBaseCreate::~CDataBaseCreate()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
void CDataBaseCreate::SetInternalModuleName(std::string sName)
{
    m_sInternalModuleName = sName;
}

//-------------------------------------------------------------------------------
void CDataBaseCreate::SetDeviceControlName(std::string sName)
{
    m_sDeviceControlName = sName;
}

//-------------------------------------------------------------------------------
uint8_t CDataBaseCreate::Init(void)
{
    std::cout << "CDataBaseCreate Init"  << std::endl;
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
    SetCustomerDataContainer(GetExecutorDataContainerPointer());
}

//-----------------------------------------------------------------------------------------------------
// создаёт стартовую базу данных размерностей.
void DimentionsParametersDataBaseCreate(uint8_t* puiBlockDataPointer)
{
    TDimentionParameterPackOne *pxDimentionParameterPackOne;
    TDimentionParameter *pxDimentionParameter;
    TDimentionParameter axDimentionsParametersDescriptionWork[DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY];

    memset(axDimentionsParametersDescriptionWork,
           0,
           sizeof(axDimentionsParametersDescriptionWork));

    pxDimentionParameter = axDimentionsParametersDescriptionWork;

    // создадим стартовую базу данных размерностей.
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "град.С");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "%");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "МПа");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "КПа");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "кг/см2");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "атм");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "об/мин");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "1/с");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "мм/с2");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "м3/ч");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "нм3/ч");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "кг/ч");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "т/ч");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "м");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "резерв");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "резерв");

    // получим указатель на буфер с вновь созданной стартовой базой данных прибора.
    pxDimentionParameter = axDimentionsParametersDescriptionWork;
    // получим указатель на временный буфер в котором сформируем блок базы данных
    // в общем формате хранения
    pxDimentionParameterPackOne = (TDimentionParameterPackOne*)puiBlockDataPointer;
    // преобразуем созданную базу данных в общий формат.
    for (int i = 0;
            i < DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY;
            i++)
    {
        // скопируем один описатель, в буфер общей базы данных прибора.
        memcpy(pxDimentionParameterPackOne[i].acDimentionParameterText,
               pxDimentionParameter[i].acDimentionParameterText,
               DIMENSIONS_PARAMETERS_NAME_LENGTH);
    }
}

//-----------------------------------------------------------------------------------------------------
//#include <iostream>
//#include <cstring> // для memset()
//#include <vector>
//
//using namespace std;

// Класс, представляющий описатель текстового реквизита дискретного сигнала
class DiscreteSygnalTextDescriptor
{
public:
    string acTextDescriptor;  // строка текстового реквизита
};

// Функция для создания стартовой базы данных прибора с использованием класса std::string
void iDiscreteSygnalTextDescriptorStartDataBaseCreate()
{
    int i;
    unsigned char nucBlockCounter = 0;
    unsigned char nucBlocksInBlockCounter = 0; // один описатель - один блок, в общем блоке.
    unsigned char nucDiscreteInputCounter = 0;
    unsigned char nucModuleInputCounter = 0;
    // количество уставок по одному входу(4 - LL, L, H, HH).
    unsigned char nucInputSetPointCounter = 0;
    unsigned char nucModuleCounter = 1;
    i = 0;

    // Создание вектора для нормализованной стартовой базы данных прибора
    vector<DiscreteSygnalTextDescriptor> xDiscreteSygnalTextDescriptorPackOne;

    // Начнём с первого входа.
    nucDiscreteInputCounter = 0;
    nucModuleInputCounter = 0;

    // Создадим первую часть стартовой базы данных текстовых реквизитов дискретных сигналов.
    // для сигналов пораждаемых модулями дискретного ввода.
    while (i < (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity * DISCRETE_MODULE_INPUT_QUANTITY))
    {
        // Создадим строку текстового реквизита дискретного сигнала
        string textDescriptor = "Вх. DI" + to_string(nucModuleCounter) + "_" + to_string(nucDiscreteInputCounter + 1);
        xDiscreteSygnalTextDescriptorPackOne.push_back({textDescriptor});

        // Следующий вход модуля дискретного ввода.
        nucDiscreteInputCounter++;
        // Следующий описатель.
        i++;

        // Обработаны все входы модуля дискретного ввода?
        if (nucDiscreteInputCounter == DISCRETE_MODULE_INPUT_QUANTITY)
        {
            // Начнём с первого входа.
            nucDiscreteInputCounter = 0;
            // Следующий модуль.
            nucModuleCounter++;
        }
    }

    // Начнём с первого входа.
    nucDiscreteInputCounter = 0;
    nucModuleInputCounter = 0;

    // Создадим вторую часть стартовой базы данных текстовых реквизитов дискретных сигналов.
    // для сигналов пораждаемых модулями аналогового ввода.
    // один аналоговый вход пораждает четыре дискретных сигнала - уставки: LL, L, H, HH.
    // Начнём с создания описателя для уставки LL+L.
    unsigned char ucFlowControl = 1; // ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;

    while (i < ((xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity * DISCRETE_MODULE_INPUT_QUANTITY) +
                (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity * ANALOG_MODULE_INPUT_QUANTITY *
                 ANALOGUE_INPUT_DI_VALUE_QUANTITY)))
    {
        switch(ucFlowControl)
        {
        case 1: // ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L
            // Создадим строку текстового реквизита дискретного сигнала.
            string textDescriptor = "Вх. AI" + to_string(nucModuleCounter) + "_" + to_string(nucModuleInputCounter + 1) + "_LL";
            xDiscreteSygnalTextDescriptorPackOne.push_back({textDescriptor});

            // Перейдём к заполнению описателя следующей уставки.
            ucFlowControl = 2; // ANALOGUE_INPUT_SET_POINT_VIOLATION_L
            break;
        case 2: // ANALOGUE_INPUT_SET_POINT_VIOLATION_L
            // Создадим строку текстового реквизита дискретного сигнала.
            string textDescriptor = "Вх. AI" + to_string(nucModuleCounter) + "_" + to_string(nucModuleInputCounter + 1) + "_L ";
            xDiscreteSygnalTextDescriptorPackOne.push_back({textDescriptor});

            // Перейдём к заполнению описателя следующей уставки.
            ucFlowControl = 3; // ANALOGUE_INPUT_SET_POINT_VIOLATION_H
            break;
        case 3: // ANALOGUE_INPUT_SET_POINT_VIOLATION_H
            // Создадим строку текстового реквизита дискретного сигнала.
            string textDescriptor = "Вх. AI" + to_string(nucModuleCounter) + "_" + to_string(nucModuleInputCounter + 1) + "_H ";
            xDiscreteSygnalTextDescriptorPackOne.push_back({textDescriptor});

            // Перейдём к заполнению описателя следующей уставки.
            ucFlowControl = 4; // ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H
            break;
        case 4: // ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H
            // Создадим строку текстового реквизита дискретного сигнала.
            string textDescriptor = "Вх. AI" + to_string(nucModuleCounter) + "_" + to_string(nucModuleInputCounter + 1) + "_HH";
            xDiscreteSygnalTextDescriptorPackOne.push_back({textDescriptor});

            // Перейдём к заполнению описателя следующей уставки.
            ucFlowControl = 1; // ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L
            break;
        default:
            break;
        };

        nucDiscreteInputCounter++;
        nucInputSetPointCounter++;
        // количество уставок по одному входу(4 - LL, L, H, HH).
        // Прошли по всем уставкам одного входа?
        if (nucInputSetPointCounter >= 4)
        {
            nucInputSetPointCounter = 0;
            // Следующий вход.
            nucModuleInputCounter++;
        }
        // Следующий описатель.
        i++;

        // Обработаны все дискретные сигналы пораждённые входами модуля аналогового ввода?
        if (nucDiscreteInputCounter == (ANALOG_MODULE_INPUT_QUANTITY * ANALOGUE_INPUT_DI_VALUE_QUANTITY))
        {
            // Начнём с первого входа.
            nucDiscreteInputCounter = 0;
            nucModuleInputCounter = 0;
            // Начнём с первой уставки - LL+L.
            nucInputSetPointCounter = 0;
            // Следующий модуль.
            nucModuleCounter++;
        }
    }

    // Получим указатель на вновь созданной нормализованной стартовой базой данных прибора
    vector<DiscreteSygnalTextDescriptor>* pxDiscreteSygnalTextDescriptorPackOne = &xDiscreteSygnalTextDescriptorPackOne;

    // Получим указатель на базу данных прибора в общем формате.
    TDiscreteSygnalTextDescriptorPackOne* pxDiscreteSygnalTextDescriptorPackOnePtr = new TDiscreteSygnalTextDescriptorPackOne[xDiscreteSygnalTextDescriptorPackOne.size()];
    memcpy(pxDiscreteSygnalTextDescriptorPackOnePtr, &xDiscreteSygnalTextDescriptorPackOne[0], xDiscreteSygnalTextDescriptorPackOne.size() * sizeof(TDiscreteSygnalTextDescriptorPackOne));

    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;

    // Преобразуем созданную базу данных в общий формат.
    for (int i = 0; i < MAX_HANDLED_DISCRETE_INPUT; i++)
    {
        // Обработан весь блок базы данных?
        if (nucBlocksInBlockCounter == TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // следующий блок базы данных.
            nucBlockCounter++;
            // Получим указатель на следующий блок в общей базе данных прибора.
            pxDiscreteSygnalTextDescriptorPackOnePtr = (TDiscreteSygnalTextDescriptorPackOne*)&(xPlcDataBase.axPlcDataBaseBlocks[TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }

        // скопируем один описатель текстовых реквизитов дискретного сигнала, в буфер общего формата.
        memcpy((uint8_t*)&pxDiscreteSygnalTextDescriptorPackOnePtr[nucBlocksInBlockCounter],
               (uint8_t*)&xDiscreteSygnalTextDescriptorPackOnePtr[i],
               sizeof(struct TDiscreteSygnalTextDescriptorPackOne));

        // следующий описатель.
        nucBlocksInBlockCounter++;
    }
}

//-----------------------------------------------------------------------------------------------------
// создаёт стартовую базу данных текстовых реквизитов дискретных сигналов.
int iDiscreteSygnalTextDescriptorStartDataBaseCreate(void)
{
    int i;
    unsigned char nucBlockCounter;
    unsigned char nucBlocksInBlockCounter; // один описатель - один блок, в общем блоке.
    unsigned char nucDiscreteInputCounter;
    unsigned char nucModuleInputCounter;
    // количество уставок по одному входу(4 - LL, L, H, HH).
    unsigned char nucInputSetPointCounter;
    unsigned char nucModuleCounter;
    unsigned char ucFlowControl;
    TDiscreteSygnalTextDescriptorPackOne *pxDiscreteSygnalTextDescriptorPackOne;
    TDiscreteSygnalTextDescriptorPackOne xDiscreteSygnalTextDescriptorPackOne;
    TDiscreteSygnalTextDescriptor *pxDiscreteSygnalTextDescriptor;


    // получим указатель на буфер для нормализованной стартовой базы данных прибора.
    pxDiscreteSygnalTextDescriptor = axDiscreteSygnalTextDescriptor;
    // начнём с первого входа.
    nucDiscreteInputCounter = 0;
    nucModuleInputCounter = 0;
    // начнём с первого модуля.
    nucModuleCounter = 1;
    i = 0;

    memset((unsigned char*)pxDiscreteSygnalTextDescriptor,
           0,
           (sizeof(struct TDiscreteSygnalTextDescriptor)));

    // создадим первую часть стартовой базы данных текстовых реквизитов дискретных сигналов.
    // для сигналов пораждаемых модулями дискретного ввода.
    while (i < (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity *
                DISCRETE_MODULE_INPUT_QUANTITY))
    {
        // создадим строку текстового реквизита дискретного сигнала.
        sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                "%s_%02d_%02d  ",
                "Вх. DI",
                nucModuleCounter,
                nucDiscreteInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
        // следующий вход модуля дискретного ввода.
        nucDiscreteInputCounter++;
        // следующий описатель.
        i++;
        // обработаны все входы модуля дискретного ввода?
        if (nucDiscreteInputCounter == DISCRETE_MODULE_INPUT_QUANTITY)
        {
            // начнём с первого входа.
            nucDiscreteInputCounter = 0;
            // следующий модуль.
            nucModuleCounter++;
        }
    }

    // начнём с первого входа.
    nucDiscreteInputCounter = 0;
    nucModuleInputCounter = 0;
    // начнём с первой уставки - LL+L.
    nucInputSetPointCounter = 0;
    // начнём с первого модуля.
    nucModuleCounter = 1;

    // создадим вторую часть стартовой базы данных текстовых реквизитов дискретных сигналов.
    // для сигналов пораждаемых модулями аналогового ввода.
    // один аналоговый вход пораждает четыре дискретных сигнала - уставки: LL, L, H, HH.
    // начнём с создания описателя для уставки LL+L.
    ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
    while (i < ((xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity *
                 DISCRETE_MODULE_INPUT_QUANTITY) +
                (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity *
                 ANALOG_MODULE_INPUT_QUANTITY *
                 ANALOGUE_INPUT_DI_VALUE_QUANTITY)))
    {
        switch(ucFlowControl)
        {
        case ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L:
            // создадим строку текстового реквизита дискретного сигнала.
            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                    "%s_%02d_%01d_LL",
                    "Вх. AI",
                    nucModuleCounter,
                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
            // перейдём к заполнению описателя следующей уставки.
            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
            break;

        case ANALOGUE_INPUT_SET_POINT_VIOLATION_L:
            // создадим строку текстового реквизита дискретного сигнала.
            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                    "%s_%02d_%01d_L ",
                    "Вх. AI",
                    nucModuleCounter,
                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);

            // перейдём к заполнению описателя следующей уставки.
            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
            break;

        case ANALOGUE_INPUT_SET_POINT_VIOLATION_H:
            // создадим строку текстового реквизита дискретного сигнала.
            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                    "%s_%02d_%01d_H ",
                    "Вх. AI",
                    nucModuleCounter,
                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);

            // перейдём к заполнению описателя следующей уставки.
            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
            break;

        case ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H:
            // создадим строку текстового реквизита дискретного сигнала.
            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                    "%s_%02d_%01d_HH",
                    "Вх. AI",
                    nucModuleCounter,
                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);

            // перейдём к заполнению описателя следующей уставки.
            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
            break;

        default:
            break;
        };

        nucDiscreteInputCounter++;
        nucInputSetPointCounter++;
        // количество уставок по одному входу(4 - LL, L, H, HH).
        // прошли по всем уставкам одного входа?
        if (nucInputSetPointCounter >= 4)
        {
            nucInputSetPointCounter = 0;
            // следующий вход.
            nucModuleInputCounter++;
        }
        // следующий описатель.
        i++;

        // обработаны все дискретные сигналы пораждённые входами модуля аналогового ввода?
        if (nucDiscreteInputCounter == (ANALOG_MODULE_INPUT_QUANTITY *
                                        ANALOGUE_INPUT_DI_VALUE_QUANTITY))
        {
            // начнём с первого входа.
            nucDiscreteInputCounter = 0;
            nucModuleInputCounter = 0;
            // начнём с первой уставки - LL+L.
            nucInputSetPointCounter = 0;
            // следующий модуль.
            nucModuleCounter++;
        }
    }

    // получим указатель на буфер с вновь созданной нормализованной стартовой базой данных прибора.
    pxDiscreteSygnalTextDescriptor = axDiscreteSygnalTextDescriptor;
    // получим указатель на базу данных прибора в общем формате.
    pxDiscreteSygnalTextDescriptorPackOne = (TDiscreteSygnalTextDescriptorPackOne*)&(xPlcDataBase.
                                            axPlcDataBaseBlocks[TEXT_TITLES_DATA_BASE_BLOCK_OFFSET].
                                            aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;

    // преобразуем созданную базу данных в общий формат.
    for (int i = 0;
            i < MAX_HANDLED_DISCRETE_INPUT;
            i++)
    {
        // обработан весь блок базы данных?
        if (nucBlocksInBlockCounter == TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // следующий блок базы данных.
            nucBlockCounter++;
            // получим указатель на следующий блок в общей базе данных прибора.
            pxDiscreteSygnalTextDescriptorPackOne = (TDiscreteSygnalTextDescriptorPackOne*)&(xPlcDataBase.
                                                    axPlcDataBaseBlocks[TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                                                    aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }

        // скопируем один описатель текстовых реквизитов дискретного сигнала, в буфер общего формата.
        memcpy((&xDiscreteSygnalTextDescriptorPackOne.
                acTextDescriptor),
               (&pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
               DISCRETE_SYGNAL_NAME_LENGTH);

        memcpy((uint8_t*)&pxDiscreteSygnalTextDescriptorPackOne[nucBlocksInBlockCounter],
               (uint8_t*)&xDiscreteSygnalTextDescriptorPackOne,
               sizeof(struct TDiscreteSygnalTextDescriptorPackOne));

        // следующий описатель.
        nucBlocksInBlockCounter++;
    }
}

//-------------------------------------------------------------------------------
uint8_t CDataBaseCreate::Fsm(void)
{
//    std::cout << "CDataBaseCreate::Fsm 1" << endl;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CDataBaseCreate::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CDataBaseCreate::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CDataBaseCreate::Fsm START"  << std::endl;
        Init();
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CDataBaseCreate::Fsm INIT 1"  << std::endl;
        m_uiInternalModuleId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleName);

        m_uiDeviceControlId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDeviceControlName);

        SetFsmState(READY);

        break;

    case READY:
//        std::cout << "CDataBaseCreate::Fsm READY"  << std::endl;
        break;

    case DONE_OK:
//        std::cout << "CDataBaseCreate::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CDataBaseCreate::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START:
        std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING:
//        std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else
        {
            std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
        std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
            std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
        std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
        }

        break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(GetFsmNextStateDoneWaitingDoneError());
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateDoneWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case DATA_BASE_CREATE_START:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_START"  << std::endl;
        {
            *(GetResources() -> GetDeviceConfigSearchPointer()) = {0};
        }
        SetFsmState(DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_START);
        break;

//    case DATA_BASE_CREATE_EXECUTOR_ANSWER_PROCESSING:
//        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
//        {
//            CDataContainerDataBase* pxExecutorDataContainer =
//                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//            CDataContainerDataBase* pxCustomerDataContainer =
//                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
//
//            uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
//            uint16_t  uiLength = sizeof(auiTempData);
//            memcpy(pxExecutorDataContainer -> m_puiDataPointer,
//                   auiTempData,
//                   uiLength);
//
//            pxExecutorDataContainer -> m_uiDataLength = uiLength;
//
//            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
//                   (pxExecutorDataContainer -> m_puiDataPointer),
//                   pxExecutorDataContainer -> m_uiDataLength);
//            pxCustomerDataContainer -> m_uiDataLength =
//                pxExecutorDataContainer -> m_uiDataLength;
//
//            std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_EXECUTOR_DONE_CHECK_WAITING uiLength "  << (int)uiLength << std::endl;
//            std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_EXECUTOR_DONE_CHECK_WAITING pxCustomerDataContainer -> m_uiDataLength "  << (int)pxCustomerDataContainer -> m_uiDataLength << std::endl;
//
//            SetFsmState(DONE_OK);
//        }
//        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_START:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_START"  << std::endl;
        {
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiInternalModuleId;
            pxDataContainer -> m_uiFsmCommandState =
                CInternalModule::SEARCH_MODULES_START;
            pxDataContainer -> m_puiDataPointer =
                (uint8_t*)(GetResources() -> GetDeviceConfigSearchPointer());

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
//            CDataContainerDataBase* pxExecutorDataContainer =
//                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//            CDataContainerDataBase* pxCustomerDataContainer =
//                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
//
//            uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
//            uint16_t  uiLength = sizeof(auiTempData);
////            memcpy(pxExecutorDataContainer -> m_puiDataPointer,
////                   auiTempData,
////                   uiLength);
//
//            pxExecutorDataContainer -> m_uiDataLength = uiLength;
//
//            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
//                   (pxExecutorDataContainer -> m_puiDataPointer),
//                   pxExecutorDataContainer -> m_uiDataLength);
//            pxCustomerDataContainer -> m_uiDataLength =
//                pxExecutorDataContainer -> m_uiDataLength;
//
//            std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_EXECUTOR_ANSWER_PROCESSING uiLength "  << (int)uiLength << std::endl;
//            std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_EXECUTOR_ANSWER_PROCESSING pxCustomerDataContainer -> m_uiDataLength "  << (int)pxCustomerDataContainer -> m_uiDataLength << std::endl;

            SetFsmState(DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_START);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_START:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_START"  << std::endl;
        {
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiInternalModuleId;
            pxDataContainer -> m_uiFsmCommandState =
                CInternalModule::SERVICE_DATA_CREATE_START;
            pxDataContainer -> m_puiDataPointer =
                (uint8_t*)(GetResources() -> GetDeviceConfigSearchPointer());

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
//            CDataContainerDataBase* pxExecutorDataContainer =
//                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//            CDataContainerDataBase* pxCustomerDataContainer =
//                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
//
////            uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
////            uint16_t  uiLength = sizeof(auiTempData);
////            memcpy(pxExecutorDataContainer -> m_puiDataPointer,
////                   auiTempData,
////                   uiLength);
//            uint16_t  uiLength = sizeof(struct TConfigDataPackOne);
//            memcpy(pxExecutorDataContainer -> m_puiDataPointer,
//                   (uint8_t*)(GetResources() -> GetDeviceConfigSearchPointer()),
//                   uiLength);
//
//            pxExecutorDataContainer -> m_uiDataLength = uiLength;
//
//            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
//                   (pxExecutorDataContainer -> m_puiDataPointer),
//                   pxExecutorDataContainer -> m_uiDataLength);
//            pxCustomerDataContainer -> m_uiDataLength =
//                pxExecutorDataContainer -> m_uiDataLength;
//
//            std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_EXECUTOR_ANSWER_PROCESSING uiLength "  << (int)uiLength << std::endl;
//            std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_EXECUTOR_ANSWER_PROCESSING pxCustomerDataContainer -> m_uiDataLength "  << (int)pxCustomerDataContainer -> m_uiDataLength << std::endl;
//
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//            SetFsmState(DONE_OK);
        }
        break;

//-------------------------------------------------------------------------------
    case CONFIGURATION_REQUEST_START:
        std::cout << "CDataBaseCreate::Fsm CONFIGURATION_REQUEST_START"  << std::endl;
        {
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }

        SetFsmState(CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING);
        break;

    case CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
//            CDataContainerDataBase* pxExecutorDataContainer =
//                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//            CDataContainerDataBase* pxCustomerDataContainer =
//                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
//
////            uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
////            uint16_t  uiLength = sizeof(auiTempData);
////            memcpy(pxExecutorDataContainer -> m_puiDataPointer,
////                   auiTempData,
////                   uiLength);
//            uint16_t  uiLength = sizeof(struct TConfigDataPackOne);
//            memcpy(pxExecutorDataContainer -> m_puiDataPointer,
//                   (uint8_t*)(GetResources() -> GetDeviceConfigSearchPointer()),
//                   uiLength);
//
//            pxExecutorDataContainer -> m_uiDataLength = uiLength;
//
//            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
//                   (pxExecutorDataContainer -> m_puiDataPointer),
//                   pxExecutorDataContainer -> m_uiDataLength);
//            pxCustomerDataContainer -> m_uiDataLength =
//                pxExecutorDataContainer -> m_uiDataLength;
//
//            std::cout << "CDataBaseCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING uiLength "  << (int)uiLength << std::endl;
//            std::cout << "CDataBaseCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING pxCustomerDataContainer -> m_uiDataLength "  << (int)pxCustomerDataContainer -> m_uiDataLength << std::endl;
//            std::cout << "CDataBaseCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING pxExecutorDataContainer -> m_uiDataLength "  << (int)pxExecutorDataContainer -> m_uiDataLength << std::endl;
//
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//            SetFsmState(DONE_OK);
        }
        break;

//-------------------------------------------------------------------------------
    case DIMENTIONS_PARAMETERS_CREATE_START:
        std::cout << "CDataBaseCreate::Fsm DIMENTIONS_PARAMETERS_CREATE_START"  << std::endl;
        {
            DimentionsParametersDataBaseCreate(m_puiIntermediateBuff);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
            pxDataContainer -> m_uiFsmCommandState =
                CDeviceControl::DATA_BASE_BLOCK_START_WRITE;
            pxDataContainer -> m_uiDataIndex = DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START);
            SetFsmNextStateDoneOk(DIMENTIONS_PARAMETERS_CREATE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DIMENTIONS_PARAMETERS_CREATE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm DIMENTIONS_PARAMETERS_CREATE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(DONE_OK);
        }
        break;

//-------------------------------------------------------------------------------
    default:
        break;
    }
}

//-------------------------------------------------------------------------------

