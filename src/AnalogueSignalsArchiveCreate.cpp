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
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

#include "Task.h"
#include "Resources.h"
#include "Platform.h"
#include "DataStore.h"
#include "DataContainer.h"
#include "InternalModule.h"
#include "ConfigurationCreate.h"
#include "StorageDevice.h"
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

//    m_puiHoldingRegisters = m_pxResources -> GetInputRegisters();
    m_puiHoldingRegisters = m_pxResources -> GetHoldingRegisters();
    m_puiInputRegisters = m_pxResources -> GetInputRegisters();


    // Получим указатель на буфер с серийным номером и идентификатором прибора.
    m_puiSerialAndId =
        (GetResources() -> m_puiSerialAndId);

//    m_uiBadAnswerCounter = 0;
}

//-------------------------------------------------------------------------------
void CAnalogueSignalsArchiveCreate::CreateArchiveEntry(void)
{
//    std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 1"  << std::endl;
//
//    struct TAnalogueSignalsArchiveHourData
//    {
//        time_t currentTime; // Переменная для хранения текущего времени
//        float fAin1;       // Переменная первого входа
//        float fAin2;       // Переменная второго входа
//        float fAin3;       // Переменная третьего входа
//        float fAin4;       // Переменная четвертого входа
//    };

    // Получаем текущее время
    time_t now = time(nullptr);
    // Получаем текущую дату
    struct tm tstructCurrent = *gmtime(&now);


//-------------------------------------------------------------------------------
// обновление текущего времени в массивах модбас.

    unsigned short *pusDestination;
    // получим указатель на буфер с текущим временем в рабочем массиве прибора.
    pusDestination =
        (unsigned short*)&m_puiInputRegisters[CURRENT_TIME_OFFSET_INPUT_REGISTERS];

    pusDestination[CURRENT_TIME_SECOND_OFFSET] = tstructCurrent.tm_sec;
    pusDestination[CURRENT_TIME_MINUTE_OFFSET] = tstructCurrent.tm_min;
    pusDestination[CURRENT_TIME_HOUR_OFFSET] = tstructCurrent.tm_hour;
    pusDestination[CURRENT_TIME_MONTH_DAY_OFFSET] = tstructCurrent.tm_mday;
    pusDestination[CURRENT_TIME_MONTH_OFFSET] = (tstructCurrent.tm_mon) + 1;
    pusDestination[CURRENT_TIME_YEAR_OFFSET] = (tstructCurrent.tm_year) - 100;

    // сегодня воскресение?
    if ((tstructCurrent.tm_wday) == LINUX_WEEK_DAY_SUNDAY)
    {
        pusDestination[CURRENT_TIME_WEEK_DAY_OFFSET] = WEEK_DAY_SUNDAY;
    }
    else
    {
        pusDestination[CURRENT_TIME_WEEK_DAY_OFFSET] = tstructCurrent.tm_wday;
    }

////    std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 11"  << std::endl;
//    pusDestination =
//        (unsigned short*)&m_puiHoldingRegisters[CURRENT_TIME_OFFSET_HOLDING_REGISTERS];
//
//    pusDestination[CURRENT_TIME_SECOND_OFFSET] = tstructCurrent.tm_sec;
//    pusDestination[CURRENT_TIME_MINUTE_OFFSET] = tstructCurrent.tm_min;
//    pusDestination[CURRENT_TIME_HOUR_OFFSET] = tstructCurrent.tm_hour;
//    pusDestination[CURRENT_TIME_MONTH_DAY_OFFSET] = tstructCurrent.tm_mday;
//    pusDestination[CURRENT_TIME_MONTH_OFFSET] = (tstructCurrent.tm_mon) + 1;
//    pusDestination[CURRENT_TIME_YEAR_OFFSET] = (tstructCurrent.tm_year) - 100;
//
//    // сегодня воскресение?
//    if ((tstructCurrent.tm_wday) == LINUX_WEEK_DAY_SUNDAY)
//    {
//        pusDestination[CURRENT_TIME_WEEK_DAY_OFFSET] = WEEK_DAY_SUNDAY;
//    }
//    else
//    {
//        pusDestination[CURRENT_TIME_WEEK_DAY_OFFSET] = tstructCurrent.tm_wday;
//    }

//    std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 111"  << std::endl;
//    memcpy(&m_puiHoldingRegisters[CURRENT_TIME_OFFSET_HOLDING_REGISTERS],
//           pusDestination,
//           (CURRENT_TIME_BYTE_QUANTITY * sizeof(short)));

//        return;

////    // прошла минута?
////    if (ui8CurrentTimeSaveDelayCounter != tstructCurrent.tm_min)
////    {
////        ui8CurrentTimeSaveDelayCounter = tstructCurrent.tm_min;
////        // сохраним текущее время в FRAM.
////        iFramWrite(FRAM_LAST_SAVED_TIME_OFFSET,
////                   (uint8_t*)&xCurrentTime,
////                   sizeof(xCurrentTime));
////    }
//-------------------------------------------------------------------------------


    // архивная запись создаётся раз в секунду. этот метод вызывается с удвоенной частотой,
    // чтобы не нарушить синхронизацию времени главного цикла и текущего времени, раз в 5 главных
    // циклов по 100mc.
    // не наступила новая секунда?
    if (tstructCurrent.tm_sec == m_iLastSecond)
    {
        return;
    }
    else
    {
        m_iLastSecond = tstructCurrent.tm_sec;
    }

    TAnalogueSignalsArchiveHourData data;

    // Заполняем переменные структуры данными
    data.currentTime = now;
    data.fAin1 = (float)(m_pfAnalogueInputsValue[0]); // Пример значения для fAin1
    data.fAin2 = (float)(m_pfAnalogueInputsValue[1]); // Пример значения для fAin2
    data.fAin3 = (float)(m_pfAnalogueInputsValue[2]); // Пример значения для fAin3
    data.fAin4 = (float)(m_pfAnalogueInputsValue[3]); // Пример значения для fAin4

    // при первом вызове функции ещё нет файла архива текущих суток. его нужно создать
    // нет предыдущих значений переменных m_iLastHour и m_iLastDay. их нужно получить.
    // это первый вызов функции?
    if (m_bIsStartState)
    {
        std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 2"  << std::endl;
        m_bIsStartState = false;

        // Обновляем значения для следующей проверки
//        m_iLastHour = tstructCurrent.tm_min;
        m_iLastHour = tstructCurrent.tm_hour;

        // Обновляем значения для следующей проверки
////        m_iLastDay = tstructCurrent.tm_hour;
        m_iLastDay = tstructCurrent.tm_mday;


        // каждый новый час начинаем запись в fram сначала.
        m_uiCurrentOffset = 0;

        // блок записи новых ежесекундных данных в fram.
        {
            // имя устройства fram памяти.
            const std::string hourArchiveFramFile = "/dev/mtd0";

            std::ofstream hourArchiveFramOutputStream(hourArchiveFramFile, std::ios::binary | std::ios::in | std::ios::out);
            if (!hourArchiveFramOutputStream.is_open())
            {
                std::cerr << "Failed to open for write /dev/mtd0" << std::endl;
                return;
            }

            // Записываем данные в файл fram
            // установим указатель на данные новой ежесекундной записи.
            hourArchiveFramOutputStream.seekp(m_uiCurrentOffset, std::ios::beg);
            hourArchiveFramOutputStream.write(reinterpret_cast<const char*>(&data), sizeof(TAnalogueSignalsArchiveHourData));
            // Закрываем файл
            hourArchiveFramOutputStream.close();
            m_uiCurrentOffset += sizeof(TAnalogueSignalsArchiveHourData);

//            // Записываем данные в файл fram
//            CStorageDeviceSpiFram::Write((uint8_t*)(&data),
//                                         m_uiCurrentOffset,
//                                         sizeof(TAnalogueSignalsArchiveHourData));
//            m_uiCurrentOffset += sizeof(TAnalogueSignalsArchiveHourData);
        }

        // блок создания нового файла суточного архива.
        {
            // создадим файл нового суточного архива.
            // Форматируем дату
            char dateStr[80];
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tstructCurrent);

            // Получаем текущий год и месяц
            int year = tstructCurrent.tm_year + 1900;
            int month = tstructCurrent.tm_mon + 1;
            int hour = tstructCurrent.tm_hour;
            int minute = tstructCurrent.tm_min;

            std::string cSerialAndIdStr;
            // Копируем данные из m_puiSerialAndId в cSerialAndIdStr
            cSerialAndIdStr.assign((const char*)m_puiSerialAndId, SERIAL_AND_ID_DATA_BASE_BLOCK_LENGTH);
            //    // Создаем пути к папкам и файлу
            //    std::string pathToYearFolder = "AnalogueMeasureArchives_" + std::to_string(year);
            //    std::string pathToMonthFolder = pathToYearFolder + "/" + std::to_string(month);
            //    std::string dailyArchveFlashFile = pathToMonthFolder + "/AnalogueMeasure_" + dateStr + ".csv";

            // Создаем пути к папкам и файлу
            std::string pathToYearFolder = "/home/debian/AnalogueMeasureArchives_" + cSerialAndIdStr + "_" + std::to_string(year);
            std::string pathToMonthFolder = pathToYearFolder + "/" + std::to_string(month);
            std::string dailyArchveFlashFile = pathToMonthFolder + "/AnalogueMeasure_" + dateStr + ".csv";
//            std::string dailyArchveFlashFile = pathToMonthFolder + "/AnalogueMeasure_" + dateStr + "-" + std::to_string(hour) + ".csv";
            //        std::string dailyArchveFlashFile = pathToMonthFolder + "/AnalogueMeasure_" + dateStr + "-" + std::to_string(m_iFileNumberCounter) + ".csv";

            // Проверка и создание директорий (используем POSIX функции)
            if (mkdir(pathToYearFolder.c_str(), 0755) == -1)
            {
                perror("Error creating Year Folder");
            }

            if (mkdir(pathToMonthFolder.c_str(), 0755) == -1)
            {
                perror("Error creating Month Folder");
            }

            // запомним имя файла текущего суточного архива. при наступлении новых суток создадим
            // сжатый файл архива прошедших с этим именем.
            m_sCurrentDailyArchveFlashFile = dailyArchveFlashFile;
        }

        // блок создания заголовка в файле текущего суточного архива.
        {
            // Открываем выходной файл для добавления данных
            std::ofstream dailyArchveFlashOutputStream(m_sCurrentDailyArchveFlashFile, std::ios::app | std::ios::in | std::ios::out);
            if (!dailyArchveFlashOutputStream.is_open())
            {
                std::cerr << "Failed to open for write: " << m_sCurrentDailyArchveFlashFile << std::endl;
            }

            // Записываем заголовок
            dailyArchveFlashOutputStream << "Дата;Время;AIn1;AIn2;AIn3;AIn4" << std::endl;
            // Закрываем файл
            dailyArchveFlashOutputStream.close();
        }
    }
    else
    {
//        std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 3"  << std::endl;
        // Если текущий час отличаются от предыдущего,
        // значит, наступил новый час.
        // наступил новый час?
//        if (tstructCurrent.tm_min != m_iLastHour)
        if (tstructCurrent.tm_hour != m_iLastHour)
        {
            std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 4"  << std::endl;
            std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry tm_min "  << (float)tstructCurrent.tm_min << std::endl;
            // Обновляем значения для следующей проверки
//            m_iLastHour = tstructCurrent.tm_min;
            m_iLastHour = tstructCurrent.tm_hour;

            // блок считывания данных из fram и записи в файл текущего суточного архива.
            {
                // имя устройства fram памяти.
                const std::string hourArchiveFramFile = "/dev/mtd0";

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
                std::ofstream dailyArchveFlashOutputStream(m_sCurrentDailyArchveFlashFile, std::ios::app | std::ios::in | std::ios::out);
                if (!dailyArchveFlashOutputStream.is_open())
                {
                    std::cerr << "Failed to open for write: " << m_sCurrentDailyArchveFlashFile << std::endl;
                    hourArchiveFramInputStream.close();
                    hourArchiveFramOutputStream.close();
                    return;
                }

                // Получаем общую длину файла
                size_t fileSize = m_uiCurrentOffset;
                std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry m_uiCurrentOffset "  << (float)m_uiCurrentOffset << std::endl;

                // Вычисляем количество структур TAnalogueSignalsArchiveHourData в файле
                size_t numDataObjects = (fileSize / sizeof(TAnalogueSignalsArchiveHourData));
                std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry numDataObjects "  << (float)numDataObjects << std::endl;

                // Считываем и преобразуем данные из fram во флеш.
                for (size_t i = 0; i < numDataObjects; i++)
                {
                    TAnalogueSignalsArchiveHourData readData;
                    // установим указатель на данные ежесекундной записи.
                    hourArchiveFramInputStream.seekg((i * sizeof(TAnalogueSignalsArchiveHourData)), std::ios::beg);
                    hourArchiveFramInputStream.read(reinterpret_cast<char*>(&readData), sizeof(TAnalogueSignalsArchiveHourData));

                    // больше нет данных для чтения?
                    if (!hourArchiveFramInputStream.gcount())
                    {
                        std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 5"  << std::endl;
                        break;
                    }

                    // Получаем дату из предыдущих сохранённых данных.
                    struct tm tstructRead = *gmtime(&readData.currentTime);

                    // Форматируем дату и время
                    char dateStr[80];
                    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tstructRead);

                    char timeStr[80];
                    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &tstructRead);

                    // Записываем данные в файл текущего суточного архива.
                    dailyArchveFlashOutputStream <<
                                                 dateStr << ";" <<
                                                 timeStr << ";" <<
                                                 readData.fAin1 << ";" <<
                                                 readData.fAin2 << ";" <<
                                                 readData.fAin3 << ";" <<
                                                 readData.fAin4 <<
                                                 std::endl;
                }

                // Закрываем файл
                hourArchiveFramInputStream.close();
                hourArchiveFramOutputStream.close();
                dailyArchveFlashOutputStream.close();
            }

            // каждый новый час начинаем запись в fram сначала.
            m_uiCurrentOffset = 0;

            // блок записи новых ежесекундных данных в fram.
            {
                // имя устройства fram памяти.
                const std::string hourArchiveFramFile = "/dev/mtd0";

                std::ofstream hourArchiveFramOutputStream(hourArchiveFramFile, std::ios::binary | std::ios::in | std::ios::out);
                if (!hourArchiveFramOutputStream.is_open())
                {
                    std::cerr << "Failed to open for write /dev/mtd0" << std::endl;
                    return;
                }

                // Записываем данные в файл fram
                // установим указатель на данные новой ежесекундной записи.
                hourArchiveFramOutputStream.seekp(m_uiCurrentOffset, std::ios::beg);
                hourArchiveFramOutputStream.write(reinterpret_cast<const char*>(&data), sizeof(TAnalogueSignalsArchiveHourData));
                // Закрываем файл
                hourArchiveFramOutputStream.close();
                m_uiCurrentOffset += sizeof(TAnalogueSignalsArchiveHourData);

//                // Записываем данные в файл fram
//                CStorageDeviceSpiFram::Write((uint8_t*)(&data),
//                                             m_uiCurrentOffset,
//                                             sizeof(TAnalogueSignalsArchiveHourData));
//                m_uiCurrentOffset += sizeof(TAnalogueSignalsArchiveHourData);
            }


            // Если текущий день отличаются от предыдущего,
            // значит, наступили новые сутки.
            // наступили новые сутки?
