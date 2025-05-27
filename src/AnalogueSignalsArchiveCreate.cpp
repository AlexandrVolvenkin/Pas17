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
#include <fstream>
#include <ctime>

#include "Task.h"
#include "Resources.h"
#include "Platform.h"
#include "DataStore.h"
#include "DataContainer.h"
#include "InternalModule.h"
#include "ConfigurationCreate.h"
#include "AnalogueSignalsArchiveCreate.h"

using namespace std;

//-------------------------------------------------------------------------------
CAnalogueSignalsArchiveCreate::CAnalogueSignalsArchiveCreate()
{
    std::cout << "CAnalogueSignalsArchiveCreate constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[256];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CAnalogueSignalsArchiveCreate::~CAnalogueSignalsArchiveCreate()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
void CAnalogueSignalsArchiveCreate::SetInternalModuleName(std::string sName)
{
    m_sInternalModuleName = sName;
}

//-------------------------------------------------------------------------------
uint8_t CAnalogueSignalsArchiveCreate::Init(void)
{
    std::cout << "CAnalogueSignalsArchiveCreate Init"  << std::endl;
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
    SetCustomerDataContainer(GetExecutorDataContainerPointer());

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

    Allocate();
}

//-------------------------------------------------------------------------------
void CAnalogueSignalsArchiveCreate::Allocate(void)
{
    std::cout << "CAnalogueSignalsArchiveCreate::Allocate 1"  << std::endl;

////    m_uiAddress = xMemoryAllocationContext.uiAddress;
////    m_puiRxBuffer = xMemoryAllocationContext.puiRxBuffer;
////    m_puiTxBuffer = xMemoryAllocationContext.puiTxBuffer;
////    m_puiErrorCode = xMemoryAllocationContext.puiErrorCode;
//
//    // Получим указатель на место в массиве дискретных входов для текущего модуля.
//    m_puiDiscreteInputsState =
//        &(GetResources() ->
//          m_puiDiscreteInputsState[GetResources() ->
//                                                  m_uiUsedDiscreteInputsState]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedDiscreteInputsState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//
//
//    // Получим указатель на место в массиве достоверности дискретных входов для текущего модуля.
//    m_puiDiscreteInputsBadState =
//        &(GetResources() ->
//          m_puiDiscreteInputsBadState[GetResources() ->
//                                                     m_uiUsedDiscreteInputsBadState]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedDiscreteInputsBadState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;


    // Получим указатель на место в массиве аналоговых входов для текущего модуля.
    m_pfAnalogueInputsValue =
        &(GetResources() ->
          m_pfAnalogueInputsValue[0]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedAnalogueInputsValue +=
//        MUVR_ANALOG_INPUT_QUANTITY;


//    // Получим указатель на место в массиве состояния аналоговых входов для текущего модуля.
//    m_puiAnalogueInputsState =
//        &(GetResources() ->
//          m_puiAnalogueInputsState[GetResources() ->
//                                                  m_uiUsedAnalogueInputsState]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedAnalogueInputsBadState +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // Получим указатель на место в массиве достоверности аналоговых входов для текущего модуля.
//    m_puiAnalogueInputsBadState =
//        &(GetResources() ->
//          m_puiAnalogueInputsBadState[GetResources() ->
//                                                     m_uiUsedAnalogueInputsBadState]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedAnalogueInputsBadState +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // Получим указатель на место в массиве состояний дискретных сигналов порождаемых аналоговыми входами.
//    m_puiAnalogueInputDiscreteInputsState =
//        &(GetResources() ->
//          m_puiAnalogueInputDiscreteInputsState[GetResources() ->
//                                 m_uiUsedAnalogueInputDiscreteInputsState]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedAnalogueInputDiscreteInputsState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//
//
//    // Получим указатель на место в массиве флагов недостоверности состояний дискретных сигналов порождаемых аналоговыми входами.
//    m_puiAnalogueInputDiscreteInputsBadState =
//        &(GetResources() ->
//          m_puiAnalogueInputDiscreteInputsBadState[GetResources() ->
//                                 m_uiUsedAnalogueInputDiscreteInputsBadState]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedAnalogueInputDiscreteInputsBadState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//
//
//    // Получим указатель на место в массиве отключения аналоговых входов для текущего модуля.
//    m_puiAnalogueInputsOff =
//        &(GetResources() ->
//          m_puiAnalogueInputsOff[GetResources() ->
//                                                m_uiUsedAnalogueInputsOff]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedAnalogueInputsOff +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // Получим указатель на место в массиве распакованной во внутренний формат базы данных
//    // аналоговых сигналов.
//    m_pxAnalogueInputDescriptionWork =
//        &(GetResources() ->
//          m_pxAnalogueInputDescriptionWork[GetResources() ->
//                                                          m_uiUsedAnalogueInputDescriptionWork]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedAnalogueInputDescriptionWork +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // Получим указатель на место в массиве аналоговых входов для текущего модуля.
//    m_puiReperPointsAdcBuffer =
//        &(GetResources() ->
//          m_puiReperPointsAdcBuffer[GetResources() ->
//                                                   m_uiUsedReperPointsAdcBuffer]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedReperPointsAdcBuffer +=
//        ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH;


//    // Получим указатель на место в рабочем массиве дискретных сигналов для текущего модуля.
//    m_pxDiscreteSignalsDescriptionWork =
//        &(GetResources() ->
//          m_pxDiscreteSignalsDescriptionWork[GetResources() ->
//                                 m_uiUsedDiscreteSignalsDescriptionWork]);
//    // Увеличим общий объём выделенной памяти.
//    GetResources() ->
//    m_uiUsedDiscreteSignalsDescriptionWork +=
//        ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH;
//
//    m_uiBadAnswerCounter = 0;
}

//-------------------------------------------------------------------------------
void CAnalogueSignalsArchiveCreate::CreateArchiveEntry(void)
{
//    std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 1"  << std::endl;

    const std::string dailyArchveFlashFile = "DailyArchve.dat";
    const std::string hourArchiveFramFile = "/dev/mtd0";

    struct Data
    {
        time_t currentTime; // Переменная для хранения текущего времени
        float fAin1;       // Переменная первого входа
        float fAin2;       // Переменная второго входа
        float fAin3;       // Переменная третьего входа
        float fAin4;       // Переменная четвертого входа
    };

    Data data;
    Data readData;

    // Получаем текущее время
    time_t now = time(nullptr);
    // Получаем текущую дату
    struct tm tstructCurrent = *gmtime(&now);

    // Заполняем переменные структуры данными
    data.currentTime = now;
    data.fAin1 = (float)(m_pfAnalogueInputsValue[0]); // Пример значения для fAin1
    data.fAin2 = (float)(m_pfAnalogueInputsValue[1]); // Пример значения для fAin2
    data.fAin3 = (float)(m_pfAnalogueInputsValue[2]); // Пример значения для fAin3
    data.fAin4 = (float)(m_pfAnalogueInputsValue[3]); // Пример значения для fAin4

    std::ifstream hourArchiveFramInputStream(hourArchiveFramFile, std::ios::binary | std::ios::in | std::ios::out);
    if (!hourArchiveFramInputStream.is_open())
    {
        std::cerr << "Failed to open for read /dev/mtd0" << std::endl;
        return;
    }
    std::ofstream hourArchiveFramOutputStream(hourArchiveFramFile, std::ios::binary | std::ios::in | std::ios::out);
    if (!hourArchiveFramOutputStream.is_open())
    {
        std::cerr << "Failed to open for write /dev/mtd0" << std::endl;
        hourArchiveFramInputStream.close();
        return;
    }

    // Открываем выходной файл для добавления данных
    std::ifstream dailyArchveFlashInputStream(dailyArchveFlashFile, std::ios::app | std::ios::in | std::ios::out);
    if (!dailyArchveFlashInputStream.is_open())
    {
        std::cerr << "Failed to open for read: " << dailyArchveFlashFile << std::endl;
        hourArchiveFramInputStream.close();
        hourArchiveFramOutputStream.close();
        return;
    }
    // Открываем выходной файл для добавления данных
    std::ofstream dailyArchveFlashOutputStream(dailyArchveFlashFile, std::ios::app | std::ios::in | std::ios::out);
    if (!dailyArchveFlashOutputStream.is_open())
    {
        std::cerr << "Failed to open for write: " << dailyArchveFlashFile << std::endl;
        hourArchiveFramInputStream.close();
        hourArchiveFramOutputStream.close();
        dailyArchveFlashInputStream.close();
        return;
    }

    // Если текущие часы отличаются от предыдущих,
    // значит, было наступление нового часа
    if (tstructCurrent.tm_min != m_iLastHour)
//    if ((tstructCurrent.tm_min != m_iLastHour) &&
//            (tstructCurrent.tm_sec == 0))
//    if (tstructCurrent.tm_hour != m_iLastHour)
    {
        std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 2"  << std::endl;
        // Обновляем значения для следующей проверки
        m_iLastHour = tstructCurrent.tm_min;
//        m_iLastHour = tstructCurrent.tm_hour;
        std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry tm_min "  << (float)tstructCurrent.tm_min << std::endl;

        std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 3"  << std::endl;
        // Получаем общую длину файла
        size_t fileSize = m_uiCurrentOffset;

        std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 4 fileSize "  << (float)fileSize << std::endl;
//        // файл пустой?
//        if (fileSize == 0)
//        {
//            std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 5"  << std::endl;
//            std::cerr << "Файл пустой." << std::endl;
//
//            std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 6"  << std::endl;
//            hourArchiveFramOutputStream.seekp(m_uiCurrentOffset, std::ios::beg);
//            hourArchiveFramOutputStream.write(reinterpret_cast<const char*>(&data), sizeof(Data));
//            m_uiCurrentOffset += sizeof(Data);
//            return;
//        }

        // Вычисляем количество структур Data в файле
        size_t numDataObjects = (fileSize / sizeof(Data));
        std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 7 numDataObjects "  << (float)numDataObjects << std::endl;

        // Считываем и преобразуем данные
        for (size_t i = 0; i < numDataObjects; i++)
        {
            hourArchiveFramInputStream.seekg((i * sizeof(Data)), std::ios::beg);
            hourArchiveFramInputStream.read(reinterpret_cast<char*>(&readData), sizeof(Data));

//            if (!hourArchiveFramInputStream.gcount())
//            {
//                std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 9"  << std::endl;
//                std::cerr << "Ошибка при чтении файла: не все байты прочитаны!" << std::endl;
//                break;
//            }

            // Получаем текущую дату
            struct tm tstructRead = *gmtime(&readData.currentTime);

            // Форматируем дату и время
            char dateStr[80];
            strftime(dateStr, sizeof(dateStr), "%d-%m-%Y", &tstructRead);

            char timeStr[80];
            strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &tstructRead);

            // Записываем данные в файл
            dailyArchveFlashOutputStream <<
                                         dateStr << ";" <<
                                         timeStr << ";" <<
                                         readData.fAin1 << ";" <<
                                         readData.fAin2 << ";" <<
                                         readData.fAin3 << ";" <<
                                         readData.fAin4 <<
                                         std::endl;
        }

        // каждый новый час начинаем запись в fram сначала.
        m_uiCurrentOffset = 0;

//        {
//            char timeStr[80];
//            strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &tstructCurrent);
//            std::cout << "Время 1 : " << timeStr << std::endl;
//        }

        // Записываем данные в файл /dev/mtd0
        hourArchiveFramOutputStream.seekp(m_uiCurrentOffset, std::ios::beg);
        hourArchiveFramOutputStream.write(reinterpret_cast<const char*>(&data), sizeof(Data));
//        usleep(10000);
//        hourArchiveFramOutputStream.flush();
        m_uiCurrentOffset += sizeof(Data);

        // Если текущие дни отличаются от предыдущих,
        // значит, было наступление новой сутки
//    if ((tstructCurrent.tm_min != m_iLastHour) &&
//            (tstructCurrent.tm_sec == 0))
        if (tstructCurrent.tm_mday != m_iLastDay)
        {
            std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 12"  << std::endl;
            // Обновляем значения для следующей проверки
            m_iLastDay = tstructCurrent.tm_mday;
        }
    }
    else
    {
//        std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 13"  << std::endl;

//        {
//            char timeStr[80];
//            strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &tstructCurrent);
//            std::cout << "Время 2 : " << timeStr << std::endl;
//        }

        // Записываем данные в файл /dev/mtd0
        hourArchiveFramOutputStream.seekp(m_uiCurrentOffset, std::ios::beg);
        hourArchiveFramOutputStream.write(reinterpret_cast<const char*>(&data), sizeof(Data));
//        usleep(10000);
//        hourArchiveFramOutputStream.flush();
        m_uiCurrentOffset += sizeof(Data);
    }

// Закрываем файл
    hourArchiveFramInputStream.close();
    hourArchiveFramOutputStream.close();
    dailyArchveFlashInputStream.close();
    dailyArchveFlashOutputStream.close();









//    // Записываем данные в файл /dev/mtd0
//    {
//////    std::ofstream output(hourArchiveFramFile, std::ios::binary | std::ios_base::app);
//        std::ofstream output(hourArchiveFramFile, std::ios::binary | std::ios::trunc);
//        if (!output.is_open())
//        {
//            std::cerr << "Failed to open /dev/mtd0" << std::endl;
//            return;
//        }
//        output.write(reinterpret_cast<const char*>(&data), sizeof(Data));
//        output.close(); // Закрываем файл после записи
//    }
//
//    // Чтение данных из /dev/mtd0 и сохранение их в DailyArchve.dat
//    {
//        std::ifstream input(hourArchiveFramFile, std::ios::binary);
//        if (!input.is_open())
//        {
//            std::cerr << "Failed to open /dev/mtd0" << std::endl;
//            return;
//        }
//        input.read(reinterpret_cast<char*>(&readData), sizeof(Data));
//        input.close(); // Закрываем файл после записи
//    }
//
//    std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry /dev/mtd0"  << std::endl;
//    // Выводим данные из /dev/mtd0 на экран
//    std::cout << "Read Time: " << asctime(localtime(&readData.currentTime)) << std::endl;
//    std::cout << "fAin1: " << readData.fAin1 << std::endl;
//    std::cout << "fAin2: " << readData.fAin2 << std::endl;
//    std::cout << "fAin3: " << readData.fAin3 << std::endl;
//    std::cout << "fAin4: " << readData.fAin4 << std::endl;
//
//
//    // Записываем данные в файл DailyArchve.dat
//    {
//        std::ofstream output(dailyArchveFlashFile, std::ios::binary | std::ios::trunc);
//        if (!output.is_open())
//        {
//            std::cerr << "Failed to open DailyArchve.dat" << std::endl;
//            return;
//        }
//        output.write(reinterpret_cast<const char*>(&readData), sizeof(Data));
//        output.close(); // Закрываем файл после записи
//    }
//
//    // Чтение данных из DailyArchve.dat
////    Data readData;
//    {
//        std::ifstream input(dailyArchveFlashFile, std::ios::binary);
//        if (!input.is_open())
//        {
//            std::cerr << "Failed to open DailyArchve.dat" << std::endl;
//            return;
//        }
//        input.read(reinterpret_cast<char*>(&readData), sizeof(Data));
//        input.close(); // Закрываем файл после записи
//    }
//
//
//    std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry DailyArchve.dat"  << std::endl;
//    // Выводим данные из DailyArchve.dat на экран
//    std::cout << "Read Time: " << asctime(localtime(&readData.currentTime)) << std::endl;
//    std::cout << "fAin1: " << readData.fAin1 << std::endl;
//    std::cout << "fAin2: " << readData.fAin2 << std::endl;
//    std::cout << "fAin3: " << readData.fAin3 << std::endl;
//    std::cout << "fAin4: " << readData.fAin4 << std::endl;















//    bool bIsFileExist = false;
//
////    // Получаем текущую дату
////    time_t now = time(0);
////    struct tm tstruct = *gmtime(&now);
////    struct tm tstruct = *gmtime(&readData.currentTime);
//
////    // Форматируем дату и время
////    char dateStr[80];
////    strftime(dateStr, sizeof(dateStr), "%d-%m-%Y", &tstruct);
////
////    char timeStr[80];
////    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &tstruct);
//
//    // Создаем имя файла с датой
//    std::string filename = "AnalogueMeasure_" + std::string(dateStr) + ".csv";
//
//    // Проверяем, существует ли файл
//    if (std::ifstream(filename))
//    {
////        std::cout << "Файл уже существует: " << filename << std::endl;
//        bIsFileExist = true;
//    }
//
//    // Открываем файл для добавления данных
//    std::ofstream output(filename, std::ios::app);
//
//    if (!output.is_open())
//    {
//        std::cerr << "Не удалось открыть файл!" << std::endl;
//        return;
//    }
//
////    std::cout << "CreateArchiveEntry filename "  << dateStr << std::endl;
//
//    if (!bIsFileExist)
//    {
//        // Записываем заголовок
////        output << "Дата;Время;AIn1;AIn2;AIn3;AIn4" << std::endl;
////        output << "   Дата   " << ";" << "   Время   " << ";" << "   AIn1   " << ";" << "   AIn2   " << ";" << "   AIn3   " << ";" << "   AIn4   " << std::endl;
//        output << "Дата;Время;AIn1;AIn2;AIn3;AIn4" << std::endl;
//    }
//
////    // Записываем данные в файл
////    output <<
////         dateStr << ";" <<
////         timeStr << ";" <<
////         (float)(m_pfAnalogueInputsValue[0]) << ";" <<
////         (float)(m_pfAnalogueInputsValue[1]) << ";" <<
////         (float)(m_pfAnalogueInputsValue[2]) << ";" <<
////         (float)(m_pfAnalogueInputsValue[3]) <<
////         std::endl;
//
//    // Записываем данные в файл
//    output <<
//         dateStr << ";" <<
//         timeStr << ";" <<
//         readData.fAin1 << ";" <<
//         readData.fAin2 << ";" <<
//         readData.fAin3 << ";" <<
//         readData.fAin4 <<
//         std::endl;
//
//    // Закрываем файл
//    output.close();
//
////    std::cout << "Данные успешно записаны в data.csv" << std::endl;

}

//-------------------------------------------------------------------------------
uint8_t CAnalogueSignalsArchiveCreate::Fsm(void)
{
//    std::cout << "CAnalogueSignalsArchiveCreate::Fsm 1" << endl;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CAnalogueSignalsArchiveCreate::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CAnalogueSignalsArchiveCreate::Fsm START"  << std::endl;
        Init();
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CAnalogueSignalsArchiveCreate::Fsm INIT 1"  << std::endl;
        m_uiInternalModuleId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleName);
        SetFsmState(READY);

        break;

    case READY:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm READY"  << std::endl;
        break;

    case DONE_OK:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START:
        std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else
        {
            std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
        std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
            std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
        std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
        }

        break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(GetFsmNextStateDoneWaitingDoneError());
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CAnalogueSignalsArchiveCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateDoneWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case ANALOGUE_SIGNALS_ARCHIVE_CREATE_START:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm ANALOGUE_SIGNALS_ARCHIVE_CREATE_START"  << std::endl;
    {
        CreateArchiveEntry();
        SetFsmState(ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
    }
    break;

    case ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
//        std::cout << "CAnalogueSignalsArchiveCreate::Fsm ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    default:
        break;
    }
}

//-------------------------------------------------------------------------------

