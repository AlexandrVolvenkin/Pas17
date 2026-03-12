//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <string>
//#include <filesystem>

#include <iostream>
#include <dirent.h>
#include <string>
#include <algorithm>
#include <memory>
#include <thread>
#include <sys/stat.h>
//#include <sys/mount.h>

#include "Timer.h"
#include "Task.h"
#include "Platform.h"
#include "Resources.h"
#include "DataStore.h"
#include "DataContainer.h"
#include "Link.h"
#include "AnalogueSignals.h"
#include "ConfigurationCreate.h"
#include "InternalModuleMuvr.h"
#include "ModbusSlave.h"
#include "Parse.h"
#include "SettingsLoad.h"
#include "SettingsSet.h"
#include "DeviceControl.h"

using namespace std;

//-------------------------------------------------------------------------------
CDeviceControl::CDeviceControl()
{
    //std::cout << "CDeviceControl constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[CDataStore::MAX_BLOCK_LENGTH];
    xFileSaveStateDataPackOne.uiFileSaveState = WRITE_IDDLE;
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CDeviceControl::~CDeviceControl()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
uint8_t CDeviceControl::Init(void)
{
    std::cout << "CDeviceControl Init"  << std::endl;
//    m_pxCommandDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
//                               AddDataContainer(std::make_shared<CDataContainerDataBase>()));
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));

    SetCustomerDataContainer(GetExecutorDataContainerPointer());

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
    pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

    Allocate();
}

//-------------------------------------------------------------------------------
void CDeviceControl::Allocate(void)
{
    //std::cout << "CDeviceControl::Allocate 1"  << std::endl;

    pxCurrentTime = &(GetResources() -> xCurrentTime);

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


//    // Получим указатель на место в массиве аналоговых входов для текущего модуля.
//    m_pfAnalogueInputsValue =
//        &(GetResources() ->
//          m_pfAnalogueInputsValue[0]);
////    // Увеличим общий объём выделенной памяти.
////    GetResources() ->
////    m_uiUsedAnalogueInputsValue +=
////        MUVR_ANALOG_INPUT_QUANTITY;


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

////    m_puiHoldingRegisters = m_pxResources -> GetInputRegisters();
//    m_puiHoldingRegisters = m_pxResources -> GetHoldingRegisters();
//    m_puiInputRegisters = m_pxResources -> GetInputRegisters();
//
//
//    // Получим указатель на буфер с серийным номером и идентификатором прибора.
//    m_puiSerialAndId =
//        (GetResources() -> m_puiSerialAndId);


    m_puiHoldingRegisters = m_pxResources -> GetHoldingRegisters();
    m_puiInputRegisters = m_pxResources -> GetInputRegisters();


    // Получим указатель на буфер с серийным номером и идентификатором прибора.
    m_puiSerialAndId =
        (GetResources() -> m_puiSerialAndId);

//    m_uiBadAnswerCounter = 0;
}

//-------------------------------------------------------------------------------
void CDeviceControl::SetInternalModuleMuvrName(std::string sName)
{
    m_sInternalModuleMuvrName = sName;
}

//-----------------------------------------------------------------------------------------------------
// устанавливает системное время Linux.
// Байт 1 - секунда 0 – 59;
// Байт 2 - минута 0 –59;
// Байт 3 - час 0 – 23;
// Байт 4 - день 1 – 31;
// Байт 5 - месяц 1 – 12.
// Байт 6 - год 0 – 99.
// Байт 7 – день недели 1 – 7.
void CDeviceControl::LinuxCurrentTimeSet(unsigned char *pucSource)
{
//    std::cout << "CDeviceControl LinuxCurrentTimeSet 1"  << std::endl;

    time_t rawtime;
    struct tm *timeinfo;
    struct timeval systime;

    rawtime = time(NULL);
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    timeinfo -> tm_sec = pucSource[CURRENT_TIME_SECOND_OFFSET];
    timeinfo -> tm_min = pucSource[CURRENT_TIME_MINUTE_OFFSET];
    timeinfo -> tm_hour = pucSource[CURRENT_TIME_HOUR_OFFSET];
    timeinfo -> tm_mday = pucSource[CURRENT_TIME_MONTH_DAY_OFFSET];
    (timeinfo -> tm_mon) = pucSource[CURRENT_TIME_MONTH_OFFSET] - 1;
    (timeinfo -> tm_year) = pucSource[CURRENT_TIME_YEAR_OFFSET] + 100;
    timeinfo -> tm_wday = pucSource[CURRENT_TIME_WEEK_DAY_OFFSET];

    systime =
    {
        mktime(timeinfo),
        0
    };
    settimeofday(&systime, NULL);
    system("sudo hwclock --systohc");
//    system("sudo hwclock -w");
}

//-----------------------------------------------------------------------------------------------------
// обрабатывает входящие сообщения от Modbus интерфейсов по 70 функции - УСТАНОВКА ТЕКУЩЕГО ВРЕМЕНИ ВО ВСЕ ПРИБОРЫ СЕТИ .
// источник - ПАС-05_И3_10 2013.pdf.
// 4.3.11 ФУНКЦИЯ 70: УСТАНОВКА ТЕКУЩЕГО ВРЕМЕНИ ВО ВСЕ ПРИБОРЫ СЕТИ
// Запрос
// Широковещательный запрос (адрес=0), адресован всем SL сети. Передается 7
// байтов.
// Байт 1 - секунда 0 – 59;
// Байт 2 - минута 0 –59;
// Байт 3 - час 0 – 23;
// Байт 4 - день 1 – 31;
// Байт 5 - месяц 1 – 12.
// Байт 6 - год 0 – 99.
// Байт 7 – день недели 1 – 7.
// Ответ
// На данный запрос SL не отвечает.
void CDeviceControl::CurrentTimeSet(void)
{
//    std::cout << "CDeviceControl CurrentTimeSet 1"  << std::endl;

    CDataContainerDataBase* pxDataContainer =
        (CDataContainerDataBase*)GetCustomerDataContainerPointer();

    LinuxCurrentTimeSet(pxDataContainer -> m_puiDataPointer);

    (pxDataContainer -> m_uiDataLength) =
        CURRENT_TIME_BYTE_QUANTITY;
}

//-----------------------------------------------------------------------------------------------------
void CDeviceControl::CurrentTimeUpdate(void)
{
//    //std::cout << "CDeviceControl CurrentTimeUpdate 1"  << std::endl;

    // Получаем текущее время
    time_t now = time(nullptr);
    // Получаем текущую дату
    struct tm tstructCurrent = *gmtime(&now);
    pxCurrentTime = &tstructCurrent;

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
////    if (uiCurrentTimeSaveDelayCounter != tstructCurrent.tm_min)
////    {
////        uiCurrentTimeSaveDelayCounter = tstructCurrent.tm_min;
////        // сохраним текущее время в FRAM.
////        iFramWrite(FRAM_LAST_SAVED_TIME_OFFSET,
////                   (uint8_t*)&xCurrentTime,
////                   sizeof(xCurrentTime));
////    }
}

//-----------------------------------------------------------------------------------------------------
std::vector<std::string> listFilesByPrefix(const std::string& prefix)
{
    std::vector<std::string> files;
    DIR* dir = opendir("/home/debian");

    if (dir != nullptr)
    {
        struct dirent* entry;

        while ((entry = readdir(dir)) != nullptr)
        {
            // Игнорируем текущий и предыдущий каталоги
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            std::string fileName(entry->d_name);
            // Проверяем, начинается ли имя файла с заданного префикса
            if (fileName.compare(0, prefix.length(), prefix) == 0)
            {
                files.push_back(fileName);
            }
        }

        closedir(dir);
    }
    else
    {
        std::cerr << "Ошибка при открытии каталога" << std::endl;
    }

    return files;
}

//-----------------------------------------------------------------------------------------------------
void CDeviceControl::AnalogueMeasureArchiveWrite(void)
{
    //std::cout << "CDeviceControl AnalogueMeasureArchiveWrite 1"  << std::endl;

    char acCommand[128];

    xFileSaveStateDataPackOne.uiFileSaveState = WRITE_IDDLE;

    CParse xCArchiveSaveParse;
    xCArchiveSaveParse.GetDiskInfoNew();
    for (uint8_t i; i < 4; i++)
    {
        //std::cout << "CDeviceControl AnalogueMeasureArchiveWrite acName " <<
//                  (xCArchiveSaveParse.axTDiskInfo[i].acName) << std::endl;
    }

    std::string sDiskName;
    sDiskName = xCArchiveSaveParse.axTDiskInfo[1].acName;
    //std::cout << "CDeviceControl AnalogueMeasureArchiveWrite sDiskName " <<
//              (sDiskName) << std::endl;
// Начнем поиск имени, начиная с "sd" в конце строки sDiskName
    size_t found = sDiskName.rfind("sd");
    if (found != std::string::npos)
    {
        // Скопируем часть строки начиная с найденной позиции
        sDiskName = sDiskName.substr(found);
        sDiskName[4] = '\0';  // Добавляем завершающий ноль
        //std::cout << "CDeviceControl AnalogueMeasureArchiveWrite sDiskName " <<
//                  (sDiskName) << std::endl;

        std::string cSerialAndIdStr;
        // Копируем данные из m_puiSerialAndId в cSerialAndIdStr
        cSerialAndIdStr.assign((const char*)m_puiSerialAndId, SERIAL_AND_ID_DATA_BASE_BLOCK_LENGTH);

        // Создаем начальные символы имени файла архива
        std::string prefix = "AnalogueMeasureArchives_" + cSerialAndIdStr;

        //std::cout << "CDeviceControl AnalogueMeasureArchiveWrite prefix " <<
//                  (prefix) << std::endl;

        std::vector<std::string> matchingFiles = listFilesByPrefix(prefix);

        // создадим команду размонтирования выбранного диска.
        sprintf(acCommand,
                "%s",
                "sudo umount /mnt/usb"
               );
        //std::cout << "CDeviceControl AnalogueMeasureArchiveWriteacCommand " << acCommand << std::endl;
        // размонтируем выбранный диск.
        system(acCommand);
        usleep(100000);

        // создадим команду монтирования выбранного диска.
        sprintf(acCommand,
                "%s%s %s",
                "sudo mount /dev/",
                sDiskName.c_str(),
                "/mnt/usb"
               );
        //std::cout << "CDeviceControl AnalogueMeasureArchiveWriteacCommand " << acCommand << std::endl;
        // примонтируем выбранный диск.
        system(acCommand);
        usleep(100000);

        if (!matchingFiles.empty())
        {
            std::cout << "Имя файлов, начинающихся с \"" << prefix << "\":" << std::endl;
            for (const auto& file : matchingFiles)
            {
                std::cout << file << std::endl;

                // создадим команду сохранения
                sprintf(acCommand,
                        "%s%s %s%s",
                        "sudo cp -r -f /home/debian/",
                        file.c_str(),
                        "/mnt/usb/",
                        " && sync");
                //std::cout << "CDeviceControl AnalogueMeasureArchiveWriteacCommand " << acCommand << std::endl;

                // сохраним файл.
                // файл сохранён успешно?
                if (!(system(acCommand)))
                {
                    //std::cout << "CDeviceControl AnalogueMeasureArchiveWrite 3"  << std::endl;
                    xFileSaveStateDataPackOne.uiFileSaveState = WRITE_OK;
                }
                else
                {
                    //std::cout << "CDeviceControl AnalogueMeasureArchiveWrite 4"  << std::endl;
                    xFileSaveStateDataPackOne.uiFileSaveState = WRITE_ERROR;
                }
            }
        }
        else
        {
            std::cout << "Нет файлов, начинающихся с \"" << prefix << "\"." << std::endl;
            xFileSaveStateDataPackOne.uiFileSaveState = WRITE_ERROR;
        }

        // создадим команду размонтирования выбранного диска.
        sprintf(acCommand,
                "%s",
                "sudo umount /mnt/usb"
               );
        //std::cout << "CDeviceControl AnalogueMeasureArchiveWriteacCommand " << acCommand << std::endl;
        // размонтируем выбранный диск.
        system(acCommand);
        usleep(100000);

        EventsArchiveWrite();
    }
    else
    {
        // Если "sd" не найдено, установим пустую строку
//        sDiskName = "";
        xFileSaveStateDataPackOne.uiFileSaveState = WRITE_ERROR;
    }
}

