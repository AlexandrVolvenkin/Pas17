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
#include "InternalModuleMuvr.h"
#include "ConfigurationCreate.h"
#include "DataBaseCreate.h"

using namespace std;

//-------------------------------------------------------------------------------
CDataBaseCreate::CDataBaseCreate()
{
    std::cout << "CDataBaseCreate constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[CDataStore::MAX_BLOCK_LENGTH];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CDataBaseCreate::~CDataBaseCreate()
{
    delete[] m_puiIntermediateBuff;
    delete[] GetResources() ->
    m_pxDiscreteSygnalTextTitlesWork;
}

//-------------------------------------------------------------------------------
void CDataBaseCreate::SetInternalModuleName(std::string sName)
{
    m_sInternalModuleName = sName;
}

//-------------------------------------------------------------------------------
void CDataBaseCreate::SetInternalModuleMuvrName(std::string sName)
{
    m_sInternalModuleMuvrName = sName;
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

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

    GetResources() ->
    m_pxDiscreteSygnalTextTitlesWork = new TDiscreteSygnalTextTitle[MAX_HANDLED_DISCRETE_INPUT];
}

//-------------------------------------------------------------------------------
// создаёт стартовую базу данных размерностей.
void CDataBaseCreate::DimentionsParametersDataBaseCreate(uint8_t* puiBlockDataPointer)
{
    TDimentionParameterPackOne *pxDimentionParameterPackOne;
    TDimentionParameter *pxDimentionParameter;
    TDimentionParameter axDimentionsParametersDescriptionWork[DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY];

    memset(puiBlockDataPointer,
           0,
           CDataStore::MAX_BLOCK_LENGTH);

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

        std::cout << "Dimention "  << i << " " << pxDimentionParameterPackOne[i].acDimentionParameterText << std::endl;
    }
}

//-------------------------------------------------------------------------------
// создаёт стартовую базу текстовых реквизитов модулей аналогового ввода.
void CDataBaseCreate::AnalogoueInputModuleDiscreteSignalsTextTitlesCreate(uint8_t* puiBlockDataPointer)
{
//-------------------------------------------------------------------------------
    TDiscreteSygnalTextTitlePackOne *pxDiscreteSygnalTextTitlePackOne;
    TDiscreteSygnalTextTitle *pxDiscreteSygnalTextTitle;

    memset(puiBlockDataPointer,
           0,
           CDataStore::MAX_BLOCK_LENGTH);

    TDiscreteSygnalTextTitle axDiscreteSygnalTextTitles[] =
    {
        {"Диск.вход.DIn1"},
        {"Диск.вход.DIn2"},
        {"Диск.вход.DIn3"},
        {"Диск.вход.DIn4"},
        {"Диск.вход.DIn5"},
        {"Диск.вход.DIn6"},

        {"Изм.зн.AIn1<LL"},
        {"Изм.зн.AIn1<L "},
        {"Изм.зн.AIn1>H "},
        {"Изм.зн.AIn1>HH"},

        {"Изм.зн.AIn2<LL"},
        {"Изм.зн.AIn2<L "},
        {"Изм.зн.AIn2>H "},
        {"Изм.зн.AIn2>HH"},

        {"Изм.зн.AIn3<LL"},
        {"Изм.зн.AIn3<L "},
        {"Изм.зн.AIn3>H "},
        {"Изм.зн.AIn3>HH"},

        {"Изм.зн.AIn4<LL"},
        {"Изм.зн.AIn4<L "},
        {"Изм.зн.AIn4>H "},
        {"Изм.зн.AIn4>HH"},
    };

    // Вычисляем количество элементов в массиве
    uint8_t uiElementNumber = (sizeof(axDiscreteSygnalTextTitles) /
                               sizeof(struct TDiscreteSygnalTextTitle));
    pxDiscreteSygnalTextTitle = axDiscreteSygnalTextTitles;
    // получим указатель на место во временном буфере, в котором сформируем блок
    // базы данных текстовых реквизитов дискрктных сигналов поорождаемых аналоговыми входами,
    // в общем формате хранения.
    pxDiscreteSygnalTextTitlePackOne = (TDiscreteSygnalTextTitlePackOne*)puiBlockDataPointer;
    // преобразуем созданную базу данных в общий формат.
    for (int i = 0;
            i < uiElementNumber;
            i++)
    {
        // скопируем один описатель, в буфер общей базы данных прибора.
        memcpy(pxDiscreteSygnalTextTitlePackOne[i].acTextDescriptor,
               pxDiscreteSygnalTextTitle[i].acTextDescriptor,
               DISCRETE_SYGNAL_NAME_LENGTH);

        std::cout << "TextDescriptor "  << i << " " << pxDiscreteSygnalTextTitlePackOne[i].acTextDescriptor << std::endl;
    }
}

