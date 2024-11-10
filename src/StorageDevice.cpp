
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
#include "DataStore.h"
#include "DataContainer.h"
#include "StorageDevice.h"

//-------------------------------------------------------------------------------
CStorageDeviceFileSystem::CStorageDeviceFileSystem()
{
    std::cout << "CStorageDeviceFileSystem constructor"  << std::endl;
    // получим имя класса.
    sprintf(GetTaskNamePointer(),
            "%s",
            typeid(*this).name());
    m_pxDataContainer = std::make_shared<CDataContainerDataBase>();
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CStorageDeviceFileSystem::~CStorageDeviceFileSystem()
{

}

////-------------------------------------------------------------------------------
//// Передаёт данные контекста записи блока автомату устройства хранения и запускает процесс записи.
//uint8_t CStorageDeviceFileSystem::PassingDataAndStartWrite(uint16_t uiOffset, uint8_t *puiSource, uint16_t uiLength)
//{
//    SetOffset(uiOffset);
//    SetBufferPointer(puiSource);
//    SetLength(uiLength);
//
//    if (GetFsmState() == IDDLE)
//    {
//        SetFsmState(WRITE_DATA_START);
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}

//-------------------------------------------------------------------------------
void CStorageDeviceFileSystem::GetArgumentData(void)
{
//        // Создание промежуточного указателя
//        std::unique_ptr<CDataContainerDataBase> pxDataContainer = m_pxDataContainer; // Указатель на объект CDataContainerDataBase
    CDataContainerDataBase* pxDataContainer =
        (static_cast<CDataContainerDataBase*>(m_pxDataContainer.get()));

    if (pxDataContainer -> m_uiFsmCommandState != 0)
    {
        SetFsmState(pxDataContainer -> m_uiFsmCommandState);
        pxDataContainer -> m_uiFsmCommandState = 0;
    }

    m_puiBuffer = m_pxTaskCustomer -> GetArgumentDataPointer() -> m_puiDataPointer;
    m_uiOffset = m_pxTaskCustomer -> GetArgumentDataPointer() -> m_uiDataOffset;
    m_uiLength = m_pxTaskCustomer -> GetArgumentDataPointer() -> m_uiDataLength;
}

//-------------------------------------------------------------------------------
// Передаёт данные контекста записи блока автомату устройства хранения и запускает процесс записи.
bool CStorageDeviceFileSystem::WriteBlock(uint8_t *puiSource, uint16_t uiOffset, uint16_t uiLength)
{
    std::cout << "CStorageDeviceFileSystem WriteBlock uiOffset " << (int)uiOffset  << std::endl;
    std::cout << "CStorageDeviceFileSystem WriteBlock uiLength " << (int)uiLength  << std::endl;
    if ((GetFsmState() == READY) ||
            (GetFsmState() == WRITE_DATA_ERROR) ||
            (GetFsmState() == READ_DATA_ERROR))
    {
        std::cout << "CStorageDeviceFileSystem WriteBlock 1"  << std::endl;
        m_uiOffset = uiOffset;
        m_puiBuffer = puiSource;
        m_uiLength = uiLength;
        SetFsmState(WRITE_DATA_START);
//        return true;
        return Write();
    }
    else
    {
        std::cout << "CStorageDeviceFileSystem WriteBlock 2"  << std::endl;
        return false;
    }
}

