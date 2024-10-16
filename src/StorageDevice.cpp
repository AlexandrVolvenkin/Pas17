
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
#include "DataStore.h"
#include "StorageDevice.h"

//-------------------------------------------------------------------------------
CStorageDeviceFileSystem::CStorageDeviceFileSystem()
{
    SetFsmState(IDDLE);
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
// Передаёт данные контекста записи блока автомату устройства хранения и запускает процесс записи.
bool CStorageDeviceFileSystem::WriteBlock(uint8_t *puiSource, uint16_t uiOffset, uint16_t uiLength)
{
    if ((GetFsmState() == IDDLE) ||
            (GetFsmState() == WRITE_DATA_ERROR) ||
            (GetFsmState() == READ_DATA_ERROR))
    {
        m_uiOffset = uiOffset;
        m_puiBuffer = puiSource;
        m_uiLength = uiLength;
        SetFsmState(WRITE_DATA_START);
        return true;
    }
    else
    {
        return false;
    }
}

//-------------------------------------------------------------------------------
// Записывает блок данных в устройство хранения.
uint8_t CStorageDeviceFileSystem::Write(void)
{
    uint16_t uiOffset = m_uiOffset;
    uint8_t *puiSource = m_puiBuffer;
    uint16_t uiLength = m_uiLength;

//    cout << "CStorageDeviceFileSystem::Write uiOffset" << " " << (int)uiOffset << endl;
//    cout << "CStorageDeviceFileSystem::Write uiLength" << " " << (int)uiLength << endl;
    if ((uiOffset + uiLength) < MAX_BUFFER_LENGTH)
    {
        ofstream outdata;
        // Чтобы добавить и не стереть старые данные откроем файл на чтение и запись.
        outdata.open(pccFileName, (ios::binary | ios::in | ios::out));
        // Файл не существует?
        if (!outdata)
        {
            cerr << "CStorageDeviceFileSystem::Write Error: file could not be opened" << endl;
            // чтобы создать файл откроем только на запись.
            outdata.open(pccFileName, (ios::binary | ios::out));
            // Файл не создан?
            if (!outdata)
            {
                cerr << "CStorageDeviceFileSystem::Write Error: file could not be created" << endl;
                return 0;
            }
        }

        outdata.seekp(uiOffset, ios_base::beg);
        outdata.write((char*)puiSource, uiLength);

        // закроем файл.
        outdata.close();

//        SetIsDataWrited(true);
        return 1;
    }
    else
    {
        return 0;
    }
}

//-------------------------------------------------------------------------------
// Передаёт данные контекста записи блока автомату устройства хранения и запускает процесс записи.
bool CStorageDeviceFileSystem::ReadBlock(uint8_t *puiDestination, uint16_t uiOffset, uint16_t uiLength)
{
    if ((GetFsmState() == IDDLE) ||
            (GetFsmState() == WRITE_DATA_ERROR) ||
            (GetFsmState() == READ_DATA_ERROR))
    {
        m_uiOffset = uiOffset;
        m_puiBuffer = puiDestination;
        m_uiLength = uiLength;
        SetFsmState(READ_DATA_START);
        return true;
    }
    else
    {
        return false;
    }
}

//-------------------------------------------------------------------------------
// Считывает блок данных из устройства хранения.
uint8_t CStorageDeviceFileSystem::Read(void)
{
    uint16_t uiOffset = m_uiOffset;
    uint8_t *puiDestination = m_puiBuffer;
    uint16_t uiLength = m_uiLength;
//    cout << "CStorageDeviceFileSystem::Read uiOffset" << " " << (int)uiOffset << endl;
//    cout << "CStorageDeviceFileSystem::Read uiLength" << " " << (int)uiLength << endl;
    if ((uiOffset + uiLength) < MAX_BUFFER_LENGTH)
    {
        ifstream indata;
        // откроем файл.
        indata.open(pccFileName, (ios::in | ios::binary));
        if (!indata)
        {
            cerr << "CStorageDeviceFileSystem::Read Error: file could not be opened" << endl;
            return 0;
        }
        else
        {
            // установим смещение в файле.
            indata.seekg(uiOffset, ios_base::beg);
            // прочитаем файл.
            indata.read(reinterpret_cast<char*>(puiDestination),
                        uiLength);
        }
        // закроем файл.
        indata.close();
        return 1;
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------
uint8_t CStorageDeviceFileSystem::Fsm(void)
{
//    std::cout << "CStorageDeviceFileSystem::Fsm 1" << endl;
    switch (GetFsmState())
    {
    case START:
        std::cout << "CStorageDeviceFileSystem::Fsm START"  << std::endl;
//        SetFsmState(MODBUS_SLAVE_LINK_LAYER);
        break;

    case READY:
        //std::cout << "CStorageDeviceFileSystem::Fsm READY"  << std::endl;
        break;

    case IDDLE:
        //std::cout << "CStorageDeviceFileSystem::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CStorageDeviceFileSystem::Fsm STOP"  << std::endl;[[[]=
//        SetFsmState(START);
        break;

    case WRITE_DATA_START:
        //std::cout << "CMainProductionCycle::Fsm WRITE_DATA_START"  << std::endl;
        Write();
        SetFsmState(DATA_WRITED_SUCCESSFULLY);
        break;

    case DATA_WRITED_SUCCESSFULLY:
        std::cout << "CStorageDeviceFileSystem::Fsm DATA_WRITED_SUCCESSFULLY"  << std::endl;
        break;

    case WRITE_DATA_ERROR:
        //std::cout << "CMainProductionCycle::Fsm WRITE_DATA_ERROR"  << std::endl;
        break;

    case READ_DATA_START:
        //std::cout << "CMainProductionCycle::Fsm READ_DATA_START"  << std::endl;
        Read();
        SetFsmState(DATA_READED_SUCCESSFULLY);
        break;

    case DATA_READED_SUCCESSFULLY:
        //std::cout << "CMainProductionCycle::Fsm DATA_READED_SUCCESSFULLY"  << std::endl;
        break;

    case READ_DATA_ERROR:
        //std::cout << "CMainProductionCycle::Fsm READ_DATA_ERROR"  << std::endl;
        break;

    default:
        break;
    }

    return GetFsmState();
}

//-------------------------------------------------------------------------------
