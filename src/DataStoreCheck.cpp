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
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CDataStoreCheck::~CDataStoreCheck()
{

}

//-------------------------------------------------------------------------------
uint8_t CDataStoreCheck::Init(void)
{
    std::cout << "CDataStoreCheck Init"  << std::endl;
    m_pxCommandDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                               AddDataContainer(std::make_shared<CDataContainerDataBase>()));
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));

    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));

    SetCustomerDataContainer(GetExecutorDataContainerPointer());
    SetCurrentCustomerDataContainer(GetExecutorDataContainerPointer());
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
        Init();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CDataStoreCheck::Fsm INIT"  << std::endl;
        {
            CTaskInterface* pxTask =
                GetResources() ->
                GetTaskPointerByNameFromMap(m_sStorageDeviceName);

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

    case DONE_OK:
//        std::cout << "CDataStoreCheck::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CDataStoreCheck::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

////-------------------------------------------------------------------------------
//    case SUBTASK_EXECUTOR_READY_CHECK_START:
//        std::cout << "CDataStoreCheck::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
//        {
//            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
//            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
//        }
//        break;
//
//    case SUBTASK_EXECUTOR_READY_CHECK_START:
//        std::cout << "CDataStoreCheck::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
//        {
//            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
//            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
//        }
//
//        break;
//
//    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
////        //std::cout << "CDataStoreCheck::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
//    {
//        if (SetTaskData(GetCurrentExecutorDataContainerPointer()))
//        {
//            std::cout << "CDataStoreCheck::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
//            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
//        }
//        else
//        {
//            std::cout << "CDataStoreCheck::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
//            // Время ожидания выполнения запроса закончилось?
//            if (GetTimerPointer() -> IsOverflow())
//            {
//                std::cout << "CDataStoreCheck::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
//                ((CDataContainerDataBase*)GetCurrentCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
//                SetFsmState(DONE_ERROR);
//            }
//        }
//    }
//    break;
//
//    case SUBTASK_EXECUTOR_DONE_CHECK_START:
//        std::cout << "CDataStoreCheck::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
//        {
//            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
//            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
//        }
//
//        break;
//
//    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
////        //std::cout << "CDataStoreCheck::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
//    {
//        CDataContainerDataBase* pxDataContainer =
//            (CDataContainerDataBase*)GetCurrentExecutorDataContainerPointer();
//
//        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;
//
//        if (uiFsmState == DONE_OK)
//        {
//            std::cout << "CDataStoreCheck::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
//
//            uiFsmState = GetFsmNextSubTaskState();
//            if (uiFsmState == NO_SUBTASK)
//            {
//                std::cout << "CDataStoreCheck::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
//                ((CDataContainerDataBase*)GetCurrentCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//                SetFsmState(DONE_OK);
//            }
//            else
//            {
//                std::cout << "CDataStoreCheck::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
//                SetFsmState(uiFsmState);
//            }
//        }
//        else if (uiFsmState == DONE_ERROR)
//        {
//            std::cout << "CDataStoreCheck::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 5"  << std::endl;
//            ((CDataContainerDataBase*)GetCurrentCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
//            SetFsmState(DONE_ERROR);
//        }
//        else
//        {
//            // Время ожидания выполнения запроса закончилось?
//            if (GetTimerPointer() -> IsOverflow())
//            {
//                std::cout << "CDataStoreCheck::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 6"  << std::endl;
//                ((CDataContainerDataBase*)GetCurrentCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
//                SetFsmState(DONE_ERROR);
//            }
//        }
//    }
//    break;

//-------------------------------------------------------------------------------
    case DATA_STORE_CHECK_START:
        std::cout << "CDataStoreCheck::Fsm DATA_STORE_CHECK_START"  << std::endl;
        // Сбросим ошибки декодирования алгоритмом Хемминга.
        CHammingCodes::SetErrorCode(CHammingCodes::NONE_ERROR);
        m_uiRecoveryAttemptCounter = 0;
        SetFsmState(TEMPORARY_SERVICE_SECTION_DATA_CHECK);
        break;

    case TEMPORARY_SERVICE_SECTION_DATA_CHECK:
        std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_DATA_CHECK 1"  << std::endl;
        // Временный служебный блок не повреждён?
        if (ReadTemporaryServiceSection())
        {
            std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_DATA_CHECK 2"  << std::endl;
            SetFsmState(TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK);
        }
        else
        {
            std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_DATA_CHECK 3"  << std::endl;
            // Временный служебный блок повреждён, вероятно во время последнего сеанса записи.
            // Проверим целостность хранилища по постоянному служебному блоку.
            SetFsmState(SERVICE_SECTION_DATA_CHECK);
        }
        break;

    case TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK:
        std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK"  << std::endl;
        // Проверим связанность блоков хранилища с временным служебным блоком, а следовательно их целостность.
        std::cout << "CDataStoreCheck::Fsm GetStoredBlocksNumber() "  << (int)(GetStoredBlocksNumber()) << std::endl;
        for (uint16_t i = 0;
                i < GetStoredBlocksNumber();
                i++)
        {
            switch (ReadBlock(auiTempArray, i))
            {
            case 0:
                std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 9"  << std::endl;
                break;

            case 1:
            {
                std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 5"  << std::endl;
                // Блок восстановлен алгоритмом Хемминга после обнаружения ошибки?
                if (CHammingCodes::GetErrorCode() != CHammingCodes::NONE_ERROR)
                {
                    std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 6"  << std::endl;
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
            break;

            case 2:
            {
                std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 2"  << std::endl;
                // Установим индекс блока, с сохранённым Crc которого,
                // будем сравнивать Crc блока сохранённое во временном буфере.
                SetBlockIndex(i);
                // Crc блока из временного буфера совпадает с Crc блока
                // сохранённого во временном служебном буфере по текущему индексу?
                if (CheckTemporaryBlock())
                {
                    std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 3"  << std::endl;
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


//                    CDataContainerDataBase* pxCommandDataContainer =
//                        static_cast<CDataContainerDataBase*>(GetCommandDataContainerPointer());

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
                    std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 4"  << std::endl;
                    // Блок не связан с временным служебным буфером.
                    // Возможно произошла ошибка во время записи временного служебного блока.
                    // Продолжим проверку.
                    SetFsmState(SERVICE_SECTION_DATA_CHECK);
                    break;
                }
            }
            break;

            default:
                std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 7"  << std::endl;
                break;
            }

//            // Блок не связан с временным служебным буфером(или повреждён)?
//            if (!(ReadBlock(auiTempArray, i)))
//            {
//        std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 2"  << std::endl;
//                // Установим индекс блока, с сохранённым Crc которого,
//                // будем сравнивать Crc блока сохранённое во временном буфере.
//                SetBlockIndex(i);
//                // Crc блока из временного буфера совпадает с Crc блока
//                // сохранённого во временном служебном буфере по текущему индексу?
//                if (CheckTemporaryBlock())
//                {
//        std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 3"  << std::endl;
//                    // Скопируем данные блока считанные при проверке во вспомогательный буфер.
//                    memcpy(auiTempArray,
//                           GetIntermediateBuff(),
//                           GetBlockLength(i));
//                    // Данные блока успешно записаны во временные буферы,
//                    // но при записи в хранилище произошёл сбой.
//                    // Требуется повторная запись данных блока из временных буферов в хранилище.
//                    WriteBlock(auiTempArray,
//                               GetBlockLength(i),
//                               i);
//
//
////                    CDataContainerDataBase* pxCommandDataContainer =
////                        static_cast<CDataContainerDataBase*>(GetCommandDataContainerPointer());
//
////    pxDataContainer ->
////    SetDataIndex(uiBlock);
////    pxDataContainer ->
////    SetDataPointer(puiSource);
////    pxDataContainer ->
////    SetDataOffset(0);
////    pxDataContainer ->
////    SetDataLength(uiLength);
//
////    SetFsmCommandState(START_WRITE_TEMPORARY_BLOCK_DATA);
//
//
//                    // Установим время ожидания окончания записи.
//                    GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
//                    SetFsmState(CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING);
//                    break;
//                }
//                else
//                {
//        std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 4"  << std::endl;
//                    // Блок не связан с временным служебным буфером.
//                    // Возможно произошла ошибка во время записи временного служебного блока.
//                    // Продолжим проверку.
//                    SetFsmState(SERVICE_SECTION_DATA_CHECK);
//                    break;
//                }
//            }
//            else
//            {
//        std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 5"  << std::endl;
//                // Блок восстановлен алгоритмом Хемминга после обнаружения ошибки?
//                if (CHammingCodes::GetErrorCode() != CHammingCodes::NONE_ERROR)
//                {
//        std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 6"  << std::endl;
//                    // Сбросим ошибки декодирования алгоритмом Хемминга.
//                    CHammingCodes::SetErrorCode(CHammingCodes::NONE_ERROR);
//                    cout << "CHammingCodes::GetErrorCode 1 uiBlock" << (int)i << endl;
//
//                    // Требуется повторная запись данных в хранилище.
//                    WriteBlock(auiTempArray,
//                               GetBlockLength(i),
//                               i);
//
//                    // Установим время ожидания окончания записи.
//                    GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
//                    SetFsmState(CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING);
//                    break;
//                }
//            }

//            // Авансом.
//            SetFsmState(SERVICE_SECTION_DATA_WRITE_START);
        }

        std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 8"  << std::endl;
        // Авансом.
        SetFsmState(SERVICE_SECTION_DATA_WRITE_START);
        break;

    case CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING:
        std::cout << "CDataStoreCheck::Fsm CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING"  << std::endl;
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
        std::cout << "CDataStoreCheck::Fsm SERVICE_SECTION_DATA_WRITE_START"  << std::endl;
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
        std::cout << "CDataStoreCheck::Fsm SERVICE_SECTION_DATA_WRITE_END_WAITING"  << std::endl;
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
        std::cout << "CDataStoreCheck::Fsm SERVICE_SECTION_DATA_CHECK"  << std::endl;
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
        std::cout << "CDataStoreCheck::Fsm SERVICE_SECTION_LINKED_BLOCKS_CHECK"  << std::endl;
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
        cerr << "CDataStoreCheck::Fsm DATA_STORE_NEW_VERSION_ACCEPTED" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmOperationStatus(DATA_STORE_NEW_VERSION_ACCEPTED);
        SetFsmState(READY);
        break;

    case DATA_STORE_OLD_VERSION_ACCEPTED:
        // Хранилище не обновлено.
        cerr << "CDataStoreCheck::Fsm DATA_STORE_OLD_VERSION_ACCEPTED" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmOperationStatus(DATA_STORE_OLD_VERSION_ACCEPTED);
        SetFsmState(READY);
        break;

    case DATA_STORE_CHECK_OK:
        cerr << "CDataStoreCheck::Fsm DATA_STORE_CHECK_OK" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmOperationStatus(DATA_STORE_CHECK_OK);
        SetFsmState(READY);
        break;

    case DATA_STORE_CHECK_ERROR:
        // Хранилище повреждено.
        cerr << "CDataStoreCheck::Fsm DATA_STORE_CHECK_ERROR" << endl;
        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmOperationStatus(DATA_STORE_CHECK_ERROR);
        SetFsmState(STOP);
        break;

    case DATA_STORE_CHECK_REPEAT:
        std::cout << "CDataStoreCheck::Fsm DATA_STORE_CHECK_REPEAT"  << std::endl;
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