//-----------------------------------------------------------------------------------------------------
void CDeviceControl::EventsArchiveWrite(void)
{
    //std::cout << "CDeviceControl EventsArchiveWrite 1"  << std::endl;

    char acCommand[128];

    xFileSaveStateDataPackOne.uiFileSaveState = WRITE_IDDLE;

    CParse xCArchiveSaveParse;
    xCArchiveSaveParse.GetDiskInfoNew();
    for (uint8_t i; i < 4; i++)
    {
        //std::cout << "CDeviceControl EventsArchiveWrite acName " <<
//                  (xCArchiveSaveParse.axTDiskInfo[i].acName) << std::endl;
    }

    std::string sDiskName;
    sDiskName = xCArchiveSaveParse.axTDiskInfo[1].acName;
    //std::cout << "CDeviceControl EventsArchiveWrite sDiskName " <<
//              (sDiskName) << std::endl;
// Начнем поиск имени, начиная с "sd" в конце строки sDiskName
    size_t found = sDiskName.rfind("sd");
    if (found != std::string::npos)
    {
        // Скопируем часть строки начиная с найденной позиции
        sDiskName = sDiskName.substr(found);
        sDiskName[4] = '\0';  // Добавляем завершающий ноль
        //std::cout << "CDeviceControl EventsArchiveWrite sDiskName " <<
//                  (sDiskName) << std::endl;

        std::string cSerialAndIdStr;
        // Копируем данные из m_puiSerialAndId в cSerialAndIdStr
        cSerialAndIdStr.assign((const char*)m_puiSerialAndId, SERIAL_AND_ID_DATA_BASE_BLOCK_LENGTH);

        // Создаем начальные символы имени файла архива
        std::string prefix = "EventsArchive_" + cSerialAndIdStr;

        //std::cout << "CDeviceControl EventsArchiveWrite prefix " <<
//                  (prefix) << std::endl;

        std::vector<std::string> matchingFiles = listFilesByPrefix(prefix);

        // создадим команду размонтирования выбранного диска.
        sprintf(acCommand,
                "%s",
                "sudo umount /mnt/usb"
               );
        //std::cout << "CDeviceControl EventsArchiveWriteacCommand " << acCommand << std::endl;
        // размонтируем выбранный диск.
        system(acCommand);
        usleep(100000);

        // создадим команду монтирования выбранного диска.
        sprintf(acCommand,
                "%s%s %s",
                "sudo mount /dev/",
                sDiskName.c_str(),
                "/mnt/usb"
               );
        //std::cout << "CDeviceControl EventsArchiveWriteacCommand " << acCommand << std::endl;
        // примонтируем выбранный диск.
        system(acCommand);
        usleep(100000);

        if (!matchingFiles.empty())
        {
            std::cout << "Имя файлов, начинающихся с \"" << prefix << "\":" << std::endl;
            for (const auto& file : matchingFiles)
            {
                std::cout << file << std::endl;

                // создадим команду сохранения
                sprintf(acCommand,
                        "%s%s %s%s",
                        "sudo cp -r -f /home/debian/",
                        file.c_str(),
                        "/mnt/usb/",
                        " && sync");
                //std::cout << "CDeviceControl EventsArchiveWriteacCommand " << acCommand << std::endl;

                // сохраним файл.
                // файл сохранён успешно?
                if (!(system(acCommand)))
                {
                    //std::cout << "CDeviceControl EventsArchiveWrite 3"  << std::endl;
                    xFileSaveStateDataPackOne.uiFileSaveState = WRITE_OK;
                }
                else
                {
                    //std::cout << "CDeviceControl EventsArchiveWrite 4"  << std::endl;
                    xFileSaveStateDataPackOne.uiFileSaveState = WRITE_ERROR;
                }
            }
        }
        else
        {
            std::cout << "Нет файлов, начинающихся с \"" << prefix << "\"." << std::endl;
            xFileSaveStateDataPackOne.uiFileSaveState = WRITE_ERROR;
        }

        // создадим команду размонтирования выбранного диска.
        sprintf(acCommand,
                "%s",
                "sudo umount /mnt/usb"
               );
        //std::cout << "CDeviceControl EventsArchiveWriteacCommand " << acCommand << std::endl;
        // размонтируем выбранный диск.
        system(acCommand);
        usleep(100000);
    }
    else
    {
        // Если "sd" не найдено, установим пустую строку
//        sDiskName = "";
        xFileSaveStateDataPackOne.uiFileSaveState = WRITE_ERROR;
    }
}

//-------------------------------------------------------------------------------
// обрабатывает входящие сообщения от Modbus интерфейсов по 71 функции - чтение данных онлайн, модулей аналогового ввода.
// передаёт измеренные значения аналоговых входов, реперные точки АЦП, значения ТХС.
void CDeviceControl::OnlineDataRead(void)
{
    //std::cout << "CDeviceControl OnlineDataRead 1"  << std::endl;
    float *pfSource;
    uint8_t *pucSource;
    uint8_t *pucSource2;
    uint8_t *pucDestination;
    uint8_t *pucTempArray;
    unsigned int nuiBusyTimeCounter;
    uint8_t nucIndexNumber;
    uint8_t ucTempData;
    unsigned short usCrc;
    uint8_t ucFlowControl;
    uint8_t ucAddressLow;
    uint8_t ucAddressHigh;
    int i;
    int j;
    int nb;

// ucAddressHigh -
// если в старшем регистре адреса Modbus бит7 = 0, то запрашиваются реперные точки - (бит0 - бит6) - адрес аналогового входа.
// если в старшем регистре адреса Modbus бит7 = 1, то запрашивается ТХС и (бит0 - бит2) - относительный адрес модуля МВСТ3.
// ucAddressLow - требуемое количество аналоговых входов.

    uint16_t uiAddress =
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataOffset);
    ucAddressLow = (uint8_t)(uiAddress);
    ucAddressHigh = (uint8_t)((uiAddress) >> 8);
    pucDestination =
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);

    TConfigDataPackOne* pxDeviceConfigSearch =
        (GetResources() -> GetDeviceConfigSearchPointer());

    // запрос ТХС или реперных точек?
    if (ucAddressHigh & MUVR_TXS_REQUEST_MASK)
    {
        //std::cout << "CDeviceControl OnlineDataRead 2"  << std::endl;
// запрос ТХС.
//        cout << "TXS" << endl;

//        iModuleMvai5TxsRead(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
//
//        sprintf((char*)pucDestination,
//                "%7.4f",
//                ((TTxsData*)(xAllModulesContext.axAllModulesContext[nucIndexNumber].
//                             xModuleContextStatic.
//                             pucTxsBufferPointer)) -> fTxsValue);

//        pucDestination += ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY;
//        *pucDestination = ((TTxsData*)(xAllModulesContext.axAllModulesContext[nucIndexNumber].
//                                       xModuleContextStatic.
//                                       pucTxsBufferPointer)) -> ucTxsStat;

        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataLength) =
            (ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY +
             ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY);
    }
    else
    {
        //std::cout << "CDeviceControl OnlineDataRead 3"  << std::endl;
// запрос реперных точек.

        // адрес аналогового входа принадлежит внутреннему или внешнему модулю?
        if ((pxDeviceConfigSearch -> uiServiceAnalogueInputModuleQuantity) >=
                (((ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK) /
                  MUVR_ANALOG_INPUT_QUANTITY) + 1))
        {
            //std::cout << "CDeviceControl OnlineDataRead 4"  << std::endl;
            // адрес аналогового входа принадлежит внутреннему модулю.

            // получим адрес значения аналогового входа.
            pfSource = &(GetResources() ->
                         m_pfAnalogueInputsValue[(ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK)]);
            // получим адрес значения состояние канала аналогового входа.
            pucSource2 = &(GetResources() ->
                           m_puiAnalogueInputsState[ucAddressHigh]);
            for (i = 0; i < (ucAddressLow); i++)
            {
                // преобразуем первые 7 цифр float значения, в ASCII символы, для отображения в строковом виде в программаторе.
                sprintf((char*)pucDestination,
                        "%7.4f",
                        pfSource[i]);
                pucDestination += ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY;
                // получим значения состояния канала аналогового входа.
                *pucDestination = *pucSource2++;
                pucDestination += ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY;
            }

            (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataLength) =
                ((ucAddressLow *
                  (ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY +
                   ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY)));

            //std::cout << "CDeviceControl::OnlineDataRead m_uiDataLength "  <<
//                      (int)(((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataLength) << std::endl;
        }
        else
        {
            //std::cout << "CDeviceControl OnlineDataRead 5"  << std::endl;
            // адрес аналогового входа принадлежит внешнему модулю.
//            cout << "LSNU" << endl;

            // получим адрес значения аналогового входа.
            pfSource = &(GetResources() ->
                         m_pfAnalogueInputsValue[(ucAddressHigh & ANALOGUE_INPUT_ADDRESS_MASK)]);
            // получим адрес значения состояние канала аналогового входа.
            pucSource2 = &(GetResources() ->
                           m_puiAnalogueInputsState[ucAddressHigh]);
            for (i = 0; i < (ucAddressLow); i++)
            {
                // преобразуем первые 7 цифр float значения, в ASCII символы, для отображения в строковом виде в программаторе.
                sprintf((char*)pucDestination,
                        "%7.4f",
                        pfSource[i]);
                pucDestination += ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY;
                // получим значения состояния канала аналогового входа.
                *pucDestination = *pucSource2++;
                pucDestination += ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY;
            }

            (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataLength) =
                ((ucAddressLow *
                  (ONLINE_DATA_READ_ONE_INPUT_ASCII_BYTE_QUANTITY +
                   ONLINE_DATA_READ_ONE_INPUT_STAT_AI_BYTE_QUANTITY)));

            //std::cout << "CDeviceControl::OnlineDataRead m_uiDataLength "  <<
//                      (int)(((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataLength) << std::endl;
        }
    }

    (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataOffset) = 0;

//    if (ucModuleError ==
//            PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_TXS_READ_ERROR)
//    {
//        pxModbusMapping -> current_message_address_common =
//            PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_TXS_READ_ERROR;
//        cout << "PROGRAMMING_DATA_BASE_WRITE_BLOCK_MVA_TXS_READ_ERROR" << endl;
//        ucModuleError = 0;
//    }
//
//    if (ucModuleError ==
//            MUVR_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR)
//    {
//        pxModbusMapping -> current_message_address_common =
//            MUVR_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR;
//        cout << "MUVR_GET_REPER_POINTS_ADC_DATA_COMMAND_ERROR" << endl;
//        ucModuleError = 0;
//    }

}

//-------------------------------------------------------------------------------
// обрабатывает входящие сообщения от Modbus интерфейсов, по 5 функции.
uint8_t CDeviceControl::ModbusFunction5Handler(void)
{
    //std::cout << "CDeviceControl ModbusFunction5Handler 1" << std::endl;
    uint8_t nucIndexNumber;
    int i;

    uint16_t uiAddress =
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataOffset);
    uint8_t* pucDestination =
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);

//    //std::cout << "CDeviceControl::Fsm uiAddress "  << (int)uiAddress << std::endl;
//-------------------------------------------------------------------------------
// Function 5
    // смотрим, по какому адресу записывается бит.
    switch(uiAddress -
            COILS_ARRAY_MODBUS_BEGIN_ADDRESS)
    {
    // квитирование с верхнего уровня.
    case DEVICE_CONTROL_PC_KVIT:
    {
        //std::cout << "CDeviceControl ModbusFunction5Handler 2" << std::endl;
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();
        // бит установлен?
        if ((pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]))
        {
            // если установлен - сбросим.
            (pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]) = 0;
            (GetResources() -> m_uiModbusReceipt) = 1;
        }
        else
        {
            (GetResources() -> m_uiModbusReceipt) = 0;
        }
        SetFsmState(MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        return 1;
    }
    break;

    // сброс с верхнего уровня.
    case DEVICE_CONTROL_PC_RESET:
    {
        //std::cout << "CDeviceControl ModbusFunction5Handler 3" << std::endl;
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();
        // бит установлен?
        if ((pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]))
        {
            // если установлен - сбросим.
            (pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]) = 0;
            (GetResources() -> m_uiModbusReset) = 1;
        }
        else
        {
            (GetResources() -> m_uiModbusReset) = 0;
        }
        SetFsmState(MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        return 1;
    }
    break;

    // блокировка с верхнего уровня.
    case DEVICE_CONTROL_BLOCK:
    {
        //std::cout << "CDeviceControl ModbusFunction5Handler 4" << std::endl;
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();
        // бит установлен?
        if ((pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]))
        {
            // если установлен - сбросим.
            (pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]) = 0;
            (GetResources() -> m_uiModbusBlocked) = 1;
        }
        else
        {
            (GetResources() -> m_uiModbusBlocked) = 0;
        }
        SetFsmState(MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        return 1;
    }
    break;

    default:
//    {
//        CDataContainerDataBase* pxDataContainer =
//            (CDataContainerDataBase*)GetCustomerDataContainerPointer();
//        // бит установлен?
//        if ((pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]))
//        {
//            // если установлен - сбросим.
//            (pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]) = 0;
////            (GetResources() -> m_puiCoils[uiAddress]) = 1;
//        }
//        else
//        {
////            (GetResources() -> m_puiCoils[uiAddress]) = 0;
//        }
//        SetFsmState(MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
//        return 1;
//    }
        break;
    };

