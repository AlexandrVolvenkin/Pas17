//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <iostream>
#include <string.h>
#include <typeinfo>

#include "Timer.h"
#include "Task.h"
#include "Resources.h"
#include "StorageDevice.h"
#include "DataStore.h"
#include "Crc.h"
#include "HammingCodes.h"
#include "DataContainer.h"
#include "DataStoreCheck.h"


//-------------------------------------------------------------------------------
CDataStoreCheck::CDataStoreCheck()
{
    std::cout << "CDataStoreCheck constructor"  << std::endl;
    // получим имя класса.
    sprintf(GetTaskNamePointer(),
            "%s",
            typeid(*this).name());
    m_pxStorageDevice = 0;
    m_pxCommandDataContainer = new CDataContainerDataBase();
//    m_pxOperatingDataContainer = new CDataContainerDataBase();
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CDataStoreCheck::~CDataStoreCheck()
{
    delete m_pxOperatingDataContainer;
    delete m_pxCommandDataContainer;
}

//-------------------------------------------------------------------------------
size_t CDataStoreCheck::GetObjectLength(void)
{
    std::cout << "CDataStoreCheck GetObjectLength"  << std::endl;
    return sizeof(*this);
}

//-------------------------------------------------------------------------------
// Проверяет целостность данных хранилища.
// Восстанавлмвает повреждённые данные с помощью алгоритма Хемминга.
// Восстанавливает данные записываемого блока при сбое питания и т.д.
uint8_t CDataStoreCheck::Check(void)
{
    // Запустим процесс проверки и восстановления хранилища.
    SetFsmCommandState(DATA_STORE_CHECK_START);
}

//-------------------------------------------------------------------------------
uint8_t CDataStoreCheck::Fsm(void)
{
//        std::cout << "CDataStoreCheck::Fsm 1"  << std::endl;
    uint8_t auiTempArray[CDataStore::MAX_SERVICE_SECTION_DATA_LENGTH];

    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CDataStoreCheck::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        std::cout << "CDataStoreCheck::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CDataStoreCheck::Fsm START"  << std::endl;
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CDataStoreCheck::Fsm INIT"  << std::endl;
        {
            CTaskInterface* pxTask =
                GetResources() ->
                GetCommonTaskFromMapPointer(m_sStorageDeviceName);

            if (pxTask != 0)
            {
                if (pxTask -> GetFsmState() >= READY)
                {
                    SetStorageDevice((CStorageDeviceInterface*)pxTask);
                    SetFsmState(READY);
                    std::cout << "CDataStoreCheck::Fsm READY"  << std::endl;
                }
            }
            else
            {
                if (GetTimerPointer() -> IsOverflow())
                {
                    SetFsmState(STOP);
                    std::cout << "CDataStoreCheck::Fsm STOP"  << std::endl;
                }
            }
        }
        break;

    case READY:
//        std::cout << "CDataStoreCheck::Fsm READY"  << std::endl;
        if (GetFsmCommandState() != 0)
        {
            SetFsmState(GetFsmCommandState());
            SetFsmCommandState(0);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_STORE_CHECK_START:
        // Сбросим ошибки декодирования алгоритмом Хемминга.
        CHammingCodes::SetErrorCode(CHammingCodes::NONE_ERROR);
        m_uiRecoveryAttemptCounter = 0;
        SetFsmState(TEMPORARY_SERVICE_SECTION_DATA_CHECK);
        break;

    case TEMPORARY_SERVICE_SECTION_DATA_CHECK:
        // Временный служебный блок не повреждён?
        if (ReadTemporaryServiceSection())
        {
            SetFsmState(TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK);
        }
        else
        {
            // Временный служебный блок повреждён, вероятно во время последнего сеанса записи.
            // Проверим целостность хранилища по постоянному служебному блоку.
            SetFsmState(SERVICE_SECTION_DATA_CHECK);
        }
        break;

    case TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK:
        // Проверим связанность блоков хранилища с временным служебным блоком, а следовательно их целостность.
        for (uint16_t i = 0;
                i < GetStoredBlocksNumber();
                i++)
        {
            // Блок не связан с временным служебным буфером(или повреждён)?
            if (!(ReadBlock(auiTempArray, i)))
            {
                // Установим индекс блока, с сохранённым Crc которого,
                // будем сравнивать Crc блока сохранённое во временном буфере.
                SetBlockIndex(i);
                // Crc блока из временного буфера совпадает с Crc блока
                // сохранённого во временном служебном буфере по текущему индексу?
                if (CheckTemporaryBlock())
                {
                    // Скопируем данные блока считанные при проверке во вспомогательный буфер.
                    memcpy(auiTempArray,
                           GetIntermediateBuff(),
                           GetBlockLength(i));
                    // Данные блока успешно записаны во временные буферы,
                    // но при записи в хранилище произошёл сбой.
                    // Требуется повторная запись данных блока из временных буферов в хранилище.
                    WriteBlock(auiTempArray,
                               GetBlockLength(i),
                               i);


                    CDataContainerDataBase* pxCommandDataContainer =
                        static_cast<CDataContainerDataBase*>(GetCommandDataContainerPointer());

//    pxDataContainer ->
//    SetDataIndex(uiBlock);
//    pxDataContainer ->
//    SetDataPointer(puiSource);
//    pxDataContainer ->
//    SetDataOffset(0);
//    pxDataContainer ->
//    SetDataLength(uiLength);

//    SetFsmCommandState(START_WRITE_TEMPORARY_BLOCK_DATA);


                    // Установим время ожидания окончания записи.
                    GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
                    SetFsmState(CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING);
                    break;
                }
                else
                {
                    // Блок не связан с временным служебным буфером.
                    // Возможно произошла ошибка во время записи временного служебного блока.
                    // Продолжим проверку.
                    SetFsmState(SERVICE_SECTION_DATA_CHECK);
                    break;
                }
            }
            else
            {
                // Блок восстановлен алгоритмом Хемминга после обнаружения ошибки?
                if (CHammingCodes::GetErrorCode() != CHammingCodes::NONE_ERROR)
                {
                    // Сбросим ошибки декодирования алгоритмом Хемминга.
                    CHammingCodes::SetErrorCode(CHammingCodes::NONE_ERROR);
                    cout << "CHammingCodes::GetErrorCode 1 uiBlock" << (int)i << endl;

                    // Требуется повторная запись данных в хранилище.
                    WriteBlock(auiTempArray,
                               GetBlockLength(i),
                               i);

                    // Установим время ожидания окончания записи.
                    GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
                    SetFsmState(CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING);
                    break;
                }
            }

            // Авансом.
            SetFsmState(SERVICE_SECTION_DATA_WRITE_START);
        }
        break;

    case CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING:
        // Ожидаем окончания записи автоматом устройства хранения.
        // Устройство хранения закончило запись успешно?
        if (m_pxStorageDevice -> IsDoneOk())
        {
            SetFsmState(DATA_STORE_CHECK_REPEAT);
        }
        // Устройство хранения закончило запись не успешно?
        else if (m_pxStorageDevice -> IsDoneError())
        {
            SetFsmState(DATA_STORE_CHECK_REPEAT);
        }
        else
        {
            // Время ожидания окончания записи закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                SetFsmState(DATA_STORE_CHECK_REPEAT);
            }
        }
        break;

    case SERVICE_SECTION_DATA_WRITE_START:
        // Блоки привязаны к временному служебному блоку, следовательно целы.
        // Как минимум, последний сеанс записи во временные буферы прощёл успешо.
        // Не будем проверять целостность и совпадение служебного блока, обновим сразу.
        // Обновим служебный блок.
        ServiceSectionWritePrepare();
        // Установим время ожидания окончания записи.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
        // Запустим процесс записи служебного блока.
        SetFsmState(SERVICE_SECTION_DATA_WRITE_END_WAITING);
        break;

    case SERVICE_SECTION_DATA_WRITE_END_WAITING:
        // Ожидаем окончания записи автоматом устройства хранения.
        // Устройство хранения закончило запись успешно?
        if (m_pxStorageDevice -> IsDoneOk())
        {
            // Служебный блок не повреждён?
            if (ReadServiceSection())
            {
                SetFsmState(DATA_STORE_NEW_VERSION_ACCEPTED);
            }
            else
            {
                SetFsmState(DATA_STORE_CHECK_REPEAT);
            }
        }
        // Устройство хранения закончило запись не успешно?
        else if (m_pxStorageDevice -> IsDoneError())
        {
            SetFsmState(DATA_STORE_CHECK_REPEAT);
        }
        else
        {
            // Время ожидания окончания записи закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                SetFsmState(DATA_STORE_CHECK_REPEAT);
            }
        }

        break;

    case SERVICE_SECTION_DATA_CHECK:
        // Мы здесь если временный служебный блок не связан с данными хранимых блоков(или повреждён).
        // Произошёл сбой во время записи. Попытаемся вернуть предыдущее состояние хранилища.
        // Служебный блок не повреждён?
        if (ReadServiceSection())
        {
            SetFsmState(SERVICE_SECTION_LINKED_BLOCKS_CHECK);
        }
        else
        {
            std::cout << "CDataStoreCheck::Fsm 1"  << std::endl;
            SetFsmState(DATA_STORE_CHECK_ERROR);
        }
        break;

    case SERVICE_SECTION_LINKED_BLOCKS_CHECK:
        // Проверим связанность блоков хранилища со служебным блоком, а следовательно их целостность.
        for (uint16_t i = 0;
                i < GetStoredBlocksNumber();
                i++)
        {
            // Блок не связан со служебным буфером(или повреждён)?
            if (!(ReadBlock(auiTempArray, i)))
            {
                // Блок не связан со служебным буфером.
                // Возможно произошла ошибка во время записи служебного блока.
                // Восстановить данные нельзя.
                SetFsmState(DATA_STORE_CHECK_ERROR);
                break;
            }
            else
            {
                // Блок восстановлен алгоритмом Хемминга после обнаружения ошибки?
                if (CHammingCodes::GetErrorCode() != CHammingCodes::NONE_ERROR)
                {
                    // Сбросим ошибки декодирования алгоритмом Хемминга.
                    CHammingCodes::SetErrorCode(CHammingCodes::NONE_ERROR);
                    cout << "CHammingCodes::GetErrorCode 2 uiBlock" << (int)i << endl;

                    // Требуется повторная запись данных в хранилище.
                    WriteBlock(auiTempArray,
                               GetBlockLength(i),
                               i);

                    SetFsmState(DATA_STORE_CHECK_ERROR);
                    break;
                }
            }

            // Авансом.
            // Блоки привязаны к служебному блоку, следовательно целы.
            // Восстановим предыдущую копию хранилища.
            SetFsmState(DATA_STORE_OLD_VERSION_ACCEPTED);
        }
        break;

    case DATA_STORE_NEW_VERSION_ACCEPTED:
        // Хранилище обновлено.
        cerr << "DATA_STORE_NEW_VERSION_ACCEPTED" << endl;
        SetFsmOperationStatus(DATA_STORE_NEW_VERSION_ACCEPTED);
        SetFsmState(READY);
        break;

    case DATA_STORE_OLD_VERSION_ACCEPTED:
        // Хранилище не обновлено.
        cerr << "DATA_STORE_OLD_VERSION_ACCEPTED" << endl;
        SetFsmOperationStatus(DATA_STORE_OLD_VERSION_ACCEPTED);
        SetFsmState(READY);
        break;

    case DATA_STORE_CHECK_OK:
        cerr << "DATA_STORE_CHECK_OK" << endl;
        SetFsmOperationStatus(DATA_STORE_CHECK_OK);
        SetFsmState(READY);
        break;

    case DATA_STORE_CHECK_ERROR:
        // Хранилище повреждено.
        cerr << "DATA_STORE_CHECK_ERROR" << endl;
        SetFsmOperationStatus(DATA_STORE_CHECK_ERROR);
        SetFsmState(STOP);
        break;

    case DATA_STORE_CHECK_REPEAT:
        // Продолжим проверку и попытки восстановления.
        // Ещё есть возможность для восстановления?
        if (m_uiRecoveryAttemptCounter < RECOVERY_ATTEMPTS_NUMBER)
        {
            m_uiRecoveryAttemptCounter++;
            // Повторим проверку хранилища после восстановления.
            SetFsmState(TEMPORARY_SERVICE_SECTION_DATA_CHECK);
        }
        else
        {
            SetFsmState(DATA_STORE_CHECK_ERROR);
        }
        break;

    default:
        break;
    }

    return GetFsmState();
}