////            if (tstructCurrent.tm_hour != m_iLastDay)
            if (tstructCurrent.tm_mday != m_iLastDay)
            {
                std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 6"  << std::endl;
                std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry tm_min "  << (float)tstructCurrent.tm_min << std::endl;
                std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry tm_hour "  << (float)tstructCurrent.tm_hour << std::endl;

                // Обновляем значения для следующей проверки
////                m_iLastDay = tstructCurrent.tm_hour;
                m_iLastDay = tstructCurrent.tm_mday;

                // блок сжатия файла текущего суточного архива.
                {
                    // сожмём архив предыдущих суток.
                    {
                        // Команда для gzip
                        std::string command = "gzip " + m_sCurrentDailyArchveFlashFile;

                        // Выполнение команды
                        int result = system(command.c_str());

                        if (result == 0)
                        {
                            std::cout << "Файл успешно сжат и сохранен." << std::endl;
                        }
                        else
                        {
                            std::cerr << "Ошибка при выполнении команды." << std::endl;
                        }
                    }

                    {
                        // удалим несжатый файл архива предыдущих суток.
                        // Команда для rm
                        std::string command = "sudo rm -f -R " + m_sCurrentDailyArchveFlashFile;

                        // Выполнение команды
                        int result = system(command.c_str());

                        if (result == 0)
                        {
                            std::cout << "Файл успешно удалён. " << m_sCurrentDailyArchveFlashFile << std::endl;
                        }
                        else
                        {
                            std::cerr << "Ошибка при удалении файла. " << m_sCurrentDailyArchveFlashFile << std::endl;
                        }
                    }
                }

                // блок создания нового файла суточного архива.
                {
                    // создадим новый файл суточного архива.
                    // Форматируем дату
                    char dateStr[80];
                    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tstructCurrent);

                    // Получаем текущий год и месяц
                    int year = tstructCurrent.tm_year + 1900;
                    int month = tstructCurrent.tm_mon + 1;
                    int hour = tstructCurrent.tm_hour;
                    int minute = tstructCurrent.tm_min;

                    // Получаем серийный номер и идентификатор
                    std::string cSerialAndIdStr;
                    // Копируем данные из m_puiSerialAndId в cSerialAndIdStr
                    cSerialAndIdStr.assign((const char*)m_puiSerialAndId, SERIAL_AND_ID_DATA_BASE_BLOCK_LENGTH);

                    //    // Создаем пути к папкам и файлу
                    //    std::string pathToYearFolder = "AnalogueMeasureArchives_" + std::to_string(year);
                    //    std::string pathToMonthFolder = pathToYearFolder + "/" + std::to_string(month);
                    //    std::string dailyArchveFlashFile = pathToMonthFolder + "/AnalogueMeasure_" + dateStr + ".csv";

                    // Создаем пути к папкам и файлу
                    std::string pathToYearFolder = "/home/debian/AnalogueMeasureArchives_" + cSerialAndIdStr + "_" + std::to_string(year);
                    std::string pathToMonthFolder = pathToYearFolder + "/" + std::to_string(month);
                    std::string dailyArchveFlashFile = pathToMonthFolder + "/AnalogueMeasure_" + dateStr + "-" + std::to_string(hour) + ".csv";
                    //        std::string dailyArchveFlashFile = pathToMonthFolder + "/AnalogueMeasure_" + dateStr + "-" + std::to_string(m_iFileNumberCounter) + ".csv";

                    // Проверка и создание директорий (используем POSIX функции)
                    if (mkdir(pathToYearFolder.c_str(), 0755) == -1)
                    {
                        perror("Error creating Year Folder");
                    }

                    if (mkdir(pathToMonthFolder.c_str(), 0755) == -1)
                    {
                        perror("Error creating Month Folder");
                    }

                    // запомним имя файла текущего суточного архива. при наступлении новых суток создадим
                    // сжатый файл архива прошедших с этим именем.
                    m_sCurrentDailyArchveFlashFile = dailyArchveFlashFile;
                }

                // блок создания заголовка в файле текущего суточного архива.
                {
                    // Открываем выходной файл для добавления данных
                    std::ofstream dailyArchveFlashOutputStream(m_sCurrentDailyArchveFlashFile, std::ios::app | std::ios::in | std::ios::out);
                    if (!dailyArchveFlashOutputStream.is_open())
                    {
                        std::cerr << "Failed to open for write: " << m_sCurrentDailyArchveFlashFile << std::endl;
                    }

                    // Записываем заголовок
                    //        output << "Дата;Время;AIn1;AIn2;AIn3;AIn4" << std::endl;
                    //        output << "   Дата   " << ";" << "   Время   " << ";" << "   AIn1   " << ";" << "   AIn2   " << ";" << "   AIn3   " << ";" << "   AIn4   " << std::endl;
                    dailyArchveFlashOutputStream << "Дата;Время;AIn1;AIn2;AIn3;AIn4" << std::endl;
                    // Закрываем файл
                    dailyArchveFlashOutputStream.close();
                }
            }
        }
        else
        {
//        std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 10"  << std::endl;
            // блок записи новых ежесекундных данных в fram.
            {
                const std::string hourArchiveFramFile = "/dev/mtd0";

                std::ofstream hourArchiveFramOutputStream(hourArchiveFramFile, std::ios::binary | std::ios::in | std::ios::out);
                if (!hourArchiveFramOutputStream.is_open())
                {
                    std::cerr << "Failed to open for write /dev/mtd0" << std::endl;
                    return;
                }

                // Записываем данные в файл fram
                // установим указатель на данные новой ежесекундной записи.
                hourArchiveFramOutputStream.seekp(m_uiCurrentOffset, std::ios::beg);
                hourArchiveFramOutputStream.write(reinterpret_cast<const char*>(&data), sizeof(TAnalogueSignalsArchiveHourData));
                // Закрываем файл
                hourArchiveFramOutputStream.close();
                m_uiCurrentOffset += sizeof(TAnalogueSignalsArchiveHourData);


//                // Записываем данные в файл fram
//                CStorageDeviceSpiFram::Write((uint8_t*)(&data),
//                                             m_uiCurrentOffset,
//                                             sizeof(TAnalogueSignalsArchiveHourData));
//                m_uiCurrentOffset += sizeof(TAnalogueSignalsArchiveHourData);
            }
        }
    }
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
    case ANALOGUE_SIGNALS_SERIAL_AND_ID_LOAD_START:
        std::cout << "CAnalogueSignalsArchiveCreate::Fsm ANALOGUE_SIGNALS_SERIAL_AND_ID_LOAD_START"  << std::endl;
        {
            SetFsmState(ANALOGUE_SIGNALS_SERIAL_AND_ID_LOAD_DATA_BASE_BLOCKS_READ_START);
        }
        break;

    case ANALOGUE_SIGNALS_SERIAL_AND_ID_LOAD_DATA_BASE_BLOCKS_READ_START:
        std::cout << "CAnalogueSignalsArchiveCreate::Fsm ANALOGUE_SIGNALS_SERIAL_AND_ID_LOAD_DATA_BASE_BLOCKS_READ_START"  << std::endl;
        {
//            m_uiDataStoreId =
//                GetResources() ->
//                GetTaskIdByNameFromMap(m_sDataStoreName);
            uint8_t uiDataStoreId =
                GetResources() ->
                GetTaskIdByNameFromMap("DataStoreFileSystem");

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::READ_BLOCK_DATA_START;
            // серийный номер и технологическая позиция блок 97
            pxDataContainer -> m_uiDataIndex = SERIAL_AND_ID_DATA_BASE_BLOCK_OFFSET;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(ANALOGUE_SIGNALS_SERIAL_AND_ID_LOAD_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(ANALOGUE_SIGNALS_SERIAL_AND_ID_LOAD_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(ANALOGUE_SIGNALS_SERIAL_AND_ID_LOAD_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(ANALOGUE_SIGNALS_SERIAL_AND_ID_LOAD_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case ANALOGUE_SIGNALS_SERIAL_AND_ID_LOAD_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CAnalogueSignalsArchiveCreate::Fsm ANALOGUE_SIGNALS_SERIAL_AND_ID_LOAD_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(ANALOGUE_SIGNALS_SERIAL_AND_ID_SET_START);
        }
        break;

    case ANALOGUE_SIGNALS_SERIAL_AND_ID_LOAD_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CAnalogueSignalsArchiveCreate::Fsm ANALOGUE_SIGNALS_SERIAL_AND_ID_LOAD_DATA_BASE_BLOCKS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

    case ANALOGUE_SIGNALS_SERIAL_AND_ID_SET_START:
        std::cout << "CAnalogueSignalsArchiveCreate::Fsm ANALOGUE_SIGNALS_SERIAL_AND_ID_SET_START"  << std::endl;
        {
            CDataContainerDataBase* pxExecutorDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();

            memcpy(m_puiSerialAndId,
                   (pxExecutorDataContainer -> m_puiDataPointer),
                   pxExecutorDataContainer -> m_uiDataLength);

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
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
        std::cout << "CAnalogueSignalsArchiveCreate::Fsm ANALOGUE_SIGNALS_ARCHIVE_CREATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
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