//-------------------------------------------------------------------------------
    // смотрим, по какому адресу записывается бит.
    switch(((uiAddress -
             COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & 0xFF00))
    {
    // включение-выключение режима калибровки.
    case DEVICE_CONTROL_CALIBRATION_ON_OFF:
        // перед этим кто либо другой не вывел из обработки аналоговый вход?
        if (((GetResources() -> m_uiAinOffProcessOwnerIndex) == AIN_OFF_PROCESS_OWNER_IS_NONE) ||
                ((GetResources() -> m_uiAinOffProcessOwnerIndex) == AIN_OFF_PROCESS_OWNER_IS_PROGRAMMER))
        {
            // вычислим индекс модуля в массиве контекста, к которому поступила команда - калибровка.
            nucIndexNumber = (((((GetResources() -> GetDeviceConfigSearchPointer()) -> uiLastAnalogueInputModuleIndex) +
                                CONVERT_INTEGER_TO_NATURAL_NUMBER) -
                               ((GetResources() -> GetDeviceConfigSearchPointer()) -> uiServiceAnalogueInputModuleQuantity))  +
                              (((uiAddress -
                                 COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4));

            cout << "DEVICE_CONTROL_CALIBRATION_ON nucIndexNumber " << (int)nucIndexNumber << endl;
            cout << "DEVICE_CONTROL_CALIBRATION_ON uiLastAnalogueInputModuleIndex " << (int)(((GetResources() -> GetDeviceConfigSearchPointer()) ->
                    uiLastAnalogueInputModuleIndex) + CONVERT_INTEGER_TO_NATURAL_NUMBER) << endl;
            // по индексу - nucIndexNumber есть модуль?
            if (nucIndexNumber <=
                    (((GetResources() -> GetDeviceConfigSearchPointer()) ->
                      uiLastAnalogueInputModuleIndex) +
                     CONVERT_INTEGER_TO_NATURAL_NUMBER))
            {
                // получим указатель на контейнер с данными заказчика.
                CDataContainerDataBase* pxDataContainer =
                    (CDataContainerDataBase*)GetCustomerDataContainerPointer();
                // бит установлен?
                if ((pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]))
                {
                    // включение режима калибровки.
                    cout << "DEVICE_CONTROL_CALIBRATION_ON" << endl;
                    // если установлен - сбросим.
                    (pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]) = 0;
                    // получим указатель на контейнер с данными исполнителя..
                    pxDataContainer =
                        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
                    // передадим драйверу модуля номер калибруемого входа.
                    // в DO-D2 - № входа в модуле (0-нет режима калибровки).
                    (pxDataContainer -> m_puiDataPointer[COMMON_INDEX_OFFSET]) = (((uiAddress -
                            COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_INPUT_NUMBER_MASK));

                    cout << "DEVICE_CONTROL_CALIBRATION_ON ucCommonIndex " << (int)(pxDataContainer -> m_puiDataPointer[0]) << endl;
                    // вернём в рабочее состояние все выведенные из обработки аналоговые входы.
                    for (i = 0; i < MAX_HANDLED_ANALOGUE_INPUT; i++)
                    {
                        (GetResources() -> m_puiAnalogueInputsOff[i]) = 0;
                    }

                    (GetResources() -> m_puiAnalogueInputsOff[(((((uiAddress - // вычисляем номер модуля
                            COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
                            CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4) *
                            ANALOG_MODULE_INPUT_QUANTITY) +

                            (((uiAddress - // вычисляем номер входа
                               COILS_ARRAY_MODBUS_BEGIN_ADDRESS) &
                              CALIBRATION_INPUT_NUMBER_MASK) -
                             CONVERT_NATURAL_NUMBER_TO_INTEGER))]) = 1;

                    (GetResources() -> m_uiAinOffProcessOwnerIndex) = AIN_OFF_PROCESS_OWNER_IS_PROGRAMMER;
                }
                else
                {
                    // выключение режима калибровки.
                    cout << "DEVICE_CONTROL_CALIBRATION_OFF" << endl;
                    // получим указатель на контейнер с данными исполнителя..
                    pxDataContainer =
                        (CDataContainerDataBase*)GetExecutorDataContainerPointer();
                    // передадим драйверу модуля номер калибруемого входа. если 0, калибровка выключена.
                    // в DO-D2 - № входа в модуле (0-нет режима калибровки).
                    (pxDataContainer -> m_puiDataPointer[COMMON_INDEX_OFFSET]) = 0;
                    // вернём в рабочее состояние все выведенные из обработки аналоговые входы.
                    for (i = 0; i < MAX_HANDLED_ANALOGUE_INPUT; i++)
                    {
                        (GetResources() -> m_puiAnalogueInputsOff[i]);
                    }

                    (GetResources() -> m_uiAinOffProcessOwnerIndex) = AIN_OFF_PROCESS_OWNER_IS_NONE;
                }
            }
        }

        SetFsmState(MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_START);
        return 1;
        break;

    // калибровка начала шкалы - НШК.
    case DEVICE_CONTROL_SET_BOTTOM_OF_SCALE:
        // вычислим индекс модуля в массиве контекста, к которому поступила команда.
        nucIndexNumber = (((((GetResources() -> GetDeviceConfigSearchPointer()) -> uiLastAnalogueInputModuleIndex) +
                            CONVERT_INTEGER_TO_NATURAL_NUMBER) -
                           ((GetResources() -> GetDeviceConfigSearchPointer()) -> uiServiceAnalogueInputModuleQuantity))  +
                          (((uiAddress -
                             COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4));
        // по индексу - nucIndexNumber есть модуль?
        if (nucIndexNumber <=
                (((GetResources() -> GetDeviceConfigSearchPointer()) ->
                  uiLastAnalogueInputModuleIndex) + CONVERT_INTEGER_TO_NATURAL_NUMBER))
        {
            // получим указатель на контейнер с данными заказчика.
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
            // бит установлен?
            if ((pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]))
            {
                // если установлен - сбросим.
                (pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]) = 0;
                // получим указатель на контейнер с данными исполнителя..
                pxDataContainer =
                    (CDataContainerDataBase*)GetExecutorDataContainerPointer();
                // передадим драйверу модуля команду - калибровка начала шкалы - НШК.
                (pxDataContainer -> m_puiDataPointer[COMMON_COMMAND_CONTROL_OFFSET]) =
                    MUVR_COMMAND_CONTROL_SET_BOTTOM_OF_SCALE;
            }
        }

        SetFsmState(MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_START);
        return 1;
        break;

    // калибровка конца шкалы - ВШК.
    case DEVICE_CONTROL_SET_TOP_OF_SCALE:
        // вычислим индекс модуля в массиве контекста, к которому поступила команда.
        nucIndexNumber = (((((GetResources() -> GetDeviceConfigSearchPointer()) -> uiLastAnalogueInputModuleIndex) +
                            CONVERT_INTEGER_TO_NATURAL_NUMBER) -
                           ((GetResources() -> GetDeviceConfigSearchPointer()) -> uiServiceAnalogueInputModuleQuantity))  +
                          (((uiAddress -
                             COILS_ARRAY_MODBUS_BEGIN_ADDRESS) & CALIBRATION_ANALOG_MODULE_NUMBER_MASK) >> 4));
        // по индексу - nucIndexNumber есть модуль?
        if (nucIndexNumber <=
                (((GetResources() -> GetDeviceConfigSearchPointer()) ->
                  uiLastAnalogueInputModuleIndex) +
                 CONVERT_INTEGER_TO_NATURAL_NUMBER))
        {
            // получим указатель на контейнер с данными заказчика.
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
            // бит установлен?
            if ((pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]))
            {
                // если установлен - сбросим.
                (pxDataContainer -> m_puiDataPointer[BIT_STATE_OFFSET]) = 0;
                // получим указатель на контейнер с данными исполнителя..
                pxDataContainer =
                    (CDataContainerDataBase*)GetExecutorDataContainerPointer();
                // передадим драйверу модуля команду - калибровка начала шкалы - НШК.
                (pxDataContainer -> m_puiDataPointer[COMMON_COMMAND_CONTROL_OFFSET]) =
                    MUVR_COMMAND_CONTROL_SET_TOP_OF_SCALE;
            }
        }

        SetFsmState(MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_START);
        return 1;
        break;

    default:
        break;
    };

////-------------------------------------------------------------------------------
//// инкремент уставок - SP, OUT, регуляторов модулей токового вывода - MTVI5.
//	// адрес записываемого бита находится в диапазоне инкремента уставок - SP, OUT, регуляторов модулей токового вывода?
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= INCREMENT_SP_OUT_BIT_ARRAY_OFFSET) &&
//			((pxModbusMapping -> current_message_address_bits -
//			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (INCREMENT_SP_OUT_BIT_ARRAY_OFFSET +
//					  INCREMENT_SP_OUT_BIT_ARRAY_LENGTH)))
//	{
//		// бит установлен или сброшен?
//		if (pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//		{
//			// если установлен - сбросим.
//			pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//			cout << "INCREMENT_SP_OUT_1__BIT_ARRAY_OFFSET 1" << endl;
//			// установим флаг инкремента-декремента уставок - SP, OUT, регуляторов.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1; //?
//		}
//		else
//		{
//			cout << "INCREMENT_SP_OUT_1__BIT_ARRAY_OFFSET 0" << endl;
//			// сбросим флаг инкремента-декремента уставок - SP, OUT, регуляторов.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0; //?
//		}
//
//		// вычислим индекс модуля в массиве контекста, к которому поступила команда.
//		nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
//							((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
//							 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
//						   ((uint8_t)(((uiAddress -
//											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//											 INCREMENT_SP_OUT_BIT_ARRAY_OFFSET) /
//											(MTVI5_ANALOG_OUTPUT_QUANTITY))));
//
//		// по индексу - nucIndexNumber есть модуль?
//		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
//		{
////        if (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex)
////        {
//			// вычислим номер регулятора модуля токового вывода, в котором инкрементируем-декрементируем уставку.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			ucCommonIndex = (uint8_t)((((uiAddress -
//											   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//											  INCREMENT_SP_OUT_BIT_ARRAY_OFFSET) %
//											 MTVI5_ANALOG_OUTPUT_QUANTITY) +
//											CONVERT_INTEGER_TO_NATURAL_NUMBER);
//
//// в драйвере модуля токового вывода, данные уставок - SP, OUT,
//// поступают из рабочего массива axAnalogueOutputRegulatorContAv[смещение на номер регулятора],
//// после изменения туда же и возвращаются. из этого же массива, по протоколу Modbus,
//// устройства верхнего уровня(HMI панели, системы SCADA, программаторы) получают данные для отображения на дисплее.
////  на текущем шаге обмен данными драйвера с модулем по SPI интерфейсу проходит в режиме одновременной записи-чтения.
//// после записи новые данные на выходе модуля появятся только на следующем шаге записи-чтения.
//// и если на текущем шаге данные с выхода модуля просто вернуть обратно, то никаких изменений не произойдёт.
////  поэтому, после записи в модуль значения уставки, изменённого на величину в переменной usAuxiliaryCounter(выражается в процентах),
//// оно же отправляется в массив источник.
////  так же, это удобно для корректного отображения устройствами верхнего уровня, изменённых данных.
//// так как вызов драйвера модуля токового вывода происходит один раз в секунду,
//// а функци vDeviceControlModbusMessageHandler(), максимум - время обработки самого медленного модуля,
//// то за это время, оператор нажатием кнопки мыши, может инкрементировать-декрементировать значение уставки
//// N - раз. значение - N помещается в переменную usAuxiliaryCounter. в конце каждого цикла обмена данными
//// драйвера с модулем, переменная usAuxiliaryCounter - обнуляется.
//			// изменим значение переменной, на которое в драйвере модуля изменится значение уставки.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			usAuxiliaryCounter += 1;
//
//			// передадим драйверу модуля команду - инкрементировать.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			ucCommandControl = DEVICE_CONTROL_MTVI5_INCREMENT_SP_OUT;
//		}
//	}
//
////-------------------------------------------------------------------------------
//// декремент уставок - SP, OUT, регуляторов модулей токового вывода - MTVI5.
//// адрес записываемого бита находится в диапазоне декремента уставок - SP, OUT, регуляторов модулей токового вывода?
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= DECREMENT_SP_OUT_BIT_ARRAY_OFFSET) &&
//			((pxModbusMapping -> current_message_address_bits -
//			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (DECREMENT_SP_OUT_BIT_ARRAY_OFFSET +
//					  DECREMENT_SP_OUT_BIT_ARRAY_LENGTH)))
//	{
//		// бит установлен или сброшен?
//		if (pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//		{
//			// если установлен - сбросим.
//			pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//			cout << "DECREMENT_SP_OUT_BIT_ARRAY_OFFSET 1" << endl;
//			// установим флаг инкремента-декремента уставок - SP, OUT, регуляторов.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1; //?
//		}
//		else
//		{
//			cout << "DECREMENT_SP_OUT_BIT_ARRAY_OFFSET 0" << endl;
//			// сбросим флаг инкремента- декремента уставок - SP, OUT, регуляторов.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0; //?
//		}
//
//		// вычислим индекс модуля в массиве контекста, к которому поступила команда.
//		nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
//							((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
//							 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
//						   ((uint8_t)(((uiAddress -
//											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//											 DECREMENT_SP_OUT_BIT_ARRAY_OFFSET) /
//											(MTVI5_ANALOG_OUTPUT_QUANTITY))));
//		// по индексу - nucIndexNumber есть модуль?
//		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
//		{
//			// вычислим номер регулятора модуля токового вывода, в котором инкрементируем-декрементируем уставку.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			ucCommonIndex = (uint8_t)((((uiAddress -
//											   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//											  DECREMENT_SP_OUT_BIT_ARRAY_OFFSET) %
//											 MTVI5_ANALOG_OUTPUT_QUANTITY) +
//											CONVERT_INTEGER_TO_NATURAL_NUMBER);
//
//// в драйвере модуля токового вывода, данные уставок - SP, OUT,
//// поступают из рабочего массива axAnalogueOutputRegulatorContAv[смещение на номер регулятора],
//// после изменения туда же и возвращаются. из этого же массива, по протоколу Modbus,
//// устройства верхнего уровня(HMI панели, системы SCADA, программаторы) получают данные для отображения на дисплее.
////  на текущем шаге обмен данными драйвера с модулем по SPI интерфейсу проходит в режиме одновременной записи-чтения.
//// после записи новые данные на выходе модуля появятся только на следующем шаге записи-чтения.
//// и если на текущем шаге данные с выхода модуля просто вернуть обратно, то никаких изменений не произойдёт.
////  поэтому, после записи в модуль значения уставки, изменённого на величину в переменной usAuxiliaryCounter(выражается в процентах),
//// оно же отправляется в массив источник.
////  так же, это удобно для корректного отображения устройствами верхнего уровня, изменённых данных.
//// так как вызов драйвера модуля токового вывода происходит один раз в секунду,
//// а функци vDeviceControlModbusMessageHandler() - максимум время обработки самого медленного модуля,
//// то за это время, оператор нажатием кнопки мыши, может инкрементировать-декрементировать значение уставки
//// N - раз. значение - N помещается в переменную usAuxiliaryCounter. в конце каждого цикла обмена данными
//// драйвера с модулем, переменная usAuxiliaryCounter - обнуляется.
//			// изменим значение переменной, на которое в драйвере модуля изменится значение уставки.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			usAuxiliaryCounter += 1;
//
//			// передадим драйверу модуля команду - декрементировать.
//			xAllModulesContext.axAllModulesContext[nucIndexNumber].
//			xModuleContextDinamic.
//			ucCommandControl = DEVICE_CONTROL_MTVI5_DECREMENT_SP_OUT;
//		}
//	}
//
////-------------------------------------------------------------------------------
//// запись дискретных сигналов регуляторов(CONT_ST), модулей токового вывода - MTVI5.
//	// адрес записываемого бита находится в диапазоне массива дискретных сигналов регуляторов(CONT_ST) модулей токового вывода?
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= CONT_ST_BIT_ARRAY_OFFSET) &&
//			((pxModbusMapping -> current_message_address_bits -
//			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (CONT_ST_BIT_ARRAY_OFFSET +
//					  CONT_ST_BIT_ARRAY_LENGTH)))
//	{
//		// бит установлен или сброшен?
//		if (pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//		{
//			// если установлен - сбросим.
//			pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//			cout << "CONT_ST_BIT_ARRAY 1" << endl;
//			// установим бит дискретных сигналов регуляторов.
////            aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
////                           COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1; //?
//			aucContStWriteBitArray[(pxModbusMapping -> current_message_address_bits -
//									COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//								   CONT_ST_BIT_ARRAY_OFFSET] = 1;
//		}
//		else
//		{
//			cout << "CONT_ST_BIT_ARRAY 0" << endl;
//			// сбросим бит дискретных сигналов регуляторов.
////            aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
////                           COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0; //?
//			aucContStWriteBitArray[(pxModbusMapping -> current_message_address_bits -
//									COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//								   CONT_ST_BIT_ARRAY_OFFSET] = 0;
//		}
//
////        vBitToByteArray(&aucContStWriteByteArray[0],
////                        &aucCoilsArray[CONT_ST_BIT_ARRAY_OFFSET],
////                        CONT_ST_BYTE_ARRAY_LENGTH);
//		vBitToByteArray(aucContStWriteByteArray,
//						aucContStWriteBitArray,
//						CONT_ST_BYTE_ARRAY_LENGTH);
//
//		// вычислим индекс модуля в массиве контекста, к которому поступила команда.
//		nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex) + 1) -
//							((xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueOutputQuantity) /
//							 MTVI5_ANALOG_OUTPUT_QUANTITY)) +
//						   ((uint8_t)(((uiAddress -
//											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//											 CONT_ST_BIT_ARRAY_OFFSET) /
//											(MTVI5_STAT_BIT_QUANTITY * MTVI5_ANALOG_OUTPUT_QUANTITY))));
//		// по индексу - nucIndexNumber есть модуль?
//		if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueOutputModuleIndex))
//		{
////            // передадим драйверу модуля команду - запись дискретных сигналов регуляторов.
////            xAllModulesContext.axAllModulesContext[nucIndexNumber].
////            xModuleContextDinamic.
////            ucCommandControl = DEVICE_CONTROL_MTVI5_CONT_ST_WRITE;
//		}
//	}
//
////-------------------------------------------------------------------------------
//// запись команд управления верхнего уровня.
//	// адрес записываемого бита находится в диапазоне массива команд управления верхнего уровня?
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= LOG_COM_BIT_ARRAY_OFFSET) &&
//			((pxModbusMapping -> current_message_address_bits -
//			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (LOG_COM_BIT_ARRAY_OFFSET +
//					  LOG_COM_BIT_ARRAY_LENGTH)))
//	{
//		// бит установлен или сброшен?
//		if (pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//		{
//			// если установлен - сбросим.
//			pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//			cout << "LOG_COM_BIT_ARRAY_OFFSET 1" << endl;
//			// установим бит команд управления верхнего уровня.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 1;
//		}
//		else
//		{
//			cout << "LOG_COM_BIT_ARRAY_OFFSET 0" << endl;
//			// сбросим бит команд управления верхнего уровня.
//			aucCoilsArray[(pxModbusMapping -> current_message_address_bits -
//						   COILS_ARRAY_MODBUS_BEGIN_ADDRESS)] = 0;
//		}
//	}
//
////-------------------------------------------------------------------------------
//// выводит из обработки аналоговый вход.
////    // сообщение пришло с интерфейса ModbusRTU HMI?
////    if ((pxModbusMapping -> message_sourse) ==
////            MODBUS_MESSAGE_SOURCE_RTU_HMI)
//	//    {
//	// адрес записываемого бита находится в диапазоне массива вывода из обработки аналоговых входов?
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) >= AIN_OFF_BIT_ARRAY_OFFSET) &&
//			((pxModbusMapping -> current_message_address_bits -
//			  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) < (AIN_OFF_BIT_ARRAY_OFFSET +
//					  AIN_OFF_BIT_ARRAY_LENGTH)))
//	{
//		// перед этим кто либо другой не вывел из обработки аналоговый вход?
//		if ((xMainFlagRegister.uiAinOffProcessOwnerIndex == AIN_OFF_PROCESS_OWNER_IS_NONE) ||
//				(xMainFlagRegister.uiAinOffProcessOwnerIndex == AIN_OFF_PROCESS_OWNER_IS_HMI))
//		{
//			// вычислим индекс модуля в массиве контекста, к которому поступила команда.
//			nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex) + 1) -
//								(xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)) +
//							   ((uint8_t)(((uiAddress -
//												  COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//												 AIN_OFF_BIT_ARRAY_OFFSET) /
//												(MVAI5_ANALOG_INPUT_QUANTITY))));
//			// по индексу - nucIndexNumber есть модуль?
//			if (nucIndexNumber <= (xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex))
//			{
//				// вернём в рабочее состояние выведенный из обработки аналоговый вход.
//				for (i = 0; i < MAX_HANDLED_ANALOGUE_INPUT; i++)
//				{
//					// аналоговый вход - i выведен из обработки?
//					if (aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET + i])
//					{
//						// вернём в рабочее состояние выведенный из обработки аналоговый вход - i.
//						aucCoilsArray[AIN_OFF_BIT_ARRAY_OFFSET + i] = 0;
////                        // вычислим индекс модуля в массиве контекста, который в текущий момент выполняет команду.
////                        nucIndexNumber =  ((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex) + 1) -
////                                            (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)) +
////                                           ((uint8_t)((i) /
////                                                            (MVAI5_ANALOG_INPUT_QUANTITY))));
//						// передадим драйверу модуля номер калибруемого входа. если 0, калибровка выключена.
//						// выключим калибровку всех входов модуля;
//						xAllModulesContext.axAllModulesContext[
//							((((xPlcConfigService.xPlcConfigServiceData.ucLastAnalogueInputModuleIndex) + 1) -
//							  (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)) +
//							 ((uint8_t)((i) /
//											  (MVAI5_ANALOG_INPUT_QUANTITY))))].
//						xModuleContextDinamic.
//						ucCommonIndex = 0;
//					}
//				}
//
//				// бит установлен или сброшен?
//				if (pxModbusMapping->tab_bits[uiAddress -
//											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//				{
//					// включение режима калибровки.
//					cout << "AIN_OFF_BIT_ARRAY_OFFSET 1" << endl;
//					// если установлен - сбросим.
//					pxModbusMapping->tab_bits[uiAddress -
//											  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//					// вычислим и передадим драйверу модуля номер калибруемого входа.
//					xAllModulesContext.axAllModulesContext[nucIndexNumber].
//					xModuleContextDinamic.
//					ucCommonIndex = (uint8_t)((((uiAddress -
//													   COILS_ARRAY_MODBUS_BEGIN_ADDRESS) -
//													  AIN_OFF_BIT_ARRAY_OFFSET) %
//													 MVAI5_ANALOG_INPUT_QUANTITY) +
//													CONVERT_INTEGER_TO_NATURAL_NUMBER);
//					// выведем аналоговый вход из обработки.
//					aucCoilsArray[(pxModbusMapping -> current_message_address_bits) -
//								  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 1;
//					xMainFlagRegister.uiAinOffProcessOwnerIndex = AIN_OFF_PROCESS_OWNER_IS_HMI;
//
////                    aucDoValueBlockByteArray[(pxModbusMapping -> current_message_address_bits) -
////                                             COILS_ARRAY_MODBUS_BEGIN_ADDRESS - AIN_OFF_BIT_ARRAY_OFFSET] = 1;
////                    uiTempCommand = xAllModulesContext.axAllModulesContext[nucIndexNumber].
////                                     xModuleContextDinamic.
////                                     ucCommonIndex;
//				}
//				else
//				{
//					// выключение режима калибровки.
//					cout << "AIN_OFF_BIT_ARRAY_OFFSET 0" << endl;
//					xMainFlagRegister.uiAinOffProcessOwnerIndex = AIN_OFF_PROCESS_OWNER_IS_NONE;
//
////                    aucDoValueBlockByteArray[(pxModbusMapping -> current_message_address_bits) -
////                                             COILS_ARRAY_MODBUS_BEGIN_ADDRESS - AIN_OFF_BIT_ARRAY_OFFSET] = 0;
////                    uiTempCommand = 0;
//				}
//			}
//
//
//		}
//	}
//
////-------------------------------------------------------------------------------
//// принимает команду - запись конфигурации.
//	if (((pxModbusMapping -> current_message_address_bits -
//			COILS_ARRAY_MODBUS_BEGIN_ADDRESS) ==
//			CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_OFFSET))
//	{
//		// бит установлен или сброшен?
//		if (pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS])
//		{
//			// если установлен - сбросим.
//			pxModbusMapping->tab_bits[uiAddress -
//									  COILS_ARRAY_MODBUS_BEGIN_ADDRESS] = 0;
//			cout << "CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_OFFSET 1" << endl;
//
//			switch(xPlcConfigDataTransmit.xConfigData.ucErrorCode)
//			{
//			case CONFIGURATION_ERROR_SEQUENCE_ERROR:
//			case CONFIGURATION_ERROR_EMPTY_SLOT:
//				break;
//
//			case CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE:
////            iPlcConfigurationCreate();
////            vMainTaskMemoryMapFormation();
//				// сохранение конфигурации и базы данных.
//				iPlcConfigurationSave();
//				iPlcDataBaseSave();
////    vWorkArraysUpdate();
//				iBadModuleBuffClear();
//				// сброс флагов ошибок конфигурации и базы данных,
//				// для перехода на нормальную циклическую работу.
//				fbPlcConfigurationError = 0;
//				fbPlcConfigurationFileError = 0;
//				fbPlcDataBaseFileError = 0;
//				// установим флаг "перезагрузка программы".
//				fbPlcErrorReset = 1;
//				xPlcConfigDataTransmit.xConfigData.ucErrorCode = 0;
//
//				// зарегистрируем событие.
//				CEvents::EventRegistration(
//					0,
//					(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
//					 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//					0,
//					"Новая конф. и бд.");
//
//				cout << "CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE 2" << endl;
//				break;
//			case CONFIGURATION_ERROR_NEW_CONFIGURATION:
////            iPlcConfigurationCreate();
////            vMainTaskMemoryMapFormation();
//				// сохранение конфигурации и базы данных.
//				iPlcConfigurationSave();
//				//                iPlcDataBaseSave();
////    vWorkArraysUpdate();
//				iBadModuleBuffClear();
//				// сброс флагов ошибок конфигурации и базы данных,
//				// для перехода на нормальную циклическую работу.
//				fbPlcConfigurationError = 0;
//				fbPlcConfigurationFileError = 0;
//				fbPlcDataBaseFileError = 0;
//				// установим флаг "перезагрузка программы".
//				fbPlcErrorReset = 1;
//				xPlcConfigDataTransmit.xConfigData.ucErrorCode = 0;
//
//				// зарегистрируем событие.
//				CEvents::EventRegistration(
//					0,
//					(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
//					 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
//					0,
//					"Новая конфигурация.");
//
//				cout << "CONFIGURATION_ERROR_NEW_CONFIGURATION 3" << endl;
//				break;
//			case CONFIGURATION_ERROR_BAD_MODULE_FINDED_AND_REGISTERED:
//				// сброс флагов ошибок конфигурации и базы данных,
//				// для перехода на нормальную циклическую работу.
//				fbPlcConfigurationError = 0;
//				fbPlcConfigurationFileError = 0;
//				fbPlcDataBaseFileError = 0;
//				// установим флаг "перезагрузка программы".
//				fbPlcErrorReset = 1;
//				break;
//
//			default:
//				break;
//			};
//
//		}
//		else
//		{
//			cout << "CONFIGURATION_SAVE_COMMAND_BIT_ARRAY_OFFSET 0" << endl;
//			// сбросим бит запись - конфигурации.
//		}
//	}

    return 0;
}

