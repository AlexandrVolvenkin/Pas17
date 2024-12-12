//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
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
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CDeviceControl::~CDeviceControl()
{
    //dtor
}

////-------------------------------------------------------------------------------
//void CDeviceControl::SetResources(CResources* pxResources)
//{
//    m_pxResources = pxResources;
//}
//
////-------------------------------------------------------------------------------
//CResources* CDeviceControl::GetResources(void)
//{
//    return m_pxResources;
//}

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

    uint16_t uiLength;

    uiLength = m_pxResources ->
               m_pxDataStore ->
               GetBlockLength(uiBlockIndex);
    std::cout << "CDeviceControl::DataBaseRead uiLength " << (int)uiLength << std::endl;
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
    return ((static_cast<CDataContainerDataBase*>(GetCommandDataContainerPointer())) ->
            GetFsmOperationStatus());
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
        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CDeviceControl::Fsm INIT 1"  << std::endl;
        {
            if (GetDataStoreLink() == 0)
            {
                std::cout << "CDeviceControl::Fsm INIT 5"  << std::endl;
                CLinkInterface* pxLink = GetResources() ->
                                         CreateLinkByPerformerName(GetDataStoreLinkName());
                if (pxLink != 0)
                {
                    std::cout << "CDeviceControl::Fsm INIT 6"  << std::endl;
                    SetDataStoreLink(pxLink);
                    CDataContainerDataBase* pxDataContainerDataBase = new CDataContainerDataBase();
                    pxLink ->
                    SetDataContainer(pxDataContainerDataBase);

//                    auto pxDataContainer = std::make_shared<CDataContainerDataBase>();
//                    pxLink ->
//                    SetDataContainer(pxDataContainer.get());
                    pxLink ->
                    GetDataContainerPointer() ->
                    SetContainerData(0,
                                     0,
                                     0,
                                     0,
                                     0);
                }
                else
                {
                    std::cout << "CDeviceControl::Fsm INIT 7"  << std::endl;
                    if (GetTimerPointer() -> IsOverflow())
                    {
                        SetFsmState(STOP);
                    }
                }
            }
            else
            {
                std::cout << "CDeviceControl::Fsm INIT 8"  << std::endl;
                uiReadyTaskCounter += 1;
            }
        }

        if (uiReadyTaskCounter > 0)
        {
            SetFsmState(READY);
        }

        break;

    case READY:
//        std::cout << "CDeviceControl::Fsm READY"  << std::endl;
    {

        if (GetCommandDataLink() != 0)
        {
            std::cout << "CDeviceControl::Fsm READY 2"  << std::endl;
            SetOperatingDataLink(GetCommandDataLink());
            SetFsmState(GetCommandDataLink() ->
                        GetDataContainerPointer() ->
                        GetFsmCommandState());
            GetCommandDataLink() ->
            GetDataContainerPointer() ->
            SetFsmCommandState(0);
            SetCommandDataLink(0);
        }
    }
    break;

    case DONE_OK:
        std::cout << "CDeviceControl::Fsm DONE_OK"  << std::endl;
        SetFsmOperationStatus(DONE_OK);
        SetFsmState(READY);
        break;

    case DONE_ERROR:
        std::cout << "CDeviceControl::Fsm DONE_ERROR"  << std::endl;
        SetFsmOperationStatus(DONE_ERROR);
        SetFsmState(READY);
        break;

    case DATA_BASE_BLOCK_READ:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_READ"  << std::endl;
        {
            uint16_t uiLength = ((CDataStore*)(GetDataStoreLink() ->
                                               GetTaskPerformerPointer())) ->
                                ReadBlock(GetOperatingDataLink() ->
                                          GetDataContainerPointer() ->
                                          GetDataPointer(),
                                          GetOperatingDataLink() ->
                                          GetDataContainerPointer() ->
                                          GetDataIndex());

            GetOperatingDataLink() ->
            GetDataContainerPointer() ->
            SetDataLength(uiLength);

            SetFsmOperationStatus(DONE_OK);
            SetFsmState(READY);
        }
        break;

    case DATA_BASE_BLOCK_START_WRITE:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_START_WRITE"  << std::endl;
        {
            //        m_uiRequestRetryCounter = 0;
            uint8_t uiBlockIndex = GetOperatingDataLink() ->
                                   GetDataContainerPointer() ->
                                   GetDataIndex();
    std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_START_WRITE uiBlockIndex "  << (int)uiBlockIndex << std::endl;

            if (((CDataStore*)(GetDataStoreLink() ->
                               GetTaskPerformerPointer())) ->
                    WriteBlock(GetOperatingDataLink() ->
                               GetDataContainerPointer() ->
                               GetDataPointer(),
                               ((CDataStore*)(GetDataStoreLink() ->
                                              GetTaskPerformerPointer())) ->
                               GetBlockLength(uiBlockIndex),
                               uiBlockIndex
                              ))
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
                SetFsmState(READY);
            }

        }
        break;

    case DATA_BASE_BLOCK_WRITE_END_WAITING:
        std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_END_WAITING"  << std::endl;
        // Ожидаем окончания записи автоматом устройства хранения.
        // Устройство хранения закончило запись успешно?
        if (GetDataStoreLink() ->
                GetTaskPerformerPointer() ->
                IsDoneOk())
        {
            std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_END_WAITING 1"  << std::endl;
            SetFsmOperationStatus(DONE_OK);
            SetFsmState(READY);
        }
        // Устройство хранения закончило запись не успешно?
        else if (GetDataStoreLink() ->
                 GetTaskPerformerPointer() ->
                 IsDoneError())
        {
            std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_END_WAITING 2"  << std::endl;
            SetFsmOperationStatus(DONE_ERROR);
            SetFsmState(READY);
        }
        else
        {
            // Время ожидания окончания записи закончилось?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDeviceControl::Fsm DATA_BASE_BLOCK_WRITE_END_WAITING 3"  << std::endl;
                SetFsmOperationStatus(DONE_ERROR);
                SetFsmState(READY);
            }
        }
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------
