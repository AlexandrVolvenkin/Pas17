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
    // ������� ��� ������.
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
// ��������� ����������� ������ ���������.
// ��������������� ����������� ������ � ������� ��������� ��������.
// ��������������� ������ ������������� ����� ��� ���� ������� � �.�.
uint8_t CDataStoreCheck::Check(void)
{
    // �������� ������� �������� � �������������� ���������.
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
//            // ����� �������� ���������� ������� �����������?
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
//            // ����� �������� ���������� ������� �����������?
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
        // ������� ������ ������������� ���������� ��������.
        CHammingCodes::SetErrorCode(CHammingCodes::NONE_ERROR);
        m_uiRecoveryAttemptCounter = 0;
        SetFsmState(TEMPORARY_SERVICE_SECTION_DATA_CHECK);
        break;

    case TEMPORARY_SERVICE_SECTION_DATA_CHECK:
        std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_DATA_CHECK 1"  << std::endl;
        // ��������� ��������� ���� �� ��������?
        if (ReadTemporaryServiceSection())
        {
            std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_DATA_CHECK 2"  << std::endl;
            SetFsmState(TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK);
        }
        else
        {
            std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_DATA_CHECK 3"  << std::endl;
            // ��������� ��������� ���� ��������, �������� �� ����� ���������� ������ ������.
            // �������� ����������� ��������� �� ����������� ���������� �����.
            SetFsmState(SERVICE_SECTION_DATA_CHECK);
        }
        break;

    case TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK:
        std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK"  << std::endl;
        // �������� ����������� ������ ��������� � ��������� ��������� ������, � ������������� �� �����������.
        std::cout << "CDataStoreCheck::Fsm GetStoredBlocksNumber() "  << (int)(GetStoredBlocksNumber()) << std::endl;
        for (uint16_t i = 0;
                i < GetStoredBlocksNumber();
                i++)
        {
            switch (ReadBlock(auiTempArray, i))
            {
            case 0:
                break;

            case 1:
            {
                std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 5"  << std::endl;
                // ���� ������������ ���������� �������� ����� ����������� ������?
                if (CHammingCodes::GetErrorCode() != CHammingCodes::NONE_ERROR)
                {
                    std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 6"  << std::endl;
                    // ������� ������ ������������� ���������� ��������.
                    CHammingCodes::SetErrorCode(CHammingCodes::NONE_ERROR);
                    cout << "CHammingCodes::GetErrorCode 1 uiBlock" << (int)i << endl;

                    // ��������� ��������� ������ ������ � ���������.
                    WriteBlock(auiTempArray,
                               GetBlockLength(i),
                               i);

                    // ��������� ����� �������� ��������� ������.
                    GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
                    SetFsmState(CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING);
                    break;
                }
            }
            break;

            case 2:
            {
                std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 2"  << std::endl;
                // ��������� ������ �����, � ���������� Crc ��������,
                // ����� ���������� Crc ����� ���������� �� ��������� ������.
                SetBlockIndex(i);
                // Crc ����� �� ���������� ������ ��������� � Crc �����
                // ����������� �� ��������� ��������� ������ �� �������� �������?
                if (CheckTemporaryBlock())
                {
                    std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 3"  << std::endl;
                    // ��������� ������ ����� ��������� ��� �������� �� ��������������� �����.
                    memcpy(auiTempArray,
                           GetIntermediateBuff(),
                           GetBlockLength(i));
                    // ������ ����� ������� �������� �� ��������� ������,
                    // �� ��� ������ � ��������� ��������� ����.
                    // ��������� ��������� ������ ������ ����� �� ��������� ������� � ���������.
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


                    // ��������� ����� �������� ��������� ������.
                    GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
                    SetFsmState(CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING);
                    break;
                }
                else
                {
                    std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 4"  << std::endl;
                    // ���� �� ������ � ��������� ��������� �������.
                    // �������� ��������� ������ �� ����� ������ ���������� ���������� �����.
                    // ��������� ��������.
                    SetFsmState(SERVICE_SECTION_DATA_CHECK);
                    break;
                }
            }
            break;

            default:
                std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 7"  << std::endl;
                break;
            }

//            // ���� �� ������ � ��������� ��������� �������(��� ��������)?
//            if (!(ReadBlock(auiTempArray, i)))
//            {
//        std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 2"  << std::endl;
//                // ��������� ������ �����, � ���������� Crc ��������,
//                // ����� ���������� Crc ����� ���������� �� ��������� ������.
//                SetBlockIndex(i);
//                // Crc ����� �� ���������� ������ ��������� � Crc �����
//                // ����������� �� ��������� ��������� ������ �� �������� �������?
//                if (CheckTemporaryBlock())
//                {
//        std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 3"  << std::endl;
//                    // ��������� ������ ����� ��������� ��� �������� �� ��������������� �����.
//                    memcpy(auiTempArray,
//                           GetIntermediateBuff(),
//                           GetBlockLength(i));
//                    // ������ ����� ������� �������� �� ��������� ������,
//                    // �� ��� ������ � ��������� ��������� ����.
//                    // ��������� ��������� ������ ������ ����� �� ��������� ������� � ���������.
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
//                    // ��������� ����� �������� ��������� ������.
//                    GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
//                    SetFsmState(CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING);
//                    break;
//                }
//                else
//                {
//        std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 4"  << std::endl;
//                    // ���� �� ������ � ��������� ��������� �������.
//                    // �������� ��������� ������ �� ����� ������ ���������� ���������� �����.
//                    // ��������� ��������.
//                    SetFsmState(SERVICE_SECTION_DATA_CHECK);
//                    break;
//                }
//            }
//            else
//            {
//        std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 5"  << std::endl;
//                // ���� ������������ ���������� �������� ����� ����������� ������?
//                if (CHammingCodes::GetErrorCode() != CHammingCodes::NONE_ERROR)
//                {
//        std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 6"  << std::endl;
//                    // ������� ������ ������������� ���������� ��������.
//                    CHammingCodes::SetErrorCode(CHammingCodes::NONE_ERROR);
//                    cout << "CHammingCodes::GetErrorCode 1 uiBlock" << (int)i << endl;
//
//                    // ��������� ��������� ������ ������ � ���������.
//                    WriteBlock(auiTempArray,
//                               GetBlockLength(i),
//                               i);
//
//                    // ��������� ����� �������� ��������� ������.
//                    GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
//                    SetFsmState(CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING);
//                    break;
//                }
//            }

//            // �������.
//            SetFsmState(SERVICE_SECTION_DATA_WRITE_START);
        }

        std::cout << "CDataStoreCheck::Fsm TEMPORARY_SERVICE_SECTION_LINKED_BLOCKS_CHECK 8"  << std::endl;
        // �������.
        SetFsmState(SERVICE_SECTION_DATA_WRITE_START);
        break;

    case CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING:
        std::cout << "CDataStoreCheck::Fsm CORRUPTED_BLOCK_RECOVERY_WRITE_END_WAITING"  << std::endl;
        // ������� ��������� ������ ��������� ���������� ��������.
        // ���������� �������� ��������� ������ �������?
        if (m_pxStorageDevice -> IsDoneOk())
        {
            SetFsmState(DATA_STORE_CHECK_REPEAT);
        }
        // ���������� �������� ��������� ������ �� �������?
        else if (m_pxStorageDevice -> IsDoneError())
        {
            SetFsmState(DATA_STORE_CHECK_REPEAT);
        }
        else
        {
            // ����� �������� ��������� ������ �����������?
            if (GetTimerPointer() -> IsOverflow())
            {
                SetFsmState(DATA_STORE_CHECK_REPEAT);
            }
        }
        break;

    case SERVICE_SECTION_DATA_WRITE_START:
        std::cout << "CDataStoreCheck::Fsm SERVICE_SECTION_DATA_WRITE_START"  << std::endl;
        // ����� ��������� � ���������� ���������� �����, ������������� ����.
        // ��� �������, ��������� ����� ������ �� ��������� ������ ������ ������.
        // �� ����� ��������� ����������� � ���������� ���������� �����, ������� �����.
        // ������� ��������� ����.
        ServiceSectionWritePrepare();
        // ��������� ����� �������� ��������� ������.
        GetTimerPointer() -> Set(WRITE_END_WAITING_TIMEOUT);
        // �������� ������� ������ ���������� �����.
        SetFsmState(SERVICE_SECTION_DATA_WRITE_END_WAITING);
        break;

    case SERVICE_SECTION_DATA_WRITE_END_WAITING:
        std::cout << "CDataStoreCheck::Fsm SERVICE_SECTION_DATA_WRITE_END_WAITING"  << std::endl;
        // ������� ��������� ������ ��������� ���������� ��������.
        // ���������� �������� ��������� ������ �������?
        if (m_pxStorageDevice -> IsDoneOk())
        {
            // ��������� ���� �� ��������?
            if (ReadServiceSection())
            {
                SetFsmState(DATA_STORE_NEW_VERSION_ACCEPTED);
            }
            else
            {
                SetFsmState(DATA_STORE_CHECK_REPEAT);
            }
        }
        // ���������� �������� ��������� ������ �� �������?
        else if (m_pxStorageDevice -> IsDoneError())
        {
            SetFsmState(DATA_STORE_CHECK_REPEAT);
        }
        else
        {
            // ����� �������� ��������� ������ �����������?
            if (GetTimerPointer() -> IsOverflow())
            {
                SetFsmState(DATA_STORE_CHECK_REPEAT);
            }
        }

        break;

    case SERVICE_SECTION_DATA_CHECK:
        std::cout << "CDataStoreCheck::Fsm SERVICE_SECTION_DATA_CHECK"  << std::endl;
        // �� ����� ���� ��������� ��������� ���� �� ������ � ������� �������� ������(��� ��������).
        // ��������� ���� �� ����� ������. ���������� ������� ���������� ��������� ���������.
        // ��������� ���� �� ��������?
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
        // �������� ����������� ������ ��������� �� ��������� ������, � ������������� �� �����������.
        for (uint16_t i = 0;
                i < GetStoredBlocksNumber();
                i++)
        {
            // ���� �� ������ �� ��������� �������(��� ��������)?
            if (!(ReadBlock(auiTempArray, i)))
            {
                // ���� �� ������ �� ��������� �������.
                // �������� ��������� ������ �� ����� ������ ���������� �����.
                // ������������ ������ ������.
                SetFsmState(DATA_STORE_CHECK_ERROR);
                break;
            }
            else
            {
                // ���� ������������ ���������� �������� ����� ����������� ������?
                if (CHammingCodes::GetErrorCode() != CHammingCodes::NONE_ERROR)
                {
                    // ������� ������ ������������� ���������� ��������.
                    CHammingCodes::SetErrorCode(CHammingCodes::NONE_ERROR);
                    cout << "CHammingCodes::GetErrorCode 2 uiBlock" << (int)i << endl;

                    // ��������� ��������� ������ ������ � ���������.
                    WriteBlock(auiTempArray,
                               GetBlockLength(i),
                               i);

                    SetFsmState(DATA_STORE_CHECK_ERROR);
                    break;
                }
            }

            // �������.
            // ����� ��������� � ���������� �����, ������������� ����.
            // ����������� ���������� ����� ���������.
            SetFsmState(DATA_STORE_OLD_VERSION_ACCEPTED);
        }
        break;

    case DATA_STORE_NEW_VERSION_ACCEPTED:
        // ��������� ���������.
        cerr << "CDataStoreCheck::Fsm DATA_STORE_NEW_VERSION_ACCEPTED" << endl;
//        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmOperationStatus(DATA_STORE_NEW_VERSION_ACCEPTED);
        SetFsmState(READY);
        break;

    case DATA_STORE_OLD_VERSION_ACCEPTED:
        // ��������� �� ���������.
        cerr << "CDataStoreCheck::Fsm DATA_STORE_OLD_VERSION_ACCEPTED" << endl;
//        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmOperationStatus(DATA_STORE_OLD_VERSION_ACCEPTED);
        SetFsmState(READY);
        break;

    case DATA_STORE_CHECK_OK:
        cerr << "CDataStoreCheck::Fsm DATA_STORE_CHECK_OK" << endl;
//        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
        SetFsmOperationStatus(DATA_STORE_CHECK_OK);
        SetFsmState(READY);
        break;

    case DATA_STORE_CHECK_ERROR:
        // ��������� ����������.
        cerr << "CDataStoreCheck::Fsm DATA_STORE_CHECK_ERROR" << endl;
//        ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
        SetFsmOperationStatus(DATA_STORE_CHECK_ERROR);
        SetFsmState(STOP);
        break;

    case DATA_STORE_CHECK_REPEAT:
        std::cout << "CDataStoreCheck::Fsm DATA_STORE_CHECK_REPEAT"  << std::endl;
        // ��������� �������� � ������� ��������������.
        // ��� ���� ����������� ��� ��������������?
        if (m_uiRecoveryAttemptCounter < RECOVERY_ATTEMPTS_NUMBER)
        {
            m_uiRecoveryAttemptCounter++;
            // �������� �������� ��������� ����� ��������������.
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
