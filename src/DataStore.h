//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CDATASTORE_H
#define CDATASTORE_H

#include <stdint.h>
#include <iostream>
#include <fstream>


class CTimer;
class CTask;
class CResources;
class CStorageDeviceInterface;
class CStorageDeviceFileSystem;
class CDataContainerInterface;
class CDataContainerDataBase;
class CMessageBoxInterface;
class CMessageBoxGeneral;

using namespace std;

//-------------------------------------------------------------------------------
// ��������� ������ ������.
// ��������� ����������� ������ ���������.
// ��������������� ����������� ������ � ������� ��������� ��������.
// ��������������� ������ ������������� ����� ��� ���� ������� � �.�.
// ������ ���������:
// 1 - ���� ��������� ������.
// 2 - ����������� ���� ��� ������ ��������� ��������� ������.
// 3 - ��������� ����� ������������� �����.
// 4 - ������������ � ��������������� �������������� ��������� �������.
class CDataStore : public CTask
{
public:
    enum
    {
        CRC_LENGTH = 2,
        TAIL_LENGTH = 2,
        MAX_BLOCK_LENGTH = 256,
        MAX_ENCODED_BLOCK_LENGTH =
            ((MAX_BLOCK_LENGTH + TAIL_LENGTH) + ((MAX_BLOCK_LENGTH + TAIL_LENGTH) / 2)),
        MAX_BLOCKS_NUMBER = 100,
    };

    enum
    {
        READY_TO_WRITE_WAITING_TIMEOUT = 200,
        WRITE_END_WAITING_TIMEOUT = 10000,
    };

    enum
    {
        START_WRITE_BLOCK_DATA = NEXT_STEP,
        READY_TO_WRITE_WAITING_BLOCK_DATA,
        WRITE_END_WAITING_BLOCK_DATA,

        START_WRITE_TEMPORARY_BLOCK_DATA,
        READY_TO_WRITE_WAITING_TEMPORARY_BLOCK_DATA,
        WRITE_END_WAITING_TEMPORARY_BLOCK_DATA,

        START_WRITE_TEMPORARY_SERVICE_SECTION_DATA,
        READY_TO_WRITE_WAITING_TEMPORARY_SERVICE_SECTION_DATA,
        WRITE_END_WAITING_TEMPORARY_SERVICE_SECTION_DATA,

        START_WRITE_SERVICE_SECTION_DATA,
        READY_TO_WRITE_WAITING_SERVICE_SECTION_DATA,
        WRITE_END_WAITING_SERVICE_SECTION_DATA,

        DATA_WRITED_SUCCESSFULLY,
        WRITE_ERROR,
    };

    struct TBlockPositionData
    {
        // �������� �� ������ �����.
        uint16_t uiOffset;
        // ������ ��������� �����.
        uint16_t uiLength;
        // ������ ��������������� ��������� �����.
        uint16_t uiEncodedLength;
        // Crc ����� �������� �������� �� ���� � ��������� �����.
        // �� ��� ���������� ������������ ����������� ����� � ��� �������������� � ���������.
        uint16_t uiCrc;
    };

    struct TServiseSectionData
    {
        // �������� �� ��������� ����� ��� ������ ������ �����.
        uint16_t uiFreeSpaceOffset;
        // ������ ���������� �����.
        uint16_t uiLength;
        // ������ �������������� ���������� �����.
        // ������������ ����������������������� ��� ��������(8,4).
        // ����������� - 1.5: ���� ���� ������������� � ������� ����� 12 ���,
        // �� ���� ���� �������� ������ ���������� ��� ����� ������������.
        uint16_t uiEncodedLength;
        uint16_t uiStoredBlocksNumber;
        // ����������� ����� ����������� �� ������� ����������� ���� ������, �� ������� ���������.
        // ����������� ��� ������ � ����������� ������� ����� ������ ����� ������������.
        // ���� ��� � ������������ ��������������� � ���, ��� ���� ������ ������� �� ��������,
        // � �� ������������ �������������. � ���� ������ ������ ��������� � ����� ������������ �� ������,
        // ������ ������������ ��� ������ ���� ������.
        uint16_t uiCrcOfBlocksCrc;
        TBlockPositionData axBlockPositionData[MAX_BLOCKS_NUMBER];
    };

