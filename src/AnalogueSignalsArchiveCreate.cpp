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
//    // ������� ��������� �� ����� � ������� ���������� ������ ��� �������� ������.
//    m_puiDiscreteInputsState =
//        &(GetResources() ->
//          m_puiDiscreteInputsState[GetResources() ->
//                                                  m_uiUsedDiscreteInputsState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedDiscreteInputsState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ������������� ���������� ������ ��� �������� ������.
//    m_puiDiscreteInputsBadState =
//        &(GetResources() ->
//          m_puiDiscreteInputsBadState[GetResources() ->
//                                                     m_uiUsedDiscreteInputsBadState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedDiscreteInputsBadState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;


    // ������� ��������� �� ����� � ������� ���������� ������ ��� �������� ������.
    m_pfAnalogueInputsValue =
        &(GetResources() ->
          m_pfAnalogueInputsValue[0]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputsValue +=
//        MUVR_ANALOG_INPUT_QUANTITY;


//    // ������� ��������� �� ����� � ������� ��������� ���������� ������ ��� �������� ������.
//    m_puiAnalogueInputsState =
//        &(GetResources() ->
//          m_puiAnalogueInputsState[GetResources() ->
//                                                  m_uiUsedAnalogueInputsState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputsBadState +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ������������� ���������� ������ ��� �������� ������.
//    m_puiAnalogueInputsBadState =
//        &(GetResources() ->
//          m_puiAnalogueInputsBadState[GetResources() ->
//                                                     m_uiUsedAnalogueInputsBadState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputsBadState +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ��������� ���������� �������� ����������� ����������� �������.
//    m_puiAnalogueInputDiscreteInputsState =
//        &(GetResources() ->
//          m_puiAnalogueInputDiscreteInputsState[GetResources() ->
//                                 m_uiUsedAnalogueInputDiscreteInputsState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputDiscreteInputsState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ������ ��������������� ��������� ���������� �������� ����������� ����������� �������.
//    m_puiAnalogueInputDiscreteInputsBadState =
//        &(GetResources() ->
//          m_puiAnalogueInputDiscreteInputsBadState[GetResources() ->
//                                 m_uiUsedAnalogueInputDiscreteInputsBadState]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputDiscreteInputsBadState +=
//        MUVR_DISCRETE_SIGNALS_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ���������� ���������� ������ ��� �������� ������.
//    m_puiAnalogueInputsOff =
//        &(GetResources() ->
//          m_puiAnalogueInputsOff[GetResources() ->
//                                                m_uiUsedAnalogueInputsOff]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputsOff +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ������������� �� ���������� ������ ���� ������
//    // ���������� ��������.
//    m_pxAnalogueInputDescriptionWork =
//        &(GetResources() ->
//          m_pxAnalogueInputDescriptionWork[GetResources() ->
//                                                          m_uiUsedAnalogueInputDescriptionWork]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedAnalogueInputDescriptionWork +=
//        MUVR_ANALOG_INPUT_QUANTITY;
//
//
//    // ������� ��������� �� ����� � ������� ���������� ������ ��� �������� ������.
//    m_puiReperPointsAdcBuffer =
//        &(GetResources() ->
//          m_puiReperPointsAdcBuffer[GetResources() ->
//                                                   m_uiUsedReperPointsAdcBuffer]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedReperPointsAdcBuffer +=
//        ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH;


//    // ������� ��������� �� ����� � ������� ������� ���������� �������� ��� �������� ������.
//    m_pxDiscreteSignalsDescriptionWork =
//        &(GetResources() ->
//          m_pxDiscreteSignalsDescriptionWork[GetResources() ->
//                                 m_uiUsedDiscreteSignalsDescriptionWork]);
//    // �������� ����� ����� ���������� ������.
//    GetResources() ->
//    m_uiUsedDiscreteSignalsDescriptionWork +=
//        ANALOGUE_INPUT_MODULE_REPER_POINTS_ADC_DATA_BASE_BLOCK_LENGTH;

//    m_puiHoldingRegisters = m_pxResources -> GetInputRegisters();
    m_puiHoldingRegisters = m_pxResources -> GetHoldingRegisters();
    m_puiInputRegisters = m_pxResources -> GetInputRegisters();
//
//    m_uiBadAnswerCounter = 0;
}

