//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <memory>

#include "Timer.h"
#include "Task.h"
#include "Platform.h"
#include "Resources.h"
#include "DataStore.h"
#include "DataContainer.h"
#include "Link.h"
#include "DeviceControl.h"

using namespace std;

//-------------------------------------------------------------------------------
CDeviceControl::CDeviceControl()
{
    std::cout << "CDeviceControl constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[256];
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
}

//-------------------------------------------------------------------------------
bool CDeviceControl::SetTaskData(CDataContainerDataBase* pxDataContainer)
{
    std::cout << "CDeviceControl::SetTaskData 1" << std::endl;
    uint8_t uiFsmState = GetFsmState();

    if ((uiFsmState == READY) ||
            (uiFsmState == DONE_OK) ||
            (uiFsmState == DONE_ERROR))
    {
        std::cout << "CDeviceControl::SetTaskData 2" << std::endl;
        *m_pxOperatingDataContainer = *pxDataContainer;
        SetFsmState(m_pxOperatingDataContainer -> m_uiFsmCommandState);
        return true;
    }
    else
    {
        std::cout << "CDeviceControl::SetTaskData 3" << std::endl;
        return false;
    }
}

//-------------------------------------------------------------------------------
bool CDeviceControl::GetTaskData(CDataContainerDataBase* pxDataContainer)
{
    std::cout << "CDeviceControl::SetTaskData 1" << std::endl;

    m_pxOperatingDataContainer -> m_uiFsmCommandState = GetFsmState();
    *pxDataContainer = *m_pxOperatingDataContainer;

    return true;
}

//-------------------------------------------------------------------------------
CDataContainerDataBase* CDeviceControl::GetTaskData(void)
{
    std::cout << "CDeviceControl::GetTaskData 1" << std::endl;

    m_pxOperatingDataContainer -> m_uiFsmCommandState = GetFsmState();
    m_pxOperatingDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;
    m_pxOperatingDataContainer -> m_uiDataLength =
        m_pxResources ->
        m_pxDataStore ->
        GetBlockLength(m_pxOperatingDataContainer -> m_uiDataIndex);

    return m_pxOperatingDataContainer;
}

//-------------------------------------------------------------------------------
size_t CDeviceControl::GetObjectLength(void)
{
    std::cout << "CDeviceControl GetObjectLength"  << std::endl;
    return sizeof(*this);
}

//-------------------------------------------------------------------------------
uint16_t CDeviceControl::ConfigurationRead(uint8_t *puiDestination)
{
    std::cout << "CDeviceControl::ConfigurationRead 1" << std::endl;
    uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
    memcpy(puiDestination, auiTempData, sizeof(auiTempData));
    return sizeof(auiTempData);
}

