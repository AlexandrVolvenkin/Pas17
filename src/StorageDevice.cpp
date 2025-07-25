
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
#include "MessageBox.h"
#include "StorageDevice.h"

//-------------------------------------------------------------------------------
CStorageDeviceFileSystem::CStorageDeviceFileSystem()
{
    std::cout << "CStorageDeviceFileSystem constructor"  << std::endl;
    // ������� ��� ������.
    sprintf(GetTaskNamePointer(),
            "%s",
            typeid(*this).name());
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CStorageDeviceFileSystem::~CStorageDeviceFileSystem()
{

}

//-------------------------------------------------------------------------------
// ������� ������ ��������� ������ ����� �������� ���������� �������� � ��������� ������� ������.
bool CStorageDeviceFileSystem::WriteBlock(CDataContainerDataBase* pxDataContainer)
{
    std::cout << "CStorageDeviceFileSystem WriteBlock 1"  << std::endl;
    if (m_pxCommandDataContainer == 0)
    {
        std::cout << "CStorageDeviceFileSystem WriteBlock 2"  << std::endl;
        m_pxCommandDataContainer = pxDataContainer;
        SetFsmCommandState(WRITE_DATA_START);
        return true;
    }
    else
    {
        std::cout << "CStorageDeviceFileSystem WriteBlock 3"  << std::endl;
        return false;
    }
}

//-------------------------------------------------------------------------------
// ���������� ���� ������ � ���������� ��������.
uint8_t CStorageDeviceFileSystem::Write(void)
{
    std::cout << "CStorageDeviceFileSystem Write"  << std::endl;

    CDataContainerDataBase* pxDataContainer = m_pxOperatingDataContainer;

    uint8_t *puiDataPointer = pxDataContainer -> m_puiDataPointer;
    uint16_t uiOffset = pxDataContainer -> m_uiDataOffset;
    uint16_t uiLength = pxDataContainer -> m_uiDataLength;

    cout << "CStorageDeviceFileSystem::Write puiDataPointer" << " " << (int)puiDataPointer << endl;
    cout << "CStorageDeviceFileSystem::Write uiOffset" << " " << (int)uiOffset << endl;
    cout << "CStorageDeviceFileSystem::Write uiLength" << " " << (int)uiLength << endl;

    if ((uiOffset + uiLength) < MAX_BUFFER_LENGTH)
    {
        std::cout << "CStorageDeviceFileSystem Write 1"  << std::endl;
        ofstream outdata;
        // ����� �������� � �� ������� ������ ������ ������� ���� �� ������ � ������.
        outdata.open(pccFileName, (ios::binary | ios::in | ios::out));
        // ���� �� ����������?
        if (!outdata)
        {
            std::cout << "CStorageDeviceFileSystem Write 2"  << std::endl;
            cerr << "CStorageDeviceFileSystem::Write Error: file could not be opened" << endl;
            // ����� ������� ���� ������� ������ �� ������.
            outdata.open(pccFileName, (ios::binary | ios::out));
            // ���� �� ������?
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
        // ������� ����.
        outdata.close();

        std::cout << "CStorageDeviceFileSystem Write 6"  << std::endl;
        return 1;
    }
    else
    {
        std::cout << "CStorageDeviceFileSystem Write 7"  << std::endl;
        return 0;
    }
}

//-------------------------------------------------------------------------------
bool CStorageDeviceFileSystem::ReadBlock(CDataContainerDataBase* pxDataContainer)
{
    std::cout << "CStorageDeviceFileSystem ReadBlock 1"  << std::endl;
    if (m_pxCommandDataContainer == 0)
    {
        std::cout << "CStorageDeviceFileSystem ReadBlock 2"  << std::endl;
        m_pxCommandDataContainer = pxDataContainer;
//        SetFsmCommandState(READ_DATA_START);
        m_pxOperatingDataContainer = m_pxCommandDataContainer;
        m_pxCommandDataContainer = 0;
        return Read();
//        return true;
    }
    else
    {
        return false;
    }
}

//-------------------------------------------------------------------------------
// ��������� ���� ������ �� ���������� ��������.
uint8_t CStorageDeviceFileSystem::Read(void)
{
    std::cout << "CStorageDeviceFileSystem Read 1"  << std::endl;

    CDataContainerDataBase* pxDataContainer = m_pxOperatingDataContainer;

    uint8_t *puiDataPointer = pxDataContainer -> m_puiDataPointer;
    uint16_t uiOffset = pxDataContainer -> m_uiDataOffset;
    uint16_t uiLength = pxDataContainer -> m_uiDataLength;

//    cout << "CStorageDeviceFileSystem::Read uiOffset" << " " << (int)uiOffset << endl;
//    cout << "CStorageDeviceFileSystem::Read uiLength" << " " << (int)uiLength << endl;
    if ((uiOffset + uiLength) < MAX_BUFFER_LENGTH)
    {
        std::cout << "CStorageDeviceFileSystem Read 2"  << std::endl;
        ifstream indata;
        // ������� ����.
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
            // ��������� �������� � �����.
            indata.seekg(uiOffset, ios_base::beg);
            // ��������� ����.
            indata.read(reinterpret_cast<char*>(puiDataPointer),
                        uiLength);
        }
        // ������� ����.
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
        SetFsmState(INIT);
        break;

    case INIT:
        //std::cout << "CStorageDeviceFileSystem::Fsm INIT"  << std::endl;
        m_pxCommandDataContainer = 0;
        m_pxOperatingDataContainer = 0;
        SetFsmCommandState(0);
        SetFsmState(READY);
        break;

    case READY:
        //std::cout << "CStorageDeviceFileSystem::Fsm READY"  << std::endl;
    {
        if (m_pxCommandDataContainer != 0)
        {
            std::cout << "CStorageDeviceFileSystem::Fsm READY 2"  << std::endl;
            m_pxOperatingDataContainer = m_pxCommandDataContainer;
            SetFsmState(GetFsmCommandState());
            SetFsmCommandState(0);
            m_pxCommandDataContainer = 0;
        }
    }
    break;

    case DONE_OK:
        //std::cout << "CStorageDeviceFileSystem::Fsm DONE_OK"  << std::endl;
        SetFsmOperationStatus(DONE_OK);
        SetFsmState(READY);
        break;

    case DONE_ERROR:
        //std::cout << "CStorageDeviceFileSystem::Fsm DONE_ERROR"  << std::endl;
        SetFsmOperationStatus(DONE_ERROR);
        SetFsmState(READY);
        break;

    case WRITE_DATA_START:
        std::cout << "CStorageDeviceFileSystem::Fsm WRITE_DATA_START"  << std::endl;
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
        SetFsmOperationStatus(DONE_OK);
        SetFsmState(READY);
        break;

    case WRITE_DATA_ERROR:
        std::cout << "CMainProductionCycle::Fsm WRITE_DATA_ERROR"  << std::endl;
        SetFsmOperationStatus(DONE_ERROR);
        SetFsmState(READY);
        break;

    case READ_DATA_START:
        std::cout << "CMainProductionCycle::Fsm READ_DATA_START"  << std::endl;
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
        SetFsmOperationStatus(DONE_OK);
        SetFsmState(READY);
        break;

    case READ_DATA_ERROR:
        std::cout << "CMainProductionCycle::Fsm READ_DATA_ERROR"  << std::endl;
        SetFsmOperationStatus(DONE_ERROR);
        SetFsmState(READY);
        break;

    default:
        break;
    }

    return GetFsmState();
}

//-------------------------------------------------------------------------------










//-------------------------------------------------------------------------------
CStorageDeviceSpiFram::CStorageDeviceSpiFram()
{
    std::cout << "CStorageDeviceSpiFram constructor"  << std::endl;
//    // ������� ��� ������.
//    sprintf(GetTaskNamePointer(),
//            "%s",
//            typeid(*this).name());
//    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CStorageDeviceSpiFram::~CStorageDeviceSpiFram()
{

}

//-------------------------------------------------------------------------------
// ���������� ���� ������ � ���������� ��������.
uint8_t CStorageDeviceSpiFram::Write(uint16_t uiOffset,
                                     uint8_t* puiDataPointer,
                                     uint16_t uiLength)
{
//    std::cout << "CStorageDeviceSpiFram Write"  << std::endl;

    if ((uiOffset + uiLength) < MAX_BUFFER_LENGTH)
    {
        // ��� ���������� fram ������.
        const std::string sFileName = "/dev/mtd0";

        std::ofstream FramOutputStream(sFileName, std::ios::binary | std::ios::in | std::ios::out);
        if (!FramOutputStream.is_open())
        {
            std::cerr << "Failed to open for write /dev/mtd0" << std::endl;
            return 0;
        }

        // ���������� ������ � ���� fram
        // ��������� ��������� �� ������ ����� ������������ ������.
        FramOutputStream.seekp(uiOffset, std::ios::beg);
        FramOutputStream.write(reinterpret_cast<char*>(puiDataPointer), uiLength);
        // ��������� ����
        FramOutputStream.close();

        return 1;
    }
    else
    {
        std::cout << "CStorageDeviceSpiFram Write 7"  << std::endl;
        return 0;
    }
}

//-------------------------------------------------------------------------------
// ��������� ���� ������ �� ���������� ��������.
uint8_t CStorageDeviceSpiFram::Read(uint8_t* puiDataPointer,
                                    uint16_t uiOffset,
                                    uint16_t uiLength)
{
//    std::cout << "CStorageDeviceSpiFram Read 1"  << std::endl;

//    cout << "CStorageDeviceSpiFram::Read uiOffset" << " " << (int)uiOffset << endl;
//    cout << "CStorageDeviceSpiFram::Read uiLength" << " " << (int)uiLength << endl;
    if ((uiOffset + uiLength) < MAX_BUFFER_LENGTH)
    {
        // ��� ���������� fram ������.
        const std::string sFileName = "/dev/mtd0";

        std::ifstream FramInputStream(sFileName, std::ios::binary | std::ios::in | std::ios::out);
        if (!FramInputStream.is_open())
        {
            std::cerr << "Failed to open for read /dev/mtd0" << std::endl;
            return 0;
        }

        // ��������� ��������� �� ������.
        FramInputStream.seekg(uiOffset, std::ios::beg);
        FramInputStream.read(reinterpret_cast<char*>(puiDataPointer), uiLength);
        FramInputStream.close();

        return 1;
    }
    else
    {
        std::cout << "CStorageDeviceSpiFram Read 5"  << std::endl;
        return 0;
    }
}

//-------------------------------------------------------------------------------