//-------------------------------------------------------------------------------
// Записывает блок данных в устройство хранения.
uint8_t CStorageDeviceFileSystem::Write(void)
{
    std::cout << "CStorageDeviceFileSystem Write"  << std::endl;
    CDataContainerDataBase* pxDataContainer =
        (static_cast<CDataContainerDataBase*>(m_pxDataContainer.get()));

    uint8_t *puiDataPointer = pxDataContainer -> m_puiDataPointer;
    uint16_t uiOffset = pxDataContainer -> m_uiDataOffset;
    uint16_t uiLength = pxDataContainer -> m_uiDataLength;

//    cout << "CStorageDeviceFileSystem::Write uiOffset" << " " << (int)uiOffset << endl;
//    cout << "CStorageDeviceFileSystem::Write uiLength" << " " << (int)uiLength << endl;
    if ((uiOffset + uiLength) < MAX_BUFFER_LENGTH)
    {
        std::cout << "CStorageDeviceFileSystem Write 1"  << std::endl;
        ofstream outdata;
        // Чтобы добавить и не стереть старые данные откроем файл на чтение и запись.
        outdata.open(pccFileName, (ios::binary | ios::in | ios::out));
        // Файл не существует?
        if (!outdata)
        {
            std::cout << "CStorageDeviceFileSystem Write 2"  << std::endl;
            cerr << "CStorageDeviceFileSystem::Write Error: file could not be opened" << endl;
            // чтобы создать файл откроем только на запись.
            outdata.open(pccFileName, (ios::binary | ios::out));
            // Файл не создан?
            if (!outdata)
            {
                std::cout << "CStorageDeviceFileSystem Write 3"  << std::endl;
                cerr << "CStorageDeviceFileSystem::Write Error: file could not be created" << endl;
                return 0;
            }
        }

        std::cout << "CStorageDeviceFileSystem Write 4 1"  << std::endl;
        outdata.seekp(uiOffset, ios_base::beg);
        std::cout << "CStorageDeviceFileSystem Write 4"  << std::endl;
        outdata.write((char*)puiDataPointer, uiLength);

        std::cout << "CStorageDeviceFileSystem Write 5"  << std::endl;
        // закроем файл.
        outdata.close();

        std::cout << "CStorageDeviceFileSystem Write 6"  << std::endl;
//        SetIsDataWrited(true);
        return 1;
    }
    else
    {
        std::cout << "CStorageDeviceFileSystem Write 7"  << std::endl;
        return 0;
    }
}

//-------------------------------------------------------------------------------
// Передаёт данные контекста записи блока автомату устройства хранения и запускает процесс записи.
bool CStorageDeviceFileSystem::ReadBlock(uint8_t *puiDestination, uint16_t uiOffset, uint16_t uiLength)
{
    std::cout << "CStorageDeviceFileSystem ReadBlock 1"  << std::endl;
    if ((GetFsmState() == READY) ||
            (GetFsmState() == WRITE_DATA_ERROR) ||
            (GetFsmState() == READ_DATA_ERROR))
    {
        std::cout << "CStorageDeviceFileSystem ReadBlock 2"  << std::endl;
        m_uiOffset = uiOffset;
        m_puiBuffer = puiDestination;
        m_uiLength = uiLength;
        return Read();
//        SetFsmState(READ_DATA_START);
//        return true;
    }
    else
    {
        std::cout << "CStorageDeviceFileSystem ReadBlock 3"  << std::endl;
        return false;
    }
}

//-------------------------------------------------------------------------------
// Считывает блок данных из устройства хранения.
uint8_t CStorageDeviceFileSystem::Read(void)
{
    std::cout << "CStorageDeviceFileSystem Read 1"  << std::endl;
    CDataContainerDataBase* pxDataContainer =
        (static_cast<CDataContainerDataBase*>(m_pxDataContainer.get()));

    uint8_t *puiDataPointer = pxDataContainer -> m_puiDataPointer;
    uint16_t uiOffset = pxDataContainer -> m_uiDataOffset;
    uint16_t uiLength = pxDataContainer -> m_uiDataLength;

//    cout << "CStorageDeviceFileSystem::Read uiOffset" << " " << (int)uiOffset << endl;
//    cout << "CStorageDeviceFileSystem::Read uiLength" << " " << (int)uiLength << endl;
    if ((uiOffset + uiLength) < MAX_BUFFER_LENGTH)
    {
        std::cout << "CStorageDeviceFileSystem Read 2"  << std::endl;
        ifstream indata;
        // откроем файл.
        indata.open(pccFileName, (ios::in | ios::binary));
        if (!indata)
        {
            std::cout << "CStorageDeviceFileSystem Read 3"  << std::endl;
            cerr << "CStorageDeviceFileSystem::Read Error: file could not be opened" << endl;
            return 0;
        }
        else
        {
            std::cout << "CStorageDeviceFileSystem Read 4"  << std::endl;
            // установим смещение в файле.
            indata.seekg(uiOffset, ios_base::beg);
            // прочитаем файл.
            indata.read(reinterpret_cast<char*>(puiDataPointer),
                        uiLength);
        }
        // закроем файл.
        indata.close();
        return 1;
    }
    else
    {
        std::cout << "CStorageDeviceFileSystem Read 5"  << std::endl;
        return 0;
    }
}