//-------------------------------------------------------------------------------
void CAnalogueSignalsArchiveCreate::CreateArchiveEntry(void)
{
//    std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 1"  << std::endl;

    struct Data
    {
        time_t currentTime; // ���������� ��� �������� �������� �������
        float fAin1;       // ���������� ������� �����
        float fAin2;       // ���������� ������� �����
        float fAin3;       // ���������� �������� �����
        float fAin4;       // ���������� ���������� �����
    };

    // �������� ������� �����
    time_t now = time(nullptr);
    // �������� ������� ����
    struct tm tstructCurrent = *gmtime(&now);


//-------------------------------------------------------------------------------
// ���������� �������� ������� � �������� ������.

    unsigned short *pusDestination;
    // ������� ��������� �� ����� � ������� �������� � ������� ������� �������.
    pusDestination =
        (unsigned short*)&m_puiInputRegisters[CURRENT_TIME_OFFSET_INPUT_REGISTERS];

    pusDestination[CURRENT_TIME_SECOND_OFFSET] = tstructCurrent.tm_sec;
    pusDestination[CURRENT_TIME_MINUTE_OFFSET] = tstructCurrent.tm_min;
    pusDestination[CURRENT_TIME_HOUR_OFFSET] = tstructCurrent.tm_hour;
    pusDestination[CURRENT_TIME_MONTH_DAY_OFFSET] = tstructCurrent.tm_mday;
    pusDestination[CURRENT_TIME_MONTH_OFFSET] = (tstructCurrent.tm_mon) + 1;
    pusDestination[CURRENT_TIME_YEAR_OFFSET] = (tstructCurrent.tm_year) - 100;

    // ������� �����������?
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
//    // ������� �����������?
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

////    // ������ ������?
////    if (ui8CurrentTimeSaveDelayCounter != tstructCurrent.tm_min)
////    {
////        ui8CurrentTimeSaveDelayCounter = tstructCurrent.tm_min;
////        // �������� ������� ����� � FRAM.
////        iFramWrite(FRAM_LAST_SAVED_TIME_OFFSET,
////                   (uint8_t*)&xCurrentTime,
////                   sizeof(xCurrentTime));
////    }
//-------------------------------------------------------------------------------


    // �������� ������ �������� ��� � �������. ���� ����� ���������� � ��������� ��������,
    // ����� �� �������� ������������� ������� �������� ����� � �������� �������, ��� � 5 �������
    // ������ �� 100mc.
    // �� ��������� ����� �������?
    if (tstructCurrent.tm_sec == m_iLastSecond)
    {
        return;
    }
    else
    {
        m_iLastSecond = tstructCurrent.tm_sec;
    }

    Data data;

    // ��������� ���������� ��������� �������
    data.currentTime = now;
    data.fAin1 = (float)(m_pfAnalogueInputsValue[0]); // ������ �������� ��� fAin1
    data.fAin2 = (float)(m_pfAnalogueInputsValue[1]); // ������ �������� ��� fAin2
    data.fAin3 = (float)(m_pfAnalogueInputsValue[2]); // ������ �������� ��� fAin3
    data.fAin4 = (float)(m_pfAnalogueInputsValue[3]); // ������ �������� ��� fAin4

    // ��� ������ ������ ������� ��� ��� ����� ������ ������� �����. ��� ����� �������
    // ��� ���������� �������� ���������� m_iLastHour � m_iLastDay. �� ����� ��������.
    // ��� ������ ����� �������?
    if (m_bIsStartState)
    {
        std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 2"  << std::endl;
        m_bIsStartState = false;

        // ��������� �������� ��� ��������� ��������
        m_iLastHour = tstructCurrent.tm_min;
//        m_iLastHour = tstructCurrent.tm_hour;

        // ��������� �������� ��� ��������� ��������
        m_iLastDay = tstructCurrent.tm_hour;
////            m_iLastDay = tstructCurrent.tm_mday;


        // ������ ����� ��� �������� ������ � fram �������.
        m_uiCurrentOffset = 0;

        // ���� ������ ����� ������������ ������ � fram.
        {
            // ��� ���������� fram ������.
            const std::string hourArchiveFramFile = "/dev/mtd0";

            std::ofstream hourArchiveFramOutputStream(hourArchiveFramFile, std::ios::binary | std::ios::in | std::ios::out);
            if (!hourArchiveFramOutputStream.is_open())
            {
                std::cerr << "Failed to open for write /dev/mtd0" << std::endl;
                return;
            }

            // ���������� ������ � ���� fram
            // ��������� ��������� �� ������ ����� ������������ ������.
            hourArchiveFramOutputStream.seekp(m_uiCurrentOffset, std::ios::beg);
            hourArchiveFramOutputStream.write(reinterpret_cast<const char*>(&data), sizeof(Data));
            m_uiCurrentOffset += sizeof(Data);
            // ��������� ����
            hourArchiveFramOutputStream.close();
        }

        // ���� �������� ������ ����� ��������� ������.
        {
            // �������� ���� ������ ��������� ������.
            // ����������� ����
            char dateStr[80];
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tstructCurrent);

            // �������� ������� ��� � �����
            int year = tstructCurrent.tm_year + 1900;
            int month = tstructCurrent.tm_mon + 1;
            int hour = tstructCurrent.tm_hour;
            int minute = tstructCurrent.tm_min;

            //    // ������� ���� � ������ � �����
            //    std::string pathToYearFolder = "AnalogueMeasureArchives_" + std::to_string(year);
            //    std::string pathToMonthFolder = pathToYearFolder + "/" + std::to_string(month);
            //    std::string dailyArchveFlashFile = pathToMonthFolder + "/AnalogueMeasure_" + dateStr + ".csv";

            // ������� ���� � ������ � �����
            std::string pathToYearFolder = "/home/debian/AnalogueMeasureArchives_" + std::to_string(year);
            std::string pathToMonthFolder = pathToYearFolder + "/" + std::to_string(month);
            std::string dailyArchveFlashFile = pathToMonthFolder + "/AnalogueMeasure_" + dateStr + "-" + std::to_string(hour) + ".csv";
            //        std::string dailyArchveFlashFile = pathToMonthFolder + "/AnalogueMeasure_" + dateStr + "-" + std::to_string(m_iFileNumberCounter) + ".csv";

            // �������� � �������� ���������� (���������� POSIX �������)
            if (mkdir(pathToYearFolder.c_str(), 0755) == -1)
            {
                perror("Error creating Year Folder");
            }

            if (mkdir(pathToMonthFolder.c_str(), 0755) == -1)
            {
                perror("Error creating Month Folder");
            }

            // �������� ��� ����� �������� ��������� ������. ��� ����������� ����� ����� ��������
            // ������ ���� ������ ��������� � ���� ������.
            m_sCurrentDailyArchveFlashFile = dailyArchveFlashFile;
        }

        // ���� �������� ��������� � ����� �������� ��������� ������.
        {
            // ��������� �������� ���� ��� ���������� ������
            std::ofstream dailyArchveFlashOutputStream(m_sCurrentDailyArchveFlashFile, std::ios::app | std::ios::in | std::ios::out);
            if (!dailyArchveFlashOutputStream.is_open())
            {
                std::cerr << "Failed to open for write: " << m_sCurrentDailyArchveFlashFile << std::endl;
            }

            // ���������� ���������
            dailyArchveFlashOutputStream << "����;�����;AIn1;AIn2;AIn3;AIn4" << std::endl;
            // ��������� ����
            dailyArchveFlashOutputStream.close();
        }
    }
    else
    {
//        std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 3"  << std::endl;
        // ���� ������� ��� ���������� �� �����������,
        // ������, �������� ����� ���.
        // �������� ����� ���?
        if (tstructCurrent.tm_min != m_iLastHour)
//    if (tstructCurrent.tm_hour != m_iLastHour)
        {
            std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 4"  << std::endl;
            std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry tm_min "  << (float)tstructCurrent.tm_min << std::endl;
            // ��������� �������� ��� ��������� ��������
            m_iLastHour = tstructCurrent.tm_min;
//        m_iLastHour = tstructCurrent.tm_hour;

            // ���� ���������� ������ �� fram � ������ � ���� �������� ��������� ������.
            {
                // ��� ���������� fram ������.
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

                // ��������� �������� ���� ��� ���������� ������
                std::ofstream dailyArchveFlashOutputStream(m_sCurrentDailyArchveFlashFile, std::ios::app | std::ios::in | std::ios::out);
                if (!dailyArchveFlashOutputStream.is_open())
                {
                    std::cerr << "Failed to open for write: " << m_sCurrentDailyArchveFlashFile << std::endl;
                    hourArchiveFramInputStream.close();
                    hourArchiveFramOutputStream.close();
                    return;
                }

                // �������� ����� ����� �����
                size_t fileSize = m_uiCurrentOffset;
                std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry m_uiCurrentOffset "  << (float)m_uiCurrentOffset << std::endl;

                // ��������� ���������� �������� Data � �����
                size_t numDataObjects = (fileSize / sizeof(Data));
                std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry numDataObjects "  << (float)numDataObjects << std::endl;

                // ��������� � ����������� ������ �� fram �� ����.
                for (size_t i = 0; i < numDataObjects; i++)
                {
                    Data readData;
                    // ��������� ��������� �� ������ ������������ ������.
                    hourArchiveFramInputStream.seekg((i * sizeof(Data)), std::ios::beg);
                    hourArchiveFramInputStream.read(reinterpret_cast<char*>(&readData), sizeof(Data));

                    // ������ ��� ������ ��� ������?
                    if (!hourArchiveFramInputStream.gcount())
                    {
                        std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 5"  << std::endl;
                        break;
                    }

                    // �������� ���� �� ���������� ���������� ������.
                    struct tm tstructRead = *gmtime(&readData.currentTime);

                    // ����������� ���� � �����
                    char dateStr[80];
                    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tstructRead);

                    char timeStr[80];
                    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &tstructRead);

                    // ���������� ������ � ���� �������� ��������� ������.
                    dailyArchveFlashOutputStream <<
                                                 dateStr << ";" <<
                                                 timeStr << ";" <<
                                                 readData.fAin1 << ";" <<
                                                 readData.fAin2 << ";" <<
                                                 readData.fAin3 << ";" <<
                                                 readData.fAin4 <<
                                                 std::endl;
                }

                // ��������� ����
                hourArchiveFramInputStream.close();
                hourArchiveFramOutputStream.close();
                dailyArchveFlashOutputStream.close();
            }

            // ������ ����� ��� �������� ������ � fram �������.
            m_uiCurrentOffset = 0;

            // ���� ������ ����� ������������ ������ � fram.
            {
                // ��� ���������� fram ������.
                const std::string hourArchiveFramFile = "/dev/mtd0";

                std::ofstream hourArchiveFramOutputStream(hourArchiveFramFile, std::ios::binary | std::ios::in | std::ios::out);
                if (!hourArchiveFramOutputStream.is_open())
                {
                    std::cerr << "Failed to open for write /dev/mtd0" << std::endl;
                    return;
                }

                // ���������� ������ � ���� fram
                // ��������� ��������� �� ������ ����� ������������ ������.
                hourArchiveFramOutputStream.seekp(m_uiCurrentOffset, std::ios::beg);
                hourArchiveFramOutputStream.write(reinterpret_cast<const char*>(&data), sizeof(Data));
                m_uiCurrentOffset += sizeof(Data);
                // ��������� ����
                hourArchiveFramOutputStream.close();
            }


            // ���� ������� ���� ���������� �� �����������,
            // ������, ��������� ����� �����.
            // ��������� ����� �����?
            //        m_iLastDay++;
            //        if (m_iLastDay >= 3)
            if (tstructCurrent.tm_hour != m_iLastDay)
                ////        if (tstructCurrent.tm_mday != m_iLastDay)
            {
                std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 6"  << std::endl;
                std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry tm_min "  << (float)tstructCurrent.tm_min << std::endl;
                std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry tm_hour "  << (float)tstructCurrent.tm_hour << std::endl;

                // ��������� �������� ��� ��������� ��������
                //            m_iLastDay = 0;
                //            m_iFileNumberCounter++;
                m_iLastDay = tstructCurrent.tm_hour;
                ////            m_iLastDay = tstructCurrent.tm_mday;

                // ���� ������ ����� �������� ��������� ������.
                {
                    // ����� ����� ���������� �����.
                    {
                        // ������� ��� gzip
                        std::string command = "gzip " + m_sCurrentDailyArchveFlashFile;

                        // ���������� �������
                        int result = system(command.c_str());

                        if (result == 0)
                        {
                            std::cout << "���� ������� ���� � ��������." << std::endl;
                        }
                        else
                        {
                            std::cerr << "������ ��� ���������� �������." << std::endl;
                        }
                    }

                    {
                        // ������ �������� ���� ������ ���������� �����.
                        // ������� ��� rm
                        std::string command = "sudo rm -f -R " + m_sCurrentDailyArchveFlashFile;

                        // ���������� �������
                        int result = system(command.c_str());

                        if (result == 0)
                        {
                            std::cout << "���� ������� �����. " << m_sCurrentDailyArchveFlashFile << std::endl;
                        }
                        else
                        {
                            std::cerr << "������ ��� �������� �����. " << m_sCurrentDailyArchveFlashFile << std::endl;
                        }
                    }
                }

                // ���� �������� ������ ����� ��������� ������.
                {
                    // �������� ����� ���� ��������� ������.
                    // ����������� ����
                    char dateStr[80];
                    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tstructCurrent);

                    // �������� ������� ��� � �����
                    int year = tstructCurrent.tm_year + 1900;
                    int month = tstructCurrent.tm_mon + 1;
                    int hour = tstructCurrent.tm_hour;
                    int minute = tstructCurrent.tm_min;

                    //    // ������� ���� � ������ � �����
                    //    std::string pathToYearFolder = "AnalogueMeasureArchives_" + std::to_string(year);
                    //    std::string pathToMonthFolder = pathToYearFolder + "/" + std::to_string(month);
                    //    std::string dailyArchveFlashFile = pathToMonthFolder + "/AnalogueMeasure_" + dateStr + ".csv";

                    // ������� ���� � ������ � �����
                    std::string pathToYearFolder = "/home/debian/AnalogueMeasureArchives_" + std::to_string(year);
                    std::string pathToMonthFolder = pathToYearFolder + "/" + std::to_string(month);
                    std::string dailyArchveFlashFile = pathToMonthFolder + "/AnalogueMeasure_" + dateStr + "-" + std::to_string(hour) + ".csv";
                    //        std::string dailyArchveFlashFile = pathToMonthFolder + "/AnalogueMeasure_" + dateStr + "-" + std::to_string(m_iFileNumberCounter) + ".csv";

                    // �������� � �������� ���������� (���������� POSIX �������)
                    if (mkdir(pathToYearFolder.c_str(), 0755) == -1)
                    {
                        perror("Error creating Year Folder");
                    }

                    if (mkdir(pathToMonthFolder.c_str(), 0755) == -1)
                    {
                        perror("Error creating Month Folder");
                    }

                    // �������� ��� ����� �������� ��������� ������. ��� ����������� ����� ����� ��������
                    // ������ ���� ������ ��������� � ���� ������.
                    m_sCurrentDailyArchveFlashFile = dailyArchveFlashFile;
                }

                // ���� �������� ��������� � ����� �������� ��������� ������.
                {
                    // ��������� �������� ���� ��� ���������� ������
                    std::ofstream dailyArchveFlashOutputStream(m_sCurrentDailyArchveFlashFile, std::ios::app | std::ios::in | std::ios::out);
                    if (!dailyArchveFlashOutputStream.is_open())
                    {
                        std::cerr << "Failed to open for write: " << m_sCurrentDailyArchveFlashFile << std::endl;
                    }

                    // ���������� ���������
                    //        output << "����;�����;AIn1;AIn2;AIn3;AIn4" << std::endl;
                    //        output << "   ����   " << ";" << "   �����   " << ";" << "   AIn1   " << ";" << "   AIn2   " << ";" << "   AIn3   " << ";" << "   AIn4   " << std::endl;
                    dailyArchveFlashOutputStream << "����;�����;AIn1;AIn2;AIn3;AIn4" << std::endl;
                    // ��������� ����
                    dailyArchveFlashOutputStream.close();
                }
            }
        }
        else
        {
//        std::cout << "CAnalogueSignalsArchiveCreate::CreateArchiveEntry 10"  << std::endl;
            // ���� ������ ����� ������������ ������ � fram.
            {
                const std::string hourArchiveFramFile = "/dev/mtd0";

                std::ofstream hourArchiveFramOutputStream(hourArchiveFramFile, std::ios::binary | std::ios::in | std::ios::out);
                if (!hourArchiveFramOutputStream.is_open())
                {
                    std::cerr << "Failed to open for write /dev/mtd0" << std::endl;
                    return;
                }

                // ���������� ������ � ���� fram
                // ��������� ��������� �� ������ ����� ������������ ������.
                hourArchiveFramOutputStream.seekp(m_uiCurrentOffset, std::ios::beg);
                hourArchiveFramOutputStream.write(reinterpret_cast<const char*>(&data), sizeof(Data));
                m_uiCurrentOffset += sizeof(Data);

                // ��������� ����
                hourArchiveFramOutputStream.close();
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
            // ����� �������� ���������� ������� �����������?
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
            // ����� �������� ���������� ������� �����������?
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
            // ����� �������� ���������� ������� �����������?
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