//-------------------------------------------------------------------------------
uint16_t CDeviceControl::DataBaseBlockReadBlockRelatedAction(void)
{
    //std::cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction 1" << std::endl;

    // номер принятого для записи блока базы данных.
    uint8_t uiBlockIndex =
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex);
    //std::cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction  uiBlockIndex "  << (int)uiBlockIndex << std::endl;


    switch(uiBlockIndex)
    {
    case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET" << endl;
        {
            SetFsmState(DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_START);
        }
        break;

    case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction TEXT_TITLES_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case REFERENCE_POINTS_ADC_CODES_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction REFERENCE_POINTS_ADC_CODES_DATA_BASE_BLOCK_OFFSET" << endl;
        {
            SetFsmState(DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_START);
        }
        break;

    case SERIAL_AND_ID_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction SERIAL_AND_ID_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case CONFIGURATION_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction CONFIGURATION_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    default:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction default" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;
    };

    return 0;
}

//-------------------------------------------------------------------------------
uint16_t CDeviceControl::DataBaseBlockWriteBlockRelatedAction(void)
{
    //std::cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction 1" << std::endl;

    // номер принятого для записи блока базы данных.
    uint8_t uiBlockIndex =
        (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex);
    //std::cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction  uiBlockIndex "  << (int)uiBlockIndex << std::endl;


    switch(uiBlockIndex)
    {
    case ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET" << endl;
        {
            SetFsmState(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_START);
        }
        break;

    case DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction DISCRETE_INPUT_SYGNALS_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction CURRENT_OUTPUT_MODULE_PSP_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case TEXT_TITLES_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction TEXT_TITLES_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case REFERENCE_POINTS_ADC_CODES_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockReadBlockRelatedAction REFERENCE_POINTS_ADC_CODES_DATA_BASE_BLOCK_OFFSET" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case SERIAL_AND_ID_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction SERIAL_AND_ID_DATA_BASE_BLOCK_OFFSET" << endl;
        // установим флаг, что записан блок 97 базы данных с идентификатором прибора.
        // при первом запуске конфигурация ещё не сохранена в блоке базы данных
        // и во время проверки конфигурации при сравнении созданной и сохранённой
        // происходит ошибка и прибор переходит в
        // аварийное состояние ожидая подтверждения конфигурации.
        // конфигурация считается подтверждённой после записи 97 блока базы данных
        //  с идентификатором прибора.
        m_fbIsConfigurationConfirmed = true;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;

    case NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET" << endl;
        {
            SetFsmState(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_START);
        }
        break;

    case CONFIGURATION_DATA_BASE_BLOCK_OFFSET:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction CONFIGURATION_DATA_BASE_BLOCK_OFFSET" << endl;
//        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//        SetFsmState(DONE_OK);
        {
            SetFsmState(DATA_BASE_BLOCK_SETTINGS_WRITE_START);
        }

        break;

    default:
        cout << "CDeviceControl::DataBaseBlockWriteBlockRelatedAction default" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
        break;
    };

    return 0;
}

