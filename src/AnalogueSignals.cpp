
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <typeinfo>
#include <array>

#include "Task.h"
#include "Platform.h"
#include "Resources.h"
#include "DataContainer.h"
#include "DeviceControl.h"
#include "Configuration.h"
#include "STEP5_floating_point.h"
#include "InternalModule.h"
#include "InternalModuleMuvr.h"
#include "DeviceControl.h"
#include "AnalogueSignals.h"


using namespace std;

//-------------------------------------------------------------------------------
CAnalogueSignals::CAnalogueSignals()
{
    std::cout << "CAnalogueSignals constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[1024];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CAnalogueSignals::~CAnalogueSignals()
{
    delete[] m_puiIntermediateBuff;
}

//-------------------------------------------------------------------------------
uint8_t CAnalogueSignals::Init(void)
{
    std::cout << "CAnalogueSignals Init"  << std::endl;
    m_pxOperatingDataContainer = static_cast<CDataContainerDataBase*>(GetResources() ->
                                 AddDataContainer(std::make_shared<CDataContainerDataBase>()));

}

//-------------------------------------------------------------------------------
bool CAnalogueSignals::SetTaskData(CDataContainerDataBase* pxDataContainer)
{
    std::cout << "CAnalogueSignals::SetTaskData 1" << std::endl;

    if (IsTaskReady())
    {
        std::cout << "CAnalogueSignals::SetTaskData 2" << std::endl;
        *m_pxOperatingDataContainer = *pxDataContainer;
        SetFsmState(m_pxOperatingDataContainer -> m_uiFsmCommandState);
        return true;
    }
    else
    {
        std::cout << "CAnalogueSignals::SetTaskData 3" << std::endl;
        return false;
    }
}

//-------------------------------------------------------------------------------
bool CAnalogueSignals::GetTaskData(CDataContainerDataBase* pxDataContainer)
{
//    std::cout << "CAnalogueSignals::GetTaskData 1" << std::endl;

    m_pxOperatingDataContainer -> m_uiFsmCommandState = GetFsmState();
    *pxDataContainer = *m_pxOperatingDataContainer;

    return true;
}

//-------------------------------------------------------------------------------
void CAnalogueSignals::Allocate(void)
{
    std::cout << "CAnalogueSignals::Allocate 1"  << std::endl;
}

//-------------------------------------------------------------------------------
// преобразовывает из общего формата базы данных, в формат хранения в RAM, IEEE754.
// с текстовым реквизитом.
// во первых параметры описателей аналогового входа используют разные типы модулей,
// поэтому удобно иметь в RAM данные в нужном формате. во вторых, данные в этом
// формате должны быть доступны по протоколу Modbus.
void CAnalogueSignals::Exstract(void)
{
    std::cout << "CAnalogueSignals::Exstract 1"  << std::endl;
    uint8_t nucBlockCounter;
    uint8_t nucBlocksInBlockCounter; // один описатель - один блок, в общем блоке.
    TAnalogueInputDescriptionDataBasePackOne *pxSourse;
//    TAnalogueInputDescriptionWork *pxDestination;

//    pxDestination =
//        axAnalogueInputDescriptionWork;
//    pxSourse = (TAnalogueInputDescriptionDataBasePackOne*)(&xPlcDataBase.
//               axPlcDataBaseBlocks[ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET].
//               aucPlcDataBaseBlockData[0]);
    pxSourse = (TAnalogueInputDescriptionDataBasePackOne*)(m_puiIntermediateBuff);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;

    for (int i = 0;
            i < (m_pxOperatingDataContainer -> m_uiDataLength);
            i++)
    {
//        // обработан весь блок базы данных?
//        if (nucBlocksInBlockCounter == ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
//        {
//            // следующий блок базы данных.
//            nucBlockCounter++;
////            pxSourse = (TAnalogueInputDescriptionDataBasePackOne*)(&xPlcDataBase.
////                       axPlcDataBaseBlocks[ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
////                       aucPlcDataBaseBlockData[0]);
//            nucBlocksInBlockCounter = 0;
//        }

        TAnalogueInputDescriptionWork xAnalogueInputDescriptionWork;

        xAnalogueInputDescriptionWork.ucScaleType =
            (pxSourse[nucBlocksInBlockCounter].
             ucServiseL & ANALOGUE_INPUT_SCALE_TYPE_MASK);
        xAnalogueInputDescriptionWork.ucDimentionCode =
            ((pxSourse[nucBlocksInBlockCounter].
              ucServiseL & ANALOGUE_INPUT_DIMENTION_CODE_MASK) >> 4);
        xAnalogueInputDescriptionWork.ucRangeCode =
            (pxSourse[nucBlocksInBlockCounter].
             ucServiseH & ANALOGUE_INPUT_RANGE_CODE_MASK);

        xAnalogueInputDescriptionWork.fMinOfScale =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucMinOfScaleMantissLsb));
        xAnalogueInputDescriptionWork.fMaxOfScale =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucMaxOfScaleMantissLsb));
        xAnalogueInputDescriptionWork.fSetPointLL =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucSetPointLLMantissLsb));
        xAnalogueInputDescriptionWork.fSetPointL =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucSetPointLMantissLsb));
        xAnalogueInputDescriptionWork.fSetPointH =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucSetPointHMantissLsb));
        xAnalogueInputDescriptionWork.fSetPointHH =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucSetPointHHMantissLsb));

        memcpy((&xAnalogueInputDescriptionWork.acTextDescriptor),
               (&pxSourse[nucBlocksInBlockCounter].
                acTextDescriptor),
               ANALOGUE_INPUT_NAME_LENGTH);
        xAnalogueInputDescriptionWork.
        acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH] =
            END_OF_STRING;

        GetResources() ->
        m_pxAnalogueInputDescriptionWork[i] = xAnalogueInputDescriptionWork;


        nucBlocksInBlockCounter++;
    }