//-------------------------------------------------------------------------------
// создаёт стартовую базу текстовых реквизитов аналоговых сигналов модулей аналогового ввода.
void CDataBaseCreate::AnalogoueInputModuleAnalogoueSignalsTextTitlesCreate(uint8_t* puiBlockDataPointer)
{
//-------------------------------------------------------------------------------
    TDiscreteSygnalTextTitlePackOne *pxDiscreteSygnalTextTitlePackOne;
    TDiscreteSygnalTextTitle *pxDiscreteSygnalTextTitle;

    memset(puiBlockDataPointer,
           0,
           CDataStore::MAX_BLOCK_LENGTH);

    TDiscreteSygnalTextTitle axDiscreteSygnalTextTitles[] =
    {
        {"______________"},
        {"______________"},
        {"______________"},
        {"______________"},
        {"______________"},
        {"______________"},
        {"______________"}
    };

    // Вычисляем количество элементов в массиве
    uint8_t uiElementNumber = (sizeof(axDiscreteSygnalTextTitles) /
                               sizeof(struct TDiscreteSygnalTextTitle));
    pxDiscreteSygnalTextTitle = axDiscreteSygnalTextTitles;
    // получим указатель на место во временном буфере, в котором сформируем первую часть блока
    // базы данных текстовых реквизитов аналоговых сигналов, в общем формате хранения.
    pxDiscreteSygnalTextTitlePackOne = (TDiscreteSygnalTextTitlePackOne*)puiBlockDataPointer;
    // преобразуем созданную базу данных в общий формат.
    for (int i = 0;
            i < uiElementNumber;
            i++)
    {
        // скопируем один описатель, в буфер общей базы данных прибора.
        memcpy(pxDiscreteSygnalTextTitlePackOne[i].acTextDescriptor,
               pxDiscreteSygnalTextTitle[i].acTextDescriptor,
               DISCRETE_SYGNAL_NAME_LENGTH);

        std::cout << "TextDescriptor "  << i << " " << pxDiscreteSygnalTextTitlePackOne[i].acTextDescriptor << std::endl;
    }

//-------------------------------------------------------------------------------
    TAnalogoueSignalsTextTitlePackOne *pxAnalogoueSignalsTextTitlePackOne;
    TAnalogoueSignalsTextTitle *pxAnalogoueSignalsTextTitle;

    TAnalogoueSignalsTextTitle axAnalogoueSignalsTextTitles[] =
    {
        {"Аналоговый входной сигнал AIn1_____"},
        {"Аналоговый входной сигнал AIn2_____"},
        {"Аналоговый входной сигнал AIn3_____"},
        {"Аналоговый входной сигнал AIn4_____"}
    };

    // получим указатель на место во временном буфере, в котором сформируем вторую часть блока
    // базы данных текстовых реквизитов аналоговых сигналов, в общем формате хранения.
    pxAnalogoueSignalsTextTitlePackOne =
        (TAnalogoueSignalsTextTitlePackOne*)(puiBlockDataPointer +
                (sizeof(struct TDiscreteSygnalTextTitle) *
                 uiElementNumber));

    // Вычисляем количество элементов в массиве
    uiElementNumber = (sizeof(axAnalogoueSignalsTextTitles) /
                       sizeof(struct TAnalogoueSignalsTextTitle));
    pxAnalogoueSignalsTextTitle = axAnalogoueSignalsTextTitles;
    // преобразуем созданную базу данных в общий формат.
    for (int i = 0;
            i < uiElementNumber;
            i++)
    {
        // скопируем один описатель, в буфер общей базы данных прибора.
        memcpy(pxAnalogoueSignalsTextTitlePackOne[i].acTextDescriptor,
               pxAnalogoueSignalsTextTitle[i].acTextDescriptor,
               ANALOGUE_SYGNAL_NAME_LENGTH);

        std::cout << "TextDescriptor "  << i << " " << pxAnalogoueSignalsTextTitlePackOne[i].acTextDescriptor << std::endl;
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

        m_uiDataStoreId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDataStoreName);

        m_uiInternalModuleMuvrId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleMuvrName);

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
//            // для создания новой базы данных нужно создать новый сервисный блок в хранилище,
//            // чтобы стереть прошлую информацию о сохранённых блоках.
//            m_pxDataStoreFileSystem -> CreateServiceSection();
            m_uiBlocksCounter = 0;
            SetFsmState(DATA_BASE_CREATE_DEFAULT_ALL_DATA_BASE_BLOCKS_WRITE_START);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_CREATE_DEFAULT_ALL_DATA_BASE_BLOCKS_WRITE_START:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_DEFAULT_ALL_DATA_BASE_BLOCKS_WRITE_START"  << std::endl;
        {
            memset(m_puiIntermediateBuff,
                   0,
                   CDataStore::MAX_BLOCK_LENGTH);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
            pxDataContainer -> m_uiDataIndex = m_uiBlocksCounter;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_CREATE_DEFAULT_ALL_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DATA_BASE_CREATE_DEFAULT_ALL_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_DEFAULT_ALL_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            m_uiBlocksCounter++;

            if (m_uiBlocksCounter >= CDataStore::MAX_BLOCKS_NUMBER)
            {
                SetFsmState(DATA_BASE_CREATE_DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_START);
            }
            else
            {
                SetFsmState(DATA_BASE_CREATE_DEFAULT_ALL_DATA_BASE_BLOCKS_WRITE_START);
            }
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_CREATE_DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_START:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_START"  << std::endl;
        {
            uint8_t uiDiscreteSignalsId =
                GetResources() ->
                GetTaskIdByNameFromMap("DiscreteSignals");

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = uiDiscreteSignalsId;
            pxDataContainer -> m_uiFsmCommandState =
                CDiscreteSignals::DISCRETE_SIGNALS_CREATE_DATA_BASE_START;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_CREATE_DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DATA_BASE_CREATE_DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_DISCRETE_SIGNALS_CREATE_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(DATA_BASE_CREATE_DIMENTIONS_PARAMETERS_CREATE_START);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_CREATE_DIMENTIONS_PARAMETERS_CREATE_START:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_DIMENTIONS_PARAMETERS_CREATE_START"  << std::endl;
        {
            DimentionsParametersDataBaseCreate(m_puiIntermediateBuff);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
            // стартовые текстовые реквизиты размерностей блок 35
            pxDataContainer -> m_uiDataIndex = DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_CREATE_DIMENTIONS_PARAMETERS_CREATE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DATA_BASE_CREATE_DIMENTIONS_PARAMETERS_CREATE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_DIMENTIONS_PARAMETERS_CREATE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(DATA_BASE_CREATE_TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_CREATE_TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START"  << std::endl;
        {
            AnalogoueInputModuleDiscreteSignalsTextTitlesCreate(m_puiIntermediateBuff);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
            // стартовые текстовые реквизиты дискретных сигналов блок 40
            pxDataContainer -> m_uiDataIndex = TEXT_TITLES_DATA_BASE_BLOCK_OFFSET;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_CREATE_TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DATA_BASE_CREATE_TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(DATA_BASE_CREATE_TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_CREATE_TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START"  << std::endl;
        {
            AnalogoueInputModuleAnalogoueSignalsTextTitlesCreate(m_puiIntermediateBuff);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
            // стартовые текстовые реквизиты аналоговых сигналов блок 41
            pxDataContainer -> m_uiDataIndex = (TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + 1);
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_CREATE_TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DATA_BASE_CREATE_TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(DATA_BASE_CREATE_CONFIGURATION_DATA_BASE_BLOCKS_WRITE_START);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_CREATE_CONFIGURATION_DATA_BASE_BLOCKS_WRITE_START:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_CONFIGURATION_DATA_BASE_BLOCKS_WRITE_START"  << std::endl;
        {
            CConfigurationCreate::ConfigurationToProgrammerFormat((CConfigurationCreate::TConfigDataProgrammerPackOne*)(m_puiIntermediateBuff),
                    (GetResources() -> GetDeviceConfigSearchPointer()));

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
            // конфигурация блок 100
            pxDataContainer -> m_uiDataIndex = CONFIGURATION_DATA_BASE_BLOCK_OFFSET;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_CREATE_CONFIGURATION_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DATA_BASE_CREATE_CONFIGURATION_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_CONFIGURATION_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(DATA_BASE_CREATE_NETWORK_ADDRESS_DATA_BASE_BLOCKS_WRITE_START);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_CREATE_NETWORK_ADDRESS_DATA_BASE_BLOCKS_WRITE_START:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_NETWORK_ADDRESS_DATA_BASE_BLOCKS_WRITE_START"  << std::endl;
        {
            memset(m_puiIntermediateBuff,
                   0,
                   CDataStore::MAX_BLOCK_LENGTH);
            // установим сетевой адрес по умолчанию 1 в целых числах.
            m_puiIntermediateBuff[0] = 0;

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
            // сетевой адрес блок 99
            pxDataContainer -> m_uiDataIndex = NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_CREATE_NETWORK_ADDRESS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DATA_BASE_CREATE_NETWORK_ADDRESS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_NETWORK_ADDRESS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(DATA_BASE_CREATE_MODULE_MUVR_DATA_BASE_READ_START);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_CREATE_MODULE_MUVR_DATA_BASE_READ_START:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_MODULE_MUVR_DATA_BASE_READ_START"  << std::endl;
        {
            m_uiInternalModuleMuvrId =
                GetResources() ->
                GetTaskIdByNameFromMap(m_sInternalModuleMuvrName);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiInternalModuleMuvrId;
            pxDataContainer -> m_uiFsmCommandState =
                CInternalModuleMuvr::MUVR_DATA_BASE_READ;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_CREATE_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DATA_BASE_CREATE_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(DATA_BASE_CREATE_MODULE_MUVR_DATA_BASE_BLOCKS_WRITE_START);
        }
        break;

    case DATA_BASE_CREATE_MODULE_MUVR_DATA_BASE_BLOCKS_WRITE_START:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_MODULE_MUVR_DATA_BASE_BLOCKS_WRITE_START"  << std::endl;
        {
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
            // модуль аналоговых сигналов мувр блок 1
            pxDataContainer -> m_uiDataIndex = ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_CREATE_MODULE_MUVR_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DATA_BASE_CREATE_MODULE_MUVR_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_MODULE_MUVR_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

//-------------------------------------------------------------------------------
    default:
        break;
    }
}

//-------------------------------------------------------------------------------

