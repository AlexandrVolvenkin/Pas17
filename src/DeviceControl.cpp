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
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CDeviceControl::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CDeviceControl::Fsm STOP"  << std::endl;[[[]=
        SetFsmState(START);
        break;

    case START:
        std::cout << "CDeviceControl::Fsm START"  << std::endl;
//        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
                    SetFsmState(READY);
        break;

    case INIT:
        std::cout << "CDeviceControl::Fsm INIT 1"  << std::endl;
//        {
//            CTaskInterface* pxTask =
//                GetResources() ->
//                GetCommonTaskFromMapPointer(m_sModbusSlaveLinkLayerName);
//
//            if (pxTask != 0)
//            {
//                std::cout << "CDeviceControl::Fsm INIT 2"  << std::endl;
//                if (pxTask -> GetFsmState() >= READY)
//                {
//                    SetModbusSlaveLinkLayer((CDeviceControlLinkLayer*)pxTask);
//                    SetFsmState(READY);
//                    std::cout << "CDeviceControl::Fsm READY"  << std::endl;
//                }
//            }
//            else
//            {
//                std::cout << "CDeviceControl::Fsm INIT 3"  << std::endl;
//                if (GetTimerPointer() -> IsOverflow())
//                {
//                    SetFsmState(STOP);
//                    std::cout << "CDeviceControl::Fsm STOP"  << std::endl;
//                }
//            }
//        }
//
//
//        {
//            CTaskInterface* pxTask =
//                GetResources() ->
//                GetCommonTaskFromMapPointer(m_sDeviceControlName);
//
//            if (pxTask != 0)
//            {
//                std::cout << "CDeviceControl::Fsm INIT 2"  << std::endl;
//                if (pxTask -> GetFsmState() >= READY)
//                {
//                    SetDeviceControl((CDeviceControl*)pxTask);
//                    SetFsmState(READY);
//                    std::cout << "CDeviceControl::Fsm READY"  << std::endl;
//                }
//            }
//            else
//            {
//                std::cout << "CDeviceControl::Fsm INIT 3"  << std::endl;
//                if (GetTimerPointer() -> IsOverflow())
//                {
//                    SetFsmState(STOP);
//                    std::cout << "CDeviceControl::Fsm STOP"  << std::endl;
//                }
//            }
//        }

        break;

    case READY:
//        std::cout << "CDeviceControl::Fsm READY"  << std::endl;
//        {
//            if (m_pxCommandDataContainer != 0)
//            {
//                std::cout << "CDeviceControl::Fsm READY 2"  << std::endl;
//                m_pxOperatingDataContainer = m_pxCommandDataContainer;
//                SetFsmState(GetFsmCommandState());
//                SetFsmCommandState(0);
//                m_pxCommandDataContainer = 0;
//            }
//        }


//        {
//            if (GetFsmCommandState() != 0)
//            {
//                SetFsmState(GetFsmCommandState());
//                SetFsmCommandState(0);
//            }
//        }
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

    default:
        break;
    }
}

//-------------------------------------------------------------------------------