//    {
//        std::cout << "CAnalogueSignals::Exstract m_pxAnalogueInputDescriptionWork"  << std::endl;
//        uint8_t *pucSourceTemp;
//        pucSourceTemp = (uint8_t*)(GetResources() -> m_pxAnalogueInputDescriptionWork);
//        for(int i=0; i<64; )
//        {
//            for(int j=0; j<8; j++)
//            {
//                cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//            }
//            cout << endl;
//            i += 8;
//        }
//    }

}

//-------------------------------------------------------------------------------
uint8_t CAnalogueSignals::Fsm(void)
{
////    std::cout << "CAnalogueSignals::Fsm 1" << endl;
////    uint8_t auiIntermediateBuff[256];
//
//    switch (GetFsmState())
//    {
//    case IDDLE:
////        std::cout << "CAnalogueSignals::Fsm IDDLE"  << std::endl;
//        break;
//
//    case STOP:
////        //std::cout << "CAnalogueSignals::Fsm STOP"  << std::endl;
//        break;
//
//    case START:
//        std::cout << "CAnalogueSignals::Fsm START"  << std::endl;
//        Init();
//        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
//        SetFsmState(INIT);
//        break;
//
//    case INIT:
////        std::cout << "CAnalogueSignals::Fsm INIT 1"  << std::endl;
//        m_uiDeviceControlId =
//            GetResources() ->
//            GetTaskIdByNameFromMap(m_sDeviceControlName);
//        SetFsmState(READY);
//
//        break;
//
//    case READY:
//        break;
//
//    case DONE_OK:
////        std::cout << "CAnalogueSignals::Fsm DONE_OK"  << std::endl;
//        break;
//
//    case DONE_ERROR:
////        std::cout << "CAnalogueSignals::Fsm DONE_ERROR"  << std::endl;
//        break;
//
//    case DATA_BASE_BLOCK_START_READ:
//        std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_START_READ"  << std::endl;
//
//        {
//            CInternalModuleMuvr* pxInternalModuleMuvr =
//                (CInternalModuleMuvr*)(GetResources() ->
//                                       GetTaskPointerByNameFromMap("InternalModuleMuvr"));
//
//            m_pxOperatingDataContainer -> m_uiFsmCommandState =
//                CInternalModuleMuvr::MUVR_DATA_BASE_READ;
//            m_pxOperatingDataContainer -> m_puiDataPointer =
//                m_puiIntermediateBuff;
//            pxInternalModuleMuvr ->
//            SetTaskData(m_pxOperatingDataContainer);
//            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
//            SetFsmState(DATA_BASE_BLOCK_READ_END_WAITING);
//        }
//        break;
//
//    case DATA_BASE_BLOCK_READ_END_WAITING:
////        std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_READ_END_WAITING"  << std::endl;
//    {
//        CInternalModuleMuvr* pxInternalModuleMuvr =
//            (CInternalModuleMuvr*)(GetResources() ->
//                                   GetTaskPointerByNameFromMap("InternalModuleMuvr"));
//
//        pxInternalModuleMuvr ->
//        GetTaskData(m_pxOperatingDataContainer);
//
//        uint8_t uiFsmState = m_pxOperatingDataContainer -> m_uiFsmCommandState;
//
//        if (uiFsmState == DONE_OK)
//        {
//            std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_READ_END_WAITING 2"  << std::endl;
//
//            {
//                std::cout << "CAnalogueSignals::Fsm m_puiIntermediateBuff"  << std::endl;
//                uint8_t *pucSourceTemp;
//                pucSourceTemp = (uint8_t*)m_puiIntermediateBuff;
//                for(int i=0; i<64; )
//                {
//                    for(int j=0; j<8; j++)
//                    {
//                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                    }
//                    cout << endl;
//                    i += 8;
//                }
//            }
//
//            Exstract();
//            SetFsmState(DONE_OK);
//        }
//        else if (uiFsmState == DONE_ERROR)
//        {
//            std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_READ_END_WAITING 3"  << std::endl;
//            SetFsmState(DONE_ERROR);
//        }
//        else
//        {
//            // Время ожидания выполнения запроса закончилось?
//            if (GetTimerPointer() -> IsOverflow())
//            {
//                std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_READ_END_WAITING 4"  << std::endl;
//                SetFsmState(DONE_ERROR);
//            }
//        }
//    }
//    break;
//
//    case DATA_BASE_BLOCK_STORAGE_START_WRITE:
//        std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_STORAGE_START_WRITE"  << std::endl;
//
//        {
//            CDeviceControl* pxDeviceControl =
//                (CDeviceControl*)(GetResources() ->
//                                  GetTaskPointerByNameFromMap("DeviceControlRtuUpperLevel"));
//
//            m_pxOperatingDataContainer -> m_uiFsmCommandState =
//                CDeviceControl::DATA_BASE_BLOCK_WRITE_START;
//            m_pxOperatingDataContainer -> m_uiDataIndex = 1;
//            m_pxOperatingDataContainer -> m_puiDataPointer =
//                m_puiIntermediateBuff;
//            pxDeviceControl ->
//            SetTaskData(m_pxOperatingDataContainer);
//            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
//            SetFsmState(DATA_BASE_BLOCK_STORAGE_WRITE_END_WAITING);
//
//
//        CDataContainerDataBase* pxDataContainer =
//            (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//        pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
//        pxDataContainer -> m_uiFsmCommandState =
//            CDeviceControl::DATA_BASE_BLOCK_READ;
//        pxDataContainer -> m_uiDataIndex = uiBlockIndex;
//        pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;
//
//        if (SetTaskData(GetExecutorDataContainerPointer()))
//        {
//            //std::cout << "CModbusSlave::DataBaseRead 4" << std::endl;
//            SetFsmState(REQUEST_PROCESSING_EXECUTOR_DONE_CHECK_START);
//        }
//        else
//        {
//            //std::cout << "CModbusSlave::DataBaseRead 5" << std::endl;
//            SetFsmState(REQUEST_PROCESSING_EXECUTOR_READY_CHECK_START);
//        }
//        }
//        break;
//
//    case DATA_BASE_BLOCK_STORAGE_WRITE_END_WAITING:
////        std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_STORAGE_WRITE_END_WAITING"  << std::endl;
//    {
//        CDeviceControl* pxDeviceControl =
//            (CDeviceControl*)(GetResources() ->
//                              GetTaskPointerByNameFromMap("DeviceControlRtuUpperLevel"));
//
//        pxDeviceControl ->
//        GetTaskData(m_pxOperatingDataContainer);
//
//        uint8_t uiFsmState = m_pxOperatingDataContainer -> m_uiFsmCommandState;
//
//        if (uiFsmState == DONE_OK)
//        {
//            std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_STORAGE_WRITE_END_WAITING 2"  << std::endl;
//            std::cout << "CAnalogueSignals::Fsm m_uiDataLength "  << (int)(m_pxOperatingDataContainer -> m_uiDataLength) << std::endl;
//            {
//                std::cout << "CAnalogueSignals::Fsm m_puiIntermediateBuff"  << std::endl;
//                uint8_t *pucSourceTemp;
//                pucSourceTemp = (uint8_t*)m_puiIntermediateBuff;
//                for(int i=0; i<64; )
//                {
//                    for(int j=0; j<8; j++)
//                    {
//                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                    }
//                    cout << endl;
//                    i += 8;
//                }
//            }
//
//            SetFsmState(DONE_OK);
//        }
//        else if (uiFsmState == DONE_ERROR)
//        {
//            std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_STORAGE_WRITE_END_WAITING 3"  << std::endl;
//            SetFsmState(DONE_ERROR);
//        }
//        else
//        {
//            // Время ожидания выполнения запроса закончилось?
//            if (GetTimerPointer() -> IsOverflow())
//            {
//                std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_STORAGE_WRITE_END_WAITING 4"  << std::endl;
//                SetFsmState(DONE_ERROR);
//            }
//        }
//    }
//    break;
//
//    case DATA_BASE_BLOCK_MODULE_START_WRITE:
//        std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_MODULE_START_WRITE"  << std::endl;
//
//        {
//            CInternalModuleMuvr* pxInternalModuleMuvr =
//                (CInternalModuleMuvr*)(GetResources() ->
//                                       GetTaskPointerByNameFromMap("InternalModuleMuvr"));
//
//            m_pxOperatingDataContainer -> m_uiFsmCommandState =
//                CInternalModuleMuvr::MUVR_WRITE_DATA_BASE;
//            m_pxOperatingDataContainer -> m_uiDataIndex = 1;
//            m_pxOperatingDataContainer -> m_puiDataPointer =
//                m_puiIntermediateBuff;
//            pxInternalModuleMuvr ->
//            SetTaskData(m_pxOperatingDataContainer);
//            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
//            SetFsmState(DATA_BASE_BLOCK_MODULE_WRITE_END_WAITING);
//        }
//        break;
//
//    case DATA_BASE_BLOCK_MODULE_WRITE_END_WAITING:
////        std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_MODULE_WRITE_END_WAITING"  << std::endl;
//    {
//        CInternalModuleMuvr* pxInternalModuleMuvr =
//            (CInternalModuleMuvr*)(GetResources() ->
//                                   GetTaskPointerByNameFromMap("InternalModuleMuvr"));
//
//        pxInternalModuleMuvr ->
//        GetTaskData(m_pxOperatingDataContainer);
//
//        uint8_t uiFsmState = m_pxOperatingDataContainer -> m_uiFsmCommandState;
//
//        if (uiFsmState == DONE_OK)
//        {
//            std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_MODULE_WRITE_END_WAITING 2"  << std::endl;
//            std::cout << "CAnalogueSignals::Fsm m_uiDataLength "  << (int)(m_pxOperatingDataContainer -> m_uiDataLength) << std::endl;
//            {
//                std::cout << "CAnalogueSignals::Fsm m_puiIntermediateBuff"  << std::endl;
//                uint8_t *pucSourceTemp;
//                pucSourceTemp = (uint8_t*)m_puiIntermediateBuff;
//                for(int i=0; i<64; )
//                {
//                    for(int j=0; j<8; j++)
//                    {
//                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                    }
//                    cout << endl;
//                    i += 8;
//                }
//            }
//
//            SetFsmState(DONE_OK);
//        }
//        else if (uiFsmState == DONE_ERROR)
//        {
//            std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_MODULE_WRITE_END_WAITING 3"  << std::endl;
//            SetFsmState(DONE_ERROR);
//        }
//        else
//        {
//            // Время ожидания выполнения запроса закончилось?
//            if (GetTimerPointer() -> IsOverflow())
//            {
//                std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_MODULE_WRITE_END_WAITING 4"  << std::endl;
//                SetFsmState(DONE_ERROR);
//            }
//        }
//    }
//    break;
//
//    case DATA_BASE_BLOCK_CHECK_START:
//        std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_START"  << std::endl;
//        SetFsmState(DATA_BASE_BLOCK_CHECK_MODULE_BLOCK_READ);
//        break;
//
//    case DATA_BASE_BLOCK_CHECK_MODULE_BLOCK_READ:
//        std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_MODULE_BLOCK_READ"  << std::endl;
//
//        {
//            CInternalModuleMuvr* pxInternalModuleMuvr =
//                (CInternalModuleMuvr*)(GetResources() ->
//                                       GetTaskPointerByNameFromMap("InternalModuleMuvr"));
//
//            m_pxOperatingDataContainer -> m_uiFsmCommandState =
//                CInternalModuleMuvr::MUVR_DATA_BASE_READ;
//            m_pxOperatingDataContainer -> m_puiDataPointer =
//                m_puiIntermediateBuff;
//            pxInternalModuleMuvr ->
//            SetTaskData(m_pxOperatingDataContainer);
//            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
//            SetFsmState(DATA_BASE_BLOCK_CHECK_MODULE_BLOCK_READ_END_WAITING);
//        }
//        break;
//
//    case DATA_BASE_BLOCK_CHECK_MODULE_BLOCK_READ_END_WAITING:
////        std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_MODULE_BLOCK_READ_END_WAITING"  << std::endl;
//    {
//        CInternalModuleMuvr* pxInternalModuleMuvr =
//            (CInternalModuleMuvr*)(GetResources() ->
//                                   GetTaskPointerByNameFromMap("InternalModuleMuvr"));
//
//        pxInternalModuleMuvr ->
//        GetTaskData(m_pxOperatingDataContainer);
//
//        uint8_t uiFsmState = m_pxOperatingDataContainer -> m_uiFsmCommandState;
//
//        if (uiFsmState == DONE_OK)
//        {
//            std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_MODULE_BLOCK_READ_END_WAITING 2"  << std::endl;
//
//            {
//                std::cout << "CAnalogueSignals::Fsm m_puiIntermediateBuff"  << std::endl;
//                uint8_t *pucSourceTemp;
//                pucSourceTemp = (uint8_t*)m_puiIntermediateBuff;
//                for(int i=0; i<192; )
//                {
//                    for(int j=0; j<8; j++)
//                    {
//                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                    }
//                    cout << endl;
//                    i += 8;
//                }
//            }
//
//            Exstract();
//            memcpy(&m_puiIntermediateBuff[256],
//                   &m_puiIntermediateBuff[0],
//                   256);
//            SetFsmState(DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_READ);
//        }
//        else if (uiFsmState == DONE_ERROR)
//        {
//            std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_MODULE_BLOCK_READ_END_WAITING 3"  << std::endl;
//            SetFsmState(DONE_ERROR);
//        }
//        else
//        {
//            // Время ожидания выполнения запроса закончилось?
//            if (GetTimerPointer() -> IsOverflow())
//            {
//                std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_MODULE_BLOCK_READ_END_WAITING 4"  << std::endl;
//                SetFsmState(DONE_ERROR);
//            }
//        }
//    }
//    break;
//
//    case DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_READ:
//        std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_READ"  << std::endl;
//
//        {
//            CDeviceControl* pxDeviceControl =
//                (CDeviceControl*)(GetResources() ->
//                                  GetTaskPointerByNameFromMap("DeviceControlRtuUpperLevel"));
//
//            m_pxOperatingDataContainer -> m_uiFsmCommandState =
//                CDeviceControl::DATA_BASE_BLOCK_READ;
//            m_pxOperatingDataContainer -> m_uiDataIndex = 1;
//            m_pxOperatingDataContainer -> m_puiDataPointer =
//                m_puiIntermediateBuff;
//            pxDeviceControl ->
//            SetTaskData(m_pxOperatingDataContainer);
//            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
//            SetFsmState(DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_READ_END_WAITING);
//        }
//        break;
//
//    case DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_READ_END_WAITING:
////        std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_READ_END_WAITING"  << std::endl;
//    {
//        CDeviceControl* pxDeviceControl =
//            (CDeviceControl*)(GetResources() ->
//                              GetTaskPointerByNameFromMap("DeviceControlRtuUpperLevel"));
//
//        pxDeviceControl ->
//        GetTaskData(m_pxOperatingDataContainer);
//
//        uint8_t uiFsmState = m_pxOperatingDataContainer -> m_uiFsmCommandState;
//
//        if (uiFsmState == DONE_OK)
//        {
//            std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_READ_END_WAITING 2"  << std::endl;
//            std::cout << "CAnalogueSignals::Fsm m_uiDataLength "  << (int)(m_pxOperatingDataContainer -> m_uiDataLength) << std::endl;
//            {
//                std::cout << "CAnalogueSignals::Fsm m_puiIntermediateBuff"  << std::endl;
//                uint8_t *pucSourceTemp;
//                pucSourceTemp = (uint8_t*)m_puiIntermediateBuff;
//                for(int i=0; i<192; )
//                {
//                    for(int j=0; j<8; j++)
//                    {
//                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                    }
//                    cout << endl;
//                    i += 8;
//                }
//            }
//
//            SetFsmState(DATA_BASE_BLOCK_CHECK_MODULE_AND_STORAGE_BLOCK_COMPARE);
//        }
//        else if (uiFsmState == DONE_ERROR)
//        {
//            std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_READ_END_WAITING 3"  << std::endl;
//            SetFsmState(DONE_ERROR);
//        }
//        else
//        {
//            // Время ожидания выполнения запроса закончилось?
//            if (GetTimerPointer() -> IsOverflow())
//            {
//                std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_READ_END_WAITING 4"  << std::endl;
//                SetFsmState(DONE_ERROR);
//            }
//        }
//    }
//    break;
//
//    case DATA_BASE_BLOCK_CHECK_MODULE_AND_STORAGE_BLOCK_COMPARE:
//        std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_MODULE_AND_STORAGE_BLOCK_COMPARE 1"  << std::endl;
//        // база данных совпадает?
//        if (memcmp((const void*)&m_puiIntermediateBuff[256],
//                   (const void*)&m_puiIntermediateBuff[0],
//                   168) == 0)
//        {
//            std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_MODULE_AND_STORAGE_BLOCK_COMPARE 2"  << std::endl;
//            SetFsmState(DONE_OK);
//        }
//        else
//        {
//            std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_MODULE_AND_STORAGE_BLOCK_COMPARE 3"  << std::endl;
//            SetFsmState(DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_START_WRITE);
//        }
//        break;
//
//    case DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_START_WRITE:
//        std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_START_WRITE"  << std::endl;
//
//        {
//            CDeviceControl* pxDeviceControl =
//                (CDeviceControl*)(GetResources() ->
//                                  GetTaskPointerByNameFromMap("DeviceControlRtuUpperLevel"));
//
//            m_pxOperatingDataContainer -> m_uiFsmCommandState =
//                CDeviceControl::DATA_BASE_BLOCK_WRITE_START;
//            m_pxOperatingDataContainer -> m_uiDataIndex = 1;
//            m_pxOperatingDataContainer -> m_puiDataPointer =
//                &m_puiIntermediateBuff[256];
//            pxDeviceControl ->
//            SetTaskData(m_pxOperatingDataContainer);
//            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
//            SetFsmState(DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_WRITE_END_WAITING);
//        }
//        break;
//
//    case DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_WRITE_END_WAITING:
////        std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_WRITE_END_WAITING"  << std::endl;
//    {
//        CDeviceControl* pxDeviceControl =
//            (CDeviceControl*)(GetResources() ->
//                              GetTaskPointerByNameFromMap("DeviceControlRtuUpperLevel"));
//
//        pxDeviceControl ->
//        GetTaskData(m_pxOperatingDataContainer);
//
//        uint8_t uiFsmState = m_pxOperatingDataContainer -> m_uiFsmCommandState;
//
//        if (uiFsmState == DONE_OK)
//        {
//            std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_WRITE_END_WAITING 2"  << std::endl;
////            std::cout << "CAnalogueSignals::Fsm m_uiDataLength "  << (int)(m_pxOperatingDataContainer -> m_uiDataLength) << std::endl;
////            {
////                std::cout << "CAnalogueSignals::Fsm m_puiIntermediateBuff"  << std::endl;
////                uint8_t *pucSourceTemp;
////                pucSourceTemp = (uint8_t*)m_puiIntermediateBuff;
////                for(int i=0; i<64; )
////                {
////                    for(int j=0; j<8; j++)
////                    {
////                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
////                    }
////                    cout << endl;
////                    i += 8;
////                }
////            }
//
//            SetFsmState(DONE_OK);
//        }
//        else if (uiFsmState == DONE_ERROR)
//        {
//            std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_WRITE_END_WAITING 3"  << std::endl;
//            SetFsmState(DONE_ERROR);
//        }
//        else
//        {
//            // Время ожидания выполнения запроса закончилось?
//            if (GetTimerPointer() -> IsOverflow())
//            {
//                std::cout << "CAnalogueSignals::Fsm DATA_BASE_BLOCK_CHECK_STORAGE_BLOCK_WRITE_END_WAITING 4"  << std::endl;
//                SetFsmState(DONE_ERROR);
//            }
//        }
//    }
//    break;
//
//    default:
//        break;
//    }
}

//-------------------------------------------------------------------------------
