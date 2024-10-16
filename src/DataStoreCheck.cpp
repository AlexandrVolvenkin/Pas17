//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <iostream>
#include <string.h>

#include "Task.h"
#include "Resources.h"
#include "StorageDevice.h"
#include "DataStore.h"
#include "DataStoreCheck.h"
#include "Crc.h"
#include "HammingCodes.h"


//-------------------------------------------------------------------------------
CDataStoreCheck::CDataStoreCheck()
{
    SetFsmState(IDDLE);
}

//-------------------------------------------------------------------------------
CDataStoreCheck::CDataStoreCheck(CDataStore* pxDataStore) :
    m_pxDataStore(pxDataStore)
{
    SetFsmState(IDDLE);
}

//-------------------------------------------------------------------------------
CDataStoreCheck::~CDataStoreCheck()
{
    //dtor
}

//-------------------------------------------------------------------------------
void CDataStoreCheck::SetDataStoreName(std::string sName)
{
    m_sDataStoreName = sName;
}

//-------------------------------------------------------------------------------
void CDataStoreCheck::SetDataStore(CDataStore* pxDataStore)
{
    m_pxDataStore = pxDataStore;
}

//-------------------------------------------------------------------------------
// Проверяет целостность данных хранилища.
// Восстанавлмвает повреждённые данные с помощью алгоритма Хемминга.
// Восстанавливает данные записываемого блока при сбое питания и т.д.
uint8_t CDataStoreCheck::Check(void)
{
    // Запустим процесс проверки и восстановления хранилища.
    SetFsmState(DATA_STORE_CHECK_START);
}