    struct TServiseSection
    {
        TServiseSectionData xServiseSectionData;
        // Crc ���������� �����.
        uint16_t uiCrc;
    };

    enum
    {
        // ������� ���� ����� ���� ���� ��� ���� �������.
        TEMPORARY_BLOCK_DATA_BEGIN = 8,
        TEMPORARY_SERVICE_SECTION_DATA_BEGIN =
            (TEMPORARY_BLOCK_DATA_BEGIN + MAX_ENCODED_BLOCK_LENGTH),
        SERVICE_SECTION_DATA_BEGIN =
            TEMPORARY_SERVICE_SECTION_DATA_BEGIN +
            (sizeof(struct TServiseSection) + (sizeof(struct TServiseSection) / 2)),
        MAX_SERVICE_SECTION_DATA_LENGTH =
            (sizeof(struct TServiseSection) + (sizeof(struct TServiseSection) / 2)),
        TEMPORARY_BLOCK_INDEX = MAX_BLOCKS_NUMBER,
        TEMPORARY_SERVICE_SECTION_INDEX = MAX_BLOCKS_NUMBER + 1,
        SERVICE_SECTION_INDEX = MAX_BLOCKS_NUMBER + 2,
        SERVICE_SECTION_BLOCK_NUMBER = 3,
    };

    CDataStore();
    virtual ~CDataStore();

    uint8_t Init(void);
    size_t GetObjectLength(void);

    void SetStorageDeviceName(std::string sName);
    void SetStorageDevice(CStorageDeviceInterface* pxStorageDevice);

    uint8_t WriteBlock(uint8_t *puiSource, uint16_t uiLength, uint8_t uiBlock);
    bool WriteBlock(CDataContainerDataBase* pxDataContainer);
    uint16_t ReadBlock(uint8_t *, uint8_t );
    void CrcOfBlocksCrcCreate(void);
    bool CrcOfBlocksCrcCheck(void);
    uint8_t Fsm(void);

    uint8_t* GetIntermediateBuff(void)
    {
        return m_puiIntermediateBuff;
    };

    uint8_t GetBlockLength(uint8_t uiBlock);

    uint16_t GetStoredBlocksNumber(void)
    {
        return m_xServiseSection.xServiseSectionData.uiStoredBlocksNumber;
    };

    void SetBlockIndex(uint8_t uiBlockIndex);

//protected:
    void CreateServiceSection(void);
    uint8_t TemporaryServiceSectionWritePrepare(void);
    uint8_t ServiceSectionWritePrepare(void);
    uint8_t ReadTemporaryServiceSection(void);
    uint8_t ReadServiceSection(void);
    uint8_t CheckTemporaryBlock(void);
    uint8_t CheckBlock(void);
    uint8_t TemporaryBlockWritePrepare(void);
    uint8_t BlockWritePrepare(void);

protected:
    uint8_t m_uiRequestRetryCounter;

    std::string m_sStorageDeviceName;
    // ��������� �� ������ ������ ���������� ��������.
    CStorageDeviceInterface* m_pxStorageDevice;
    // ��������� ������ ������� ��������.
    TServiseSection m_xServiseSection;
    // ������ ����������� ���� ������.
    uint16_t m_auiBlocksCurrentCrc[MAX_BLOCKS_NUMBER];
    // ��������������� �����.
    uint8_t* m_puiIntermediateBuff;

    CDataContainerDataBase* m_pxCommandDataContainer;
    CDataContainerDataBase* m_pxOperatingDataContainer;
};

//-------------------------------------------------------------------------------
#endif // CDATASTORE_H