//-------------------------------------------------------------------------------
uint8_t CDeviceControl::Fsm(void)
{
//    //std::cout << "CDeviceControl::Fsm 1" << endl;
    uint8_t uiReadyTaskCounter = 0;
    switch (GetFsmState())
    {
    case IDDLE:
//        //std::cout << "CDeviceControl::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        ////std::cout << "CDeviceControl::Fsm STOP"  << std::endl;
        break;

    case START:
        //std::cout << "CDeviceControl::Fsm START"  << std::endl;
        Init();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
//        //std::cout << "CDeviceControl::Fsm INIT 1"  << std::endl;
    {
        CTaskInterface* pxTask =
            GetResources() ->
            GetTaskPointerByNameFromMap(m_sDataStoreName);

        if (pxTask != 0)
        {
//                //std::cout << "CDeviceControl::Fsm INIT 2"  << std::endl;
            if (pxTask -> GetFsmState() >= READY)
            {
//                    //std::cout << "CDeviceControl::Fsm INIT 3"  << std::endl;
                SetDataStore((CDataStore*)pxTask);
                uiReadyTaskCounter += 1;
//                    SetFsmState(READY);
            }
        }
        else
        {
//                //std::cout << "CDeviceControl::Fsm INIT 4"  << std::endl;
            if (GetTimerPointer() -> IsOverflow())
            {
                //std::cout << "CDeviceControl::Fsm INIT 5"  << std::endl;
                SetFsmState(STOP);
            }
        }
    }

    if (uiReadyTaskCounter > 0)
    {
        SetFsmState(READY);
    }


    m_uiConfigurationCreateId =
        GetResources() ->
        GetTaskIdByNameFromMap(m_sConfigurationCreateName);
    break;

    case READY:
//        //std::cout << "CDeviceControl::Fsm READY"  << std::endl;
//    {
//
//        if (GetCommandDataLink() != 0)
//        {
//            //std::cout << "CDeviceControl::Fsm READY 2"  << std::endl;
//            SetOperatingDataLink(GetCommandDataLink());
//            SetFsmState(GetCommandDataLink() ->
//                        GetDataContainerPointer() ->
//                        GetFsmCommandState());
//            GetCommandDataLink() ->
//            GetDataContainerPointer() ->
//            SetFsmCommandState(0);
//            SetCommandDataLink(0);
//        }
//    }
        break;

    case DONE_OK:
//        //std::cout << "CDeviceControl::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        //std::cout << "CDeviceControl::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START:
        //std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START"  << std::endl;
    {
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING);
    }
    break;

    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING:
//        //std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            //std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 2"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else
        {
            //std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                //std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
        //std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
    {
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
    }
    break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        //std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            //std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
            //std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                //std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
        //std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
    {
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
    }

    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        //std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            //std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else if (uiFsmState == DONE_ERROR)
        {
            //std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(GetFsmNextStateDoneWaitingDoneError());
        }
        else
        {
            // Время ожидания выполнения запроса закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                //std::cout << "CDeviceControl::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateDoneWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case CONFIGURATION_REQUEST_START:
        //std::cout << "CDeviceControl::Fsm CONFIGURATION_REQUEST_START"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiConfigurationCreateId;
        pxDataContainer -> m_uiFsmCommandState =
            CConfigurationCreate::CONFIGURATION_REQUEST_START;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(CONFIGURATION_REQUEST_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(CONFIGURATION_REQUEST_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(CONFIGURATION_REQUEST_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(CONFIGURATION_REQUEST_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case CONFIGURATION_REQUEST_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        CDataContainerDataBase* pxExecutorDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        CDataContainerDataBase* pxCustomerDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();

        memcpy(pxCustomerDataContainer -> m_puiDataPointer,
               (pxExecutorDataContainer -> m_puiDataPointer),
               pxExecutorDataContainer -> m_uiDataLength);
        pxCustomerDataContainer -> m_uiDataLength =
            pxExecutorDataContainer -> m_uiDataLength;

        //std::cout << "CDeviceControl::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING pxCustomerDataContainer -> m_uiDataLength "  << (int)pxCustomerDataContainer -> m_uiDataLength << std::endl;
        //std::cout << "CDeviceControl::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING pxExecutorDataContainer -> m_uiDataLength "  << (int)pxExecutorDataContainer -> m_uiDataLength << std::endl;

        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case CONFIGURATION_REQUEST_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case STATE_DATA_READ_START:
        //std::cout << "CDeviceControl::Fsm STATE_DATA_READ_START"  << std::endl;
    {
        struct stat STAT;

        if(stat("/dev/sda", &STAT) == 0)
        {
            //std::cout << "CDeviceControl::Fsm STATE_DATA_READ_START 2"  << std::endl;
            (GetResources() -> GetDeviceStateDataPointer()) ->
            uiFlashConnectorStatus =
                FLASH_CONNECTOR_INSERTED;
        }
        else
        {
            //std::cout << "CDeviceControl::Fsm STATE_DATA_READ_START 3"  << std::endl;
            (GetResources() -> GetDeviceStateDataPointer()) ->
            uiFlashConnectorStatus =
                FLASH_CONNECTOR_EMPTY;
        }

        SetFsmState(STATE_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);

//            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
//            SetFsmNextStateDoneOk(STATE_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
//            SetFsmNextStateReadyWaitingError(STATE_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
//            SetFsmNextStateDoneWaitingError(STATE_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
//            SetFsmNextStateDoneWaitingDoneError(STATE_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case STATE_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm STATE_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        CDataContainerDataBase* pxCustomerDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();

        pxCustomerDataContainer -> m_uiDataLength =
            sizeof(struct TDeviceStateDataPackOne);
        memcpy(pxCustomerDataContainer -> m_puiDataPointer,
               (GetResources() -> GetDeviceStateDataPointer()),
               pxCustomerDataContainer -> m_uiDataLength);

        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case STATE_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm STATE_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case ANALOGUE_MEASURE_ARCHIVE_WRITE_START:
        //std::cout << "CDeviceControl::Fsm ANALOGUE_MEASURE_ARCHIVE_WRITE_START"  << std::endl;
    {
        // Ожидаем завершение первого потока (если он еще выполняется)
        if (!m_pxAnalogueMeasureArchiveWriteThread || m_pxAnalogueMeasureArchiveWriteThread->joinable())
        {
            // Разорваем связь между потоком и его владелецом
            if (m_pxAnalogueMeasureArchiveWriteThread)
            {
                m_pxAnalogueMeasureArchiveWriteThread->detach();
            }
            m_pxAnalogueMeasureArchiveWriteThread.reset(); // Сбросим указатель
        }

        // Перезапускаем поток
        m_pxAnalogueMeasureArchiveWriteThread = std::make_shared<std::thread>(&CDeviceControl::AnalogueMeasureArchiveWrite, this);

        SetFsmState(ANALOGUE_MEASURE_ARCHIVE_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
    }
    break;

    case ANALOGUE_MEASURE_ARCHIVE_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm ANALOGUE_MEASURE_ARCHIVE_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case ANALOGUE_MEASURE_ARCHIVE_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm ANALOGUE_MEASURE_ARCHIVE_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST_START:
        //std::cout << "CDeviceControl::Fsm ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST_START"  << std::endl;
    {
        SetFsmState(ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
    }
    break;

    case ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        CDataContainerDataBase* pxCustomerDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();

        pxCustomerDataContainer -> m_uiDataLength =
            sizeof(struct TFileSaveStateDataPackOne);
        memcpy(pxCustomerDataContainer -> m_puiDataPointer,
               (uint8_t*)(&xFileSaveStateDataPackOne),
               pxCustomerDataContainer -> m_uiDataLength);

        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case DEVICE_CONTROL_DOMAIN_DATA_WRITE_START:
        //std::cout << "CDeviceControl::Fsm DEVICE_CONTROL_DOMAIN_DATA_WRITE_START"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();

        // получим код опции
        uint8_t uiDeviceControlDomainFsmState =
            (pxDataContainer -> m_puiDataPointer[OPTION_CODE_OFFSET]);

        switch(uiDeviceControlDomainFsmState)
        {
        // запись архива аналоговых измерений
        case DEVICE_CONTROL_DOMAIN_DATA_WRITE_ANALOGUE_MEASURE_ARCHIVE_WRITE:
            //cout << "CDeviceControl::Fsm DEVICE_CONTROL_DOMAIN_DATA_WRITE_ANALOGUE_MEASURE_ARCHIVE_WRITE" << endl;
        {
            // Ожидаем завершение первого потока (если он еще выполняется)
            if (!m_pxAnalogueMeasureArchiveWriteThread || m_pxAnalogueMeasureArchiveWriteThread->joinable())
            {
                // Разорваем связь между потоком и его владелецом
                if (m_pxAnalogueMeasureArchiveWriteThread)
                {
                    m_pxAnalogueMeasureArchiveWriteThread->detach();
                }
                m_pxAnalogueMeasureArchiveWriteThread.reset(); // Сбросим указатель
            }

            // Перезапускаем поток
            m_pxAnalogueMeasureArchiveWriteThread = std::make_shared<std::thread>(&CDeviceControl::AnalogueMeasureArchiveWrite, this);

            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            // размер заголовка суб протокола 2 байта: колчество байт в pdu  и код опци
            uint16_t  uiLength = PREAMBLE_LENGTH;
            pxCustomerDataContainer -> m_uiDataLength = uiLength;
            // формируем заголовок
            // в протоколе используется только размер pdu. вычтем 1 байт(размер pdu)
            (pxCustomerDataContainer -> m_puiDataPointer[PDU_LENGTH_OFFSET]) =
                (uiLength - PDU_LENGTH_LENGTH);
            (pxCustomerDataContainer -> m_puiDataPointer[OPTION_CODE_OFFSET]) =
                DEVICE_CONTROL_DOMAIN_DATA_WRITE_ANALOGUE_MEASURE_ARCHIVE_WRITE;

            SetFsmState(DEVICE_CONTROL_DOMAIN_DATA_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        }
        break;

        // запись данных ADC регуляторов в модуль
        case DEVICE_CONTROL_DOMAIN_DATA_WRITE_REGULATORS_DAC_DATA_WRITE:
            //cout << "CDeviceControl::Fsm DEVICE_CONTROL_DOMAIN_DATA_WRITE_REGULATORS_DAC_DATA_WRITE" << endl;
        {
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            pxCustomerDataContainer -> m_uiDataLength =
                sizeof(struct TRegulatorsDacDataPackOne);
            memcpy((GetResources() -> GetRegulatorsDacDataPointer()),
                   &(pxCustomerDataContainer -> m_puiDataPointer[DATA_OFFSET]),
                   pxCustomerDataContainer -> m_uiDataLength);

//            CDataContainerDataBase* pxCustomerDataContainer =
//                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            // размер заголовка суб протокола 2 байта: колчество байт в pdu  и код опци
            uint16_t  uiLength = PREAMBLE_LENGTH;
            pxCustomerDataContainer -> m_uiDataLength = uiLength;
            // формируем заголовок
            // в протоколе используется только размер pdu. вычтем 1 байт(размер pdu)
            (pxCustomerDataContainer -> m_puiDataPointer[PDU_LENGTH_OFFSET]) =
                (uiLength - PDU_LENGTH_LENGTH);
            (pxCustomerDataContainer -> m_puiDataPointer[OPTION_CODE_OFFSET]) =
                DEVICE_CONTROL_DOMAIN_DATA_WRITE_REGULATORS_DAC_DATA_WRITE;

            SetFsmState(DEVICE_CONTROL_DOMAIN_DATA_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        }
        break;

        // запись адреса устройства.
        case DEVICE_CONTROL_DOMAIN_DATA_WRITE_SLAVE_ADDRESS_SETTINGS_WRITE:
            //cout << "CDeviceControl::Fsm DEVICE_CONTROL_DOMAIN_DATA_WRITE_SLAVE_ADDRESS_SETTINGS_WRITE" << endl;
        {
            SetFsmState(SLAVE_ADDRESS_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_START);
        }
        break;

        // запись параметров интерфейса связи верхнего уровня.
        case DEVICE_CONTROL_DOMAIN_DATA_WRITE_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE:
            //cout << "CDeviceControl::Fsm DEVICE_CONTROL_DOMAIN_DATA_WRITE_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE" << endl;
        {
            SetFsmState(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_START);
        }
        break;

        // запись параметров интерфейса связи верхнего уровня.
        case DEVICE_CONTROL_DOMAIN_DATA_WRITE_TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE:
            //cout << "CDeviceControl::Fsm DEVICE_CONTROL_DOMAIN_DATA_WRITE_TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE" << endl;
        {
            SetFsmState(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_START);
        }
        break;

        default:
            //cout << "CDeviceControl::Fsm default" << endl;
        {
            SetFsmState(DEVICE_CONTROL_DOMAIN_DATA_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;
        };
    }
    break;

    case DEVICE_CONTROL_DOMAIN_DATA_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DEVICE_CONTROL_DOMAIN_DATA_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case DEVICE_CONTROL_DOMAIN_DATA_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DEVICE_CONTROL_DOMAIN_DATA_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case DEVICE_CONTROL_DOMAIN_DATA_READ_START:
        //std::cout << "CDeviceControl::Fsm DEVICE_CONTROL_DOMAIN_DATA_READ_START"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();

        // получим код опции
        uint8_t uiDeviceControlDomainFsmState =
            (pxDataContainer -> m_puiDataPointer[OPTION_CODE_OFFSET]);

        switch(uiDeviceControlDomainFsmState)
        {
        // запись архива аналоговых измерений
        case DEVICE_CONTROL_DOMAIN_DATA_READ_STATE_DATA_READ:
            //cout << "CDeviceControl::Fsm DEVICE_CONTROL_DOMAIN_DATA_READ_STATE_DATA_READ" << endl;
        {
            struct stat STAT;

            if(stat("/dev/sda", &STAT) == 0)
            {
                //std::cout << "CDeviceControl::Fsm STATE_DATA_READ_START 2"  << std::endl;
                (GetResources() -> GetDeviceStateDataPointer()) ->
                uiFlashConnectorStatus =
                    FLASH_CONNECTOR_INSERTED;
            }
            else
            {
                //std::cout << "CDeviceControl::Fsm STATE_DATA_READ_START 3"  << std::endl;
                (GetResources() -> GetDeviceStateDataPointer()) ->
                uiFlashConnectorStatus =
                    FLASH_CONNECTOR_EMPTY;
            }

            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            uint16_t  uiLength =
                (sizeof(struct TDeviceStateDataPackOne) + PREAMBLE_LENGTH);
            pxCustomerDataContainer -> m_uiDataLength = uiLength;
            // формируем заголовок
            // в протоколе используется только размер pdu. вычтем 1 байт(размер pdu)
            (pxCustomerDataContainer -> m_puiDataPointer[PDU_LENGTH_OFFSET]) =
                (uiLength - PDU_LENGTH_LENGTH);
            (pxCustomerDataContainer -> m_puiDataPointer[OPTION_CODE_OFFSET]) =
                DEVICE_CONTROL_DOMAIN_DATA_READ_STATE_DATA_READ;
            // крпируем данные
            memcpy(&(pxCustomerDataContainer -> m_puiDataPointer[DATA_OFFSET]),
                   (GetResources() -> GetDeviceStateDataPointer()),
                   (sizeof(struct TDeviceStateDataPackOne)));

            SetFsmState(DEVICE_CONTROL_DOMAIN_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        }
        break;

        // запись данных ADC регуляторов в модуль
        case DEVICE_CONTROL_DOMAIN_DATA_READ_ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST:
            //cout << "CDeviceControl::Fsm DEVICE_CONTROL_DOMAIN_DATA_READ_ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST" << endl;
        {
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            uint16_t  uiLength =
                (sizeof(struct TFileSaveStateDataPackOne) + PREAMBLE_LENGTH);
            pxCustomerDataContainer -> m_uiDataLength = uiLength;
            // формируем заголовок
            // в протоколе используется только размер pdu. вычтем 1 байт(размер pdu)
            (pxCustomerDataContainer -> m_puiDataPointer[PDU_LENGTH_OFFSET]) =
                (uiLength - PDU_LENGTH_LENGTH);
            (pxCustomerDataContainer -> m_puiDataPointer[OPTION_CODE_OFFSET]) =
                DEVICE_CONTROL_DOMAIN_DATA_READ_ANALOGUE_MEASURE_ARCHIVE_WRITE_STATE_REQUEST;
            // крпируем данные
            memcpy(&(pxCustomerDataContainer -> m_puiDataPointer[DATA_OFFSET]),
                   (uint8_t*)(&xFileSaveStateDataPackOne),
                   (sizeof(struct TFileSaveStateDataPackOne)));

            SetFsmState(DEVICE_CONTROL_DOMAIN_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        }
        break;

        // чтение адреса устройства.
        case DEVICE_CONTROL_DOMAIN_DATA_READ_SLAVE_ADDRESS_SETTINGS_READ:
            //cout << "CDeviceControl::Fsm DEVICE_CONTROL_DOMAIN_DATA_READ_SLAVE_ADDRESS_SETTINGS_READ" << endl;
        {
            SetFsmState(SLAVE_ADDRESS_SETTINGS_READ_START);
        }
        break;

        // чтение параметров rs485 интерфейса связи верхнего уровня.
        case DEVICE_CONTROL_DOMAIN_DATA_READ_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ:
            //cout << "CDeviceControl::Fsm DEVICE_CONTROL_DOMAIN_DATA_READ_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ" << endl;
        {
            SetFsmState(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_START);
        }
        break;

        // чтение параметров Eternet интерфейса связи верхнего уровня.
        case DEVICE_CONTROL_DOMAIN_DATA_READ_TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ:
            //cout << "CDeviceControl::Fsm DEVICE_CONTROL_DOMAIN_DATA_READ_TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ" << endl;
        {
            SetFsmState(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_START);
        }
        break;

        default:
            cout << "CDeviceControl::Fsm default" << endl;
            {
                SetFsmState(DEVICE_CONTROL_DOMAIN_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            }
            break;
        };
    }
    break;

    case DEVICE_CONTROL_DOMAIN_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DEVICE_CONTROL_DOMAIN_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case DEVICE_CONTROL_DOMAIN_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DEVICE_CONTROL_DOMAIN_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_START:
        //std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_START"  << std::endl;
    {
        m_uiInternalModuleMuvrId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleMuvrName);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiInternalModuleMuvrId;
        pxDataContainer -> m_uiFsmCommandState =
            CInternalModuleMuvr::MUVR_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_INPUT_SET_TOP_OR_BOTTOM_OF_SCALE_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_START:
        //std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_START"  << std::endl;
    {
        m_uiInternalModuleMuvrId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleMuvrName);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiInternalModuleMuvrId;
        pxDataContainer -> m_uiFsmCommandState =
            CInternalModuleMuvr::MUVR_CALIBRATION_ON_OFF;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_INPUT_CALIBRATION_COMAND_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case MODBUS_FUNCTION_5_HANDLER_START:
        //std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_START"  << std::endl;
    {
        if (ModbusFunction5Handler())
        {
            SetFsmState(MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        }
        else
        {
            SetFsmState(MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
    }
    break;

    case MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
//        //std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
//            CDataContainerDataBase* pxExecutorDataContainer =
//                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//            CDataContainerDataBase* pxCustomerDataContainer =
//                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
//
//            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
//                   (pxExecutorDataContainer -> m_puiDataPointer),
//                   pxExecutorDataContainer -> m_uiDataLength);
//            pxCustomerDataContainer -> m_uiDataLength =
//                pxExecutorDataContainer -> m_uiDataLength;

        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm MODBUS_FUNCTION_5_HANDLER_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case TIME_SET_START:
        //std::cout << "CDeviceControl::Fsm TIME_SET_START"  << std::endl;
    {
        CurrentTimeSet();
        SetFsmState(TIME_SET_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
    }
    break;

    case TIME_SET_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm TIME_SET_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case TIME_SET_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm TIME_SET_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case TIME_UPDATE_START:
//        //std::cout << "CDeviceControl::Fsm TIME_UPDATE_START"  << std::endl;
    {
        CurrentTimeUpdate();
        SetFsmState(TIME_UPDATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
    }
    break;

    case TIME_UPDATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
//        //std::cout << "CDeviceControl::Fsm TIME_UPDATE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case TIME_UPDATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm TIME_UPDATE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case ONLINE_DATA_READ_START:
        //std::cout << "CDeviceControl::Fsm ONLINE_DATA_READ_START"  << std::endl;
    {
        OnlineDataRead();
        SetFsmState(ONLINE_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
    }
    break;

    case ONLINE_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm ONLINE_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
//            CDataContainerDataBase* pxExecutorDataContainer =
//                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//            CDataContainerDataBase* pxCustomerDataContainer =
//                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
//
//            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
//                   (pxExecutorDataContainer -> m_puiDataPointer),
//                   pxExecutorDataContainer -> m_uiDataLength);
//            pxCustomerDataContainer -> m_uiDataLength =
//                pxExecutorDataContainer -> m_uiDataLength;

        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case ONLINE_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm ONLINE_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    // чзапись адреса устройства.
    case SLAVE_ADDRESS_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_START:
        //cout << "CDeviceControl::Fsm SLAVE_ADDRESS_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_START" << endl;
    {
        m_uiDataStoreId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDataStoreName);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
        pxDataContainer -> m_uiFsmCommandState =
            CDataStore::READ_BLOCK_DATA_START;
        // параметры настроек блок 101
        pxDataContainer -> m_uiDataIndex = SETTINGS_DATA_BASE_BLOCK_OFFSET;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(SLAVE_ADDRESS_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(SLAVE_ADDRESS_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(SLAVE_ADDRESS_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(SLAVE_ADDRESS_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case SLAVE_ADDRESS_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SLAVE_ADDRESS_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(SLAVE_ADDRESS_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_START);
        }
        break;

    case SLAVE_ADDRESS_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SLAVE_ADDRESS_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

    // запись параметров интерфейса связи верхнего уровня.
    case SLAVE_ADDRESS_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_START:
        //cout << "CDeviceControl::Fsm SLAVE_ADDRESS_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_START" << endl;
    {
        memcpy((uint8_t*)(&(((TPlcSettingsPackOne*)(m_puiIntermediateBuff)) -> xSlaveAddressesSettingsPackOne)),
               &(((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer[DATA_OFFSET]),
               (sizeof(struct TSlaveAddressesSettingsPackOne)));

        uint8_t uiTaskId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDataStoreName);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = uiTaskId;
        pxDataContainer -> m_uiFsmCommandState =
            CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
        // параметры настроек блок 101
        pxDataContainer -> m_uiDataIndex = SETTINGS_DATA_BASE_BLOCK_OFFSET;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(SLAVE_ADDRESS_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(SLAVE_ADDRESS_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(SLAVE_ADDRESS_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(SLAVE_ADDRESS_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case SLAVE_ADDRESS_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SLAVE_ADDRESS_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//            SetFsmState(DONE_OK);
            SetFsmState(SLAVE_ADDRESS_SETTINGS_WRITE_INTERFACE_RESTART_START);
        }
        break;

    case SLAVE_ADDRESS_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SLAVE_ADDRESS_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

    // перезагрузка интерфейса.
    case SLAVE_ADDRESS_SETTINGS_WRITE_INTERFACE_RESTART_START:
        //cout << "CDeviceControl::Fsm SLAVE_ADDRESS_SETTINGS_WRITE_INTERFACE_RESTART_START" << endl;
    {
        uint8_t uiTaskId =
            GetResources() ->
            GetTaskIdByNameFromMap("SettingsLoad");

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = uiTaskId;
        pxDataContainer -> m_uiFsmCommandState =
            CSettingsLoad::SETTINGS_LOAD_STOP_RTU_UPPER_LEVEL_INTERFACE;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(SLAVE_ADDRESS_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(SLAVE_ADDRESS_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(SLAVE_ADDRESS_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(SLAVE_ADDRESS_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case SLAVE_ADDRESS_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SLAVE_ADDRESS_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            // размер заголовка суб протокола 2 байта: колчество байт в pdu  и код опци
            uint16_t  uiLength = PREAMBLE_LENGTH;
            pxCustomerDataContainer -> m_uiDataLength = uiLength;
            // формируем заголовок
            // в протоколе используется только размер pdu. вычтем 1 байт(размер pdu)
            (pxCustomerDataContainer -> m_puiDataPointer[PDU_LENGTH_OFFSET]) =
                (uiLength - PDU_LENGTH_LENGTH);
            (pxCustomerDataContainer -> m_puiDataPointer[OPTION_CODE_OFFSET]) =
                DEVICE_CONTROL_DOMAIN_DATA_WRITE_SLAVE_ADDRESS_SETTINGS_WRITE;

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case SLAVE_ADDRESS_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SLAVE_ADDRESS_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    // чтение параметров интерфейса связи верхнего уровня.
    case SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_START:
        //cout << "CDeviceControl::Fsm SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_START" << endl;
    {
        m_uiDataStoreId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDataStoreName);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
        pxDataContainer -> m_uiFsmCommandState =
            CDataStore::READ_BLOCK_DATA_START;
        // параметры настроек блок 101
        pxDataContainer -> m_uiDataIndex = SETTINGS_DATA_BASE_BLOCK_OFFSET;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_START);
        }
        break;

    case SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

    // запись параметров интерфейса связи верхнего уровня.
    case SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_START:
        //cout << "CDeviceControl::Fsm SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_START" << endl;
    {
        memcpy((uint8_t*)(&(((TPlcSettingsPackOne*)(m_puiIntermediateBuff)) -> xRs485HighLevelSettingsPackOne)),
               &(((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer[DATA_OFFSET]),
               (sizeof(struct TPortSettingsPackOne)));

        uint8_t uiTaskId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDataStoreName);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = uiTaskId;
        pxDataContainer -> m_uiFsmCommandState =
            CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
        // параметры настроек блок 101
        pxDataContainer -> m_uiDataIndex = SETTINGS_DATA_BASE_BLOCK_OFFSET;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//            SetFsmState(DONE_OK);
            SetFsmState(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_START);
        }
        break;

    case SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

    // перезагрузка интерфейса.
    case SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_START:
        //cout << "CDeviceControl::Fsm SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_START" << endl;
    {
        uint8_t uiTaskId =
            GetResources() ->
            GetTaskIdByNameFromMap("SettingsLoad");

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = uiTaskId;
        pxDataContainer -> m_uiFsmCommandState =
            CSettingsLoad::SETTINGS_LOAD_STOP_RTU_UPPER_LEVEL_INTERFACE;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            // размер заголовка суб протокола 2 байта: колчество байт в pdu  и код опци
            uint16_t  uiLength = PREAMBLE_LENGTH;
            pxCustomerDataContainer -> m_uiDataLength = uiLength;
            // формируем заголовок
            // в протоколе используется только размер pdu. вычтем 1 байт(размер pdu)
            (pxCustomerDataContainer -> m_puiDataPointer[PDU_LENGTH_OFFSET]) =
                (uiLength - PDU_LENGTH_LENGTH);
            (pxCustomerDataContainer -> m_puiDataPointer[OPTION_CODE_OFFSET]) =
                DEVICE_CONTROL_DOMAIN_DATA_WRITE_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE;

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    // чзаписьадреса устройства.
    case TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_START:
        //cout << "CDeviceControl::Fsm TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_START" << endl;
    {
        m_uiDataStoreId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDataStoreName);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
        pxDataContainer -> m_uiFsmCommandState =
            CDataStore::READ_BLOCK_DATA_START;
        // параметры настроек блок 101
        pxDataContainer -> m_uiDataIndex = SETTINGS_DATA_BASE_BLOCK_OFFSET;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_START);
        }
        break;

    case TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

    // запись параметров интерфейса связи верхнего уровня.
    case TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_START:
        //cout << "CDeviceControl::Fsm TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_START" << endl;
    {
        memcpy((uint8_t*)(&(((TPlcSettingsPackOne*)(m_puiIntermediateBuff)) -> xEthernetSettingsPackOne)),
               &(((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer[DATA_OFFSET]),
               (sizeof(struct TEthernetSettingsPackOne)));

        uint8_t uiTaskId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDataStoreName);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = uiTaskId;
        pxDataContainer -> m_uiFsmCommandState =
            CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
        // параметры настроек блок 101
        pxDataContainer -> m_uiDataIndex = SETTINGS_DATA_BASE_BLOCK_OFFSET;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//            SetFsmState(DONE_OK);
            SetFsmState(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_START);
        }
        break;

    case TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

    // перезагрузка интерфейса.
    case TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_START:
        //cout << "CDeviceControl::Fsm TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_START" << endl;
    {
        uint8_t uiTaskId =
            GetResources() ->
            GetTaskIdByNameFromMap("SettingsLoad");

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = uiTaskId;
        pxDataContainer -> m_uiFsmCommandState =
            CSettingsLoad::SETTINGS_LOAD_STOP_RTU_UPPER_LEVEL_INTERFACE;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            // размер заголовка суб протокола 2 байта: колчество байт в pdu  и код опци
            uint16_t  uiLength = PREAMBLE_LENGTH;
            pxCustomerDataContainer -> m_uiDataLength = uiLength;
            // формируем заголовок
            // в протоколе используется только размер pdu. вычтем 1 байт(размер pdu)
            (pxCustomerDataContainer -> m_puiDataPointer[PDU_LENGTH_OFFSET]) =
                (uiLength - PDU_LENGTH_LENGTH);
            (pxCustomerDataContainer -> m_puiDataPointer[OPTION_CODE_OFFSET]) =
                DEVICE_CONTROL_DOMAIN_DATA_WRITE_TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE;

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    // чтение адреса устройства.
    case SLAVE_ADDRESS_SETTINGS_READ_START:
        //cout << "CDeviceControl::Fsm SLAVE_ADDRESS_SETTINGS_READ_START" << endl;
    {
        m_uiDataStoreId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDataStoreName);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
        pxDataContainer -> m_uiFsmCommandState =
            CDataStore::READ_BLOCK_DATA_START;
        // параметры настроек блок 101
        pxDataContainer -> m_uiDataIndex = SETTINGS_DATA_BASE_BLOCK_OFFSET;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(SLAVE_ADDRESS_SETTINGS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(SLAVE_ADDRESS_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(SLAVE_ADDRESS_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(SLAVE_ADDRESS_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case SLAVE_ADDRESS_SETTINGS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SLAVE_ADDRESS_SETTINGS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            uint16_t  uiLength =
                (sizeof(struct TSlaveAddressesSettingsPackOne) + PREAMBLE_LENGTH);
            pxCustomerDataContainer -> m_uiDataLength = uiLength;
            // формируем заголовок
            // в протоколе используется только размер pdu. вычтем 1 байт(размер pdu)
            (pxCustomerDataContainer -> m_puiDataPointer[PDU_LENGTH_OFFSET]) =
                (uiLength - PDU_LENGTH_LENGTH);
            (pxCustomerDataContainer -> m_puiDataPointer[OPTION_CODE_OFFSET]) =
                DEVICE_CONTROL_DOMAIN_DATA_READ_SLAVE_ADDRESS_SETTINGS_READ;
            memcpy(&(pxCustomerDataContainer -> m_puiDataPointer[DATA_OFFSET]),
                   (uint8_t*)(&(((TPlcSettingsPackOne*)(m_puiIntermediateBuff)) -> xSlaveAddressesSettingsPackOne)),
                   (sizeof(struct TSlaveAddressesSettingsPackOne)));

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case SLAVE_ADDRESS_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SLAVE_ADDRESS_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    // чтение параметров интерфейса связи верхнего уровня.
    case SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_START:
        //cout << "CDeviceControl::Fsm SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_START" << endl;
    {
        m_uiDataStoreId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDataStoreName);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
        pxDataContainer -> m_uiFsmCommandState =
            CDataStore::READ_BLOCK_DATA_START;
        // параметры настроек блок 101
        pxDataContainer -> m_uiDataIndex = SETTINGS_DATA_BASE_BLOCK_OFFSET;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            uint16_t  uiLength =
                (sizeof(struct TPortSettingsPackOne) + PREAMBLE_LENGTH);
            pxCustomerDataContainer -> m_uiDataLength = uiLength;
            // формируем заголовок
            // в протоколе используется только размер pdu. вычтем 1 байт(размер pdu)
            (pxCustomerDataContainer -> m_puiDataPointer[PDU_LENGTH_OFFSET]) =
                (uiLength - PDU_LENGTH_LENGTH);
            (pxCustomerDataContainer -> m_puiDataPointer[OPTION_CODE_OFFSET]) =
                DEVICE_CONTROL_DOMAIN_DATA_READ_SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ;
            memcpy(&(pxCustomerDataContainer -> m_puiDataPointer[DATA_OFFSET]),
                   (uint8_t*)(&(((TPlcSettingsPackOne*)(m_puiIntermediateBuff)) -> xRs485HighLevelSettingsPackOne)),
                   (sizeof(struct TPortSettingsPackOne)));

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm SERIAL_PORT_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    // чтение параметров интерфейса связи верхнего уровня.
    case TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_START:
        //cout << "CDeviceControl::Fsm TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_START" << endl;
    {
        m_uiDataStoreId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDataStoreName);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
        pxDataContainer -> m_uiFsmCommandState =
            CDataStore::READ_BLOCK_DATA_START;
        // параметры настроек блок 101
        pxDataContainer -> m_uiDataIndex = SETTINGS_DATA_BASE_BLOCK_OFFSET;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            CDataContainerDataBase* pxCustomerDataContainer =
                (CDataContainerDataBase*)GetCustomerDataContainerPointer();

            uint16_t  uiLength =
                (sizeof(struct TEthernetSettingsPackOne) + PREAMBLE_LENGTH);
            pxCustomerDataContainer -> m_uiDataLength = uiLength;
            // формируем заголовок
            // в протоколе используется только размер pdu. вычтем 1 байт(размер pdu)
            (pxCustomerDataContainer -> m_puiDataPointer[PDU_LENGTH_OFFSET]) =
                (uiLength - PDU_LENGTH_LENGTH);
            (pxCustomerDataContainer -> m_puiDataPointer[OPTION_CODE_OFFSET]) =
                DEVICE_CONTROL_DOMAIN_DATA_READ_TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ;
            memcpy(&(pxCustomerDataContainer -> m_puiDataPointer[DATA_OFFSET]),
                   (uint8_t*)(&(((TPlcSettingsPackOne*)(m_puiIntermediateBuff)) -> xEthernetSettingsPackOne)),
                   (sizeof(struct TEthernetSettingsPackOne)));

            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm TCP_COMMUNICATION_DEVICE_UPPER_LEVEL_SETTINGS_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_READ:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ 1"  << std::endl;
        {
            m_uiDataStoreId =
                GetResources() ->
                GetTaskIdByNameFromMap(m_sDataStoreName);

            uint8_t* puiDataPointer =
                (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);
            uint8_t uiBlockIndex =
                (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
            pxDataContainer -> m_uiFsmCommandState =
                CDataStore::READ_BLOCK_DATA_START;
            pxDataContainer -> m_uiDataIndex = uiBlockIndex;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_BLOCK_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
            SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
        break;

    case DATA_BASE_BLOCK_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        CDataContainerDataBase* pxExecutorDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        CDataContainerDataBase* pxCustomerDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();

        memcpy(pxCustomerDataContainer -> m_puiDataPointer,
               (pxExecutorDataContainer -> m_puiDataPointer),
               pxExecutorDataContainer -> m_uiDataLength);
        pxCustomerDataContainer -> m_uiDataLength =
            pxExecutorDataContainer -> m_uiDataLength;

        // выполним дополнительное действие связанное с чтением определённого блока базы данных.
        // например при чтении блока модуля аналоговых сигналов прочитаем блок из модуля.
        DataBaseBlockReadBlockRelatedAction();
    }
    break;

    case DATA_BASE_BLOCK_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_START:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_START"  << std::endl;
    {
        m_uiInternalModuleMuvrId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleMuvrName);

        uint8_t* puiDataPointer =
            (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);
        uint8_t uiBlockIndex =
            (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiInternalModuleMuvrId;
        pxDataContainer -> m_uiFsmCommandState =
            CInternalModuleMuvr::MUVR_DATA_BASE_READ;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        CDataContainerDataBase* pxExecutorDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        CDataContainerDataBase* pxCustomerDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();

        memcpy(pxCustomerDataContainer -> m_puiDataPointer,
               (pxExecutorDataContainer -> m_puiDataPointer),
               pxExecutorDataContainer -> m_uiDataLength);
        pxCustomerDataContainer -> m_uiDataLength =
            pxExecutorDataContainer -> m_uiDataLength;

        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ_MODULE_MUVR_DATA_BASE_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_START:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_START"  << std::endl;
    {
        m_uiInternalModuleMuvrId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleMuvrName);

        uint8_t* puiDataPointer =
            (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);
        uint8_t uiBlockIndex =
            (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiInternalModuleMuvrId;
        pxDataContainer -> m_uiFsmCommandState =
            CInternalModuleMuvr::MUVR_REPER_POINTS_ADC_READ;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        CDataContainerDataBase* pxExecutorDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        CDataContainerDataBase* pxCustomerDataContainer =
            (CDataContainerDataBase*)GetCustomerDataContainerPointer();

        memcpy(pxCustomerDataContainer -> m_puiDataPointer,
               (pxExecutorDataContainer -> m_puiDataPointer),
               pxExecutorDataContainer -> m_uiDataLength);
        pxCustomerDataContainer -> m_uiDataLength =
            pxExecutorDataContainer -> m_uiDataLength;

        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ_REFERENCE_POINTS_ADC_CODES_MODULE_MUVR_DATA_READ_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;
//-------------------------------------------------------------------------------



//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_WRITE_START:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_START"  << std::endl;
    {
        m_uiDataStoreId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDataStoreName);

        uint8_t* puiDataPointer =
            (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);
        uint8_t uiBlockIndex =
            (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex);
        uint8_t uiDataLength =
            (m_pxDataStore ->
             GetBlockLength((((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiDataIndex)));

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
        pxDataContainer -> m_uiFsmCommandState =
            CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
        pxDataContainer -> m_uiDataIndex = uiBlockIndex;
        pxDataContainer -> m_puiDataPointer = puiDataPointer;
        pxDataContainer -> m_uiDataLength = uiDataLength;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        // сообщим о том, что процесс записи блока начался нормально.
        // при записи блока в базу данных не ожидается результат окончания выполнения задачи.
        // проверка результата выполнения производится в _FC_PROGRAMMING_COMPLETION_REQUEST 14 функция.
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
    }
    break;

    case DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        // выполним дополнительное действие связанное с записью определённого блока базы данных.
        // например при записи блока модуля аналоговых сигналов запишим блок и в модуль.
        DataBaseBlockWriteBlockRelatedAction();
    }
    break;

    case DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_START:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_START 1"  << std::endl;
    {
        if ((GetPreviousFsmState() == DONE_OK) ||
                (GetPreviousFsmState() == READY))
        {
            //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_START 2"  << std::endl;
            SetFsmState(DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        }
        else if (GetPreviousFsmState() == DONE_ERROR)
        {
            //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_START 3"  << std::endl;
            SetFsmState(DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
    }
    break;

    case DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case CONFIGURATION_CONFIRMATION_CHECK_START:
//        //std::cout << "CDeviceControl::Fsm CONFIGURATION_CONFIRMATION_CHECK_START"  << std::endl;
    {
        // проверим записан ли блок 97 базы данных с идентификатором прибора.
        // при первом запуске конфигурация ещё не сохранена в блоке базы данных
        // и во время проверки конфигурации при сравнении созданной и сохранённой
        // происходит ошибка и прибор переходит в
        // аварийное состояние ожидая подтверждения конфигурации.
        // конфигурация считается подтверждённой после записи 97 блока базы данных
        //  с идентификатором прибора.
        if (m_fbIsConfigurationConfirmed)
        {
            std::cout << "CDataStore::Fsm CONFIGURATION_CONFIRMATION_CHECK_START 2"  << std::endl;
            SetFsmState(CONFIGURATION_CONFIRMATION_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        }
        else
        {
//                std::cout << "CDataStore::Fsm CONFIGURATION_CONFIRMATION_CHECK_START 3"  << std::endl;
            SetFsmState(CONFIGURATION_CONFIRMATION_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        }
    }
    break;

    case CONFIGURATION_CONFIRMATION_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm CONFIGURATION_CONFIRMATION_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case CONFIGURATION_CONFIRMATION_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
//        //std::cout << "CDeviceControl::Fsm CONFIGURATION_CONFIRMATION_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_MODULE_MUVR_WRITE_START:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_MODULE_MUVR_WRITE_START"  << std::endl;
    {
        m_uiInternalModuleMuvrId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleMuvrName);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiInternalModuleMuvrId;
        pxDataContainer -> m_uiFsmCommandState =
            CInternalModuleMuvr::MUVR_WRITE_DATA_BASE;
        pxDataContainer -> m_puiDataPointer =
            (((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer);

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_START);
    }
    break;

    case DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_MODULE_MUVR_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_START:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_START"  << std::endl;
    {
        m_uiInternalModuleMuvrId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleMuvrName);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiInternalModuleMuvrId;
        pxDataContainer -> m_uiFsmCommandState =
            CInternalModuleMuvr::MUVR_WRITE_DATA_BASE_CHECK;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_MODULE_MUVR_WRITE_COMPLETE_CHECK_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

////-------------------------------------------------------------------------------
//    case DATA_BASE_BLOCK_SERIAL_AND_ID_WRITE_START:
//        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_SERIAL_AND_ID_WRITE_START"  << std::endl;
//        {
//            uint8_t uiAddress =
//                ((((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer)[0] +
//                 CONVERT_INTEGER_TO_NATURAL_NUMBER);
//            //std::cout << "CDeviceControl::Fsm uiAddress "  << (int)uiAddress << std::endl;
//
//            CModbusSlave* pxModbusRtuSlaveUpperLevel =
//                (CModbusSlave*)(GetResources() ->
//                                GetTaskPointerByNameFromMap("ModbusRtuSlaveUpperLevel"));
//            pxModbusRtuSlaveUpperLevel ->
//            SetOwnAddress(uiAddress);
//
//            CModbusSlave* pxModbusTcpSlaveUpperLevel =
//                (CModbusSlave*)(GetResources() ->
//                                GetTaskPointerByNameFromMap("ModbusTcpSlaveUpperLevel"));
//            pxModbusTcpSlaveUpperLevel ->
//            SetOwnAddress(uiAddress);
//
//            SetFsmState(DATA_BASE_BLOCK_SERIAL_AND_ID_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
//        }
//        break;
//
//    case DATA_BASE_BLOCK_SERIAL_AND_ID_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
//        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_SERIAL_AND_ID_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
//        {
////            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//            SetFsmState(DONE_OK);
//        }
//        break;
//
//    case DATA_BASE_BLOCK_SERIAL_AND_ID_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
//        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_SERIAL_AND_ID_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
//        {
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
//            SetFsmState(DONE_ERROR);
//        }
//        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_START:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_START"  << std::endl;
    {
//        uint8_t uiAddress =
//            ((((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer)[0] +
//             CONVERT_INTEGER_TO_NATURAL_NUMBER);
//        //std::cout << "CDeviceControl::Fsm uiAddress "  << (int)uiAddress << std::endl;
//
////        CModbusSlave* pxModbusRtuSlaveUpperLevel =
////            (CModbusSlave*)(GetResources() ->
////                            GetTaskPointerByNameFromMap("ModbusRtuSlaveUpperLevel"));
////        pxModbusRtuSlaveUpperLevel ->
////        SetOwnAddress(uiAddress);
////
////        CModbusSlave* pxModbusTcpSlaveUpperLevel =
////            (CModbusSlave*)(GetResources() ->
////                            GetTaskPointerByNameFromMap("ModbusTcpSlaveUpperLevel"));
////        pxModbusTcpSlaveUpperLevel ->
////        SetOwnAddress(uiAddress);
//
//        ((&(((TPlcSettingsPackOne*)(m_puiIntermediateBuff)) ->
//            xSlaveAddressesSettingsPackOne)) ->
//         uiSlaveAddress) =
//             uiAddress;
//
//        uint8_t uiTaskId =
//            GetResources() ->
//            GetTaskIdByNameFromMap(m_sDataStoreName);
//
//        CDataContainerDataBase* pxDataContainer =
//            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//        pxDataContainer -> m_uiTaskId = uiTaskId;
//        pxDataContainer -> m_uiFsmCommandState =
//            CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
//        // параметры настроек блок 101
//        pxDataContainer -> m_uiDataIndex = SETTINGS_DATA_BASE_BLOCK_OFFSET;
//        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;
//
//        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
//        SetFsmNextStateDoneOk(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
//        SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
//        SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
//        SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);

        SetFsmState(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
    }
    break;

    case DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//        SetFsmState(DONE_OK);
        SetFsmState(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_READ_SETTINGS_BLOCK_DATA_START);
    }
    break;

    case DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
    {
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmState(DONE_ERROR);
    }
    break;

    // чтение адреса устройства.
    case DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_READ_SETTINGS_BLOCK_DATA_START:
        //cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_READ_SETTINGS_BLOCK_DATA_START" << endl;
    {
        m_uiDataStoreId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDataStoreName);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = m_uiDataStoreId;
        pxDataContainer -> m_uiFsmCommandState =
            CDataStore::READ_BLOCK_DATA_START;
        // параметры настроек блок 101
        pxDataContainer -> m_uiDataIndex = SETTINGS_DATA_BASE_BLOCK_OFFSET;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_WRITE_SETTINGS_BLOCK_DATA_START);
        }
        break;

    case DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_READ_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

    // запись параметров интерфейса связи верхнего уровня.
    case DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_WRITE_SETTINGS_BLOCK_DATA_START:
        //cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_WRITE_SETTINGS_BLOCK_DATA_START" << endl;
    {
        uint8_t uiAddress =
            ((((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer)[0] +
             CONVERT_INTEGER_TO_NATURAL_NUMBER);
        //std::cout << "CDeviceControl::Fsm uiAddress "  << (int)uiAddress << std::endl;

        ((&(((TPlcSettingsPackOne*)(m_puiIntermediateBuff)) ->
            xSlaveAddressesSettingsPackOne)) ->
         uiSlaveAddress) =
             uiAddress;

//        memcpy((uint8_t*)(&(((TPlcSettingsPackOne*)(m_puiIntermediateBuff)) -> xSlaveAddressesSettingsPackOne)),
//               &(((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_puiDataPointer[DATA_OFFSET]),
//               (sizeof(struct TSlaveAddressesSettingsPackOne)));

        uint8_t uiTaskId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDataStoreName);

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = uiTaskId;
        pxDataContainer -> m_uiFsmCommandState =
            CDataStore::START_WRITE_TEMPORARY_BLOCK_DATA;
        // параметры настроек блок 101
        pxDataContainer -> m_uiDataIndex = SETTINGS_DATA_BASE_BLOCK_OFFSET;
        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//            SetFsmState(DONE_OK);
            SetFsmState(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_INTERFACE_RESTART_START);
        }
        break;

    case DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_WRITE_SETTINGS_BLOCK_DATA_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

    // перезагрузка интерфейса.
    case DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_INTERFACE_RESTART_START:
        //cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_INTERFACE_RESTART_START" << endl;
    {
        uint8_t uiTaskId =
            GetResources() ->
            GetTaskIdByNameFromMap("SettingsLoad");

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = uiTaskId;
        pxDataContainer -> m_uiFsmCommandState =
            CSettingsLoad::SETTINGS_LOAD_STOP_RTU_UPPER_LEVEL_INTERFACE;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
        {
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(DONE_OK);
        }
        break;

    case DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_NETWORK_ADDRESS_WRITE_INTERFACE_RESTART_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
        {
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(DONE_ERROR);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_BLOCK_SETTINGS_WRITE_START:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_SETTINGS_WRITE_START"  << std::endl;
    {
        uint8_t uiTaskId =
            GetResources() ->
            GetTaskIdByNameFromMap("SettingsLoad");

        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
        pxDataContainer -> m_uiTaskId = uiTaskId;
        pxDataContainer -> m_uiFsmCommandState =
            CSettingsLoad::SETTINGS_LOAD_STOP_RTU_UPPER_LEVEL_INTERFACE;

        SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
        SetFsmNextStateDoneOk(DATA_BASE_BLOCK_SETTINGS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING);
        SetFsmNextStateReadyWaitingError(DATA_BASE_BLOCK_SETTINGS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingError(DATA_BASE_BLOCK_SETTINGS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
        SetFsmNextStateDoneWaitingDoneError(DATA_BASE_BLOCK_SETTINGS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING);
    }
    break;

    case DATA_BASE_BLOCK_SETTINGS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_SETTINGS_WRITE_EXECUTOR_DONE_OK_ANSWER_PROCESSING"  << std::endl;
    {
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmState(DONE_OK);
    }
    break;

    case DATA_BASE_BLOCK_SETTINGS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING:
        //std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_SETTINGS_WRITE_EXECUTOR_DONE_ERROR_ANSWER_PROCESSING"  << std::endl;
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