//-------------------------------------------------------------------------------
uint8_t CDataStoreCheck::Fsm(void)
{
    uint8_t auiTempArray[CDataStore::MAX_BLOCK_LENGTH];

    switch (GetFsmState())
    {
    case IDDLE:
        break;

    case START:
        std::cout << "CDataStoreCheck::Fsm START"  << std::endl;
        std::cout << "CDataStoreCheck::Fsm m_sDataStoreName" << " " << (m_sDataStoreName) << std::endl;
        SetDataStore((CDataStore*)
                         (GetResources() ->
                          GetCommonTaskFromMapPointer(m_sDataStoreName)));
        SetFsmState(READY);
        break;

    case READY:
        std::cout << "CDataStoreCheck::Fsm READY"  << std::endl;
        break;

    case STOP:
        std::cout << "CDataStoreCheck::Fsm STOP"  << std::endl;
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
        if (m_pxDataStore -> ReadTemporaryServiceSection())
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
                i < m_pxDataStore -> GetStoredBlocksNumber();
                i++)
        {
            // Блок не связан с временным служебным буфером(или повреждён)?
            if (!(m_pxDataStore -> ReadBlock(auiTempArray, i)))
            {
                // Установим индекс блока, с сохранённым Crc которого,
                // будем сравнивать Crc блока сохранённое во временном буфере.
                m_pxDataStore -> SetBlockIndex(i);
                // Crc блока из временного буфера совпадает с Crc блока
                // сохранённого во временном служебном буфере по текущему индексу?
                if (m_pxDataStore -> CheckTemporaryBlock())
                {
                    // Скопируем данные блока считанные при проверке во вспомогательный буфер.
                    memcpy(auiTempArray,
                           m_pxDataStore -> GetIntermediateBuff(),
                           m_pxDataStore -> GetBlockLength(i));
                    // Данные блока успешно записаны во временные буферы,
                    // но при записи в хранилище произошёл сбой.
                    // Требуется повторная запись данных блока из временных буферов в хранилище.
                    m_pxDataStore -> WriteBlock(auiTempArray,
                                                m_pxDataStore -> GetBlockLength(i),
                                                i);

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
                    m_pxDataStore -> WriteBlock(auiTempArray,
                                                m_pxDataStore -> GetBlockLength(i),
                                                i);

                    SetFsmState(CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING);
                    break;
                }
            }

            // Авансом.
            SetFsmState(SERVICE_SECTION_DATA_WRITE_START);
        }
        break;

    case CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING:
        // Ожидаем окончания записи автоматом хранилища.
        // Сохранённый во временном буфере блок записан в хранилище?
//            if (GetFsmEvent() == CDataStore::WRITE_OK_FSM_EVENT)
//            {
        if (m_pxDataStore -> Fsm() ==
                CDataStore::DATA_WRITED_SUCCESSFULLY)
        {
            m_pxDataStore -> SetFsmState(CDataStore::IDDLE);
            SetFsmState(DATA_STORE_CHECK_REPEAT);
        }
        // При записи блока произошла ошибка?
//            else if (GetFsmEvent() == CDataStore::WRITE_ERROR_FSM_EVENT)
//            {
        else if (m_pxDataStore -> Fsm() ==
                 CDataStore::WRITE_ERROR)
        {
            m_pxDataStore -> SetFsmState(CDataStore::IDDLE);
            SetFsmState(DATA_STORE_CHECK_REPEAT);
        }
        break;

    case SERVICE_SECTION_DATA_WRITE_START:
        // Блоки привязаны к временному служебному блоку, следовательно целы.
        // Как минимум, последний сеанс записи во временные буферы прощёл успешо.
        // Не будем проверять целостность и совпадение служебного блока, обновим сразу.
        // Обновим служебный блок.
//            SetFsmEvent(WRITE_IN_PROGRESS_FSM_EVENT);
        // Запустим процесс записи служебного блока.
        m_pxDataStore -> SetFsmState(CDataStore::START_WRITE_SERVICE_SECTION_DATA);
        SetFsmState(SERVICE_SECTION_DATA_WRITE_END_WAITING);
        break;

    case SERVICE_SECTION_DATA_WRITE_END_WAITING:
        // Ожидаем окончания записи автоматом хранилища.
        // Служебный блок записан в хранилище?
//            if (GetFsmEvent() == CDataStore::WRITE_OK_FSM_EVENT)
//            {
        if (m_pxDataStore -> Fsm() ==
                CDataStore::DATA_WRITED_SUCCESSFULLY)
        {
            m_pxDataStore -> SetFsmState(CDataStore::IDDLE);
            // Служебный блок не повреждён?
            if (m_pxDataStore -> ReadServiceSection())
            {
                SetFsmState(DATA_STORE_NEW_VERSION_ACCEPTED);
            }
            else
            {
                SetFsmState(DATA_STORE_CHECK_REPEAT);
            }
        }
        // При записи блока произошла ошибка?
//            else if (GetFsmEvent() == CDataStore::WRITE_ERROR_FSM_EVENT)
//            {
        else if (m_pxDataStore -> Fsm() ==
                 CDataStore::WRITE_ERROR)
        {
            m_pxDataStore -> SetFsmState(CDataStore::IDDLE);
            SetFsmState(DATA_STORE_CHECK_REPEAT);
        }
        break;

    case SERVICE_SECTION_DATA_CHECK:
        // Мы здесь если временный служебный блок не связан с данными хранимых блоков(или повреждён).
        // Произошёл сбой во время записи. Попытаемся вернуть предыдущее состояние хранилища.
        // Служебный блок не повреждён?
        if (m_pxDataStore -> ReadServiceSection())
        {
            SetFsmState(SERVICE_SECTION_LINKED_BLOCKS_CHECK);
        }
        else
        {
            SetFsmState(DATA_STORE_CHECK_ERROR);
        }
        break;

    case SERVICE_SECTION_LINKED_BLOCKS_CHECK:
        // Проверим связанность блоков хранилища со служебным блоком, а следовательно их целостность.
        for (uint16_t i = 0;
                i < m_pxDataStore -> GetStoredBlocksNumber();
                i++)
        {
            // Блок не связан со служебным буфером(или повреждён)?
            if (!(m_pxDataStore -> ReadBlock(auiTempArray, i)))
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
                    m_pxDataStore -> WriteBlock(auiTempArray,
                                                m_pxDataStore -> GetBlockLength(i),
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
        break;

    case DATA_STORE_OLD_VERSION_ACCEPTED:
        // Хранилище не обновлено.
        cerr << "DATA_STORE_OLD_VERSION_ACCEPTED" << endl;
        break;

    case DATA_STORE_CHECK_OK:
        break;

    case DATA_STORE_CHECK_ERROR:
        // Хранилище повреждено.
        cerr << "DATA_STORE_CHECK_ERROR" << endl;
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