//---------------------------------------------------------------------------------
uint8_t CStorageDeviceFileSystem::Fsm(void)
{
//    std::cout << "CStorageDeviceFileSystem::Fsm 1" << endl;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CStorageDeviceFileSystem::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CStorageDeviceFileSystem::Fsm STOP"  << std::endl;[[[]=
        break;

    case START:
        std::cout << "CStorageDeviceFileSystem::Fsm START"  << std::endl;
        std::cout << "CStorageDeviceFileSystem::Fsm m_sTaskCustomerName" << " " << (m_sTaskCustomerName) << std::endl;
//        GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
        SetFsmState(INIT);
        break;

    case INIT:
        //std::cout << "CStorageDeviceFileSystem::Fsm INIT"  << std::endl;
        SetFsmState(READY);
        break;

    case READY:
        //std::cout << "CStorageDeviceFileSystem::Fsm READY"  << std::endl;
    {
////        // Создание промежуточного указателя
////        std::unique_ptr<CDataContainerDataBase> pxDataContainer = m_pxDataContainer; // Указатель на объект CDataContainerDataBase
//        CDataContainerDataBase* pxDataContainer =
//            (static_cast<CDataContainerDataBase*>(m_pxDataContainer.get()));

//        if (pxDataContainer -> m_uiFsmCommandState != 0)
//        {
//            SetFsmState(pxDataContainer -> m_uiFsmCommandState);
//            pxDataContainer -> m_uiFsmCommandState = 0;
//        }

        if (GetFsmCommandState() != 0)
        {
            SetFsmState(GetFsmCommandState());
            SetFsmCommandState(0);
        }
    }
    break;

    case WRITE_DATA_START:
        std::cout << "CMainProductionCycle::Fsm WRITE_DATA_START"  << std::endl;
//        GetArgumentData();
        if (Write())
        {
            SetFsmState(DATA_WRITED_SUCCESSFULLY);
        }
        else
        {
            SetFsmState(WRITE_DATA_ERROR);
        }
        break;

    case DATA_WRITED_SUCCESSFULLY:
        std::cout << "CStorageDeviceFileSystem::Fsm DATA_WRITED_SUCCESSFULLY"  << std::endl;
        SetFsmAnswerState(DATA_WRITED_SUCCESSFULLY);
        SetFsmState(READY);
        break;

    case WRITE_DATA_ERROR:
        std::cout << "CMainProductionCycle::Fsm WRITE_DATA_ERROR"  << std::endl;
        SetFsmAnswerState(WRITE_DATA_ERROR);
        SetFsmState(READY);
        break;

    case READ_DATA_START:
        std::cout << "CMainProductionCycle::Fsm READ_DATA_START"  << std::endl;
//        GetArgumentData();
        if (Read())
        {
            SetFsmState(DATA_READED_SUCCESSFULLY);
        }
        else
        {
            SetFsmState(READ_DATA_ERROR);
        }
        break;

    case DATA_READED_SUCCESSFULLY:
        std::cout << "CMainProductionCycle::Fsm DATA_READED_SUCCESSFULLY"  << std::endl;
        SetFsmAnswerState(DATA_READED_SUCCESSFULLY);
        SetFsmState(READY);
        break;

    case READ_DATA_ERROR:
        std::cout << "CMainProductionCycle::Fsm READ_DATA_ERROR"  << std::endl;
        SetFsmAnswerState(READ_DATA_ERROR);
        SetFsmState(READY);
        break;

    default:
        break;
    }

    return GetFsmState();
}

//-------------------------------------------------------------------------------