//-------------------------------------------------------------------------------
uint16_t CDeviceControl::DataBaseBlockRead(uint8_t *puiDestination, uint8_t uiBlockIndex)
{
    std::cout << "CDeviceControl::DataBaseRead 1" << std::endl;
    uint8_t uiFsmState = GetFsmState();

    if ((uiFsmState == READY) ||
            (uiFsmState == DONE_OK) ||
            (uiFsmState == DONE_ERROR))
    {
        std::cout << "CDeviceControl::DataBaseRead 2" << std::endl;
        SetFsmState(DATA_BASE_BLOCK_READ);
        return 1;
    }
    else
    {
        std::cout << "CDeviceControl::DataBaseRead 3" << std::endl;
        return 0;

    }
//    uint16_t uiLength;
//
//    uiLength = m_pxResources ->
//               m_pxDataStore ->
//               GetBlockLength(uiBlockIndex);
//
//    std::cout << "CDeviceControl::DataBaseRead uiLength " << (int)uiLength << std::endl;
//    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CDeviceControl::DataBaseBlockReadAnswer(void)
{
    std::cout << "CDeviceControl::DataBaseBlockReadAnswer 1" << std::endl;

    uint16_t uiLength;

    return uiLength;
}

//-------------------------------------------------------------------------------
uint16_t CDeviceControl::DataBaseBlockWrite(uint8_t *puiSource, uint8_t uiBlockIndex)
{
    std::cout << "CDeviceControl::DataBaseWrite 1" << std::endl;
    return 0;
}

//-------------------------------------------------------------------------------
uint8_t CDeviceControl::GetFsmOperationStatus(void)
{
//    return ((static_cast<CDataContainerDataBase*>(GetCommandDataContainerPointer())) ->
//            GetFsmOperationStatus());
}

//-------------------------------------------------------------------------------
uint8_t CDeviceControl::Fsm(void)
{
//    std::cout << "CDeviceControl::Fsm 1" << endl;
    uint8_t uiReadyTaskCounter = 0;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CDeviceControl::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CDeviceControl::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CDeviceControl::Fsm START"  << std::endl;
        Init();
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
//        std::cout << "CDeviceControl::Fsm INIT 1"  << std::endl;
        {
            CTaskInterface* pxTask =
                GetResources() ->
                GetTaskPointerByNameFromMap(m_sDataStoreName);

            if (pxTask != 0)
            {
//                std::cout << "CDeviceControl::Fsm INIT 2"  << std::endl;
                if (pxTask -> GetFsmState() >= READY)
                {
//                    std::cout << "CDeviceControl::Fsm INIT 3"  << std::endl;
                    SetDataStore((CDataStore*)pxTask);
                    uiReadyTaskCounter += 1;
//                    SetFsmState(READY);
                }
            }
            else
            {
//                std::cout << "CDeviceControl::Fsm INIT 4"  << std::endl;
                if (GetTimerPointer() -> IsOverflow())
                {
                std::cout << "CDeviceControl::Fsm INIT 5"  << std::endl;
                    SetFsmState(STOP);
                }
            }
        }

        if (uiReadyTaskCounter > 0)
        {
            SetFsmState(READY);
        }

        break;

    case READY:
//        std::cout << "CDeviceControl::Fsm READY"  << std::endl;
//    {
//
//        if (GetCommandDataLink() != 0)
//        {
//            std::cout << "CDeviceControl::Fsm READY 2"  << std::endl;
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
//        std::cout << "CDeviceControl::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CDeviceControl::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

    case DATA_BASE_BLOCK_READ:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ 1"  << std::endl;
        {
            uint16_t uiLength =
                m_pxDataStore ->
                ReadBlock(m_pxOperatingDataContainer -> m_puiDataPointer,
                          m_pxOperatingDataContainer -> m_uiDataIndex);

            if (uiLength)
            {
                std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ 2"  << std::endl;
                m_pxOperatingDataContainer -> m_uiDataLength = uiLength;
                SetFsmState(DONE_OK);
            }
            else
            {
                std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ 3"  << std::endl;
                m_pxOperatingDataContainer -> m_uiDataLength = uiLength;
//                SetFsmState(DONE_ERROR);
                SetFsmState(DONE_OK);
            }
        }
        break;

    case DATA_BASE_BLOCK_START_WRITE:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_START_WRITE"  << std::endl;
        {
            //        m_uiRequestRetryCounter = 0;
            uint8_t uiBlockIndex = m_pxOperatingDataContainer -> m_uiDataIndex;
            std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_START_WRITE uiBlockIndex "  << (int)uiBlockIndex << std::endl;

            if (m_pxDataStore ->
                    WriteBlock(m_pxOperatingDataContainer -> m_puiDataPointer,
                               (m_pxDataStore ->
                                GetBlockLength(uiBlockIndex)),
                               uiBlockIndex))
            {
                std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_START_WRITE 2"  << std::endl;
                // Установим время ожидания окончания записи.
                GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
                SetFsmState(DATA_BASE_BLOCK_WRITE_END_WAITING);
            }
            else
            {
                std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_START_WRITE 3"  << std::endl;
                SetFsmOperationStatus(DONE_ERROR);
//                SetFsmState(READY);
                SetFsmState(DONE_ERROR);
            }

        }
        break;

    case DATA_BASE_BLOCK_WRITE_END_WAITING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_END_WAITING"  << std::endl;
        // Ожидаем окончания записи автоматом устройства хранения.
        // Устройство хранения закончило запись успешно?
        if (m_pxDataStore ->
                IsDoneOk())
        {
            std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_END_WAITING 1"  << std::endl;
            SetFsmOperationStatus(DONE_OK);
//                SetFsmState(READY);
            SetFsmState(DONE_OK);
        }
        // Устройство хранения закончило запись не успешно?
        else if (m_pxDataStore ->
                 IsDoneError())
        {
            std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_END_WAITING 2"  << std::endl;
            SetFsmOperationStatus(DONE_ERROR);
//                SetFsmState(READY);
            SetFsmState(DONE_ERROR);
        }
        else
        {
            // Время ожидания окончания записи закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_END_WAITING 3"  << std::endl;
                SetFsmOperationStatus(DONE_ERROR);
//                SetFsmState(READY);
                SetFsmState(DONE_ERROR);
            }
        }
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------
